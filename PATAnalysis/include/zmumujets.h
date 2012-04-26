#ifndef zmumujets_h
#define zmumujets_h

#include <iostream>
#include <fstream>
#include <string>

#include "TFile.h"
#include "TParameter.h"
#include "TDSet.h"
#include "TSystem.h"

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
6.  DYtoMuMu_All		(not skimmed)

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

static string SampleName;

inline void name(int i){
if(i==-9)SampleName="Test";
else if(i==0)SampleName="Data2011";
else if(i==1)SampleName="DYtoLL_All";
else if(i==2)SampleName="TTJets_All";
else if(i==3)SampleName="WJetsToLNu_All";
else if(i==4)SampleName="WWJetsTo2L2Nu_All";
else if(i==5)SampleName="QCD_Pt20MuEnPt10_All";
else if(i==6)SampleName="DYtoMuMu_All";

else if(i==11)SampleName="DYtoLL_Train";
else if(i==12)SampleName="TTJets_Train";
else if(i==13)SampleName="WJetsToLNu_Train";
else if(i==14)SampleName="WWJetsTo2L2Nu_Train";
else if(i==15)SampleName="QCD_Pt20MuEnPt10_Train";

else if(i==21)SampleName="DYtoLL_Sample";
else if(i==22)SampleName="TTJets_Sample";
else if(i==23)SampleName="WJetsToLNu_Sample";
else if(i==24)SampleName="WWJetsTo2L2Nu_Sample";
else if(i==25)SampleName="QCD_Pt20MuEnPt10_Sample";

else SampleName="";
}

typedef struct{
double _xsec;
double _EventFilter;
int _EventsPerFile;
int _EventNumber;
} par;

par ParStruct = {1., 1., 0, 0}; 

void Parameters(int i, par* Par){
switch(i){
case(-9):
//TEST
Par->_xsec=1.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(0):
//Data2011 (skimmed)
Par->_xsec=1.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(1):
//DYtoLL (not skimmed) 24026288 events = 7883 pb-1
Par->_xsec=3048.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(2):
//TTJets (skimmed) 3401947 events = 21600 pb-1
Par->_xsec=157.5;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=3401947;
break;
case(3):
//WJetsToLNu (skimmed) 73092433 events = 2335 pb-1
Par->_xsec=31314;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=73092433;
break;
case(4):
//WWJetsTo2L2Nu (skimmed) 10956 events = 255 pb-1
Par->_xsec=43.0;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=10956;
break;
case(5):
//QCD_Pt20MuEnPt10 (skimmed) 8747418 events = 25 pb-1
Par->_xsec=2.966E8;
Par->_EventFilter=0.00118;
Par->_EventsPerFile=0;
Par->_EventNumber=8747418;
break;
case(6):
//DYtoMuMu_All (not skimmed) events = 
Par->_xsec=1666.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;

case(11):
//DYtoLL Train (not skimmed)
Par->_xsec=3048.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(12):
//TTJets Train (skimmed)
Par->_xsec=157.5;
Par->_EventFilter=1.;
Par->_EventsPerFile=47914;
Par->_EventNumber=0;
break;
case(13):
//WJetsToLNu Train (skimmed)
Par->_xsec=31314;
Par->_EventFilter=1.;
Par->_EventsPerFile=49353;
Par->_EventNumber=0;
break;
case(14):
//WWJetsTo2L2Nu Train (skimmed) 10956 events = 255 pb-1 (unico file)
Par->_xsec=43.0;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=10956;
break;
case(15):
//QCD_Pt20MuEnPt10 Train (skimmed) 8747418 events = 25 pb-1
Par->_xsec=2.966E8;
Par->_EventFilter=0.00118;
Par->_EventsPerFile=49142;
Par->_EventNumber=0;
break;
case(21):
//DYtoLL Sample (not skimmed)
Par->_xsec=3048.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(22):
//TTJets Train (skimmed)
Par->_xsec=157.5;
Par->_EventFilter=1.;
Par->_EventsPerFile=47914;
Par->_EventNumber=0;
break;
case(23):
//WJetsToLNu Train (skimmed)
Par->_xsec=31314;
Par->_EventFilter=1.;
Par->_EventsPerFile=49353;
Par->_EventNumber=0;
break;
case(24):
//WWJetsTo2L2Nu Train (skimmed) 10956 events = 255 pb-1 (unico file)
Par->_xsec=43.0;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=10956;
break;
case(25):
//QCD_Pt20MuEnPt10 Train (skimmed) 8747418 events = 25 pb-1
Par->_xsec=2.966E8;
Par->_EventFilter=0.00118;
Par->_EventsPerFile=49142;
Par->_EventNumber=0;
break;
default:
Par->_xsec=1.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
}

}

