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
 
  double targetLumi = 100.;
  

  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec( ".x /raid/lenzip/CMSSW/test/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/remote.C" );

  TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
  pl->Save("*","/raid/lenzip/CMSSW/test/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/Log.txt");



/*  
  //process the background
  TDSet* muDS = getDS("mudata.txt") ;
  TNamed* configbg = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/config_signal.py");
  p->AddInput(configbg);
  p->Process(muDS, "FWLiteTSelector");
  p->ClearInput();
  delete muDS;
*/  
/*  TFile* in = new TFile("signal.root");
  TFile* out = new TFile("Mu_Eff.root", "RECREATE");

  EfficiencyAnalyzerMuon mueffana(in, out, "EfficiencyMuon");
  mueffana.analyze();

  delete out;
*/  
/*
  //process the signal
  TDSet* signalDS = getDS("signaldata.txt");
  TNamed* configsignal = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/config_signal.py");
  p->AddInput(configsignal);       
  p->Process(signalDS, "FWLiteTSelector");
  delete signalDS;

  TFile* in = new TFile("signal.root");
  TFile* out = new TFile("signal_eff.root", "RECREATE");

  EfficiencyAnalyzerMuon mueffana(in, out, "EfficiencyMuon");
  mueffana.analyze();

  delete out;
*/ 

/*
  TDSet* qcdDS = getDS("qcd.txt");
  TNamed* configqcd = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/config_background_QCD.py");
  p->AddInput(configqcd);       
  p->Process(qcdDS, "FWLiteTSelector");
*/  
/*  TNamed* configqcd_samesign = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/config_background_samesign.py");
  p->AddInput(configqcd_samesign);
  p->Process(qcdDS, "FWLiteTSelector");
*/  
  //delete qcdDS;

/*  
  TDSet* ttbarDS = getDS("ttbar.txt");
  TNamed* configttbar = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/config_background_TTbar.py");
  p->AddInput(configttbar);
  p->Process(ttbarDS, "FWLiteTSelector");
  delete ttbarDS;

  TDSet* wDS = getDS("w.txt");
  TNamed* configw = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/config_background_Wjets.py");
  p->AddInput(configw);
  p->Process(wDS, "FWLiteTSelector");
  delete wDS;
*/
  TDSet* trainingDS = getDS("zmumu_training.txt");
  TNamed* configtraining = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/test/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/config_signal_training.py");
  p->AddInput(configtraining);
  p->Process(trainingDS, "FWLiteTSelector");
  delete trainingDS;
 
  TDSet* signalDS = getDS("zmumu.txt");
  TNamed* configsignal = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/test/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/config_signal.py");
  p->AddInput(configsignal);
  p->Process(signalDS, "FWLiteTSelector");
  delete signalDS;
  TFile trainingoutput("training.root");
  TFile measuredoutput("signal.root");
  TFile unfolding("unfolding.root", "RECREATE");
  Unfolding unfoldingAnalyzer(&trainingoutput, &measuredoutput, &unfolding, "EfficiencyMuon", "RecoMuon");
  unfoldingAnalyzer.analyze();
  
/*

  //merge results
  TFileMerger fm(true);
  fm.AddFile("qcd.root");
  fm.AddFile("ttbar.root");
  fm.AddFile("signal.root");
  fm.OutputFile("total.root");
  fm.Merge();
*/
/*
  TDSet* dsdata = getDS("tracks_data.txt") ;
  TNamed* configdata = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/config_Dec09_data.py");
  p->AddInput(configdata);
  p->Process(dsdata, "FWLiteTSelector");
  p->ClearInput();
  delete dsdata;

  TDSet* dsmc = getDS("tracks_mc.txt") ;
  TNamed* configmc = new TNamed("ConfigFile", "/raid/lenzip/CMSSW/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/config_Dec09_mc.py");
  p->AddInput(configmc);
  p->Process(dsmc, "FWLiteTSelector");
  p->ClearInput();
  delete dsmc;
  p->Close();
*/
}
