#include <iostream>
//#include "Signal.h"
#include "Firenze/PATAnalysis/include/EfficiencyAnalyzerMuon.h"
#include "Firenze/PATAnalysis/include/Unfolding.h"
#include "TFile.h"
#include "TTree.h"
//#include "RooUnfold/RooUnfoldResponse.h"
#include "TParameter.h"
#include "TFileMerger.h"
#include "TDSet.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TEnv.h"
#include "TProof.h"
#include "TProofLog.h"
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
  //p->SetParallel(4);
  

  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec( ".x /data/lenzip/RapGap/CMSSW_3_5_6/src/Firenze/PATAnalysis/bin/remote.C" );

  TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
  pl->Save("*","/raid/lenzip/CMSSW/test/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/Log.txt");



  TDSet* dsdata = getDS("test.txt");//"tracks_data.txt") ;
  TNamed* configdata = new TNamed("ConfigFile", "/data/lenzip/RapGap/CMSSW_3_5_6/src/Firenze/PATAnalysis/bin/config_Dec09_data.py");
  p->AddInput(configdata);
  p->Process(dsdata, "FWLiteTSelector");
  p->ClearInput();
  delete dsdata; 
/*  
  TDSet* dsmc = getDS("tracks_mc.txt") ;
  TNamed* configmc = new TNamed("ConfigFile", "/data/lenzip/RapGap/CMSSW_3_5_6/src/Firenze/PATAnalysis/bin/config_Dec09_mc.py");
  p->AddInput(configmc);
  p->Process(dsmc, "FWLiteTSelector");
  p->ClearInput();
  delete dsmc;*/
  p->Close();
}
