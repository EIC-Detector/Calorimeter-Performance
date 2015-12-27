makeMapping_FHCAL()
{
  /* Global detector position / transformation */
  float hhcal_x0 =  0.0; // cm,
  float hhcal_y0 =  0.0; // cm,
  float hhcal_z0 = 400.0; // cm,

  float hhcal_rot_x0 =  0.0;
  float hhcal_rot_y0 =  0.0;
  float hhcal_rot_z0 =  0.0;

  /* Detector envelope size (conde shape) */
  float hhcal_rmin1 = 5.0; // cm
  float hhcal_rmax1 = 262.0; // cm
  float hhcal_rmin2 = 5.0; // cm
  float hhcal_rmax2 = 336.9; // cm
  float hhcal_dz = 100.0; // cm (FULL SIZE)

  /* Tower parameters */
  float tower_dx = 10.0; // cm
  float tower_dy = 10.0; // cm
  float tower_dz = 100.0; // cm (FULL SIZE)

  // assume center tower is centered at (0,0)
  // find index of 'theoretical' center tower on chessboard
  unsigned n_towers_j = 2 * ( (unsigned)( (hhcal_rmax1+0.5*tower_dx) / tower_dx) ) + 1;
  unsigned n_towers_k = n_towers_j;

  unsigned j_center = n_towers_j / 2 + 1;
  unsigned k_center = j_center;
 
  float xpos_j0_k0 = -1 * ( (float)( n_towers_j - 1 ) / 2 ) * tower_dx - tower_dx;;
  float ypos_j0_k0 = xpos_j0_k0;

  cout << "n_towers_j = " << n_towers_j << endl;
  cout << "n_towers_k = " << n_towers_k << endl;
  cout << "xpos_j0_k0 = " << xpos_j0_k0 << endl;
  cout << "ypos_j0_k0 = " << ypos_j0_k0 << endl;
  cout << "center tower j = " << j_center << " / k = " << k_center << " is at " << xpos_j0_k0 + j_center * tower_dx << " , " <<  ypos_j0_k0 + k_center * tower_dy << endl;

  // create map
  ofstream fout("towerMap_FHCAL_latest.txt");

  /* Global detector transformation */
  fout << "#Global detector geometry and transforamtion; lengths given in cm" << endl;
  fout << "Gtype " << 1 << endl;
  fout << "Gr1_inner " << hhcal_rmin1 << endl;
  fout << "Gr1_outer " << hhcal_rmax1 << endl;
  fout << "Gr2_inner " << hhcal_rmin2 << endl;
  fout << "Gr2_outer " << hhcal_rmax2 << endl;
  fout << "Gdz " << hhcal_dz << endl;
  fout << "Gx0 " << hhcal_x0 << endl;
  fout << "Gy0 " << hhcal_y0 << endl;
  fout << "Gz0 " << hhcal_z0 << endl;
  fout << "Grot_x " << hhcal_rot_x0 << endl;
  fout << "Grot_y " << hhcal_rot_y0 << endl;
  fout << "Grot_z " << hhcal_rot_z0 << endl;
  fout << "Gtower_dx " << tower_dx << endl;
  fout << "Gtower_dy " << tower_dy << endl;
  fout << "Gtower_dz " << tower_dz << endl;

  /* Tower mapping */
  fout << "#Tower type,idx_j,idx_k,idx_l,x[cm],y[cm],z[cm],dx[cm],dy[cm],dz[cm],rot_x,rot_y,rot_z" << endl;

  unsigned idx_l = 0;

  for (unsigned idx_j = 0; idx_j < n_towers_j; idx_j++)
    {
      for (unsigned idx_k = 0; idx_k < n_towers_k; idx_k++)
        {

	  // check if all four corners are within envelope volume
	  float xpos = xpos_j0_k0 + idx_j * tower_dx;
	  float ypos = ypos_j0_k0 + idx_k * tower_dy;
	  float zpos = 0;

	  float r_corner_1 = sqrt( pow( xpos + tower_dx/2. , 2 ) + pow( ypos + tower_dy/2. , 2 ) );
	  float r_corner_2 = sqrt( pow( xpos - tower_dx/2. , 2 ) + pow( ypos + tower_dy/2. , 2 ) );
	  float r_corner_3 = sqrt( pow( xpos + tower_dx/2. , 2 ) + pow( ypos - tower_dy/2. , 2 ) );
	  float r_corner_4 = sqrt( pow( xpos - tower_dx/2. , 2 ) + pow( ypos - tower_dy/2. , 2 ) );

	  if ( r_corner_1 > hhcal_rmax1 ||
	       r_corner_2 > hhcal_rmax1 ||
	       r_corner_3 > hhcal_rmax1 ||
	       r_corner_4 > hhcal_rmax1 )
	    continue;

	  if ( r_corner_1 < hhcal_rmin1 ||
	       r_corner_2 < hhcal_rmin1 ||
	       r_corner_3 < hhcal_rmin1 ||
	       r_corner_4 < hhcal_rmin1 )
	    continue;

	  if ( idx_j == j_center && idx_k == k_center )
	    continue;

	  //	  fout << idx_j << " " << idx_k << " " << idx_l << " " << xpos << " " << ypos << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0 0" << endl;

	  fout << "Tower " << 0 << " " << idx_j << " " << idx_k << " " << idx_l << " " << xpos << " " << ypos << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;

	}

    }

  fout.close();

}
