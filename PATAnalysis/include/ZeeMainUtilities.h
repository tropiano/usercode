#ifndef ZeeMainUtilities_h
#define ZeeMainUtilities_h

#include <iostream>
#include <fstream>
#include <string>

#include "TFile.h"
#include "TParameter.h"
#include "TDSet.h"
#include "TSystem.h"

using namespace std;

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

void makeCfg(string sample, bool GEN, bool RECO, bool EFF, bool NTUPLE, int Acc, int Trg, int Qual, int Imp, int Iso, int EiD, const char* path, const char *cfgName, const char* sourceList, const char *outFile, const char* Norm, const char* Sumw2, int EventsPerFile, const char* electronID, double xsec, double targetLumi, const char* GenParticleMatch){

//cfg
string cfgPath = path;
cfgPath+=cfgName;

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
cfg<<"ReportName        = cms.string(\""<<RepName.c_str()<<"\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"Sumw2       = cms.bool("<<Sumw2<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(RECO){
cfg<<"process.zeerec = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"RecoElectron\"),"<<std::endl;
cfg<<"Name        = cms.string(\"RecoElectron\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"electronID = cms.string(\""<<electronID<<"\"),"<<std::endl;
cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
cfg<<"Qual = cms.int32("<<Qual<<"),"<<std::endl;
cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
cfg<<"EiD = cms.int32("<<EiD<<"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"Sumw2       = cms.bool("<<Sumw2<<"),"<<std::endl;
cfg<<"GenParticleMatch = cms.bool("<<GenParticleMatch<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(EFF){
cfg<<"process.zeeeff = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"EfficiencyElectron\"),"<<std::endl;
cfg<<"Name        = cms.string(\"EfficiencyElectron\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
cfg<<"electronID = cms.string(\""<<electronID<<"\"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
cfg<<"Qual = cms.int32("<<Qual<<"),"<<std::endl;
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
cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
cfg<<"electronID = cms.string(\""<<electronID<<"\"),"<<std::endl;
cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
cfg<<"Qual = cms.int32("<<Qual<<"),"<<std::endl;
cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
cfg<<"EiD = cms.int32("<<EiD<<"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}


cfg.close();

}

#endif
