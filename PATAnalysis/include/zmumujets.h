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
-1. Unknown
0.  Full_mod_output_dati2011 			(skimmed)
1.  Full_mod_output_DYtoMuMu_M20_Fall11.root	(not skimmed)
*/

static string SampleName;

inline void name(int i){
if(i==-1)SampleName="";
else if(i==0)SampleName="Data2011";
else if(i==1)SampleName="DYtoMuMu_Fall11";
/*else if(i==2)SampleName="Z_Madgraph_Z2";
else if(i==3)SampleName="Z_Madgraph_D6T";
else if(i==4)SampleName="Z_Pythia_Z2";
else if(i==5)SampleName="TT_Pythia";
else if(i==6)SampleName="Wlnu_Madgraph";
else if(i==7)SampleName="WWEE_Pythia";
else if(i==8)SampleName="ZZEE_Pythia";
else if(i==9)SampleName="WZEE_Pythia";
else if(i==10)SampleName="QCD_BCtoE_Pythia_Pt20to30";
else if(i==11)SampleName="QCD_BCtoE_Pythia_Pt30to80";
else if(i==12)SampleName="QCD_BCtoE_Pythia_Pt80to170";
else if(i==13)SampleName="QCD_EMEnriched_Pythia_Pt20to30";
else if(i==14)SampleName="QCD_EMEnriched_Pythia_Pt30to80";
else if(i==15)SampleName="QCD_EMEnriched_Pythia_Pt80to170";*/
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
//Data2011
Par->_xsec=1.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(1):
//DYtoMuMu_Fall11
Par->_xsec=1667.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
/*case(2):
//Z_Madgraph_Z2
Par->_xsec=3048.0;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(3):
//Z_Madgraph_D6T
Par->_xsec=3048.0;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(4):
//Z_Pythia_Z2
Par->_xsec=1666.0;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;
case(5):
//TT_Pythia (skimmed)
Par->_xsec=157.5;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=1099550.0;
break;
case(6):
//Wlnu_Madgraph (skimmed)
Par->_xsec=31314.0;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=15161497.0;
break;
case(7):
//WWEE_Pythia (skimmed)
Par->_xsec=43.0; //(NLO xsec)
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=2050240.0;
break;
case(8):
//ZZEE_Pythia (skimmed)
Par->_xsec=5.9; //(NLO xsec)
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=2113368.0;
break;
case(9):
//WZEE_Pythia (skimmed)
Par->_xsec=18.2; //(NLO xsec)
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=2185664.0;
break;
case(10):
//QCD_BCtoE_Pythia_Pt20to30 (skimmed)
Par->_xsec=236000000.;
Par->_EventFilter=0.00056;
Par->_EventsPerFile=0;
Par->_EventNumber=2243439.0; 
break;
case(11):
//QCD_BCtoE_Pythia_Pt30to80 (skimmed)
Par->_xsec=59500000.;
Par->_EventFilter=0.0023;
Par->_EventsPerFile=0;
Par->_EventNumber=1995502.0;
break;
case(12):
//QCD_BCtoE_Pythia_Pt80to170 (skimmed)
Par->_xsec=900000.;
Par->_EventFilter=0.0104;
Par->_EventsPerFile=0;
Par->_EventNumber=1043390.0;
break;
case(13):
//QCD_EMEnriched_Pythia_Pt20to30  (skimmed)
Par->_xsec=236000000.;
Par->_EventFilter=0.0104;
Par->_EventsPerFile=0;
Par->_EventNumber=36375274.0;
break;
case(14):
//QCD_EMEnriched_Pythia_Pt30to80  (skimmed)
Par->_xsec=59500000.;
Par->_EventFilter=0.065;
Par->_EventsPerFile=0;
Par->_EventNumber=71834019.0;
break;
case(15):
//QCD_EMEnriched_Pythia_Pt80to170  (skimmed)
Par->_xsec=900000.;
Par->_EventFilter=0.155;
Par->_EventsPerFile=0;
Par->_EventNumber=8073559.0;
break;
default:
Par->_xsec=1.;
Par->_EventFilter=1.;
Par->_EventsPerFile=0;
Par->_EventNumber=0;
break;*/
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

void makeCfg(string sample, string selections, string JetType, bool GEN, bool RECO, bool EFF, bool NTUPLE, int Acc, int Trg, int Qual, int Imp, int Iso, int MuID, const char* path, const char* sourceList, const char *outFile, const char* Norm, int EventsPerFile, int EventNumber, int ProcEvents, double xsec, double targetLumi, string NtupleFill, int JECUnc, string JECUncFilePath){

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


cfg.close();

}

#endif
