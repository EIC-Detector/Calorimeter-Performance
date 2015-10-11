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
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"

using namespace std;

G4CaloClusterAnalysis::G4CaloClusterAnalysis(const std::string name , const std::string filename ):
  SubsysReco( name ),
  _truth_info_container(NULL),
  _cluster(NULL),
  _filename(filename),
  _node_name_truth("G4TruthInfo"),
  _nevent(0),
  _h_esum(NULL)
{

}

G4CaloClusterAnalysis::~G4CaloClusterAnalysis()
{
}

int G4CaloClusterAnalysis::Init( PHCompositeNode* topNode )
{

  /* Create new output file */
  _outfile = new TFile(_filename.c_str(), "RECREATE");

  return 0;
}


void G4CaloClusterAnalysis::SetStoreESum( int h_nbins , float h_xmin , float h_xmax )
{
  /* Book histogram */
  _h_esum = new TH1F( "h_esum" , "" , h_nbins, h_xmin, h_xmax );
  _h_esum->GetXaxis()->SetTitle("E [GeV]");
  _h_esum->GetYaxis()->SetTitle("# Entries / #Sigma Entries");
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

  /* Get Cluster from input nodes */
  unsigned nnodes = _node_cluster_names.size();

  if ( !nnodes )
    {
      cout << PHWHERE << " WARNING: No cluster input node defined." << endl;
      return -1;//ABORTEVENT;
    }

  /* Loop over all input nodes for cluster */
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
	      /* Get raw cluster and energy */
	      cluster_i= dynamic_cast<CaloClusterv1*>( (*clusterit).second );
	      double energy = cluster_i->get_energy();

	      event_esum += energy;

	      /* Store single-cluster values */
	      // ...

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

  /* Store full-event values */
  if ( _h_esum )
    {
      _h_esum->Fill( event_esum );
    }

  return 0;
}

int G4CaloClusterAnalysis::End(PHCompositeNode * topNode)
{

  /* Select output file */
  _outfile->cd();

  /* Write histograms to output file */
  if ( _h_esum )
    _h_esum->Write();

  /* Write & Close output file */
  _outfile->Write();
  _outfile->Close();

  return 0;
}
