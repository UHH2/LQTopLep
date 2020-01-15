#include "UHH2/LQTopLep/include/LQMassReconstruction.h"
#include "UHH2/core/include/LorentzVector.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/Utils.h"
#include <math.h>

#include <cassert>

using namespace uhh2;
using namespace std;

bool muonPairWithOppositeCharge(Event & event, size_t max_n = 99){
  bool charge_opposite = false;
  for(unsigned int i=0; i<min(event.muons->size(), max_n); i++){
    for(unsigned int j=0; j<min(event.muons->size(), max_n); j++){
      if(j>i){
        if(event.muons->at(i).charge() != event.muons->at(j).charge()) {
          charge_opposite = true;
        }
      }
    }
  }
  return charge_opposite;
}

bool electronPairWithOppositeCharge(Event & event, size_t max_n = 99){
  bool charge_opposite = false;
  for(unsigned int i=0; i<min(event.electrons->size(), max_n); i++){
    for(unsigned int j=0; j<min(event.electrons->size(), max_n); j++){
      if(j>i){
        if(event.electrons->at(i).charge() != event.electrons->at(j).charge()) {
          charge_opposite = true;
        }
      }
    }
  }
  return charge_opposite;
}


HighMassInclusiveLQReconstruction::HighMassInclusiveLQReconstruction(Context & ctx, const NeutrinoReconstructionMethod & neutrinofunction): m_neutrinofunction(neutrinofunction) {
  h_recohyps = ctx.get_handle<vector<LQReconstructionHypothesis>>("LQHypotheses");
  h_is_mlq_reconstructed = ctx.get_handle<bool>("is_mlq_reconstructed");
  h_mlq_reco_mode_lq  = ctx.get_handle<TString>("mlq_reco_mode_lq");
  h_mlq_reco_mode_top = ctx.get_handle<TString>("mlq_reco_mode_top");
}

HighMassInclusiveLQReconstruction::~HighMassInclusiveLQReconstruction() {}


