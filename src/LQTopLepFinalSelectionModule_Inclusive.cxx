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

  class LQTopLepFinalSelectionModule_Inclusive: public ModuleBASE {
  public:

    explicit LQTopLepFinalSelectionModule_Inclusive(Context & ctx);
    virtual bool process(Event & event) override;
    void book_histograms(uhh2::Context&, vector<string>);
    void book_pdf_histograms(uhh2::Context&, vector<string>);
    void fill_histograms(uhh2::Event&, string, string, bool);

  private:

    bool is_mc;

    Year year;
    TString dataset_version;

    vector<TString> handlenames, systnames, systshift, systshift_scale;
    vector<string> histogramtags, histogramtags_scale, histogramtags_pdf;

    uhh2::Event::Handle<float> h_eventweight_lumi, h_eventweight_final;
    vector<uhh2::Event::Handle<float>> systweight_handles, systweight_scale_handles, scalefactor_handles;

    uhh2::Event::Handle<bool> h_is_mlq_reconstructed;
    uhh2::Event::Handle<float> h_mlq, h_chi2;
    uhh2::Event::Handle<TString> h_mlq_reco_mode_lq, h_mlq_reco_mode_top, h_region;

  };


  void LQTopLepFinalSelectionModule_Inclusive::book_histograms(uhh2::Context& ctx, vector<string> tags){
    for(const auto & tag : tags){
      cout << "booking histograms with tag " << tag << endl;
      string mytag = "FinalSelection_srmu_" + tag;
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = "FinalSelection_srmu_catA_" + tag;
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = "FinalSelection_srmu_catB_" + tag;
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = "FinalSelection_ttbar_" + tag;
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = "FinalSelection_ttbar_catB_" + tag;
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = "FinalSelection_dycrmu_" + tag;
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = "FinalSelection_dycrmu_catA_" + tag;
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = "FinalSelection_dycrmu_catB_" + tag;
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = "FinalSelection_dycrele_" + tag;
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = "FinalSelection_dycrele_catA_" + tag;
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
      mytag = "FinalSelection_dycrele_catB_" + tag;
      book_HFolder(mytag, new LQTopLepHists(ctx,mytag));
    }
  }

  void LQTopLepFinalSelectionModule_Inclusive::book_pdf_histograms(uhh2::Context& ctx, vector<string> tags){
    for(const auto & tag : tags){
      cout << "booking pdf histograms with tag " << tag << endl;
      string mytag = "FinalSelection_srmu_" + tag;
      book_HFolder(mytag, new LQTopLepPDFHists(ctx,mytag));
      mytag = "FinalSelection_srmu_catA_" + tag;
      book_HFolder(mytag, new LQTopLepPDFHists(ctx,mytag));
      mytag = "FinalSelection_srmu_catB_" + tag;
      book_HFolder(mytag, new LQTopLepPDFHists(ctx,mytag));
      mytag = "FinalSelection_ttbar_" + tag;
      book_HFolder(mytag, new LQTopLepPDFHists(ctx,mytag));
      mytag = "FinalSelection_ttbar_catB_" + tag;
      book_HFolder(mytag, new LQTopLepPDFHists(ctx,mytag));
      mytag = "FinalSelection_dycrmu_" + tag;
      book_HFolder(mytag, new LQTopLepPDFHists(ctx,mytag));
      mytag = "FinalSelection_dycrmu_catA_" + tag;
      book_HFolder(mytag, new LQTopLepPDFHists(ctx,mytag));
      mytag = "FinalSelection_dycrmu_catB_" + tag;
      book_HFolder(mytag, new LQTopLepPDFHists(ctx,mytag));
      mytag = "FinalSelection_dycrele_" + tag;
      book_HFolder(mytag, new LQTopLepPDFHists(ctx,mytag));
      mytag = "FinalSelection_dycrele_catA_" + tag;
      book_HFolder(mytag, new LQTopLepPDFHists(ctx,mytag));
      mytag = "FinalSelection_dycrele_catB_" + tag;
      book_HFolder(mytag, new LQTopLepPDFHists(ctx,mytag));
    }
  }

  void LQTopLepFinalSelectionModule_Inclusive::fill_histograms(uhh2::Event& event, string tag, string region, bool is_mlq_reconstructed){
    string mytag = "FinalSelection_" + region + "_" + tag;
    HFolder(mytag)->fill(event);
    if(is_mlq_reconstructed) mytag = "FinalSelection_" + region + "_catA_" + tag;
    else                     mytag = "FinalSelection_" + region + "_catB_" + tag;
    HFolder(mytag)->fill(event);
  }



  LQTopLepFinalSelectionModule_Inclusive::LQTopLepFinalSelectionModule_Inclusive(Context & ctx){
    for(auto & kv : ctx.get_all()){
      cout << " " << kv.first << " = " << kv.second << endl;
    }

    is_mc = ctx.get("dataset_type") == "MC";
    dataset_version = ctx.get("dataset_version");
    year = extract_year(ctx);

    h_eventweight_lumi = ctx.get_handle<float>("eventweight_lumi");
    h_eventweight_final = ctx.get_handle<float>("eventweight_final");
    h_mlq_reco_mode_lq = ctx.get_handle<TString>("mlq_reco_mode_lq");
    h_mlq_reco_mode_top = ctx.get_handle<TString>("mlq_reco_mode_top");
    h_region = ctx.get_handle<TString>("region");

    h_is_mlq_reconstructed = ctx.get_handle<bool>("is_mlq_reconstructed");
    h_mlq = ctx.get_handle<float>("mlq");
    h_chi2 = ctx.get_handle<float>("chi2");

    // Declare all systematics names in consistent ordering
    systnames = {"muid", "pu", "eleid", "elereco", "muiso", "mutrigger", "btag_bc", "btag_udsg"};
    handlenames = {"weight_sfmu_id", "weight_pu", "weight_sfelec_id", "weight_sfelec_reco", "weight_sfmu_iso", "weight_sfmu_trigger", "weight_btag" , "weight_btag"};

    systshift = {"up", "down"};
    if(systnames.size() != handlenames.size()) throw runtime_error("In LQTopLepFinalModule.cxx: Length of systnames and handlenames is not equal.");

    histogramtags = {};
    histogramtags.emplace_back("nominal");

    for(unsigned int i=0; i<systnames.size(); i++){
      for(unsigned int j=0; j<systshift.size(); j++){

        TString handlename = handlenames[i] + "_" + systshift[j];
        TString sf_name = handlenames[i];

        // PU doesn't exist in data (grr!!). Give it a dummy and never use it again. Doesn't matter on data anyway.
        if(systnames[i] == "pu" && !is_mc){
          handlename = "eventweight_final";
          sf_name = "eventweight_final";
        }

        // B-tagging uncertainties require a special treatment
        if(systnames[i] == "btag_bc"){
          handlename = handlenames[i] + "_bc_" + systshift[j];
        }
        else if(systnames[i] == "btag_udsg"){
          handlename = handlenames[i] + "_udsg_" + systshift[j];
        }

        uhh2::Event::Handle<float> handle1 = ctx.declare_event_output<float>((string)handlename);
        uhh2::Event::Handle<float> handle2 = ctx.declare_event_output<float>((string)sf_name);
        systweight_handles.emplace_back(handle1);
        scalefactor_handles.emplace_back(handle2);

        TString histname = systnames[i] + "_" + systshift[j];
        histogramtags.emplace_back(histname);
      }
    }


    //book all the histogram folders
    book_histograms(ctx, histogramtags);

    // Scale variation
    histogramtags_scale = {};
    systshift_scale = {"upup", "upnone", "noneup", "nonedown", "downnone", "downdown"};
    for(unsigned int i=0; i<systshift_scale.size(); i++){
      TString handlename = "weight_murmuf_" + systshift_scale[i];

      uhh2::Event::Handle<float> handle = ctx.declare_event_output<float>((string)handlename);
      systweight_scale_handles.emplace_back(handle);

      TString histname = "scale_" + systshift_scale[i];
      histogramtags_scale.emplace_back(histname);
    }

    book_histograms(ctx, histogramtags_scale);


    // Separately book one set of PDF hists (each contains the 100 variations for M_Tprime)
    book_pdf_histograms(ctx, {"pdf"});

  }


  bool LQTopLepFinalSelectionModule_Inclusive::process(Event & event) {
    cout << "Line: " << __LINE__ << endl;

    TString handle_region = event.get(h_region);
    string region = (string)handle_region;
    cout << "region: " << region << endl;
    bool is_mlq_reconstructed = event.get(h_is_mlq_reconstructed);

    // Read out nominal eventweight
    float weight_nominal = event.get(h_eventweight_final);

    // Fill histograms once with nominal weights
    event.weight = weight_nominal;
    cout << "Line: " << __LINE__ << endl;

    fill_histograms(event, "nominal", region, is_mlq_reconstructed);

    // Loop over easy systematics
    for(unsigned int i=0; i<systnames.size(); i++){    
      cout << "Line: " << __LINE__ << endl;
      cout << "Systname: " << systnames[i] << endl;
      for(unsigned int j=0; j<systshift.size(); j++){
	cout << "Line: " << __LINE__ << endl;
        int idx = 2*i + j;
 	cout << "Line: " << __LINE__ << endl;
	cout << "systweight_handles: " << event.get(systweight_handles[idx]) << endl;
        float systweight = event.get(systweight_handles[idx]);
	cout << "Line: " << __LINE__ << endl;
        float sfweight = event.get(scalefactor_handles[idx]);
	cout << "Line: " << __LINE__ << endl;
        event.weight = weight_nominal * systweight / sfweight;
	cout << "Line: " << __LINE__ << endl;

        TString tag = systnames[i] + "_" + systshift[j];
        fill_histograms(event, (string)tag, region, is_mlq_reconstructed);
      }
    }
    cout << "Line: " << __LINE__ << endl;

    // Loop over scale systematics
    for(unsigned int j=0; j<systshift_scale.size(); j++){

      float systweight = event.get(systweight_scale_handles[j]);
      event.weight = weight_nominal * systweight;

      TString tag = "scale_" + systshift_scale[j];
      fill_histograms(event, (string)tag, region, is_mlq_reconstructed);
    }
    cout << "Line: " << __LINE__ << endl;

    // Fill PDF histograms
    event.weight = weight_nominal;
    fill_histograms(event, "pdf", region, is_mlq_reconstructed);




    cout << "Line: " << __LINE__ << endl;



    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the LQTopLepFinalSelectionModule_Inclusive is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepFinalSelectionModule_Inclusive)

}
