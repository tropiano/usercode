#ifndef PATAnalysis_Unfolding_h
#define PATAnalysis_Unfolding_h

#include "TFile.h"
#include <string> 

class  Unfolding{

  public:
    Unfolding(TFile* training, TFile* measured, TFile* output, std::string response_dirname, std::string reco_dirname);
    
    ~Unfolding(){};

    void analyze();

  private:
    TDirectory* _response_dir;
    TDirectory* _reco_dir;
    TFile* _output;
};


#endif
