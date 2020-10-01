#include "../include/cosmetics.h"
#include "../include/constants.h"
#include "../include/Tools.h"
#include <TString.h>
#include <iostream>
#include <TStyle.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TText.h>
#include <TPaveText.h>
#include <TGaxis.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TGraphAsymmErrors.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TROOT.h>
#include <TKey.h>
#include <TLatex.h>
#include <TClass.h>
#include <fstream>

using namespace std;

void cosmetics();

//channel = ech, much, comb
void AnalysisTool::PlotLimitsCombine(TString channel){
  /*
  ==========================================
  |                                          |
  |  handy script to make a nice limit plot  |
  |                                          |
  ==========================================   */


  //shortcut to modifications:
  bool with_data = false;
  cout << "Channel: " << channel << endl;

  //0) general cosmetics
  cosmetics();



  //2) Read in LQ cross sections
  TFile* f_vectortheory = new TFile("include/LQxsecs.root", "READ");
  TGraph* g_theory_vector_k1 = (TGraph*)f_vectortheory->Get("vlqxsec");
  TGraph* g_theory_vector_k1_up = (TGraph*)f_vectortheory->Get("vlqxsecUp");
  TGraph* g_theory_vector_k1_down = (TGraph*)f_vectortheory->Get("vlqxsecDown");
  TGraph* g_theory_vector_k0 = (TGraph*)f_vectortheory->Get("vlqxsec_k0");
  TGraph* g_theory_vector_k0_up = (TGraph*)f_vectortheory->Get("vlqxsecUp_k0");
  TGraph* g_theory_vector_k0_down = (TGraph*)f_vectortheory->Get("vlqxsecDown_k0");
  TFile* f_scalartheory = new TFile("include/LQxsecs_scalar.root", "READ");
  TGraph* g_theory = (TGraph*)f_scalartheory->Get("slqxsec");
  TGraph* g_theory_up = (TGraph*)f_scalartheory->Get("slqxsecUp");
  TGraph* g_theory_down = (TGraph*)f_scalartheory->Get("slqxsecDown");

  vector<double> x_theo = {};
  for(int i=0; i<g_theory->GetN(); i++){
    double x, y;
    g_theory->GetPoint(i, x, y);
    x_theo.emplace_back(x);
  }

  // Store cross sections to vectors (to find the mass limit later)
  vector<double> theory, theory_up, theory_down, theory_vector_k1, theory_vector_k1_up, theory_vector_k1_down, theory_vector_k0, theory_vector_k0_up, theory_vector_k0_down;
  for(unsigned int i=0; i<x_theo.size(); i++){
    theory.emplace_back(g_theory->Eval(x_theo[i]));
    theory_up.emplace_back(g_theory_up->Eval(x_theo[i]));
    theory_down.emplace_back(g_theory_down->Eval(x_theo[i]));
    theory_vector_k1.emplace_back(g_theory_vector_k1->Eval(x_theo[i]));
    theory_vector_k1_up.emplace_back(g_theory_vector_k1_up->Eval(x_theo[i]));
    theory_vector_k1_down.emplace_back(g_theory_vector_k1_down->Eval(x_theo[i]));
    theory_vector_k0.emplace_back(g_theory_vector_k0->Eval(x_theo[i]));
    theory_vector_k0_up.emplace_back(g_theory_vector_k0_up->Eval(x_theo[i]));
    theory_vector_k0_down.emplace_back(g_theory_vector_k0_down->Eval(x_theo[i]));
  }




  // Read in limits
  TString path = AnalysisTool::combine_path + "output/";

  // Get limit masspoints
  TString varname;
  if(channel == "ech") varname = "LQtoTE";
  else if(channel == "much") varname = "LQtoTMu";
  else if(channel == "comb") varname = "LQ_comb";

  TString txtname = path + "masspoints_" + varname + ".txt";
  ifstream myfile(txtname);

  int n_points = -1;
  vector<double> mass;
  myfile >> n_points;
  double val = -1.;
  while(!myfile.eof()){
    myfile >> val;
    mass.emplace_back(val);
  }
  myfile.clear();

  // Read limits from rootfiles
  vector<double> expected, expected_high_68, expected_high_95, expected_low_68, expected_low_95, observed;
  for(int i=0; i<n_points; i++){
    TString filename = path + "higgsCombine" + varname + ".AsymptoticLimits.mH";
    filename += mass[i];
    filename += ".root";
    TFile* infile = new TFile(filename, "READ");

    TTreeReader Reader("limit", infile);
    TTreeReaderValue<float> quantile(Reader, "quantileExpected");
    TTreeReaderValue<double> r(Reader, "limit");

    while (Reader.Next()) {
      float q = *quantile;
      double rr = *r;

      // Convert r values in cross section limits (multiply by cross section)


      /*if(mass[i] == 2000){
	rr *= 100.;
	rr *= g_theory->Eval(mass[i]);
	}
	else */ if(mass[i] != 200 && mass[i] != 300 && mass[i] != 400){
        rr *= g_theory->Eval(mass[i]);
      }
      else{ //200, 300 and 400 masspoints were scaled down by 10 when reading out histograms for combine
        rr /= 10.;
        rr *= g_theory->Eval(mass[i]);
      }

      if(q<0.) observed.emplace_back(rr);
      else if(q<0.1) expected_low_95.emplace_back(rr);
      else if(q<0.4) expected_low_68.emplace_back(rr);
      else if(q<0.8) expected.emplace_back(rr);
      else if(q<0.9) expected_high_68.emplace_back(rr);
      else           expected_high_95.emplace_back(rr);
    }
  }




  vector<double> observed_fine;
  double mass_low = -1, mass_high = -1;
  int idx_low = -1;
  // Interpolate linearly between mass points to match theory curve granularity
  for(unsigned int j=0; j<x_theo.size(); j++){
    double mymass;
    mymass = x_theo[j];
    bool match = false;
    for(unsigned int k=0; k<mass.size(); k++){
      if(mymass == mass[k]){
        match = true;
        observed_fine.emplace_back(observed[k]);
        mass_low = mass[k];
        mass_high = mass[k+1];
        idx_low = k;
      }
    }
    if(!match){
      observed_fine.emplace_back((fabs(mymass-mass_low)) * ((observed[idx_low] - observed[idx_low+1])/(mass_low - mass_high)) + observed[idx_low]);
    }
  }


  int idx_obs_low = -1, idx_obs_high = -1, idx_obs_low_vector_k1 = -1, idx_obs_high_vector_k1 = -1, idx_obs_low_vector_k0 = -1, idx_obs_high_vector_k0 = -1;
  bool obsfound = false, obsfound_vector_k1 = false, obsfound_vector_k0 = false;

  //find intersection -- Obs
  for(unsigned int j=0; j<x_theo.size(); j++){
    if(theory[j] < observed_fine[j] && !obsfound && x_theo[j] >= 300){
      idx_obs_low  = j-1;
      idx_obs_high = j;
      obsfound = true;
    }
    if(theory_vector_k1[j] < observed_fine[j] && !obsfound_vector_k1 && x_theo[j] >= 300){
      idx_obs_low_vector_k1  = j-1;
      idx_obs_high_vector_k1 = j;
      obsfound_vector_k1 = true;
    }
    else if(x_theo[j] == x_theo[x_theo.size()-1]){
      idx_obs_low_vector_k1  = j-1;
      idx_obs_high_vector_k1 = j;
      obsfound_vector_k1 = true;
    }
    if(theory_vector_k0[j] < observed_fine[j] && !obsfound_vector_k0 && x_theo[j] >= 300){
      idx_obs_low_vector_k0  = j-1;
      idx_obs_high_vector_k0 = j;
      obsfound_vector_k0 = true;
    }
  }




  cout << "Observed mass limit scalar: " << ((( theory[idx_obs_low] - (theory[idx_obs_high] - theory[idx_obs_low])/(x_theo[idx_obs_high]-x_theo[idx_obs_low])*x_theo[idx_obs_low])-(observed_fine[idx_obs_low] - (observed_fine[idx_obs_high] - observed_fine[idx_obs_low])/(x_theo[idx_obs_high]-x_theo[idx_obs_low])*x_theo[idx_obs_low]))/(((observed_fine[idx_obs_high] - observed_fine[idx_obs_low])/(x_theo[idx_obs_high] -x_theo[idx_obs_low]))-((theory[idx_obs_high] - theory[idx_obs_low])/(x_theo[idx_obs_high]-x_theo[idx_obs_low])))) << " GeV" << endl;

  double masslimit_vector_k1 = ((( theory_vector_k1[idx_obs_low_vector_k1] - (theory_vector_k1[idx_obs_high_vector_k1] - theory_vector_k1[idx_obs_low_vector_k1])/(x_theo[idx_obs_high_vector_k1]-x_theo[idx_obs_low_vector_k1])*x_theo[idx_obs_low_vector_k1])-(observed_fine[idx_obs_low_vector_k1] - (observed_fine[idx_obs_high_vector_k1] - observed_fine[idx_obs_low_vector_k1])/(x_theo[idx_obs_high_vector_k1]-x_theo[idx_obs_low_vector_k1])*x_theo[idx_obs_low_vector_k1]))/(((observed_fine[idx_obs_high_vector_k1] - observed_fine[idx_obs_low_vector_k1])/(x_theo[idx_obs_high_vector_k1] -x_theo[idx_obs_low_vector_k1]))-((theory_vector_k1[idx_obs_high_vector_k1] - theory_vector_k1[idx_obs_low_vector_k1])/(x_theo[idx_obs_high_vector_k1]-x_theo[idx_obs_low_vector_k1]))));
  if(masslimit_vector_k1 > 2000) masslimit_vector_k1 = 2000;
  cout << "Observed mass limit vector k1: " << masslimit_vector_k1 << " GeV" << endl;

  cout << "Observed mass limit vector k0: " << ((( theory_vector_k0[idx_obs_low_vector_k0] - (theory_vector_k0[idx_obs_high_vector_k0] - theory_vector_k0[idx_obs_low_vector_k0])/(x_theo[idx_obs_high_vector_k0]-x_theo[idx_obs_low_vector_k0])*x_theo[idx_obs_low_vector_k0])-(observed_fine[idx_obs_low_vector_k0] - (observed_fine[idx_obs_high_vector_k0] - observed_fine[idx_obs_low_vector_k0])/(x_theo[idx_obs_high_vector_k0]-x_theo[idx_obs_low_vector_k0])*x_theo[idx_obs_low_vector_k0]))/(((observed_fine[idx_obs_high_vector_k0] - observed_fine[idx_obs_low_vector_k0])/(x_theo[idx_obs_high_vector_k0] -x_theo[idx_obs_low_vector_k0]))-((theory_vector_k0[idx_obs_high_vector_k0] - theory_vector_k0[idx_obs_low_vector_k0])/(x_theo[idx_obs_high_vector_k0]-x_theo[idx_obs_low_vector_k0])))) << " GeV" << endl;





  //5) convert to upper and lower errors to be used in TGraphAsymmErrors

  vector<double> expected_up_68, expected_down_68, expected_up_95, expected_down_95;
  for(int i=0; i<n_points; i++){
    expected_up_68.push_back(expected_high_68[i] - expected[i]);
    expected_up_95.push_back(expected_high_95[i] - expected[i]);
    expected_down_68.push_back(expected[i] - expected_low_68[i]);
    expected_down_95.push_back(expected[i] - expected_low_95[i]);
  }

  //6) build graph objects
  TGraphAsymmErrors* g_expected_68 = new TGraphAsymmErrors(n_points,&mass[0],&expected[0],0,0,&expected_down_68[0], &expected_up_68[0]);
  TGraphAsymmErrors* g_expected_95 = new TGraphAsymmErrors(n_points,&mass[0],&expected[0],0,0,&expected_down_95[0], &expected_up_95[0]);

  // TGraph* g_theory = new TGraph(theory.size(),&x_theo[0],&theory[0]);
  // TGraph* g_theory_up = new TGraph(theory.size(),&x_theo[0],&theory_up[0]);
  // TGraph* g_theory_down = new TGraph(theory.size(),&x_theo[0],&theory_down[0]);
  TGraph* g_expected = new TGraph(n_points,&mass[0],&expected[0]);
  TGraph* g_observed = new TGraph(n_points,&mass[0],&observed[0]);

  //7) cosmetics
  g_theory->SetLineWidth(3);
  g_theory->SetLineStyle(9);
  g_theory_up->SetLineWidth(3);
  g_theory_up->SetLineStyle(3);
  g_theory_down->SetLineWidth(3);
  g_theory_down->SetLineStyle(3);
  g_theory_vector_k1->SetLineWidth(3);
  g_theory_vector_k1->SetLineStyle(9);
  g_theory_vector_k1->SetLineColor(kMagenta-7);
  g_theory_vector_k1_up->SetLineWidth(3);
  g_theory_vector_k1_up->SetLineStyle(3);
  g_theory_vector_k1_up->SetLineColor(kMagenta-7);
  g_theory_vector_k1_down->SetLineWidth(3);
  g_theory_vector_k1_down->SetLineStyle(3);
  g_theory_vector_k1_down->SetLineColor(kMagenta-7);
  g_theory_vector_k0->SetLineWidth(3);
  g_theory_vector_k0->SetLineStyle(9);
  g_theory_vector_k0->SetLineColor(kRed+1);
  g_theory_vector_k0_up->SetLineWidth(3);
  g_theory_vector_k0_up->SetLineStyle(3);
  g_theory_vector_k0_up->SetLineColor(kRed+1);
  g_theory_vector_k0_down->SetLineWidth(3);
  g_theory_vector_k0_down->SetLineStyle(3);
  g_theory_vector_k0_down->SetLineColor(kRed+1);
  g_observed->SetLineWidth(3);
  g_observed->SetLineStyle(1);
  g_expected->SetLineWidth(3);
  g_expected->SetLineStyle(2);
  g_expected_68->SetFillStyle(1001);
  g_expected_95->SetFillStyle(1001);
  g_expected_68->SetFillColor(kGreen+1);
  g_expected_95->SetFillColor(kOrange);
  g_expected_68->SetLineWidth(0);
  g_expected_95->SetLineWidth(0);

  g_expected_95->SetMaximum(10000);
  g_expected_95->SetMinimum(5E-5);
  g_expected_95->SetTitle("");

  //8) Draw Graphs
  TCanvas* c = new TCanvas("c", "Nice limit plot", 800,600);
  gPad->SetLogy();
  // gPad->SetBottomMargin(0.13);
  // gPad->SetLeftMargin(0.16);
  // gPad->SetRightMargin(0.06);
  g_expected_95->Draw("A3");
  g_expected_68->Draw("3 SAME");
  g_expected->Draw("SAME");
  if(with_data) g_observed->Draw("SAME");
  g_theory->Draw("SAME");
  g_theory_up->Draw("SAME");
  g_theory_down->Draw("SAME");
  g_theory_vector_k1->Draw("SAME");
  g_theory_vector_k1_up->Draw("SAME");
  g_theory_vector_k1_down->Draw("SAME");
  g_theory_vector_k0->Draw("SAME");
  g_theory_vector_k0_up->Draw("SAME");
  g_theory_vector_k0_down->Draw("SAME");

  //9) Legend
  TLegend *leg;
  leg = new TLegend(0.60,0.67,0.95,0.92,"");//x+0.1
  //leg->SetHeader("95% CL upper limits");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetLineColor(1);
  leg->SetTextFont(42);
  leg->SetTextSize(0.041);
  leg->SetHeader("95% CL upper limit");
  ((TLegendEntry*)leg->GetListOfPrimitives()->First())->SetTextSize(0.045);
  if(with_data)leg->AddEntry(g_observed, "Observed","L");
  leg->AddEntry(g_expected, "Expected","L");
  leg->AddEntry(g_expected_68, "68% expected","F");
  leg->AddEntry(g_expected_95, "95% expected","F");
  leg->Draw();


  TLegend *leg2;
  leg2 = new TLegend(0.60,0.43,0.95,0.62,"");//x+0.1
  //leg->SetHeader("95% CL upper limits");
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.030);
  leg2->SetFillStyle(0);
  leg2->SetLineColor(1);
  leg2->SetTextFont(42);
  leg2->SetTextSize(0.041);
  leg2->SetHeader("LQ pair production");
  ((TLegendEntry*)leg2->GetListOfPrimitives()->First())->SetTextSize(0.045);

  leg2->AddEntry(g_theory, "Scalar LQ","L");
  leg2->AddEntry(g_theory_vector_k0, "Vector LQ (#kappa = 0)","L");
  leg2->AddEntry(g_theory_vector_k1, "Vector LQ (#kappa = 1)","L");
  leg2->Draw();

  //10) CMS tags
  TString infotext = infotexts.at(AnalysisTool::year);
  TLatex *text1 = new TLatex(3.5, 24, infotext);
  text1->SetNDC();
  text1->SetTextAlign(33);
  text1->SetX(0.965);
  text1->SetTextFont(42);
  text1->SetTextSize(0.046);
  text1->SetY(0.99);
  text1->Draw();

  TString cmstext = "CMS";
  TLatex *text2 = new TLatex(3.5, 24, cmstext);
  text2->SetNDC();
  text2->SetTextAlign(13);
  text2->SetX(0.15);
  text2->SetTextFont(62);
  text2->SetTextSize(0.072);
  text2->SetY(0.905);
  // text2->Draw();

  TString preltext = "Supplementary";
  TLatex *text3 = new TLatex(3.5, 24, preltext);
  text3->SetNDC();
  text3->SetTextAlign(13);
  text3->SetX(0.263);
  text3->SetTextFont(52);
  text3->SetTextSize(0.05);
  text3->SetY(0.893);
  // text3->Draw();


  //12) care about axes
  TH1D* h = (TH1D*)g_expected_95->GetHistogram();
  h->GetXaxis()->SetRangeUser(mass[0], mass[n_points-1]);
  h->SetXTitle("M_{LQ} [GeV]");
  if (channel == "much") h->SetYTitle("#sigma_{LQLQ} #times #bf{#it{#Beta}}^{2}(LQ#rightarrow t#mu) [pb]");
  else if (channel == "ech") h->SetYTitle("#sigma_{LQLQ} #times #bf{#it{#Beta}}^{2}(LQ#rightarrow te) [pb]");
  else if (channel == "comb") h->SetYTitle("#sigma_{LQLQ} #times #bf{#it{#Beta}}^{2}(LQ comb) [pb]");
  h->GetYaxis()->SetTitleSize(0.048);
  h->GetYaxis()->SetTitleOffset(1.05);
  h->Draw("AXIS SAME");
  gPad->SetTopMargin(0.06);
  gPad->SetLeftMargin(0.11);
  gPad->SetRightMargin(0.035);
  gPad->SetBottomMargin(0.11);


  // c->SaveAs(AnalysisTool::theta_path + "output/limitplot_combine_mc_fullsyst_much.eps");
  // c->SaveAs(AnalysisTool::theta_path + "output/limitplot_combine_mc_fullsyst_much.pdf");

  // had no permission to write in that directory
  c->SaveAs("limitplot_combine_mc_fullsyst_" + channel + ".eps");
  c->SaveAs("limitplot_combine_mc_fullsyst_" + channel + ".pdf");
}


void cosmetics(){

  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.055, "XYZ");
  gStyle->SetTitleXOffset(0.9);
  gStyle->SetTitleYOffset(1.25);
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.007, "XYZ");
  gStyle->SetLabelSize(0.05, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(505, "XYZ");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
}
