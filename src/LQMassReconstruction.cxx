#include "UHH2/LQTopLep/include/LQMassReconstruction.h"
#include "UHH2/core/include/LorentzVector.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/Utils.h"
#include <math.h>

#include <cassert>

using namespace uhh2;
using namespace std;


HighMassInclusiveLQReconstruction::HighMassInclusiveLQReconstruction(Context & ctx, const NeutrinoReconstructionMethod & neutrinofunction): m_neutrinofunction(neutrinofunction) {
  h_recohyps = ctx.get_handle<vector<LQReconstructionHypothesis>>("LQHypotheses");
  h_is_mlq_reconstructed = ctx.get_handle<bool>("is_mlq_reconstructed");
  h_mlq_reco_mode = ctx.get_handle<TString>("mlq_reco_mode");
}

HighMassInclusiveLQReconstruction::~HighMassInclusiveLQReconstruction() {}


bool HighMassInclusiveLQReconstruction::process(uhh2::Event & event) {
  assert(event.jets);
  assert(event.met);
  int n_final_hyps = 0;


  // Requirements:
  // 1) "muon":  >=2 electrons, >=1 muon -->Take hardest muon
  // 2) "ele":   >=3 electrons, ==0 muons -->Take 3 hardest electrons
  TString mode = event.get(h_mlq_reco_mode);
  if(mode != "ele" && mode != "muon") return false;

  std::vector<LQReconstructionHypothesis> recoHyps; // this vector saves every hypothesis


  if(mode == "muon") { // muon case

    const Particle & muon = event.muons->at(0); // hardest lepton is automatically a muon
    LorentzVector muon_v4 = muon.v4();
    
    // reconstructed neutrino
    vector<LorentzVector> neutrinos = m_neutrinofunction( muon.v4(), event.met->v4());

    
    unsigned int n_electrons = event.electrons->size();
    unsigned int n_jets = event.jets->size();
    if (n_jets>7) n_jets = 7; // only consider first 7 jets
    
    const unsigned int max_j = pow(3, n_jets); // loop over max_j possibilities

    // loop over possible neutrino solutions
    for (const auto & neutrino_p4 : neutrinos) {
      /*
      cout << "test NeutrinoReconstruction 1: " << neutrino_p4.px()  << endl;
      cout << "test NeutrinoReconstruction 2: " << neutrino_p4.py()  << endl;
      cout << "test NeutrinoReconstruction 3: " << neutrino_p4.pz()  << endl;
      cout << "test NeutrinoReconstruction 4: " << neutrino_p4.pt()  << endl;
      cout << "test NeutrinoReconstruction 5: " << neutrino_p4.e()  << endl;
      */




      const LorentzVector wlep_v4 = muon.v4() + neutrino_p4; // mass of leptonic decaying W
      // loop over max_j
      for (unsigned int j=0; j<max_j; j++) {
	LorentzVector tophad_v4; // hadronic decaying top
	LorentzVector toplep_v4 = wlep_v4; // leptonic decaying top
	int hadjets = 0;
	int lepjets = 0;
	int num = j;

	LQReconstructionHypothesis hyp;
	vector<Particle> had_jets, lep_jets;
	
	// loop over n_jets
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
          //in case num%3==2 do not take this jet at all
          //shift the trigits of num to the right:
          num /= 3;
	} // loop over n_jets
	
	hyp.set_tophad_jets(had_jets);
	hyp.set_toplep_jets(lep_jets);

	// fill only hypotheses with at least one jet assigned to each top quark
	if(hadjets>0 && lepjets>0) {
	  int max_i = pow(3, n_electrons);
	  // loop over all electron combs for every jet comb
	  for (int i=0; i<max_i; i++) {
	    LorentzVector e1_v4;
            LorentzVector e2_v4;
            int hadE  = 0; // electrons from LQ with hadronic decaying top
            int lepE = 0; // electrons from LQ with leptonic decaying top
            int num = i;
	    for(unsigned int k=0; k<n_electrons; k++){
              if(num%3==0){
                e1_v4 = event.electrons->at(k).v4();
                hyp.set_ele_had(event.electrons->at(k)); // had is only a hypothesis not having considered the charge!!!
                hadE++;
              }
              if(num%3==1){
                e2_v4 = event.electrons->at(k).v4();
                hyp.set_ele_lep(event.electrons->at(k)); // lep is only a hypothesis not having considered the charge!!!
                lepE++;
              }
              //for num%3==2 do nothing
              num /= 3;
            } // loop over n_electrons
	    
	    Particle e_2 = hyp.ele_lep();
	    Particle e_1 = hyp.ele_had();
	    if(hadE ==1 && lepE == 1 && (e_1.charge() != e_2.charge())) { // require 1 electron for each top, oppisite charges
	      if(e_2.charge() != muon.charge()) { // case: e_2 is electron from LQ with leptonic decaying top
		hyp.set_ele_had_v4(e1_v4);
                hyp.set_ele_lep_v4(e2_v4); // e2 really is the leptonic one.
                hyp.set_ele_had(hyp.ele_had());
                hyp.set_ele_lep(hyp.ele_lep());
                hyp.set_tophad_v4(tophad_v4);
                hyp.set_toplep_v4(toplep_v4);
                hyp.set_muon(muon);
                hyp.set_muon_v4(muon_v4);
                hyp.set_neutrino_v4(neutrino_p4);
                hyp.set_tophad_jets(had_jets);
                hyp.set_toplep_jets(lep_jets);
	      }

	      else { // case: e_2 is electron from LQ with hadronic decaying top quark
		hyp.set_ele_had_v4(e2_v4);
                hyp.set_ele_lep_v4(e1_v4); // e1 really is the leptonic one, the original hypothesis was wrong.
                hyp.set_ele_had(hyp.ele_lep());
                hyp.set_ele_lep(hyp.ele_had());
                hyp.set_tophad_v4(tophad_v4);
                hyp.set_toplep_v4(toplep_v4);
                hyp.set_muon(muon);
                hyp.set_muon_v4(muon_v4);
                hyp.set_neutrino_v4(neutrino_p4);
                hyp.set_tophad_jets(had_jets);
                hyp.set_toplep_jets(lep_jets);
	      }

	      recoHyps.emplace_back(move(hyp));
              n_final_hyps++;
	    } // 1 electron per top
	  } // loop over max_i
	} // at least one jet per top quark
      } // loop over max_j
    } // loop over neutrino solutions
  } // muon case
  
  
  else if(mode == "ele") { // ele case
    
    //sum should be either +1 or -1, if there is a pair with opposite charges as required by the reco
    vector<Electron> recoelectrons;
    int idx=0;
    for(const auto & e : *event.electrons){
      if(idx<3){ //Only consider 3 leading Electrons
        recoelectrons.emplace_back(e);
        idx++;
      }
    }
    
    double sum_charges = 0;
    int inx = 0;
    
    
    for(const auto & ele : recoelectrons){
      sum_charges += ele.charge();
    }
    if(fabs(sum_charges) != 1) throw runtime_error("In ElectronicLQReconstruction: All electrons have the same charge. Should not be the case.");
    
    //find electrons with same charges, one of these must come from W if our assumption is correct. Must be ==2 candidates
    vector<Particle> electron_candidates;
    vector<bool> used_as_candidate;
    inx=0;
    for(const auto & ele : recoelectrons){
      bool used = false;
      /*
      if(sum_charges > 0){
        if(ele.charge() > 0){
          electron_candidates.push_back(ele);
          used = true;
        }
      }
      else{
        if(ele.charge() < 0){
          electron_candidates.push_back(ele);
          used = true;
        }
      }
      */
      if(sum_charges>0 && ele.charge()>0) {
	electron_candidates.push_back(ele);
	used = true;
      }
      if(sum_charges<0 && ele.charge()<0) {
	electron_candidates.push_back(ele);
	used = true;
      }
      used_as_candidate.push_back(used);
      inx++;
    }
    if(electron_candidates.size() != 2) throw runtime_error("In ElectronicLQReconstruction: Not ==2 electron candidates. Should have been prohibited by earlier runtime_error-conditions...");
    
    // loop over possible electron candidates
    for (int m=0; m<2; m++) {
      //      unsigned int current_electron = 99;
      const Particle electron = electron_candidates.at(m);
      LorentzVector electron_v4 = electron.v4();
      /*
      if(m == 0){
        if(used_as_candidate[0]) current_electron = 0;
        else if(used_as_candidate[1]) current_electron = 1;
        else throw runtime_error("In ElectronicLQReconstruction: Neither the first nor the second entry of used_as_candidate is 'true'. There are 2 candidates out of 3 electrons, two entries should be true, which is not possible anymore.");
      }
      else if(m==1){
        if(used_as_candidate[0] && used_as_candidate[1]) current_electron = 1;
        else if(used_as_candidate[0] && !used_as_candidate[1] && used_as_candidate[2]) current_electron = 2;
        else if(!used_as_candidate[0] && used_as_candidate[1] && used_as_candidate[2]) current_electron = 2;
        else throw runtime_error("In ElectronicLQReconstruction: Couldn't find second electron-candidate.");
      }*/

    
    // reconstructed neutrino
    vector<LorentzVector> neutrinos = m_neutrinofunction( electron.v4(), event.met->v4());

    unsigned int n_electrons = recoelectrons.size();
    unsigned int n_jets = event.jets->size();
    if (n_jets>7) n_jets = 7; // only consider first 7 jets
    
    const unsigned int max_j = pow(3, n_jets); // loop over max_j possibilities


    // loop over possible neutrino solutions
    for (const auto & neutrino_p4 : neutrinos) {
      const LorentzVector wlep_v4 = electron.v4() + neutrino_p4; // mass of leptonic decaying W
      // loop over max_j
      for (unsigned int j=0; j<max_j; j++) {
	LorentzVector tophad_v4; // hadronic decaying top
	LorentzVector toplep_v4 = wlep_v4; // leptonic decaying top
	int hadjets = 0;
	int lepjets = 0;
	int num = j;

	LQReconstructionHypothesis hyp;
	vector<Particle> had_jets, lep_jets;
	
	// loop over n_jets
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
          //in case num%3==2 do not take this jet at all
          //shift the trigits of num to the right:
          num /= 3;
	} // loop over n_jets
	
	hyp.set_tophad_jets(had_jets);
	hyp.set_toplep_jets(lep_jets);

	// fill only hypotheses with at least one jet assigned to each top quark
	if(hadjets>0 && lepjets>0) {
	  int max_i = pow(3, n_electrons-1);
	  // loop over all electron combs for every jet comb
	  for (int i=0; i<max_i; i++) {
	    LorentzVector e1_v4;
            LorentzVector e2_v4;
            int hadE  = 0; // electrons from LQ with hadronic decaying top
            int lepE = 0; // electrons from LQ with leptonic decaying top
            int num = i;
	    for(unsigned int k=0; k<n_electrons; k++){
              if(num%3==0){
                e1_v4 = recoelectrons.at(k).v4();
                hyp.set_ele_had(recoelectrons.at(k)); // had is only a hypothesis not having considered the charge!!!
                hadE++;
              }
              if(num%3==1){
                e2_v4 = recoelectrons.at(k).v4();
                hyp.set_ele_lep(recoelectrons.at(k)); // lep is only a hypothesis not having considered the charge!!!
                lepE++;
              }
              //for num%3==2 do nothing
              num /= 3;
            } // loop over n_electrons
	    
	    Particle e_2 = hyp.ele_lep();
	    Particle e_1 = hyp.ele_had();
	    if(hadE ==1 && lepE == 1 && (e_1.charge() != e_2.charge())) { // require 1 electron for each top, oppisite charges
	      if(e_2.charge() != electron.charge()) { // case: e_2 is electron from LQ with leptonic decaying top
		hyp.set_ele_had_v4(e1_v4);
                hyp.set_ele_lep_v4(e2_v4); // e2 really is the leptonic one.
                hyp.set_ele_had(hyp.ele_had());
                hyp.set_ele_lep(hyp.ele_lep());
                hyp.set_tophad_v4(tophad_v4);
                hyp.set_toplep_v4(toplep_v4);
                hyp.set_electron(electron);
                hyp.set_electron_v4(electron_v4);
                hyp.set_neutrino_v4(neutrino_p4);
                hyp.set_tophad_jets(had_jets);
                hyp.set_toplep_jets(lep_jets);
	      }

	      else { // case: e_2 is electron from LQ with hadronic decaying top quark
		hyp.set_ele_had_v4(e2_v4);
                hyp.set_ele_lep_v4(e1_v4); // e1 really is the leptonic one, the original hypothesis was wrong.
                hyp.set_ele_had(hyp.ele_lep());
                hyp.set_ele_lep(hyp.ele_had());
                hyp.set_tophad_v4(tophad_v4);
                hyp.set_toplep_v4(toplep_v4);
                hyp.set_electron(electron);
                hyp.set_electron_v4(electron_v4);
                hyp.set_neutrino_v4(neutrino_p4);
                hyp.set_tophad_jets(had_jets);
                hyp.set_toplep_jets(lep_jets);
	      }

	      recoHyps.emplace_back(move(hyp));
              n_final_hyps++;
	    } // 1 electron per top
	  } // loop over max_i
	} // at least one jet per top quark
      } // loop over max_j
    } // loop over neutrino solutions
    } // electron candidates
  }// ele case
    
    
  else throw runtime_error("In InclusiveLQReconstruction: Neither muon nor electron case. Should have been prohibited by earlier runtime_error-conditions.");
    
    
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
