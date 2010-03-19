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


void BackPlots(){
	
	int _Acc  = 1;
	int _Trg  = 2;
	int _Qual = 0;
	int _Imp  = 2;
	int _Iso  = 3;
	int _EiD  = 4;
	
	string _RecoCutFlags[7];
	for(int i=0; i<7; i++){
		_RecoCutFlags[i] = "_1";}
	
	_RecoCutFlags[_Acc] =  "_Acc";
	_RecoCutFlags[_Trg] =  "_Trg";
	_RecoCutFlags[_Qual] = "_Qual";
	_RecoCutFlags[_Imp] =  "_Imp";
	_RecoCutFlags[_Iso] =  "_Iso";
	_RecoCutFlags[_EiD] =  "_EiD";

TFile *signal_file = TFile::Open("Zee_7TeV_RL01_10234.root");

        if(!signal_file){
	cout<<"Error! Signal files doesn't exist!"<<endl;
	return;
	}

TFile* QCD = TFile::Open("QCDall_norm100.root");

        if(!QCD){
	cout<<"Error! QCD files doesn't exist!"<<endl;
	return;
	}
	
TFile* TTbar = TFile::Open("TTbar_norm100.root");

        if(!TTbar){
	cout<<"Error! TTbar files doesn't exist!"<<endl;
	return;
	}
	
TFile* Wenu = TFile::Open("Wenu_norm100.root");

        if(!Wenu){
	cout<<"Error! Wenu files doesn't exist!"<<endl;
	return;
	}
	
TFile* outplots = new TFile("TestBackgroundPlots.root", "RECREATE");

TDirectory *Mass, *Jet;

Mass     = outplots->mkdir("ZMass");
Jet      = outplots->mkdir("JetNumber");
	
	
   /////////////   Z Mass
	
	ofstream cut;
	cut.open("Cuts_Report_test.txt");
	
	Mass->cd();
	
	string Signal_recMassZ_name = "RecoElectron/recZ_Plots/recMassZ";
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
	Signal_recMassZ_name+=_RecoCutFlags[6].c_str();
	TH1D* Signal_recMassZ_123456 = (TH1D*) signal_file->Get(Signal_recMassZ_name.c_str());
	
	if(!Signal_recMassZ_123456){
	cout<<"Error! Cut sequence wrong!"<<endl;
	return;
	}
	
	string QCD_recMassZ_name = "RecoElectron/recZ_Plots/recMassZ";
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
	QCD_recMassZ_name+=_RecoCutFlags[6].c_str();
	TH1D* QCD_recMassZ_123456 = (TH1D*) QCD->Get(QCD_recMassZ_name.c_str());
	
	string TTbar_recMassZ_name = "RecoElectron/recZ_Plots/recMassZ";
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
	TTbar_recMassZ_name+=_RecoCutFlags[6].c_str();
	TH1D* TTbar_recMassZ_123456 = (TH1D*) TTbar->Get(TTbar_recMassZ_name.c_str());
	
	string Wenu_recMassZ_name = "RecoElectron/recZ_Plots/recMassZ";
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
	Wenu_recMassZ_name+=_RecoCutFlags[6].c_str();
	TH1D* Wenu_recMassZ_123456 = (TH1D*) Wenu->Get(Wenu_recMassZ_name.c_str());
	
	
	TCanvas *ZMass_1 = new TCanvas;
	Signal_recMassZ_1->SetLineColor(1);
	Signal_recMassZ_1->SetLineWidth(3);
	Signal_recMassZ_1->Draw();
	Signal_recMassZ_1->GetXaxis()->SetTitle("rec Z Mass");
	Signal_recMassZ_1->SetTitle("Signal");
	QCD_recMassZ_1->SetLineColor(2);
	QCD_recMassZ_1->SetLineWidth(2);
	QCD_recMassZ_1->SetFillColor(2);
	QCD_recMassZ_1->SetFillStyle(3002);
	QCD_recMassZ_1->SetTitle("QCD");
	QCD_recMassZ_1->Draw("hist sames");
	TTbar_recMassZ_1->SetLineColor(4);
	TTbar_recMassZ_1->SetLineWidth(2);
	TTbar_recMassZ_1->SetFillColor(4);
	TTbar_recMassZ_1->SetFillStyle(3002);
	TTbar_recMassZ_1->SetTitle("TTbar");
	TTbar_recMassZ_1->Draw("hist sames");
	Wenu_recMassZ_1->SetLineColor(6);
	Wenu_recMassZ_1->SetLineWidth(2);
	Wenu_recMassZ_1->SetFillColor(6);
	Wenu_recMassZ_1->SetFillStyle(3001);
	Wenu_recMassZ_1->SetTitle("Wenu");
	Wenu_recMassZ_1->Draw("hist sames");
	TLegend *LegM_1 = ZMass_1->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegM_1->Draw();
	Signal_recMassZ_1->SetTitle("Reconstructed Z Mass");
	string ZMass_name = "ZMass";
	ZMass_name+=_RecoCutFlags[1].c_str();
	ZMass_1->Write(ZMass_name.c_str());
	ZMass_1->Close();
	
	double sig_err_1, qcd_err_1, tt_err_1, w_err_1;
	
	cut<<endl<<"Cut level = "<<_RecoCutFlags[1]<<endl;
	
	cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_1->IntegralAndError(0,-1,sig_err_1,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_1<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_1->IntegralAndError(0,-1,qcd_err_1,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_1<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_1->IntegralAndError(0,-1,tt_err_1,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_1<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_1->IntegralAndError(0,-1,w_err_1,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_1<<endl<<endl;

	if(_RecoCutFlags[2] != "_1"){
	TCanvas *ZMass_12 = new TCanvas;
	Signal_recMassZ_12->SetLineColor(1);
	Signal_recMassZ_12->SetLineWidth(3);
	Signal_recMassZ_12->Draw();
	Signal_recMassZ_12->GetXaxis()->SetTitle("rec Z Mass");
	Signal_recMassZ_12->SetTitle("Signal");
	QCD_recMassZ_12->SetLineColor(2);
	QCD_recMassZ_12->SetLineWidth(2);
	QCD_recMassZ_12->SetFillColor(2);
	QCD_recMassZ_12->SetFillStyle(3002);
	QCD_recMassZ_12->SetTitle("QCD");
	QCD_recMassZ_12->Draw("hist sames");
	TTbar_recMassZ_12->SetLineColor(4);
	TTbar_recMassZ_12->SetLineWidth(2);
	TTbar_recMassZ_12->SetFillColor(4);
	TTbar_recMassZ_12->SetFillStyle(3002);
	TTbar_recMassZ_12->SetTitle("TTbar");
	TTbar_recMassZ_12->Draw("hist sames");
	Wenu_recMassZ_12->SetLineColor(6);
	Wenu_recMassZ_12->SetLineWidth(2);
	Wenu_recMassZ_12->SetFillColor(6);
	Wenu_recMassZ_12->SetFillStyle(3001);
	Wenu_recMassZ_12->SetTitle("Wenu");
	Wenu_recMassZ_12->Draw("hist sames");
	TLegend *LegM_12 = ZMass_12->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegM_12->Draw();
	Signal_recMassZ_12->SetTitle("Reconstructed Z Mass");
	ZMass_name+=_RecoCutFlags[2].c_str();
	ZMass_12->Write(ZMass_name.c_str());
		ZMass_12->Close();}
	
	double sig_err_12, qcd_err_12, tt_err_12, w_err_12;
	
	cut<<endl<<"Cut level = "<<_RecoCutFlags[1]<<", "<<_RecoCutFlags[2]<<endl;
	
	cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_12->IntegralAndError(0,-1,sig_err_12,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_12<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_12->IntegralAndError(0,-1,qcd_err_12,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_12<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_12->IntegralAndError(0,-1,tt_err_12,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_12<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_12->IntegralAndError(0,-1,w_err_12,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_12<<endl<<endl;

	
	if(_RecoCutFlags[3] != "_1"){
	TCanvas *ZMass_123 = new TCanvas;
	Signal_recMassZ_123->SetLineColor(1);
	Signal_recMassZ_123->SetLineWidth(3);
	Signal_recMassZ_123->Draw();
	Signal_recMassZ_123->GetXaxis()->SetTitle("rec Z Mass");
	Signal_recMassZ_123->SetTitle("Signal");
	QCD_recMassZ_123->SetLineColor(2);
	QCD_recMassZ_123->SetLineWidth(2);
	QCD_recMassZ_123->SetFillColor(2);
	QCD_recMassZ_123->SetFillStyle(3002);
	QCD_recMassZ_123->SetTitle("QCD");
	QCD_recMassZ_123->Draw("hist sames");
	TTbar_recMassZ_123->SetLineColor(4);
	TTbar_recMassZ_123->SetLineWidth(2);
	TTbar_recMassZ_123->SetFillColor(4);
	TTbar_recMassZ_123->SetFillStyle(3002);
	TTbar_recMassZ_123->SetTitle("TTbar");
	TTbar_recMassZ_123->Draw("hist sames");
	Wenu_recMassZ_123->SetLineColor(6);
	Wenu_recMassZ_123->SetLineWidth(2);
	Wenu_recMassZ_123->SetFillColor(6);
	Wenu_recMassZ_123->SetFillStyle(3001);
	Wenu_recMassZ_123->SetTitle("Wenu");
	Wenu_recMassZ_123->Draw("hist sames");
	TLegend *LegM_123 = ZMass_123->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegM_123->Draw();
	Signal_recMassZ_123->SetTitle("Reconstructed Z Mass");
	ZMass_name+=_RecoCutFlags[3].c_str();
	ZMass_123->Write(ZMass_name.c_str());
		ZMass_123->Close();}
	
	double sig_err_123, qcd_err_123, tt_err_123, w_err_123;
	
	cut<<endl<<"Cut level = "<<_RecoCutFlags[1]<<", "<<_RecoCutFlags[2]<<", "<<_RecoCutFlags[3]<<endl;
	
	cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_123->IntegralAndError(0,-1,sig_err_123,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_123<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_123->IntegralAndError(0,-1,qcd_err_123,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_123<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_123->IntegralAndError(0,-1,tt_err_123,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_123<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_123->IntegralAndError(0,-1,w_err_123,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_123<<endl<<endl;
	
	if(_RecoCutFlags[4] != "_1"){
	TCanvas *ZMass_1234 = new TCanvas;
	Signal_recMassZ_1234->SetLineColor(1);
	Signal_recMassZ_1234->SetLineWidth(3);
	Signal_recMassZ_1234->Draw();
	Signal_recMassZ_1234->GetXaxis()->SetTitle("rec Z Mass");
	Signal_recMassZ_1234->SetTitle("Signal");
	QCD_recMassZ_1234->SetLineColor(2);
	QCD_recMassZ_1234->SetLineWidth(2);
	QCD_recMassZ_1234->SetFillColor(2);
	QCD_recMassZ_1234->SetFillStyle(3002);
	QCD_recMassZ_1234->SetTitle("QCD");
	QCD_recMassZ_1234->Draw("hist sames");
	TTbar_recMassZ_1234->SetLineColor(4);
	TTbar_recMassZ_1234->SetLineWidth(2);
	TTbar_recMassZ_1234->SetFillColor(4);
	TTbar_recMassZ_1234->SetFillStyle(3002);
	TTbar_recMassZ_1234->SetTitle("TTbar");
	TTbar_recMassZ_1234->Draw("hist sames");
	Wenu_recMassZ_1234->SetLineColor(6);
	Wenu_recMassZ_1234->SetLineWidth(2);
	Wenu_recMassZ_1234->SetFillColor(6);
	Wenu_recMassZ_1234->SetFillStyle(3001);
	Wenu_recMassZ_1234->SetTitle("Wenu");
	Wenu_recMassZ_1234->Draw("hist sames");
	TLegend *LegM_1234 = ZMass_1234->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegM_1234->Draw();
	Signal_recMassZ_1234->SetTitle("Reconstructed Z Mass");
	ZMass_name+=_RecoCutFlags[4].c_str();
	ZMass_1234->Write(ZMass_name.c_str());
	ZMass_1234->Close();}
	
	double sig_err_1234, qcd_err_1234, tt_err_1234, w_err_1234;
	
	cut<<endl<<"Cut level = "<<_RecoCutFlags[1]<<", "<<_RecoCutFlags[2]<<", "<<_RecoCutFlags[3]<<", "<<_RecoCutFlags[4]<<endl;
	
	cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_1234->IntegralAndError(0,-1,sig_err_1234,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_1234<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_1234->IntegralAndError(0,-1,qcd_err_1234,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_1234<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_1234->IntegralAndError(0,-1,tt_err_1234,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_1234<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_1234->IntegralAndError(0,-1,w_err_1234,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_1234<<endl<<endl;

	if(_RecoCutFlags[5] != "_1"){
	TCanvas *ZMass_12345 = new TCanvas;
	Signal_recMassZ_12345->SetLineColor(1);
	Signal_recMassZ_12345->SetLineWidth(3);
	Signal_recMassZ_12345->Draw();
	Signal_recMassZ_12345->GetXaxis()->SetTitle("rec Z Mass");
	Signal_recMassZ_12345->SetTitle("Signal");
	QCD_recMassZ_12345->SetLineColor(2);
	QCD_recMassZ_12345->SetLineWidth(2);
	QCD_recMassZ_12345->SetFillColor(2);
	QCD_recMassZ_12345->SetFillStyle(3002);
	QCD_recMassZ_12345->SetTitle("QCD");
	QCD_recMassZ_12345->Draw("hist sames");
	TTbar_recMassZ_12345->SetLineColor(4);
	TTbar_recMassZ_12345->SetLineWidth(2);
	TTbar_recMassZ_12345->SetFillColor(4);
	TTbar_recMassZ_12345->SetFillStyle(3002);
	TTbar_recMassZ_12345->SetTitle("TTbar");
	TTbar_recMassZ_12345->Draw("hist sames");
	Wenu_recMassZ_12345->SetLineColor(6);
	Wenu_recMassZ_12345->SetLineWidth(2);
	Wenu_recMassZ_12345->SetFillColor(6);
	Wenu_recMassZ_12345->SetFillStyle(3001);
	Wenu_recMassZ_12345->SetTitle("Wenu");
	Wenu_recMassZ_12345->Draw("hist sames");
	TLegend *LegM_12345 = ZMass_12345->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegM_12345->Draw();
	Signal_recMassZ_12345->SetTitle("Reconstructed Z Mass");
	ZMass_name+=_RecoCutFlags[5].c_str();
	ZMass_12345->Write(ZMass_name.c_str());
	ZMass_12345->Close();}
	
	double sig_err_12345, qcd_err_12345, tt_err_12345, w_err_12345;
	
	cut<<endl<<"Cut level = "<<_RecoCutFlags[1]<<", "<<_RecoCutFlags[2]<<", "<<_RecoCutFlags[3]<<", "<<_RecoCutFlags[4]<<", "<<_RecoCutFlags[5]<<endl;
	
	cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_12345->IntegralAndError(0,-1,sig_err_12345,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_12345<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_12345->IntegralAndError(0,-1,qcd_err_12345,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_12345<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_12345->IntegralAndError(0,-1,tt_err_12345,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_12345<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_12345->IntegralAndError(0,-1,w_err_12345,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_12345<<endl<<endl;
	
	if(_RecoCutFlags[6] != "_1"){
	TCanvas *ZMass_123456 = new TCanvas;
	Signal_recMassZ_123456->SetLineColor(1);
	Signal_recMassZ_123456->SetLineWidth(3);
	Signal_recMassZ_123456->Draw();
	Signal_recMassZ_123456->GetXaxis()->SetTitle("rec Z Mass");
	Signal_recMassZ_123456->SetTitle("Signal");
	QCD_recMassZ_123456->SetLineColor(2);
	QCD_recMassZ_123456->SetLineWidth(2);
	QCD_recMassZ_123456->SetFillColor(2);
	QCD_recMassZ_123456->SetFillStyle(3002);
	QCD_recMassZ_123456->SetTitle("QCD");
	QCD_recMassZ_123456->Draw("hist sames");
	TTbar_recMassZ_123456->SetLineColor(4);
	TTbar_recMassZ_123456->SetLineWidth(2);
	TTbar_recMassZ_123456->SetFillColor(4);
	TTbar_recMassZ_123456->SetFillStyle(3002);
	TTbar_recMassZ_123456->SetTitle("TTbar");
	TTbar_recMassZ_123456->Draw("hist sames");
	Wenu_recMassZ_123456->SetLineColor(6);
	Wenu_recMassZ_123456->SetLineWidth(2);
	Wenu_recMassZ_123456->SetFillColor(6);
	Wenu_recMassZ_123456->SetFillStyle(3001);
	Wenu_recMassZ_123456->SetTitle("Wenu");
	Wenu_recMassZ_123456->Draw("hist sames");
	TLegend *LegM_123456 = ZMass_123456->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegM_123456->Draw();
	Signal_recMassZ_123456->SetTitle("Reconstructed Z Mass");
	ZMass_name+=_RecoCutFlags[6].c_str();
	ZMass_123456->Write(ZMass_name.c_str());
	ZMass_123456->Close();}
	
	double sig_err_123456, qcd_err_123456, tt_err_123456, w_err_123456;
	
	cut<<endl<<"Cut level = "<<_RecoCutFlags[1]<<", "<<_RecoCutFlags[2]<<", "<<_RecoCutFlags[3]<<", "<<_RecoCutFlags[4]<<", "<<_RecoCutFlags[5]<<", "<<_RecoCutFlags[6]<<endl;
	
	cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_123456->IntegralAndError(0,-1,sig_err_123456,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_123456<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_123456->IntegralAndError(0,-1,qcd_err_123456,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_123456<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_123456->IntegralAndError(0,-1,tt_err_123456,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_123456<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_123456->IntegralAndError(0,-1,w_err_123456,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_123456<<endl<<endl;
	
	cut.close();
	

	////////// Jet number
	
	Jet->cd();
	
	string Signal_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
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
	Signal_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* Signal_recJetN_123456 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	
	string QCD_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
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
	QCD_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* QCD_recJetN_123456 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	
	string TTbar_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
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
	TTbar_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* TTbar_recJetN_123456 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	
	string Wenu_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
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
	Wenu_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* Wenu_recJetN_123456 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());

	TCanvas *JetN_1 = new TCanvas;
	Signal_recJetN_1->SetLineColor(1);
	Signal_recJetN_1->SetLineWidth(3);
	Signal_recJetN_1->Draw();
	Signal_recJetN_1->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_1->SetTitle("Signal");
	QCD_recJetN_1->SetLineColor(2);
	QCD_recJetN_1->SetLineWidth(2);
	QCD_recJetN_1->SetFillColor(2);
	QCD_recJetN_1->SetFillStyle(3002);
	QCD_recJetN_1->SetTitle("QCD");
	QCD_recJetN_1->Draw("hist sames");	
	TTbar_recJetN_1->SetLineColor(4);
	TTbar_recJetN_1->SetLineWidth(2);
	TTbar_recJetN_1->SetFillColor(4);
	TTbar_recJetN_1->SetFillStyle(3002);
	TTbar_recJetN_1->SetTitle("TTbar");
	TTbar_recJetN_1->Draw("hist sames");
	Wenu_recJetN_1->SetLineColor(6);
	Wenu_recJetN_1->SetLineWidth(2);
	Wenu_recJetN_1->SetFillColor(6);
	Wenu_recJetN_1->SetFillStyle(3001);
	Wenu_recJetN_1->SetTitle("Wenu");
	Wenu_recJetN_1->Draw("hist sames");
	TLegend *LegJ_1 = JetN_1->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_1->Draw();
	Signal_recJetN_1->SetTitle("Reconstructed #jet");
	string JetN_name = "JetN";
	JetN_name+=_RecoCutFlags[1].c_str();
	JetN_1->Write(JetN_name.c_str());
	JetN_1->Close();
	
	if(_RecoCutFlags[2] != "_1"){
	TCanvas *JetN_12 = new TCanvas;
	Signal_recJetN_12->SetLineColor(1);
	Signal_recJetN_12->SetLineWidth(3);
	Signal_recJetN_12->Draw();
	Signal_recJetN_12->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_12->SetTitle("Signal");
	QCD_recJetN_12->SetLineColor(2);
	QCD_recJetN_12->SetLineWidth(2);
	QCD_recJetN_12->SetFillColor(2);
	QCD_recJetN_12->SetFillStyle(3002);
	QCD_recJetN_12->SetTitle("QCD");
	QCD_recJetN_12->Draw("hist sames");	
	TTbar_recJetN_12->SetLineColor(4);
	TTbar_recJetN_12->SetLineWidth(2);
	TTbar_recJetN_12->SetFillColor(4);
	TTbar_recJetN_12->SetFillStyle(3002);
	TTbar_recJetN_12->SetTitle("TTbar");
	TTbar_recJetN_12->Draw("hist sames");
	Wenu_recJetN_12->SetLineColor(6);
	Wenu_recJetN_12->SetLineWidth(2);
	Wenu_recJetN_12->SetFillColor(6);
	Wenu_recJetN_12->SetFillStyle(3001);
	Wenu_recJetN_12->SetTitle("Wenu");
	Wenu_recJetN_12->Draw("hist sames");
	TLegend *LegJ_12 = JetN_12->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_12->Draw();
	Signal_recJetN_12->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[2].c_str();
	JetN_12->Write(JetN_name.c_str());
	JetN_12->Close();}
	
	if(_RecoCutFlags[3] != "_1"){
	TCanvas *JetN_123 = new TCanvas;
	Signal_recJetN_123->SetLineColor(1);
	Signal_recJetN_123->SetLineWidth(3);
	Signal_recJetN_123->Draw();
	Signal_recJetN_123->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_123->SetTitle("Signal");
	QCD_recJetN_123->SetLineColor(2);
	QCD_recJetN_123->SetLineWidth(2);
	QCD_recJetN_123->SetFillColor(2);
	QCD_recJetN_123->SetFillStyle(3002);
	QCD_recJetN_123->SetTitle("QCD");
	QCD_recJetN_123->Draw("hist sames");	
	TTbar_recJetN_123->SetLineColor(4);
	TTbar_recJetN_123->SetLineWidth(2);
	TTbar_recJetN_123->SetFillColor(4);
	TTbar_recJetN_123->SetFillStyle(3002);
	TTbar_recJetN_123->SetTitle("TTbar");
	TTbar_recJetN_123->Draw("hist sames");
	Wenu_recJetN_123->SetLineColor(6);
	Wenu_recJetN_123->SetLineWidth(2);
	Wenu_recJetN_123->SetFillColor(6);
	Wenu_recJetN_123->SetFillStyle(3001);
	Wenu_recJetN_123->SetTitle("Wenu");
	Wenu_recJetN_123->Draw("hist sames");
	TLegend *LegJ_123 = JetN_123->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_123->Draw();
	Signal_recJetN_123->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[3].c_str();
	JetN_123->Write(JetN_name.c_str());
	JetN_123->Close();}
	
	if(_RecoCutFlags[4] != "_1"){
	TCanvas *JetN_1234 = new TCanvas;
	Signal_recJetN_1234->SetLineColor(1);
	Signal_recJetN_1234->SetLineWidth(3);
	Signal_recJetN_1234->Draw();
	Signal_recJetN_1234->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_1234->SetTitle("Signal");
	QCD_recJetN_1234->SetLineColor(2);
	QCD_recJetN_1234->SetLineWidth(2);
	QCD_recJetN_1234->SetFillColor(2);
	QCD_recJetN_1234->SetFillStyle(3002);
	QCD_recJetN_1234->SetTitle("QCD");
	QCD_recJetN_1234->Draw("hist sames");	
	TTbar_recJetN_1234->SetLineColor(4);
	TTbar_recJetN_1234->SetLineWidth(2);
	TTbar_recJetN_1234->SetFillColor(4);
	TTbar_recJetN_1234->SetFillStyle(3002);
	TTbar_recJetN_1234->SetTitle("TTbar");
	TTbar_recJetN_1234->Draw("hist sames");
	Wenu_recJetN_1234->SetLineColor(6);
	Wenu_recJetN_1234->SetLineWidth(2);
	Wenu_recJetN_1234->SetFillColor(6);
	Wenu_recJetN_1234->SetFillStyle(3001);
	Wenu_recJetN_1234->SetTitle("Wenu");
	Wenu_recJetN_1234->Draw("hist sames");
	TLegend *LegJ_1234 = JetN_1234->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_1234->Draw();
	Signal_recJetN_1234->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[4].c_str();
	JetN_1234->Write(JetN_name.c_str());
	JetN_1234->Close();}
	
	if(_RecoCutFlags[5] != "_1"){
	TCanvas *JetN_12345 = new TCanvas;
	Signal_recJetN_12345->SetLineColor(1);
	Signal_recJetN_12345->SetLineWidth(3);
	Signal_recJetN_12345->Draw();
	Signal_recJetN_12345->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_12345->SetTitle("Signal");
	QCD_recJetN_12345->SetLineColor(2);
	QCD_recJetN_12345->SetLineWidth(2);
	QCD_recJetN_12345->SetFillColor(2);
	QCD_recJetN_12345->SetFillStyle(3002);
	QCD_recJetN_12345->SetTitle("QCD");
	QCD_recJetN_12345->Draw("hist sames");	
	TTbar_recJetN_12345->SetLineColor(4);
	TTbar_recJetN_12345->SetLineWidth(2);
	TTbar_recJetN_12345->SetFillColor(4);
	TTbar_recJetN_12345->SetFillStyle(3002);
	TTbar_recJetN_12345->SetTitle("TTbar");
	TTbar_recJetN_12345->Draw("hist sames");
	Wenu_recJetN_12345->SetLineColor(6);
	Wenu_recJetN_12345->SetLineWidth(2);
	Wenu_recJetN_12345->SetFillColor(6);
	Wenu_recJetN_12345->SetFillStyle(3001);
	Wenu_recJetN_12345->SetTitle("Wenu");
	Wenu_recJetN_12345->Draw("hist sames");
	TLegend *LegJ_12345 = JetN_12345->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_12345->Draw();
	Signal_recJetN_12345->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[5].c_str();
	JetN_12345->Write(JetN_name.c_str());
	JetN_12345->Close();}
	
	if(_RecoCutFlags[6] != "_1"){
	TCanvas *JetN_123456 = new TCanvas;
	Signal_recJetN_123456->SetLineColor(1);
	Signal_recJetN_123456->SetLineWidth(3);
	Signal_recJetN_123456->Draw();
	Signal_recJetN_123456->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_123456->SetTitle("Signal");
	QCD_recJetN_123456->SetLineColor(2);
	QCD_recJetN_123456->SetLineWidth(2);
	QCD_recJetN_123456->SetFillColor(2);
	QCD_recJetN_123456->SetFillStyle(3002);
	QCD_recJetN_123456->SetTitle("QCD");
	QCD_recJetN_123456->Draw("hist sames");	
	TTbar_recJetN_123456->SetLineColor(4);
	TTbar_recJetN_123456->SetLineWidth(2);
	TTbar_recJetN_123456->SetFillColor(4);
	TTbar_recJetN_123456->SetFillStyle(3002);
	TTbar_recJetN_123456->SetTitle("TTbar");
	TTbar_recJetN_123456->Draw("hist sames");
	Wenu_recJetN_123456->SetLineColor(6);
	Wenu_recJetN_123456->SetLineWidth(2);
	Wenu_recJetN_1234566->SetFillColor(6);
	Wenu_recJetN_123456->SetFillStyle(3001);
	Wenu_recJetN_123456->SetTitle("Wenu");
	Wenu_recJetN_123456->Draw("hist sames");
	TLegend *LegJ_123456 = JetN_123456->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_123456->Draw();
	Signal_recJetN_123456->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[6].c_str();
	JetN_123456->Write(JetN_name.c_str());
	JetN_123456->Close();}
	
	
	
  outplots->Write();
  outplots->Close();
}
