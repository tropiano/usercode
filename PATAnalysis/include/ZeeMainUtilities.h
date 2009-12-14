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

void makeCfg(bool GEN, bool RECO, bool EFF, bool NTUPLE, const char *cfgPath, const char* source, const char *outFile, const char* Norm, int EventsPerFile, const char* electronID, double xsec, double targetLumi){

string Source = "/afs/cern.ch/user/s/sfrosali/scratch0/Zjets/CMSSW_3_1_4/src/Firenze/PATAnalysis/bin/";
Source+=source;

//Ouput File Name
string outputFile = outFile;
outputFile+=".root";

//ReportName
string RepName = "/afs/cern.ch/user/s/sfrosali/scratch0/Zjets/CMSSW_3_1_4/src/Firenze/PATAnalysis/bin/Report_";
RepName+=outFile;
RepName+=".txt";

ofstream cfg;
cfg.open(cfgPath);

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
cfg<<"EventsPerFile  = cms.double("<<EventsPerFile<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(RECO){
cfg<<"process.zeerec = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"RecoElectron\"),"<<std::endl;
cfg<<"Name        = cms.string(\"RecoElectron\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"electronID = cms.string(\""<<electronID<<"\"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.double("<<EventsPerFile<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(EFF){
cfg<<"process.zeeeff = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"EfficiencyElectron\"),"<<std::endl;
cfg<<"Name        = cms.string(\"EfficiencyElectron\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"electronID = cms.string(\""<<electronID<<"\")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}

if(NTUPLE){
cfg<<"process.zeentuple = cms.PSet ("<<std::endl;
cfg<<"type        = cms.string(\"RecoElectronNtuple\"),"<<std::endl;
cfg<<"Name        = cms.string(\"RecoElectronNtuple\"),"<<std::endl;
cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
cfg<<"electronID = cms.string(\""<<electronID<<"\"),"<<std::endl;
cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
cfg<<"EventsPerFile  = cms.double("<<EventsPerFile<<")"<<std::endl;
cfg<<")"<<std::endl<<std::endl;
}


cfg.close();

}

#endif
