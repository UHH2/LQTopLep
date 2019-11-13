#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
//hists
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/JetHists.h"
#include "UHH2/common/include/LuminosityHists.h"
#include "UHH2/common/include/EventHists.h"
//selections
#include "UHH2/common/include/LumiSelection.h"
#include "UHH2/common/include/NSelections.h"
//IDs
#include "UHH2/common/include/ElectronIds.h"
#include "UHH2/common/include/MuonIds.h"
// my own classes
#include "UHH2/LQTopLep/include/LQTopLepSelections.h"
#include "UHH2/LQTopLep/include/LQTopLepHists.h"
//#include "UHH2/LQTopLep/include/LQTopLepModules.h"


using namespace std;
using namespace uhh2;

namespace uhh2examples {

  /** \brief Basic analysis example of an AnalysisModule (formerly 'cycle') in UHH2
   * 
   * This is the central class which calls other AnalysisModules, Hists or Selection classes.
   * This AnalysisModule, in turn, is called (via AnalysisModuleRunner) by SFrame.
   */
  class LQTopLepPreselectionModule: public AnalysisModule {
  public:
    
    explicit LQTopLepPreselectionModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    
    std::unique_ptr<CommonModules> common;
    
    std::unique_ptr<JetCleaner> jetcleaner;
    std::unique_ptr<ElectronCleaner> electroncleaner;
    unique_ptr<MuonCleaner> muoncleaner;

    //unique_ptr<ElectronFakeRateWeights> SF_eleFakeRate;
    //unique_ptr<MuonFakeRateWeights> SF_muFakeRate;

   
    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor,
    // to avoid memory leaks.
    std::unique_ptr<Selection> njet_sel, nele_sel, dijet_sel, lumi_sel, ht_sel, ele_trigger_sel, mu_trigger_sel;
    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_nocuts, h_cleaner, h_2jets, h_2lep, h_st350;

    ElectronId eleId;
    JetId jetId;
    MuonId muId;
    
    bool is_mc;
    TString Sys_EleFakeRate, path_EleFakeRate, Sys_MuFakeRate, path_MuFakeRate;


    uhh2::Event::Handle<vector<Jet>> h_raw_jets_ele;
    uhh2::Event::Handle<vector<Particle>> h_raw_genjets_ele;


    uhh2::Event::Handle<double> FakeRateWeightEle;
    uhh2::Event::Handle<double> FakeRateWeightEleUp;
    uhh2::Event::Handle<double> FakeRateWeightEleDown;
    uhh2::Event::Handle<double> FakeRateWeightMu;
    uhh2::Event::Handle<double> FakeRateWeightMuUp;
    uhh2::Event::Handle<double> FakeRateWeightMuDown;
  };


