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
    std::unique_ptr<Selection> nelectrons_sel, nmuons_sel, trigger1_muon_sel, trigger2_muon_sel, trigger1_electron_sel, trigger2_electron_sel, trigger3_electron_sel;

    ElectronId eleId;
    MuonId muId;
    bool is_mc;
    TString dataset_version;
    Year year;


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
    dataset_version = ((TString)ctx.get("dataset_version"));
    year = extract_year(ctx);


    // Object kinematic cuts
    // Just ==1 e and ==1 mu with pt > 30
    double muonpt = 30.;
    double electronpt = 30.;
    if(year == Year::is2017v2) electronpt = 40;
    if(year == Year::is2018) electronpt = 35;

    double leptoneta = 2.4;


    // Object IDs
    eleId = AndId<Electron>(ElectronID_Fall17_tight, PtEtaCut(electronpt, leptoneta));
    if (year == Year::is2016v2) muId = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(muonpt, leptoneta), MuonIso(0.15));
    else                        muId = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(muonpt, leptoneta), MuonID(Muon::PFIsoTight));


    // CommonModules
    common.reset(new CommonModules());
    common->disable_lumisel();
    common->disable_jersmear();
    common->disable_jec();
    common->set_electron_id(eleId);
    common->set_muon_id(muId);
    common->init(ctx);


    // Lepton scale factors
    if(year == Year::is2016v2 || year == Year::is2016v3){
      SF_muonID.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonID_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightID_DEN_genTracks_eta_pt", 0., "id", false, "nominal"));
      SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonIso_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt", 0., "iso", false, "nominal"));

      SF_eleReco.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root", 1, "reco", "nominal"));
      SF_eleID.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/2016LegacyReReco_ElectronTight_Fall17V2.root", 1, "id", "nominal"));
    }
    else if(year == Year::is2017v1 || year == Year::is2017v2){
      //
      SF_muonID.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2017/MuonID_94X_RunBCDEF_SF_ID.root", "NUM_TightID_DEN_genTracks_pt_abseta", 0., "id", true, "nominal"));
      SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2017/MuonIso_94X_RunBCDEF_SF_ISO.root", "NUM_TightRelIso_DEN_TightIDandIPCut_pt_abseta", 0., "iso", true, "nominal"));

      SF_eleReco.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2017/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root", 1, "reco", "nominal"));
      SF_eleID.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2017/2017_ElectronTight.root", 1, "id", "nominal"));
    }
    else if(year == Year::is2018){
      //
      SF_muonID.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2018/Muon_ID_SF_RunABCD.root", "NUM_TightID_DEN_TrackerMuons_pt_abseta", 0., "id", true, "nominal"));
      SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2018/Muon_Iso_SF_RunABCD.root", "NUM_TightRelIso_DEN_TightIDandIPCut_pt_abseta", 0., "iso", true, "nominal"));

      SF_eleReco.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2018/egammaEffi.txt_EGM2D_updatedAll.root", 1, "reco", "nominal"));
      SF_eleID.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2018/2018_ElectronTight.root", 1, "id", "nominal"));
    }


    // Selections
    nelectrons_sel.reset(new NElectronSelection(1, 1));
    nmuons_sel.reset(new NMuonSelection(1, 1));
    if(year == Year::is2016v2 || year == Year::is2016v3){
      trigger1_muon_sel.reset(new TriggerSelection("HLT_IsoMu24_v*"));
      trigger2_muon_sel.reset(new TriggerSelection("HLT_IsoTkMu24_v*"));
      trigger1_electron_sel.reset(new TriggerSelection("HLT_Ele27_WPTight_Gsf_v*"));
      trigger2_electron_sel.reset(new TriggerSelection("HLT_Ele115_CaloIdVT_GsfTrkIdT_v*"));
      trigger3_electron_sel.reset(new TriggerSelection("HLT_Photon175_v*"));
    }
    else if(year == Year::is2017v1 || year == Year::is2017v2){
      trigger1_muon_sel.reset(new TriggerSelection("HLT_IsoMu27_v*"));
      trigger2_muon_sel.reset(new TriggerSelection("HLT_IsoMu27_v*"));
      trigger1_electron_sel.reset(new TriggerSelection("HLT_Ele35_WPTight_Gsf_v*"));
      trigger2_electron_sel.reset(new TriggerSelection("HLT_Ele115_CaloIdVT_GsfTrkIdT_v*"));
      trigger3_electron_sel.reset(new TriggerSelection("HLT_Photon200_v*"));
    }
    else if(year == Year::is2018){
      trigger1_muon_sel.reset(new TriggerSelection("HLT_IsoMu24_v*"));
      trigger2_muon_sel.reset(new TriggerSelection("HLT_IsoMu24_v*"));
      trigger1_electron_sel.reset(new TriggerSelection("HLT_Ele32_WPTight_Gsf_v*"));
      trigger2_electron_sel.reset(new TriggerSelection("HLT_Ele115_CaloIdVT_GsfTrkIdT_v*"));
      trigger3_electron_sel.reset(new TriggerSelection("HLT_Photon200_v*"));
    }



    // Book histograms
    vector<string> histogram_tags = {"Cleaner", "Electron1", "Muon1", "LeptonSF", "ElectronTriggerBefore", "ElectronTriggerBefore_pt30to200", "ElectronTriggerBefore_pt30to175", "ElectronTriggerBefore_pt30to100", "ElectronTriggerBefore_pt30to50", "ElectronTriggerBefore_pt50to200", "ElectronTriggerBefore_pt50to175", "ElectronTriggerBefore_pt50to100", "ElectronTriggerBefore_pt100to200", "ElectronTriggerBefore_pt100to175", "ElectronTriggerBefore_pt200toInf", "ElectronTriggerBefore_pt175toInf", "ElectronTriggerBefore_pt100toInf", "ElectronTriggerBefore_eta0to0p9", "ElectronTriggerBefore_eta0p9to1p2", "ElectronTriggerBefore_eta1p2to2p1", "ElectronTriggerBefore_eta2p1to2p4", "ElectronTriggerAfter", "ElectronTriggerAfter_pt30to200", "ElectronTriggerAfter_pt30to175", "ElectronTriggerAfter_pt30to100", "ElectronTriggerAfter_pt30to50", "ElectronTriggerAfter_pt50to200", "ElectronTriggerAfter_pt50to175", "ElectronTriggerAfter_pt50to100", "ElectronTriggerAfter_pt100to200", "ElectronTriggerAfter_pt100to175", "ElectronTriggerAfter_pt100toInf", "ElectronTriggerAfter_pt200toInf", "ElectronTriggerAfter_pt175toInf", "ElectronTriggerAfter_eta0to0p9", "ElectronTriggerAfter_eta0p9to1p2", "ElectronTriggerAfter_eta1p2to2p1", "ElectronTriggerAfter_eta2p1to2p4", "MuonTriggerBefore", "MuonTriggerBefore_pt30to200", "MuonTriggerBefore_pt30to175", "MuonTriggerBefore_pt30to100", "MuonTriggerBefore_pt30to50", "MuonTriggerBefore_pt50to200", "MuonTriggerBefore_pt50to175", "MuonTriggerBefore_pt50to100", "MuonTriggerBefore_pt100to200", "MuonTriggerBefore_pt100to175", "MuonTriggerBefore_pt100toInf", "MuonTriggerBefore_pt200toInf", "MuonTriggerBefore_pt175toInf", "MuonTriggerBefore_eta0to0p9", "MuonTriggerBefore_eta0p9to1p2", "MuonTriggerBefore_eta1p2to2p1", "MuonTriggerBefore_eta2p1to2p4", "MuonTriggerAfter", "MuonTriggerAfter_pt30to200", "MuonTriggerAfter_pt30to175", "MuonTriggerAfter_pt30to100", "MuonTriggerAfter_pt30to50", "MuonTriggerAfter_pt50to200", "MuonTriggerAfter_pt50to175", "MuonTriggerAfter_pt50to100", "MuonTriggerAfter_pt100to200", "MuonTriggerAfter_pt100to175", "MuonTriggerAfter_pt100toInf", "MuonTriggerAfter_pt200toInf", "MuonTriggerAfter_pt175toInf", "MuonTriggerAfter_eta0to0p9", "MuonTriggerAfter_eta0p9to1p2", "MuonTriggerAfter_eta1p2to2p1", "MuonTriggerAfter_eta2p1to2p4", "ElectronTriggerBefore_pt30to120", "ElectronTriggerBefore_pt50to120", "ElectronTriggerBefore_pt100to120", "ElectronTriggerBefore_pt120to175", "ElectronTriggerBefore_pt120to200", "ElectronTriggerBefore_pt120toInf","ElectronTriggerAfter_pt30to120", "ElectronTriggerAfter_pt50to120", "ElectronTriggerAfter_pt100to120", "ElectronTriggerAfter_pt120to175", "ElectronTriggerAfter_pt120to200", "ElectronTriggerAfter_pt120toInf"};
    book_histograms(ctx, histogram_tags);


  }


  bool LQTopLepTriggerFullselectionModule::process(Event & event) {

    // HEM 16/17 issue in data and MC for 2018
    if(year == Year::is2018){
      if(event.electrons->size() < 1) throw runtime_error("Event does not have any electrons, should have one.");
      double eleeta = event.electrons->at(0).eta();
      double elephi = event.electrons->at(0).phi();
      if(-2.964 < eleeta && eleeta < -1.305 && -1.6 < elephi && elephi < -0.7) return false;

    }

    bool pass_muon_trigger = (trigger1_muon_sel->passes(event) || trigger2_muon_sel->passes(event));
    bool pass_electron_trigger;

    // 3 e trigger case
    // if(!is_mc && event.run < 299368) pass_electron_trigger = (trigger1_electron_sel->passes(event) || trigger3_electron_sel->passes(event));
    // else pass_electron_trigger = (trigger1_electron_sel->passes(event) || trigger2_electron_sel->passes(event) || trigger3_electron_sel->passes(event));

    // 2 e trigger case
    pass_electron_trigger = (trigger1_electron_sel->passes(event) || trigger3_electron_sel->passes(event));


    // if(!is_mc){
    //   if(event.run >=299368) pass_electron_trigger = (trigger1_electron_sel->passes(event) || trigger2_electron_sel->passes(event) || trigger3_electron_sel->passes(event));
    //   else pass_electron_trigger = (trigger1_electron_sel->passes(event) || trigger3_electron_sel->passes(event));
    // }
    // else pass_electron_trigger = (trigger1_electron_sel->passes(event) || trigger2_electron_sel->passes(event) || trigger3_electron_sel->passes(event));


    bool pass_electron_trigger_for_muoneff;

    // for 3 e trigger case (need this for muon efficiencies?)
    // if(!is_mc && event.run < 299368) pass_electron_trigger_for_muoneff = (trigger1_electron_sel->passes(event) || trigger3_electron_sel->passes(event));
    // else pass_electron_trigger_for_muoneff = (trigger1_electron_sel->passes(event) || trigger2_electron_sel->passes(event) || trigger3_electron_sel->passes(event));
    // if(year == Year::is2016v2 || year == Year::is2016v3 || year == Year::is2017v2){
    //   if(!is_mc && dataset_version.Contains("Electron")) {
    //     if(event.run < 299368) pass_electron_trigger_for_muoneff = trigger1_electron_sel->passes(event);
    //     else pass_electron_trigger_for_muoneff = (trigger1_electron_sel->passes(event) || trigger2_electron_sel->passes(event));
    //
    //   }
    //   else if (!is_mc && dataset_version.Contains("Photon")) {
    //     if(event.run < 299368) pass_electron_trigger_for_muoneff = !trigger1_electron_sel->passes(event) && trigger3_electron_sel->passes(event);
    //     else pass_electron_trigger_for_muoneff = !trigger1_electron_sel->passes(event) && !trigger2_electron_sel->passes(event) && trigger3_electron_sel->passes(event);
    //   }
    // }

    // for 2 e trigger case
    // MC and 2018
    pass_electron_trigger_for_muoneff = (trigger1_electron_sel->passes(event) || trigger3_electron_sel->passes(event));

    //  2016 & 2017
    if(year == Year::is2016v2 || year == Year::is2016v3 || year == Year::is2017v2){
      if(!is_mc && dataset_version.Contains("Electron")) {
        pass_electron_trigger_for_muoneff = trigger1_electron_sel->passes(event);
      }
      else if (!is_mc && dataset_version.Contains("Photon")) {
        pass_electron_trigger_for_muoneff = !trigger1_electron_sel->passes(event) && trigger3_electron_sel->passes(event);
      }
    }


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
    if(pass_muon_trigger) {
      fill_histograms(event,"ElectronTriggerBefore");

      if(event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerBefore_pt30to200");
      if(event.electrons->at(0).pt() < 175) fill_histograms(event, "ElectronTriggerBefore_pt30to175");
      if(event.electrons->at(0).pt() < 100) fill_histograms(event, "ElectronTriggerBefore_pt30to100");
      if(event.electrons->at(0).pt() < 50) fill_histograms(event, "ElectronTriggerBefore_pt30to50");
      if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 100) fill_histograms(event, "ElectronTriggerBefore_pt50to100");
      if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerBefore_pt50to200");
      if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 175) fill_histograms(event, "ElectronTriggerBefore_pt50to175");
      if(event.electrons->at(0).pt() > 100 && event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerBefore_pt100to200");
      if(event.electrons->at(0).pt() > 100 && event.electrons->at(0).pt() < 175) fill_histograms(event, "ElectronTriggerBefore_pt100to175");
      if(event.electrons->at(0).pt() > 100) fill_histograms(event, "ElectronTriggerBefore_pt100toInf");
      if(event.electrons->at(0).pt() > 200) fill_histograms(event, "ElectronTriggerBefore_pt200toInf");
      if(event.electrons->at(0).pt() > 175) fill_histograms(event, "ElectronTriggerBefore_pt175toInf");

      // for pt>120 GeV trigger
      if(event.electrons->at(0).pt() > 30 && event.electrons->at(0).pt() < 120) fill_histograms(event, "ElectronTriggerBefore_pt30to120");
      if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 120) fill_histograms(event, "ElectronTriggerBefore_pt50to120");
      if(event.electrons->at(0).pt() > 100 && event.electrons->at(0).pt() < 120) fill_histograms(event, "ElectronTriggerBefore_pt100to120");
      if(event.electrons->at(0).pt() > 120 && event.electrons->at(0).pt() < 175) fill_histograms(event, "ElectronTriggerBefore_pt120to175");
      if(event.electrons->at(0).pt() > 120 && event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerBefore_pt120to200");
      if(event.electrons->at(0).pt() > 120) fill_histograms(event, "ElectronTriggerBefore_pt120toInf");

      if(fabs(event.electrons->at(0).eta()) < 0.9) fill_histograms(event, "ElectronTriggerBefore_eta0to0p9");
      if(fabs(event.electrons->at(0).eta()) > 0.9 && fabs(event.electrons->at(0).eta()) < 1.2) fill_histograms(event, "ElectronTriggerBefore_eta0p9to1p2");
      if(fabs(event.electrons->at(0).eta()) > 1.2 && fabs(event.electrons->at(0).eta()) < 2.1) fill_histograms(event, "ElectronTriggerBefore_eta1p2to2p1");
      if(fabs(event.electrons->at(0).eta()) > 2.1 && fabs(event.electrons->at(0).eta()) < 2.4) fill_histograms(event, "ElectronTriggerBefore_eta2p1to2p4");





      if(pass_electron_trigger){
        fill_histograms(event, "ElectronTriggerAfter");

        if(event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerAfter_pt30to200");
        if(event.electrons->at(0).pt() < 175) fill_histograms(event, "ElectronTriggerAfter_pt30to175");
        if(event.electrons->at(0).pt() < 100) fill_histograms(event, "ElectronTriggerAfter_pt30to100");
        if(event.electrons->at(0).pt() < 50) fill_histograms(event, "ElectronTriggerAfter_pt30to50");
        if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 100) fill_histograms(event, "ElectronTriggerAfter_pt50to100");
        if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerAfter_pt50to200");
        if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 175) fill_histograms(event, "ElectronTriggerAfter_pt50to175");
        if(event.electrons->at(0).pt() > 100 && event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerAfter_pt100to200");
        if(event.electrons->at(0).pt() > 100 && event.electrons->at(0).pt() < 175) fill_histograms(event, "ElectronTriggerAfter_pt100to175");
        if(event.electrons->at(0).pt() > 100) fill_histograms(event, "ElectronTriggerAfter_pt100toInf");
        if(event.electrons->at(0).pt() > 200) fill_histograms(event, "ElectronTriggerAfter_pt200toInf");
        if(event.electrons->at(0).pt() > 175) fill_histograms(event, "ElectronTriggerAfter_pt175toInf");

        // for pt>120 GeV trigger
        if(event.electrons->at(0).pt() > 30 && event.electrons->at(0).pt() < 120) fill_histograms(event, "ElectronTriggerAfter_pt30to120");
        if(event.electrons->at(0).pt() > 50 && event.electrons->at(0).pt() < 120) fill_histograms(event, "ElectronTriggerAfter_pt50to120");
        if(event.electrons->at(0).pt() > 100 && event.electrons->at(0).pt() < 120) fill_histograms(event, "ElectronTriggerAfter_pt100to120");
        if(event.electrons->at(0).pt() > 120 && event.electrons->at(0).pt() < 175) fill_histograms(event, "ElectronTriggerAfter_pt120to175");
        if(event.electrons->at(0).pt() > 120 && event.electrons->at(0).pt() < 200) fill_histograms(event, "ElectronTriggerAfter_pt120to200");
        if(event.electrons->at(0).pt() > 120) fill_histograms(event, "ElectronTriggerAfter_pt120toInf");

        if(fabs(event.electrons->at(0).eta()) < 0.9) fill_histograms(event, "ElectronTriggerAfter_eta0to0p9");
        if(fabs(event.electrons->at(0).eta()) > 0.9 && fabs(event.electrons->at(0).eta()) < 1.2) fill_histograms(event, "ElectronTriggerAfter_eta0p9to1p2");
        if(fabs(event.electrons->at(0).eta()) > 1.2 && fabs(event.electrons->at(0).eta()) < 2.1) fill_histograms(event, "ElectronTriggerAfter_eta1p2to2p1");
        if(fabs(event.electrons->at(0).eta()) > 2.1 && fabs(event.electrons->at(0).eta()) < 2.4) fill_histograms(event, "ElectronTriggerAfter_eta2p1to2p4");
      }
    }

    // Start with muon-SF derivation --> require electron trigger and then calculate mu-trigger efficiency
    if(pass_electron_trigger_for_muoneff) {
      fill_histograms(event,"MuonTriggerBefore");

      if(event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerBefore_pt30to200");
      if(event.muons->at(0).pt() < 175) fill_histograms(event, "MuonTriggerBefore_pt30to175");
      if(event.muons->at(0).pt() < 100) fill_histograms(event, "MuonTriggerBefore_pt30to100");
      if(event.muons->at(0).pt() < 50) fill_histograms(event, "MuonTriggerBefore_pt30to50");
      if(event.muons->at(0).pt() > 50 && event.muons->at(0).pt() < 100) fill_histograms(event, "MuonTriggerBefore_pt50to100");
      if(event.muons->at(0).pt() > 50 && event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerBefore_pt50to200");
      if(event.muons->at(0).pt() > 50 && event.muons->at(0).pt() < 175) fill_histograms(event, "MuonTriggerBefore_pt50to175");
      if(event.muons->at(0).pt() > 100 && event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerBefore_pt100to200");
      if(event.muons->at(0).pt() > 100 && event.muons->at(0).pt() < 175) fill_histograms(event, "MuonTriggerBefore_pt100to175");
      if(event.muons->at(0).pt() > 100) fill_histograms(event, "MuonTriggerBefore_pt100toInf");
      if(event.muons->at(0).pt() > 200) fill_histograms(event, "MuonTriggerBefore_pt200toInf");
      if(event.muons->at(0).pt() > 175) fill_histograms(event, "MuonTriggerBefore_pt175toInf");

      if(fabs(event.muons->at(0).eta()) < 0.9) fill_histograms(event, "MuonTriggerBefore_eta0to0p9");
      if(fabs(event.muons->at(0).eta()) > 0.9 && fabs(event.muons->at(0).eta()) < 1.2) fill_histograms(event, "MuonTriggerBefore_eta0p9to1p2");
      if(fabs(event.muons->at(0).eta()) > 1.2 && fabs(event.muons->at(0).eta()) < 2.1) fill_histograms(event, "MuonTriggerBefore_eta1p2to2p1");
      if(fabs(event.muons->at(0).eta()) > 2.1 && fabs(event.muons->at(0).eta()) < 2.4) fill_histograms(event, "MuonTriggerBefore_eta2p1to2p4");



      if(pass_muon_trigger){
        fill_histograms(event, "MuonTriggerAfter");

        if(event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerAfter_pt30to200");
        if(event.muons->at(0).pt() < 175) fill_histograms(event, "MuonTriggerAfter_pt30to175");
        if(event.muons->at(0).pt() < 100) fill_histograms(event, "MuonTriggerAfter_pt30to100");
        if(event.muons->at(0).pt() < 50) fill_histograms(event, "MuonTriggerAfter_pt30to50");
        if(event.muons->at(0).pt() > 50 && event.muons->at(0).pt() < 100) fill_histograms(event, "MuonTriggerAfter_pt50to100");
        if(event.muons->at(0).pt() > 50 && event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerAfter_pt50to200");
        if(event.muons->at(0).pt() > 50 && event.muons->at(0).pt() < 175) fill_histograms(event, "MuonTriggerAfter_pt50to175");
        if(event.muons->at(0).pt() > 100 && event.muons->at(0).pt() < 200) fill_histograms(event, "MuonTriggerAfter_pt100to200");
        if(event.muons->at(0).pt() > 100 && event.muons->at(0).pt() < 175) fill_histograms(event, "MuonTriggerAfter_pt100to175");
        if(event.muons->at(0).pt() > 100) fill_histograms(event, "MuonTriggerAfter_pt100toInf");
        if(event.muons->at(0).pt() > 200) fill_histograms(event, "MuonTriggerAfter_pt200toInf");
        if(event.muons->at(0).pt() > 175) fill_histograms(event, "MuonTriggerAfter_pt175toInf");

        if(fabs(event.muons->at(0).eta()) < 0.9) fill_histograms(event, "MuonTriggerAfter_eta0to0p9");
        if(fabs(event.muons->at(0).eta()) > 0.9 && fabs(event.muons->at(0).eta()) < 1.2) fill_histograms(event, "MuonTriggerAfter_eta0p9to1p2");
        if(fabs(event.muons->at(0).eta()) > 1.2 && fabs(event.muons->at(0).eta()) < 2.1) fill_histograms(event, "MuonTriggerAfter_eta1p2to2p1");
        if(fabs(event.muons->at(0).eta()) > 2.1 && fabs(event.muons->at(0).eta()) < 2.4) fill_histograms(event, "MuonTriggerAfter_eta2p1to2p4");
      }
    }

    return true;
  }

  UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepTriggerFullselectionModule)

}
