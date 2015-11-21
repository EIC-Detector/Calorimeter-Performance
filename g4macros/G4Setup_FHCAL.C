using namespace std;

// global macro parameters
bool overlapcheck = false; // set to true if you want to check for overlaps

void
FHCALInit()
{
}

/* simplified setup including only limited subdetectors
 */
void
FHCALSetup(PHG4Reco* g4Reco, const int absorberactive = 0)
{
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();


  /** Use dedicated FHCAL module */
  PHG4ForwardHcalSubsystem *hhcal = new PHG4ForwardHcalSubsystem("FHCAL");

  ostringstream mapping_hhcal;

  /* path to central copy of calibrations repositry */
  mapping_hhcal << getenv("OFFLINE_MAIN") << "/share/calibrations";

  mapping_hhcal << "/ForwardHcal/mapping/towerMap_FHCAL_v003.txt";
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

}
