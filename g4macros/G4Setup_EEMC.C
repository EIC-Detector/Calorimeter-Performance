using namespace std;

bool overlapcheck = false; // set to true if you want to check for overlaps

void
EEMCInit()
{
  // load detector macros
}

/* simplified setup including only limited subdetectors
 */
void
EEMCSetup(PHG4Reco* g4Reco, const int absorberactive = 0)
{
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();


  /////////////////////////////////////////////////
  //  electron going detectors
  /////////////////////////////////////////////////

  PHG4CrystalCalorimeterSubsystem *eecal = new PHG4CrystalCalorimeterSubsystem("EEMC");

  /* Use non-projective geometry */
  ostringstream mapping_eecal;
  mapping_eecal << getenv("OFFLINE_MAIN") <<
    "/share/calibrations/CrystalCalorimeter/mapping/towerMap_EEMC_v002.txt";
  cout << mapping_eecal.str() << endl;
  eecal->SetTowerMappingFile( mapping_eecal.str() );


  /* register Ecal module */
  eecal->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem( eecal );


  /* load mapping in geometry manager for later use in Fun4All */
  CaloTowerGeomManager* geoman = CaloTowerGeomManager::instance();
  geoman->ReadGeometryFromTable( "EEMC" , mapping_eecal.str() );


  /* 'Envelope' detector to catch leaing energy */
  //  PHG4EnvelopeSubsystem *envelope = new PHG4EnvelopeSubsystem("ENVELOPE");
  //  envelope->OverlapCheck(overlapcheck);
  //  g4Reco->registerSubsystem( envelope );

  //  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  //  g4Reco->registerSubsystem(truth);

}
