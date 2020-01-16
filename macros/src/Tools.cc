#include <TString.h>
#include <TFile.h>
#include <iostream>

#include "../include/Tools.h"

using namespace std;

AnalysisTool::AnalysisTool(TString year_) : year(year_)
{
  base_path = "/nfs/dust/cms/user/reimersa/LQTopLep/";
  theta_path = "/nfs/dust/cms/user/reimersa/theta_LQTopLep/utils2/Limits_MC/";
  trigger_tag = "Trigger/";
  final_tag = "Finalselection/MuonChannel/";



  yeartag = "";
  if(year == "2016") yeartag = "2016v3";
  else if (year == "2017") yeartag = "2017v2";
  else if (year == "2018") yeartag = "2018";
  else throw runtime_error("Invalid year_ specified");

}
