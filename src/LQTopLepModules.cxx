#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/LQTopLep/include/LQTopLepModules.h"
#include "UHH2/common/include/JetIds.h"
#include "UHH2/common/include/JetCorrections.h"
#include <TH1D.h>


using namespace uhh2;
using namespace std;



MuonTriggerWeights::MuonTriggerWeights(Context & ctx, TString path_, Year year_): path(path_), year(year_){

  h_muon_weight      = ctx.declare_event_output<float>("weight_sfmu_trigger");
  h_muon_weight_up   = ctx.declare_event_output<float>("weight_sfmu_trigger_up");
  h_muon_weight_down = ctx.declare_event_output<float>("weight_sfmu_trigger_down");

  auto dataset_type = ctx.get("dataset_type");
  bool is_mc = dataset_type == "MC";
  if(!is_mc){
    cout << "Warning: MuonTriggerWeights will not have an effect on this non-MC sample (dataset_type = '" + dataset_type + "')" << endl;
    return;
  }

  TString yeartag = "2016";
  if(year == Year::is2017v1 || year == Year::is2017v2) yeartag = "2017";
  else if(year == Year::is2018) yeartag = "2018";
  unique_ptr<TFile> file_30to50, file_50to100, file_100to200, file_200toinf;
  file_30to50.reset(new TFile(path+"/" + yeartag + "/MuonTriggerScaleFactors_eta_mu_binned_pt30to50.root","READ"));
  file_50to100.reset(new TFile(path+"/" + yeartag + "/MuonTriggerScaleFactors_eta_mu_binned_pt50to100.root","READ"));
  file_100to200.reset(new TFile(path+"/" + yeartag + "/MuonTriggerScaleFactors_eta_mu_binned_pt100to200.root","READ"));
  file_200toinf.reset(new TFile(path+"/" + yeartag + "/MuonTriggerScaleFactors_eta_mu_binned_pt200toInf.root","READ"));

  g_sf_30to50.reset((TGraphAsymmErrors*)file_30to50->Get("ScaleFactors"));
  g_sf_50to100.reset((TGraphAsymmErrors*)file_50to100->Get("ScaleFactors"));
  g_sf_100to200.reset((TGraphAsymmErrors*)file_100to200->Get("ScaleFactors"));
  g_sf_200toinf.reset((TGraphAsymmErrors*)file_200toinf->Get("ScaleFactors"));

}

