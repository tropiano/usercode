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

using namespace std;

void SCComp(string selections) {

        gROOT->SetStyle("Plain");

        int _Acc  = 1;
        int _Trg  = 2;
	int _Qual = 0;
	int _Imp  = 3;
	int _Iso  = 4;
	int _EiD  = 5;
	
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

gROOT->SetStyle("Plain");

TFile *Signal = new TFile ("Zee_7TeV_RL01_10234.root");
        if(!Signal){
	cout<<"Error! Signal files doesn't exist!"<<endl;
	return;
	}
TFile *Background = new TFile ("QCD_all_norm100.root");
        if(!Background){
	cout<<"Error! Background files doesn't exist!"<<endl;
	return;
	}
TFile *outplots = new TFile("TestSCComp.root", "RECREATE");

TDirectory *SignalDir = (TDirectory*) Signal->Get("RecoElectron/SameSign");
TDirectory *BackDir = (TDirectory*) Background->Get("RecoElectron/SameSign");

string recMassZSC_name = "recMassZSC";
recMassZSC_name+=_RecoCutFlags[1].c_str();
TH1D *SignalSC_1 = (TH1D*) SignalDir->Get(recMassZSC_name.c_str());
TH1D *BackSC_1 = (TH1D*) BackDir->Get(recMassZSC_name.c_str());
recMassZSC_name+=_RecoCutFlags[2].c_str();
TH1D *SignalSC_12 = (TH1D*) SignalDir->Get(recMassZSC_name.c_str());
TH1D *BackSC_12 = (TH1D*) BackDir->Get(recMassZSC_name.c_str());
recMassZSC_name+=_RecoCutFlags[3].c_str();
TH1D *SignalSC_123 = (TH1D*) SignalDir->Get(recMassZSC_name.c_str());
TH1D *BackSC_123 = (TH1D*) BackDir->Get(recMassZSC_name.c_str());
recMassZSC_name+=_RecoCutFlags[4].c_str();
TH1D *SignalSC_1234 = (TH1D*) SignalDir->Get(recMassZSC_name.c_str());
TH1D *BackSC_1234 = (TH1D*) BackDir->Get(recMassZSC_name.c_str());
recMassZSC_name+=_RecoCutFlags[5].c_str();
TH1D *SignalSC_12345 = (TH1D*) SignalDir->Get(recMassZSC_name.c_str());
TH1D *BackSC_12345 = (TH1D*) BackDir->Get(recMassZSC_name.c_str());
recMassZSC_name+=_RecoCutFlags[6].c_str();
TH1D *SignalSC_123456 = (TH1D*) SignalDir->Get(recMassZSC_name.c_str());
TH1D *BackSC_123456 = (TH1D*) BackDir->Get(recMassZSC_name.c_str());

        if(!BackSC_123456){
	cout<<"Error! Cut sequence wrong!"<<endl;
	return;
	}

TCanvas *MassComp_1 = new TCanvas;
MassComp_1->SetLogy();
string MassComp_name = "MassComp";
SignalSC_1->GetXaxis()->SetTitle("Invariant Mass");
SignalSC_1->SetTitle("Reconstructed Invariant Mass for Signal (black) and Background (red) Same Charge electron pairs");
SignalSC_1->Draw();
BackSC_1->SetLineColor(2);
BackSC_1->Draw("sames");
MassComp_name+=_RecoCutFlags[1].c_str();
MassComp_1->Write(MassComp_name.c_str());
MassComp_1->Close();

TCanvas *MassComp_12 = new TCanvas;
MassComp_12->SetLogy();
SignalSC_12->GetXaxis()->SetTitle("Invariant Mass");
SignalSC_12->SetTitle("Reconstructed Invariant Mass for Signal (black) and Background (red) Same Charge electron pairs");
SignalSC_12->Draw();
BackSC_12->SetLineColor(2);
BackSC_12->Draw("sames");
MassComp_name+=_RecoCutFlags[2].c_str();
MassComp_12->Write(MassComp_name.c_str());
MassComp_12->Close();

TCanvas *MassComp_123 = new TCanvas;
MassComp_123->SetLogy();
SignalSC_123->GetXaxis()->SetTitle("Invariant Mass");
SignalSC_123->SetTitle("Reconstructed Invariant Mass for Signal (black) and Background (red) Same Charge electron pairs");
SignalSC_123->Draw();
BackSC_123->SetLineColor(2);
BackSC_123->Draw("sames");
MassComp_name+=_RecoCutFlags[3].c_str();
MassComp_123->Write(MassComp_name.c_str());
MassComp_123->Close();

TCanvas *MassComp_1234 = new TCanvas;
MassComp_1234->SetLogy();
SignalSC_1234->GetXaxis()->SetTitle("Invariant Mass");
SignalSC_1234->SetTitle("Reconstructed Invariant Mass for Signal (black) and Background (red) Same Charge electron pairs");
SignalSC_1234->Draw();
BackSC_1234->SetLineColor(2);
BackSC_1234->Draw("sames");
MassComp_name+=_RecoCutFlags[4].c_str();
MassComp_1234->Write(MassComp_name.c_str());
MassComp_1234->Close();

TCanvas *MassComp_12345 = new TCanvas;
MassComp_12345->SetLogy();
SignalSC_12345->GetXaxis()->SetTitle("Invariant Mass");
SignalSC_12345->SetTitle("Reconstructed Invariant Mass for Signal (black) and Background (red) Same Charge electron pairs");
SignalSC_12345->Draw();
BackSC_12345->SetLineColor(2);
BackSC_12345->Draw("sames");
MassComp_name+=_RecoCutFlags[5].c_str();
MassComp_12345->Write(MassComp_name.c_str());
MassComp_12345->Close();

TCanvas *MassComp_123456 = new TCanvas;
MassComp_123456->SetLogy();
SignalSC_123456->GetXaxis()->SetTitle("Invariant Mass");
SignalSC_123456->SetTitle("Reconstructed Invariant Mass for Signal (black) and Background (red) Same Charge electron pairs");
SignalSC_123456->Draw();
BackSC_123456->SetLineColor(2);
BackSC_123456->Draw("sames");
MassComp_name+=_RecoCutFlags[6].c_str();
MassComp_123456->Write(MassComp_name.c_str());
MassComp_123456->Close();

outplots->Write();
outplots->Close();

}

