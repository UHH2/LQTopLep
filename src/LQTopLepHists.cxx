#include "../include/LQTopLepHists.h"
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

LQTopLepHists::LQTopLepHists(Context & ctx, const string & dirname): Hists(ctx, dirname){

  // jets
  book<TH1F>("N_jets", "N_{jets}", 21, -0.5, 20.5);
  book<TH1F>("pt_jet", "p_{T}^{jets} [GeV]", 50, 0, 1500);
  book<TH1F>("pt_jet1", "p_{T}^{jet 1} [GeV]", 50, 0, 1500);
  book<TH1F>("pt_jet2", "p_{T}^{jet 2} [GeV]", 50, 0, 1500);
  book<TH1F>("pt_jet3", "p_{T}^{jet 3} [GeV]", 50, 0, 1500);
  book<TH1F>("eta_jet", "#eta^{jets}", 50, -2.5, 2.5);
  book<TH1F>("eta_jet1", "#eta^{jet 1}", 50, -2.5, 2.5);
  book<TH1F>("eta_jet2", "#eta^{jet 2}", 50, -2.5, 2.5);
  book<TH1F>("eta_jet3", "#eta^{jet 3}", 50, -2.5, 2.5);
  book<TH1F>("phi_jet", "#phi^{jets}", 35, -3.5, 3.5);
  book<TH1F>("phi_jet1", "#phi^{jet 1}", 35, -3.5, 3.5);
  book<TH1F>("phi_jet2", "#phi^{jet 2}", 35, -3.5, 3.5);
  book<TH1F>("phi_jet3", "#phi^{jet 3}", 35, -3.5, 3.5);
  book<TH1F>("m_jet", "m^{jets}", 50, 0, 500);
  book<TH1F>("m_jet1", "m^{jet 1}", 50, 0, 500);
  book<TH1F>("m_jet2", "m^{jet 2}", 50, 0, 500);
  book<TH1F>("m_jet3", "m^{jet 3}", 50, 0, 500);
  book<TH1F>("csv_jet", "CSV^{jets}", 20, 0, 1);
  book<TH1F>("csv_jet1", "CSV^{jet 1}", 20, 0, 1);
  book<TH1F>("csv_jet2", "CSV^{jet 2}", 20, 0, 1);
  book<TH1F>("csv_jet3", "CSV^{jet 3}", 20, 0, 1);
  book<TH1F>("N_bJets_loose", "N_{jets}^{CSV loose}", 11, -0.5, 10.5);
  book<TH1F>("N_bJets_med", "N_{jets}^{CSV medium}", 11, -0.5, 10.5);
  book<TH1F>("N_bJets_tight", "N_{jets}^{CSV tight}", 11, -0.5, 10.5);
  book<TH1F>("N_deepjet_loose", "N_{jets}^{DeepJet loose}", 11, -0.5, 10.5);
  book<TH1F>("N_deepjet_med", "N_{jets}^{DeepJet medium}", 11, -0.5, 10.5);
  book<TH1F>("N_deepjet_tight", "N_{jets}^{DeepJet tight}", 11, -0.5, 10.5);


  book<TH1F>("N_mu", "N^{#mu}", 11, -0.5, 10.5);
  book<TH1F>("pt_mu", "p_{T}^{#mu} [GeV]", 50, 0, 1500);
  book<TH1F>("pt_mu1", "p_{T}^{#mu 1} [GeV]", 50, 0, 1500);
  book<TH1F>("pt_mu2", "p_{T}^{#mu 2} [GeV]", 50, 0, 1500);
  book<TH1F>("eta_mu", "#eta^{#mu}", 50, -2.5, 2.5);
  book<TH1F>("eta_mu1", "#eta^{#mu 1}", 50, -2.5, 2.5);
  book<TH1F>("eta_mu2", "#eta^{#mu 2}", 50, -2.5, 2.5);
  book<TH1F>("phi_mu", "#phi^{#mu}", 35, -3.5, 3.5);
  book<TH1F>("phi_mu1", "#phi^{#mu 1}", 35, 3.5, 3.5);
  book<TH1F>("phi_mu2", "#phi^{#mu 2}", 35, 3.5, 3.5);
  book<TH1F>("reliso_mu", "#mu rel. Iso", 40, 0, 0.5);
  book<TH1F>("reliso_mu1", "#mu 1 rel. Iso", 40, 0, 0.5);
  book<TH1F>("reliso_mu2", "#mu 2 rel. Iso", 40, 0, 0.5);
  book<TH1F>("reliso_mu_rebin", "#mu rel. Iso ", 400, 0, 5);
  book<TH1F>("reliso_mu1_rebin", "#mu 1 rel. Iso ", 400, 0, 5);
  book<TH1F>("reliso_mu2_rebin", "#mu 2 rel. Iso ", 400, 0, 5);
  book<TH1F>("N_ele", "N^{e}", 11, -0.5, 10.5);
  book<TH1F>("pt_ele", "p_{T}^{e} [GeV]", 50, 0, 1500);
  book<TH1F>("pt_ele1", "p_{T}^{e 1} [GeV]", 50, 0, 1500);
  book<TH1F>("pt_ele2", "p_{T}^{e 2} [GeV]", 50, 0, 1500);
  book<TH1F>("eta_ele", "#eta^{e}", 50, -2.5, 2.5);
  book<TH1F>("eta_ele1", "#eta^{ele 1}", 50, -2.5, 2.5);
  book<TH1F>("eta_ele2", "#eta^{ele 2}", 50, -2.5, 2.5);
  book<TH1F>("phi_ele", "#phi^{e}", 35, -3.5, 3.5);
  book<TH1F>("phi_ele1", "#phi^{e 1}", 35, -3.5, 3.5);
  book<TH1F>("phi_ele2", "#phi^{e 2}", 35, -3.5, 3.5);
  book<TH1F>("reliso_ele", "e rel. Iso", 40, 0, 0.5);
  book<TH1F>("reliso_ele1", "e 1 rel. Iso", 40, 0, 0.5);
  book<TH1F>("reliso_ele2", "e 2 rel. Iso", 40, 0, 0.5);
  book<TH1F>("reliso_ele_rebin", "e rel. Iso", 400, 0, 5);
  book<TH1F>("reliso_ele1_rebin", "e 1 rel. Iso", 400, 0, 5);
  book<TH1F>("reliso_ele2_rebin", "e 2 rel. Iso", 400, 0, 5);
  book<TH1F>("M_mumu", "M_{#mu#mu} [GeV]",75 , 0, 500);
  book<TH1F>("M_ee", "M_{ee} [GeV]",75 , 0, 500);


  book<TH1F>("NPV", "number of primary vertices", 91, -0.50, 90.5);
  book<TH1F>("MET", "missing E_{T} [GeV]", 50, 0, 7000);
  book<TH1F>("MET_rebin", "missing E_{T} [GeV]", 50, 0, 1500);
  book<TH1F>("MET_rebin2", "missing E_{T} [GeV]", 30, 0, 1500);
  book<TH1F>("MET_rebin3", "missing E_{T} [GeV]", 15, 0, 1500);
  book<TH1F>("ST", "S_{T} [GeV]", 50, 0, 7000);
  book<TH1F>("ST_rebin", "S_{T} [GeV]", 200, 0, 5000);
  book<TH1F>("ST_rebin2", "S_{T} [GeV]", 100, 0, 5000);
  book<TH1F>("ST_rebin3", "S_{T} [GeV]", 50, 0, 5000);
  book<TH1F>("STjets", "S_{T}^{jets} [GeV]", 50, 0, 7000);
  book<TH1F>("STjets_rebin", "S_{T}^{jets} [GeV]", 200, 0, 5000);
  book<TH1F>("STjets_rebin2", "S_{T}^{jets} [GeV]", 100, 0, 5000);
  book<TH1F>("STjets_rebin3", "S_{T}^{jets} [GeV]", 50, 0, 5000);
  book<TH1F>("STlep", "S_{T}^{lep} [GeV]", 50, 0, 7000);
  book<TH1F>("STlep_rebin", "S_{T}^{lep} [GeV]", 50, 0, 1500);
  book<TH1F>("STlep_rebin2", "S_{T}^{lep} [GeV]", 30, 0, 1500);
  book<TH1F>("STlep_rebin3", "S_{T}^{lep} [GeV]", 15, 0, 1500);

  book<TH1F>("sum_event_weights", "BinContent = sum(eventweights)", 1, 0.5, 1.5);

  is_mc = ctx.get("dataset_type") == "MC";

}


