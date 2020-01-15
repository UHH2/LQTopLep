#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/PDFWeights.h"

using namespace uhh2;
using namespace std;

class LQTopLepPDFWeightsModule: public AnalysisModule {

 public:
  explicit LQTopLepPDFWeightsModule(Context & ctx);
  ~LQTopLepPDFWeightsModule();
  virtual bool process(Event & event) override;

 private:
  PDFWeights* m_pdfweights;

  std::vector<double> m_sumofweights;
  int m_Ntotal;
  TString m_pdfname;
  TString m_oname;
  TString m_postfix;
  TString m_outpath;
};

LQTopLepPDFWeightsModule::~LQTopLepPDFWeightsModule(){

  cout << "N kept events:  " << m_Ntotal << endl;

  TString outfilename = m_outpath;
  outfilename += "/";
  outfilename += m_oname;
  outfilename +="_";
  outfilename += m_pdfname;
  outfilename += "_weights_";
  outfilename += m_postfix;
  outfilename += ".txt";

  std::ofstream outfile;
  cout << "outfilename: " << outfilename << endl;
  outfile.open(((std::string)outfilename).c_str());
  outfile << m_Ntotal <<std::endl;
  for(unsigned int i=0; i< m_pdfweights->GetNWeights(); ++i){
    outfile<< m_sumofweights[i] << " ";
  }
  outfile.close();

  delete m_pdfweights;

}

LQTopLepPDFWeightsModule::LQTopLepPDFWeightsModule(Context & ctx){

  m_pdfname = "NNPDF30_lo_as_0130";
  m_pdfweights = new PDFWeights(m_pdfname);

  m_Ntotal=0;

  m_sumofweights.clear();
  for(unsigned int i=0; i< m_pdfweights->GetNWeights(); ++i){
    m_sumofweights.push_back(0);
  }

  m_oname = ctx.get("dataset_version");
  m_postfix = ctx.get("PDFWeightsPostfix");
  m_outpath = ctx.get("PDFWeightsOutpath");
}

bool LQTopLepPDFWeightsModule::process(Event & event) {


  std::vector<double> weights = m_pdfweights->GetWeightList(event);
  if(!weights.size()) return false;
  m_Ntotal++;

  for(unsigned int i=0; i< m_pdfweights->GetNWeights(); ++i){
    m_sumofweights[i]+=weights[i];

  }

  return true;
}

UHH2_REGISTER_ANALYSIS_MODULE(LQTopLepPDFWeightsModule)