bool MuonTriggerWeights::process(Event & event){

  if(event.isRealData){
    event.set(h_muon_weight, 1.);
    event.set(h_muon_weight_up, 1.);
    event.set(h_muon_weight_down, 1.);
    return true;
  }

  const Muon muon = event.muons->at(0);
  double eta = muon.eta();
  double pt = muon.pt();
  if(fabs(eta) > 2.4) throw runtime_error("In LQToTopMuModules.cxx, MuonTriggerWeights.process(): Muon-|eta| > 2.4 is not supported at the moment.");
  if(pt < 30) throw runtime_error("In LQToTopMuModules.cxx, MuonTriggerWeights.process(): Muon-pt < 30 is not supported at the moment.");

  // find number of correct eta bin
  int idx = 0;
  if(pt < 50){
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_30to50->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_30to50->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  if(pt < 100){
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_50to100->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_50to100->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  else if (pt < 200){
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_100to200->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_100to200->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  else{
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_200toinf->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_200toinf->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }


  //access scale factors, add 2% t&p systematic uncertainty
  double sf, sf_up, sf_down, dummy_x;
  double stat_up = -1., stat_down = -1., tp = 0.02, total_up = -1., total_down = -1.;
  if(pt < 50){
    g_sf_30to50->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_30to50->GetErrorYhigh(idx);
    stat_down = g_sf_30to50->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }
  if(pt < 100){
    g_sf_50to100->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_50to100->GetErrorYhigh(idx);
    stat_down = g_sf_50to100->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }
  else if(pt < 200){
    g_sf_100to200->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_100to200->GetErrorYhigh(idx);
    stat_down = g_sf_100to200->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }
  else{
    g_sf_200toinf->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_200toinf->GetErrorYhigh(idx);
    stat_down = g_sf_200toinf->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }

  event.weight *= sf;
  event.set(h_muon_weight, sf);
  event.set(h_muon_weight_up, sf_up);
  event.set(h_muon_weight_down, sf_down);
  return true;
}




ElectronTriggerWeights::ElectronTriggerWeights(Context & ctx, TString path_, Year year_): path(path_), year(year_){

  h_ele_weight      = ctx.declare_event_output<float>("weight_sfelec_trigger");
  h_ele_weight_up   = ctx.declare_event_output<float>("weight_sfelec_trigger_up");
  h_ele_weight_down = ctx.declare_event_output<float>("weight_sfelec_trigger_down");

  auto dataset_type = ctx.get("dataset_type");
  bool is_mc = dataset_type == "MC";
  if(!is_mc){
    cout << "Warning: ElectronTriggerWeights will not have an effect on this non-MC sample (dataset_type = '" + dataset_type + "')" << endl;
    return;
  }

  TString yeartag = "2016";
  if(year == Year::is2017v1 || year == Year::is2017v2) yeartag = "2017";
  else if(year == Year::is2018) yeartag = "2018";
  unique_ptr<TFile> file_pt1, file_pt2, file_pt3, file_pt4;
  file_pt1.reset(new TFile(path+"/" + yeartag + "/ElectronTriggerScaleFactors_eta_ele_binned_pt30to50.root","READ"));
  file_pt2.reset(new TFile(path+"/" + yeartag + "/ElectronTriggerScaleFactors_eta_ele_binned_pt50to100.root","READ"));
  if(yeartag == "2016"){
    file_pt3.reset(new TFile(path+"/" + yeartag + "/ElectronTriggerScaleFactors_eta_ele_binned_pt100to175.root","READ"));
    file_pt4.reset(new TFile(path+"/" + yeartag + "/ElectronTriggerScaleFactors_eta_ele_binned_pt175toInf.root","READ"));
  }
  else throw runtime_error("In ElectronTriggerWeights::ElectronTriggerWeights: Specify SF files for 2017 and 2018 for high-pt thresholds");

  if(yeartag == "2016") pt_bins = {30, 50, 100, 175};

  g_sf_pt1.reset((TGraphAsymmErrors*)file_pt1->Get("ScaleFactors"));
  g_sf_pt2.reset((TGraphAsymmErrors*)file_pt2->Get("ScaleFactors"));
  g_sf_pt3.reset((TGraphAsymmErrors*)file_pt3->Get("ScaleFactors"));
  g_sf_pt4.reset((TGraphAsymmErrors*)file_pt4->Get("ScaleFactors"));

}

bool ElectronTriggerWeights::process(Event & event){

  if(event.isRealData){
    event.set(h_ele_weight, 1.);
    event.set(h_ele_weight_up, 1.);
    event.set(h_ele_weight_down, 1.);
    return true;
  }

  const Electron ele = event.electrons->at(0);
  double eta = ele.eta();
  double pt = ele.pt();
  if(fabs(eta) > 2.4) throw runtime_error("In LQToTopMuModules.cxx, ElectronTriggerWeights.process(): ele-|eta| > 2.4 is not supported at the moment.");
  if(pt < pt_bins[0]) throw runtime_error("In LQToTopMuModules.cxx, ElectronTriggerWeights.process(): too small ele-pt for this year, not supported.");

  // find number of correct eta bin
  int idx = 0;
  if(pt < pt_bins[1]){
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_pt1->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_pt1->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  if(pt < pt_bins[2]){
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_pt2->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_pt2->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  else if (pt < pt_bins[3]){
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_pt3->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_pt3->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  else{
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_pt4->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_pt4->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }


  //access scale factors, add 2% t&p systematic uncertainty
  double sf, sf_up, sf_down, dummy_x;
  double stat_up = -1., stat_down = -1., tp = 0.02, total_up = -1., total_down = -1.;
  if(pt < pt_bins[1]){
    g_sf_pt1->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_pt1->GetErrorYhigh(idx);
    stat_down = g_sf_pt1->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }
  if(pt < pt_bins[2]){
    g_sf_pt2->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_pt2->GetErrorYhigh(idx);
    stat_down = g_sf_pt2->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }
  else if(pt < pt_bins[3]){
    g_sf_pt3->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_pt3->GetErrorYhigh(idx);
    stat_down = g_sf_pt3->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }
  else{
    g_sf_pt4->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_pt4->GetErrorYhigh(idx);
    stat_down = g_sf_pt4->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }

  event.weight *= sf;
  event.set(h_ele_weight, sf);
  event.set(h_ele_weight_up, sf_up);
  event.set(h_ele_weight_down, sf_down);
  return true;
}


























// ElectronTriggerWeights::ElectronTriggerWeights(Context & ctx, TString path_, TString SysDirection_): path(path_), SysDirection(SysDirection_){
//
//   auto dataset_type = ctx.get("dataset_type");
//   bool is_mc = dataset_type == "MC";
//   if(!is_mc){
//     cout << "Warning: ElectronTriggerWeights will not have an effect on this non-MC sample (dataset_type = '" + dataset_type + "')" << endl;
//     return;
//   }
//
//   unique_ptr<TFile> file;
//   file.reset(new TFile(path,"READ"));
//
//   Eff_lowpt_MC.reset((TGraphAsymmErrors*)file->Get("gr_lowpt_eta_TTbar_eff"));
//   Eff_highpt_MC.reset((TGraphAsymmErrors*)file->Get("gr_highpt_eta_TTbar_eff"));
//   Eff_lowpt_DATA.reset((TGraphAsymmErrors*)file->Get("gr_lowpt_eta_DATA_eff"));
//   Eff_highpt_DATA.reset((TGraphAsymmErrors*)file->Get("gr_highpt_eta_DATA_eff"));
//
//   if(SysDirection != "nominal" && SysDirection != "up" && SysDirection != "down") throw runtime_error("In LQToTopMuModules.cxx, ElectronTriggerWeights.process(): Invalid SysDirection specified.");
//
// }
//
// bool ElectronTriggerWeights::process(Event & event){
//
//   if(event.isRealData) return true;
//
//   const auto ele = event.electrons->at(0);
//   double eta = ele.eta();
//   if(fabs(eta) > 2.4) throw runtime_error("In LQToTopMuModules.cxx, ElectronTriggerWeights.process(): Ele-|eta| > 2.4 is not supported at the moment.");
//
//
//   //find right bin in eta
//   int idx = 0;
//   bool lowpt = false;
//   if(30 <= ele.pt() && ele.pt() < 120){
//     lowpt = true;
//     //lowpt trigger
//     bool keep_going = true;
//     while(keep_going){
//       double x,y;
//       Eff_lowpt_MC->GetPoint(idx,x,y);
//       keep_going = eta > x + Eff_lowpt_MC->GetErrorXhigh(idx);
//       if(keep_going) idx++;
//     }
//   }
//   else if(ele.pt() >= 120){
//     //highpt trigger
//     bool keep_going = true;
//     while(keep_going){
//       double x,y;
//       Eff_highpt_MC->GetPoint(idx,x,y);
//       keep_going = eta > x + Eff_highpt_MC->GetErrorXhigh(idx);
//       if(keep_going) idx++;
//     }
//   }
//   else throw runtime_error("In LQToTopMuModules.cxx, ElectronTriggerWeights.process(): Electron has pt<30. Clean electron collection before applying weights.");
//
//   //access efficiencies for MC and DATA, possibly accout for systematics = statistical + add. 2% up/down
//   double eff_data = -1, eff_mc = -1, dummy_x;
//   double stat_data = -1, stat_mc = -1, tp = 0.02, total_syst_data = -1, total_syst_mc = -1;
//   if(lowpt){
//     Eff_lowpt_MC->GetPoint(idx,dummy_x,eff_mc);
//     Eff_lowpt_DATA->GetPoint(idx,dummy_x,eff_data);
//
//     if(SysDirection == "up"){
//       stat_mc = Eff_lowpt_MC->GetErrorYlow(idx);
//       stat_data = Eff_lowpt_DATA->GetErrorYhigh(idx);
//       total_syst_mc = sqrt(pow(stat_mc,2) + pow(tp,2));
//       total_syst_data = sqrt(pow(stat_data,2) + pow(tp,2));
//
//       eff_mc -= total_syst_mc;
//       eff_data += total_syst_data;
//     }
//     else if(SysDirection == "down"){
//       stat_mc = Eff_lowpt_MC->GetErrorYhigh(idx);
//       stat_data = Eff_lowpt_DATA->GetErrorYlow(idx);
//       total_syst_mc = sqrt(pow(stat_mc,2) + pow(tp,2));
//       total_syst_data = sqrt(pow(stat_data,2) + pow(tp,2));
//
//       eff_mc += Eff_lowpt_MC->GetErrorYhigh(idx);
//       eff_data -= Eff_lowpt_DATA->GetErrorYlow(idx);
//     }
//   }
//   else{
//     Eff_highpt_MC->GetPoint(idx,dummy_x,eff_mc);
//     Eff_highpt_DATA->GetPoint(idx,dummy_x,eff_data);
//
//     if(SysDirection == "up"){
//       stat_mc = Eff_highpt_MC->GetErrorYlow(idx);
//       stat_data = Eff_highpt_DATA->GetErrorYhigh(idx);
//       total_syst_mc = sqrt(pow(stat_mc,2) + pow(tp,2));
//       total_syst_data = sqrt(pow(stat_data,2) + pow(tp,2));
//
//       eff_mc -= Eff_highpt_MC->GetErrorYlow(idx);
//       eff_data += Eff_highpt_DATA->GetErrorYhigh(idx);
//     }
//     else if(SysDirection == "down"){
//       stat_mc = Eff_highpt_MC->GetErrorYhigh(idx);
//       stat_data = Eff_highpt_DATA->GetErrorYlow(idx);
//       total_syst_mc = sqrt(pow(stat_mc,2) + pow(tp,2));
//       total_syst_data = sqrt(pow(stat_data,2) + pow(tp,2));
//
//       eff_mc += Eff_highpt_MC->GetErrorYhigh(idx);
//       eff_data -= Eff_highpt_DATA->GetErrorYlow(idx);
//     }
//   }
//
//   //Scale weight by (eff_data) / (eff_mc)
//   double SF = eff_data/eff_mc;
//   event.weight *= SF;
//
//   return true;
// }



JetCorrectorVariable::JetCorrectorVariable(uhh2::Context & ctx, const std::vector<std::string> & JEC_files): JetCorrector(ctx, JEC_files){}

bool JetCorrectorVariable::correct_collection(uhh2::Event & event, std::vector<Jet> & jets){

  //apply jet corrections
  for(auto & jet : jets){
    correct_jet(*corrector, jet, event, jec_uncertainty, direction);
  }
  return true;
};


/*

ElectronFakeRateWeights::ElectronFakeRateWeights(Context & ctx, const std::vector<std::string> & JEC_files, TString path_, TString SysDirection_, const string label_jets, const string label_genjets):  path(path_), SysDirection(SysDirection_){

auto dataset_type = ctx.get("dataset_type");
bool is_mc = dataset_type == "MC";
if(!is_mc){
cout << "Warning: ElectronFakeRateWeights will not have an effect on this non-MC sample (dataset_type = '" + dataset_type + "')" << endl;
return;
}

unique_ptr<TFile> file;
file.reset(new TFile(path,"READ"));

SF.reset((TGraphAsymmErrors*)file->Get("ScaleFactors"));
//Read the SF file and store all SFs and their pt-range
n_points = SF->GetN();
for(int i=0; i<n_points; i++){
double x,y;
SF->GetPoint(i,x,y);
x_low.emplace_back(x-SF->GetErrorXlow(i));
x_high.emplace_back(x+SF->GetErrorXhigh(i));
}

if(SysDirection != "nominal" && SysDirection != "up" && SysDirection != "down") throw runtime_error("In LQToTopMuModules.cxx, ElectronFakeRateWeights.process(): Invalid SysDirection specified.");

jet_corrector.reset(new JetCorrectorVariable(ctx, JEC_files));

bool jer_was_applied = ctx.get("meta_jer_applied", "") == "true";
if(jer_was_applied) ctx.set_metadata("jer_applied", "false", true);
//jet_smearer.reset(new JetSmearerVariable(ctx, label_jets, label_genjets, JERSmearing::SF_13TeV_2016));
jet_smearer.reset(new GenericJetResolutionSmearer(ctx, label_jets, label_genjets, true, JERSmearing::SF_13TeV_2016));
if(!jer_was_applied) ctx.set_metadata("jer_applied", "false", true);

jet_id = AndId<Jet>(JetPFID(JetPFID::WP_LOOSE), PtEtaCut(30.0, 2.4));

FakeRateWeightEle = ctx.get_handle<double>("FakeRateWeightEle");
FakeRateWeightEleUp = ctx.get_handle<double>("FakeRateWeightEleUp");
FakeRateWeightEleDown = ctx.get_handle<double>("FakeRateWeightEleDown");
h_jets = ctx.get_handle<std::vector<Jet>>(label_jets);

}

bool ElectronFakeRateWeights::process(Event & event){


vector<Jet> *jets = &event.get(h_jets);
jet_corrector->correct_collection(event, *jets);
jet_smearer->process(event);

if(event.isRealData || event.electrons->size() < 1){
event.set(FakeRateWeightEle,1.);
event.set(FakeRateWeightEleUp,1.);
event.set(FakeRateWeightEleDown,1.);
return false;
}

//find fake-electrons
vector<bool> is_fake;

//if the number of gen and reco-muons are the same, assume muons are real
unsigned int n_genele = 0;
for(const auto & gp : *event.genparticles){
if(fabs(gp.pdgId()) != 11) continue;
n_genele++;
}
//no fakes if number of gen and reco electrons is the same
if(n_genele == event.electrons->size()){
event.set(FakeRateWeightEle,1.);
event.set(FakeRateWeightEleUp,1.);
event.set(FakeRateWeightEleDown,1.);
return false;
}
else{
//if ngen and nreco are unequal, try to match muons to muons within 0.1 and to taus within 0.2
unsigned int n_matched_to_ele = 0, n_matched_to_tau = 0, n_matched_to_b = 0, n_matched_to_c = 0;
for(const auto & ele : *event.electrons){
bool is_matched = false;
for(const auto & gp : *event.genparticles){
if(fabs(gp.pdgId()) == 11){
if(deltaR(gp,ele) < 0.1 && !is_matched){
is_matched = true;
n_matched_to_ele++;
}
}
else if(fabs(gp.pdgId()) == 15){
if(deltaR(gp,ele) < 0.2 && !is_matched){
is_matched = true;
n_matched_to_tau++;
}
}
else if(fabs(gp.pdgId()) == 5){
if(deltaR(gp,ele) < 0.2 && !is_matched){
is_matched = true;
n_matched_to_b++;
}
}
else if(fabs(gp.pdgId()) == 4){
if(deltaR(gp,ele) < 0.2 && !is_matched){
is_matched = true;
n_matched_to_c++;
}
}
}
is_fake.push_back(!is_matched);
}
if(n_matched_to_tau + n_matched_to_ele + n_matched_to_b + n_matched_to_c == event.electrons->size()){
event.set(FakeRateWeightEle,1.);
event.set(FakeRateWeightEleUp,1.);
event.set(FakeRateWeightEleDown,1.);
return false;
}
}

//find jets matching fake-electrons
vector<bool> faking_jet;
for(auto & jet : *jets){

bool faking = false;
double dr_min = 999;
for(unsigned int i=0; i<event.electrons->size(); i++){

if(!is_fake[i]) continue;
//consider only jets to be eligible for faking the electron that fulfill the jetId used when deriving the scale factors
if(!jet_id(jet,event)) continue;

if(deltaR(event.electrons->at(i), jet) < 0.4) faking = true;
if(deltaR(event.electrons->at(i), jet) < dr_min) dr_min = deltaR(event.electrons->at(i), jet);
}
faking_jet.push_back(faking);
}


//apply SF to the eventweight for each faking jet
double SF_final = 1, SF_final_up = 1, SF_final_down = 1;
for(unsigned int i=0; i<jets->size(); i++){
if(!faking_jet[i]) continue;
double pt = event.jets->at(i).pt();

//find right bin
int bin = -1;
for(int j=0; j<n_points; j++){
if(pt >= x_low[j] && pt < x_high[j]) bin = j;
}

//possibly accout for systematics = statistical, inflate stat unc. by 50% if >800 GeV
double x,scale_factor, scale_factor_up, scale_factor_down, mult = 1;
if(pt > x_high[n_points-1]){
bin = n_points - 1;
mult = 1.5;
}
else if(bin == -1) throw runtime_error("In ElectronFakeRateWeights::process: Did not find right SF-bin for this jet.");

SF->GetPoint(bin,x,scale_factor);
scale_factor_up   = scale_factor + mult * SF->GetErrorYhigh(bin);
scale_factor_down = scale_factor - mult * SF->GetErrorYlow(bin);

if(SysDirection == "up") event.weight *= scale_factor_up;
else if(SysDirection == "down") event.weight *= scale_factor_down;
else if(SysDirection == "nominal") event.weight *= scale_factor;
else throw runtime_error("In ElectronFakeRateWeights::process(): SysDirection is not one of the following: ['up', 'down', 'nominal']");

SF_final      *= scale_factor;
SF_final_up   *= scale_factor_up;
SF_final_down *= scale_factor_down;

}

event.set(FakeRateWeightEle,SF_final);
event.set(FakeRateWeightEleUp,SF_final_up);
event.set(FakeRateWeightEleDown,SF_final_down);


return true;
}





MuonFakeRateWeights::MuonFakeRateWeights(Context & ctx, TString path_, TString SysDirection_):  path(path_), SysDirection(SysDirection_){

auto dataset_type = ctx.get("dataset_type");
bool is_mc = dataset_type == "MC";
if(!is_mc){
cout << "Warning: MuonFakeRateWeights will not have an effect on this non-MC sample (dataset_type = '" + dataset_type + "')" << endl;
return;
}

unique_ptr<TFile> file;
file.reset(new TFile(path,"READ"));
SF.reset((TGraphAsymmErrors*)file->Get("ScaleFactors"));

if(SysDirection != "nominal" && SysDirection != "up" && SysDirection != "down") throw runtime_error("In LQToTopMuModules.cxx, MuonFakeRateWeights.process(): Invalid SysDirection specified.");


FakeRateWeightMu = ctx.get_handle<double>("FakeRateWeightMu");
FakeRateWeightMuUp = ctx.get_handle<double>("FakeRateWeightMuUp");
FakeRateWeightMuDown = ctx.get_handle<double>("FakeRateWeightMuDown");

}

bool MuonFakeRateWeights::process(Event & event){

if(event.isRealData || event.muons->size() < 1){
event.set(FakeRateWeightMu,1.);
event.set(FakeRateWeightMuUp,1.);
event.set(FakeRateWeightMuDown,1.);
return false;
}

//find fake-muons
vector<bool> is_fake;
unsigned int n_genmu = 0;
for(const auto & gp : *event.genparticles){
if(fabs(gp.pdgId()) != 13) continue;
n_genmu++;
}

if(n_genmu == event.muons->size()){
event.set(FakeRateWeightMu,1.);
event.set(FakeRateWeightMuUp,1.);
event.set(FakeRateWeightMuDown,1.);
return false;
}
else{
//if ngen and nreco are unequal, try to match muons to muons within 0.1 and to taus within 0.2
unsigned int n_matched_to_muons = 0, n_matched_to_taus = 0, n_matched_to_b = 0, n_matched_to_c = 0;
for(const auto & mu : *event.muons){
bool is_matched = false;
for(const auto & gp : *event.genparticles){
if(fabs(gp.pdgId()) == 13){
if(deltaR(gp,mu) < 0.1 && !is_matched){
is_matched = true;
n_matched_to_muons++;
}
}
else if(fabs(gp.pdgId()) == 15){
if(deltaR(gp,mu) < 0.2 && !is_matched){
is_matched = true;
n_matched_to_taus++;
}
}
else if(fabs(gp.pdgId()) == 5){
if(deltaR(gp,mu) < 0.2 && !is_matched){
is_matched = true;
n_matched_to_b++;
}
}
else if(fabs(gp.pdgId()) == 4){
if(deltaR(gp,mu) < 0.2 && !is_matched){
is_matched = true;
n_matched_to_c++;
}
}
}
is_fake.push_back(!is_matched);
}
if(n_matched_to_taus + n_matched_to_muons + n_matched_to_b + n_matched_to_c == event.muons->size()){
event.set(FakeRateWeightMu,1.);
event.set(FakeRateWeightMuUp,1.);
event.set(FakeRateWeightMuDown,1.);
return false;
}
}


//apply SF to the eventweight for each fake muon
double SF_final = 1, SF_final_up = 1, SF_final_down = 1;
for(unsigned int i=0; i<event.muons->size(); i++){
if(!is_fake[i]) continue;
int bin = 0;

double x,scale_factor, scale_factor_up, scale_factor_down;
SF->GetPoint(bin,x,scale_factor);
scale_factor_up   = scale_factor + SF->GetErrorYhigh(bin);
scale_factor_down = scale_factor - SF->GetErrorYlow(bin);

if(SysDirection == "up") event.weight *= scale_factor_up;
else if(SysDirection == "down") event.weight *= scale_factor_down;
else if(SysDirection == "nominal") event.weight *= scale_factor;
else throw runtime_error("In MuonFakeRateWeights::process(): SysDirection is not one of the following: ['up', 'down', 'nominal']");

SF_final      *= scale_factor;
SF_final_up   *= scale_factor_up;
SF_final_down *= scale_factor_down;

}

event.set(FakeRateWeightMu,SF_final);
event.set(FakeRateWeightMuUp,SF_final_up);
event.set(FakeRateWeightMuDown,SF_final_down);


return true;
}
*/
