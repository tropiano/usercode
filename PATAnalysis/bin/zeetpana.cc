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
  string training_sign = "TPFiles/ZJets_Madgraph_Spring10_Signal_train.root";
  string training_back = "TPFiles/Background_All_train.root";
  string total = "TPFiles/Data_Aug18_25_NoTrg.root";
  
  //Output files
  string output_train_sign =  "TPAnalyzer/TPAnalyzer_Signal_train_VBTF_Imp.root";
  string output_train_back =  "TPAnalyzer/TPAnalyzer_Background_train_VBTF_Imp.root";
  string output_total =  "TPAnalyzer/TPAnalyzer_Data_VBTF_Imp.root";

  //Selections used
  string selections = "VBTF";
  
  //Cut to evaluate
  string cut = "_Imp";
  
  //Analyzer steps
  bool tr_sig = true;
  bool tr_back = true;
  bool total_an = true;
  
  //Mass bin
  int massbin = 4;
  
  //IF directories are different from default ones
  string nodef_dir = "";
  string nodef_secdir = "";
  
  //Selections
  
  int _Acc  = 1;
  int _Trg  = 2;
  int _Qual = 0;
  int _Imp  = 3;
  int _Iso  = 4;
  int _EiD  = 5;
	
  string _RecoCutFlags[7];
	
  for(int i=0; i<7; i++){
  _RecoCutFlags[i] = "_1";}
   
  if(selections=="VPJ"){
  _RecoCutFlags[_Acc] =  "_AccVPJ";
  _RecoCutFlags[_Iso] =  "_IsoVPJ";
  _RecoCutFlags[_EiD] =  "_EiDVPJ";}
  if(selections=="VBTF"){
  _RecoCutFlags[_Acc] =  "_AccVBTF";
  _RecoCutFlags[_Iso] =  "_IsoVBTF";
  _RecoCutFlags[_EiD] =  "_EiDVBTF";}
     
  _RecoCutFlags[_Trg] =  "_Trg";
  _RecoCutFlags[_Qual] = "_Qual";
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
	
  if(selections=="VPJ")dir+="Tag&Probe";
  if(selections=="VBTF"){
  dir+="Tag&Probe0";
  sec_el_dir+="Tag&Probe1";}
	
  int ncut = 1;	
  while(cut!=_RecoCutFlags[ncut]){
  dir+=_RecoCutFlags[ncut].c_str();
  sec_el_dir+=_RecoCutFlags[ncut].c_str();
  ncut++;
  if(ncut>6)throw cms::Exception("PATAnalysis:zeetpana_CutNotFound") << "ERROR! Cut selected not found!";
  }
	
  dir+=_RecoCutFlags[ncut].c_str();
  sec_el_dir+=_RecoCutFlags[ncut].c_str();
	
  if(nodef_dir!="")dir=nodef_dir;
  if(nodef_secdir!="")sec_el_dir=nodef_secdir;
  
  EfficiencyAnalyzerElectron TP_TrainSign(train_sign, out_train_sign, dir.c_str(), sec_el_dir.c_str());
  EfficiencyAnalyzerElectron TP_TrainBack(train_back, out_train_back, dir.c_str(), sec_el_dir.c_str()); 
  EfficiencyAnalyzerElectron TP_Total(total_in, total_out, dir.c_str(), sec_el_dir.c_str(), out_train_sign, out_train_back);
 
  if(tr_sig)TP_TrainSign.analyze(massbin,"signal");
  if(tr_back)TP_TrainBack.analyze(massbin,"background");
  if(total_an)TP_Total.analyze(massbin,"total");
 
  if(output_train_sign!="")delete out_train_sign;
  if(output_train_back!="")delete out_train_back;
  if(output_total!="")delete total_out;
     
return 0;

}
