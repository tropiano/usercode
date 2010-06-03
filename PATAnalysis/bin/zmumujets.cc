#include <iostream>
//#include "Signal.h"
#include "Firenze/PATAnalysis/include/EfficiencyAnalyzerMuon.h"
#include "Firenze/PATAnalysis/include/Unfolding.h"
#include "TFile.h"
#include "TTree.h"
//#include "RooUnfold/RooUnfoldResponse.h"
#include "TParameter.h"
#include "TFileMerger.h"
#include "TDSet.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TEnv.h"
#include "TProof.h"
#include "TProofLog.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include <fstream>


using namespace std;

TDSet* getDS(const char* filename){
  TDSet* out = new TDSet( "dataset", "Events");
  ifstream infile;
  infile.open(filename);
  string datafile;
  while(getline ( infile, datafile )){
    out->Add(datafile.c_str());
  }
  return out; 
}


int main(){
  gEnv->SetValue("Proof.Sandbox", "/data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/proof");

  TProof * p = TProof::Open("");
  //p->SetParallel(4);
 
  //double targetLumi = 100.;
 
  std::string pwd(gSystem->Getenv("PWD")); 

  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec( ".x /data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/remote.C" );

  TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
  pl->Save("*","/data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/Log.txt");
  
  TDSet* dataDS = getDS("/data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/data.txt");
  TNamed* configdata = new TNamed("ConfigFile", (pwd+"/config_data_zmumu.py").c_str());
  p->AddInput(configdata);
  p->Process(dataDS, "FWLiteTSelector");
  delete dataDS;
  
  TDSet* signalDS = getDS("/data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/signal.txt");
  TNamed* configsignal = new TNamed("ConfigFile", (pwd+"/config_signal.py").c_str());
  p->AddInput(configsignal);
  p->Process(signalDS, "FWLiteTSelector");
  delete signalDS;

  TDSet* qcdDS = getDS("/data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/qcd.txt");
  TNamed* configqcd = new TNamed("ConfigFile", (pwd+"/config_background_qcd.py").c_str());
  p->AddInput(configqcd);
  p->Process(qcdDS, "FWLiteTSelector");
  delete qcdDS;

  TDSet* ttbarDS = getDS("/data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/ttbar.txt");
  TNamed* configttbar = new TNamed("ConfigFile", (pwd+"/config_background_ttbar.py").c_str());
  p->AddInput(configttbar);
  p->Process(ttbarDS, "FWLiteTSelector");
  delete ttbarDS;
  
  TDSet* wDS = getDS("/data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/w.txt");
  TNamed* configw = new TNamed("ConfigFile", (pwd+"/config_background_w.py").c_str());
  p->AddInput(configw);
  p->Process(wDS, "FWLiteTSelector");
  delete wDS;
  
  TDSet* signalDWDS = getDS("/data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/signal_DW.txt");
  TNamed* configDW = new TNamed("ConfigFile", (pwd+"/config_signal_DW.py").c_str());
  p->AddInput(configDW);
  p->Process(signalDWDS, "FWLiteTSelector");
  delete signalDWDS; 

  TDSet* signalP0DS = getDS("/data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/signal_P0.txt");
  TNamed* configP0 = new TNamed("ConfigFile", (pwd+"/config_signal_P0.py").c_str());
  p->AddInput(configP0);
  p->Process(signalP0DS, "FWLiteTSelector");
  delete signalP0DS;

  TDSet* signalProPT0DS = getDS("/data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/signal_ProPT0.txt");
  TNamed* configProPT0 = new TNamed("ConfigFile", (pwd+"/config_signal_ProPT0.py").c_str());
  p->AddInput(configProPT0);
  p->Process(signalProPT0DS, "FWLiteTSelector");
  delete signalProPT0DS; 

  TDSet* signalProQ20DS = getDS("/data/lenzip/RapGap/CMSSW_3_5_8/src/Firenze/PATAnalysis/bin/signal_ProQ20.txt");
  TNamed* configProQ20 = new TNamed("ConfigFile", (pwd+"/config_signal_ProQ20.py").c_str());
  p->AddInput(configProQ20);
  p->Process(signalProQ20DS, "FWLiteTSelector");
  delete signalProQ20DS;

  p->Close();

  //default tune
  TFile in("signal.root");
  TFile out1("signal_Eff_vsReco.root", "RECREATE");
  TFile out2("signal_Eff_vsGen.root", "RECREATE");
  EfficiencyAnalyzerMuon mueffana1(&in, &out1, 0, "EfficiencyMuonVSRecoMultiNoScale");
  mueffana1.analyze();
  EfficiencyAnalyzerMuon mueffana2(&in, &out2, 0, "EfficiencyMuonVSGenMultiNoScale");
  mueffana2.analyze();
  out1.Close();
  out2.Close();
  //tune DW
  TFile inDW("signalDW.root");
  TFile out1DW("signal_Eff_vsRecoDW.root", "RECREATE");
  TFile out2DW("signal_Eff_vsGenDW.root", "RECREATE");
  EfficiencyAnalyzerMuon mueffana1DW(&inDW, &out1DW, 0, "EfficiencyMuonVSRecoMultiNoScale");
  mueffana1DW.analyze();
  EfficiencyAnalyzerMuon mueffana2DW(&inDW, &out2DW, 0, "EfficiencyMuonVSGenMultiNoScale");
  mueffana2DW.analyze();
  out1DW.Close();
  out2DW.Close();
  //tune P0
  TFile inP0("signalP0.root");
  TFile out1P0("signal_Eff_vsRecoP0.root", "RECREATE");
  TFile out2P0("signal_Eff_vsGenP0.root", "RECREATE");
  EfficiencyAnalyzerMuon mueffana1P0(&inP0, &out1P0, 0, "EfficiencyMuonVSRecoMultiNoScale");
  mueffana1P0.analyze();
  EfficiencyAnalyzerMuon mueffana2P0(&inP0, &out2P0, 0, "EfficiencyMuonVSGenMultiNoScale");
  mueffana2P0.analyze();
  out1P0.Close();
  out2P0.Close();
  //tune P0
  TFile inProPT0("signalProPT0.root");
  TFile out1ProPT0("signal_Eff_vsRecoProPT0.root", "RECREATE");
  TFile out2ProPT0("signal_Eff_vsGenProPT0.root", "RECREATE");
  EfficiencyAnalyzerMuon mueffana1ProPT0(&inProPT0, &out1ProPT0, 0, "EfficiencyMuonVSRecoMultiNoScale");
  mueffana1ProPT0.analyze();
  EfficiencyAnalyzerMuon mueffana2ProPT0(&inProPT0, &out2ProPT0, 0, "EfficiencyMuonVSGenMultiNoScale");
  mueffana2ProPT0.analyze();
  out1ProPT0.Close();
  out2ProPT0.Close();
  //tune P0
  TFile inProQ20("signalProQ20.root");
  TFile out1ProQ20("signal_Eff_vsRecoProQ20.root", "RECREATE");
  TFile out2ProQ20("signal_Eff_vsGenProQ20.root", "RECREATE");
  EfficiencyAnalyzerMuon mueffana1ProQ20(&inProQ20, &out1ProQ20, 0, "EfficiencyMuonVSRecoMultiNoScale");
  mueffana1ProQ20.analyze();
  EfficiencyAnalyzerMuon mueffana2ProQ20(&inProQ20, &out2ProQ20, 0, "EfficiencyMuonVSGenMultiNoScale");
  mueffana2ProQ20.analyze();
  out1ProQ20.Close();
  out2ProQ20.Close();


  EfficiencyAnalyzerMuon zyield_ana(&in, 0, 0, "EfficiencyMuonVSRecoMulti");
  zyield_ana.analyze("z_yield.tex");

  TFile inW("w.root");
  EfficiencyAnalyzerMuon wyield_ana(&inW, 0, 0, "EfficiencyMuonVSRecoMulti");
  wyield_ana.analyze("w_yield.tex");

  TFile inQCD("qcd.root");
  EfficiencyAnalyzerMuon qcdyield_ana(&inQCD, 0, 0, "EfficiencyMuonVSRecoMulti");
  qcdyield_ana.analyze("qcd_yield.tex");

  TFile inTTbar("ttbar.root");
  EfficiencyAnalyzerMuon ttbaryield_ana(&inTTbar, 0, 0, "EfficiencyMuonVSRecoMulti");
  ttbaryield_ana.analyze("ttbar_yield.tex");

  //TFile in("zmumu.root");
  //EfficiencyAnalyzerMuon yield_ana(&in, 0, 0, "EfficiencyMuonVSRecoMulti");
  //yield_ana.analyze("yield.tex");


}
