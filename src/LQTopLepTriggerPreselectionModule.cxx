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
#include <UHH2/LQTopLep/include/ModuleBASE.h>


using namespace std;
using namespace uhh2;

namespace uhh2examples {

  /** \brief Basic analysis example of an AnalysisModule (formerly 'cycle') in UHH2
  *
  * This is the central class which calls other AnalysisModules, Hists or Selection classes.
  * This AnalysisModule, in turn, is called (via AnalysisModuleRunner) by SFrame.
  */
  class LQTopLepTriggerPreselectionModule: public ModuleBASE {
  public:

    explicit LQTopLepTriggerPreselectionModule(Context & ctx);
    virtual bool process(Event & event) override;
    void book_histograms(uhh2::Context&, vector<string>);
    void fill_histograms(uhh2::Event&, string);

  private:

    std::unique_ptr<CommonModules> common;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor,
    // to avoid memory leaks.
    std::unique_ptr<Selection> nelectrons_lowpt_sel, nelectrons_highpt_sel, nmuons_lowpt_sel, nmuons_highpt_sel, njets_sel;

    ElectronId eleId_lowpt, eleId_highpt;
    MuonId muId_lowpt, muId_highpt;
    JetId jetId;

    bool is_mc;


  };

  void LQTopLepTriggerPreselectionModule::book_histograms(uhh2::Context& ctx, vector<string> tags){
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

  void LQTopLepTriggerPreselectionModule::fill_histograms(uhh2::Event& event, string tag){
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


  LQTopLepTriggerPreselectionModule::LQTopLepTriggerPreselectionModule(Context & ctx){

    cout << "Hello World from LQTopLepTriggerPreselectionModule!" << endl;
    for(auto & kv : ctx.get_all()) cout << " " << kv.first << " = " << kv.second << endl;

    is_mc = ctx.get("dataset_type") == "MC";
    Year year = extract_year(ctx);


    // Object kinematic cuts
    // Idea: require == 1 lepton with >30 gev (minimum offline threshold for trigger plateaus), while not throwing away leptons with 10 < pt < 30 (for plotting the turn-on).
    // lowpt for cleaning and requiring at least 1
    // highpt for requiring == 1
    double leptonlowpt = 10.;
    double leptonhighpt = 30.;
    double leptoneta = 2.4;
    double jetpt = 30.;
    double jeteta = 2.4;


    // Object IDs
    JetId jet_pfid = JetPFID(JetPFID::WP_TIGHT_CHS);
    jetId = AndId<Jet>(jet_pfid, PtEtaCut(jetpt, jeteta));
    eleId_lowpt = AndId<Electron>(ElectronID_Fall17_tight, PtEtaCut(leptonlowpt, leptoneta));
    eleId_highpt = AndId<Electron>(ElectronID_Fall17_tight, PtEtaCut(leptonhighpt, leptoneta));
    if (year == Year::is2016v2){
      muId_lowpt = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(leptonlowpt, leptoneta), MuonIso(0.15));
      muId_highpt = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(leptonhighpt, leptoneta), MuonIso(0.15));
    }
    else{
      muId_lowpt = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(leptonlowpt, leptoneta), MuonID(Muon::PFIsoTight));
      muId_highpt = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(leptonhighpt, leptoneta), MuonID(Muon::PFIsoTight));
    }


    // CommonModules
    common.reset(new CommonModules());
    common->switch_jetlepcleaner(true);
    common->set_electron_id(eleId_lowpt);
    common->set_muon_id(muId_lowpt);
    common->set_jet_id(jetId);
    common->switch_jetPtSorter();
    common->init(ctx);


    // Selections
    nelectrons_lowpt_sel.reset(new NElectronSelection(1, -1));
    nmuons_lowpt_sel.reset(new NMuonSelection(1, -1));
    nelectrons_highpt_sel.reset(new NElectronSelection(1, 1, eleId_highpt));
    nmuons_highpt_sel.reset(new NMuonSelection(1, 1, muId_highpt));
    njets_sel.reset(new NJetSelection(2, -1));


    // Book histograms
    vector<string> histogram_tags = {"NoCuts", "Cleaner", "2Jets", "ElectronLowpt", "MuonLowpt", "ElectronHighpt", "MuonHighpt"};
    book_histograms(ctx, histogram_tags);


  }


  bool LQTopLepTriggerPreselectionModule::process(Event & event) {


    fill_histograms(event,"NoCuts");

    bool pass_common = common->process(event);
    if(!pass_common) return false;
    fill_histograms(event,"Cleaner");

    if(!njets_sel->passes(event)) return false;
    fill_histograms(event,"2Jets");

    if(!nelectrons_lowpt_sel->passes(event)) return false;
    fill_histograms(event,"ElectronLowpt");

    if(!nmuons_lowpt_sel->passes(event)) return false;
    fill_histograms(event,"MuonLowpt");

    if(!nelectrons_highpt_sel->passes(event)) return false;
    fill_histograms(event,"ElectronHighpt");

    if(!nmuons_highpt_sel->passes(event)) return false;
    fill_histograms(event,"MuonHighpt");

    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the LQTopLepTriggerPreselectionModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepTriggerPreselectionModule)

}
