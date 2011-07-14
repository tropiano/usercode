#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

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
0.  Data_RUN2010A (skimmed)
1.  Data_RUN2010B (skimmed)
2.  Z_Madgraph
3.  TT_Pythia (skimmed)
4.  Wlnu_Madgraph (skimmed)
5.  WWEE_Pythia (skimmed)
6.  ZZEE_Pythia (skimmed)
7.  WZEE_Pythia (skimmed)
8.  QCD_BCtoE_Pythia_Pt20to30 (skimmed)
9.  QCD_BCtoE_Pythia_Pt30to80 (skimmed)
10. QCD_BCtoE_Pythia_Pt80to170 (skimmed)
11. QCD_EMEnriched_Pythia_Pt20to30  (skimmed)
12. QCD_EMEnriched_Pythia_Pt30to80  (skimmed)
13. QCD_EMEnriched_Pythia_Pt80to170 (skimmed)
*/

int main(int argc, char *argv[]) {

int PreDefName, ProcEvents;
  string sourceList, sample, outNameSuf;

  if (argc<3) {
    cout << "Usage is zeejets.exe PreDefName, ProcEvents, stop" << endl;
    return 0;}
  else
  
  PreDefName=atoi(argv[1]);
    
  //Number of events to be processed
  ProcEvents=atoi(argv[2]);

  //List of Source Files  
 
  string Data_RUN2010A = "SourceFilesElectrons/DATAEE_RUN2010A_L1FastJet.list";
  string Data_RUN2010B = "SourceFilesElectrons/DATAEE_RUN2010B_L1FastJet.list";
  string Zpj = "SourceFilesElectrons/ZMadgraph_L1FastJet_Winter10_399.list";
  string TT = "SourceFilesElectrons/TTPYTHIAEE.list"; 
  string Wlnu = "SourceFilesElectrons/WJETSEE.list";
  string WWEE = "SourceFilesElectrons/WWEE.list";
  string ZZEE = "SourceFilesElectrons/ZZEE.list";
  string WZEE = "SourceFilesElectrons/WZEE.list";
  string bce_2030 = "SourceFilesElectrons/QCDBCE20.list";
  string bce_3080 = "SourceFilesElectrons/QCDBCE30.list";
  string bce_80170 = "SourceFilesElectrons/QCDBCE80.list";
  string em_2030 = "SourceFilesElectrons/QCDEM20.list";
  string em_3080 = "SourceFilesElectrons/QCDEM30.list";
  string em_80170 = "SourceFilesElectrons/QCDEM80.list";

  
  //Sample: Data -> "data"; MC-> "mc"
  sample = "mc";
  
  //Modules
  bool GEN = true;
  bool RECO = true;
  bool EFF = true;
  bool NTUPLE = true;
  
  //Ntuple - "zcand" = saves only z candidates; "acc" = save only events with Z in the acceptance; "all" = saves all the events  
  string NtupleFill = "acc";
  
  //Cuts
  int Acc  = 1;
  int Trg  = 2;
  int Imp  = 3;
  int Conv = 4;
  int Iso  = 5;
  int EiD  = 6;
  
  //Selections: "SYM" = V+jets selections (symmetric); "ASYM" = Vector Boson Task Force (a-symmetric)
  string selections = "ASYM";
  
  //Jet Type - "PF": PFJets, "PFL1CORR": PFJets L1 Corrected
  string JetType = "PFL1CORR";
  
  //Jet multiplicity for Tag&Probe: "incl" = Inclusive; "excl" = Exclusive
  string TPMult = "incl";
  
  //Normalization
  string Norm = "True";
  string Sumw2= "True";
  double targetLumi = 50.; //pb-1
  if(PreDefName==0 || PreDefName==1) Norm="False";   // Do not normalize for data
  
  //Gen Particle Matching
  string GenParticleMatch = "False";
  
  //Number of CPUs
  int CPU = 8;

  if(PreDefName==0){
    sourceList = Data_RUN2010A.c_str();  
    sample = "data";}
  else if (PreDefName==1){
    sourceList = Data_RUN2010B.c_str();
    sample = "data";}
  else if (PreDefName==2)
    sourceList = Zpj.c_str();
  else if (PreDefName==3)
    sourceList = TT.c_str();
  else if (PreDefName==4)
    sourceList = Wlnu.c_str();
  else if (PreDefName==5)
    sourceList = WWEE.c_str();
  else if (PreDefName==6)
    sourceList = ZZEE.c_str();
  else if (PreDefName==7)
    sourceList = WZEE.c_str();
  else if (PreDefName==8)
    sourceList = bce_2030.c_str();
  else if (PreDefName==9)
    sourceList = bce_3080.c_str();
  else if (PreDefName==10)
    sourceList = bce_80170.c_str();
  else if (PreDefName==11)
    sourceList = em_2030.c_str();
  else if (PreDefName==12)
    sourceList = em_3080.c_str();
  else if (PreDefName==13)
    sourceList = em_80170.c_str();

  cout << "RUNNING ON " << sourceList << endl;
  cout << "PreDefName " << PreDefName << endl;

  name(PreDefName);
  string outputName = SampleName;
  outputName+=outNameSuf;
  
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
  
  //Log
  bool Log = false;
  
  //Path of PATAnalysis dir - DO NOT FORGET THE SLASH AT THE END OF THE PATH
  string path="/data/sfrosali/Zjets/Commit/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/";
  
  if(sample=="data"){
  GenParticleMatch = "False";
  GEN = false;}

  makeCfg(sample, selections, JetType, TPMult, GEN, RECO, EFF, NTUPLE, Acc, Trg, Conv, Imp, Iso, EiD, path.c_str(), sourceList.c_str(), outputName.c_str(), Norm.c_str(), Sumw2.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, GenParticleMatch.c_str(), NtupleFill);

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
