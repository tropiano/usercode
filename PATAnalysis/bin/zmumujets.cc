#include <iostream>
//#include "Signal.h"
#include "Firenze/PATAnalysis/include/Background.h"
#include "TFile.h"
#include "TTree.h"
//#include "RooUnfold/RooUnfoldResponse.h"
#include "TParameter.h"
#include "TDSet.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TProof.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"



using namespace std;


int main(){
  TProof * p = TProof::Open("");
  

  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  //gSystem->Load("libFWCoreTFWLiteSelector");
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec( ".x remote.C" ); 

  TDSet train( "ciao", "Events");
  train.Add("/raid/lenzip/CMSSW/CMSSW_3_1_2/src/Firenze/Reduction/test/ZmumuPythia/res/test_pat_174.root");
  //TFile trainFile("/raid/lenzip/CMSSW/CMSSW_3_1_2/src/Firenze/Reduction/test/ZmumuPythia/res/test_pat_174.root");
  train.Print();
  //train.GetTreeHeader(p)->Print();

  //TTree* train = (TTree*) trainFile.Get("Events");

  //TFile* outtrain = new TFile("train.root", "RECREATE");
  TNamed* outtrain = new TNamed("OutputFile", "train.root");

  TList config;
  TParameter<double>* csec = new TParameter<double>("CrossSection", 1200);
  TParameter<double>* lumi = new TParameter<double>("Luminosity", 100);
  config.Add(csec);
  config.Add(lumi);
  config.Add(outtrain);

  p->AddInput(csec);
  p->AddInput(lumi);
  p->AddInput(outtrain);

  //processing signal
  //Background trainsignal; //517 is the target luminosity and actually the real lumy of one of the two samples
  //trainsignal.SetInputList(&config);
  //p->Process(&train, "FWLiteTSelector", "", 500, 2000);//&trainsignal);
  p->Process(&train, "Background", "");//&trainsignal);
  //train.Process("FWLiteTSelector");
  p->ClearInput();
  p->Close();
  //train->Process(&trainsignal);

}
