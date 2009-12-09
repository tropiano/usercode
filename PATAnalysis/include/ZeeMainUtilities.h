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

void makeCfg(const char* cfgname, const char* source, const char *outFile, const char* Norm, const char* Sumw2, const char* electronID, double xsec, double targetLumi){

string Source = "/afs/cern.ch/user/s/sfrosali/scratch0/Zjets/CMSSW_3_1_4/src/Firenze/PATAnalysis/bin/";
Source+=source;

ofstream cfg;
string cfgpath="/afs/cern.ch/user/s/sfrosali/scratch0/Zjets/CMSSW_3_1_4/src/Firenze/PATAnalysis/bin/python/";
cfgpath+=cfgname;
cfg.open(cfgpath.c_str());

cfg<<"import FWCore.ParameterSet.Config as cms"<<std::endl;

cfg<<"from Firenze.PATAnalysis.config_Zee_signal import *"<<std::endl<<std::endl;

//Ouput File Name
string outputFile = outFile;
outputFile+=".root";
cfg<<"process.outputFile.name = cms.string(\""<<outputFile.c_str()<<"\")"<<std::endl<<std::endl;

//ReportName
string RepName = "/afs/cern.ch/user/s/sfrosali/scratch0/Zjets/CMSSW_3_1_4/src/Firenze/PATAnalysis/bin/Report_";
RepName+=outFile;
RepName+=".txt";
cfg<<"process.zeegen.ReportName = cms.string(\""<<RepName.c_str()<<"\")"<<std::endl<<std::endl;

//Input File Path
cfg<<"process.zeegen.sourceFileList = cms.string(\""<<Source.c_str()<<"\")"<<std::endl;
cfg<<"process.zeerec.sourceFileList = cms.string(\""<<Source.c_str()<<"\")"<<std::endl;
cfg<<"process.zeeeff.sourceFileList = cms.string(\""<<Source.c_str()<<"\")"<<std::endl<<std::endl;

//Normalization
cfg<<"process.zeegen.Norm = cms.bool("<<Norm<<")"<<std::endl;
cfg<<"process.zeerec.Norm = cms.bool("<<Norm<<")"<<std::endl<<std::endl;

//Sumw2
cfg<<"process.zeegen.Sumw2 = cms.bool("<<Sumw2<<")"<<std::endl;
cfg<<"process.zeerec.Sumw2 = cms.bool("<<Sumw2<<")"<<std::endl;
cfg<<"process.zeeeff.Sumw2 = cms.bool("<<Sumw2<<")"<<std::endl<<std::endl;

//Electron ID
cfg<<"process.zeerec.electronID = cms.string(\""<<electronID<<"\")"<<std::endl;
cfg<<"process.zeeeff.electronID = cms.string(\""<<electronID<<"\")"<<std::endl<<std::endl;

//Cross Section
cfg<<"process.zeegen.CrossSection = cms.double("<<xsec<<")"<<std::endl;
cfg<<"process.zeerec.CrossSection = cms.double("<<xsec<<")"<<std::endl<<std::endl;

//Target Lumi
cfg<<"process.zeegen.targetLumi = cms.double("<<targetLumi<<")"<<std::endl;
cfg<<"process.zeerec.targetLumi = cms.double("<<targetLumi<<")"<<std::endl<<std::endl;

cfg.close();

}

#endif
