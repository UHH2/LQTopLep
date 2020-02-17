#include "../include/LQTopLepTriggerHists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/common/include/JetIds.h"
#include <math.h>

#include "TH1F.h"
#include "TH2D.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

LQTopLepTriggerHists::LQTopLepTriggerHists(Context & ctx, const string & dirname): Hists(ctx, dirname){

  // book all histograms here
  book<TH1F>("pt_ele_fine", "p_{T}^{e} [GeV]", 200, 0, 1000);
  double bins[13] = {0,30,65,100,150,200,250,300,350,400,500,700,1000};
  book<TH1F>("pt_ele_binned", "p_{T}^{e} [GeV]", 12, bins);
  book<TH1F>("eta_ele", "#eta^{e}", 24, -2.4, 2.4);
  book<TH1F>("eta_ele_binned", "#eta^{e}", 8, -2.4, 2.4);
  book<TH1D>("dR_ele_mu", "#Delta R(e, #mu)", 50, 0, 5);
  book<TH1D>("dRmin_ele_jet", "#Delta R^{min}(e, jet)", 50, 0, 5);
  book<TH1D>("dRmin_ele_obj", "#Delta R^{min}(e, obj)", 50, 0, 5);

  book<TH1F>("pt_mu_fine", "p_{T}^{#mu} [GeV]", 200, 0, 1000);
  book<TH1F>("pt_mu_binned", "p_{T}^{#mu} [GeV]", 12, bins);
  book<TH1F>("eta_mu", "#eta^{#mu}", 24, -2.4, 2.4);
  book<TH1F>("eta_mu_binned", "#eta^{#mu}", 8, -2.4, 2.4);
  book<TH1D>("dR_mu_ele", "#Delta R(#mu, e)", 50, 0, 5);
  book<TH1D>("dRmin_mu_jet", "#Delta R^{min}(#mu, jet)", 50, 0, 5);
  book<TH1D>("dRmin_mu_obj", "#Delta R^{min}(#mu, obj)", 50, 0, 5);
}


void LQTopLepTriggerHists::fill(const Event & event){

  // Don't forget to always use the weight when filling.
  double weight = event.weight;

  if(event.electrons->size() != 1 || event.muons->size() != 1) return;
  // if(event.electrons->size() != 1 || event.muons->size() != 1) throw runtime_error("In LQTopLepTriggerHists.cxx: Not ==1 electron or muon --> this is needed for Tag&Probe selection.");
  double pt_ele = event.electrons->at(0).pt();
  double eta_ele = event.electrons->at(0).eta();
  double pt_mu = event.muons->at(0).pt();
  double eta_mu = event.muons->at(0).eta();

  hist("pt_ele_fine")->Fill(pt_ele, weight);
  hist("pt_ele_binned")->Fill(pt_ele, weight);
  hist("eta_ele")->Fill(eta_ele, weight);
  hist("eta_ele_binned")->Fill(eta_ele, weight);

  hist("pt_mu_fine")->Fill(pt_mu, weight);
  hist("pt_mu_binned")->Fill(pt_mu, weight);
  hist("eta_mu")->Fill(eta_mu, weight);
  hist("eta_mu_binned")->Fill(eta_mu, weight);

  double dR_e_mu = deltaR(event.electrons->at(0),event.muons->at(0));
  hist("dR_ele_mu")->Fill(dR_e_mu,weight);
  hist("dR_mu_ele")->Fill(dR_e_mu,weight);

  double dR_min_e_jet = 999999;
  for(unsigned int i=0; i<event.jets->size(); i++){
    double dr = deltaR(event.electrons->at(0),event.jets->at(i));
    if(dr < dR_min_e_jet) dR_min_e_jet = dr;
  }
  hist("dRmin_ele_jet")->Fill(dR_min_e_jet,weight);

  double dR_min_mu_jet = 999999;
  for(unsigned int i=0; i<event.jets->size(); i++){
    double dr = deltaR(event.muons->at(0),event.jets->at(i));
    if(dr < dR_min_mu_jet) dR_min_mu_jet = dr;
  }
  hist("dRmin_mu_jet")->Fill(dR_min_mu_jet,weight);

  double dR_min_e_obj = min(dR_e_mu, dR_min_e_jet);
  hist("dRmin_ele_obj")->Fill(dR_min_e_obj,weight);

  double dR_min_mu_obj = min(dR_e_mu, dR_min_mu_jet);
  hist("dRmin_mu_obj")->Fill(dR_min_mu_obj,weight);

} //Methode



LQTopLepTriggerHists::~LQTopLepTriggerHists(){}
