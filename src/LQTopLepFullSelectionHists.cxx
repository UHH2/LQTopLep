#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/MCWeight.h"
//hists
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/JetHists.h"
#include "UHH2/common/include/LuminosityHists.h"
#include "UHH2/common/include/EventHists.h"
//selections
#include "UHH2/common/include/LumiSelection.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/TriggerSelection.h"
#include "UHH2/common/include/AdditionalSelections.h"
//IDs
#include "UHH2/common/include/ElectronIds.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/common/include/JetIds.h"
// my own classes
#include "UHH2/LQTopLep/include/LQTopLepSelections.h"
#include "UHH2/LQTopLep/include/LQTopLepFullSelectionHists.h"
#include "UHH2/LQTopLep/include/LQTopLepModules.h"
#include "UHH2/LQTopLep/include/LQMassReconstruction.h"
#include "UHH2/LQTopLep/include/LQReconstructionHypothesis.h"
#include "UHH2/LQTopLep/include/LQReconstructionHypothesisDiscriminators.h"

#include <math.h>
#include "UHH2/core/include/Tags.h"
#include "UHH2/common/include/Utils.h"



#include "TH1F.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;


// invariant mass of a lorentzVector, but save for timelike / spacelike vectors
float inv_mass(const LorentzVector & p4) {
  if(p4.isTimelike()){
    return p4.mass();
  }
  else{
    //return -sqrt(-p4.mass2());
    return sqrt(-p4.mass2());
  }
}





