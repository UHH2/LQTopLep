#pragma once

#include <cmath>
#include <iostream>
#include <TString.h>
#include <TFile.h>

class AnalysisTool {

public:

  // Constructors, destructor
  AnalysisTool(int year_);
  AnalysisTool(const AnalysisTool &) = default;
  AnalysisTool & operator = (const AnalysisTool &) = default;
  ~AnalysisTool() = default;

  // Main functions
  void CalculateTriggerEfficiencies();
  void AddSystematicsEleTrigger();
  void AddSystematicsMuonTrigger();
  void PDFRMS();
  void ScaleVariationEnvelope();
  void ProduceThetaHistograms_mc_fullsyst_much();
  void ProduceCombineHistograms_mc_fullsyst_much();
  void PlotLimits();
  void PlotLimitsCombine();

private:
  TString base_path, theta_path, combine_path, trigger_tag, full_tag, final_tag, uhh2_path;
  TString year;
  TString yeartag;



};
