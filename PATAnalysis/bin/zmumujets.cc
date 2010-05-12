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
  gEnv->SetValue("Proof.Sandbox", "/data/lenzip/RapGap/CMSSW_3_5_6/src/Firenze/PATAnalysis/bin/proof");

  TProof * p = TProof::Open("");
  //p->SetParallel(4);
 
  double targetLumi = 100.;
  

  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec( ".x /data/lenzip/RapGap/CMSSW_3_5_6/src/Firenze/PATAnalysis/bin/remote.C" );

  TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
  pl->Save("*","/data/lenzip/RapGap/CMSSW_3_5_6/src/Firenze/PATAnalysis/bin/Log.txt");


  TDSet* trainingDS = getDS("data.txt");
  TNamed* configtraining = new TNamed("ConfigFile", "/data/lenzip/RapGap/CMSSW_3_5_6/src/Firenze/PATAnalysis/bin/config_data_zmumu.py");
  p->AddInput(configtraining);
  p->Process(trainingDS, "FWLiteTSelector");
  delete trainingDS;
  p->Close();

  TFile in("zmumu.root");
  EfficiencyAnalyzerMuon yield_ana(&in, 0, 0, "EfficiencyMuonVSRecoMulti");
  yield_ana.analyze("yield.tex");


}
