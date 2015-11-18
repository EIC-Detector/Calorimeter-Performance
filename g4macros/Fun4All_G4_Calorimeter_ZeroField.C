int
Fun4All_G4_Calorimeter_ZeroField(
				 int nEvents = 10,
				 const char * outputFile = "G4ePHENIX_e-_p10_eta-2_detailed_TEST.root",
				 const char * ptype = "e-",
				 float ppmin = 10,
				 float ppmax = 10,
				 float petamin = -2,
				 float petamax = -2,
				 float phimin = (TMath::Pi() / 4),
				 float phimax = (TMath::Pi() / 4)
				 )
{

  cout << "Run Fun4All_G4_Calorimeter_ZeroField with... " << endl;
  cout << "... nEvents = " << nEvents << endl;
  cout << "... particle type = " << ptype << endl;
  cout << "... momentum (min) = " << ppmin << endl;
  cout << "... momentum (max) = " << ppmax << endl;
  cout << "... eta (min) = " << petamin << endl;
  cout << "... eta (max) = " << petamax << endl;
  cout << "... phi (min) = " << phimin << endl;
  cout << "... phi (max) = " << phimax << endl;


  //======================
  // What to run
  //======================

  bool do_EEMC = true;
  bool do_FEMC = true;
  bool do_FHCAL = true;

  bool do_ShowerAnalysis = false;
  bool do_TowerAnalysis = false;

  //Option to convert DST to human command readable TTree for quick poke around the outputs
  bool do_DSTReader = true;

  //---------------
  // Load libraries
  //---------------

  const int absorberactive = 0; // set to 1 to make all absorbers active volumes
  const bool verbosity = false; // very slow but very detailed logs
  //const bool verbosity = true; // very slow but very detailed logs

  //  gSystem->Load("libphool.so"); // core library
  gSystem->Load("libfun4all.so"); // core library
  gSystem->Load("libg4detectors.so"); // detector modules
  gSystem->Load("libcemc.so"); // tower, digitization etc for calorimeter
  gSystem->Load("libg4eval.so");
  gSystem->Load("libg4ana_calo.so"); // calorimeter analysis

  /* Choose detector configuration */
  if ( do_EEMC )
    {
      gROOT->LoadMacro("G4Setup_EEMC.C");
      EEMCInit();
    }

  if ( do_FEMC )
    {
      gROOT->LoadMacro("G4Setup_FEMC.C");
      FEMCInit();
    }

  if ( do_FHCAL )
    {
      gROOT->LoadMacro("G4Setup_FHCAL.C");
      FHCALInit();
    }

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc = recoConsts::instance();
  // By default every random number generator uses
  // PHRandomSeed() which reads /dev/urandom to get its seed
  // if the RANDOMSEED flag is set its value is taken as seed
  // You ca neither set this to a random value using PHRandomSeed()
  // which will make all seeds identical (not sure what the point of
  // this would be:
  //  rc->set_IntFlag("RANDOMSEED",PHRandomSeed());
  // or set it to a fixed value so you can debug your code
  rc->set_IntFlag("RANDOMSEED", 12345);

  //-----------------
  // Event generation
  //-----------------
  PHG4ParticleGenerator *gen = new PHG4ParticleGenerator();
  gen->set_name(ptype); // e,pi,mu,p,gamma
  gen->set_vtx(0, 0, 0);
  gen->set_z_range(0, 0);
  gen->set_eta_range( petamin, petamax );
  gen->set_phi_range( phimin, phimax );
  //gen->set_phi_range( 0 , 2 * TMath::Pi() );
  gen->set_mom_range(ppmin, ppmax); // 1 10 20 50
  // gen->Verbosity(1);

  se->registerSubsystem(gen);


  //---------------------
  // Detector description from loaded macro
  //---------------------
  PHG4Reco* g4Reco = new PHG4Reco();
  g4Reco->set_rapidity_coverage(1.1); // temperary value assigned by ePHENIX LoI

  g4Reco->set_field(0); // use const solenoid field

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  if ( do_EEMC )
    EEMCSetup(g4Reco, absorberactive);

  if ( do_FEMC )
    FEMCSetup(g4Reco, absorberactive);

  if ( do_FHCAL )
    FHCALSetup(g4Reco, absorberactive);

  se->registerSubsystem(g4Reco);


  //----------------------
  // Build Calorimeter Tower
  //----------------------
  if ( do_EEMC )
    {
      ostringstream mapping_eemc;
      mapping_eemc << getenv("OFFLINE_MAIN") <<
	"/share/calibrations/CrystalCalorimeter/mapping/towerMap_EEMC_v002.txt";

      RawTowerBuilderByHitIndex* tower_EEMC = new RawTowerBuilderByHitIndex("TowerBuilder_EEMC");
      tower_EEMC->Detector("EEMC");
      tower_EEMC->set_sim_tower_node_prefix("SIM");
      tower_EEMC->GeometryTableFile( mapping_eemc.str() );

      se->registerSubsystem(tower_EEMC);
    }

  if ( do_FEMC )
    {
      ostringstream mapping_femc;
      mapping_femc << getenv("OFFLINE_MAIN") <<
	"/share/calibrations/ForwardEcal/mapping/towerMap_FEMC_v002.txt";

      RawTowerBuilderByHitIndex* tower_FEMC = new RawTowerBuilderByHitIndex("TowerBuilder_FEMC");
      tower_FEMC->Detector("FEMC");
      tower_FEMC->set_sim_tower_node_prefix("SIM");
      tower_FEMC->GeometryTableFile( mapping_femc.str() );

      se->registerSubsystem(tower_FEMC);
    }

  if ( do_FHCAL )
    {
      ostringstream mapping_fhcal;
      mapping_fhcal << getenv("OFFLINE_MAIN") <<
	"/share/calibrations/ForwardHcal/mapping/towerMap_FHCAL_v002.txt";

      RawTowerBuilderByHitIndex* tower_FHCAL = new RawTowerBuilderByHitIndex("TowerBuilder_FHCAL");
      tower_FHCAL->Detector("FHCAL");
      tower_FHCAL->set_sim_tower_node_prefix("SIM");
      tower_FHCAL->GeometryTableFile( mapping_fhcal.str() );

      se->registerSubsystem(tower_FHCAL);
    }


  //----------------------
  // Calorimeter Tower Digitization
  //----------------------
  if ( do_EEMC )
    {
      // CMS lead tungstate barrel ECAL at 18 degree centrigrade: 4.5 photoelectrons per MeV
      const double EEMC_photoelectron_per_GeV = 4500;

      RawTowerDigitizer *TowerDigitizer_EEMC = new RawTowerDigitizer("EEMCRawTowerDigitizer");
      TowerDigitizer_EEMC->Detector("EEMC");
      TowerDigitizer_EEMC->Verbosity(verbosity);
      TowerDigitizer_EEMC->set_raw_tower_node_prefix("RAW");
      TowerDigitizer_EEMC->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitalization);
      TowerDigitizer_EEMC->set_pedstal_central_ADC(0);
      TowerDigitizer_EEMC->set_pedstal_width_ADC(8);// eRD1 test beam setting
      TowerDigitizer_EEMC->set_photonelec_ADC(1);//not simulating ADC discretization error
      TowerDigitizer_EEMC->set_photonelec_yield_visible_GeV( EEMC_photoelectron_per_GeV );
      TowerDigitizer_EEMC->set_zero_suppression_ADC(16); // eRD1 test beam setting
      se->registerSubsystem( TowerDigitizer_EEMC );
    }

  if ( do_FEMC )
    {
      const double FEMC_photoelectron_per_GeV = 500; //500 photon per total GeV deposition

      RawTowerDigitizer *TowerDigitizer_FEMC = new RawTowerDigitizer("FEMCRawTowerDigitizer");
      TowerDigitizer_FEMC->Detector("FEMC");
      TowerDigitizer_FEMC->Verbosity(verbosity);
      TowerDigitizer_FEMC->set_raw_tower_node_prefix("RAW");
      TowerDigitizer_FEMC->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitalization);
      TowerDigitizer_FEMC->set_pedstal_central_ADC(0);
      TowerDigitizer_FEMC->set_pedstal_width_ADC(8);// eRD1 test beam setting
      TowerDigitizer_FEMC->set_photonelec_ADC(1);//not simulating ADC discretization error
      TowerDigitizer_FEMC->set_photonelec_yield_visible_GeV( FEMC_photoelectron_per_GeV );
      TowerDigitizer_FEMC->set_zero_suppression_ADC(16); // eRD1 test beam setting
      se->registerSubsystem( TowerDigitizer_FEMC );
    }

  if ( do_FHCAL )
    {
      const double FHCAL_photoelectron_per_GeV = 500;

      RawTowerDigitizer *TowerDigitizer_FHCAL = new RawTowerDigitizer("FHCALRawTowerDigitizer");
      TowerDigitizer_FHCAL->Detector("FHCAL");
      TowerDigitizer_FHCAL->Verbosity(verbosity);
      TowerDigitizer_FHCAL->set_raw_tower_node_prefix("RAW");
      TowerDigitizer_FHCAL->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitalization);
      TowerDigitizer_FHCAL->set_pedstal_central_ADC(0);
      TowerDigitizer_FHCAL->set_pedstal_width_ADC(8);// eRD1 test beam setting
      TowerDigitizer_FHCAL->set_photonelec_ADC(1);//not simulating ADC discretization error
      TowerDigitizer_FHCAL->set_photonelec_yield_visible_GeV( FHCAL_photoelectron_per_GeV );
      TowerDigitizer_FHCAL->set_zero_suppression_ADC(16); // eRD1 test beam setting
      se->registerSubsystem( TowerDigitizer_FHCAL );

    }

  //----------------------
  // Calorimeter Tower Calibration
  //----------------------
  if ( do_EEMC )
    {
      // CMS lead tungstate barrel ECAL at 18 degree centrigrade: 4.5 photoelectrons per MeV
      const double EEMC_photoelectron_per_GeV = 4500;

      RawTowerCalibration *TowerCalibration_EEMC = new RawTowerCalibration("EEMCRawTowerCalibration");
      TowerCalibration_EEMC->Detector("EEMC");
      TowerCalibration_EEMC->Verbosity(verbosity);
      TowerCalibration_EEMC->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
      TowerCalibration_EEMC->set_calib_const_GeV_ADC( 1. / EEMC_photoelectron_per_GeV );
      TowerCalibration_EEMC->set_pedstal_ADC( 0 );
      se->registerSubsystem( TowerCalibration_EEMC );
    }

  if ( do_FEMC )
    {
      const double FEMC_photoelectron_per_GeV = 500; //500 photon per total GeV deposition

      RawTowerCalibration *TowerCalibration_FEMC = new RawTowerCalibration("FEMCRawTowerCalibration");
      TowerCalibration_FEMC->Detector("FEMC");
      TowerCalibration_FEMC->Verbosity(verbosity);
      TowerCalibration_FEMC->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
      TowerCalibration_FEMC->set_calib_const_GeV_ADC( 1. / FEMC_photoelectron_per_GeV );
      TowerCalibration_FEMC->set_pedstal_ADC( 0 );
      se->registerSubsystem( TowerCalibration_FEMC );
    }

  if ( do_FHCAL )
    {
      const double FHCAL_photoelectron_per_GeV = 500;

      RawTowerCalibration *TowerCalibration_FHCAL = new RawTowerCalibration("FHCALRawTowerCalibration");
      TowerCalibration_FHCAL->Detector("FHCAL");
      TowerCalibration_FHCAL->Verbosity(verbosity);
      TowerCalibration_FHCAL->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
      TowerCalibration_FHCAL->set_calib_const_GeV_ADC( 1. / FHCAL_photoelectron_per_GeV );
      TowerCalibration_FHCAL->set_pedstal_ADC( 0 );
      se->registerSubsystem( TowerCalibration_FHCAL );
    }


  //----------------------
  // Build Calorimeter Cluster
  //----------------------
