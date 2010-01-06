#include <vector>
#include <iostream>
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


void Plot(){
	
	int _Acc  = 1;
	int _Qual = 2;
	int _Imp  = 3;
	int _Iso  = 4;
	int _EiD  = 5;
	
	string _RecoCutFlags[6];
	for(int i=0; i<6; i++){
		_RecoCutFlags[i] = "_1";}
	
	_RecoCutFlags[_Acc] = "_Acc";
	_RecoCutFlags[_Qual] = "_Qual";
	_RecoCutFlags[_Imp] = "_Imp";
	_RecoCutFlags[_Iso] = "_Iso";
	_RecoCutFlags[_EiD] = "_EiD";

TFile *signal_file = TFile::Open("OutputFiles_Zee/Signal/New_selections/Signal_NewSel_Loose_Iso01.root");

TFile* QCD = TFile::Open("OutputFiles_Zee/Background/FullSample/New_Selections/Iso01/QCD_All_FullSample.root");
	
TFile* TTbar = TFile::Open("OutputFiles_Zee/Background/FullSample/New_Selections/Iso01/TTbar_RobustLoose_FullSample.root");
	
TFile* Wenu = TFile::Open("OutputFiles_Zee/Background/FullSample/New_Selections/Iso01/Wenu_skim_RL01.root");
	
TFile* outplots = new TFile("Plot_Background_NewCode.root", "RECREATE");

TDirectory *Mass, *Jet;

Mass     = outplots->mkdir("ZMass");
Jet      = outplots->mkdir("JetNumber");

	
   /////////////   Z Mass
	
	Mass->cd();
	
	string Signal_recMassZ_name = "RecoElectron/recMassZ";
	Signal_recMassZ_name+=_RecoCutFlags[1].c_str();
	TH1D* Signal_recMassZ_1 = (TH1D*) signal_file->Get(Signal_recMassZ_name.c_str());
	Signal_recMassZ_name+=_RecoCutFlags[2].c_str();
	TH1D* Signal_recMassZ_12 = (TH1D*) signal_file->Get(Signal_recMassZ_name.c_str());
	Signal_recMassZ_name+=_RecoCutFlags[3].c_str();
	TH1D* Signal_recMassZ_123 = (TH1D*) signal_file->Get(Signal_recMassZ_name.c_str());
	Signal_recMassZ_name+=_RecoCutFlags[4].c_str();
	TH1D* Signal_recMassZ_1234 = (TH1D*) signal_file->Get(Signal_recMassZ_name.c_str());
	Signal_recMassZ_name+=_RecoCutFlags[5].c_str();
	TH1D* Signal_recMassZ_12345 = (TH1D*) signal_file->Get(Signal_recMassZ_name.c_str());
	
	string QCD_recMassZ_name = "RecoElectron/recMassZ";
	QCD_recMassZ_name+=_RecoCutFlags[1].c_str();
	TH1D* QCD_recMassZ_1 = (TH1D*) QCD->Get(QCD_recMassZ_name.c_str());
	QCD_recMassZ_name+=_RecoCutFlags[2].c_str();
	TH1D* QCD_recMassZ_12 = (TH1D*) QCD->Get(QCD_recMassZ_name.c_str());
	QCD_recMassZ_name+=_RecoCutFlags[3].c_str();
	TH1D* QCD_recMassZ_123 = (TH1D*) QCD->Get(QCD_recMassZ_name.c_str());
	QCD_recMassZ_name+=_RecoCutFlags[4].c_str();
	TH1D* QCD_recMassZ_1234 = (TH1D*) QCD->Get(QCD_recMassZ_name.c_str());
	QCD_recMassZ_name+=_RecoCutFlags[5].c_str();
	TH1D* QCD_recMassZ_12345 = (TH1D*) QCD->Get(QCD_recMassZ_name.c_str());
	
	string TTbar_recMassZ_name = "RecoElectron/recMassZ";
	TTbar_recMassZ_name+=_RecoCutFlags[1].c_str();
	TH1D* TTbar_recMassZ_1 = (TH1D*) TTbar->Get(TTbar_recMassZ_name.c_str());
	TTbar_recMassZ_name+=_RecoCutFlags[2].c_str();
	TH1D* TTbar_recMassZ_12 = (TH1D*) TTbar->Get(TTbar_recMassZ_name.c_str());
	TTbar_recMassZ_name+=_RecoCutFlags[3].c_str();
	TH1D* TTbar_recMassZ_123 = (TH1D*) TTbar->Get(TTbar_recMassZ_name.c_str());
	TTbar_recMassZ_name+=_RecoCutFlags[4].c_str();
	TH1D* TTbar_recMassZ_1234 = (TH1D*) TTbar->Get(TTbar_recMassZ_name.c_str());
	TTbar_recMassZ_name+=_RecoCutFlags[5].c_str();
	TH1D* TTbar_recMassZ_12345 = (TH1D*) TTbar->Get(TTbar_recMassZ_name.c_str());
	
	string Wenu_recMassZ_name = "RecoElectron/recMassZ";
	Wenu_recMassZ_name+=_RecoCutFlags[1].c_str();
	TH1D* Wenu_recMassZ_1 = (TH1D*) Wenu->Get(Wenu_recMassZ_name.c_str());
	Wenu_recMassZ_name+=_RecoCutFlags[2].c_str();
	TH1D* Wenu_recMassZ_12 = (TH1D*) Wenu->Get(Wenu_recMassZ_name.c_str());
	Wenu_recMassZ_name+=_RecoCutFlags[3].c_str();
	TH1D* Wenu_recMassZ_123 = (TH1D*) Wenu->Get(Wenu_recMassZ_name.c_str());
	Wenu_recMassZ_name+=_RecoCutFlags[4].c_str();
	TH1D* Wenu_recMassZ_1234 = (TH1D*) Wenu->Get(Wenu_recMassZ_name.c_str());
	Wenu_recMassZ_name+=_RecoCutFlags[5].c_str();
	TH1D* Wenu_recMassZ_12345 = (TH1D*) Wenu->Get(Wenu_recMassZ_name.c_str());
		
	
	TCanvas *ZMass_1 = new TCanvas;
	Signal_recMassZ_1->SetLineColor(1);
	Signal_recMassZ_1->Draw();
	Signal_recMassZ_1->GetXaxis()->SetTitle("rec Z Mass");
	Signal_recMassZ_1->SetTitle("Signal");
	QCD_recMassZ_1->SetLineColor(2);
	QCD_recMassZ_1->SetFillColor(2);
	QCD_recMassZ_1->SetFillStyle(3002);
	QCD_recMassZ_1->SetTitle("QCD");
	QCD_recMassZ_1->Draw("sames");
	TTbar_recMassZ_1->SetLineColor(8);
	TTbar_recMassZ_1->SetFillColor(8);
	TTbar_recMassZ_1->SetFillStyle(3002);
	TTbar_recMassZ_1->SetTitle("TTbar");
	TTbar_recMassZ_1->Draw("sames");
	Wenu_recMassZ_1->SetLineColor(9);
	Wenu_recMassZ_1->SetFillColor(9);
	Wenu_recMassZ_1->SetFillStyle(3002);
	Wenu_recMassZ_1->SetTitle("Wenu");
	Wenu_recMassZ_1->Draw("sames");
	TLegend *LegMAcc = ZMass_1->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegMAcc->Draw();
	Signal_recMassZ_1->SetTitle("Reconstructed Z Mass");
	string ZMass_name = "ZMass";
	ZMass_name+=_RecoCutFlags[1].c_str();
	ZMass_1->Write(ZMass_name.c_str());
	ZMass_1->Close();
	
	if(_RecoCutFlags[2] != "_1"){
	TCanvas *ZMass_12 = new TCanvas;
	Signal_recMassZ_12->SetLineColor(1);
	Signal_recMassZ_12->Draw();
	Signal_recMassZ_12->GetXaxis()->SetTitle("rec Z Mass");
	Signal_recMassZ_12->SetTitle("Signal");
	QCD_recMassZ_12->SetLineColor(2);
	QCD_recMassZ_12->SetFillColor(2);
	QCD_recMassZ_12->SetFillStyle(3002);
	QCD_recMassZ_12->SetTitle("QCD");
	QCD_recMassZ_12->Draw("sames");
	TTbar_recMassZ_12->SetLineColor(8);
	TTbar_recMassZ_12->SetFillColor(8);
	TTbar_recMassZ_12->SetFillStyle(3002);
	TTbar_recMassZ_12->SetTitle("TTbar");
	TTbar_recMassZ_12->Draw("sames");
	Wenu_recMassZ_12->SetLineColor(9);
	Wenu_recMassZ_12->SetFillColor(9);
	Wenu_recMassZ_12->SetFillStyle(3002);
	Wenu_recMassZ_12->SetTitle("Wenu");
	Wenu_recMassZ_12->Draw("sames");
	TLegend *LegMAcc_Qual = ZMass_12->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegMAcc_Qual->Draw();
	Signal_recMassZ_12->SetTitle("Reconstructed Z Mass");
	ZMass_name+=_RecoCutFlags[2].c_str();
	ZMass_12->Write(ZMass_name.c_str());
		ZMass_12->Close();}
	
	if(_RecoCutFlags[3] != "_1"){
	TCanvas *ZMass_123 = new TCanvas;
	Signal_recMassZ_123->SetLineColor(1);
	Signal_recMassZ_123->Draw();
	Signal_recMassZ_123->GetXaxis()->SetTitle("rec Z Mass");
	Signal_recMassZ_123->SetTitle("Signal");
	QCD_recMassZ_123->SetLineColor(2);
	QCD_recMassZ_123->SetFillColor(2);
	QCD_recMassZ_123->SetFillStyle(3002);
	QCD_recMassZ_123->SetTitle("QCD");
	QCD_recMassZ_123->Draw("sames");
	TTbar_recMassZ_123->SetLineColor(8);
	TTbar_recMassZ_123->SetFillColor(8);
	TTbar_recMassZ_123->SetFillStyle(3002);
	TTbar_recMassZ_123->SetTitle("TTbar");
	TTbar_recMassZ_123->Draw("sames");
	Wenu_recMassZ_123->SetLineColor(9);
	Wenu_recMassZ_123->SetFillColor(9);
	Wenu_recMassZ_123->SetFillStyle(3002);
	Wenu_recMassZ_123->SetTitle("Wenu");
	Wenu_recMassZ_123->Draw("sames");
	TLegend *LegMAccQualImp = ZMass_123->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegMAccQualImp->Draw();
	Signal_recMassZ_123->SetTitle("Reconstructed Z Mass");
	ZMass_name+=_RecoCutFlags[3].c_str();
	ZMass_123->Write(ZMass_name.c_str());
		ZMass_123->Close();}
	
	if(_RecoCutFlags[4] != "_1"){
	TCanvas *ZMass_1234 = new TCanvas;
	Signal_recMassZ_1234->SetLineColor(1);
	Signal_recMassZ_1234->Draw();
	Signal_recMassZ_1234->GetXaxis()->SetTitle("rec Z Mass");
	Signal_recMassZ_1234->SetTitle("Signal");
	QCD_recMassZ_1234->SetLineColor(2);
	QCD_recMassZ_1234->SetFillColor(2);
	QCD_recMassZ_1234->SetFillStyle(3002);
	QCD_recMassZ_1234->SetTitle("QCD");
	QCD_recMassZ_1234->Draw("sames");
	TTbar_recMassZ_1234->SetLineColor(8);
	TTbar_recMassZ_1234->SetFillColor(8);
	TTbar_recMassZ_1234->SetFillStyle(3002);
	TTbar_recMassZ_1234->SetTitle("TTbar");
	TTbar_recMassZ_1234->Draw("sames");
	Wenu_recMassZ_1234->SetLineColor(9);
	Wenu_recMassZ_1234->SetFillColor(9);
	Wenu_recMassZ_1234->SetFillStyle(3002);
	Wenu_recMassZ_1234->SetTitle("Wenu");
	Wenu_recMassZ_1234->Draw("sames");
	TLegend *LegMAccQualImpIso = ZMass_1234->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegMAccQualImpIso->Draw();
	Signal_recMassZ_1234->SetTitle("Reconstructed Z Mass");
	ZMass_name+=_RecoCutFlags[4].c_str();
	ZMass_1234->Write(ZMass_name.c_str());
		ZMass_1234->Close();}
	
	if(_RecoCutFlags[5] != "_1"){
	TCanvas *ZMass_12345 = new TCanvas;
	Signal_recMassZ_12345->SetLineColor(1);
	Signal_recMassZ_12345->Draw();
	Signal_recMassZ_12345->GetXaxis()->SetTitle("rec Z Mass");
	Signal_recMassZ_12345->SetTitle("Signal");
	QCD_recMassZ_12345->SetLineColor(2);
	QCD_recMassZ_12345->SetFillColor(2);
	QCD_recMassZ_12345->SetFillStyle(3002);
	QCD_recMassZ_12345->SetTitle("QCD");
	QCD_recMassZ_12345->Draw("sames");
	TTbar_recMassZ_12345->SetLineColor(8);
	TTbar_recMassZ_12345->SetFillColor(8);
	TTbar_recMassZ_12345->SetFillStyle(3002);
	TTbar_recMassZ_12345->SetTitle("TTbar");
	TTbar_recMassZ_12345->Draw("sames");
	Wenu_recMassZ_12345->SetLineColor(9);
	Wenu_recMassZ_12345->SetFillColor(9);
	Wenu_recMassZ_12345->SetFillStyle(3002);
	Wenu_recMassZ_12345->SetTitle("Wenu");
	Wenu_recMassZ_12345->Draw("sames");
	TLegend *LegMAccQualImpIsoEiD = ZMass_12345->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegMAccQualImpIsoEiD->Draw();
	Signal_recMassZ_12345->SetTitle("Reconstructed Z Mass");
	ZMass_name+=_RecoCutFlags[5].c_str();
	ZMass_12345->Write(ZMass_name.c_str());
		ZMass_12345->Close();}
	
	
	////////// Jet number
	
	Jet->cd();
	
	string Signal_recJetN_name = "RecoElectron/IsoJetCounter";
	Signal_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* Signal_recJetN_1 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	Signal_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* Signal_recJetN_12 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	Signal_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* Signal_recJetN_123 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	Signal_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* Signal_recJetN_1234 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	Signal_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* Signal_recJetN_12345 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	
	string QCD_recJetN_name = "RecoElectron/IsoJetCounter";
	QCD_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* QCD_recJetN_1 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	QCD_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* QCD_recJetN_12 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	QCD_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* QCD_recJetN_123 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	QCD_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* QCD_recJetN_1234 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	QCD_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* QCD_recJetN_12345 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	
	string TTbar_recJetN_name = "RecoElectron/IsoJetCounter";
	TTbar_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* TTbar_recJetN_1 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	TTbar_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* TTbar_recJetN_12 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	TTbar_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* TTbar_recJetN_123 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	TTbar_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* TTbar_recJetN_1234 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	TTbar_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* TTbar_recJetN_12345 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	
	string Wenu_recJetN_name = "RecoElectron/IsoJetCounter";
	Wenu_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* Wenu_recJetN_1 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());
	Wenu_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* Wenu_recJetN_12 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());
	Wenu_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* Wenu_recJetN_123 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());
	Wenu_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* Wenu_recJetN_1234 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());
	Wenu_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* Wenu_recJetN_12345 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());
	
	TCanvas *JetN_1 = new TCanvas;
	Signal_recJetN_1->SetLineColor(1);
	Signal_recJetN_1->Draw();
	Signal_recJetN_1->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_1->SetTitle("Signal");
	QCD_recJetN_1->SetLineColor(2);
	QCD_recJetN_1->SetFillColor(2);
	QCD_recJetN_1->SetFillStyle(3002);
	QCD_recJetN_1->SetTitle("QCD");
	QCD_recJetN_1->Draw("sames");	
	TTbar_recJetN_1->SetLineColor(8);
	TTbar_recJetN_1->SetFillColor(8);
	TTbar_recJetN_1->SetFillStyle(3002);
	TTbar_recJetN_1->SetTitle("TTbar");
	TTbar_recJetN_1->Draw("sames");
	Wenu_recJetN_1->SetLineColor(9);
	Wenu_recJetN_1->SetFillColor(9);
	Wenu_recJetN_1->SetFillStyle(3002);
	Wenu_recJetN_1->SetTitle("Wenu");
	Wenu_recJetN_1->Draw("sames");
	TLegend *LegJAcc = JetN_1->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJAcc->Draw();
	Signal_recJetN_1->SetTitle("Reconstructed #jet");
	string JetN_name = "JetN";
	JetN_name+=_RecoCutFlags[1].c_str();
	JetN_1->Write(JetN_name.c_str());
	JetN_1->Close();
	
	if(_RecoCutFlags[2] != "_1"){
	TCanvas *JetN_12 = new TCanvas;
	Signal_recJetN_12->SetLineColor(1);
	Signal_recJetN_12->Draw();
	Signal_recJetN_12->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_12->SetTitle("Signal");
	QCD_recJetN_12->SetLineColor(2);
	QCD_recJetN_12->SetFillColor(2);
	QCD_recJetN_12->SetFillStyle(3002);
	QCD_recJetN_12->SetTitle("QCD");
	QCD_recJetN_12->Draw("sames");	
	TTbar_recJetN_12->SetLineColor(8);
	TTbar_recJetN_12->SetFillColor(8);
	TTbar_recJetN_12->SetFillStyle(3002);
	TTbar_recJetN_12->SetTitle("TTbar");
	TTbar_recJetN_12->Draw("sames");
	Wenu_recJetN_12->SetLineColor(9);
	Wenu_recJetN_12->SetFillColor(9);
	Wenu_recJetN_12->SetFillStyle(3002);
	Wenu_recJetN_12->SetTitle("Wenu");
	Wenu_recJetN_12->Draw("sames");
	TLegend *LegJAcc_Qual = JetN_12->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJAcc_Qual->Draw();
	Signal_recJetN_12->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[2].c_str();
	JetN_12->Write(JetN_name.c_str());
		JetN_12->Close();}
	
	if(_RecoCutFlags[3] != "_1"){
	TCanvas *JetN_123 = new TCanvas;
	Signal_recJetN_123->SetLineColor(1);
	Signal_recJetN_123->Draw();
	Signal_recJetN_123->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_123->SetTitle("Signal");
	QCD_recJetN_123->SetLineColor(2);
	QCD_recJetN_123->SetFillColor(2);
	QCD_recJetN_123->SetFillStyle(3002);
	QCD_recJetN_123->SetTitle("QCD");
	QCD_recJetN_123->Draw("sames");	
	TTbar_recJetN_123->SetLineColor(8);
	TTbar_recJetN_123->SetFillColor(8);
	TTbar_recJetN_123->SetFillStyle(3002);
	TTbar_recJetN_123->SetTitle("TTbar");
	TTbar_recJetN_123->Draw("sames");
	Wenu_recJetN_123->SetLineColor(9);
	Wenu_recJetN_123->SetFillColor(9);
	Wenu_recJetN_123->SetFillStyle(3002);
	Wenu_recJetN_123->SetTitle("Wenu");
	Wenu_recJetN_123->Draw("sames");
	TLegend *LegJAccQualImp = JetN_123->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJAccQualImp->Draw();
	Signal_recJetN_123->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[3].c_str();
	JetN_123->Write(JetN_name.c_str());
		JetN_123->Close();}
	
	if(_RecoCutFlags[4] != "_1"){
	TCanvas *JetN_1234 = new TCanvas;
	Signal_recJetN_1234->SetLineColor(1);
	Signal_recJetN_1234->Draw();
	Signal_recJetN_1234->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_1234->SetTitle("Signal");
	QCD_recJetN_1234->SetLineColor(2);
	QCD_recJetN_1234->SetFillColor(2);
	QCD_recJetN_1234->SetFillStyle(3002);
	QCD_recJetN_1234->SetTitle("QCD");
	QCD_recJetN_1234->Draw("sames");	
	TTbar_recJetN_1234->SetLineColor(8);
	TTbar_recJetN_1234->SetFillColor(8);
	TTbar_recJetN_1234->SetFillStyle(3002);
	TTbar_recJetN_1234->SetTitle("TTbar");
	TTbar_recJetN_1234->Draw("sames");
	Wenu_recJetN_1234->SetLineColor(9);
	Wenu_recJetN_1234->SetFillColor(9);
	Wenu_recJetN_1234->SetFillStyle(3002);
	Wenu_recJetN_1234->SetTitle("Wenu");
	Wenu_recJetN_1234->Draw("sames");
	TLegend *LegJAccQualImpIso = JetN_1234->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJAccQualImpIso->Draw();
	Signal_recJetN_1234->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[4].c_str();
	JetN_1234->Write(JetN_name.c_str());
		JetN_1234->Close();}
	
	if(_RecoCutFlags[5] != "_1"){
	TCanvas *JetN_12345 = new TCanvas;
	Signal_recJetN_12345->SetLineColor(1);
	Signal_recJetN_12345->Draw();
	Signal_recJetN_12345->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_12345->SetTitle("Signal");
	QCD_recJetN_12345->SetLineColor(2);
	QCD_recJetN_12345->SetFillColor(2);
	QCD_recJetN_12345->SetFillStyle(3002);
	QCD_recJetN_12345->SetTitle("QCD");
	QCD_recJetN_12345->Draw("sames");	
	TTbar_recJetN_12345->SetLineColor(8);
	TTbar_recJetN_12345->SetFillColor(8);
	TTbar_recJetN_12345->SetFillStyle(3002);
	TTbar_recJetN_12345->SetTitle("TTbar");
	TTbar_recJetN_12345->Draw("sames");
	Wenu_recJetN_12345->SetLineColor(9);
	Wenu_recJetN_12345->SetFillColor(9);
	Wenu_recJetN_12345->SetFillStyle(3002);
	Wenu_recJetN_12345->SetTitle("Wenu");
	Wenu_recJetN_12345->Draw("sames");
	TLegend *LegJAccQualImpIsoEiD = JetN_12345->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJAccQualImpIsoEiD->Draw();
	Signal_recJetN_12345->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[5].c_str();
	JetN_12345->Write(JetN_name.c_str());
		JetN_12345->Close();}
	
	
	
  outplots->Write();
  outplots->Close();
}


