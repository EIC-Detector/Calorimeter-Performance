int Min_eecal_layer = 0;
int Max_eecal_layer = 60;

void eECAL_Init()
{
  Min_eecal_layer = 0;
  Max_eecal_layer = 60;
}

/* Electron-going Crystal Calorimeter (ECAL)
 * longitudinal segmentation just for creatinn of 'longitudinal shower profiles'- no 'physical' barrier!
 * zpos is starting position in z
 */
double G4Detector_Cone_eECAL(PHG4Reco* g4Reco, double zpos, const double etamin, const double etamax, const double thickness)
{
  double layerwidth = thickness/(Max_eecal_layer-Min_eecal_layer);
  double pos = zpos - layerwidth/2;
  PHG4ConeSubsystem* eecal;

  for (int i = Min_eecal_layer; i <  Max_eecal_layer; i++)
    {
      eecal = new PHG4ConeSubsystem("EECAL", i);
      eecal->SetPlaceZ(pos);
      eecal->SetMaterial("G4_PbWO4");
      eecal->SetZlength(layerwidth/2);
      eecal->Set_eta_range(etamin, etamax);
      eecal->SetActive(true);
      eecal->SuperDetector("EECAL");
      g4Reco->registerSubsystem( eecal );
      pos -= (layerwidth);
    }

  return 0;
}
