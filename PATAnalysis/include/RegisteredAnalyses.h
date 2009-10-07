#ifndef RegisteredAnalyses
#define RegisteredAnalyses

#include "MyPatAnalysis.h"
#include "GenMuon.h"
#include "RecoMuon.h"
#include "EfficiencyMuon.h"
#include <map>

typedef MyPatAnalysis* (*anabuilder_fn)();
typedef std::map<std::string,anabuilder_fn> AnalysisBuilders;
typedef AnalysisBuilders (*anabuilders_fn)();

inline AnalysisBuilders getAnalysisBuilders(){
  AnalysisBuilders symbols;
  symbols["GenMuon"] = GenMuon::create;
  symbols["RecoMuon"] = RecoMuon::create;
  symbols["EfficiencyMuon"] = EfficiencyMuon::create;

  return symbols;
}  



#endif