  LQTopLepPreselectionModule::LQTopLepPreselectionModule(Context & ctx){
    // In the constructor, the typical tasks are to initialize the
    // member variables, in particular the AnalysisModules such as
    // CommonModules or some cleaner module, Selections and Hists.
    // But you can do more and e.g. access the configuration, as shown below.
    
    cout << "Hello World from LQTopLepPreselectionModule!" << endl;
    
    // If needed, access the configuration of the module here, e.g.:
    string testvalue = ctx.get("TestKey", "<not set>");
    cout << "TestKey in the configuration was: " << testvalue << endl;
    
    // If running in SFrame, the keys "dataset_version", "dataset_type", "dataset_lumi",
    // and "target_lumi" are set to the according values in the xml file. For CMSSW, these are
    // not set automatically, but can be set in the python config file.
    for(auto & kv : ctx.get_all()){
      cout << " " << kv.first << " = " << kv.second << endl;
    }
    
    is_mc = ctx.get("dataset_type") == "MC";
    path_EleFakeRate = ctx.get("path_EleFakeRate");
    Sys_EleFakeRate = ctx.get("Systematic_EleFakeRate"); 
    path_MuFakeRate = ctx.get("path_MuFakeRate");
    Sys_MuFakeRate = ctx.get("Systematic_MuFakeRate");

    
    // ERROR: muonID for CMSSW10
    eleId = AndId<Electron>(ElectronID_Summer16_tight, PtEtaCut(30.0, 2.4)); // pT>= 30 GeV, |nu|<2.4    
    Year year = extract_year(ctx);
    if (year == Year::is2016v2) muId = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(30.0, 2.4), MuonIso(0.15)); // loose for LQ->te, maybe change to tight at some point
    else                        muId = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(30.0, 2.4), MuonIso(0.15)); // loose for LQ->te, maybe change to tight at some point
    cout << __LINE__ << endl;


    // 1. setup other modules. CommonModules and the JetCleaner:
    common.reset(new CommonModules());
    common->switch_jetlepcleaner(true); // so that momentum from Leptons in Jets are not used twice
    common->set_electron_id(eleId); // electrons which don't fulfill ID are removed from the event
    common->set_muon_id(muId); // muons which don't fulfill ID are removed from the event
    common->set_jet_id(PtEtaCut(30.0, 2.4)); // Jets with pT<30 GeV or |nu|>2.4 are removed from the event

    common->init(ctx);
    
    electroncleaner.reset(new ElectronCleaner(eleId));
    muoncleaner.reset(new MuonCleaner(muId));


    h_raw_jets_ele = ctx.get_handle<vector<Jet>>("RawJetsEle");
    h_raw_genjets_ele = ctx.get_handle<vector<Particle>>("RawGenJetsEle");
    //h_raw_jets_mu = ctx.get_handle<vector<Jet>>("RawJetsMu");
    //h_raw_genjets_mu = ctx.get_handle<vector<Particle>>("RawGenJetsMu");


    //SF_eleFakeRate.reset(new ElectronFakeRateWeights(ctx, JERFiles::Summer16_23Sep2016_V4_L123_AK4PFchs_MC, path_EleFakeRate, Sys_EleFakeRate, "RawJetsEle", "RawGenJetsEle"));    
    //SF_muFakeRate.reset(new MuonFakeRateWeights(ctx, path_MuFakeRate, Sys_MuFakeRate));



    // 2. set up selections
    njet_sel.reset(new NJetSelection(2, -1)); // at least 2 Jets, see common/include/NSelections.h
    nele_sel.reset(new NElectronSelection(2, -1)); // at leats 2 electrons
    ht_sel.reset(new HtSelection(350)); // ST>= 350 GeV, possibly called StSelection

    dijet_sel.reset(new DijetSelection()); // see LQTopLepSelections, not sure if needed at all
    lumi_sel.reset(new LumiSelection(ctx)); // only events used with 'good working detectors'


    // 3. Set up Hists classes:

    h_nocuts.reset(new LQTopLepHists(ctx, "NoCuts"));
    h_cleaner.reset(new LQTopLepHists(ctx, "Cleaner"));
    h_2jets.reset(new LQTopLepHists(ctx, "2Jets"));
    h_2lep.reset(new LQTopLepHists(ctx, "2Leptons"));
    h_st350.reset(new LQTopLepHists(ctx, "ST350"));


    cout << __LINE__ << endl;

    //FakeRateWeightEle = ctx.declare_event_output<double>("FakeRateWeightEle");
    //FakeRateWeightEleUp = ctx.declare_event_output<double>("FakeRateWeightEleUp");
    //FakeRateWeightEleDown = ctx.declare_event_output<double>("FakeRateWeightEleDown");
    //FakeRateWeightMu = ctx.declare_event_output<double>("FakeRateWeightMu");
    //FakeRateWeightMuUp = ctx.declare_event_output<double>("FakeRateWeightMuUp");
    //FakeRateWeightMuDown = ctx.declare_event_output<double>("FakeRateWeightMuDown");
    cout << __LINE__ << endl;


  }


  bool LQTopLepPreselectionModule::process(Event & event) {
    // This is the main procedure, called for each event. Typically,
    // do some pre-processing by calling the modules' process method
    // of the modules constructed in the constructor (1).
    // Then, test whether the event passes some selection and -- if yes --
    // use it to fill the histograms (2).
    // Finally, decide whether or not to keep the event in the output (3);
    // this is controlled by the return value of this method: If it
    // returns true, the event is kept; if it returns false, the event
    // is thrown away.
    
    //cout << "LQTopLepPreselectionModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    
    // 1. run all modules other modules.
    // Note that it returns a bool, that may be False
    // (e.g. Golden JSON, MET filters), and therefore user should return early
    //bool commonResult = common->process(event);
    //if (!commonResult) return false;
    //jetcleaner->process(event);

    // 2. test selections and fill histograms
    //h_ele->fill(event);
    //h_nocuts->fill(event);
    



    // Lumi selection
    if(!is_mc) {
      if(!lumi_sel->passes(event)) return false;
    }
    h_nocuts->fill(event);

    /*
    // apply fake rate weights for electrons and muons
    if(is_mc) {

    //electrons
    vector<Jet> raw_jets;
    for(unsigned int i=0; i<event.jets->size(); i++){
    Jet jet = event.jets->at(i);
    raw_jets.push_back(jet);
    }
    event.set(h_raw_jets_ele,move(raw_jets));

    vector<Particle> gen_jets;
    for(unsigned int i=0; i<event.genjets->size(); i++){
    Particle jet = event.genjets->at(i);
    gen_jets.push_back(jet);
    }
    event.set(h_raw_genjets_ele,move(gen_jets));


    electroncleaner->process(event);
    SF_eleFakeRate->process(event);

    //muons
    muoncleaner->process(event);
    SF_muFakeRate->process(event);

    }
    else{
    event.set(FakeRateWeightEle,1.);
    event.set(FakeRateWeightEleUp,1.);
    event.set(FakeRateWeightEleDown,1.);
    event.set(FakeRateWeightMu,1.);
    event.set(FakeRateWeightMuUp,1.);
    event.set(FakeRateWeightMuDown,1.);
    }
    */


    bool pass_common = common->process(event); // check if event fulfills common criteria


    if(!pass_common) return false;
    h_cleaner->fill(event);

    if(!njet_sel->passes(event)) return false; // N_Jet>=2
    h_2jets->fill(event);

    if(!nele_sel->passes(event)) return false; // N_el>=2
    h_2lep->fill(event);

    if(!ht_sel->passes(event)) return false; // ST>=350 GeV
    h_st350->fill(event);



    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the LQTopLepPreselectionModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepPreselectionModule)

}
