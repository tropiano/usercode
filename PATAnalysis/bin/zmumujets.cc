#include <iostream>
//#include "Signal.h"
//#include "Firenze/PATAnalysis/include/Background.h"
#include "TFile.h"
#include "TTree.h"
//#include "RooUnfold/RooUnfoldResponse.h"
#include "TParameter.h"
#include "TFileMerger.h"
#include "TDSet.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TProof.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include <fstream>


using namespace std;

TDSet* getDS(const char* filename){
  TDSet* out = new TDSet( "dataset", "Events");
  ifstream infile;
  infile.open(filename);
  string datafile;
  while(getline ( infile, datafile )){
    out->Add(datafile.c_str());
  }
  return out; 
}


int main(){
  TProof * p = TProof::Open("");
  
  double targetLumi = 100.;
  

  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec( ".x remote.C" ); 

  //process the background
  TDSet* muDS = getDS("mudata.txt") ;
  TNamed* configbg = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/CMSSW_3_1_4/src/Firenze/PATAnalysis/bin/config_Mu.py");
  p->AddInput(configbg);
  p->Process(muDS, "FWLiteTSelector");
  p->ClearInput();
  delete muDS;

  //process the signal
  TDSet* signalDS = getDS("signaldata.txt");
  TNamed* configsignal = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/CMSSW_3_1_4/src/Firenze/PATAnalysis/bin/config_signal.py");
  p->AddInput(configsignal);       
  p->Process(signalDS, "FWLiteTSelector");
  delete signalDS;

  //merge results
  TFileMerger fm(true);
  fm.AddFile("Mu.root");
  fm.AddFile("signal.root");
  fm.OutputFile("total.root");
  fm.Merge();

  p->Close();

}
