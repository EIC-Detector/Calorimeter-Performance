#include "/direct/phenix+u/nfeege/sphenixsw/devel/install/g4cemc/include/g4cemc/CaloTowerID.h"

using namespace std;

// global macro parameters
bool overlapcheck = false; // set to true if you want to check for overlaps

void
G4Init()
{
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


  /** Use dedicated FEMC module */
  PHG4ForwardEcalSubsystem *femc = new PHG4ForwardEcalSubsystem("FEMC");

  ostringstream mapping_femc;

  /* path to local copy of calibrations repository */
  //mapping_femc << "calibrations";

  /* path to central copy of calibrations repositry */
  mapping_femc << getenv("OFFLINE_MAIN") << "/share/calibrations";

  mapping_femc << "/ForwardEcal/mapping/towerMap_FEMC_v002.txt";
  cout << mapping_femc.str() << endl;

  femc->SetTowerMappingFile( mapping_femc.str() );
  femc->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem( femc );


  /** Use cone objects to build sampling calorimeter */
  //gROOT->LoadMacro("G4_FEcal_Cone.C");
  //FEcalInit();
  //G4_FEcal(g4Reco, 350.0, 1.1, 5.0, 100.);

  CaloTowerGeomManager* geoman = CaloTowerGeomManager::instance();
  geoman->ReadGeometryFromTable( calotowerid::FEMC , mapping_femc.str() );

  /**
   * 'spy' tracking layer infront of calorimeter to capture particle positions right before they
   * enter the caorimeter
   */
  PHG4ConeSubsystem *spy_femc = new PHG4ConeSubsystem("SPYFEMC", 0);
  spy_femc->SetZlength(0.0001);
  spy_femc->SetPlaceZ(349.9);
  spy_femc->Set_eta_range(1.2, 5.0);
  spy_femc->SetMaterial("G4_AIR");
  spy_femc->SetActive(true);
  spy_femc->SuperDetector("SPYFEMC");
  spy_femc->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(spy_femc);

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  se->registerSubsystem(g4Reco);
}
