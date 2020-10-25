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

void AnalysisTool::ProduceCombineHistograms_combinedLQsamples(TString branchingRatio){
  cout << "ProduceCombinedHistograms_combinedLQsamples, BR: " << branchingRatio << endl;		
  float BR = branchingRatio.Atof()/10;
  cout << "BR: " << BR << endl;

  cout << "year: " << AnalysisTool::yeartag << endl;

  
  // BR = 0.0, 0.1, ... 1.0 
  // BR = BR(LQ -> te) = 1 - BR(LQ -> tmu)

  TString year = AnalysisTool::yeartag;

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
    make_pair("LQtoTMuM2000", 1.55E-05),
    make_pair("LQtoTEM200",  6.06E+01),
    make_pair("LQtoTEM300",  8.04E+00),
    make_pair("LQtoTEM400",  1.74E+00),
    make_pair("LQtoTEM500",  4.96E-01),
    make_pair("LQtoTEM600",  1.69E-01),
    make_pair("LQtoTEM700",  6.48E-02),
    make_pair("LQtoTEM800",  2.73E-02),
    make_pair("LQtoTEM900",  1.23E-02),
    make_pair("LQtoTEM1000", 5.86E-03),
    make_pair("LQtoTEM1200", 1.50E-03),
    make_pair("LQtoTEM1400", 4.32E-04),
    make_pair("LQtoTEM1700", 7.73E-05),
    make_pair("LQtoTEM2000", 1.55E-05)

  };

  TString histfolder_base = "FinalSelection";

  vector<TString> systematics = {"nominal", "scale_TTbar", "scale_DYJets", "scale_Diboson", "scale_TTV", "scale_WJets", "scale_SingleTop", "pdf", "muid", "muiso", "mutrigger", "eleid", "elereco", "eletrigger", "pu", "btag_bc", "btag_udsg"};

  TString dir_base = AnalysisTool::base_path + AnalysisTool::year + "/";
  vector<TString> region_tags = {"catA", "catB"};
  vector<TString> histinname_base = {"MLQ_rebinlimit", "ST_rebinlimit"};
  vector<TString> histoutname_base = {"MLQ", "ST"};
  // vector<TString> channel_tags = {"ech", "much"};
  vector<TString> channel_tags;


  vector<TString> samples_base = {"LQtoTLM200", "LQtoTLM300", "LQtoTLM400", "LQtoTLM500", "LQtoTLM600", "LQtoTLM700", "LQtoTLM800", "LQtoTLM900", "LQtoTLM1000", "LQtoTLM1200", "LQtoTLM1400", "LQtoTLM1700", "LQtoTLM2000", "SingleTop", "TTbar", "DYJets", "Diboson", "QCDMu",/* "QCDEle",*/ "TTV", "WJets", "DATA"}; // QCDEle empty in 2017, 2018
  channel_tags = {"srmu", "srele", "ttbar", "dycrmu", "dycrele"};

  vector<TString> samples = {};
  for(size_t i=0; i<samples_base.size(); i++){
    if(samples_base[i] != "DATA") samples.emplace_back(samples_base[i] + "_" + AnalysisTool::yeartag);
    else samples.emplace_back(samples_base[i]);
  }

  vector<TString> syst_shift = {"up", "down"};
  vector<TString> syst_shift_combine = {"Up", "Down"};


  TString filename_base = "uhh2.AnalysisModuleRunner.";
  TString outfilename = AnalysisTool::combine_path + "input/combine_histograms_mc_fullsyst_BR_" + branchingRatio + "_" + AnalysisTool::yeartag + ".root";
  
  TFile* f_out = new TFile(outfilename, "RECREATE");


  for(unsigned int region=0; region<region_tags.size(); region++){
    for(unsigned int channel=0; channel<channel_tags.size(); channel++){
      if(channel_tags[channel] == "ttbar" && region_tags[region] == "catA") continue;
      cout << "============== Channel: " << channel_tags[channel] << ", Region: " << region_tags[region] << endl;
      for(unsigned int k=0; k<systematics.size(); k++){
        TString syst = systematics[k];
	cout << "============ Syst: " << syst << endl;

        for(unsigned int m=0; m<syst_shift.size(); m++){
          TString dir_nom = dir_base;
          TString dir_scale = dir_base;
          TString dir_pdf = dir_base;
          TString dir_jerc = dir_base;

          if(syst == "nominal" && m > 0) break;
          dir_nom += AnalysisTool::final_tag + "NOMINAL/";
          dir_jerc += AnalysisTool::full_tag + syst + "_" + syst_shift[m] + "/";
          dir_pdf += AnalysisTool::final_tag + "pdf/";
          dir_scale += AnalysisTool::final_tag + "scale/";

          for(unsigned int i=0; i<samples.size(); i++){
            bool force_nominal = false;
            TString sample_in = samples[i];
            TString sample_out = samples[i];
            if(sample_out == "DATA") sample_out = "data_obs_" + AnalysisTool::yeartag;
	    //cout << "-- Sample: " << sample_in << endl;

            if(sample_in == "DATA" && syst != "nominal") force_nominal = true;
            if(!sample_in.Contains("TTbar") && syst == "scale_TTbar") force_nominal = true;
            if(!sample_in.Contains("SingleTop") && syst == "scale_SingleTop") force_nominal = true;
            if(!sample_in.Contains("DYJets") && syst == "scale_DYJets") force_nominal = true;
            if(!sample_in.Contains("Diboson") && syst == "scale_Diboson") force_nominal = true;
            if(!sample_in.Contains("TTV") && syst == "scale_TTV") force_nominal = true;
            if(!sample_in.Contains("WJets") && syst == "scale_WJets") force_nominal = true;

            TString type = "MC.";
            if(sample_in == "DATA") type = "DATA.";

            if(sample_in == "DATA"){
              if((channel_tags[channel] == "srele") || (channel_tags[channel] == "dycrele")) sample_in += "_Electron";
              else sample_in += "_Muon";
              sample_in += "_" + AnalysisTool::yeartag;
            }

            TString filename = dir_nom + filename_base + type + sample_in + ".root";
            if(!force_nominal){
              if(syst.Contains("scale_")) filename = dir_scale + sample_in + ".root";
              if(syst == "pdf") filename = dir_pdf + sample_in + ".root";
              // else if((syst == "JEC" || syst == "JER") && (sample_in.Contains("TTbar") || sample_in.Contains("SingleTop"))){
              //
              //
              //   filename = dir_jerc + sample_in + "_" + channel_tags[channel] + "_" + region_tags[region] + ".root";
              //   // cout << "filename: " << filename << endl;
              //
              // }
            }

	    

            TString histname_in = "";
            histname_in = histfolder_base + "_" + channel_tags[channel] + "_" + region_tags[region] + "_" + syst;
            if(force_nominal) histname_in = histfolder_base + "_" + channel_tags[channel] + "_" + region_tags[region] + "_nominal";
            if(syst != "nominal" && !force_nominal) histname_in += "_" + syst_shift[m];
            histname_in += "/" + histinname_base[region];
            // if(syst == "JEC" || syst == "JER") histname_in = histfolder_base + "_" + region_tags[region] + "/" + histinname_base[region];
            if(syst.Contains("scale") && !force_nominal) histname_in = histfolder_base + "_" + channel_tags[channel] + "_" + region_tags[region] + "_scale_" + syst_shift[m] +  "/" + histinname_base[region];

            TString histname_out =  histoutname_base[region] + "_" + channel_tags[channel] + "_" + region_tags[region] + "__" + sample_out;
            if(syst != "nominal") histname_out += "__" + syst + syst_shift_combine[m];


	    TFile* f_in;
	    TH1F* h_in;

	    if(sample_in.Contains("LQ")) {
	      vector<TString> signal_base = {"LQtoTEM", "LQtoTMuM", "LQtoTETMuM"};
	      // open LQtoTEM histogram to get binning
	      filename.ReplaceAll("LQtoTLM", "LQtoTEM");
	      f_in = new TFile(filename, "READ");
	      h_in = (TH1F*)f_in->Get(histname_in);

	      // get the 3 signal histograms
	      vector<TFile*> signal_infile;
	      vector<TH1F*> signal_hists;
	      for(unsigned int j=0; j<signal_base.size(); j++) {
		//cout << "filename: " << filename << endl;
		TFile* f1 = new TFile(filename, "READ");
		//signal_infile.emplace_back(f1);
		TH1F* h1 = (TH1F*)f1->Get(histname_in);
		h1->SetDirectory(0); // to decouple it from the open file directory
		signal_hists.emplace_back(h1);
		filename.ReplaceAll(signal_base[j], signal_base[(j+1)%3]);
		f1->Close();
	      }
	      // loop through bins
	      for(int l=1; l<h_in->GetNbinsX()+1; l++){
		vector<float> binContent;
		// loop over signal_base
		for(unsigned int j=0; j<signal_base.size(); j++) {
		  //cout << "Line: " << __LINE__ << endl;
		  binContent.emplace_back(signal_hists[j]->GetBinContent(l));
		  // cout << "binContent " << j << ": "  << binContent[j] << endl;
		}
		// LQLQ -> tltl = B²*(LQLQ->tete) + (1-B)²*(LQLQ->tmutmu) + (1-B²-(1-B)²)*(LQLQ->tetmu)
		float reweight = pow(BR, 2)*binContent[0] + pow(1-BR, 2)*binContent[1] + (1 - pow(BR, 2) - pow(1-BR, 2))*binContent[2];
		//cout << "reweight: " << reweight << endl;
		h_in->SetBinContent(l, reweight);
	      }
	      //for(unsigned int j=0; j<signal_base.size(); j++) {
	      //signal_infile[j]->Close();
	      //}
	    }
	    else{
	      f_in = new TFile(filename, "READ");
	      h_in = (TH1F*)f_in->Get(histname_in);
            }
	    // cout << "-- Sample_in: " << sample_in << endl;


	    if(sample_in.Contains("M200_") || sample_in.Contains("M300_") || sample_in.Contains("M400_")) {
              h_in->Scale(1./10.);
            }
	    // if(sample_in.Contains("M2000_")) h_in->Scale(100.);
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
