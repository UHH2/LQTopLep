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
#include <TFitResult.h>
#include <fstream>

using namespace std;

void FindRMS(TString infolder, map<TString, TString> samplemap, TString sample);

void AnalysisTool::PDFRMS(){

  map<TString, TString> samplemap;
  samplemap["TTbar_2016v3"] = "uhh2.AnalysisModuleRunner.MC.TTbar_2016v3.root";
  samplemap["DYJets_2016v3"] = "uhh2.AnalysisModuleRunner.MC.DYJets_2016v3.root";
  samplemap["Diboson_2016v3"] = "uhh2.AnalysisModuleRunner.MC.Diboson_2016v3.root";
  samplemap["QCDMu_2016v3"] = "uhh2.AnalysisModuleRunner.MC.QCDMu_2016v3.root";
  samplemap["TTV_2016v3"] = "uhh2.AnalysisModuleRunner.MC.TTV_2016v3.root";
  samplemap["WJets_2016v3"] = "uhh2.AnalysisModuleRunner.MC.WJets_2016v3.root";
  samplemap["SingleTop_2016v3"] = "uhh2.AnalysisModuleRunner.MC.SingleTop_2016v3.root";
  samplemap["LQtoTMuM200_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM200_2016v3.root";
  samplemap["LQtoTMuM300_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM300_2016v3.root";
  samplemap["LQtoTMuM400_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM400_2016v3.root";
  samplemap["LQtoTMuM500_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM500_2016v3.root";
  samplemap["LQtoTMuM600_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM600_2016v3.root";
  samplemap["LQtoTMuM700_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM700_2016v3.root";
  samplemap["LQtoTMuM800_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM800_2016v3.root";
  samplemap["LQtoTMuM900_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM900_2016v3.root";
  samplemap["LQtoTMuM1000_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM1000_2016v3.root";
  samplemap["LQtoTMuM1200_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM1200_2016v3.root";
  samplemap["LQtoTMuM1400_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM1400_2016v3.root";
  samplemap["LQtoTMuM1700_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM1700_2016v3.root";
  samplemap["LQtoTMuM2000_2016v3"] = "uhh2.AnalysisModuleRunner.MC.LQtoTMuM2000_2016v3.root";

  TString infolder = AnalysisTool::base_path + AnalysisTool::year + "/" + AnalysisTool::final_tag;

  FindRMS(infolder, samplemap, "TTbar_2016v3");
  FindRMS(infolder, samplemap, "DYJets_2016v3");
  FindRMS(infolder, samplemap, "Diboson_2016v3");
  FindRMS(infolder, samplemap, "QCDMu_2016v3");
  FindRMS(infolder, samplemap, "TTV_2016v3");
  FindRMS(infolder, samplemap, "WJets_2016v3");
  FindRMS(infolder, samplemap, "SingleTop_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM200_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM300_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM400_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM500_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM600_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM700_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM800_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM900_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM1000_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM1200_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM1400_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM1700_2016v3");
  FindRMS(infolder, samplemap, "LQtoTMuM2000_2016v3");

}


void FindRMS(TString infolder, map<TString, TString> samplemap, TString sample){
  gStyle->SetOptStat(0);

  // Open File
  // ==========

  // infiles
  TString infilename = infolder +"NOMINAL/"+ samplemap[sample];
  TFile* infile = new TFile(infilename, "READ");

  // outfiles
  TString outfilename = infolder + "pdf/" + sample + ".root";
  TFile* outfile = new TFile(outfilename, "RECREATE");




  // Loop through the entire file and subtract minor backgrounds from data in every single histogram in the "General"-folder (!)
  // ===========================================================================================================================

  // get list of foldernames: The PDF folders have "pdf" in their name
  infile->cd();
  vector<TString> foldernames;
  TDirectory* dir = gDirectory;
  TIter iter(dir->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)iter())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if(cl->InheritsFrom("TDirectoryFile")){
      TString name = key->ReadObj()->GetName();
      if(name != "SFrame" && name.Contains("_pdf")){
        foldernames.emplace_back(name);

        // make dirs without "pdf"
        TString makedirname = name;
        makedirname.ReplaceAll("_pdf", "_pdf_up");
        outfile->mkdir(makedirname);
        makedirname.ReplaceAll("_pdf_up", "_pdf_down");
        outfile->mkdir(makedirname);
      }
    }
  }


  for(size_t i=0; i<foldernames.size(); i++){
    TString foldername = foldernames.at(i);

    // create list of histogram names (without the _xx tag for the number of the PDF variation)
    infile->cd(foldername);
    dir = gDirectory;
    iter = dir->GetListOfKeys();

    vector<TString> histnames;
    // Loop over all objects in this dir
    while ((key = (TKey*)iter())) {
      TString myclass = "TH1F";

      // Consider only TH1F objects
      if(!(key->GetClassName() == myclass)) continue;
      TString histname = key->ReadObj()->GetName();
      if(histname.Contains("_99")){
        TString histname_base = histname;
        histname_base.ReplaceAll("_99", "");
        histnames.emplace_back(histname_base);
      }
    }

    // go through all PDF variations, there have to be ==100 of them per histogram
    vector<vector<TH1F*>> histograms;
    vector<TH1F*> histograms_nom;
    for(size_t j=0; j<histnames.size(); j++){

      TString histname = histnames[j];


      // systematic histograms (100)
      vector<TH1F*> hists_thisvar;
      for(size_t k=1; k<101; k++){
        TString readoutname = foldername + "/" + histname + "_";
        readoutname += k;
        TH1F* h = (TH1F*)infile->Get(readoutname);
        hists_thisvar.emplace_back(h);
      }
      histograms.emplace_back(hists_thisvar);


      // nominal histograms (1)
      TString foldername_nom = foldername;
      foldername_nom.ReplaceAll("_pdf", "_nominal");
      TString histname_nom = histname;
      histname_nom.ReplaceAll("_PDF", "");
      TString readoutname = foldername_nom + "/" + histname_nom;
      TH1F* h = (TH1F*)infile->Get(readoutname);
      histograms_nom.emplace_back(h);
    }


    // Now calculate RMS in current folder, in each variable, in each bin
    // ===============================================================

    vector<TH1F*> hists_up, hists_down;

    // loop through variables
    for(size_t j=0; j<histograms_nom.size(); j++){
      TH1F* h_up = (TH1F*)histograms_nom[j]->Clone();
      TH1F* h_down = (TH1F*)histograms_nom[j]->Clone();

      // loop through bins
      for(int k=1; k<histograms_nom[j]->GetNbinsX()+1; k++){

        float nom = histograms_nom[j]->GetBinContent(k);

        // loop through 100 PDF histograms
        float rms = 0.;
        for(size_t l=0; l<histograms[j].size(); l++){
          if (sample.Contains("LQto") && k==1) histograms[j][l]->Scale(histograms_nom[j]->Integral()/histograms[j][l]->Integral());
          rms += pow(histograms[j][l]->GetBinContent(k) - nom, 2);
        }
        rms /= histograms[j].size()-1.;
        rms = sqrt(rms);

        h_up->SetBinContent(k, nom + rms);
        h_down->SetBinContent(k, max((float)0., nom - rms));
      }

      hists_up.emplace_back(h_up);
      hists_down.emplace_back(h_down);
    }


    // Write histograms into file, chosing the correct folder
    TString writefoldername = foldername;
    writefoldername.ReplaceAll("_pdf", "_pdf_up");
    outfile->cd(writefoldername);
    for(size_t j=0; j<hists_up.size(); j++){
      hists_up[j]->Write();
    }
    writefoldername.ReplaceAll("_pdf_up", "_pdf_down");
    outfile->cd(writefoldername);
    for(size_t j=0; j<hists_down.size(); j++){
      hists_down[j]->Write();
    }


  }
  outfile->Close();


  delete outfile;
  delete infile;

}
