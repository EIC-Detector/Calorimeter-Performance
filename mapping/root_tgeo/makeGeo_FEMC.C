makeGeo_FEMC()
{
  // load library
  gSystem->Load("libGeom");

  // create geometry manager
  TGeoManager* gm = new TGeoManager("world", "the simplest geometry"); 

  // prepare material and medium
  TGeoMaterial* mat = new TGeoMaterial("Vacuum",0,0,0);
  TGeoMedium *med = new TGeoMedium("Vacuum",1,mat);

  // create volume
  TGeoVolume *top = gm->MakeBox("Top",med,500.,500.,500.);

  // make volume WORLD volume
  gm->SetTopVolume(top);

  // Close geometry
  //  gm->CloseGeometry();

  // show me the world
  top->SetLineColor(kMagenta);
  //  gm->SetTopVisible(); // top is generally invisible
  //  top->Draw();

  // The World Is Not Enough
  float femc_zcenter = 315; // cm
  float femc_dz = 17.0; // cm
  float femc_rmin1 = 12; // cm
  float femc_rmax1 = 225; // cm
  float femc_rmin2 = 12; // cm
  float femc_rmax2 = 246; // cm

  // make femc envelope
  TGeoVolume *femc_geo = gm->MakeCone( "femc", med, femc_dz/2., femc_rmin1, femc_rmax1, femc_rmin2, femc_rmax2 );

  TGeoTranslation femc_t(0,0,femc_zcenter);
  TGeoHMatrix *femc_tm = new TGeoHMatrix(femc_t);

  top->AddNode( femc_geo , 1 , femc_tm );

  // make femc tower
  float tower_dx = 0; //3.0; // cm
  float tower_dy = 0; //3.0; // cm
  float tower_dz = 0; //17.0; // cm

  // read tower geometry from text file
  unsigned idx_j = 0;
  unsigned idx_k = 0;
  unsigned idx_l = 0;
  float xpos = 0;
  float ypos = 0;
  float zpos = femc_zcenter;
  float dummy = 0;

  ifstream in;
  string line;
  in.open("towerMap_FEMC_latest.txt");

  int nlines = 0;

  TGeoVolume *tower_geo = NULL;

  while ( getline( in , line ) ) {

    if ( line.find("#") != string::npos )
      {
	cout << "WARNING Skipping line " << line << endl;
	continue;
      }

    stringstream(line) >> idx_j >> idx_k >> idx_l >> xpos >> ypos >> zpos >> tower_dx >> tower_dy >> tower_dz >> dummy >> dummy >> dummy >> dummy;
    if ( nlines < 5 ) printf("idx_j=%d, idx_k=%d, xpos=%f, ypos=%f\n, zpos=%f\n", idx_j, idx_k, xpos, ypos, zpos);
    nlines++;

    if ( !tower_geo )
      {
	tower_geo = gm->MakeBox("TowerX",med,tower_dx/2. , tower_dy/2. , tower_dz/2. ); 
	tower_geo->SetLineColor(kRed);
	tower_geo->SetFillColor(kBlue);
      }

    // zpos = 0 because centered within mother volume = FEMC envelope
    TGeoHMatrix *tower_tm = new TGeoHMatrix( TGeoTranslation( xpos, ypos, 0 ) );
    femc_geo->AddNode( tower_geo , 1 , tower_tm );
  }

  cout << "Total number of towers: " << nlines << endl;

  // visibility
  femc_geo->SetVisibility(true);
  femc_geo->SetVisLeaves(true);
  femc_geo->VisibleDaughters(true);
  femc_geo->SetLineColor(kRed);

  gm->SetVisOption(0);
  //  gm->SetTopVisible();
  gm->SetVisLevel(4);

  gm->CloseGeometry();
  top->Draw("ogl");

  // write output
  TFile *fout = new TFile("TGeo_FEMC.root","RECREATE");
  gm->Write();
  fout->Close();

}
