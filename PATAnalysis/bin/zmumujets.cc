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

  gEnv->SetValue("Proof.Sandbox", "/data/sandro/.proof");

  TProof * p = TProof::Open("");
  //p->SetParallel(4);
 
  //double targetLumi = 100.;
 
  std::string pwd(gSystem->Getenv("PWD")); 

  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");


  p->Exec( ".x /data/sandro/Analisi/rel_CMSSW_4_2_4/CMSSW_4_2_4/src/Firenze/PATAnalysis/bin/remote.C" );
  
  //TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
  //pl->Save("*","/data01/lenzip/Zjets/localCMSSW/CMSSW_3_6_3/src/Firenze/PATAnalysis/bin/Log.txt");
/*
  TDSet* dataDS = getDS("/data/sandro/Analisi/rel_CMSSW_4_2_4/CMSSW_4_2_4/src/Firenze/PATAnalysis/bin/SourceFilesMuons/data.txt");
  TNamed* configdata = new TNamed("ConfigFile", (pwd+"/config_data_zmumu.py").c_str());
  p->AddInput(configdata);
  p->Process(dataDS, "FWLiteTSelector");

  TNamed* configdataoffset = new TNamed("ConfigFile", (pwd+"/config_data_zmumu_offset.py").c_str());
  p->AddInput(configdataoffset);
  p->Process(dataDS, "FWLiteTSelector");
  delete dataDS;
*/  

//  TDSet* signalDS = getDS("/data/sandro/Analisi/rel_CMSSW_4_2_4/CMSSW_4_2_4/src/Firenze/PATAnalysis/bin/SourceFilesMuons/signal.txt");
  TDSet* signalDS = getDS("/data/sandro/Analisi/rel_CMSSW_4_2_4/CMSSW_4_2_4/src/Firenze/PATAnalysis/bin/SourceFilesMuons/signal.txt");
  TNamed* configsignal = new TNamed("ConfigFile", (pwd+"/config_signal.py").c_str());
  p->AddInput(configsignal);
  p->Process(signalDS, "FWLiteTSelector");
  delete signalDS;

