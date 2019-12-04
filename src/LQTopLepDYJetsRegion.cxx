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

  class LQTopLepDYJetsRegion: public AnalysisModule {
  public:
    
    explicit LQTopLepDYJetsRegion(Context & ctx); // constructor
    virtual bool process(Event & event) override; 

  private:
    
    std::unique_ptr<CommonModules> common;
    
    std::unique_ptr<JetCleaner> jetcleaner;
    std::unique_ptr<ElectronCleaner> electroncleaner;

    unique_ptr<AnalysisModule> SF_eleReco, SF_eleID, SF_muonIso, SF_muonID, SF_btag, SF_btagMedium;
    std::unique_ptr<AnalysisModule> syst_module; // for renormalization and factorization scales mu_r, mu_f
    unique_ptr<ElectronTriggerWeights> SF_eleTrigger;
   
    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor,
    // to avoid memory leaks.
    std::unique_ptr<Selection> nbtag_loose_sel, ht_lep_sel, inv_mass_veto, ele_trigger_sel1, ele_trigger_sel2, mu_trigger_sel, exactly2ele_sel, exactly0mu_sel, atLeast3ele_sel, atLeast1mu_sel, ht_sel, invmass110_sel;
    // mu trigger not used
    
    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    // std::unique_ptr<Hists> h_nocuts, h_njet, h_dijet, h_ele;

    std::unique_ptr<Hists> h_nocuts, h_jets_nocuts, h_ele_nocuts, h_mu_nocuts, h_event_nocuts, h_topjets_nocuts, h_lumi_nocuts, 
      h_trigger, h_jets_trigger, h_ele_trigger, h_mu_trigger, h_event_trigger, h_topjets_trigger, h_lumi_trigger, 
      h_nbtag, h_jets_nbtag, h_ele_nbtag, h_mu_nbtag, h_event_nbtag, h_topjets_nbtag, h_lumi_nbtag,
      h_htlep200, h_jets_htlep200, h_ele_htlep200, h_mu_htlep200, h_event_htlep200, h_topjets_htlep200, h_lumi_htlep200,
      h_invmass, h_jets_invmass, h_ele_invmass, h_mu_invmass, h_event_invmass, h_topjets_invmass, h_lumi_invmass, h_btageff;


    std::unique_ptr<Hists> h_exactly2e, h_jets_exactly2e, h_ele_exactly2e, h_mu_exactly2e, h_event_exactly2e, h_topjets_exactly2e, h_lumi_exactly2e;

    std::unique_ptr<Hists> h_2eAndExtraLep, h_jets_2eAndExtraLep, h_ele_2eAndExtraLep, h_mu_2eAndExtraLep, h_event_2eAndExtraLep, h_topjets_2eAndExtraLep, h_lumi_2eAndExtraLep;
    unique_ptr<Hists> h_finalselection, h_jets_finalselection, h_ele_finalselection, h_mu_finalselection, h_event_finalselection, h_lumi_finalselection;
    unique_ptr<Hists> h_finalselection_mlqtrue, h_jets_finalselection_mlqtrue, h_ele_finalselection_mlqtrue, h_mu_finalselection_mlqtrue, h_event_finalselection_mlqtrue, h_lumi_finalselection_mlqtrue;
    unique_ptr<Hists> h_finalselection_mlqfalse, h_jets_finalselection_mlqfalse, h_ele_finalselection_mlqfalse, h_mu_finalselection_mlqfalse, h_event_finalselection_mlqfalse, h_lumi_finalselection_mlqfalse;
    unique_ptr<Hists> h_PDF_variations;
    unique_ptr<Hists> h_invmass110;



    unique_ptr<HighMassInclusiveLQReconstruction> mlq_reco;
    //unique_ptr<HighMassInclusiveLQReconstruction> mlq_reco_test;

    unique_ptr<LQChi2Discriminator> chi2_module;
    
    JetId Btag_loose;

    BTag::algo btag_algo;
    BTag::wp wp_btag_loose, wp_btag_medium, wp_btag_tight;
    //CSVBTag::wp wp_btag_loose;

    bool is_mc, exactly2lep, do_permutations, do_scale_variation, do_pdf_variation;
    bool onlySelfmadePlots;
    string s_permutation;
    uhh2::Event::Handle<bool> h_is_mlq_reconstructed;
    uhh2::Event::Handle<TString> h_mlq_reco_mode;

    // systematic errors
    string Sys_EleID, Sys_EleReco, Sys_EleTrigger;
    string Sys_MuonID, Sys_MuonIso, Sys_MuonTrigger;
    string Sys_BTag;
    string Sys_PU;

    TString dataset_version;

    string Sys_EleFake, Sys_MuonFake;
    //uhh2::Event::Handle<double> h_FakeRateWeightEle, h_FakeRateWeightMu;

  };


  LQTopLepDYJetsRegion::LQTopLepDYJetsRegion(Context & ctx){
    // In the constructor, the typical tasks are to initialize the
    // member variables, in particular the AnalysisModules such as
    // CommonModules or some cleaner module, Selections and Hists.
    // But you can do more and e.g. access the configuration, as shown below.



    cout << "Hello World from LQTopLepDYJetsRegion!" << endl;
    for(auto & kv : ctx.get_all()){
      cout << " " << kv.first << " = " << kv.second << endl;
    }
    cout << __LINE__ << endl; 

    // set to true to only make selfmade plots
    onlySelfmadePlots = true; //does not work for false, but I dont really care


    h_is_mlq_reconstructed = ctx.get_handle<bool>("is_mlq_reconstructed");
    h_mlq_reco_mode = ctx.get_handle<TString>("mlq_reco_mode");


    is_mc = ctx.get("dataset_type") == "MC";

    do_permutations = ctx.get("Do_Permutations") == "true";
    s_permutation = ctx.get("Permutation");
    int permutation = stoi(s_permutation, nullptr, 10);
    cout << "permutation: " << permutation << endl;
    if (permutation < 0 && do_permutations) throw runtime_error("Invalid value for permutation");
    
    //Btag_loose = CSVBTag(CSVBTag::WP_LOOSE);
    //wp_btag_loose = CSVBTag::WP_LOOSE;

    btag_algo = BTag::DEEPJET;
    wp_btag_loose = BTag::WP_LOOSE;
    wp_btag_medium = BTag::WP_MEDIUM;
    wp_btag_tight = BTag::WP_TIGHT;

    JetId DeepjetLoose = BTag(btag_algo, wp_btag_loose);
    JetId DeepjetMedium = BTag(btag_algo, wp_btag_medium);
    JetId DeepjetTight = BTag(btag_algo, wp_btag_tight);
    
    int btag_value = 1;
    double stlep_value = 0.;
    double mee_max_value = 110; // should not be Mee>110 GeV
    double mee_min_value = 72;
    if(do_permutations){
      //vector<int> btag_values = {0, 1};
      //vector<double> mee_max_values = {110., 110., 110., 110., 110.};
      //vector<double> mee_min_values = {72., 60., 50., 40., 30.};
      //vector<double> stlep_values = {0.};
      vector<int> btag_values = {0, 1, 2};
      vector<double> mee_max_values = {110., 110., 110.};
      vector<double> mee_min_values = {72., 60., 50.};
      vector<double> stlep_values = {0., 100., 200.};
    
      int btag_idx = permutation % btag_values.size();
      int mee_idx = (permutation / btag_values.size()) % mee_min_values.size();
      int stlep_idx = (permutation / btag_values.size() / mee_min_values.size()) % stlep_values.size();

      btag_value = btag_values[btag_idx];
      //mee_max_value = mee_max_values[mee_idx];
      mee_min_value = mee_min_values[mee_idx];
      stlep_value = stlep_values[stlep_idx];
    }
    
    
    // electron triggers
    ele_trigger_sel1.reset(new TriggerSelection("HLT_Ele27_WPTight_Gsf_v*"));
    ele_trigger_sel2.reset(new TriggerSelection("HLT_Ele115_CaloIdVT_GsfTrkIdT_v*"));

    cout << __LINE__ << endl; 


    // initialize LQ reconstruction modules
    mlq_reco.reset(new HighMassInclusiveLQReconstruction(ctx, LQNeutrinoReconstruction));
    //mlq_reco_test.reset(new HighMassInclusiveLQReconstruction(ctx, LQNeutrinoReconstructionOld));
    chi2_module.reset(new LQChi2Discriminator(ctx, "LQHypotheses"));


    // systematic errors
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

    cout << __LINE__ << endl; 



    // Scale Factors for MonteCarlo
   // SFs for trigger: have to be remade
   SF_eleTrigger.reset(new ElectronTriggerWeights(ctx, "/nfs/dust/cms/user/reimersa/LQToTopMu/Run2_80X_v3/TagProbe/Optimization/35867fb_Iso27_NonIso115/ElectronEfficiencies.root", Sys_EleTrigger)); 

   // SFs for Id, Reco, Iso: copied from SingleTthAnalysisModule.cxx
 
    SF_muonID.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonID_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightID_DEN_genTracks_eta_pt", 0., "id", false, Sys_MuonID));
    SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonIso_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt", 0., "iso", false, Sys_MuonIso));

    SF_eleReco.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root", 1, "reco", Sys_EleReco));
    SF_eleID.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/frahmmat/CMSnew/CMSSW_10_2_10/src/UHH2/common/data/2016/2016LegacyReReco_ElectronTight_Fall17V2.root", 1, "id", Sys_EleID));






    //SF_btag.reset(new MCBTagScaleFactor(ctx,wp_btag_loose,"jets", Sys_BTag)); // comment out when re-doing SF_btag

    common.reset(new CommonModules());
    common->disable_lumisel();
    common->disable_jersmear();
    common->disable_jec();
    // common->switch_jetlepcleaner(true); // so that momentum from Leptons in Jets are not used twice
    common->init(ctx, Sys_PU); // (ctx, Sys_PU) 
    
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
   cout << __LINE__ << endl; 
    
   // 2. set up selections
    
   /*
     njet_sel.reset(new NJetSelection(2, -1)); // at least 2 Jets, see common/include/NSelections.h
     nele_sel.reset(new NElectronSelection(2, -1)); // at leats 2 electrons
     ht_sel.reset(new HtSelection(350)); // ST>= 350 GeV, possibly called StSelection

     dijet_sel.reset(new DijetSelection()); // see LQTopLepSelections, not sure if needed at all
     lumi_sel.reset(new LumiSelection(ctx)); // only events used with 'good working detectors'
   */
    
   invmass110_sel.reset(new InvMassEleEleSelection(1, 110));
   nbtag_loose_sel.reset(new NJetSelection(btag_value, -1, DeepjetLoose));
   //nbtag_loose_sel.reset(new NJetSelection(btag_value, -1, Btag_loose)); // >=1 b-Jet
   ht_lep_sel.reset(new HtLepSelection(stlep_value, -1)); // returns true if Ht>200
   inv_mass_veto.reset(new InvMassEleEleSelection(mee_min_value, mee_max_value));

   exactly2ele_sel.reset(new NElectronSelection(2, 2));
   exactly0mu_sel.reset(new NMuonSelection(0, 0));

   syst_module.reset(new MCScaleVariation(ctx));
   

   cout << __LINE__ << endl; 


   // 3. Set up Hists classes:




   h_nocuts.reset(new LQTopLepFullSelectionHists(ctx, "NoCuts"));

   h_trigger.reset(new LQTopLepFullSelectionHists(ctx, "Trigger"));

   h_invmass110.reset(new LQTopLepFullSelectionHists(ctx, "invmass110"));

   h_nbtag.reset(new LQTopLepFullSelectionHists(ctx, "Nbtag"));

   h_htlep200.reset(new LQTopLepFullSelectionHists(ctx, "htlep200"));

   h_invmass.reset(new LQTopLepFullSelectionHists(ctx, "M_ee_invmass"));

   //h_btageff.reset(new BTagMCEfficiencyHists(ctx, "BTagEff",wp_btag_loose));

   //final selection
   h_finalselection.reset(new LQTopLepFullSelectionHists(ctx, "finalselection"));

   h_finalselection_mlqtrue.reset(new LQTopLepFullSelectionHists(ctx, "finalselection_mlqtrue"));

   h_finalselection_mlqfalse.reset(new LQTopLepFullSelectionHists(ctx, "finalselection_mlqfalse"));
   cout << __LINE__ << endl; 

   h_PDF_variations.reset(new LQTopLepPDFHists(ctx, "PDF_variations", true, do_pdf_variation));

   if(!onlySelfmadePlots) {
     h_jets_nocuts.reset(new JetHists(ctx, "Jets_NoCuts"));
     h_ele_nocuts.reset(new ElectronHists(ctx, "Ele_NoCuts"));
     h_mu_nocuts.reset(new MuonHists(ctx, "Mu_NoCuts"));
     h_event_nocuts.reset(new EventHists(ctx, "Event_NoCuts"));
     h_topjets_nocuts.reset(new TopJetHists(ctx, "Topjets_NoCuts"));
     h_lumi_nocuts.reset(new LuminosityHists(ctx, "Lumi_NoCuts"));

     h_jets_trigger.reset(new JetHists(ctx, "Jets_Trigger"));
     h_ele_trigger.reset(new ElectronHists(ctx, "Ele_Trigger"));
     h_mu_trigger.reset(new MuonHists(ctx, "Mu_Trigger"));
     h_event_trigger.reset(new EventHists(ctx, "Event_Trigger"));
     h_topjets_trigger.reset(new TopJetHists(ctx, "Topjets_Trigger"));
     h_lumi_trigger.reset(new LuminosityHists(ctx, "Lumi_Trigger"));

     h_jets_nbtag.reset(new JetHists(ctx, "Jets_Nbtag"));
     h_ele_nbtag.reset(new ElectronHists(ctx, "Ele_Nbtag"));
     h_mu_nbtag.reset(new MuonHists(ctx, "Mu_Nbtag"));
     h_event_nbtag.reset(new EventHists(ctx, "Event_Nbtag"));
     h_topjets_nbtag.reset(new TopJetHists(ctx, "Topjets_Nbtag"));
     h_lumi_nbtag.reset(new LuminosityHists(ctx, "Lumi_Nbtag"));

     h_jets_htlep200.reset(new JetHists(ctx, "Jets_htlep200"));
     h_ele_htlep200.reset(new ElectronHists(ctx, "Ele_htlep200"));
     h_mu_htlep200.reset(new MuonHists(ctx, "Mu_htlep200"));
     h_event_htlep200.reset(new EventHists(ctx, "Event_htlep200"));
     h_topjets_htlep200.reset(new TopJetHists(ctx, "Topjets_htlep200"));
     h_lumi_htlep200.reset(new LuminosityHists(ctx, "Lumi_htlep200"));

     h_jets_invmass.reset(new JetHists(ctx, "Jets_M_ee_invmass"));
     h_ele_invmass.reset(new ElectronHists(ctx, "Ele_M_ee_invmass"));
     h_mu_invmass.reset(new MuonHists(ctx, "Mu_M_ee_invmass"));
     h_event_invmass.reset(new EventHists(ctx, "Event_M_ee_invmass"));
     h_topjets_invmass.reset(new TopJetHists(ctx, "Topjets_M_ee_invmass"));
     h_lumi_invmass.reset(new LuminosityHists(ctx, "Lumi_M_ee_invmass"));

     h_jets_finalselection.reset(new JetHists(ctx, "Jets_finalselection"));
     h_ele_finalselection.reset(new ElectronHists(ctx, "Ele_finalselection"));
     h_mu_finalselection.reset(new MuonHists(ctx, "Mu_finalselection"));
     h_event_finalselection.reset(new EventHists(ctx, "Event_finalselection"));
     h_lumi_finalselection.reset(new LuminosityHists(ctx, "Lumi_finalselection"));

     h_jets_finalselection_mlqtrue.reset(new JetHists(ctx, "Jets_finalselection_mlqtrue"));
     h_ele_finalselection_mlqtrue.reset(new ElectronHists(ctx, "Ele_finalselection_mlqtrue"));
     h_mu_finalselection_mlqtrue.reset(new MuonHists(ctx, "Mu_finalselection_mlqtrue"));
     h_event_finalselection_mlqtrue.reset(new EventHists(ctx, "Event_finalselection_mlqtrue"));
     h_lumi_finalselection_mlqtrue.reset(new LuminosityHists(ctx, "Lumi_finalselection_mlqtrue"));

     h_jets_finalselection_mlqfalse.reset(new JetHists(ctx, "Jets_finalselection_mlqfalse"));
     h_ele_finalselection_mlqfalse.reset(new ElectronHists(ctx, "Ele_finalselection_mlqfalse"));
     h_mu_finalselection_mlqfalse.reset(new MuonHists(ctx, "Mu_finalselection_mlqfalse"));
     h_event_finalselection_mlqfalse.reset(new EventHists(ctx, "Event_finalselection_mlqfalse"));
     h_lumi_finalselection_mlqfalse.reset(new LuminosityHists(ctx, "Lumi_finalselection_mlqfalse"));
   }



    /*
    h_exactly2e.reset(new LQTopLepFullSelectionHists(ctx, "2_Electrons"));
    h_jets_exactly2e.reset(new JetHists(ctx, "Jets_2_Electrons"));
    h_ele_exactly2e.reset(new ElectronHists(ctx, "Ele_2_Electrons"));
    h_mu_exactly2e.reset(new MuonHists(ctx, "Mu_2_Electrons"));
    h_event_exactly2e.reset(new EventHists(ctx, "Event_2_Electrons"));
    h_topjets_exactly2e.reset(new TopJetHists(ctx, "Topjets_2_Electrons"));
    h_lumi_exactly2e.reset(new LuminosityHists(ctx, "Lumi_2_Electrons"));

    h_2eAndExtraLep.reset(new LQTopLepFullSelectionHists(ctx, "2eAndExtraLep"));
    h_jets_2eAndExtraLep.reset(new JetHists(ctx, "Jets_2eAndExtraLep"));
    h_ele_2eAndExtraLep.reset(new ElectronHists(ctx, "Ele_2eAndExtraLep"));
    h_mu_2eAndExtraLep.reset(new MuonHists(ctx, "Mu_2eAndExtraLep"));
    h_event_2eAndExtraLep.reset(new EventHists(ctx, "Event_2eAndExtraLep"));
    h_topjets_2eAndExtraLep.reset(new TopJetHists(ctx, "Topjets_2eAndExtraLep"));
    h_lumi_2eAndExtraLep.reset(new LuminosityHists(ctx, "Lumi_2eAndExtraLep"));
    */

    cout << __LINE__ << endl; 



  }


  bool LQTopLepDYJetsRegion::process(Event & event) {
    // This is the main procedure, called for each event. Typically,
    // do some pre-processing by calling the modules' process method
    // of the modules constructed in the constructor (1).
    // Then, test whether the event passes some selection and -- if yes --
    // use it to fill the histograms (2).
    // Finally, decide whether or not to keep the event in the output (3);
    // this is controlled by the return value of this method: If it
    // returns true, the event is kept; if it returns false, the event
    // is thrown away.
    
    // cout << "LQTopLepDYJetsRegion: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    
    /*
    if(event.muons->size() >=1) {
      cout << "my NeutrinoReconstruction: " << LQNeutrinoReconstruction(event.muons->at(0).v4(), event.met->v4()) << endl;
      cout << "old NeutrinoReconstruction: " << LQNeutrinoReconstruction(event.muons->at(0).v4(), event.met->v4()) << endl;
    }
    */    

    // // 1. run all modules other modules.
    // common->process(event);
    // jetcleaner->process(event);
    
    // // 2. test selections and fill histograms

    cout << __LINE__ << endl; 

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
      SF_mu = event.get(h_FakeRateWeightMu);
      if(event.muons->size() == 0 && SF_mu != 1) throw runtime_error("There are no muons in the event, still the fake-rate SF is != 1...");
      event.weight *= SF_mu;
    }
    */
    
    // scale variations
    if(do_scale_variation) {
      if(dataset_version.Contains("TTbar") || dataset_version.Contains("DYJets") || dataset_version.Contains("SingleTop") || dataset_version.Contains("WJets") ||  dataset_version.Contains("Diboson") || dataset_version.Contains("TTV")) {
	
	syst_module->process(event);
      }
      else if(dataset_version.Contains("LQtoT")) return false;
    }
   
    cout << __LINE__ << endl; 

    common->process(event);

    SF_eleReco->process(event);
    SF_eleID->process(event);

    SF_muonID->process(event);
    SF_muonIso->process(event);


    h_nocuts->fill(event);
    if(!onlySelfmadePlots) {
      h_jets_nocuts->fill(event);
      h_ele_nocuts->fill(event);
      h_mu_nocuts->fill(event);
      h_event_nocuts->fill(event);
      h_topjets_nocuts->fill(event);
      h_lumi_nocuts->fill(event);
    }
    cout << __LINE__ << endl; 


    if(!(ele_trigger_sel1->passes(event) || ele_trigger_sel2->passes(event))) return false;
    SF_eleTrigger->process(event);
    //SF_btag->process(event); // comment out when re-doing SF_btag

    h_trigger->fill(event);
    if(!onlySelfmadePlots) {
      h_jets_trigger->fill(event);
      h_ele_trigger->fill(event);
      h_mu_trigger->fill(event);
      h_event_trigger->fill(event);
      h_topjets_trigger->fill(event);
      h_lumi_trigger->fill(event);
    }

    if(!invmass110_sel->passes(event)) return false;
    h_invmass110->fill(event);

    if(!inv_mass_veto->passes(event)) return false;

    h_invmass->fill(event);
    if(!onlySelfmadePlots) {
      h_jets_invmass->fill(event);
      h_ele_invmass->fill(event);
      h_mu_invmass->fill(event);
      h_event_invmass->fill(event);
      h_topjets_invmass->fill(event);
      h_lumi_invmass->fill(event);
    }

    //if(!nbtag_loose_sel->passes(event)) return false; // comment out when re-doing SF_btag

    h_nbtag->fill(event);
    if(!onlySelfmadePlots) {
      h_jets_nbtag->fill(event);
      h_ele_nbtag->fill(event);
      h_mu_nbtag->fill(event);
      h_event_nbtag->fill(event);
      h_topjets_nbtag->fill(event);
      h_lumi_nbtag->fill(event);
    }

    if(!ht_lep_sel->passes(event)) return false;

    h_htlep200->fill(event);
    if(!onlySelfmadePlots) {
      h_jets_htlep200->fill(event);
      h_ele_htlep200->fill(event);
      h_mu_htlep200->fill(event);
      h_event_htlep200->fill(event);
      h_topjets_htlep200->fill(event);
      h_lumi_htlep200->fill(event);
    }

    //h_btageff->fill(event);

    cout << __LINE__ << endl; 


    exactly2lep = (exactly2ele_sel->passes(event) && exactly0mu_sel->passes(event));
    /*
    // Histograms for case 2 electrons, 0 muons
    if(exactly2lep) {
    h_exactly2e->fill(event);
    h_jets_exactly2e->fill(event);
    h_ele_exactly2e->fill(event);
    h_mu_exactly2e->fill(event);
    h_event_exactly2e->fill(event);
    h_topjets_exactly2e->fill(event);
    h_lumi_exactly2e->fill(event);
    }

    // Histograms for case 2 electrons + extra muons or electrons
    if(!exactly2lep) {
    h_2eAndExtraLep->fill(event);
    h_jets_2eAndExtraLep->fill(event);
    h_ele_2eAndExtraLep->fill(event);
    h_mu_2eAndExtraLep->fill(event);
    h_event_2eAndExtraLep->fill(event);
    h_topjets_2eAndExtraLep->fill(event);
    h_lumi_2eAndExtraLep->fill(event);
    }
    */


    // check for at least 1 electron pair with opposite charge
    bool charge_opposite = false;
    unsigned int nmax = event.electrons->size(); // only check first 3 electrons
    if(nmax >3) nmax=3;
    for(unsigned int i=0; i<nmax; i++){
      for(unsigned int j=0; j<nmax; j++){
        if(j>i){
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

    h_finalselection->fill(event);
    if(!onlySelfmadePlots) {
      h_jets_finalselection->fill(event);
      h_ele_finalselection->fill(event);
      h_mu_finalselection->fill(event);
      h_event_finalselection->fill(event);
      h_lumi_finalselection->fill(event);
    }
    cout << __LINE__ << endl; 

    h_PDF_variations->fill(event);
    cout << __LINE__ << endl; 

    if(is_mlq_reconstructed == true)
      {
	h_finalselection_mlqtrue->fill(event);
	if(!onlySelfmadePlots) {
	  h_jets_finalselection_mlqtrue->fill(event);
	  h_ele_finalselection_mlqtrue->fill(event);
	  h_mu_finalselection_mlqtrue->fill(event);
	  h_event_finalselection_mlqtrue->fill(event);
	  h_lumi_finalselection_mlqtrue->fill(event);
	}
      }
    else
      {
	h_finalselection_mlqfalse->fill(event);
	if(!onlySelfmadePlots) {
	  h_jets_finalselection_mlqfalse->fill(event);
	  h_ele_finalselection_mlqfalse->fill(event);
	  h_mu_finalselection_mlqfalse->fill(event);
	  h_event_finalselection_mlqfalse->fill(event);
	  h_lumi_finalselection_mlqfalse->fill(event);
	}
      }


    // 3. decide whether or not to keep the current event in the output:
    return true;
   }

   // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
   // make sure the LQTopLepDYJetsRegion is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepDYJetsRegion)

}
