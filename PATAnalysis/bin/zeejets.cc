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
  string sourceList = "Signal_50pb.txt";
  string outputName = "Test_VPJ";
  string electronID = "eidRobustLoose";
  
  //Sample: mc = MonteCarlo , data = Data
  string sample = "mc";
  
  //Selections: "VPJ" = V+jets selections (old); "VBTF" = Vector Boson Task Force (new)
  string selections = "VPJ";
  
  //Normalization - If Norm = "False" and EventsPerFile = 0 -> Normalization is not applied
  double xsec = 1300;
  double targetLumi = 100.;
  string Norm = "True";
  int EventsPerFile = 0;
  int EventNumber = 0;
  string Sumw2= "True";
  
  //Number of events to be processed
  int ProcEvents = -1;
  
  //Gen Particle Matching
  string GenParticleMatch = "False";
  
  //Modules
  bool GEN = true;
  bool RECO = true;
  bool EFF = true;
  bool NTUPLE = true;
  
  //Cuts
  int Acc = 1;
  int Trg = 2;
  int Qual = 0;
  int Imp = 3;
  int Iso = 4;
  int EiD = 5;
  
  //Number of CPUs
  int CPU = 8;
  
  //Log
  bool Log = false;
  
  //Path of PATAnalysis dir - DO NOT FORGET THE SLASH AT THE END OF THE PATH
  string path = "/data/sfrosali/Zjets/Commit/CMSSW_3_5_6/src/Firenze/PATAnalysis/bin/";
  
  if(sample=="data"){
  GenParticleMatch = "False";
  GEN = false;}

  makeCfg(sample, selections, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, EiD, path.c_str(), cfgName.c_str(), sourceList.c_str(), outputName.c_str(), Norm.c_str(), Sumw2.c_str(), EventsPerFile, EventNumber, ProcEvents, electronID.c_str(), xsec, targetLumi, GenParticleMatch.c_str());

  gEnv->SetValue("Proof.Sandbox", "/data/sfrosali/.proof");

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
  p->Process(SignalDS, "FWLiteTSelector","",ProcEvents);
  
  if(Log){ 
  TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
  pl->Save("0.1",path.c_str());
  }
  
  p->ClearInput();
  delete SignalDS;

  p->Close();
   
return 0;

}
