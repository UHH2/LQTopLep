#include <iostream>
#include <memory>


#include <UHH2/SingleTth/include/ModuleBASE.h>
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
#include <UHH2/SingleTth/include/ModuleBASE.h>


using namespace std;
using namespace uhh2;

// AT the moment: basically LQ->te
// to include LQ->tmu: keep events with Ne>=2 or Nmu>=2
// missing: FakeRateWeights

namespace uhh2examples {

  /** \brief Basic analysis example of an AnalysisModule (formerly 'cycle') in UHH2
   *
   * This is the central class which calls other AnalysisModules, Hists or Selection classes.
   * This AnalysisModule, in turn, is called (via AnalysisModuleRunner) by SFrame.
   */
  class LQTopLepPreselectionModule: public ModuleBASE {
  public:

    explicit LQTopLepPreselectionModule(Context & ctx);
    virtual bool process(Event & event) override;
    void book_histograms(uhh2::Context&, vector<string>);
    void fill_histograms(uhh2::Event&, string);

  private:

    std::unique_ptr<CommonModules> common;

    std::unique_ptr<JetCleaner> jetcleaner;
    std::unique_ptr<ElectronCleaner> electroncleaner;
    unique_ptr<MuonCleaner> muoncleaner;
    std::unique_ptr<Selection> njet_sel, nele_sel, nmuons_sel, lumi_sel, ht_sel, ele_trigger_sel, mu_trigger_sel;

    ElectronId eleId;
    JetId jetId;
    MuonId muId;

    bool is_mc;
    TString Sys_EleFakeRate, path_EleFakeRate, Sys_MuFakeRate, path_MuFakeRate;

    // what's that for?
    uhh2::Event::Handle<vector<Jet>> h_raw_jets_ele;
    uhh2::Event::Handle<vector<Particle>> h_raw_genjets_ele;

    /*
      unique_ptr<ElectronFakeRateWeights> SF_eleFakeRate;
      unique_ptr<MuonFakeRateWeights> SF_muFakeRate;

      uhh2::Event::Handle<double> FakeRateWeightEle;
      uhh2::Event::Handle<double> FakeRateWeightEleUp;
      uhh2::Event::Handle<double> FakeRateWeightEleDown;
      uhh2::Event::Handle<double> FakeRateWeightMu;
      uhh2::Event::Handle<double> FakeRateWeightMuUp;
      uhh2::Event::Handle<double> FakeRateWeightMuDown;
    */
  };

  void LQTopLepPreselectionModule::book_histograms(uhh2::Context& ctx, vector<string> tags){
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

  void LQTopLepPreselectionModule::fill_histograms(uhh2::Event& event, string tag){
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


  LQTopLepPreselectionModule::LQTopLepPreselectionModule(Context & ctx){

    cout << "Hello World from LQTopLepPreselectionModule!" << endl;
    for(auto & kv : ctx.get_all()){
      cout << " " << kv.first << " = " << kv.second << endl;
    }

    is_mc = ctx.get("dataset_type") == "MC";
    Year year = extract_year(ctx);

    path_EleFakeRate = ctx.get("path_EleFakeRate");
    Sys_EleFakeRate = ctx.get("Systematic_EleFakeRate");
    path_MuFakeRate = ctx.get("path_MuFakeRate");
    Sys_MuFakeRate = ctx.get("Systematic_MuFakeRate");

    // Object IDs
    eleId = AndId<Electron>(ElectronID_Summer16_tight, PtEtaCut(30.0, 2.4));
    if (year == Year::is2016v2) muId = AndId<Muon>(MuonID(Muon::Loose), PtEtaCut(30.0, 2.4), MuonIso(0.15)); // loose for LQ->te, maybe change to tight at some point
    else                        muId = AndId<Muon>(MuonID(Muon::CutBasedIdLoose), PtEtaCut(30.0, 2.4), MuonIso(0.15)); // loose for LQ->te, maybe change to tight at some point
    cout << __LINE__ << endl;


    // CommonModules
    common.reset(new CommonModules());
    common->switch_jetlepcleaner(true);
    common->set_electron_id(eleId);
    common->set_muon_id(muId);
    common->set_jet_id(PtEtaCut(30.0, 2.4));
    common->init(ctx);

    electroncleaner.reset(new ElectronCleaner(eleId));
    muoncleaner.reset(new MuonCleaner(muId));

    // what's that for?
    h_raw_jets_ele = ctx.get_handle<vector<Jet>>("RawJetsEle");
    h_raw_genjets_ele = ctx.get_handle<vector<Particle>>("RawGenJetsEle");

    /*
      SF_eleFakeRate.reset(new ElectronFakeRateWeights(ctx, JERFiles::Summer16_23Sep2016_V4_L123_AK4PFchs_MC, path_EleFakeRate, Sys_EleFakeRate, "RawJetsEle", "RawGenJetsEle"));
      SF_muFakeRate.reset(new MuonFakeRateWeights(ctx, path_MuFakeRate, Sys_MuFakeRate));
    */


    // Selections
    njet_sel.reset(new NJetSelection(2, -1)); // at least 2 Jets
    nele_sel.reset(new NElectronSelection(2, -1)); // LQ->te: at least 2 electrons
    nmuons_sel.reset(new NMuonSelection(2, -1)); // LQ->tmu: at least 2 muons, not used yet
    ht_sel.reset(new HtSelection(350)); // ST>= 350 GeV
    lumi_sel.reset(new LumiSelection(ctx));


    // Book histograms
    vector<string> histogram_tags = {"NoCuts", "Cleaner", "2Jets", "2Ele", "ST350"};
    book_histograms(ctx, histogram_tags);


    /*
      FakeRateWeightEle = ctx.declare_event_output<double>("FakeRateWeightEle");
      FakeRateWeightEleUp = ctx.declare_event_output<double>("FakeRateWeightEleUp");
      FakeRateWeightEleDown = ctx.declare_event_output<double>("FakeRateWeightEleDown");
      FakeRateWeightMu = ctx.declare_event_output<double>("FakeRateWeightMu");
      FakeRateWeightMuUp = ctx.declare_event_output<double>("FakeRateWeightMuUp");
      FakeRateWeightMuDown = ctx.declare_event_output<double>("FakeRateWeightMuDown");
    */
  }


  bool LQTopLepPreselectionModule::process(Event & event) {



    // Lumi selection
    if(!is_mc) {
      if(!lumi_sel->passes(event)) return false;
    }
    fill_histograms(event,"NoCuts");

    /*
    // apply fake rate weights for electrons and muons
    if(is_mc){

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
    fill_histograms(event,"Cleaner");

    if(!njet_sel->passes(event)) return false; // N_Jet>=2
    fill_histograms(event,"2Jets");

    if(!nele_sel->passes(event)) return false; // N_el>=2
    fill_histograms(event,"2Ele");

    if(!ht_sel->passes(event)) return false; // ST>=350 GeV
    fill_histograms(event,"ST350");



    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the LQTopLepPreselectionModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepPreselectionModule)

}
