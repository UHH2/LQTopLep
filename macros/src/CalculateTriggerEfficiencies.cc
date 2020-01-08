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
#include <TLatex.h>
#include <iostream>


using namespace std;


void CalculateEffForVar(TString mode, TString varname, TString base_path, TString trigger_tag, TString year, TString yeartag);


void AnalysisTool::CalculateTriggerEfficiencies(){

  CalculateEffForVar("electron", "pt_ele_fine", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("electron", "pt_ele_binned", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("electron", "eta_ele", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("electron", "eta_ele_binned", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("electron", "dR_ele_mu", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("electron", "dRmin_ele_jet", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("electron", "dRmin_ele_obj", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);

  CalculateEffForVar("muon", "pt_mu_fine", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("muon", "pt_mu_binned", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("muon", "eta_mu", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("muon", "eta_mu_binned", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("muon", "dR_mu_ele", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("muon", "dRmin_mu_jet", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);
  CalculateEffForVar("muon", "dRmin_mu_obj", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);


}


void CalculateEffForVar(TString mode, TString varname, TString base_path, TString trigger_tag, TString year, TString yeartag){


  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  if(mode != "electron" && mode != "muon") throw runtime_error("Only 'electron' or 'muon' allowed as value for 'mode'.");


  // Read files
  // ==========

  unique_ptr<TFile> input_TTbar, input_DY, input_ST, input_WJ, input_DB, input_QCDMU, input_QCDELE, input_DATAMU, input_DATAELE;
  input_TTbar.reset(new TFile(base_path + year + "/" + trigger_tag + "Fullselection/uhh2.AnalysisModuleRunner.MC.TTbar_" + yeartag + ".root","READ"));
  input_DY.reset(new TFile(base_path + year + "/" + trigger_tag + "Fullselection/uhh2.AnalysisModuleRunner.MC.DYJets_" + yeartag + ".root","READ"));
  input_ST.reset(new TFile(base_path + year + "/" + trigger_tag + "Fullselection/uhh2.AnalysisModuleRunner.MC.SingleTop_" + yeartag + ".root","READ"));
  input_WJ.reset(new TFile(base_path + year + "/" + trigger_tag + "Fullselection/uhh2.AnalysisModuleRunner.MC.WJets_" + yeartag + ".root","READ"));
  input_DB.reset(new TFile(base_path + year + "/" + trigger_tag + "Fullselection/uhh2.AnalysisModuleRunner.MC.Diboson_" + yeartag + ".root","READ"));
  input_QCDMU.reset(new TFile(base_path + year + "/" + trigger_tag + "Fullselection/uhh2.AnalysisModuleRunner.MC.QCDMu_" + yeartag + ".root","READ"));
  input_QCDELE.reset(new TFile(base_path + year + "/" + trigger_tag + "Fullselection/uhh2.AnalysisModuleRunner.MC.QCDEle_" + yeartag + ".root","READ"));
  input_DATAMU.reset(new TFile(base_path + year + "/" + trigger_tag + "Fullselection/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_" + yeartag + ".root","READ"));
  input_DATAELE.reset(new TFile(base_path + year + "/" + trigger_tag + "Fullselection/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_" + yeartag + ".root","READ"));


  // Read histograms for MC and data
  // ===============================

  TString histname_before = "", histname_after = "";
  if(mode == "electron"){
    histname_before = "ElectronTriggerBefore_Trigger/" + varname;
    histname_after = "ElectronTriggerAfter_Trigger/" + varname;
  }
  else{
    histname_before = "MuonTriggerBefore_Trigger/" + varname;
    histname_after = "MuonTriggerAfter_Trigger/" + varname;
  }

  // Denominator histograms
  TH1D* h_TTbar_before = (TH1D*)input_TTbar->Get(histname_before);
  // TH1D* h_ST_before    = (TH1D*)input_ST->Get(histname_before);
  // TH1D* h_DY_before    = (TH1D*)input_DY->Get(histname_before);
  // TH1D* h_WJ_before    = (TH1D*)input_WJ->Get(histname_before);
  // TH1D* h_DB_before    = (TH1D*)input_DB->Get(histname_before);
  // TH1D* h_QCD_before;
  // if(mode == "electron") h_QCD_before = (TH1D*)input_QCDMU->Get(histname_before);
  // else                   h_QCD_before = (TH1D*)input_QCDELE->Get(histname_before);
  TH1D* h_MC_before = (TH1D*)h_TTbar_before->Clone("h_MC_before");
  // h_MC_before->Add(h_ST_before);
  // h_MC_before->Add(h_DY_before);
  // h_MC_before->Add(h_WJ_before);
  // h_MC_before->Add(h_DB_before);
  // h_MC_before->Add(h_QCD_before);

  // Numerator histograms
  TH1D* h_TTbar_after  = (TH1D*)input_TTbar->Get(histname_after);
  // TH1D* h_ST_after     = (TH1D*)input_ST->Get(histname_after);
  // TH1D* h_DY_after     = (TH1D*)input_DY->Get(histname_after);
  // TH1D* h_WJ_after     = (TH1D*)input_WJ->Get(histname_after);
  // TH1D* h_DB_after     = (TH1D*)input_DB->Get(histname_after);
  // TH1D* h_QCD_after;
  // if(mode == "electron") h_QCD_after = (TH1D*)input_QCDMU->Get(histname_after);
  // else                   h_QCD_after = (TH1D*)input_QCDELE->Get(histname_after);
  TH1D* h_MC_after = (TH1D*)h_TTbar_after->Clone("h_MC_after");
  // h_MC_after->Add(h_ST_after);
  // h_MC_after->Add(h_DY_after);
  // h_MC_after->Add(h_WJ_after);
  // h_MC_after->Add(h_DB_after);
  // h_MC_after->Add(h_QCD_after);
  TH1D* h_DATA_before, *h_DATA_after;
  if(mode == "electron"){
    h_DATA_before = (TH1D*)input_DATAMU->Get(histname_before);
    h_DATA_after   = (TH1D*)input_DATAMU->Get(histname_after);
  }
  else{
    h_DATA_before = (TH1D*)input_DATAELE->Get(histname_before);
    h_DATA_after   = (TH1D*)input_DATAELE->Get(histname_after);
  }
  cout << "done loading histograms" << endl;



  // Calculate efficiencies and SFs
  // ==============================

  // Efficiencies
  TGraphAsymmErrors* gr_mc;
  for(int i=0; i<h_MC_before->GetNbinsX(); i++){
    if(h_MC_before->GetBinContent(i+1) < h_MC_after->GetBinContent(i+1)){
      h_MC_after->SetBinContent(i+1, h_MC_before->GetBinContent(i+1));
    }
  }
  gr_mc = new TGraphAsymmErrors(h_MC_after, h_MC_before);
  TGraphAsymmErrors* gr_data = new TGraphAsymmErrors(h_DATA_after, h_DATA_before);
  HistCosmetics(gr_mc);
  HistCosmetics(gr_data);
  gr_mc->SetLineColor(kRed-4);
  gr_data->SetLineColor(kBlack);


  // SFs -- need to calculate ratio bin-by-bin, can't divide TGraphs
  const int n_bins = gr_data->GetN();
  int at_in_mc = 0, at_in_data = 0, n_points = 0;
  vector<double> SF_x, SF_y, SF_x_high, SF_x_low, SF_y_high, SF_y_low;
  for(int i=0; i<n_bins; i++){
    double x_MC, y_MC, x_DATA, y_DATA;
    double MC_x_high, MC_x_low, MC_y_high, MC_y_low;
    double DATA_y_high, DATA_y_low;

    gr_mc->GetPoint(at_in_mc,x_MC,y_MC);
    gr_data->GetPoint(at_in_data,x_DATA,y_DATA);
    // cout << "mc start: " << x_MC << endl;
    // cout << "data start: " << x_DATA << endl << endl;

    double last_data = -1, last_mc = -1;
    bool skip = false;
    while(x_MC != x_DATA){
      if(x_MC < x_DATA){
        at_in_mc++;
        gr_mc->GetPoint(at_in_mc,x_MC,y_MC);

      }
      else if(x_DATA < x_MC){
        at_in_data++;
        gr_data->GetPoint(at_in_data,x_DATA,y_DATA);
      }

      if(x_MC == last_mc && x_DATA == last_data){
        skip = true;
        break;
      }
      last_mc = x_MC;
      last_data = x_DATA;
    }
    if(skip) break;

    MC_x_high = gr_mc->GetErrorXhigh(i);
    MC_x_low = gr_mc->GetErrorXlow(i);
    MC_y_high = gr_mc->GetErrorYhigh(i);
    MC_y_low = gr_mc->GetErrorYlow(i);

    DATA_y_high = gr_data->GetErrorYhigh(i);
    DATA_y_low = gr_data->GetErrorYlow(i);

    //gaussian error propagation
    SF_x.push_back(x_MC);
    SF_x_low.push_back(MC_x_low);
    SF_x_high.push_back(MC_x_high);
    SF_y.push_back(y_DATA/y_MC);
    SF_y_low.push_back(sqrt(pow(DATA_y_low/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_high,2)));
    SF_y_high.push_back(sqrt(pow(DATA_y_high/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_low,2)));

    // cout << "x-data: " << x_DATA << ", x-mc: " << x_MC << ", x-sf: " << SF_x.back() << endl;
    // cout << "y-data: " << y_DATA << ", y-mc: " << y_MC << ", y-sf: " << SF_y.back() << endl;
    at_in_mc++;
    at_in_data++;
    n_points++;

  }

  TGraphAsymmErrors* gr_ratio = new TGraphAsymmErrors(n_bins, &SF_x[0], &SF_y[0], &SF_x_low[0], &SF_x_high[0], &SF_y_low[0], &SF_y_high[0]);
  HistCosmetics(gr_ratio,true);



  // Plot
  // ====

  TCanvas* c = new TCanvas("c", "c", 400, 400);
  TPad* p_top = SetupRatioPadTop();
  TPad* p_bot = SetupRatioPad();
  p_top->Draw();
  p_bot->Draw();
  p_top->cd();
  gr_mc->GetYaxis()->SetRangeUser(0.0001,1.1);
  gr_mc->GetYaxis()->SetTitle("Trigger efficiency");
  gr_mc->GetXaxis()->SetLimits(gr_ratio->GetXaxis()->GetXmin(),gr_ratio->GetXaxis()->GetXmax());
  gr_mc->Draw("APZ");
  gr_data->Draw("PZ SAME");

  TLegend* leg_top = new TLegend(0.3,0.1,0.5,0.3);
  leg_top->AddEntry(gr_data,"Data","le");
  leg_top->AddEntry(gr_mc,"MC","le");
  leg_top->SetBorderSize(0);
  leg_top->SetFillStyle(0);
  leg_top->Draw();

  p_bot->cd();
  gr_ratio->GetYaxis()->SetRangeUser(0.65, 1.35);
  gr_ratio->GetXaxis()->SetTitle(h_DATA_before->GetTitle());
  gr_ratio->GetYaxis()->SetTitle("DATA / MC");
  gr_ratio->GetYaxis()->SetTitleSize(15);
  gr_ratio->SetLineColor(kBlack);
  gr_ratio->SetTitle("");
  gr_ratio->Draw("APZ");

  TLine* l_unity = new TLine(gr_ratio->GetXaxis()->GetXmin(), 1, gr_ratio->GetXaxis()->GetXmax(), 1);
  l_unity->SetLineColor(kRed-4);
  l_unity->SetLineWidth(2);
  l_unity->SetLineStyle(2);
  l_unity->Draw("SAME");

  p_top->cd();
  TString infotext = infotexts[year];
  TLatex *text1 = new TLatex(3.5, 24, infotext);
  text1->SetNDC();
  text1->SetTextAlign(33);
  text1->SetX(0.94);
  text1->SetTextFont(43);
  text1->SetTextSize(16);
  text1->SetY(0.995);
  text1->Draw("SAME");

  TString plotname = "";
  if(mode == "electron") plotname += "Electron";
  else                   plotname += "Muon";
  plotname += "TriggerEfficiency_" + varname;
  // c->SaveAs(base_path + year + "/" + trigger_tag + "Fullselection/" + plotname + ".eps");
  c->SaveAs(base_path + year + "/" + trigger_tag + "Fullselection/" + plotname + ".pdf");


}