/*
  TDSet* signalMadnoPUDS = getDS("/raid/lenzip/Zjets/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/signal_Madgraph_noPU.txt");
  TNamed* configMadnoPU = new TNamed("ConfigFile", (pwd+"/config_signal_Madgraph_nopu.py").c_str());
  p->AddInput(configMadnoPU);
  p->Process(signalMadnoPUDS, "FWLiteTSelector");
  delete signalMadnoPUDS; 

  TDSet* signalMadDS = getDS("/raid/lenzip/Zjets/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/signal_Madgraph.txt");
  TNamed* configMadOffUnf = new TNamed("ConfigFile", (pwd+"/config_signal_Madgraph_offset_unfolding.py").c_str());
  p->AddInput(configMadOffUnf);
  p->Process(signalMadDS, "FWLiteTSelector"); 

  TDSet* signalMadDS = getDS("/raid/lenzip/Zjets/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/signal_Madgraph.txt");
  TNamed* configMad = new TNamed("ConfigFile", (pwd+"/config_signal_Madgraph.py").c_str());
  p->AddInput(configMad);
  p->Process(signalMadDS, "FWLiteTSelector");
  //delete signalMadDS;

  TNamed* configMadOff = new TNamed("ConfigFile", (pwd+"/config_signal_Madgraph_offset.py").c_str());
  p->AddInput(configMadOff);
  p->Process(signalMadDS, "FWLiteTSelector");
  delete signalMadDS;


  TDSet* qcdDS = getDS("/raid/lenzip/Zjets/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/qcd.txt");
  TNamed* configqcd = new TNamed("ConfigFile", (pwd+"/config_background_qcd.py").c_str());
  p->AddInput(configqcd);
  p->Process(qcdDS, "FWLiteTSelector");
  delete qcdDS;


  TDSet* ttbarDS = getDS("/raid/lenzip/Zjets/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/ttbar.txt");
  TNamed* configttbar = new TNamed("ConfigFile", (pwd+"/config_background_ttbar.py").c_str());
  p->AddInput(configttbar);
  p->Process(ttbarDS, "FWLiteTSelector");
  delete ttbarDS;
  
  TDSet* wDS = getDS("/raid/lenzip/Zjets/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/w.txt");
  TNamed* configw = new TNamed("ConfigFile", (pwd+"/config_background_w.py").c_str());
  p->AddInput(configw);
  p->Process(wDS, "FWLiteTSelector");
  delete wDS;

  TDSet* wwDS = getDS("/raid/lenzip/Zjets/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/ww.txt");
  TNamed* configww = new TNamed("ConfigFile", (pwd+"/config_background_ww.py").c_str());
  p->AddInput(configww);
  p->Process(wwDS, "FWLiteTSelector");
  delete wwDS; 

  TDSet* wzDS = getDS("/raid/lenzip/Zjets/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/wz.txt");
  TNamed* configwz = new TNamed("ConfigFile", (pwd+"/config_background_wz.py").c_str());
  p->AddInput(configwz);
  p->Process(wzDS, "FWLiteTSelector");
  delete wzDS;

  TDSet* zzDS = getDS("/raid/lenzip/Zjets/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/zz.txt");
  TNamed* configzz = new TNamed("ConfigFile", (pwd+"/config_background_zz.py").c_str());
  p->AddInput(configzz);
  p->Process(zzDS, "FWLiteTSelector");
  delete zzDS;


  TDSet* tauDS = getDS("/raid/lenzip/Zjets/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/ztautau.txt");
  TNamed* configtau = new TNamed("ConfigFile", (pwd+"/config_background_ztautau.py").c_str());
  p->AddInput(configtau);
  p->Process(tauDS, "FWLiteTSelector");
  delete tauDS;
*/  
/*
  TDSet* MinBiasnoPUDS = getDS("/data/sandro/releases/rel_CMSSW_3_8_7/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/MinBiasnoPU.txt");
  TNamed* configMinBiasnoPU = new TNamed("ConfigFile", (pwd+"/config_MinBiasnoPU.py").c_str());
  p->AddInput(configMinBiasnoPU);
  p->Process(MinBiasnoPUDS, "FWLiteTSelector");
  delete MinBiasnoPUDS;
*/
/*
  TDSet* qcd_pt15to30noPUDS = getDS("/data/sandro/releases/rel_CMSSW_3_8_7/CMSSW_3_8_7/src/Firenze/PATAnalysis/bin/qcd_pt15to30noPU.txt");
  TNamed* configqcd_pt15to30noPU = new TNamed("ConfigFile", (pwd+"/config_background_qcd_pt15to30noPU.py").c_str());
  p->AddInput(configqcd_pt15to30noPU);
  p->Process(qcd_pt15to30noPUDS, "FWLiteTSelector");
  delete qcd_pt15to30noPUDS;
*/

/*  
  TDSet* signalDWDS = getDS("/data01/lenzip/Zjets/localCMSSW/CMSSW_3_6_3/src/Firenze/PATAnalysis/bin/signal_DW.txt");
  TNamed* configDW = new TNamed("ConfigFile", (pwd+"/config_signal_DW.py").c_str());
  p->AddInput(configDW);
  p->Process(signalDWDS, "FWLiteTSelector");
  delete signalDWDS; 

  TDSet* signalP0DS = getDS("/data01/lenzip/Zjets/localCMSSW/CMSSW_3_6_3/src/Firenze/PATAnalysis/bin/signal_P0.txt");
  TNamed* configP0 = new TNamed("ConfigFile", (pwd+"/config_signal_P0.py").c_str());
  p->AddInput(configP0);
  p->Process(signalP0DS, "FWLiteTSelector");
  delete signalP0DS;

  TDSet* signalProPT0DS = getDS("/data01/lenzip/Zjets/localCMSSW/CMSSW_3_6_3/src/Firenze/PATAnalysis/bin/signal_ProPT0.txt");
  TNamed* configProPT0 = new TNamed("ConfigFile", (pwd+"/config_signal_ProPT0.py").c_str());
  p->AddInput(configProPT0);
  p->Process(signalProPT0DS, "FWLiteTSelector");
  delete signalProPT0DS; 

  TDSet* signalProQ20DS = getDS("/data01/lenzip/Zjets/localCMSSW/CMSSW_3_6_3/src/Firenze/PATAnalysis/bin/signal_ProQ20.txt");
  TNamed* configProQ20 = new TNamed("ConfigFile", (pwd+"/config_signal_ProQ20.py").c_str());
  p->AddInput(configProQ20);
  p->Process(signalProQ20DS, "FWLiteTSelector");
  delete signalProQ20DS;
*/  
  p->Close();
