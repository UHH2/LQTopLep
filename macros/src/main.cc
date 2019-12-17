#include <cmath>
#include <iostream>
#include "../include/Tools.h"
#include <TString.h>

using namespace std;

int main(){
  cout << "Hello from main()." << endl;

  AnalysisTool Analysis16("2016");
  Analysis16.CalculateTriggerEfficiencies();

  cout << "Finished main(). cya." << endl;
}
