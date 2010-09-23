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
#include "tdrstyle.C"

using namespace std;

//Selections: "VPJ" = V+jets selections (old); "VBTF" = Vector Boson Task Force (new)

void JetRate_SignalMADPYT_Data(string selections){
        
string Analysis_Note = "False";
if (Analysis_Note == "False") gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);

        
        //Signal MC
        TFile *signal_MAD_file = TFile::Open("Z_plus_jets_Madgraph_WP80_95_VBTF.root");
        TFile *signal_PYT_file = TFile::Open("Zee_WP80_95_VBTF.root");
        
        //DATA
        TFile *data = TFile::Open("Zee_5june_all_WP80_95_VBTF.root");
        
        //Output
        string out = "test_MADPYT";        
        string output = out;
        output+=".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
        
        //Normalization factor
        double iniLumi = 50.; //pb-1
        double targetLumi = 0.01188; //pb-1
        double scale = 1.;
        if(iniLumi!=0)scale = targetLumi/iniLumi;

	//colori	
	int col_MAD = 4;
	int col_PYT = 2;

	//SetRangeUser
	double nminX = 0.0; 
	double nmaxX = 6.0;
//	double nminY = 0.001; 
//	double nmaxY = 100.0;
	
	//Legenda
	string Leg_MAD = "Signal MADGRAPH";
	string Leg_PYT = "Signal PYTHIA6";
	string Leg_Data	= "Data";

	//Selections
	int _Acc  = 1;
	int _Trg  = 2;
	int _Qual = 0;
	int _Imp  = 3;
	int _Iso  = 4;
	int _EiD  = 5;

//-----------------------------------
	
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

        if(!signal_MAD_file){
	cout<<"Error! Signal MADGRAPH file doesn't exist!"<<endl;
	return;
	}

	if(!signal_PYT_file){
	cout<<"Error! Signal PYTHIA6 file doesn't exist!"<<endl;
	return;
	}

	if(!data){
	cout<<"Error! Data file doesn't exist!"<<endl;
	return;
	}
	
TDirectory *Jet = new TDirectory();
Jet = outplots->mkdir("JetRate");
Jet->cd();


 string Signal_MAD_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
	Signal_MAD_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* Signal_MAD_recJetN_1 = (TH1D*) signal_MAD_file->Get(Signal_MAD_recJetN_name.c_str());
	Signal_MAD_recJetN_1->Scale(scale);
	Signal_MAD_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* Signal_MAD_recJetN_12 = (TH1D*) signal_MAD_file->Get(Signal_MAD_recJetN_name.c_str());
	Signal_MAD_recJetN_12->Scale(scale);
	Signal_MAD_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* Signal_MAD_recJetN_123 = (TH1D*) signal_MAD_file->Get(Signal_MAD_recJetN_name.c_str());
	Signal_MAD_recJetN_123->Scale(scale);
	Signal_MAD_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* Signal_MAD_recJetN_1234 = (TH1D*) signal_MAD_file->Get(Signal_MAD_recJetN_name.c_str());
	Signal_MAD_recJetN_1234->Scale(scale);
	Signal_MAD_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* Signal_MAD_recJetN_12345 = (TH1D*) signal_MAD_file->Get(Signal_MAD_recJetN_name.c_str());
	Signal_MAD_recJetN_12345->Scale(scale);
	Signal_MAD_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* Signal_MAD_recJetN_123456 = (TH1D*) signal_MAD_file->Get(Signal_MAD_recJetN_name.c_str());
	Signal_MAD_recJetN_123456->Scale(scale);

 string Signal_PYT_recJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
	Signal_PYT_recJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* Signal_PYT_recJetN_1 = (TH1D*) signal_PYT_file->Get(Signal_PYT_recJetN_name.c_str());
	Signal_PYT_recJetN_1->Scale(scale);
	Signal_PYT_recJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* Signal_PYT_recJetN_12 = (TH1D*) signal_PYT_file->Get(Signal_PYT_recJetN_name.c_str());
	Signal_PYT_recJetN_12->Scale(scale);
	Signal_PYT_recJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* Signal_PYT_recJetN_123 = (TH1D*) signal_PYT_file->Get(Signal_PYT_recJetN_name.c_str());
	Signal_PYT_recJetN_123->Scale(scale);
	Signal_PYT_recJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* Signal_PYT_recJetN_1234 = (TH1D*) signal_PYT_file->Get(Signal_PYT_recJetN_name.c_str());
	Signal_PYT_recJetN_1234->Scale(scale);
	Signal_PYT_recJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* Signal_PYT_recJetN_12345 = (TH1D*) signal_PYT_file->Get(Signal_PYT_recJetN_name.c_str());
	Signal_PYT_recJetN_12345->Scale(scale);
	Signal_PYT_recJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* Signal_PYT_recJetN_123456 = (TH1D*) signal_PYT_file->Get(Signal_PYT_recJetN_name.c_str());
	Signal_PYT_recJetN_123456->Scale(scale);
	
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

 string cartella;
 string cartella_exclusive_name = "Exclusive";
 string cartella_inclusive_name = "Inclusive";

 string Title;
 string Title_exclusive = "Exclusive Jet rate: ";
 string Title_inclusive = "Inclusive Jet rate: ";

