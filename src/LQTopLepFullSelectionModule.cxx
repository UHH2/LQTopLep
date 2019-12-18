#include <iostream>
#include <memory>


#include <UHH2/SingleTth/include/ModuleBASE.h>
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
#include "UHH2/LQTopLep/include/LQTopLepModules.h"
#include "UHH2/LQTopLep/include/LQMassReconstruction.h"
#include "UHH2/LQTopLep/include/LQReconstructionHypothesis.h"
#include "UHH2/LQTopLep/include/LQReconstructionHypothesisDiscriminators.h"

#include "UHH2/LQTopLep/include/LQTopLepPDFHists.h"


using namespace std;
using namespace uhh2;

namespace uhh2examples {

  class LQTopLepFullSelectionModule: public ModuleBASE {
  public:
    
    explicit LQTopLepFullSelectionModule(Context & ctx); // constructor
    virtual bool process(Event & event) override; 

  private:
    
    std::unique_ptr<CommonModules> common;
    
    std::unique_ptr<JetCleaner> jetcleaner;
    std::unique_ptr<ElectronCleaner> electroncleaner;

    unique_ptr<AnalysisModule> SF_eleReco, SF_eleID, SF_muonIso, SF_muonID, SF_btag, SF_btagMedium;
    std::unique_ptr<AnalysisModule> syst_module; // for renormalization and factorization scales mu_r, mu_f
    unique_ptr<ElectronTriggerWeights> SF_eleTrigger;
   
    std::unique_ptr<Selection> nbtag_loose_sel, ht_lep_sel, inv_mass_veto, ele_trigger_sel1, ele_trigger_sel2, nele_sel, nmuons_sel;

    unique_ptr<HighMassInclusiveLQReconstruction> mlq_reco;

    unique_ptr<LQChi2Discriminator> chi2_module;
    
    JetId Btag_loose;
    BTag::algo btag_algo;
    BTag::wp wp_btag_loose, wp_btag_medium, wp_btag_tight;

    bool is_mc, do_permutations, do_scale_variation, do_pdf_variation;
    string s_permutation;
    uhh2::Event::Handle<bool> h_is_mlq_reconstructed;
    uhh2::Event::Handle<TString> h_mlq_reco_mode;

    // systematic uncertainties
    string Sys_EleID, Sys_EleReco, Sys_EleTrigger;
    string Sys_MuonID, Sys_MuonIso, Sys_MuonTrigger;
    string Sys_BTag;
    string Sys_PU;

    TString dataset_version;

    string Sys_EleFake, Sys_MuonFake;
    //uhh2::Event::Handle<double> h_FakeRateWeightEle, h_FakeRateWeightMu;

  };


  void LQTopLepTriggerFullselectionModule::book_histograms(uhh2::Context& ctx, vector<string> tags){
    for(const auto & tag : tags){
      cout << "booking histograms with tag " << tag << endl;
      string mytag = tag+"_General";
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = tag+"_Muons";
      book_HFolder(mytag, new MuonHists(ctx,mytag));
      mytag = tag+"_Electrons";
      book_HFolder(mytag, new ElectronHists(ctx,mytag));
      mytag = tag+"_Jets";
      book_HFolder(mytag, new JetHists(ctx,mytag));
      mytag = tag+"_Event";
      book_HFolder(mytag, new EventHists(ctx,mytag));
    }
  }

  void LQTopLepTriggerFullselectionModule::fill_histograms(uhh2::Event& event, string tag){
    string mytag = tag+"_General";
    HFolder(mytag)->fill(event);
    mytag = tag+"_Muons";
    HFolder(mytag)->fill(event);
    mytag = tag+"_Electrons";
    HFolder(mytag)->fill(event);
    mytag = tag+"_Jets";
    HFolder(mytag)->fill(event);
    mytag = tag+"_Event";
    HFolder(mytag)->fill(event);
  }



