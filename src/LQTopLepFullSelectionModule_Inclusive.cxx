#include <iostream>
#include <memory>


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
#include "UHH2/common/include/TopJetIds.h"
// my own classes
#include "UHH2/LQTopLep/include/LQTopLepSelections.h"
#include "UHH2/LQTopLep/include/LQTopLepFullSelectionHists.h"
#include "UHH2/LQTopLep/include/LQTopLepHists.h"
#include "UHH2/LQTopLep/include/LQTopLepModules.h"
#include "UHH2/LQTopLep/include/LQMassReconstruction.h"
#include "UHH2/LQTopLep/include/LQReconstructionHypothesis.h"
#include "UHH2/LQTopLep/include/LQReconstructionHypothesisDiscriminators.h"

#include "UHH2/LQTopLep/include/LQTopLepPDFHists.h"
#include <UHH2/LQTopLep/include/ModuleBASE.h>


using namespace std;
using namespace uhh2;

namespace uhh2examples {

  class LQTopLepFullSelectionModule_Inclusive: public ModuleBASE {
  public:

    explicit LQTopLepFullSelectionModule_Inclusive(Context & ctx);
    virtual bool process(Event & event) override;
    void book_histograms(uhh2::Context&, vector<string>);
    void fill_histograms(uhh2::Event&, string, string region);

  private:

    std::unique_ptr<CommonModules> common;

    unique_ptr<Hists> h_btageff;
    unique_ptr<AnalysisModule> SF_eleReco, SF_eleID, SF_muonIso, SF_muonID, SF_muonTrigger, SF_btag, scale_variation_module;

    std::unique_ptr<Selection> nbtag_loose_sel, st_lep_sel, inv_mass_veto_mu, inv_mass_sel_mu, inv_mass_veto_ele, inv_mass_sel_ele, inv_mass_veto_muele, muon_trigger_sel1, muon_trigger_sel2, nele_sel, nmuons_sel;

    unique_ptr<HighMassInclusiveLQReconstruction> mlq_reco;
    unique_ptr<LQChi2Discriminator> chi2_module;

    JetId Btag_loose;
    BTag::algo btag_algo;
    BTag::wp wp_btag_loose, wp_btag_medium, wp_btag_tight;

    bool is_mc, do_permutations, do_scale_variation, do_pdf_variation;
    string s_permutation;

    uhh2::Event::Handle<float> h_eventweight_lumi, h_eventweight_final;
    uhh2::Event::Handle<bool> h_is_mlq_reconstructed;
    uhh2::Event::Handle<float> h_mlq, h_chi2;
    uhh2::Event::Handle<TString> h_mlq_reco_mode_lq, h_mlq_reco_mode_top, h_region;

    // systematic uncertainties
    string Sys_EleID, Sys_EleReco;
    string Sys_MuonID, Sys_MuonIso;
    string Sys_BTag;
    string Sys_PU;

    TString dataset_version;
    Year year;

  };


  void LQTopLepFullSelectionModule_Inclusive::book_histograms(uhh2::Context& ctx, vector<string> tags){
    for(const auto & tag : tags){
      cout << "booking histograms with tag " << tag << endl;
      string mytag = tag+ "_srmu_General";
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = tag+ "_srele_General";
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = tag+ "_ttbar_General";
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = tag+ "_dycrmu_General";
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = tag+ "_dycrele_General";
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = tag+ "_incl_General";
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
    }
  }

  void LQTopLepFullSelectionModule_Inclusive::fill_histograms(uhh2::Event& event, string tag, string region){
    string mytag = tag + "_" + region +"_General";
    HFolder(mytag)->fill(event);
    mytag = tag + "_incl_General";
    HFolder(mytag)->fill(event);
  }



