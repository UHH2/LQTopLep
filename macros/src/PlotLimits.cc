#include "../include/cosmetics.h"
#include "../include/constants.h"
#include "../include/Tools.h"
#include <TString.h>
#include <iostream>
#include <TStyle.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TText.h>
#include <TPaveText.h>
#include <TGaxis.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TROOT.h>
#include <TKey.h>
#include <TLatex.h>
#include <TClass.h>
#include <fstream>

using namespace std;

void cosmetics();



void AnalysisTool::PlotLimits(){
  /*
  ==========================================
  |                                          |
  |  handy script to make a nice limit plot  |
  |                                          |
  ==========================================   */


  //shortcut to modifications:
  bool with_data = true;







  //0) general cosmetics
  cosmetics();

  //1) define masspoints
  // vector<double> x_theo, x = {200,300,400,500,600,700,800,900,1000,1200,1400,1700,2000};
  vector<double> x_theo = {};
  // const int n_points = x.size();
  for(unsigned int i=0; i<73; i++) x_theo.emplace_back(200+i*25);

  vector<double> theory = {0.606E+02, 0.342E+02, 0.203E+02, 0.126E+02, 0.804E+01, 0.531E+01, 0.359E+01, 0.248E+01, 0.174E+01, 0.125E+01, 0.906E+00, 0.666E+00, 0.496E+00, 0.374E+00, 0.284E+00, 0.218E+00, 0.169E+00, 0.132E+00, 0.103E+00, 0.815E-01, 0.648E-01, 0.518E-01, 0.416E-01, 0.336E-01, 0.273E-01, 0.222E-01, 0.182E-01, 0.150E-01, 0.123E-01, 0.102E-01, 0.845E-02, 0.702E-02, 0.586E-02, 0.490E-02, 0.411E-02, 0.346E-02, 0.291E-02, 0.246E-02, 0.208E-02, 0.177E-02, 0.150E-02, 0.128E-02, 0.109E-02, 0.931E-03, 0.796E-03, 0.682E-03, 0.585E-03, 0.503E-03, 0.432E-03, 0.372E-03, 0.322E-03, 0.277E-03, 0.240E-03, 0.208E-03, 0.180E-03, 0.156E-03, 0.135E-03, 0.117E-03, 0.102E-03, 0.889E-04, 0.773E-04, 0.675E-04, 0.588E-04, 0.513E-04, 0.449E-04, 0.392E-04, 0.343E-04, 0.300E-04, 0.262E-04, 0.230E-04, 0.201E-04, 0.177E-04, 0.155E-04};
  vector<double> theory_up = {68.6864639986, 38.7088357699, 23.1231188427, 14.2801931437, 9.14775674225, 6.04086866125, 4.09812990465, 2.83180818637, 1.9961249695, 1.43768324379, 1.04699304947, 0.773002289695, 0.577606127221, 0.435769248012, 0.332616458119, 0.255952075042, 0.198680970335, 0.155762154784, 0.12268146336, 0.0973445574252, 0.0775882758807, 0.0622727455808, 0.0502401678224, 0.0407359372195, 0.0331566970214, 0.0271049362891, 0.0222659070329, 0.0183911797357, 0.0151915220905, 0.0126239636961, 0.0105073040611, 0.00877316856006, 0.00735372018799, 0.00617455874717, 0.00519687625791, 0.00438793534258, 0.00371076525899, 0.00314667969243, 0.00267687854711, 0.00227986370728, 0.00194594170023, 0.00166354791096, 0.00142431123224, 0.00122124472433, 0.00104854900515, 0.000902791757093, 0.000776838473722, 0.000671433963321, 0.000579827602294, 0.000501096862859, 0.000434605506082, 0.000376929775342, 0.000327542903767, 0.000284878410493, 0.000247770273719, 0.000215703936219, 0.000188235420539, 0.000164100849249, 0.000143299394669, 0.000125427386986, 0.000109745030436, 9.61832703854e-05, 8.43211676849e-05, 7.39753169768e-05, 6.50156655371e-05, 5.70070210872e-05, 5.02100597108e-05, 4.4108862463e-05, 3.87574678976e-05, 3.4168258593e-05, 3.00616062962e-05, 2.65696166772e-05, 2.34120161779e-05};
  vector<double> theory_down = {52.7986603714, 29.6911642301, 17.5686999433, 10.8299579101, 6.91468893189, 4.53626296457, 3.05680585149, 2.10387635012, 1.46797058983, 1.04696059496, 0.756708205182, 0.554602109535, 0.41009563457, 0.307378381887, 0.232662294558, 0.1774, 0.136736088272, 0.106355507414, 0.0826933508426, 0.0649298461081, 0.0514156808167, 0.0408638946603, 0.0325599502214, 0.0261857164878, 0.0211144604116, 0.0170796093899, 0.0139231319871, 0.0114027649507, 0.00930652375991, 0.00764751101863, 0.00629923269506, 0.00520382269588, 0.00430951620453, 0.00358771192187, 0.00298621176372, 0.00249570543919, 0.00208675945192, 0.00174674759026, 0.00146994754324, 0.00123852939874, 0.00104112746868, 0.000883756388064, 0.000746976677178, 0.000632234205438, 0.000536577564579, 0.000455635250094, 0.000387693132405, 0.000330692135989, 0.000281119252388, 0.000239901551864, 0.000206047423487, 0.000175211199044, 0.000150290691676, 0.000128966526079, 0.000110827678946, 9.49053193805e-05, 8.1055491475e-05, 6.95501317178e-05, 5.99922626175e-05, 5.16450674407e-05, 4.44231084194e-05, 3.83215833192e-05, 3.29214374433e-05, 2.83348089492e-05, 2.44977942369e-05, 2.11387154388e-05, 1.82024846638e-05, 1.57052457174e-05, 1.35189117186e-05, 1.17106244637e-05, 1.00186508839e-05, 8.70944384368e-06, 7.49937502441e-06};

  //2) Read in vector LQ cross sections
  TFile* f_vectortheory = new TFile("include/LQxsecs.root", "READ");
  TGraph* g_vectortheory_k1 = (TGraph*)f_vectortheory->Get("vlqxsec");
  TGraph* g_vectortheory_k1_up = (TGraph*)f_vectortheory->Get("vlqxsecUp");
  TGraph* g_vectortheory_k1_down = (TGraph*)f_vectortheory->Get("vlqxsecDown");
  TGraph* g_vectortheory_k0 = (TGraph*)f_vectortheory->Get("vlqxsec_k0");
  TGraph* g_vectortheory_k0_up = (TGraph*)f_vectortheory->Get("vlqxsecUp_k0");
  TGraph* g_vectortheory_k0_down = (TGraph*)f_vectortheory->Get("vlqxsecDown_k0");

  vector<double> theory_vector_k1, theory_vector_k1_up, theory_vector_k1_down, theory_vector_k0, theory_vector_k0_up, theory_vector_k0_down;
  for(unsigned int i=0; i<x_theo.size(); i++){
    theory_vector_k1.emplace_back(g_vectortheory_k1->Eval(x_theo[i]));
    theory_vector_k1_up.emplace_back(g_vectortheory_k1_up->Eval(x_theo[i]));
    theory_vector_k1_down.emplace_back(g_vectortheory_k1_down->Eval(x_theo[i]));
    theory_vector_k0.emplace_back(g_vectortheory_k0->Eval(x_theo[i]));
    theory_vector_k0_up.emplace_back(g_vectortheory_k0_up->Eval(x_theo[i]));
    theory_vector_k0_down.emplace_back(g_vectortheory_k0_down->Eval(x_theo[i]));
  }

  delete g_vectortheory_k0_down;
  delete g_vectortheory_k0_up;
  delete g_vectortheory_k0;
  delete g_vectortheory_k1_down;
  delete g_vectortheory_k1_up;
  delete g_vectortheory_k1;
  delete f_vectortheory;




  TString filename = AnalysisTool::theta_path + "output/";

  TString txtname = "expected_limits_mc_fullsyst_much";
  txtname += ".txt";
  cout << "txtname: " << txtname << endl;
  filename += txtname;
  cout << "filename: " << filename << endl;
  ifstream myfile(filename);

  vector<double> mass, expected, expected_low_68, expected_high_68, expected_low_95, expected_high_95, observed;
  float val;
  while(!myfile.eof()){
    myfile >> val;
    mass.emplace_back(val);
    myfile >> val;
    expected.emplace_back(val);
    myfile >> val;
    expected_low_95.emplace_back(val);
    myfile >> val;
    expected_high_95.emplace_back(val);
    myfile >> val;
    expected_low_68.emplace_back(val);
    myfile >> val;
    expected_high_68.emplace_back(val);
  }
  myfile.clear();

  for(unsigned int i=0; i<expected.size(); i++){
    cout << "Mass: " << mass[i] << ", expected: " << expected[i] << endl;
  }

  filename.ReplaceAll("expected_limits", "observed_limits");
  myfile = ifstream(filename);
  while(!myfile.eof()){
    myfile >> val; // Skip the mass-line, jump to the observed limit directly
    myfile >> val;
    observed.emplace_back(val);
    myfile >> val; // Skip the uncertainty on obs
  }

  const int n_points = mass.size();
  //
  // //3) observed limits
  // vector<double> expected = {0.699,0.00749,0.00353,0.00233,0.00188,0.00144,0.00118,0.00085,0.000699,0.000514,0.000376,0.000323,0.000281};
  // vector<double> observed = {1.51,0.0115,0.00383,0.00227,0.00145,0.000853,0.000738,0.000613,0.00056,0.000446,0.000349,0.000316,0.000276};
  // vector<double> expected_low_68 = {0.461,0.00507,0.0024,0.00158,0.00129,0.000984,0.000798,0.000582,0.000477,0.000342,0.000253,0.000213,0.000182};
  // vector<double> expected_high_68 = {1.1,0.0112,0.00526,0.00348,0.00281,0.00217,0.00176,0.00127,0.00102,0.000784,0.000579,0.000504,0.000434};
  // vector<double> expected_low_95 = {0.325,0.00356,0.00172,0.00114,0.000905,0.000688,0.00059,0.00042,0.000351,0.000246,0.000181,0.000151,0.000131};
  // vector<double> expected_high_95 = {1.84,0.016,0.00735,0.00509,0.00404,0.00299,0.00251,0.00183,0.00145,0.00112,0.000824,0.000759,0.000659};



  vector<double> observed_fine;
  double mass_low = -1, mass_high = -1;
  int idx_low = -1;
  // Interpolate linearly between mass points to match theory curve granularity
  for(unsigned int j=0; j<x_theo.size(); j++){
    double mymass;
    mymass = x_theo[j];
    bool match = false;
    for(unsigned int k=0; k<mass.size(); k++){
      if(mymass == mass[k]){
        match = true;
        observed_fine.emplace_back(observed[k]);
        mass_low = mass[k];
        mass_high = mass[k+1];
        idx_low = k;
      }
    }
    if(!match){
      observed_fine.emplace_back((fabs(mymass-mass_low)) * ((observed[idx_low] - observed[idx_low+1])/(mass_low - mass_high)) + observed[idx_low]);
    }
  }


  int idx_obs_low = -1, idx_obs_high = -1, idx_obs_low_vector_k1 = -1, idx_obs_high_vector_k1 = -1, idx_obs_low_vector_k0 = -1, idx_obs_high_vector_k0 = -1;
  bool obsfound = false, obsfound_vector_k1 = false, obsfound_vector_k0 = false;

  //find intersection -- Obs
  for(unsigned int j=0; j<x_theo.size(); j++){
    //cout << "Mass: " << x_theo[j] << ", theory: " << theory[j] << ", observed: " << observed_fine[j] << endl;
    if(theory[j] < observed_fine[j] && !obsfound && x_theo[j] >= 300){
      idx_obs_low  = j-1;
      idx_obs_high = j;
      obsfound = true;
    }
    if(theory_vector_k1[j] < observed_fine[j] && !obsfound_vector_k1 && x_theo[j] >= 300){
      idx_obs_low_vector_k1  = j-1;
      idx_obs_high_vector_k1 = j;
      obsfound_vector_k1 = true;
    }
    else if(x_theo[j] == x_theo[x_theo.size()-1]){
      idx_obs_low_vector_k1  = j-1;
      idx_obs_high_vector_k1 = j;
      obsfound_vector_k1 = true;
    }
    if(theory_vector_k0[j] < observed_fine[j] && !obsfound_vector_k0 && x_theo[j] >= 300){
      idx_obs_low_vector_k0  = j-1;
      idx_obs_high_vector_k0 = j;
      obsfound_vector_k0 = true;
    }
    //cout << "theo v k1: " << theory_vector_k1[j] << ", obs: " << observed_fine[j] << ", excluded?: " << (theory_vector_k1[j] < observed_fine[j]) << endl;
  }




  cout << "Observed mass limit scalar: " << ((( theory[idx_obs_low] - (theory[idx_obs_high] - theory[idx_obs_low])/(x_theo[idx_obs_high]-x_theo[idx_obs_low])*x_theo[idx_obs_low])-(observed_fine[idx_obs_low] - (observed_fine[idx_obs_high] - observed_fine[idx_obs_low])/(x_theo[idx_obs_high]-x_theo[idx_obs_low])*x_theo[idx_obs_low]))/(((observed_fine[idx_obs_high] - observed_fine[idx_obs_low])/(x_theo[idx_obs_high] -x_theo[idx_obs_low]))-((theory[idx_obs_high] - theory[idx_obs_low])/(x_theo[idx_obs_high]-x_theo[idx_obs_low])))) << " GeV" << endl;

  double masslimit_vector_k1 = ((( theory_vector_k1[idx_obs_low_vector_k1] - (theory_vector_k1[idx_obs_high_vector_k1] - theory_vector_k1[idx_obs_low_vector_k1])/(x_theo[idx_obs_high_vector_k1]-x_theo[idx_obs_low_vector_k1])*x_theo[idx_obs_low_vector_k1])-(observed_fine[idx_obs_low_vector_k1] - (observed_fine[idx_obs_high_vector_k1] - observed_fine[idx_obs_low_vector_k1])/(x_theo[idx_obs_high_vector_k1]-x_theo[idx_obs_low_vector_k1])*x_theo[idx_obs_low_vector_k1]))/(((observed_fine[idx_obs_high_vector_k1] - observed_fine[idx_obs_low_vector_k1])/(x_theo[idx_obs_high_vector_k1] -x_theo[idx_obs_low_vector_k1]))-((theory_vector_k1[idx_obs_high_vector_k1] - theory_vector_k1[idx_obs_low_vector_k1])/(x_theo[idx_obs_high_vector_k1]-x_theo[idx_obs_low_vector_k1]))));
  if(masslimit_vector_k1 > 2000) masslimit_vector_k1 = 2000;
  cout << "Observed mass limit vector k1: " << masslimit_vector_k1 << " GeV" << endl;

  cout << "Observed mass limit vector k0: " << ((( theory_vector_k0[idx_obs_low_vector_k0] - (theory_vector_k0[idx_obs_high_vector_k0] - theory_vector_k0[idx_obs_low_vector_k0])/(x_theo[idx_obs_high_vector_k0]-x_theo[idx_obs_low_vector_k0])*x_theo[idx_obs_low_vector_k0])-(observed_fine[idx_obs_low_vector_k0] - (observed_fine[idx_obs_high_vector_k0] - observed_fine[idx_obs_low_vector_k0])/(x_theo[idx_obs_high_vector_k0]-x_theo[idx_obs_low_vector_k0])*x_theo[idx_obs_low_vector_k0]))/(((observed_fine[idx_obs_high_vector_k0] - observed_fine[idx_obs_low_vector_k0])/(x_theo[idx_obs_high_vector_k0] -x_theo[idx_obs_low_vector_k0]))-((theory_vector_k0[idx_obs_high_vector_k0] - theory_vector_k0[idx_obs_low_vector_k0])/(x_theo[idx_obs_high_vector_k0]-x_theo[idx_obs_low_vector_k0])))) << " GeV" << endl;





  //5) convert to upper and lower errors to be used in TGraphAsymmErrors

  vector<double> expected_up_68, expected_down_68, expected_up_95, expected_down_95;
  for(int i=0; i<n_points; i++){
    expected_up_68.push_back(expected_high_68[i] - expected[i]);
    expected_up_95.push_back(expected_high_95[i] - expected[i]);
    expected_down_68.push_back(expected[i] - expected_low_68[i]);
    expected_down_95.push_back(expected[i] - expected_low_95[i]);
  }

  //6) build graph objects
  TGraphAsymmErrors* g_expected_68 = new TGraphAsymmErrors(n_points,&mass[0],&expected[0],0,0,&expected_down_68[0], &expected_up_68[0]);
  TGraphAsymmErrors* g_expected_95 = new TGraphAsymmErrors(n_points,&mass[0],&expected[0],0,0,&expected_down_95[0], &expected_up_95[0]);

  TGraph* g_theory = new TGraph(theory.size(),&x_theo[0],&theory[0]);
  TGraph* g_theory_up = new TGraph(theory.size(),&x_theo[0],&theory_up[0]);
  TGraph* g_theory_down = new TGraph(theory.size(),&x_theo[0],&theory_down[0]);
  TGraph* g_theory_vector_k1 = new TGraph(theory_vector_k1.size(),&x_theo[0],&theory_vector_k1[0]);
  TGraph* g_theory_vector_k1_up = new TGraph(theory_vector_k1.size(),&x_theo[0],&theory_vector_k1_up[0]);
  TGraph* g_theory_vector_k1_down = new TGraph(theory_vector_k1.size(),&x_theo[0],&theory_vector_k1_down[0]);
  TGraph* g_theory_vector_k0 = new TGraph(theory_vector_k0.size(),&x_theo[0],&theory_vector_k0[0]);
  TGraph* g_theory_vector_k0_up = new TGraph(theory_vector_k0.size(),&x_theo[0],&theory_vector_k0_up[0]);
  TGraph* g_theory_vector_k0_down = new TGraph(theory_vector_k0.size(),&x_theo[0],&theory_vector_k0_down[0]);
  TGraph* g_expected = new TGraph(n_points,&mass[0],&expected[0]);
  TGraph* g_observed = new TGraph(n_points,&mass[0],&observed[0]);

  //7) cosmetics
  g_theory->SetLineWidth(3);
  g_theory->SetLineStyle(9);
  g_theory_up->SetLineWidth(3);
  g_theory_up->SetLineStyle(3);
  g_theory_down->SetLineWidth(3);
  g_theory_down->SetLineStyle(3);
  g_theory_vector_k1->SetLineWidth(3);
  g_theory_vector_k1->SetLineStyle(9);
  g_theory_vector_k1->SetLineColor(kMagenta-7);
  g_theory_vector_k1_up->SetLineWidth(3);
  g_theory_vector_k1_up->SetLineStyle(3);
  g_theory_vector_k1_up->SetLineColor(kMagenta-7);
  g_theory_vector_k1_down->SetLineWidth(3);
  g_theory_vector_k1_down->SetLineStyle(3);
  g_theory_vector_k1_down->SetLineColor(kMagenta-7);
  g_theory_vector_k0->SetLineWidth(3);
  g_theory_vector_k0->SetLineStyle(9);
  g_theory_vector_k0->SetLineColor(kRed+1);
  g_theory_vector_k0_up->SetLineWidth(3);
  g_theory_vector_k0_up->SetLineStyle(3);
  g_theory_vector_k0_up->SetLineColor(kRed+1);
  g_theory_vector_k0_down->SetLineWidth(3);
  g_theory_vector_k0_down->SetLineStyle(3);
  g_theory_vector_k0_down->SetLineColor(kRed+1);
  g_observed->SetLineWidth(3);
  g_observed->SetLineStyle(1);
  g_expected->SetLineWidth(3);
  g_expected->SetLineStyle(2);
  g_expected_68->SetFillStyle(1001);
  g_expected_95->SetFillStyle(1001);
  g_expected_68->SetFillColor(kGreen+1);
  g_expected_95->SetFillColor(kOrange);
  g_expected_68->SetLineWidth(0);
  g_expected_95->SetLineWidth(0);

  g_expected_95->SetMaximum(10000);
  g_expected_95->SetMinimum(5E-5);
  g_expected_95->SetTitle("");

  //8) Draw Graphs
  TCanvas* c = new TCanvas("c", "Nice limit plot", 800,600);
  gPad->SetLogy();
  g_expected_95->Draw("A3");
  g_expected_68->Draw("3 SAME");
  g_expected->Draw("SAME");
  if(with_data) g_observed->Draw("SAME");
  g_theory->Draw("SAME");
  g_theory_up->Draw("SAME");
  g_theory_down->Draw("SAME");
  g_theory_vector_k1->Draw("SAME");
  g_theory_vector_k1_up->Draw("SAME");
  g_theory_vector_k1_down->Draw("SAME");
  g_theory_vector_k0->Draw("SAME");
  g_theory_vector_k0_up->Draw("SAME");
  g_theory_vector_k0_down->Draw("SAME");

  //9) Legend
  TLegend *leg;
  leg = new TLegend(0.60,0.67,0.95,0.92,"");//x+0.1
  //leg->SetHeader("95% CL upper limits");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetLineColor(1);
  leg->SetTextFont(42);
  leg->SetTextSize(0.041);
  leg->SetHeader("95% CL upper limit");
  ((TLegendEntry*)leg->GetListOfPrimitives()->First())->SetTextSize(0.045);
  if(with_data)leg->AddEntry(g_observed, "Observed","L");
  leg->AddEntry(g_expected, "Expected","L");
  leg->AddEntry(g_expected_68, "68% expected","F");
  leg->AddEntry(g_expected_95, "95% expected","F");
  leg->Draw();


  TLegend *leg2;
  leg2 = new TLegend(0.60,0.43,0.95,0.62,"");//x+0.1
  //leg->SetHeader("95% CL upper limits");
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.030);
  leg2->SetFillStyle(0);
  leg2->SetLineColor(1);
  leg2->SetTextFont(42);
  leg2->SetTextSize(0.041);
  leg2->SetHeader("LQ pair production");
  ((TLegendEntry*)leg2->GetListOfPrimitives()->First())->SetTextSize(0.045);

  leg2->AddEntry(g_theory, "Scalar LQ","L");
  leg2->AddEntry(g_theory_vector_k0, "Vector LQ (#kappa = 0)","L");
  leg2->AddEntry(g_theory_vector_k1, "Vector LQ (#kappa = 1)","L");
  leg2->Draw();

  //10) CMS tags
  TString infotext = infotexts.at(AnalysisTool::year);
  TLatex *text1 = new TLatex(3.5, 24, infotext);
  text1->SetNDC();
  text1->SetTextAlign(33);
  text1->SetX(0.94);
  text1->SetTextFont(42);
  text1->SetTextSize(0.045);
  text1->SetY(1.);
  text1->Draw();

  TString cmstext = "CMS";
  TLatex *text2 = new TLatex(3.5, 24, cmstext);
  text2->SetNDC();
  text2->SetTextAlign(13);
  text2->SetX(0.15);
  text2->SetTextFont(62);
  text2->SetTextSize(0.072);
  text2->SetY(0.905);
  // text2->Draw();

  TString preltext = "Supplementary";
  TLatex *text3 = new TLatex(3.5, 24, preltext);
  text3->SetNDC();
  text3->SetTextAlign(13);
  text3->SetX(0.263);
  text3->SetTextFont(52);
  text3->SetTextSize(0.05);
  text3->SetY(0.893);
  // text3->Draw();


  //12) care about axes
  TH1D* h = (TH1D*)g_expected_95->GetHistogram();
  h->GetXaxis()->SetRangeUser(mass[0], mass[n_points-1]);
  h->SetXTitle("M_{LQ} [GeV]");
  h->SetYTitle("#sigma_{LQLQ} #times #bf{#it{#Beta}}^{2}(LQ#rightarrow t#mu) [pb]");
  h->GetYaxis()->SetTitleSize(0.048);
  h->GetYaxis()->SetTitleOffset(1.05);
  h->Draw("AXIS SAME");
  gPad->SetLeftMargin(0.11);
  gPad->SetRightMargin(0.035);
  gPad->SetBottomMargin(0.11);


  c->SaveAs(AnalysisTool::theta_path + "output/limitplot_mc_fullsyst_much.eps");
  c->SaveAs(AnalysisTool::theta_path + "output/limitplot_mc_fullsyst_much.pdf");


}




