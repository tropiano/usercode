#ifndef RegisteredAnalyses
#define RegisteredAnalyses

#include "MyPatAnalysis.h"
#include "GenMuon.h"
#include "GenElectron.h"
#include "RecoMuon.h"
#include "RecoTrack.h"
#include "RecoJet.h"
#include "RecoElectron.h"
#include "RecoElectronNtuple.h"
#include "EfficiencyElectron.h"
#include "EfficiencyMuon.h"
#include "RecoRapidityGaps.h"
#include "GenRapidityGaps.h"
#include "TestAnalysis.h"
#include <map>

typedef MyPatAnalysis* (*anabuilder_fn)();
typedef std::map<std::string,anabuilder_fn> AnalysisBuilders;
typedef AnalysisBuilders (*anabuilders_fn)();

inline AnalysisBuilders getAnalysisBuilders(){
  AnalysisBuilders symbols;
  symbols["GenElectron"] = GenElectron::create;
  symbols["GenMuon"] = GenMuon::create;
  symbols["RecoElectron"] = RecoElectron::create;
  symbols["RecoElectronNtuple"] = RecoElectronNtuple::create;
  symbols["RecoMuon"] = RecoMuon::create;
  symbols["RecoTrack"] = RecoTrack::create;
  symbols["RecoJet"] = RecoJet::create;
  symbols["EfficiencyElectron"] = EfficiencyElectron::create;
  symbols["EfficiencyMuon"] = EfficiencyMuon::create;
  symbols["RecoRapidityGaps"] = RecoRapidityGaps::create;
  symbols["GenRapidityGaps"] = GenRapidityGaps::create;
  symbols["TestAnalysis"] = TestAnalysis::create;

  return symbols;
}  



#endif
