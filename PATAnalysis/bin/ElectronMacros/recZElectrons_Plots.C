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

void recZElectrons_Plots(string selections){

string Analysis_Note = "False";
if (Analysis_Note == "False") gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);

string log_scale = "True";

string Tab_cut = "True";
        
                
        //Background MC
        TFile* QCD_EMEnriched_all_TF = TFile::Open("../Simulazioni/CALO30Jets/QCD_EMEnriched_CALO30Eta3_ALL.root");
        TFile* QCD_BCtoE_all_TF = TFile::Open("../Simulazioni/CALO30Jets/QCD_BCtoE_CALO30Eta3_ALL.root");
        TFile* TTbar_TF = TFile::Open("../Simulazioni/CALO30Jets/TTbarJets_Madgraph_Spring10_CALO30Eta3.root");
        TFile* W_TF = TFile::Open("../Simulazioni/CALO30Jets/WJets_Madgraph_Spring10_CALO30Eta3.root");
        
	//Signal MC
        TFile *Z_TF = TFile::Open("../Simulazioni/CALO30Jets/ZJets_Madgraph_Spring10_CALO30Eta3_Norm50_ALL.root");

        //Data
        TFile *Data_TF = TFile::Open("../Dati/CALO30Jets/Data_Sep11_CALO30Eta3.root");
        
        //Output
        string out = "../Plots/CALO30Jets/recZElectrons_Plots_CALO30Eta3_Sep11";        
        string output = out;
        output+=".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
	      
        //Normalization factor
        double iniLumi = 50.; //pb-1
        double targetLumi = 2.90274; //pb-1
        double scale = 1.;
        if(iniLumi!=0)scale = targetLumi/iniLumi;

	//rebin
	//Leading Electron
	int rebin_recLeadElPt = 2;	
	int rebin_recLeadElEta = 2;
	int rebin_recLeadElIP = 1;
	int rebin_recLeadElfBrem = 2;
	//Second Electron
	int rebin_recSecElPt = 2;	
	int rebin_recSecElEta = 2;
	int rebin_recSecElIP = 1;
	int rebin_recSecElfBrem = 2;

	//colors	
	int col_Z = 800;
	int col_QCD_EMEnriched_all= 616+3;
	int col_QCD_BCtoE_all= 800+7;
	int col_TTbar= 632;
	int col_W= 800+3;
	int col_Data= 1;

	//FillStyle	
	int style = 1001;

	//SetRangeUser
	//LeadElPt
	double nminX_recLeadElPt = 0.0; 
	double nmaxX_recLeadElPt = 200.0;
	double nminY_recLeadElPt = 0.001; 
	double nmaxY_recLeadElPt = 300.0;
	//LeadElEta
	double nminX_recLeadElEta = -10.0; 
	double nmaxX_recLeadElEta = 10.0;
	double nminY_recLeadElEta = 0.001; 
	double nmaxY_recLeadElEta = 300.0;
	//LeadElIP
	double nminX_recLeadElIP = 0.0; 
	double nmaxX_recLeadElIP = 1.0;
	double nminY_recLeadElIP = 0.001; 
	double nmaxY_recLeadElIP = 300.0;
	//LeadElfBrem	
	double nminX_recLeadElfBrem = 0.0; 
	double nmaxX_recLeadElfBrem = 4.0;
	double nminY_recLeadElfBrem = 0.001; 
	double nmaxY_recLeadElfBrem = 300.0;
	//SecElPt
	double nminX_recSecElPt= 0.0; 
	double nmaxX_recSecElPt = 200.0;
	double nminY_recSecElPt = 0.001; 
	double nmaxY_recSecElPt = 300.0;
	//SecElEta
	double nminX_recSecElEta = -10.0; 
	double nmaxX_recSecElEta = 10.0;
	double nminY_recSecElEta = 0.001; 
	double nmaxY_recSecElEta = 300.0;
	//SecElIP
	double nminX_recSecElIP = 0.0; 
	double nmaxX_recSecElIP = 1.0;
	double nminY_recSecElIP = 0.001; 
	double nmaxY_recSecElIP = 300.0;
	//SecElfBrem	
	double nminX_recSecElfBrem = 0.0; 
	double nmaxX_recSecElfBrem = 4.0;
	double nminY_recSecElfBrem = 0.001; 
	double nmaxY_recSecElfBrem = 300.0;
	
	//Legenda
	string Leg_QCD_EMEnriched_all = "QCD EM Enriched";
	string Leg_QCD_BCtoE_all = "QCD b,c #rightarrow e";
	string Leg_TTbar = "t #bar{t}+jets";
	string Leg_W = "W+jets";
	string Leg_Z = "Z+jets";
	string Leg_Data	= "Data";

	//tabella cut
	int cut_decimal = 10000;
	string Tabcut_title = "Cut level = ";
	string Tabcut_QCD_EMEnriched_all_Int = "QCD EM Enriched (PYTHIA6) Integral = ";
	string Tabcut_QCD_EMEnriched_all_Int_err = "QCD EM Enriched (PYTHIA6) Integral error = ";
	string Tabcut_QCD_BCtoE_all_Int = "QCD QCD b,c-> e (PYTHIA6) Integral = ";
	string Tabcut_QCD_BCtoE_all_Int_err = "QCD QCD b,c-> e (PYTHIA6) Integral error = ";
	string Tabcut_TTbar_Int = "TTbar+jets (MADGRAPH) Integral = ";
	string Tabcut_TTbar_Int_err = "TTbar+jets (MADGRAPH) Integral error = ";
	string Tabcut_W_Int = "W+jets (MADGRAPH) Integral = ";
	string Tabcut_W_Int_err = "W+jets (MADGRAPH) Integral error = ";
	string Tabcut_Z_Int = "Z+jets (MADGRAPH) Integral = ";
	string Tabcut_Z_Int_err = "Z+jets (MADGRAPH) Integral error = ";
	string Tabcut_riga = "-----------------------------------------------------------------------------";
	string Tabcut_Total_Int = "Total Integral = ";
	string Tabcut_Total_Int_err = "Total Integral error = ";
	string Tabcut_Data_Int = "Data Integral = ";
	string Tabcut_Data_Int_err = "Data Integral error = ";
	string Tabcut_end = "#############################################################################";
	
	
	//Selections
	int _Acc  = 1;
	int _Trg  = 2;
	int _Qual = 0;
	int _Imp  = 3;
	int _Iso  = 4;
	int _EiD  = 5;
	
