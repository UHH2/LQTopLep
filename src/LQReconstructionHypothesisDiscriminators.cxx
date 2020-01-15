#include "UHH2/LQTopLep/include/LQReconstructionHypothesisDiscriminators.h"
#include "UHH2/core/include/Utils.h"

#include <set>

using namespace uhh2;
using namespace std;

namespace {

  // invariant mass of a lorentzVector, but save for timelike / spacelike vectors
  float inv_mass(const LorentzVector & p4) {
    if(p4.isTimelike()) {
      return p4.mass();
    }
    else {
      return -sqrt(-p4.mass2());
    }
  }

}


const LQReconstructionHypothesis * get_best_hypothesis(const std::vector<LQReconstructionHypothesis> & hyps, const std::string & label) {
  const LQReconstructionHypothesis * best = nullptr;
  float current_best_disc = numeric_limits<float>::infinity();
  for(const auto & hyp : hyps){
    if(!hyp.has_discriminator(label)) continue;
    auto disc = hyp.discriminator(label);
    if(disc < current_best_disc){
      best = &hyp;
      current_best_disc = disc;
    }
  }
  if(std::isfinite(current_best_disc)){
    return best;
  }
  else{
    return nullptr;
  }
}

LQChi2Discriminator::LQChi2Discriminator(Context & ctx, const std::string & rechyps_name, const cfg & config_): config(config_) {
  h_hyps = ctx.get_handle<vector<LQReconstructionHypothesis>>(rechyps_name); //
  h_is_mlq_reconstructed = ctx.get_handle<bool>("is_mlq_reconstructed");
  h_mlq = ctx.get_handle<float>("mlq");
  h_chi2 = ctx.get_handle<float>("chi2");
}


bool LQChi2Discriminator::process(uhh2::Event & event) {
  bool is_mlq_reconstructed = event.get(h_is_mlq_reconstructed);
  if (!is_mlq_reconstructed) return false;
  auto & hyps = event.get(h_hyps); // get vector of hypotheses from event
  /*
  const double mass_thad = 181;
  const double mass_thad_sigma = 15;
  const double mass_tlep = 174;
  const double mass_tlep_sigma = 18;
  const double mass_LQ_diff_rel = -0.0087;
  const double mass_LQ_diff_rel_sigma = 0.090;
  */
  const double mass_thad = 174.;
  const double mass_thad_sigma = 16.;
  const double mass_tlep = 173.;
  const double mass_tlep_sigma = 22.;
  const double mass_LQ_diff_rel = -0.013;
  const double mass_LQ_diff_rel_sigma = 0.088;

  /*
  const double pt_LQ_diff = 0.65;
  const double pt_LQ_diff_sigma = 46.;
  const double pt_ratio = 1;
  const double pt_ratio_sigma = 0.15;
  const double dphi_LQ = 3.14;
  const double dphi_LQ_sigma = 0.09;
  */
  for(auto & hyp: hyps){ // loop over hypothesis vector
    double mass_thad_rec = inv_mass(hyp.tophad_v4());
    double mass_tlep_rec = inv_mass(hyp.toplep_v4());
    double mass_LQ_had_rec = inv_mass(hyp.LQhad_v4()); // added
    double mass_LQ_lep_rec = inv_mass(hyp.LQlep_v4()); // added
    double mass_LQ_mean_rec = (mass_LQ_had_rec + mass_LQ_lep_rec)/2;
    /*
    double pt_ratio_rec = (hyp.LQhad_v4().Pt() / hyp.LQlep_v4().Pt());
    double dphi_LQ_rec = hyp.LQhad_v4().Phi() - hyp.LQlep_v4().Phi();
    if(dphi_LQ_rec < 0) dphi_LQ_rec = dphi_LQ_rec + 2*M_PI;
    double chi2_pt_diff = pow(((hyp.LQhad_v4().Pt() - hyp.LQlep_v4().Pt()) - pt_LQ_diff) / pt_LQ_diff_sigma, 2);
    */
    double chi2_thad = pow((mass_thad_rec - mass_thad) / mass_thad_sigma, 2);
    double chi2_tlep = pow((mass_tlep_rec - mass_tlep) / mass_tlep_sigma, 2);
    double chi2_MLQdiff_rel = pow( ((mass_LQ_had_rec - mass_LQ_lep_rec)/mass_LQ_mean_rec - mass_LQ_diff_rel) / mass_LQ_diff_rel_sigma, 2);
    /*
    double chi2_pt_ratio = pow((pt_ratio_rec - pt_ratio)/pt_ratio_sigma, 2);
    double chi2_dphi_LQ = pow((dphi_LQ_rec - dphi_LQ)/dphi_LQ_sigma,2);
    */
    hyp.set_discriminator(config.discriminator_label + "_tlep", chi2_tlep);
    hyp.set_discriminator(config.discriminator_label + "_thad", chi2_thad);
    hyp.set_discriminator(config.discriminator_label + "_MLQdiff_rel", chi2_MLQdiff_rel);// added
    hyp.set_discriminator(config.discriminator_label + "_tlep_MQLdiff_rel", chi2_tlep + chi2_MLQdiff_rel);
    hyp.set_discriminator(config.discriminator_label, chi2_thad + chi2_tlep + chi2_MLQdiff_rel); // modified
    //hyp.set_discriminator(config.discriminator_label, chi2_tlep + chi2_MLQdiff_rel); // modified
    //hyp.set_discriminator(config.discriminator_label, chi2_tlep + chi2_MLQdiff_rel + chi2_pt_diff); // modified
    //hyp.set_discriminator(config.discriminator_label, chi2_tlep + chi2_MLQdiff_rel + chi2_pt_ratio + chi2_dphi_LQ); // modified
  }


  const LQReconstructionHypothesis* hyp = get_best_hypothesis( hyps, "Chi2" );
  double chi2 = hyp->discriminator("Chi2");
  double mLQmed_rec = 0;
  double mLQlep_rec = 0;
  double mLQhad_rec = 0;

  if( (hyp->LQlep_v4()).isTimelike() ) {mLQlep_rec = (hyp->LQlep_v4()).M();}
  else {mLQlep_rec = sqrt( -(hyp->LQlep_v4()).mass2());}
  if( (hyp->LQhad_v4()).isTimelike() ) {mLQhad_rec = (hyp->LQhad_v4()).M();}
  else {mLQhad_rec = sqrt( -(hyp->LQhad_v4()).mass2());}
  mLQmed_rec = (mLQhad_rec + mLQlep_rec) / 2;

  event.set(h_mlq, mLQmed_rec);
  event.set(h_chi2, chi2);
  return true;
}


