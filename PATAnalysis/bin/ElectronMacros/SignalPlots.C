#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>

#include "TH1D.h"
#include "TVectorD.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "THStack.h"
#include "TDirectory.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TGraphAsymmErrors.h"

using namespace std;


void SignalPlot(){
	
	int _Acc  = 1;
	int _Qual = 0;
	int _Imp  = 2;
	int _Iso  = 3;
	int _EiD  = 4;
	
	string _RecoCutFlags[6];
	for(int i=0; i<6; i++){
		_RecoCutFlags[i] = "_1";}
	
	_RecoCutFlags[_Acc] = "_Acc";
	_RecoCutFlags[_Qual] = "_Qual";
	_RecoCutFlags[_Imp] = "_Imp";
	_RecoCutFlags[_Iso] = "_Iso";
	_RecoCutFlags[_EiD] = "_EiD";

    TFile *signal_file = TFile::Open("Zee_7TeV_RL01_10234.root");

    TFile* outplots = new TFile("TestSignalPlots.root", "RECREATE");

TDirectory *Eff, *Acc, *Pt_Eff, *Eta_Eff, *Jet_Eff, *Mass_Eff, *Dist, *GenRec, *IsoJet, *ChargeMisID;

Eff      = outplots->mkdir("Efficiency");
Acc      = outplots->mkdir("Acceptance");
GenRec   = outplots->mkdir("GenRec_Plots");
IsoJet   = outplots->mkdir("JetIsolation");
Mass_Eff = Eff->mkdir("Efficiency_vs_ZMass");
Pt_Eff = Eff->mkdir("Efficiency_vs_ZPt");
Eta_Eff = Eff->mkdir("Efficiency_vs_ZEta");
Jet_Eff = Eff->mkdir("Efficiency_vs_GenJet");
Dist    = Eff->mkdir("Distributions");
ChargeMisID = outplots->mkdir("ChargeMisID");
	
   /////////////Efficiency

   //Denominators
   
   TH1D* AccDenom_genMassZ = (TH1D*) signal_file->Get("EfficiencyElectron/AccDenom_genMassZ");
   TH1D* AccDenom_genPtZ = (TH1D*) signal_file->Get("EfficiencyElectron/AccDenom_genPtZ");
   TH1D* AccDenom_genEtaZ = (TH1D*) signal_file->Get("EfficiencyElectron/AccDenom_genEtaZ");
   TH1D* AccDenom_GenIsoJetNumber = (TH1D*) signal_file->Get("EfficiencyElectron/AccDenom_GenIsoJetNumber");
	
   TH1D* EffDenom_genMassZ = (TH1D*) signal_file->Get("EfficiencyElectron/EffDenom_genMassZ");
   TH1D* EffDenom_genPtZ = (TH1D*) signal_file->Get("EfficiencyElectron/EffDenom_genPtZ");
   TH1D* EffDenom_genEtaZ = (TH1D*) signal_file->Get("EfficiencyElectron/EffDenom_genEtaZ");
   TH1D* EffDenom_GenIsoJetNumber = (TH1D*) signal_file->Get("EfficiencyElectron/EffDenom_GenIsoJetNumber");
	
   //Reco in acc. efficiency
    
	string genMassZEff_name = "EfficiencyElectron/genMassZEff";
	genMassZEff_name+=_RecoCutFlags[1].c_str();
    TH1D* genMassZ_1 = (TH1D*) signal_file->Get(genMassZEff_name.c_str());
	genMassZEff_name+=_RecoCutFlags[2].c_str();
	TH1D* genMassZ_12 = (TH1D*) signal_file->Get(genMassZEff_name.c_str());
	genMassZEff_name+=_RecoCutFlags[3].c_str();
	TH1D* genMassZ_123 = (TH1D*) signal_file->Get(genMassZEff_name.c_str());
	genMassZEff_name+=_RecoCutFlags[4].c_str();
	TH1D* genMassZ_1234 = (TH1D*) signal_file->Get(genMassZEff_name.c_str());
	genMassZEff_name+=_RecoCutFlags[5].c_str();
	TH1D* genMassZ_12345 = (TH1D*) signal_file->Get(genMassZEff_name.c_str());
	
	string genPtZEff_name = "EfficiencyElectron/genPtZEff";
	genPtZEff_name+=_RecoCutFlags[1].c_str();
    TH1D* genPtZ_1 = (TH1D*) signal_file->Get(genPtZEff_name.c_str());
	genPtZEff_name+=_RecoCutFlags[2].c_str();
	TH1D* genPtZ_12 = (TH1D*) signal_file->Get(genPtZEff_name.c_str());
	genPtZEff_name+=_RecoCutFlags[3].c_str();
	TH1D* genPtZ_123 = (TH1D*) signal_file->Get(genPtZEff_name.c_str());
	genPtZEff_name+=_RecoCutFlags[4].c_str();
	TH1D* genPtZ_1234 = (TH1D*) signal_file->Get(genPtZEff_name.c_str());
	genPtZEff_name+=_RecoCutFlags[5].c_str();
	TH1D* genPtZ_12345 = (TH1D*) signal_file->Get(genPtZEff_name.c_str());
	
	string genEtaZEff_name = "EfficiencyElectron/genEtaZEff";
	genEtaZEff_name+=_RecoCutFlags[1].c_str();
    TH1D* genEtaZ_1 = (TH1D*) signal_file->Get(genEtaZEff_name.c_str());
	genEtaZEff_name+=_RecoCutFlags[2].c_str();
	TH1D* genEtaZ_12 = (TH1D*) signal_file->Get(genEtaZEff_name.c_str());
	genEtaZEff_name+=_RecoCutFlags[3].c_str();
	TH1D* genEtaZ_123 = (TH1D*) signal_file->Get(genEtaZEff_name.c_str());
	genEtaZEff_name+=_RecoCutFlags[4].c_str();
	TH1D* genEtaZ_1234 = (TH1D*) signal_file->Get(genEtaZEff_name.c_str());
	genEtaZEff_name+=_RecoCutFlags[5].c_str();
	TH1D* genEtaZ_12345 = (TH1D*) signal_file->Get(genEtaZEff_name.c_str());
	
	string genIsoJetEff_name = "EfficiencyElectron/GenIsoJetEff";
	genIsoJetEff_name+=_RecoCutFlags[1].c_str();
    TH1D* GenIsoJetNumber_1 = (TH1D*) signal_file->Get(genIsoJetEff_name.c_str());
	genIsoJetEff_name+=_RecoCutFlags[2].c_str();
    TH1D* GenIsoJetNumber_12 = (TH1D*) signal_file->Get(genIsoJetEff_name.c_str());
	genIsoJetEff_name+=_RecoCutFlags[3].c_str();
	TH1D* GenIsoJetNumber_123 = (TH1D*) signal_file->Get(genIsoJetEff_name.c_str());
	genIsoJetEff_name+=_RecoCutFlags[4].c_str();
	TH1D* GenIsoJetNumber_1234 = (TH1D*) signal_file->Get(genIsoJetEff_name.c_str());
	genIsoJetEff_name+=_RecoCutFlags[5].c_str();
	TH1D* GenIsoJetNumber_12345 = (TH1D*) signal_file->Get(genIsoJetEff_name.c_str());
		
	
   //Acceptance calculation	
	
	Acc->cd();
	
	TGraphAsymmErrors Acc_VsZMass(EffDenom_genMassZ, AccDenom_genMassZ);
	Acc_VsZMass.SetNameTitle("Acc_ZMass", "Acceptance vs gen Z Mass");
	TGraphAsymmErrors Acc_VsZPt(EffDenom_genPtZ, AccDenom_genPtZ);
	Acc_VsZPt.SetNameTitle("Acc_ZPt", "Acceptance vs gen Z Pt");
	TGraphAsymmErrors Acc_VsZEta(EffDenom_genEtaZ, AccDenom_genEtaZ);
	Acc_VsZEta.SetNameTitle("Acc_ZEta", "Acceptance vs gen Z Eta");
	TGraphAsymmErrors Acc_VsJetN(EffDenom_GenIsoJetNumber, AccDenom_GenIsoJetNumber);
	Acc_VsJetN.SetNameTitle("Acc_VsJetN", "Acceptance vs gen Iso Jet Number");
	
	Acc_VsZMass.Write();
	Acc_VsZPt.Write();
	Acc_VsZEta.Write();
	Acc_VsJetN.Write();
	
   //Efficiency calculation vs Z Mass
	
	Mass_Eff->cd();
	
	string EffVsZMass_name = "EffVsZMass";
	EffVsZMass_name+=_RecoCutFlags[1].c_str();
	TGraphAsymmErrors Eff_ZMass_1(genMassZ_1, EffDenom_genMassZ);
	Eff_ZMass_1.SetNameTitle(EffVsZMass_name.c_str(), _RecoCutFlags[1].c_str());
	EffVsZMass_name+=_RecoCutFlags[2].c_str();
	TGraphAsymmErrors Eff_ZMass_12(genMassZ_12, EffDenom_genMassZ);
	Eff_ZMass_12.SetNameTitle(EffVsZMass_name.c_str(), _RecoCutFlags[2].c_str());
	EffVsZMass_name+=_RecoCutFlags[3].c_str();
	TGraphAsymmErrors Eff_ZMass_123(genMassZ_123, EffDenom_genMassZ);
	Eff_ZMass_123.SetNameTitle(EffVsZMass_name.c_str(), _RecoCutFlags[3].c_str());
	EffVsZMass_name+=_RecoCutFlags[4].c_str();
	TGraphAsymmErrors Eff_ZMass_1234(genMassZ_1234, EffDenom_genMassZ);
	Eff_ZMass_1234.SetNameTitle(EffVsZMass_name.c_str(), _RecoCutFlags[4].c_str());
	EffVsZMass_name+=_RecoCutFlags[5].c_str();
	TGraphAsymmErrors Eff_ZMass_12345(genMassZ_12345, EffDenom_genMassZ);
	Eff_ZMass_12345.SetNameTitle(EffVsZMass_name.c_str(), _RecoCutFlags[5].c_str());
	
	
	Eff_ZMass_1.Write();
	Eff_ZMass_12.Write();
	Eff_ZMass_123.Write();
	Eff_ZMass_1234.Write();
	Eff_ZMass_12345.Write(); 
	
		
   //Efficiency calculation vs Z Pt
	
	Pt_Eff->cd();
	
	string EffVsZPt_name = "EffVsZPt";
	EffVsZPt_name+=_RecoCutFlags[1].c_str();
	TGraphAsymmErrors Eff_ZPt_1(genPtZ_1, EffDenom_genPtZ);
	Eff_ZPt_1.SetNameTitle(EffVsZPt_name.c_str(), _RecoCutFlags[1].c_str());
	EffVsZPt_name+=_RecoCutFlags[2].c_str();
	TGraphAsymmErrors Eff_ZPt_12(genPtZ_12, EffDenom_genPtZ);
	Eff_ZPt_12.SetNameTitle(EffVsZPt_name.c_str(), _RecoCutFlags[2].c_str());
	EffVsZPt_name+=_RecoCutFlags[3].c_str();
	TGraphAsymmErrors Eff_ZPt_123(genPtZ_123, EffDenom_genPtZ);
	Eff_ZPt_123.SetNameTitle(EffVsZPt_name.c_str(), _RecoCutFlags[3].c_str());
	EffVsZPt_name+=_RecoCutFlags[4].c_str();
	TGraphAsymmErrors Eff_ZPt_1234(genPtZ_1234, EffDenom_genPtZ);
	Eff_ZPt_1234.SetNameTitle(EffVsZPt_name.c_str(), _RecoCutFlags[4].c_str());
	EffVsZPt_name+=_RecoCutFlags[5].c_str();
	TGraphAsymmErrors Eff_ZPt_12345(genPtZ_12345, EffDenom_genPtZ);
	Eff_ZPt_12345.SetNameTitle(EffVsZPt_name.c_str(), _RecoCutFlags[5].c_str());
	
	Eff_ZPt_1.Write();
	Eff_ZPt_12.Write();
	Eff_ZPt_123.Write();
	Eff_ZPt_1234.Write();
    Eff_ZPt_12345.Write();
	
		
   //Efficiency calculation vs Z Eta
	
	Eta_Eff->cd();
	
	string EffVsZEta_name = "EffVsZEta";
	EffVsZEta_name+=_RecoCutFlags[1].c_str();
	TGraphAsymmErrors Eff_ZEta_1(genEtaZ_1, EffDenom_genEtaZ);
	Eff_ZEta_1.SetNameTitle(EffVsZEta_name.c_str(), _RecoCutFlags[1].c_str());
	EffVsZEta_name+=_RecoCutFlags[2].c_str();
	TGraphAsymmErrors Eff_ZEta_12(genEtaZ_12, EffDenom_genEtaZ);
	Eff_ZEta_12.SetNameTitle(EffVsZEta_name.c_str(), _RecoCutFlags[2].c_str());
	EffVsZEta_name+=_RecoCutFlags[3].c_str();
	TGraphAsymmErrors Eff_ZEta_123(genEtaZ_123, EffDenom_genEtaZ);
	Eff_ZEta_123.SetNameTitle(EffVsZEta_name.c_str(), _RecoCutFlags[3].c_str());
	EffVsZEta_name+=_RecoCutFlags[4].c_str();
	TGraphAsymmErrors Eff_ZEta_1234(genEtaZ_1234, EffDenom_genEtaZ);
	Eff_ZEta_1234.SetNameTitle(EffVsZEta_name.c_str(), _RecoCutFlags[4].c_str());
	EffVsZEta_name+=_RecoCutFlags[5].c_str();
	TGraphAsymmErrors Eff_ZEta_12345(genEtaZ_12345, EffDenom_genEtaZ);
	Eff_ZEta_12345.SetNameTitle(EffVsZEta_name.c_str(), _RecoCutFlags[5].c_str());
	
	Eff_ZEta_1.Write();
	Eff_ZEta_12.Write();
	Eff_ZEta_123.Write();
	Eff_ZEta_1234.Write();
	Eff_ZEta_12345.Write();
	
		
   //Efficiency calculation vs Jet Number
	
	Jet_Eff->cd();
	
	string EffVsJetN_name = "EffVsJetN";
	EffVsJetN_name+=_RecoCutFlags[1].c_str();
	TGraphAsymmErrors Eff_Jet_1(GenIsoJetNumber_1, EffDenom_GenIsoJetNumber);
	Eff_Jet_1.SetNameTitle(EffVsJetN_name.c_str(), _RecoCutFlags[1].c_str());
	EffVsJetN_name+=_RecoCutFlags[2].c_str();
	TGraphAsymmErrors Eff_Jet_12(GenIsoJetNumber_12, EffDenom_GenIsoJetNumber);
	Eff_Jet_12.SetNameTitle(EffVsJetN_name.c_str(), _RecoCutFlags[2].c_str());
	EffVsJetN_name+=_RecoCutFlags[3].c_str();
	TGraphAsymmErrors Eff_Jet_123(GenIsoJetNumber_123, EffDenom_GenIsoJetNumber);
	Eff_Jet_123.SetNameTitle(EffVsJetN_name.c_str(), _RecoCutFlags[3].c_str());
	EffVsJetN_name+=_RecoCutFlags[4].c_str();
	TGraphAsymmErrors Eff_Jet_1234(GenIsoJetNumber_1234, EffDenom_GenIsoJetNumber);
	Eff_Jet_1234.SetNameTitle(EffVsJetN_name.c_str(), _RecoCutFlags[4].c_str());
	EffVsJetN_name+=_RecoCutFlags[5].c_str();
	TGraphAsymmErrors Eff_Jet_12345(GenIsoJetNumber_12345, EffDenom_GenIsoJetNumber);
	Eff_Jet_12345.SetNameTitle(EffVsJetN_name.c_str(), _RecoCutFlags[5].c_str());
	
	Eff_Jet_1.Write();
	Eff_Jet_12.Write();
	Eff_Jet_123.Write();
	Eff_Jet_1234.Write();
	Eff_Jet_12345.Write(); 
	
		
   //Efficiency cut plots
	
	Jet_Eff->cd();
	TCanvas *EffJet = new TCanvas;
	Eff_Jet_1.GetYaxis()->SetRangeUser(0.3,1);
	Eff_Jet_1.GetXaxis()->SetTitle("gen Jet number");
	Eff_Jet_1.SetMarkerStyle(20);
	Eff_Jet_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	Eff_Jet_12.SetLineColor(2);
	Eff_Jet_12.SetMarkerStyle(21);
	Eff_Jet_12.SetMarkerColor(2);
	    Eff_Jet_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	Eff_Jet_123.SetLineColor(3);
	Eff_Jet_123.SetMarkerStyle(22);
	Eff_Jet_123.SetMarkerColor(3);
		Eff_Jet_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	Eff_Jet_1234.SetLineColor(4);
	Eff_Jet_1234.SetMarkerStyle(23);
	Eff_Jet_1234.SetMarkerColor(4);
		Eff_Jet_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
    Eff_Jet_12345.SetLineColor(5);
	Eff_Jet_12345.SetMarkerStyle(24);
	Eff_Jet_12345.SetMarkerColor(5);
		Eff_Jet_12345.Draw("PSAME");}
	TLegend *LegEffJ = EffJet->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegEffJ->Draw();
	Eff_Jet_1.SetTitle("Cut Efficiency vs Jet Number");
	EffJet->Write("CutEfficiencyVsJet.root");
	EffJet->Close();                                                           
	
	Mass_Eff->cd();
	TCanvas *EffZMass = new TCanvas;
	Eff_ZMass_1.GetYaxis()->SetRangeUser(0.2,1);
	Eff_ZMass_1.GetXaxis()->SetTitle("gen Z Mass");
	Eff_ZMass_1.SetMarkerStyle(20);
	Eff_ZMass_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	Eff_ZMass_12.SetLineColor(2);
	Eff_ZMass_12.SetMarkerStyle(21);
	Eff_ZMass_12.SetMarkerColor(2);
		Eff_ZMass_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	Eff_ZMass_123.SetLineColor(3);
	Eff_ZMass_123.SetMarkerStyle(22);
	Eff_ZMass_123.SetMarkerColor(3);
		Eff_ZMass_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	Eff_ZMass_1234.SetLineColor(4);
	Eff_ZMass_1234.SetMarkerStyle(23);
	Eff_ZMass_1234.SetMarkerColor(4);
		Eff_ZMass_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
	Eff_ZMass_12345.SetLineColor(5);
	Eff_ZMass_12345.SetMarkerStyle(24);
	Eff_ZMass_12345.SetMarkerColor(5);
		Eff_ZMass_12345.Draw("PSAME");}
	TLegend *LegEffZM = EffZMass->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegEffZM->Draw();
	Eff_ZMass_1.SetTitle("Cut Efficiency vs Z Mass");
	EffZMass->Write("CutEfficiencyVsZMass.root");
	EffZMass->Close();  
		
	Pt_Eff->cd();
	TCanvas *EffZPt = new TCanvas;
	Eff_ZPt_1.GetYaxis()->SetRangeUser(0.2,1.2);
	Eff_ZPt_1.GetXaxis()->SetTitle("gen Z Pt");
	Eff_ZPt_1.SetMarkerStyle(20);
	Eff_ZPt_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	Eff_ZPt_12.SetLineColor(2);
	Eff_ZPt_12.SetMarkerStyle(21);
	Eff_ZPt_12.SetMarkerColor(2);
		Eff_ZPt_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	Eff_ZPt_123.SetLineColor(3);
	Eff_ZPt_123.SetMarkerStyle(22);
	Eff_ZPt_123.SetMarkerColor(3);
		Eff_ZPt_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	Eff_ZPt_1234.SetLineColor(4);
	Eff_ZPt_1234.SetMarkerStyle(23);
	Eff_ZPt_1234.SetMarkerColor(4);
		Eff_ZPt_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
	Eff_ZPt_12345.SetLineColor(5);
	Eff_ZPt_12345.SetMarkerStyle(24);
	Eff_ZPt_12345.SetMarkerColor(5);
		Eff_ZPt_12345.Draw("PSAME");}
	TLegend *LegEffZPt = EffZPt->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegEffZPt->Draw();
	Eff_ZPt_1.SetTitle("Cut Efficiency vs Z Pt");
	EffZPt->Write("CutEfficiencyVsZPt.root");
	EffZPt->Close();
	
	Eta_Eff->cd();
	TCanvas *EffZEta = new TCanvas;
	Eff_ZEta_1.GetYaxis()->SetRangeUser(0.2,1.1);
    Eff_ZEta_1.GetXaxis()->SetTitle("gen Z Eta");
	Eff_ZEta_1.SetMarkerStyle(20);
    Eff_ZEta_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	Eff_ZEta_12.SetLineColor(2);
	Eff_ZEta_12.SetMarkerStyle(21);
	Eff_ZEta_12.SetMarkerColor(2);
		Eff_ZEta_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	Eff_ZEta_123.SetLineColor(3);
	Eff_ZEta_123.SetMarkerStyle(22);
	Eff_ZEta_123.SetMarkerColor(3);
		Eff_ZEta_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	Eff_ZEta_1234.SetLineColor(4);
	Eff_ZEta_1234.SetMarkerStyle(23);
	Eff_ZEta_1234.SetMarkerColor(4);
		Eff_ZEta_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
	Eff_ZEta_12345.SetLineColor(5);
	Eff_ZEta_12345.SetMarkerStyle(24);
	Eff_ZEta_12345.SetMarkerColor(5);
		Eff_ZEta_12345.Draw("PSAME");}
	TLegend *LegEffZEta = EffZEta->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegEffZEta->Draw();
	Eff_ZEta_1.SetTitle("Cut Efficiency vs Z Eta");
	EffZEta->Write("CutEfficiencyVsZEta.root");
	EffZEta->Close();
	
	//////////// Distributions
	
	Dist->cd();
	
	string recMassZ_name = "RecoElectron/recZ_Plots/recMassZ";
	recMassZ_name+=_RecoCutFlags[1].c_str();
	TH1D* recMassZ_1 = (TH1D*) signal_file->Get(recMassZ_name.c_str());
	recMassZ_name+=_RecoCutFlags[2].c_str();
	TH1D* recMassZ_12 = (TH1D*) signal_file->Get(recMassZ_name.c_str());
	recMassZ_name+=_RecoCutFlags[3].c_str();
	TH1D* recMassZ_123 = (TH1D*) signal_file->Get(recMassZ_name.c_str());
	recMassZ_name+=_RecoCutFlags[4].c_str();
	TH1D* recMassZ_1234 = (TH1D*) signal_file->Get(recMassZ_name.c_str());
	recMassZ_name+=_RecoCutFlags[5].c_str();
	TH1D* recMassZ_12345 = (TH1D*) signal_file->Get(recMassZ_name.c_str());
	
	TCanvas *ZMass = new TCanvas;
	recMassZ_1->SetLineColor(1);
	recMassZ_1->GetXaxis()->SetTitle("rec Z Mass");
	recMassZ_1->SetTitle("Reconstructed Z Mass vs cuts applied");
	recMassZ_1->Draw();
	if(_RecoCutFlags[2] != "_1"){
	recMassZ_12->SetLineColor(2);
		recMassZ_12->Draw("sames");}
	if(_RecoCutFlags[3] != "_1"){
	recMassZ_123->SetLineColor(3);
		recMassZ_123->Draw("sames");}
	if(_RecoCutFlags[4] != "_1"){
	recMassZ_1234->SetLineColor(4);
		recMassZ_1234->Draw("sames");}
	if(_RecoCutFlags[5] != "_1"){
	recMassZ_12345->SetLineColor(5);
		recMassZ_12345->Draw("sames");}
	ZMass->Write("ZMass.root");
	ZMass->Close();
	
	string recPtZ_name = "RecoElectron/recZ_Plots/recPtZ";
	recPtZ_name+=_RecoCutFlags[1].c_str();
	TH1D* recPtZ_1 = (TH1D*) signal_file->Get(recPtZ_name.c_str());
	recPtZ_name+=_RecoCutFlags[2].c_str();
	TH1D* recPtZ_12 = (TH1D*) signal_file->Get(recPtZ_name.c_str());
	recPtZ_name+=_RecoCutFlags[3].c_str();
	TH1D* recPtZ_123 = (TH1D*) signal_file->Get(recPtZ_name.c_str());
	recPtZ_name+=_RecoCutFlags[4].c_str();
	TH1D* recPtZ_1234 = (TH1D*) signal_file->Get(recPtZ_name.c_str());
	recPtZ_name+=_RecoCutFlags[5].c_str();
	TH1D* recPtZ_12345 = (TH1D*) signal_file->Get(recPtZ_name.c_str());
	
	TCanvas *ZPt = new TCanvas;
	recPtZ_1->SetLineColor(1);
	recPtZ_1->GetXaxis()->SetTitle("rec Z Pt");
	recPtZ_1->SetTitle("Reconstructed Z Pt vs cuts applied");
	recPtZ_1->Draw();
	if(_RecoCutFlags[2] != "_1"){
	recPtZ_12->SetLineColor(2);
		recPtZ_12->Draw("sames");}
	if(_RecoCutFlags[3] != "_1"){
	recPtZ_123->SetLineColor(3);
		recPtZ_123->Draw("sames");}
	if(_RecoCutFlags[4] != "_1"){
	recPtZ_1234->SetLineColor(4);
		recPtZ_1234->Draw("sames");}
	if(_RecoCutFlags[5] != "_1"){
	recPtZ_12345->SetLineColor(5);
		recPtZ_12345->Draw("sames");}
	ZPt->Write("ZPt.root");
	ZPt->Close();
	
	string recEtaZ_name = "RecoElectron/recZ_Plots/recEtaZ";
	recEtaZ_name+=_RecoCutFlags[1].c_str();
	TH1D* recEtaZ_1 = (TH1D*) signal_file->Get(recEtaZ_name.c_str());
	recEtaZ_name+=_RecoCutFlags[2].c_str();
	TH1D* recEtaZ_12 = (TH1D*) signal_file->Get(recEtaZ_name.c_str());
	recEtaZ_name+=_RecoCutFlags[3].c_str();
	TH1D* recEtaZ_123 = (TH1D*) signal_file->Get(recEtaZ_name.c_str());
	recEtaZ_name+=_RecoCutFlags[4].c_str();
	TH1D* recEtaZ_1234 = (TH1D*) signal_file->Get(recEtaZ_name.c_str());
	recEtaZ_name+=_RecoCutFlags[5].c_str();
	TH1D* recEtaZ_12345 = (TH1D*) signal_file->Get(recEtaZ_name.c_str());
	
	TCanvas *ZEta = new TCanvas;
	recEtaZ_1->SetLineColor(1);
	recEtaZ_1->GetXaxis()->SetTitle("rec Z Eta");
	recEtaZ_1->SetTitle("Reconstructed Z Eta vs cuts applied");
	recEtaZ_1->Draw();
	if(_RecoCutFlags[2] != "_1"){
	recEtaZ_12->SetLineColor(2);
		recEtaZ_12->Draw("sames");}
	if(_RecoCutFlags[3] != "_1"){
	recEtaZ_123->SetLineColor(3);
		recEtaZ_123->Draw("sames");}
	if(_RecoCutFlags[4] != "_1"){
	recEtaZ_1234->SetLineColor(4);
		recEtaZ_1234->Draw("sames");}
	if(_RecoCutFlags[5] != "_1"){
	recEtaZ_12345->SetLineColor(5);
		recEtaZ_12345->Draw("sames");}
	ZEta->Write("ZEta.root");
	ZEta->Close();
	
	string recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
	recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* recJetN_1 = (TH1D*) signal_file->Get(recJetN_name.c_str());
	recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* recJetN_12 = (TH1D*) signal_file->Get(recJetN_name.c_str());
	recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* recJetN_123 = (TH1D*) signal_file->Get(recJetN_name.c_str());
	recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* recJetN_1234 = (TH1D*) signal_file->Get(recJetN_name.c_str());
	recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* recJetN_12345 = (TH1D*) signal_file->Get(recJetN_name.c_str());
	
	TCanvas *JetN = new TCanvas;
	recJetN_1->SetLineColor(1);
	recJetN_1->GetXaxis()->SetTitle("rec Jet number");
	recJetN_1->SetTitle("Reconstructed Jet number vs cuts applied");
	recJetN_1->Draw();
	if(_RecoCutFlags[2] != "_1"){
	recJetN_12->SetLineColor(2);
		recJetN_12->Draw("sames");}
	if(_RecoCutFlags[3] != "_1"){
	recJetN_123->SetLineColor(3);
		recJetN_123->Draw("sames");}
	if(_RecoCutFlags[4] != "_1"){
	recJetN_1234->SetLineColor(4);
		recJetN_1234->Draw("sames");}
	if(_RecoCutFlags[5] != "_1"){
	recJetN_12345->SetLineColor(5);
		recJetN_12345->Draw("sames");}
	JetN->Write("JetN.root");
	JetN->Close();
	
	////////////////// Gen - Rec Plots
	
	GenRec->cd();
	
	TH1D* GENMassZ_Acc = (TH1D*) signal_file->Get("GenElectron/genZ_Plots/genMassZ_Acc");
    TH1D* GENPtZ_Acc = (TH1D*) signal_file->Get("GenElectron/genZ_Plots/genPtZ_Acc");
    TH1D* GENEtaZ_Acc = (TH1D*) signal_file->Get("GenElectron/genZ_Plots/genEtaZ_Acc");
    TH1D* GENJetN_Acc = (TH1D*) signal_file->Get("GenElectron/genJet_Plots/GenIsoJetCounter_Acc");
	
	TCanvas *GenRec_ZMass = new TCanvas;
	recMassZ_12345->SetLineColor(1);
	recMassZ_12345->GetXaxis()->SetTitle("Z Mass");
	recMassZ_12345->SetTitle("Reconstructed (black) and Generated (red) Z Mass");
	recMassZ_12345->Draw();
	GENMassZ_Acc->SetLineColor(2);
	GENMassZ_Acc->Draw("sames");
	GenRec_ZMass->Write("GenRec_ZMass.root");
	GenRec_ZMass->Close();
	
	TCanvas *GenRec_ZPt = new TCanvas;
	recPtZ_12345->SetLineColor(1);
	recPtZ_12345->GetXaxis()->SetTitle("Z Pt");
	recPtZ_12345->SetTitle("Reconstructed (black) and Generated (red) Z Pt");
	recPtZ_12345->Draw();
	GENPtZ_Acc->SetLineColor(2);
	GENPtZ_Acc->Draw("sames");
	GenRec_ZPt->Write("GenRec_ZPt.root");
	GenRec_ZPt->Close();
	
	TCanvas *GenRec_ZEta = new TCanvas;
	recEtaZ_12345->SetLineColor(1);
	recEtaZ_12345->GetXaxis()->SetTitle("Z Eta");
	recEtaZ_12345->SetTitle("Reconstructed (black) and Generated (red) Z Eta");
	recEtaZ_12345->Draw();
	GENEtaZ_Acc->SetLineColor(2);
	GENEtaZ_Acc->Draw("sames");
	GenRec_ZEta->Write("GenRec_ZEta.root");
	GenRec_ZEta->Close();
	
	TCanvas *GenRec_JetN = new TCanvas;
	recJetN_12345->SetLineColor(1);
	recJetN_12345->GetXaxis()->SetTitle("Jet number");
	recJetN_12345->SetTitle("Reconstructed (black) and Generated (red) Jet Number");
	recJetN_12345->Draw();
	GENJetN_Acc->SetLineColor(2);
	GENJetN_Acc->Draw("sames");
	GenRec_JetN->Write("GenRec_JetN.root");
	GenRec_JetN->Close();
	
	////////// Jet Isolation
	
	IsoJet->cd();
	
	TH1D* GENJetPt = (TH1D*) signal_file->Get("GenElectron/genJet_Plots/GenJetPt_Acc");
	TH1D* GENIsoJetPt = (TH1D*) signal_file->Get("GenElectron/genJet_Plots/GenIsoJetPt_Acc");
	
	string RECJetPt_name = "RecoElectron/recJet_Plots/RecoJetPt";
	RECJetPt_name+=_RecoCutFlags[1].c_str();
	RECJetPt_name+=_RecoCutFlags[2].c_str();
	RECJetPt_name+=_RecoCutFlags[3].c_str();
	RECJetPt_name+=_RecoCutFlags[4].c_str();
	RECJetPt_name+=_RecoCutFlags[5].c_str();
	TH1D* RECJetPt_12345 = (TH1D*) signal_file->Get(RECJetPt_name.c_str());
	
	string RECIsoJetPt_name = "RecoElectron/recJet_Plots/RecoIsoJetPt";
	RECIsoJetPt_name+=_RecoCutFlags[1].c_str();
	RECIsoJetPt_name+=_RecoCutFlags[2].c_str();
	RECIsoJetPt_name+=_RecoCutFlags[3].c_str();
	RECIsoJetPt_name+=_RecoCutFlags[4].c_str();
	RECIsoJetPt_name+=_RecoCutFlags[5].c_str();
	TH1D* RECIsoJetPt_12345 = (TH1D*) signal_file->Get(RECIsoJetPt_name.c_str());
	
	TCanvas *AllJetPt = new TCanvas;
	RECJetPt_12345->GetXaxis()->SetTitle("Jet Pt");
	RECJetPt_12345->SetTitle("Reconstructed (black) and Generated (red) Jet Pt, NO jet-isolation cut applied");
	RECJetPt_12345->Draw();
	GENJetPt->SetLineColor(2);
	GENJetPt->Draw("sames");
	AllJetPt->Write("AllJetPt.root");
	AllJetPt->Close();
	
	TCanvas *IsoJetPt = new TCanvas;
	RECIsoJetPt_12345->GetXaxis()->SetTitle("Jet Pt");
	RECIsoJetPt_12345->SetTitle("Reconstructed (black) and Generated (red) Jet Pt, jet-isolation cut applied");
	RECIsoJetPt_12345->Draw();
	GENIsoJetPt->SetLineColor(2);
	GENIsoJetPt->Draw("sames");
	IsoJetPt->Write("IsoJetPt.root");
	IsoJetPt->Close();
	
	////////// Charge Misidentification
	
	ChargeMisID->cd();
	
    TH1D* AllEl_Pt_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_Pt_Acc");
	TH1D* AllEl_Eta_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_Eta_Acc");
	TH1D* AllEl_Hit_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_Hit_Acc");
	TH1D* AllEl_fBrem_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_fBrem_Acc");
	TH1D* AllEl_IP_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_IP_Acc");
	TH1D* ChargeMisID_Pt_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_Pt_Acc");
	TH1D* ChargeMisID_Eta_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_Eta_Acc");
	TH1D* ChargeMisID_Hit_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_Hit_Acc");
	TH1D* ChargeMisID_fBrem_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_fBrem_Acc");
	TH1D* ChargeMisID_IP_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_IP_Acc");
	
	
	ChargeMisID_Eta_Acc->Divide(AllEl_Eta_Acc);
	ChargeMisID_Eta_Acc->GetXaxis()->SetTitle("gen Eta");
	ChargeMisID_Eta_Acc->SetName("ChargeMisID_Eta");
	ChargeMisID_Eta_Acc->Write();
	ChargeMisID_Pt_Acc->Divide(AllEl_Pt_Acc);
	ChargeMisID_Pt_Acc->GetXaxis()->SetTitle("gen Pt");
	ChargeMisID_Pt_Acc->SetName("ChargeMisID_Pt");
	ChargeMisID_Pt_Acc->Write();
	ChargeMisID_Hit_Acc->Divide(AllEl_Hit_Acc);
	ChargeMisID_Hit_Acc->GetXaxis()->SetTitle("Number of Hits on track");
	ChargeMisID_Hit_Acc->SetName("ChargeMisID_Hit");
	ChargeMisID_Hit_Acc->Write();
	ChargeMisID_IP_Acc->Divide(AllEl_IP_Acc);
	ChargeMisID_IP_Acc->GetXaxis()->SetTitle("Impact Parameter");
	ChargeMisID_IP_Acc->SetName("ChargeMisID_IP");
	ChargeMisID_IP_Acc->Write();
	ChargeMisID_fBrem_Acc->Divide(AllEl_fBrem_Acc);
	ChargeMisID_fBrem_Acc->GetXaxis()->SetTitle("fBrem");
	ChargeMisID_fBrem_Acc->SetName("ChargeMisID_fBrem");
	ChargeMisID_fBrem_Acc->Write();	
		

  outplots->Write();
  outplots->Close();
}
