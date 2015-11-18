#include "G4CaloTowerAnalysis.h"

#include <boost/foreach.hpp>
#include <map>
#include <sstream>

/* PHG4 includes */
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4Particle.h>

#include <g4cemc/RawTowerv1.h>
#include <g4cemc/RawTowerContainer.h>

#include <g4cemc/RawTowerGeomv2.h>
#include <g4cemc/RawTowerGeomContainerv1.h>

/* Fun4All includes */
#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/getClass.h>

/* ROOT includes */
#include <TFile.h>
#include <TNtuple.h>
#include <TMath.h>

using namespace std;

G4CaloTowerAnalysis::G4CaloTowerAnalysis(const std::string name , const std::string filename ):
  SubsysReco( name ),
  _truth_info_container(NULL),
  _tower(NULL),
  _towergeom(NULL),
  _filename(filename),
  _node_name_truth("G4TruthInfo"),
  _nevent(0),
  _t_tower(NULL)
{

}

G4CaloTowerAnalysis::~G4CaloTowerAnalysis()
{
}

int G4CaloTowerAnalysis::Init( PHCompositeNode* topNode )
{

  /* Create new output file */
  _outfile = new TFile(_filename.c_str(), "RECREATE");

  _t_tower = new TNtuple("t_tower","tower information",
			 "event:caloID:towerID:idx1:idx2:x:y:z:eta:phi:r:"
			 "dx:dy:dz:e:"
			 "gparticleID:gflavor:"
			 "geta:gphi:ge:gpt");

  return 0;
}

int G4CaloTowerAnalysis::process_event( PHCompositeNode* topNode )
{
  /* Increment event counter */
  _nevent++;

  /* Get the Geant4 Truth particle information container */
  _truth_info_container = findNode::getClass<PHG4TruthInfoContainer>(topNode,_node_name_truth.c_str());
  if(!_truth_info_container)
    {
      cout << PHWHERE << " WARNING: Can't find PHG4TruthInfoContainer." << endl;
      return -1;//ABORTEVENT;
    }

  /* Get truth info */
  PHG4TruthInfoContainer::Map map = _truth_info_container->GetPrimaryMap();

  // select first primary particle in map- assume single-particle event (particle gun)
  PHG4Particle* primary = map.begin()->second;

  float gpid = primary->get_track_id();
  float gflavor = primary->get_pid();

  float gpx = primary->get_px();
  float gpy = primary->get_py();
  float gpz = primary->get_pz();
  float ge = primary->get_e();

  float gpt = sqrt(gpx*gpx+gpy*gpy);
  float geta = NAN;
  if (gpt != 0.0) geta = asinh(gpz/gpt);
  float gphi = atan2(gpy,gpx);


  /* Get Tower from input nodes */
  unsigned nnodes = _node_name_tower.size();
  unsigned ngeomnodes = _node_name_tower_geom.size();

  if ( !nnodes )
    {
      cout << PHWHERE << " WARNING: No tower input node defined." << endl;
      return -1;//ABORTEVENT;
    }

  if ( !ngeomnodes )
    {
      cout << PHWHERE << " WARNING: No tower geometry node defined." << endl;
      return -1;//ABORTEVENT;
    }

  if ( nnodes != ngeomnodes )
    {
      cout << PHWHERE << " WARNING: Number of tower and tower geometry nodes do not match." << endl;
      return -1;//ABORTEVENT;
    }

  /* Loop over all input nodes for tower */
  for (unsigned i = 0; i < nnodes; i++)
    {
      RawTowerContainer *_tower = findNode::getClass<RawTowerContainer>(topNode, _node_name_tower.at(i).c_str());
      RawTowerGeomContainer *_towergeom = findNode::getClass<RawTowerGeomContainer>(topNode, _node_name_tower_geom.at(i).c_str());

      if (_tower)
	{

	  /* loop over all towers in the event from this container */
	  RawTowerContainer::ConstIterator towerit;
	  RawTowerContainer::ConstRange towers_begin_end = _tower->getTowers();

	  RawTowerv1* tower_i = NULL;

	  for (towerit = towers_begin_end.first; towerit != towers_begin_end.second; towerit++)
	    {
	      /* Get raw tower and energy */
	      tower_i= dynamic_cast<RawTowerv1*>( (*towerit).second );

	      RawTowerDefs::keytype towerid = tower_i->get_id();
	      float e = tower_i->get_energy();

	      float calo_id = RawTowerDefs::decode_caloid( towerid );
	      float tower_idx1 = RawTowerDefs::decode_index1( towerid );
	      float tower_idx2 = RawTowerDefs::decode_index2( towerid );

	      float x = _towergeom->get_tower_geometry( towerid )->get_center_x();
	      float y = 0;
	      float z = 0;
	      float dx = 0;
	      float dy = 0;
	      float dz = 0;
	      float eta = 0;
	      float phi = 0;
	      float r = 0;

	      /* Fill output tree */
	      float tower_data[21] = {_nevent,
				      calo_id,
				      towerid,
				      tower_idx1,
				      tower_idx2,
				      x,
				      y,
				      z,
				      eta,
				      phi,
				      r,
				      dx,
				      dy,
				      dz,
				      e,
				      gpid,
				      gflavor,
				      geta,
				      gphi,
				      ge,
				      gpt
	      };

	      _t_tower->Fill(tower_data);


	      /* Print tower and neighbor information */
	      if ( _nevent == 1 )
		{
		  cout << "*** Event #" << _nevent << " Tower ("
		       << RawTowerDefs::convert_caloid_to_name( RawTowerDefs::decode_caloid( towerid ) ) << " , "
		       << RawTowerDefs::decode_index1( towerid ) << " , "
		       << RawTowerDefs::decode_index2( towerid ) << ") : "
		       << tower_i->get_energy() <<  endl;

		  cout << "** Position:   x = " << x << " ,  y = " << y << " ,  z = " << z << endl;
		  cout << "** Dimension: dx = " << dx << " , dy = " << dy << " , dz = " << dz << endl;

		  /* print neighbor information */
		  //vector< unsigned int > v_tower_neighbors = geoman->GetNeighbors( towerid );
		  //
		  //for ( unsigned i = 0; i < v_tower_neighbors.size(); i++ )
		  //  {
		  //    /* energy of neighbo tower */
		  //    float e_neighbor = 0;
		  //
		  //    /* Check if neighbor tower has energy deposit */
		  //    if ( (_tower->getTower( v_tower_neighbors.at( i ) )) )
		  //	e_neighbor = (_tower->getTower( v_tower_neighbors.at( i ) ))->get_energy();
		  //
		  //    cout << "* Adjacent Tower ("
		  //	   << calotowerid::DecodeCalorimeterName( v_tower_neighbors.at( i ) ) << " , "
		  //	   << calotowerid::DecodeTowerIndex1( v_tower_neighbors.at( i ) ) << " , "
		  //	   << calotowerid::DecodeTowerIndex2( v_tower_neighbors.at( i ) ) << ") : "
		  //	   << e_neighbor << endl;
		  //  }
		}
	    }
	}
    }

  return 0;
}

int G4CaloTowerAnalysis::End(PHCompositeNode * topNode)
{

  /* Select output file */
  _outfile->cd();

  /* Write tree to output file */
  _t_tower->Write();

  /* Write & Close output file */
  _outfile->Write();
  _outfile->Close();

  return 0;
}
