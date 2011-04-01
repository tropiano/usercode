#ifndef zeejets_h
#define zeejets_h

#include <iostream>
#include <fstream>
#include <string>

#include "TFile.h"
#include "TParameter.h"
#include "TDSet.h"
#include "TSystem.h"

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

static string SampleName;

inline void name(int i){
if(i==-1)SampleName="";
else if(i==0)SampleName="Data";
else if(i==1)SampleName="ZJets_Madgraph_Spring10";
else if(i==2)SampleName="Zee_Pythia_Spring10";
else if(i==3)SampleName="WJets_Madgraph_Spring10";
else if(i==4)SampleName="Wenu_Pythia_Spring10";
else if(i==5)SampleName="TTbarJets_Madgraph_Spring10";
else if(i==6)SampleName="TTbar_Pythia_Spring10";
else if(i==7)SampleName="QCD_BCtoE_Pythia_Pt20to30_Spring10";
else if(i==8)SampleName="QCD_BCtoE_Pythia_Pt30to80_Spring10";
else if(i==9)SampleName="QCD_BCtoE_Pythia_Pt80to170_Spring10";
else if(i==10)SampleName="QCD_EMEnriched_Pythia_Pt20to30_Spring10";
else if(i==11)SampleName="QCD_EMEnriched_Pythia_Pt30to80_Spring10";
else if(i==12)SampleName="QCD_EMEnriched_Pythia_Pt80to170_Spring10";
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
case(0):
//Data
Par->_xsec=1.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(1):
//ZJets_Madgraph_Spring10
Par->_xsec=2946.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(2):
//Zee_Pythia_Spring10
Par->_xsec=1667.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(3):
//WJets_Madgraph_Spring10
Par->_xsec=30936.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(4):
//Wenu_Pythia_Spring10
Par->_xsec=8159.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(5):
//TTbarJets_Madgraph_Spring10
Par->_xsec=162.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(6):
//TTbar_Pythia_Spring10
Par->_xsec=162.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(7):
//QCD_BCtoE_Pythia_Pt20to30_Spring10
Par->_xsec=235500000.;
Par->_EventFilter=0.00046;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(8):
//QCD_BCtoE_Pythia_Pt30to80_Spring10
Par->_xsec=59300000.;
Par->_EventFilter=0.00234;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(9):
//QCD_BCtoE_Pythia_Pt80to170_Spring10
Par->_xsec=906000.;
Par->_EventFilter=0.0104;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(10):
//QCD_EMEnriched_Pythia_Pt20to30_Spring10  (skimmed)
Par->_xsec=235500000.;
Par->_EventFilter=0.0073;
Par->_EventsPerFile=0;
Par->_EventNumber=33809839;
break;
case(11):
//QCD_EMEnriched_Pythia_Pt30to80_Spring10  (skimmed)
Par->_xsec=59300000.;
Par->_EventFilter=0.059;
Par->_EventsPerFile=0;
Par->_EventNumber=42377278;
break;
case(12):
//QCD_EMEnriched_Pythia_Pt80to170_Spring10  (skimmed)
Par->_xsec=906000.;
Par->_EventFilter=0.148;
Par->_EventsPerFile=0;
Par->_EventNumber=5394911;
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

void makeCfg(string sample, string selections, string JetType, string TPMult, bool GEN, bool RECO, bool EFF, bool NTUPLE, int Acc, int Trg, int Conv, int Imp, int Iso, int EiD, const char* path, const char* sourceList, const char *outFile, const char* Norm, const char* Sumw2, int EventsPerFile, int EventNumber, int ProcEvents, double xsec, double targetLumi, const char* GenParticleMatch, string NtupleFill){

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
cfg<<"process.zeegen = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"GenElectron\"),"<<std::endl;
cfg<<"Name        = cms.string(\"GenElectron\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"Sumw2       = cms.bool("<<Sumw2<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(RECO){
cfg<<"process.zeerec = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"RecoElectron\"),"<<std::endl;
cfg<<"Name        = cms.string(\"RecoElectron\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
cfg<<"JetType = cms.string(\""<<JetType.c_str()<<"\"),"<<std::endl;
cfg<<"ReportName        = cms.string(\""<<RepName.c_str()<<"\"),"<<std::endl;
cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
cfg<<"Conv = cms.int32("<<Conv<<"),"<<std::endl;
cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
cfg<<"EiD = cms.int32("<<EiD<<"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"Sumw2       = cms.bool("<<Sumw2<<"),"<<std::endl;
cfg<<"GenParticleMatch = cms.bool("<<GenParticleMatch<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(EFF){
cfg<<"process.zeeeff = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"EfficiencyElectron\"),"<<std::endl;
cfg<<"Name        = cms.string(\"EfficiencyElectron\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
cfg<<"JetType = cms.string(\""<<JetType.c_str()<<"\"),"<<std::endl;
cfg<<"TPMult = cms.string(\""<<TPMult.c_str()<<"\"),"<<std::endl;
cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<"),"<<std::endl;
cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
cfg<<"Conv = cms.int32("<<Conv<<"),"<<std::endl;
cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
cfg<<"EiD = cms.int32("<<EiD<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(NTUPLE){
cfg<<"process.zeentuple = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"RecoElectronNtuple\"),"<<std::endl;
cfg<<"Name        = cms.string(\"RecoElectronNtuple\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
cfg<<"Conv = cms.int32("<<Conv<<"),"<<std::endl;
cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
cfg<<"EiD = cms.int32("<<EiD<<"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"NtupleFill        = cms.string(\""<<NtupleFill<<"\"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}


cfg.close();

}

#endif
