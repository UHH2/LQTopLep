#include "../include/Tools.h"
#include "../include/cosmetics.h"
#include <TH1D.h>
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>
#include <iostream>


using namespace std;


void CalculateEffForVar(TString varname, TString base_path, TString trigger_tag, TString year, TString yeartag){

cout << "yeartag: " << yeartag << endl;
unique_ptr<TFile> input_TTbar, input_DY, input_ST, input_WJ, input_DB, input_QCDMU, input_QCDELE, input_DATAMU, input_DATAELE;
input_TTbar.reset(new TFile(base_path + trigger_tag + year + "/Fullselection/uhh2.AnalysisModuleRunner.MC.TTbar_" + yeartag + ".root","READ"));
input_DY.reset(new TFile(base_path + trigger_tag + year + "/Fullselection/uhh2.AnalysisModuleRunner.MC.DYJets_" + yeartag + ".root","READ"));
input_ST.reset(new TFile(base_path + trigger_tag + year + "/Fullselection/uhh2.AnalysisModuleRunner.MC.SingleTop_" + yeartag + ".root","READ"));
input_WJ.reset(new TFile(base_path + trigger_tag + year + "/Fullselection/uhh2.AnalysisModuleRunner.MC.WJets_" + yeartag + ".root","READ"));
input_DB.reset(new TFile(base_path + trigger_tag + year + "/Fullselection/uhh2.AnalysisModuleRunner.MC.Diboson_" + yeartag + ".root","READ"));
input_QCDMU.reset(new TFile(base_path + trigger_tag + year + "/Fullselection/uhh2.AnalysisModuleRunner.MC.QCDMu_" + yeartag + ".root","READ"));
input_QCDELE.reset(new TFile(base_path + trigger_tag + year + "/Fullselection/uhh2.AnalysisModuleRunner.MC.QCDEle_" + yeartag + ".root","READ"));
input_DATAMU.reset(new TFile(base_path + trigger_tag + year + "/Fullselection/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_" + yeartag + ".root","READ"));
input_DATAELE.reset(new TFile(base_path + trigger_tag + year + "/Fullselection/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_" + yeartag + ".root","READ"));


//in fine pt-bins
// TH1D* h_pt_TTbar_before = (TH1D*)input_TTbar->Get("TagProbe_NoCuts/pt_ele_fine");
// TH1D* h_pt_ST_before = (TH1D*)input_ST->Get("TagProbe_NoCuts/pt_ele_fine");
// TH1D* h_pt_DY_before = (TH1D*)input_DY->Get("TagProbe_NoCuts/pt_ele_fine");
// TH1D* h_pt_WJ_before = (TH1D*)input_WJ->Get("TagProbe_NoCuts/pt_ele_fine");
// TH1D* h_pt_DB_before = (TH1D*)input_DB->Get("TagProbe_NoCuts/pt_ele_fine");
// TH1D* h_pt_QCD_before = (TH1D*)input_QCD->Get("TagProbe_NoCuts/pt_ele_fine");
// h_pt_TTbar_before->Add(h_pt_ST_before);
// h_pt_TTbar_before->Add(h_pt_DY_before);
// h_pt_TTbar_before->Add(h_pt_WJ_before);
// h_pt_TTbar_before->Add(h_pt_DB_before);
// h_pt_TTbar_before->Add(h_pt_QCD_before);
// TH1D* h_pt_TTbar_after  = (TH1D*)input_TTbar->Get("TagProbe_Trigger/pt_ele_fine");
// TH1D* h_pt_ST_after = (TH1D*)input_ST->Get("TagProbe_Trigger/pt_ele_fine");
// TH1D* h_pt_DY_after = (TH1D*)input_DY->Get("TagProbe_Trigger/pt_ele_fine");
// TH1D* h_pt_WJ_after = (TH1D*)input_WJ->Get("TagProbe_Trigger/pt_ele_fine");
// TH1D* h_pt_DB_after = (TH1D*)input_DB->Get("TagProbe_Trigger/pt_ele_fine");
// TH1D* h_pt_QCD_after = (TH1D*)input_QCD->Get("TagProbe_Trigger/pt_ele_fine");
// h_pt_TTbar_after->Add(h_pt_ST_after);
// h_pt_TTbar_after->Add(h_pt_DY_after);
// h_pt_TTbar_after->Add(h_pt_WJ_after);
// h_pt_TTbar_after->Add(h_pt_DB_after);
// h_pt_TTbar_after->Add(h_pt_QCD_after);
// TH1D* h_pt_DATA_before  = (TH1D*)input_DATA->Get("TagProbe_NoCuts/pt_ele_fine");
// TH1D* h_pt_DATA_after   = (TH1D*)input_DATA->Get("TagProbe_Trigger/pt_ele_fine");
cout << "done loading a block" << endl;

}

