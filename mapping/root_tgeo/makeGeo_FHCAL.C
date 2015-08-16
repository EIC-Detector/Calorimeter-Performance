makeGeo_FHCAL()
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
  float hhcal_zcenter = 400; // cm
  float hhcal_dz = 100.0; // cm
  float hhcal_rmin1 = 5.0; // cm
  float hhcal_rmax1 = 262.0; // cm
  float hhcal_rmin2 = 5.0; // cm
  float hhcal_rmax2 = 336.9.0; // cm

  // make hHcal envelope
  TGeoVolume *hhcal_geo = gm->MakeCone( "hHcal", med, hhcal_dz/2., hhcal_rmin1, hhcal_rmax1, hhcal_rmin2, hhcal_rmax2 );

  TGeoTranslation hhcal_t(0,0,hhcal_zcenter);
  TGeoHMatrix *hhcal_tm = new TGeoHMatrix(hhcal_t);

  top->AddNode( hhcal_geo , 1 , hhcal_tm );

  // read tower geometry from text file
  unsigned idx_j = 0;
  unsigned idx_k = 0;
  unsigned idx_l = 0;
  float xpos = 0;
  float ypos = 0;
  float zpos = hhcal_zcenter;
  float tower_dx = 0; //10.0; // cm
  float tower_dy = 0; //10.0; // cm
  float tower_dz = 0; //100.0; // cm

  float dummy = 0;

  ifstream in;
  string line;
  in.open("towerMap_FHCAL_latest.txt");

  int nlines = 0;

  // make hHcal tower
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

    // IF this is the first tower, make tower
    if ( !tower_geo )
      {
	tower_geo = gm->MakeBox("TowerX",med,tower_dx/2. , tower_dy/2. , tower_dz/2. ); 
	tower_geo->SetLineColor(kRed);
	tower_geo->SetFillColor(kBlue);
      }

    // zpos = 0 because centered within mother volume = HCAL envelope
    TGeoHMatrix *tower_tm = new TGeoHMatrix( TGeoTranslation( xpos, ypos, 0 ) );
    hhcal_geo->AddNode( tower_geo , 1 , tower_tm );
  }

  cout << "Total number of towers: " << nlines << endl;

  // visibility
  hhcal_geo->SetVisibility(true);
  hhcal_geo->SetVisLeaves(true);
  hhcal_geo->VisibleDaughters(true);
  hhcal_geo->SetLineColor(kRed);

  gm->SetVisOption(0);
  //  gm->SetTopVisible();
  gm->SetVisLevel(4);

  gm->CloseGeometry();
  top->Draw("ogl");

  // write output
  TFile *fout = new TFile("TGeo_FHCAL.root","RECREATE");
  gm->Write();
  fout->Close();

}
