#include "Firenze/PATAnalysis/include/EfficiencyAnalyzerElectron.h"
#include "Firenze/PATAnalysis/include/TagAndProbeAnalyzer.h"
#include <iostream>

#include "TH1D.h"
#include "TGraphAsymmErrors.h"

EfficiencyAnalyzerElectron::EfficiencyAnalyzerElectron(TFile* input, TFile* output, std::string dirname, TFile* training_signal, TFile* training_background): 

_dir(0), _output(output), 
_training_signal(training_signal), _training_background(training_background)

{
  
  _dir = (TDirectory*) input->Get(dirname.c_str());
  
}

void EfficiencyAnalyzerElectron::analyze(int bin, std::string fitOptions){
  if (!_dir) {
    std::cout << "Error in EfficiencyAnalyzerElectron, dir not found in file" << std::endl;
    return;
  }
  
  _output->cd();
  
  TagAndProbeAnalyzer tpana(_dir, _output, "dataset");
  tpana.analyze(bin, fitOptions);

}
