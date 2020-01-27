#include <cmath>
#include <iostream>
#include "../include/Tools.h"
#include <TString.h>

using namespace std;

int main(){
  cout << "Hello from main()." << endl;

  AnalysisTool Analysis16(2016);
  // Analysis16.CalculateTriggerEfficiencies();
  // Analysis16.PDFRMS();
  // Analysis16.ScaleVariationEnvelope();
  Analysis16.ProduceCombineHistograms_mc_fullsyst_much();
  // Analysis16.PlotLimits();

  cout << "Finished main(). cya." << endl;
}
