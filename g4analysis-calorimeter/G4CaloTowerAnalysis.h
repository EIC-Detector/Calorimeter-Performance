#ifndef _G4_CALO_TOWER_ANALYSIS_H_
#define _G4_CALO_TOWER_ANALYSIS_H_

/*g4cemc includes */
#include <g4cemc/CaloTowerv1.h>
#include <g4cemc/CaloTowerContainer.h>

/* STL includes */
#include <string>
#include <vector>

/* Fun4All includes */
#include <fun4all/SubsysReco.h>

/* Forward declerations */
class PHCompositeNode;
class PHG4TruthInfoContainer;
class Fun4AllHistoManager;

class CaloTower;
class CaloTowerv1;
class CaloTowerContainer;

class TFile;
class TNtuple;

/** Analyze tower in calorimeter and store information in ROOT output file.
 *
 * \author Nils Feege <nils.feege@stonybrook.edu>
 * \version $$Revision: 1.1 $$
 * \date $$Date: 2015/03/19 23:31:21 $$
 */
class G4CaloTowerAnalysis : public SubsysReco
{
 public:

  /* Default constructor */
  G4CaloTowerAnalysis( const std::string name = "G4CaloTowerAnalysis", const std::string filename = "G4CaloTowerAnalysis.root" );

  /* Default destructor */
  virtual ~G4CaloTowerAnalysis();

  /* Full initialization method (called before first event)*/
  int Init(PHCompositeNode *);

  /* Event processing method - called for each event */
  int process_event(PHCompositeNode *);

  /* End of run method */
  int End(PHCompositeNode *);

  /* Set name for node with True particle information */
  void AddTrueParticleNode(const std::string &name) {_node_name_truth = name;}

  /* Add node name for another tower node (adding multiple nodes will make code
     loop through all of them for each event) */
  void AddTowerNode(const std::string &name) {_node_tower_names.push_back(name);}

protected:

  /* ROOT output file */
  TFile *_outfile;

  PHG4TruthInfoContainer *_truth_info_container;
  CaloTowerContainer *_tower;

  std::string _filename;
  std::string _node_name_truth;
  std::vector< std::string > _node_tower_names;

  int _nevent;

  TNtuple* _t_tower;
};

#endif
