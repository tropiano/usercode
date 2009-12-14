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

/*  string cfgName = "config_Zee.py";
  string sourceList = "files_Zee.txt";
  string outputName = "Signal_NewSel_RobustLoose_Iso03";
  string electronID = "eidRobustLoose";
  double xsec = 1944.;
  double targetLumi = 100.;
  string Norm = "True";
  bool Log = false; */

  string cfgName = "config_Zee.py";
  string sourceList = "files_test.txt";
  string outputName = "Signal_test";
  string electronID = "eidRobustLoose";
  double xsec = 1944.;
  double targetLumi = 100.;
  string Norm = "True";
  int EventsPerFile = 0;
  bool GEN = true;
  bool RECO = true;
  bool EFF = true;
  bool NTUPLE = true;
  bool Log = false;
  
  string cfgPath = "/afs/cern.ch/user/s/sfrosali/scratch0/Zjets/Commit/CMSSW_3_1_4/src/Firenze/PATAnalysis/bin/";
  cfgPath += cfgName.c_str();

  makeCfg(GEN, RECO, EFF, NTUPLE, cfgPath.c_str(), sourceList.c_str(), outputName.c_str(), Norm.c_str(), EventsPerFile, electronID.c_str(), xsec, targetLumi);

  gEnv->SetValue("Proof.Sandbox", "/raid/sfrosali/.proof");

  TProof * p = TProof::Open("");
  
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec(".x remote.C");

  TDSet* SignalDS = getDS(sourceList.c_str());
  
  TNamed* configsignal = new TNamed("ConfigFile", cfgPath.c_str());
  p->AddInput(configsignal);
  p->Process(SignalDS, "FWLiteTSelector");
  
 if(Log){ 
 TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
 pl->Save("0.0","/afs/cern.ch/user/s/sfrosali/scratch0/Zjets/Commit/CMSSW_3_1_4/src/Firenze/PATAnalysis/bin/Log.txt");
 }
  
  p->ClearInput();
  delete SignalDS;

  p->Close();
   
return 0;

}