void cosmetics(){
  TStyle *thesisStyle = new TStyle("thesisStyle","Style for Thesis"); //hard


  // For the canvas:
  thesisStyle->SetCanvasBorderMode(0);
  thesisStyle->SetCanvasColor(kWhite);
  thesisStyle->SetCanvasDefH(600); //Height of canvas
  thesisStyle->SetCanvasDefW(600); //Width of canvas
  thesisStyle->SetCanvasDefX(0);   //POsition on screen
  thesisStyle->SetCanvasDefY(0);

  // For the Pad:
  thesisStyle->SetPadBorderMode(0);
  // thesisStyle->SetPadBorderSize(Width_t size = 1);
  thesisStyle->SetPadColor(kWhite);
  thesisStyle->SetPadGridX(false);
  thesisStyle->SetPadGridY(false);
  thesisStyle->SetGridColor(0);
  thesisStyle-> SetPadTickX(1);
  thesisStyle->   SetPadTickY(1);
  //thesisStyle->SetGridStyle(3);
  //thesisStyle->SetGridWidth(1);

  // For the frame:
  thesisStyle->SetFrameBorderMode(0);
  thesisStyle->SetFrameBorderSize(3);
  thesisStyle->SetFrameFillColor(0);
  thesisStyle->SetFrameFillStyle(0);
  thesisStyle->SetFrameLineColor(1);
  thesisStyle->SetFrameLineStyle(1);
  thesisStyle->SetFrameLineWidth(3);

  // For the histo:
  // thesisStyle->SetHistFillColor(1);
  // thesisStyle->SetHistFillStyle(0);
  thesisStyle->SetHistLineColor(1);
  thesisStyle->SetHistLineStyle(0);
  thesisStyle->SetHistLineWidth(3);
  // thesisStyle->SetLegoInnerR(Float_t rad = 0.5);
  // thesisStyle->SetNumberContours(Int_t number = 20);

  thesisStyle->SetEndErrorSize(2);
  //  thesisStyle->SetErrorMarker(20);
  thesisStyle->SetErrorX(0.);

  thesisStyle->SetMarkerStyle(20);

  //For the fit/function:
  thesisStyle->SetOptFit(0);
  thesisStyle->SetFitFormat("5.4g");
  thesisStyle->SetFuncColor(2);
  thesisStyle->SetFuncStyle(1);
  thesisStyle->SetFuncWidth(3);

  //For the date:
  thesisStyle->SetOptDate(0);
  // thesisStyle->SetDateX(Float_t x = 0.01);
  // thesisStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  thesisStyle->SetOptFile(0);
  thesisStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  thesisStyle->SetStatColor(kWhite);
  thesisStyle->SetStatFont(42);
  thesisStyle->SetStatFontSize(0.025);
  thesisStyle->SetStatTextColor(1);
  thesisStyle->SetStatFormat("6.4g");
  thesisStyle->SetStatBorderSize(1);
  thesisStyle->SetStatH(0.1);
  thesisStyle->SetStatW(0.15);
  // thesisStyle->SetStatStyle(Style_t style = 1001);
  // thesisStyle->SetStatX(Float_t x = 0);
  // thesisStyle->SetStatY(Float_t y = 0);

  // Margins:
  thesisStyle->SetPadTopMargin(0.05);
  thesisStyle->SetPadBottomMargin(0.13);
  thesisStyle->SetPadLeftMargin(0.16);
  thesisStyle->SetPadRightMargin(0.06);

  // For the Global title:

  thesisStyle->SetOptTitle(0);
  thesisStyle->SetTitleFont(42);
  thesisStyle->SetTitleColor(1);
  thesisStyle->SetTitleTextColor(1);
  thesisStyle->SetTitleFillColor(10);
  thesisStyle->SetTitleFontSize(0.05);
  // thesisStyle->SetTitleH(0); // Set the height of the title box
  // thesisStyle->SetTitleW(0); // Set the width of the title box
  // thesisStyle->SetTitleX(0); // Set the position of the title box
  // thesisStyle->SetTitleY(0.985); // Set the position of the title box
  // thesisStyle->SetTitleStyle(Style_t style = 1001);
  // thesisStyle->SetTitleBorderSize(2);

  // For the axis titles:

  thesisStyle->SetTitleColor(1, "XYZ");
  thesisStyle->SetTitleFont(42, "XYZ");
  thesisStyle->SetTitleSize(0.055, "XYZ");
  // thesisStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // thesisStyle->SetTitleYSize(Float_t size = 0.02);
  thesisStyle->SetTitleXOffset(0.9);//0.9
  thesisStyle->SetTitleYOffset(1.25);//1.35
  // thesisStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  thesisStyle->SetLabelColor(1, "XYZ");
  thesisStyle->SetLabelFont(42, "XYZ");
  thesisStyle->SetLabelOffset(0.007, "XYZ");
  thesisStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:

  thesisStyle->SetAxisColor(1, "XYZ");
  thesisStyle->SetStripDecimals(kTRUE);
  thesisStyle->SetTickLength(0.03, "XYZ");
  thesisStyle->SetNdivisions(505, "XYZ");//510
  //thesisStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  //thesisStyle->SetPadTickY(1);
  thesisStyle->SetPadTickX(1);
  thesisStyle->SetPadTickY(1);
  thesisStyle->SetNdivisions(510, "Z");//510

  // Change for log plots:
  thesisStyle->SetOptLogx(0);
  thesisStyle->SetOptLogy(0);
  thesisStyle->SetOptLogz(0);

  // Postscript options:
  thesisStyle->SetPaperSize(20.,20.);
  // thesisStyle->SetLineScalePS(Float_t scale = 3);
  // thesisStyle->SetLineStyleString(Int_t i, const char* text);
  // thesisStyle->SetHeaderPS(const char* header);
  // thesisStyle->SetTitlePS(const char* pstitle);

  // thesisStyle->SetBarOffset(Float_t baroff = 0.5);
  // thesisStyle->SetBarWidth(Float_t barwidth = 0.5);
  // thesisStyle->SetPaintTextFormat(const char* format = "g");
  // thesisStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // thesisStyle->SetTimeOffset(Double_t toffset);
  // thesisStyle->SetHistMinimumZero(kTRUE);

  thesisStyle->cd();
}
