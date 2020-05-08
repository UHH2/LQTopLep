#include "../include/Tools.h"
#include "../include/cosmetics.h"
#include "../include/constants.h"
#include <TH1D.h>
#include <TH2D.h>
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


void AddDYSyst(TString mode, TString varname, TString base_path, TString trigger_tag, TString year, TString uhh2_path, double etalow, double etahigh);
void AddPTSyst(TString mode, TString varname, TString base_path, TString trigger_tag, TString year, TString yeartag, TString uhh2_path, double etalow, double etahigh);
void CombineSysts(TString mode, TString varname, TString base_path, TString trigger_tag, TString year, TString yeartag, TString uhh2_path, double etalow, double etahigh);

void AnalysisTool::AddSystematicsMuonTrigger(){



  vector<pair<double,double>> eta_regions = {
    make_pair(0,   0.9),
    make_pair(0.9,   1.2),
    make_pair(1.2,   2.1),
    make_pair(2.1,   2.4),
  };

  for(size_t i=0; i<eta_regions.size(); i++){

    double etalow = eta_regions[i].first;
    double etahigh = eta_regions[i].second;

    AddDYSyst("muon_eta", "pt_mu_binned_official", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::uhh2_path, etalow, etahigh);
    AddDYSyst("muon_eta", "pt_mu_binned_official_rebin", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::uhh2_path, etalow, etahigh);

    AddPTSyst("muon_eta", "pt_mu_binned_official", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag, AnalysisTool::uhh2_path, etalow, etahigh);
    AddPTSyst("muon_eta", "pt_mu_binned_official_rebin", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag, AnalysisTool::uhh2_path, etalow, etahigh);

    CombineSysts("muon_eta", "pt_mu_binned_official", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag, AnalysisTool::uhh2_path, etalow, etahigh);
    CombineSysts("muon_eta", "pt_mu_binned_official_rebin", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag, AnalysisTool::uhh2_path, etalow, etahigh);


  }


}