LQTopLepFullSelectionHists::LQTopLepFullSelectionHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here
  // jets
  book<TH1F>("N_jets", "number of jets", 16, -0.5, 15.5);  
  book<TH1F>("pt_jets", "p_{T}^{jets} [GeV]", 50, 0, 1600); // pt of all jets in one histogram
  book<TH1F>("eta_jets", "#eta jets", 40, -2.5, 2.5);

  book<TH1F>("eta_jet1", "#eta^{jet 1}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet2", "#eta^{jet 2}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet3", "#eta^{jet 3}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet4", "#eta^{jet 4}", 40, -2.5, 2.5);
  book<TH1D>("pt_jet1", "p_{T}^{jet 1} [GeV]", 100, 0, 2300);
  book<TH1D>("pt_jet2", "p_{T}^{jet 2} [GeV]", 100, 0, 1700);
  book<TH1D>("pt_jet3", "p_{T}^{jet 3} [GeV]", 100, 0, 1000);
  book<TH1D>("pt_jet4", "p_{T}^{jet 4} [GeV]", 100, 0, 800);
  book<TH1D>("pt_jet5", "p_{T}^{jet 5} [GeV]", 100, 0, 800);
  book<TH1D>("pt_jet6", "p_{T}^{jet 6} [GeV]", 100, 0, 800);




  // bjets
  book<TH1F>("N_bjets_loose", "number of b-tagged jets N_{bjets}^{loose}", 11, -0.5, 10.5);
  book<TH1F>("N_bjets_med", "number of b-tagged jets N_{bjets}^{med}", 11, -0.5, 10.5);
  book<TH1F>("N_bjets_tight", "number of b-tagged jets N_{bjets}^{tight}", 11, -0.5, 10.5);



  // electrons
  book<TH1F>("N_e", "number of electrons", 9, -0.5, 8.5);
  book<TH1F>("pt_e", "p_{T}^{e} [GeV]", 50, 0, 1600);

  book<TH1F>("pt_e1", "p_{T}^{e 1} [GeV]", 60, 0, 2000);
  book<TH1F>("pt_e2", "p_{T}^{e 2} [GeV]", 60, 0, 2000);
  book<TH1F>("pt_e3", "p_{T}^{e 3} [GeV]", 60, 0, 2000);


  book<TH1F>("eta_e", "#eta electrons", 40, -2.1, 2.1);

  book<TH1F>("eta_e1", "#eta^{e 1}", 40, -2.1, 2.1);
  book<TH1F>("eta_e2", "#eta^{e 2}", 40, -2.1, 2.1);
  book<TH1F>("eta_e3", "#eta^{e 3}", 40, -2.1, 2.1);

  book<TH1F>("reliso_e", "e rel. Iso", 40, 0, 0.5);

  // muons
  book<TH1F>("N_mu", "number of muons", 7, -0.5, 6.5);
  book<TH1F>("pt_mu", "p_{T}^{#mu} [GeV]", 40, 0, 200);

  book<TH1F>("pt_mu1", "p_{T}^{#mu 1} [GeV]", 40, 0, 200);
  book<TH1F>("pt_mu2", "p_{T}^{#mu 2} [GeV]", 40, 0, 200);
  book<TH1F>("pt_mu3", "p_{T}^{#mu 3} [GeV]", 40, 0, 200);

  book<TH1F>("eta_mu", "#eta muons", 40, -2.1, 2.1);

  book<TH1F>("eta_mu1", "#eta^{#mu 1}", 40, -2.1, 2.1);
  book<TH1F>("eta_mu2", "#eta^{#mu 2}", 40, -2.1, 2.1);
  book<TH1F>("eta_mu3", "#eta^{#mu 3}", 40, -2.1, 2.1);

  book<TH1F>("reliso_mu", "#mu rel. Iso", 40, 0, 0.5);

  book<TH1F>("M_ee", "M_{ee} [GeV]",50 , 0, 1000);
  book<TH1F>("M_ee0to150", "M_{ee} [GeV]",30 , 0, 150);

  book<TH1F>("M_emu", "M_{e#mu} [GeV]",50 , 0, 1000);
  book<TH1F>("M_emu0to150", "M_{e#mu} [GeV]",30 , 0, 150);

  
  // electron and muon fakes
  book<TH1F>("n_fake_ele", "number of fake electrons in all events", 1, -0.5, 0.5);
  book<TH1F>("n_fake_ele_MLQ", "number of fake electrons, MLQ reconstructed", 1, -0.5, 0.5);
  book<TH1F>("n_fake_ele_NoMLQ", "number of fake electrons, MLQ not reconstructed", 1, -0.5, 0.5);

  book<TH1F>("n_fake_mu", "number of fake muons in all events", 1, -0.5, 0.5);
  book<TH1F>("n_fake_mu_MLQ", "number of fake muons, MLQ reconstructed", 1, -0.5, 0.5);
  book<TH1F>("n_fake_mu_NoMLQ", "number of fake muons, MLQ not reconstructed", 1, -0.5, 0.5);

  // primary vertices
  book<TH1F>("N_pv", "number of primary vertices", 50, 0, 50);
  book<TH1F>("N_pv2", "number of primary vertices", 80, 0, 80);
  // S_T, MET
  book<TH1D>("S_T", "S_{T} [GeV]", 200, 0, 7000);
  book<TH1D>("S_T_tinybins", "S_{T} [GeV]", 200, 0, 4500);
  book<TH1D>("S_T_smallbins", "S_{T} [GeV]", 100, 0, 4500);
  book<TH1D>("S_T_medbins", "S_{T} [GeV]", 50, 0, 4500);
  book<TH1D>("S_T_bigbins", "S_{T} [GeV]", 25, 0, 4500);
  vector<double> bins_ST_2lep = {350,525,700,875,1050,1225,1400,1575,1750,1925,2100,2450,3000};
  vector<double> morebins_ST_2lep = {350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1450,1600,1750,1900,2100,2400,3000};
  vector<double> bins_ST_3orMoreLep = {350,500,650,800,950,1250};
  book<TH1D>("S_T_limits1", "S_{T} [GeV]", bins_ST_2lep.size()-1, &bins_ST_2lep[0]);
  book<TH1D>("S_T_limits1moreBins", "S_{T} [GeV]", morebins_ST_2lep.size()-1, &morebins_ST_2lep[0]);
  book<TH1D>("S_T_limits2", "S_{T} [GeV]", bins_ST_3orMoreLep.size()-1, &bins_ST_3orMoreLep[0]);



  book<TH1D>("S_Tlep", "S_{T}^{lep} [GeV]", 60, 0, 2000);
  book<TH1D>("S_Tlep0to500", "S_{T}^{lep} [GeV]", 50, 0, 500);
  book<TH1D>("S_Tjets", "S_{T}^{jets} [GeV]", 100, 0, 2000);
  book<TH1D>("S_Tjets0to500", "S_{T}^{jets} [GeV]", 50, 0, 500);
  book<TH1D>("MET", "missing E_{T} [GeV]", 50, 0, 1000);

  // LQ mass reconstruction
  book<TH1D>("M_LQ_lep", "M_{LQ, lep} [GeV]", 60, 0, 3000);
  book<TH1D>("M_LQ_had", "M_{LQ, had} [GeV]", 60, 0, 3000);

  book<TH1D>("M_LQ", "M_{LQ, mean} [GeV]", 60, 0, 3000);
  book<TH1D>("M_LQ_smallbins", "M_{LQ, mean} [GeV]", 45, 0, 3000);
  book<TH1D>("M_LQ_medbins", "M_{LQ, mean} [GeV]", 30, 0, 3000);
  book<TH1D>("M_LQ_bigbins", "M_{LQ, mean} [GeV]", 15, 0, 3000);
  vector<double> bins_MLQ = {0,250,350,450,600,750,1000};
  book<TH1D>("M_LQ_limits", "M_{LQ, mean} [GeV]", bins_MLQ.size()-1, &bins_MLQ[0]);

  book<TH1F>("chi2", "#chi^{2}", 100, 0,200);
  book<TH1F>("chi2_smallbins", "#chi^{2}", 40, 0,200);
  book<TH1F>("chi2_medbins", "#chi^{2}", 20, 0,200);
  book<TH1F>("chi2_bigbins", "#chi^{2}", 10, 0,200);
  



  // general
  book<TH1D>("sum_event_weights", "BinContent = sum(eventweights)", 1, 0.5, 1.5);
  book<TH1D>("mindr_mujet", "min(#Delta R) between a muon and a jet", 50,0,5);
  book<TH1D>("mindr_elejet", "min(#Delta R) between an electron and a jet", 50,0,5);


  h_recohyps = ctx.get_handle<vector<LQReconstructionHypothesis>>("LQHypotheses");
  h_is_mlq_reconstructed = ctx.get_handle<bool>("is_mlq_reconstructed");
  h_mlq_reco_mode = ctx.get_handle<TString>("mlq_reco_mode");

  is_mc = ctx.get("dataset_type") == "MC";

  h_FakeRateWeightEleNominal =ctx.get_handle<double>("FakeRateWeightEle");
  h_FakeRateWeightMuNominal =ctx.get_handle<double>("FakeRateWeightMu");
}


void LQTopLepFullSelectionHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.
  double weight = event.weight;


  // cout << __LINE__ << endl;

  // jets
  std::vector<Jet>* jets = event.jets;
  int Njets = jets->size();
  hist("N_jets")->Fill(Njets, weight);

  for(int i=0; i <Njets; i++) {
    hist("pt_jets")->Fill(jets->at(i).pt(), weight);
    hist("eta_jets")->Fill(jets->at(i).eta(), weight);
  }
  

  if(Njets>=1){
    hist("eta_jet1")->Fill(jets->at(0).eta(), weight);
  }
  if(Njets>=2){
    hist("eta_jet2")->Fill(jets->at(1).eta(), weight);
  }
  if(Njets>=3){
    hist("eta_jet3")->Fill(jets->at(2).eta(), weight);
  }
  if(Njets>=4){
    hist("eta_jet4")->Fill(jets->at(3).eta(), weight);
  }
  if(Njets>=1){
    hist("pt_jet1")->Fill(jets->at(0).pt(), weight);
  }
  if(Njets>=2){
    hist("pt_jet2")->Fill(jets->at(1).pt(), weight);
  }
  if(Njets>=3){
    hist("pt_jet3")->Fill(jets->at(2).pt(), weight);
  }
  if(Njets>=4){
    hist("pt_jet4")->Fill(jets->at(3).pt(), weight);
  }
  if(Njets>=5){
    hist("pt_jet5")->Fill(jets->at(4).pt(), weight);
  }
  if(Njets>=6){
    hist("pt_jet6")->Fill(jets->at(5).pt(), weight);
  }


  // b jets
  std::vector<Jet> bjets_loose, bjets_med, bjets_tight;
  CSVBTag btag_loose = CSVBTag(CSVBTag::WP_LOOSE);
  CSVBTag btag_med = CSVBTag(CSVBTag::WP_MEDIUM);
  CSVBTag btag_tight = CSVBTag(CSVBTag::WP_TIGHT);

  for (unsigned int i=0; i < jets->size(); i++) {
    if(btag_loose(jets->at(i), event)) {
      bjets_loose.push_back(jets->at(i));
    }
    if(btag_med(jets->at(i), event)) {
      bjets_med.push_back(jets->at(i));
      }
    if(btag_tight(jets->at(i), event)) {
      bjets_tight.push_back(jets->at(i));   
    }
  }
  hist("N_bjets_loose")->Fill(bjets_loose.size(), weight);  
  hist("N_bjets_med")->Fill(bjets_med.size(), weight);
  hist("N_bjets_tight")->Fill(bjets_tight.size(), weight);




  // Muons
  std::vector<Muon>* muonVector = event.muons;
  int Nmuons = event.muons->size();
  hist("N_mu")->Fill(Nmuons, weight);
  for (const Muon & thismu : *event.muons){
      hist("pt_mu")->Fill(thismu.pt(), weight);
      hist("eta_mu")->Fill(thismu.eta(), weight);
      hist("reliso_mu")->Fill(thismu.relIso(), weight);
  }
  if(Nmuons>=1){
    hist("pt_mu1")->Fill(muonVector->at(0).pt(), weight);
    hist("eta_mu1")->Fill(muonVector->at(0).eta(), weight);
  }
  if(Nmuons>=2){
    hist("pt_mu2")->Fill(muonVector->at(1).pt(), weight);
    hist("eta_mu2")->Fill(muonVector->at(1).eta(), weight);
  }
  if(Nmuons>=3){
    hist("pt_mu3")->Fill(muonVector->at(2).pt(), weight);
    hist("eta_mu3")->Fill(muonVector->at(2).eta(), weight);
  }
  

  // Electrons  
  std::vector<Electron>* electronVector = event.electrons;

  const int Nele = event.electrons->size();
  hist("N_e")->Fill(Nele, weight);
  for (const Electron & thise : *event.electrons) {
    hist("pt_e")->Fill(thise.pt(), weight);
    hist("eta_e")->Fill(thise.eta(), weight);
    hist("reliso_e")->Fill(thise.relIso(), weight);
  }
  if(Nele>=1) { // should be in every case
    hist("pt_e1")->Fill(electronVector->at(0).pt(), weight);
    hist("eta_e1")->Fill(electronVector->at(0).eta(), weight);
  }
  if(Nele>=2) { // should be in every case
    hist("pt_e2")->Fill(electronVector->at(1).pt(), weight);
    hist("eta_e2")->Fill(electronVector->at(1).eta(), weight);
  }
  if(Nele>=3) {
    hist("pt_e3")->Fill(electronVector->at(2).pt(), weight);
    hist("eta_e3")->Fill(electronVector->at(2).eta(), weight);
  }

  // M_ee
  double m_ee;
  LorentzVector electrons[Nele];
  for(int i=0; i<Nele; i++){
    electrons[i] = event.electrons->at(i).v4();
  }
  for(int i=0; i<Nele; i++){
    for(int j=0; j<Nele; j++){
      if(j > i){
	m_ee = (electrons[i] + electrons[j]).M();
	hist("M_ee")->Fill(m_ee, weight);
	hist("M_ee0to150")->Fill(m_ee, weight);
      }
    }
  }

  //M_emu
  double m_emu;
  LorentzVector muons[Nmuons];
  for (int i=0; i<Nmuons; i++) {
    muons[i] = event.muons->at(i).v4();
  }
  for(int i=0; i<Nele; i++){
    for(int j=0; j<Nmuons; j++){
      m_emu = (electrons[i] + muons[j]).M();
      hist("M_emu")->Fill(m_emu, weight);
      hist("M_emu0to150")->Fill(m_emu, weight);
    }
  }

  // primary vertices
  int Npvs = event.pvs->size();
  hist("N_pv")->Fill(Npvs, weight);
  hist("N_pv2")->Fill(Npvs, weight);

  //ST
  auto met = event.met->pt();
  double st = 0.0;
  double st_jets = 0.0;
  double st_lep = 0.0;
  for(const auto & jet : *event.jets){
    st_jets += jet.pt();
  }
  //Bedeutung der for-Schleife
  /*const auto jets = event.jets;
    for(unsigned int i=0; i<jets.size();i++){
    Jet jet=jets[i];
    ht +=jet.pt();
    }*/
  for(const auto & electron : *event.electrons){
    st_lep += electron.pt();
  }
  for(const auto & muon : *event.muons){
    st_lep += muon.pt();
  }

  st = st_lep + st_jets + met;
  hist("S_T")->Fill(st, weight);
  hist("S_T_tinybins")->Fill(st, weight);
  hist("S_T_smallbins")->Fill(st, weight);
  hist("S_T_medbins")->Fill(st, weight);
  hist("S_T_bigbins")->Fill(st, weight);
  if(st < 3000) { // if Bedingung wahrscheinlich nicht notwendig
    hist("S_T_limits1")->Fill(st, weight);
    hist("S_T_limits1moreBins")->Fill(st, weight);
  }
  if(st >= 3000) {
    hist("S_T_limits1")->Fill(2999, weight); // Zahlen vielleicht noch anpassen
    hist("S_T_limits1moreBins")->Fill(2999, weight);
  }


  if(st < 1250) {
    hist("S_T_limits2")->Fill(st, weight);
  }
  if(st >= 1250) {
    hist("S_T_limits2")->Fill(1249, weight);
  }


  //ST lep, jet, MET
  hist("S_Tlep")->Fill(st_lep, weight);
  hist("S_Tlep0to500")->Fill(st_lep, weight);
  hist("S_Tjets")->Fill(st_jets, weight);
  hist("S_Tjets0to500")->Fill(st_jets, weight);
  hist("MET")->Fill(met, weight);

  // cout << __LINE__ << ", No of e: " << event.electrons->size()  << endl;
  // LQ mass reconstruction
  bool notTTbarRegion = (event.electrons->size() >=2); // only for 2+e case
  if (notTTbarRegion) {

  
    TString mlq_reco_mode = event.get(h_mlq_reco_mode);
    bool is_mlq_reconstructed = event.get(h_is_mlq_reconstructed);
 
    if(is_mlq_reconstructed) {

      vector<LQReconstructionHypothesis> recohyps = event.get(h_recohyps);
      const LQReconstructionHypothesis* besthyp = get_best_hypothesis(recohyps, "Chi2");
      double mlq_lep = inv_mass(besthyp->LQlep_v4());
      double mlq_had = inv_mass(besthyp->LQhad_v4());
      double mlq_avg = (mlq_lep + mlq_had)/2.0;
      double chi2 = besthyp->discriminator("Chi2");

      hist("M_LQ_lep")->Fill(mlq_avg, weight);
      hist("M_LQ_had")->Fill(mlq_avg, weight);
      hist("M_LQ")->Fill(mlq_avg, weight);
      hist("M_LQ_smallbins")->Fill(mlq_avg, weight);
      hist("M_LQ_medbins")->Fill(mlq_avg, weight);
      hist("M_LQ_bigbins")->Fill(mlq_avg, weight);
    
      if(mlq_avg < 1000) {
	hist("M_LQ_limits")->Fill(mlq_avg, weight);
      }
      else {
	hist("M_LQ_limits")->Fill(999, weight);
      }

      hist("chi2")->Fill(chi2, weight);
      hist("chi2_smallbins")->Fill(chi2, weight);
      hist("chi2_medbins")->Fill(chi2, weight);
      hist("chi2_bigbins")->Fill(chi2, weight);


    }
  }

    



  //sum_event_weights
  hist("sum_event_weights")->Fill(1., weight);

  //min_deltaR Myon
  double mindr_mujet = 999;
  for(const Muon & thismu : *event.muons){
    for(const Jet & thisjet : *event.jets){
      double dr = deltaR(thismu,thisjet);
      if (dr < mindr_mujet) mindr_mujet = dr;
    }
    hist("mindr_mujet")->Fill(mindr_mujet,weight);
  }
  //min_deltaR Electron 
  double mindr_elejet = 999;
  for(const Electron & thisele : *event.electrons){
    for(const Jet & thisjet : *event.jets){
      double dr = deltaR(thisele,thisjet);
      if (dr < mindr_elejet) mindr_elejet = dr;
    }
    hist("mindr_elejet")->Fill(mindr_elejet,weight);
  }

  // electron and muons fakes

  double SF_ele = 1;
  double SF_mu = 1;

  if(is_mc) {
    
    SF_ele = event.get(h_FakeRateWeightEleNominal);
    SF_mu = event.get(h_FakeRateWeightMuNominal);

    bool is_fake_ele = (SF_ele != 1);
    bool is_fake_mu =  (SF_mu  != 1);



    if(is_fake_ele) {
      hist("n_fake_ele")->Fill(0., weight);
      if (notTTbarRegion) {
	bool is_mlq_reconstructed = event.get(h_is_mlq_reconstructed);
	if(is_mlq_reconstructed) hist("n_fake_ele_MLQ")->Fill(0., weight);
	else                     hist("n_fake_ele_NoMLQ")->Fill(0., weight);
      }
    }
    if(is_fake_mu) {
      hist("n_fake_mu")->Fill(0., weight);
      if (notTTbarRegion) {
	bool is_mlq_reconstructed = event.get(h_is_mlq_reconstructed);
	if(is_mlq_reconstructed) hist("n_fake_mu_MLQ")->Fill(0., weight);
	else                     hist("n_fake_mu_NoMLQ")->Fill(0., weight);
      }
    }
    
  }


}

LQTopLepFullSelectionHists::~LQTopLepFullSelectionHists(){}
