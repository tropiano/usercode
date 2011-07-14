#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TFileMerger.h"
#include "TSystem.h"
#include "TROOT.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Firenze/PATAnalysis/include/EfficiencyAnalyzerElectron.h"

using namespace std;

int main() {

  //Input files
  string training_sign = "/data/sfrosali/Zjets/SecondLife/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/TagProbe_Incl/TPFiles/Z_Madgraph_JetPt30_TPwTrgBit.root";
  string training_back = "/data/sfrosali/Zjets/SecondLife/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/TagProbe_Incl/TPFiles/Background_All_JetPt30_TPwTrgBit.root";
  string total = "/data/sfrosali/Zjets/SecondLife/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/TagProbe_Incl/TPFiles/Data_RUN2010A-B_JetPt30_TPwTrgBit.root";
  
  //Output files
  string output_train_sign =  
"/data/sfrosali/Zjets/SecondLife/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/TagProbe_Incl/TPAnalyzer/TPwTrgBit/TrainSig_JetPt30_TPwTrgBit_Incl_Global.root";
  string output_train_back =  
"/data/sfrosali/Zjets/SecondLife/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/TagProbe_Incl/TPAnalyzer/TPwTrgBit/TrainBack_JetPt30_TPwTrgBit_Incl_Global.root";
  string output_total =  
"/data/sfrosali/Zjets/SecondLife/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/TagProbe_Incl/TPAnalyzer/TPwTrgBit/DATA_JetPt30_TPwTrgBit_Incl_Global.root";

  //Selections used
  string selections = "ASYM";
  
  //Cut to evaluate
  string cut = "Global";
  
  //Analyzer steps
  bool tr_sig = true;
  bool tr_back = true;
  bool total_an = true;
  
  //Mass bin
  int massbin = 4;
  
  //IF directories are different from default ones
  string nodef_dir = "";
  string nodef_secdir = "";
  
  //Cuts
  int _Acc  = 1;
  int _Trg  = 2;
  int _Imp  = 3;
  int _Conv = 4;
  int _Iso  = 5;
  int _EiD  = 6;
	
  string _RecoCutFlags[7];
	
  for(int i=0; i<7; i++){
  _RecoCutFlags[i] = "_1";}
   
  if(selections=="SYM"){
   _RecoCutFlags[_Acc] =  "_AccSYM";
   _RecoCutFlags[_Conv] = "_ConvSYM"; 
   _RecoCutFlags[_Iso] =  "_IsoSYM";
   _RecoCutFlags[_EiD] =  "_EiDSYM";}
   if(selections=="ASYM"){
   _RecoCutFlags[_Acc] =  "_AccASYM";
   _RecoCutFlags[_Conv] = "_ConvASYM"; 
   _RecoCutFlags[_Iso] =  "_IsoASYM";
   _RecoCutFlags[_EiD] =  "_EiDASYM";}
     
   _RecoCutFlags[_Trg] =  "_Trg";   
   _RecoCutFlags[_Imp] =  "_Imp";
  
  TFile* train_sign = 0;
  TFile* train_back = 0;
  TFile* total_in = 0;
  TFile* out_train_sign = 0;
  TFile* out_train_back = 0;
  TFile* total_out = 0;
  
  if(training_sign!="")train_sign = new TFile(training_sign.c_str());
  if(training_back!="")train_back = new TFile(training_back.c_str());
  if(total!="")total_in = new TFile(total.c_str());
  
  if(output_train_sign!=""&&tr_sig)out_train_sign = new TFile(output_train_sign.c_str(), "RECREATE");
  if(output_train_sign!=""&&!tr_sig)out_train_sign = new TFile(output_train_sign.c_str());
  if(output_train_back!=""&&tr_back)out_train_back = new TFile(output_train_back.c_str(), "RECREATE");
  if(output_train_back!=""&&!tr_back)out_train_back = new TFile(output_train_back.c_str());
  if(output_total!=""&&total_an)total_out = new TFile(output_total.c_str(), "RECREATE");
  if(output_total!=""&&!total_an)total_out = new TFile(output_total.c_str());
  
  string dir = "EfficiencyElectron/";
  string sec_el_dir = "EfficiencyElectron/";
  
  string dataset = "datasetElectron";
	
  if(selections=="SYM")dir+="Tag&Probe";
  if(selections=="ASYM"){
  dir+="Tag&Probe0";
  sec_el_dir+="Tag&Probe1";}
  
  if(cut!="Global"){	
  
  int ncut = 1;	
  while(cut!=_RecoCutFlags[ncut]){
  dir+=_RecoCutFlags[ncut].c_str();
  sec_el_dir+=_RecoCutFlags[ncut].c_str();
  dataset+=_RecoCutFlags[ncut].c_str();
  ncut++;
  if(ncut>6)throw cms::Exception("PATAnalysis:zeetpana_CutNotFound") << "ERROR! Cut selected not found!";
  }
  
  dir+=_RecoCutFlags[ncut].c_str();
  sec_el_dir+=_RecoCutFlags[ncut].c_str();
  dataset+=_RecoCutFlags[ncut].c_str();
  dataset+="_";
  
  }else{
  
  dir+="_Global";
  sec_el_dir+="_Global";
  dataset+="_Global_";
  
  }
  	
  if(nodef_dir!="")dir=nodef_dir;
  if(nodef_secdir!="")sec_el_dir=nodef_secdir;
  
  EfficiencyAnalyzerElectron TP_TrainSign(train_sign, out_train_sign, dir.c_str(), sec_el_dir.c_str(), dataset.c_str());
  EfficiencyAnalyzerElectron TP_TrainBack(train_back, out_train_back, dir.c_str(), sec_el_dir.c_str(), dataset.c_str()); 
  EfficiencyAnalyzerElectron TP_Total(total_in, total_out, dir.c_str(), sec_el_dir.c_str(), dataset.c_str(), out_train_sign, out_train_back);
 
  if(tr_sig)TP_TrainSign.analyze(massbin,"signal");
  if(tr_back)TP_TrainBack.analyze(massbin,"background");
  if(total_an)TP_Total.analyze(massbin,"total");
 
  if(output_train_sign!="")delete out_train_sign;
  if(output_train_back!="")delete out_train_back;
  if(output_total!="")delete total_out;
     
return 0;

}
