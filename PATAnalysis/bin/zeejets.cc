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
  string Sumw2= "False";
  bool Log = false; */

  string cfgName = "config_TTbar.py";
  string sourceList = "TTbar_list.txt";
  string outputName = "TTbar_NewSel_RobustLoose_Iso01_PlotIso";
  string electronID = "eidRobustLoose";
  double xsec = 242.8;
  double targetLumi = 100.;
  string Norm = "True";
  string Sumw2= "False";
  bool Log = false; 

  makeCfg(cfgName.c_str(), sourceList.c_str(), outputName.c_str(), Norm.c_str(), Sumw2.c_str(), electronID.c_str(), xsec, targetLumi);

  gEnv->SetValue("Proof.Sandbox", "/raid/sfrosali/.proof");

  TProof * p = TProof::Open("");
  
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec(".x remote.C");

  TDSet* SignalDS = getDS(sourceList.c_str());
  
  string cfgPath = "/afs/cern.ch/user/s/sfrosali/scratch0/Zjets/CMSSW_3_1_4/src/Firenze/PATAnalysis/bin/python/";
  cfgPath += cfgName.c_str();
  
  TNamed* configsignal = new TNamed("ConfigFile", cfgPath.c_str());
  p->AddInput(configsignal);
  p->Process(SignalDS, "FWLiteTSelector");
  
 if(Log){ 
 TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
 pl->Save("0.0","/afs/cern.ch/user/s/sfrosali/scratch0/Zjets/CMSSW_3_1_4/src/Firenze/PATAnalysis/bin/Log.txt");
 }
  
  p->ClearInput();
  delete SignalDS;

  p->Close();
   
return 0;

}
