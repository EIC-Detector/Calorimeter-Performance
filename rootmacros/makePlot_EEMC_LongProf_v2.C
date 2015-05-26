#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"

/**
 * \file ${file_name}
 * \brief Macro to generate calorimeter longitudinal profile
 * \author Thomas Krahulik <thomas.karhulik@stonybrook.edu>
 */

int makePlot_EEMC_LongProf_v2()

{

  vector< TString > v_fnames;

  v_fnames.push_back("/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Shower_EEMC_p05.root");
  v_fnames.push_back("/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Shower_EEMC_p10.root");
  v_fnames.push_back("/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Shower_EEMC_p15.root");
  v_fnames.push_back("/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Shower_EEMC_p20.root");
  v_fnames.push_back("/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Shower_EEMC_p25.root");
  v_fnames.push_back("/direct/phenix+u/tkrahul/EIC_tkrahulik_work/Analysis/G4ePHENIX_EEMC_Simulations/RootSimulations/Lin_and_Res_Tests/G4Shower_EEMC_p30.root");
 
  /*------------------------Get Input Histograms---------------------------*/

  vector< TH1F* > v_l_prof;

  TFile *f_in = NULL;
  for ( unsigned i = 0; i < v_fnames.size(); i++)
    {
      f_in = new TFile( v_fnames.at(i), "OPEN" );
      v_l_prof.push_back( (TH1F*)f_in->Get("h_lprof") );
      //v_l_prof.at(i)->GetEntries()->Print() ;
      v_l_prof.at(i)->Print();
    }

  /*-----------------Convert z-axiz to Radiation Length--------------------*/

  vector< TH1F* > v_l_prof_v2;

  for ( unsigned i = 0; i < v_l_prof.size(); i++)
    { 
      TH1F* h_raw = v_l_prof.at( i );
      TH1F* h_digi = (TH1F*)h_raw->Clone("h_digi");
      h_digi->Reset();

      for ( int idx1 = 0; idx1 < h_raw->GetNbinsX(); idx1++)
	{
	  float lraw_i = h_raw->GetXaxis()->GetBinCenter( idx1 );
	  float craw_i = h_raw->GetBinContent( idx1 );
	  for ( int idx2 = 0; idx2 < craw_i; idx2++ )
	    {
	      //h_digi->Fill(lraw_i);
	      h_digi->Fill((lraw_i)/0.8903);
	    }
	    }
      v_l_prof_v2.push_back ( h_con );
    }

  /*-------------------------Create Shower Fit-----------------------------*/

  TF1 *f_lprof_fit = new TF1("f_lprof_fit", "[0]*(x**[1])*(exp(-[2]*x))", 0, 125);

  /*---------------------------Histogram Fits------------------------------*/

  vector< float > v_show_max;

  for ( unsigned i = 0; i < v_l_prof.size(); i++ )
    {
      TCanvas *cfit = new TCanvas();
      cfit->cd();
      v_l_prof_v2.at( i )->Draw();
      //v_l_prof_v2.at( i )->GetXaxis()->SetLimits(-50,125);
      v_l_prof_v2.at( i )->Fit("f_lprof_fit");

      //v_show_max.push_back( v_l_prof_v2.at( i )->GetFunction("f_lprof_fit")->GetParameter( 1 ));
      //std:: cout << v_show_max.at( i ) << endl;

    }
 
  //v_show_max->Print();

  return 0;
}
