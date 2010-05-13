#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TFileMerger.h"
#include "TSystem.h"
#include "TROOT.h"
#include "Firenze/PATAnalysis/include/EfficiencyAnalyzerElectron.h"

using namespace std;

int main() {

  //Input files
  string training_sign = "TagProbeSample_VPJ/TagProbe_Signal_train_VPJ.root";
  string training_back = "TagProbeSample_VPJ/TagProbe_Background_train_Norm50_VPJ.root";
  string total = "TagProbeSample_VPJ/TagProbe_Total_50pb_VPJ.root";
  
  //Output files
  string output_train_sign =  "TPAnalyzer_VPJ/TPAnalyzer_Signal_train_VPJ_EiD.root";
  string output_train_back =  "TPAnalyzer_VPJ/TPAnalyzer_Background_train_VPJ_EiD.root";
  string output_total =  "TPAnalyzer_VPJ/TPAnalyzer_Total_50pb_VPJ_EiD.root";

  //Selections used
  string selections = "VPJ";
  
  //Cut to evaluate
  string cut = "EiD";
  
  //Analyzer steps
  bool tr_sig = true;
  bool tr_back = true;
  bool total_an = true;
  
  //Mass bin
  int massbin = 4;
  
  //IF directories are different from default ones
  string nodef_dir = "";
  string nodef_secdir = "";
  
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
  
  string dir = "";
  string sec_el_dir = "";
  
  if(selections=="VPJ"){
  if(cut=="Imp")dir = "EfficiencyElectron/Tag&Probe_Acc_Trg_Imp";
  if(cut=="Iso")dir = "EfficiencyElectron/Tag&Probe_Acc_Trg_Imp_Iso";
  if(cut=="EiD")dir = "EfficiencyElectron/Tag&Probe_Acc_Trg_Imp_Iso_EiD";
  }
  
  if(selections=="VBTF"){
  if(cut=="Imp"){
  dir = "EfficiencyElectron/Tag&Probe0_AccVBTF_Trg_Imp";
  sec_el_dir = "EfficiencyElectron/Tag&Probe1_AccVBTF_Trg_Imp";}
  if(cut=="Iso"){
  dir = "EfficiencyElectron/Tag&Probe0_AccVBTF_Trg_Imp_IsoVBTF";
  sec_el_dir = "EfficiencyElectron/Tag&Probe1_AccVBTF_Trg_Imp_IsoVBTF";}
  if(cut=="EiD"){
  dir = "EfficiencyElectron/Tag&Probe0_AccVBTF_Trg_Imp_IsoVBTF_EiDVBTF";
  sec_el_dir = "EfficiencyElectron/Tag&Probe1_AccVBTF_Trg_Imp_IsoVBTF_EiDVBTF";}
  }
  
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