void AnalysisTool::CalculateTriggerEfficiencies(){
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  CalculateEffForVar("pt_ele_fine", AnalysisTool::base_path, AnalysisTool::trigger_tag, AnalysisTool::year, AnalysisTool::yeartag);


  //
  //
  // //in broad pt-bins
  // TH1D* h_pt_binned_TTbar_before = (TH1D*)input_TTbar->Get("TagProbe_NoCuts/pt_ele_binned");
  // TH1D* h_pt_binned_ST_before = (TH1D*)input_ST->Get("TagProbe_NoCuts/pt_ele_binned");
  // TH1D* h_pt_binned_DY_before = (TH1D*)input_DY->Get("TagProbe_NoCuts/pt_ele_binned");
  // TH1D* h_pt_binned_WJ_before = (TH1D*)input_WJ->Get("TagProbe_NoCuts/pt_ele_binned");
  // TH1D* h_pt_binned_DB_before = (TH1D*)input_DB->Get("TagProbe_NoCuts/pt_ele_binned");
  // TH1D* h_pt_binned_QCD_before = (TH1D*)input_QCD->Get("TagProbe_NoCuts/pt_ele_binned");
  // h_pt_binned_TTbar_before->Add(h_pt_binned_ST_before);
  // h_pt_binned_TTbar_before->Add(h_pt_binned_DY_before);
  // h_pt_binned_TTbar_before->Add(h_pt_binned_WJ_before);
  // h_pt_binned_TTbar_before->Add(h_pt_binned_DB_before);
  // h_pt_binned_TTbar_before->Add(h_pt_binned_QCD_before);
  // TH1D* h_pt_binned_TTbar_after  = (TH1D*)input_TTbar->Get("TagProbe_Trigger/pt_ele_binned");
  // TH1D* h_pt_binned_ST_after = (TH1D*)input_ST->Get("TagProbe_Trigger/pt_ele_binned");
  // TH1D* h_pt_binned_DY_after = (TH1D*)input_DY->Get("TagProbe_Trigger/pt_ele_binned");
  // TH1D* h_pt_binned_WJ_after = (TH1D*)input_WJ->Get("TagProbe_Trigger/pt_ele_binned");
  // TH1D* h_pt_binned_DB_after = (TH1D*)input_DB->Get("TagProbe_Trigger/pt_ele_binned");
  // TH1D* h_pt_binned_QCD_after = (TH1D*)input_QCD->Get("TagProbe_Trigger/pt_ele_binned");
  // h_pt_binned_TTbar_after->Add(h_pt_binned_ST_after);
  // h_pt_binned_TTbar_after->Add(h_pt_binned_DY_after);
  // h_pt_binned_TTbar_after->Add(h_pt_binned_WJ_after);
  // h_pt_binned_TTbar_after->Add(h_pt_binned_DB_after);
  // h_pt_binned_TTbar_after->Add(h_pt_binned_QCD_after);
  // TH1D* h_pt_binned_DATA_before  = (TH1D*)input_DATA->Get("TagProbe_NoCuts/pt_ele_binned");
  // TH1D* h_pt_binned_DATA_after   = (TH1D*)input_DATA->Get("TagProbe_Trigger/pt_ele_binned");
  // cout << "done loading a block" << endl;
  //
  //
  // //plateau of lowpt trigger: 30-120
  // TH1D* h_lowpt_eta_TTbar_before = (TH1D*)input_TTbar->Get("TagProbe_Ele30to110/eta_ele_binned");
  // TH1D* h_lowpt_eta_ST_before = (TH1D*)input_ST->Get("TagProbe_Ele30to110/eta_ele_binned");
  // TH1D* h_lowpt_eta_DY_before = (TH1D*)input_DY->Get("TagProbe_Ele30to110/eta_ele_binned");
  // TH1D* h_lowpt_eta_WJ_before = (TH1D*)input_WJ->Get("TagProbe_Ele30to110/eta_ele_binned");
  // TH1D* h_lowpt_eta_DB_before = (TH1D*)input_DB->Get("TagProbe_Ele30to110/eta_ele_binned");
  // TH1D* h_lowpt_eta_QCD_before = (TH1D*)input_QCD->Get("TagProbe_Ele30to110/eta_ele_binned");
  // h_lowpt_eta_TTbar_before->Add(h_lowpt_eta_ST_before);
  // h_lowpt_eta_TTbar_before->Add(h_lowpt_eta_DY_before);
  // h_lowpt_eta_TTbar_before->Add(h_lowpt_eta_WJ_before);
  // h_lowpt_eta_TTbar_before->Add(h_lowpt_eta_DB_before);
  // h_lowpt_eta_TTbar_before->Add(h_lowpt_eta_QCD_before);
  // TH1D* h_lowpt_eta_TTbar_after  = (TH1D*)input_TTbar->Get("TagProbe_30to110Plateau/eta_ele_binned");
  // TH1D* h_lowpt_eta_ST_after = (TH1D*)input_ST->Get("TagProbe_30to110Plateau/eta_ele_binned");
  // TH1D* h_lowpt_eta_DY_after = (TH1D*)input_DY->Get("TagProbe_30to110Plateau/eta_ele_binned");
  // TH1D* h_lowpt_eta_WJ_after = (TH1D*)input_WJ->Get("TagProbe_30to110Plateau/eta_ele_binned");
  // TH1D* h_lowpt_eta_DB_after = (TH1D*)input_DB->Get("TagProbe_30to110Plateau/eta_ele_binned");
  // TH1D* h_lowpt_eta_QCD_after = (TH1D*)input_QCD->Get("TagProbe_30to110Plateau/eta_ele_binned");
  // h_lowpt_eta_TTbar_after->Add(h_lowpt_eta_ST_after);
  // h_lowpt_eta_TTbar_after->Add(h_lowpt_eta_DY_after);
  // h_lowpt_eta_TTbar_after->Add(h_lowpt_eta_WJ_after);
  // h_lowpt_eta_TTbar_after->Add(h_lowpt_eta_DB_after);
  // h_lowpt_eta_TTbar_after->Add(h_lowpt_eta_QCD_after);
  // TH1D* h_lowpt_eta_DATA_before  = (TH1D*)input_DATA->Get("TagProbe_Ele30to110/eta_ele_binned");
  // TH1D* h_lowpt_eta_DATA_after   = (TH1D*)input_DATA->Get("TagProbe_30to110Plateau/eta_ele_binned");
  // cout << "done loading a block" << endl;
  //
  //
  // //plateau of lowpt trigger: 30-50
  // TH1D* h_verylowpt_eta_TTbar_before = (TH1D*)input_TTbar->Get("TagProbe_Ele30to50/eta_ele_binned");
  // TH1D* h_verylowpt_eta_ST_before = (TH1D*)input_ST->Get("TagProbe_Ele30to50/eta_ele_binned");
  // TH1D* h_verylowpt_eta_DY_before = (TH1D*)input_DY->Get("TagProbe_Ele30to50/eta_ele_binned");
  // TH1D* h_verylowpt_eta_WJ_before = (TH1D*)input_WJ->Get("TagProbe_Ele30to50/eta_ele_binned");
  // TH1D* h_verylowpt_eta_DB_before = (TH1D*)input_DB->Get("TagProbe_Ele30to50/eta_ele_binned");
  // TH1D* h_verylowpt_eta_QCD_before = (TH1D*)input_QCD->Get("TagProbe_Ele30to50/eta_ele_binned");
  // h_verylowpt_eta_TTbar_before->Add(h_verylowpt_eta_ST_before);
  // h_verylowpt_eta_TTbar_before->Add(h_verylowpt_eta_DY_before);
  // h_verylowpt_eta_TTbar_before->Add(h_verylowpt_eta_WJ_before);
  // h_verylowpt_eta_TTbar_before->Add(h_verylowpt_eta_DB_before);
  // h_verylowpt_eta_TTbar_before->Add(h_verylowpt_eta_QCD_before);
  // TH1D* h_verylowpt_eta_TTbar_after  = (TH1D*)input_TTbar->Get("TagProbe_30to50Plateau/eta_ele_binned");
  // TH1D* h_verylowpt_eta_ST_after = (TH1D*)input_ST->Get("TagProbe_30to50Plateau/eta_ele_binned");
  // TH1D* h_verylowpt_eta_DY_after = (TH1D*)input_DY->Get("TagProbe_30to50Plateau/eta_ele_binned");
  // TH1D* h_verylowpt_eta_WJ_after = (TH1D*)input_WJ->Get("TagProbe_30to50Plateau/eta_ele_binned");
  // TH1D* h_verylowpt_eta_DB_after = (TH1D*)input_DB->Get("TagProbe_30to50Plateau/eta_ele_binned");
  // TH1D* h_verylowpt_eta_QCD_after = (TH1D*)input_QCD->Get("TagProbe_30to50Plateau/eta_ele_binned");
  // h_verylowpt_eta_TTbar_after->Add(h_verylowpt_eta_ST_after);
  // h_verylowpt_eta_TTbar_after->Add(h_verylowpt_eta_DY_after);
  // h_verylowpt_eta_TTbar_after->Add(h_verylowpt_eta_WJ_after);
  // h_verylowpt_eta_TTbar_after->Add(h_verylowpt_eta_DB_after);
  // h_verylowpt_eta_TTbar_after->Add(h_verylowpt_eta_QCD_after);
  // TH1D* h_verylowpt_eta_DATA_before  = (TH1D*)input_DATA->Get("TagProbe_Ele30to50/eta_ele_binned");
  // TH1D* h_verylowpt_eta_DATA_after   = (TH1D*)input_DATA->Get("TagProbe_30to50Plateau/eta_ele_binned");
  //
  //
  // //plateau of mediumpt trigger: 50-120
  // TH1D* h_mediumpt_eta_TTbar_before = (TH1D*)input_TTbar->Get("TagProbe_Ele50to110/eta_ele_binned");
  // TH1D* h_mediumpt_eta_ST_before = (TH1D*)input_ST->Get("TagProbe_Ele50to110/eta_ele_binned");
  // TH1D* h_mediumpt_eta_DY_before = (TH1D*)input_DY->Get("TagProbe_Ele50to110/eta_ele_binned");
  // TH1D* h_mediumpt_eta_WJ_before = (TH1D*)input_WJ->Get("TagProbe_Ele50to110/eta_ele_binned");
  // TH1D* h_mediumpt_eta_DB_before = (TH1D*)input_DB->Get("TagProbe_Ele50to110/eta_ele_binned");
  // TH1D* h_mediumpt_eta_QCD_before = (TH1D*)input_QCD->Get("TagProbe_Ele50to110/eta_ele_binned");
  // h_mediumpt_eta_TTbar_before->Add(h_mediumpt_eta_ST_before);
  // h_mediumpt_eta_TTbar_before->Add(h_mediumpt_eta_DY_before);
  // h_mediumpt_eta_TTbar_before->Add(h_mediumpt_eta_WJ_before);
  // h_mediumpt_eta_TTbar_before->Add(h_mediumpt_eta_DB_before);
  // h_mediumpt_eta_TTbar_before->Add(h_mediumpt_eta_QCD_before);
  // TH1D* h_mediumpt_eta_TTbar_after  = (TH1D*)input_TTbar->Get("TagProbe_50to110Plateau/eta_ele_binned");
  // TH1D* h_mediumpt_eta_ST_after = (TH1D*)input_ST->Get("TagProbe_50to110Plateau/eta_ele_binned");
  // TH1D* h_mediumpt_eta_DY_after = (TH1D*)input_DY->Get("TagProbe_50to110Plateau/eta_ele_binned");
  // TH1D* h_mediumpt_eta_WJ_after = (TH1D*)input_WJ->Get("TagProbe_50to110Plateau/eta_ele_binned");
  // TH1D* h_mediumpt_eta_DB_after = (TH1D*)input_DB->Get("TagProbe_50to110Plateau/eta_ele_binned");
  // TH1D* h_mediumpt_eta_QCD_after = (TH1D*)input_QCD->Get("TagProbe_50to110Plateau/eta_ele_binned");
  // h_mediumpt_eta_TTbar_after->Add(h_mediumpt_eta_ST_after);
  // h_mediumpt_eta_TTbar_after->Add(h_mediumpt_eta_DY_after);
  // h_mediumpt_eta_TTbar_after->Add(h_mediumpt_eta_WJ_after);
  // h_mediumpt_eta_TTbar_after->Add(h_mediumpt_eta_DB_after);
  // h_mediumpt_eta_TTbar_after->Add(h_mediumpt_eta_QCD_after);
  // TH1D* h_mediumpt_eta_DATA_before  = (TH1D*)input_DATA->Get("TagProbe_Ele50to110/eta_ele_binned");
  // TH1D* h_mediumpt_eta_DATA_after   = (TH1D*)input_DATA->Get("TagProbe_50to110Plateau/eta_ele_binned");
  //
  //
  // //plateau of only non-lowpt trigger
  // TH1D* h_highpt_eta_TTbar_before = (TH1D*)input_TTbar->Get("TagProbe_Ele110/eta_ele_binned");
  // TH1D* h_highpt_eta_ST_before = (TH1D*)input_ST->Get("TagProbe_Ele110/eta_ele_binned");
  // TH1D* h_highpt_eta_DY_before = (TH1D*)input_DY->Get("TagProbe_Ele110/eta_ele_binned");
  // TH1D* h_highpt_eta_WJ_before = (TH1D*)input_WJ->Get("TagProbe_Ele110/eta_ele_binned");
  // TH1D* h_highpt_eta_DB_before = (TH1D*)input_DB->Get("TagProbe_Ele110/eta_ele_binned");
  // TH1D* h_highpt_eta_QCD_before = (TH1D*)input_QCD->Get("TagProbe_Ele110/eta_ele_binned");
  // h_highpt_eta_TTbar_before->Add(h_highpt_eta_ST_before);
  // h_highpt_eta_TTbar_before->Add(h_highpt_eta_DY_before);
  // h_highpt_eta_TTbar_before->Add(h_highpt_eta_WJ_before);
  // h_highpt_eta_TTbar_before->Add(h_highpt_eta_DB_before);
  // h_highpt_eta_TTbar_before->Add(h_highpt_eta_QCD_before);
  // TH1D* h_highpt_eta_TTbar_after  = (TH1D*)input_TTbar->Get("TagProbe_110Plateau/eta_ele_binned");
  // TH1D* h_highpt_eta_ST_after = (TH1D*)input_ST->Get("TagProbe_110Plateau/eta_ele_binned");
  // TH1D* h_highpt_eta_DY_after = (TH1D*)input_DY->Get("TagProbe_110Plateau/eta_ele_binned");
  // TH1D* h_highpt_eta_WJ_after = (TH1D*)input_WJ->Get("TagProbe_110Plateau/eta_ele_binned");
  // TH1D* h_highpt_eta_DB_after = (TH1D*)input_DB->Get("TagProbe_110Plateau/eta_ele_binned");
  // TH1D* h_highpt_eta_QCD_after = (TH1D*)input_QCD->Get("TagProbe_110Plateau/eta_ele_binned");
  // h_highpt_eta_TTbar_after->Add(h_highpt_eta_ST_after);
  // h_highpt_eta_TTbar_after->Add(h_highpt_eta_DY_after);
  // h_highpt_eta_TTbar_after->Add(h_highpt_eta_WJ_after);
  // h_highpt_eta_TTbar_after->Add(h_highpt_eta_DB_after);
  // h_highpt_eta_TTbar_after->Add(h_highpt_eta_QCD_after);
  // TH1D* h_highpt_eta_DATA_before  = (TH1D*)input_DATA->Get("TagProbe_Ele110/eta_ele_binned");
  // TH1D* h_highpt_eta_DATA_after   = (TH1D*)input_DATA->Get("TagProbe_110Plateau/eta_ele_binned");
  // cout << "done loading a block" << endl;
  //
  // // //plateau of lowpt trigger
  // TH1D* h_lowpt_dr_TTbar_before = (TH1D*)input_TTbar->Get("TagProbe_Ele30to110/dRmin_ele_jet");
  // TH1D* h_lowpt_dr_ST_before = (TH1D*)input_ST->Get("TagProbe_Ele30to110/dRmin_ele_jet");
  // TH1D* h_lowpt_dr_DY_before = (TH1D*)input_DY->Get("TagProbe_Ele30to110/dRmin_ele_jet");
  // TH1D* h_lowpt_dr_WJ_before = (TH1D*)input_WJ->Get("TagProbe_Ele30to110/dRmin_ele_jet");
  // TH1D* h_lowpt_dr_DB_before = (TH1D*)input_DB->Get("TagProbe_Ele30to110/dRmin_ele_jet");
  // TH1D* h_lowpt_dr_QCD_before = (TH1D*)input_QCD->Get("TagProbe_Ele30to110/dRmin_ele_jet");
  // h_lowpt_dr_TTbar_before->Add(h_lowpt_dr_ST_before);
  // h_lowpt_dr_TTbar_before->Add(h_lowpt_dr_DY_before);
  // h_lowpt_dr_TTbar_before->Add(h_lowpt_dr_WJ_before);
  // h_lowpt_dr_TTbar_before->Add(h_lowpt_dr_DB_before);
  // h_lowpt_dr_TTbar_before->Add(h_lowpt_dr_QCD_before);
  // TH1D* h_lowpt_dr_TTbar_after  = (TH1D*)input_TTbar->Get("TagProbe_30to110Plateau/dRmin_ele_jet");
  // TH1D* h_lowpt_dr_ST_after = (TH1D*)input_ST->Get("TagProbe_30to110Plateau/dRmin_ele_jet");
  // TH1D* h_lowpt_dr_DY_after = (TH1D*)input_DY->Get("TagProbe_30to110Plateau/dRmin_ele_jet");
  // TH1D* h_lowpt_dr_WJ_after = (TH1D*)input_WJ->Get("TagProbe_30to110Plateau/dRmin_ele_jet");
  // TH1D* h_lowpt_dr_DB_after = (TH1D*)input_DB->Get("TagProbe_30to110Plateau/dRmin_ele_jet");
  // TH1D* h_lowpt_dr_QCD_after = (TH1D*)input_QCD->Get("TagProbe_30to110Plateau/dRmin_ele_jet");
  // h_lowpt_dr_TTbar_after->Add(h_lowpt_dr_ST_after);
  // h_lowpt_dr_TTbar_after->Add(h_lowpt_dr_DY_after);
  // h_lowpt_dr_TTbar_after->Add(h_lowpt_dr_WJ_after);
  // h_lowpt_dr_TTbar_after->Add(h_lowpt_dr_DB_after);
  // h_lowpt_dr_TTbar_after->Add(h_lowpt_dr_QCD_after);
  // TH1D* h_lowpt_dr_DATA_before  = (TH1D*)input_DATA->Get("TagProbe_Ele30to110/dRmin_ele_jet");
  // TH1D* h_lowpt_dr_DATA_after   = (TH1D*)input_DATA->Get("TagProbe_30to110Plateau/dRmin_ele_jet");
  //
  //
  // // //plateau of only non-lowpt trigger
  // TH1D* h_highpt_dr_TTbar_before = (TH1D*)input_TTbar->Get("TagProbe_Ele110/dRmin_ele_jet");
  // TH1D* h_highpt_dr_ST_before = (TH1D*)input_ST->Get("TagProbe_Ele110/dRmin_ele_jet");
  // TH1D* h_highpt_dr_DY_before = (TH1D*)input_DY->Get("TagProbe_Ele110/dRmin_ele_jet");
  // TH1D* h_highpt_dr_WJ_before = (TH1D*)input_WJ->Get("TagProbe_Ele110/dRmin_ele_jet");
  // TH1D* h_highpt_dr_DB_before = (TH1D*)input_DB->Get("TagProbe_Ele110/dRmin_ele_jet");
  // TH1D* h_highpt_dr_QCD_before = (TH1D*)input_QCD->Get("TagProbe_Ele110/dRmin_ele_jet");
  // h_highpt_dr_TTbar_before->Add(h_highpt_dr_ST_before);
  // h_highpt_dr_TTbar_before->Add(h_highpt_dr_DY_before);
  // h_highpt_dr_TTbar_before->Add(h_highpt_dr_WJ_before);
  // h_highpt_dr_TTbar_before->Add(h_highpt_dr_DB_before);
  // h_highpt_dr_TTbar_before->Add(h_highpt_dr_QCD_before);
  // TH1D* h_highpt_dr_TTbar_after  = (TH1D*)input_TTbar->Get("TagProbe_110Plateau/dRmin_ele_jet");
  // TH1D* h_highpt_dr_ST_after = (TH1D*)input_ST->Get("TagProbe_110Plateau/dRmin_ele_jet");
  // TH1D* h_highpt_dr_DY_after = (TH1D*)input_DY->Get("TagProbe_110Plateau/dRmin_ele_jet");
  // TH1D* h_highpt_dr_WJ_after = (TH1D*)input_WJ->Get("TagProbe_110Plateau/dRmin_ele_jet");
  // TH1D* h_highpt_dr_DB_after = (TH1D*)input_DB->Get("TagProbe_110Plateau/dRmin_ele_jet");
  // TH1D* h_highpt_dr_QCD_after = (TH1D*)input_QCD->Get("TagProbe_110Plateau/dRmin_ele_jet");
  // h_highpt_dr_TTbar_after->Add(h_highpt_dr_ST_after);
  // h_highpt_dr_TTbar_after->Add(h_highpt_dr_DY_after);
  // h_highpt_dr_TTbar_after->Add(h_highpt_dr_WJ_after);
  // h_highpt_dr_TTbar_after->Add(h_highpt_dr_DB_after);
  // h_highpt_dr_TTbar_after->Add(h_highpt_dr_QCD_after);
  // TH1D* h_highpt_dr_DATA_before  = (TH1D*)input_DATA->Get("TagProbe_Ele110/dRmin_ele_jet");
  // TH1D* h_highpt_dr_DATA_after   = (TH1D*)input_DATA->Get("TagProbe_110Plateau/dRmin_ele_jet");
  //
  //
  // cout << "Got all histograms" << endl;
  // //get efficiencies
  // TGraphAsymmErrors* gr_pt_mc;
  // double is_efficiency_fine = true;
  // for(unsigned int i=0; i<h_pt_TTbar_before->GetNbinsX(); i++){
  //   if(h_pt_TTbar_before->GetBinContent(i+1) < h_pt_TTbar_after->GetBinContent(i+1)){
  //     h_pt_TTbar_after->SetBinContent(i+1, h_pt_TTbar_before->GetBinContent(i+1));
  //     // is_efficiency_fine = false;
  //   }
  // }
  // gr_pt_mc = new TGraphAsymmErrors(h_pt_TTbar_after, h_pt_TTbar_before);
  // // if(is_efficiency_fine) gr_pt_mc = new TGraphAsymmErrors(h_pt_TTbar_after, h_pt_TTbar_before);
  // // else gr_pt_mc = new TGraphAsymmErrors(h_pt_TTbar_after, h_pt_TTbar_before,"pois");
  // TGraphAsymmErrors* gr_pt_data = new TGraphAsymmErrors(h_pt_DATA_after, h_pt_DATA_before);
  // HistCosmetics(gr_pt_mc);
  // gr_pt_mc->SetLineColor(kRed);
  // HistCosmetics(gr_pt_data);
  // gr_pt_data->SetLineColor(kBlack);
  // TGraphAsymmErrors* gr_pt_binned_mc;
  // double is_efficiency_binned = true;
  // for(unsigned int i=0; i<h_pt_binned_TTbar_before->GetNbinsX(); i++){
  //   if(h_pt_binned_TTbar_before->GetBinContent(i+1) < h_pt_binned_TTbar_after->GetBinContent(i+1)){
  //     h_pt_binned_TTbar_after->SetBinContent(i+1, h_pt_binned_TTbar_before->GetBinContent(i+1));
  //     // is_efficiency_binned = false;
  //   }
  // }
  // // if(is_efficiency_binned) gr_pt_binned_mc = new TGraphAsymmErrors(h_pt_binned_TTbar_after, h_pt_binned_TTbar_before);
  // // else gr_pt_binned_mc = new TGraphAsymmErrors(h_pt_binned_TTbar_after, h_pt_binned_TTbar_before,"pois");
  // gr_pt_binned_mc = new TGraphAsymmErrors(h_pt_binned_TTbar_after, h_pt_binned_TTbar_before);
  // TGraphAsymmErrors* gr_pt_binned_data = new TGraphAsymmErrors(h_pt_binned_DATA_after, h_pt_binned_DATA_before);
  // HistCosmetics(gr_pt_binned_mc);
  // gr_pt_binned_mc->SetLineColor(kRed);
  // HistCosmetics(gr_pt_binned_data);
  // gr_pt_binned_data->SetLineColor(kBlack);
  // TGraphAsymmErrors* gr_lowpt_eta_mc = new TGraphAsymmErrors(h_lowpt_eta_TTbar_after, h_lowpt_eta_TTbar_before);
  // TGraphAsymmErrors* gr_lowpt_eta_data = new TGraphAsymmErrors(h_lowpt_eta_DATA_after, h_lowpt_eta_DATA_before);
  // HistCosmetics(gr_lowpt_eta_mc);
  // gr_lowpt_eta_mc->SetLineColor(kRed);
  // HistCosmetics(gr_lowpt_eta_data);
  // gr_lowpt_eta_data->SetLineColor(kBlack);
  // TGraphAsymmErrors* gr_verylowpt_eta_mc = new TGraphAsymmErrors(h_verylowpt_eta_TTbar_after, h_verylowpt_eta_TTbar_before);
  // TGraphAsymmErrors* gr_verylowpt_eta_data = new TGraphAsymmErrors(h_verylowpt_eta_DATA_after, h_verylowpt_eta_DATA_before);
  // HistCosmetics(gr_verylowpt_eta_mc);
  // gr_verylowpt_eta_mc->SetLineColor(kRed);
  // HistCosmetics(gr_verylowpt_eta_data);
  // gr_verylowpt_eta_data->SetLineColor(kBlack);
  // TGraphAsymmErrors* gr_mediumpt_eta_mc = new TGraphAsymmErrors(h_mediumpt_eta_TTbar_after, h_mediumpt_eta_TTbar_before);
  // TGraphAsymmErrors* gr_mediumpt_eta_data = new TGraphAsymmErrors(h_mediumpt_eta_DATA_after, h_mediumpt_eta_DATA_before);
  // HistCosmetics(gr_mediumpt_eta_mc);
  // gr_mediumpt_eta_mc->SetLineColor(kRed);
  // HistCosmetics(gr_mediumpt_eta_data);
  // gr_mediumpt_eta_data->SetLineColor(kBlack);
  // TGraphAsymmErrors* gr_highpt_eta_mc = new TGraphAsymmErrors(h_highpt_eta_TTbar_after, h_highpt_eta_TTbar_before);
  // TGraphAsymmErrors* gr_highpt_eta_data = new TGraphAsymmErrors(h_highpt_eta_DATA_after, h_highpt_eta_DATA_before);
  // HistCosmetics(gr_highpt_eta_mc);
  // gr_highpt_eta_mc->SetLineColor(kRed);
  // HistCosmetics(gr_highpt_eta_data);
  // gr_highpt_eta_data->SetLineColor(kBlack);
  // TGraphAsymmErrors* gr_lowpt_dr_mc = new TGraphAsymmErrors(h_lowpt_dr_TTbar_after, h_lowpt_dr_TTbar_before);
  // TGraphAsymmErrors* gr_lowpt_dr_data = new TGraphAsymmErrors(h_lowpt_dr_DATA_after, h_lowpt_dr_DATA_before);
  // HistCosmetics(gr_lowpt_dr_mc);
  // gr_lowpt_dr_mc->SetLineColor(kRed);
  // HistCosmetics(gr_lowpt_dr_data);
  // gr_lowpt_dr_data->SetLineColor(kBlack);
  // TGraphAsymmErrors* gr_highpt_dr_mc = new TGraphAsymmErrors(h_highpt_dr_TTbar_after, h_highpt_dr_TTbar_before);
  // TGraphAsymmErrors* gr_highpt_dr_data = new TGraphAsymmErrors(h_highpt_dr_DATA_after, h_highpt_dr_DATA_before);
  // HistCosmetics(gr_highpt_dr_mc);
  // gr_highpt_dr_mc->SetLineColor(kRed);
  // HistCosmetics(gr_highpt_dr_data);
  // gr_highpt_dr_data->SetLineColor(kBlack);
  //
  //
  // //calculate proper ratio
  // const int n_bins_pt = gr_pt_data->GetN();
  // int at_in_mc = 0, at_in_data = 0, n_points = 0;
  // vector<double> SF_pt_x, SF_pt_y, SF_pt_x_high, SF_pt_x_low, SF_pt_y_high, SF_pt_y_low;
  // for(int i=0; i<n_bins_pt; i++){
  //   double x_MC, y_MC, x_DATA, y_DATA;
  //   double MC_x_high, MC_x_low, MC_y_high, MC_y_low;
  //   double DATA_y_high, DATA_y_low;
  //
  //   gr_pt_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //   gr_pt_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //   cout << "mc start: " << x_MC << endl;
  //   cout << "data start: " << x_DATA << endl << endl;
  //
  //   double last_data = -1, last_mc = -1;
  //   bool skip = false;
  //   while(x_MC != x_DATA){
  //     if(x_MC < x_DATA){
  //       at_in_mc++;
  //       gr_pt_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //
  //     }
  //     else if(x_DATA < x_MC){
  //       at_in_data++;
  //       gr_pt_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //     }
  //
  //     if(x_MC == last_mc && x_DATA == last_data){
  //       skip = true;
  //       break;
  //     }
  //     last_mc = x_MC;
  //     last_data = x_DATA;
  //   }
  //   if(skip) break;
  //
  //   MC_x_high = gr_pt_mc->GetErrorXhigh(i);
  //   MC_x_low = gr_pt_mc->GetErrorXlow(i);
  //   MC_y_high = gr_pt_mc->GetErrorYhigh(i);
  //   MC_y_low = gr_pt_mc->GetErrorYlow(i);
  //
  //   DATA_y_high = gr_pt_data->GetErrorYhigh(i);
  //   DATA_y_low = gr_pt_data->GetErrorYlow(i);
  //
  //   //gaussian error propagation
  //   SF_pt_x.push_back(x_MC);
  //   SF_pt_x_low.push_back(MC_x_low);
  //   SF_pt_x_high.push_back(MC_x_high);
  //   SF_pt_y.push_back(y_DATA/y_MC);
  //   SF_pt_y_low.push_back(sqrt(pow(DATA_y_low/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_high,2)));
  //   SF_pt_y_high.push_back(sqrt(pow(DATA_y_high/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_low,2)));
  //
  //   cout << "x-data: " << x_DATA << ", x-mc: " << x_MC << ", x-sf: " << SF_pt_x.back() << endl;
  //   at_in_mc++;
  //   at_in_data++;
  //   n_points++;
  //
  // }
  //
  // const int n_bins_pt_binned = gr_pt_binned_data->GetN();
  // at_in_mc = 0, at_in_data = 0, n_points = 0;
  // vector<double> SF_pt_binned_x, SF_pt_binned_y, SF_pt_binned_x_high, SF_pt_binned_x_low, SF_pt_binned_y_high, SF_pt_binned_y_low;
  // for(int i=0; i<n_bins_pt_binned; i++){
  //   double x_MC, y_MC, x_DATA, y_DATA;
  //   double MC_x_high, MC_x_low, MC_y_high, MC_y_low;
  //   double DATA_y_high, DATA_y_low;
  //
  //   gr_pt_binned_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //   gr_pt_binned_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //   cout << "mc start: " << x_MC << endl;
  //   cout << "data start: " << x_DATA << endl << endl;
  //
  //   double last_data = -1, last_mc = -1;
  //   bool skip = false;
  //   while(x_MC != x_DATA){
  //     if(x_MC < x_DATA){
  //       at_in_mc++;
  //       gr_pt_binned_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //
  //     }
  //     else if(x_DATA < x_MC){
  //       at_in_data++;
  //       gr_pt_binned_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //     }
  //
  //     if(x_MC == last_mc && x_DATA == last_data){
  //       skip = true;
  //       break;
  //     }
  //     last_mc = x_MC;
  //     last_data = x_DATA;
  //   }
  //   if(skip) break;
  //
  //   MC_x_high = gr_pt_binned_mc->GetErrorXhigh(i);
  //   MC_x_low = gr_pt_binned_mc->GetErrorXlow(i);
  //   MC_y_high = gr_pt_binned_mc->GetErrorYhigh(i);
  //   MC_y_low = gr_pt_binned_mc->GetErrorYlow(i);
  //
  //   DATA_y_high = gr_pt_binned_data->GetErrorYhigh(i);
  //   DATA_y_low = gr_pt_binned_data->GetErrorYlow(i);
  //
  //   //gaussian error propagation
  //   SF_pt_binned_x.push_back(x_MC);
  //   SF_pt_binned_x_low.push_back(MC_x_low);
  //   SF_pt_binned_x_high.push_back(MC_x_high);
  //   SF_pt_binned_y.push_back(y_DATA/y_MC);
  //   SF_pt_binned_y_low.push_back(sqrt(pow(DATA_y_low/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_high,2)));
  //   SF_pt_binned_y_high.push_back(sqrt(pow(DATA_y_high/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_low,2)));
  //
  //   cout << "x-data: " << x_DATA << ", x-mc: " << x_MC << ", x-sf: " << SF_pt_binned_x.back() << endl;
  //   at_in_mc++;
  //   at_in_data++;
  //   n_points++;
  //
  // }
  //
  // const int n_bins_lowpt_eta = gr_lowpt_eta_data->GetN();
  // at_in_mc = 0, at_in_data = 0, n_points = 0;
  // vector<double> SF_lowpt_eta_x, SF_lowpt_eta_y, SF_lowpt_eta_x_high, SF_lowpt_eta_x_low, SF_lowpt_eta_y_high, SF_lowpt_eta_y_low;
  // for(int i=0; i<n_bins_lowpt_eta; i++){
  //   double x_MC, y_MC, x_DATA, y_DATA;
  //   double MC_x_high, MC_x_low, MC_y_high, MC_y_low;
  //   double DATA_y_high, DATA_y_low;
  //
  //   gr_lowpt_eta_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //   gr_lowpt_eta_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //   double last_data = -1, last_mc = -1;
  //   bool skip = false;
  //   while(x_MC != x_DATA){
  //     if(x_MC < x_DATA){
  //       at_in_mc++;
  //       gr_lowpt_eta_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //     }
  //     else if(x_DATA < x_MC){
  //       at_in_data++;
  //       gr_lowpt_eta_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //     }
  //     cout << "mc: " << x_MC << endl;
  //     cout << "data: " << x_DATA << endl << endl;
  //
  //     if(x_MC == last_mc && x_DATA == last_data){
  //       skip = true;
  //       break;
  //     }
  //     last_mc = x_MC;
  //     last_data = x_DATA;
  //   }
  //   if(skip) break;
  //
  //   MC_x_high = gr_lowpt_eta_mc->GetErrorXhigh(i);
  //   MC_x_low = gr_lowpt_eta_mc->GetErrorXlow(i);
  //   MC_y_high = gr_lowpt_eta_mc->GetErrorYhigh(i);
  //   MC_y_low = gr_lowpt_eta_mc->GetErrorYlow(i);
  //
  //   DATA_y_high = gr_lowpt_eta_data->GetErrorYhigh(i);
  //   DATA_y_low = gr_lowpt_eta_data->GetErrorYlow(i);
  //
  //   //gaussian error propagation
  //   SF_lowpt_eta_x.push_back(x_MC);
  //   SF_lowpt_eta_x_low.push_back(MC_x_low);
  //   SF_lowpt_eta_x_high.push_back(MC_x_high);
  //   SF_lowpt_eta_y.push_back(y_DATA/y_MC);
  //   SF_lowpt_eta_y_low.push_back(sqrt(pow(DATA_y_low/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_high,2)));
  //   SF_lowpt_eta_y_high.push_back(sqrt(pow(DATA_y_high/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_low,2)));
  //
  //   cout << "x-data: " << x_DATA << ", x-mc: " << x_MC << ", x-sf: " << SF_lowpt_eta_x.back() << endl;
  //   at_in_mc++;
  //   at_in_data++;
  //   n_points++;
  // }
  //
  // const int n_bins_verylowpt_eta = gr_verylowpt_eta_data->GetN();
  // at_in_mc = 0, at_in_data = 0, n_points = 0;
  // vector<double> SF_verylowpt_eta_x, SF_verylowpt_eta_y, SF_verylowpt_eta_x_high, SF_verylowpt_eta_x_low, SF_verylowpt_eta_y_high, SF_verylowpt_eta_y_low;
  // for(int i=0; i<n_bins_verylowpt_eta; i++){
  //   double x_MC, y_MC, x_DATA, y_DATA;
  //   double MC_x_high, MC_x_low, MC_y_high, MC_y_low;
  //   double DATA_y_high, DATA_y_low;
  //
  //   gr_verylowpt_eta_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //   gr_verylowpt_eta_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //   double last_data = -1, last_mc = -1;
  //   bool skip = false;
  //   while(x_MC != x_DATA){
  //     if(x_MC < x_DATA){
  //       at_in_mc++;
  //       gr_verylowpt_eta_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //     }
  //     else if(x_DATA < x_MC){
  //       at_in_data++;
  //       gr_verylowpt_eta_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //     }
  //     cout << "mc: " << x_MC << endl;
  //     cout << "data: " << x_DATA << endl << endl;
  //
  //     if(x_MC == last_mc && x_DATA == last_data){
  //       skip = true;
  //       break;
  //     }
  //     last_mc = x_MC;
  //     last_data = x_DATA;
  //   }
  //   if(skip) break;
  //
  //   MC_x_high = gr_verylowpt_eta_mc->GetErrorXhigh(i);
  //   MC_x_low = gr_verylowpt_eta_mc->GetErrorXlow(i);
  //   MC_y_high = gr_verylowpt_eta_mc->GetErrorYhigh(i);
  //   MC_y_low = gr_verylowpt_eta_mc->GetErrorYlow(i);
  //
  //   DATA_y_high = gr_verylowpt_eta_data->GetErrorYhigh(i);
  //   DATA_y_low = gr_verylowpt_eta_data->GetErrorYlow(i);
  //
  //   //gaussian error propagation
  //   SF_verylowpt_eta_x.push_back(x_MC);
  //   SF_verylowpt_eta_x_low.push_back(MC_x_low);
  //   SF_verylowpt_eta_x_high.push_back(MC_x_high);
  //   SF_verylowpt_eta_y.push_back(y_DATA/y_MC);
  //   SF_verylowpt_eta_y_low.push_back(sqrt(pow(DATA_y_low/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_high,2)));
  //   SF_verylowpt_eta_y_high.push_back(sqrt(pow(DATA_y_high/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_low,2)));
  //
  //   cout << "x-data: " << x_DATA << ", x-mc: " << x_MC << ", x-sf: " << SF_verylowpt_eta_x.back() << endl;
  //   at_in_mc++;
  //   at_in_data++;
  //   n_points++;
  // }
  //
  // const int n_bins_mediumpt_eta = gr_mediumpt_eta_data->GetN();
  // at_in_mc = 0, at_in_data = 0, n_points = 0;
  // vector<double> SF_mediumpt_eta_x, SF_mediumpt_eta_y, SF_mediumpt_eta_x_high, SF_mediumpt_eta_x_low, SF_mediumpt_eta_y_high, SF_mediumpt_eta_y_low;
  // for(int i=0; i<n_bins_mediumpt_eta; i++){
  //   double x_MC, y_MC, x_DATA, y_DATA;
  //   double MC_x_high, MC_x_low, MC_y_high, MC_y_low;
  //   double DATA_y_high, DATA_y_low;
  //
  //   gr_mediumpt_eta_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //   gr_mediumpt_eta_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //   double last_data = -1, last_mc = -1;
  //   bool skip = false;
  //   while(x_MC != x_DATA){
  //     if(x_MC < x_DATA){
  //       at_in_mc++;
  //       gr_mediumpt_eta_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //     }
  //     else if(x_DATA < x_MC){
  //       at_in_data++;
  //       gr_mediumpt_eta_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //     }
  //     cout << "mc: " << x_MC << endl;
  //     cout << "data: " << x_DATA << endl << endl;
  //
  //     if(x_MC == last_mc && x_DATA == last_data){
  //       skip = true;
  //       break;
  //     }
  //     last_mc = x_MC;
  //     last_data = x_DATA;
  //   }
  //   if(skip) break;
  //
  //   MC_x_high = gr_mediumpt_eta_mc->GetErrorXhigh(i);
  //   MC_x_low = gr_mediumpt_eta_mc->GetErrorXlow(i);
  //   MC_y_high = gr_mediumpt_eta_mc->GetErrorYhigh(i);
  //   MC_y_low = gr_mediumpt_eta_mc->GetErrorYlow(i);
  //
  //   DATA_y_high = gr_mediumpt_eta_data->GetErrorYhigh(i);
  //   DATA_y_low = gr_mediumpt_eta_data->GetErrorYlow(i);
  //
  //   //gaussian error propagation
  //   SF_mediumpt_eta_x.push_back(x_MC);
  //   SF_mediumpt_eta_x_low.push_back(MC_x_low);
  //   SF_mediumpt_eta_x_high.push_back(MC_x_high);
  //   SF_mediumpt_eta_y.push_back(y_DATA/y_MC);
  //   SF_mediumpt_eta_y_low.push_back(sqrt(pow(DATA_y_low/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_high,2)));
  //   SF_mediumpt_eta_y_high.push_back(sqrt(pow(DATA_y_high/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_low,2)));
  //
  //   cout << "x-data: " << x_DATA << ", x-mc: " << x_MC << ", x-sf: " << SF_mediumpt_eta_x.back() << endl;
  //   at_in_mc++;
  //   at_in_data++;
  //   n_points++;
  // }
  //
  // const int n_bins_highpt_eta = gr_highpt_eta_data->GetN();
  // at_in_mc = 0, at_in_data = 0, n_points = 0;
  // vector<double> SF_highpt_eta_x, SF_highpt_eta_y, SF_highpt_eta_x_high, SF_highpt_eta_x_low, SF_highpt_eta_y_high, SF_highpt_eta_y_low;
  // for(int i=0; i<n_bins_highpt_eta; i++){
  //   double x_MC, y_MC, x_DATA, y_DATA;
  //   double MC_x_high, MC_x_low, MC_y_high, MC_y_low;
  //   double DATA_y_high, DATA_y_low;
  //
  //   gr_highpt_eta_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //   gr_highpt_eta_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //   double last_data = -1, last_mc = -1;
  //   bool skip = false;
  //   while(x_MC != x_DATA){
  //     if(x_MC < x_DATA){
  //       at_in_mc++;
  //       gr_highpt_eta_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //     }
  //     else if(x_DATA < x_MC){
  //       at_in_data++;
  //       gr_highpt_eta_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //     }
  //     cout << "mc: " << x_MC << endl;
  //     cout << "data: " << x_DATA << endl << endl;
  //
  //     if(x_MC == last_mc && x_DATA == last_data){
  //       skip = true;
  //       break;
  //     }
  //     last_mc = x_MC;
  //     last_data = x_DATA;
  //   }
  //   if(skip) break;
  //
  //   MC_x_high = gr_highpt_eta_mc->GetErrorXhigh(i);
  //   MC_x_low = gr_highpt_eta_mc->GetErrorXlow(i);
  //   MC_y_high = gr_highpt_eta_mc->GetErrorYhigh(i);
  //   MC_y_low = gr_highpt_eta_mc->GetErrorYlow(i);
  //
  //   DATA_y_high = gr_highpt_eta_data->GetErrorYhigh(i);
  //   DATA_y_low = gr_highpt_eta_data->GetErrorYlow(i);
  //
  //   //gaussian error propagation
  //   SF_highpt_eta_x.push_back(x_MC);
  //   SF_highpt_eta_x_low.push_back(MC_x_low);
  //   SF_highpt_eta_x_high.push_back(MC_x_high);
  //   SF_highpt_eta_y.push_back(y_DATA/y_MC);
  //   SF_highpt_eta_y_low.push_back(sqrt(pow(DATA_y_low/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_high,2)));
  //   SF_highpt_eta_y_high.push_back(sqrt(pow(DATA_y_high/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_low,2)));
  //
  //   cout << "x-data: " << x_DATA << ", x-mc: " << x_MC << ", x-sf: " << SF_highpt_eta_x.back() << endl;
  //   at_in_mc++;
  //   at_in_data++;
  //   n_points++;
  // }
  //
  //
  //
  // const int n_bins_lowpt_dr = gr_lowpt_dr_data->GetN();
  // at_in_mc = 0, at_in_data = 0, n_points = 0;
  // vector<double> SF_lowpt_dr_x, SF_lowpt_dr_y, SF_lowpt_dr_x_high, SF_lowpt_dr_x_low, SF_lowpt_dr_y_high, SF_lowpt_dr_y_low;
  // for(int i=0; i<n_bins_lowpt_dr; i++){
  //   double x_MC, y_MC, x_DATA, y_DATA;
  //   double MC_x_high, MC_x_low, MC_y_high, MC_y_low;
  //   double DATA_y_high, DATA_y_low;
  //
  //   gr_lowpt_dr_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //   gr_lowpt_dr_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //   double last_data = -1, last_mc = -1;
  //   bool skip = false;
  //   while(x_MC != x_DATA){
  //     if(x_MC < x_DATA){
  //       at_in_mc++;
  //       gr_lowpt_dr_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //     }
  //     else if(x_DATA < x_MC){
  //       at_in_data++;
  //       gr_lowpt_dr_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //     }
  //     cout << "mc: " << x_MC << endl;
  //     cout << "data: " << x_DATA << endl << endl;
  //
  //     if(x_MC == last_mc && x_DATA == last_data){
  //       skip = true;
  //       break;
  //     }
  //     last_mc = x_MC;
  //     last_data = x_DATA;
  //   }
  //   if(skip) break;
  //
  //   MC_x_high = gr_lowpt_dr_mc->GetErrorXhigh(i);
  //   MC_x_low = gr_lowpt_dr_mc->GetErrorXlow(i);
  //   MC_y_high = gr_lowpt_dr_mc->GetErrorYhigh(i);
  //   MC_y_low = gr_lowpt_dr_mc->GetErrorYlow(i);
  //
  //   DATA_y_high = gr_lowpt_dr_data->GetErrorYhigh(i);
  //   DATA_y_low = gr_lowpt_dr_data->GetErrorYlow(i);
  //
  //   //gaussian error propagation
  //   SF_lowpt_dr_x.push_back(x_MC);
  //   SF_lowpt_dr_x_low.push_back(MC_x_low);
  //   SF_lowpt_dr_x_high.push_back(MC_x_high);
  //   SF_lowpt_dr_y.push_back(y_DATA/y_MC);
  //   SF_lowpt_dr_y_low.push_back(sqrt(pow(DATA_y_low/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_high,2)));
  //   SF_lowpt_dr_y_high.push_back(sqrt(pow(DATA_y_high/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_low,2)));
  //
  //   cout << "x-data: " << x_DATA << ", x-mc: " << x_MC << ", x-sf: " << SF_lowpt_dr_x.back() << endl;
  //   at_in_mc++;
  //   at_in_data++;
  //   n_points++;
  // }
  //
  // const int n_bins_highpt_dr = gr_highpt_dr_data->GetN();
  // at_in_mc = 0, at_in_data = 0, n_points = 0;
  // vector<double> SF_highpt_dr_x, SF_highpt_dr_y, SF_highpt_dr_x_high, SF_highpt_dr_x_low, SF_highpt_dr_y_high, SF_highpt_dr_y_low;
  // for(int i=0; i<n_bins_highpt_dr; i++){
  //   double x_MC, y_MC, x_DATA, y_DATA;
  //   double MC_x_high, MC_x_low, MC_y_high, MC_y_low;
  //   double DATA_y_high, DATA_y_low;
  //
  //   gr_highpt_dr_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //   gr_highpt_dr_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //   double last_data = -1, last_mc = -1;
  //   bool skip = false;
  //   while(x_MC != x_DATA){
  //     if(x_MC < x_DATA){
  //       at_in_mc++;
  //       gr_highpt_dr_mc->GetPoint(at_in_mc,x_MC,y_MC);
  //     }
  //     else if(x_DATA < x_MC){
  //       at_in_data++;
  //       gr_highpt_dr_data->GetPoint(at_in_data,x_DATA,y_DATA);
  //     }
  //     cout << "mc: " << x_MC << endl;
  //     cout << "data: " << x_DATA << endl << endl;
  //
  //     if(x_MC == last_mc && x_DATA == last_data){
  //       skip = true;
  //       break;
  //     }
  //     last_mc = x_MC;
  //     last_data = x_DATA;
  //   }
  //   if(skip) break;
  //
  //   MC_x_high = gr_highpt_dr_mc->GetErrorXhigh(i);
  //   MC_x_low = gr_highpt_dr_mc->GetErrorXlow(i);
  //   MC_y_high = gr_highpt_dr_mc->GetErrorYhigh(i);
  //   MC_y_low = gr_highpt_dr_mc->GetErrorYlow(i);
  //
  //   DATA_y_high = gr_highpt_dr_data->GetErrorYhigh(i);
  //   DATA_y_low = gr_highpt_dr_data->GetErrorYlow(i);
  //
  //   //gaussian error propagation
  //   SF_highpt_dr_x.push_back(x_MC);
  //   SF_highpt_dr_x_low.push_back(MC_x_low);
  //   SF_highpt_dr_x_high.push_back(MC_x_high);
  //   SF_highpt_dr_y.push_back(y_DATA/y_MC);
  //   SF_highpt_dr_y_low.push_back(sqrt(pow(DATA_y_low/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_high,2)));
  //   SF_highpt_dr_y_high.push_back(sqrt(pow(DATA_y_high/y_MC,2) + pow(y_DATA/y_MC/y_MC*MC_y_low,2)));
  //
  //   cout << "x-data: " << x_DATA << ", x-mc: " << x_MC << ", x-sf: " << SF_highpt_dr_x.back() << endl;
  //   at_in_mc++;
  //   at_in_data++;
  //   n_points++;
  // }
  //
  // TGraphAsymmErrors* gr_pt_ratio = new TGraphAsymmErrors(n_bins_pt, &SF_pt_x[0], &SF_pt_y[0], &SF_pt_x_low[0], &SF_pt_x_high[0], &SF_pt_y_low[0], &SF_pt_y_high[0]);
  // HistCosmetics(gr_pt_ratio,true);
  // TGraphAsymmErrors* gr_pt_binned_ratio = new TGraphAsymmErrors(n_bins_pt_binned, &SF_pt_binned_x[0], &SF_pt_binned_y[0], &SF_pt_binned_x_low[0], &SF_pt_binned_x_high[0], &SF_pt_binned_y_low[0], &SF_pt_binned_y_high[0]);
  // HistCosmetics(gr_pt_binned_ratio,true);
  // TGraphAsymmErrors* gr_lowpt_eta_ratio = new TGraphAsymmErrors(n_bins_lowpt_eta, &SF_lowpt_eta_x[0], &SF_lowpt_eta_y[0], &SF_lowpt_eta_x_low[0], &SF_lowpt_eta_x_high[0], &SF_lowpt_eta_y_low[0], &SF_lowpt_eta_y_high[0]);
  // HistCosmetics(gr_lowpt_eta_ratio,true);
  // TGraphAsymmErrors* gr_verylowpt_eta_ratio = new TGraphAsymmErrors(n_bins_verylowpt_eta, &SF_verylowpt_eta_x[0], &SF_verylowpt_eta_y[0], &SF_verylowpt_eta_x_low[0], &SF_verylowpt_eta_x_high[0], &SF_verylowpt_eta_y_low[0], &SF_verylowpt_eta_y_high[0]);
  // HistCosmetics(gr_verylowpt_eta_ratio,true);
  // TGraphAsymmErrors* gr_mediumpt_eta_ratio = new TGraphAsymmErrors(n_bins_mediumpt_eta, &SF_mediumpt_eta_x[0], &SF_mediumpt_eta_y[0], &SF_mediumpt_eta_x_low[0], &SF_mediumpt_eta_x_high[0], &SF_mediumpt_eta_y_low[0], &SF_mediumpt_eta_y_high[0]);
  // HistCosmetics(gr_mediumpt_eta_ratio,true);
  // TGraphAsymmErrors* gr_highpt_eta_ratio = new TGraphAsymmErrors(n_bins_highpt_eta, &SF_highpt_eta_x[0], &SF_highpt_eta_y[0], &SF_highpt_eta_x_low[0], &SF_highpt_eta_x_high[0], &SF_highpt_eta_y_low[0], &SF_highpt_eta_y_high[0]);
  // HistCosmetics(gr_highpt_eta_ratio,true);
  // TGraphAsymmErrors* gr_lowpt_dr_ratio = new TGraphAsymmErrors(n_bins_lowpt_dr, &SF_lowpt_dr_x[0], &SF_lowpt_dr_y[0], &SF_lowpt_dr_x_low[0], &SF_lowpt_dr_x_high[0], &SF_lowpt_dr_y_low[0], &SF_lowpt_dr_y_high[0]);
  // HistCosmetics(gr_lowpt_dr_ratio,true);
  // TGraphAsymmErrors* gr_highpt_dr_ratio = new TGraphAsymmErrors(n_bins_highpt_dr, &SF_highpt_dr_x[0], &SF_highpt_dr_y[0], &SF_highpt_dr_x_low[0], &SF_highpt_dr_x_high[0], &SF_highpt_dr_y_low[0], &SF_highpt_dr_y_high[0]);
  // HistCosmetics(gr_highpt_dr_ratio,true);
  //
  // TCanvas* c_pt = new TCanvas("c_pt", "pt-dependent", 400, 400);
  // TPad* p_top = SetupRatioPadTop();
  // TPad* p_bot = SetupRatioPad();
  // p_top->Draw();
  // p_bot->Draw();
  // p_top->cd();
  // gr_pt_mc->GetYaxis()->SetRangeUser(0.0001,1.1);
  // gr_pt_mc->GetYaxis()->SetTitle("Trigger efficiency");
  // gr_pt_mc->GetXaxis()->SetLimits(0,gr_pt_ratio->GetXaxis()->GetXmax());
  // gr_pt_mc->Draw("APZ");
  // gr_pt_data->Draw("PZ SAME");
  //
  // TLegend* leg_top = new TLegend(0.3,0.1,0.5,0.3);
  // leg_top->AddEntry(gr_pt_data,"Data","le");
  // leg_top->AddEntry(gr_pt_mc,"MC","le");
  // leg_top->SetBorderSize(0);
  // leg_top->SetFillStyle(0);
  // leg_top->Draw();
  //
  // p_bot->cd();
  // gr_pt_ratio->GetYaxis()->SetRangeUser(0.65, 1.35);
  // gr_pt_ratio->SetTitle("Electron trigger scale factors");
  // gr_pt_ratio->GetXaxis()->SetTitle("p_{T}^{e} [GeV]");
  // gr_pt_ratio->GetYaxis()->SetTitle("DATA / MC");
  // gr_pt_ratio->GetYaxis()->SetTitleSize(15);
  // gr_pt_ratio->SetLineColor(kBlack);
  // gr_pt_ratio->SetTitle("");
  // gr_pt_ratio->Draw("APZ");
  //
  // TLine* l_unity = new TLine(0, 1, gr_pt_ratio->GetXaxis()->GetXmax(), 1);
  // l_unity->SetLineColor(kRed);
  // l_unity->SetLineWidth(2);
  // l_unity->SetLineStyle(2);
  // l_unity->Draw("SAME");
  // c_pt->SaveAs(AnalysisTool::base_path + "ElectronTriggerEfficiencies_pt.eps");
  // //c_pt->SaveAs("pt_test.eps");
  //
  // TCanvas* c_pt_binned = new TCanvas("c_pt_binned", "pt_binned-dependent", 400, 400);
  // TPad* p_top_pt_binned = SetupRatioPadTop();
  // TPad* p_bot_pt_binned = SetupRatioPad();
  // p_top_pt_binned->Draw();
  // p_bot_pt_binned->Draw();
  // p_top_pt_binned->cd();
  // gr_pt_binned_mc->GetYaxis()->SetRangeUser(0.0001,1.1);
  // gr_pt_binned_mc->GetYaxis()->SetTitle("Trigger efficiency");
  // gr_pt_binned_mc->GetXaxis()->SetLimits(0,gr_pt_binned_ratio->GetXaxis()->GetXmax());
  // gr_pt_binned_mc->Draw("APZ");
  // gr_pt_binned_data->Draw("PZ SAME");
  //
  // TLegend* leg_top_pt_binned = new TLegend(0.3,0.1,0.5,0.3);
  // leg_top_pt_binned->AddEntry(gr_pt_binned_data,"Data","le");
  // leg_top_pt_binned->AddEntry(gr_pt_binned_mc,"MC","le");
  // leg_top_pt_binned->SetBorderSize(0);
  // leg_top_pt_binned->SetFillStyle(0);
  // leg_top_pt_binned->Draw();
  //
  // p_bot_pt_binned->cd();
  // gr_pt_binned_ratio->GetYaxis()->SetRangeUser(0.65, 1.35);
  // gr_pt_binned_ratio->SetTitle("Electron trigger scale factors");
  // gr_pt_binned_ratio->GetXaxis()->SetTitle("p_{T}^{e} [GeV]");
  // gr_pt_binned_ratio->GetYaxis()->SetTitle("DATA / MC");
  // gr_pt_binned_ratio->GetYaxis()->SetTitleSize(15);
  // gr_pt_binned_ratio->SetLineColor(kBlack);
  // gr_pt_binned_ratio->SetTitle("");
  // gr_pt_binned_ratio->Draw("APZ");
  //
  // TLine* l_unity_pt_binned = new TLine(0, 1, gr_pt_binned_ratio->GetXaxis()->GetXmax(), 1);
  // l_unity_pt_binned->SetLineColor(kRed);
  // l_unity_pt_binned->SetLineWidth(2);
  // l_unity_pt_binned->SetLineStyle(2);
  // l_unity_pt_binned->Draw("SAME");
  // c_pt_binned->SaveAs(AnalysisTool::base_path + "ElectronTriggerEfficiencies_pt_binned.eps");
  // //c_pt->SaveAs("pt_test.eps");
  //
  // TCanvas* c_lowpt_eta = new TCanvas("c_lowpt_eta", "lowpt_eta-dependent", 400, 400);
  // TPad* p_top_lowpt_eta = SetupRatioPadTop();
  // TPad* p_bot_lowpt_eta = SetupRatioPad();
  // p_top_lowpt_eta->Draw();
  // p_bot_lowpt_eta->Draw();
  // p_top_lowpt_eta->cd();
  // gr_lowpt_eta_mc->GetYaxis()->SetRangeUser(0.0001,1.1);
  // gr_lowpt_eta_mc->GetYaxis()->SetTitle("Trigger efficiency");
  // gr_lowpt_eta_mc->GetXaxis()->SetLimits(gr_lowpt_eta_ratio->GetXaxis()->GetXmin(),gr_lowpt_eta_ratio->GetXaxis()->GetXmax());
  // gr_lowpt_eta_mc->Draw("APZ");
  // gr_lowpt_eta_data->Draw("PZ SAME");
  //
  // TLegend* leg_top_lowpt_eta = new TLegend(0.3,0.1,0.7,0.4);
  // leg_top_lowpt_eta->SetHeader("30 GeV #leq p_{T}^{e} < 120 GeV");
  // leg_top_lowpt_eta->AddEntry(gr_lowpt_eta_data,"Data","le");
  // leg_top_lowpt_eta->AddEntry(gr_lowpt_eta_mc,"MC","le");
  // leg_top_lowpt_eta->SetBorderSize(0);
  // leg_top_lowpt_eta->SetFillStyle(0);
  // leg_top_lowpt_eta->Draw();
  //
  // p_bot_lowpt_eta->cd();
  // gr_lowpt_eta_ratio->GetYaxis()->SetRangeUser(0.65, 1.35);
  // gr_lowpt_eta_ratio->SetTitle("Electron trigger scale factors");
  // gr_lowpt_eta_ratio->GetXaxis()->SetTitle("#eta^{e} [GeV]");
  // gr_lowpt_eta_ratio->GetYaxis()->SetTitle("DATA / MC");
  // gr_lowpt_eta_ratio->GetYaxis()->SetTitleSize(15);
  // gr_lowpt_eta_ratio->SetLineColor(kBlack);
  // gr_lowpt_eta_ratio->SetTitle("");
  // gr_lowpt_eta_ratio->Draw("APZ");
  //
  // TLine* l_unity_lowpt_eta = new TLine(gr_lowpt_eta_ratio->GetXaxis()->GetXmin(), 1, gr_lowpt_eta_ratio->GetXaxis()->GetXmax(), 1);
  // l_unity_lowpt_eta->SetLineColor(kRed);
  // l_unity_lowpt_eta->SetLineWidth(2);
  // l_unity_lowpt_eta->SetLineStyle(2);
  // l_unity_lowpt_eta->Draw("SAME");
  // c_lowpt_eta->SaveAs(AnalysisTool::base_path + "ElectronTriggerEfficiencies_lowpt_eta.eps");
  //
  // TCanvas* c_verylowpt_eta = new TCanvas("c_verylowpt_eta", "verylowpt_eta-dependent", 400, 400);
  // TPad* p_top_verylowpt_eta = SetupRatioPadTop();
  // TPad* p_bot_verylowpt_eta = SetupRatioPad();
  // p_top_verylowpt_eta->Draw();
  // p_bot_verylowpt_eta->Draw();
  // p_top_verylowpt_eta->cd();
  // gr_verylowpt_eta_mc->GetYaxis()->SetRangeUser(0.0001,1.1);
  // gr_verylowpt_eta_mc->GetYaxis()->SetTitle("Trigger efficiency");
  // gr_verylowpt_eta_mc->GetXaxis()->SetLimits(gr_verylowpt_eta_ratio->GetXaxis()->GetXmin(),gr_verylowpt_eta_ratio->GetXaxis()->GetXmax());
  // gr_verylowpt_eta_mc->Draw("APZ");
  // gr_verylowpt_eta_data->Draw("PZ SAME");
  //
  // TLegend* leg_top_verylowpt_eta = new TLegend(0.3,0.1,0.7,0.4);
  // leg_top_verylowpt_eta->SetHeader("30 GeV #leq p_{T}^{e} < 50 GeV");
  // leg_top_verylowpt_eta->AddEntry(gr_verylowpt_eta_data,"Data","le");
  // leg_top_verylowpt_eta->AddEntry(gr_verylowpt_eta_mc,"MC","le");
  // leg_top_verylowpt_eta->SetBorderSize(0);
  // leg_top_verylowpt_eta->SetFillStyle(0);
  // leg_top_verylowpt_eta->Draw();
  //
  // p_bot_verylowpt_eta->cd();
  // gr_verylowpt_eta_ratio->GetYaxis()->SetRangeUser(0.65, 1.35);
  // gr_verylowpt_eta_ratio->SetTitle("Electron trigger scale factors");
  // gr_verylowpt_eta_ratio->GetXaxis()->SetTitle("#eta^{e} [GeV]");
  // gr_verylowpt_eta_ratio->GetYaxis()->SetTitle("DATA / MC");
  // gr_verylowpt_eta_ratio->GetYaxis()->SetTitleSize(15);
  // gr_verylowpt_eta_ratio->SetLineColor(kBlack);
  // gr_verylowpt_eta_ratio->SetTitle("");
  // gr_verylowpt_eta_ratio->Draw("APZ");
  //
  // TLine* l_unity_verylowpt_eta = new TLine(gr_verylowpt_eta_ratio->GetXaxis()->GetXmin(), 1, gr_verylowpt_eta_ratio->GetXaxis()->GetXmax(), 1);
  // l_unity_verylowpt_eta->SetLineColor(kRed);
  // l_unity_verylowpt_eta->SetLineWidth(2);
  // l_unity_verylowpt_eta->SetLineStyle(2);
  // l_unity_verylowpt_eta->Draw("SAME");
  // c_verylowpt_eta->SaveAs(AnalysisTool::base_path + "ElectronTriggerEfficiencies_verylowpt_eta.eps");
  //
  // TCanvas* c_mediumpt_eta = new TCanvas("c_mediumpt_eta", "mediumpt_eta-dependent", 400, 400);
  // TPad* p_top_mediumpt_eta = SetupRatioPadTop();
  // TPad* p_bot_mediumpt_eta = SetupRatioPad();
  // p_top_mediumpt_eta->Draw();
  // p_bot_mediumpt_eta->Draw();
  // p_top_mediumpt_eta->cd();
  // gr_mediumpt_eta_mc->GetYaxis()->SetRangeUser(0.0001,1.1);
  // gr_mediumpt_eta_mc->GetYaxis()->SetTitle("Trigger efficiency");
  // gr_mediumpt_eta_mc->GetXaxis()->SetLimits(gr_mediumpt_eta_ratio->GetXaxis()->GetXmin(),gr_mediumpt_eta_ratio->GetXaxis()->GetXmax());
  // gr_mediumpt_eta_mc->Draw("APZ");
  // gr_mediumpt_eta_data->Draw("PZ SAME");
  //
  // TLegend* leg_top_mediumpt_eta = new TLegend(0.3,0.1,0.7,0.4);
  // leg_top_mediumpt_eta->SetHeader("50 GeV #leq p_{T}^{e} < 120 GeV");
  // leg_top_mediumpt_eta->AddEntry(gr_mediumpt_eta_data,"Data","le");
  // leg_top_mediumpt_eta->AddEntry(gr_mediumpt_eta_mc,"MC","le");
  // leg_top_mediumpt_eta->SetBorderSize(0);
  // leg_top_mediumpt_eta->SetFillStyle(0);
  // leg_top_mediumpt_eta->Draw();
  //
  // p_bot_mediumpt_eta->cd();
  // gr_mediumpt_eta_ratio->GetYaxis()->SetRangeUser(0.65, 1.35);
  // gr_mediumpt_eta_ratio->SetTitle("Electron trigger scale factors");
  // gr_mediumpt_eta_ratio->GetXaxis()->SetTitle("#eta^{e} [GeV]");
  // gr_mediumpt_eta_ratio->GetYaxis()->SetTitle("DATA / MC");
  // gr_mediumpt_eta_ratio->GetYaxis()->SetTitleSize(15);
  // gr_mediumpt_eta_ratio->SetLineColor(kBlack);
  // gr_mediumpt_eta_ratio->SetTitle("");
  // gr_mediumpt_eta_ratio->Draw("APZ");
  //
  // TLine* l_unity_mediumpt_eta = new TLine(gr_mediumpt_eta_ratio->GetXaxis()->GetXmin(), 1, gr_mediumpt_eta_ratio->GetXaxis()->GetXmax(), 1);
  // l_unity_mediumpt_eta->SetLineColor(kRed);
  // l_unity_mediumpt_eta->SetLineWidth(2);
  // l_unity_mediumpt_eta->SetLineStyle(2);
  // l_unity_mediumpt_eta->Draw("SAME");
  // c_mediumpt_eta->SaveAs(AnalysisTool::base_path + "ElectronTriggerEfficiencies_mediumpt_eta.eps");
  //
  // TCanvas* c_highpt_eta = new TCanvas("c_highpt_eta", "highpt_eta-dependent", 400, 400);
  // TPad* p_top_highpt_eta = SetupRatioPadTop();
  // TPad* p_bot_highpt_eta = SetupRatioPad();
  // p_top_highpt_eta->Draw();
  // p_bot_highpt_eta->Draw();
  // p_top_highpt_eta->cd();
  // gr_highpt_eta_mc->GetYaxis()->SetRangeUser(0.0001,1.1);
  // gr_highpt_eta_mc->GetYaxis()->SetTitle("Trigger efficiency");
  // gr_highpt_eta_mc->GetXaxis()->SetLimits(gr_highpt_eta_ratio->GetXaxis()->GetXmin(),gr_highpt_eta_ratio->GetXaxis()->GetXmax());
  // gr_highpt_eta_mc->Draw("APZ");
  // gr_highpt_eta_data->Draw("PZ SAME");
  //
  // TLegend* leg_top_highpt_eta = new TLegend(0.3,0.1,0.6,0.4);
  // leg_top_highpt_eta->SetHeader("p_{T}^{e} #geq 120 GeV");
  // leg_top_highpt_eta->AddEntry(gr_highpt_eta_data,"Data","le");
  // leg_top_highpt_eta->AddEntry(gr_highpt_eta_mc,"MC","le");
  // leg_top_highpt_eta->SetBorderSize(0);
  // leg_top_highpt_eta->SetFillStyle(0);
  // leg_top_highpt_eta->Draw();
  //
  // p_bot_highpt_eta->cd();
  // gr_highpt_eta_ratio->GetYaxis()->SetRangeUser(0.65,1.35);
  // gr_highpt_eta_ratio->SetTitle("Electron trigger scale factors");
  // gr_highpt_eta_ratio->GetXaxis()->SetTitle("#eta^{e} [GeV]");
  // gr_highpt_eta_ratio->GetYaxis()->SetTitle("DATA / MC");
  // gr_highpt_eta_ratio->GetYaxis()->SetTitleSize(15);
  // gr_highpt_eta_ratio->SetLineColor(kBlack);
  // gr_highpt_eta_ratio->SetTitle("");
  // gr_highpt_eta_ratio->Draw("APZ");
  //
  // TLine* l_unity_highpt_eta = new TLine(gr_highpt_eta_ratio->GetXaxis()->GetXmin(), 1, gr_highpt_eta_ratio->GetXaxis()->GetXmax(), 1);
  // l_unity_highpt_eta->SetLineColor(kRed);
  // l_unity_highpt_eta->SetLineWidth(2);
  // l_unity_highpt_eta->SetLineStyle(2);
  // l_unity_highpt_eta->Draw("SAME");
  // c_highpt_eta->SaveAs(AnalysisTool::base_path + "ElectronTriggerEfficiencies_highpt_eta.eps");
  // //c_pt->SaveAs("pt_test.eps");
  //
  //
  //
  // TCanvas* c_lowpt_dr = new TCanvas("c_lowpt_dr", "lowpt_dr-dependent", 400, 400);
  // TPad* p_top_lowpt_dr = SetupRatioPadTop();
  // TPad* p_bot_lowpt_dr = SetupRatioPad();
  // p_top_lowpt_dr->Draw();
  // p_bot_lowpt_dr->Draw();
  // p_top_lowpt_dr->cd();
  // gr_lowpt_dr_mc->GetYaxis()->SetRangeUser(0.0001,1.1);
  // gr_lowpt_dr_mc->GetYaxis()->SetTitle("Trigger efficiency");
  // gr_lowpt_dr_mc->GetXaxis()->SetLimits(gr_lowpt_dr_ratio->GetXaxis()->GetXmin(),gr_lowpt_dr_ratio->GetXaxis()->GetXmax());
  // gr_lowpt_dr_mc->Draw("APZ");
  // gr_lowpt_dr_data->Draw("PZ SAME");
  //
  // TLegend* leg_top_lowpt_dr = new TLegend(0.3,0.1,0.7,0.4);
  // leg_top_lowpt_dr->SetHeader("30 GeV #leq p_{T}^{e} < 120 GeV");
  // leg_top_lowpt_dr->AddEntry(gr_lowpt_dr_data,"Data","le");
  // leg_top_lowpt_dr->AddEntry(gr_lowpt_dr_mc,"MC","le");
  // leg_top_lowpt_dr->SetBorderSize(0);
  // leg_top_lowpt_dr->SetFillStyle(0);
  // leg_top_lowpt_dr->Draw();
  //
  // p_bot_lowpt_dr->cd();
  // gr_lowpt_dr_ratio->GetYaxis()->SetRangeUser(0.65, 1.35);
  // gr_lowpt_dr_ratio->SetTitle("Electron trigger scale factors");
  // gr_lowpt_dr_ratio->GetXaxis()->SetTitle("#DeltaR(e, closest jet)");
  // gr_lowpt_dr_ratio->GetYaxis()->SetTitle("DATA / MC");
  // gr_lowpt_dr_ratio->GetYaxis()->SetTitleSize(15);
  // gr_lowpt_dr_ratio->SetLineColor(kBlack);
  // gr_lowpt_dr_ratio->SetTitle("");
  // gr_lowpt_dr_ratio->Draw("APZ");
  //
  // TLine* l_unity_lowpt_dr = new TLine(gr_lowpt_dr_ratio->GetXaxis()->GetXmin(), 1, gr_lowpt_dr_ratio->GetXaxis()->GetXmax(), 1);
  // l_unity_lowpt_dr->SetLineColor(kRed);
  // l_unity_lowpt_dr->SetLineWidth(2);
  // l_unity_lowpt_dr->SetLineStyle(2);
  // l_unity_lowpt_dr->Draw("SAME");
  // c_lowpt_dr->SaveAs(AnalysisTool::base_path + "ElectronTriggerEfficiencies_lowpt_dr.eps");
  //
  // TCanvas* c_highpt_dr = new TCanvas("c_highpt_dr", "highpt_dr-dependent", 400, 400);
  // TPad* p_top_highpt_dr = SetupRatioPadTop();
  // TPad* p_bot_highpt_dr = SetupRatioPad();
  // p_top_highpt_dr->Draw();
  // p_bot_highpt_dr->Draw();
  // p_top_highpt_dr->cd();
  // gr_highpt_dr_mc->GetYaxis()->SetRangeUser(0.0001,1.1);
  // gr_highpt_dr_mc->GetYaxis()->SetTitle("Trigger efficiency");
  // gr_highpt_dr_mc->GetXaxis()->SetLimits(gr_highpt_dr_ratio->GetXaxis()->GetXmin(),gr_highpt_dr_ratio->GetXaxis()->GetXmax());
  // gr_highpt_dr_mc->Draw("APZ");
  // gr_highpt_dr_data->Draw("PZ SAME");
  //
  // TLegend* leg_top_highpt_dr = new TLegend(0.3,0.1,0.6,0.4);
  // leg_top_highpt_dr->SetHeader("p_{T}^{e} #geq 120 GeV");
  // leg_top_highpt_dr->AddEntry(gr_highpt_dr_data,"Data","le");
  // leg_top_highpt_dr->AddEntry(gr_highpt_dr_mc,"MC","le");
  // leg_top_highpt_dr->SetBorderSize(0);
  // leg_top_highpt_dr->SetFillStyle(0);
  // leg_top_highpt_dr->Draw();
  //
  // p_bot_highpt_dr->cd();
  // gr_highpt_dr_ratio->GetYaxis()->SetRangeUser(0.65, 1.35);
  // gr_highpt_dr_ratio->SetTitle("Electron trigger scale factors");
  // gr_highpt_dr_ratio->GetXaxis()->SetTitle("#DeltaR(e, closest jet)");
  // gr_highpt_dr_ratio->GetYaxis()->SetTitle("DATA / MC");
  // gr_highpt_dr_ratio->GetYaxis()->SetTitleSize(15);
  // gr_highpt_dr_ratio->SetLineColor(kBlack);
  // gr_highpt_dr_ratio->SetTitle("");
  // gr_highpt_dr_ratio->Draw("APZ");
  //
  // TLine* l_unity_highpt_dr = new TLine(gr_highpt_dr_ratio->GetXaxis()->GetXmin(), 1, gr_highpt_dr_ratio->GetXaxis()->GetXmax(), 1);
  // l_unity_highpt_dr->SetLineColor(kRed);
  // l_unity_highpt_dr->SetLineWidth(2);
  // l_unity_highpt_dr->SetLineStyle(2);
  // l_unity_highpt_dr->Draw("SAME");
  // c_highpt_dr->SaveAs(AnalysisTool::base_path + "ElectronTriggerEfficiencies_highpt_dr.eps");
  // //c_pt->SaveAs("pt_test.eps");
  //
  //
  // /*
  // TFile* out = new TFile(AnalysisTool::base_path + "ElectronEfficiencies.root","RECREATE");
  // c_pt->Write();
  // c_highpt_eta->Write();
  // c_lowpt_eta->Write();
  // gr_lowpt_eta_mc->SetName("gr_lowpt_eta_TTbar_eff");
  // gr_lowpt_eta_mc->Write();
  // gr_lowpt_eta_data->SetName("gr_lowpt_eta_DATA_eff");
  // gr_lowpt_eta_data->Write();
  // gr_highpt_eta_mc->SetName("gr_highpt_eta_TTbar_eff");
  // gr_highpt_eta_mc->Write();
  // gr_highpt_eta_data->SetName("gr_highpt_eta_DATA_eff");
  // gr_highpt_eta_data->Write();
  // out->Close();
  // */
  //
  //
  // delete h_pt_DATA_after;
  // delete h_pt_DATA_before;
  // delete h_pt_QCD_after;
  // delete h_pt_DB_after;
  // delete h_pt_WJ_after;
  // delete h_pt_DY_after;
  // delete h_pt_ST_after;
  // delete h_pt_TTbar_after;
  // delete h_pt_QCD_before;
  // delete h_pt_DB_before;
  // delete h_pt_WJ_before;
  // delete h_pt_DY_before;
  // delete h_pt_ST_before;
  // delete h_pt_TTbar_before;

}
