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
#include "UHH2/LQTopLep/include/LQTopLepModules.h"
#include <UHH2/SingleTth/include/ModuleBASE.h>


using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class LQTopLepPreselectionModule_Inclusive: public ModuleBASE {
  public:

    explicit LQTopLepPreselectionModule_Inclusive(Context & ctx);
    virtual bool process(Event & event) override;
    void book_histograms(uhh2::Context&, vector<string>);
    void fill_histograms(uhh2::Event&, string);

  private:

    std::unique_ptr<CommonModules> common;
    std::unique_ptr<Selection> njet_sel, nmuons_sel, nmuons_sel_ttbar, nelectrons_sel_ttbar, st_sel;

    ElectronId EleId;
    JetId Jet_ID;
    MuonId MuId;

    bool is_mc;
  };

  void LQTopLepPreselectionModule_Inclusive::book_histograms(uhh2::Context& ctx, vector<string> tags){
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

  void LQTopLepPreselectionModule_Inclusive::fill_histograms(uhh2::Event& event, string tag){
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


  LQTopLepPreselectionModule_Inclusive::LQTopLepPreselectionModule_Inclusive(Context & ctx){

    for(auto & kv : ctx.get_all()){
      cout << " " << kv.first << " = " << kv.second << endl;
    }

    is_mc = ctx.get("dataset_type") == "MC";
    Year year = extract_year(ctx);

    // Object IDs
    JetId jet_pfid = JetPFID(JetPFID::WP_TIGHT_CHS);
    EleId = AndId<Electron>(ElectronID_Fall17_tight, PtEtaCut(30.0, 2.4));
    if (year == Year::is2016v2) MuId = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(30.0, 2.4), MuonIso(0.15));
    else                        MuId = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(30.0, 2.4), MuonID(Muon::PFIsoTight));
    Jet_ID = AndId<Jet>(jet_pfid, PtEtaCut(30.0, 2.4));


    // CommonModules
    common.reset(new CommonModules());
    common->switch_jetlepcleaner(true);
    common->set_electron_id(EleId);
    common->set_muon_id(MuId);
    common->set_jet_id(Jet_ID);
    common->switch_jetPtSorter();
    common->init(ctx);


    // Selections
    njet_sel.reset(new NJetSelection(2, -1));
    nmuons_sel.reset(new NMuonSelection(2, -1));
    nmuons_sel_ttbar.reset(new NMuonSelection(1, 1));
    nelectrons_sel_ttbar.reset(new NElectronSelection(1, 1));
    st_sel.reset(new HtSelection(350));


    // Book histograms
    vector<string> histogram_tags = {"NoCuts", "Cleaner", "2Jets", "ST350", "2Muons", "1Muon1Electron"};
    book_histograms(ctx, histogram_tags);
  }


  bool LQTopLepPreselectionModule_Inclusive::process(Event & event) {

    fill_histograms(event,"NoCuts");

    bool pass_common = common->process(event);
    if(!pass_common) return false;
    fill_histograms(event,"Cleaner");

    if(!njet_sel->passes(event)) return false;
    fill_histograms(event,"2Jets");

    if(!st_sel->passes(event)) return false;
    fill_histograms(event,"ST350");

    if(!nmuons_sel->passes(event) && !(nmuons_sel_ttbar->passes(event) && nelectrons_sel_ttbar->passes(event))) return false;
    if(nmuons_sel->passes(event)){
      fill_histograms(event,"2Muons");
    }
    else{
        fill_histograms(event,"1Muon1Electron");
    }



    return true;
  }

  UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepPreselectionModule_Inclusive)

}
