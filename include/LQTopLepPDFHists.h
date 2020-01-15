#pragma once

#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/TTbarGen.h"
#include "UHH2/LQTopLep/include/LQReconstructionHypothesisDiscriminators.h"
#include "UHH2/LQTopLep/include/LQReconstructionHypothesis.h"
//#include "UHH2/LQToTopMu/include/LQGen.h"
#include "UHH2/common/include/PDFWeights.h"


namespace uhh2examples {

  /**  \brief Example class for booking and filling histograms
  *
  * NOTE: This class uses the 'hist' method to retrieve histograms.
  * This requires a string lookup and is therefore slow if you have
  * many histograms. Therefore, it is recommended to use histogram
  * pointers as member data instead, like in 'common/include/ElectronHists.h'.
  */
  class LQTopLepPDFHists: public uhh2::Hists {
  public:
    // use the same constructor arguments as Hists for forwarding:
    LQTopLepPDFHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;

  protected:
    std::vector<TString> histo_names1, histo_names2;
    uhh2::Event::Handle<bool> h_is_mlq_reconstructed;
    uhh2::Event::Handle<float> h_mlq;
    bool is_mc, take_ntupleweights;
    TString m_oname;
    std::unique_ptr<PDFWeights> m_pdfweights;


    virtual ~LQTopLepPDFHists();
  };

}