//------------------------------------------
	
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

        
        if(!QCD_EMEnriched_all_TF){
	cout<<"Error! QCD EM Enriched file doesn't exist!"<<endl;
	return;
	}
	
	if(!QCD_BCtoE_all_TF){
	cout<<"Error! QCD BC to E file doesn't exist!"<<endl;
	return;
	}

        if(!TTbar_TF){
	cout<<"Error! TTbar file doesn't exist!"<<endl;
	return;
	}
	
        if(!W_TF){
	cout<<"Error! W file doesn't exist!"<<endl;
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
string cartella2a = "recZElectrons_Plots";	

string cartellaplot;

//grafici
int rebin = 0;
double nminX = 0., nmaxX = 0., nminY = 0. , nmaxY = 0.;

string grafico_name;
string grafico_name_Cartella2a_recLeadElPt = "recLeadElPt";
string grafico_name_Cartella2a_recLeadElEta = "recLeadElEta";
string grafico_name_Cartella2a_recLeadElIP = "recLeadElIP";
string grafico_name_Cartella2a_recLeadElfBrem = "recLeadElfBrem";
string grafico_name_Cartella2a_recSecElPt = "recSecElPt";
string grafico_name_Cartella2a_recSecElEta = "recSecElEta";
string grafico_name_Cartella2a_recSecElIP = "recSecElIP";
string grafico_name_Cartella2a_recSecElfBrem = "recSecElfBrem";

string asseX_name;
string asseY_name;
string asseX_name_Cartella2a_recLeadElPt = "Reconstructed Leading Electron P_{T} (GeV/c))";
string asseY_name_Cartella2a_recLeadElPt = Form("Events/(%.0f GeV/c)", 1.0*rebin_recLeadElPt); //bins da 1 GeV/c
string asseX_name_Cartella2a_recLeadElEta = "Reconstructed Leading Electron #eta"; 
string asseY_name_Cartella2a_recLeadElEta = Form("Events/(%.1f)", 0.05*rebin_recLeadElEta); //bins da 0.05
string asseX_name_Cartella2a_recLeadElIP = "Reconstructed Leading Electron IP (cm)";
string asseY_name_Cartella2a_recLeadElIP = Form("Events/(%.3f)", 0.001*rebin_recLeadElIP); //bins da 0.001
string asseX_name_Cartella2a_recLeadElfBrem = "Reconstructed Leading Electron fBrem";
string asseY_name_Cartella2a_recLeadElfBrem = Form("Events/(%.2f)", 0.02*rebin_recLeadElfBrem); //bins da 0.02
string asseX_name_Cartella2a_recSecElPt = "Reconstructed Second Electron P_{T} (GeV/c))";
string asseY_name_Cartella2a_recSecElPt = Form("Events/(%.0f GeV/c)", 1.0*rebin_recSecElPt); //bins da 1 GeV/c
string asseX_name_Cartella2a_recSecElEta = "Reconstructed Second Electron #eta"; 
string asseY_name_Cartella2a_recSecElEta = Form("Events/(%.1f)", 0.05*rebin_recSecElEta); //bins da 0.05
string asseX_name_Cartella2a_recSecElIP = "Reconstructed Second Electron IP (cm)";
string asseY_name_Cartella2a_recSecElIP = Form("Events/(%.3f)", 0.001*rebin_recSecElIP); //bins da 0.001
string asseX_name_Cartella2a_recSecElfBrem = "Reconstructed Second Electron fBrem";
string asseY_name_Cartella2a_recSecElfBrem = Form("Events/(%.2f)", 0.02*rebin_recSecElfBrem); //bins da 0.02



TDirectory *Dir_1;
Dir_1 = outplots->mkdir(cartella1.c_str());
Dir_1->cd();


//--------------------------------------- directory recZElectrons_Plots


TDirectory *Dir_2a;
Dir_2a = Dir_1->mkdir(cartella2a.c_str());
Dir_2a->cd();

TDirectoryFile *Cartella1_QCD_EMEnriched_all = (TDirectoryFile*) QCD_EMEnriched_all_TF->Get(cartella1.c_str());
TDirectoryFile *Cartella1_QCD_EMEnriched_all_Cartella2a = (TDirectoryFile*) Cartella1_QCD_EMEnriched_all->Get(cartella2a.c_str());

TDirectoryFile *Cartella1_QCD_BCtoE_all = (TDirectoryFile*) QCD_BCtoE_all_TF->Get(cartella1.c_str());
TDirectoryFile *Cartella1_QCD_BCtoE_all_Cartella2a = (TDirectoryFile*) Cartella1_QCD_BCtoE_all->Get(cartella2a.c_str());

TDirectoryFile *Cartella1_W = (TDirectoryFile*) W_TF->Get(cartella1.c_str());
TDirectoryFile *Cartella1_W_Cartella2a = (TDirectoryFile*) Cartella1_W->Get(cartella2a.c_str());

TDirectoryFile *Cartella1_TTbar = (TDirectoryFile*) TTbar_TF->Get(cartella1.c_str());
TDirectoryFile *Cartella1_TTbar_Cartella2a = (TDirectoryFile*) Cartella1_TTbar->Get(cartella2a.c_str());

TDirectoryFile *Cartella1_Z = (TDirectoryFile*) Z_TF->Get(cartella1.c_str());
TDirectoryFile *Cartella1_Z_Cartella2a = (TDirectoryFile*) Cartella1_Z->Get(cartella2a.c_str());

TDirectoryFile *Cartella1_Data = (TDirectoryFile*) Data_TF->Get(cartella1.c_str());
TDirectoryFile *Cartella1_Data_Cartella2a = (TDirectoryFile*) Cartella1_Data->Get(cartella2a.c_str());

//----------------------------------------- Grafici cartella recZElectrons_Plots

for(int b=1; b<9; b++){
if(b==1){cartellaplot = grafico_name_Cartella2a_recLeadElPt;
	 grafico_name = grafico_name_Cartella2a_recLeadElPt;
	 asseX_name = asseX_name_Cartella2a_recLeadElPt; 
	 asseY_name = asseY_name_Cartella2a_recLeadElPt;
	 rebin = rebin_recLeadElPt;
	 nminX = nminX_recLeadElPt;
	 nmaxX = nmaxX_recLeadElPt;
	 nminY = nminY_recLeadElPt;
	 nmaxY = nmaxY_recLeadElPt;}

else if(b==2){cartellaplot = grafico_name_Cartella2a_recLeadElEta;
	      grafico_name = grafico_name_Cartella2a_recLeadElEta;
	      asseX_name = asseX_name_Cartella2a_recLeadElEta; 
	      asseY_name = asseY_name_Cartella2a_recLeadElEta;
	      rebin = rebin_recLeadElEta;
	      nminX = nminX_recLeadElEta;
	      nmaxX = nmaxX_recLeadElEta;
	      nminY = nminY_recLeadElEta;
	      nmaxY = nmaxY_recLeadElEta;}

else if(b==3){cartellaplot = grafico_name_Cartella2a_recLeadElIP;
	      grafico_name = grafico_name_Cartella2a_recLeadElIP;
	      asseX_name = asseX_name_Cartella2a_recLeadElIP; 
	      asseY_name = asseY_name_Cartella2a_recLeadElIP;
	      rebin = rebin_recLeadElIP;
	      nminX = nminX_recLeadElIP;
	      nmaxX = nmaxX_recLeadElIP;
	      nminY = nminY_recLeadElIP;
	      nmaxY = nmaxY_recLeadElIP;}

else if(b==4){cartellaplot = grafico_name_Cartella2a_recLeadElfBrem;
	      grafico_name = grafico_name_Cartella2a_recLeadElfBrem;
	      asseX_name = asseX_name_Cartella2a_recLeadElfBrem; 
	      asseY_name = asseY_name_Cartella2a_recLeadElfBrem;
	      rebin = rebin_recLeadElfBrem;
	      nminX = nminX_recLeadElfBrem;
	      nmaxX = nmaxX_recLeadElfBrem;
	      nminY = nminY_recLeadElfBrem;
	      nmaxY = nmaxY_recLeadElfBrem;}

if(b==5){cartellaplot = grafico_name_Cartella2a_recSecElPt;
	 grafico_name = grafico_name_Cartella2a_recSecElPt;
	 asseX_name = asseX_name_Cartella2a_recSecElPt; 
	 asseY_name = asseY_name_Cartella2a_recSecElPt;
	 rebin = rebin_recSecElPt;
	 nminX = nminX_recSecElPt;
	 nmaxX = nmaxX_recSecElPt;
	 nminY = nminY_recSecElPt;
	 nmaxY = nmaxY_recSecElPt;}

else if(b==6){cartellaplot = grafico_name_Cartella2a_recSecElEta;
	      grafico_name = grafico_name_Cartella2a_recSecElEta;
	      asseX_name = asseX_name_Cartella2a_recSecElEta; 
	      asseY_name = asseY_name_Cartella2a_recSecElEta;
	      rebin = rebin_recSecElEta;
	      nminX = nminX_recSecElEta;
	      nmaxX = nmaxX_recSecElEta;
	      nminY = nminY_recSecElEta;
	      nmaxY = nmaxY_recSecElEta;}

else if(b==7){cartellaplot = grafico_name_Cartella2a_recSecElIP;
	      grafico_name = grafico_name_Cartella2a_recSecElIP;
	      asseX_name = asseX_name_Cartella2a_recSecElIP; 
	      asseY_name = asseY_name_Cartella2a_recSecElIP;
	      rebin = rebin_recSecElIP;
	      nminX = nminX_recSecElIP;
	      nmaxX = nmaxX_recSecElIP;
	      nminY = nminY_recSecElIP;
	      nmaxY = nmaxY_recSecElIP;}

else if(b==8){cartellaplot = grafico_name_Cartella2a_recSecElfBrem;
	      grafico_name = grafico_name_Cartella2a_recSecElfBrem;
	      asseX_name = asseX_name_Cartella2a_recSecElfBrem; 
	      asseY_name = asseY_name_Cartella2a_recSecElfBrem;
	      rebin = rebin_recSecElfBrem;
	      nminX = nminX_recSecElfBrem;
	      nmaxX = nmaxX_recSecElfBrem;
	      nminY = nminY_recSecElfBrem;
	      nmaxY = nmaxY_recSecElfBrem;}


TDirectory *Dir_3;
Dir_3 = Dir_2a->mkdir(cartellaplot.c_str());
Dir_3->cd();

	string QCD_EMEnriched_all_name = grafico_name.c_str();
	QCD_EMEnriched_all_name+=_RecoCutFlags[1].c_str();
	TH1D* histoQCD_EMEnriched_all_1 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_1->Scale(scale);
	histoQCD_EMEnriched_all_1->Rebin(rebin);
	QCD_EMEnriched_all_name+=_RecoCutFlags[2].c_str();
	TH1D* histoQCD_EMEnriched_all_12 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_12->Scale(scale);
	histoQCD_EMEnriched_all_12->Rebin(rebin);
	QCD_EMEnriched_all_name+=_RecoCutFlags[3].c_str();
	TH1D* histoQCD_EMEnriched_all_123 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_123->Scale(scale);
	histoQCD_EMEnriched_all_123->Rebin(rebin);
	QCD_EMEnriched_all_name+=_RecoCutFlags[4].c_str();
	TH1D* histoQCD_EMEnriched_all_1234 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_1234->Scale(scale);
	histoQCD_EMEnriched_all_1234->Rebin(rebin);
	QCD_EMEnriched_all_name+=_RecoCutFlags[5].c_str();
	TH1D* histoQCD_EMEnriched_all_12345 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_12345->Scale(scale);
	histoQCD_EMEnriched_all_12345->Rebin(rebin);
	QCD_EMEnriched_all_name+=_RecoCutFlags[6].c_str();
	TH1D* histoQCD_EMEnriched_all_123456 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_123456->Scale(scale);
	histoQCD_EMEnriched_all_123456->Rebin(rebin);
	
	string QCD_BCtoE_all_name = grafico_name.c_str();
	QCD_BCtoE_all_name+=_RecoCutFlags[1].c_str();
	TH1D* histoQCD_BCtoE_all_1 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_1->Scale(scale);
	histoQCD_BCtoE_all_1->Rebin(rebin);
	QCD_BCtoE_all_name+=_RecoCutFlags[2].c_str();
	TH1D* histoQCD_BCtoE_all_12 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_12->Scale(scale);
	histoQCD_BCtoE_all_12->Rebin(rebin);
	QCD_BCtoE_all_name+=_RecoCutFlags[3].c_str();
	TH1D* histoQCD_BCtoE_all_123 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_123->Scale(scale);
	histoQCD_BCtoE_all_123->Rebin(rebin);
	QCD_BCtoE_all_name+=_RecoCutFlags[4].c_str();
	TH1D* histoQCD_BCtoE_all_1234 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_1234->Scale(scale);
	histoQCD_BCtoE_all_1234->Rebin(rebin);
	QCD_BCtoE_all_name+=_RecoCutFlags[5].c_str();
	TH1D* histoQCD_BCtoE_all_12345 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_12345->Scale(scale);
	histoQCD_BCtoE_all_12345->Rebin(rebin);
	QCD_BCtoE_all_name+=_RecoCutFlags[6].c_str();
	TH1D* histoQCD_BCtoE_all_123456 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_123456->Scale(scale);
	histoQCD_BCtoE_all_123456->Rebin(rebin);
	
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

	string W_name = grafico_name.c_str();
	W_name+=_RecoCutFlags[1].c_str();
	TH1D* histoW_1 = (TH1D*) Cartella1_W_Cartella2a->Get(W_name.c_str());
	histoW_1->Scale(scale);
	histoW_1->Rebin(rebin);
	W_name+=_RecoCutFlags[2].c_str();
	TH1D* histoW_12 = (TH1D*) Cartella1_W_Cartella2a->Get(W_name.c_str());
	histoW_12->Scale(scale);
	histoW_12->Rebin(rebin);
	W_name+=_RecoCutFlags[3].c_str();
	TH1D* histoW_123 = (TH1D*) Cartella1_W_Cartella2a->Get(W_name.c_str());
	histoW_123->Scale(scale);
	histoW_123->Rebin(rebin);
	W_name+=_RecoCutFlags[4].c_str();
	TH1D* histoW_1234 = (TH1D*) Cartella1_W_Cartella2a->Get(W_name.c_str());
	histoW_1234->Scale(scale);
	histoW_1234->Rebin(rebin);
	W_name+=_RecoCutFlags[5].c_str();
	TH1D* histoW_12345 = (TH1D*) Cartella1_W_Cartella2a->Get(W_name.c_str());
	histoW_12345->Scale(scale);
	histoW_12345->Rebin(rebin);
	W_name+=_RecoCutFlags[6].c_str();
	TH1D* histoW_123456 = (TH1D*) Cartella1_W_Cartella2a->Get(W_name.c_str());
	histoW_123456->Scale(scale);
	histoW_123456->Rebin(rebin);
	

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
	
        histoQCD_EMEnriched_all_1->SetLineColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_1->SetLineWidth(2);
	histoQCD_EMEnriched_all_1->SetFillColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_1->SetFillStyle(style);
	histoQCD_EMEnriched_all_1->SetMarkerColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_1->SetTitle(c_name.c_str());
	histoQCD_EMEnriched_all_1->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histoQCD_EMEnriched_all_1->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histoQCD_EMEnriched_all_1->GetXaxis()->SetTitle(asseX_name.c_str());
	histoQCD_EMEnriched_all_1->GetYaxis()->SetTitle(asseY_name.c_str());
	histoQCD_EMEnriched_all_1->Draw("hist");
	histoQCD_BCtoE_all_1->SetLineColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_1->SetLineWidth(2);
	histoQCD_BCtoE_all_1->SetFillColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_1->SetFillStyle(style);
	histoQCD_BCtoE_all_1->SetMarkerColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_1->Draw("hist same");	
	histoTTbar_1->SetLineColor(col_TTbar);
	histoTTbar_1->SetLineWidth(2);
	histoTTbar_1->SetFillColor(col_TTbar);
	histoTTbar_1->SetFillStyle(style);
	histoTTbar_1->SetMarkerColor(col_TTbar);
	histoTTbar_1->Draw("hist same");
	histoW_1->SetLineColor(col_W);
	histoW_1->SetLineWidth(2);
	histoW_1->SetFillColor(col_W);
	histoW_1->SetFillStyle(style);
	histoW_1->SetMarkerColor(col_W);
	histoW_1->Draw("hist same");
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
	Leg_1->AddEntry(histoQCD_EMEnriched_all_1,Leg_QCD_EMEnriched_all.c_str());
   	Leg_1->AddEntry(histoQCD_BCtoE_all_1,Leg_QCD_BCtoE_all.c_str());
	Leg_1->AddEntry(histoTTbar_1,Leg_TTbar.c_str());
	Leg_1->AddEntry(histoW_1,Leg_W.c_str());
	Leg_1->AddEntry(histoZ_1,Leg_Z.c_str());
	Leg_1->AddEntry(histoData_1,Leg_Data.c_str());	
	   	
	Leg_1->Draw("LP");
	
	c_1->Write(c_name.c_str());
	c_1->Close();
	
	if(Tab_cut == "True"){
	double err_QCD_EMEnriched_all_1, err_QCD_BCtoE_all_1, err_TTbar_1, err_W_1, err_Z_1, err_Data_1;
	
	cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
	cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_1->IntegralAndError(0,-1,err_QCD_EMEnriched_all_1,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_1*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_1->IntegralAndError(0,-1,err_QCD_BCtoE_all_1,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_1*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_1->IntegralAndError(0,-1,err_TTbar_1,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_1*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_W_Int.c_str()<<((float)((int)(histoW_1->IntegralAndError(0,-1,err_W_1,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_W_Int_err.c_str()<<((float)((int)(err_W_1*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_1->IntegralAndError(0,-1,err_Z_1,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_1*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
	cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_1->IntegralAndError(0,-1,err_QCD_EMEnriched_all_1,"") + histoQCD_BCtoE_all_1->IntegralAndError(0,-1,err_QCD_BCtoE_all_1,"") + histoTTbar_1->IntegralAndError(0,-1,err_TTbar_1,"") + histoW_1->IntegralAndError(0,-1,err_W_1,"") + histoZ_1->IntegralAndError(0,-1,err_Z_1,""))*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_1),2)+pow((err_QCD_BCtoE_all_1),2) + pow((err_TTbar_1),2) + pow((err_W_1),2) + pow((err_Z_1),2))*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_1->IntegralAndError(0,-1,err_Data_1,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_1*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_end.c_str()<<endl;
	}

	if(_RecoCutFlags[2] != "_1"){

	
	TCanvas *c_12 = new TCanvas();

	if (log_scale == "True") {c_12->SetLogy();}

	c_name+=_RecoCutFlags[2].c_str();
	
        histoQCD_EMEnriched_all_12->SetLineColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_12->SetLineWidth(2);
	histoQCD_EMEnriched_all_12->SetFillColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_12->SetFillStyle(style);
	histoQCD_EMEnriched_all_12->SetMarkerColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_12->SetTitle(c_name.c_str());
	histoQCD_EMEnriched_all_12->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histoQCD_EMEnriched_all_12->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histoQCD_EMEnriched_all_12->GetXaxis()->SetTitle(asseX_name.c_str());
	histoQCD_EMEnriched_all_12->GetYaxis()->SetTitle(asseY_name.c_str());
	histoQCD_EMEnriched_all_12->Draw("hist");
	histoQCD_BCtoE_all_12->SetLineColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_12->SetLineWidth(2);
	histoQCD_BCtoE_all_12->SetFillColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_12->SetFillStyle(style);
	histoQCD_BCtoE_all_12->SetMarkerColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_12->Draw("hist same");	
	histoTTbar_12->SetLineColor(col_TTbar);
	histoTTbar_12->SetLineWidth(2);
	histoTTbar_12->SetFillColor(col_TTbar);
	histoTTbar_12->SetFillStyle(style);
	histoTTbar_12->SetMarkerColor(col_TTbar);
	histoTTbar_12->Draw("hist same");
	histoW_12->SetLineColor(col_W);
	histoW_12->SetLineWidth(2);
	histoW_12->SetFillColor(col_W);
	histoW_12->SetFillStyle(style);
	histoW_12->SetMarkerColor(col_W);
	histoW_12->Draw("hist same");
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
	Leg_12->AddEntry(histoQCD_EMEnriched_all_12,Leg_QCD_EMEnriched_all.c_str());
   	Leg_12->AddEntry(histoQCD_BCtoE_all_12,Leg_QCD_BCtoE_all.c_str());
	Leg_12->AddEntry(histoTTbar_12,Leg_TTbar.c_str());
	Leg_12->AddEntry(histoW_12,Leg_W.c_str());
	Leg_12->AddEntry(histoZ_12,Leg_Z.c_str());
	Leg_12->AddEntry(histoData_12,Leg_Data.c_str());	
	   	
	Leg_12->Draw("LP");
	
	c_12->Write(c_name.c_str());
	c_12->Close();
	
	if(Tab_cut == "True"){
	double err_QCD_EMEnriched_all_12, err_QCD_BCtoE_all_12, err_TTbar_12, err_W_12, err_Z_12, err_Data_12;
	
	cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
	cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_12->IntegralAndError(0,-1,err_QCD_EMEnriched_all_12,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_12*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_12->IntegralAndError(0,-1,err_QCD_BCtoE_all_12,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_12*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_12->IntegralAndError(0,-1,err_TTbar_12,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_12*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_W_Int.c_str()<<((float)((int)(histoW_12->IntegralAndError(0,-1,err_W_12,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_W_Int_err.c_str()<<((float)((int)(err_W_12*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_12->IntegralAndError(0,-1,err_Z_12,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_12*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
	cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_12->IntegralAndError(0,-1,err_QCD_EMEnriched_all_12,"") + histoQCD_BCtoE_all_12->IntegralAndError(0,-1,err_QCD_BCtoE_all_12,"") + histoTTbar_12->IntegralAndError(0,-1,err_TTbar_12,"") + histoW_12->IntegralAndError(0,-1,err_W_12,"") + histoZ_12->IntegralAndError(0,-1,err_Z_12,""))*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_12),2)+pow((err_QCD_BCtoE_all_12),2) + pow((err_TTbar_12),2) + pow((err_W_12),2) + pow((err_Z_12),2))*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_12->IntegralAndError(0,-1,err_Data_12,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_12*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_end.c_str()<<endl;
	}

	}
	
	if(_RecoCutFlags[3] != "_1"){

	
	TCanvas *c_123 = new TCanvas();

	if (log_scale == "True") {c_123->SetLogy();}
	
	c_name+=_RecoCutFlags[3].c_str();
	
        histoQCD_EMEnriched_all_123->SetLineColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_123->SetLineWidth(2);
	histoQCD_EMEnriched_all_123->SetFillColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_123->SetFillStyle(style);
	histoQCD_EMEnriched_all_123->SetMarkerColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_123->SetTitle(c_name.c_str());
	histoQCD_EMEnriched_all_123->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histoQCD_EMEnriched_all_123->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histoQCD_EMEnriched_all_123->GetXaxis()->SetTitle(asseX_name.c_str());
	histoQCD_EMEnriched_all_123->GetYaxis()->SetTitle(asseY_name.c_str());
	histoQCD_EMEnriched_all_123->Draw("hist");
	histoQCD_BCtoE_all_123->SetLineColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_123->SetLineWidth(2);
	histoQCD_BCtoE_all_123->SetFillColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_123->SetFillStyle(style);
	histoQCD_BCtoE_all_123->SetMarkerColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_123->Draw("hist same");	
	histoTTbar_123->SetLineColor(col_TTbar);
	histoTTbar_123->SetLineWidth(2);
	histoTTbar_123->SetFillColor(col_TTbar);
	histoTTbar_123->SetFillStyle(style);
	histoTTbar_123->SetMarkerColor(col_TTbar);
	histoTTbar_123->Draw("hist same");
	histoW_123->SetLineColor(col_W);
	histoW_123->SetLineWidth(2);
	histoW_123->SetFillColor(col_W);
	histoW_123->SetFillStyle(style);
	histoW_123->SetMarkerColor(col_W);
	histoW_123->Draw("hist same");
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
	Leg_123->AddEntry(histoQCD_EMEnriched_all_123,Leg_QCD_EMEnriched_all.c_str());
   	Leg_123->AddEntry(histoQCD_BCtoE_all_123,Leg_QCD_BCtoE_all.c_str());
	Leg_123->AddEntry(histoTTbar_123,Leg_TTbar.c_str());
	Leg_123->AddEntry(histoW_123,Leg_W.c_str());
	Leg_123->AddEntry(histoZ_123,Leg_Z.c_str());
	Leg_123->AddEntry(histoData_123,Leg_Data.c_str());	
	   	
	Leg_123->Draw("LP");
	
	c_123->Write(c_name.c_str());
	c_123->Close();
	
	if(Tab_cut == "True"){
	double err_QCD_EMEnriched_all_123, err_QCD_BCtoE_all_123, err_TTbar_123, err_W_123, err_Z_123, err_Data_123;
	
	cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
	cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_123->IntegralAndError(0,-1,err_QCD_EMEnriched_all_123,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_123*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_123->IntegralAndError(0,-1,err_QCD_BCtoE_all_123,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_123*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_123->IntegralAndError(0,-1,err_TTbar_123,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_123*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_W_Int.c_str()<<((float)((int)(histoW_123->IntegralAndError(0,-1,err_W_123,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_W_Int_err.c_str()<<((float)((int)(err_W_123*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_123->IntegralAndError(0,-1,err_Z_123,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_123*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
	cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_123->IntegralAndError(0,-1,err_QCD_EMEnriched_all_123,"") + histoQCD_BCtoE_all_123->IntegralAndError(0,-1,err_QCD_BCtoE_all_123,"") + histoTTbar_123->IntegralAndError(0,-1,err_TTbar_123,"") + histoW_123->IntegralAndError(0,-1,err_W_123,"") + histoZ_123->IntegralAndError(0,-1,err_Z_123,""))*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_123),2)+pow((err_QCD_BCtoE_all_123),2) + pow((err_TTbar_123),2) + pow((err_W_123),2) + pow((err_Z_123),2))*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_123->IntegralAndError(0,-1,err_Data_123,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_123*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_end.c_str()<<endl;
	}

	}
	

	if(_RecoCutFlags[4] != "_1"){


	TCanvas *c_1234 = new TCanvas();

	if (log_scale == "True") {c_1234->SetLogy();}
	
	c_name+=_RecoCutFlags[4].c_str();
	
        histoQCD_EMEnriched_all_1234->SetLineColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_1234->SetLineWidth(2);
	histoQCD_EMEnriched_all_1234->SetFillColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_1234->SetFillStyle(style);
	histoQCD_EMEnriched_all_1234->SetMarkerColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_1234->SetTitle(c_name.c_str());
	histoQCD_EMEnriched_all_1234->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histoQCD_EMEnriched_all_1234->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histoQCD_EMEnriched_all_1234->GetXaxis()->SetTitle(asseX_name.c_str());
	histoQCD_EMEnriched_all_1234->GetYaxis()->SetTitle(asseY_name.c_str());
	histoQCD_EMEnriched_all_1234->Draw("hist");
	histoQCD_BCtoE_all_1234->SetLineColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_1234->SetLineWidth(2);
	histoQCD_BCtoE_all_1234->SetFillColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_1234->SetFillStyle(style);
	histoQCD_BCtoE_all_1234->SetMarkerColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_1234->Draw("hist same");	
	histoTTbar_1234->SetLineColor(col_TTbar);
	histoTTbar_1234->SetLineWidth(2);
	histoTTbar_1234->SetFillColor(col_TTbar);
	histoTTbar_1234->SetFillStyle(style);
	histoTTbar_1234->SetMarkerColor(col_TTbar);
	histoTTbar_1234->Draw("hist same");
	histoW_1234->SetLineColor(col_W);
	histoW_1234->SetLineWidth(2);
	histoW_1234->SetFillColor(col_W);
	histoW_1234->SetFillStyle(style);
	histoW_1234->SetMarkerColor(col_W);
	histoW_1234->Draw("hist same");
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
	Leg_1234->AddEntry(histoQCD_EMEnriched_all_1234,Leg_QCD_EMEnriched_all.c_str());
   	Leg_1234->AddEntry(histoQCD_BCtoE_all_1234,Leg_QCD_BCtoE_all.c_str());
	Leg_1234->AddEntry(histoTTbar_1234,Leg_TTbar.c_str());
	Leg_1234->AddEntry(histoW_1234,Leg_W.c_str());
	Leg_1234->AddEntry(histoZ_1234,Leg_Z.c_str());
	Leg_1234->AddEntry(histoData_1234,Leg_Data.c_str());	
	   	
	Leg_1234->Draw("LP");
	
	c_1234->Write(c_name.c_str());
	c_1234->Close();
	
	if(Tab_cut == "True"){
	double err_QCD_EMEnriched_all_1234, err_QCD_BCtoE_all_1234, err_TTbar_1234, err_W_1234, err_Z_1234, err_Data_1234;
	
	cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
	cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_1234->IntegralAndError(0,-1,err_QCD_EMEnriched_all_1234,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_1234*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_1234->IntegralAndError(0,-1,err_QCD_BCtoE_all_1234,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_1234*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_1234->IntegralAndError(0,-1,err_TTbar_1234,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_1234*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_W_Int.c_str()<<((float)((int)(histoW_1234->IntegralAndError(0,-1,err_W_1234,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_W_Int_err.c_str()<<((float)((int)(err_W_1234*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_1234->IntegralAndError(0,-1,err_Z_1234,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_1234*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
	cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_1234->IntegralAndError(0,-1,err_QCD_EMEnriched_all_1234,"") + histoQCD_BCtoE_all_1234->IntegralAndError(0,-1,err_QCD_BCtoE_all_1234,"") + histoTTbar_1234->IntegralAndError(0,-1,err_TTbar_1234,"") + histoW_1234->IntegralAndError(0,-1,err_W_1234,"") + histoZ_1234->IntegralAndError(0,-1,err_Z_1234,""))*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_1234),2)+pow((err_QCD_BCtoE_all_1234),2) + pow((err_TTbar_1234),2) + pow((err_W_1234),2) + pow((err_Z_1234),2))*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_1234->IntegralAndError(0,-1,err_Data_1234,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_1234*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_end.c_str()<<endl;
	}	

	}
	
	

	if(_RecoCutFlags[5] != "_1"){
	
	
	TCanvas *c_12345 = new TCanvas();

	if (log_scale == "True") {c_12345->SetLogy();}
	
	c_name+=_RecoCutFlags[5].c_str();
	
        histoQCD_EMEnriched_all_12345->SetLineColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_12345->SetLineWidth(2);
	histoQCD_EMEnriched_all_12345->SetFillColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_12345->SetFillStyle(style);
	histoQCD_EMEnriched_all_12345->SetMarkerColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_12345->SetTitle(c_name.c_str());
	histoQCD_EMEnriched_all_12345->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histoQCD_EMEnriched_all_12345->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histoQCD_EMEnriched_all_12345->GetXaxis()->SetTitle(asseX_name.c_str());
	histoQCD_EMEnriched_all_12345->GetYaxis()->SetTitle(asseY_name.c_str());
	histoQCD_EMEnriched_all_12345->Draw("hist");
	histoQCD_BCtoE_all_12345->SetLineColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_12345->SetLineWidth(2);
	histoQCD_BCtoE_all_12345->SetFillColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_12345->SetFillStyle(style);
	histoQCD_BCtoE_all_12345->SetMarkerColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_12345->Draw("hist same");	
	histoTTbar_12345->SetLineColor(col_TTbar);
	histoTTbar_12345->SetLineWidth(2);
	histoTTbar_12345->SetFillColor(col_TTbar);
	histoTTbar_12345->SetFillStyle(style);
	histoTTbar_12345->SetMarkerColor(col_TTbar);
	histoTTbar_12345->Draw("hist same");
	histoW_12345->SetLineColor(col_W);
	histoW_12345->SetLineWidth(2);
	histoW_12345->SetFillColor(col_W);
	histoW_12345->SetFillStyle(style);
	histoW_12345->SetMarkerColor(col_W);
	histoW_12345->Draw("hist same");
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
	Leg_12345->AddEntry(histoQCD_EMEnriched_all_12345,Leg_QCD_EMEnriched_all.c_str());
   	Leg_12345->AddEntry(histoQCD_BCtoE_all_12345,Leg_QCD_BCtoE_all.c_str());
	Leg_12345->AddEntry(histoTTbar_12345,Leg_TTbar.c_str());
	Leg_12345->AddEntry(histoW_12345,Leg_W.c_str());
	Leg_12345->AddEntry(histoZ_12345,Leg_Z.c_str());
	Leg_12345->AddEntry(histoData_12345,Leg_Data.c_str());	
	   	
	Leg_12345->Draw("LP");
	
	c_12345->Write(c_name.c_str());
	c_12345->Close();
	
	if(Tab_cut == "True"){
	double err_QCD_EMEnriched_all_12345, err_QCD_BCtoE_all_12345, err_TTbar_12345, err_W_12345, err_Z_12345, err_Data_12345;
	
	cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
	cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_12345->IntegralAndError(0,-1,err_QCD_EMEnriched_all_12345,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_12345*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_12345->IntegralAndError(0,-1,err_QCD_BCtoE_all_12345,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_12345*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_12345->IntegralAndError(0,-1,err_TTbar_12345,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_12345*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_W_Int.c_str()<<((float)((int)(histoW_12345->IntegralAndError(0,-1,err_W_12345,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_W_Int_err.c_str()<<((float)((int)(err_W_12345*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_12345->IntegralAndError(0,-1,err_Z_12345,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_12345*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
	cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_12345->IntegralAndError(0,-1,err_QCD_EMEnriched_all_12345,"") + histoQCD_BCtoE_all_12345->IntegralAndError(0,-1,err_QCD_BCtoE_all_12345,"") + histoTTbar_12345->IntegralAndError(0,-1,err_TTbar_12345,"") + histoW_12345->IntegralAndError(0,-1,err_W_12345,"") + histoZ_12345->IntegralAndError(0,-1,err_Z_12345,""))*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_12345),2)+pow((err_QCD_BCtoE_all_12345),2) + pow((err_TTbar_12345),2) + pow((err_W_12345),2) + pow((err_Z_12345),2))*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_12345->IntegralAndError(0,-1,err_Data_12345,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_12345*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_end.c_str()<<endl;
	}	

	}
	


	if(_RecoCutFlags[6] != "_1"){

	
	TCanvas *c_123456 = new TCanvas();

	if (log_scale == "True") {c_123456->SetLogy();}
	
	c_name+=_RecoCutFlags[6].c_str();
	
        histoQCD_EMEnriched_all_123456->SetLineColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_123456->SetLineWidth(2);
	histoQCD_EMEnriched_all_123456->SetFillColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_123456->SetFillStyle(style);
	histoQCD_EMEnriched_all_123456->SetMarkerColor(col_QCD_EMEnriched_all);
	histoQCD_EMEnriched_all_123456->SetTitle(c_name.c_str());
	histoQCD_EMEnriched_all_123456->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histoQCD_EMEnriched_all_123456->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histoQCD_EMEnriched_all_123456->GetXaxis()->SetTitle(asseX_name.c_str());
	histoQCD_EMEnriched_all_123456->GetYaxis()->SetTitle(asseY_name.c_str());
	histoQCD_EMEnriched_all_123456->Draw("hist");
	histoQCD_BCtoE_all_123456->SetLineColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_123456->SetLineWidth(2);
	histoQCD_BCtoE_all_123456->SetFillColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_123456->SetFillStyle(style);
	histoQCD_BCtoE_all_123456->SetMarkerColor(col_QCD_BCtoE_all);
	histoQCD_BCtoE_all_123456->Draw("hist same");	
	histoTTbar_123456->SetLineColor(col_TTbar);
	histoTTbar_123456->SetLineWidth(2);
	histoTTbar_123456->SetFillColor(col_TTbar);
	histoTTbar_123456->SetFillStyle(style);
	histoTTbar_123456->SetMarkerColor(col_TTbar);
	histoTTbar_123456->Draw("hist same");
	histoW_123456->SetLineColor(col_W);
	histoW_123456->SetLineWidth(2);
	histoW_123456->SetFillColor(col_W);
	histoW_123456->SetFillStyle(style);
	histoW_123456->SetMarkerColor(col_W);
	histoW_123456->Draw("hist same");
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
	Leg_123456->AddEntry(histoQCD_EMEnriched_all_123456,Leg_QCD_EMEnriched_all.c_str());
   	Leg_123456->AddEntry(histoQCD_BCtoE_all_123456,Leg_QCD_BCtoE_all.c_str());
	Leg_123456->AddEntry(histoTTbar_123456,Leg_TTbar.c_str());
	Leg_123456->AddEntry(histoW_123456,Leg_W.c_str());
	Leg_123456->AddEntry(histoZ_123456,Leg_Z.c_str());
	Leg_123456->AddEntry(histoData_123456,Leg_Data.c_str());	
	   	
	Leg_123456->Draw("LP");
	
	c_123456->Write(c_name.c_str());
	c_123456->Close();
	
	if(Tab_cut == "True"){
	double err_QCD_EMEnriched_all_123456, err_QCD_BCtoE_all_123456, err_TTbar_123456, err_W_123456, err_Z_123456, err_Data_123456;
	
	cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
	cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_123456->IntegralAndError(0,-1,err_QCD_EMEnriched_all_123456,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_123456*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_123456->IntegralAndError(0,-1,err_QCD_BCtoE_all_123456,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_123456*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_123456->IntegralAndError(0,-1,err_TTbar_123456,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_123456*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_W_Int.c_str()<<((float)((int)(histoW_123456->IntegralAndError(0,-1,err_W_123456,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_W_Int_err.c_str()<<((float)((int)(err_W_123456*cut_decimal)))/cut_decimal<<endl<<endl;
	cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_123456->IntegralAndError(0,-1,err_Z_123456,"")*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_123456*cut_decimal)))/cut_decimal<<endl;
	cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
	cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_123456->IntegralAndError(0,-1,err_QCD_EMEnriched_all_123456,"") + histoQCD_BCtoE_all_123456->IntegralAndError(0,-1,err_QCD_BCtoE_all_123456,"") + histoTTbar_123456->IntegralAndError(0,-1,err_TTbar_123456,"") + histoW_123456->IntegralAndError(0,-1,err_W_123456,"") + histoZ_123456->IntegralAndError(0,-1,err_Z_123456,""))*cut_decimal)))/cut_decimal<<endl;
	cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_123456),2)+pow((err_QCD_BCtoE_all_123456),2) + pow((err_TTbar_123456),2) + pow((err_W_123456),2) + pow((err_Z_123456),2))*cut_decimal)))/cut_decimal<<endl<<endl;
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
	

	Stackd_1->Add(histoQCD_EMEnriched_all_1);
	Stackd_1->Add(histoQCD_BCtoE_all_1);	
	Stackd_1->Add(histoTTbar_1);
	Stackd_1->Add(histoW_1);
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
	StackLeg_1->AddEntry(histoQCD_EMEnriched_all_1,Leg_QCD_EMEnriched_all.c_str());
   	StackLeg_1->AddEntry(histoQCD_BCtoE_all_1,Leg_QCD_BCtoE_all.c_str());
	StackLeg_1->AddEntry(histoTTbar_1,Leg_TTbar.c_str());
	StackLeg_1->AddEntry(histoW_1,Leg_W.c_str());
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

	Stackd_12->Add(histoQCD_EMEnriched_all_12);
	Stackd_12->Add(histoQCD_BCtoE_all_12);	
	Stackd_12->Add(histoTTbar_12);
	Stackd_12->Add(histoW_12);
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
	StackLeg_12->AddEntry(histoQCD_EMEnriched_all_12,Leg_QCD_EMEnriched_all.c_str());
   	StackLeg_12->AddEntry(histoQCD_BCtoE_all_12,Leg_QCD_BCtoE_all.c_str());
	StackLeg_12->AddEntry(histoTTbar_12,Leg_TTbar.c_str());
	StackLeg_12->AddEntry(histoW_12,Leg_W.c_str());
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

	Stackd_123->Add(histoQCD_EMEnriched_all_123);
	Stackd_123->Add(histoQCD_BCtoE_all_123);	
	Stackd_123->Add(histoTTbar_123);
	Stackd_123->Add(histoW_123);
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
	StackLeg_123->AddEntry(histoQCD_EMEnriched_all_123,Leg_QCD_EMEnriched_all.c_str());
   	StackLeg_123->AddEntry(histoQCD_BCtoE_all_123,Leg_QCD_BCtoE_all.c_str());
	StackLeg_123->AddEntry(histoTTbar_123,Leg_TTbar.c_str());
	StackLeg_123->AddEntry(histoW_123,Leg_W.c_str());
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

	Stackd_1234->Add(histoQCD_EMEnriched_all_1234);
	Stackd_1234->Add(histoQCD_BCtoE_all_1234);	
	Stackd_1234->Add(histoTTbar_1234);
	Stackd_1234->Add(histoW_1234);
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
	StackLeg_1234->AddEntry(histoQCD_EMEnriched_all_1234,Leg_QCD_EMEnriched_all.c_str());
   	StackLeg_1234->AddEntry(histoQCD_BCtoE_all_1234,Leg_QCD_BCtoE_all.c_str());
	StackLeg_1234->AddEntry(histoTTbar_1234,Leg_TTbar.c_str());
	StackLeg_1234->AddEntry(histoW_1234,Leg_W.c_str());
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

	Stackd_12345->Add(histoQCD_EMEnriched_all_12345);
	Stackd_12345->Add(histoQCD_BCtoE_all_12345);	
	Stackd_12345->Add(histoTTbar_12345);
	Stackd_12345->Add(histoW_12345);
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
	StackLeg_12345->AddEntry(histoQCD_EMEnriched_all_12345,Leg_QCD_EMEnriched_all.c_str());
   	StackLeg_12345->AddEntry(histoQCD_BCtoE_all_12345,Leg_QCD_BCtoE_all.c_str());
	StackLeg_12345->AddEntry(histoTTbar_12345,Leg_TTbar.c_str());
	StackLeg_12345->AddEntry(histoW_12345,Leg_W.c_str());
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

	Stackd_123456->Add(histoQCD_EMEnriched_all_123456);
	Stackd_123456->Add(histoQCD_BCtoE_all_123456);	
	Stackd_123456->Add(histoTTbar_123456);
	Stackd_123456->Add(histoW_123456);
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
	StackLeg_123456->AddEntry(histoQCD_EMEnriched_all_123456,Leg_QCD_EMEnriched_all.c_str());
   	StackLeg_123456->AddEntry(histoQCD_BCtoE_all_123456,Leg_QCD_BCtoE_all.c_str());
	StackLeg_123456->AddEntry(histoTTbar_123456,Leg_TTbar.c_str());
	StackLeg_123456->AddEntry(histoW_123456,Leg_W.c_str());
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
