#include "Firenze/PATAnalysis/include/EfficiencyAnalyzerElectron.h"
#include "Firenze/PATAnalysis/include/TagAndProbeAnalyzerElectron.h"
#include <iostream>

#include "TH1D.h"
#include "TGraphAsymmErrors.h"

EfficiencyAnalyzerElectron::EfficiencyAnalyzerElectron(TFile* input, TFile* output, string dirname, string dirname1, string dataset, TFile* training_signal, TFile* training_background): 

_dir(0), _dir1(0), _dirname(dirname), _dirname1(dirname1), _dataset(dataset), _output(output), 
_training_signal(training_signal), _training_background(training_background){
  
	if(input&&_dirname!="") _dir = (TDirectory*) input->Get(dirname.c_str());
  
	if(input&&_dirname1!="") _dir1 = (TDirectory*) input->Get(dirname1.c_str());
}


void EfficiencyAnalyzerElectron::analyze(int bin, std::string fitOptions){
  
	if(_dirname!=""){
		if (!_dir) {
			std::cout << "### Error in EfficiencyAnalyzerElectron, dir \""<<_dirname.c_str()<<"\" not found in file"<< std::endl;
			return;
		}
	}
  
	if(_dirname1!=""){
		if (!_dir1) {
			cout<<"### dir1"<<endl;
			std::cout << "### Error in EfficiencyAnalyzerElectron, dir \""<<_dirname.c_str()<<"\" not found in file"<< std::endl;
			return;
		}
	}
  
	_output->cd();
  
	TagAndProbeAnalyzerElectron tpana(_dir, _output, _dir1, _dataset, true, _training_signal, _training_background);
	tpana.analyze(bin, fitOptions);

}
