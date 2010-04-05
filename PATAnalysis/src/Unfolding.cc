#include "Firenze/PATAnalysis/include/Unfolding.h"
#include <iostream>

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
#include "RooUnfoldBinByBin.h"
#include "RooUnfoldSvd.h"

#include "TH1D.h"

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
  RooUnfoldResponse* response = (RooUnfoldResponse*) _response_dir->Get("ResponseMatrix");
  if (!response) {
    std::cout << "Error in Unfolding, ResponseMatrix not found" << std::endl;
    return;
  }
  
  std::cout << "response nbins measured " << response->GetNbinsMeasured() << std::endl;
  std::cout << "response nbins truth " << response->GetNbinsTruth() << std::endl;
 
  TH1* recoMulti = (TH1*) _reco_dir->Get("recMulti"); 
  if (!recoMulti){
    std::cout << "Error in Unfolding, recMulti plot not found" << std::endl;   
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
