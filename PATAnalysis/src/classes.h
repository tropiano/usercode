#include "Firenze/PATAnalysis/include/FWLiteTSelector.h"
#include "Firenze/PATAnalysis/include/RecoMuon.h"
#include "Firenze/PATAnalysis/include/GenMuon.h"
#include "Firenze/PATAnalysis/include/EfficiencyMuon.h"

struct SignalMuon : public FWLiteTSelector<RecoMuon, GenMuon, EfficiencyMuon> {};

namespace {
  namespace {
  }
}

