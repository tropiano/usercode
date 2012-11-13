#ifndef RegisteredAnalyses
#define RegisteredAnalyses

#include "MyPatAnalysis.h"
#include "GenElectron.h"
#include "RecoElectron.h"
#include "EfficiencyElectron.h"
#include "RecoElectronNtuple.h"
//#include "GenMuon.h"
//#include "RecoMuon.h"
//#include "EfficiencyMuon.h"
//#include "RecoMuonNtuple.h"
//#include "DeltaRAnalyzerMuon.h"
#include <map>

typedef MyPatAnalysis* (*anabuilder_fn)();
typedef std::map<std::string,anabuilder_fn> AnalysisBuilders;
typedef AnalysisBuilders (*anabuilders_fn)();

inline AnalysisBuilders getAnalysisBuilders(){
  AnalysisBuilders symbols;
  symbols["GenElectron"] = GenElectron::create;
  symbols["RecoElectron"] = RecoElectron::create;
  symbols["EfficiencyElectron"] = EfficiencyElectron::create;
  symbols["RecoElectronNtuple"] = RecoElectronNtuple::create;
//  symbols["GenMuon"] = GenMuon::create;
//  symbols["RecoMuon"] = RecoMuon::create;
//  symbols["EfficiencyMuon"] = EfficiencyMuon::create;
//  symbols["RecoMuonNtuple"] = RecoMuonNtuple::create;
//  symbols["DeltaRAnalyzerMuon"] = DeltaRAnalyzerMuon::create;

  return symbols;
}  



#endif
