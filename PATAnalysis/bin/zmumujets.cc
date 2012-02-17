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
#include "Firenze/PATAnalysis/include/zmumujets.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

using namespace std;

/*
-1. All
0.  Data2011 (skimmed)
1.  DYtoMuMu_Fall11 (not skimmed)
*/

//argc>=0: single sample; argc=-1: all samples; 
//argv: number of events to be processed

int main(int argc, char *argv[]) {

int PreDefName, ProcEvents;
  string sourceList, sample, outNameSuf;

  if (argc<3) {
    cout << "Usage is zmumujets.exe PreDefName, ProcEvents, stop" << endl;
    return 0;}
  else
  
  PreDefName=atoi(argv[1]);
    
  //Number of events to be processed
  ProcEvents=atoi(argv[2]);

  //List of Source Files  
 
  string Data2011 = "SourceFilesMuons/data.txt";
  string DYtoMuMu_Fall11 = "SourceFilesMuons/signal.txt";
  
  //Path of PATAnalysis dir - DO NOT FORGET THE SLASH AT THE END OF THE PATH
  string path="/data/sfrosali/Zjets/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/";
  
  //Modules
  bool GEN    = true;
  bool RECO   = true;
  bool EFF    = true;
  bool NTUPLE = true;
  
  //Ntuple - "zcand" = saves only z candidates; "acc" = save only events with Z in the acceptance; "all" = saves all the events  
  string NtupleFill = "all";
  
  //Cuts
  int Acc     = 1;
  int Trg     = 2;
  int Imp     = 3;
  int Qual    = 4;
  int Iso     = 5;
  int MuID    = 0;
  
  //Selections: "SYM" = V+jets selections (symmetric); "ASYM" = Vector Boson Task Force (a-symmetric)
  string selections = "SYM";
  
  //RECO Jet Type 
  string JetType = "selectedJetsL1Corrected";
  
  //JEC Uncertainty applied to RecoMuonNtuple: 0 = NotApplied, 1 = Added, -1 = Subtracted
  int JECUnc = 0; //default value = 0
  string JECUncFilePath = "/data/sfrosali/Zjets/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/JECUncertainty/Jec10V1_Uncertainty_AK5PF.txt";
  
  //Normalization
  string Norm = "True";
  double targetLumi = 50.; //pb-1
  if(PreDefName==0) Norm="False";   // Do not normalize for data
  
  //Number of CPUs
  int CPU = 5;
  
  //Log (non funziona)
  bool Log = false;
  
  //Sample: Data -> "data"; MC -> "mc"
 
  if(PreDefName==0){
    sourceList = Data2011.c_str();  
    sample = "data";}
  else if (PreDefName==1){
    sourceList = DYtoMuMu_Fall11.c_str();
    sample = "mc";}
/*  else if (PreDefName==2)
    sourceList = Zpj_Z2.c_str();
  else if (PreDefName==3)
    sourceList = Zpj_D6T.c_str();
  else if (PreDefName==4)
    sourceList = Zpj_pZ2.c_str();
  else if (PreDefName==5)
    sourceList = TT.c_str();
  else if (PreDefName==6)
    sourceList = Wlnu.c_str();
  else if (PreDefName==7)
    sourceList = WWEE.c_str();
  else if (PreDefName==8)
    sourceList = ZZEE.c_str();
  else if (PreDefName==9)
    sourceList = WZEE.c_str();
  else if (PreDefName==10)
    sourceList = bce_2030.c_str();
  else if (PreDefName==11)
    sourceList = bce_3080.c_str();
  else if (PreDefName==12)
    sourceList = bce_80170.c_str();
  else if (PreDefName==13)
    sourceList = em_2030.c_str();
  else if (PreDefName==14)
    sourceList = em_3080.c_str();
  else if (PreDefName==15)
    sourceList = em_80170.c_str();*/

  cout << "RUNNING ON " << sourceList << endl;
  cout << "PreDefName " << PreDefName << endl;

  name(PreDefName);
  string outputName = SampleName;
  outputName+=outNameSuf;
  
  double xsec = 1.0; 
  double EventFilter = 1.0;
  int EventsPerFile = 0;
  int EventNumber = 0;
  
  if(sample=="data")GEN = false;
  
  if(PreDefName>-1){
  
  //Parameters
  Parameters(PreDefName, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
    
  makeCfg(sample, selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), sourceList.c_str(), outputName.c_str(), Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  }
  
  if(PreDefName==-1){
  
  Parameters(0, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("data", selections, JetType, false, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), Data2011.c_str(), "Data2011", "False", EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(1, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), DYtoMuMu_Fall11.c_str(), "DYtoMuMu_Fall11", "False", EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  }
  
  /*if(PreDefName==-1 || PreDefName==-3 || PreDefName==-4){
  
  Parameters(2, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRnew", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), Zpj_Z2.c_str(), "Z_Madgraph_Z2", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(3, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRnew", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), Zpj_D6T.c_str(), "Z_Madgraph_D6T", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(4, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRnew", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), Zpj_pZ2.c_str(), "Z_Pythia_Z2", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  }*/
  
 /* if(PreDefName==-1 || PreDefName==-3 || PreDefName==-5){
  
  Parameters(5, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRold", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), TT.c_str(), "TT_Pythia", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(6, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRold", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), Wlnu.c_str(), "Wlnu_Madgraph", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(7, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRold", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), WWEE.c_str(), "WWEE_Pythia", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(8, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRold", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), ZZEE.c_str(), "ZZEE_Pythia", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(9, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRold", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), WZEE.c_str(), "WZEE_Pythia", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(10, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRold", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), bce_2030.c_str(), "QCD_BCtoE_Pythia_Pt20to30", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(11, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRold", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), bce_3080.c_str(), "QCD_BCtoE_Pythia_Pt30to80", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(12, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRold", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), bce_80170.c_str(), "QCD_BCtoE_Pythia_Pt80to170", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(13, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRold", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), em_2030.c_str(), "QCD_EMEnriched_Pythia_Pt20to30", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(14, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRold", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), em_3080.c_str(), "QCD_EMEnriched_Pythia_Pt30to80", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(15, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, "PFL1CORRold", GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), em_80170.c_str(), "QCD_EMEnriched_Pythia_Pt80to170", Norm.c_str(), EventsPerFile, EventNumber, -1, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  }*/

  gEnv->SetValue("Proof.Sandbox", "/data/sfrosali/.proof");

  TProof * p = TProof::Open("");
  
  p->SetParallel(CPU);
  
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec(".x remote.C");

  if(PreDefName>-1){
  TDSet* SignalDS = getDS(sourceList.c_str());
  
  string cfgPath = path;
  cfgPath+=outputName;
  cfgPath+=".py";
  
  TNamed* configsignal = new TNamed("ConfigFile", cfgPath.c_str());
  p->AddInput(configsignal);
  p->Process(SignalDS, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS;
  
  }
  
  if(PreDefName==-1){
  
  string cfgPath_0=path+"Data2011.py";
  TDSet* SignalDS_0 = getDS(Data2011.c_str());
  TNamed* configsignal_0 = new TNamed("ConfigFile", cfgPath_0.c_str());
  p->AddInput(configsignal_0);
  p->Process(SignalDS_0, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_0;
  string cfgPath_1=path+"DYtoMuMu_Fall11.py";
  TDSet* SignalDS_1 = getDS(DYtoMuMu_Fall11.c_str());
  TNamed* configsignal_1 = new TNamed("ConfigFile", cfgPath_1.c_str());
  p->AddInput(configsignal_1);
  p->Process(SignalDS_1, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_1;
  
  }
  
/*  if(PreDefName==-1 || PreDefName==-3 || PreDefName==-4){
  
  string cfgPath_2=path+"Z_Madgraph_Z2.py";
  TDSet* SignalDS_2 = getDS(Zpj_D6T.c_str());
  TNamed* configsignal_2 = new TNamed("ConfigFile", cfgPath_2.c_str());
  p->AddInput(configsignal_2);
  p->Process(SignalDS_2, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_2;
  string cfgPath_3=path+"Z_Madgraph_D6T.py";
  TDSet* SignalDS_3 = getDS(Zpj_Z2.c_str());
  TNamed* configsignal_3 = new TNamed("ConfigFile", cfgPath_3.c_str());
  p->AddInput(configsignal_3);
  p->Process(SignalDS_3, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_3;
  string cfgPath_4=path+"Z_Pythia_Z2.py";
  TDSet* SignalDS_4 = getDS(Zpj_pZ2.c_str());
  TNamed* configsignal_4 = new TNamed("ConfigFile", cfgPath_4.c_str());
  p->AddInput(configsignal_4);
  p->Process(SignalDS_4, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_4;
  
  }
  
  if(PreDefName==-1 || PreDefName==-3 || PreDefName==-5){
  
  string cfgPath_5=path+"TT_Pythia.py";
  TDSet* SignalDS_5 = getDS(TT.c_str());
  TNamed* configsignal_5 = new TNamed("ConfigFile", cfgPath_5.c_str());
  p->AddInput(configsignal_5);
  p->Process(SignalDS_5, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_5;
  string cfgPath_6=path+"Wlnu_Madgraph.py";
  TDSet* SignalDS_6 = getDS(Wlnu.c_str());
  TNamed* configsignal_6 = new TNamed("ConfigFile", cfgPath_6.c_str());
  p->AddInput(configsignal_6);
  p->Process(SignalDS_6, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_6;
  string cfgPath_7=path+"WWEE_Pythia.py";
  TDSet* SignalDS_7 = getDS(WWEE.c_str());
  TNamed* configsignal_7 = new TNamed("ConfigFile", cfgPath_7.c_str());
  p->AddInput(configsignal_7);
  p->Process(SignalDS_7, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_7; 
  string cfgPath_8=path+"ZZEE_Pythia.py";
  TDSet* SignalDS_8 = getDS(ZZEE.c_str());
  TNamed* configsignal_8 = new TNamed("ConfigFile", cfgPath_8.c_str());
  p->AddInput(configsignal_8);
  p->Process(SignalDS_8, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_8;
  string cfgPath_9=path+"WZEE_Pythia.py";
  TDSet* SignalDS_9 = getDS(WZEE.c_str());
  TNamed* configsignal_9 = new TNamed("ConfigFile", cfgPath_9.c_str());
  p->AddInput(configsignal_9);
  p->Process(SignalDS_9, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_9;
  string cfgPath_10=path+"QCD_BCtoE_Pythia_Pt20to30.py";
  TDSet* SignalDS_10 = getDS(bce_2030.c_str());
  TNamed* configsignal_10 = new TNamed("ConfigFile", cfgPath_10.c_str());
  p->AddInput(configsignal_10);
  p->Process(SignalDS_10, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_10;
  string cfgPath_11=path+"QCD_BCtoE_Pythia_Pt30to80.py";
  TDSet* SignalDS_11 = getDS(bce_3080.c_str());
  TNamed* configsignal_11 = new TNamed("ConfigFile", cfgPath_11.c_str());
  p->AddInput(configsignal_11);
  p->Process(SignalDS_11, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_11;
  string cfgPath_12=path+"QCD_BCtoE_Pythia_Pt80to170.py";
  TDSet* SignalDS_12 = getDS(bce_80170.c_str());
  TNamed* configsignal_12 = new TNamed("ConfigFile", cfgPath_12.c_str());
  p->AddInput(configsignal_12);
  p->Process(SignalDS_12, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_12;
  string cfgPath_13=path+"QCD_EMEnriched_Pythia_Pt20to30.py";
  TDSet* SignalDS_13 = getDS(em_2030.c_str());
  TNamed* configsignal_13 = new TNamed("ConfigFile", cfgPath_13.c_str());
  p->AddInput(configsignal_13);
  p->Process(SignalDS_13, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_13;
  string cfgPath_14=path+"QCD_EMEnriched_Pythia_Pt30to80.py";
  TDSet* SignalDS_14 = getDS(em_3080.c_str());
  TNamed* configsignal_14 = new TNamed("ConfigFile", cfgPath_14.c_str());
  p->AddInput(configsignal_14);
  p->Process(SignalDS_14, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_14;
  string cfgPath_15=path+"QCD_EMEnriched_Pythia_Pt80to170.py";
  TDSet* SignalDS_15 = getDS(em_80170.c_str());
  TNamed* configsignal_15 = new TNamed("ConfigFile", cfgPath_15.c_str());
  p->AddInput(configsignal_15);
  p->Process(SignalDS_15, "FWLiteTSelector","",-1);
  p->ClearInput();
  delete SignalDS_15;  
  
  }*/
  
  if(Log){ 
  TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
  pl->Save("0.1",path.c_str());
  }

  p->Close();
   
return 0;

}
