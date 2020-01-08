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
//IDs
#include "UHH2/common/include/ElectronIds.h"
#include "UHH2/common/include/MuonIds.h"
// my own classes
#include "UHH2/LQTopLep/include/LQTopLepSelections.h"
#include "UHH2/LQTopLep/include/LQTopLepHists.h"
#include "UHH2/LQTopLep/include/LQTopLepTriggerHists.h"
#include <UHH2/LQTopLep/include/ModuleBASE.h>


using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class LQTopLepTriggerFullselectionModule: public ModuleBASE {
  public:

    explicit LQTopLepTriggerFullselectionModule(Context & ctx);
    virtual bool process(Event & event) override;
    void book_histograms(uhh2::Context&, vector<string>);
    void fill_histograms(uhh2::Event&, string);

  private:

    std::unique_ptr<CommonModules> common;
    unique_ptr<AnalysisModule> SF_muonID, SF_muonIso, SF_eleReco, SF_eleID;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor,
    // to avoid memory leaks.
    std::unique_ptr<Selection> nelectrons_sel, nmuons_sel, trigger1_muon_sel, trigger2_muon_sel, trigger1_electron_sel, trigger2_electron_sel;

    ElectronId eleId;
    MuonId muId;
    bool is_mc;


  };

  void LQTopLepTriggerFullselectionModule::book_histograms(uhh2::Context& ctx, vector<string> tags){
    for(const auto & tag : tags){
      cout << "booking histograms with tag " << tag << endl;
      string mytag = tag+"_General";
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = tag+"_Trigger";
      book_HFolder(mytag, new LQTopLepTriggerHists(ctx,mytag));
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
    mytag = tag+"_Trigger";
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


  LQTopLepTriggerFullselectionModule::LQTopLepTriggerFullselectionModule(Context & ctx){

    cout << "Hello World from LQTopLepTriggerFullselectionModule!" << endl;
    for(auto & kv : ctx.get_all()) cout << " " << kv.first << " = " << kv.second << endl;

    is_mc = ctx.get("dataset_type") == "MC";
    Year year = extract_year(ctx);


    // Object kinematic cuts
    // Just ==1 e and ==1 mu with pt > 30
    double leptonpt = 30.;
    double leptoneta = 2.4;


    // Object IDs
    eleId = AndId<Electron>(ElectronID_Fall17_tight, PtEtaCut(leptonpt, leptoneta));
    if (year == Year::is2016v2) muId = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(leptonpt, leptoneta), MuonIso(0.15));
    else                        muId = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(leptonpt, leptoneta), MuonID(Muon::PFIsoTight));


    // CommonModules
    common.reset(new CommonModules());
    common->disable_lumisel();
    common->disable_jersmear();
    common->disable_jec();
    common->set_electron_id(eleId);
    common->set_muon_id(muId);
    common->init(ctx);


    // Lepton scale factors
    SF_muonID.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonID_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightID_DEN_genTracks_eta_pt", 0., "id", false, "nominal"));
    SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonIso_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt", 0., "iso", false, "nominal"));

    SF_eleReco.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root", 1, "reco", "nominal"));
    SF_eleID.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/2016LegacyReReco_ElectronTight_Fall17V2.root", 1, "id", "nominal"));



    // Selections
    nelectrons_sel.reset(new NElectronSelection(1, 1));
    nmuons_sel.reset(new NMuonSelection(1, 1));
    if(year == Year::is2016v2 || year == Year::is2016v3){
      trigger1_muon_sel.reset(new TriggerSelection("HLT_IsoMu24_v*"));
      trigger2_muon_sel.reset(new TriggerSelection("HLT_IsoTkMu24_v*"));
      trigger1_electron_sel.reset(new TriggerSelection("HLT_Ele27_WPTight_Gsf_v*"));
      trigger2_electron_sel.reset(new TriggerSelection("HLT_Photon175_v*"));
    }
    else if(year == Year::is2017v1 || year == Year::is2017v2){
      trigger1_muon_sel.reset(new TriggerSelection("HLT_IsoMu27_v*"));
      trigger2_muon_sel.reset(new TriggerSelection("HLT_IsoMu27_v*"));
      trigger1_electron_sel.reset(new TriggerSelection("HLT_Ele35_WPTight_Gsf_v*"));
      trigger2_electron_sel.reset(new TriggerSelection("HLT_Photon200_v*"));
    }
    else if(year == Year::is2018){
      trigger1_muon_sel.reset(new TriggerSelection("HLT_IsoMu24_v*"));
      trigger2_muon_sel.reset(new TriggerSelection("HLT_IsoMu24_v*"));
      trigger1_electron_sel.reset(new TriggerSelection("HLT_Ele32_WPTight_Gsf_v*"));
      trigger2_electron_sel.reset(new TriggerSelection("HLT_Ele32_WPTight_Gsf_v*"));
    }



    // Book histograms
    vector<string> histogram_tags = {"Cleaner", "Electron1", "Muon1", "LeptonSF", "ElectronTriggerBefore", "ElectronTriggerBefore_pt30to200", "ElectronTriggerBefore_pt30to50", "ElectronTriggerBefore_pt50to200", "ElectronTriggerBefore_pt50to100", "ElectronTriggerBefore_pt100to200", "ElectronTriggerBefore_pt200toInf", "ElectronTriggerBefore_pt100toInf", "ElectronTriggerAfter", "ElectronTriggerAfter_pt30to200", "ElectronTriggerAfter_pt30to50", "ElectronTriggerAfter_pt50to200", "ElectronTriggerAfter_pt50to100", "ElectronTriggerAfter_pt100to200", "ElectronTriggerAfter_pt100toInf", "ElectronTriggerAfter_pt200toInf", "MuonTriggerBefore", "MuonTriggerBefore_pt30to200", "MuonTriggerBefore_pt30to50", "MuonTriggerBefore_pt50to200", "MuonTriggerBefore_pt50to100", "MuonTriggerBefore_pt100to200", "MuonTriggerBefore_pt100toInf", "MuonTriggerBefore_pt200toInf", "MuonTriggerAfter", "MuonTriggerAfter_pt30to200", "MuonTriggerAfter_pt30to50", "MuonTriggerAfter_pt50to200", "MuonTriggerAfter_pt50to100", "MuonTriggerAfter_pt100to200", "MuonTriggerAfter_pt100toInf", "MuonTriggerAfter_pt200toInf"};
    book_histograms(ctx, histogram_tags);


  }


  bool LQTopLepTriggerFullselectionModule::process(Event & event) {

    bool pass_muon_trigger = (trigger1_muon_sel->passes(event) || trigger2_muon_sel->passes(event));
    bool pass_electron_trigger = (trigger1_electron_sel->passes(event) || trigger2_electron_sel->passes(event));

    bool pass_common = common->process(event);
    if(!pass_common) return false;
    fill_histograms(event,"Cleaner");

    if(!nelectrons_sel->passes(event)) return false;
    fill_histograms(event,"Electron1");

    if(!nmuons_sel->passes(event)) return false;
    fill_histograms(event,"Muon1");

    SF_muonID->process(event);
    SF_muonIso->process(event);
    SF_eleReco->process(event);
    SF_eleID->process(event);
    fill_histograms(event,"LeptonSF");

    // Start with electron-SF derivation --> require muon trigger and then calculate e-trigger efficiency
    if(pass_muon_trigger){
      fill_histograms(event,"ElectronTriggerBefore");

      if(event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerBefore_pt30to200");
      if(event.electrons->at(0).pt() < 50) fill_histograms(event, "ElectronTriggerBefore_pt30to50");
      if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 100) fill_histograms(event, "ElectronTriggerBefore_pt50to100");
      if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerBefore_pt50to200");
      if(event.electrons->at(0).pt() > 100 && event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerBefore_pt100to200");
      if(event.electrons->at(0).pt() > 100) fill_histograms(event, "ElectronTriggerBefore_pt100toInf");
      if(event.electrons->at(0).pt() > 200) fill_histograms(event, "ElectronTriggerBefore_pt200toInf");

      if(pass_electron_trigger){
        fill_histograms(event, "ElectronTriggerAfter");

        if(event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerAfter_pt30to200");
        if(event.electrons->at(0).pt() < 50) fill_histograms(event, "ElectronTriggerAfter_pt30to50");
        if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 100) fill_histograms(event, "ElectronTriggerAfter_pt50to100");
        if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerAfter_pt50to200");
        if(event.electrons->at(0).pt() > 100 && event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerAfter_pt100to200");
        if(event.electrons->at(0).pt() > 100) fill_histograms(event, "ElectronTriggerAfter_pt100toInf");
        if(event.electrons->at(0).pt() > 200) fill_histograms(event, "ElectronTriggerAfter_pt200toInf");
      }
    }

    // Start with muon-SF derivation --> require electron trigger and then calculate mu-trigger efficiency
    if(pass_electron_trigger){
      fill_histograms(event,"MuonTriggerBefore");

      if(event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerBefore_pt30to200");
      if(event.muons->at(0).pt() < 50) fill_histograms(event, "MuonTriggerBefore_pt30to50");
      if(event.muons->at(0).pt() > 50 && event.muons->at(0).pt() < 100) fill_histograms(event, "MuonTriggerBefore_pt50to100");
      if(event.muons->at(0).pt() > 50 && event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerBefore_pt50to200");
      if(event.muons->at(0).pt() > 100 && event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerBefore_pt100to200");
      if(event.muons->at(0).pt() > 100) fill_histograms(event, "MuonTriggerBefore_pt100toInf");
      if(event.muons->at(0).pt() > 200) fill_histograms(event, "MuonTriggerBefore_pt200toInf");

      if(pass_muon_trigger){
        fill_histograms(event, "MuonTriggerAfter");

        if(event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerAfter_pt30to200");
        if(event.muons->at(0).pt() < 50) fill_histograms(event, "MuonTriggerAfter_pt30to50");
        if(event.muons->at(0).pt() > 50 && event.muons->at(0).pt() < 100) fill_histograms(event, "MuonTriggerAfter_pt50to100");
        if(event.muons->at(0).pt() > 50 && event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerAfter_pt50to200");
        if(event.muons->at(0).pt() > 100 && event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerAfter_pt100to200");
        if(event.muons->at(0).pt() > 100) fill_histograms(event, "MuonTriggerAfter_pt100toInf");
        if(event.muons->at(0).pt() > 200) fill_histograms(event, "MuonTriggerAfter_pt200toInf");
      }
    }

    return true;
  }

  UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepTriggerFullselectionModule)

}
