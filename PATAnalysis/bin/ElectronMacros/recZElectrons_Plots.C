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

//Selections: "SYM" = Symmetric selections; "ASYM" = Asymmetric selections

void recZElectrons_Plots(string selections){

string Analysis_Note = "False";
if (Analysis_Note == "False") gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);

string log_scale = "True";

string Tab_cut = "True";
        
                
	//Background MC
	TFile* QCD_EMEnriched_all_TF = TFile::Open("MC_Fall10_387/JetPt15/QCD_EMEnriched_Pythia_All.root");
	TFile* QCD_BCtoE_all_TF = TFile::Open("MC_Fall10_387/JetPt15/QCD_BCtoE_Pythia_All.root");
	TFile* TTbar_TF = TFile::Open("MC_Fall10_387/JetPt15/TT_Pythia.root");
	TFile* Wlnu_TF = TFile::Open("MC_Fall10_387/JetPt15/Wlnu_Madgraph.root");
	TFile* WWEE_TF = TFile::Open("MC_Fall10_387/JetPt15/WWEE_Pythia.root");
	TFile* WZEE_TF = TFile::Open("MC_Fall10_387/JetPt15/WZEE_Pythia.root");
	TFile* ZZEE_TF = TFile::Open("MC_Fall10_387/JetPt15/ZZEE_Pythia.root");
	
	//Signal MC
	TFile *Z_TF = TFile::Open("MC_Winter10_399/Z_Madgraph_L1FastJet_JetPt15_399.root");
	
	//Data
	TFile *Data_TF = TFile::Open("DATA_Dec22ReReco/JetPt15/Data_RUN2010A-B_L1FastJet_399.root");
	
	//Output
	string out = "recZElectronsPlots_L1FastJet_JetPt15_399";        
	string output = out;
	output+=".root";
	TFile* outplots = new TFile(output.c_str(), "RECREATE");
	
	//Normalization factor
	double iniLumi = 50.; //pb-1
	double targetLumi = 36.176; //pb-1
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
	//EiD variables
	int rebin_DeltaEtaIn = 1;	
	int rebin_DeltaPhiIn = 1;
	int rebin_HoverE = 1;
	int rebin_SigmaIEtaIEta = 1;
	//Iso variables
	int rebin_LeadRelIso_PreIso = 1;	
	int rebin_SecRelIso_PreIso = 1;
	

	//colors	
	int col_Z = 800;
	int col_QCD_EMEnriched_all= 616+3;
	int col_QCD_BCtoE_all= 800+7;
	int col_TTbar= 632;
	int col_Wlnu= 800+3;
	int col_WWEE= 800+4;
	int col_WZEE= 800+5;
	int col_ZZEE= 800+6;
	int col_Data= 1;

	//FillStyle	
	int style = 1001;

	//SetRangeUser
	//LeadElPt
	double nminX_recLeadElPt = 0.0; 
	double nmaxX_recLeadElPt = 200.0;
	double nminY_recLeadElPt = 0.1; 
	double nmaxY_recLeadElPt = 3000.0;
	//LeadElEta
	double nminX_recLeadElEta = -10.0; 
	double nmaxX_recLeadElEta = 10.0;
	double nminY_recLeadElEta = 0.1; 
	double nmaxY_recLeadElEta = 3000.0;
	//LeadElIP
	double nminX_recLeadElIP = 0.0; 
	double nmaxX_recLeadElIP = 1.0;
	double nminY_recLeadElIP = 10; 
	double nmaxY_recLeadElIP = 25000.0;
	//LeadElfBrem	
	double nminX_recLeadElfBrem = 0.0; 
	double nmaxX_recLeadElfBrem = 4.0;
	double nminY_recLeadElfBrem = 0.1; 
	double nmaxY_recLeadElfBrem = 3000.0;
	//SecElPt
	double nminX_recSecElPt= 0.0; 
	double nmaxX_recSecElPt = 200.0;
	double nminY_recSecElPt = 0.1; 
	double nmaxY_recSecElPt = 3000.0;
	//SecElEta
	double nminX_recSecElEta = -10.0; 
	double nmaxX_recSecElEta = 10.0;
	double nminY_recSecElEta = 0.1; 
	double nmaxY_recSecElEta = 25000.0;
	//SecElIP
	double nminX_recSecElIP = 0.0; 
	double nmaxX_recSecElIP = 1.0;
	double nminY_recSecElIP = 10; 
	double nmaxY_recSecElIP = 25000.0;
	//SecElfBrem	
	double nminX_recSecElfBrem = 0.0; 
	double nmaxX_recSecElfBrem = 4.0;
	double nminY_recSecElfBrem = 0.1; 
	double nmaxY_recSecElfBrem = 3000.0;	
	//DeltaEtaIn
	double nminX_DeltaEtaIn = -0.1; 
	double nmaxX_DeltaEtaIn = 0.1;
	double nminY_DeltaEtaIn = 0.1; 
	double nmaxY_DeltaEtaIn = 10000.0;
	//DeltaPhiIn
	double nminX_DeltaPhiIn = -0.30; 
	double nmaxX_DeltaPhiIn = 0.30;
	double nminY_DeltaPhiIn = 0.1; 
	double nmaxY_DeltaPhiIn = 10000.0;
	//HoverE
	double nminX_HoverE = 0.0; 
	double nmaxX_HoverE = 0.1;
	double nminY_HoverE = 0.1; 
	double nmaxY_HoverE = 25000.0;
	//SigmaIEtaIEta
	double nminX_SigmaIEtaIEta = 0.0; 
	double nmaxX_SigmaIEtaIEta = 0.035;
	double nminY_SigmaIEtaIEta = 0.1; 
	double nmaxY_SigmaIEtaIEta = 10000.0;	
	//CombRelIso
	double nminX_LeadRelIso_PreIso = 0.0; 
	double nmaxX_LeadRelIso_PreIso = 0.5;
	double nminY_LeadRelIso_PreIso = 10; 
	double nmaxY_LeadRelIso_PreIso = 2700.0;
	//CombRelIso
	double nminX_SecRelIso_PreIso = 0.0; 
	double nmaxX_SecRelIso_PreIso = 0.5;
	double nminY_SecRelIso_PreIso = 10; 
	double nmaxY_SecRelIso_PreIso = 2700.0;
	
	
	//Legenda
	string Leg_QCD_EMEnriched_all = "QCD EM Enriched";
	string Leg_QCD_BCtoE_all = "QCD b,c #rightarrow e";
	string Leg_TTbar = "t #bar{t}+jets";
	string Leg_Wlnu = "W+lnu";
	string Leg_WWEE = "WW #rightarrow ee";
	string Leg_WZEE = "WZ #rightarrow ee";
	string Leg_ZZEE = "ZZ #rightarrow ee";
	string Leg_Z = "Z+jets";
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
	string Tabcut_QCD_EMEnriched_all_Int = "QCD EM Enriched Integral = ";
	string Tabcut_QCD_EMEnriched_all_Int_err = "QCD EM Enriched Integral error = ";
	string Tabcut_QCD_BCtoE_all_Int = "QCD QCD b,c-> e Integral = ";
	string Tabcut_QCD_BCtoE_all_Int_err = "QCD QCD b,c-> e Integral error = ";
	string Tabcut_TTbar_Int = "TTbar+jets Integral = ";
	string Tabcut_TTbar_Int_err = "TTbar+jets Integral error = ";
	string Tabcut_Wlnu_Int = "W -> lnu Integral = ";
	string Tabcut_Wlnu_Int_err = "W -> lnu Integral error = ";
	string Tabcut_WWEE_Int = "WW -> ee Integral = ";
	string Tabcut_WWEE_Int_err = "WW -> ee Integral error = ";
	string Tabcut_ZZEE_Int = "ZZ -> ee Integral = ";
	string Tabcut_ZZEE_Int_err = "ZZ -> ee Integral error = ";
	string Tabcut_WZEE_Int = "WZ -> ee Integral = ";
	string Tabcut_WZEE_Int_err = "WZ -> ee Integral error = ";	
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
	
	if(!Wlnu_TF){
		cout<<"Error! Wlnu file doesn't exist!"<<endl;
		return;
	}
	
	if(!WWEE_TF){
		cout<<"Error! WWEE file doesn't exist!"<<endl;
		return;
	}
	
	if(!ZZEE_TF){
		cout<<"Error! ZZEE file doesn't exist!"<<endl;
		return;
	}
	
	if(!WZEE_TF){
		cout<<"Error! WZEE file doesn't exist!"<<endl;
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
string grafico_name_Cartella2a_DeltaEtaIn_PreEiD = "DeltaEtaIn_PreEiD";
string grafico_name_Cartella2a_DeltaPhiIn_PreEiD = "DeltaPhiIn_PreEiD";
string grafico_name_Cartella2a_HoverE_PreEiD = "HoverE_PreEiD";
string grafico_name_Cartella2a_SigmaIEtaIEta_PreEiD = "SigmaIEtaIEta_PreEiD";
string grafico_name_Cartella2a_DeltaEtaIn_EiD = "DeltaEtaIn_EiD";
string grafico_name_Cartella2a_DeltaPhiIn_EiD = "DeltaPhiIn_EiD";
string grafico_name_Cartella2a_HoverE_EiD = "HoverE_EiD";
string grafico_name_Cartella2a_SigmaIEtaIEta_EiD = "SigmaIEtaIEta_EiD";
string grafico_name_Cartella2a_LeadRelIso_PreIso = "recRelIsoLead_PreIso";
string grafico_name_Cartella2a_SecRelIso_PreIso = "recRelIsoSec_PreIso";

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
string asseX_name_Cartella2a_DeltaEtaIn_PreEiD = "Reconstructed Electron Delta Eta - Pre EiD cut";
string asseY_name_Cartella2a_DeltaEtaIn_PreEiD = Form("Events/(%.3f)", 0.002*rebin_DeltaEtaIn); //bins da 0.002
string asseX_name_Cartella2a_DeltaPhiIn_PreEiD = "Reconstructed Electron Delta Phi - Pre EiD cut";
string asseY_name_Cartella2a_DeltaPhiIn_PreEiD = Form("Events/(%.3f)", 0.008*rebin_DeltaPhiIn); //bins da 0.008
string asseX_name_Cartella2a_HoverE_PreEiD = "Reconstructed Electron H over E - Pre EiD cut";
string asseY_name_Cartella2a_HoverE_PreEiD = Form("Events/(%.3f)", 0.001*rebin_HoverE); //bins da 0.001
string asseX_name_Cartella2a_SigmaIEtaIEta_PreEiD = "Reconstructed Electron SigmaIEtaIEta - Pre EiD cut";
string asseY_name_Cartella2a_SigmaIEtaIEta_PreEiD = Form("Events/(%.3f)", 0.001*rebin_SigmaIEtaIEta); //bins da 0.001
string asseX_name_Cartella2a_DeltaEtaIn_EiD = "Reconstructed Electron Delta Eta - EiD cut";
string asseY_name_Cartella2a_DeltaEtaIn_EiD = Form("Events/(%.3f)", 0.002*rebin_DeltaEtaIn); //bins da 0.002
string asseX_name_Cartella2a_DeltaPhiIn_EiD = "Reconstructed Electron Delta Phi - EiD cut";
string asseY_name_Cartella2a_DeltaPhiIn_EiD = Form("Events/(%.3f)", 0.008*rebin_DeltaPhiIn); //bins da 0.008
string asseX_name_Cartella2a_HoverE_EiD = "Reconstructed Electron H over E - EiD cut";
string asseY_name_Cartella2a_HoverE_EiD = Form("Events/(%.3f)", 0.001*rebin_HoverE); //bins da 0.001
string asseX_name_Cartella2a_SigmaIEtaIEta_EiD = "Reconstructed Electron SigmaIEtaIEta - EiD cut";
string asseY_name_Cartella2a_SigmaIEtaIEta_EiD = Form("Events/(%.3f)", 0.001*rebin_SigmaIEtaIEta); //bins da 0.001
string asseX_name_Cartella2a_LeadRelIso_PreIso = "Reconstructed Lead Electron Rel. Isolation - Pre Iso cut";
string asseY_name_Cartella2a_LeadRelIso_PreIso = Form("Events/(%.2f)", 0.01*rebin_SigmaIEtaIEta); //bins da 0.01
string asseX_name_Cartella2a_SecRelIso_PreIso = "Reconstructed Second Electron Rel. Isolation - Pre Iso cut";
string asseY_name_Cartella2a_SecRelIso_PreIso = Form("Events/(%.2f)", 0.01*rebin_SigmaIEtaIEta); //bins da 0.01

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
	
	TDirectoryFile *Cartella1_TTbar = (TDirectoryFile*) TTbar_TF->Get(cartella1.c_str());
	TDirectoryFile *Cartella1_TTbar_Cartella2a = (TDirectoryFile*) Cartella1_TTbar->Get(cartella2a.c_str());
	
	TDirectoryFile *Cartella1_Wlnu = (TDirectoryFile*) Wlnu_TF->Get(cartella1.c_str());
	TDirectoryFile *Cartella1_Wlnu_Cartella2a = (TDirectoryFile*) Cartella1_Wlnu->Get(cartella2a.c_str());
	
	TDirectoryFile *Cartella1_WWEE = (TDirectoryFile*) WWEE_TF->Get(cartella1.c_str());
	TDirectoryFile *Cartella1_WWEE_Cartella2a = (TDirectoryFile*) Cartella1_WWEE->Get(cartella2a.c_str());
	
	TDirectoryFile *Cartella1_WZEE = (TDirectoryFile*) WZEE_TF->Get(cartella1.c_str());
	TDirectoryFile *Cartella1_WZEE_Cartella2a = (TDirectoryFile*) Cartella1_WZEE->Get(cartella2a.c_str());
	
	TDirectoryFile *Cartella1_ZZEE = (TDirectoryFile*) ZZEE_TF->Get(cartella1.c_str());
	TDirectoryFile *Cartella1_ZZEE_Cartella2a = (TDirectoryFile*) Cartella1_ZZEE->Get(cartella2a.c_str());
	
	TDirectoryFile *Cartella1_Z = (TDirectoryFile*) Z_TF->Get(cartella1.c_str());
	TDirectoryFile *Cartella1_Z_Cartella2a = (TDirectoryFile*) Cartella1_Z->Get(cartella2a.c_str());
	
	TDirectoryFile *Cartella1_Data = (TDirectoryFile*) Data_TF->Get(cartella1.c_str());
	TDirectoryFile *Cartella1_Data_Cartella2a = (TDirectoryFile*) Cartella1_Data->Get(cartella2a.c_str());

//----------------------------------------- Grafici cartella recZElectrons_Plots

for(int b=1; b<19; b++){
	
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

else if(b==9){cartellaplot = grafico_name_Cartella2a_DeltaEtaIn_PreEiD;
	grafico_name = grafico_name_Cartella2a_DeltaEtaIn_PreEiD;
	asseX_name = asseX_name_Cartella2a_DeltaEtaIn_PreEiD; 
	asseY_name = asseY_name_Cartella2a_DeltaEtaIn_PreEiD;
	rebin = rebin_DeltaEtaIn;
	nminX = nminX_DeltaEtaIn;
	nmaxX = nmaxX_DeltaEtaIn;
	nminY = nminY_DeltaEtaIn;
	nmaxY = nmaxY_DeltaEtaIn;}
	
else if(b==10){cartellaplot = grafico_name_Cartella2a_DeltaPhiIn_PreEiD;
	grafico_name = grafico_name_Cartella2a_DeltaPhiIn_PreEiD;
	asseX_name = asseX_name_Cartella2a_DeltaPhiIn_PreEiD; 
	asseY_name = asseY_name_Cartella2a_DeltaPhiIn_PreEiD;
	rebin = rebin_DeltaPhiIn;
	nminX = nminX_DeltaPhiIn;
	nmaxX = nmaxX_DeltaPhiIn;
	nminY = nminY_DeltaPhiIn;
	nmaxY = nmaxY_DeltaPhiIn;}
	
else if(b==11){cartellaplot = grafico_name_Cartella2a_HoverE_PreEiD;
	grafico_name = grafico_name_Cartella2a_HoverE_PreEiD;
	asseX_name = asseX_name_Cartella2a_HoverE_PreEiD; 
	asseY_name = asseY_name_Cartella2a_HoverE_PreEiD;
	rebin = rebin_HoverE;
	nminX = nminX_HoverE;
	nmaxX = nmaxX_HoverE;
	nminY = nminY_HoverE;
	nmaxY = nmaxY_HoverE;}
	
else if(b==12){cartellaplot = grafico_name_Cartella2a_SigmaIEtaIEta_PreEiD;
	grafico_name = grafico_name_Cartella2a_SigmaIEtaIEta_PreEiD;
	asseX_name = asseX_name_Cartella2a_SigmaIEtaIEta_PreEiD; 
	asseY_name = asseY_name_Cartella2a_SigmaIEtaIEta_PreEiD;
	rebin = rebin_SigmaIEtaIEta;
	nminX = nminX_SigmaIEtaIEta;
	nmaxX = nmaxX_SigmaIEtaIEta;
	nminY = nminY_SigmaIEtaIEta;
	nmaxY = nmaxY_SigmaIEtaIEta;}
	
else if(b==13){cartellaplot = grafico_name_Cartella2a_DeltaEtaIn_EiD;
	grafico_name = grafico_name_Cartella2a_DeltaEtaIn_EiD;
	asseX_name = asseX_name_Cartella2a_DeltaEtaIn_EiD; 
	asseY_name = asseY_name_Cartella2a_DeltaEtaIn_EiD;
	rebin = rebin_DeltaEtaIn;
	nminX = nminX_DeltaEtaIn;
	nmaxX = nmaxX_DeltaEtaIn;
	nminY = nminY_DeltaEtaIn;
	nmaxY = nmaxY_DeltaEtaIn;}
	
else if(b==14){cartellaplot = grafico_name_Cartella2a_DeltaPhiIn_EiD;
	grafico_name = grafico_name_Cartella2a_DeltaPhiIn_EiD;
	asseX_name = asseX_name_Cartella2a_DeltaPhiIn_EiD; 
	asseY_name = asseY_name_Cartella2a_DeltaPhiIn_EiD;
	rebin = rebin_DeltaPhiIn;
	nminX = nminX_DeltaPhiIn;
	nmaxX = nmaxX_DeltaPhiIn;
	nminY = nminY_DeltaPhiIn;
	nmaxY = nmaxY_DeltaPhiIn;}
	
else if(b==15){cartellaplot = grafico_name_Cartella2a_HoverE_EiD;
	grafico_name = grafico_name_Cartella2a_HoverE_EiD;
	asseX_name = asseX_name_Cartella2a_HoverE_EiD; 
	asseY_name = asseY_name_Cartella2a_HoverE_EiD;
	rebin = rebin_HoverE;
	nminX = nminX_HoverE;
	nmaxX = nmaxX_HoverE;
	nminY = nminY_HoverE;
	nmaxY = nmaxY_HoverE;}
	
else if(b==16){cartellaplot = grafico_name_Cartella2a_SigmaIEtaIEta_EiD;
	grafico_name = grafico_name_Cartella2a_SigmaIEtaIEta_EiD;
	asseX_name = asseX_name_Cartella2a_SigmaIEtaIEta_EiD; 
	asseY_name = asseY_name_Cartella2a_SigmaIEtaIEta_EiD;
	rebin = rebin_SigmaIEtaIEta;
	nminX = nminX_SigmaIEtaIEta;
	nmaxX = nmaxX_SigmaIEtaIEta;
	nminY = nminY_SigmaIEtaIEta;
	nmaxY = nmaxY_SigmaIEtaIEta;}
	
else if(b==17){cartellaplot = grafico_name_Cartella2a_LeadRelIso_PreIso;
	grafico_name = grafico_name_Cartella2a_LeadRelIso_PreIso;
	asseX_name = asseX_name_Cartella2a_LeadRelIso_PreIso; 
	asseY_name = asseY_name_Cartella2a_LeadRelIso_PreIso;
	rebin = rebin_LeadRelIso_PreIso;
	nminX = nminX_LeadRelIso_PreIso;
	nmaxX = nmaxX_LeadRelIso_PreIso;
	nminY = nminY_LeadRelIso_PreIso;
	nmaxY = nmaxY_LeadRelIso_PreIso;}
	
else if(b==18){cartellaplot = grafico_name_Cartella2a_SecRelIso_PreIso;
	grafico_name = grafico_name_Cartella2a_SecRelIso_PreIso;
	asseX_name = asseX_name_Cartella2a_SecRelIso_PreIso; 
	asseY_name = asseY_name_Cartella2a_SecRelIso_PreIso;
	rebin = rebin_SecRelIso_PreIso;
	nminX = nminX_SecRelIso_PreIso;
	nmaxX = nmaxX_SecRelIso_PreIso;
	nminY = nminY_SecRelIso_PreIso;
	nmaxY = nmaxY_SecRelIso_PreIso;}

TDirectory *Dir_3;
Dir_3 = Dir_2a->mkdir(cartellaplot.c_str());
Dir_3->cd();
    
	TH1D* histoQCD_EMEnriched_all_1;
	TH1D* histoQCD_EMEnriched_all_12;
	TH1D* histoQCD_EMEnriched_all_123;
	TH1D* histoQCD_EMEnriched_all_1234;
	TH1D* histoQCD_EMEnriched_all_12345;
	TH1D* histoQCD_EMEnriched_all_123456;
	string QCD_EMEnriched_all_name = grafico_name.c_str();
	if(b>8){
	histoQCD_EMEnriched_all_1 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_1->Scale(scale);
	histoQCD_EMEnriched_all_1->Rebin(rebin);
	}else{
	QCD_EMEnriched_all_name+=_RecoCutFlags[1].c_str();
	histoQCD_EMEnriched_all_1 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_1->Scale(scale);
	histoQCD_EMEnriched_all_1->Rebin(rebin);
	QCD_EMEnriched_all_name+=_RecoCutFlags[2].c_str();
	histoQCD_EMEnriched_all_12 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_12->Scale(scale);
	histoQCD_EMEnriched_all_12->Rebin(rebin);
	QCD_EMEnriched_all_name+=_RecoCutFlags[3].c_str();
	histoQCD_EMEnriched_all_123 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_123->Scale(scale);
	histoQCD_EMEnriched_all_123->Rebin(rebin);
	QCD_EMEnriched_all_name+=_RecoCutFlags[4].c_str();
	histoQCD_EMEnriched_all_1234 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_1234->Scale(scale);
	histoQCD_EMEnriched_all_1234->Rebin(rebin);
	QCD_EMEnriched_all_name+=_RecoCutFlags[5].c_str();
	histoQCD_EMEnriched_all_12345 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_12345->Scale(scale);
	histoQCD_EMEnriched_all_12345->Rebin(rebin);
	QCD_EMEnriched_all_name+=_RecoCutFlags[6].c_str();
	histoQCD_EMEnriched_all_123456 = (TH1D*) Cartella1_QCD_EMEnriched_all_Cartella2a->Get(QCD_EMEnriched_all_name.c_str());
	histoQCD_EMEnriched_all_123456->Scale(scale);
	histoQCD_EMEnriched_all_123456->Rebin(rebin);
	}
	
	TH1D* histoQCD_BCtoE_all_1;
	TH1D* histoQCD_BCtoE_all_12;
	TH1D* histoQCD_BCtoE_all_123;
	TH1D* histoQCD_BCtoE_all_1234;
	TH1D* histoQCD_BCtoE_all_12345;
	TH1D* histoQCD_BCtoE_all_123456;
	string QCD_BCtoE_all_name = grafico_name.c_str();
	if(b>8){
	histoQCD_BCtoE_all_1 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_1->Scale(scale);
	histoQCD_BCtoE_all_1->Rebin(rebin);
	}else{
	QCD_BCtoE_all_name+=_RecoCutFlags[1].c_str();
	histoQCD_BCtoE_all_1 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_1->Scale(scale);
	histoQCD_BCtoE_all_1->Rebin(rebin);
	QCD_BCtoE_all_name+=_RecoCutFlags[2].c_str();
	histoQCD_BCtoE_all_12 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_12->Scale(scale);
	histoQCD_BCtoE_all_12->Rebin(rebin);
	QCD_BCtoE_all_name+=_RecoCutFlags[3].c_str();
	histoQCD_BCtoE_all_123 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_123->Scale(scale);
	histoQCD_BCtoE_all_123->Rebin(rebin);
	QCD_BCtoE_all_name+=_RecoCutFlags[4].c_str();
	histoQCD_BCtoE_all_1234 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_1234->Scale(scale);
	histoQCD_BCtoE_all_1234->Rebin(rebin);
	QCD_BCtoE_all_name+=_RecoCutFlags[5].c_str();
	histoQCD_BCtoE_all_12345 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_12345->Scale(scale);
	histoQCD_BCtoE_all_12345->Rebin(rebin);
	QCD_BCtoE_all_name+=_RecoCutFlags[6].c_str();
	histoQCD_BCtoE_all_123456 = (TH1D*) Cartella1_QCD_BCtoE_all_Cartella2a->Get(QCD_BCtoE_all_name.c_str());
	histoQCD_BCtoE_all_123456->Scale(scale);
	histoQCD_BCtoE_all_123456->Rebin(rebin);
	}
	
	TH1D* histoTTbar_1;
	TH1D* histoTTbar_12;
	TH1D* histoTTbar_123;
	TH1D* histoTTbar_1234;
	TH1D* histoTTbar_12345;
	TH1D* histoTTbar_123456;
	string TTbar_name = grafico_name.c_str();
	if(b>8){
	histoTTbar_1 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_1->Scale(scale);
	histoTTbar_1->Rebin(rebin);
	}else{
	TTbar_name+=_RecoCutFlags[1].c_str();
	histoTTbar_1 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_1->Scale(scale);
	histoTTbar_1->Rebin(rebin);
	TTbar_name+=_RecoCutFlags[2].c_str();
	histoTTbar_12 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_12->Scale(scale);
	histoTTbar_12->Rebin(rebin);
	TTbar_name+=_RecoCutFlags[3].c_str();
	histoTTbar_123 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_123->Scale(scale);
	histoTTbar_123->Rebin(rebin);
	TTbar_name+=_RecoCutFlags[4].c_str();
	histoTTbar_1234 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_1234->Scale(scale);
	histoTTbar_1234->Rebin(rebin);
	TTbar_name+=_RecoCutFlags[5].c_str();
	histoTTbar_12345 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_12345->Scale(scale);
	histoTTbar_12345->Rebin(rebin);
	TTbar_name+=_RecoCutFlags[6].c_str();
	histoTTbar_123456 = (TH1D*) Cartella1_TTbar_Cartella2a->Get(TTbar_name.c_str());
	histoTTbar_123456->Scale(scale);
	histoTTbar_123456->Rebin(rebin);
	}
    
	TH1D* histoWlnu_1;
	TH1D* histoWlnu_12;
	TH1D* histoWlnu_123;
	TH1D* histoWlnu_1234;
	TH1D* histoWlnu_12345;
	TH1D* histoWlnu_123456;
	string Wlnu_name = grafico_name.c_str();
	if(b>8){
		histoWlnu_1 = (TH1D*) Cartella1_Wlnu_Cartella2a->Get(Wlnu_name.c_str());
		histoWlnu_1->Scale(scale);
		histoWlnu_1->Rebin(rebin);
	}else{
	    Wlnu_name+=_RecoCutFlags[1].c_str();
		histoWlnu_1 = (TH1D*) Cartella1_Wlnu_Cartella2a->Get(Wlnu_name.c_str());
		histoWlnu_1->Scale(scale);
		histoWlnu_1->Rebin(rebin);
		Wlnu_name+=_RecoCutFlags[2].c_str();
		histoWlnu_12 = (TH1D*) Cartella1_Wlnu_Cartella2a->Get(Wlnu_name.c_str());
		histoWlnu_12->Scale(scale);
		histoWlnu_12->Rebin(rebin);
		Wlnu_name+=_RecoCutFlags[3].c_str();
		histoWlnu_123 = (TH1D*) Cartella1_Wlnu_Cartella2a->Get(Wlnu_name.c_str());
		histoWlnu_123->Scale(scale);
		histoWlnu_123->Rebin(rebin);
		Wlnu_name+=_RecoCutFlags[4].c_str();
		histoWlnu_1234 = (TH1D*) Cartella1_Wlnu_Cartella2a->Get(Wlnu_name.c_str());
		histoWlnu_1234->Scale(scale);
		histoWlnu_1234->Rebin(rebin);
		Wlnu_name+=_RecoCutFlags[5].c_str();
		histoWlnu_12345 = (TH1D*) Cartella1_Wlnu_Cartella2a->Get(Wlnu_name.c_str());
		histoWlnu_12345->Scale(scale);
		histoWlnu_12345->Rebin(rebin);
		Wlnu_name+=_RecoCutFlags[6].c_str();
		histoWlnu_123456 = (TH1D*) Cartella1_Wlnu_Cartella2a->Get(Wlnu_name.c_str());
		histoWlnu_123456->Scale(scale);
		histoWlnu_123456->Rebin(rebin);
	}
	
	TH1D* histoWWEE_1;
	TH1D* histoWWEE_12;
	TH1D* histoWWEE_123;
	TH1D* histoWWEE_1234;
	TH1D* histoWWEE_12345;
	TH1D* histoWWEE_123456;
	string WWEE_name = grafico_name.c_str();
	if(b>8){
		histoWWEE_1 = (TH1D*) Cartella1_WWEE_Cartella2a->Get(WWEE_name.c_str());
		histoWWEE_1->Scale(scale);
		histoWWEE_1->Rebin(rebin);
	}else{
		WWEE_name+=_RecoCutFlags[1].c_str();
		histoWWEE_1 = (TH1D*) Cartella1_WWEE_Cartella2a->Get(WWEE_name.c_str());
		histoWWEE_1->Scale(scale);
		histoWWEE_1->Rebin(rebin);
		WWEE_name+=_RecoCutFlags[2].c_str();
		histoWWEE_12 = (TH1D*) Cartella1_WWEE_Cartella2a->Get(WWEE_name.c_str());
		histoWWEE_12->Scale(scale);
		histoWWEE_12->Rebin(rebin);
		WWEE_name+=_RecoCutFlags[3].c_str();
		histoWWEE_123 = (TH1D*) Cartella1_WWEE_Cartella2a->Get(WWEE_name.c_str());
		histoWWEE_123->Scale(scale);
		histoWWEE_123->Rebin(rebin);
		WWEE_name+=_RecoCutFlags[4].c_str();
		histoWWEE_1234 = (TH1D*) Cartella1_WWEE_Cartella2a->Get(WWEE_name.c_str());
		histoWWEE_1234->Scale(scale);
		histoWWEE_1234->Rebin(rebin);
		WWEE_name+=_RecoCutFlags[5].c_str();
		histoWWEE_12345 = (TH1D*) Cartella1_WWEE_Cartella2a->Get(WWEE_name.c_str());
		histoWWEE_12345->Scale(scale);
		histoWWEE_12345->Rebin(rebin);
		WWEE_name+=_RecoCutFlags[6].c_str();
		histoWWEE_123456 = (TH1D*) Cartella1_WWEE_Cartella2a->Get(WWEE_name.c_str());
		histoWWEE_123456->Scale(scale);
		histoWWEE_123456->Rebin(rebin);
	}
	
	TH1D* histoWZEE_1;
	TH1D* histoWZEE_12;
	TH1D* histoWZEE_123;
	TH1D* histoWZEE_1234;
	TH1D* histoWZEE_12345;
	TH1D* histoWZEE_123456;
	string WZEE_name = grafico_name.c_str();
	if(b>8){
		histoWZEE_1 = (TH1D*) Cartella1_WZEE_Cartella2a->Get(WZEE_name.c_str());
		histoWZEE_1->Scale(scale);
		histoWZEE_1->Rebin(rebin);
	}else{
		WZEE_name+=_RecoCutFlags[1].c_str();
		histoWZEE_1 = (TH1D*) Cartella1_WZEE_Cartella2a->Get(WZEE_name.c_str());
		histoWZEE_1->Scale(scale);
		histoWZEE_1->Rebin(rebin);
		WZEE_name+=_RecoCutFlags[2].c_str();
		histoWZEE_12 = (TH1D*) Cartella1_WZEE_Cartella2a->Get(WZEE_name.c_str());
		histoWZEE_12->Scale(scale);
		histoWZEE_12->Rebin(rebin);
		WZEE_name+=_RecoCutFlags[3].c_str();
		histoWZEE_123 = (TH1D*) Cartella1_WZEE_Cartella2a->Get(WZEE_name.c_str());
		histoWZEE_123->Scale(scale);
		histoWZEE_123->Rebin(rebin);
		WZEE_name+=_RecoCutFlags[4].c_str();
		histoWZEE_1234 = (TH1D*) Cartella1_WZEE_Cartella2a->Get(WZEE_name.c_str());
		histoWZEE_1234->Scale(scale);
		histoWZEE_1234->Rebin(rebin);
		WZEE_name+=_RecoCutFlags[5].c_str();
		histoWZEE_12345 = (TH1D*) Cartella1_WZEE_Cartella2a->Get(WZEE_name.c_str());
		histoWZEE_12345->Scale(scale);
		histoWZEE_12345->Rebin(rebin);
		WZEE_name+=_RecoCutFlags[6].c_str();
		histoWZEE_123456 = (TH1D*) Cartella1_WZEE_Cartella2a->Get(WZEE_name.c_str());
		histoWZEE_123456->Scale(scale);
		histoWZEE_123456->Rebin(rebin);
	}
	
	TH1D* histoZZEE_1;
	TH1D* histoZZEE_12;
	TH1D* histoZZEE_123;
	TH1D* histoZZEE_1234;
	TH1D* histoZZEE_12345;
	TH1D* histoZZEE_123456;
	string ZZEE_name = grafico_name.c_str();
	if(b>8){
		histoZZEE_1 = (TH1D*) Cartella1_ZZEE_Cartella2a->Get(ZZEE_name.c_str());
		histoZZEE_1->Scale(scale);
		histoZZEE_1->Rebin(rebin);
	}else{
		ZZEE_name+=_RecoCutFlags[1].c_str();
		histoZZEE_1 = (TH1D*) Cartella1_ZZEE_Cartella2a->Get(ZZEE_name.c_str());
		histoZZEE_1->Scale(scale);
		histoZZEE_1->Rebin(rebin);
		ZZEE_name+=_RecoCutFlags[2].c_str();
		histoZZEE_12 = (TH1D*) Cartella1_ZZEE_Cartella2a->Get(ZZEE_name.c_str());
		histoZZEE_12->Scale(scale);
		histoZZEE_12->Rebin(rebin);
		ZZEE_name+=_RecoCutFlags[3].c_str();
		histoZZEE_123 = (TH1D*) Cartella1_ZZEE_Cartella2a->Get(ZZEE_name.c_str());
		histoZZEE_123->Scale(scale);
		histoZZEE_123->Rebin(rebin);
		ZZEE_name+=_RecoCutFlags[4].c_str();
		histoZZEE_1234 = (TH1D*) Cartella1_ZZEE_Cartella2a->Get(ZZEE_name.c_str());
		histoZZEE_1234->Scale(scale);
		histoZZEE_1234->Rebin(rebin);
		ZZEE_name+=_RecoCutFlags[5].c_str();
		histoZZEE_12345 = (TH1D*) Cartella1_ZZEE_Cartella2a->Get(ZZEE_name.c_str());
		histoZZEE_12345->Scale(scale);
		histoZZEE_12345->Rebin(rebin);
		ZZEE_name+=_RecoCutFlags[6].c_str();
		histoZZEE_123456 = (TH1D*) Cartella1_ZZEE_Cartella2a->Get(ZZEE_name.c_str());
		histoZZEE_123456->Scale(scale);
		histoZZEE_123456->Rebin(rebin);
	}
	
	TH1D* histoZ_1;
	TH1D* histoZ_12;
	TH1D* histoZ_123;
	TH1D* histoZ_1234;
	TH1D* histoZ_12345;
	TH1D* histoZ_123456;
 	string Z_name = grafico_name.c_str();
	if(b>8){
	histoZ_1 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_1->Scale(scale);
	histoZ_1->Rebin(rebin);
	}else{
	Z_name+=_RecoCutFlags[1].c_str();
	histoZ_1 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_1->Scale(scale);
	histoZ_1->Rebin(rebin);	
	Z_name+=_RecoCutFlags[2].c_str();
	histoZ_12 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_12->Scale(scale);
	histoZ_12->Rebin(rebin);
	Z_name+=_RecoCutFlags[3].c_str();
	histoZ_123 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_123->Scale(scale);
	histoZ_123->Rebin(rebin);
	Z_name+=_RecoCutFlags[4].c_str();
	histoZ_1234 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_1234->Scale(scale);
	histoZ_1234->Rebin(rebin);
	Z_name+=_RecoCutFlags[5].c_str();
	histoZ_12345 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_12345->Scale(scale);
	histoZ_12345->Rebin(rebin);
	Z_name+=_RecoCutFlags[6].c_str();
	histoZ_123456 = (TH1D*) Cartella1_Z_Cartella2a->Get(Z_name.c_str());
	histoZ_123456->Scale(scale);
	histoZ_123456->Rebin(rebin);
	}

	if(!histoZ_123456){
	cout<<"Error! Cut sequence wrong!"<<endl;
	return;
	}

    TH1D* histoData_1;
	TH1D* histoData_12;
	TH1D* histoData_123;
	TH1D* histoData_1234;
	TH1D* histoData_12345;
	TH1D* histoData_123456;
	string Data_name = grafico_name.c_str();
	if(b>8){
	histoData_1 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_1->Rebin(rebin);
	}else{
	Data_name+=_RecoCutFlags[1].c_str();
	histoData_1 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_1->Rebin(rebin);	
	Data_name+=_RecoCutFlags[2].c_str();
	histoData_12 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_12->Rebin(rebin);
	Data_name+=_RecoCutFlags[3].c_str();
	histoData_123 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_123->Rebin(rebin);
	Data_name+=_RecoCutFlags[4].c_str();
	histoData_1234 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_1234->Rebin(rebin);	
	Data_name+=_RecoCutFlags[5].c_str();
	histoData_12345 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_12345->Rebin(rebin);
	Data_name+=_RecoCutFlags[6].c_str();
	histoData_123456 = (TH1D*) Cartella1_Data_Cartella2a->Get(Data_name.c_str());
	histoData_123456->Rebin(rebin);
	}
	
TDirectory *Dir_4a;
Dir_4a = Dir_3->mkdir("Separated_Plots");
Dir_4a->cd();
	
	TCanvas *c_1 = new TCanvas();
	
	if (log_scale == "True") {c_1->SetLogy();}
	
	string c_name = grafico_name.c_str();	
	if(b<9)c_name+=_RecoCutFlags[1].c_str();
	
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
	histoWlnu_1->SetLineColor(col_Wlnu);
	histoWlnu_1->SetLineWidth(2);
	histoWlnu_1->SetFillColor(col_Wlnu);
	histoWlnu_1->SetFillStyle(style);
	histoWlnu_1->SetMarkerColor(col_Wlnu);
	histoWlnu_1->Draw("hist same");	
	histoWWEE_1->SetLineColor(col_WWEE);
	histoWWEE_1->SetLineWidth(2);
	histoWWEE_1->SetFillColor(col_WWEE);
	histoWWEE_1->SetFillStyle(style);
	histoWWEE_1->SetMarkerColor(col_WWEE);
	histoWWEE_1->Draw("hist same");
	histoWZEE_1->SetLineColor(col_WZEE);
	histoWZEE_1->SetLineWidth(2);
	histoWZEE_1->SetFillColor(col_WZEE);
	histoWZEE_1->SetFillStyle(style);
	histoWZEE_1->SetMarkerColor(col_WZEE);
	histoWZEE_1->Draw("hist same");
	histoZZEE_1->SetLineColor(col_ZZEE);
	histoZZEE_1->SetLineWidth(2);
	histoZZEE_1->SetFillColor(col_ZZEE);
	histoZZEE_1->SetFillStyle(style);
	histoZZEE_1->SetMarkerColor(col_ZZEE);
	histoZZEE_1->Draw("hist same");	
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
	Leg_1->AddEntry(histoWlnu_1,Leg_Wlnu.c_str());
	Leg_1->AddEntry(histoWWEE_1,Leg_WWEE.c_str());
	Leg_1->AddEntry(histoWZEE_1,Leg_WZEE.c_str());
	Leg_1->AddEntry(histoZZEE_1,Leg_ZZEE.c_str());
	Leg_1->AddEntry(histoZ_1,Leg_Z.c_str());
	Leg_1->AddEntry(histoData_1,Leg_Data.c_str());	
		
	Leg_1->Draw("LP");
	
	c_1->Write(c_name.c_str());
	c_1->Close();
	
	if(Tab_cut == "True"){
		double err_QCD_EMEnriched_all_1, err_QCD_BCtoE_all_1, err_TTbar_1, err_Wlnu_1, err_WWEE_1, err_WZEE_1, err_ZZEE_1, err_Z_1, err_Data_1;
		
		cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
		cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_1->IntegralAndError(0,-1,err_QCD_EMEnriched_all_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_1*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_1->IntegralAndError(0,-1,err_QCD_BCtoE_all_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_1*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_1->IntegralAndError(0,-1,err_TTbar_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_1*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Wlnu_Int.c_str()<<((float)((int)(histoWlnu_1->IntegralAndError(0,-1,err_Wlnu_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Wlnu_Int_err.c_str()<<((float)((int)(err_Wlnu_1*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_WWEE_Int.c_str()<<((float)((int)(histoWWEE_1->IntegralAndError(0,-1,err_WWEE_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_WWEE_Int_err.c_str()<<((float)((int)(err_WWEE_1*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_WZEE_Int.c_str()<<((float)((int)(histoWZEE_1->IntegralAndError(0,-1,err_WZEE_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_WZEE_Int_err.c_str()<<((float)((int)(err_WZEE_1*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_ZZEE_Int.c_str()<<((float)((int)(histoZZEE_1->IntegralAndError(0,-1,err_ZZEE_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_ZZEE_Int_err.c_str()<<((float)((int)(err_ZZEE_1*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_1->IntegralAndError(0,-1,err_Z_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_1*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
		cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_1->IntegralAndError(0,-1,err_QCD_EMEnriched_all_1,"") + histoQCD_BCtoE_all_1->IntegralAndError(0,-1,err_QCD_BCtoE_all_1,"") + histoTTbar_1->IntegralAndError(0,-1,err_TTbar_1,"") + histoWlnu_1->IntegralAndError(0,-1,err_Wlnu_1,"") + histoWWEE_1->IntegralAndError(0,-1,err_WWEE_1,"") + histoWZEE_1->IntegralAndError(0,-1,err_WZEE_1,"") + histoZZEE_1->IntegralAndError(0,-1,err_ZZEE_1,"") + histoZ_1->IntegralAndError(0,-1,err_Z_1,""))*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_1),2)+pow((err_QCD_BCtoE_all_1),2) + pow((err_TTbar_1),2) + pow((err_Wlnu_1),2) + pow((err_WWEE_1),2) + pow((err_WZEE_1),2) + pow((err_ZZEE_1),2) + pow((err_Z_1),2))*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_1->IntegralAndError(0,-1,err_Data_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_1*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_end.c_str()<<endl;
	}
	
	
	if(b<9){
	
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
		histoWlnu_12->SetLineColor(col_Wlnu);
		histoWlnu_12->SetLineWidth(2);
		histoWlnu_12->SetFillColor(col_Wlnu);
		histoWlnu_12->SetFillStyle(style);
		histoWlnu_12->SetMarkerColor(col_Wlnu);
		histoWlnu_12->Draw("hist same");	
		histoWWEE_12->SetLineColor(col_WWEE);
		histoWWEE_12->SetLineWidth(2);
		histoWWEE_12->SetFillColor(col_WWEE);
		histoWWEE_12->SetFillStyle(style);
		histoWWEE_12->SetMarkerColor(col_WWEE);
		histoWWEE_12->Draw("hist same");
		histoWZEE_12->SetLineColor(col_WZEE);
		histoWZEE_12->SetLineWidth(2);
		histoWZEE_12->SetFillColor(col_WZEE);
		histoWZEE_12->SetFillStyle(style);
		histoWZEE_12->SetMarkerColor(col_WZEE);
		histoWZEE_12->Draw("hist same");
		histoZZEE_12->SetLineColor(col_ZZEE);
		histoZZEE_12->SetLineWidth(2);
		histoZZEE_12->SetFillColor(col_ZZEE);
		histoZZEE_12->SetFillStyle(style);
		histoZZEE_12->SetMarkerColor(col_ZZEE);
		histoZZEE_12->Draw("hist same");	
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
		Leg_12->AddEntry(histoWlnu_12,Leg_Wlnu.c_str());
		Leg_12->AddEntry(histoWWEE_12,Leg_WWEE.c_str());
		Leg_12->AddEntry(histoWZEE_12,Leg_WZEE.c_str());
		Leg_12->AddEntry(histoZZEE_12,Leg_ZZEE.c_str());
		Leg_12->AddEntry(histoZ_12,Leg_Z.c_str());
		Leg_12->AddEntry(histoData_12,Leg_Data.c_str());	
	   	
		Leg_12->Draw("LP");
		
		c_12->Write(c_name.c_str());
		c_12->Close();
		
		if(Tab_cut == "True"){
			double err_QCD_EMEnriched_all_12, err_QCD_BCtoE_all_12, err_TTbar_12, err_Wlnu_12, err_WWEE_12, err_WZEE_12, err_ZZEE_12, err_Z_12, err_Data_12;
			
			cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
			cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_12->IntegralAndError(0,-1,err_QCD_EMEnriched_all_12,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_12*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_12->IntegralAndError(0,-1,err_QCD_BCtoE_all_12,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_12*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_12->IntegralAndError(0,-1,err_TTbar_12,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_12*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_Wlnu_Int.c_str()<<((float)((int)(histoWlnu_12->IntegralAndError(0,-1,err_Wlnu_12,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Wlnu_Int_err.c_str()<<((float)((int)(err_Wlnu_12*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_WWEE_Int.c_str()<<((float)((int)(histoWWEE_12->IntegralAndError(0,-1,err_WWEE_12,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_WWEE_Int_err.c_str()<<((float)((int)(err_WWEE_12*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_WZEE_Int.c_str()<<((float)((int)(histoWZEE_12->IntegralAndError(0,-1,err_WZEE_12,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_WZEE_Int_err.c_str()<<((float)((int)(err_WZEE_12*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_ZZEE_Int.c_str()<<((float)((int)(histoZZEE_12->IntegralAndError(0,-1,err_ZZEE_12,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_ZZEE_Int_err.c_str()<<((float)((int)(err_ZZEE_12*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_12->IntegralAndError(0,-1,err_Z_12,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_12*cut_decimal)))/cut_decimal<<endl;
			cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
			cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_12->IntegralAndError(0,-1,err_QCD_EMEnriched_all_12,"") + histoQCD_BCtoE_all_12->IntegralAndError(0,-1,err_QCD_BCtoE_all_12,"") + histoTTbar_12->IntegralAndError(0,-1,err_TTbar_12,"") + histoWlnu_12->IntegralAndError(0,-1,err_Wlnu_12,"") + histoWWEE_12->IntegralAndError(0,-1,err_WWEE_12,"") + histoWZEE_12->IntegralAndError(0,-1,err_WZEE_12,"") + histoZZEE_12->IntegralAndError(0,-1,err_ZZEE_12,"") + histoZ_12->IntegralAndError(0,-1,err_Z_12,""))*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_12),2)+pow((err_QCD_BCtoE_all_12),2) + pow((err_TTbar_12),2) + pow((err_Wlnu_12),2) + pow((err_WWEE_12),2) + pow((err_WZEE_12),2) + pow((err_ZZEE_12),2) + pow((err_Z_12),2))*cut_decimal)))/cut_decimal<<endl<<endl;
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
		histoWlnu_123->SetLineColor(col_Wlnu);
		histoWlnu_123->SetLineWidth(2);
		histoWlnu_123->SetFillColor(col_Wlnu);
		histoWlnu_123->SetFillStyle(style);
		histoWlnu_123->SetMarkerColor(col_Wlnu);
		histoWlnu_123->Draw("hist same");	
		histoWWEE_123->SetLineColor(col_WWEE);
		histoWWEE_123->SetLineWidth(2);
		histoWWEE_123->SetFillColor(col_WWEE);
		histoWWEE_123->SetFillStyle(style);
		histoWWEE_123->SetMarkerColor(col_WWEE);
		histoWWEE_123->Draw("hist same");
		histoWZEE_123->SetLineColor(col_WZEE);
		histoWZEE_123->SetLineWidth(2);
		histoWZEE_123->SetFillColor(col_WZEE);
		histoWZEE_123->SetFillStyle(style);
		histoWZEE_123->SetMarkerColor(col_WZEE);
		histoWZEE_123->Draw("hist same");
		histoZZEE_123->SetLineColor(col_ZZEE);
		histoZZEE_123->SetLineWidth(2);
		histoZZEE_123->SetFillColor(col_ZZEE);
		histoZZEE_123->SetFillStyle(style);
		histoZZEE_123->SetMarkerColor(col_ZZEE);
		histoZZEE_123->Draw("hist same");	
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
		Leg_123->AddEntry(histoWlnu_123,Leg_Wlnu.c_str());
		Leg_123->AddEntry(histoWWEE_123,Leg_WWEE.c_str());
		Leg_123->AddEntry(histoWZEE_123,Leg_WZEE.c_str());
		Leg_123->AddEntry(histoZZEE_123,Leg_ZZEE.c_str());
		Leg_123->AddEntry(histoZ_123,Leg_Z.c_str());
		Leg_123->AddEntry(histoData_123,Leg_Data.c_str());	
	   	
		Leg_123->Draw("LP");
		
		c_123->Write(c_name.c_str());
		c_123->Close();
		
		if(Tab_cut == "True"){
			double err_QCD_EMEnriched_all_123, err_QCD_BCtoE_all_123, err_TTbar_123, err_Wlnu_123, err_WWEE_123, err_WZEE_123, err_ZZEE_123, err_Z_123, err_Data_123;
			
			cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
			cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_123->IntegralAndError(0,-1,err_QCD_EMEnriched_all_123,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_123*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_123->IntegralAndError(0,-1,err_QCD_BCtoE_all_123,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_123*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_123->IntegralAndError(0,-1,err_TTbar_123,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_123*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_Wlnu_Int.c_str()<<((float)((int)(histoWlnu_123->IntegralAndError(0,-1,err_Wlnu_123,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Wlnu_Int_err.c_str()<<((float)((int)(err_Wlnu_123*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_WWEE_Int.c_str()<<((float)((int)(histoWWEE_123->IntegralAndError(0,-1,err_WWEE_123,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_WWEE_Int_err.c_str()<<((float)((int)(err_WWEE_123*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_WZEE_Int.c_str()<<((float)((int)(histoWZEE_123->IntegralAndError(0,-1,err_WZEE_123,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_WZEE_Int_err.c_str()<<((float)((int)(err_WZEE_123*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_ZZEE_Int.c_str()<<((float)((int)(histoZZEE_123->IntegralAndError(0,-1,err_ZZEE_123,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_ZZEE_Int_err.c_str()<<((float)((int)(err_ZZEE_123*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_123->IntegralAndError(0,-1,err_Z_123,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_123*cut_decimal)))/cut_decimal<<endl;
			cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
			cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_123->IntegralAndError(0,-1,err_QCD_EMEnriched_all_123,"") + histoQCD_BCtoE_all_123->IntegralAndError(0,-1,err_QCD_BCtoE_all_123,"") + histoTTbar_123->IntegralAndError(0,-1,err_TTbar_123,"") + histoWlnu_123->IntegralAndError(0,-1,err_Wlnu_123,"") + histoWWEE_123->IntegralAndError(0,-1,err_WWEE_123,"") + histoWZEE_123->IntegralAndError(0,-1,err_WZEE_123,"") + histoZZEE_123->IntegralAndError(0,-1,err_ZZEE_123,"") + histoZ_123->IntegralAndError(0,-1,err_Z_123,""))*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_123),2)+pow((err_QCD_BCtoE_all_123),2) + pow((err_TTbar_123),2) + pow((err_Wlnu_123),2) + pow((err_WWEE_123),2) + pow((err_WZEE_123),2) + pow((err_ZZEE_123),2) + pow((err_Z_123),2))*cut_decimal)))/cut_decimal<<endl<<endl;
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
		histoWlnu_1234->SetLineColor(col_Wlnu);
		histoWlnu_1234->SetLineWidth(2);
		histoWlnu_1234->SetFillColor(col_Wlnu);
		histoWlnu_1234->SetFillStyle(style);
		histoWlnu_1234->SetMarkerColor(col_Wlnu);
		histoWlnu_1234->Draw("hist same");	
		histoWWEE_1234->SetLineColor(col_WWEE);
		histoWWEE_1234->SetLineWidth(2);
		histoWWEE_1234->SetFillColor(col_WWEE);
		histoWWEE_1234->SetFillStyle(style);
		histoWWEE_1234->SetMarkerColor(col_WWEE);
		histoWWEE_1234->Draw("hist same");
		histoWZEE_1234->SetLineColor(col_WZEE);
		histoWZEE_1234->SetLineWidth(2);
		histoWZEE_1234->SetFillColor(col_WZEE);
		histoWZEE_1234->SetFillStyle(style);
		histoWZEE_1234->SetMarkerColor(col_WZEE);
		histoWZEE_1234->Draw("hist same");
		histoZZEE_1234->SetLineColor(col_ZZEE);
		histoZZEE_1234->SetLineWidth(2);
		histoZZEE_1234->SetFillColor(col_ZZEE);
		histoZZEE_1234->SetFillStyle(style);
		histoZZEE_1234->SetMarkerColor(col_ZZEE);
		histoZZEE_1234->Draw("hist same");	
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
		Leg_1234->AddEntry(histoWlnu_1234,Leg_Wlnu.c_str());
		Leg_1234->AddEntry(histoWWEE_1234,Leg_WWEE.c_str());
		Leg_1234->AddEntry(histoWZEE_1234,Leg_WZEE.c_str());
		Leg_1234->AddEntry(histoZZEE_1234,Leg_ZZEE.c_str());
		Leg_1234->AddEntry(histoZ_1234,Leg_Z.c_str());
		Leg_1234->AddEntry(histoData_1234,Leg_Data.c_str());		
	   	
		Leg_1234->Draw("LP");
		
		c_1234->Write(c_name.c_str());
		c_1234->Close();
		
		if(Tab_cut == "True"){
			double err_QCD_EMEnriched_all_1234, err_QCD_BCtoE_all_1234, err_TTbar_1234, err_Wlnu_1234, err_WWEE_1234, err_WZEE_1234, err_ZZEE_1234, err_Z_1234, err_Data_1234;
			
			cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
			cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_1234->IntegralAndError(0,-1,err_QCD_EMEnriched_all_1234,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_1234*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_1234->IntegralAndError(0,-1,err_QCD_BCtoE_all_1234,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_1234*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_1234->IntegralAndError(0,-1,err_TTbar_1234,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_1234*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_Wlnu_Int.c_str()<<((float)((int)(histoWlnu_1234->IntegralAndError(0,-1,err_Wlnu_1234,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Wlnu_Int_err.c_str()<<((float)((int)(err_Wlnu_1234*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_WWEE_Int.c_str()<<((float)((int)(histoWWEE_1234->IntegralAndError(0,-1,err_WWEE_1234,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_WWEE_Int_err.c_str()<<((float)((int)(err_WWEE_1234*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_WZEE_Int.c_str()<<((float)((int)(histoWZEE_1234->IntegralAndError(0,-1,err_WZEE_1234,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_WZEE_Int_err.c_str()<<((float)((int)(err_WZEE_1234*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_ZZEE_Int.c_str()<<((float)((int)(histoZZEE_1234->IntegralAndError(0,-1,err_ZZEE_1234,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_ZZEE_Int_err.c_str()<<((float)((int)(err_ZZEE_1234*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_1234->IntegralAndError(0,-1,err_Z_1234,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_1234*cut_decimal)))/cut_decimal<<endl;
			cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
			cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_1234->IntegralAndError(0,-1,err_QCD_EMEnriched_all_1234,"") + histoQCD_BCtoE_all_1234->IntegralAndError(0,-1,err_QCD_BCtoE_all_1234,"") + histoTTbar_1234->IntegralAndError(0,-1,err_TTbar_1234,"") + histoWlnu_1234->IntegralAndError(0,-1,err_Wlnu_1234,"") + histoWWEE_1234->IntegralAndError(0,-1,err_WWEE_1234,"") + histoWZEE_1234->IntegralAndError(0,-1,err_WZEE_1234,"") + histoZZEE_1234->IntegralAndError(0,-1,err_ZZEE_1234,"") + histoZ_1234->IntegralAndError(0,-1,err_Z_1234,""))*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_1234),2)+pow((err_QCD_BCtoE_all_1234),2) + pow((err_TTbar_1234),2) + pow((err_Wlnu_1234),2) + pow((err_WWEE_1234),2) + pow((err_WZEE_1234),2) + pow((err_ZZEE_1234),2) + pow((err_Z_1234),2))*cut_decimal)))/cut_decimal<<endl<<endl;
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
		histoWlnu_12345->SetLineColor(col_Wlnu);
		histoWlnu_12345->SetLineWidth(2);
		histoWlnu_12345->SetFillColor(col_Wlnu);
		histoWlnu_12345->SetFillStyle(style);
		histoWlnu_12345->SetMarkerColor(col_Wlnu);
		histoWlnu_12345->Draw("hist same");	
		histoWWEE_12345->SetLineColor(col_WWEE);
		histoWWEE_12345->SetLineWidth(2);
		histoWWEE_12345->SetFillColor(col_WWEE);
		histoWWEE_12345->SetFillStyle(style);
		histoWWEE_12345->SetMarkerColor(col_WWEE);
		histoWWEE_12345->Draw("hist same");
		histoWZEE_12345->SetLineColor(col_WZEE);
		histoWZEE_12345->SetLineWidth(2);
		histoWZEE_12345->SetFillColor(col_WZEE);
		histoWZEE_12345->SetFillStyle(style);
		histoWZEE_12345->SetMarkerColor(col_WZEE);
		histoWZEE_12345->Draw("hist same");
		histoZZEE_12345->SetLineColor(col_ZZEE);
		histoZZEE_12345->SetLineWidth(2);
		histoZZEE_12345->SetFillColor(col_ZZEE);
		histoZZEE_12345->SetFillStyle(style);
		histoZZEE_12345->SetMarkerColor(col_ZZEE);
		histoZZEE_12345->Draw("hist same");	
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
		Leg_12345->AddEntry(histoWlnu_12345,Leg_Wlnu.c_str());
		Leg_12345->AddEntry(histoWWEE_12345,Leg_WWEE.c_str());
		Leg_12345->AddEntry(histoWZEE_12345,Leg_WZEE.c_str());
		Leg_12345->AddEntry(histoZZEE_12345,Leg_ZZEE.c_str());
		Leg_12345->AddEntry(histoZ_12345,Leg_Z.c_str());
		Leg_12345->AddEntry(histoData_12345,Leg_Data.c_str());	
	   	
		Leg_12345->Draw("LP");
		
		c_12345->Write(c_name.c_str());
		c_12345->Close();
		
		if(Tab_cut == "True"){
			double err_QCD_EMEnriched_all_12345, err_QCD_BCtoE_all_12345, err_TTbar_12345, err_Wlnu_12345, err_WWEE_12345, err_WZEE_12345, err_ZZEE_12345, err_Z_12345, err_Data_12345;
			
			cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
			cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_12345->IntegralAndError(0,-1,err_QCD_EMEnriched_all_12345,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_12345*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_12345->IntegralAndError(0,-1,err_QCD_BCtoE_all_12345,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_12345*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_12345->IntegralAndError(0,-1,err_TTbar_12345,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_12345*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_Wlnu_Int.c_str()<<((float)((int)(histoWlnu_12345->IntegralAndError(0,-1,err_Wlnu_12345,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Wlnu_Int_err.c_str()<<((float)((int)(err_Wlnu_12345*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_WWEE_Int.c_str()<<((float)((int)(histoWWEE_12345->IntegralAndError(0,-1,err_WWEE_12345,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_WWEE_Int_err.c_str()<<((float)((int)(err_WWEE_12345*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_WZEE_Int.c_str()<<((float)((int)(histoWZEE_12345->IntegralAndError(0,-1,err_WZEE_12345,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_WZEE_Int_err.c_str()<<((float)((int)(err_WZEE_12345*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_ZZEE_Int.c_str()<<((float)((int)(histoZZEE_12345->IntegralAndError(0,-1,err_ZZEE_12345,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_ZZEE_Int_err.c_str()<<((float)((int)(err_ZZEE_12345*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_12345->IntegralAndError(0,-1,err_Z_12345,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_12345*cut_decimal)))/cut_decimal<<endl;
			cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
			cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_12345->IntegralAndError(0,-1,err_QCD_EMEnriched_all_12345,"") + histoQCD_BCtoE_all_12345->IntegralAndError(0,-1,err_QCD_BCtoE_all_12345,"") + histoTTbar_12345->IntegralAndError(0,-1,err_TTbar_12345,"") + histoWlnu_12345->IntegralAndError(0,-1,err_Wlnu_12345,"") + histoWWEE_12345->IntegralAndError(0,-1,err_WWEE_12345,"") + histoWZEE_12345->IntegralAndError(0,-1,err_WZEE_12345,"") + histoZZEE_12345->IntegralAndError(0,-1,err_ZZEE_12345,"") + histoZ_12345->IntegralAndError(0,-1,err_Z_12345,""))*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_12345),2)+pow((err_QCD_BCtoE_all_12345),2) + pow((err_TTbar_12345),2) + pow((err_Wlnu_12345),2) + pow((err_WWEE_12345),2) + pow((err_WZEE_12345),2) + pow((err_ZZEE_12345),2) + pow((err_Z_12345),2))*cut_decimal)))/cut_decimal<<endl<<endl;
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
		histoWlnu_123456->SetLineColor(col_Wlnu);
		histoWlnu_123456->SetLineWidth(2);
		histoWlnu_123456->SetFillColor(col_Wlnu);
		histoWlnu_123456->SetFillStyle(style);
		histoWlnu_123456->SetMarkerColor(col_Wlnu);
		histoWlnu_123456->Draw("hist same");	
		histoWWEE_123456->SetLineColor(col_WWEE);
		histoWWEE_123456->SetLineWidth(2);
		histoWWEE_123456->SetFillColor(col_WWEE);
		histoWWEE_123456->SetFillStyle(style);
		histoWWEE_123456->SetMarkerColor(col_WWEE);
		histoWWEE_123456->Draw("hist same");
		histoWZEE_123456->SetLineColor(col_WZEE);
		histoWZEE_123456->SetLineWidth(2);
		histoWZEE_123456->SetFillColor(col_WZEE);
		histoWZEE_123456->SetFillStyle(style);
		histoWZEE_123456->SetMarkerColor(col_WZEE);
		histoWZEE_123456->Draw("hist same");
		histoZZEE_123456->SetLineColor(col_ZZEE);
		histoZZEE_123456->SetLineWidth(2);
		histoZZEE_123456->SetFillColor(col_ZZEE);
		histoZZEE_123456->SetFillStyle(style);
		histoZZEE_123456->SetMarkerColor(col_ZZEE);
		histoZZEE_123456->Draw("hist same");	
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
		Leg_123456->AddEntry(histoWlnu_123456,Leg_Wlnu.c_str());
		Leg_123456->AddEntry(histoWWEE_123456,Leg_WWEE.c_str());
		Leg_123456->AddEntry(histoWZEE_123456,Leg_WZEE.c_str());
		Leg_123456->AddEntry(histoZZEE_123456,Leg_ZZEE.c_str());
		Leg_123456->AddEntry(histoZ_123456,Leg_Z.c_str());
		Leg_123456->AddEntry(histoData_123456,Leg_Data.c_str());	
	   	
		Leg_123456->Draw("LP");
		
		c_123456->Write(c_name.c_str());
		c_123456->Close();
		
		if(Tab_cut == "True"){
			double err_QCD_EMEnriched_all_123456, err_QCD_BCtoE_all_123456, err_TTbar_123456, err_Wlnu_123456, err_WWEE_123456, err_WZEE_123456, err_ZZEE_123456, err_Z_123456, err_Data_123456;
			
			cut<<endl<<Tabcut_title.c_str()<<c_name.c_str()<<endl;
			cut<<endl<<Tabcut_QCD_EMEnriched_all_Int.c_str()<<((float)((int)(histoQCD_EMEnriched_all_123456->IntegralAndError(0,-1,err_QCD_EMEnriched_all_123456,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_QCD_EMEnriched_all_Int_err.c_str()<<((float)((int)(err_QCD_EMEnriched_all_123456*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_QCD_BCtoE_all_Int.c_str()<<((float)((int)(histoQCD_BCtoE_all_123456->IntegralAndError(0,-1,err_QCD_BCtoE_all_123456,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_QCD_BCtoE_all_Int_err.c_str()<<((float)((int)(err_QCD_BCtoE_all_123456*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_123456->IntegralAndError(0,-1,err_TTbar_123456,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_123456*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_Wlnu_Int.c_str()<<((float)((int)(histoWlnu_123456->IntegralAndError(0,-1,err_Wlnu_123456,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Wlnu_Int_err.c_str()<<((float)((int)(err_Wlnu_123456*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_WWEE_Int.c_str()<<((float)((int)(histoWWEE_123456->IntegralAndError(0,-1,err_WWEE_123456,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_WWEE_Int_err.c_str()<<((float)((int)(err_WWEE_123456*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_WZEE_Int.c_str()<<((float)((int)(histoWZEE_123456->IntegralAndError(0,-1,err_WZEE_123456,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_WZEE_Int_err.c_str()<<((float)((int)(err_WZEE_123456*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_ZZEE_Int.c_str()<<((float)((int)(histoZZEE_123456->IntegralAndError(0,-1,err_ZZEE_123456,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_ZZEE_Int_err.c_str()<<((float)((int)(err_ZZEE_123456*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_123456->IntegralAndError(0,-1,err_Z_123456,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_123456*cut_decimal)))/cut_decimal<<endl;
			cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
			cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_EMEnriched_all_123456->IntegralAndError(0,-1,err_QCD_EMEnriched_all_123456,"") + histoQCD_BCtoE_all_123456->IntegralAndError(0,-1,err_QCD_BCtoE_all_123456,"") + histoTTbar_123456->IntegralAndError(0,-1,err_TTbar_123456,"") + histoWlnu_123456->IntegralAndError(0,-1,err_Wlnu_123456,"") + histoWWEE_123456->IntegralAndError(0,-1,err_WWEE_123456,"") + histoWZEE_123456->IntegralAndError(0,-1,err_WZEE_123456,"") + histoZZEE_123456->IntegralAndError(0,-1,err_ZZEE_123456,"") + histoZ_123456->IntegralAndError(0,-1,err_Z_123456,""))*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_EMEnriched_all_123456),2)+pow((err_QCD_BCtoE_all_123456),2) + pow((err_TTbar_123456),2) + pow((err_Wlnu_123456),2) + pow((err_WWEE_123456),2) + pow((err_WZEE_123456),2) + pow((err_ZZEE_123456),2) + pow((err_Z_123456),2))*cut_decimal)))/cut_decimal<<endl<<endl;
			cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_123456->IntegralAndError(0,-1,err_Data_123456,"")*cut_decimal)))/cut_decimal<<endl;
			cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_123456*cut_decimal)))/cut_decimal<<endl;
			cut<<endl<<Tabcut_end.c_str()<<endl;
		}
		
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
	
	if(b<9)Stackd_name+=_RecoCutFlags[1].c_str();
	
	Stackd_1->Add(histoQCD_EMEnriched_all_1);
	Stackd_1->Add(histoQCD_BCtoE_all_1);	
	Stackd_1->Add(histoTTbar_1);
	Stackd_1->Add(histoWlnu_1);
	Stackd_1->Add(histoWWEE_1);
	Stackd_1->Add(histoWZEE_1);
	Stackd_1->Add(histoZZEE_1);
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
	StackLeg_1->AddEntry(histoWlnu_1,Leg_Wlnu.c_str());
	StackLeg_1->AddEntry(histoWWEE_1,Leg_WWEE.c_str());
	StackLeg_1->AddEntry(histoWZEE_1,Leg_WZEE.c_str());
	StackLeg_1->AddEntry(histoZZEE_1,Leg_ZZEE.c_str());
	StackLeg_1->AddEntry(histoZ_1,Leg_Z.c_str());
	StackLeg_1->AddEntry(histoData_1,Leg_Data.c_str());	
	
	StackLeg_1->Draw("LP");
	
	d_1->Write(Stackd_name.c_str());
	d_1->Close();
	
	if(b<9){
	
	if(_RecoCutFlags[2] != "_1"){
		
		TCanvas *d_12 = new TCanvas();
		
		if (log_scale == "True") {d_12->SetLogy();}
		
		THStack *Stackd_12 = new THStack();
		
		Stackd_name+=_RecoCutFlags[2].c_str();
		
		Stackd_12->Add(histoQCD_EMEnriched_all_12);
		Stackd_12->Add(histoQCD_BCtoE_all_12);	
		Stackd_12->Add(histoTTbar_12);
		Stackd_12->Add(histoWlnu_12);
		Stackd_12->Add(histoWWEE_12);
		Stackd_12->Add(histoWZEE_12);
		Stackd_12->Add(histoZZEE_12);
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
		StackLeg_12->AddEntry(histoWlnu_12,Leg_Wlnu.c_str());
		StackLeg_12->AddEntry(histoWWEE_12,Leg_WWEE.c_str());
		StackLeg_12->AddEntry(histoWZEE_12,Leg_WZEE.c_str());
		StackLeg_12->AddEntry(histoZZEE_12,Leg_ZZEE.c_str());
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
		Stackd_123->Add(histoWlnu_123);
		Stackd_123->Add(histoWWEE_123);
		Stackd_123->Add(histoWZEE_123);
		Stackd_123->Add(histoZZEE_123);
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
		StackLeg_123->AddEntry(histoWlnu_123,Leg_Wlnu.c_str());
		StackLeg_123->AddEntry(histoWWEE_123,Leg_WWEE.c_str());
		StackLeg_123->AddEntry(histoWZEE_123,Leg_WZEE.c_str());
		StackLeg_123->AddEntry(histoZZEE_123,Leg_ZZEE.c_str());
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
		Stackd_1234->Add(histoWlnu_1234);
		Stackd_1234->Add(histoWWEE_1234);
		Stackd_1234->Add(histoWZEE_1234);
		Stackd_1234->Add(histoZZEE_1234);
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
		StackLeg_1234->AddEntry(histoWlnu_1234,Leg_Wlnu.c_str());
		StackLeg_1234->AddEntry(histoWWEE_1234,Leg_WWEE.c_str());
		StackLeg_1234->AddEntry(histoWZEE_1234,Leg_WZEE.c_str());
		StackLeg_1234->AddEntry(histoZZEE_1234,Leg_ZZEE.c_str());
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
		Stackd_12345->Add(histoWlnu_12345);
		Stackd_12345->Add(histoWWEE_12345);
		Stackd_12345->Add(histoWZEE_12345);
		Stackd_12345->Add(histoZZEE_12345);
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
		StackLeg_12345->AddEntry(histoWlnu_12345,Leg_Wlnu.c_str());
		StackLeg_12345->AddEntry(histoWWEE_12345,Leg_WWEE.c_str());
		StackLeg_12345->AddEntry(histoWZEE_12345,Leg_WZEE.c_str());
		StackLeg_12345->AddEntry(histoZZEE_12345,Leg_ZZEE.c_str());
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
		Stackd_123456->Add(histoWlnu_123456);
		Stackd_123456->Add(histoWWEE_123456);
		Stackd_123456->Add(histoWZEE_123456);
		Stackd_123456->Add(histoZZEE_123456);
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
		StackLeg_123456->AddEntry(histoWlnu_123456,Leg_Wlnu.c_str());
		StackLeg_123456->AddEntry(histoWWEE_123456,Leg_WWEE.c_str());
		StackLeg_123456->AddEntry(histoWZEE_123456,Leg_WZEE.c_str());
		StackLeg_123456->AddEntry(histoZZEE_123456,Leg_ZZEE.c_str());
		StackLeg_123456->AddEntry(histoZ_123456,Leg_Z.c_str());
		StackLeg_123456->AddEntry(histoData_123456,Leg_Data.c_str());	
	   	
		StackLeg_123456->Draw("LP");
		
		d_123456->Write(Stackd_name.c_str());
		d_123456->Close();
	}
		
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
