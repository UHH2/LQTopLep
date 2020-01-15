#pragma once

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "UHH2/LQTopLep/include/LQReconstructionHypothesis.h"
//#include "UHH2/common/include/TTbarGen.h"
//#include "UHH2/LQToTopE/include/LQGen.h"

const LQReconstructionHypothesis * get_best_hypothesis(const std::vector<LQReconstructionHypothesis> & hyps, const std::string & label);

class LQChi2Discriminator: public uhh2::AnalysisModule {
public:
    struct cfg {
        std::string discriminator_label;
        cfg(): discriminator_label("Chi2"){}
    };

    LQChi2Discriminator(uhh2::Context & ctx, const std::string & rechyps_name="LQHypotheses", const cfg & config = cfg());
    virtual bool process(uhh2::Event & event) override;

private:
    uhh2::Event::Handle<std::vector<LQReconstructionHypothesis>> h_hyps;
    uhh2::Event::Handle<bool> h_is_mlq_reconstructed;
    uhh2::Event::Handle<float> h_mlq, h_chi2;
    cfg config;
};



/*class LQHadronicChi2Discriminator: public uhh2::AnalysisModule {
public:
struct cfg {
std::string discriminator_label;
cfg(): discriminator_label("Chi2Hadronic"){}
};

LQHadronicChi2Discriminator(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
virtual bool process(uhh2::Event & event) override;

private:
uhh2::Event::Handle<std::vector<LQReconstructionHypothesis>> h_hyps;
cfg config;
};*/


/** \brief Top-DeltaR quality flag for Monte-Carlo
*
* Requires a TTbarGen object in the event (see TTbarGen.h).
*
* Writes "TopDRMC" quality flags to the reconstruction hypotheses, which is the sum of Delta R values
* between the generated and reconstructed top quarks for the top and anti-top.
*
* NOTE: This used to be called "BestPossible" in UHHAnalysis.
*/


/* not needed I think

class LQTopDRMCDiscriminator: public uhh2::AnalysisModule {
public:
struct cfg {
std::string ttbargen_name;
std::string discriminator_label;
cfg(): ttbargen_name("ttbargen"), discriminator_label("TopDRMC"){}
};

LQTopDRMCDiscriminator(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
virtual bool process(uhh2::Event & event) override;

private:

uhh2::Event::Handle<std::vector<LQReconstructionHypothesis>> h_hyps;
uhh2::Event::Handle<TTbarGen> h_ttbargen;
cfg config;
};
*/



/** \brief Try to match the reconstruction hypotheses to Monte-Carlo truth, jet-by-jet
*
* Requires a TTbarGen object in the event (see TTbarGen.h).
*
* Writes a "CorrectMatch" quality flags to the reconstruction hypotheses, which is the sum of Delta R values
* between the four generated and reconstructed matrix-element final-state partons and the DeltaR between the
* true neutrino and the reconstructed neutrino. The discriminator is set to infinity
* if one of the final-state partons could not be matched to a jet within Delta R < 0.3 (note that no
* such match is done for the neutrino).
*
* NOTE: This class only works for events which are (on gen-level) electron+jets or muon+jets. Otherwise, all discriminator
* values are set to +infinity. The reconstructed lepton is ignored in this discriminator criterion.
*/


/* not needed I think

class LQCorrectMatchDiscriminator: public uhh2::AnalysisModule {
public:
struct cfg {
std::string ttbargen_name;
std::string LQLQbargen_name;
std::string discriminator_label;
cfg(): ttbargen_name("ttbargen"), LQLQbargen_name("LQLQbargen"), discriminator_label("CorrectMatch"){}
};

LQCorrectMatchDiscriminator(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
virtual bool process(uhh2::Event & event) override;

private:

uhh2::Event::Handle<std::vector<LQReconstructionHypothesis>> h_hyps;
uhh2::Event::Handle<TTbarGen> h_ttbargen;
uhh2::Event::Handle<LQGen> h_LQLQbargen;
cfg config;
};
*/
