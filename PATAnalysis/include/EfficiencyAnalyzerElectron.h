#ifndef PATAnalysis_EfficiencyAnalyzerElectron_h
#define PATAnalysis_EfficiencyAnalyzerElectron_h

#include "TFile.h"
#include <string> 

class EfficiencyAnalyzerElectron {

  public:
    EfficiencyAnalyzerElectron(TFile* input=0, TFile* output=0, std::string dir="", std::string dir1="", std::string dataset="dataset", TFile* training_signal=0, TFile* training_background=0);
    
    ~EfficiencyAnalyzerElectron(){};

    void analyze(int, std::string);

  private:
    TDirectory* _dir;
    TDirectory* _dir1;
    std::string _dirname;
    std::string _dirname1;
    std::string _dataset;
    TFile* _output;
    TFile* _training_signal;
    TFile* _training_background;
    
};


#endif