for(int a=1; a<3; a++){

	if(a==1){cartella = cartella_exclusive_name;
		 Title = Title_exclusive;}
         
	if(a==2){cartella = cartella_inclusive_name;
		 Title = Title_inclusive;
         
		for(int n=1; n<10; n++){
                
                double err_Signal_MAD_recJetN_1, err_Signal_PYT_recJetN_1, err_data_recJetN_1;        
		Signal_MAD_recJetN_1->SetBinContent(n, Signal_MAD_recJetN_1->IntegralAndError(n, 10, err_Signal_MAD_recJetN_1));
		Signal_MAD_recJetN_1->SetBinError(n, err_Signal_MAD_recJetN_1);
        	Signal_PYT_recJetN_1->SetBinContent(n, Signal_PYT_recJetN_1->IntegralAndError(n, 10, err_Signal_PYT_recJetN_1));
        	Signal_PYT_recJetN_1->SetBinError(n, err_Signal_PYT_recJetN_1);
		data_recJetN_1->SetBinContent(n, data_recJetN_1->IntegralAndError(n, 10, err_data_recJetN_1));
		data_recJetN_1->SetBinError(n, err_data_recJetN_1);

		double err_Signal_MAD_recJetN_12, err_Signal_PYT_recJetN_12, err_data_recJetN_12;        
		Signal_MAD_recJetN_12->SetBinContent(n, Signal_MAD_recJetN_12->IntegralAndError(n, 10, err_Signal_MAD_recJetN_12));
		Signal_MAD_recJetN_12->SetBinError(n, err_Signal_MAD_recJetN_12);
        	Signal_PYT_recJetN_12->SetBinContent(n, Signal_PYT_recJetN_12->IntegralAndError(n, 10, err_Signal_PYT_recJetN_12));
        	Signal_PYT_recJetN_12->SetBinError(n, err_Signal_PYT_recJetN_12);
		data_recJetN_12->SetBinContent(n, data_recJetN_12->IntegralAndError(n, 10, err_data_recJetN_12));
		data_recJetN_12->SetBinError(n, err_data_recJetN_12);

		double err_Signal_MAD_recJetN_123, err_Signal_PYT_recJetN_123, err_data_recJetN_123;        
		Signal_MAD_recJetN_123->SetBinContent(n, Signal_MAD_recJetN_123->IntegralAndError(n, 10, err_Signal_MAD_recJetN_123));
		Signal_MAD_recJetN_123->SetBinError(n, err_Signal_MAD_recJetN_123);
        	Signal_PYT_recJetN_123->SetBinContent(n, Signal_PYT_recJetN_123->IntegralAndError(n, 10, err_Signal_PYT_recJetN_123));
        	Signal_PYT_recJetN_123->SetBinError(n, err_Signal_PYT_recJetN_123);
		data_recJetN_123->SetBinContent(n, data_recJetN_123->IntegralAndError(n, 10, err_data_recJetN_123));
		data_recJetN_123->SetBinError(n, err_data_recJetN_123);

		double err_Signal_MAD_recJetN_1234, err_Signal_PYT_recJetN_1234, err_data_recJetN_1234;        
		Signal_MAD_recJetN_1234->SetBinContent(n, Signal_MAD_recJetN_1234->IntegralAndError(n, 10, err_Signal_MAD_recJetN_1234));
		Signal_MAD_recJetN_1234->SetBinError(n, err_Signal_MAD_recJetN_1234);
        	Signal_PYT_recJetN_1234->SetBinContent(n, Signal_PYT_recJetN_1234->IntegralAndError(n, 10, err_Signal_PYT_recJetN_1234));
        	Signal_PYT_recJetN_1234->SetBinError(n, err_Signal_PYT_recJetN_1234);
		data_recJetN_1234->SetBinContent(n, data_recJetN_1234->IntegralAndError(n, 10, err_data_recJetN_1234));
		data_recJetN_1234->SetBinError(n, err_data_recJetN_1234);
	
		double err_Signal_MAD_recJetN_12345, err_Signal_PYT_recJetN_12345, err_data_recJetN_12345;        
		Signal_MAD_recJetN_12345->SetBinContent(n, Signal_MAD_recJetN_12345->IntegralAndError(n, 10, err_Signal_MAD_recJetN_12345));
		Signal_MAD_recJetN_12345->SetBinError(n, err_Signal_MAD_recJetN_12345);
        	Signal_PYT_recJetN_12345->SetBinContent(n, Signal_PYT_recJetN_12345->IntegralAndError(n, 10, err_Signal_PYT_recJetN_12345));
        	Signal_PYT_recJetN_12345->SetBinError(n, err_Signal_PYT_recJetN_12345);
		data_recJetN_12345->SetBinContent(n, data_recJetN_12345->IntegralAndError(n, 10, err_data_recJetN_12345));
		data_recJetN_12345->SetBinError(n, err_data_recJetN_12345);

		double err_Signal_MAD_recJetN_123456, err_Signal_PYT_recJetN_123456, err_data_recJetN_123456;        
		Signal_MAD_recJetN_123456->SetBinContent(n, Signal_MAD_recJetN_123456->IntegralAndError(n, 10, err_Signal_MAD_recJetN_123456));
		Signal_MAD_recJetN_123456->SetBinError(n, err_Signal_MAD_recJetN_123456);
        	Signal_PYT_recJetN_123456->SetBinContent(n, Signal_PYT_recJetN_123456->IntegralAndError(n, 10, err_Signal_PYT_recJetN_123456));
        	Signal_PYT_recJetN_123456->SetBinError(n, err_Signal_PYT_recJetN_123456);
		data_recJetN_123456->SetBinContent(n, data_recJetN_123456->IntegralAndError(n, 10, err_data_recJetN_123456));
		data_recJetN_123456->SetBinError(n, err_data_recJetN_123456);

		}
	}

TDirectory *Dir = new TDirectory;
Dir = Jet->mkdir(cartella.c_str());
Dir->cd();

	
	TCanvas *JetN_1 = new TCanvas();

	string JetN_name = "JetN";	
	JetN_name+=_RecoCutFlags[1].c_str();
	
	data_recJetN_1->SetLineColor(1);
	data_recJetN_1->SetLineWidth(2);
	data_recJetN_1->SetMarkerStyle(20);
	data_recJetN_1->SetTitle((Title + JetN_name).c_str());
	data_recJetN_1->GetXaxis()->SetRangeUser(nminX, nmaxX);
	//data_recJetN_1->GetYaxis()->SetRangeUser(nminY, nmaxY);
	data_recJetN_1->GetXaxis()->SetTitle("Number of jets");
	data_recJetN_1->GetYaxis()->SetTitle("Events");
	data_recJetN_1->Draw();	
	Signal_MAD_recJetN_1->SetLineColor(col_MAD);
	Signal_MAD_recJetN_1->SetLineWidth(2);
	Signal_MAD_recJetN_1->SetMarkerColor(col_MAD);
	Signal_MAD_recJetN_1->Draw("hist same");
	Signal_PYT_recJetN_1->SetLineColor(col_PYT);
	Signal_PYT_recJetN_1->SetLineWidth(2);
	Signal_PYT_recJetN_1->SetMarkerColor(col_PYT);
	Signal_PYT_recJetN_1->Draw("hist same");	
		

	TLegend *LegJ_1 = new TLegend(0.51,0.67,0.88,0.88);
	LegJ_1->SetFillColor(0);
	LegJ_1->SetBorderSize(0);
	LegJ_1->AddEntry(Signal_MAD_recJetN_1,Leg_MAD.c_str());
   	LegJ_1->AddEntry(Signal_PYT_recJetN_1,Leg_PYT.c_str());
   	LegJ_1->AddEntry(data_recJetN_1,Leg_Data.c_str());
   	LegJ_1->Draw("LP");
	
	JetN_1->Write(JetN_name.c_str());
	JetN_1->Close();
	
	
	if(_RecoCutFlags[2] != "_1"){

	TCanvas *JetN_12 = new TCanvas();

	
	JetN_name+=_RecoCutFlags[2].c_str();
	
	data_recJetN_12->SetLineColor(1);
	data_recJetN_12->SetLineWidth(2);
	data_recJetN_12->SetMarkerStyle(20);
	data_recJetN_12->SetTitle((Title + JetN_name).c_str());
	data_recJetN_12->GetXaxis()->SetRangeUser(nminX, nmaxX);
	//data_recJetN_12->GetYaxis()->SetRangeUser(nminY, nmaxY);
	data_recJetN_12->GetXaxis()->SetTitle("Number of jets");
	data_recJetN_12->GetYaxis()->SetTitle("Events");
	data_recJetN_12->Draw();	
	Signal_MAD_recJetN_12->SetLineColor(col_MAD);
	Signal_MAD_recJetN_12->SetLineWidth(2);
	Signal_MAD_recJetN_12->SetMarkerColor(col_MAD);
	Signal_MAD_recJetN_12->Draw("hist same");
	Signal_PYT_recJetN_12->SetLineColor(col_PYT);
	Signal_PYT_recJetN_12->SetLineWidth(2);
	Signal_PYT_recJetN_12->SetMarkerColor(col_PYT);
	Signal_PYT_recJetN_12->Draw("hist same");	
		

	TLegend *LegJ_12 = new TLegend(0.51,0.67,0.88,0.88);
	LegJ_12->SetFillColor(0);
	LegJ_12->SetBorderSize(0);
	LegJ_12->AddEntry(Signal_MAD_recJetN_12,Leg_MAD.c_str());
   	LegJ_12->AddEntry(Signal_PYT_recJetN_12,Leg_PYT.c_str());
   	LegJ_12->AddEntry(data_recJetN_12,Leg_Data.c_str());
   	LegJ_12->Draw("LP");
	
	JetN_12->Write(JetN_name.c_str());
	JetN_12->Close();

	}
	
	if(_RecoCutFlags[3] != "_1"){

	
	TCanvas *JetN_123 = new TCanvas();

	
	JetN_name+=_RecoCutFlags[3].c_str();
	
	data_recJetN_123->SetLineColor(1);
	data_recJetN_123->SetLineWidth(2);
	data_recJetN_123->SetMarkerStyle(20);
	data_recJetN_123->SetTitle((Title + JetN_name).c_str());
	data_recJetN_123->GetXaxis()->SetRangeUser(nminX, nmaxX);
	//data_recJetN_123->GetYaxis()->SetRangeUser(nminY, nmaxY);
	data_recJetN_123->GetXaxis()->SetTitle("Number of jets");
	data_recJetN_123->GetYaxis()->SetTitle("Events");
	data_recJetN_123->Draw();	
	Signal_MAD_recJetN_123->SetLineColor(col_MAD);
	Signal_MAD_recJetN_123->SetLineWidth(2);
	Signal_MAD_recJetN_123->SetMarkerColor(col_MAD);
	Signal_MAD_recJetN_123->Draw("hist same");
	Signal_PYT_recJetN_123->SetLineColor(col_PYT);
	Signal_PYT_recJetN_123->SetLineWidth(2);
	Signal_PYT_recJetN_123->SetMarkerColor(col_PYT);
	Signal_PYT_recJetN_123->Draw("hist same");	
		

	TLegend *LegJ_123 = new TLegend(0.51,0.67,0.88,0.88);
	LegJ_123->SetFillColor(0);
	LegJ_123->SetBorderSize(0);
	LegJ_123->AddEntry(Signal_MAD_recJetN_123,Leg_MAD.c_str());
   	LegJ_123->AddEntry(Signal_PYT_recJetN_123,Leg_PYT.c_str());
   	LegJ_123->AddEntry(data_recJetN_123,Leg_Data.c_str());
   	LegJ_123->Draw("LP");
	
	JetN_123->Write(JetN_name.c_str());
	JetN_123->Close();


	}
	
	if(_RecoCutFlags[4] != "_1"){


	TCanvas *JetN_1234 = new TCanvas();

	
	JetN_name+=_RecoCutFlags[4].c_str();
	
	data_recJetN_1234->SetLineColor(1);
	data_recJetN_1234->SetLineWidth(2);
	data_recJetN_1234->SetMarkerStyle(20);
	data_recJetN_1234->SetTitle((Title + JetN_name).c_str());
	data_recJetN_1234->GetXaxis()->SetRangeUser(nminX, nmaxX);
	//data_recJetN_1234->GetYaxis()->SetRangeUser(nminY, nmaxY);
	data_recJetN_1234->GetXaxis()->SetTitle("Number of jets");
	data_recJetN_1234->GetYaxis()->SetTitle("Events");
	data_recJetN_1234->Draw();	
	Signal_MAD_recJetN_1234->SetLineColor(col_MAD);
	Signal_MAD_recJetN_1234->SetLineWidth(2);
	Signal_MAD_recJetN_1234->SetMarkerColor(col_MAD);
	Signal_MAD_recJetN_1234->Draw("hist same");
	Signal_PYT_recJetN_1234->SetLineColor(col_PYT);
	Signal_PYT_recJetN_1234->SetLineWidth(2);
	Signal_PYT_recJetN_1234->SetMarkerColor(col_PYT);
	Signal_PYT_recJetN_1234->Draw("hist same");	
		

	TLegend *LegJ_1234 = new TLegend(0.51,0.67,0.88,0.88);
	LegJ_1234->SetFillColor(0);
	LegJ_1234->SetBorderSize(0);
	LegJ_1234->AddEntry(Signal_MAD_recJetN_1234,Leg_MAD.c_str());
   	LegJ_1234->AddEntry(Signal_PYT_recJetN_1234,Leg_PYT.c_str());
   	LegJ_1234->AddEntry(data_recJetN_1234,Leg_Data.c_str());
   	LegJ_1234->Draw("LP");
	
	JetN_1234->Write(JetN_name.c_str());
	JetN_1234->Close();


	}
	
	
	if(_RecoCutFlags[5] != "_1"){
	

	TCanvas *JetN_12345 = new TCanvas();

	
	JetN_name+=_RecoCutFlags[5].c_str();
	
	data_recJetN_12345->SetLineColor(1);
	data_recJetN_12345->SetLineWidth(2);
	data_recJetN_12345->SetMarkerStyle(20);
	data_recJetN_12345->SetTitle((Title + JetN_name).c_str());
	data_recJetN_12345->GetXaxis()->SetRangeUser(nminX, nmaxX);
	//data_recJetN_12345->GetYaxis()->SetRangeUser(nminY, nmaxY);
	data_recJetN_12345->GetXaxis()->SetTitle("Number of jets");
	data_recJetN_12345->GetYaxis()->SetTitle("Events");
	data_recJetN_12345->Draw();	
	Signal_MAD_recJetN_12345->SetLineColor(col_MAD);
	Signal_MAD_recJetN_12345->SetLineWidth(2);
	Signal_MAD_recJetN_12345->SetMarkerColor(col_MAD);
	Signal_MAD_recJetN_12345->Draw("hist same");
	Signal_PYT_recJetN_12345->SetLineColor(col_PYT);
	Signal_PYT_recJetN_12345->SetLineWidth(2);
	Signal_PYT_recJetN_12345->SetMarkerColor(col_PYT);
	Signal_PYT_recJetN_12345->Draw("hist same");	
		

	TLegend *LegJ_12345 = new TLegend(0.51,0.67,0.88,0.88);
	LegJ_12345->SetFillColor(0);
	LegJ_12345->SetBorderSize(0);
	LegJ_12345->AddEntry(Signal_MAD_recJetN_12345,Leg_MAD.c_str());
   	LegJ_12345->AddEntry(Signal_PYT_recJetN_12345,Leg_PYT.c_str());
   	LegJ_12345->AddEntry(data_recJetN_12345,Leg_Data.c_str());
   	LegJ_12345->Draw("LP");
	
	JetN_12345->Write(JetN_name.c_str());
	JetN_12345->Close();

	}
	
	if(_RecoCutFlags[6] != "_1"){


	TCanvas *JetN_123456 = new TCanvas();

	
	JetN_name+=_RecoCutFlags[6].c_str();
	
	data_recJetN_123456->SetLineColor(1);
	data_recJetN_123456->SetLineWidth(2);
	data_recJetN_123456->SetMarkerStyle(20);
	data_recJetN_123456->SetTitle((Title + JetN_name).c_str());
	data_recJetN_123456->GetXaxis()->SetRangeUser(nminX, nmaxX);
	//data_recJetN_123456->GetYaxis()->SetRangeUser(nminY, nmaxY);
	data_recJetN_123456->GetXaxis()->SetTitle("Number of jets");
	data_recJetN_123456->GetYaxis()->SetTitle("Events");
	data_recJetN_123456->Draw();	
	Signal_MAD_recJetN_123456->SetLineColor(col_MAD);
	Signal_MAD_recJetN_123456->SetLineWidth(2);
	Signal_MAD_recJetN_123456->SetMarkerColor(col_MAD);
	Signal_MAD_recJetN_123456->Draw("hist same");
	Signal_PYT_recJetN_123456->SetLineColor(col_PYT);
	Signal_PYT_recJetN_123456->SetLineWidth(2);
	Signal_PYT_recJetN_123456->SetMarkerColor(col_PYT);
	Signal_PYT_recJetN_123456->Draw("hist same");	
		

	TLegend *LegJ_123456 = new TLegend(0.51,0.67,0.88,0.88);
	LegJ_123456->SetFillColor(0);
	LegJ_123456->SetBorderSize(0);
	LegJ_123456->AddEntry(Signal_MAD_recJetN_123456,Leg_MAD.c_str());
   	LegJ_123456->AddEntry(Signal_PYT_recJetN_123456,Leg_PYT.c_str());
   	LegJ_123456->AddEntry(data_recJetN_123456,Leg_Data.c_str());
   	LegJ_123456->Draw("LP");
	
	JetN_123456->Write(JetN_name.c_str());
	JetN_123456->Close();
	
	}

Jet->cd();

	}
  
  outplots->cd();
  outplots->Write();
  outplots->Close();
}
	