bool HighMassInclusiveLQReconstruction::process(uhh2::Event & event) {
  assert(event.jets);
  assert(event.met);


  // Check pre-requisites and return false, if they are not met
  // ==========================================================

  std::vector<LQReconstructionHypothesis> recoHyps = {};
  TString mode_lq = "none";
  TString mode_top = "none";
  event.set(h_recohyps, recoHyps);
  event.set(h_mlq_reco_mode_lq, mode_lq);
  event.set(h_mlq_reco_mode_top, mode_top);
  event.set(h_is_mlq_reconstructed, false);

  // At least 2 muons or 2 electrons present
  if(event.muons->size() < 2 && event.electrons->size() < 2) return false;


  // Decide for electron or muon mode (for LQ decay):
  if(event.muons->size() >= 2 && event.electrons->size() < 2)      mode_lq = "muon";
  else if(event.electrons->size() >= 2 && event.muons->size() < 2) mode_lq = "ele";
  else{
    if(event.muons->at(0).pt() > event.electrons->at(0).pt()) mode_lq = "muon";
    else                                                      mode_lq = "ele";
  }
  if(!(mode_lq == "muon" || mode_lq == "ele")) return false;
  event.set(h_mlq_reco_mode_lq, mode_lq);


  // Decide for electron or muon mode (for top decay)
  // 1) >=2 muons, >=1 electron -->  Use hardest electron and any 2 out of all muons
  // 2) >=3 muons, ==0 electrons --> Use 3 hardest muons
  // 1) >=2 electrons, >=1 muon -->  Use hardest muon and any 2 out of all electrons
  // 2) >=3 electrons, ==0 muons --> Use 3 hardest electrons
  bool is_ele_case = false;
  bool is_muon_case = false;
  bool charge_opposite_muon = muonPairWithOppositeCharge(event);
  bool charge_opposite_muon_first3 = muonPairWithOppositeCharge(event, 3);
  bool charge_opposite_electron = electronPairWithOppositeCharge(event);
  bool charge_opposite_electron_first3 = electronPairWithOppositeCharge(event, 3);

  if(mode_lq == "muon"){
    if(event.muons->size() >= 2 && event.electrons->size() >= 1 && charge_opposite_muon) is_ele_case = true;
    else if(event.electrons->size() == 0 && event.muons->size() >= 3 && charge_opposite_muon_first3) is_muon_case = true;
  }
  else{
    if(event.electrons->size() >= 2 && event.muons->size() >= 1 && charge_opposite_electron) is_muon_case = true;
    else if(event.muons->size() == 0 && event.electrons->size() >= 3 && charge_opposite_electron_first3) is_ele_case = true;
  }
  if(!(is_ele_case || is_muon_case)) return false;

  if(is_ele_case) mode_top = "ele";
  else            mode_top = "muon";
  event.set(h_mlq_reco_mode_top, mode_top);


  // Start the reconstruction
  // ========================


  vector<Particle> LQ_lepton_candidates;
  vector<Particle> W_lepton_candidates;
  if(mode_lq == "muon"){
    if(is_ele_case){
      for(size_t i = 0; i < event.muons->size(); i++){
        LQ_lepton_candidates.emplace_back(event.muons->at(i));
      }
      W_lepton_candidates.emplace_back(event.electrons->at(0));
    }
    else{
      for(size_t i = 0; i < event.muons->size(); i++){
        if(i < 3){
          LQ_lepton_candidates.emplace_back(event.muons->at(i));
          W_lepton_candidates.emplace_back(event.muons->at(i));
        }
      }
    }
  }
  else{
    if(is_muon_case){
      for(size_t i = 0; i < event.electrons->size(); i++){
        LQ_lepton_candidates.emplace_back(event.electrons->at(i));
      }
      W_lepton_candidates.emplace_back(event.muons->at(0));
    }
    else{
      for(size_t i = 0; i < event.electrons->size(); i++){
        if(i < 3){
          LQ_lepton_candidates.emplace_back(event.electrons->at(i));
          W_lepton_candidates.emplace_back(event.electrons->at(i));
        }
      }
    }

  }

  // Loop over lepton-top assignments
  for(size_t m=0; m<W_lepton_candidates.size(); m++){


    const Particle & W_lepton = W_lepton_candidates.at(m);
    LorentzVector W_lepton_v4 = W_lepton.v4();

    //reconstruct neutrino
    std::vector<LorentzVector> neutrinos = m_neutrinofunction(W_lepton_v4, event.met->v4());

    unsigned int n_LQ_leptons = LQ_lepton_candidates.size();
    unsigned int n_jets = event.jets->size();
    if(n_jets>7) n_jets=7;
    const size_t max_j = pow(3, n_jets);

    //loop over neutrino solutions and jet assignments to fill hyotheses
    for(const auto & neutrino_p4 : neutrinos) {
      const LorentzVector wlep_v4 = W_lepton_v4 + neutrino_p4;
      for (unsigned int j=0; j < max_j; j++) {
        LorentzVector tophad_v4;
        LorentzVector toplep_v4 = wlep_v4;
        int hadjets=0;
        int lepjets=0;
        int num = j;
        LQReconstructionHypothesis hyp;
        vector<Particle> had_jets, lep_jets;
        for (unsigned int k=0; k<n_jets; k++) {
          if(num%3==0) {
            tophad_v4 = tophad_v4 + event.jets->at(k).v4();
            had_jets.push_back(event.jets->at(k));
            hadjets++;
          }

          if(num%3==1) {
            toplep_v4 = toplep_v4 + event.jets->at(k).v4();
            lep_jets.push_back(event.jets->at(k));
            lepjets++;
          }

          num /= 3;
        }

        hyp.set_tophad_jets(had_jets);
        hyp.set_toplep_jets(lep_jets);

        //search jet with highest pt assigned to leptonic top
        int blep_idx(-1);
        float maxpt(-1.);
        for(unsigned int n=0; n<hyp.toplep_jets().size(); ++n){
          if(maxpt< hyp.toplep_jets().at(n).pt()){
            maxpt = hyp.toplep_jets().at(n).pt();
            blep_idx = n;
          }
        }
        if(blep_idx != -1) hyp.set_blep_v4(hyp.toplep_jets().at(blep_idx).v4());

        //fill only hypotheses with at least one jet assigned to each top quark
        if(hadjets>0 && lepjets>0) {
          size_t max_i = pow(3,n_LQ_leptons); // analogous to jet combinations
          for(size_t i=0; i<max_i; i++){ // for each jet comb loop over all possible muon combs
            LorentzVector lepton_LQ1_v4;
            LorentzVector lepton_LQ2_v4;
            int hadlepton=0;
            int leplepton=0;
            int num = i;
            for(size_t k=0; k<n_LQ_leptons; k++){
              if(mode_lq == "muon" && is_muon_case && (k == m)) continue; // in this case, all leptons are muons
              else if(mode_lq == "electron" && is_ele_case && (k == m)) continue; // in this case, all leptons are electrons

              if(num%3==0){
                lepton_LQ1_v4 = LQ_lepton_candidates.at(k).v4();
                hyp.set_lepton_LQhad(LQ_lepton_candidates.at(k)); // had is only a hypothesis not having considered the charge!!!
                hadlepton++;
              }
              if(num%3==1){
                lepton_LQ2_v4 = LQ_lepton_candidates.at(k).v4();
                hyp.set_lepton_LQlep(LQ_lepton_candidates.at(k)); // lep is only a hypothesis not having considered the charge!!!
                leplepton++;
              }
              //for num%3==2 do nothing
              num /= 3;
            }

            Particle lepton_2 = hyp.lepton_LQlep();
            Particle lepton_1 = hyp.lepton_LQhad();
            if(hadlepton==1 && leplepton==1 && (lepton_1.charge()!=lepton_2.charge())){ //require exactly 1 muon assigned to each top, opposite charges

              if(lepton_2.charge() != W_lepton.charge()){ //W-lepton and leptonic lepton must have opposite charges
                hyp.set_lepton_LQhad_v4(lepton_LQ1_v4);
                hyp.set_lepton_LQlep_v4(lepton_LQ2_v4); // lepton2 really is the leptonic one.
                hyp.set_lepton_LQhad(hyp.lepton_LQhad());
                hyp.set_lepton_LQlep(hyp.lepton_LQlep());
                hyp.set_tophad_v4(tophad_v4);
                hyp.set_toplep_v4(toplep_v4);
                hyp.set_lepton_w(W_lepton);
                hyp.set_lepton_w_v4(W_lepton_v4);
                hyp.set_neutrino_v4(neutrino_p4);
                hyp.set_tophad_jets(had_jets);
                hyp.set_toplep_jets(lep_jets);
              } // charge comparison
              else{
                hyp.set_lepton_LQhad_v4(lepton_LQ2_v4);
                hyp.set_lepton_LQlep_v4(lepton_LQ1_v4); // mu1 really is the leptonic one, the original hypothesis was wrong.
                hyp.set_lepton_LQhad(hyp.lepton_LQlep());
                hyp.set_lepton_LQlep(hyp.lepton_LQhad());
                hyp.set_tophad_v4(tophad_v4);
                hyp.set_toplep_v4(toplep_v4);
                hyp.set_lepton_w(W_lepton);
                hyp.set_lepton_w_v4(W_lepton_v4);
                hyp.set_neutrino_v4(neutrino_p4);
                hyp.set_tophad_jets(had_jets);
                hyp.set_toplep_jets(lep_jets);
              } // charge comparison 2

              recoHyps.emplace_back(move(hyp));

            } // 1 muon per top
          } // muon combs for-loop
        } // if at least 1 jet is assigned to each top quark
      } // 3^n_jets jet combinations * n_muon muon combinations
    } // neutrinos
  }

  // cout << "MLQ reconstructed! Number of hypotheses: " << recoHyps.size() << endl;
  event.set(h_recohyps, move(recoHyps));
  event.set(h_is_mlq_reconstructed, true);
  return true;
}



