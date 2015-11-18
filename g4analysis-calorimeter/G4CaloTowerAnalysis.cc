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
  _node_name_tower("NONE"),
  _node_name_tower_geom("NONE"),
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

int G4CaloTowerAnalysis::InitRun( PHCompositeNode* topNode )
{
  /* Get the Geant4 Truth particle information container */
  _truth_info_container = findNode::getClass<PHG4TruthInfoContainer>(topNode,_node_name_truth.c_str());
  if(!_truth_info_container)
    {
      cout << PHWHERE << " WARNING: Can't find PHG4TruthInfoContainer." << endl;
      return -1;//ABORTEVENT;
    }

  /* Get Tower geometry from input node */
  _towergeom = findNode::getClass<RawTowerGeomContainer>(topNode, _node_name_tower_geom.c_str());
  if(!_towergeom)
    {
      cout << PHWHERE << " WARNING: Can't find RawTowerGeomContainer " << _node_name_tower_geom << endl;
      return -1;//ABORTEVENT;
    }

  /* Get Tower from input node */
  _tower = findNode::getClass<RawTowerContainer>(topNode, _node_name_tower.c_str());
  if(!_tower)
    {
      cout << PHWHERE << " WARNING: Can't find RawTowerContainer " << _node_name_tower << endl;
      return -1;//ABORTEVENT;
    }

  return 0;
}

int G4CaloTowerAnalysis::process_event( PHCompositeNode* topNode )
{
  /* Increment event counter */
  _nevent++;

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

  float gp = sqrt(gpx*gpx+gpy*gpy+gpz*gpz);
  float gpt = sqrt(gpx*gpx+gpy*gpy);

  float geta = atanh( gpz / gp );
  float gphi = atan2(gpy,gpx);

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
      float y = _towergeom->get_tower_geometry( towerid )->get_center_y();
      float z = _towergeom->get_tower_geometry( towerid )->get_center_z();
      float dx = _towergeom->get_tower_geometry( towerid )->get_size_x();
      float dy = _towergeom->get_tower_geometry( towerid )->get_size_y();
      float dz = _towergeom->get_tower_geometry( towerid )->get_size_z();
      float eta = _towergeom->get_tower_geometry( towerid )->get_eta();
      float phi = _towergeom->get_tower_geometry( towerid )->get_phi();
      float r = _towergeom->get_tower_geometry( towerid )->get_center_radius();

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
      if ( _nevent == 1 && verbosity > 2 )
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
