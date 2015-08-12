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


  /** Use dedicated FHCAL module */
  PHG4ForwardHcalSubsystem *hhcal = new PHG4ForwardHcalSubsystem("FHCAL");

  ostringstream mapping_hhcal;
  mapping_hhcal << getenv("OFFLINE_MAIN") <<
    "/share/calibrations/ForwardHcal/mapping/towerMap_FHCAL_v001.txt";
  cout << mapping_hhcal.str() << endl;

  hhcal->SetTowerMappingFile( mapping_hhcal.str() );
  hhcal->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem( hhcal );


  /** Use cone objects to build sampling calorimeter */
  //gROOT->LoadMacro("G4_FHcal_Cone.C");
  //FHCalInit();
  //G4_FHCal(g4Reco, 350.0, 1.1, 5.0, 100.);


  /**
   * 'spy' tracking layer infront of calorimeter to capture particle positions right before they
   * enter the caorimeter
   */
  PHG4ConeSubsystem *spy_hhcal = new PHG4ConeSubsystem("SPYHHCAL", 0);
  spy_hhcal->SetZlength(0.0001);
  spy_hhcal->SetPlaceZ(349.9);
  spy_hhcal->Set_eta_range(1.2, 5.0);
  spy_hhcal->SetMaterial("G4_AIR");
  spy_hhcal->SetActive(true);
  spy_hhcal->SuperDetector("SPYHHCAL");
  spy_hhcal->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(spy_hhcal);

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  se->registerSubsystem(g4Reco);
}
