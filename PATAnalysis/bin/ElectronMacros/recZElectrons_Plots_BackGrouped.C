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

gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);

string log_scale = "True";
string Tab_cut = "True";

//Start histo (default = 1)
int b_ini = 1;
//End histo (max. = 21)
int b_end = 21; 

        //Selections
	int _Acc  = 1;
	int _Trg  = 2;
	int _Imp  = 3;
	int _Conv = 6;
	int _Iso  = 4;
	int _EiD  = 5;        
                
	//Background MC
	TFile* QCD_TF = TFile::Open("/data/sfrosali/Zjets/AnalisiSelezioni/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/MC_Winter10_123645/Background/QCD_All.root");
	TFile* EWK_TF = TFile::Open("/data/sfrosali/Zjets/AnalisiSelezioni/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/MC_Winter10_123645/Background/EWK_All.root");
	TFile* TTbar_TF = TFile::Open("/data/sfrosali/Zjets/AnalisiSelezioni/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/MC_Winter10_123645/Background/TT_Pythia.root");
	
	//Signal MC
	TFile *Z_TF = TFile::Open("/data/sfrosali/Zjets/AnalisiSelezioni/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/MC_Winter10_123645/Signal/Z_Madgraph_Z2.root");
	
	//Data
	TFile *Data_TF = TFile::Open("/data/sfrosali/Zjets/AnalisiSelezioni/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/Data_123645/Data_RUN2010A-B.root");
	
	//Output
	string out = "recZElectrons_123645";        
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
	int rebin_recLeadElfBrem = 2; 
	int rebin_recLeadElConvCotDist = 2;
	int rebin_recLeadElConvMissHit = 1;
	//Second Electron
	int rebin_recSecElPt = 2;	
	int rebin_recSecElEta = 2;  
	int rebin_recSecElfBrem = 2; 
	int rebin_recSecElConvCotDist = 2;
	int rebin_recSecElConvMissHit = 1;
	//IP
	int rebin_recElIP = 1;
	//EiD variables - Barrel
	int rebin_DeltaEtaIn_Barrel = 1;	
	int rebin_DeltaPhiIn_Barrel = 1;
	int rebin_HoverE_Barrel = 1;
	int rebin_SigmaIEtaIEta_Barrel = 1;
	//EiD variables - Endcap
	int rebin_DeltaEtaIn_Endcap = 1;	
	int rebin_DeltaPhiIn_Endcap = 1;
	int rebin_HoverE_Endcap = 1;
	int rebin_SigmaIEtaIEta_Endcap = 1;
	//Iso variables
	int rebin_RelIso_Barrel = 1;	
	int rebin_RelIso_Endcap = 1;
	
	//colors	
	int col_Z = kOrange;
	int col_QCD = kMagenta+1;
	int col_EWK = kOrange+8;
	int col_TTbar = kCyan+1;
	int col_Data = 1;

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
	//LeadElfBrem	
	double nminX_recLeadElfBrem = 0.0; 
	double nmaxX_recLeadElfBrem = 4.0;
	double nminY_recLeadElfBrem = 0.1; 
	double nmaxY_recLeadElfBrem = 3000.0;
	//Lead Conv CotDist
	double nminX_recLeadElConvCotDist = 0.0; 
	double nmaxX_recLeadElConvCotDist = 0.2;
	double nminY_recLeadElConvCotDist = 0.1; 
	double nmaxY_recLeadElConvCotDist = 1000.;
	//Lead Conv Missing Hit
	double nminX_recLeadElConvMissHit = 0.0; 
	double nmaxX_recLeadElConvMissHit = 7.;
	double nminY_recLeadElConvMissHit = 0.1; 
	double nmaxY_recLeadElConvMissHit = 100000.;	
	//Lead Conv CotDist
	double nminX_recSecElConvCotDist = 0.0; 
	double nmaxX_recSecElConvCotDist = 0.2;
	double nminY_recSecElConvCotDist = 0.1; 
	double nmaxY_recSecElConvCotDist = 1000.;
	//Sec Conv Missing Hit
	double nminX_recSecElConvMissHit = 0.0; 
	double nmaxX_recSecElConvMissHit = 7.;
	double nminY_recSecElConvMissHit = 0.1; 
	double nmaxY_recSecElConvMissHit = 100000.;	
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
	//SecElfBrem	
	double nminX_recSecElfBrem = 0.0; 
	double nmaxX_recSecElfBrem = 4.0;
	double nminY_recSecElfBrem = 0.1; 
	double nmaxY_recSecElfBrem = 3000.0;	
	//ElIP
	double nminX_recElIP = 0.0; 
	double nmaxX_recElIP = 1.0;
	double nminY_recElIP = 0.1; 
	double nmaxY_recElIP = 25000.0;
	//DeltaEtaIn - Barrel
	double nminX_DeltaEtaIn_Barrel = -0.1; 
	double nmaxX_DeltaEtaIn_Barrel = 0.1;
	double nminY_DeltaEtaIn_Barrel = 0.1; 
	double nmaxY_DeltaEtaIn_Barrel = 10000.0;
	//DeltaPhiIn -Barrel
	double nminX_DeltaPhiIn_Barrel = -0.30; 
	double nmaxX_DeltaPhiIn_Barrel = 0.30;
	double nminY_DeltaPhiIn_Barrel = 0.1; 
	double nmaxY_DeltaPhiIn_Barrel = 10000.0;
	//HoverE -Barrel
	double nminX_HoverE_Barrel = 0.0; 
	double nmaxX_HoverE_Barrel = 0.1;
	double nminY_HoverE_Barrel = 0.1; 
	double nmaxY_HoverE_Barrel = 25000.0;
	//SigmaIEtaIEta - Barrel
	double nminX_SigmaIEtaIEta_Barrel = 0.0; 
	double nmaxX_SigmaIEtaIEta_Barrel = 0.035;
	double nminY_SigmaIEtaIEta_Barrel = 0.1; 
	double nmaxY_SigmaIEtaIEta_Barrel = 10000.0;	
	//DeltaEtaIn - Endcap
	double nminX_DeltaEtaIn_Endcap = -0.1; 
	double nmaxX_DeltaEtaIn_Endcap = 0.1;
	double nminY_DeltaEtaIn_Endcap = 0.1; 
	double nmaxY_DeltaEtaIn_Endcap = 10000.0;
	//DeltaPhiIn -Endcap
	double nminX_DeltaPhiIn_Endcap = -0.30; 
	double nmaxX_DeltaPhiIn_Endcap = 0.30;
	double nminY_DeltaPhiIn_Endcap = 0.1; 
	double nmaxY_DeltaPhiIn_Endcap = 10000.0;
	//HoverE -Endcap
	double nminX_HoverE_Endcap = 0.0; 
	double nmaxX_HoverE_Endcap = 0.1;
	double nminY_HoverE_Endcap = 0.1; 
	double nmaxY_HoverE_Endcap = 25000.0;
	//SigmaIEtaIEta - Endcap
	double nminX_SigmaIEtaIEta_Endcap = 0.0; 
	double nmaxX_SigmaIEtaIEta_Endcap = 0.035;
	double nminY_SigmaIEtaIEta_Endcap = 0.1; 
	double nmaxY_SigmaIEtaIEta_Endcap = 10000.0;		
	//CombRelIso - Barrel
	double nminX_RelIso_Barrel = 0.0; 
	double nmaxX_RelIso_Barrel = 0.5;
	double nminY_RelIso_Barrel = 0.1; 
	double nmaxY_RelIso_Barrel = 2700.0;
	//CombRelIso - Endcap
	double nminX_RelIso_Endcap = 0.0; 
	double nmaxX_RelIso_Endcap = 0.5;
	double nminY_RelIso_Endcap = 0.1; 
	double nmaxY_RelIso_Endcap = 2700.0;
	
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
string cartella2a = "recZElectrons_Plots";	

