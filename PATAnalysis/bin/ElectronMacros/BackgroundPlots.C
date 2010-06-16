#include <vector>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <math.h>

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
#include "TMath.h"

using namespace std;

//Selections: "VPJ" = V+jets selections (old); "VBTF" = Vector Boson Task Force (new)

void BackPlots(string selections){

        gROOT->SetStyle("Plain");
        
        //Signal MC
        TFile *signal_file = TFile::Open("Signal_Spring10_PYTHIA6.root");
        
        //Background MC
        TFile* QCD = TFile::Open("QCD_EMEnriched_Spring10_all.root");
        TFile* QCDbce = TFile::Open("QCD_BCtoE_Spring10_all.root");
        TFile* TTbar = TFile::Open("TTbar_Spring10_PYTHIA6.root");
        TFile* Wenu = TFile::Open("Wenu_Spring10_PYTHIA6.root");
        
        //DATA
        TFile *data = TFile::Open("Zee_5june_DATA.root");
        
        //Output
        string out = "BackgroundPlots_Spring10_VBTF_11pb";
        //string out = "Test";
        string output = out;
        output+=".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
        
        //Normalization factor
        double iniLumi = 50.; //pb-1
        double targetLumi = 0.011; //pb-1
        double scale = 1.;
        if(iniLumi!=0)scale = targetLumi/iniLumi;
	
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

        if(!signal_file){
	cout<<"Error! Signal files doesn't exist!"<<endl;
	return;
	}

        if(!QCD){
	cout<<"Error! QCD files doesn't exist!"<<endl;
	return;
	}
	
        if(!TTbar){
	cout<<"Error! TTbar files doesn't exist!"<<endl;
	return;
	}
	
        if(!Wenu){
	cout<<"Error! Wenu files doesn't exist!"<<endl;
	return;
	}
	
TDirectory *Mass, *Jet;

Mass     = outplots->mkdir("ZMass");
Jet      = outplots->mkdir("JetNumber");
	
	
   /////////////   Z Mass
	
	ofstream cut;
	string outcut="CutsReport_";
	outcut+=out;
	outcut+=".txt";
	cut.open(outcut.c_str());
	
	ofstream jetm;
	string outjet="JetMultiReport_";
	outjet+=out;
	outjet+=".txt";
	jetm.open(outjet.c_str());
	
	jetm<<endl<<"Lumi = "<<targetLumi<<" pb-1"<<endl;
	
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
	
	/*cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_1->IntegralAndError(0,-1,sig_err_1,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_1<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_1->IntegralAndError(0,-1,qcd_err_1,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_1<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_1->IntegralAndError(0,-1,tt_err_1,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_1<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_1->IntegralAndError(0,-1,w_err_1,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_1<<endl<<endl;*/

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
	
	/*cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_12->IntegralAndError(0,-1,sig_err_12,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_12<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_12->IntegralAndError(0,-1,qcd_err_12,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_12<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_12->IntegralAndError(0,-1,tt_err_12,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_12<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_12->IntegralAndError(0,-1,w_err_12,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_12<<endl<<endl;*/

	
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
	
	/*cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_123->IntegralAndError(0,-1,sig_err_123,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_123<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_123->IntegralAndError(0,-1,qcd_err_123,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_123<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_123->IntegralAndError(0,-1,tt_err_123,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_123<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_123->IntegralAndError(0,-1,w_err_123,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_123<<endl<<endl;*/
	
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
	
	/*cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_1234->IntegralAndError(0,-1,sig_err_1234,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_1234<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_1234->IntegralAndError(0,-1,qcd_err_1234,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_1234<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_1234->IntegralAndError(0,-1,tt_err_1234,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_1234<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_1234->IntegralAndError(0,-1,w_err_1234,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_1234<<endl<<endl;*/

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
	
	/*cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_12345->IntegralAndError(0,-1,sig_err_12345,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_12345<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_12345->IntegralAndError(0,-1,qcd_err_12345,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_12345<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_12345->IntegralAndError(0,-1,tt_err_12345,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_12345<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_12345->IntegralAndError(0,-1,w_err_12345,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_12345<<endl<<endl;*/
	
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
	
	/*cut<<endl<<"Signal Integral   = "<<Signal_recMassZ_123456->IntegralAndError(0,-1,sig_err_123456,"")<<endl;
	cut<<"Signal Int. error = "<<sig_err_123456<<endl<<endl;
	cut<<"QCD Integral   = "<<QCD_recMassZ_123456->IntegralAndError(0,-1,qcd_err_123456,"")<<endl;
	cut<<"QCD Int. error = "<<qcd_err_123456<<endl<<endl;
	cut<<"TTbar Integral   = "<<TTbar_recMassZ_123456->IntegralAndError(0,-1,tt_err_123456,"")<<endl;
	cut<<"TTbar Int. error = "<<tt_err_123456<<endl<<endl;
	cut<<"Wenu Integral   = "<<Wenu_recMassZ_123456->IntegralAndError(0,-1,w_err_123456,"")<<endl;
	cut<<"Wenu Int. error = "<<w_err_123456<<endl<<endl;*/
	
	cut.close();
	

	////////// Jet number
	
	Jet->cd();
	
	string Signal_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
	Signal_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* Signal_recJetN_1 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	Signal_recJetN_1->Scale(scale);
	Signal_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* Signal_recJetN_12 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	Signal_recJetN_12->Scale(scale);
	Signal_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* Signal_recJetN_123 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	Signal_recJetN_123->Scale(scale);
	Signal_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* Signal_recJetN_1234 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	Signal_recJetN_1234->Scale(scale);
	Signal_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* Signal_recJetN_12345 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	Signal_recJetN_12345->Scale(scale);
	Signal_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* Signal_recJetN_123456 = (TH1D*) signal_file->Get(Signal_recJetN_name.c_str());
	Signal_recJetN_123456->Scale(scale);
	
	string QCD_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
	QCD_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* QCD_recJetN_1 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	QCD_recJetN_1->Scale(scale);
	QCD_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* QCD_recJetN_12 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	QCD_recJetN_12->Scale(scale);
	QCD_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* QCD_recJetN_123 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	QCD_recJetN_123->Scale(scale);
	QCD_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* QCD_recJetN_1234 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	QCD_recJetN_1234->Scale(scale);
	QCD_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* QCD_recJetN_12345 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	QCD_recJetN_12345->Scale(scale);
	QCD_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* QCD_recJetN_123456 = (TH1D*) QCD->Get(QCD_recJetN_name.c_str());
	QCD_recJetN_123456->Scale(scale);
	
	string QCDbce_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
	QCDbce_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* QCDbce_recJetN_1 = (TH1D*) QCDbce->Get(QCDbce_recJetN_name.c_str());
	QCDbce_recJetN_1->Scale(scale);
	QCDbce_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* QCDbce_recJetN_12 = (TH1D*) QCDbce->Get(QCDbce_recJetN_name.c_str());
	QCDbce_recJetN_12->Scale(scale);
	QCDbce_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* QCDbce_recJetN_123 = (TH1D*) QCDbce->Get(QCDbce_recJetN_name.c_str());
	QCDbce_recJetN_123->Scale(scale);
	QCDbce_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* QCDbce_recJetN_1234 = (TH1D*) QCDbce->Get(QCDbce_recJetN_name.c_str());
	QCDbce_recJetN_1234->Scale(scale);
	QCDbce_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* QCDbce_recJetN_12345 = (TH1D*) QCDbce->Get(QCDbce_recJetN_name.c_str());
	QCDbce_recJetN_12345->Scale(scale);
	QCDbce_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* QCDbce_recJetN_123456 = (TH1D*) QCDbce->Get(QCDbce_recJetN_name.c_str());
	QCDbce_recJetN_123456->Scale(scale);
	
	string TTbar_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
	TTbar_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* TTbar_recJetN_1 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	TTbar_recJetN_1->Scale(scale);
	TTbar_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* TTbar_recJetN_12 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	TTbar_recJetN_12->Scale(scale);
	TTbar_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* TTbar_recJetN_123 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	TTbar_recJetN_123->Scale(scale);
	TTbar_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* TTbar_recJetN_1234 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	TTbar_recJetN_1234->Scale(scale);
	TTbar_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* TTbar_recJetN_12345 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	TTbar_recJetN_12345->Scale(scale);
	TTbar_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* TTbar_recJetN_123456 = (TH1D*) TTbar->Get(TTbar_recJetN_name.c_str());
	TTbar_recJetN_123456->Scale(scale);
	
	string Wenu_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
	Wenu_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* Wenu_recJetN_1 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());
	Wenu_recJetN_1->Scale(scale);
	Wenu_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* Wenu_recJetN_12 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());
	Wenu_recJetN_12->Scale(scale);
	Wenu_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* Wenu_recJetN_123 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());
	Wenu_recJetN_123->Scale(scale);
	Wenu_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* Wenu_recJetN_1234 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());
	Wenu_recJetN_1234->Scale(scale);
	Wenu_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* Wenu_recJetN_12345 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());
	Wenu_recJetN_12345->Scale(scale);
	Wenu_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* Wenu_recJetN_123456 = (TH1D*) Wenu->Get(Wenu_recJetN_name.c_str());
	Wenu_recJetN_123456->Scale(scale);
	
	string data_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
	data_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* data_recJetN_1 = (TH1D*) data->Get(data_recJetN_name.c_str());
	data_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* data_recJetN_12 = (TH1D*) data->Get(data_recJetN_name.c_str());
	data_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* data_recJetN_123 = (TH1D*) data->Get(data_recJetN_name.c_str());
	data_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* data_recJetN_1234 = (TH1D*) data->Get(data_recJetN_name.c_str());
	data_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* data_recJetN_12345 = (TH1D*) data->Get(data_recJetN_name.c_str());
	data_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* data_recJetN_123456 = (TH1D*) data->Get(data_recJetN_name.c_str());

	TCanvas *JetN_1 = new TCanvas;
	Signal_recJetN_1->SetLineColor(1);
	Signal_recJetN_1->SetLineWidth(3);
	Signal_recJetN_1->SetFillColor(14);
	Signal_recJetN_1->Draw();
	Signal_recJetN_1->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_1->SetTitle("Signal");
	QCD_recJetN_1->SetLineColor(2);
	QCD_recJetN_1->SetLineWidth(2);
	QCD_recJetN_1->SetFillColor(2);
	QCD_recJetN_1->SetFillStyle(3002);
	QCD_recJetN_1->SetTitle("QCD");
	QCD_recJetN_1->Draw("hist sames");
	QCDbce_recJetN_1->SetLineColor(3);
	QCDbce_recJetN_1->SetLineWidth(2);
	QCDbce_recJetN_1->SetFillColor(3);
	QCDbce_recJetN_1->SetFillStyle(3002);
	QCDbce_recJetN_1->SetTitle("QCDbce");
	QCDbce_recJetN_1->Draw("hist sames");	
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
	data_recJetN_1->SetLineColor(1);
	data_recJetN_1->SetLineWidth(2);
	data_recJetN_1->SetMarkerStyle(20);
	data_recJetN_1->SetTitle("Data");
	data_recJetN_1->Draw("sames");
	TLegend *LegJ_1 = JetN_1->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_1->SetFillColor(0);
	LegJ_1->Draw("LP");
	Signal_recJetN_1->SetTitle("Reconstructed #jet");
	string JetN_name = "JetN";
	JetN_name+=_RecoCutFlags[1].c_str();
	JetN_1->Write(JetN_name.c_str());
	JetN_1->Close();
	
	TCanvas *StackJetNCan_1 = new TCanvas;
	THStack *StackJetN_1 = new THStack;
	Signal_recJetN_1->SetTitle("Signal");
	StackJetN_1->Add(QCD_recJetN_1);
	StackJetN_1->Add(QCDbce_recJetN_1);	
	StackJetN_1->Add(TTbar_recJetN_1);
	StackJetN_1->Add(Wenu_recJetN_1);
	StackJetN_1->Add(Signal_recJetN_1);
	StackJetN_1->SetTitle("Reconstructed #jet");
	StackJetN_1->Draw("hist");
	data_recJetN_1->Draw("sames");
	TLegend *StackLegJ_1 = StackJetNCan_1->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	StackLegJ_1->SetFillColor(0);
	StackLegJ_1->Draw("LP");
	StackJetNCan_1->Write("JetStack_1");
	StackJetNCan_1->Close();
	
	jetm<<endl<<"Selection = "<<_RecoCutFlags[1]<<endl<<endl;
	jetm<<"Jet#	0	1	2	3	4	5"<<endl<<endl;
	jetm<<"QCDEM	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCD_recJetN_1->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"QCDBCE	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCDbce_recJetN_1->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"TTbar	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(TTbar_recJetN_1->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Wenu	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Wenu_recJetN_1->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Signal	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Signal_recJetN_1->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"---------------------------------------------------------"<<endl;
	jetm<<"Total	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)((QCD_recJetN_1->GetBinContent(i)+QCDbce_recJetN_1->GetBinContent(i)+TTbar_recJetN_1->GetBinContent(i)+Wenu_recJetN_1->GetBinContent(i)+Signal_recJetN_1->GetBinContent(i))*1000)))/1000<<"	";
	jetm<<endl<<"DATA	";
	for(int i = 1; i < 7; i++)jetm<<data_recJetN_1->GetBinContent(i)<<"	";
	jetm<<endl<<endl;
	
	
	if(_RecoCutFlags[2] != "_1"){
	TCanvas *JetN_12 = new TCanvas;
	Signal_recJetN_12->SetLineColor(1);
	Signal_recJetN_12->SetLineWidth(3);
	Signal_recJetN_12->SetFillColor(14);
	Signal_recJetN_12->Draw();
	Signal_recJetN_12->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_12->SetTitle("Signal");
	QCD_recJetN_12->SetLineColor(2);
	QCD_recJetN_12->SetLineWidth(2);
	QCD_recJetN_12->SetFillColor(2);
	QCD_recJetN_12->SetFillStyle(3002);
	QCD_recJetN_12->SetTitle("QCD");
	QCD_recJetN_12->Draw("hist sames");
	QCDbce_recJetN_12->SetLineColor(3);
	QCDbce_recJetN_12->SetLineWidth(2);
	QCDbce_recJetN_12->SetFillColor(3);
	QCDbce_recJetN_12->SetFillStyle(3002);
	QCDbce_recJetN_12->SetTitle("QCDbce");
	QCDbce_recJetN_12->Draw("hist sames");	
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
	data_recJetN_12->SetLineColor(1);
	data_recJetN_12->SetLineWidth(2);
	data_recJetN_12->SetMarkerStyle(20);
	data_recJetN_12->SetTitle("Data");
	data_recJetN_12->Draw("sames");
	TLegend *LegJ_12 = JetN_12->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_12->SetFillColor(0);
	LegJ_12->Draw("LP");
	Signal_recJetN_12->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[2].c_str();
	JetN_12->Write(JetN_name.c_str());
	JetN_12->Close();
	
	TCanvas *StackJetNCan_12 = new TCanvas;
	THStack *StackJetN_12 = new THStack;
	Signal_recJetN_12->SetTitle("Signal");
	StackJetN_12->Add(QCD_recJetN_12);	
	StackJetN_12->Add(QCDbce_recJetN_12);
	StackJetN_12->Add(TTbar_recJetN_12);
	StackJetN_12->Add(Wenu_recJetN_12);
	StackJetN_12->Add(Signal_recJetN_12);
	StackJetN_12->SetTitle("Reconstructed #jet");
	StackJetN_12->Draw("hist");
	data_recJetN_12->Draw("sames");
	TLegend *StackLegJ_12 = StackJetNCan_12->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	StackLegJ_12->SetFillColor(0);
	StackLegJ_12->Draw("LP");
	StackJetNCan_12->Write("JetStack_12");
	StackJetNCan_12->Close();
	
	jetm<<endl<<"Selection = "<<_RecoCutFlags[2]<<endl<<endl;
	jetm<<"Jet#	0	1	2	3	4	5"<<endl<<endl;
	jetm<<"QCDEM	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCD_recJetN_12->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"QCDBCE	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCDbce_recJetN_12->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"TTbar	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(TTbar_recJetN_12->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Wenu	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Wenu_recJetN_12->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Signal	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Signal_recJetN_12->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"---------------------------------------------------------"<<endl;
	jetm<<"Total	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)((QCD_recJetN_12->GetBinContent(i)+QCDbce_recJetN_12->GetBinContent(i)+TTbar_recJetN_12->GetBinContent(i)+Wenu_recJetN_12->GetBinContent(i)+Signal_recJetN_12->GetBinContent(i))*1000)))/1000<<"	";
	jetm<<endl<<"DATA	";
	for(int i = 1; i < 7; i++)jetm<<data_recJetN_12->GetBinContent(i)<<"	";
	jetm<<endl<<endl;
	
	}
	
	if(_RecoCutFlags[3] != "_1"){
	TCanvas *JetN_123 = new TCanvas;
	Signal_recJetN_123->SetLineColor(1);
	Signal_recJetN_123->SetLineWidth(3);
	Signal_recJetN_123->SetFillColor(14);
	Signal_recJetN_123->Draw();
	Signal_recJetN_123->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_123->SetTitle("Signal");
	QCD_recJetN_123->SetLineColor(2);
	QCD_recJetN_123->SetLineWidth(2);
	QCD_recJetN_123->SetFillColor(2);
	QCD_recJetN_123->SetFillStyle(3002);
	QCD_recJetN_123->SetTitle("QCD");
	QCD_recJetN_123->Draw("hist sames");
	QCDbce_recJetN_123->SetLineColor(3);
	QCDbce_recJetN_123->SetLineWidth(2);
	QCDbce_recJetN_123->SetFillColor(3);
	QCDbce_recJetN_123->SetFillStyle(3002);
	QCDbce_recJetN_123->SetTitle("QCDbce");
	QCDbce_recJetN_123->Draw("hist sames");	
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
	data_recJetN_123->SetLineColor(1);
	data_recJetN_123->SetLineWidth(2);
	data_recJetN_123->SetMarkerStyle(20);
	data_recJetN_123->SetTitle("Data");
	data_recJetN_123->Draw("sames");
	TLegend *LegJ_123 = JetN_123->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_123->SetFillColor(0);
	LegJ_123->Draw("LP");
	Signal_recJetN_123->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[3].c_str();
	JetN_123->Write(JetN_name.c_str());
	JetN_123->Close();
	
	TCanvas *StackJetNCan_123 = new TCanvas;
	THStack *StackJetN_123 = new THStack;
	Signal_recJetN_123->SetTitle("Signal");
	StackJetN_123->Add(QCD_recJetN_123);
	StackJetN_123->Add(QCDbce_recJetN_123);
   	StackJetN_123->Add(TTbar_recJetN_123);
	StackJetN_123->Add(Wenu_recJetN_123);
	StackJetN_123->Add(Signal_recJetN_123);
	StackJetN_123->SetTitle("Reconstructed #jet");
	StackJetN_123->Draw("hist");
	data_recJetN_123->Draw("sames");
	TLegend *StackLegJ_123 = StackJetNCan_123->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	StackLegJ_123->SetFillColor(0);
	StackLegJ_123->Draw("LP");
	StackJetNCan_123->Write("JetStack_123");
	StackJetNCan_123->Close();
	
	jetm<<endl<<"Selection = "<<_RecoCutFlags[3]<<endl<<endl;
	jetm<<"Jet#	0	1	2	3	4	5"<<endl<<endl;
	jetm<<"QCDEM	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCD_recJetN_123->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"QCDBCE	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCDbce_recJetN_123->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"TTbar	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(TTbar_recJetN_123->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Wenu	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Wenu_recJetN_123->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Signal	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Signal_recJetN_123->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"---------------------------------------------------------"<<endl;
	jetm<<"Total	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)((QCD_recJetN_123->GetBinContent(i)+QCDbce_recJetN_123->GetBinContent(i)+TTbar_recJetN_123->GetBinContent(i)+Wenu_recJetN_123->GetBinContent(i)+Signal_recJetN_123->GetBinContent(i))*1000)))/1000<<"	";
	jetm<<endl<<"DATA	";
	for(int i = 1; i < 7; i++)jetm<<data_recJetN_123->GetBinContent(i)<<"	";
	jetm<<endl<<endl;
	
	}
	
	if(_RecoCutFlags[4] != "_1"){
	TCanvas *JetN_1234 = new TCanvas;
	Signal_recJetN_1234->SetLineColor(1);
	Signal_recJetN_1234->SetLineWidth(3);
	Signal_recJetN_1234->SetFillColor(14);
	Signal_recJetN_1234->Draw();
	Signal_recJetN_1234->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_1234->SetTitle("Signal");
	QCD_recJetN_1234->SetLineColor(2);
	QCD_recJetN_1234->SetLineWidth(2);
	QCD_recJetN_1234->SetFillColor(2);
	QCD_recJetN_1234->SetFillStyle(3002);
	QCD_recJetN_1234->SetTitle("QCD");
	QCD_recJetN_1234->Draw("hist sames");	
	QCDbce_recJetN_1234->SetLineColor(3);
	QCDbce_recJetN_1234->SetLineWidth(2);
	QCDbce_recJetN_1234->SetFillColor(3);
	QCDbce_recJetN_1234->SetFillStyle(3002);
	QCDbce_recJetN_1234->SetTitle("QCDbce");
	QCDbce_recJetN_1234->Draw("hist sames");
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
	data_recJetN_1234->SetLineColor(1);
	data_recJetN_1234->SetLineWidth(2);
	data_recJetN_1234->SetMarkerStyle(20);
	data_recJetN_1234->SetTitle("Data");
	data_recJetN_1234->Draw("sames");
	TLegend *LegJ_1234 = JetN_1234->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_1234->SetFillColor(0);
	LegJ_1234->Draw("LP");
	Signal_recJetN_1234->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[4].c_str();
	JetN_1234->Write(JetN_name.c_str());
	JetN_1234->Close();
	
	TCanvas *StackJetNCan_1234 = new TCanvas;
	THStack *StackJetN_1234 = new THStack;
	Signal_recJetN_1234->SetTitle("Signal");
	StackJetN_1234->Add(QCD_recJetN_1234);
	StackJetN_1234->Add(QCDbce_recJetN_1234);	
	StackJetN_1234->Add(TTbar_recJetN_1234);
	StackJetN_1234->Add(Wenu_recJetN_1234);
	StackJetN_1234->Add(Signal_recJetN_1234);
	StackJetN_1234->SetTitle("Reconstructed #jet");
	StackJetN_1234->Draw("hist");
	data_recJetN_1234->Draw("sames");
	TLegend *StackLegJ_1234 = StackJetNCan_1234->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	StackLegJ_1234->SetFillColor(0);
	StackLegJ_1234->Draw("LP");
	StackJetNCan_1234->Write("JetStack_1234");
	StackJetNCan_1234->Close();
	
	jetm<<endl<<"Selection = "<<_RecoCutFlags[4]<<endl<<endl;
	jetm<<"Jet#	0	1	2	3	4	5"<<endl<<endl;
	jetm<<"QCDEM	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCD_recJetN_1234->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"QCDBCE	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCDbce_recJetN_1234->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"TTbar	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(TTbar_recJetN_1234->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Wenu	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Wenu_recJetN_1234->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Signal	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Signal_recJetN_1234->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"---------------------------------------------------------"<<endl;
	jetm<<"Total	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)((QCD_recJetN_1234->GetBinContent(i)+QCDbce_recJetN_1234->GetBinContent(i)+TTbar_recJetN_1234->GetBinContent(i)+Wenu_recJetN_1234->GetBinContent(i)+Signal_recJetN_1234->GetBinContent(i))*1000)))/1000<<"	";
	jetm<<endl<<"DATA	";
	for(int i = 1; i < 7; i++)jetm<<data_recJetN_1234->GetBinContent(i)<<"	";
	jetm<<endl<<endl;
	
	}
	
	
	if(_RecoCutFlags[5] != "_1"){
	TCanvas *JetN_12345 = new TCanvas;
	Signal_recJetN_12345->SetLineColor(1);
	Signal_recJetN_12345->SetLineWidth(3);
	Signal_recJetN_12345->SetFillColor(14);
	Signal_recJetN_12345->Draw();
	Signal_recJetN_12345->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_12345->SetTitle("Signal");
	QCD_recJetN_12345->SetLineColor(2);
	QCD_recJetN_12345->SetLineWidth(2);
	QCD_recJetN_12345->SetFillColor(2);
	QCD_recJetN_12345->SetFillStyle(3002);
	QCD_recJetN_12345->SetTitle("QCD");
	QCD_recJetN_12345->Draw("hist sames");
	QCDbce_recJetN_12345->SetLineColor(3);
	QCDbce_recJetN_12345->SetLineWidth(2);
	QCDbce_recJetN_12345->SetFillColor(3);
	QCDbce_recJetN_12345->SetFillStyle(3002);
	QCDbce_recJetN_12345->SetTitle("QCDbce");
	QCDbce_recJetN_12345->Draw("hist sames");
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
	data_recJetN_12345->SetLineColor(1);
	data_recJetN_12345->SetLineWidth(2);
	data_recJetN_12345->SetMarkerStyle(20);
	data_recJetN_12345->SetTitle("Data");
	data_recJetN_12345->Draw("sames");
	TLegend *LegJ_12345 = JetN_12345->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_12345->SetFillColor(0);
	LegJ_12345->Draw("LP");
	Signal_recJetN_12345->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[5].c_str();
	JetN_12345->Write(JetN_name.c_str());
	JetN_12345->Close();
	
	TCanvas *StackJetNCan_12345 = new TCanvas;
	THStack *StackJetN_12345 = new THStack;
	Signal_recJetN_12345->SetTitle("Signal");
	StackJetN_12345->Add(QCD_recJetN_12345);
	StackJetN_12345->Add(QCDbce_recJetN_12345);	
	StackJetN_12345->Add(TTbar_recJetN_12345);
	StackJetN_12345->Add(Wenu_recJetN_12345);
	StackJetN_12345->Add(Signal_recJetN_12345);
	StackJetN_12345->SetTitle("Reconstructed #jet");
	StackJetN_12345->Draw("hist");
	data_recJetN_12345->Draw("sames");
	TLegend *StackLegJ_12345 = StackJetNCan_12345->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	StackLegJ_12345->SetFillColor(0);
	StackLegJ_12345->Draw("LP");
	StackJetNCan_12345->Write("JetStack_12345");
	StackJetNCan_12345->Close();
	
	jetm<<endl<<"Selection = "<<_RecoCutFlags[5]<<endl<<endl;
	jetm<<"Jet#	0	1	2	3	4	5"<<endl<<endl;
	jetm<<"QCDEM	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCD_recJetN_12345->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"QCDBCE	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCDbce_recJetN_12345->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"TTbar	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(TTbar_recJetN_12345->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Wenu	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Wenu_recJetN_12345->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Signal	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Signal_recJetN_12345->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"---------------------------------------------------------"<<endl;
	jetm<<"Total	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)((QCD_recJetN_12345->GetBinContent(i)+QCDbce_recJetN_12345->GetBinContent(i)+TTbar_recJetN_12345->GetBinContent(i)+Wenu_recJetN_12345->GetBinContent(i)+Signal_recJetN_12345->GetBinContent(i))*1000)))/1000<<"	";
	jetm<<endl<<"DATA	";
	for(int i = 1; i < 7; i++)jetm<<data_recJetN_12345->GetBinContent(i)<<"	";
	jetm<<endl<<endl;
	
	}
	
	if(_RecoCutFlags[6] != "_1"){
	TCanvas *JetN_123456 = new TCanvas;
	Signal_recJetN_123456->SetLineColor(1);
	Signal_recJetN_123456->SetLineWidth(3);
	Signal_recJetN_123456->SetFillColor(14);
	Signal_recJetN_123456->Draw();
	Signal_recJetN_123456->GetXaxis()->SetTitle("rec #jet");
	Signal_recJetN_123456->SetTitle("Signal");
	QCD_recJetN_123456->SetLineColor(2);
	QCD_recJetN_123456->SetLineWidth(2);
	QCD_recJetN_123456->SetFillColor(2);
	QCD_recJetN_123456->SetFillStyle(3002);
	QCD_recJetN_123456->SetTitle("QCD");
	QCD_recJetN_123456->Draw("hist sames");
	QCDbce_recJetN_123456->SetLineColor(3);
	QCDbce_recJetN_123456->SetLineWidth(2);
	QCDbce_recJetN_123456->SetFillColor(3);
	QCDbce_recJetN_123456->SetFillStyle(3002);
	QCDbce_recJetN_123456->SetTitle("QCDbce");
	QCDbce_recJetN_123456->Draw("hist sames");	
	TTbar_recJetN_123456->SetLineColor(4);
	TTbar_recJetN_123456->SetLineWidth(2);
	TTbar_recJetN_123456->SetFillColor(4);
	TTbar_recJetN_123456->SetFillStyle(3002);
	TTbar_recJetN_123456->SetTitle("TTbar");
	TTbar_recJetN_123456->Draw("hist sames");
	Wenu_recJetN_123456->SetLineColor(6);
	Wenu_recJetN_123456->SetLineWidth(2);
	Wenu_recJetN_123456->SetFillColor(6);
	Wenu_recJetN_123456->SetFillStyle(3001);
	Wenu_recJetN_123456->SetTitle("Wenu");
	Wenu_recJetN_123456->Draw("hist sames");
	data_recJetN_123456->SetLineColor(1);
	data_recJetN_123456->SetLineWidth(2);
	data_recJetN_123456->SetMarkerStyle(20);
	data_recJetN_123456->SetTitle("Data");
	data_recJetN_123456->Draw("sames");
	TLegend *LegJ_123456 = JetN_123456->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegJ_123456->SetFillColor(0);
	LegJ_123456->Draw("LP");
	Signal_recJetN_123456->SetTitle("Reconstructed #jet");
	JetN_name+=_RecoCutFlags[6].c_str();
	JetN_123456->Write(JetN_name.c_str());
	JetN_123456->Close();
	
        TCanvas *StackJetNCan_123456 = new TCanvas;	
	THStack *StackJetN_123456 = new THStack;
	Signal_recJetN_123456->SetTitle("Signal");
	StackJetN_123456->Add(QCD_recJetN_123456);
	StackJetN_123456->Add(QCDbce_recJetN_123456);	
	StackJetN_123456->Add(TTbar_recJetN_123456);
	StackJetN_123456->Add(Wenu_recJetN_123456);
	StackJetN_123456->Add(Signal_recJetN_123456);
	StackJetN_123456->SetTitle("Reconstructed #jet");
	StackJetN_123456->Draw("hist");
	data_recJetN_123456->Draw("sames");
	TLegend *StackLegJ_123456 = StackJetNCan_123456->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	StackLegJ_123456->SetFillColor(0);
	StackLegJ_123456->Draw("LP");
	StackJetNCan_123456->Write("JetStack_123456");
	StackJetNCan_123456->Close();
	
	jetm<<endl<<"Selection = "<<_RecoCutFlags[6]<<endl<<endl;
	jetm<<"Jet#	0	1	2	3	4	5"<<endl<<endl;
	jetm<<"QCDEM	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCD_recJetN_123456->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"QCDBCE	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(QCDbce_recJetN_123456->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"TTbar	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(TTbar_recJetN_123456->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Wenu	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Wenu_recJetN_123456->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"Signal	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)(Signal_recJetN_123456->GetBinContent(i)*1000)))/1000<<"	";
	jetm<<endl<<"---------------------------------------------------------"<<endl;
	jetm<<"Total	";
	for(int i = 1; i < 7; i++)jetm<<((float)((int)((QCD_recJetN_123456->GetBinContent(i)+QCDbce_recJetN_123456->GetBinContent(i)+TTbar_recJetN_123456->GetBinContent(i)+Wenu_recJetN_123456->GetBinContent(i)+Signal_recJetN_123456->GetBinContent(i))*1000)))/1000<<"	";
	jetm<<endl<<"DATA	";
	for(int i = 1; i < 7; i++)jetm<<data_recJetN_123456->GetBinContent(i)<<"	";
	jetm<<endl<<endl;
	
	}
	
	jetm.close();
	
  outplots->Write();
  outplots->Close();
}
