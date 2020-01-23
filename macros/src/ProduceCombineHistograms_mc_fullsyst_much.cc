#include "../include/cosmetics.h"
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
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TROOT.h>
#include <TKey.h>
#include <TLatex.h>
#include <TClass.h>
#include <fstream>

using namespace std;

void AnalysisTool::ProduceCombineHistograms_mc_fullsyst_much(){

  map<TString, float> xsecs = {
    make_pair("LQtoTMuM200",  6.06E+01),
    make_pair("LQtoTMuM300",  8.04E+00),
    make_pair("LQtoTMuM400",  1.74E+00),
    make_pair("LQtoTMuM500",  4.96E-01),
    make_pair("LQtoTMuM600",  1.69E-01),
    make_pair("LQtoTMuM700",  6.48E-02),
    make_pair("LQtoTMuM800",  2.73E-02),
    make_pair("LQtoTMuM900",  1.23E-02),
    make_pair("LQtoTMuM1000", 5.86E-03),
    make_pair("LQtoTMuM1200", 1.50E-03),
    make_pair("LQtoTMuM1400", 4.32E-04),
    make_pair("LQtoTMuM1700", 7.73E-05),
    make_pair("LQtoTMuM2000", 1.55E-05)
  };

  vector<TString> samples_base = {"LQtoTMuM200", "LQtoTMuM300", "LQtoTMuM400", "LQtoTMuM500", "LQtoTMuM600", "LQtoTMuM700", "LQtoTMuM800", "LQtoTMuM900", "LQtoTMuM1000", "LQtoTMuM1200", "LQtoTMuM1400", "LQtoTMuM1700", "LQtoTMuM2000", "SingleTop", "TTbar", "DYJets", "Diboson", "QCDMu", "TTV", "WJets", "DATA"};
  vector<TString> samples = {};
  for(size_t i=0; i<samples_base.size(); i++){
    if(samples_base[i] != "DATA") samples.emplace_back(samples_base[i] + "_" + AnalysisTool::yeartag);
    else samples.emplace_back(samples_base[i]);
  }

  TString histfolder_base = "FinalSelection";

  vector<TString> systematics = {"nominal", "scale_TTbar", "scale_DYJets", "scale_Diboson", "scale_TTV", "scale_WJets", "scale_SingleTop", "pdf", "muid", "muiso", "mutrigger", "eleid", "elereco", "pu", "btag_bc", "btag_udsg"};

  TString dir_base = AnalysisTool::base_path + AnalysisTool::year + "/";
  vector<TString> region_tags = {"catA", "catB"};
  vector<TString> histinname_base = {"MLQ_rebinlimit", "ST_rebinlimit"};
  vector<TString> histoutname_base = {"MLQ", "ST"};
  // vector<TString> channel_tags = {"ech", "much"};
  vector<TString> channel_tags = {"much"};


  vector<TString> syst_shift = {"up", "down"};
  vector<TString> syst_shift_combine = {"Up", "Down"};


  TString filename_base = "uhh2.AnalysisModuleRunner.";

  TString outfilename = AnalysisTool::theta_path + "input/combine_histograms_mc_fullsyst_much.root";
  TFile* f_out = new TFile(outfilename, "RECREATE");


  for(unsigned int region=0; region<region_tags.size(); region++){
    for(unsigned int channel=0; channel<channel_tags.size(); channel++){

      for(unsigned int k=0; k<systematics.size(); k++){
        TString syst = systematics[k];
        cout << "============ Syst: " << syst << endl;

        for(unsigned int m=0; m<syst_shift.size(); m++){
          TString dir = dir_base;
          if(syst == "nominal" && m > 0) break;

          if(!(syst == "JEC" || syst == "JER" || syst == "pdf" || syst.Contains("scale"))) dir += "Finalselection/MuonChannel/NOMINAL/";
          else if(syst == "JEC" || syst == "JER") dir += "Fullselection/MuonChannel/" + syst + "_" + syst_shift[m] + "/";
          else if(syst == "pdf") dir += "Finalselection/MuonChannel/" + syst + "/";
          else if(syst.Contains("scale")) dir += "Finalselection/MuonChannel/scale/";


          for(unsigned int i=0; i<samples.size(); i++){
            TString sample_in = samples[i];
            TString sample_out = samples[i];
            if(sample_out == "DATA") sample_out = "data_obs" + AnalysisTool::yeartag;
            cout << "-- Sample: " << sample_in << endl;

            if(sample_in == "DATA" && syst != "nominal") continue;
            if(!sample_in.Contains("TTbar") && syst == "scale_TTbar") continue;
            if(!sample_in.Contains("SingleTop") && syst == "scale_SingleTop") continue;
            if(!sample_in.Contains("DYJets") && syst == "scale_DYJets") continue;
            if(!sample_in.Contains("Diboson") && syst == "scale_Diboson") continue;
            if(!sample_in.Contains("TTV") && syst == "scale_TTV") continue;
            if(!sample_in.Contains("WJets") && syst == "scale_WJets") continue;

            TString type = "MC.";
            if(sample_in == "DATA") type = "DATA.";

            if(sample_in == "DATA"){
              if(channel_tags[channel] == "ech") sample_in += "_Electron";
              else sample_in += "_Muon";
              sample_in += "_" + AnalysisTool::yeartag;
            }

            TString filename = dir + filename_base + type + sample_in + ".root";
            if(syst.Contains("scale_")) filename = dir + sample_in + ".root";
            if(syst == "pdf") filename = dir + sample_in + ".root";
            // else if((syst == "JEC" || syst == "JER") && (sample_in.Contains("TTbar") || sample_in.Contains("SingleTop"))){
            //
            //
            //   filename = dir + sample_in + "_" + channel_tags[channel] + "_" + region_tags[region] + ".root";
            //   // cout << "filename: " << filename << endl;
            //
            // }
            TFile* f_in = new TFile(filename, "READ");

            TString histname_in =  histfolder_base + "_" + region_tags[region] + "_" + syst;
            if(syst != "nominal") histname_in += "_" + syst_shift[m];
            histname_in += "/" + histinname_base[region];
            // if(syst == "JEC" || syst == "JER") histname_in = histfolder_base + "_" + region_tags[region] + "/" + histinname_base[region];
            if(syst.Contains("scale")) histname_in = histfolder_base + "_" + region_tags[region] + "_scale_" + syst_shift[m] +  "/" + histinname_base[region];
            TString histname_out =  histoutname_base[region] + "_" + region_tags[region] + "__" + sample_out;
            if(syst != "nominal") histname_out += "__" + syst + syst_shift_combine[m];
            if(samples[i] == "DATA") histname_out.ReplaceAll("DATA", "data_obs_" + AnalysisTool::yeartag);

            TH1F* h_in = (TH1F*)f_in->Get(histname_in);
            h_in->SetName(histname_out);
            f_out->cd();
            h_in->Write();
            f_in->Close();
          }
        }
      }
    }
  }

  cout << "finished!" << endl;



}
