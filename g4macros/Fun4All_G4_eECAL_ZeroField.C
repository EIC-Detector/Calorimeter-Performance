int
Fun4All_G4_eECAL_ZeroField(
			   int nEvents = 10,
			   const char * outputFile = "G4ePHENIX_e-_p10_eta-2_detailed_TEST.root",
			   const char * ptype = "e-",
			   float ppmin = 10,
			   float ppmax = 10,
			   float petamin = -2,
			   float petamax = -2
			   )
{

  cout << "Run Fun4All_G4_eECAL_ZeroField with... " << endl;
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
  //  const bool verbosity = true; // very slow but very detailed logs

  gSystem->Load("libphool.so"); // core library
  gSystem->Load("libfun4all.so"); // core library
  gSystem->Load("libg4detectors.so"); // detector modules
  gSystem->Load("libcemc.so"); // tower, digitization etc for calorimeter
  gSystem->Load("libg4ana_calo.so"); // calorimeter analysis

  /* Choose detector configuration */
  gROOT->LoadMacro("G4Setup_eECAL.C");
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
  gen->set_phi_range(TMath::Pi() / 2, TMath::Pi() / 2);
  //gen->set_phi_range( 0, 2 * TMath::Pi() );
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
  G4CaloShowerAnalysis* showerAnalysis = new G4CaloShowerAnalysis( "G4Shower_eECAL" , "G4Shower_eECAL.root" );
  showerAnalysis->AddG4HitNode("G4HIT_eECAL");
  showerAnalysis->SetStoreESum( true , 100 , 0 , 20 );

  se->registerSubsystem(showerAnalysis);

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
      //      g4->StartGui();
      g4->ApplyCommand("/control/execute eic.mac");
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
