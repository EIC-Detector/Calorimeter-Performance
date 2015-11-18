#ifndef _G4_CALO_TOWER_ANALYSIS_H_
#define _G4_CALO_TOWER_ANALYSIS_H_

/* STL includes */
#include <string>
#include <vector>

/* Fun4All includes */
#include <fun4all/SubsysReco.h>

/* Forward declerations */
class PHCompositeNode;
class PHG4TruthInfoContainer;
class Fun4AllHistoManager;

class RawTowerv1;
class RawTowerContainer;

class RawTowerGeomv2;
class RawTowerGeomContainerv1;

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

  /* Full initialization method for run (called before first event)*/
  int InitRun(PHCompositeNode *);

  /* Event processing method - called for each event */
  int process_event(PHCompositeNode *);

  /* End of run method */
  int End(PHCompositeNode *);

  /* Set name for node with True particle information */
  void SetTrueParticleNode(const std::string &name) {_node_name_truth = name;}

  /* Add node name for another tower node (adding multiple nodes will make code
     loop through all of them for each event) */
  void SetTowerNode(const std::string &name) {_node_name_tower = name;}

  /* Add node name for another tower geometry node (adding multiple nodes will make code
     loop through all of them for each event) */
  void SetTowerGeometryNode(const std::string &name) {_node_name_tower_geom = name;}

protected:

  /* ROOT output file */
  TFile *_outfile;

  PHG4TruthInfoContainer *_truth_info_container;
  RawTowerContainer *_tower;
  RawTowerGeomContainerv1 *_towergeom;

  std::string _filename;
  std::string _node_name_truth;
  std::string _node_name_tower;
  std::string _node_name_tower_geom;

  int _nevent;

  TNtuple* _t_tower;
};

#endif
