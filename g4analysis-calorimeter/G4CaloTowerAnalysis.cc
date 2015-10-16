#include "G4CaloTowerAnalysis.h"

#include <boost/foreach.hpp>
#include <map>
#include <sstream>

/* PHG4 includes */
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4Particle.h>

#include <g4cemc/CaloTowerID.h>
#include <g4cemc/CaloTowerGeomManager.h>

/* Fun4All includes */
#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllReturnCodes.h>

#include <fun4all/getClass.h>

/* ROOT includes */
#include <TFile.h>
#include <TNtuple.h>
#include <TMath.h>

using namespace std;

G4CaloTowerAnalysis::G4CaloTowerAnalysis(const std::string name , const std::string filename ):
  SubsysReco( name ),
  _truth_info_container(NULL),
  _tower(NULL),
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
  //  for (PHG4TruthInfoContainer::ConstIterator iter = map.begin(); iter != map.end();  ++iter)
  //    {

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
  //}


  /* Get Tower from input nodes */
  unsigned nnodes = _node_tower_names.size();

  if ( !nnodes )
    {
      cout << PHWHERE << " WARNING: No tower input node defined." << endl;
      return -1;//ABORTEVENT;
    }

  /* Loop over all input nodes for tower */
  for (unsigned i = 0; i < nnodes; i++)
    {
      CaloTowerContainer *_tower = findNode::getClass<CaloTowerContainer>(topNode, _node_tower_names.at(i).c_str());

      if (_tower)
	{

	  /* loop over all towers in the event from this container */
	  CaloTowerContainer::ConstIterator towerit;
	  CaloTowerContainer::ConstRange towers_begin_end = _tower->getTowers();

	  CaloTowerv1* tower_i = NULL;

	  for (towerit = towers_begin_end.first; towerit != towers_begin_end.second; towerit++)
	    {
	      /* Get raw tower and energy */
	      tower_i= dynamic_cast<CaloTowerv1*>( (*towerit).second );

	      unsigned int towerid = tower_i->get_id();
	      float e = tower_i->get_energy();

	      float calo_id = calotowerid::DecodeCalorimeterId( towerid );
	      float tower_idx1 = calotowerid::DecodeTowerIndex1( towerid );
	      float tower_idx2 = calotowerid::DecodeTowerIndex2( towerid );

	      /* Get geometry manager for calorimeter towers */
	      CaloTowerGeomManager *geoman = CaloTowerGeomManager::instance();

	      float x,y,z,dx,dy,dz,eta,phi,r;
	      geoman->GetPositionXYZ( towerid , x, y, z );
	      geoman->GetSizeXYZ( towerid , dx, dy, dz );
	      geoman->GetPositionEtaPhiR( towerid , eta, phi, r );


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
		       << calotowerid::DecodeCalorimeterName( towerid ) << " , "
		       << calotowerid::DecodeTowerIndex1( towerid ) << " , "
		       << calotowerid::DecodeTowerIndex2( towerid ) << ") : "
		       << tower_i->get_energy() <<  endl;

		  vector< unsigned int > v_tower_neighbors = geoman->GetNeighbors( towerid );

		  cout << "** Position:   x = " << x << " ,  y = " << y << " ,  z = " << z << endl;
		  cout << "** Dimension: dx = " << dx << " , dy = " << dy << " , dz = " << dz << endl;

		  for ( unsigned i = 0; i < v_tower_neighbors.size(); i++ )
		    {
		      /* energy of neighbo tower */
		      float e_neighbor = 0;

		      /* Check if neighbor tower has energy deposit */
		      if ( (_tower->getTower( v_tower_neighbors.at( i ) )) )
			e_neighbor = (_tower->getTower( v_tower_neighbors.at( i ) ))->get_energy();

		      cout << "* Adjacent Tower ("
			   << calotowerid::DecodeCalorimeterName( v_tower_neighbors.at( i ) ) << " , "
			   << calotowerid::DecodeTowerIndex1( v_tower_neighbors.at( i ) ) << " , "
			   << calotowerid::DecodeTowerIndex2( v_tower_neighbors.at( i ) ) << ") : "
			   << e_neighbor << endl;
		    }
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