  LQTopLepFullSelectionModule::LQTopLepFullSelectionModule(Context & ctx){
    cout << __LINE__ << endl; 


    cout << "Hello World from LQTopLepFullSelectionModule!" << endl;
    for(auto & kv : ctx.get_all()){
      cout << " " << kv.first << " = " << kv.second << endl;
    }

    h_is_mlq_reconstructed = ctx.get_handle<bool>("is_mlq_reconstructed");
    h_mlq_reco_mode = ctx.get_handle<TString>("mlq_reco_mode");


    is_mc = ctx.get("dataset_type") == "MC";


    // BTagging
    btag_algo = BTag::DEEPJET;
    wp_btag_loose = BTag::WP_LOOSE;
    wp_btag_medium = BTag::WP_MEDIUM;
    wp_btag_tight = BTag::WP_TIGHT;

    JetId DeepjetLoose = BTag(btag_algo, wp_btag_loose);
    JetId DeepjetMedium = BTag(btag_algo, wp_btag_medium);
    JetId DeepjetTight = BTag(btag_algo, wp_btag_tight);



    // Permutations
    do_permutations = ctx.get("Do_Permutations") == "true";
    s_permutation = ctx.get("Permutation");
    int permutation = stoi(s_permutation, nullptr, 10);
    cout << "permutation: " << permutation << endl;
    if (permutation < 0 && do_permutations) throw runtime_error("Invalid value for permutation");
    
    int btag_value = 1;
    double mee_value = 110., stlep_value = 200.;
    if(do_permutations){
      vector<int> btag_values = {1, 2};
      vector<double> mee_values = {100., 110., 130.};
      vector<double> stlep_values = {0., 150., 200., 300.};
    
      int btag_idx = permutation % btag_values.size();
      int mee_idx = (permutation / btag_values.size()) % mee_values.size();
      int stlep_idx = (permutation / btag_values.size() / mee_values.size()) % stlep_values.size();

      btag_value = btag_values[btag_idx];
      mee_value = mee_values[mee_idx];
      stlep_value = stlep_values[stlep_idx];
    }

    
    // electron triggers
    ele_trigger_sel1.reset(new TriggerSelection("HLT_Ele27_WPTight_Gsf_v*"));
    ele_trigger_sel2.reset(new TriggerSelection("HLT_Ele115_CaloIdVT_GsfTrkIdT_v*"));


    // initialize LQ reconstruction modules
    mlq_reco.reset(new HighMassInclusiveLQReconstruction(ctx, LQNeutrinoReconstruction));
    chi2_module.reset(new LQChi2Discriminator(ctx, "LQHypotheses"));

    cout << __LINE__ << endl; 

    // systematic uncertainties
    Sys_EleID = ctx.get("Systematic_EleID");
    Sys_EleReco = ctx.get("Systematic_EleReco");
    Sys_EleTrigger = ctx.get("Systematic_EleTrigger");
    Sys_MuonID = ctx.get("Systematic_MuonID");
    Sys_MuonIso = ctx.get("Systematic_MuonIso");
    //Sys_MuonTrigger = ctx.get("Systematic_MuonTrigger");
    Sys_BTag = ctx.get("Systematic_BTag");
    Sys_PU = ctx.get("Systematic_PU");

    Sys_EleFake = ctx.get("Systematic_EleFake");
    Sys_MuonFake = ctx.get("Systematic_MuonFake");


    do_scale_variation = (ctx.get("ScaleVariationMuR") == "up" || ctx.get("ScaleVariationMuR") == "down" ||  ctx.get("ScaleVariationMuF") == "up" || ctx.get("ScaleVariationMuF") == "down"); 
    do_pdf_variation = (ctx.get("ScaleVariationProcess") == "true");
    dataset_version = ctx.get("dataset_version");



    // Scale Factors for MonteCarlo
    // SFs for trigger: have to be remade
    SF_eleTrigger.reset(new ElectronTriggerWeights(ctx, "/nfs/dust/cms/user/reimersa/LQToTopMu/Run2_80X_v3/TagProbe/Optimization/35867fb_Iso27_NonIso115/ElectronEfficiencies.root", Sys_EleTrigger)); 

    // SFs for Id, Reco, Iso: copied from SingleTthAnalysisModule.cxx
 
    SF_muonID.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonID_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightID_DEN_genTracks_eta_pt", 0., "id", false, Sys_MuonID));
    SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonIso_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt", 0., "iso", false, Sys_MuonIso));

    SF_eleReco.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root", 1, "reco", Sys_EleReco));
    SF_eleID.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/2016LegacyReReco_ElectronTight_Fall17V2.root", 1, "id", Sys_EleID));

    //SF_btag.reset(new MCBTagScaleFactor(ctx, btag_algo, wp_btag_loose, "jets", Sys_BTag)); // comment out when re-doing SF_btag
 

    // CommonModules
   common.reset(new CommonModules());
   common->disable_lumisel();
   common->disable_jersmear();
   common->disable_jec();
   cout << __LINE__ << endl; 
   common->init(ctx/*, Sys_PU*/);


   /*
     if(Sys_EleFake == "nominal")   h_FakeRateWeightEle =ctx.get_handle<double>("FakeRateWeightEle");
   else if(Sys_EleFake == "up")   h_FakeRateWeightEle =ctx.get_handle<double>("FakeRateWeightEleUp");
   else if(Sys_EleFake == "down") h_FakeRateWeightEle =ctx.get_handle<double>("FakeRateWeightEleDown");
   else throw runtime_error("Sys_EleFake is not one of the following: ['up', 'down', 'nominal']");
   if(Sys_MuonFake == "nominal")   h_FakeRateWeightMu =ctx.get_handle<double>("FakeRateWeightMu");
   else if(Sys_MuonFake == "up")   h_FakeRateWeightMu =ctx.get_handle<double>("FakeRateWeightMuUp");
   else if(Sys_MuonFake == "down") h_FakeRateWeightMu =ctx.get_handle<double>("FakeRateWeightMuDown");
   else throw runtime_error("Sys_MuonFake is not one of the following: ['up', 'down', 'nominal']");
    */


   // Selections
    
   nbtag_loose_sel.reset(new NJetSelection(btag_value, -1, DeepjetLoose));
   ht_lep_sel.reset(new HtLepSelection(stlep_value, -1)); // returns true if STlep>200
   inv_mass_veto.reset(new InvMassEleEleVeto(0., mee_value)); // returns true if M_ee>111 GeV
   // split analysis into LQ->te and LQ->tmu and create hists for both inside this module?
   nele_sel.reset(new NElectronSelection(2, -1));
   nmuons_sel.reset(new NMuonSelection(2, -1));
 
   syst_module.reset(new MCScaleVariation(ctx));



   // Book histograms
   vector<string> histogram_tags = {"Cleaner", "Trigger", "Nbtag", "STlep", "EleInvMass", /*BTagEff_M_ee,*/ "Finalselection", "Finalselection_mlqtrue", "Finalselection_mlqfalse" /*, "PDF_variations"*/};
    book_histograms(ctx, histogram_tags);


   //h_btageff.reset(new BTagMCEfficiencyHists(ctx, "BTagEff",wp_btag_loose));

   //h_PDF_variations.reset(new LQTopLepPDFHists(ctx, "PDF_variations", true, do_pdf_variation));

  }


