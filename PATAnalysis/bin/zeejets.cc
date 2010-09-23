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
#include "Firenze/PATAnalysis/include/zeejets.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

using namespace std;

/*
-1. Unknown
0.  Data
1.  ZJets_Madgraph_Spring10
2.  Zee_Pythia_Spring10
3.  WJets_Madgraph_Spring10
4.  Wenu_Pythia_Spring10
5.  TTbarJets_Madgraph_Spring10
6.  TTbar_Pythia_Spring10
7.  QCD_BCtoE_Pythia_Pt20to30_Spring10
8.  QCD_BCtoE_Pythia_Pt30to80_Spring10
9.  QCD_BCtoE_Pythia_Pt80to170_Spring10
10. QCD_EMEnriched_Pythia_Pt20to30_Spring10  (skimmed)
11. QCD_EMEnriched_Pythia_Pt30to80_Spring10  (skimmed)
12. QCD_EMEnriched_Pythia_Pt80to170_Spring10 (skimmed)
*/

int main() {

  //Job
  
  int PreDefName = 0;
  
  string sourceList = "Data.txt";
  string outNameSuf="_test";
  name(PreDefName);
  string outputName = SampleName;
  outputName+=outNameSuf;
  
  //Sample: mc = MonteCarlo , data = Data
  string sample = "data";
  
  //Selections: "VPJ" = V+jets selections (old); "VBTF" = Vector Boson Task Force (new)
  string selections = "VBTF";
  
  //Jet Type - "CALO": CaloJets, "PF": PFJets
  string JetType = "PF";
  
  //Jet multiplicity for Tag&Probe
  string TPMult = "incl";
  
  //Normalization
  string Norm = "False";
  string Sumw2= "True";
  double targetLumi = 50.; //pb-1
  
  //Parameters
  Parameters(PreDefName, &ParStruct);
  double xsec=ParStruct._xsec;
  double EventFilter=ParStruct._EventFilter;
  int EventsPerFile=ParStruct._EventsPerFile;
  int EventNumber=ParStruct._EventNumber;
  
  //Parameters defined by user
  if(PreDefName==-1){
  xsec = 1667; //pb
  EventFilter = 1.;
  EventsPerFile = 0;
  EventNumber = 0;}
  
  //Number of events to be processed
  int ProcEvents = -1;
  
  //Gen Particle Matching
  string GenParticleMatch = "False";
  
  //Ntuple - "zcand" = saves only z candidates; "all" = saves all the events
  string NtupleFill = "zcand";
  
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
  string path = "/data/sfrosali/Zjets/Commit/CMSSW_3_6_3/src/Firenze/PATAnalysis/bin/";
  
  if(sample=="data"){
  GenParticleMatch = "False";
  GEN = false;}

  makeCfg(sample, selections, JetType, TPMult, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, EiD, path.c_str(), sourceList.c_str(), outputName.c_str(), Norm.c_str(), Sumw2.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, GenParticleMatch.c_str(), NtupleFill);

  gEnv->SetValue("Proof.Sandbox", "/data/sfrosali/.proof");

  TProof * p = TProof::Open("");
  
  p->SetParallel(CPU);
  
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec(".x remote.C");

  TDSet* SignalDS = getDS(sourceList.c_str());
  
  string cfgPath = path;
  cfgPath+=outputName;
  cfgPath+=".py";
  
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
