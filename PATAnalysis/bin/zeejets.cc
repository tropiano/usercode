#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TParameter.h"
#include "TFileMerger.h"
#include "TDSet.h"
#include "TChain.h"
#include "TSystem.h"
#include "TEnv.h"
#include "TROOT.h"
#include "TProof.h"
#include "TProofLog.h"
#include "Firenze/PATAnalysis/include/ZeeMainUtilities.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

using namespace std;

int main() {

  //Job
  string cfgName = "config.py";
  string sourceList = "files_test.txt";
  string outputName = "test";
  string electronID = "eidRobustLoose";
  
  //Normalization
  double xsec = 1300.;
  double targetLumi = 100.;
  string Norm = "True";
  int EventsPerFile = 0;
  string Sumw2= "True";
  
  //Gen Particle Matching
  string GenParticleMatch = "True";
  
  //Modules
  bool GEN = true;
  bool RECO = true;
  bool EFF = true;
  bool NTUPLE = false;
  
  //Cuts
  int Acc = 1;
  int Qual = 0;
  int Imp = 2;
  int Iso = 3;
  int EiD = 4;
  
  //Number of CPUs
  int CPU = 8;
  
  //Log
  bool Log = false;
  
  //DO NOT FORGET THE SLASH AT THE END OF THE PATH
  string path = "/raid/sfrosali/Zjets/Commit/CMSSW_3_3_6/src/Firenze/PATAnalysis/bin/";

  makeCfg(GEN, RECO, EFF, NTUPLE, Acc, Qual, Imp, Iso, EiD, path.c_str(), cfgName.c_str(), sourceList.c_str(), outputName.c_str(), Norm.c_str(), Sumw2.c_str(), EventsPerFile, electronID.c_str(), xsec, targetLumi, GenParticleMatch.c_str());

  gEnv->SetValue("Proof.Sandbox", "/raid/sfrosali/.proof");

  TProof * p = TProof::Open("");
  
  p->SetParallel(CPU);
  
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec(".x remote.C");

  TDSet* SignalDS = getDS(sourceList.c_str());
  
  string cfgPath = path;
  cfgPath+=cfgName;
  
  TNamed* configsignal = new TNamed("ConfigFile", cfgPath.c_str());
  p->AddInput(configsignal);
  p->Process(SignalDS, "FWLiteTSelector");
  
  if(Log){ 
  TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
  pl->Save("0.1",path.c_str());
  }
  
  p->ClearInput();
  delete SignalDS;

  p->Close();
   
return 0;

}
