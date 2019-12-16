#pragma once

#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/TTbarGen.h"


namespace uhh2examples {

class LQTopLepTriggerHists: public uhh2::Hists {

  public:
    LQTopLepTriggerHists(uhh2::Context & ctx, const std::string & dirname);
    virtual void fill(const uhh2::Event & ev) override;

  protected:
    virtual ~LQTopLepTriggerHists();

};

}
