/**
 * \file ${file_name}
 * \brief Macro to generate calorimeter linearity and resolution plots
 * \author Thomas Krahulik <thomas.karhulik@stonybrook.edu>
 *
 * makePlot_EEMC_Sims: Fills histograms of energy deposition per event using
 * information stored in Tree. Uses histograms of several varying initial
 * momenta to plot linearity and resolution of EEMC.
 */

int makePlot_EEMC_Sims(
		       const TString infile_e_p5 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p5_eta-2_detailed_100.root",

		       const TString infile_e_p10 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p10_eta-2_detailed_100.root",

		       const TString infile_e_p15 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p15_eta-2_detailed_100.root",

		       const TString infile_e_p20 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p20_eta-2_detailed_100.root",

		       const TString infile_e_p25 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p25_eta-2_detailed_100.root",

		       const TString infile_e_p30 = "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/G4ePHENIX_EEMC_e-_p30_eta-2_detailed_100.root"
)

{
  //gStyle->SetOptStat(0);

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

  /*--------------------------------*/

  /* 1. Deposited Energy Per Event */

  /* -------- Electron at p = 5 GeV--------*/
  
  TH1F* h_esum1_e_p5 = new TH1F("h_esum1_e_p5","", 100, -0.5, 99.5);
  h_esum1_e_p5->GetXaxis()-> SetTitle("Event #");
  h_esum1_e_p5->GetYaxis()->SetTitle("Sigma E_{vis} [GeV]");

  TCanvas *c_esum1_e_p5 = new TCanvas();
  h_esum1_e_p5->Draw();

  /*Fill Histogram With Energy Sum per Event*/
  TH1F* h_esum2_e_p5 = new TH1F("h_esum2_e_p5", "", 102, -0.05, 10**7);
  h_esum2_e_p5->GetXaxis()->SetTitle("#Sigma E_{vis} [GeV]");
  h_esum2_e_p5->GetYaxis()->SetTitle(" # events");

  for ( int i=0; i < h_esum1_e_p5->GetNbinsX(); i++)
    h_esum2_e_p5->Fill(h_esum1_e_p5->GetBinContent( i ));

  h_esum2_e_p5->SetLineColor(kBlue);

  TLegend *leg_esum2_p5 = new TLegend(0.45,0.7,0.75,0.9);
  leg_esum2_p5->SetNColumns(1);
  leg_esum2_p5->SetHeader("5GeV");
  leg_esum2_p5->AddEntry( h_esum2_e_p5 , "e^{-}" , "l" );

  TCanvas *c_esum2_p5 = new TCanvas();
  h_esum2_e_p5->Draw();
  h_esum2_e_p5->Fit("gaus","","");

  leg_esum2_p5->Draw();
  gPad->RedrawAxis();

  c_esum2_p5->SetLogy();

  //c_esum2_p5->Print("Plots/EEMC_Evis_e-.eps");
  //c_esum2_p5->Print("Plots/EEMC_Evis_e-.png");

   /* -------- Electron at p = 10 GeV--------*/
  
  TH1F* h_esum1_e_p10 = new TH1F("h_esum1_e_p10","", 100, -0.5, 99.5);
  h_esum1_e_p10->GetXaxis()-> SetTitle("Event #");
  h_esum1_e_p10->GetYaxis()->SetTitle("Sigma E_{vis} [GeV]");

  TCanvas *c_esum1_e_p10 = new TCanvas();
  h_esum1_e_p10->Draw();

  /*Fill Histogram With Energy Sum per Event*/
  TH1F* h_esum2_e_p10 = new TH1F("h_esum2_e_p10", "", 102, -0.05, 10**7);
  h_esum2_e_p10->GetXaxis()->SetTitle("#Sigma E_{vis} [GeV]");
  h_esum2_e_p10->GetYaxis()->SetTitle(" # events");

  for ( int i=0; i < h_esum1_e_p10->GetNbinsX(); i++)
    h_esum2_e_p10->Fill(h_esum1_e_p10->GetBinContent( i ));

  h_esum2_e_p10->SetLineColor(kBlue);

  TLegend *leg_esum2_p10 = new TLegend(0.45,0.7,0.75,0.9);
  leg_esum2_p10->SetNColumns(1);
  leg_esum2_p10->SetHeader("10GeV");
  leg_esum2_p10->AddEntry( h_esum2_e_p5 , "e^{-}" , "l" );

  TCanvas *c_esum2_p10 = new TCanvas();
  h_esum2_e_p10->Draw();
  h_esum2_e_p10->Fit("gaus","","");

  leg_esum2_p10->Draw();
  gPad->RedrawAxis();

  c_esum2_p10->SetLogy();

  //c_esum2_p10->Print("Plots/EEMC_Evis_e-.eps");
  //c_esum2_p510>Print("Plots/EEMC_Evis_e-.png");

   /* -------- Electron at p = 15 GeV--------*/
  
  TH1F* h_esum1_e_p15 = new TH1F("h_esum1_e_p15","", 100, -0.5, 99.5);
  h_esum1_e_p15->GetXaxis()-> SetTitle("Event #");
  h_esum1_e_p15->GetYaxis()->SetTitle("Sigma E_{vis} [GeV]");

  TCanvas *c_esum1_e_p15 = new TCanvas();
  h_esum1_e_p15->Draw();

  /*Fill Histogram With Energy Sum per Event*/
  TH1F* h_esum2_e_p15 = new TH1F("h_esum2_e_p15", "", 102, -0.05, 10**7);
  h_esum2_e_p15->GetXaxis()->SetTitle("#Sigma E_{vis} [GeV]");
  h_esum2_e_p15->GetYaxis()->SetTitle(" # events");

  for ( int i=0; i < h_esum1_e_p15->GetNbinsX(); i++)
    h_esum2_e_p15->Fill(h_esum1_e_p15->GetBinContent( i ));

  h_esum2_e_p15->SetLineColor(kBlue);

  TLegend *leg_esum2_p15 = new TLegend(0.45,0.7,0.75,0.9);
  leg_esum2_p15->SetNColumns(1);
  leg_esum2_p15->SetHeader("15GeV");
  leg_esum2_p15->AddEntry( h_esum2_e_p15 , "e^{-}" , "l" );

  TCanvas *c_esum2_p15 = new TCanvas();
  h_esum2_e_p15->Draw();
  h_esum2_e_p15->Fit("gaus","","");

  leg_esum2_p15->Draw();
  gPad->RedrawAxis();

  c_esum2_p15->SetLogy();

  //c_esum2_p15->Print("Plots/EEMC_Evis_e-.eps");
  //c_esum2_p15->Print("Plots/EEMC_Evis_e-.png");

   /* -------- Electron at p = 20 GeV--------*/
  
  TH1F* h_esum1_e_p20 = new TH1F("h_esum1_e_p20","", 100, -0.5,99.5 );
  h_esum1_e_p20->GetXaxis()-> SetTitle("Event #");
  h_esum1_e_p20->GetYaxis()->SetTitle("Sigma E_{vis} [GeV]");

  TCanvas *c_esum1_e_p20 = new TCanvas();
  h_esum1_e_p20->Draw();

  /*Fill Histogram With Energy Sum per Event*/
  TH1F* h_esum2_e_p20 = new TH1F("h_esum2_e_p20", "", 102, -0.05, 10**7);
  h_esum2_e_p20->GetXaxis()->SetTitle("#Sigma E_{vis} [GeV]");
  h_esum2_e_p20->GetYaxis()->SetTitle(" # events");

  for ( int i=0; i < h_esum1_e_p20->GetNbinsX(); i++)
    h_esum2_e_p20->Fill(h_esum1_e_p20->GetBinContent( i ));

  h_esum2_e_p20->SetLineColor(kBlue);

  TLegend *leg_esum2_p20 = new TLegend(0.45,0.7,0.75,0.9);
  leg_esum2_p20->SetNColumns(1);
  leg_esum2_p20->SetHeader("20GeV");
  leg_esum2_p20->AddEntry( h_esum2_e_p20 , "e^{-}" , "l" );

  TCanvas *c_esum2_p20 = new TCanvas();
  h_esum2_e_p20->Draw();
  h_esum2_e_p20->Fit("gaus","","");

  leg_esum2_p20->Draw();
  gPad->RedrawAxis();

  c_esum2_p20->SetLogy();

  //c_esum2_p20->Print("Plots/EEMC_Evis_e-.eps");
  //c_esum2_p20->Print("Plots/EEMC_Evis_e-.png");

   /* -------- Electron at p = 25 GeV--------*/
  
  TH1F* h_esum1_e_p25 = new TH1F("h_esum1_e_p25","", 100, -0.5, 99.5);
  h_esum1_e_p25->GetXaxis()-> SetTitle("Event #");
  h_esum1_e_p25->GetYaxis()->SetTitle("Sigma E_{vis} [GeV]");

  TCanvas *c_esum1_e_p25 = new TCanvas();
  h_esum1_e_p25->Draw();

  /*Fill Histogram With Energy Sum per Event*/
  TH1F* h_esum2_e_p25 = new TH1F("h_esum2_e_p25", "", 102, -0.05, 10**7);
  h_esum2_e_p25->GetXaxis()->SetTitle("#Sigma E_{vis} [GeV]");
  h_esum2_e_p25->GetYaxis()->SetTitle(" # events");

  for ( int i=0; i < h_esum1_e_p25->GetNbinsX(); i++)
    h_esum2_e_p25->Fill(h_esum1_e_p25->GetBinContent( i ));

  h_esum2_e_p25->SetLineColor(kBlue);

  TLegend *leg_esum2_p25 = new TLegend(0.45,0.7,0.75,0.9);
  leg_esum2_p25->SetNColumns(1);
  leg_esum2_p25->SetHeader("25GeV");
  leg_esum2_p25->AddEntry( h_esum2_e_p25 , "e^{-}" , "l" );

  TCanvas *c_esum2_p25 = new TCanvas();
  h_esum2_e_p25->Draw();
  h_esum2_e_p25->Fit("gaus","","");

  leg_esum2_p25->Draw();
  gPad->RedrawAxis();

  c_esum2_p25->SetLogy();

  //c_esum2_p25->Print("Plots/EEMC_Evis_e-.eps");
  //c_esum2_p25->Print("Plots/EEMC_Evis_e-.png");

   /* -------- Electron at p = 30 GeV--------*/
  
  TH1F* h_esum1_e_p30 = new TH1F("h_esum1_e_p30","", 100, -0.5, 99.5);
  h_esum1_e_p30->GetXaxis()-> SetTitle("Event #");
  h_esum1_e_p30->GetYaxis()->SetTitle("Sigma E_{vis} [GeV]");

  TCanvas *c_esum1_e_p30 = new TCanvas();
  h_esum1_e_p30->Draw();

  /*Fill Histogram With Energy Sum per Event*/
  TH1F* h_esum2_e_p30 = new TH1F("h_esum2_e_p30", "", 102, -0.05, 10**7);
  h_esum2_e_p30->GetXaxis()->SetTitle("#Sigma E_{vis} [GeV]");
  h_esum2_e_p30->GetYaxis()->SetTitle(" # events");

  for ( int i=0; i < h_esum1_e_p30->GetNbinsX(); i++)
    h_esum2_e_p30->Fill(h_esum1_e_p30->GetBinContent( i ));

  h_esum2_e_p30->SetLineColor(kBlue);

  TLegend *leg_esum2_p30 = new TLegend(0.45,0.7,0.75,0.9);
  leg_esum2_p30->SetNColumns(1);
  leg_esum2_p30->SetHeader("30GeV");
  leg_esum2_p30->AddEntry( h_esum2_e_p30 , "e^{-}" , "l" );

  TCanvas *c_esum2_p30 = new TCanvas();
  h_esum2_e_p30->Draw();
  h_esum2_e_p30->Fit("gaus","","");

  leg_esum2_p30->Draw();
  gPad->RedrawAxis();

  c_esum2_p30->SetLogy();

  //c_esum2_p30->Print("Plots/EEMC_Evis_e-.eps");
  //c_esum2_p30->Print("Plots/EEMC_Evis_e-.png");

  /*------------------------------------*/

  /*-----------2. Linearity-------------*/
  TF1 *gauss_p5 = h_esum2_e_p5->GetFunction("gaus");
  TF1 *gauss_p10 = h_esum2_e_p10->GetFunction("gaus");
  TF1 *gauss_p15 = h_esum2_e_p15->GetFunction("gaus");
  TF1 *gauss_p20 = h_esum2_e_p20->GetFunction("gaus"); 
  TF1 *gauss_p25 = h_esum2_e_p25->GetFunction("gaus");
  TF1 *gauss_p30 = h_esum2_e_p30->GetFunction("gaus");

  int n = 6;
  Float_t lin_x [] = {5,10,15,20,25,30};
  Float_t lin_y [] = {gauss_p5->GetParameter(1),gauss_p10->GetParameter(1),gauss_p15->GetParameter(1),gauss_p20->GetParameter(1),gauss_p25->GetParameter(1),gauss_p30->GetParameter(1)};
  Float_t lin_x_err [] = {0,0,0,0,0,0};
  Float_t lin_y_err [] = {gauss_p5->GetParError(1),gauss_p10->GetParError(1),gauss_p15->GetParError(1),gauss_p20->GetParError(1),gauss_p25->GetParError(1),gauss_p30->GetParError(1)};

  TGraphErrors *eemc_lin = new TGraphErrors(n,lin_x,lin_y,lin_x_err,lin_y_err);
  eemc_lin->SetTitle("");
  eemc_lin->GetXaxis()->SetTitle("p_{Beam} [GeV]");
  eemc_lin->GetYaxis()->SetTitle("E_{Dep} [GeV]");
 
  TF1 *lin_fit = new TF1("lin_fit","[0]+[1]*x",0,30);
  lin_fit->SetParameter(0,0);

  TCanvas *c_eemc_lin = new TCanvas ();
  eemc_lin->SetMarkerStyle(20);
  eemc_lin->SetMarkerSize(0.7);
  eemc_lin->SetLineWidth(1);
  eemc_lin->Draw("AP");
  eemc_lin->Fit("lin_fit","","");

  c_eemc_lin->Print("Plots/EEMC_Lin_e-.eps");
  c_eemc_lin->Print("Plots/EEMC_Lin_e-.png");

  /*-------------------------------------*/
  
  /*---------- 3. Resolution ------------*/

  int m =6;
  Float_t res_x [] = {5,10,15,20,25,30};
  Float_t res_y [] = {(gauss_p5->GetParameter(2))/(gauss_p5->GetParameter(1)),(gauss_p10->GetParameter(2))/(gauss_p10->GetParameter(1)),(gauss_p15->GetParameter(2))/(gauss_p15->GetParameter(1)),(gauss_p20->GetParameter(2))/(gauss_p20->GetParameter(1)),(gauss_p25->GetParameter(2))/(gauss_p25->GetParameter(1)),(gauss_p30->GetParameter(2))/(gauss_p30->GetParameter(1))};

  TGraph *eemc_res = new TGraph(m,res_x,res_y);
  eemc_res->SetTitle("");
  eemc_res->GetXaxis()->SetTitle("p_{Beam} [GeV]");
  eemc_res->GetYaxis()->SetTitle("#sigma/E_{Dep}");

  TF1 *res_fit = new TF1("res_fit","[0]*1/sqrt(x)",0,40);
  
  TCanvas *c_eemc_res = new TCanvas ();
  eemc_res->SetMarkerStyle(20);
  eemc_res->SetMarkerSize(0.7);
  eemc_res->Draw("AP");
  eemc_res->Fit("res_fit","","");

  c_eemc_res->Print("Plots/EEMC_Res_e-.eps");
  c_eemc_res->Print("Plots/EEMC_Res_e-.png");

  /*-------------------------------------*/

  return 0;
}
