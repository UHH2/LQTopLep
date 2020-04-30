#pragma once

#include <TString.h>

using namespace std;

const map<const TString, const TString> infotexts{
  {"2016", "35.9 fb^{-1} (13 TeV)"},
  {"2017", "41.5 fb^{-1} (13 TeV)"},
  {"2018", "59.7 fb^{-1} (13 TeV)"}
};

const map<const TString, const TString> offline_e_thresholds{
  {"2016", "30"},
  {"2017", "40"},
  {"2018", "35"}
};
