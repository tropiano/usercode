#include "Firenze/PATAnalysis/include/Unfolding.h"
#include <iostream>

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
#include "RooUnfoldBinByBin.h"
#include "RooUnfoldSvd.h"

#include "TH1D.h"
#include "TH2D.h"

Unfolding::Unfolding(TFile* training, TFile* measured, TFile* output, std::string response_dirname, std::string reco_dirname): _output(output){
	_response_dir = (TDirectory*) training->Get(response_dirname.c_str());
	_reco_dir     = (TDirectory*) measured->Get(reco_dirname.c_str());
}


void Unfolding::analyze(){

	if (!_response_dir) {
		std::cout << "Error in Unfolding, directory containing response matrix not found" << std::endl;
		return;
	}
	if (!_reco_dir){
		std::cout << "Error in Unfolding, directory containing reco distribution not found" << std::endl;  
		return;
	}

	TH1D* measured = (TH1D*) _response_dir->Get("measured");
	TH1D* truth = (TH1D*) _response_dir->Get("truth");
	TH2D* ResponseMatrix = (TH2D*) _response_dir->Get("ResponseMatrix");

	if ((!measured)||(!truth)||(!ResponseMatrix)) {
		std::cout << "Error in Unfolding, measured or truth or ResponseMatrix plot on training file not found" << std::endl;
		return;
	}

	RooUnfoldResponse* response = new RooUnfoldResponse(measured, truth, ResponseMatrix);
  
	std::cout << "response nbins measured " << response->GetNbinsMeasured() << std::endl;
	std::cout << "response nbins truth " << response->GetNbinsTruth() << std::endl;
 
	TH1* recoMulti = (TH1*) _reco_dir->Get("measured"); 
	if (!recoMulti){
		std::cout << "Error in Unfolding, measured plot on measured file not found" << std::endl;   
		return;
	}
	recoMulti->Print();

	RooUnfoldBayes bayes("multiBayesUnfolding", "multiBayesUnfolding");
	bayes.Setup(response, recoMulti, 4);
	RooUnfoldBinByBin bin("multiBinByBinUnfolding", "multiBinByBinUnfolding");
	bin.Setup(response, recoMulti);  
	RooUnfoldSvd svd("multiSvdUnfolding", "multiSvdUnfolding");
	svd.Setup(response, recoMulti);

	bayes.Print();

	TH1* _unfBayes = bayes.Hreco();
	TH1* _unfBin   = bin.Hreco();
	TH1* _unfSvd   = svd.Hreco();

	_output->cd();

	_unfBayes->Write(); 
	_unfBin->Write();
	_unfSvd->Write();
}
