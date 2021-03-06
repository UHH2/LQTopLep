#pragma once

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/ObjectIdUtils.h"
#include "UHH2/common/include/JetCorrections.h"
#include "UHH2/common/include/JetIds.h"
#include <TFile.h>
#include <TGraphAsymmErrors.h>
//#include "LHAPDF/LHAPDF.h"
#include "TSystem.h"



class MuonTriggerWeights: public uhh2::AnalysisModule{

 public:
  explicit MuonTriggerWeights(uhh2::Context & ctx, TString path_, Year year_);
  virtual bool process(uhh2::Event & event) override;


 private:
  TString path;
  Year year;
  std::unique_ptr<TGraphAsymmErrors> g_sf_0to0p9, g_sf_0p9to1p2, g_sf_1p2to2p1, g_sf_2p1to2p4;
  uhh2::Event::Handle<float> h_muon_weight, h_muon_weight_up, h_muon_weight_down;

};
class ElectronTriggerWeights: public uhh2::AnalysisModule{

 public:
  explicit ElectronTriggerWeights(uhh2::Context & ctx, TString path_, Year year_);
  virtual bool process(uhh2::Event & event) override;

 private:
  TString path;
  Year year;
  std::vector<float> pt_bins;
  std::unique_ptr<TGraphAsymmErrors> g_sf_pt1, g_sf_pt2, g_sf_pt3;
  uhh2::Event::Handle<float> h_ele_weight, h_ele_weight_up, h_ele_weight_down;

};







// class ElectronTriggerWeights: public uhh2::AnalysisModule{
//
//  public:
//   explicit ElectronTriggerWeights(uhh2::Context & ctx, TString path_, TString SysDirection_);
//   virtual bool process(uhh2::Event & event) override;
//
//  private:
//   TString path, SysDirection;
//   std::unique_ptr<TGraphAsymmErrors> Eff_lowpt_MC, Eff_lowpt_DATA, Eff_highpt_MC, Eff_highpt_DATA;
//
// };

class JetCorrectorVariable: public JetCorrector{

 public:
  explicit JetCorrectorVariable(uhh2::Context & ctx, const std::vector<std::string> & JEC_files);
  bool correct_collection(uhh2::Event & event, std::vector<Jet> & jets);


};
/*
class ElectronFakeRateWeights: public uhh2::AnalysisModule{

 public:
  explicit ElectronFakeRateWeights(uhh2::Context & ctx, const std::vector<std::string> & JEC_files, TString path_, TString SysDirection_, const std::string label_jets, const std::string label_genjets);
  virtual bool process(uhh2::Event & event) override;

 protected:
  TString path, SysDirection;
  std::unique_ptr<TGraphAsymmErrors> SF;
  std::vector<double> x_low, x_high;
  int n_points;
  std::unique_ptr<JetCorrectorVariable> jet_corrector;
  std::unique_ptr<GenericJetResolutionSmearer> jet_smearer;
  JetId jet_id;
  uhh2::Event::Handle<double> FakeRateWeightEle;
  uhh2::Event::Handle<double> FakeRateWeightEleUp;
  uhh2::Event::Handle<double> FakeRateWeightEleDown;
  uhh2::Event::Handle<std::vector<Jet>> h_jets;

};

class MuonFakeRateWeights: public uhh2::AnalysisModule{

 public:
  explicit MuonFakeRateWeights(uhh2::Context & ctx, TString path_, TString SysDirection_);
  virtual bool process(uhh2::Event & event) override;

 protected:
  TString path, SysDirection;
  std::unique_ptr<TGraphAsymmErrors> SF;
  uhh2::Event::Handle<double> FakeRateWeightMu;
  uhh2::Event::Handle<double> FakeRateWeightMuUp;
  uhh2::Event::Handle<double> FakeRateWeightMuDown;

};
*/
