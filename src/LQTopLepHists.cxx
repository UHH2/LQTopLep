#include "UHH2/LQTopLep/include/LQTopLepHists.h"
#include "UHH2/core/include/Event.h"

#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

LQTopLepHists::LQTopLepHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here
  // jets
  book<TH1F>("N_jets", "N_{jets}", 20, 0, 20);  
  book<TH1F>("pt_jets", "p_{T}^{jets} [GeV]", 50, 0, 1600); // pt of all jets in one histogram
  book<TH1F>("eta_jets", "#eta jets", 40, -2.5, 2.5);

  book<TH1F>("N_PU", "N_{PU}", 100, 0, 100);  
  book<TH1F>("eta_jet1", "#eta^{jet 1}", 40, -2.5, 2.5);
  book<TH1D>("pt_jet1", "#p_{T}^{jet 1}", 100, 0, 3000);
  book<TH1D>("pt_jet2", "#p_{T}^{jet 2}", 100, 0, 3000);
  book<TH1D>("pt_jet3", "#p_{T}^{jet 3}", 100, 0, 3000);
  book<TH1F>("eta_jet2", "#eta^{jet 2}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet3", "#eta^{jet 3}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet4", "#eta^{jet 4}", 40, -2.5, 2.5);



  // muons
  book<TH1F>("N_mu", "N^{#mu}", 10, 0, 10);
  book<TH1F>("pt_mu", "p_{T}^{#mu} [GeV/c]", 40, 0, 200);
  book<TH1F>("eta_mu", "#eta^{#mu}", 40, -2.1, 2.1);
  book<TH1F>("reliso_mu", "#mu rel. Iso", 40, 0, 0.5);

  // electrons
  
  book<TH1F>("N_e", "number of electrons", 9, -0.5, 8.5);
  book<TH1F>("pt_e", "p_{T}^{e} [GeV]", 50, 0, 1600);
  book<TH1F>("eta_e", "#eta electrons", 40, -2.1, 2.1);
  book<TH1F>("reliso_e", "e rel. Iso", 40, 0, 0.5);


  // primary vertices
  book<TH1F>("N_pv", "N^{PV}", 50, 0, 50);

  // general
  book<TH1D>("S_T", "S_{T}", 100, 0, 5000);
  book<TH1D>("sum_event_weights", "BinContent = sum(eventweights)", 1, 0.5, 1.5);
}


void LQTopLepHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.
  double weight = event.weight;
  
  std::vector<Jet>* jets = event.jets;
  int Njets = jets->size();
  hist("N_jets")->Fill(Njets, weight);
  if(!event.isRealData)  hist("N_PU")->Fill(event.genInfo->pileup_TrueNumInteractions(), weight);

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

  int Nmuons = event.muons->size();
  hist("N_mu")->Fill(Nmuons, weight);
  for (const Muon & thismu : *event.muons){
      hist("pt_mu")->Fill(thismu.pt(), weight);
      hist("eta_mu")->Fill(thismu.eta(), weight);
      hist("reliso_mu")->Fill(thismu.relIso(), weight);
  } 
  // Electrons  
  //std::vector<Electron>* electronVector = event.electrons;

  const int Nele = event.electrons->size();
  hist("N_e")->Fill(Nele, weight);
  for (const Electron & thise : *event.electrons) {
    hist("pt_e")->Fill(thise.pt(), weight);
    hist("eta_e")->Fill(thise.eta(), weight);
    hist("reliso_e")->Fill(thise.relIso(), weight);
  }
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
    st +=jet.pt();
    }*/
  for(const auto & electron : *event.electrons){
    st_lep += electron.pt();
  }
  for(const auto & muon : *event.muons){
    st_lep += muon.pt();
  }

  st = st_lep + st_jets + met;
  hist("S_T")->Fill(st, weight);

  int Npvs = event.pvs->size();
  hist("N_pv")->Fill(Npvs, weight);

  //sum_event_weights
  hist("sum_event_weights")->Fill(1., weight);


}

LQTopLepHists::~LQTopLepHists(){}