  bool LQTopLepFullSelectionModule::process(Event & event) {


    event.set(h_is_mlq_reconstructed, false);
    event.set(h_mlq_reco_mode, "none");


    /*
    //Apply fake-rate scale factors
    double SF_ele = 1, SF_mu = 1;
    double n_matched_to_taus = 0, n_matched_to_muons = 0; // reason for this line?

    if(is_mc){
    //cout <<endl << "NEW EVENT" << endl << "Before applying SF: weight = " << event.weight << endl;
    SF_ele = event.get(h_FakeRateWeightEle);
    if(event.electrons->size() == 0 && SF_ele != 1) throw runtime_error("There are no electrons in the event, still the fake-rate SF is != 1...");
      event.weight *= SF_ele;
      //cout << "After ele fake rate weights: " << event.weight << endl;
      SF_mu = event.get(h_FakeRateWeightMu);
      if(event.muons->size() == 0 && SF_mu != 1) throw runtime_error("There are no muons in the event, still the fake-rate SF is != 1...");
      event.weight *= SF_mu;
      //cout << "After mu fake rate weights: " << event.weight << endl;
    }
    */

    // scale variations
    if(do_scale_variation) {
      if(dataset_version.Contains("TTbar") || dataset_version.Contains("DYJets") || dataset_version.Contains("SingleTop") || dataset_version.Contains("WJets") ||  dataset_version.Contains("Diboson") || dataset_version.Contains("TTV")) {
	
	syst_module->process(event);
      }
      else if(dataset_version.Contains("LQtoT")) return false;
    }
   

    bool pass_common = common->process(event);
    if(!pass_common) return false;

    SF_eleReco->process(event);
    SF_eleID->process(event);
    SF_muonID->process(event); 
    SF_muonIso->process(event);

    fill_histograms(event,"Cleaner");


    if(!(ele_trigger_sel1->passes(event) || ele_trigger_sel2->passes(event))) return false;
    SF_eleTrigger->process(event);
    //SF_btag->process(event); // comment out when re-doing SF_btag
    fill_histograms(event,"Trigger");

    if(!nbtag_loose_sel->passes(event)) return false; // comment out when re-doing SF_btag
    fill_histograms(event,"Nbtag");

    if(!ht_lep_sel->passes(event)) return false;
    fill_histograms(event,"STlep");

    if(!inv_mass_veto->passes(event)) return false;
    fill_histograms(event,"EleInvMass");


    //h_btageff_invmass111->fill(event);



    // check for at least 1 electron pair with opposite charge
    bool charge_opposite = false;
    unsigned int nmax = event.electrons->size(); // only check first 3 electrons
    if(nmax >3) nmax=3;
    for(unsigned int i=0; i<nmax; i++){
      for(unsigned int j=0; j<nmax; j++){
        if(j>i) {
          if(event.electrons->at(i).charge() != event.electrons->at(j).charge()) {
            charge_opposite = true;
          }
        }
      }
    }


    
    // reconstruct MLQ 
    // to reconstruct the LQ mass, at least 1 extra lepton in addition to the two electrons is needed
    // if there is an additional muon and electron, the muon is used to reconstruct the LQ mass.
    bool reconstructMLQ_mu = (event.electrons->size() >= 2 && event.muons->size() >= 1 && charge_opposite); 
    bool reconstructMLQ_e = (event.electrons->size() >=3 && event.muons->size() == 0 && charge_opposite);
    // bool reconstructMLQ = reconstructMLQ_mu || reconstructMLQ_e;
    if(reconstructMLQ_mu) event.set(h_mlq_reco_mode, "muon");
    else if(reconstructMLQ_e) event.set(h_mlq_reco_mode, "ele");
    if(event.get(h_mlq_reco_mode) != "none" && event.get(h_mlq_reco_mode) != "ele" && event.get(h_mlq_reco_mode) != "muon") throw runtime_error("'h_mlq_reco_mode' contains an invalid value!");

    if(event.get(h_mlq_reco_mode) == "ele" || event.get(h_mlq_reco_mode) == "muon") {
      mlq_reco->process(event);
      chi2_module->process(event);
    }


    bool is_mlq_reconstructed = event.get(h_is_mlq_reconstructed);
    //cout << "Final event weight: " << event.weight << endl;

    fill_histograms(event,"Finalselection");

    
    //h_PDF_variations->fill(event);

    if(is_mlq_reconstructed == true)
      {
	fill_histograms(event,"Finalselection_mlqtrue");
      }
    else
      {
	fill_histograms(event,"Finalselection_mlqfalse");
		
	h_finalselection_mlqfalse->fill(event);
      }

    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the LQTopLepFullSelectionModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepFullSelectionModule)

}
