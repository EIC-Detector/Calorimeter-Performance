#include "/direct/phenix+u/nfeege/sphenixsw/devel/install/g4cemc/include/g4cemc/CaloTowerID.h"

using namespace std;

// global macro parameters
//double Min_forward_eta = 1.15;

bool overlapcheck = false; // set to true if you want to check for overlaps

void
G4Init()
{
  // load detector macros
}

/* simplified setup including only limited subdetectors
 */
void
G4Setup(const int absorberactive = 0, const float field = 0)
{
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4Reco* g4Reco = new PHG4Reco();
  g4Reco->set_rapidity_coverage(1.1); // temperary value assigned by ePHENIX LoI

  g4Reco->set_field(field); // use const solenoid field

  /////////////////////////////////////////////////
  //  electron going detectors
  /////////////////////////////////////////////////

  PHG4CrystalCalorimeterSubsystem *eecal = new PHG4CrystalCalorimeterSubsystem("EEMC");

  /* Use non-projective geometry */
  ostringstream mapping_eecal;
  //mapping_eecal << "calibrations/CrystalCalorimeter/mapping/towerMap_EEMC_v002.txt";
  mapping_eecal << getenv("OFFLINE_MAIN") <<
    "/share/calibrations/CrystalCalorimeter/mapping/towerMap_EEMC_v002.txt";
  cout << mapping_eecal.str() << endl;
  eecal->SetTowerMappingFile( mapping_eecal.str() );


  /* Use projective geometry */
  //ostringstream mapping_eecal;
  //mapping_eecal << "/direct/phenix+u/jlab/github/sPHENIX-Fork/calibrations/CrystalCalorimeter/mapping/crystals_v005.txt";
  //ostringstream mapping_eecal_2;
  //mapping_eecal_2 << "/direct/phenix+u/jlab/github/sPHENIX-Fork/calibrations/CrystalCalorimeter/mapping/4_by_4_construction_v005.txt";
  //
  //eecal->SetProjectiveGeometry( mapping_eecal.str() , mapping_eecal_2.str() );


  /* register Ecal module */
  eecal->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem( eecal );


  /* load mapping in geometry manager */
  CaloTowerGeomManager* geoman = CaloTowerGeomManager::instance();
  geoman->ReadGeometryFromTable( calotowerid::EEMC , mapping_eecal.str() );


  /* 'Envelope' detector to catch leaing energy */
  PHG4EnvelopeSubsystem *envelope = new PHG4EnvelopeSubsystem("ENVELOPE");
  envelope->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem( envelope );

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  se->registerSubsystem(g4Reco);
}