TDSet* getDS(const char* filename){
  TDSet* out = new TDSet("dataset", "Events");
  ifstream infile;
  infile.open(filename);
  string datafile;
  while(getline (infile, datafile)){
    out->Add(datafile.c_str());
  }
  return out; 
}

void makeCfg(string sample, string selections, string JetType, bool GEN, bool RECO, bool EFF, bool NTUPLE, bool DELTAR, int Acc, int Trg, int Qual, int Imp, int Iso, int MuID, const char* path, const char* sourceList, const char *outFile, const char* Norm, int EventsPerFile, int EventNumber, int ProcEvents, double xsec, double targetLumi, string NtupleFill, int JECUnc, string JECUncFilePath){

//cfg
string cfgPath = path;
cfgPath+= outFile;
cfgPath+=".py";

//source
string Source = path;
Source+=sourceList;

//Ouput File Name
string outputFile = outFile;
outputFile+=".root";

//ReportName
string RepName = path;
RepName+="Report_";
RepName+=outFile;
RepName+=".txt";

ofstream cfg;
cfg.open(cfgPath.c_str());

cfg<<"import FWCore.ParameterSet.Config as cms"<<std::endl<<std::endl;

cfg<<"process=cms.Process(\"test\")"<<std::endl<<std::endl;

cfg<<"process.outputFile = cms.PSet(name = cms.string(\""<<outputFile.c_str()<<"\"))"<<std::endl<<std::endl;

if(GEN){
cfg<<"process.zmumugen = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"GenMuon\"),"<<std::endl;
cfg<<"Name        = cms.string(\"GenMuon\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(RECO){
cfg<<"process.zmumurec = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"RecoMuon\"),"<<std::endl;
cfg<<"Name        = cms.string(\"RecoMuon\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
cfg<<"JetType = cms.string(\""<<JetType.c_str()<<"\"),"<<std::endl;
cfg<<"ReportName        = cms.string(\""<<RepName.c_str()<<"\"),"<<std::endl;
cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
cfg<<"Qual = cms.int32("<<Qual<<"),"<<std::endl;
cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
cfg<<"MuID = cms.int32("<<MuID<<"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"JECUnc  = cms.int32("<<JECUnc<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(EFF){
cfg<<"process.zmumueff = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"EfficiencyMuon\"),"<<std::endl;
cfg<<"Name        = cms.string(\"EfficiencyMuon\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
cfg<<"JetType = cms.string(\""<<JetType.c_str()<<"\"),"<<std::endl;
cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<"),"<<std::endl;
cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
cfg<<"Qual = cms.int32("<<Qual<<"),"<<std::endl;
cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
cfg<<"MuID = cms.int32("<<MuID<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(NTUPLE){
cfg<<"process.zmumuntuple = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"RecoMuonNtuple\"),"<<std::endl;
cfg<<"Name        = cms.string(\"RecoMuonNtuple\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
cfg<<"JetType = cms.string(\""<<JetType.c_str()<<"\"),"<<std::endl;
cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
cfg<<"Qual = cms.int32("<<Qual<<"),"<<std::endl;
cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
cfg<<"MuID = cms.int32("<<MuID<<"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"NtupleFill        = cms.string(\""<<NtupleFill<<"\"),"<<std::endl;
cfg<<"JECUnc  = cms.int32("<<JECUnc<<"),"<<std::endl;
cfg<<"JECUncFilePath        = cms.string(\""<<JECUncFilePath<<"\"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(DELTAR){
cfg<<"process.zmumudeltar = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"DeltaRAnalyzerMuon\"),"<<std::endl;
cfg<<"Name        = cms.string(\"DeltaRAnalyzerMuon\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
cfg<<"JetType = cms.string(\""<<JetType.c_str()<<"\"),"<<std::endl;
cfg<<"ReportName        = cms.string(\""<<RepName.c_str()<<"\"),"<<std::endl;
cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
cfg<<"Qual = cms.int32("<<Qual<<"),"<<std::endl;
cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
cfg<<"MuID = cms.int32("<<MuID<<"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"JECUnc  = cms.int32("<<JECUnc<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

cfg.close();

}

#endif
