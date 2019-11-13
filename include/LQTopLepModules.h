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



class ElectronTriggerWeights: public uhh2::AnalysisModule{

 public:
  explicit ElectronTriggerWeights(uhh2::Context & ctx, TString path_, TString SysDirection_);
  virtual bool process(uhh2::Event & event) override;

 private:
  TString path, SysDirection;
  std::unique_ptr<TGraphAsymmErrors> Eff_lowpt_MC, Eff_lowpt_DATA, Eff_highpt_MC, Eff_highpt_DATA;

};

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