string cartellaplot;

//grafici
int rebin = 0;
double nminX = 0., nmaxX = 0., nminY = 0. , nmaxY = 0.;

string grafico_name;
string grafico_name_Cartella2a_recLeadElPt = "recLeadElPt";
string grafico_name_Cartella2a_recLeadElEta = "recLeadElEta";
string grafico_name_Cartella2a_recLeadElfBrem = "recLeadElfBrem";
string grafico_name_Cartella2a_recLeadElConvCotDist = "recLeadElConvCotDist";
string grafico_name_Cartella2a_recLeadElConvMissHit = "recLeadElConvMissHit";
string grafico_name_Cartella2a_recSecElPt = "recSecElPt";
string grafico_name_Cartella2a_recSecElEta = "recSecElEta";
string grafico_name_Cartella2a_recSecElfBrem = "recSecElfBrem";
string grafico_name_Cartella2a_recSecElConvCotDist = "recSecElConvCotDist";
string grafico_name_Cartella2a_recSecElConvMissHit = "recSecElConvMissHit";
string grafico_name_Cartella2a_recElIP = "recElIP";
string grafico_name_Cartella2a_DeltaEtaIn_Barrel_PreEiD = "DeltaEtaIn_Barrel_PreEiD";
string grafico_name_Cartella2a_DeltaPhiIn_Barrel_PreEiD = "DeltaPhiIn_Barrel_PreEiD";
string grafico_name_Cartella2a_HoverE_Barrel_PreEiD = "HoverE_Barrel_PreEiD";
string grafico_name_Cartella2a_SigmaIEtaIEta_Barrel_PreEiD = "SigmaIEtaIEta_Barrel_PreEiD";
string grafico_name_Cartella2a_RelIso_Barrel_PreIso = "recRelIso_Barrel_PreIso";
string grafico_name_Cartella2a_DeltaEtaIn_Endcap_PreEiD = "DeltaEtaIn_Endcap_PreEiD";
string grafico_name_Cartella2a_DeltaPhiIn_Endcap_PreEiD = "DeltaPhiIn_Endcap_PreEiD";
string grafico_name_Cartella2a_HoverE_Endcap_PreEiD = "HoverE_Endcap_PreEiD";
string grafico_name_Cartella2a_SigmaIEtaIEta_Endcap_PreEiD = "SigmaIEtaIEta_Endcap_PreEiD";
string grafico_name_Cartella2a_RelIso_Endcap_PreIso = "recRelIso_Endcap_PreIso";

