#ifndef TagAndProbeAnalyzerElectron_h
#define TagAndProbeAnalyzerElectron_h

#include "TDirectory.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include <string>
#include <fstream>
#include <iostream>

#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooFitResult.h"
#include "RooCategory.h"

//#include "Utilities.h"


class TagAndProbeAnalyzerElectron {
public :

    TagAndProbeAnalyzerElectron(TDirectory* input, TFile* output, TDirectory* sec_input, std::string name, bool performfits=true, TFile* training_signal=0, TFile* training_background=0);

    ~TagAndProbeAnalyzerElectron();

    void analyze(unsigned int nbins, std::string option="signal");
    
    TGraphAsymmErrors createDoubleEfficiency(const TGraphAsymmErrors& single) const;
    
    TGraphAsymmErrors createAsymmCutEfficiency(const TGraphAsymmErrors& single0, const TGraphAsymmErrors& single1) const;

private:  
  std::pair<RooFitResult*, RooRealVar*> fit(RooAbsData* data, const char* name, std::string option = "signal") ;
  bool _initialized;
  TDirectory* _input;
  TDirectory* _sec_input;
  TFile* _output;
  std::string _name;
  std::string _dataset;
  bool _performfits;
  TFile *_training_signal, *_training_background;
  RooDataSet* _rootree; 
  RooDataSet* _rootree1;
  //tree vars
  RooRealVar _mass;
  RooRealVar _bin;
  RooRealVar _probe, _weight;
  RooCategory _passprobe_cat;
  RooArgSet* _argset;
  
};

#endif

