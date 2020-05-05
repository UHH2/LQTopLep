#include "../include/Tools.h"
#include "../include/cosmetics.h"
#include "../include/constants.h"
#include <TH1D.h>
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>
#include <TEfficiency.h>
#include <TLatex.h>
#include <iostream>


using namespace std;




void AnalysisTool::AddSystematicsEleTrigger(){

  double pt_firsttrigger = 30.;
  if(AnalysisTool::year == "2017") pt_firsttrigger = 40.;
  if(AnalysisTool::year == "2018") pt_firsttrigger = 35.;

  double pt_secondtrigger = 200.;
  if(AnalysisTool::year == "2016") pt_secondtrigger = 175.;

  TString filename_nominal = base_path + year + "/" + trigger_tag + "ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to";
  filename_nominal += pt_secondtrigger;
  filename_nominal += ".root";

  TString filename_lowpt = base_path + year + "/" + trigger_tag + "ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to50.root";

  TString filename_highpt = base_path + year + "/" + trigger_tag + "ElectronTriggerScaleFactors_eta_ele_binned_official_pt50to";
  filename_highpt += pt_secondtrigger;
  filename_highpt += ".root";

  unique_ptr<TFile> f_nom, f_lowpt, f_highpt;
  f_nom   .reset(new TFile(filename_nominal, "READ"));
  f_lowpt .reset(new TFile(filename_lowpt,   "READ"));
  f_highpt.reset(new TFile(filename_highpt,  "READ"));

  TString graphname = "ScaleFactors";

  unique_ptr<TGraphAsymmErrors> g_nom, g_lowpt, g_highpt;
  g_nom.   reset((TGraphAsymmErrors*)f_nom->   Get(graphname));
  g_lowpt. reset((TGraphAsymmErrors*)f_lowpt-> Get(graphname));
  g_highpt.reset((TGraphAsymmErrors*)f_highpt->Get(graphname));


  // loop through bins
  vector<double> x_new, y_new, x_lo_new, x_hi_new, y_lo_new, y_hi_new;
  for(int i=0; i<g_nom->GetN(); i++){
    double y_nom = 0, y_low = 0, y_high = 0, x = 0;
    g_nom->GetPoint(i,x,y_nom);
    g_lowpt->GetPoint(i,x,y_low);
    g_highpt->GetPoint(i,x,y_high);

    // cout << endl << "bin: " << i << endl;
    // cout << "eyh before: " << g_nom->GetErrorYhigh(i) << ", eyl before: " << g_nom->GetErrorYlow(i) << endl;
    double syserr = max(fabs(y_nom-y_low), fabs(y_nom-y_high));
    // cout << "ynom: " << y_nom << ", ylow: " << y_low << ", yhigh: " << y_high << ", syserr: " << syserr << endl;
    // cout << "eyh after: " << g_nom->GetErrorYhigh(i) << ", eyl after: " << g_nom->GetErrorYlow(i) << endl;

    x_new.   emplace_back(x);
    x_lo_new.emplace_back(g_nom->GetErrorXlow(i));
    x_hi_new.emplace_back(g_nom->GetErrorXhigh(i));
    y_new.   emplace_back(y_nom);
    y_hi_new.emplace_back(  sqrt(pow(g_nom->GetErrorYhigh(i), 2) + pow(syserr, 2))  );
    y_lo_new.emplace_back(  sqrt(pow(g_nom->GetErrorYlow(i), 2)  + pow(syserr, 2))  );
  }

  unique_ptr<TGraphAsymmErrors> g_new;
  g_new.reset(new TGraphAsymmErrors(x_new.size(), &x_new[0], &y_new[0], &x_lo_new[0], &x_hi_new[0], &y_lo_new[0], &y_hi_new[0]) );
  g_new->SetName(graphname);
  HistCosmetics(g_nom.get());
  g_nom->SetLineColor(2);
  g_nom->SetMarkerColor(2);
  HistCosmetics(g_new.get());

  // plot old and new SFs in one plot
  TCanvas* c = new TCanvas("c", "c", 400, 400);
  TPad* p = SetupPad();
  p->Draw();
  p->cd();

  g_new->GetHistogram()->GetYaxis()->SetRangeUser(0.75, 1.25);
  g_new->GetHistogram()->GetYaxis()->SetTitle("Electron trigger SF");
  g_new->GetHistogram()->GetXaxis()->SetTitle("#eta^{e}");
  g_new->Draw("AP");
  g_nom->Draw("P SAME");

  TLegend* leg = new TLegend(0.5,0.7,0.9,0.9);
  TString headertext = "";
  headertext += pt_firsttrigger;
  headertext += " GeV < p_{T}^{e} < ";
  headertext += pt_secondtrigger;
  headertext += " GeV";
  leg->SetHeader(headertext);
  leg->AddEntry(g_nom.get(),"Statistical uncertainty only","e");
  leg->AddEntry(g_new.get(),"Total uncertainty","e");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->Draw();


  TString infotext = infotexts.at(year);
  TLatex *text1 = new TLatex(3.5, 24, infotext);
  text1->SetNDC();
  text1->SetTextAlign(33);
  text1->SetX(0.94);
  text1->SetTextFont(43);
  text1->SetTextSize(16);
  text1->SetY(0.995);
  text1->Draw("SAME");

  TString plotname = base_path + year + "/" + trigger_tag + "ElectronTriggerScaleFactorComparison.pdf";
  c->SaveAs(plotname);

  TString outfilename = base_path + year + "/" + trigger_tag + "ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to";
  outfilename += pt_secondtrigger;
  outfilename += "_withsyst.root";
  unique_ptr<TFile> fout;
  fout.reset(new TFile(outfilename, "RECREATE"));
  fout->cd();
  g_new->Write();
  fout->Close();

}