string asseX_name;
string asseY_name;
string asseX_name_Cartella2a_recLeadElPt = "Reconstructed Leading Electron P_{T} (GeV/c))";
string asseY_name_Cartella2a_recLeadElPt = Form("Events/(%.0f GeV/c)", 1.0*rebin_recLeadElPt); //bins da 1 GeV/c
string asseX_name_Cartella2a_recLeadElEta = "Reconstructed Leading Electron #eta"; 
string asseY_name_Cartella2a_recLeadElEta = Form("Events/(%.1f)", 0.05*rebin_recLeadElEta); //bins da 0.05
string asseX_name_Cartella2a_recLeadElfBrem = "Reconstructed Leading Electron fBrem";
string asseY_name_Cartella2a_recLeadElfBrem = Form("Events/(%.2f)", 0.02*rebin_recLeadElfBrem); //bins da 0.02
string asseX_name_Cartella2a_recLeadElConvCotDist = "Reco Leading Electron Conv - sqrt(Cot2+Dist2)";
string asseY_name_Cartella2a_recLeadElConvCotDist = Form("Events/(%.3f)", 0.002*rebin_recLeadElConvCotDist); //bins da 0.002
string asseX_name_Cartella2a_recLeadElConvMissHit = "Reconstructed Leading Electron Conv - Missing Hit";
string asseY_name_Cartella2a_recLeadElConvMissHit = Form("Events/(%.1f)",1.0*rebin_recLeadElConvMissHit); //bins da 1
string asseX_name_Cartella2a_recSecElPt = "Reconstructed Second Electron P_{T} (GeV/c))";
string asseY_name_Cartella2a_recSecElPt = Form("Events/(%.0f GeV/c)", 1.0*rebin_recSecElPt); //bins da 1 GeV/c
string asseX_name_Cartella2a_recSecElEta = "Reconstructed Second Electron #eta"; 
string asseY_name_Cartella2a_recSecElEta = Form("Events/(%.1f)", 0.05*rebin_recSecElEta); //bins da 0.05
string asseX_name_Cartella2a_recSecElfBrem = "Reconstructed Second Electron fBrem";
string asseY_name_Cartella2a_recSecElfBrem = Form("Events/(%.2f)", 0.02*rebin_recSecElfBrem); //bins da 0.02
string asseX_name_Cartella2a_recSecElConvCotDist = "Reco Second Electron Conv - sqrt(Cot2+Dist2)";
string asseY_name_Cartella2a_recSecElConvCotDist = Form("Events/(%.3f)", 0.002*rebin_recSecElConvCotDist); //bins da 0.002
string asseX_name_Cartella2a_recSecElConvMissHit = "Reconstructed Second Electron Conv - Missing Hit";
string asseY_name_Cartella2a_recSecElConvMissHit = Form("Events/(%.1f)",1.0*rebin_recSecElConvMissHit); //bins da 1
string asseX_name_Cartella2a_recElIP = "Reconstructed Electron IP (cm)";
string asseY_name_Cartella2a_recElIP = Form("Events/(%.3f)", 0.001*rebin_recElIP); //bins da 0.001
string asseX_name_Cartella2a_DeltaEtaIn_Barrel_PreEiD = "Reconstructed Electron Delta Eta Barrel - Pre EiD cut";
string asseY_name_Cartella2a_DeltaEtaIn_Barrel_PreEiD = Form("Events/(%.3f)", 0.002*rebin_DeltaEtaIn_Barrel); //bins da 0.002
string asseX_name_Cartella2a_DeltaPhiIn_Barrel_PreEiD = "Reconstructed Electron Delta Phi Barrel - Pre EiD cut";
string asseY_name_Cartella2a_DeltaPhiIn_Barrel_PreEiD = Form("Events/(%.3f)", 0.008*rebin_DeltaPhiIn_Barrel); //bins da 0.008
string asseX_name_Cartella2a_HoverE_Barrel_PreEiD = "Reconstructed Electron H over E Barrel - Pre EiD cut";
string asseY_name_Cartella2a_HoverE_Barrel_PreEiD = Form("Events/(%.3f)", 0.001*rebin_HoverE_Barrel); //bins da 0.001
string asseX_name_Cartella2a_SigmaIEtaIEta_Barrel_PreEiD = "Reconstructed Electron SigmaIEtaIEta Barrel - Pre EiD cut";
string asseY_name_Cartella2a_SigmaIEtaIEta_Barrel_PreEiD = Form("Events/(%.3f)", 0.001*rebin_SigmaIEtaIEta_Barrel); //bins da 0.001
string asseX_name_Cartella2a_RelIso_Barrel_PreIso = "Reconstructed Electron Rel. Isolation Barrel - Pre Iso cut";
string asseY_name_Cartella2a_RelIso_Barrel_PreIso = Form("Events/(%.2f)", 0.01*rebin_RelIso_Barrel); //bins da 0.01
string asseX_name_Cartella2a_DeltaEtaIn_Endcap_PreEiD = "Reconstructed Electron Delta Eta Endcap - Pre EiD cut";
string asseY_name_Cartella2a_DeltaEtaIn_Endcap_PreEiD = Form("Events/(%.3f)", 0.002*rebin_DeltaEtaIn_Endcap); //bins da 0.002
string asseX_name_Cartella2a_DeltaPhiIn_Endcap_PreEiD = "Reconstructed Electron Delta Phi Endcap - Pre EiD cut";
string asseY_name_Cartella2a_DeltaPhiIn_Endcap_PreEiD = Form("Events/(%.3f)", 0.008*rebin_DeltaPhiIn_Endcap); //bins da 0.008
string asseX_name_Cartella2a_HoverE_Endcap_PreEiD = "Reconstructed Electron H over E Endcap - Pre EiD cut";
string asseY_name_Cartella2a_HoverE_Endcap_PreEiD = Form("Events/(%.3f)", 0.001*rebin_HoverE_Endcap); //bins da 0.001
string asseX_name_Cartella2a_SigmaIEtaIEta_Endcap_PreEiD = "Reconstructed Electron SigmaIEtaIEta Endcap - Pre EiD cut";
string asseY_name_Cartella2a_SigmaIEtaIEta_Endcap_PreEiD = Form("Events/(%.3f)", 0.001*rebin_SigmaIEtaIEta_Endcap); //bins da 0.001
string asseX_name_Cartella2a_RelIso_Endcap_PreIso = "Reconstructed Electron Rel. Isolation Endcap - Pre Iso cut";
string asseY_name_Cartella2a_RelIso_Endcap_PreIso = Form("Events/(%.2f)", 0.01*rebin_RelIso_Endcap); //bins da 0.01


