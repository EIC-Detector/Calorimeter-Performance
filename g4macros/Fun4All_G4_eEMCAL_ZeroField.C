#include "/direct/phenix+u/nfeege/sphenixsw/devel/install/g4cemc/include/g4cemc/CaloTowerID.h"

int
Fun4All_G4_eEMCAL_ZeroField(
			   int nEvents = 10,
			   const char * outputFile = "G4ePHENIX_e-_p10_eta-2_detailed_TEST.root",
			   const char * ptype = "e-",
			   float ppmin = 10,
			   float ppmax = 10,
			   float petamin = -2,
			   float petamax = -2
			   )
{

  cout << "Run Fun4All_G4_eEMCAL_ZeroField with... " << endl;
  cout << "... nEvents = " << nEvents << endl;
  cout << "... particle type = " << ptype << endl;
  cout << "... momentum (min) = " << ppmin << endl;
  cout << "... momentum (max) = " << ppmax << endl;
  cout << "... eta (min) = " << petamin << endl;
  cout << "... eta (max) = " << petamax << endl;

  //---------------
  // Load libraries
  //---------------

  const int absorberactive = 0; // set to 1 to make all absorbers active volumes
  const bool verbosity = false; // very slow but very detailed logs
  //const bool verbosity = true; // very slow but very detailed logs

  gSystem->Load("libphool.so"); // core library
  gSystem->Load("libfun4all.so"); // core library
  gSystem->Load("libg4detectors.so"); // detector modules
  gSystem->Load("libcemc.so"); // tower, digitization etc for calorimeter
  gSystem->Load("libg4ana_calo.so"); // calorimeter analysis

  /* Choose detector configuration */
  gROOT->LoadMacro("G4Setup_eEMCAL.C");
  G4Init(); // initialize layer numbers

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  //-----------------
  // Event generation
  //-----------------
  PHG4ParticleGenerator *gen = new PHG4ParticleGenerator();
  // gen->set_seed(TRandom3(0).GetSeed()); // random seed
  gen->set_seed(1234); // fixed seed
  gen->set_name(ptype); // e,pi,mu,p,gamma
  gen->set_vtx(0, 0, 0);
  gen->set_z_range(0, 0);
  gen->set_eta_range(petamin, petamax);
  gen->set_phi_range(TMath::Pi() / 4.0 , TMath::Pi() / 4.0 );
  //gen->set_phi_range(TMath::Pi() / 2, TMath::Pi() / 2 );
  //gen->set_phi_range( 0 , 2 * TMath::Pi() );
  gen->set_mom_range(ppmin, ppmax); // 1 10 20 50
  // gen->Verbosity(1);

  se->registerSubsystem(gen);

  //---------------------
  // Detector description from loaded macro
  //---------------------
  G4Setup(absorberactive, 0);

  //----------------------
  // G4Hit analysis for Calorimeter
  //----------------------

  ostringstream name;
  name.str("");
  name << "G4Shower_EEMC_Default" << "_p_"<< ppmin << "_" << ppmax << "_GeV" << "_eta_" << petamin << "_" << petamax << "_" << nEvents << ".root" ;

  G4CaloShowerAnalysis* showerAnalysis = new G4CaloShowerAnalysis( "G4Shower_EEMC" , name.str().c_str() );
  showerAnalysis->AddG4HitNode("G4HIT_EEMC");
  showerAnalysis->SetStoreESum( true , 4001 , -0.005 , 40.005 );
  showerAnalysis->SetComparison( true , "G4HIT_ENVELOPE_ENVELOPE" );
  showerAnalysis->SetComparisonSum( true , "G4HIT_ABSORBER_EEMC" );
  
  se->registerSubsystem(showerAnalysis);

  //-----------------------------------------
  //
  //
/*
  ostringstream new_name;
  new_name.str("");
  new_name << "G4Shower_ENVELOPE_EEMC" << "_p_"<< ppmin << "_" << ppmax << "_GeV" << "_eta_" << petamin << "_" << petamax << "_" << nEvents << ".root" ;

  G4CaloShowerAnalysis* absorberAnalysis = new G4CaloShowerAnalysis( "G4Shower_ENVELOPE_ENVELOPE" , new_name.str().c_str() );
  absorberAnalysis->AddG4HitNode("G4HIT_ENVELOPE_ENVELOPE");
  absorberAnalysis->SetStoreESum( true , 40001 , -0.005 , 40.005 );

  se->registerSubsystem(absorberAnalysis);
*/
  //-----------------------------------------

  
  //----------------------
  // Build Calorimeter Tower
  //----------------------
  CaloTowerBuilderByHitIndex* tower_EEMC = new CaloTowerBuilderByHitIndex();
  tower_EEMC->Detector("EEMC", calotowerid::EEMC);

  se->registerSubsystem(tower_EEMC);

  /*
  //----------------------
  // Digitization
  //----------------------
  CrystalCalorimeterDigitization* digi_EEMC = new CrystalCalorimeterDigitization("CrystalCalorimeterDigitization","TOWER_EEMC","TDIGI_EEMC");

  se->registerSubsystem(digi_EEMC);
  */

  //----------------------
  // Tower analysis for Calorimeter
  //----------------------

  ostringstream name_2;
  name_2.str("");
  name_2 << "TowerAna_EEMC" << "_p_"<< ppmin << "_" << ppmax << "_GeV" << "_eta_" << petamin << "_" << petamax << "_" << nEvents << ".root" ;

  G4CaloTowerAnalysis* towerAnalysis = new G4CaloTowerAnalysis( "TowerAna_EEMC" , name_2.str().c_str() );
  towerAnalysis->AddTowerNode("TOWER_EEMC");
//  towerAnalysis->AddTowerNode("TDIGI_EEMC");
  towerAnalysis->SetStoreESum( true , 10000001 , -0.005 , 10000000.005 );

  se->registerSubsystem(towerAnalysis);
  

  //--------------
  // IO management
  //--------------
  // for single particle generators we just need something which drives
  // the event loop, the Dummy Input Mgr does just that
  Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
  se->registerInputManager(in);

  //-----------------
  // Event processing
  //-----------------
  if (nEvents <= 1)
    {
      PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
      //g4->StartGui();
      g4->ApplyCommand("/control/execute eic.mac");
/*
      g4->ApplyCommand("/vis/open DAWNFILE");
      g4->ApplyCommand("/vis/drawVolume");
      g4->ApplyCommand("/vis/viewer/flush");
      g4->ApplyCommand("/vis/scene/add/trajectories");
      g4->ApplyCommand("/vis/scene/add/hits");
*/   
      se->run(1);

      se->End();
      std::cout << "All done" << std::endl;
    }
  else
    {
      if (verbosity)
        {
          se->Verbosity(3);
          PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
          g4->Verbosity(3);
          g4->ApplyCommand("/control/verbose 5");
          g4->ApplyCommand("/run/verbose  5");
          g4->ApplyCommand("/tracking/verbose 5");
       }

      se->run(nEvents);

      se->End();
      std::cout << "All done" << std::endl;
      delete se;
      gSystem->Exit(0);
    }

  //-----
  // Exit
  //-----

  return 0;
}
