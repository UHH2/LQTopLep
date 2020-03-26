#include <TString.h>
#include <TFile.h>
#include <iostream>

#include "../include/Tools.h"

using namespace std;

AnalysisTool::AnalysisTool(int year_)
{
  base_path = "/nfs/dust/cms/user/reimersa/LQTopLep/";
  theta_path = "/nfs/dust/cms/user/reimersa/theta_LQTopLep/utils2/Limits_MC/";
  combine_path = "../data/datacards/";
  trigger_tag = "Trigger/Fullselection_3eTrigger/";
  final_tag = "Finalselection/Inclusive/";
  full_tag = final_tag;
  full_tag.ReplaceAll("Finalselection", "Fullselection");


  year = "";
  year += year_;
  yeartag = "";
  if(year == "2016") yeartag = "2016v3";
  else if (year == "2017") yeartag = "2017v2";
  else if (year == "2018") yeartag = "2018";
  else throw runtime_error("Invalid year_ specified");

}
