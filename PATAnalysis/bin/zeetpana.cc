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

  string training_sign = "Signal_TP_RL01_TagRL005TrgMatch_120345_train.root";
  string training_back = "Background_TP_RL01_TagRL005TrgMatch_120345_train.root";
  string total = "Total_TP_RL01_TagRL005TrgMatch_120345_Sample.root";
  
  string output_train_sign =  "TPAnalyzer_SignalTrain_Imp.root";
  string output_train_back =  "TPAnalyzer_BackgroundTrain_Imp.root";
  string output_total =  "TPAnalyzer_TotalSample_Imp.root";
  
  string dir = "EfficiencyElectron/Tag&Probe_Acc_Trg_Imp";

  TFile* train_sign = new TFile(training_sign.c_str());
  TFile* train_back = new TFile(training_back.c_str());
  TFile* total_in = new TFile(total.c_str());
  
  TFile* out_train_sign = new TFile(output_train_sign.c_str(), "RECREATE");
  TFile* out_train_back = new TFile(output_train_back.c_str(), "RECREATE");
  TFile* total_out = new TFile(output_total.c_str(), "RECREATE");

  EfficiencyAnalyzerElectron TP_TrainSign(train_sign, out_train_sign, dir.c_str());
  TP_TrainSign.analyze(5,"signal");
  EfficiencyAnalyzerElectron TP_TrainBack(train_back, out_train_back, dir.c_str());
  TP_TrainBack.analyze(5,"background");
  EfficiencyAnalyzerElectron TP_Total(total_in, total_out, dir.c_str(), out_train_sign, out_train_back);
  TP_Total.analyze(5,"total");
 
  delete out_train_sign;
  delete out_train_back;
  delete total_out;
     
return 0;

}
