#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TPaveStats.h"
#include "TText.h"
#include "TPaveText.h"
#include <fstream>
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TAttAxis.h"


using namespace std;

void divide() {
    
        gROOT->SetStyle("Plain");

        int _Acc  = 1;
        int _Trg  = 2;
	int _Qual = 0;
	int _Imp  = 3;
	int _Iso  = 4;
	int _EiD  = 5;
	
	string _RecoCutFlags[7];
	for(int i=0; i<7; i++){
		_RecoCutFlags[i] = "_1";}
	
	_RecoCutFlags[_Acc] =  "_Acc";
	_RecoCutFlags[_Trg] =  "_Trg";
	_RecoCutFlags[_Qual] = "_Qual";
	_RecoCutFlags[_Imp] =  "_Imp";
	_RecoCutFlags[_Iso] =  "_Iso";
	_RecoCutFlags[_EiD] =  "_EiD";

gROOT->SetStyle("Plain");
gStyle->SetOptStat(1001111);
gStyle->SetOptFit(1111);

TFile *inputFile = new TFile ("test_data.root");
        if(!inputFile){
	cout<<"Error! Input files doesn't exist!"<<endl;
	return;
	}
TFile* outplots = new TFile("SS_divide_data.root", "RECREATE");

TDirectory *PtDir = outplots->mkdir("Pt");
TDirectory *EtaDir = outplots->mkdir("Eta");
TDirectory *fBremDir = outplots->mkdir("fBrem");
TDirectory *MassDir = outplots->mkdir("InvMass");

TDirectory *OC = (TDirectory*) inputFile->Get("RecoElectron/recZElectrons_Plots");
TDirectory *SC = (TDirectory*) inputFile->Get("RecoElectron/SameSign");

//Pt
PtDir->cd();

string recLeadElPt_name = "recLeadElPt";
recLeadElPt_name+=_RecoCutFlags[1].c_str();
TH1D *PtHistoOC_1 = (TH1D*) OC->Get(recLeadElPt_name.c_str());
recLeadElPt_name+=_RecoCutFlags[2].c_str();
TH1D *PtHistoOC_12 = (TH1D*) OC->Get(recLeadElPt_name.c_str());
recLeadElPt_name+=_RecoCutFlags[3].c_str();
TH1D *PtHistoOC_123 = (TH1D*) OC->Get(recLeadElPt_name.c_str());
recLeadElPt_name+=_RecoCutFlags[4].c_str();
TH1D *PtHistoOC_1234 = (TH1D*) OC->Get(recLeadElPt_name.c_str());
recLeadElPt_name+=_RecoCutFlags[5].c_str();
TH1D *PtHistoOC_12345 = (TH1D*) OC->Get(recLeadElPt_name.c_str());
recLeadElPt_name+=_RecoCutFlags[6].c_str();
TH1D *PtHistoOC_123456 = (TH1D*) OC->Get(recLeadElPt_name.c_str());

        if(!PtHistoOC_123456){
	cout<<"Error! Cut sequence wrong!"<<endl;
	return;
	}

string recLeadElPtSC_name = "recLeadElPtSC";
recLeadElPtSC_name+=_RecoCutFlags[1].c_str();
TH1D *PtHistoSC_1 = (TH1D*) SC->Get(recLeadElPtSC_name.c_str());
recLeadElPtSC_name+=_RecoCutFlags[2].c_str();
TH1D *PtHistoSC_12 = (TH1D*) SC->Get(recLeadElPtSC_name.c_str());
recLeadElPtSC_name+=_RecoCutFlags[3].c_str();
TH1D *PtHistoSC_123 = (TH1D*) SC->Get(recLeadElPtSC_name.c_str());
recLeadElPtSC_name+=_RecoCutFlags[4].c_str();
TH1D *PtHistoSC_1234 = (TH1D*) SC->Get(recLeadElPtSC_name.c_str());
recLeadElPtSC_name+=_RecoCutFlags[5].c_str();
TH1D *PtHistoSC_12345 = (TH1D*) SC->Get(recLeadElPtSC_name.c_str());
recLeadElPtSC_name+=_RecoCutFlags[6].c_str();
TH1D *PtHistoSC_123456 = (TH1D*) SC->Get(recLeadElPtSC_name.c_str());

TCanvas *PtCanvas_1 = new TCanvas;
PtHistoSC_1->Divide(PtHistoOC_1);
PtHistoSC_1->SetTitle("Reconstructed Leading Electron Pt: Same Charge/Opposite Charge");
string PtName = "PtRatio";
PtName+=_RecoCutFlags[1].c_str();
PtHistoSC_1->SetName(PtName.c_str());
PtHistoSC_1->SetLineColor(1);
PtHistoSC_1->SetXTitle("Pt");
PtHistoSC_1->SetYTitle("Events SC/OC");
PtHistoSC_1->GetYaxis()->SetTitleOffset(1.4);
if(PtHistoSC_1->GetEntries()>10)PtHistoSC_1->Fit("pol0");
PtHistoSC_1->Write();
PtCanvas_1->Close();

TCanvas *PtCanvas_12 = new TCanvas;
PtHistoSC_12->Divide(PtHistoOC_12);
PtHistoSC_12->SetTitle("Reconstructed Leading Electron Pt: Same Charge/Opposite Charge");
PtName+=_RecoCutFlags[2].c_str();
PtHistoSC_12->SetName(PtName.c_str());
PtHistoSC_12->SetLineColor(1);
PtHistoSC_12->SetXTitle("Pt");
PtHistoSC_12->SetYTitle("Events SC/OC");
PtHistoSC_12->GetYaxis()->SetTitleOffset(1.4);
if(PtHistoSC_12->GetEntries()>10)PtHistoSC_12->Fit("pol0");
PtHistoSC_12->Write();
PtCanvas_12->Close();

TCanvas *PtCanvas_123 = new TCanvas;
PtHistoSC_123->Divide(PtHistoOC_123);
PtHistoSC_123->SetTitle("Reconstructed Leading Electron Pt: Same Charge/Opposite Charge");
PtName+=_RecoCutFlags[3].c_str();
PtHistoSC_123->SetName(PtName.c_str());
PtHistoSC_123->SetLineColor(1);
PtHistoSC_123->SetXTitle("Pt");
PtHistoSC_123->SetYTitle("Events SC/OC");
PtHistoSC_123->GetYaxis()->SetTitleOffset(1.4);
if(PtHistoSC_123->GetEntries()>10)PtHistoSC_123->Fit("pol0");
PtHistoSC_123->Write();
PtCanvas_123->Close();

TCanvas *PtCanvas_1234 = new TCanvas;
PtHistoSC_1234->Divide(PtHistoOC_1234);
PtHistoSC_1234->SetTitle("Reconstructed Leading Electron Pt: Same Charge/Opposite Charge");
PtName+=_RecoCutFlags[4].c_str();
PtHistoSC_1234->SetName(PtName.c_str());
PtHistoSC_1234->SetLineColor(1);
PtHistoSC_1234->SetXTitle("Pt");
PtHistoSC_1234->SetYTitle("Events SC/OC");
PtHistoSC_1234->GetYaxis()->SetTitleOffset(1.4);
if(PtHistoSC_1234->GetEntries()>10)PtHistoSC_1234->Fit("pol0");
PtHistoSC_1234->Write();
PtCanvas_1234->Close();

TCanvas *PtCanvas_12345 = new TCanvas;
PtHistoSC_12345->Divide(PtHistoOC_12345);
PtHistoSC_12345->SetTitle("Reconstructed Leading Electron Pt: Same Charge/Opposite Charge");
PtName+=_RecoCutFlags[5].c_str();
PtHistoSC_12345->SetName(PtName.c_str());
PtHistoSC_12345->SetLineColor(1);
PtHistoSC_12345->SetXTitle("Pt");
PtHistoSC_12345->SetYTitle("Events SC/OC");
PtHistoSC_12345->GetYaxis()->SetTitleOffset(1.4);
if(PtHistoSC_12345->GetEntries()>10)PtHistoSC_12345->Fit("pol0");
PtHistoSC_12345->Write();
PtCanvas_12345->Close();

TCanvas *PtCanvas_123456 = new TCanvas;
PtHistoSC_123456->Divide(PtHistoOC_123456);
PtHistoSC_123456->SetTitle("Reconstructed Leading Electron Pt: Same Charge/Opposite Charge");
PtName+=_RecoCutFlags[6].c_str();
PtHistoSC_123456->SetName(PtName.c_str());
PtHistoSC_123456->SetLineColor(1);
PtHistoSC_123456->SetXTitle("Pt");
PtHistoSC_123456->SetYTitle("Events SC/OC");
PtHistoSC_123456->GetYaxis()->SetTitleOffset(1.4);
if(PtHistoSC_123456->GetEntries()>10)PtHistoSC_123456->Fit("pol0");
PtHistoSC_123456->Write();
PtCanvas_123456->Close();

//Eta
EtaDir->cd();

string recLeadElEta_name = "recLeadElEta";
recLeadElEta_name+=_RecoCutFlags[1].c_str();
TH1D *EtaHistoOC_1 = (TH1D*) OC->Get(recLeadElEta_name.c_str());
recLeadElEta_name+=_RecoCutFlags[2].c_str();
TH1D *EtaHistoOC_12 = (TH1D*) OC->Get(recLeadElEta_name.c_str());
recLeadElEta_name+=_RecoCutFlags[3].c_str();
TH1D *EtaHistoOC_123 = (TH1D*) OC->Get(recLeadElEta_name.c_str());
recLeadElEta_name+=_RecoCutFlags[4].c_str();
TH1D *EtaHistoOC_1234 = (TH1D*) OC->Get(recLeadElEta_name.c_str());
recLeadElEta_name+=_RecoCutFlags[5].c_str();
TH1D *EtaHistoOC_12345 = (TH1D*) OC->Get(recLeadElEta_name.c_str());
recLeadElEta_name+=_RecoCutFlags[6].c_str();
TH1D *EtaHistoOC_123456 = (TH1D*) OC->Get(recLeadElEta_name.c_str());

string recLeadElEtaSC_name = "recLeadElEtaSC";
recLeadElEtaSC_name+=_RecoCutFlags[1].c_str();
TH1D *EtaHistoSC_1 = (TH1D*) SC->Get(recLeadElEtaSC_name.c_str());
recLeadElEtaSC_name+=_RecoCutFlags[2].c_str();
TH1D *EtaHistoSC_12 = (TH1D*) SC->Get(recLeadElEtaSC_name.c_str());
recLeadElEtaSC_name+=_RecoCutFlags[3].c_str();
TH1D *EtaHistoSC_123 = (TH1D*) SC->Get(recLeadElEtaSC_name.c_str());
recLeadElEtaSC_name+=_RecoCutFlags[4].c_str();
TH1D *EtaHistoSC_1234 = (TH1D*) SC->Get(recLeadElEtaSC_name.c_str());
recLeadElEtaSC_name+=_RecoCutFlags[5].c_str();
TH1D *EtaHistoSC_12345 = (TH1D*) SC->Get(recLeadElEtaSC_name.c_str());
recLeadElEtaSC_name+=_RecoCutFlags[6].c_str();
TH1D *EtaHistoSC_123456 = (TH1D*) SC->Get(recLeadElEtaSC_name.c_str());

TCanvas *EtaCanvas_1 = new TCanvas;
EtaHistoSC_1->Divide(EtaHistoOC_1);
EtaHistoSC_1->SetTitle("Reconstructed Leading Electron Eta: Same Charge/Opposite Charge");
string EtaName = "EtaRatio";
EtaName+=_RecoCutFlags[1].c_str();
EtaHistoSC_1->SetName(EtaName.c_str());
EtaHistoSC_1->SetLineColor(1);
EtaHistoSC_1->SetXTitle("Eta");
EtaHistoSC_1->SetYTitle("Events SC/OC");
EtaHistoSC_1->GetYaxis()->SetTitleOffset(1.4);
if(EtaHistoSC_1->GetEntries()>10)EtaHistoSC_1->Fit("pol0");
EtaHistoSC_1->Write();
EtaCanvas_1->Close();

TCanvas *EtaCanvas_12 = new TCanvas;
EtaHistoSC_12->Divide(EtaHistoOC_12);
EtaHistoSC_12->SetTitle("Reconstructed Leading Electron Eta: Same Charge/Opposite Charge");
EtaName+=_RecoCutFlags[2].c_str();
EtaHistoSC_12->SetName(EtaName.c_str());
EtaHistoSC_12->SetLineColor(1);
EtaHistoSC_12->SetXTitle("Eta");
EtaHistoSC_12->SetYTitle("Events SC/OC");
EtaHistoSC_12->GetYaxis()->SetTitleOffset(1.4);
if(EtaHistoSC_12->GetEntries()>10)EtaHistoSC_12->Fit("pol0");
EtaHistoSC_12->Write();
EtaCanvas_12->Close();

TCanvas *EtaCanvas_123 = new TCanvas;
EtaHistoSC_123->Divide(EtaHistoOC_123);
EtaHistoSC_123->SetTitle("Reconstructed Leading Electron Eta: Same Charge/Opposite Charge");
EtaName+=_RecoCutFlags[3].c_str();
EtaHistoSC_123->SetName(EtaName.c_str());
EtaHistoSC_123->SetLineColor(1);
EtaHistoSC_123->SetXTitle("Eta");
EtaHistoSC_123->SetYTitle("Events SC/OC");
EtaHistoSC_123->GetYaxis()->SetTitleOffset(1.4);
if(EtaHistoSC_123->GetEntries()>10)EtaHistoSC_123->Fit("pol0");
EtaHistoSC_123->Write();
EtaCanvas_123->Close();

TCanvas *EtaCanvas_1234 = new TCanvas;
EtaHistoSC_1234->Divide(EtaHistoOC_1234);
EtaHistoSC_1234->SetTitle("Reconstructed Leading Electron Eta: Same Charge/Opposite Charge");
EtaName+=_RecoCutFlags[4].c_str();
EtaHistoSC_1234->SetName(EtaName.c_str());
EtaHistoSC_1234->SetLineColor(1);
EtaHistoSC_1234->SetXTitle("Eta");
EtaHistoSC_1234->SetYTitle("Events SC/OC");
EtaHistoSC_1234->GetYaxis()->SetTitleOffset(1.4);
if(EtaHistoSC_1234->GetEntries()>10)EtaHistoSC_1234->Fit("pol0");
EtaHistoSC_1234->Write();
EtaCanvas_1234->Close();

TCanvas *EtaCanvas_12345 = new TCanvas;
EtaHistoSC_12345->Divide(EtaHistoOC_12345);
EtaHistoSC_12345->SetTitle("Reconstructed Leading Electron Eta: Same Charge/Opposite Charge");
EtaName+=_RecoCutFlags[5].c_str();
EtaHistoSC_12345->SetName(EtaName.c_str());
EtaHistoSC_12345->SetLineColor(1);
EtaHistoSC_12345->SetXTitle("Eta");
EtaHistoSC_12345->SetYTitle("Events SC/OC");
EtaHistoSC_12345->GetYaxis()->SetTitleOffset(1.4);
if(EtaHistoSC_12345->GetEntries()>10)EtaHistoSC_12345->Fit("pol0");
EtaHistoSC_12345->Write();
EtaCanvas_12345->Close();

TCanvas *EtaCanvas_123456 = new TCanvas;
EtaHistoSC_123456->Divide(EtaHistoOC_123456);
EtaHistoSC_123456->SetTitle("Reconstructed Leading Electron Eta: Same Charge/Opposite Charge");
EtaName+=_RecoCutFlags[6].c_str();
EtaHistoSC_123456->SetName(EtaName.c_str());
EtaHistoSC_123456->SetLineColor(1);
EtaHistoSC_123456->SetXTitle("Eta");
EtaHistoSC_123456->SetYTitle("Events SC/OC");
EtaHistoSC_123456->GetYaxis()->SetTitleOffset(1.4);
if(EtaHistoSC_123456->GetEntries()>10)EtaHistoSC_123456->Fit("pol0");
EtaHistoSC_123456->Write();
EtaCanvas_123456->Close();

//fBrem
fBremDir->cd();

string recLeadElfBrem_name = "recLeadElfBrem";
recLeadElfBrem_name+=_RecoCutFlags[1].c_str();
TH1D *fBremHistoOC_1 = (TH1D*) OC->Get(recLeadElfBrem_name.c_str());
recLeadElfBrem_name+=_RecoCutFlags[2].c_str();
TH1D *fBremHistoOC_12 = (TH1D*) OC->Get(recLeadElfBrem_name.c_str());
recLeadElfBrem_name+=_RecoCutFlags[3].c_str();
TH1D *fBremHistoOC_123 = (TH1D*) OC->Get(recLeadElfBrem_name.c_str());
recLeadElfBrem_name+=_RecoCutFlags[4].c_str();
TH1D *fBremHistoOC_1234 = (TH1D*) OC->Get(recLeadElfBrem_name.c_str());
recLeadElfBrem_name+=_RecoCutFlags[5].c_str();
TH1D *fBremHistoOC_12345 = (TH1D*) OC->Get(recLeadElfBrem_name.c_str());
recLeadElfBrem_name+=_RecoCutFlags[6].c_str();
TH1D *fBremHistoOC_123456 = (TH1D*) OC->Get(recLeadElfBrem_name.c_str());

string recLeadElfBremSC_name = "recLeadElfBremSC";
recLeadElfBremSC_name+=_RecoCutFlags[1].c_str();
TH1D *fBremHistoSC_1 = (TH1D*) SC->Get(recLeadElfBremSC_name.c_str());
recLeadElfBremSC_name+=_RecoCutFlags[2].c_str();
TH1D *fBremHistoSC_12 = (TH1D*) SC->Get(recLeadElfBremSC_name.c_str());
recLeadElfBremSC_name+=_RecoCutFlags[3].c_str();
TH1D *fBremHistoSC_123 = (TH1D*) SC->Get(recLeadElfBremSC_name.c_str());
recLeadElfBremSC_name+=_RecoCutFlags[4].c_str();
TH1D *fBremHistoSC_1234 = (TH1D*) SC->Get(recLeadElfBremSC_name.c_str());
recLeadElfBremSC_name+=_RecoCutFlags[5].c_str();
TH1D *fBremHistoSC_12345 = (TH1D*) SC->Get(recLeadElfBremSC_name.c_str());
recLeadElfBremSC_name+=_RecoCutFlags[6].c_str();
TH1D *fBremHistoSC_123456 = (TH1D*) SC->Get(recLeadElfBremSC_name.c_str());

TCanvas *fBremCanvas_1 = new TCanvas;
fBremHistoSC_1->Divide(fBremHistoOC_1);
fBremHistoSC_1->SetTitle("Reconstructed Leading Electron fBrem: Same Charge/Opposite Charge");
string fBremName = "fBremRatio";
fBremName+=_RecoCutFlags[1].c_str();
fBremHistoSC_1->SetName(fBremName.c_str());
fBremHistoSC_1->SetLineColor(1);
fBremHistoSC_1->SetXTitle("fBrem");
fBremHistoSC_1->SetYTitle("Events SC/OC");
fBremHistoSC_1->GetYaxis()->SetTitleOffset(1.4);
if(fBremHistoSC_1->GetEntries()>10)fBremHistoSC_1->Fit("pol0");
fBremHistoSC_1->Write();
fBremCanvas_1->Close();

TCanvas *fBremCanvas_12 = new TCanvas;
fBremHistoSC_12->Divide(fBremHistoOC_12);
fBremHistoSC_12->SetTitle("Reconstructed Leading Electron fBrem: Same Charge/Opposite Charge");
fBremName+=_RecoCutFlags[2].c_str();
fBremHistoSC_12->SetName(fBremName.c_str());
fBremHistoSC_12->SetLineColor(1);
fBremHistoSC_12->SetXTitle("fBrem");
fBremHistoSC_12->SetYTitle("Events SC/OC");
fBremHistoSC_12->GetYaxis()->SetTitleOffset(1.4);
if(fBremHistoSC_12->GetEntries()>10)fBremHistoSC_12->Fit("pol0");
fBremHistoSC_12->Write();
fBremCanvas_12->Close();

TCanvas *fBremCanvas_123 = new TCanvas;
fBremHistoSC_123->Divide(fBremHistoOC_123);
fBremHistoSC_123->SetTitle("Reconstructed Leading Electron fBrem: Same Charge/Opposite Charge");
fBremName+=_RecoCutFlags[3].c_str();
fBremHistoSC_123->SetName(fBremName.c_str());
fBremHistoSC_123->SetLineColor(1);
fBremHistoSC_123->SetXTitle("fBrem");
fBremHistoSC_123->SetYTitle("Events SC/OC");
fBremHistoSC_123->GetYaxis()->SetTitleOffset(1.4);
if(fBremHistoSC_123->GetEntries()>10)fBremHistoSC_123->Fit("pol0");
fBremHistoSC_123->Write();
fBremCanvas_123->Close();

TCanvas *fBremCanvas_1234 = new TCanvas;
fBremHistoSC_1234->Divide(fBremHistoOC_1234);
fBremHistoSC_1234->SetTitle("Reconstructed Leading Electron fBrem: Same Charge/Opposite Charge");
fBremName+=_RecoCutFlags[4].c_str();
fBremHistoSC_1234->SetName(fBremName.c_str());
fBremHistoSC_1234->SetLineColor(1);
fBremHistoSC_1234->SetXTitle("fBrem");
fBremHistoSC_1234->SetYTitle("Events SC/OC");
fBremHistoSC_1234->GetYaxis()->SetTitleOffset(1.4);
if(fBremHistoSC_1234->GetEntries()>10)fBremHistoSC_1234->Fit("pol0");
fBremHistoSC_1234->Write();
fBremCanvas_1234->Close();

TCanvas *fBremCanvas_12345 = new TCanvas;
fBremHistoSC_12345->Divide(fBremHistoOC_12345);
fBremHistoSC_12345->SetTitle("Reconstructed Leading Electron fBrem: Same Charge/Opposite Charge");
fBremName+=_RecoCutFlags[5].c_str();
fBremHistoSC_12345->SetName(fBremName.c_str());
fBremHistoSC_12345->SetLineColor(1);
fBremHistoSC_12345->SetXTitle("fBrem");
fBremHistoSC_12345->SetYTitle("Events SC/OC");
fBremHistoSC_12345->GetYaxis()->SetTitleOffset(1.4);
if(fBremHistoSC_12345->GetEntries()>10)fBremHistoSC_12345->Fit("pol0");
fBremHistoSC_12345->Write();
fBremCanvas_12345->Close();

TCanvas *fBremCanvas_123456 = new TCanvas;
fBremHistoSC_123456->Divide(fBremHistoOC_123456);
fBremHistoSC_123456->SetTitle("Reconstructed Leading Electron fBrem: Same Charge/Opposite Charge");
fBremName+=_RecoCutFlags[6].c_str();
fBremHistoSC_123456->SetName(fBremName.c_str());
fBremHistoSC_123456->SetLineColor(1);
fBremHistoSC_123456->SetXTitle("fBrem");
fBremHistoSC_123456->SetYTitle("Events SC/OC");
fBremHistoSC_123456->GetYaxis()->SetTitleOffset(1.4);
if(fBremHistoSC_123456->GetEntries()>10)fBremHistoSC_123456->Fit("pol0");
fBremHistoSC_123456->Write();
fBremCanvas_123456->Close();

//Mass
MassDir->cd();

TDirectory *OC_Z = (TDirectory*) inputFile->Get("RecoElectron/recZ_Plots");

string recMassZ_name = "recMassZ";
recMassZ_name+=_RecoCutFlags[1].c_str();
TH1D *MassHistoOC_1 = (TH1D*) OC_Z->Get(recMassZ_name.c_str());
recMassZ_name+=_RecoCutFlags[2].c_str();
TH1D *MassHistoOC_12 = (TH1D*) OC_Z->Get(recMassZ_name.c_str());
recMassZ_name+=_RecoCutFlags[3].c_str();
TH1D *MassHistoOC_123 = (TH1D*) OC_Z->Get(recMassZ_name.c_str());
recMassZ_name+=_RecoCutFlags[4].c_str();
TH1D *MassHistoOC_1234 = (TH1D*) OC_Z->Get(recMassZ_name.c_str());
recMassZ_name+=_RecoCutFlags[5].c_str();
TH1D *MassHistoOC_12345 = (TH1D*) OC_Z->Get(recMassZ_name.c_str());
recMassZ_name+=_RecoCutFlags[6].c_str();
TH1D *MassHistoOC_123456 = (TH1D*) OC_Z->Get(recMassZ_name.c_str());

string recMassZSC_name = "recMassZSC";
recMassZSC_name+=_RecoCutFlags[1].c_str();
TH1D *MassHistoSC_1 = (TH1D*) SC->Get(recMassZSC_name.c_str());
recMassZSC_name+=_RecoCutFlags[2].c_str();
TH1D *MassHistoSC_12 = (TH1D*) SC->Get(recMassZSC_name.c_str());
recMassZSC_name+=_RecoCutFlags[3].c_str();
TH1D *MassHistoSC_123 = (TH1D*) SC->Get(recMassZSC_name.c_str());
recMassZSC_name+=_RecoCutFlags[4].c_str();
TH1D *MassHistoSC_1234 = (TH1D*) SC->Get(recMassZSC_name.c_str());
recMassZSC_name+=_RecoCutFlags[5].c_str();
TH1D *MassHistoSC_12345 = (TH1D*) SC->Get(recMassZSC_name.c_str());
recMassZSC_name+=_RecoCutFlags[6].c_str();
TH1D *MassHistoSC_123456 = (TH1D*) SC->Get(recMassZSC_name.c_str());

TCanvas *MassCanvas_1 = new TCanvas;
MassHistoSC_1->Divide(MassHistoOC_1);
MassHistoSC_1->SetTitle("Reconstructed Invariant Mass: Same Charge/Opposite Charge");
string MassName = "MassRatio";
MassName+=_RecoCutFlags[1].c_str();
MassHistoSC_1->SetName(MassName.c_str());
MassHistoSC_1->SetLineColor(1);
MassHistoSC_1->SetXTitle("Mass");
MassHistoSC_1->SetYTitle("Events SC/OC");
MassHistoSC_1->GetYaxis()->SetTitleOffset(1.4);
if(MassHistoSC_1->GetEntries()>100)MassHistoSC_1->Fit("pol0");
MassHistoSC_1->Write();
MassCanvas_1->Close();

TCanvas *MassCanvas_12 = new TCanvas;
MassHistoSC_12->Divide(MassHistoOC_12);
MassHistoSC_12->SetTitle("Reconstructed Invariant Mass: Same Charge/Opposite Charge");
MassName+=_RecoCutFlags[2].c_str();
MassHistoSC_12->SetName(MassName.c_str());
MassHistoSC_12->SetLineColor(1);
MassHistoSC_12->SetXTitle("Mass");
MassHistoSC_12->SetYTitle("Events SC/OC");
MassHistoSC_12->GetYaxis()->SetTitleOffset(1.4);
if(MassHistoSC_12->GetEntries()>100)MassHistoSC_12->Fit("pol0");
MassHistoSC_12->Write();
MassCanvas_12->Close();

TCanvas *MassCanvas_123 = new TCanvas;
MassHistoSC_123->Divide(MassHistoOC_123);
MassHistoSC_123->SetTitle("Reconstructed Invariant Mass: Same Charge/Opposite Charge");
MassName+=_RecoCutFlags[3].c_str();
MassHistoSC_123->SetName(MassName.c_str());
MassHistoSC_123->SetLineColor(1);
MassHistoSC_123->SetXTitle("Mass");
MassHistoSC_123->SetYTitle("Events SC/OC");
MassHistoSC_123->GetYaxis()->SetTitleOffset(1.4);
if(MassHistoSC_123->GetEntries()>100)MassHistoSC_123->Fit("pol0");
MassHistoSC_123->Write();
MassCanvas_123->Close();

TCanvas *MassCanvas_1234 = new TCanvas;
MassHistoSC_1234->Divide(MassHistoOC_1234);
MassHistoSC_1234->SetTitle("Reconstructed Invariant Mass: Same Charge/Opposite Charge");
MassName+=_RecoCutFlags[4].c_str();
MassHistoSC_1234->SetName(MassName.c_str());
MassHistoSC_1234->SetLineColor(1);
MassHistoSC_1234->SetXTitle("Mass");
MassHistoSC_1234->SetYTitle("Events SC/OC");
MassHistoSC_1234->GetYaxis()->SetTitleOffset(1.4);
if(MassHistoSC_1234->GetEntries()>100)MassHistoSC_1234->Fit("pol0");
MassHistoSC_1234->Write();
MassCanvas_1234->Close();

TCanvas *MassCanvas_12345 = new TCanvas;
MassHistoSC_12345->Divide(MassHistoOC_12345);
MassHistoSC_12345->SetTitle("Reconstructed Invariant Mass: Same Charge/Opposite Charge");
MassName+=_RecoCutFlags[5].c_str();
MassHistoSC_12345->SetName(MassName.c_str());
MassHistoSC_12345->SetLineColor(1);
MassHistoSC_12345->SetXTitle("Mass");
MassHistoSC_12345->SetYTitle("Events SC/OC");
MassHistoSC_12345->GetYaxis()->SetTitleOffset(1.4);
if(MassHistoSC_12345->GetEntries()>100)MassHistoSC_12345->Fit("pol0");
MassHistoSC_12345->Write();
MassCanvas_12345->Close();

TCanvas *MassCanvas_123456 = new TCanvas;
MassHistoSC_123456->Divide(MassHistoOC_123456);
MassHistoSC_123456->SetTitle("Reconstructed Invariant Mass: Same Charge/Opposite Charge");
MassName+=_RecoCutFlags[6].c_str();
MassHistoSC_123456->SetName(MassName.c_str());
MassHistoSC_123456->SetLineColor(1);
MassHistoSC_123456->SetXTitle("Mass");
MassHistoSC_123456->SetYTitle("Events SC/OC");
MassHistoSC_123456->GetYaxis()->SetTitleOffset(1.4);
if(MassHistoSC_123456->GetEntries()>100)MassHistoSC_123456->Fit("pol0");
MassHistoSC_123456->Write();
MassCanvas_123456->Close();

  outplots->Write();
  outplots->Close();
}