/* not needed I think

LQTopDRMCDiscriminator::LQTopDRMCDiscriminator(Context & ctx, const std::string & rechyps_name, const cfg & config_): config(config_){
h_hyps = ctx.get_handle<vector<LQReconstructionHypothesis>>(rechyps_name);
h_ttbargen = ctx.get_handle<TTbarGen>(config.ttbargen_name);
}



bool LQTopDRMCDiscriminator::process(uhh2::Event & event){
auto & hyps = event.get(h_hyps);
const auto & ttbargen = event.get(h_ttbargen);
for(auto & hyp: hyps){
auto deltar_sum = deltaR(ttbargen.Top(), hyp.top_v4()) + deltaR(ttbargen.Antitop(), hyp.antitop_v4());
hyp.set_discriminator(config.discriminator_label, deltar_sum);
}
return true;
}



LQCorrectMatchDiscriminator::LQCorrectMatchDiscriminator(Context & ctx, const std::string & rechyps_name, const cfg & config_): config(config_){
h_hyps = ctx.get_handle<vector<LQReconstructionHypothesis>>(rechyps_name);
h_ttbargen = ctx.get_handle<TTbarGen>(config.ttbargen_name);
h_LQLQbargen = ctx.get_handle<LQGen>(config.LQLQbargen_name);
}

namespace {

// match particle p to one of the jets (Delta R < 0.3); return the deltaR
// of the match.
template<typename T> // T should inherit from Particle
float match_dr(const Particle & p, const std::vector<T> & jets, int& index){
float mindr = 999999;
index = -1;
for(unsigned int i=0; i<jets.size(); ++i){
float dR = deltaR(p, jets.at(i));
if( dR <0.3 && dR<mindr) {
mindr=dR;
index=i;
}
}
return mindr;
}


}
//changes for mu --> ele start here
bool LQCorrectMatchDiscriminator::process(uhh2::Event & event){  // replaced 'infinity' with '999999'
auto & hyps = event.get(h_hyps);
const auto & ttbargen = event.get(h_ttbargen);
const auto & LQLQbargen = event.get(h_LQLQbargen);
auto dec = ttbargen.DecayChannel();
if(dec == TTbarGen::e_muhad) { // if not 1x Electron, 1x hadronic
// note that it is allowed that two partons from the hadronic ttbar decay match the same jet.
for(auto & hyp: hyps) {
auto hadr_jets = hyp.tophad_jets();
auto lept_jets = hyp.toplep_jets();
auto hadr_ele = hyp.ele_had();
auto lept_ele = hyp.ele_lep();
auto mu = hyp.muon();

if(lept_jets.size() != 1){
hyp.set_discriminator(config.discriminator_label, 999999);
continue;
}
if(hadr_jets.size() > 3){ // < 3 is allowed ...
hyp.set_discriminator(config.discriminator_label, 999999);
continue;
}

//index lists of jets that can be matched to partons
std::set<int> matched_hadr_jets;

// match b jets
int index_l, index_h;
float correct_dr = match_dr(ttbargen.BLep(), lept_jets, index_l) + match_dr(ttbargen.BHad(), hadr_jets, index_h);
if(index_h >= 0) matched_hadr_jets.insert(index_h);
//match quarks from W decays
correct_dr += match_dr(ttbargen.Q1(), hadr_jets, index_h);
if(index_h >= 0) matched_hadr_jets.insert(index_h);
correct_dr += match_dr(ttbargen.Q2(), hadr_jets, index_h);
if(index_h >= 0) matched_hadr_jets.insert(index_h);

// if not all jets of the hadronic side of the reconstruction could be matched: infinite
// value:
if(matched_hadr_jets.size() != hadr_jets.size()){
hyp.set_discriminator(config.discriminator_label, 999999);
continue;
}

// match electrons: always 1 lept. & 1 had. electron
float dR_ele_lept1 = deltaR(LQLQbargen.eleLQ(), lept_ele);
float dR_ele_lept2 = deltaR(LQLQbargen.eleAntiLQ(), lept_ele);
float dR_ele_hadr1 = deltaR(LQLQbargen.eleLQ(), hadr_ele);
float dR_ele_hadr2 = deltaR(LQLQbargen.eleAntiLQ(), hadr_ele);
float dR_min_ges = 999999;

// calculate dR for all possible matches (R=0.3): exactly 1 electron is assigned to each LQ
// lept1 : eleLQ - hypothesis
if(dR_ele_lept1 <= 0.3 && dR_ele_hadr2 <= 0.3){
if(dR_ele_lept1 + dR_ele_hadr2 < dR_min_ges){
dR_min_ges = dR_ele_lept1 + dR_ele_hadr2;
}
}
// lept2 : eleLQ - hypothesis
if(dR_ele_lept2 <= 0.3 && dR_ele_hadr1 <= 0.3){
if(dR_ele_lept2 + dR_ele_hadr1 < dR_min_ges){
dR_min_ges = dR_ele_lept2 + dR_ele_hadr1;
}
}

// kick out events, where the reconstructed electrons are too close to each other or a reconstructed jet
int dummie_index;
if(match_dr(lept_ele, hadr_jets, dummie_index) <= 0.3) {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(match_dr(lept_ele, lept_jets, dummie_index) <= 0.3) {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(match_dr(hadr_ele, hadr_jets, dummie_index) <= 0.3) {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(match_dr(hadr_ele, lept_jets, dummie_index) <= 0.3) {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(deltaR(lept_ele, hadr_ele) <= 0.3)                   {hyp.set_discriminator(config.discriminator_label, 999999); continue;}

// add minimum of dR. which electron was assigned to which LQ is irrelevant.
correct_dr += dR_min_ges;

// kick out events, where the reconstructed muon is too close to a reco muon or reco jet
if(match_dr(mu, hadr_jets, dummie_index) <= 0.3)     {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(match_dr(mu, lept_jets, dummie_index) <= 0.3)     {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(deltaR(mu, lept_ele) <= 0.3)                       {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(deltaR(mu, hadr_ele) <= 0.3)                       {hyp.set_discriminator(config.discriminator_label, 999999); continue;}

// require primary muon to be matched since we expect this one to be the one originating from the W leptonic decay
// only 1 out of the 4 gen W-decay products can be a charged lepton. The lepton has already been forced to be an muon by ~L117: dec = EEbarGen::e_muhad
float dR_mu = deltaR(ttbargen.ChargedLepton(), mu);
if(dR_mu <= 0.3){
correct_dr += dR_mu;
}
else{
hyp.set_discriminator(config.discriminator_label, 999999);
continue;
}

// add deltaR between reconstructed and true neutrino
correct_dr += deltaR(ttbargen.Neutrino(), hyp.neutrino_v4());

//set final dr as discriminator value
hyp.set_discriminator(config.discriminator_label, correct_dr);

}
return true;
}
else if(dec == TTbarGen::e_ehad){
// note that it is allowed that two partons from the hadronic ttbar decay match the same jet.
for(auto & hyp: hyps){
auto hadr_jets = hyp.tophad_jets();
auto lept_jets = hyp.toplep_jets();
auto hadr_ele = hyp.ele_had();
auto lept_ele = hyp.ele_lep();
auto ele = hyp.electron();

if(lept_jets.size() != 1){
hyp.set_discriminator(config.discriminator_label, 999999);
continue;
}
if(hadr_jets.size() > 3){ // < 3 is allowed ...
hyp.set_discriminator(config.discriminator_label, 999999);
continue;
}

//index lists of jets that can be matched to partons
std::set<int> matched_hadr_jets;

// match b jets
int index_l, index_h;
float correct_dr = match_dr(ttbargen.BLep(), lept_jets, index_l) + match_dr(ttbargen.BHad(), hadr_jets, index_h);

if(index_h >= 0) matched_hadr_jets.insert(index_h);
//match quarks from W decays
correct_dr += match_dr(ttbargen.Q1(), hadr_jets, index_h);
if(index_h >= 0) matched_hadr_jets.insert(index_h);
correct_dr += match_dr(ttbargen.Q2(), hadr_jets, index_h);
if(index_h >= 0) matched_hadr_jets.insert(index_h);

// if not all jets of the hadronic side of the reconstruction could be matched: infinite
// value:
if(matched_hadr_jets.size() != hadr_jets.size()){
hyp.set_discriminator(config.discriminator_label, 999999);
continue;
}

// match electrons: always 1 lept. & 1 had. electron
float dR_ele_lept1 = deltaR(LQLQbargen.eleLQ(), lept_ele);
float dR_ele_lept2 = deltaR(LQLQbargen.eleAntiLQ(), lept_ele);
float dR_ele_hadr1 = deltaR(LQLQbargen.eleLQ(), hadr_ele);
float dR_ele_hadr2 = deltaR(LQLQbargen.eleAntiLQ(), hadr_ele);
float dR_min_ges = 999999;

// calculate dR for all possible matches (R=0.3): exactly 1 electron is assigned to each LQ
// lept1 : eleLQ - hypothesis
if(dR_ele_lept1 <= 0.3 && dR_ele_hadr2 <= 0.3){
if(dR_ele_lept1 + dR_ele_hadr2 < dR_min_ges){
dR_min_ges = dR_ele_lept1 + dR_ele_hadr2;
}
}
// lept2 : eleLQ - hypothesis
if(dR_ele_lept2 <= 0.3 && dR_ele_hadr1 <= 0.3){
if(dR_ele_lept2 + dR_ele_hadr1 < dR_min_ges){
dR_min_ges = dR_ele_lept2 + dR_ele_hadr1;
}
}

// kick out events, where the reconstructed electrons are too close to each other or a reconstructed jet
int dummie_index;
if(match_dr(lept_ele, hadr_jets, dummie_index) <= 0.3) {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(match_dr(lept_ele, lept_jets, dummie_index) <= 0.3) {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(match_dr(hadr_ele, hadr_jets, dummie_index) <= 0.3) {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(match_dr(hadr_ele, lept_jets, dummie_index) <= 0.3) {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(deltaR(lept_ele, hadr_ele) <= 0.3)                   {hyp.set_discriminator(config.discriminator_label, 999999); continue;}

// add minimum of dR. which electron was assigned to which LQ is irrelevant.
correct_dr += dR_min_ges;

// kick out events, where the reconstructed muon is too close to a reco electron or reco jet
if(match_dr(ele, hadr_jets, dummie_index) <= 0.3)     {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(match_dr(ele, lept_jets, dummie_index) <= 0.3)     {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(deltaR(ele, lept_ele) <= 0.3)                       {hyp.set_discriminator(config.discriminator_label, 999999); continue;}
if(deltaR(ele, hadr_ele) <= 0.3)                       {hyp.set_discriminator(config.discriminator_label, 999999); continue;}

// require primary muon to be matched since we expect this one to be the one originating from the W leptonic decay
// only 1 out of the 4 gen W-decay products can be a charged lepton. The lepton has already been forced to be an muon by ~L117: dec = EEbarGen::e_muhad
float dR_ele = deltaR(ttbargen.ChargedLepton(), ele);
if(dR_ele <= 0.3){
correct_dr += dR_ele;
}
else{
hyp.set_discriminator(config.discriminator_label, 999999);
continue;}

// add deltaR between reconstructed and true neutrino
correct_dr += deltaR(ttbargen.Neutrino(), hyp.neutrino_v4());

//set final dr as discriminator value
hyp.set_discriminator(config.discriminator_label, correct_dr);

}
return true;









}
else{
for(auto & hyp: hyps){
hyp.set_discriminator(config.discriminator_label, 999999);
}
return true;

}
}

*/