// selfmade, so maybe wrong
// passed testing, so I guess it's working as intended
std::vector<LorentzVector> LQNeutrinoReconstruction(const LorentzVector & lepton, const LorentzVector & met) {

  // TVector3 ptLep = toVector(lepton); // why is that px,py,pz and not x,y,z?
  // ptLep.SetZ(0);
  // TVector3 ptNeutr = toVector(met); // met = pT_neutr
  // ptNeutr.SetZ(0);

  float ptLepNeutr = lepton.px() * met.px() + lepton.py() * met.py();

  float ptLep = sqrt( pow(lepton.px(),2) + pow(lepton.py(),2) );
  float ptNeutr = sqrt( pow(met.px(),2) + pow(met.py(),2) );
  const float Wmass = 80.379;
  float mu = Wmass * Wmass / 2 + ptLepNeutr;

  float A = mu * lepton.pz() / pow(ptLep,2);
  float B = pow(mu,2) * pow(lepton.pz(),2) / pow(ptLep,4);
  float C = ( pow(lepton.e(),2) * pow(ptNeutr,2) - pow(mu,2) ) / pow(ptLep,2);

  vector<LorentzVector> solutions;

  if(B-C >= 0) {
    LorentzVectorXYZE solution1(met.px(), met.py(), A + sqrt(B-C), 0);
    solution1.SetE(solution1.P());
    solutions.emplace_back(toPtEtaPhi(solution1));
    LorentzVectorXYZE solution2(met.px(), met.py(), A - sqrt(B-C), 0);
    solution2.SetE(solution2.P());
    solutions.emplace_back(toPtEtaPhi(solution2));
  }
  else {
    // take only the real part of the solution for pz
    LorentzVectorXYZE solution(met.px(), met.py(), A, 0);
    solution.SetE(solution.P()); // p = E
    solutions.emplace_back(toPtEtaPhi(solution));
  }
  return solutions;
}



