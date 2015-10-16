#include "G4CaloClusterAnalysis.h"

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
#include <TMath.h>
#include <TNtuple.h>

using namespace std;

G4CaloClusterAnalysis::G4CaloClusterAnalysis(const std::string name , const std::string filename ):
  SubsysReco( name ),
  _truth_info_container(NULL),
  _cluster(NULL),
  _filename(filename),
  _node_name_truth("G4TruthInfo"),
  _nevent(0),
  _t_cluster(NULL)
{

}

G4CaloClusterAnalysis::~G4CaloClusterAnalysis()
{
}

int G4CaloClusterAnalysis::Init( PHCompositeNode* topNode )
{

  /* Create new output file */
  _outfile = new TFile(_filename.c_str(), "RECREATE");

  _t_cluster = new TNtuple("t_cluster","cluster information",
			   "event:ncluster:clusterID:eta:phi:e:volume:"
			   "density:ntowers:"
			   "gparticleID:gflavor:"
			   "geta:gphi:ge:gpt");

  return 0;
}


int G4CaloClusterAnalysis::process_event( PHCompositeNode* topNode )
{

  /* Increment event counter */
  _nevent++;

  /* List of full event parameters */
  float event_esum = 0;

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

  /* Get Cluster from input nodes */
  unsigned nnodes = _node_cluster_names.size();

  if ( !nnodes )
    {
      cout << PHWHERE << " WARNING: No cluster input node defined." << endl;
      return -1;//ABORTEVENT;
    }

  /* Variables to store information of cluster with maximum energy in event */
  float e_max = 0;
  float eta_max = 0;
  float phi_max = 0;
  float volume_max = 0;
  float density_max = 0;
  float clusterID_max = 0;
  float ntowers_max = 0;

  float ncluster = 0;

  /* Loop over all input nodes for cluster and look for maximum energy cluster */
  for (unsigned i = 0; i < nnodes; i++)
    {
      CaloClusterContainer *_cluster = findNode::getClass<CaloClusterContainer>(topNode, _node_cluster_names.at(i).c_str());

      if (_cluster)
	{

	  /* loop over all clusters in the event from this container */
	  CaloClusterContainer::ConstIterator clusterit;
	  CaloClusterContainer::ConstRange clusters_begin_end = _cluster->getClusters();

	  CaloClusterv1* cluster_i = NULL;

	  for (clusterit = clusters_begin_end.first; clusterit != clusters_begin_end.second; clusterit++)
	    {
	      ncluster++;

	      /* Get raw cluster and energy */
	      cluster_i= dynamic_cast<CaloClusterv1*>( (*clusterit).second );
	      double energy = cluster_i->get_energy();

	      event_esum += energy;

	      /* check if this is maximum energy cluster */
	      if ( energy > e_max )
		{
		  e_max = energy;
		  eta_max = cluster_i->get_eta();
		  phi_max = cluster_i->get_phi();
		  volume_max = cluster_i->get_volume();
		  density_max = e_max / volume_max;
		  clusterID_max = cluster_i->get_id();
		  ntowers_max = cluster_i->getNTowers();
		}


	      /* Print cluster and neighbor information */
	      if ( _nevent == 1 )
		{
		  unsigned int clusterid = cluster_i->get_id();
		  cout << "*** Event #" << _nevent << " Cluster (" << clusterid << ") : Energy "
		       << cluster_i->get_energy() <<  ", Volume " << cluster_i->get_volume() << " , #Towers: " << cluster_i->getNTowers() << endl;

		  cout << "** Eta = " << cluster_i->get_eta() << " ,  Phi = " << cluster_i->get_phi() << endl;

		  cout << "** x,y,z = " << cluster_i->get_x() << " , " << cluster_i->get_y() << " , " << cluster_i->get_z() << endl;

		  CaloCluster::TowerConstRange begin_end = cluster_i->get_towers();
		  CaloCluster::TowerConstIterator iter;

		  cout << "** Towers(Energy) included: \n";
		  for (iter = begin_end.first; iter != begin_end.second; ++iter)
		    {
		      cout << (iter->first) << "(" << (iter->second) << ") \n";
		    }
		  cout << endl;


		}
	    }
	}
    }


  /* Fill tree with information from this event */
  float cluster_data[15] = {_nevent,
			    ncluster,
			    clusterID_max,
			    eta_max,
			    phi_max,
			    e_max,
			    volume_max,
			    density_max,
			    ntowers_max,
			    gpid,
			    gflavor,
			    geta,
			    gphi,
			    ge,
			    gpt
  };

  _t_cluster->Fill(cluster_data);

  return 0;
}

int G4CaloClusterAnalysis::End(PHCompositeNode * topNode)
{

  /* Select output file */
  _outfile->cd();

  /* Write Tree to output file */
  _t_cluster->Write();

  /* Write & Close output file */
  _outfile->Write();
  _outfile->Close();

  return 0;
}
