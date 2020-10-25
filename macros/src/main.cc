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
  // Analysis16.AddSystematicsMuonTrigger();
  // Analysis16.PDFRMS();
  // Analysis16.ScaleVariationEnvelope();
  // Analysis16.ProduceCombineHistograms_mc_fullsyst("comb");
  // Analysis16.ProduceCombineHistograms_mc_fullsyst("much");
  // Analysis16.ProduceCombineHistograms_mc_fullsyst("ech");


  // Run combine script before for this step
  //Analysis16.PlotLimitsCombine("ech");
  //Analysis16.PlotLimitsCombine("much");




  // Analysis17.CalculateTriggerEfficiencies();
  // Analysis17.AddSystematicsEleTrigger();
  // Analysis17.AddSystematicsMuonTrigger();
  // Analysis17.PDFRMS();
  // Analysis17.ScaleVariationEnvelope();
  // Analysis17.ProduceCombineHistograms_mc_fullsyst("comb");




  // Analysis18.CalculateTriggerEfficiencies();
  // Analysis18.AddSystematicsEleTrigger();
  // Analysis18.AddSystematicsMuonTrigger();
  // Analysis18.PDFRMS();
  // Analysis18.ScaleVariationEnvelope();
  // Analysis18.ProduceCombineHistograms_mc_fullsyst("comb");

  //Analysis18.ProduceCombineHistograms_combinedLQsamples("10");
  //Analysis18.ProduceCombineHistograms_combinedLQsamples("09");
  //Analysis18.ProduceCombineHistograms_combinedLQsamples("08");
  //Analysis18.ProduceCombineHistograms_combinedLQsamples("07");
  //Analysis18.ProduceCombineHistograms_combinedLQsamples("06");
  //Analysis18.ProduceCombineHistograms_combinedLQsamples("05");
  //Analysis18.ProduceCombineHistograms_combinedLQsamples("04");
  //Analysis18.ProduceCombineHistograms_combinedLQsamples("03");
  //Analysis18.ProduceCombineHistograms_combinedLQsamples("02");
  //Analysis18.ProduceCombineHistograms_combinedLQsamples("01");
  //Analysis18.ProduceCombineHistograms_combinedLQsamples("00");

  // Run combine script before for this step
  //Analysis18.PlotLimitsCombine("comb", "10");
  Analysis18.PlotLimitsCombine("comb", "00");




  // // Old theta macros
  // Analysis16.ProduceThetaHistograms_mc_fullsyst_much();
  // Analysis16.PlotLimits();

  cout << "Finished main(). cya." << endl;
}