void AddDYSyst(TString mode, TString varname, TString base_path, TString trigger_tag, TString year, TString uhh2_path, double etalow, double etahigh){

  map<double, TString> etamap_string = {
    make_pair(0., "0"),
    make_pair(0.9, "0p9"),
    make_pair(1.2, "1p2"),
    make_pair(2.1, "2p1"),
    make_pair(2.4, "2p4"),
  };


  TString filename_nominal = base_path + year + "/" + trigger_tag + "MuonTriggerScaleFactors_" + varname + "_eta";
  filename_nominal += etamap_string[etalow];
  filename_nominal += "to";
  filename_nominal += etamap_string[etahigh];
  filename_nominal += ".root";
  cout << "filename_nominal: " << filename_nominal << endl;

  TString path_official = uhh2_path + "common/data/" + year + "/";
  TString filename_official = "";
  if(year == "2016")       filename_official = "MuonTrigger_EfficienciesAndSF_average_RunBtoH.root";
  else if (year == "2017") filename_official = "MuonTrigger_EfficienciesAndSF_RunBtoF_Nov17Nov2017.root";
  else if (year == "2018") filename_official = "Muon_Trigger_Eff_SF_AfterMuonHLTUpdate.root";
  filename_official = path_official + filename_official;

  unique_ptr<TFile> f_nom, f_off;
  f_nom.reset(new TFile(filename_nominal,  "READ"));
  f_off.reset(new TFile(filename_official, "READ"));

  TString graphname = "ScaleFactors";
  TString histname = "";
  if(year == "2016")       histname = "IsoMu24_OR_IsoTkMu24_PtEtaBins/pt_abseta_ratio";
  else if (year == "2017") histname = "IsoMu27_PtEtaBins/pt_abseta_ratio";
  else if (year == "2018") histname = "IsoMu24_PtEtaBins/pt_abseta_ratio";


  unique_ptr<TGraphAsymmErrors> g_nom;
  unique_ptr<TH2D> g_off;
  g_nom.reset((TGraphAsymmErrors*)f_nom->Get(graphname));
  g_off.reset((TH2D*)f_off->Get(histname));

  double eta_readout = (etahigh - etalow)/2. + etalow;
  // cout << "eta_readout: " << eta_readout << endl;

  // loop through our SFs, for each bin find all official SFs falling into that bin
  for(int i=0; i<g_nom->GetN(); i++){
    double pt, sf_nom;
    g_nom->GetPoint(i, pt, sf_nom);
    double pt_lo = pt - g_nom->GetErrorXlow(i);
    double pt_hi = pt + g_nom->GetErrorXhigh(i);
    double sf_lo = sf_nom - g_nom->GetErrorYlow(i);
    double sf_hi = sf_nom + g_nom->GetErrorYhigh(i);
    cout << "pt = " << pt_lo << "-" << pt_hi << endl;

    vector<double> sfs_off = {};
    int number_off_bins = g_off->FindFixBin(pt_hi, eta_readout) - g_off->FindFixBin(pt_lo, eta_readout) + 1;
    cout << "number of overlapping bins: " << number_off_bins << endl;
    for(int j=0; j<number_off_bins; j++){
      int readoutbin = g_off->FindFixBin(pt_lo, eta_readout) + j;
      double tmp = g_off->GetBinContent(readoutbin);
      if(tmp == 0.) continue;
      sfs_off.emplace_back(tmp);
      cout << "official SF in this bin: " << tmp << endl;
    }
    cout << "nominal sf: " << sf_nom << endl;
    cout << "high sf:    " << sf_hi << endl;
    cout << "low sf:     " << sf_lo << endl;

    double max_dev = 0.;
    double furthest_sf_off = -1.;
    for(size_t j=0; j<sfs_off.size(); j++){
      double dev = fabs(sf_nom - sfs_off[j]);
      if(dev > max_dev){
        max_dev = dev;
        furthest_sf_off = sfs_off[j];
      }
    }

    //halve max_dev
    max_dev /= 2;

    // inflate error such that it covers even the furthest point
    double new_err_up = g_nom->GetErrorYhigh(i);
    double new_err_down = g_nom->GetErrorYlow(i);
    double inflate_errors_by = 0.;
    // if(furthest_sf_off > sf_nom + new_err_up){
    //   // new_err_up = new_err_up + (furthest_sf_off - (sf_nom + new_err_up));
    //   inflate_errors_by = furthest_sf_off - (sf_nom + new_err_up);
    // }
    // else if(furthest_sf_off < sf_nom - new_err_down){
    //   // new_err_down = new_err_down + (sf_nom - new_err_down) - furthest_sf_off;
    //   inflate_errors_by = (sf_nom - new_err_down) - furthest_sf_off;
    // }
    // new_err_up   += inflate_errors_by;
    // new_err_down += inflate_errors_by;
    new_err_up = sqrt(pow(g_nom->GetErrorYhigh(i), 2) + pow(max_dev, 2));
    new_err_down = sqrt(pow(g_nom->GetErrorYlow(i), 2) + pow(max_dev, 2));

    cout << "extra systematic would be: " << max_dev << endl;
    cout << "sf with stat errors:  " << sf_nom << " + " << g_nom->GetErrorYhigh(i) << " - " << g_nom->GetErrorYlow(i) << endl;
    cout << "sf with total errors: " << sf_nom << " + " << sqrt(pow(g_nom->GetErrorYhigh(i), 2) + pow(max_dev, 2)) << " - " << sqrt(pow(g_nom->GetErrorYlow(i), 2) + pow(max_dev, 2)) << endl;
    cout << "sf with inflated errors: " << sf_nom << " + " << new_err_up << " - " << new_err_down << endl;
    g_nom->SetPointEYhigh(i, new_err_up);
    g_nom->SetPointEYlow(i,  new_err_down);
  }

  TString outfilename = filename_nominal.ReplaceAll(".root", "_withDYsyst.root");
  cout << "outfilename: " << outfilename << endl;
  unique_ptr<TFile> fout;
  fout.reset(new TFile(outfilename, "RECREATE"));
  g_nom->Write();
  fout->Close();

}


