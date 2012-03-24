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

-9. TEST

-1. All
0.  dati2011 			(skimmed)
1.  DYtoLL_All			(not skimmed)
2.  TTJets_All			(skimmed)
3.  WJetsToLNu_All		(skimmed)
4.  WWJetsTo2L2Nu_All		(skimmed)
5.  QCD_Pt20MuEnPt10_All	(skimmed)

-2.  All Train
11.  DYtoLL_Train		(not skimmed)
12.  TTJets_Train		(skimmed)
13.  WJetsToLNu_Train		(skimmed)
14.  WWJetsTo2L2Nu_Train	(skimmed)
15.  QCD_Pt20MuEnPt10_Train	(skimmed)

-3.  All Sample
21.  DYtoLL_Sample		(not skimmed)
22.  TTJets_Sample		(skimmed)
23.  WJetsToLNu_Sample		(skimmed)
24.  WWJetsTo2L2Nu_Sample	(skimmed)
25.  QCD_Pt20MuEnPt10_Sample	(skimmed)
*/

//argc: sample
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
  string SourceFilesDir			= "/data/sandro/Analisi/rel_CMSSW_4_2_5_commit/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/SourceFilesMuons/";

  string Data2011 			= "data.list";
  string DYtoLL_All 			= "DYtoLL_All.list";  
  string TTJets_All			= "TTJets_All.list";
  string WJetsToLNu_All			= "WJetsToLNu_All.list";
  string WWJetsTo2L2Nu_All		= "WWJetsTo2L2Nu_All.list";
  string QCD_Pt20MuEnPt10_All		= "QCD_Pt20MuEnPt10_All.list";
  
  string DYtoLL_Train 			= "DYtoLL_Train.list";
  string TTJets_Train			= "TTJets_Train.list";
  string WJetsToLNu_Train		= "WJetsToLNu_Train.list";
  string WWJetsTo2L2Nu_Train		= "WWJetsTo2L2Nu_Train.list";
  string QCD_Pt20MuEnPt10_Train		= "QCD_Pt20MuEnPt10_Train.list";
  
  string DYtoLL_Sample 			= "DYtoLL_Sample.list";
  string TTJets_Sample			= "TTJets_Sample.list";
  string WJetsToLNu_Sample		= "WJetsToLNu_Sample.list";
  string WWJetsTo2L2Nu_Sample		= "WWJetsTo2L2Nu_Sample.list";
  string QCD_Pt20MuEnPt10_Sample	= "QCD_Pt20MuEnPt10_Sample.list";
  
  string Test 		        	= "Test.list";
  sample = "mc";
 
  //tag to recognize the analysis in the output file
  string analysis = "_DeltaR"; 

  //Path of PATAnalysis dir - DO NOT FORGET THE SLASH AT THE END OF THE PATH
  string path="/data/sandro/Analisi/rel_CMSSW_4_2_5_commit/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/PythonMuons/";
  
  //Modules
  bool GEN    = false;
  bool RECO   = false;
  bool EFF    = false;
  bool NTUPLE = false;
  bool DELTAR = true;
  
  //Ntuple - "zcand" = saves only z candidates; "acc" = save only events with Z in the acceptance; "all" = saves all the events  
  string NtupleFill = "all";
  
  //Cuts: if number = 0, the cut doesn't affect the Z REC selection  
  int Acc     = 1;
  int Trg     = 0;
  int Imp     = 2;
  int Qual    = 3;
  int Iso     = 4;
  int MuID    = 0;
  
  //Selections: "SYM" = V+jets selections (symmetric); "ASYM" = Vector Boson Task Force (a-symmetric)
  string selections = "SYM";
  
  //RECO Jet Type 
  string JetType = "selectedJetsL1Corrected";
  
  //JEC Uncertainty applied to RecoMuonNtuple: 0 = NotApplied, 1 = Added, -1 = Subtracted
  int JECUnc = 0; //default value = 0
  string JECUncFilePath = "/data/sfrosali/Zjets/Commit/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/JECUncertainty/Jec10V1_Uncertainty_AK5PF.txt";
  
  //Normalization
  string Norm = "True";
  double targetLumi = 100.; //pb-1
  if(PreDefName==0) Norm="False";   // Do not normalize for data
  
  //Number of CPUs
  int CPU = 8;
  
  //Log (non funziona)
  bool Log = false;
  
  //Sample: Data -> "data"; MC -> "mc"
  
  if(PreDefName==-9)sourceList = (SourceFilesDir + Test).c_str();  
 
  if(PreDefName==0){
    sourceList = (SourceFilesDir + Data2011).c_str();  
    sample = "data";}
  else if (PreDefName==1){
    sourceList = (SourceFilesDir + DYtoLL_All).c_str();
    sample = "mc";}
  else if (PreDefName==2){
    sourceList = (SourceFilesDir + TTJets_All).c_str();
    sample = "mc";}
  else if (PreDefName==3){
    sourceList = (SourceFilesDir + WJetsToLNu_All).c_str();
    sample = "mc";}
  else if (PreDefName==4){
    sourceList = (SourceFilesDir + WWJetsTo2L2Nu_All).c_str();
    sample = "mc";}
  else if (PreDefName==5){
    sourceList = (SourceFilesDir + QCD_Pt20MuEnPt10_All).c_str();
    sample = "mc";}
    
  else if (PreDefName==11){
    sourceList = (SourceFilesDir + DYtoLL_Train).c_str();
    sample = "mc";}
  else if (PreDefName==12){
    sourceList = (SourceFilesDir + TTJets_Train).c_str();
    sample = "mc";}
  else if (PreDefName==13){
    sourceList = (SourceFilesDir + WJetsToLNu_Train).c_str();
    sample = "mc";}
  else if (PreDefName==14){
    sourceList = (SourceFilesDir + WWJetsTo2L2Nu_Train).c_str();
    sample = "mc";}
  else if (PreDefName==15){
    sourceList = (SourceFilesDir + QCD_Pt20MuEnPt10_Train).c_str();
    sample = "mc";}
    
  else if (PreDefName==21){
    sourceList = (SourceFilesDir + DYtoLL_Sample).c_str();
    sample = "mc";}
  else if (PreDefName==22){
    sourceList = (SourceFilesDir + TTJets_Sample).c_str();
    sample = "mc";}
  else if (PreDefName==23){
    sourceList = (SourceFilesDir + WJetsToLNu_Sample).c_str();
    sample = "mc";}
  else if (PreDefName==24){
    sourceList = (SourceFilesDir + WWJetsTo2L2Nu_Sample).c_str();
    sample = "mc";}
  else if (PreDefName==25){
    sourceList = (SourceFilesDir + QCD_Pt20MuEnPt10_Sample).c_str();
    sample = "mc";}

  cout << "RUNNING ON " << sourceList << endl;
  cout << "PreDefName " << PreDefName << endl;

  name(PreDefName);
  string outputName = SampleName;
  outputName+=analysis+=outNameSuf;
  
  double xsec = 1.0; 
  double EventFilter = 1.0;
  int EventsPerFile = 0;
  int EventNumber = 0;
  
  if(sample=="data")GEN = false;
  
  if(PreDefName>-1 || PreDefName==-9){
  
  //Parameters
  Parameters(PreDefName, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
    
  makeCfg(sample, selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), sourceList.c_str(), outputName.c_str(), Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  }
  
  if(PreDefName==-1){
  
  Parameters(0, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("data", selections, JetType, false, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), Data2011.c_str(), "Data2011", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(1, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), DYtoLL_All.c_str(), "DYtoLL_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(2, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), TTJets_All.c_str(), "TTJets_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(3, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), WJetsToLNu_All.c_str(), "WJetsToLNu_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(4, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), WWJetsTo2L2Nu_All.c_str(), "WWJetsTo2L2Nu_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(5, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), QCD_Pt20MuEnPt10_All.c_str(), "QCD_Pt20MuEnPt10_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  }
  
  if(PreDefName==-2){
  
  Parameters(11, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), DYtoLL_Train.c_str(), "DYtoLL_Train", "False", EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);//Il train del segnale non è norm. per avere un conf. corretto con effMC (altrimenti avrei le barre d'errore sbagliate)
  
  Parameters(12, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), TTJets_Train.c_str(), "TTJets_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(13, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), WJetsToLNu_Train.c_str(), "WJetsToLNu_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(14, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), WWJetsTo2L2Nu_Train.c_str(), "WWJetsTo2L2Nu_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(15, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), QCD_Pt20MuEnPt10_Train.c_str(), "QCD_Pt20MuEnPt10_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  }
  
  if(PreDefName==-3){
  
  Parameters(21, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), DYtoLL_Sample.c_str(), "DYtoLL_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(22, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), TTJets_Sample.c_str(), "TTJets_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(23, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), WJetsToLNu_Sample.c_str(), "WJetsToLNu_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(24, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), WWJetsTo2L2Nu_Sample.c_str(), "WWJetsTo2L2Nu_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  Parameters(25, &ParStruct);
  xsec=ParStruct._xsec;
  EventFilter=ParStruct._EventFilter;
  EventsPerFile=ParStruct._EventsPerFile;
  EventNumber=ParStruct._EventNumber;
  makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, DELTAR, Acc, Trg, Qual, Imp, Iso, MuID, path.c_str(), QCD_Pt20MuEnPt10_Sample.c_str(), "QCD_Pt20MuEnPt10_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
  
  }

  gEnv->SetValue("Proof.Sandbox", "/data/sandro/.proof");

  TProof * p = TProof::Open("");
  
  p->SetParallel(CPU);
  
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec(".x /data/sandro/Analisi/rel_CMSSW_4_2_5_commit/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/remote.C");

  if(PreDefName>-1 || PreDefName==-9){
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
  p->Process(SignalDS_0, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_0;
  string cfgPath_1=path+"DYtoLL_All.py";
  TDSet* SignalDS_1 = getDS(DYtoLL_All.c_str());
  TNamed* configsignal_1 = new TNamed("ConfigFile", cfgPath_1.c_str());
  p->AddInput(configsignal_1);
  p->Process(SignalDS_1, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_1;  
  string cfgPath_2=path+"TTJets_All.py";
  TDSet* SignalDS_2 = getDS(TTJets_All.c_str());
  TNamed* configsignal_2 = new TNamed("ConfigFile", cfgPath_2.c_str());
  p->AddInput(configsignal_2);
  p->Process(SignalDS_2, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_2;
  string cfgPath_3=path+"WJetsToLNu_All.py";
  TDSet* SignalDS_3 = getDS(WJetsToLNu_All.c_str());
  TNamed* configsignal_3 = new TNamed("ConfigFile", cfgPath_3.c_str());
  p->AddInput(configsignal_3);
  p->Process(SignalDS_3, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_3;
  string cfgPath_4=path+"WWJetsTo2L2Nu_All.py";
  TDSet* SignalDS_4 = getDS(WWJetsTo2L2Nu_All.c_str());
  TNamed* configsignal_4 = new TNamed("ConfigFile", cfgPath_4.c_str());
  p->AddInput(configsignal_4);
  p->Process(SignalDS_4, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_4;
  string cfgPath_5=path+"QCD_Pt20MuEnPt10_All.py";
  TDSet* SignalDS_5 = getDS(QCD_Pt20MuEnPt10_All.c_str());
  TNamed* configsignal_5 = new TNamed("ConfigFile", cfgPath_5.c_str());
  p->AddInput(configsignal_5);
  p->Process(SignalDS_5, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_5; 
  }
  
  if(PreDefName==-2){ 
  string cfgPath_1=path+"DYtoLL_Train.py";              
  TDSet* SignalDS_1 = getDS(DYtoLL_Train.c_str());
  TNamed* configsignal_1 = new TNamed("ConfigFile", cfgPath_1.c_str());
  p->AddInput(configsignal_1);
  p->Process(SignalDS_1, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_1; 
  string cfgPath_2=path+"TTJets_Train.py";
  TDSet* SignalDS_2 = getDS(TTJets_Train.c_str());
  TNamed* configsignal_2 = new TNamed("ConfigFile", cfgPath_2.c_str());
  p->AddInput(configsignal_2);
  p->Process(SignalDS_2, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_2;
  string cfgPath_3=path+"WJetsToLNu_Train.py";
  TDSet* SignalDS_3 = getDS(WJetsToLNu_Train.c_str());
  TNamed* configsignal_3 = new TNamed("ConfigFile", cfgPath_3.c_str());
  p->AddInput(configsignal_3);
  p->Process(SignalDS_3, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_3;
  string cfgPath_4=path+"WWJetsTo2L2Nu_Train.py";
  TDSet* SignalDS_4 = getDS(WWJetsTo2L2Nu_Train.c_str());
  TNamed* configsignal_4 = new TNamed("ConfigFile", cfgPath_4.c_str());
  p->AddInput(configsignal_4);
  p->Process(SignalDS_4, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_4;
  string cfgPath_5=path+"QCD_Pt20MuEnPt10_Train.py";
  TDSet* SignalDS_5 = getDS(QCD_Pt20MuEnPt10_Train.c_str());
  TNamed* configsignal_5 = new TNamed("ConfigFile", cfgPath_5.c_str());
  p->AddInput(configsignal_5);
  p->Process(SignalDS_5, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_5; 
  }
  
  if(PreDefName==-3){ 
  string cfgPath_1=path+"DYtoLL_Sample.py";            
  TDSet* SignalDS_1 = getDS(DYtoLL_Sample.c_str());
  TNamed* configsignal_1 = new TNamed("ConfigFile", cfgPath_1.c_str());
  p->AddInput(configsignal_1);
  p->Process(SignalDS_1, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_1;
  string cfgPath_2=path+"TTJets_Sample.py";
  TDSet* SignalDS_2 = getDS(TTJets_Sample.c_str());
  TNamed* configsignal_2 = new TNamed("ConfigFile", cfgPath_2.c_str());
  p->AddInput(configsignal_2);
  p->Process(SignalDS_2, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_2;
  string cfgPath_3=path+"WJetsToLNu_Sample.py";
  TDSet* SignalDS_3 = getDS(WJetsToLNu_Sample.c_str());
  TNamed* configsignal_3 = new TNamed("ConfigFile", cfgPath_3.c_str());
  p->AddInput(configsignal_3);
  p->Process(SignalDS_3, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_3;
  string cfgPath_4=path+"WWJetsTo2L2Nu_Sample.py";
  TDSet* SignalDS_4 = getDS(WWJetsTo2L2Nu_Sample.c_str());
  TNamed* configsignal_4 = new TNamed("ConfigFile", cfgPath_4.c_str());
  p->AddInput(configsignal_4);
  p->Process(SignalDS_4, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_4;
  string cfgPath_5=path+"QCD_Pt20MuEnPt10_Sample.py";
  TDSet* SignalDS_5 = getDS(QCD_Pt20MuEnPt10_Sample.c_str());
  TNamed* configsignal_5 = new TNamed("ConfigFile", cfgPath_5.c_str());
  p->AddInput(configsignal_5);
  p->Process(SignalDS_5, "FWLiteTSelector","",ProcEvents);
  p->ClearInput();
  delete SignalDS_5; 
  }
  
  if(Log){ 
  TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
  pl->Save("0.1",path.c_str());
  }

  p->Close();
   
return 0;

}
