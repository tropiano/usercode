#ifndef PATAnalysis_EfficiencyAnalyzerElectron_h
#define PATAnalysis_EfficiencyAnalyzerElectron_h

#include "TFile.h"
#include <string> 

class EfficiencyAnalyzerElectron {

  public:
    EfficiencyAnalyzerElectron(TFile* input, TFile* output, std::string dir, TFile* training_signal=0, TFile* training_background=0);
    
    ~EfficiencyAnalyzerElectron(){};

    void analyze(int, std::string);

  private:
    TDirectory* _dir;
    TFile* _output;
    TFile* _training_signal;
    TFile* _training_background;
    
};


#endif
