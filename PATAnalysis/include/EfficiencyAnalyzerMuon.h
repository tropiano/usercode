#ifndef PATAnalysis_EfficiencyAnalyzerMuon_h
#define PATAnalysis_EfficiencyAnalyzerMuon_h

#include "TFile.h"
#include <string> 

class EfficiencyAnalyzerMuon {

  public:
    EfficiencyAnalyzerMuon(TFile* input, TFile* output, std::string dir);
    
    ~EfficiencyAnalyzerMuon(){};

    void analyze();

  private:
    TDirectory* _dir;
    TFile* _output;
};


#endif
