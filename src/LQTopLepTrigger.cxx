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
#include "UHH2/common/include/TriggerSelection.h"
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
  class LQTopLepTrigger: public AnalysisModule {
  public:
    
    explicit LQTopLepTrigger(Context & ctx);
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
    std::unique_ptr<Selection> nele_sel, nmuons_sel, njets_sel, lumi_sel, ele_trigger_sel1, ele_trigger_sel2, mu_trigger_sel1, mu_trigger_sel2;
    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_nocuts, h_cleaner, h_2jets, h_1e, h_1mu, h_mu_trigger, h_ele_trigger;

    ElectronId eleId;
    JetId jetId;
    MuonId muId;
    
    bool is_mc;


    uhh2::Event::Handle<vector<Jet>> h_raw_jets_ele;
    uhh2::Event::Handle<vector<Particle>> h_raw_genjets_ele;


  };


  LQTopLepTrigger::LQTopLepTrigger(Context & ctx){
    
    cout << "Hello World from LQTopLepTrigger!" << endl;
    
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

    // e and mu tight
    eleId = AndId<Electron>(ElectronID_Summer16_tight, PtEtaCut(10.0, 2.4)); // pT>= 10 GeV, |nu|<2.4    
    Year year = extract_year(ctx);
    if (year == Year::is2016v2) muId = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(30.0, 2.4), MuonIso(0.15));
    else                        muId = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(30.0, 2.4), MuonIso(0.15));
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




    // 2. set up selections
    nele_sel.reset(new NElectronSelection(1,1)); // 1e pT>10, |eta| < 2.4
    nmuons_sel.reset(new NMuonSelection(1,1)); // 1mu pT>30, |eta| < 2.4
    njets_sel.reset(new NJetSelection(2, -1));

    lumi_sel.reset(new LumiSelection(ctx)); // only events used with 'good working detectors'

    // muon trigger
    mu_trigger_sel1.reset(new TriggerSelection("HLT_IsoMu24_v*"));
    mu_trigger_sel2.reset(new TriggerSelection("HLT_IsoTkMu24_v*"));

    // electron triggers
    ele_trigger_sel1.reset(new TriggerSelection("HLT_Ele27_WPTight_Gsf_v*"));
    ele_trigger_sel2.reset(new TriggerSelection("HLT_Ele115_CaloIdVT_GsfTrkIdT_v*"));


    // 3. Set up Hists classes:

    h_nocuts.reset(new LQTopLepHists(ctx, "NoCuts"));
    h_cleaner.reset(new LQTopLepHists(ctx, "Cleaner"));
    h_2jets.reset(new LQTopLepHists(ctx, "2Jets"));
    h_1e.reset(new LQTopLepHists(ctx, "1Electron"));
    h_1mu.reset(new LQTopLepHists(ctx, "1Muon"));
    h_mu_trigger.reset(new LQTopLepHists(ctx, "MuonTrigger"));
    h_ele_trigger.reset(new LQTopLepHists(ctx, "EleTrigger"));




  }


  bool LQTopLepTrigger::process(Event & event) {
    // This is the main procedure, called for each event. Typically,
    // do some pre-processing by calling the modules' process method
    // of the modules constructed in the constructor (1).
    // Then, test whether the event passes some selection and -- if yes --
    // use it to fill the histograms (2).
    // Finally, decide whether or not to keep the event in the output (3);
    // this is controlled by the return value of this method: If it
    // returns true, the event is kept; if it returns false, the event
    // is thrown away.
    
    //cout << "LQTopLepTrigger: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    
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

    


    bool pass_common = common->process(event); // check if event fulfills common criteria


    if(!pass_common) return false;
    h_cleaner->fill(event);

    if(!njets_sel->passes(event)) return false; // N_Jet>=2
    h_2jets->fill(event);

    if(!nele_sel->passes(event)) return false; // N_el=1
    h_1e->fill(event);

    if(!nmuons_sel->passes(event)) return false; // N_mu=1


    if(!(mu_trigger_sel1->passes(event) || mu_trigger_sel2->passes(event))) return false;  
    h_mu_trigger->fill(event);
    // #events that pass selection

    if(!(ele_trigger_sel1->passes(event) || ele_trigger_sel2->passes(event))) return false;
    h_ele_trigger->fill(event);
    // #events that pass trigger+selection

    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the LQTopLepTrigger is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepTrigger)

}
