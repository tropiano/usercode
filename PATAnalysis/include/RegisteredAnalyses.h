#ifndef RegisteredAnalyses
#define RegisteredAnalyses

#include "MyPatAnalysis.h"
#include "GenMuon.h"
#include "GenElectron.h"
#include "RecoMuon.h"
#include "RecoElectron.h"
#include "RecoElectronNtuple.h"
#include "EfficiencyElectron.h"
#include "EfficiencyMuon.h"
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
  symbols["EfficiencyElectron"] = EfficiencyElectron::create;
  symbols["EfficiencyMuon"] = EfficiencyMuon::create;

  return symbols;
}  



#endif
