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

//Selections: "SYM" = Symmetric selections; "ASYM" = Asymmetric

void recZ_Plots(string selections){

string Analysis_Note = "False";
if (Analysis_Note == "False") gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);

string log_scale = "True";

string Tab_cut = "True";
        
                
	//Background MC
	TFile* QCD_TF = TFile::Open("MC_Fall10_387/JetPt30/QCD_ALL.root");
	TFile* EWK_TF = TFile::Open("MC_Fall10_387/JetPt30/EWK_ALL.root");
	TFile* TTbar_TF = TFile::Open("MC_Fall10_387/JetPt30/TT_Pythia.root");
	
	//Signal MC
	TFile *Z_TF = TFile::Open("MC_Winter10_399/Z_Madgraph_L1FastJet_JetPt30_EffCorr.root");
	
	//Data
	TFile *Data_TF = TFile::Open("DATA_Dec22ReReco/JetPt30/Data_RUN2010A-B_L1FastJet_399.root");
        
        //Output
        string out = "recZPlots_L1FastJet_JetPt30_BackGrouped";        
        string output = out;
        output+=".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
	      
        //Normalization factor
        double iniLumi = 50.; //pb-1
        double targetLumi = 36.176; //pb-1
        double scale = 1.;
        if(iniLumi!=0)scale = targetLumi/iniLumi;

	//rebin
	int rebin_recMassZ = 5;
	int rebin_recPtZ = 2;
	int rebin_recEtaZ = 5;
		
	//colors	
	int col_Z = kOrange;
	int col_QCD = kMagenta+1;
	int col_EWK = kOrange+8;
	int col_TTbar = kCyan+1;
	int col_Data = 1;

	//FillStyle	
	int style = 1001;

	//SetRangeUser
	//MassZ
	double nminX_recMassZ= 50.0; 
	double nmaxX_recMassZ = 150.0;
	double nminY_recMassZ = 0.1; 
	double nmaxY_recMassZ = 3000.0;
	//PtZ
	double nminX_recPtZ = 0.0; 
	double nmaxX_recPtZ = 200.0;
	double nminY_recPtZ = 0.1; 
	double nmaxY_recPtZ = 1500.0;
	//EtaZ
	double nminX_recEtaZ = -10.0; 
	double nmaxX_recEtaZ = 10.0;
	double nminY_recEtaZ = 0.1; 
	double nmaxY_recEtaZ = 1500.0;
	
	//Legenda
	string Leg_QCD = "QCD";
	string Leg_EWK = "EWK";
	string Leg_TTbar = "t #bar{t}+jets";
	string Leg_Z = "Z #rightarrow ee";
	string Leg_Data	= "Data";

	//tabella cut
	int cut_decimal = 10000;
	string Tabcut_title = "Cut level = ";
	string Tabcut_ZMass = "Z Mass Mean = ";
	string Tabcut_ZMass_err = "Z Mass Mean error = ";
	string Tabcut_ZMass_um = " GeV/c²";
	string Tabcut_ZPt = "Z Pt Mean = ";
	string Tabcut_ZPt_err = "Z Pt Mean error = ";
	string Tabcut_ZPt_um = " GeV/c";
	string Tabcut_ZEta = "Z Eta Mean = ";
	string Tabcut_ZEta_err = "Z Eta Mean error = ";
	string Tabcut_ZEta_um = " ";
	string Tabcut_QCD_Int = "QCD Integral = ";
	string Tabcut_QCD_Int_err = "QCD Integral error = ";
	string Tabcut_TTbar_Int = "TTbar+jets Integral = ";
	string Tabcut_TTbar_Int_err = "TTbar+jets Integral error = ";
	string Tabcut_EWK_Int = "EWK Integral = ";
	string Tabcut_EWK_Int_err = "EWK Integral error = ";
	string Tabcut_Z_Int = "Z+jets Integral = ";
	string Tabcut_Z_Int_err = "Z+jets Integral error = ";
	string Tabcut_riga = "-----------------------------------------------------------------------------";
	string Tabcut_Total_Int = "Total Integral = ";
	string Tabcut_Total_Int_err = "Total Integral error = ";
	string Tabcut_Data_Int = "Data Integral = ";
	string Tabcut_Data_Int_err = "Data Integral error = ";
	string Tabcut_end = "#############################################################################";
	
	
	//Selections
	int _Acc  = 1;
	int _Trg  = 2;
	int _Imp  = 3;
	int _Conv = 4;
	int _Iso  = 5;
	int _EiD  = 6;
	
//------------------------------------------
	
	string _RecoCutFlags[7];
	
	for(int i=0; i<7; i++){
       _RecoCutFlags[i] = "_1";}
   
       if(selections=="SYM"){
       _RecoCutFlags[_Acc] =  "_AccSYM";
       _RecoCutFlags[_Iso] =  "_IsoSYM";
       _RecoCutFlags[_EiD] =  "_EiDSYM";}
       if(selections=="ASYM"){
       _RecoCutFlags[_Acc] =  "_AccASYM";
       _RecoCutFlags[_Iso] =  "_IsoASYM";
       _RecoCutFlags[_EiD] =  "_EiDASYM";}
     
       _RecoCutFlags[_Trg] =  "_Trg";
       _RecoCutFlags[_Conv] = "_ConvASYM";
       _RecoCutFlags[_Imp] =  "_Imp";
	        
	if(!QCD_TF){
	cout<<"Error! QCD file doesn't exist!"<<endl;
	return;
	}

	if(!TTbar_TF){
	cout<<"Error! TTbar file doesn't exist!"<<endl;
	return;
	}
	
	if(!EWK_TF){
	cout<<"Error! EWK file doesn't exist!"<<endl;
	return;
	}
	
	if(!Z_TF){
	cout<<"Error! Signal file doesn't exist!"<<endl;
	return;
	}	
	
	if(!Data_TF){
	cout<<"Error! Data file doesn't exist!"<<endl;
	return;
	}


	ofstream cut;
	string outcut = out;
	outcut+="_CutsReport";
	outcut+=".txt";
	cut.open(outcut.c_str());
	
	cut<<endl<<"Lumi = "<<targetLumi<<" pb-1"<<endl<<endl<<Tabcut_end.c_str()<<endl;
		
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//cartelle

string cartella1 = "RecoElectron";
string cartella2a = "recZ_Plots";	

string cartellaplot;
	
	//grafici
int rebin = 0;
double nminX = 0., nmaxX = 0., nminY = 0. , nmaxY = 0.;

string grafico_name;
string grafico_name_Cartella2a_recMassZ = "recMassZ";
string grafico_name_Cartella2a_recPtZ = "recPtZ";
string grafico_name_Cartella2a_recEtaZ = "recEtaZ";

string asseX_name;
string asseY_name;

