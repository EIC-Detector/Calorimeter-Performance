/**
 * \file ${file_name}
 * \brief EEMC: Longitudinal Shower Profile Along z Axis
 * \author Thomas Krahulik <thomas.karhulik@stonybrook.edu>
 */


int makePlot_EEMC_LongProf(
		       const TString infile_e_p5 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p5_eta-2_detailed_100.root",

		       const TString infile_e_p10 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p10_eta-2_detailed_100.root",

		       const TString infile_e_p15 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p15_eta-2_detailed_100.root",

		       const TString infile_e_p20 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p20_eta-2_detailed_100.root",

		       const TString infile_e_p25 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p25_eta-2_detailed_100.root",

		       const TString infile_e_p30 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p30_eta-2_detailed_100.root"
)

{
  gStyle->SetOptStat(0);

  /*--------------------------------*/
  /* Get Input Files */
  TFile *f_in_e_p5 = new TFile( infile_e_p5, "OPEN");
  TTree* T_e_p5 = (TTree*)f_in_e_p5->Get("T");
  T_e_p5->Print();

  TFile *f_in_e_p10 = new TFile( infile_e_p10, "OPEN");
  TTree* T_e_p10 = (TTree*)f_in_e_p10->Get("T");
  T_e_p10->Print();

  TFile *f_in_e_p15 = new TFile( infile_e_p15, "OPEN");
  TTree* T_e_p15 = (TTree*)f_in_e_p15->Get("T");
  T_e_p15->Print();

  TFile *f_in_e_p20 = new TFile( infile_e_p20, "OPEN");
  TTree* T_e_p20 = (TTree*)f_in_e_p20->Get("T");
  T_e_p20->Print();

  TFile *f_in_e_p25 = new TFile( infile_e_p25, "OPEN");
  TTree* T_e_p25 = (TTree*)f_in_e_p25->Get("T");
  T_e_p25->Print();

  TFile *f_in_e_p30 = new TFile( infile_e_p30, "OPEN");
  TTree* T_e_p30 = (TTree*)f_in_e_p30->Get("T");
  T_e_p30->Print();


     /*  1.Shower Profiles of Electrons at Beam Energies of 5-30 GeV  */
  TH1F* hlprof_e_p30 = new TH1F("h_lprof_e_p30","",50,-5.0,40.0);
  hlprof_e_p30->GetXaxis()->SetTitle("Calorimter Depth [X_{0}]");
  hlprof_e_p30->GetYaxis()->SetTitle("E_{vis} / event [GeV]");


  TH1F* hlprof_e_p10 = hlprof_e_p30->Clone("h_lprof_e_p10");
  //TH1F* hlprof_e_p15 = hlprof_e_p30->Clone("h_lprof_e_p15");
  TH1F* hlprof_e_p20 = hlprof_e_p30->Clone("h_lprof_e_p20");
  //TH1F* hlprof_e_p25 = hlprof_e_p30->Clone("h_lprof_e_p25");
  TH1F* hlprof_e_p5 = hlprof_e_p30->Clone("h_lprof_e_p5");


  /*Convert z Position from Centimeters to Radiation Length*/
  /*Radiation Length of Lead-Tungstate: 0.8903cm*/
  T_e_p30->Draw("((-1*G4HIT_EEMC.z[][0])-99.0)/0.8903 >> h_lprof_e_p30","G4HIT_EEMC.edep","goff");
  hlprof_e_p30->Scale( 1. / T_e_p30->GetEntries() );

   T_e_p10->Draw("((-1*G4HIT_EEMC.z[][0])-99.0)/0.8903 >> h_lprof_e_p10","G4HIT_EEMC.edep","goff");
  hlprof_e_p10->Scale( 1. / T_e_p10->GetEntries() );

  //T_e_p15->Draw("((-1*G4HIT_EEMC.z[][0])-99.0)/0.8903 >> h_lprof_e_p15","G4HIT_EEMC.edep","goff");
  //hlprof_e_p15->Scale( 1. / T_e_p15->GetEntries() );

 T_e_p20->Draw("((-1*G4HIT_EEMC.z[][0])-99.0)/0.8903 >> h_lprof_e_p20","G4HIT_EEMC.edep","goff");
  hlprof_e_p20->Scale( 1. / T_e_p20->GetEntries() );

  //T_e_p25->Draw("((-1*G4HIT_EEMC.z[][0])-99.0)/0.8903 >> h_lprof_e_p25","G4HIT_EEMC.edep","goff");
  //hlprof_e_p25->Scale( 1. / T_e_p25->GetEntries() );

 T_e_p5->Draw("((-1*G4HIT_EEMC.z[][0])-99.0)/0.8903 >> h_lprof_e_p5","G4HIT_EEMC.edep","goff");
  hlprof_e_p5->Scale( 1. / T_e_p5->GetEntries() );



  hlprof_e_p5->SetLineColor(kBlack);
  hlprof_e_p5->SetMarkerColor(kBlack);

  hlprof_e_p10->SetLineColor(kBlue);
  hlprof_e_p10->SetMarkerColor(kBlue);

  //hlprof_e_p15->SetLineColor(kGreen);
  //hlprof_e_p15->SetMarkerColor(kGreen);

  hlprof_e_p20->SetLineColor(kRed);
  hlprof_e_p20->SetMarkerColor(kRed);

  //hlprof_e_p25->SetLineColor(kOrange);
  //hlprof_e_p25->SetMarkerColor(kOrange);

  hlprof_e_p30->SetLineColor(kGreen);
  hlprof_e_p30->SetMarkerColor(kGreen);
  
  /*
  TF1 *lprof_fit = new TF1("lprof_fit","[0]*(x**[1])*(exp(-[2]*x))",0,50);
  lprof_fit->SetParameter(0,0.14);
  lprof_fit->SetParameter(1,3);
  lprof_fit->SetParameter(2,0.4);
  */

  TLegend *leg_lprof = new TLegend(0.50,0.7,0.75,0.9);
  leg_lprof->SetNColumns(1);
  leg_lprof->SetHeader( "Shower Profiles" );
  leg_lprof->AddEntry( hlprof_e_p5 , "5 GeV" , "l");
  leg_lprof->AddEntry( hlprof_e_p10, "10 GeV", "l");
  leg_lprof->AddEntry( hlprof_e_p20, "20 GeV", "l");
  leg_lprof->AddEntry( hlprof_e_p30, "30 GeV", "l");

  TCanvas *cprof1_p30 = new TCanvas();
  hlprof_e_p30->Draw("");
  hlprof_e_p10->Draw("sames");
  //hlprof_e_p15->Draw("sames");
  hlprof_e_p20->Draw("sames");
  //hlprof_e_p25->Draw("sames");
  hlprof_e_p5->Draw("sames");
  //hlprof_e_p30->Fit("lprof_fit","","");

  leg_lprof->Draw();

  gPad->RedrawAxis();

  cprof1_p30->Print("Plots/EEMC_LongProf_e-.eps");
  cprof1_p30->Print("Plots/EEMC_LongProf_e-.png");

  cout << "Integral of Longitudinal Shower Profile:" << hlprof_e_p30->Integral() << "GeV" << endl;

  return(0);
}