//  if ( do_EEMC )
//    {
//      RawClusterBuilder* cluster_EEMC = new RawClusterBuilder("EEMCClusterBuilder");
//      cluster_EEMC->Detector("EEMC");
//      cluster_EEMC->Verbosity(verbosity);
//      se->registerSubsystem(cluster_EEMC);
//    }
//
//  if ( do_FEMCg )
//    {
//      RawClusterBuilder* cluster_FEMC = new RawClusterBuilder("FEMCClusterBuilder");
//      cluster_FEMC->Detector("FEMC");
//      cluster_FEMC->Verbosity(verbosity);
//      se->registerSubsystem(cluster_FEMC);
//    }
//
//  if ( do_FHCAL )
//    {
//      RawClusterBuilder* cluster_FHCAL = new RawClusterBuilder("FHCALClusterBuilder");
//      cluster_FHCAL->Detector("FHCAL");
//      cluster_FHCAL->Verbosity(verbosity);
//      se->registerSubsystem(cluster_FHCAL);
//    }


  //----------------------
  // G4Hit analysis for Calorimeter
  //----------------------
  if ( do_ShowerAnalysis )
    {
      if ( do_EEMC )
	{
	  ostringstream fname_hit_eemc;
	  fname_hit_eemc.str("");
	  fname_hit_eemc << "G4Hit_Eemc_Default" << "_p_"<< ppmin << "_" << ppmax << "_GeV"
			 << "_eta_"  << petamin << "_" << petamax <<  "_" << "phi_" << phimin << "_" << phimax << "_" << nEvents << ".root" ;

	  G4CaloShowerAnalysis* hitAna_EEMC = new G4CaloShowerAnalysis( "G4Hit_Eemc" , fname_hit_eemc.str().c_str() );
	  hitAna_EEMC->AddG4HitNode("G4HIT_EEMC");
	  hitAna_EEMC->SetStoreESum( true , 4001 , -0.005 , 40.005 );
	  hitAna_EEMC->SetComparison( true , "G4HIT_ENVELOPE_ENVELOPE" );
	  hitAna_EEMC->SetComparisonSum( true , "G4HIT_ABSORBER_EEMC" );

	  se->registerSubsystem(hitAna_EEMC);
	}

      if ( do_FEMC )
	{
	  //...
	}

      if ( do_FHCAL )
	{
	  //...
	}
    }

  //----------------------
  // G4Hit analysis for Calorimeter Envelope
  //----------------------
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
  // Tower analysis for Calorimeter
  //----------------------
  if ( do_TowerAnalysis )
    {
      if ( do_EEMC )
	{
	  ostringstream fname_tower_eemc;
	  fname_tower_eemc.str("");
	  fname_tower_eemc << "TowerAna_EEMC" << "_p_"<< ppmin << "_" << ppmax << "_GeV"
			   << "_eta_"  << petamin << "_" << petamax <<  "_" << "phi_" << phimin << "_" << phimax << "_" << nEvents << ".root" ;

	  G4RawTowerAnalysis* towerAna_EEMC = new G4RawTowerAnalysis( "TowerAna_EEMC" , fname_tower_eemc.str().c_str() );
	  towerAna_EEMC->AddTowerNode("TOWER_EEMC");
	  se->registerSubsystem(towerAna_EEMC);
	}

      if ( do_FEMC )
	{
	  ostringstream fname_tower_femc;
	  fname_tower_femc.str("");
	  fname_tower_femc << "TowerAna_FEMC" << "_p_"<< ppmin << "_" << ppmax << "_GeV"
			   << "_eta_"  << petamin << "_" << petamax <<  "_" << "phi_" << phimin << "_" << phimax << "_" << nEvents << ".root" ;

	  G4RawTowerAnalysis* towerAna_FEMC = new G4RawTowerAnalysis( "TowerAna_FEMC" , fname_tower_femc.str().c_str() );
	  towerAna_FEMC->AddTowerNode("TOWER_FEMC");
	  se->registerSubsystem(towerAna_FEMC);
	}

      if ( do_FHCAL )
	{
	  ostringstream fname_tower_fhcal;
	  fname_tower_fhcal.str("");
	  fname_tower_fhcal << "TowerAna_FHCAL" << "_p_"<< ppmin << "_" << ppmax << "_GeV"
			   << "_eta_"  << petamin << "_" << petamax <<  "_" << "phi_" << phimin << "_" << phimax << "_" << nEvents << ".root" ;

	  G4RawTowerAnalysis* towerAna_FHCAL = new G4RawTowerAnalysis( "TowerAna_FHCAL" , fname_tower_fhcal.str().c_str() );
	  towerAna_FHCAL->AddTowerNode("TOWER_FHCAL");
	  se->registerSubsystem(towerAna_FHCAL);
	}
    }


  //----------------------
  // Cluster analysis for Calorimeter
  //----------------------