string asseX_name_Cartella2a_recMassZ ="Reconstructed Z Mass (GeV/c^{2})";
string asseY_name_Cartella2a_recMassZ = Form("Events/(%.1f GeV/c^{2})", 0.5*rebin_recMassZ); //bins da 0.5 Gev/c2
string asseX_name_Cartella2a_recPtZ = "Reconstructed Z P_{T} (GeV/c)"; 
string asseY_name_Cartella2a_recPtZ = Form("Events/(%.0f GeV/c)", 1.0*rebin_recPtZ); //bins da 1 GeV/c
string asseX_name_Cartella2a_recEtaZ = "Reconstructed Z #eta";
string asseY_name_Cartella2a_recEtaZ = Form("Events/(%.1f)",0.2*rebin_recEtaZ); //bins da 0.2

//tabella cut
string Tabcut_Mean, Tabcut_Mean_err, Tabcut_Mean_um;

TDirectory *Dir_1;
Dir_1 = outplots->mkdir(cartella1.c_str());
Dir_1->cd();


//--------------------------------------- directory recZ_Plots

TDirectory *Dir_2a;
Dir_2a = Dir_1->mkdir(cartella2a.c_str());
Dir_2a->cd();
	
TDirectoryFile *Cartella1_QCD = (TDirectoryFile*) QCD_TF->Get(cartella1.c_str());
TDirectoryFile *Cartella1_QCD_Cartella2a = (TDirectoryFile*) Cartella1_QCD->Get(cartella2a.c_str());

TDirectoryFile *Cartella1_TTbar = (TDirectoryFile*) TTbar_TF->Get(cartella1.c_str());
TDirectoryFile *Cartella1_TTbar_Cartella2a = (TDirectoryFile*) Cartella1_TTbar->Get(cartella2a.c_str());
	
TDirectoryFile *Cartella1_EWK = (TDirectoryFile*) EWK_TF->Get(cartella1.c_str());
TDirectoryFile *Cartella1_EWK_Cartella2a = (TDirectoryFile*) Cartella1_EWK->Get(cartella2a.c_str());

TDirectoryFile *Cartella1_Z = (TDirectoryFile*) Z_TF->Get(cartella1.c_str());
TDirectoryFile *Cartella1_Z_Cartella2a = (TDirectoryFile*) Cartella1_Z->Get(cartella2a.c_str());

TDirectoryFile *Cartella1_Data = (TDirectoryFile*) Data_TF->Get(cartella1.c_str());
TDirectoryFile *Cartella1_Data_Cartella2a = (TDirectoryFile*) Cartella1_Data->Get(cartella2a.c_str());

//----------------------------------------- Grafici cartella recZ_Plots

