#include "Firenze/PATAnalysis/include/EfficiencyAnalyzerMuon.h"
#include "Firenze/PATAnalysis/include/TagAndProbeAnalyzer.h"
#include <iostream>

#include "TH1D.h"
#include "TGraphAsymmErrors.h"

EfficiencyAnalyzerMuon::EfficiencyAnalyzerMuon(TFile* input, TFile* output, std::string dirname): _dir(0), _output(output){
  //file->Open("UPDATE");
  _dir = (TDirectory*) input->Get(dirname.c_str());
}


void EfficiencyAnalyzerMuon::analyze(){
  if (!_dir) {
    std::cout << "Error in EfficiencyAnalyzerMuon, dir not found in file" << std::endl;
    return;
  }

  TH1D *acc_num = (TH1D *) _dir->Get("acceptance_numerator");
  TH1D *acc_den = (TH1D *) _dir->Get("acceptance_denominator");

  TH1D *TwoMuons = (TH1D *) _dir->Get("TwoMuons");
  TH1D *TM_MuTriggered = (TH1D *) _dir->Get("TM_MuT");
  TH1D *TM_MuT_OppositeCharge = (TH1D *) _dir->Get("TM_MuT_OppositeCharge");
  TH1D *TM_MuT_OC_Mass  = (TH1D *) _dir->Get("TM_MuT_OC_Mass");
  TH1D *TM_MuT_OC_M_QualityCuts = (TH1D *) _dir->Get("TM_MuT_OC_M_QualityCuts");
  TH1D *TM_MuT_OC_M_QC_DXY  = (TH1D *) _dir->Get("TM_MuT_OC_M_QC_DXY");
  TH1D *TM_MuT_OC_M_QC_DXY_Iso = (TH1D *) _dir->Get("TM_MuT_OC_M_QC_DXY_Iso"); 
 
  TH1D *generalefficiency_numerator = (TH1D *) _dir->Get("efficiency_numerator"); 
  TH1D *generalefficiency_denominator = (TH1D *) _dir->Get("efficiency_denominator");

  _output->cd();
  TGraphAsymmErrors acceptance(acc_num, acc_den);
  acceptance.SetNameTitle("acceptance", "acceptance");
  
  TGraphAsymmErrors efficiency(generalefficiency_numerator, generalefficiency_denominator);
  efficiency.SetNameTitle("efficiency", "Reconstruction efficiency");

  TGraphAsymmErrors TwoMuonsEff(TwoMuons, generalefficiency_denominator);
  TwoMuonsEff.SetNameTitle("TwoMuonsEff", "2 muons eff");

  TGraphAsymmErrors TM_MuTriggeredEff(TM_MuTriggered, TwoMuons);
  TM_MuTriggeredEff.SetNameTitle("TM_MuTriggeredEff", "2 muons + trigger eff");

  TGraphAsymmErrors TM_MuT_OppositeCharge_Eff(TM_MuT_OppositeCharge, TM_MuTriggered);
  TM_MuT_OppositeCharge_Eff.SetNameTitle("TM_MuT_OppositeCharge_Eff", "2 triggered muons Opposite Charge eff");

  TGraphAsymmErrors TM_MuT_OC_Mass_Eff(TM_MuT_OC_Mass, TM_MuT_OppositeCharge);
  TM_MuT_OC_Mass_Eff.SetNameTitle("TM_MuT_OC_Mass_Eff", "2 triggered muons Opposite Charge with Mass constraint");

  TGraphAsymmErrors TM_MuT_OC_M_QualityCuts_Eff(TM_MuT_OC_M_QualityCuts, TM_MuT_OC_Mass);
  TM_MuT_OC_M_QualityCuts_Eff.SetNameTitle("TM_MuT_OC_M_QualityCuts_Eff", "2 triggered muons Opposite Charge with Mass constraint Quality Cuts eff");

  TGraphAsymmErrors TM_MuT_OC_M_QC_DXY_Eff(TM_MuT_OC_M_QC_DXY, TM_MuT_OC_M_QualityCuts);
  TM_MuT_OC_M_QC_DXY_Eff.SetNameTitle("TM_MuT_OC_M_QC_DXY_Eff", "2 triggered muons Opposite Charge with Mass constraint with Quality Cuts and DXY cut eff");

  TGraphAsymmErrors TM_MuT_OC_M_QC_DXY_Iso_Eff(TM_MuT_OC_M_QC_DXY_Iso, TM_MuT_OC_M_QC_DXY);
  TM_MuT_OC_M_QC_DXY_Iso_Eff.SetNameTitle("TM_MuT_OC_M_QC_DXY_Iso_Eff", "2 triggered muons Opposite Charge Quality Cuts and DXY cut, mass constraint with isolation eff");

  acceptance.Write();

  efficiency.Write();

  TwoMuonsEff.Write();
  TM_MuTriggeredEff.Write();
  TM_MuT_OppositeCharge_Eff.Write();
  TM_MuT_OC_Mass_Eff.Write();
  TM_MuT_OC_M_QualityCuts_Eff.Write();
  TM_MuT_OC_M_QC_DXY_Eff.Write();
  TM_MuT_OC_M_QC_DXY_Iso_Eff.Write();
/*
  TagAndProbeAnalyzer tpana(_dir, _output, "TM_MuT_OC_M_QC_DXY_Iso");
  tpana.analyze(2);
*/ 
}
