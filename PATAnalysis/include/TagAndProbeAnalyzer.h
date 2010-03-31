#ifndef TagAndProbeAnalyzer_h
#define TagAndProbeAnalyzer_h

#include "TDirectory.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include <string>
#include <fstream>
#include <iotream>

#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooFitResult.h"
#include "RooCategory.h"

//#include "Utilities.h"


class TagAndProbeAnalyzer {
public :

    TagAndProbeAnalyzer(TDirectory* input, TFile* output, std::string name, bool performfits=true, TFile* training_signal=0, TFile* training_background=0);

    ~TagAndProbeAnalyzer();

    void analyze(unsigned int nbins, std::string option="signal");
    
    TGraphAsymmErrors createDoubleMuonEfficiency(const TGraphAsymmErrors& single) const;

private:  
  std::pair<RooFitResult*, RooRealVar*> fit(RooAbsData* data, const char* name, std::string option = "signal") ;
  bool _initialized;
  TDirectory* _input;
  TFile* _output;
  std::string _name;
  bool _performfits;
  TFile *_training_signal, *_training_background;
  RooDataSet* _rootree;
  //tree vars
  RooRealVar _mass;
  RooRealVar _bin;
  RooRealVar _probe, /*_passprobe,*/ _weight;
  RooCategory _passprobe_cat;
  RooArgSet* _argset;
  
};

#endif