for(int b=1; b<4; b++){
if(b==1){cartellaplot = grafico_name_Cartella2a_recMassZ;
	 grafico_name = grafico_name_Cartella2a_recMassZ;
	 asseX_name = asseX_name_Cartella2a_recMassZ; 
	 asseY_name = asseY_name_Cartella2a_recMassZ;
	 rebin = rebin_recMassZ;
	 nminX = nminX_recMassZ;
	 nmaxX = nmaxX_recMassZ;
	 nminY = nminY_recMassZ;
	 nmaxY = nmaxY_recMassZ;
	 Tabcut_Mean = Tabcut_ZMass;
	 Tabcut_Mean_err = Tabcut_ZMass_err; 
	 Tabcut_Mean_um = Tabcut_ZMass_um;}

else if(b==2){cartellaplot = grafico_name_Cartella2a_recPtZ;
	      grafico_name = grafico_name_Cartella2a_recPtZ;
	      asseX_name = asseX_name_Cartella2a_recPtZ; 
	      asseY_name = asseY_name_Cartella2a_recPtZ;
	      rebin = rebin_recPtZ;
	      nminX = nminX_recPtZ;
	      nmaxX = nmaxX_recPtZ;
	      nminY = nminY_recPtZ;
	      nmaxY = nmaxY_recPtZ;
	      Tabcut_Mean = Tabcut_ZPt;
	      Tabcut_Mean_err = Tabcut_ZPt_err; 
	      Tabcut_Mean_um = Tabcut_ZPt_um;}

else if(b==3){cartellaplot = grafico_name_Cartella2a_recEtaZ;
	      grafico_name = grafico_name_Cartella2a_recEtaZ;
	      asseX_name = asseX_name_Cartella2a_recEtaZ; 
	      asseY_name = asseY_name_Cartella2a_recEtaZ;
	      rebin = rebin_recEtaZ;
	      nminX = nminX_recEtaZ;
	      nmaxX = nmaxX_recEtaZ;
	      nminY = nminY_recEtaZ;
	      nmaxY = nmaxY_recEtaZ;
	      Tabcut_Mean = Tabcut_ZEta;
	      Tabcut_Mean_err = Tabcut_ZEta_err; 
	      Tabcut_Mean_um = Tabcut_ZEta_um;}


TDirectory *Dir_3;
Dir_3 = Dir_2a->mkdir(cartellaplot.c_str());
Dir_3->cd();

	string QCD_name = grafico_name.c_str();
	QCD_name+=_RecoCutFlags[1].c_str();
	TH1D* histoQCD_1 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_1->Scale(scale);
	histoQCD_1->Rebin(rebin);
	QCD_name+=_RecoCutFlags[2].c_str();
	TH1D* histoQCD_12 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_12->Scale(scale);
	histoQCD_12->Rebin(rebin);
	QCD_name+=_RecoCutFlags[3].c_str();
	TH1D* histoQCD_123 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_123->Scale(scale);
	histoQCD_123->Rebin(rebin);
	QCD_name+=_RecoCutFlags[4].c_str();
	TH1D* histoQCD_1234 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_1234->Scale(scale);
	histoQCD_1234->Rebin(rebin);
	QCD_name+=_RecoCutFlags[5].c_str();
	TH1D* histoQCD_12345 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_12345->Scale(scale);
	histoQCD_12345->Rebin(rebin);
	QCD_name+=_RecoCutFlags[6].c_str();
	TH1D* histoQCD_123456 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_123456->Scale(scale);
	histoQCD_123456->Rebin(rebin);
	
	string TTbar_name = grafico_name.c_str();
	TTbar_name+=_RecoCutFlags[1].c_str();
	TH1D* histoTTbar_1 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_1->Scale(scale);
	histoTTbar_1->Rebin(rebin);
	TTbar_name+=_RecoCutFlags[2].c_str();
	TH1D* histoTTbar_12 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_12->Scale(scale);
	histoTTbar_12->Rebin(rebin);
	TTbar_name+=_RecoCutFlags[3].c_str();
	TH1D* histoTTbar_123 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_123->Scale(scale);
	histoTTbar_123->Rebin(rebin);
	TTbar_name+=_RecoCutFlags[4].c_str();
	TH1D* histoTTbar_1234 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_1234->Scale(scale);
	histoTTbar_1234->Rebin(rebin);
	TTbar_name+=_RecoCutFlags[5].c_str();
	TH1D* histoTTbar_12345 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_12345->Scale(scale);
	histoTTbar_12345->Rebin(rebin);
	TTbar_name+=_RecoCutFlags[6].c_str();
	TH1D* histoTTbar_123456 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_123456->Scale(scale);
	histoTTbar_123456->Rebin(rebin);

	string EWK_name = grafico_name.c_str();
	EWK_name+=_RecoCutFlags[1].c_str();
	TH1D* histoEWK_1 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
	histoEWK_1->Scale(scale);
	histoEWK_1->Rebin(rebin);
	EWK_name+=_RecoCutFlags[2].c_str();
	TH1D* histoEWK_12 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
	histoEWK_12->Scale(scale);
	histoEWK_12->Rebin(rebin);
	EWK_name+=_RecoCutFlags[3].c_str();
	TH1D* histoEWK_123 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
	histoEWK_123->Scale(scale);
	histoEWK_123->Rebin(rebin);
	EWK_name+=_RecoCutFlags[4].c_str();
	TH1D* histoEWK_1234 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
	histoEWK_1234->Scale(scale);
	histoEWK_1234->Rebin(rebin);
	EWK_name+=_RecoCutFlags[5].c_str();
	TH1D* histoEWK_12345 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
	histoEWK_12345->Scale(scale);
	histoEWK_12345->Rebin(rebin);
	EWK_name+=_RecoCutFlags[6].c_str();
	TH1D* histoEWK_123456 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
	histoEWK_123456->Scale(scale);
	histoEWK_123456->Rebin(rebin);
	
 	string Z_name = grafico_name.c_str();
	Z_name+=_RecoCutFlags[1].c_str();
	TH1D* histoZ_1 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_1->Scale(scale);
	histoZ_1->Rebin(rebin);	
	Z_name+=_RecoCutFlags[2].c_str();
	TH1D* histoZ_12 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_12->Scale(scale);
	histoZ_12->Rebin(rebin);
	Z_name+=_RecoCutFlags[3].c_str();
	TH1D* histoZ_123 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_123->Scale(scale);
	histoZ_123->Rebin(rebin);
	Z_name+=_RecoCutFlags[4].c_str();
	TH1D* histoZ_1234 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_1234->Scale(scale);
	histoZ_1234->Rebin(rebin);
	Z_name+=_RecoCutFlags[5].c_str();
	TH1D* histoZ_12345 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_12345->Scale(scale);
	histoZ_12345->Rebin(rebin);
	Z_name+=_RecoCutFlags[6].c_str();
	TH1D* histoZ_123456 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_123456->Scale(scale);
	histoZ_123456->Rebin(rebin);

	if(!histoZ_123456){
	cout<<"Error! Cut sequence wrong!"<<endl;
	return;
	}


	string Data_name = grafico_name.c_str();
	Data_name+=_RecoCutFlags[1].c_str();
	TH1D* histoData_1 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_1->Rebin(rebin);	
	Data_name+=_RecoCutFlags[2].c_str();
	TH1D* histoData_12 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_12->Rebin(rebin);
	Data_name+=_RecoCutFlags[3].c_str();
	TH1D* histoData_123 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_123->Rebin(rebin);
	Data_name+=_RecoCutFlags[4].c_str();
	TH1D* histoData_1234 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_1234->Rebin(rebin);	
	Data_name+=_RecoCutFlags[5].c_str();
	TH1D* histoData_12345 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_12345->Rebin(rebin);
	Data_name+=_RecoCutFlags[6].c_str();
	TH1D* histoData_123456 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_123456->Rebin(rebin);

TDirectory *Dir_4a;
Dir_4a = Dir_3->mkdir("Separated_Plots");
Dir_4a->cd();
		
	TCanvas *c_1 = new TCanvas();
	
	if (log_scale == "True") {c_1->SetLogy();}

	string c_name = grafico_name.c_str();	
	c_name+=_RecoCutFlags[1].c_str();
	
	histoQCD_1->SetLineColor(col_QCD);
	histoQCD_1->SetLineWidth(2);
	histoQCD_1->SetFillColor(col_QCD);
	histoQCD_1->SetFillStyle(style);
	histoQCD_1->SetMarkerColor(col_QCD);
	histoQCD_1->SetTitle(c_name.c_str());
	histoQCD_1->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histoQCD_1->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histoQCD_1->GetXaxis()->SetTitle(asseX_name.c_str());
	histoQCD_1->GetYaxis()->SetTitle(asseY_name.c_str());
	histoQCD_1->Draw("hist");
	histoTTbar_1->SetLineColor(col_TTbar);
	histoTTbar_1->SetLineWidth(2);
	histoTTbar_1->SetFillColor(col_TTbar);
	histoTTbar_1->SetFillStyle(style);
	histoTTbar_1->SetMarkerColor(col_TTbar);
	histoTTbar_1->Draw("hist same");
	histoEWK_1->SetLineColor(col_EWK);
	histoEWK_1->SetLineWidth(2);
	histoEWK_1->SetFillColor(col_EWK);
	histoEWK_1->SetFillStyle(style);
	histoEWK_1->SetMarkerColor(col_EWK);
	histoEWK_1->Draw("hist same");	
	histoZ_1->SetLineColor(col_Z);
	histoZ_1->SetLineWidth(2);
	histoZ_1->SetFillColor(col_Z);
	histoZ_1->SetFillStyle(style);
	histoZ_1->SetMarkerColor(col_Z);
	histoZ_1->Draw("hist same");
	histoData_1->SetLineColor(col_Data);
	histoData_1->SetLineWidth(2);
	histoData_1->SetMarkerStyle(20);
	histoData_1->Draw("same");			

	TLegend *Leg_1 = new TLegend(0.51,0.67,0.88,0.88);
	Leg_1->SetFillColor(0);
	Leg_1->SetBorderSize(0);
	Leg_1->AddEntry(histoQCD_1,Leg_QCD.c_str());
	Leg_1->AddEntry(histoTTbar_1,Leg_TTbar.c_str());
	Leg_1->AddEntry(histoEWK_1,Leg_EWK.c_str());
	Leg_1->AddEntry(histoZ_1,Leg_Z.c_str());
	Leg_1->AddEntry(histoData_1,Leg_Data.c_str());	
	   	
	Leg_1->Draw("LP");
	
	c_1->Write(c_name.c_str());
	c_1->Close();
	
	if(Tab_cut == "True"){
	double err_QCD_1, err_TTbar_1, err_EWK_1, err_Z_1, err_Data_1;
	
	cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
	cut<<endl<< Tabcut_Mean.c_str() <<((float)((int)(histoData_1->GetMean(1)*cut_decimal)))/cut_decimal<< Tabcut_Mean_um.c_str()<<endl;
	cut<< Tabcut_Mean_err.c_str() <<((float)((int)(histoData_1->GetMeanError()*cut_decimal)))/cut_decimal<<Tabcut_Mean_um.c_str()<<endl;
	cut<<endl<<Tabcut_riga.c_str()<<endl;
	cut<<endl<<Tabcut_QCD_Int.c_str()<<((float)((long)(histoQCD_1->IntegralAndError(0,-1,err_QCD_1,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_Int_err.c_str()<<((float)((int)(err_QCD_1*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_1->IntegralAndError(0,-1,err_TTbar_1,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_1*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_EWK_Int.c_str()<<((float)((int)(histoEWK_1->IntegralAndError(0,-1,err_EWK_1,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_EWK_Int_err.c_str()<<((float)((int)(err_EWK_1*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_1->IntegralAndError(0,-1,err_Z_1,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_1*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
	cut<<Tabcut_Total_Int.c_str()<<((float)((long)((histoQCD_1->IntegralAndError(0,-1,err_QCD_1,"") + histoTTbar_1->IntegralAndError(0,-1,err_TTbar_1,"") + histoEWK_1->IntegralAndError(0,-1,err_EWK_1,"") + histoZ_1->IntegralAndError(0,-1,err_Z_1,""))*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_1),2) + pow((err_TTbar_1),2) + pow((err_EWK_1),2) + pow((err_Z_1),2))*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Data_Int.c_str()<<((float)((long)(histoData_1->IntegralAndError(0,-1,err_Data_1,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_1*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_end.c_str()<<endl;
	}

	if(_RecoCutFlags[2] != "_1"){

	
	TCanvas *c_12 = new TCanvas();

	if (log_scale == "True") {c_12->SetLogy();}

	c_name+=_RecoCutFlags[2].c_str();
	
		histoQCD_12->SetLineColor(col_QCD);
		histoQCD_12->SetLineWidth(2);
		histoQCD_12->SetFillColor(col_QCD);
		histoQCD_12->SetFillStyle(style);
		histoQCD_12->SetMarkerColor(col_QCD);
		histoQCD_12->SetTitle(c_name.c_str());
		histoQCD_12->GetXaxis()->SetRangeUser(nminX, nmaxX);
		histoQCD_12->GetYaxis()->SetRangeUser(nminY, nmaxY);
		histoQCD_12->GetXaxis()->SetTitle(asseX_name.c_str());
		histoQCD_12->GetYaxis()->SetTitle(asseY_name.c_str());
		histoQCD_12->Draw("hist");
		histoTTbar_12->SetLineColor(col_TTbar);
		histoTTbar_12->SetLineWidth(2);
		histoTTbar_12->SetFillColor(col_TTbar);
		histoTTbar_12->SetFillStyle(style);
		histoTTbar_12->SetMarkerColor(col_TTbar);
		histoTTbar_12->Draw("hist same");
		histoEWK_12->SetLineColor(col_EWK);
		histoEWK_12->SetLineWidth(2);
		histoEWK_12->SetFillColor(col_EWK);
		histoEWK_12->SetFillStyle(style);
		histoEWK_12->SetMarkerColor(col_EWK);
		histoEWK_12->Draw("hist same");	
		histoZ_12->SetLineColor(col_Z);
		histoZ_12->SetLineWidth(2);
		histoZ_12->SetFillColor(col_Z);
		histoZ_12->SetFillStyle(style);
		histoZ_12->SetMarkerColor(col_Z);
		histoZ_12->Draw("hist same");
		histoData_12->SetLineColor(col_Data);
		histoData_12->SetLineWidth(2);
		histoData_12->SetMarkerStyle(20);
		histoData_12->Draw("same");			
		
		TLegend *Leg_12 = new TLegend(0.51,0.67,0.88,0.88);
		Leg_12->SetFillColor(0);
		Leg_12->SetBorderSize(0);
		Leg_12->AddEntry(histoQCD_12,Leg_QCD.c_str());
		Leg_12->AddEntry(histoTTbar_12,Leg_TTbar.c_str());
		Leg_12->AddEntry(histoEWK_12,Leg_EWK.c_str());
		Leg_12->AddEntry(histoZ_12,Leg_Z.c_str());
		Leg_12->AddEntry(histoData_12,Leg_Data.c_str());	
	   	
	Leg_12->Draw("LP");
	
	c_12->Write(c_name.c_str());
	c_12->Close();
	
	if(Tab_cut == "True"){
		double err_QCD_12, err_TTbar_12, err_EWK_12, err_Z_12, err_Data_12;
		
		cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
		cut<<endl<< Tabcut_Mean.c_str() <<((float)((int)(histoData_12->GetMean(1)*cut_decimal)))/cut_decimal<< Tabcut_Mean_um.c_str()<<endl;
		cut<< Tabcut_Mean_err.c_str() <<((float)((int)(histoData_12->GetMeanError()*cut_decimal)))/cut_decimal<<Tabcut_Mean_um.c_str()<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl;
		cut<<endl<<Tabcut_QCD_Int.c_str()<<((float)((int)(histoQCD_12->IntegralAndError(0,-1,err_QCD_12,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_QCD_Int_err.c_str()<<((float)((int)(err_QCD_12*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_12->IntegralAndError(0,-1,err_TTbar_12,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_12*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_EWK_Int.c_str()<<((float)((int)(histoEWK_12->IntegralAndError(0,-1,err_EWK_12,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_EWK_Int_err.c_str()<<((float)((int)(err_EWK_12*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_12->IntegralAndError(0,-1,err_Z_12,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_12*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
		cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_12->IntegralAndError(0,-1,err_QCD_12,"") + histoTTbar_12->IntegralAndError(0,-1,err_TTbar_12,"") + histoEWK_12->IntegralAndError(0,-1,err_EWK_12,"") + histoZ_12->IntegralAndError(0,-1,err_Z_12,""))*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_12),2) + pow((err_TTbar_12),2) + pow((err_EWK_12),2) + pow((err_Z_12),2))*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_12->IntegralAndError(0,-1,err_Data_12,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_12*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_end.c_str()<<endl;
	}

	}
	
	if(_RecoCutFlags[3] != "_1"){

	
	TCanvas *c_123 = new TCanvas();

	if (log_scale == "True") {c_123->SetLogy();}
	
	c_name+=_RecoCutFlags[3].c_str();
	
		histoQCD_123->SetLineColor(col_QCD);
		histoQCD_123->SetLineWidth(2);
		histoQCD_123->SetFillColor(col_QCD);
		histoQCD_123->SetFillStyle(style);
		histoQCD_123->SetMarkerColor(col_QCD);
		histoQCD_123->SetTitle(c_name.c_str());
		histoQCD_123->GetXaxis()->SetRangeUser(nminX, nmaxX);
		histoQCD_123->GetYaxis()->SetRangeUser(nminY, nmaxY);
		histoQCD_123->GetXaxis()->SetTitle(asseX_name.c_str());
		histoQCD_123->GetYaxis()->SetTitle(asseY_name.c_str());
		histoQCD_123->Draw("hist");
		histoTTbar_123->SetLineColor(col_TTbar);
		histoTTbar_123->SetLineWidth(2);
		histoTTbar_123->SetFillColor(col_TTbar);
		histoTTbar_123->SetFillStyle(style);
		histoTTbar_123->SetMarkerColor(col_TTbar);
		histoTTbar_123->Draw("hist same");
		histoEWK_123->SetLineColor(col_EWK);
		histoEWK_123->SetLineWidth(2);
		histoEWK_123->SetFillColor(col_EWK);
		histoEWK_123->SetFillStyle(style);
		histoEWK_123->SetMarkerColor(col_EWK);
		histoEWK_123->Draw("hist same");	
		histoZ_123->SetLineColor(col_Z);
		histoZ_123->SetLineWidth(2);
		histoZ_123->SetFillColor(col_Z);
		histoZ_123->SetFillStyle(style);
		histoZ_123->SetMarkerColor(col_Z);
		histoZ_123->Draw("hist same");
		histoData_123->SetLineColor(col_Data);
		histoData_123->SetLineWidth(2);
		histoData_123->SetMarkerStyle(20);
		histoData_123->Draw("same");			
		
		TLegend *Leg_123 = new TLegend(0.51,0.67,0.88,0.88);
		Leg_123->SetFillColor(0);
		Leg_123->SetBorderSize(0);
		Leg_123->AddEntry(histoQCD_123,Leg_QCD.c_str());
		Leg_123->AddEntry(histoTTbar_123,Leg_TTbar.c_str());
		Leg_123->AddEntry(histoEWK_123,Leg_EWK.c_str());
		Leg_123->AddEntry(histoZ_123,Leg_Z.c_str());
		Leg_123->AddEntry(histoData_123,Leg_Data.c_str());	
	   	
	Leg_123->Draw("LP");
	
	c_123->Write(c_name.c_str());
	c_123->Close();
	
	if(Tab_cut == "True"){
		double err_QCD_123, err_TTbar_123, err_EWK_123, err_Z_123, err_Data_123;
		
		cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
		cut<<endl<< Tabcut_Mean.c_str() <<((float)((int)(histoData_123->GetMean(1)*cut_decimal)))/cut_decimal<< Tabcut_Mean_um.c_str()<<endl;
		cut<< Tabcut_Mean_err.c_str() <<((float)((int)(histoData_123->GetMeanError()*cut_decimal)))/cut_decimal<<Tabcut_Mean_um.c_str()<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl;
		cut<<endl<<Tabcut_QCD_Int.c_str()<<((float)((int)(histoQCD_123->IntegralAndError(0,-1,err_QCD_123,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_QCD_Int_err.c_str()<<((float)((int)(err_QCD_123*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_123->IntegralAndError(0,-1,err_TTbar_123,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_123*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_EWK_Int.c_str()<<((float)((int)(histoEWK_123->IntegralAndError(0,-1,err_EWK_123,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_EWK_Int_err.c_str()<<((float)((int)(err_EWK_123*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_123->IntegralAndError(0,-1,err_Z_123,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_123*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
		cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_123->IntegralAndError(0,-1,err_QCD_123,"") + histoTTbar_123->IntegralAndError(0,-1,err_TTbar_123,"") + histoEWK_123->IntegralAndError(0,-1,err_EWK_123,"") + histoZ_123->IntegralAndError(0,-1,err_Z_123,""))*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_123),2) + pow((err_TTbar_123),2) + pow((err_EWK_123),2) + pow((err_Z_123),2))*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_123->IntegralAndError(0,-1,err_Data_123,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_123*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_end.c_str()<<endl;
	}

	}
	

	if(_RecoCutFlags[4] != "_1"){


	TCanvas *c_1234 = new TCanvas();

	if (log_scale == "True") {c_1234->SetLogy();}
	
	c_name+=_RecoCutFlags[4].c_str();
	
		histoQCD_1234->SetLineColor(col_QCD);
		histoQCD_1234->SetLineWidth(2);
		histoQCD_1234->SetFillColor(col_QCD);
		histoQCD_1234->SetFillStyle(style);
		histoQCD_1234->SetMarkerColor(col_QCD);
		histoQCD_1234->SetTitle(c_name.c_str());
		histoQCD_1234->GetXaxis()->SetRangeUser(nminX, nmaxX);
		histoQCD_1234->GetYaxis()->SetRangeUser(nminY, nmaxY);
		histoQCD_1234->GetXaxis()->SetTitle(asseX_name.c_str());
		histoQCD_1234->GetYaxis()->SetTitle(asseY_name.c_str());
		histoQCD_1234->Draw("hist");
		histoTTbar_1234->SetLineColor(col_TTbar);
		histoTTbar_1234->SetLineWidth(2);
		histoTTbar_1234->SetFillColor(col_TTbar);
		histoTTbar_1234->SetFillStyle(style);
		histoTTbar_1234->SetMarkerColor(col_TTbar);
		histoTTbar_1234->Draw("hist same");
		histoEWK_1234->SetLineColor(col_EWK);
		histoEWK_1234->SetLineWidth(2);
		histoEWK_1234->SetFillColor(col_EWK);
		histoEWK_1234->SetFillStyle(style);
		histoEWK_1234->SetMarkerColor(col_EWK);
		histoEWK_1234->Draw("hist same");	
		histoZ_1234->SetLineColor(col_Z);
		histoZ_1234->SetLineWidth(2);
		histoZ_1234->SetFillColor(col_Z);
		histoZ_1234->SetFillStyle(style);
		histoZ_1234->SetMarkerColor(col_Z);
		histoZ_1234->Draw("hist same");
		histoData_1234->SetLineColor(col_Data);
		histoData_1234->SetLineWidth(2);
		histoData_1234->SetMarkerStyle(20);
		histoData_1234->Draw("same");			
		
		TLegend *Leg_1234 = new TLegend(0.51,0.67,0.88,0.88);
		Leg_1234->SetFillColor(0);
		Leg_1234->SetBorderSize(0);
		Leg_1234->AddEntry(histoQCD_1234,Leg_QCD.c_str());
		Leg_1234->AddEntry(histoTTbar_1234,Leg_TTbar.c_str());
		Leg_1234->AddEntry(histoEWK_1234,Leg_EWK.c_str());
		Leg_1234->AddEntry(histoZ_1234,Leg_Z.c_str());
		Leg_1234->AddEntry(histoData_1234,Leg_Data.c_str());		
	   	
	Leg_1234->Draw("LP");
	
	c_1234->Write(c_name.c_str());
	c_1234->Close();
	
	if(Tab_cut == "True"){
		double err_QCD_1234, err_TTbar_1234, err_EWK_1234, err_Z_1234, err_Data_1234;
		
		cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
		cut<<endl<< Tabcut_Mean.c_str() <<((float)((int)(histoData_1234->GetMean(1)*cut_decimal)))/cut_decimal<< Tabcut_Mean_um.c_str()<<endl;
		cut<< Tabcut_Mean_err.c_str() <<((float)((int)(histoData_1234->GetMeanError()*cut_decimal)))/cut_decimal<<Tabcut_Mean_um.c_str()<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl;
		cut<<endl<<Tabcut_QCD_Int.c_str()<<((float)((int)(histoQCD_1234->IntegralAndError(0,-1,err_QCD_1234,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_QCD_Int_err.c_str()<<((float)((int)(err_QCD_1234*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_1234->IntegralAndError(0,-1,err_TTbar_1234,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_1234*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_EWK_Int.c_str()<<((float)((int)(histoEWK_1234->IntegralAndError(0,-1,err_EWK_1234,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_EWK_Int_err.c_str()<<((float)((int)(err_EWK_1234*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_1234->IntegralAndError(0,-1,err_Z_1234,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_1234*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
		cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_1234->IntegralAndError(0,-1,err_QCD_1234,"") + histoTTbar_1234->IntegralAndError(0,-1,err_TTbar_1234,"") + histoEWK_1234->IntegralAndError(0,-1,err_EWK_1234,"") + histoZ_1234->IntegralAndError(0,-1,err_Z_1234,""))*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_1234),2) + pow((err_TTbar_1234),2) + pow((err_EWK_1234),2) + pow((err_Z_1234),2))*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_1234->IntegralAndError(0,-1,err_Data_1234,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_1234*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_end.c_str()<<endl;
	}	

	}
	
	

	if(_RecoCutFlags[5] != "_1"){
	
	
	TCanvas *c_12345 = new TCanvas();

	if (log_scale == "True") {c_12345->SetLogy();}
	
	c_name+=_RecoCutFlags[5].c_str();
	
		histoQCD_12345->SetLineColor(col_QCD);
		histoQCD_12345->SetLineWidth(2);
		histoQCD_12345->SetFillColor(col_QCD);
		histoQCD_12345->SetFillStyle(style);
		histoQCD_12345->SetMarkerColor(col_QCD);
		histoQCD_12345->SetTitle(c_name.c_str());
		histoQCD_12345->GetXaxis()->SetRangeUser(nminX, nmaxX);
		histoQCD_12345->GetYaxis()->SetRangeUser(nminY, nmaxY);
		histoQCD_12345->GetXaxis()->SetTitle(asseX_name.c_str());
		histoQCD_12345->GetYaxis()->SetTitle(asseY_name.c_str());
		histoQCD_12345->Draw("hist");
		histoTTbar_12345->SetLineColor(col_TTbar);
		histoTTbar_12345->SetLineWidth(2);
		histoTTbar_12345->SetFillColor(col_TTbar);
		histoTTbar_12345->SetFillStyle(style);
		histoTTbar_12345->SetMarkerColor(col_TTbar);
		histoTTbar_12345->Draw("hist same");
		histoEWK_12345->SetLineColor(col_EWK);
		histoEWK_12345->SetLineWidth(2);
		histoEWK_12345->SetFillColor(col_EWK);
		histoEWK_12345->SetFillStyle(style);
		histoEWK_12345->SetMarkerColor(col_EWK);
		histoEWK_12345->Draw("hist same");	
		histoZ_12345->SetLineColor(col_Z);
		histoZ_12345->SetLineWidth(2);
		histoZ_12345->SetFillColor(col_Z);
		histoZ_12345->SetFillStyle(style);
		histoZ_12345->SetMarkerColor(col_Z);
		histoZ_12345->Draw("hist same");
		histoData_12345->SetLineColor(col_Data);
		histoData_12345->SetLineWidth(2);
		histoData_12345->SetMarkerStyle(20);
		histoData_12345->Draw("same");			
		
		TLegend *Leg_12345 = new TLegend(0.51,0.67,0.88,0.88);
		Leg_12345->SetFillColor(0);
		Leg_12345->SetBorderSize(0);
		Leg_12345->AddEntry(histoQCD_12345,Leg_QCD.c_str());
		Leg_12345->AddEntry(histoTTbar_12345,Leg_TTbar.c_str());
		Leg_12345->AddEntry(histoEWK_12345,Leg_EWK.c_str());
		Leg_12345->AddEntry(histoZ_12345,Leg_Z.c_str());
		Leg_12345->AddEntry(histoData_12345,Leg_Data.c_str());	
	   	
	Leg_12345->Draw("LP");
	
	c_12345->Write(c_name.c_str());
	c_12345->Close();
	
	if(Tab_cut == "True"){
		double err_QCD_12345, err_TTbar_12345, err_EWK_12345, err_Z_12345, err_Data_12345;
		
		cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
		cut<<endl<< Tabcut_Mean.c_str() <<((float)((int)(histoData_12345->GetMean(1)*cut_decimal)))/cut_decimal<< Tabcut_Mean_um.c_str()<<endl;
		cut<< Tabcut_Mean_err.c_str() <<((float)((int)(histoData_12345->GetMeanError()*cut_decimal)))/cut_decimal<<Tabcut_Mean_um.c_str()<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl;
		cut<<endl<<Tabcut_QCD_Int.c_str()<<((float)((int)(histoQCD_12345->IntegralAndError(0,-1,err_QCD_12345,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_QCD_Int_err.c_str()<<((float)((int)(err_QCD_12345*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_12345->IntegralAndError(0,-1,err_TTbar_12345,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_12345*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_EWK_Int.c_str()<<((float)((int)(histoEWK_12345->IntegralAndError(0,-1,err_EWK_12345,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_EWK_Int_err.c_str()<<((float)((int)(err_EWK_12345*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_12345->IntegralAndError(0,-1,err_Z_12345,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_12345*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
		cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_12345->IntegralAndError(0,-1,err_QCD_12345,"") + histoTTbar_12345->IntegralAndError(0,-1,err_TTbar_12345,"") + histoEWK_12345->IntegralAndError(0,-1,err_EWK_12345,"") + histoZ_12345->IntegralAndError(0,-1,err_Z_12345,""))*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_12345),2) + pow((err_TTbar_12345),2) + pow((err_EWK_12345),2) + pow((err_Z_12345),2))*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_12345->IntegralAndError(0,-1,err_Data_12345,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_12345*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_end.c_str()<<endl;
	}	

	}
	


	if(_RecoCutFlags[6] != "_1"){

	
	TCanvas *c_123456 = new TCanvas();

	if (log_scale == "True") {c_123456->SetLogy();}
	
	c_name+=_RecoCutFlags[6].c_str();
	
		histoQCD_123456->SetLineColor(col_QCD);
		histoQCD_123456->SetLineWidth(2);
		histoQCD_123456->SetFillColor(col_QCD);
		histoQCD_123456->SetFillStyle(style);
		histoQCD_123456->SetMarkerColor(col_QCD);
		histoQCD_123456->SetTitle(c_name.c_str());
		histoQCD_123456->GetXaxis()->SetRangeUser(nminX, nmaxX);
		histoQCD_123456->GetYaxis()->SetRangeUser(nminY, nmaxY);
		histoQCD_123456->GetXaxis()->SetTitle(asseX_name.c_str());
		histoQCD_123456->GetYaxis()->SetTitle(asseY_name.c_str());
		histoQCD_123456->Draw("hist");
		histoTTbar_123456->SetLineColor(col_TTbar);
		histoTTbar_123456->SetLineWidth(2);
		histoTTbar_123456->SetFillColor(col_TTbar);
		histoTTbar_123456->SetFillStyle(style);
		histoTTbar_123456->SetMarkerColor(col_TTbar);
		histoTTbar_123456->Draw("hist same");
		histoEWK_123456->SetLineColor(col_EWK);
		histoEWK_123456->SetLineWidth(2);
		histoEWK_123456->SetFillColor(col_EWK);
		histoEWK_123456->SetFillStyle(style);
		histoEWK_123456->SetMarkerColor(col_EWK);
		histoEWK_123456->Draw("hist same");	
		histoZ_123456->SetLineColor(col_Z);
		histoZ_123456->SetLineWidth(2);
		histoZ_123456->SetFillColor(col_Z);
		histoZ_123456->SetFillStyle(style);
		histoZ_123456->SetMarkerColor(col_Z);
		histoZ_123456->Draw("hist same");
		histoData_123456->SetLineColor(col_Data);
		histoData_123456->SetLineWidth(2);
		histoData_123456->SetMarkerStyle(20);
		histoData_123456->Draw("same");			
		
		TLegend *Leg_123456 = new TLegend(0.51,0.67,0.88,0.88);
		Leg_123456->SetFillColor(0);
		Leg_123456->SetBorderSize(0);
		Leg_123456->AddEntry(histoQCD_123456,Leg_QCD.c_str());
		Leg_123456->AddEntry(histoTTbar_123456,Leg_TTbar.c_str());
		Leg_123456->AddEntry(histoEWK_123456,Leg_EWK.c_str());
		Leg_123456->AddEntry(histoZ_123456,Leg_Z.c_str());
		Leg_123456->AddEntry(histoData_123456,Leg_Data.c_str());	
	   	
	Leg_123456->Draw("LP");
	
	c_123456->Write(c_name.c_str());
	c_123456->Close();
	
	if(Tab_cut == "True"){
		double err_QCD_123456, err_TTbar_123456, err_EWK_123456, err_Z_123456, err_Data_123456;
		
		cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
		cut<<endl<< Tabcut_Mean.c_str() <<((float)((int)(histoData_123456->GetMean(1)*cut_decimal)))/cut_decimal<< Tabcut_Mean_um.c_str()<<endl;
		cut<< Tabcut_Mean_err.c_str() <<((float)((int)(histoData_123456->GetMeanError()*cut_decimal)))/cut_decimal<<Tabcut_Mean_um.c_str()<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl;
		cut<<endl<<Tabcut_QCD_Int.c_str()<<((float)((int)(histoQCD_123456->IntegralAndError(0,-1,err_QCD_123456,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_QCD_Int_err.c_str()<<((float)((int)(err_QCD_123456*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_123456->IntegralAndError(0,-1,err_TTbar_123456,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_123456*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_EWK_Int.c_str()<<((float)((int)(histoEWK_123456->IntegralAndError(0,-1,err_EWK_123456,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_EWK_Int_err.c_str()<<((float)((int)(err_EWK_123456*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_123456->IntegralAndError(0,-1,err_Z_123456,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_123456*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
		cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_123456->IntegralAndError(0,-1,err_QCD_123456,"") + histoTTbar_123456->IntegralAndError(0,-1,err_TTbar_123456,"") + histoEWK_123456->IntegralAndError(0,-1,err_EWK_123456,"") + histoZ_123456->IntegralAndError(0,-1,err_Z_123456,""))*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_123456),2) + pow((err_TTbar_123456),2) + pow((err_EWK_123456),2) + pow((err_Z_123456),2))*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_123456->IntegralAndError(0,-1,err_Data_123456,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_123456*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_end.c_str()<<endl;
	}

	}

Dir_3->cd();

TDirectory *Dir_4b;
Dir_4b = Dir_3->mkdir("Stacked_Plots");
Dir_4b->cd();
	
	TCanvas *d_1 = new TCanvas();
	
	if (log_scale == "True") {d_1->SetLogy();}

	THStack *Stackd_1 = new THStack();

	string Stack = "Stack_";
	string Stackd_name = (Stack + grafico_name).c_str();	

	Stackd_name+=_RecoCutFlags[1].c_str();
	

	Stackd_1->Add(histoQCD_1);
	Stackd_1->Add(histoTTbar_1);
	Stackd_1->Add(histoEWK_1);
	Stackd_1->Add(histoZ_1);
	Stackd_1->Draw("hist");
	
gPad->Update();
gStyle->SetOptStat(0);

	Stackd_1->SetTitle(Stackd_name.c_str());
	Stackd_1->GetXaxis()->SetRangeUser(nminX, nmaxX);
	Stackd_1->SetMinimum(nminY);
	Stackd_1->SetMaximum(nmaxY);
	Stackd_1->GetXaxis()->SetTitle(asseX_name.c_str());
	Stackd_1->GetYaxis()->SetTitle(asseY_name.c_str());

        histoData_1->Draw("same");			

	TLegend *StackLeg_1 = new TLegend(0.51,0.67,0.88,0.88);
	StackLeg_1->SetFillColor(0);
	StackLeg_1->SetBorderSize(0);
	StackLeg_1->AddEntry(histoQCD_1,Leg_QCD.c_str());
	StackLeg_1->AddEntry(histoTTbar_1,Leg_TTbar.c_str());
	StackLeg_1->AddEntry(histoEWK_1,Leg_EWK.c_str());
	StackLeg_1->AddEntry(histoZ_1,Leg_Z.c_str());
	StackLeg_1->AddEntry(histoData_1,Leg_Data.c_str());	
	   	
	StackLeg_1->Draw("LP");
	
	d_1->Write(Stackd_name.c_str());
	d_1->Close();
	
	if(_RecoCutFlags[2] != "_1"){

	TCanvas *d_12 = new TCanvas();
	
	if (log_scale == "True") {d_12->SetLogy();}

	THStack *Stackd_12 = new THStack();
	
	Stackd_name+=_RecoCutFlags[2].c_str();

		Stackd_12->Add(histoQCD_12);
		Stackd_12->Add(histoTTbar_12);
		Stackd_12->Add(histoEWK_12);
		Stackd_12->Add(histoZ_12);
		Stackd_12->Draw("hist");
	
gPad->Update();
gStyle->SetOptStat(0);

	Stackd_12->SetTitle(Stackd_name.c_str());
	Stackd_12->GetXaxis()->SetRangeUser(nminX, nmaxX);
	Stackd_12->SetMinimum(nminY);
	Stackd_12->SetMaximum(nmaxY);
	Stackd_12->GetXaxis()->SetTitle(asseX_name.c_str());
	Stackd_12->GetYaxis()->SetTitle(asseY_name.c_str());

        histoData_12->Draw("same");			

		TLegend *StackLeg_12 = new TLegend(0.51,0.67,0.88,0.88);
		StackLeg_12->SetFillColor(0);
		StackLeg_12->SetBorderSize(0);
		StackLeg_12->AddEntry(histoQCD_12,Leg_QCD.c_str());
		StackLeg_12->AddEntry(histoTTbar_12,Leg_TTbar.c_str());
		StackLeg_12->AddEntry(histoEWK_12,Leg_EWK.c_str());
		StackLeg_12->AddEntry(histoZ_12,Leg_Z.c_str());
		StackLeg_12->AddEntry(histoData_12,Leg_Data.c_str());	
	   	
	StackLeg_12->Draw("LP");
	
	d_12->Write(Stackd_name.c_str());
	d_12->Close();
	}

	if(_RecoCutFlags[3] != "_1"){

	TCanvas *d_123 = new TCanvas();
	
	if (log_scale == "True") {d_123->SetLogy();}
	
	THStack *Stackd_123 = new THStack();

	Stackd_name+=_RecoCutFlags[3].c_str();

		Stackd_123->Add(histoQCD_123);
		Stackd_123->Add(histoTTbar_123);
		Stackd_123->Add(histoEWK_123);
		Stackd_123->Add(histoZ_123);
		Stackd_123->Draw("hist");
	
gPad->Update();
gStyle->SetOptStat(0);

	Stackd_123->SetTitle(Stackd_name.c_str());
	Stackd_123->GetXaxis()->SetRangeUser(nminX, nmaxX);
	Stackd_123->SetMinimum(nminY);
	Stackd_123->SetMaximum(nmaxY);
	Stackd_123->GetXaxis()->SetTitle(asseX_name.c_str());
	Stackd_123->GetYaxis()->SetTitle(asseY_name.c_str());

        histoData_123->Draw("same");			

		TLegend *StackLeg_123 = new TLegend(0.51,0.67,0.88,0.88);
		StackLeg_123->SetFillColor(0);
		StackLeg_123->SetBorderSize(0);
		StackLeg_123->AddEntry(histoQCD_123,Leg_QCD.c_str());
		StackLeg_123->AddEntry(histoTTbar_123,Leg_TTbar.c_str());
		StackLeg_123->AddEntry(histoEWK_123,Leg_EWK.c_str());
		StackLeg_123->AddEntry(histoZ_123,Leg_Z.c_str());
		StackLeg_123->AddEntry(histoData_123,Leg_Data.c_str());	
	   	
	StackLeg_123->Draw("LP");
	
	d_123->Write(Stackd_name.c_str());
	d_123->Close();
	}

	if(_RecoCutFlags[4] != "_1"){

	TCanvas *d_1234 = new TCanvas();
	
	if (log_scale == "True") {d_1234->SetLogy();}

	THStack *Stackd_1234 = new THStack();
	
	Stackd_name+=_RecoCutFlags[4].c_str();

		Stackd_1234->Add(histoQCD_1234);
		Stackd_1234->Add(histoTTbar_1234);
		Stackd_1234->Add(histoEWK_1234);
		Stackd_1234->Add(histoZ_1234);
		Stackd_1234->Draw("hist");
	
gPad->Update();
gStyle->SetOptStat(0);

	Stackd_1234->SetTitle(Stackd_name.c_str());
	Stackd_1234->GetXaxis()->SetRangeUser(nminX, nmaxX);
	Stackd_1234->SetMinimum(nminY);
	Stackd_1234->SetMaximum(nmaxY);
	Stackd_1234->GetXaxis()->SetTitle(asseX_name.c_str());
	Stackd_1234->GetYaxis()->SetTitle(asseY_name.c_str());

        histoData_1234->Draw("same");			

		TLegend *StackLeg_1234 = new TLegend(0.51,0.67,0.88,0.88);
		StackLeg_1234->SetFillColor(0);
		StackLeg_1234->SetBorderSize(0);
		StackLeg_1234->AddEntry(histoQCD_1234,Leg_QCD.c_str());
		StackLeg_1234->AddEntry(histoTTbar_1234,Leg_TTbar.c_str());
		StackLeg_1234->AddEntry(histoEWK_1234,Leg_EWK.c_str());
		StackLeg_1234->AddEntry(histoZ_1234,Leg_Z.c_str());
		StackLeg_1234->AddEntry(histoData_1234,Leg_Data.c_str());	
	   	
	StackLeg_1234->Draw("LP");
	
	d_1234->Write(Stackd_name.c_str());
	d_1234->Close();
	}

	if(_RecoCutFlags[5] != "_1"){

	TCanvas *d_12345 = new TCanvas();
	
	if (log_scale == "True") {d_12345->SetLogy();}
	
	THStack *Stackd_12345 = new THStack();

	Stackd_name+=_RecoCutFlags[5].c_str();

		Stackd_12345->Add(histoQCD_12345);
		Stackd_12345->Add(histoTTbar_12345);
		Stackd_12345->Add(histoEWK_12345);
		Stackd_12345->Add(histoZ_12345);
		Stackd_12345->Draw("hist");
	
gPad->Update();
gStyle->SetOptStat(0);

	Stackd_12345->SetTitle(Stackd_name.c_str());
	Stackd_12345->GetXaxis()->SetRangeUser(nminX, nmaxX);
	Stackd_12345->SetMinimum(nminY);
	Stackd_12345->SetMaximum(nmaxY);
	Stackd_12345->GetXaxis()->SetTitle(asseX_name.c_str());
	Stackd_12345->GetYaxis()->SetTitle(asseY_name.c_str());

        histoData_12345->Draw("same");			

		TLegend *StackLeg_12345 = new TLegend(0.51,0.67,0.88,0.88);
		StackLeg_12345->SetFillColor(0);
		StackLeg_12345->SetBorderSize(0);
		StackLeg_12345->AddEntry(histoQCD_12345,Leg_QCD.c_str());
		StackLeg_12345->AddEntry(histoTTbar_12345,Leg_TTbar.c_str());
		StackLeg_12345->AddEntry(histoEWK_12345,Leg_EWK.c_str());
		StackLeg_12345->AddEntry(histoZ_12345,Leg_Z.c_str());
		StackLeg_12345->AddEntry(histoData_12345,Leg_Data.c_str());
	   	
	StackLeg_12345->Draw("LP");
	
	d_12345->Write(Stackd_name.c_str());
	d_12345->Close();
	}

	if(_RecoCutFlags[6] != "_1"){

	TCanvas *d_123456 = new TCanvas();
	
	if (log_scale == "True") {d_123456->SetLogy();}
	
	THStack *Stackd_123456 = new THStack();

	Stackd_name+=_RecoCutFlags[6].c_str();

		Stackd_123456->Add(histoQCD_123456);
		Stackd_123456->Add(histoTTbar_123456);
		Stackd_123456->Add(histoEWK_123456);
		Stackd_123456->Add(histoZ_123456);
		Stackd_123456->Draw("hist");
	
gPad->Update();
gStyle->SetOptStat(0);

	Stackd_123456->SetTitle(Stackd_name.c_str());
	Stackd_123456->GetXaxis()->SetRangeUser(nminX, nmaxX);
	Stackd_123456->SetMinimum(nminY);
	Stackd_123456->SetMaximum(nmaxY);
	Stackd_123456->GetXaxis()->SetTitle(asseX_name.c_str());
	Stackd_123456->GetYaxis()->SetTitle(asseY_name.c_str());

        histoData_123456->Draw("same");			

		TLegend *StackLeg_123456 = new TLegend(0.51,0.67,0.88,0.88);
		StackLeg_123456->SetFillColor(0);
		StackLeg_123456->SetBorderSize(0);
		StackLeg_123456->AddEntry(histoQCD_123456,Leg_QCD.c_str());
		StackLeg_123456->AddEntry(histoTTbar_123456,Leg_TTbar.c_str());
		StackLeg_123456->AddEntry(histoEWK_123456,Leg_EWK.c_str());
		StackLeg_123456->AddEntry(histoZ_123456,Leg_Z.c_str());
		StackLeg_123456->AddEntry(histoData_123456,Leg_Data.c_str());	
	   	
	StackLeg_123456->Draw("LP");
	
	d_123456->Write(Stackd_name.c_str());
	d_123456->Close();
	}

Dir_3->cd();
Dir_2a->cd();
Dir_1->cd();


}	

  cut.close();

  outplots->cd();
  outplots->Write();
  outplots->Close();
}