/*
  //default tune
  TFile in("signal.root");
  TFile out1("signal_Eff_vsReco.root", "RECREATE");
  TFile out2("signal_Eff_vsGen.root", "RECREATE");
  EfficiencyAnalyzerMuon mueffana1(&in, &out1, 0, "EfficiencyMuonVSRecoMultiNoScale");
  mueffana1.analyze("z_yieldVsRec_noscale.tex");
  EfficiencyAnalyzerMuon mueffana2(&in, &out2, 0, "EfficiencyMuonVSGenMultiNoScale");
  mueffana2.analyze("z_yieldVsGen_noscale.tex");
  mueffana2.analyze();
  out1.Close();
  out2.Close();
*/
/*
  //madgraph
  TFile inMadgraph("signalMadgraph.root");
  TFile out1Madgraph("signal_Eff_vsRecoMadgraph.root", "RECREATE");
  TFile out2Madgraph("signal_Eff_vsGenMadgraph.root", "RECREATE");
  EfficiencyAnalyzerMuon mueffana1Madgraph(&inMadgraph, &out1Madgraph, 0, "EfficiencyMuonVSRecoMultiNoScale");
  mueffana1Madgraph.analyze("z_yieldVsRecMadgraph_noscale.tex");
  //EfficiencyAnalyzerMuon mueffana2Madgraph(&inMadgraph, &out2Madgraph, 0, "EfficiencyMuonVSGenMultiNoScale");
  //mueffana2Madgraph.analyze("z_yieldVsGenMadgraph_noscale.tex");
  out1Madgraph.Close();
  out2Madgraph.Close();
*/  
/*  
  //tune DW
  TFile inDW("signalDW.root");
  TFile out1DW("signal_Eff_vsRecoDW.root", "RECREATE");
  TFile out2DW("signal_Eff_vsGenDW.root", "RECREATE");
  EfficiencyAnalyzerMuon mueffana1DW(&inDW, &out1DW, 0, "EfficiencyMuonVSRecoMultiNoScale");
  mueffana1DW.analyze("z_yieldVsRecDW_noscale.tex");
  EfficiencyAnalyzerMuon mueffana2DW(&inDW, &out2DW, 0, "EfficiencyMuonVSGenMultiNoScale");
  mueffana2DW.analyze("z_yieldVsGenDW_noscale.tex");
  out1DW.Close();
  out2DW.Close();
  //tune P0
  TFile inP0("signalP0.root");
  TFile out1P0("signal_Eff_vsRecoP0.root", "RECREATE");
  TFile out2P0("signal_Eff_vsGenP0.root", "RECREATE");
  EfficiencyAnalyzerMuon mueffana1P0(&inP0, &out1P0, 0, "EfficiencyMuonVSRecoMultiNoScale");
  mueffana1P0.analyze("z_yieldVsRecP0_noscale.tex");
  EfficiencyAnalyzerMuon mueffana2P0(&inP0, &out2P0, 0, "EfficiencyMuonVSGenMultiNoScale");
  mueffana2P0.analyze("z_yieldVsGenP0_noscale.tex");
  out1P0.Close();
  out2P0.Close();
  //tune P0
  TFile inProPT0("signalProPT0.root");
  TFile out1ProPT0("signal_Eff_vsRecoProPT0.root", "RECREATE");
  TFile out2ProPT0("signal_Eff_vsGenProPT0.root", "RECREATE");
  EfficiencyAnalyzerMuon mueffana1ProPT0(&inProPT0, &out1ProPT0, 0, "EfficiencyMuonVSRecoMultiNoScale");
  mueffana1ProPT0.analyze("z_yieldVsRecProPT0_noscale.tex");
  EfficiencyAnalyzerMuon mueffana2ProPT0(&inProPT0, &out2ProPT0, 0, "EfficiencyMuonVSGenMultiNoScale");
  mueffana2ProPT0.analyze("z_yieldVsGenProPT0_noscale.tex");
  out1ProPT0.Close();
  out2ProPT0.Close();
  //tune P0
  TFile inProQ20("signalProQ20.root");
  TFile out1ProQ20("signal_Eff_vsRecoProQ20.root", "RECREATE");
  TFile out2ProQ20("signal_Eff_vsGenProQ20.root", "RECREATE");
  EfficiencyAnalyzerMuon mueffana1ProQ20(&inProQ20, &out1ProQ20, 0, "EfficiencyMuonVSRecoMultiNoScale");
  mueffana1ProQ20.analyze("z_yieldVsRecProQ20_noscale.tex");
  EfficiencyAnalyzerMuon mueffana2ProQ20(&inProQ20, &out2ProQ20, 0, "EfficiencyMuonVSGenMultiNoScale");
  mueffana2ProQ20.analyze("z_yieldVsGenProQ20_noscale.tex");
  out1ProQ20.Close();
  out2ProQ20.Close();
*/  
/*
  EfficiencyAnalyzerMuon zyield_ana(&in, 0, 0, "EfficiencyMuonVSRecoMulti");
  zyield_ana.analyze("z_yield.tex", 0.198 );


  EfficiencyAnalyzerMuon zyield_anaMad(&inMadgraph, 0, 0, "EfficiencyMuonVSRecoMulti");
  zyield_anaMad.analyze("z_yieldMad.tex", 0.198);


  TFile inW("w.root");
  EfficiencyAnalyzerMuon wyield_ana(&inW, 0, 0, "EfficiencyMuonVSRecoMulti");
  wyield_ana.analyze("w_yield.tex", 0.198);  
  wyield_ana.analyze("w_yield_noscale.tex", 2087693/(10312.*0.742));

  TFile inQCD("qcd.root");
  EfficiencyAnalyzerMuon qcdyield_ana(&inQCD, 0, 0, "EfficiencyMuonVSRecoMulti");
  qcdyield_ana.analyze("qcd_yield.tex", 0.198);
  qcdyield_ana.analyze("qcd_yield_noscale.tex", 5120334/(296900000.*2.684e-4));

  TFile inTTbar("ttbar.root");
  EfficiencyAnalyzerMuon ttbaryield_ana(&inTTbar, 0, 0, "EfficiencyMuonVSRecoMulti");
  ttbaryield_ana.analyze("ttbar_yield.tex", 0.198);
  ttbaryield_ana.analyze("ttbar_yield_noscale.tex", 632010/95.);

  TFile intau("ztautau.root");
  EfficiencyAnalyzerMuon tauyield_ana(&intau, 0, 0, "EfficiencyMuonVSRecoMulti");
  tauyield_ana.analyze("ztautau_yield.tex", 0.198);
  tauyield_ana.analyze("ztautau_yield_noscale.tex", 1369709/1667.);

  TFile indata("zmumu.root");
  EfficiencyAnalyzerMuon yield_ana(&indata, 0, 0, "EfficiencyMuonVSRecoMulti");
  yield_ana.analyze("yield.tex");
*/
}