void AddPTSyst(TString mode, TString varname, TString base_path, TString trigger_tag, TString year, TString yeartag, TString uhh2_path, double etalow, double etahigh){

  TString dataname_ele = "Electron";
  if(year == "2017" || year == "2016") dataname_ele = "EGamma";


  map<double, TString> etamap_string = {
    make_pair(0., "0"),
    make_pair(0.9, "0p9"),
    make_pair(1.2, "1p2"),
    make_pair(2.1, "2p1"),
    make_pair(2.4, "2p4"),
  };

  // Read files
  // ==========

  unique_ptr<TFile> input_TTbar, input_DY, input_ST, input_WJ, input_DB, input_QCDELE, input_DATAELE;
  input_TTbar.reset(new TFile(base_path + year + "/" + trigger_tag + "uhh2.AnalysisModuleRunner.MC.TTbar_" + yeartag + ".root","READ"));
  input_DY.reset(new TFile(base_path + year + "/" + trigger_tag + "uhh2.AnalysisModuleRunner.MC.DYJets_" + yeartag + ".root","READ"));
  input_ST.reset(new TFile(base_path + year + "/" + trigger_tag + "uhh2.AnalysisModuleRunner.MC.SingleTop_" + yeartag + ".root","READ"));
  input_WJ.reset(new TFile(base_path + year + "/" + trigger_tag + "uhh2.AnalysisModuleRunner.MC.WJets_" + yeartag + ".root","READ"));
  input_DB.reset(new TFile(base_path + year + "/" + trigger_tag + "uhh2.AnalysisModuleRunner.MC.Diboson_" + yeartag + ".root","READ"));
  input_QCDELE.reset(new TFile(base_path + year + "/" + trigger_tag + "uhh2.AnalysisModuleRunner.MC.QCDEle_" + yeartag + ".root","READ"));
  input_DATAELE.reset(new TFile(base_path + year + "/" + trigger_tag + "uhh2.AnalysisModuleRunner.DATA.DATA_" + dataname_ele + "_" + yeartag + ".root","READ"));


  // Read histograms for MC and data
  // ===============================

  TString histname_before = "", histname_after = "";

  TString etalow_str = etamap_string[etalow];
  TString etahigh_str = etamap_string[etahigh];
  histname_before = "MuonTriggerBefore";
  histname_before += "_eta" + etalow_str + "to" + etahigh_str + "_Trigger/" + varname;
  histname_after = "MuonTriggerAfter";
  histname_after += "_eta" + etalow_str + "to" + etahigh_str + "_Trigger/" + varname;


  //data histograms
  TH1D* h_DATA_before = (TH1D*)input_DATAELE->Get(histname_before);
  TH1D* h_DATA_after  = (TH1D*)input_DATAELE->Get(histname_after);

  // Denominator histograms
  TH1D* h_TTbar_before = (TH1D*)input_TTbar->Get(histname_before);
  TH1D* h_ST_before    = (TH1D*)input_ST->Get(histname_before);
  TH1D* h_DY_before    = (TH1D*)input_DY->Get(histname_before);
  TH1D* h_WJ_before    = (TH1D*)input_WJ->Get(histname_before);
  TH1D* h_DB_before    = (TH1D*)input_DB->Get(histname_before);

  // Numerator histograms
  TH1D* h_TTbar_after  = (TH1D*)input_TTbar->Get(histname_after);
  TH1D* h_ST_after     = (TH1D*)input_ST->Get(histname_after);
  TH1D* h_DY_after     = (TH1D*)input_DY->Get(histname_after);
  TH1D* h_WJ_after     = (TH1D*)input_WJ->Get(histname_after);
  TH1D* h_DB_after     = (TH1D*)input_DB->Get(histname_after);


  // scale ttbar (before and after) such that total MC matches data before the trigger
  vector<double> ttbarsfs = {};
  for(int i=1; i<h_DATA_before->GetNbinsX()+1; i++){
    double data = h_DATA_before->GetBinContent(i);
    double mc = h_TTbar_before->GetBinContent(i) + h_ST_before->GetBinContent(i) + h_DY_before->GetBinContent(i) + h_WJ_before->GetBinContent(i) + h_DB_before->GetBinContent(i);
    double frac_ttbar = (h_TTbar_before->GetBinContent(i) + h_ST_before->GetBinContent(i)) / mc;

    double datattbarsf = 1.;
    if(mc > 0) datattbarsf = (data-mc)/(mc*frac_ttbar) + 1; // correct formula, checked it on paper
    ttbarsfs.emplace_back(datattbarsf);

    // subtract fake portion from data (before scaling ttbar)
    double fake = h_DY_before->GetBinContent(i) + h_WJ_before->GetBinContent(i);
    double genu = h_TTbar_before->GetBinContent(i) + h_ST_before->GetBinContent(i) + h_DB_before->GetBinContent(i);
    double fake_after = h_DY_after->GetBinContent(i) + h_WJ_after->GetBinContent(i);
    double genu_after = h_TTbar_after->GetBinContent(i) + h_ST_after->GetBinContent(i) + h_DB_after->GetBinContent(i);

    double sf = 1.;
    if(fake+genu > 0.) sf = data/(fake+genu);

    //subtract fake*sf from data
    h_DATA_before->SetBinContent(i, data - fake*sf);
    h_DATA_after ->SetBinContent(i, h_DATA_after->GetBinContent(i) - fake_after*sf);
  }

  for(size_t i=0; i<ttbarsfs.size(); i++){
    cout << "bin: " << i+1 << ", ttbarsf: " << ttbarsfs[i] << endl;
    h_TTbar_before->SetBinContent(i+1, h_TTbar_before->GetBinContent(i+1) * ttbarsfs[i]);
    h_TTbar_after->SetBinContent(i+1, h_TTbar_after->GetBinContent(i+1) * ttbarsfs[i]);
    h_TTbar_before->SetBinError(i+1, h_TTbar_before->GetBinError(i+1) * ttbarsfs[i]);
    h_TTbar_after->SetBinError(i+1, h_TTbar_after->GetBinError(i+1) * ttbarsfs[i]);
    h_ST_before->SetBinContent(i+1, h_ST_before->GetBinContent(i+1) * ttbarsfs[i]);
    h_ST_after->SetBinContent(i+1, h_ST_after->GetBinContent(i+1) * ttbarsfs[i]);
    h_ST_before->SetBinError(i+1, h_ST_before->GetBinError(i+1) * ttbarsfs[i]);
    h_ST_after->SetBinError(i+1, h_ST_after->GetBinError(i+1) * ttbarsfs[i]);
  }

  TH1D* h_MC_before = (TH1D*)h_TTbar_before->Clone("h_MC_before");
  h_MC_before->Add(h_ST_before);
  h_MC_before->Add(h_DB_before);
  TH1D* h_FAKE_before = (TH1D*)h_DY_before->Clone("h_FAKE_before");
  h_FAKE_before->Add(h_WJ_before);

  TH1D* h_MC_after = (TH1D*)h_TTbar_after->Clone("h_MC_after");
  h_MC_after->Add(h_ST_after);
  h_MC_after->Add(h_DB_after);
  TH1D* h_FAKE_after = (TH1D*)h_DY_after->Clone("h_FAKE_after");
  h_FAKE_after->Add(h_WJ_after);




  // Calculate efficiencies and SFs
  // ==============================

  // Efficiencies
  TGraphAsymmErrors* gr_mc;
  for(int i=-1; i<h_MC_before->GetNbinsX()+1; i++){
    // cout << "Bin: " << i+1 << ", Data before: " << h_DATA_before->GetBinContent(i+1) << " +- " << h_DATA_before->GetBinError(i+1) << " ---- Data after: " << h_DATA_after->GetBinContent(i+1) << " +- " << h_DATA_after->GetBinError(i+1) << endl;
    if(h_MC_before->GetBinContent(i+1) < h_MC_after->GetBinContent(i+1)){
      h_MC_after->SetBinContent(i+1, h_MC_before->GetBinContent(i+1));
      h_MC_after->SetBinError(i+1, h_MC_before->GetBinError(i+1));
    }
    if(h_DATA_before->GetBinContent(i+1) < h_DATA_after->GetBinContent(i+1)){
      h_DATA_after->SetBinContent(i+1, h_DATA_before->GetBinContent(i+1));
      h_DATA_after->SetBinError(i+1, h_DATA_before->GetBinError(i+1));
    }
    if(h_MC_before->GetBinContent(i+1) <= 0) h_MC_before->SetBinContent(i+1, 0.);
    if(h_MC_after->GetBinContent(i+1) <= 0) h_MC_after->SetBinContent(i+1, 0.);
    // cout << "MC bin " << i+1 << ", before: " << h_MC_before->GetBinContent(i+1) << ", after: " << h_MC_after->GetBinContent(i+1) << endl;
    // cout << "DATA bin " << i+1 << ", before: " << h_DATA_before->GetBinContent(i+1) << ", after: " << h_DATA_after->GetBinContent(i+1) << endl;
    if(h_MC_before->GetBinContent(i+1) < h_MC_after->GetBinContent(i+1)) cout << "+++++++++++++++ MC in bin " << i+1 << ", before < after" << endl;
    if(h_DATA_before->GetBinContent(i+1) < h_DATA_after->GetBinContent(i+1)) cout << "+++++++++++++++ DATA in bin " << i+1 << ", before < after, setting them equal" << endl;
  }
  // cout << "Trying for mc" << endl;
  gr_mc = new TGraphAsymmErrors(h_MC_after, h_MC_before, "cl=0.683 b(1,1) mode");
  TGraphAsymmErrors* gr_data = new TGraphAsymmErrors(h_DATA_after, h_DATA_before, "cl=0.683 b(1,1) mode");
  HistCosmetics(gr_mc);
  HistCosmetics(gr_data);
  gr_mc->SetLineColor(kRed-4);
  gr_data->SetLineColor(kBlack);
  for(int i=0; i<gr_data->GetN(); i++){
    double tmx, tmy;
    gr_data->GetPoint(i, tmx, tmy);
    cout << "bin: " << i << ", eff in data: " << tmy << endl;
    gr_mc->GetPoint(i, tmx, tmy);
    cout << "bin: " << i << ", eff in mc: " << tmy << endl;
  }


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
    // gr_data->GetPoint(at_in_data,x_DATA,y_DATA);

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
    // cout << "y_data: " << y_DATA << " + " << DATA_y_high << " - " << DATA_y_low << endl;
    // cout << "y_mc: " << y_MC << " + " << MC_y_high << " - " << MC_y_low << endl;
    SF_y_low.push_back(sqrt(pow(DATA_y_low/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_high,2)));
    SF_y_high.push_back(sqrt(pow(DATA_y_high/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_low,2)));

    // cout << "x-data: " << x_DATA << ", x-mc: " << x_MC << ", x-sf: " << SF_x.back() << endl;
    // cout << "y-data: " << y_DATA << ", y-mc: " << y_MC << ", y-sf: " << SF_y.back() << endl;
    at_in_mc++;
    at_in_data++;
    n_points++;

  }

  TGraphAsymmErrors* gr_ratio = new TGraphAsymmErrors(SF_x.size(), &SF_x[0], &SF_y[0], &SF_x_low[0], &SF_x_high[0], &SF_y_low[0], &SF_y_high[0]);
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
  gr_ratio->SetName("ScaleFactors");
  gr_ratio->Draw("APZ");

  TLine* l_unity = new TLine(gr_ratio->GetXaxis()->GetXmin(), 1, gr_ratio->GetXaxis()->GetXmax(), 1);
  l_unity->SetLineColor(kRed-4);
  l_unity->SetLineWidth(2);
  l_unity->SetLineStyle(2);
  l_unity->Draw("SAME");

  p_top->cd();
  TString infotext = infotexts.at(year);
  TLatex *text1 = new TLatex(3.5, 24, infotext);
  text1->SetNDC();
  text1->SetTextAlign(33);
  text1->SetX(0.94);
  text1->SetTextFont(43);
  text1->SetTextSize(16);
  text1->SetY(0.995);
  text1->Draw("SAME");

  TString ptlowfortag = etalow_str;
  TString pttext = ptlowfortag + " GeV < p_{T}^{e} < " + etahigh_str + " GeV";
  if(mode.Contains("_eta")){
    pttext.ReplaceAll("p_{T}^{e}", "|#eta^{e}|");
    pttext.ReplaceAll(" GeV", "");
    pttext.ReplaceAll("p", ".");
  }
  if(mode.Contains("muon")) pttext.ReplaceAll("^{e}", "^{#mu}");


  TLatex *text2 = new TLatex(3.5, 24, pttext);
  text2->SetNDC();
  // text2->SetTextAlign(33);
  text2->SetX(0.3);
  text2->SetTextFont(43);
  text2->SetTextSize(14);
  text2->SetY(0.32);
  text2->Draw("SAME");

  gErrorIgnoreLevel = kInfo;
  TString plotname = "";
  if(mode.Contains("electron"))  plotname += "Electron";
  else if(mode.Contains("muon")) plotname += "Muon";
  plotname += "TriggerEfficiency_" + varname;
  TString modeDiscriminator = "_pt";
  if(mode.Contains("eta")) modeDiscriminator = "_eta";
  plotname += modeDiscriminator;
  plotname += etalow_str + "to" + etahigh_str+ "_TTbarPTReweighted";

  // c->SaveAs(base_path + year + "/" + trigger_tag + plotname + ".eps");
  c->SaveAs(base_path + year + "/" + trigger_tag + plotname + ".pdf");



  unique_ptr<TFile> fout;
  TString outfilename = base_path + year + "/" + trigger_tag + plotname + ".root";
  outfilename.ReplaceAll("TriggerEfficiency", "TriggerScaleFactors");
  fout.reset(new TFile(outfilename, "RECREATE"));
  gr_ratio->Write();
  fout->Close();


}


