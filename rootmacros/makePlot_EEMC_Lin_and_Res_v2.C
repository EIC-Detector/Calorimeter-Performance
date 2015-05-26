#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TRandom.h"
#include "TMath.h"

/**
 * \file ${file_name}
 * \brief Macro to generate calorimeter linearity and resolution plots
 * \author Thomas Krahulik <thomas.karhulik@stonybrook.edu>
 */

int makePlot_EEMC_Lin_and_Res_v2()
{
  /*----------------Basic Configuration-------------------*/

  vector< float > v_pbeam;
  vector< float > v_pberr;
  vector< TString > v_fnames;

  v_pbeam.push_back(  5 );
  v_pbeam.push_back( 10 );
  v_pbeam.push_back( 15 );
  v_pbeam.push_back( 20 );
  v_pbeam.push_back( 25 );
  //v_pbeam.push_back( 30 );

  v_pberr.push_back( 0 );
  v_pberr.push_back( 0 );
  v_pberr.push_back( 0 );
  v_pberr.push_back( 0 );
  v_pberr.push_back( 0 );
  //v_pberr.push_back( 0 );

  v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Digi_EEMC_p05.root" );
  v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Digi_EEMC_p10.root" );
  v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Digi_EEMC_p15.root" );
  v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Digi_EEMC_p20.root" );
  v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Digi_EEMC_p25.root" );
  //v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Digi_EEMC_p30.root" );
    
  //v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Raw_EEMC_p05.root" );
  //v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Raw_EEMC_p10.root" );
  //v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Raw_EEMC_p15.root" );
  //v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Raw_EEMC_p20.root" );
  //v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Raw_EEMC_p25.root" );
  //v_fnames.push_back( "/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Tower_Raw_EEMC_p30.root" );

  /*----------------Get Input Histograms-------------------*/

  vector< TH1F* > v_h_esum;

  TFile *f_in = NULL;
  for ( unsigned i = 0; i < v_fnames.size(); i++ )
    {

      f_in = new TFile( v_fnames.at(i), "OPEN" );
      v_h_esum.push_back( (TH1F*)f_in->Get("h_esum") );
      v_h_esum.at( i )->Print();
      
    }

  /*------------------------------------------------*/

  /*----------------Modify Histograms-------------------*/

  for ( unsigned i = 0; i < v_h_esum.size(); i++ )
    {
      v_h_esum.at( i )->Rebin( 250 );
    }

  /*----------Convert Digitized Photons to Energy--------------*/
  /*  
  vector< TH1F* > v_h_esum_v2;

  float NperMeV = 200;
  float EtoMeV = 1000;

  for ( unsigned i = 0; i < v_h_esum.size(); i++ )
    {
      TH1F* h_raw = v_h_esum.at( i );
      TH1F* h_digi = (TH1F*)h_raw->Clone("h_digi");
      h_digi->Reset();

	for ( int idx1 = 0; idx1 < h_raw->GetNbinsX(); idx1++ )
	  {
	    float eraw_i = h_raw->GetXaxis()->GetBinCenter( idx1 );
	    float craw_i = h_raw->GetBinContent( idx1 );

	    for ( int idx2 = 0; idx2 < craw_i; idx2++ )
	      {
		h_digi->Fill( eraw_i / (NperMeV * EtoMeV ) );
	      }
	  }
      v_h_esum_v2.push_back ( h_digi );
      }
  /*----------------AdHoc Digitization-------------------*/
  /*  
  vector< TH1F* > v_h_esum_v2;

  TRandom *rand = new TRandom();

  float NperMeV = 200;
  float EtoMeV = 1000;

  for ( unsigned i = 0; i < v_h_esum.size(); i++ )
    {

      TH1F* h_raw = v_h_esum.at( i );
      TH1F* h_digi = (TH1F*)h_raw->Clone("h_digi");
      h_digi->Reset();

      for ( int idx1 = 0; idx1 < h_raw->GetNbinsX(); idx1++ )
	{
	  float eraw_i = h_raw->GetXaxis()->GetBinCenter( idx1 );
	  float craw_i = h_raw->GetBinContent( idx1 );

	  for ( int idx2 = 0; idx2 < craw_i; idx2++ )
	    {
	      h_digi->Fill( rand->Poisson( NperMeV * EtoMeV * eraw_i ) / ( NperMeV * EtoMeV ) );
	    }
	}

      v_h_esum_v2.push_back( h_digi );
  
    }
  
  */
  /*----------------Gaussian Fits-------------------*/

  vector< float > v_gauss_mean;
  vector< float > v_gauss_merr;
  vector< float > v_gauss_sigm;
  vector< float > v_gauss_serr;

    for ( unsigned i = 0; i < v_h_esum.size(); i++ )
    //for ( unsigned i = 0; i < v_h_esum_v2.size(); i++ )
    {
      TCanvas *cfit = new TCanvas();
      cfit->cd();
      
      //without AdHoc Digitization
               v_h_esum.at( i )->Fit("gaus","","");
	    
	    v_gauss_mean.push_back( v_h_esum.at( i )->GetFunction("gaus")->GetParameter( 1 )/200 );
	    v_gauss_merr.push_back( v_h_esum.at( i )->GetFunction("gaus")->GetParError(1)/200);
	    v_gauss_sigm.push_back( v_h_esum.at( i )->GetFunction("gaus")->GetParameter( 2 ) / v_h_esum.at( i )->GetFunction("gaus")->GetParameter( 1 ) );

	    v_gauss_serr.push_back( v_gauss_sigm.at( i )*(sqrt(((v_gauss_merr.at(i))/(v_gauss_mean.at(i))) + ((v_h_esum.at( i )->GetFunction("gaus")->GetParError(2))/(v_h_esum.at( i )->GetFunction("gaus")->GetParameter(2))))));

	    //v_gauss_sigm.push_back( v_h_esum.at( i )->GetFunction("gaus")->GetParameter( 2 ) / v_pbeam.at( i ));
	  
	    /*          
	    //with AdHoc Digitization
	    
	    v_h_esum_v2.at( i )->Fit("gaus","","");

	    v_gauss_mean.push_back( v_h_esum_v2.at( i )->GetFunction("gaus")->GetParameter( 1 ) );
	    v_gauss_merr.push_back( v_h_esum_v2.at( i )->GetFunction("gaus")->GetParError(1));
      
	    //v_gauss_sigm.push_back( v_h_esum_v2.at( i )->GetFunction("gaus")->GetParameter( 2 ) / v_pbeam.at( i ));

	    v_gauss_sigm.push_back( v_h_esum_v2.at( i )->GetFunction("gaus")->GetParameter( 2 ) / v_h_esum_v2.at( i )->GetFunction("gaus")->GetParameter( 1 ) );

	    //v_gauss_serr.push_back( v_h_esum_v2.at(i)->GetFunction("gaus")->GetParError(2));

            v_gauss_serr.push_back( v_gauss_sigm.at( i )*(sqrt(((v_gauss_merr.at(i))/(v_gauss_mean.at(i))) + ((v_h_esum_v2.at( i )->GetFunction("gaus")->GetParError(2))/(v_h_esum_v2.at( i )->GetFunction("gaus")->GetParameter(2))))));
	    */
	    
    }
	    
  /*-----------------------------------------------*/

  /*-------------------Linearity-------------------*/

    TGraphErrors *eemc_lin = new TGraphErrors(v_pbeam.size(), &(v_pbeam[0]), &(v_gauss_mean[0]), &(v_pberr[0]), &(v_gauss_merr[0]));
    //TGraph *eemc_lin = new TGraph ( v_pbeam.size(), &(v_pbeam[0]), &(v_gauss_mean[0]) );
  eemc_lin->SetTitle("");
  eemc_lin->GetXaxis()->SetTitle("p_{Beam} [GeV]");
  //eemc_lin->GetXaxis()->SetTitleSize(0.06);
  //eemc_lin->GetXaxis()->SetLabelSize(0.06);
  eemc_lin->GetYaxis()->SetTitle("E_{Dep} [GeV]");
  //eemc_lin->GetYaxis()->SetTitleSize(0.06);

  //TF1 *lin_fit = new TF1("lin_fit", "[0]*x+0", 0, 40);

  TCanvas *c_eemc_lin = new TCanvas();
  c_eemc_lin->cd();
  eemc_lin->SetMarkerStyle(20);
  eemc_lin->SetMarkerSize(0.7);
  eemc_lin->SetLineWidth(1);
  //c_eemc_lin->SetLabelSize(0.07, "X");
  //c_eemc_lin->SetLabelOffset(1);
  eemc_lin->Draw("AP");
  eemc_lin->Fit("pol1","","");

  
  c_eemc_lin->Print("Plots/EEMC_Lin_Digi_e-_100.eps");
  c_eemc_lin->Print("Plots/EEMC_Lin_Digi_e-_100.png");

  /*-----------------------------------------------*/

  /*----------------Resolution---------------------*/

  TGraphErrors *eemc_res = new TGraphErrors( v_pbeam.size(), &(v_pbeam[0]), &(v_gauss_sigm[0]), &(v_pberr[0]), &(v_gauss_serr[0]));
  //TGraph *eemc_res = new TGraph ( v_pbeam.size(), &(v_pbeam[0]), &(v_gauss_sigm[0]) );
  eemc_res->SetTitle("");
  eemc_res->GetXaxis()->SetTitle("p_{Beam} [GeV]");
  eemc_res->GetYaxis()->SetTitle("#sigma/E_{Dep}");

  TF1 *res_fit = new TF1("res_fit", "[0]*1/sqrt(x)", 0, 40);
  //TF1 *res_the = new TF1("res_the", "0.02/sqrt(x)", 0 , 40);

  TCanvas *c_eemc_res = new TCanvas();
  c_eemc_res->cd();
  eemc_res->SetMarkerStyle(20);
  eemc_res->SetMarkerSize(0.7);
  eemc_res->Draw("AP");
  eemc_res->Fit("res_fit","","");
  
  res_the->Draw("SAME");
  res_the->SetLineColor(kBlue);
  res_fit->Print();

  c_eemc_res->Print("Plots/EEMC_Res_Digi_e-_100.eps");
  c_eemc_res->Print("Plots/EEMC_Res_Digi_e-_100.png");

  return 0;
}
