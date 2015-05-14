using namespace std;

// global macro parameters
double Min_forward_eta = 1.15;

double no_overlapp = 0.0001; // added to radii to avoid overlapping volumes
bool overlapcheck = false; // set to true if you want to check for overlaps

void
G4Init()
{
  // load detector macros
  gROOT->LoadMacro("G4Detector_Cone_eEMCAL.C");
  eEMCAL_Init();
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
  //double newzpos = G4Detector_Cone_eEMCAL(g4Reco, -99., -1.2, -4.5, 18. ); // 18cm thick PbWO4 crystal calorimeter

  PHG4CrystalCalorimeterSubsystem *eecal = new PHG4CrystalCalorimeterSubsystem("eEMCAL");
  eecal->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem( eecal );

  /**
   * 'spy' tracking layer infront of eEMCAL to capture particle positions right before they
   * enter the caorimeter
   */
  //  PHG4ConeSubsystem *spy_eecal = new PHG4ConeSubsystem("SPYEEMCAL", 0);
  //  spy_eecal->SetZlength(0.0001);
  //  spy_eecal->SetPlaceZ(-98.9);
  //  spy_eecal->Set_eta_range(-1.2, -4.5);
  //  spy_eecal->SetMaterial("G4_AIR");
  //  spy_eecal->SetActive(true);
  //  spy_eecal->SuperDetector("SPYEEMCAL");
  //  spy_eecal->OverlapCheck(overlapcheck);
  //  g4Reco->registerSubsystem(spy_eecal);

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  se->registerSubsystem(g4Reco);
}