void LQTopLepHists::fill(const Event & event){

  double weight = event.weight;
  hist("sum_event_weights")->Fill(1., weight);


  /*
  █      ██ ███████ ████████ ███████
  █      ██ ██         ██    ██
  █      ██ █████      ██    ███████
  █ ██   ██ ██         ██         ██
  █  █████  ███████    ██    ███████
  */


  vector<Jet>* jets = event.jets;
  int Njets = jets->size();
  hist("N_jets")->Fill(Njets, weight);

  for(unsigned int i=0; i<jets->size(); i++){
    hist("pt_jet")->Fill(jets->at(i).pt(),weight);
    hist("eta_jet")->Fill(jets->at(i).eta(),weight);
    hist("phi_jet")->Fill(jets->at(i).phi(),weight);
    hist("m_jet")->Fill(jets->at(i).v4().M(),weight);
    hist("csv_jet")->Fill(jets->at(i).btag_combinedSecondaryVertex(), weight);
    if(i==0){
      hist("pt_jet1")->Fill(jets->at(i).pt(),weight);
      hist("eta_jet1")->Fill(jets->at(i).eta(),weight);
      hist("phi_jet1")->Fill(jets->at(i).phi(),weight);
      hist("m_jet1")->Fill(jets->at(i).v4().M(),weight);
      hist("csv_jet1")->Fill(jets->at(i).btag_combinedSecondaryVertex(), weight);
    }
    else if(i==1){
      hist("pt_jet2")->Fill(jets->at(i).pt(),weight);
      hist("eta_jet2")->Fill(jets->at(i).eta(),weight);
      hist("phi_jet2")->Fill(jets->at(i).phi(),weight);
      hist("m_jet2")->Fill(jets->at(i).v4().M(),weight);
      hist("csv_jet2")->Fill(jets->at(i).btag_combinedSecondaryVertex(), weight);
    }
    else if(i==2){
      hist("pt_jet3")->Fill(jets->at(i).pt(),weight);
      hist("eta_jet3")->Fill(jets->at(i).eta(),weight);
      hist("phi_jet3")->Fill(jets->at(i).phi(),weight);
      hist("m_jet3")->Fill(jets->at(i).v4().M(),weight);
      hist("csv_jet3")->Fill(jets->at(i).btag_combinedSecondaryVertex(), weight);
    }
  }

  int Nbjets_loose = 0, Nbjets_medium = 0, Nbjets_tight = 0;
  int Ndeepjet_loose = 0, Ndeepjet_med = 0, Ndeepjet_tight = 0;
  CSVBTag Btag_loose = CSVBTag(CSVBTag::WP_LOOSE);
  CSVBTag Btag_medium = CSVBTag(CSVBTag::WP_MEDIUM);
  CSVBTag Btag_tight = CSVBTag(CSVBTag::WP_TIGHT);

  JetId DeepjetLoose = BTag(BTag::DEEPJET, BTag::WP_LOOSE);
  JetId DeepjetMedium = BTag(BTag::DEEPJET, BTag::WP_MEDIUM);
  JetId DeepjetTight = BTag(BTag::DEEPJET, BTag::WP_TIGHT);

  for (unsigned int i =0; i<jets->size(); i++) {
    if(Btag_loose(jets->at(i),event))  Nbjets_loose++;
    if(Btag_medium(jets->at(i),event)) Nbjets_medium++;
    if(Btag_tight(jets->at(i),event))  Nbjets_tight++;
    if(DeepjetLoose(jets->at(i),event))  Ndeepjet_loose++;
    if(DeepjetMedium(jets->at(i),event)) Ndeepjet_med++;
    if(DeepjetTight(jets->at(i),event))  Ndeepjet_tight++;
  }

  hist("N_bJets_loose")->Fill(Nbjets_loose,weight);
  hist("N_bJets_med")->Fill(Nbjets_medium,weight);
  hist("N_bJets_tight")->Fill(Nbjets_tight,weight);
  hist("N_deepjet_loose")->Fill(Ndeepjet_loose,weight);
  hist("N_deepjet_med")->Fill(Ndeepjet_med,weight);
  hist("N_deepjet_tight")->Fill(Ndeepjet_tight,weight);


  /*
  ███    ███ ██    ██  ██████  ███    ██ ███████
  ████  ████ ██    ██ ██    ██ ████   ██ ██
  ██ ████ ██ ██    ██ ██    ██ ██ ██  ██ ███████
  ██  ██  ██ ██    ██ ██    ██ ██  ██ ██      ██
  ██      ██  ██████   ██████  ██   ████ ███████
  */


  vector<Muon>* muons = event.muons;
  int Nmuons = muons->size();
  hist("N_mu")->Fill(Nmuons, weight);

  for(int i=0; i<Nmuons; i++){

    hist("pt_mu")->Fill(muons->at(i).pt(),weight);
    hist("eta_mu")->Fill(muons->at(i).eta(),weight);
    hist("phi_mu")->Fill(muons->at(i).phi(),weight);
    hist("reliso_mu")->Fill(muons->at(i).relIso(),weight);
    hist("reliso_mu_rebin")->Fill(muons->at(i).relIso(),weight);
    if(i==0){
      hist("pt_mu1")->Fill(muons->at(i).pt(),weight);
      hist("eta_mu1")->Fill(muons->at(i).eta(),weight);
      hist("phi_mu1")->Fill(muons->at(i).phi(),weight);
      hist("reliso_mu1")->Fill(muons->at(i).relIso(),weight);
      hist("reliso_mu1_rebin")->Fill(muons->at(i).relIso(),weight);
    }
    else if(i==1){
      hist("pt_mu2")->Fill(muons->at(i).pt(),weight);
      hist("eta_mu2")->Fill(muons->at(i).eta(),weight);
      hist("phi_mu2")->Fill(muons->at(i).phi(),weight);
      hist("reliso_mu2")->Fill(muons->at(i).relIso(),weight);
      hist("reliso_mu2_rebin")->Fill(muons->at(i).relIso(),weight);
    }
  }

  for(int i=0; i<Nmuons; i++){
    for(int j=0; j<Nmuons; j++){
      if(j <= i) continue;
      hist("M_mumu")->Fill((muons->at(i).v4() + muons->at(j).v4()).M() ,weight);
    }
  }

  /*
  ███████ ██      ███████  ██████ ████████ ██████   ██████  ███    ██ ███████
  ██      ██      ██      ██         ██    ██   ██ ██    ██ ████   ██ ██
  █████   ██      █████   ██         ██    ██████  ██    ██ ██ ██  ██ ███████
  ██      ██      ██      ██         ██    ██   ██ ██    ██ ██  ██ ██      ██
  ███████ ███████ ███████  ██████    ██    ██   ██  ██████  ██   ████ ███████
  */


  vector<Electron>* electrons = event.electrons;
  int Nelectrons = electrons->size();
  hist("N_ele")->Fill(Nelectrons, weight);

  for(int i=0; i<Nelectrons; i++){
    hist("pt_ele")->Fill(electrons->at(i).pt(),weight);
    hist("eta_ele")->Fill(electrons->at(i).eta(),weight);
    hist("phi_ele")->Fill(electrons->at(i).phi(),weight);
    hist("reliso_ele")->Fill(electrons->at(i).relIso(),weight);
    hist("reliso_ele_rebin")->Fill(electrons->at(i).relIso(),weight);
    if(i==0){
      hist("pt_ele1")->Fill(electrons->at(i).pt(),weight);
      hist("eta_ele1")->Fill(electrons->at(i).eta(),weight);
      hist("phi_ele1")->Fill(electrons->at(i).phi(),weight);
      hist("reliso_ele1")->Fill(electrons->at(i).relIso(),weight);
      hist("reliso_ele1_rebin")->Fill(electrons->at(i).relIso(),weight);
    }
    else if(i==1){
      hist("pt_ele2")->Fill(electrons->at(i).pt(),weight);
      hist("eta_ele2")->Fill(electrons->at(i).eta(),weight);
      hist("phi_ele2")->Fill(electrons->at(i).phi(),weight);
      hist("reliso_ele2")->Fill(electrons->at(i).relIso(),weight);
      hist("reliso_ele2_rebin")->Fill(electrons->at(i).relIso(),weight);
    }
  }

  for(int i=0; i<Nelectrons; i++){
    for(int j=0; j<Nelectrons; j++){
      if(j <= i) continue;
      hist("M_ee")->Fill((electrons->at(i).v4() + electrons->at(j).v4()).M() ,weight);
    }
  }



  /*
  ██████  ███████ ███    ██ ███████ ██████   █████  ██
  ██      ██      ████   ██ ██      ██   ██ ██   ██ ██
  ██  ███ █████   ██ ██  ██ █████   ██████  ███████ ██
  ██   ██ ██      ██  ██ ██ ██      ██   ██ ██   ██ ██
  ██████  ███████ ██   ████ ███████ ██   ██ ██   ██ ███████
  */



  int Npvs = event.pvs->size();
  hist("NPV")->Fill(Npvs, weight);

  double met = event.met->pt();
  double st = 0., st_jets = 0., st_lep = 0.;
  for(const auto & jet : *jets)           st_jets += jet.pt();
  for(const auto & electron : *electrons) st_lep += electron.pt();
  for(const auto & muon : *muons)         st_lep += muon.pt();
  st = st_jets + st_lep + met;

  hist("MET")->Fill(met, weight);
  hist("MET_rebin")->Fill(met, weight);
  hist("MET_rebin2")->Fill(met, weight);
  hist("MET_rebin3")->Fill(met, weight);
  hist("ST")->Fill(st, weight);
  hist("ST_rebin")->Fill(st, weight);
  hist("ST_rebin2")->Fill(st, weight);
  hist("ST_rebin3")->Fill(st, weight);
  hist("STjets")->Fill(st_jets, weight);
  hist("STjets_rebin")->Fill(st_jets, weight);
  hist("STjets_rebin2")->Fill(st_jets, weight);
  hist("STjets_rebin3")->Fill(st_jets, weight);
  hist("STlep")->Fill(st_lep, weight);
  hist("STlep_rebin")->Fill(st_lep, weight);
  hist("STlep_rebin2")->Fill(st_lep, weight);
  hist("STlep_rebin3")->Fill(st_lep, weight);


} //Methode



LQTopLepHists::~LQTopLepHists(){}