std::vector<LorentzVector> LQNeutrinoReconstructionOld(const LorentzVector & lepton, const LorentzVector & met) {
  TVector3 lepton_pT = toVector(lepton);
  lepton_pT.SetZ(0);
  TVector3 neutrino_pT = toVector(met);
  neutrino_pT.SetZ(0);
  constexpr float mass_w = 80.379f;
  float mu = mass_w * mass_w / 2 + lepton_pT * neutrino_pT;
  float A = - (lepton_pT * lepton_pT);
  float B = mu * lepton.pz();
  float C = mu * mu - lepton.e() * lepton.e() * (neutrino_pT * neutrino_pT);
  float discriminant = B * B - A * C;
  std::vector<LorentzVector> solutions;
  if (0 >= discriminant) {
    // Take only real part of the solution for pz:
    LorentzVectorXYZE solution (met.Px(),met.Py(),-B / A,0);
    solution.SetE(solution.P());
    solutions.emplace_back(toPtEtaPhi(solution));
  }
  else {
    discriminant = sqrt(discriminant);
    LorentzVectorXYZE solution (met.Px(),met.Py(),(-B - discriminant) / A,0);
    solution.SetE(solution.P());
    solutions.emplace_back(toPtEtaPhi(solution));

    LorentzVectorXYZE solution2 (met.Px(),met.Py(),(-B + discriminant) / A,0);
    solution2.SetE(solution2.P());
    solutions.emplace_back(toPtEtaPhi(solution2));
  }
  return solutions;
}