//  if ( do_ClusterAnalysis )
//    {
//      if ( do_EEMC )
//	{
//	  ostringstream fname_cluster_eemc;
//	  fname_cluster_eemc.str("");
//	  fname_cluster_eemc << "ClusterAna_EEMC" << "_p_"<< ppmin << "_" << ppmax << "_GeV"
//			     << "_eta_"  << petamin << "_" << petamax <<  "_" << "phi_" << phimin << "_" << phimax << "_" << nEvents << ".root" ;
//
//	  G4RawClusterAnalysis* clusterAna_EEMC = new G4RawClusterAnalysis( "ClusterAna_EEMC" , fname_cluster_eemc.str().c_str() );
//	  clusterAna_EEMC->AddClusterNode("CLUSTER_EEMC");
//	  se->registerSubsystem(clusterAna_EEMC);
//	}
//
//      if ( do_FEMC )
//	{
//	  	  ostringstream fname_cluster_femc;
//	  fname_cluster_femc.str("");
//	  fname_cluster_femc << "ClusterAna_FEMC" << "_p_"<< ppmin << "_" << ppmax << "_GeV"
//			     << "_eta_"  << petamin << "_" << petamax <<  "_" << "phi_" << phimin << "_" << phimax << "_" << nEvents << ".root" ;
//
//	  G4RawClusterAnalysis* clusterAna_FEMC = new G4RawClusterAnalysis( "ClusterAna_FEMC" , fname_cluster_femc.str().c_str() );
//	  clusterAna_FEMC->AddClusterNode("CLUSTER_FEMC");
//	  se->registerSubsystem(clusterAna_FEMC);
//	}
//
//      if ( do_FHCAL )
//	{
//	  ostringstream fname_cluster_fhcal;
//	  fname_cluster_fhcal.str("");
//	  fname_cluster_fhcal << "ClusterAna_FHCAL" << "_p_"<< ppmin << "_" << ppmax << "_GeV"
//			     << "_eta_"  << petamin << "_" << petamax <<  "_" << "phi_" << phimin << "_" << phimax << "_" << nEvents << ".root" ;
//
//	  G4RawClusterAnalysis* clusterAna_FHCAL = new G4RawClusterAnalysis( "ClusterAna_FHCAL" , fname_cluster_fhcal.str().c_str() );
//	  clusterAna_FHCAL->AddClusterNode("CLUSTER_FHCAL");
//	  se->registerSubsystem(clusterAna_FHCAL);
//	}
//    }


  //--------------
  // IO management
  //--------------
  // for single particle generators we just need something which drives
  // the event loop, the Dummy Input Mgr does just that
  Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
  se->registerInputManager(in);

  // DST Reader
  if (do_DSTReader)
    {

      // save a comprehensive  evaluation file
      PHG4DSTReader* ana = new PHG4DSTReader(string(outputFile) + string("_DSTReader.root"));
      ana->set_save_particle(true);
      ana->set_load_all_particle(false);
      ana->set_load_active_particle(false);
      ana->set_save_vertex(false);

      if ( do_EEMC )
	{
	ana->AddNode("EEMC");
	ana->AddTower("SIM_EEMC");
	ana->AddTower("RAW_EEMC");
	ana->AddTower("CALIB_EEMC");
	}

      if ( do_FEMC )
	{
	ana->AddNode("FEMC");
	ana->AddTower("SIM_FEMC");
	ana->AddTower("RAW_FEMC");
	ana->AddTower("CALIB_FEMC");
	}

      if ( do_FHCAL )
	{
	ana->AddNode("FHCAL");
	ana->AddTower("SIM_FHCAL");
	ana->AddTower("RAW_FHCAL");
	ana->AddTower("CALIB_FHCAL");
	}

      se->registerSubsystem(ana);
//
//      //...
//      //Convert DST to human command readable TTree for quick poke around the outputs
//      gROOT->LoadMacro("G4_DSTReader.C");
//
//      G4DSTreader( outputFile, //
//          /*int*/ absorberactive ,
//          /*bool*/ do_svtx ,
//          /*bool*/ do_preshower ,
//          /*bool*/ do_cemc ,
//          /*bool*/ do_hcalin ,
//          /*bool*/ do_magnet ,
//          /*bool*/ do_hcalout ,
//          /*bool*/ do_cemc_twr ,
//          /*bool*/ do_hcalin_twr ,
//          /*bool*/ do_magnet  ,
//          /*bool*/ do_hcalout_twr
//          );

    }

  //-----------------
  // Event processing
  //-----------------
  if (nEvents <= 1)
    {
      PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
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