  LQTopLepFullSelectionModule_Inclusive::LQTopLepFullSelectionModule_Inclusive(Context & ctx){

    for(auto & kv : ctx.get_all()){
      cout << " " << kv.first << " = " << kv.second << endl;
    }

    h_eventweight_lumi = ctx.declare_event_output<float>("eventweight_lumi");
    h_eventweight_final = ctx.declare_event_output<float>("eventweight_final");

    h_is_mlq_reconstructed = ctx.declare_event_output<bool>("is_mlq_reconstructed");
    h_mlq = ctx.declare_event_output<float>("mlq");
    h_chi2 = ctx.declare_event_output<float>("chi2");
    h_mlq_reco_mode_lq = ctx.declare_event_output<TString>("mlq_reco_mode_lq");
    h_mlq_reco_mode_top = ctx.declare_event_output<TString>("mlq_reco_mode_top");
    h_region = ctx.declare_event_output<TString>("region");


    is_mc = ctx.get("dataset_type") == "MC";
    year = extract_year(ctx);


    // BTagging
    btag_algo = BTag::DEEPJET;
    wp_btag_loose = BTag::WP_LOOSE;
    wp_btag_medium = BTag::WP_MEDIUM;
    wp_btag_tight = BTag::WP_TIGHT;

    JetId DeepjetLoose = BTag(btag_algo, wp_btag_loose);
    JetId DeepjetMedium = BTag(btag_algo, wp_btag_medium);
    JetId DeepjetTight = BTag(btag_algo, wp_btag_tight);


    // electron triggers
    muon_trigger_sel1.reset(new TriggerSelection("HLT_IsoMu24_v*"));
    muon_trigger_sel2.reset(new TriggerSelection("HLT_IsoTkMu24_v*"));


    // initialize LQ reconstruction modules
    mlq_reco.reset(new HighMassInclusiveLQReconstruction(ctx, LQNeutrinoReconstruction));
    chi2_module.reset(new LQChi2Discriminator(ctx, "LQHypotheses"));

    // Variations of mu_r and mu_f
    scale_variation_module.reset(new MCScaleVariation(ctx));

    // systematic uncertainties
    Sys_EleID = ctx.get("Systematic_EleID");
    Sys_EleReco = ctx.get("Systematic_EleReco");
    Sys_MuonID = ctx.get("Systematic_MuonID");
    Sys_MuonIso = ctx.get("Systematic_MuonIso");
    Sys_BTag = ctx.get("Systematic_BTag");
    Sys_PU = ctx.get("Systematic_PU");

    dataset_version = ctx.get("dataset_version");


    SF_muonID.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonID_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightID_DEN_genTracks_eta_pt", 0., "id", false, Sys_MuonID));
    SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonIso_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt", 0., "iso", false, Sys_MuonIso));
    SF_muonTrigger.reset(new MuonTriggerWeights(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/LQTopLep/data", year));

    SF_eleReco.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root", 1, "reco", Sys_EleReco));
    SF_eleID.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/2016LegacyReReco_ElectronTight_Fall17V2.root", 1, "id", Sys_EleID));

    SF_btag.reset(new MCBTagScaleFactor(ctx, btag_algo, wp_btag_loose, "jets", Sys_BTag));


    // CommonModules
    common.reset(new CommonModules());
    common->disable_lumisel();
    common->disable_jersmear();
    common->disable_jec();
    common->init(ctx, Sys_PU);


    // Selections
    nbtag_loose_sel.reset(new NJetSelection(1, -1, DeepjetLoose));
    st_lep_sel.reset(new HtLepSelection(200, -1));
    inv_mass_veto_mu.reset(new InvMass2MuVeto(0., 110.));
    inv_mass_veto_ele.reset(new InvMassEleEleVeto(0., 100.));
    inv_mass_sel_mu.reset(new InvMass2MuSelection(70., 110.));
    inv_mass_sel_ele.reset(new InvMassEleEleSelection(70., 110.));
    inv_mass_veto_muele.reset(new InvMassEleMuVeto(0., 110.));



    // Book histograms
    vector<string> histogram_tags = {"Cleaner", "Trigger", "TriggerSF", "BTagSF", "BTag", "Mleplep", "STlep", "FinalSelection", "FinalSelection_CatA", "FinalSelection_CatB"};
    book_histograms(ctx, histogram_tags);

    h_btageff.reset(new BTagMCEfficiencyHists(ctx, "BTagEff", DeepjetLoose));

  }


  bool LQTopLepFullSelectionModule_Inclusive::process(Event & event) {


    event.set(h_is_mlq_reconstructed, false);
    event.set(h_mlq, -1.);
    event.set(h_chi2, -1.);
    event.set(h_mlq_reco_mode_lq, "none");
    event.set(h_mlq_reco_mode_top, "none");

    string region = "";

    if(event.muons->size() == 1 && event.electrons->size() == 1) region = "ttbar";
    else if (event.muons->size() >= 2 && event.electrons->size() < 2) region = "srmu";
    else if(event.electrons->size() >= 2 && event.muons->size() < 2) region = "srele";
    else if(event.electrons->size() >= 2 && event.muons->size() >= 2){
      if(event.muons->at(0).pt() > event.electrons->at(0).pt()) region = "srmu";
      else                                                      region = "srele";
    }
    else cout << "muons: " << event.muons->size() << ", electrons: " << event.electrons->size() << endl;
    // else throw runtime_error("In LQTopLepFullSelectionModule_Inclusive::process: No region assigned to this event. What happened?");




    bool pass_common = common->process(event);
    if(!pass_common) return false;

    double eventweight_lumi = event.weight;
    event.set(h_eventweight_lumi, eventweight_lumi);

    SF_eleReco->process(event);
    SF_eleID->process(event);
    SF_muonID->process(event);
    SF_muonIso->process(event);
    fill_histograms(event,"Cleaner", region);


    if(!(muon_trigger_sel1->passes(event) || muon_trigger_sel2->passes(event))) return false;
    fill_histograms(event,"Trigger", region);

    SF_muonTrigger->process(event);
    fill_histograms(event,"TriggerSF", region);

    SF_btag->process(event);
    h_btageff->fill(event);
    fill_histograms(event, "BTagSF", region);

    if(!nbtag_loose_sel->passes(event)) return false;
    fill_histograms(event,"BTag", region);

    // Here, srmu is split into srmu and dycr
    if(region == "srmu"){
      if(!inv_mass_veto_mu->passes(event)){
        if(inv_mass_sel_mu->passes(event)) region = "dycrmu";
        else                               return false;
      }
    }
    else if(region == "srele"){
      if(!inv_mass_veto_ele->passes(event)){
        if(inv_mass_sel_ele->passes(event)) region = "dycrele";
        else                                return false;
      }
    }
    else if(region == "ttbar"){
      if(!inv_mass_veto_muele->passes(event)) return false;
    }
    fill_histograms(event,"Mleplep", region);

    if(region != "dycrmu" && region != "dycrele"){
      if(!st_lep_sel->passes(event)) return false;
    }
    fill_histograms(event,"STlep", region);

    mlq_reco->process(event);
    chi2_module->process(event);
    bool is_mlq_reconstructed = event.get(h_is_mlq_reconstructed);
    fill_histograms(event,"FinalSelection", region);

    scale_variation_module->process(event);

    if(is_mlq_reconstructed){
      fill_histograms(event,"FinalSelection_CatA", region);
    }
    else{
      fill_histograms(event,"FinalSelection_CatB", region);
    }

    event.set(h_eventweight_final, event.weight);
    event.set(h_region, region);
    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the LQTopLepFullSelectionModule_Inclusive is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepFullSelectionModule_Inclusive)

}
