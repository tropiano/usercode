#ifndef PATAnalysis_EfficiencyAnalyzerMuon_h
#define PATAnalysis_EfficiencyAnalyzerMuon_h

#include "TFile.h"
#include <string> 
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include <iostream>

class EfficiencyAnalyzerMuon {

  public:
    EfficiencyAnalyzerMuon(TFile* input, TFile* output, TFile* sec_input, std::string dir);
    
    ~EfficiencyAnalyzerMuon(){};

    void analyze(std::string="", double scale=1.);

  private:
    
    TGraphAsymmErrors inclusiveEfficiency(TH1* num, TH1* den, const char* name, const char* title) const;
    void print(const TGraphAsymmErrors& graph, std::ostream& os) const;

    TDirectory* _dir;
    TDirectory* _dir1;
    TFile* _output;
    TFile* _sec_input;
};


#endif