TDirectory *Dir_1;
Dir_1 = outplots->mkdir(cartella1.c_str());
Dir_1->cd();


//--------------------------------------- directory recZElectrons_Plots

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

//----------------------------------------- Grafici cartella recZElectrons_Plots

for(int b=b_ini; b<b_end+1; b++){
	
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

else if(b==3){cartellaplot = grafico_name_Cartella2a_recElIP;
	      grafico_name = grafico_name_Cartella2a_recElIP;
	      asseX_name = asseX_name_Cartella2a_recElIP; 
	      asseY_name = asseY_name_Cartella2a_recElIP;
	      rebin = rebin_recElIP;
	      nminX = nminX_recElIP;
	      nmaxX = nmaxX_recElIP;
	      nminY = nminY_recElIP;
	      nmaxY = nmaxY_recElIP;}

else if(b==4){cartellaplot = grafico_name_Cartella2a_recLeadElfBrem;
	      grafico_name = grafico_name_Cartella2a_recLeadElfBrem;
	      asseX_name = asseX_name_Cartella2a_recLeadElfBrem; 
	      asseY_name = asseY_name_Cartella2a_recLeadElfBrem;
	      rebin = rebin_recLeadElfBrem;
	      nminX = nminX_recLeadElfBrem;
	      nmaxX = nmaxX_recLeadElfBrem;
	      nminY = nminY_recLeadElfBrem;
	      nmaxY = nmaxY_recLeadElfBrem;}
	      
else if(b==5){cartellaplot = grafico_name_Cartella2a_recLeadElConvCotDist;
	      grafico_name = grafico_name_Cartella2a_recLeadElConvCotDist;
	      asseX_name = asseX_name_Cartella2a_recLeadElConvCotDist; 
	      asseY_name = asseY_name_Cartella2a_recLeadElConvCotDist;
	      rebin = rebin_recLeadElConvCotDist;
	      nminX = nminX_recLeadElConvCotDist;
	      nmaxX = nmaxX_recLeadElConvCotDist;
	      nminY = nminY_recLeadElConvCotDist;
	      nmaxY = nmaxY_recLeadElConvCotDist;}
	     
else if(b==6){cartellaplot = grafico_name_Cartella2a_recLeadElConvMissHit;
	      grafico_name = grafico_name_Cartella2a_recLeadElConvMissHit;
	      asseX_name = asseX_name_Cartella2a_recLeadElConvMissHit; 
	      asseY_name = asseY_name_Cartella2a_recLeadElConvMissHit;
	      rebin = rebin_recLeadElConvMissHit;
	      nminX = nminX_recLeadElConvMissHit;
	      nmaxX = nmaxX_recLeadElConvMissHit;
	      nminY = nminY_recLeadElConvMissHit;
	      nmaxY = nmaxY_recLeadElConvMissHit;}
	      
else if(b==7){cartellaplot = grafico_name_Cartella2a_recSecElPt;
	      grafico_name = grafico_name_Cartella2a_recSecElPt;
	      asseX_name = asseX_name_Cartella2a_recSecElPt; 
	      asseY_name = asseY_name_Cartella2a_recSecElPt;
	      rebin = rebin_recSecElPt;
	      nminX = nminX_recSecElPt;
	      nmaxX = nmaxX_recSecElPt;
	      nminY = nminY_recSecElPt;
	      nmaxY = nmaxY_recSecElPt;}

else if(b==8){cartellaplot = grafico_name_Cartella2a_recSecElEta;
	      grafico_name = grafico_name_Cartella2a_recSecElEta;
	      asseX_name = asseX_name_Cartella2a_recSecElEta; 
	      asseY_name = asseY_name_Cartella2a_recSecElEta;
	      rebin = rebin_recSecElEta;
	      nminX = nminX_recSecElEta;
	      nmaxX = nmaxX_recSecElEta;
	      nminY = nminY_recSecElEta;
	      nmaxY = nmaxY_recSecElEta;}

else if(b==9){cartellaplot = grafico_name_Cartella2a_recSecElfBrem;
	      grafico_name = grafico_name_Cartella2a_recSecElfBrem;
	      asseX_name = asseX_name_Cartella2a_recSecElfBrem; 
	      asseY_name = asseY_name_Cartella2a_recSecElfBrem;
	      rebin = rebin_recSecElfBrem;
	      nminX = nminX_recSecElfBrem;
	      nmaxX = nmaxX_recSecElfBrem;
	      nminY = nminY_recSecElfBrem;
	      nmaxY = nmaxY_recSecElfBrem;}
	      
else if(b==10){cartellaplot = grafico_name_Cartella2a_recSecElConvCotDist;
	      grafico_name = grafico_name_Cartella2a_recSecElConvCotDist;
	      asseX_name = asseX_name_Cartella2a_recSecElConvCotDist; 
	      asseY_name = asseY_name_Cartella2a_recSecElConvCotDist;
	      rebin = rebin_recSecElConvCotDist;
	      nminX = nminX_recSecElConvCotDist;
	      nmaxX = nmaxX_recSecElConvCotDist;
	      nminY = nminY_recSecElConvCotDist;
	      nmaxY = nmaxY_recSecElConvCotDist;}
	     
else if(b==11){cartellaplot = grafico_name_Cartella2a_recSecElConvMissHit;
	      grafico_name = grafico_name_Cartella2a_recSecElConvMissHit;
	      asseX_name = asseX_name_Cartella2a_recSecElConvMissHit; 
	      asseY_name = asseY_name_Cartella2a_recSecElConvMissHit;
	      rebin = rebin_recSecElConvMissHit;
	      nminX = nminX_recSecElConvMissHit;
	      nmaxX = nmaxX_recSecElConvMissHit;
	      nminY = nminY_recSecElConvMissHit;
	      nmaxY = nmaxY_recSecElConvMissHit;}

else if(b==12){cartellaplot = grafico_name_Cartella2a_DeltaEtaIn_Barrel_PreEiD;
	grafico_name = grafico_name_Cartella2a_DeltaEtaIn_Barrel_PreEiD;
 	asseX_name = asseX_name_Cartella2a_DeltaEtaIn_Barrel_PreEiD; 
	asseY_name = asseY_name_Cartella2a_DeltaEtaIn_Barrel_PreEiD;
	rebin = rebin_DeltaEtaIn_Barrel;
	nminX = nminX_DeltaEtaIn_Barrel;
	nmaxX = nmaxX_DeltaEtaIn_Barrel;
	nminY = nminY_DeltaEtaIn_Barrel;
	nmaxY = nmaxY_DeltaEtaIn_Barrel;}
	
else if(b==13){cartellaplot = grafico_name_Cartella2a_DeltaPhiIn_Barrel_PreEiD;
	grafico_name = grafico_name_Cartella2a_DeltaPhiIn_Barrel_PreEiD;
	asseX_name = asseX_name_Cartella2a_DeltaPhiIn_Barrel_PreEiD; 
	asseY_name = asseY_name_Cartella2a_DeltaPhiIn_Barrel_PreEiD;
	rebin = rebin_DeltaPhiIn_Barrel;
	nminX = nminX_DeltaPhiIn_Barrel;
	nmaxX = nmaxX_DeltaPhiIn_Barrel;
	nminY = nminY_DeltaPhiIn_Barrel;
	nmaxY = nmaxY_DeltaPhiIn_Barrel;}
	
else if(b==14){cartellaplot = grafico_name_Cartella2a_HoverE_Barrel_PreEiD;
	grafico_name = grafico_name_Cartella2a_HoverE_Barrel_PreEiD;
	asseX_name = asseX_name_Cartella2a_HoverE_Barrel_PreEiD; 
	asseY_name = asseY_name_Cartella2a_HoverE_Barrel_PreEiD;
	rebin = rebin_HoverE_Barrel;
	nminX = nminX_HoverE_Barrel;
	nmaxX = nmaxX_HoverE_Barrel;
	nminY = nminY_HoverE_Barrel;
	nmaxY = nmaxY_HoverE_Barrel;}
	
else if(b==15){cartellaplot = grafico_name_Cartella2a_SigmaIEtaIEta_Barrel_PreEiD;
	grafico_name = grafico_name_Cartella2a_SigmaIEtaIEta_Barrel_PreEiD;
	asseX_name = asseX_name_Cartella2a_SigmaIEtaIEta_Barrel_PreEiD; 
	asseY_name = asseY_name_Cartella2a_SigmaIEtaIEta_Barrel_PreEiD;
	rebin = rebin_SigmaIEtaIEta_Barrel;
	nminX = nminX_SigmaIEtaIEta_Barrel;
	nmaxX = nmaxX_SigmaIEtaIEta_Barrel;
	nminY = nminY_SigmaIEtaIEta_Barrel;
	nmaxY = nmaxY_SigmaIEtaIEta_Barrel;}
	
else if(b==16){cartellaplot = grafico_name_Cartella2a_DeltaEtaIn_Endcap_PreEiD;
	grafico_name = grafico_name_Cartella2a_DeltaEtaIn_Endcap_PreEiD;
	asseX_name = asseX_name_Cartella2a_DeltaEtaIn_Endcap_PreEiD; 
	asseY_name = asseY_name_Cartella2a_DeltaEtaIn_Endcap_PreEiD;
	rebin = rebin_DeltaEtaIn_Endcap;
	nminX = nminX_DeltaEtaIn_Endcap;
	nmaxX = nmaxX_DeltaEtaIn_Endcap;
	nminY = nminY_DeltaEtaIn_Endcap;
	nmaxY = nmaxY_DeltaEtaIn_Endcap;}
	
else if(b==17){cartellaplot = grafico_name_Cartella2a_DeltaPhiIn_Endcap_PreEiD;
	grafico_name = grafico_name_Cartella2a_DeltaPhiIn_Endcap_PreEiD;
	asseX_name = asseX_name_Cartella2a_DeltaPhiIn_Endcap_PreEiD; 
	asseY_name = asseY_name_Cartella2a_DeltaPhiIn_Endcap_PreEiD;
	rebin = rebin_DeltaPhiIn_Endcap;
	nminX = nminX_DeltaPhiIn_Endcap;
	nmaxX = nmaxX_DeltaPhiIn_Endcap;
	nminY = nminY_DeltaPhiIn_Endcap;
	nmaxY = nmaxY_DeltaPhiIn_Endcap;}
	
else if(b==18){cartellaplot = grafico_name_Cartella2a_HoverE_Endcap_PreEiD;
	grafico_name = grafico_name_Cartella2a_HoverE_Endcap_PreEiD;
	asseX_name = asseX_name_Cartella2a_HoverE_Endcap_PreEiD; 
	asseY_name = asseY_name_Cartella2a_HoverE_Endcap_PreEiD;
	rebin = rebin_HoverE_Endcap;
	nminX = nminX_HoverE_Endcap;
	nmaxX = nmaxX_HoverE_Endcap;
	nminY = nminY_HoverE_Endcap;
	nmaxY = nmaxY_HoverE_Endcap;}
	
else if(b==19){cartellaplot = grafico_name_Cartella2a_SigmaIEtaIEta_Endcap_PreEiD;
	grafico_name = grafico_name_Cartella2a_SigmaIEtaIEta_Endcap_PreEiD;
	asseX_name = asseX_name_Cartella2a_SigmaIEtaIEta_Endcap_PreEiD; 
	asseY_name = asseY_name_Cartella2a_SigmaIEtaIEta_Endcap_PreEiD;
	rebin = rebin_SigmaIEtaIEta_Endcap;
	nminX = nminX_SigmaIEtaIEta_Endcap;
	nmaxX = nmaxX_SigmaIEtaIEta_Endcap;
	nminY = nminY_SigmaIEtaIEta_Endcap;
	nmaxY = nmaxY_SigmaIEtaIEta_Endcap;}
	
else if(b==20){cartellaplot = grafico_name_Cartella2a_RelIso_Barrel_PreIso;
	grafico_name = grafico_name_Cartella2a_RelIso_Barrel_PreIso;
	asseX_name = asseX_name_Cartella2a_RelIso_Barrel_PreIso; 
	asseY_name = asseY_name_Cartella2a_RelIso_Barrel_PreIso;
	rebin = rebin_RelIso_Barrel;
	nminX = nminX_RelIso_Barrel;
	nmaxX = nmaxX_RelIso_Barrel;
	nminY = nminY_RelIso_Barrel;
	nmaxY = nmaxY_RelIso_Barrel;}
	
else if(b==21){cartellaplot = grafico_name_Cartella2a_RelIso_Endcap_PreIso;
	grafico_name = grafico_name_Cartella2a_RelIso_Endcap_PreIso;
	asseX_name = asseX_name_Cartella2a_RelIso_Endcap_PreIso; 
	asseY_name = asseY_name_Cartella2a_RelIso_Endcap_PreIso;
	rebin = rebin_RelIso_Endcap;
	nminX = nminX_RelIso_Endcap;
	nmaxX = nmaxX_RelIso_Endcap;
	nminY = nminY_RelIso_Endcap;
	nmaxY = nmaxY_RelIso_Endcap;}

TDirectory *Dir_3;
Dir_3 = Dir_2a->mkdir(cartellaplot.c_str());
Dir_3->cd();
    
	TH1D* histoQCD_1;
	TH1D* histoQCD_12;
	TH1D* histoQCD_123;
	TH1D* histoQCD_1234;
	TH1D* histoQCD_12345;
	TH1D* histoQCD_123456;
	string QCD_name = grafico_name.c_str();
	if(b>11){
	histoQCD_1 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_1->Scale(scale);
	histoQCD_1->Rebin(rebin);
	}else{
	QCD_name+=_RecoCutFlags[1].c_str();
	histoQCD_1 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_1->Scale(scale);
	histoQCD_1->Rebin(rebin);
	QCD_name+=_RecoCutFlags[2].c_str();
	histoQCD_12 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_12->Scale(scale);
	histoQCD_12->Rebin(rebin);
	QCD_name+=_RecoCutFlags[3].c_str();
	histoQCD_123 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_123->Scale(scale);
	histoQCD_123->Rebin(rebin);
	QCD_name+=_RecoCutFlags[4].c_str();
	histoQCD_1234 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_1234->Scale(scale);
	histoQCD_1234->Rebin(rebin);
	QCD_name+=_RecoCutFlags[5].c_str();
	histoQCD_12345 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_12345->Scale(scale);
	histoQCD_12345->Rebin(rebin);
	QCD_name+=_RecoCutFlags[6].c_str();
	histoQCD_123456 = (TH1D*) Cartella1_QCD_Cartella2a->Get(QCD_name.c_str());
	histoQCD_123456->Scale(scale);
	histoQCD_123456->Rebin(rebin);
	}
	
	TH1D* histoTTbar_1;
	TH1D* histoTTbar_12;
	TH1D* histoTTbar_123;
	TH1D* histoTTbar_1234;
	TH1D* histoTTbar_12345;
	TH1D* histoTTbar_123456;
	string TTbar_name = grafico_name.c_str();
	if(b>11){
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
    
	TH1D* histoEWK_1;
	TH1D* histoEWK_12;
	TH1D* histoEWK_123;
	TH1D* histoEWK_1234;
	TH1D* histoEWK_12345;
	TH1D* histoEWK_123456;
	string EWK_name = grafico_name.c_str();
	if(b>11){
		histoEWK_1 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
		histoEWK_1->Scale(scale);
		histoEWK_1->Rebin(rebin);
	}else{
	    EWK_name+=_RecoCutFlags[1].c_str();
		histoEWK_1 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
		histoEWK_1->Scale(scale);
		histoEWK_1->Rebin(rebin);
		EWK_name+=_RecoCutFlags[2].c_str();
		histoEWK_12 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
		histoEWK_12->Scale(scale);
		histoEWK_12->Rebin(rebin);
		EWK_name+=_RecoCutFlags[3].c_str();
		histoEWK_123 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
		histoEWK_123->Scale(scale);
		histoEWK_123->Rebin(rebin);
		EWK_name+=_RecoCutFlags[4].c_str();
		histoEWK_1234 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
		histoEWK_1234->Scale(scale);
		histoEWK_1234->Rebin(rebin);
		EWK_name+=_RecoCutFlags[5].c_str();
		histoEWK_12345 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
		histoEWK_12345->Scale(scale);
		histoEWK_12345->Rebin(rebin);
		EWK_name+=_RecoCutFlags[6].c_str();
		histoEWK_123456 = (TH1D*) Cartella1_EWK_Cartella2a->Get(EWK_name.c_str());
		histoEWK_123456->Scale(scale);
		histoEWK_123456->Rebin(rebin);
	}
	
	TH1D* histoZ_1;
	TH1D* histoZ_12;
	TH1D* histoZ_123;
	TH1D* histoZ_1234;
	TH1D* histoZ_12345;
	TH1D* histoZ_123456;
 	string Z_name = grafico_name.c_str();
	if(b>11){
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
	if(b>11){
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
	if(b<12)c_name+=_RecoCutFlags[1].c_str();
	
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
		cut<<endl<<Tabcut_QCD_Int.c_str()<<((float)((int)(histoQCD_1->IntegralAndError(0,-1,err_QCD_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_QCD_Int_err.c_str()<<((float)((int)(err_QCD_1*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_TTbar_Int.c_str()<<((float)((int)(histoTTbar_1->IntegralAndError(0,-1,err_TTbar_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_TTbar_Int_err.c_str()<<((float)((int)(err_TTbar_1*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_EWK_Int.c_str()<<((float)((int)(histoEWK_1->IntegralAndError(0,-1,err_EWK_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_EWK_Int_err.c_str()<<((float)((int)(err_EWK_1*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Z_Int.c_str()<<((float)((int)(histoZ_1->IntegralAndError(0,-1,err_Z_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Z_Int_err.c_str()<<((float)((int)(err_Z_1*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_riga.c_str()<<endl<<endl;
		cut<<Tabcut_Total_Int.c_str()<<((float)((int)((histoQCD_1->IntegralAndError(0,-1,err_QCD_1,"") + histoTTbar_1->IntegralAndError(0,-1,err_TTbar_1,"") + histoEWK_1->IntegralAndError(0,-1,err_EWK_1,"") + histoZ_1->IntegralAndError(0,-1,err_Z_1,""))*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Total_Int_err.c_str()<<((float)((int)(sqrt(pow((err_QCD_1),2) + pow((err_TTbar_1),2) + pow((err_EWK_1),2) + pow((err_Z_1),2))*cut_decimal)))/cut_decimal<<endl<<endl;
		cut<<Tabcut_Data_Int.c_str()<<((float)((int)(histoData_1->IntegralAndError(0,-1,err_Data_1,"")*cut_decimal)))/cut_decimal<<endl;
		cut<<Tabcut_Data_Int_err.c_str()<<((float)((int)(err_Data_1*cut_decimal)))/cut_decimal<<endl;
		cut<<endl<<Tabcut_end.c_str()<<endl;
	}
	
	
	if(b<12){
	
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
	
	if(b<12)Stackd_name+=_RecoCutFlags[1].c_str();
	
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
	
	if(b<12){
	
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