void CombineSysts(TString mode, TString varname, TString base_path, TString trigger_tag, TString year, TString yeartag, TString uhh2_path, double etalow, double etahigh){


  map<double, TString> etamap_string = {
    make_pair(0., "0"),
    make_pair(0.9, "0p9"),
    make_pair(1.2, "1p2"),
    make_pair(2.1, "2p1"),
    make_pair(2.4, "2p4"),
  };

  TString filename_nominal = base_path + year + "/" + trigger_tag + "MuonTriggerScaleFactors_" + varname + "_eta";
  filename_nominal += etamap_string[etalow];
  filename_nominal += "to";
  filename_nominal += etamap_string[etahigh];
  filename_nominal += ".root";
  cout << "filename_nominal: " << filename_nominal << endl;
  TString filename_toreplace = filename_nominal;

  TString filename_withDY = filename_toreplace.ReplaceAll(".root", "_withDYsyst.root");
  cout << "filename_withDY:  " << filename_withDY << endl;
  filename_toreplace = filename_nominal;

  TString filename_withPT = filename_toreplace.ReplaceAll(".root", "_TTbarPTReweighted.root");
  cout << "filename_withPT:  " << filename_withPT << endl;

  unique_ptr<TFile> f_nom, f_dy, f_pt;
  f_nom.reset(new TFile(filename_nominal, "READ"));
  f_dy.reset(new TFile(filename_withDY, "READ"));
  f_pt.reset(new TFile(filename_withPT, "READ"));

  TString graphname = "ScaleFactors";
  unique_ptr<TGraphAsymmErrors> g_nom, g_dy, g_pt;
  g_nom.reset((TGraphAsymmErrors*)f_nom->Get(graphname));
  g_dy.reset((TGraphAsymmErrors*)f_dy->Get(graphname));
  g_pt.reset((TGraphAsymmErrors*)f_pt->Get(graphname));

  // systematic due to PT is: |nominal - withpt|, still to be done. Then, add quadratically to "total" uncertainty in "withdy"
  vector<double> x, y, x_lo, x_hi, y_lo, y_hi;
  for(int i=0; i<g_nom->GetN(); i++){
    double myx, nom, withpt;
    g_nom->GetPoint(i, myx, nom);
    g_pt->GetPoint(i, myx, withpt);
    double syst_pt = fabs(nom - withpt);
    cout << "nom: " << nom << ", withpt: " << withpt << ", resulting systematic: " << syst_pt << endl;

    x.emplace_back(myx);
    y.emplace_back(nom);
    x_lo.emplace_back(g_nom->GetErrorXlow(i));
    x_hi.emplace_back(g_nom->GetErrorXhigh(i));
    y_lo.emplace_back(sqrt(pow(g_dy->GetErrorYlow(i), 2) + pow(syst_pt, 2)));
    y_hi.emplace_back(sqrt(pow(g_dy->GetErrorYhigh(i), 2) + pow(syst_pt, 2)));
  }

  unique_ptr<TGraphAsymmErrors> g_total;
  g_total.reset(new TGraphAsymmErrors(x.size(), &x[0], &y[0], &x_lo[0], &x_hi[0], &y_lo[0], &y_hi[0]));

  HistCosmetics(g_nom.get());
  HistCosmetics(g_dy.get());
  HistCosmetics(g_total.get());
  g_nom->SetLineColor(kBlue);
  g_dy->SetLineColor(kRed);
  g_total->SetLineColor(kBlack);
  g_total->SetName("ScaleFactors");

  // plot old and new SFs in one plot
  TCanvas* c = new TCanvas("c", "c", 400, 400);
  TPad* p = SetupPad();
  p->Draw();
  p->cd();

  g_total->GetHistogram()->GetYaxis()->SetRangeUser(0.75, 1.25);
  g_total->GetHistogram()->GetYaxis()->SetTitle("Muon trigger SF");
  g_total->GetHistogram()->GetXaxis()->SetTitle("p_{T}^{#mu} [GeV]");
  g_total->Draw("AP");
  g_dy->Draw("P SAME");
  g_nom->Draw("P SAME");


  TString etalow_str = etamap_string[etalow];
  TString etahigh_str = etamap_string[etahigh];
  TLegend* leg = new TLegend(0.5,0.7,0.9,0.9);
  TString headertext = "";
  headertext += etalow_str;
  headertext += " < |#eta^{#mu}| < ";
  headertext += etahigh_str;
  headertext.ReplaceAll("p", ".");
  leg->SetHeader(headertext);
  leg->AddEntry(g_nom.get(),"Statistical uncertainty only","e");
  leg->AddEntry(g_dy.get(),"Stat. + difference to official SF","e");
  leg->AddEntry(g_total.get(),"Total uncertainty","e");
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



  TString plotname = base_path + year + "/" + trigger_tag + "MuonTriggerScaleFactors_" + varname + "_eta" + etalow_str + "to" + etahigh_str + "_Comparison.pdf";
  c->SaveAs(plotname);

  TString outfilename = plotname.ReplaceAll("_Comparison.pdf", "_withsyst.root");
  unique_ptr<TFile> fout;
  fout.reset(new TFile(outfilename, "RECREATE"));
  g_total->Write();
  fout->Close();

}
