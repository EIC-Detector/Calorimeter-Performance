#ifndef _G4_CALO_CLUSTER_ANALYSIS_H_
#define _G4_CALO_CLUSTER_ANALYSIS_H_

/*g4cemc includes */
#include <g4cemc/CaloClusterv1.h>
#include <g4cemc/CaloClusterContainer.h>

/* STL includes */
#include <string>
#include <vector>

/* Fun4All includes */
#include <fun4all/SubsysReco.h>

/* Forward declerations */
class PHCompositeNode;
class PHG4TruthInfoContainer;
class Fun4AllHistoManager;

class CaloCluster;
class CaloClusterv1;
class CaloClusterContainer;

class TFile;
class TH1F;

/** Analyze cluster in calorimeter and store information in ROOT output file.
 *
 * \author Nils Feege <nils.feege@stonybrook.edu>
 * \version $$Revision: 1.1 $$
 * \date $$Date: 2015/03/19 23:31:21 $$
 */
class G4CaloClusterAnalysis : public SubsysReco
{
 public:

  /* Default constructor */
  G4CaloClusterAnalysis( const std::string name = "G4CaloClusterAnalysis", const std::string filename = "G4CaloClusterAnalysis.root" );

  /* Default destructor */
  virtual ~G4CaloClusterAnalysis();

  /* Full initialization method (called before first event)*/
  int Init(PHCompositeNode *);

  /* Event processing method - called for each event */
  int process_event(PHCompositeNode *);

  /* End of run method */
  int End(PHCompositeNode *);

  /* Set name for node with True particle information */
  void AddTrueParticleNode(const std::string &name) {_node_name_truth = name;}

  /* Add node name for another cluster node (adding multiple nodes will make code
     loop through all of them for each event) */
  void AddClusterNode(const std::string &name) {_node_cluster_names.push_back(name);}

  /* Switch on and define parameters for histogram storing event-wise total deposited energy */
  void SetStoreESum( int h_nbins , float h_xmin , float h_xmax );

protected:

  /* ROOT output file */
  TFile *_outfile;

  PHG4TruthInfoContainer *_truth_info_container;
  CaloClusterContainer *_cluster;

  std::string _filename;
  std::string _node_name_truth;
  std::vector< std::string > _node_cluster_names;

  int _nevent;

  TH1F* _h_esum;

};

#endif
