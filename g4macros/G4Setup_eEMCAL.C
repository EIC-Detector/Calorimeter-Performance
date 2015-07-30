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

  PHG4CrystalCalorimeterSubsystem *eecal = new PHG4CrystalCalorimeterSubsystem("eEMCAL");
  eecal->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem( eecal );

  PHG4EnvelopeSubsystem *envelope = new PHG4EnvelopeSubsystem("ENVELOPE");
  envelope->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem( envelope );

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  se->registerSubsystem(g4Reco);
}
