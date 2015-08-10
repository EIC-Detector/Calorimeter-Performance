makeGeo_EEMC()
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

  // show me the world
  top->SetLineColor(kMagenta);

  // The World Is Not Enough
  float eemc_z0 = 99.0; // cm,
  float eemc_dz = 18.0; // cm
  float eemc_rmin1 = 2.2; // cm
  float eemc_rmax1 = 65.6; // cm
  float eemc_rmin2 = 2.6; // cm
  float eemc_rmax2 = 77.5; // cm

  // make eemc envelope
  TGeoVolume *eemc_geo = gm->MakeCone( "eemc", med, eemc_dz/2., eemc_rmin1, eemc_rmax1, eemc_rmin2, eemc_rmax2 );

  TGeoTranslation eemc_t(0,0,400);
  TGeoHMatrix *eemc_tm = new TGeoHMatrix(eemc_t);

  top->AddNode( eemc_geo , 1 , eemc_tm );

  // make eemc tower
  float tower_dx = 10.0; // cm
  float tower_dy = 10.0; // cm
  float tower_dz = 100.0; // cm

  TGeoVolume *tower_geo = gm->MakeBox("TowerX",med,tower_dx/2. , tower_dy/2. , tower_dz/2. ); 
  tower_geo->SetLineColor(kRed);
  tower_geo->SetFillColor(kBlue);

  // read tower geometry from text file
  unsigned idx_j = 0;
  unsigned idx_k = 0;
  unsigned idx_l = 0;
  float xpos = 0;
  float ypos = 0;
  float zpos = 0;
  float dummy = 0;

  ifstream in;
  string line;
  in.open("towerMap_EEMC_latest.txt");

  int nlines = 0;

  while ( getline( in , line ) ) {

    if ( line.find("#") != string::npos )
      {
	cout << "WARNING Skipping line " << line << endl;
	continue;
      }

    stringstream(line) >> idx_j >> idx_k >> idx_l >> xpos >> ypos >> zpos >> dummy >> dummy >> dummy >> dummy;
    if ( nlines < 5 ) printf("idx_j=%d, idx_k=%d, xpos=%f, ypos=%f\n, zpos=%f\n", idx_j, idx_k, xpos, ypos, zpos);
    nlines++;

    TGeoHMatrix *tower_tm = new TGeoHMatrix( TGeoTranslation( xpos, ypos, zpos ) );
    eemc_geo->AddNode( tower_geo , 1 , tower_tm );
  }

  cout << "Total number of towers: " << nlines << endl;

  // visibility
  eemc_geo->SetVisibility(true);
  eemc_geo->SetVisLeaves(true);
  eemc_geo->VisibleDaughters(true);
  eemc_geo->SetLineColor(kRed);

  gm->SetVisOption(0);
  gm->SetVisLevel(4);

  gm->CloseGeometry();
  top->Draw("ogl");

  // write output
  TFile *fout = new TFile("TGeo_EEMC.root","RECREATE");
  gm->Write();
  fout->Close();

}
