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
  // Analysis16.PDFRMS();
  // Analysis16.ScaleVariationEnvelope();
  // Analysis16.ProduceCombineHistograms_mc_fullsyst("comb");
  // Analysis16.ProduceCombineHistograms_mc_fullsyst("much");
  // Analysis16.ProduceCombineHistograms_mc_fullsyst("ech");


  // Run combine script before for this step
  Analysis16.PlotLimitsCombine("ech");
  Analysis16.PlotLimitsCombine("much");




  // Analysis17.CalculateTriggerEfficiencies();
  // Analysis17.AddSystematicsEleTrigger();


  // Analysis18.CalculateTriggerEfficiencies();
  // Analysis18.AddSystematicsEleTrigger();


  // // Old theta macros
  // Analysis16.ProduceThetaHistograms_mc_fullsyst_much();
  // Analysis16.PlotLimits();

  cout << "Finished main(). cya." << endl;
}
