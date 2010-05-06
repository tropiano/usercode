#ifndef PATAnalysis_EfficiencyAnalyzerMuon_h
#define PATAnalysis_EfficiencyAnalyzerMuon_h

#include "TFile.h"
#include <string> 

class EfficiencyAnalyzerMuon {

  public:
    EfficiencyAnalyzerMuon(TFile* input, TFile* output, TFile* sec_input, std::string dir);
    
    ~EfficiencyAnalyzerMuon(){};

    void analyze();

  private:
    TDirectory* _dir;
    TDirectory* _dir1;
    TFile* _output;
    TFile* _sec_input;
};


#endif
