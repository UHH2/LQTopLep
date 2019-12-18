#pragma once

#include <cmath>
#include <iostream>
#include <TString.h>
#include <TFile.h>

class AnalysisTool {

public:

  // Constructors, destructor
  AnalysisTool(TString year_);
  AnalysisTool(const AnalysisTool &) = default;
  AnalysisTool & operator = (const AnalysisTool &) = default;
  ~AnalysisTool() = default;

  // Main functions
  void CalculateTriggerEfficiencies();

private:
  TString base_path, trigger_tag;
  TString year;
  TString yeartag;



};
