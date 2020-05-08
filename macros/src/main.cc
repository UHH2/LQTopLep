#include <cmath>
#include <iostream>
#include "../include/Tools.h"
#include <TString.h>

using namespace std;

int main(){
  cout << "Hello from main()." << endl;

  AnalysisTool Analysis16(2016);
  AnalysisTool Analysis17(2017);
  AnalysisTool Analysis18(2018);



  // Analysis16.CalculateTriggerEfficiencies();
  // Analysis16.AddSystematicsEleTrigger();
  Analysis16.AddSystematicsMuonTrigger();
  // Analysis16.PDFRMS();
  // Analysis16.ScaleVariationEnvelope();
  // // Analysis16.ProduceCombineHistograms_mc_fullsyst_much();

  // Run combine script before for this step
  // Analysis16.PlotLimitsCombine();



  // Analysis17.CalculateTriggerEfficiencies();
  // Analysis17.AddSystematicsEleTrigger();
  Analysis17.AddSystematicsMuonTrigger();


  // Analysis18.CalculateTriggerEfficiencies();
  // Analysis18.AddSystematicsEleTrigger();
  Analysis18.AddSystematicsMuonTrigger();


  // // Old theta macros
  // Analysis16.ProduceThetaHistograms_mc_fullsyst_much();
  // Analysis16.PlotLimits();

  cout << "Finished main(). cya." << endl;
}
