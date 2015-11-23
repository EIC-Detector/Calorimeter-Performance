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
#include <TTree.h>
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
  _t_tower(NULL),
  _t_event(NULL)
{

}

G4CaloTowerAnalysis::~G4CaloTowerAnalysis()
{
}

int G4CaloTowerAnalysis::Init( PHCompositeNode* topNode )
{

  /* Create new output file */
  _outfile = new TFile(_filename.c_str(), "RECREATE");

  /* Create tree _t_tower */
  _t_tower = new TTree("t_tower", "Tower information");
  _t_tower->Branch( "event", &_event, "event/I" );
  _t_tower->Branch( "ntower", &_ntower, "ntower/I" );
  _t_tower->Branch( "towerID", &_towerID, "towerID[ntower]/I" );
  _t_tower->Branch( "index1", &_idx1, "index1[ntower]/I" );
  _t_tower->Branch( "index2", &_idx2, "index2[ntower]/I" );
  _t_tower->Branch( "x", &_x, "x[ntower]/F" );
  _t_tower->Branch( "y", &_y, "y[ntower]/F" );
  _t_tower->Branch( "z", &_e, "z[ntower]/F" );
  _t_tower->Branch( "eta", &_eta, "eta[ntower]/F" );
  _t_tower->Branch( "phi", &_phi, "phi[ntower]/F" );
  _t_tower->Branch( "r", &_r, "r[ntower]/F" );
  _t_tower->Branch( "volume", &_volume, "volume[ntower]/F" );
  _t_tower->Branch( "e", &_e, "e[ntower]/F" );

  /* Create tree _t_event */
  _t_event = new TTree("t_event", "Event information");
  _t_event->Branch( "event", &_event, "event/I" );
  _t_event->Branch( "e_tower_sum", &_e_tower_sum, "e_tower_sum/F" );
  _t_event->Branch( "nprimary", &_nprimary, "nprimary/I" );
  _t_event->Branch( "gparticleID", &_gparticleID, "gparticleID[nprimary]/I" );
  _t_event->Branch( "gflavor", &_gflavor, "gflavor[nprimary]/I" );
  _t_event->Branch( "gpx", &_gpx, "gpx[nprimary]/F" );
  _t_event->Branch( "gpy", &_gpy, "gpy[nprimary]/F" );
  _t_event->Branch( "gpz", &_gpz, "gpz[nprimary]/F" );
  _t_event->Branch( "geta", &_geta, "geta[nprimary]/F" );
  _t_event->Branch( "gphi", &_gphi, "gphi[nprimary]/F" );
  _t_event->Branch( "ge", &_ge, "ge[nprimary]/F" );
  _t_event->Branch( "gp", &_gp, "gp[nprimary]/F" );
  _t_event->Branch( "gpt", &_gpt, "gpt[nprimary]/F" );

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

  _nprimary = 1;

  _gparticleID[_nprimary-1] = primary->get_track_id();
  _gflavor[_nprimary-1] = primary->get_pid();

  _gpx[_nprimary-1] = primary->get_px();
  _gpy[_nprimary-1] = primary->get_py();
  _gpz[_nprimary-1] = primary->get_pz();
  _ge[_nprimary-1] = primary->get_e();

  _gp[_nprimary-1] = sqrt(_gpx[_nprimary-1] *_gpx[_nprimary-1] +_gpy[_nprimary-1] *_gpy[_nprimary-1] +_gpz[_nprimary-1] *_gpz[_nprimary-1] );
  _gpt[_nprimary-1] = sqrt(_gpx[_nprimary-1] *_gpx[_nprimary-1] +_gpy[_nprimary-1] *_gpy[_nprimary-1] );

  _geta[_nprimary-1] = atanh( _gpz[_nprimary-1] /_gp[_nprimary-1] );
  _gphi[_nprimary-1] = atan2( _gpy[_nprimary-1] , _gpx[_nprimary-1] );

  /* loop over all towers in the event from this container */
  RawTowerContainer::ConstIterator towerit;
  RawTowerContainer::ConstRange towers_begin_end = _tower->getTowers();

  RawTowerv1* tower_i = NULL;
  _ntower = 0;
  _e_tower_sum = 0;

  for (towerit = towers_begin_end.first; towerit != towers_begin_end.second; towerit++)
    {
      _ntower++;

      /* Get raw tower and energy */
      tower_i= dynamic_cast<RawTowerv1*>( (*towerit).second );

      RawTowerDefs::keytype towerid = tower_i->get_id();
      _towerID[_ntower-1] = towerid;
      _e[_ntower-1] = tower_i->get_energy();
      _e_tower_sum += _e[_ntower-1];

      _idx1[_ntower-1] = RawTowerDefs::decode_index1( towerid );
      _idx2[_ntower-1] = RawTowerDefs::decode_index2( towerid );

      _x[_ntower-1] = _towergeom->get_tower_geometry( towerid )->get_center_x();
      _y[_ntower-1] = _towergeom->get_tower_geometry( towerid )->get_center_y();
      _z[_ntower-1] = _towergeom->get_tower_geometry( towerid )->get_center_z();

      _volume[_ntower-1] = _towergeom->get_tower_geometry( towerid )->get_volume();

      _eta[_ntower-1] = _towergeom->get_tower_geometry( towerid )->get_eta();
      _phi[_ntower-1] = _towergeom->get_tower_geometry( towerid )->get_phi();
      _r[_ntower-1] = _towergeom->get_tower_geometry( towerid )->get_center_radius();


      /* Print tower information */
      if ( verbosity > 2 )
	{
	  cout << "*** Event #" << _nevent << " Tower ("
	       << RawTowerDefs::convert_caloid_to_name( RawTowerDefs::decode_caloid( towerid ) ) << " , "
	       << RawTowerDefs::decode_index1( towerid ) << " , "
	       << RawTowerDefs::decode_index2( towerid ) << ") : "
	       << tower_i->get_energy() <<  endl;
	}
    }

  /* Fill output tree */
  _t_tower->Fill();
  _t_event->Fill();

  return 0;
}

int G4CaloTowerAnalysis::End(PHCompositeNode * topNode)
{

  /* Select output file */
  _outfile->cd();

  /* Write tree to output file */
  _t_tower->Write();
  _t_event->Write();

  /* Write & Close output file */
  _outfile->Write();
  _outfile->Close();

  return 0;
}
