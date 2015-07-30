#ifndef _G4_CALO_SHOWER_ANALYSIS_H_
#define _G4_CALO_SHOWER_ANALYSIS_H_

/* STL includes */
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TGraph.h"

/* Fun4All includes */
#include <fun4all/SubsysReco.h>

/* Forward declerations */
class PHCompositeNode;
class PHG4TruthInfoContainer;
class PHG4Hit;
class PHG4HitContainer;
class Fun4AllHistoManager;

class TFile;
class TH1F;
class TGraph;

/** Analyze G4Hits in calorimeter, calculate shower parameters and store information in ROOT output file.
 *
 * \author Nils Feege <nils.feege@stonybrook.edu>
 * \version $$Revision: 1.1 $$
 * \date $$Date: 2015/03/19 23:31:21 $$
 */
class G4CaloShowerAnalysis : public SubsysReco
{
 public:

  /* Default constructor */
  G4CaloShowerAnalysis( const std::string name = "G4CaloShowerAnalysis", const std::string filename = "G4CaloShowerAnalysis.root" );

  /* Default destructor */
  virtual ~G4CaloShowerAnalysis();

  /* Full initialization method (called before first event)*/
  int Init(PHCompositeNode *);

  /* Event processing method - called for each event */
  int process_event(PHCompositeNode *);

  /* End of run method */
  int End(PHCompositeNode *);

  /* Set name for node with True particle information */
  void AddTrueParticleNode(const std::string &name) {_node_name_truth = name;}

  /* Add node name for another node with G4Hits (adding multiple nodes will make code
     loop through all of them for each event) */
  void AddG4HitNode(const std::string &name) {_node_names.push_back(name);}

  /* Switch on and define parameters for histogram storing event-wise total deposited energy */
  void SetStoreESum( bool onoff , int h_nbins , float h_xmin , float h_xmax ) {
    _store_esum = onoff;
    _h_esum_bins = h_nbins;
    _h_esum_xmin = h_xmin;
    _h_esum_xmax = h_xmax;
  }

  /* Switch on and define parameters for the comparison of the energy between two G4Hit Nodes */
  void SetComparison( bool onoff , const std::string &name ) {
    _comparison = onoff;
    _comparison_node_name = name;
  }

  void SetComparisonSum( bool onoff , const std::string &name ) {
    _comparison_sum = onoff;
    _comparison_sum_node_name = name;
  }

  /* Switch on and define parameters for histogram storing average logitudinal shower profile */
  void SetStoreLProf( bool onoff , int h_nbins , float h_xmin , float h_xmax ) {
    _store_lprof = onoff;
    _h_lprof_bins = h_nbins;
    _h_lprof_xmin = h_xmin;
    _h_lprof_xmax = h_xmax;
  }

  /* Switch on and define parameters for histogram storing average radial shower profile */
  void SetStoreRProf( bool onoff , int h_nbins , float h_xmin , float h_xmax ) {
    _store_rprof = onoff;
    _h_rprof_bins = h_nbins;
    _h_rprof_xmin = h_xmin;
    _h_rprof_xmax = h_xmax;
  }

protected:

  /* ROOT output file */
  TFile *_outfile;

  PHG4TruthInfoContainer *_truth_info_container;
  PHG4HitContainer *_g4hits;

  std::string _filename;
  std::string _node_name_truth;
  std::vector< std::string > _node_names;

  int _nevent;

  bool _store_esum;
  TH1F* _h_esum;
  int _h_esum_bins;
  int _h_esum_xmin;
  int _h_esum_xmax;

  bool _comparison;
  std::string _comparison_node_name;
  TGraph *_compare_esum;

  bool _comparison_sum;
  std::string _comparison_sum_node_name;
  TGraph *_compare_sum_esum;

  bool _store_lprof;
  TH1F* _h_lprof;
  int _h_lprof_bins;
  int _h_lprof_xmin;
  int _h_lprof_xmax;

  bool _store_rprof;
  TH1F* _h_rprof;
  int _h_rprof_bins;
  int _h_rprof_xmin;
  int _h_rprof_xmax;

};

#endif
