#include "UHH2/LQTopLep/include/LQTopLepPDFHists.h"
//#include "UHH2/LQToTopMu/include/HypothesisHistsOwn.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/common/include/JetIds.h"
#include <math.h>
#include <sstream>

#include "TH1F.h"
#include "TH2D.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;


LQTopLepPDFHists::LQTopLepPDFHists(Context & ctx, const string & dirname): Hists(ctx, dirname){

  is_mc = ctx.get("dataset_type") == "MC";

  h_mlq = ctx.get_handle<float>("mlq");
  h_is_mlq_reconstructed = ctx.get_handle<bool>("is_mlq_reconstructed");
  m_oname = ctx.get("dataset_version");
  TString m_pdfname = "NNPDF30_lo_as_0130";
  TString weightpath = ctx.get("PDFWeightPath");
  cout << "File: " << weightpath+m_oname << endl;

  Year year = extract_year(ctx);
  if(year == Year::is2017v1 || year == Year::is2017v2){
    take_ntupleweights = !(m_oname.Contains("QCD"));
  }
  else{
    take_ntupleweights = !(m_oname.Contains("QCD") || m_oname.Contains("ST_tW") || m_oname.Contains("LQto") || m_oname.Contains("DibosonNLO_ZZ_2L2Nu_2016v3"));
  }

  if(is_mc && !take_ntupleweights){
    // Only take shape effect into account for signals
    if(m_oname.Contains("LQto")) m_pdfweights.reset(new PDFWeights(m_pdfname,weightpath+m_oname));
    else m_pdfweights.reset(new PDFWeights(m_pdfname));
  }



  for(int i=0; i<100; i++){
    TString name1 = "ST_rebinlimit_PDF_";
    name1 += i+1;
    TString name2 = "MLQ_rebinlimit_PDF_";
    name2 += i+1;

    TString title1 = "S_{T} [GeV] for PDF No. ";
    title1 += i+1;
    TString title2 = "M_{LQ}^{rec} [GeV] for PDF No. ";;
    title2 += i+1;

    histo_names1.emplace_back(name1);
    histo_names2.emplace_back(name2);



    vector<double> bins_st = {0,175,350,525,700,875,1050,1225,1400,1575,1750,1925,2100,2450,2800,3000};
    vector<double> bins_mlq = {0,250,350,450,600,750,1000};
    book<TH1F>(name1, title1, bins_st.size()-1, &bins_st[0]);
    book<TH1F>(name2, title2, bins_mlq.size()-1, &bins_mlq[0]);
  }
}

void LQTopLepPDFHists::fill(const Event & event){
  double weight = event.weight;
  if(!is_mc) return;


  //ST
  auto met = event.met->pt();
  double st = 0.0;
  double st_jets = 0.0;
  double st_lep = 0.0;
  for(const auto & jet : *event.jets){
    st_jets += jet.pt();
  }
  for(const auto & electron : *event.electrons){
    st_lep += electron.pt();
  }
  for(const auto & muon : *event.muons){
    st_lep += muon.pt();
  }
  st = st_lep + st_jets + met;


  // cout << "Number of systweights:"  << event.genInfo->systweights().size() << endl;
  //   if(event.genInfo->systweights().size() < 100 && take_ntupleweights) throw runtime_error("In LQTopLepPDFHists.cxx: Systweights in event.genInfo() is empty but ntupleweights shall be taken. Is this correct? In this case add exception to take_ntupleweights.");
  //   if(event.genInfo->systweights().size() > 110 && (!take_ntupleweights)) throw runtime_error("In LQTopLepPDFHists.cxx: Systweights in event.genInfo() is NOT empty but take_ntupleweights is set to 'false'. Is this correct? In this case Thomas says the genInfo weight should be used. Add this sample to take_ntupleweights");


  bool is_mlq_reconstructed = event.get(h_is_mlq_reconstructed);
  float mlq = event.get(h_mlq);

  // Fill histograms
  if(take_ntupleweights){
    for(int i=0; i<100; i++){

      double pdf_weight = event.genInfo->systweights().at(i+9);
      double fillweight = weight * pdf_weight/event.genInfo->originalXWGTUP();
      TString name_st = histo_names1[i];
      TString name_mlq = histo_names2[i];

      if(is_mlq_reconstructed)  hist(name_mlq)->Fill(mlq, fillweight);
      if(st<2900) hist(name_st)->Fill(st, fillweight);
      else        hist(name_st)->Fill(2900, fillweight);
    }
  }
  else{
    std::vector<double> weights = m_pdfweights->GetWeightList(event);
    for(int i=0; i<100; i++){
      double fillweight = weight*weights[i];
      TString name_st = histo_names1[i];
      TString name_mlq = histo_names2[i];

      if(is_mlq_reconstructed) hist(name_mlq)->Fill(mlq, fillweight);
      if(st<2900) hist(name_st)->Fill(st, fillweight);
      else        hist(name_st)->Fill(2900, fillweight);
    }
  }
}

LQTopLepPDFHists::~LQTopLepPDFHists(){}
