#include "UHH2/LQTopLep/include/LQTopLepSelections.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;
using namespace std;


DijetSelection::DijetSelection(float dphi_min_, float third_frac_max_): dphi_min(dphi_min_), third_frac_max(third_frac_max_){}

bool DijetSelection::passes(const Event & event){
  assert(event.jets); // if this fails, it probably means jets are not read in
  if(event.jets->size() < 2) return false;
  const auto & jet0 = event.jets->at(0);
  const auto & jet1 = event.jets->at(1);
  auto dphi = deltaPhi(jet0, jet1);
  if(dphi < dphi_min) return false;
  if(event.jets->size() == 2) return true;
  const auto & jet2 = event.jets->at(2);
  auto third_jet_frac = jet2.pt() / (0.5 * (jet0.pt() + jet1.pt()));
  return third_jet_frac < third_frac_max;
}


HtSelection::HtSelection(double ht_min_, double ht_max_):ht_min(ht_min_), ht_max(ht_max_) {}
bool HtSelection::passes(const Event & event) {
  auto met = event.met->pt();

  bool pass = false;
  double ht = 0.0;
  double ht_jets = 0.0;
  double ht_lep = 0.0;

  for(const auto & jet : *event.jets){
    ht_jets += jet.pt();
  }
  for(const auto & electron : *event.electrons){
    ht_lep += electron.pt();
  }
  for(const auto & muon : *event.muons){
    ht_lep += muon.pt();
  }

  ht = ht_lep + ht_jets + met;

  pass = ht > ht_min && (ht_max < 0 || ht < ht_max);

  return pass;
}


HtLepSelection::HtLepSelection(double ht_min_, double ht_max_):ht_min(ht_min_), ht_max(ht_max_) {}
bool HtLepSelection::passes(const Event & event) {

  //auto met = event.met->pt();

  bool pass = false;
  //double ht = 0.0;
  double ht_lep = 0.0;

  for(const auto & electron : *event.electrons){
    ht_lep += electron.pt();
  }
  for(const auto & muon : *event.muons){
    ht_lep += muon.pt();
  }


  pass = ht_lep > ht_min && (ht_max < 0 || ht_lep < ht_max);

  return pass;

}

/*
// returns false if M_ee in [m_min, m_max]
InvMassEleEleVeto::InvMassEleEleVeto(double m_min_, double m_max_):m_min(m_min_), m_max(m_max_){}
bool InvMassEleEleVeto::passes(const Event & event){
bool pass = true;
int Nelectrons = event.electrons->size();
double M_ee;
for(int i=0; i<Nelectrons; i++) {
const auto & ele0 = event.electrons->at(i);
for(int j=i; j<Nelectrons; j++) {
const auto & ele1 = event.electrons->at(j);
const auto& diele = ele0.v4()+ele1.v4();
M_ee = diele.M(); // calculate M_ee for every pair of electrons
if(M_ee > m_min && M_ee < m_max) {
pass = false;
return pass;
}
}
}
return pass;
}
*/

// returns false if there is a M_ee in [m_min, m_max]
InvMassEleEleVeto::InvMassEleEleVeto(double m_min_, double m_max_):m_min(m_min_), m_max(m_max_){}
bool InvMassEleEleVeto::passes(const Event & event) {
  bool pass = true;
  int Nelectrons = event.electrons->size();
  LorentzVector electrons[Nelectrons];
  double M_ee;
  for(int i=0; i<Nelectrons; i++) {
    electrons[i] = event.electrons->at(i).v4();
  }
  for(int i=0; i<Nelectrons; i++) {
    for(int j=i; j<Nelectrons; j++) {
      const auto& diele = electrons[i]+electrons[j];
      M_ee = diele.M(); // calculate M_ee for every pair of electrons
      if(M_ee > m_min && M_ee < m_max) {
        pass = false;
        return pass;
      }
    }
  }
  return pass;
}

InvMass2MuVeto::InvMass2MuVeto(double m_min_, double m_max_):m_min(m_min_), m_max(m_max_){}
bool InvMass2MuVeto::passes(const Event & event){

  bool pass = true;
  int Nmuons = event.muons->size();
  double M_mumu;
  LorentzVector muons[Nmuons];
  for(int i=0; i<Nmuons; i++){
    muons[i] = event.muons->at(i).v4();
  }
  for(int i=0; i<Nmuons; i++){
    for(int j=0; j<Nmuons; j++){
      if(j > i){
        M_mumu = (muons[i] + muons[j]).M();
        if(M_mumu > m_min && M_mumu < m_max){
          pass = false;
        }
      }
    }
  }
  return pass;
}


// returns true if there is a M_mumu in [m_min, m_max]
InvMass2MuSelection::InvMass2MuSelection(double m_min_, double m_max_):m_min(m_min_), m_max(m_max_){}
bool InvMass2MuSelection::passes(const Event & event) {
  bool pass = false;
  int Nmuons = event.muons->size();
  LorentzVector muons[Nmuons];
  double M_mumu;
  for(int i=0; i<Nmuons; i++) {
    muons[i] = event.muons->at(i).v4();
  }
  for(int i=0; i<Nmuons; i++) {
    for(int j=0; j<Nmuons; j++) {
      if(j <= i) continue;
      const auto& dimu = muons[i]+muons[j];
      M_mumu = dimu.M(); // calculate M_mumu for every pair of muons
      if(M_mumu > m_min && (M_mumu < m_max || m_max < 0.)) {
        pass = true;
        return pass;
      }
    }
  }
  return pass;
}


InvMassEleMuVeto::InvMassEleMuVeto(double m_min_, double m_max_):m_min(m_min_), m_max(m_max_){}
bool InvMassEleMuVeto::passes(const Event & event) {
  bool pass = true;
  int Nelectrons = event.electrons->size();
  int Nmuons = event.muons->size();
  LorentzVector electrons[Nelectrons];
  LorentzVector muons[Nmuons];
  double M_emu;
  for(int i=0; i<Nelectrons; i++) {
    electrons[i] = event.electrons->at(i).v4();
  }
  for(int i=0; i<Nmuons; i++) {
    muons[i] = event.muons->at(i).v4();
  }
  for(int i=0; i<Nelectrons; i++) {
    for(int j=i; j<Nmuons; j++) {
      const auto& diele = electrons[i]+muons[j];
      M_emu = diele.M(); // calculate M_emu for every pair of leptons
      if(M_emu > m_min && M_emu < m_max) {
        pass = false;
        return pass;
      }
    }
  }
  return pass;
}


// returns true if there is a M_ee in [m_min, m_max]
InvMassEleEleSelection::InvMassEleEleSelection(double m_min_, double m_max_):m_min(m_min_), m_max(m_max_){}
bool InvMassEleEleSelection::passes(const Event & event) {
  bool pass = false;
  int Nelectrons = event.electrons->size();
  LorentzVector electrons[Nelectrons];
  double M_ee;
  for(int i=0; i<Nelectrons; i++) {
    electrons[i] = event.electrons->at(i).v4();
  }
  for(int i=0; i<Nelectrons; i++) {
    for(int j=i; j<Nelectrons; j++) {
      const auto& diele = electrons[i]+electrons[j];
      M_ee = diele.M(); // calculate M_ee for every pair of electrons
      if(M_ee > m_min && M_ee < m_max) {
        pass = true;
        return pass;
      }
    }
  }
  return pass;
}
