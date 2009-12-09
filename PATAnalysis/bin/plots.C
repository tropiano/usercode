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

using namespace std;


void Plots(string DIR){

gROOT->SetStyle("Plain");

TFile *signal_file = TFile::Open("OutputFiles/Signal_Zee_RobustTight_sumw2.root");

TFile* outplots = new TFile("AnalysisRootFiles/Analysis_plots_test.root", "RECREATE");

TDirectory *Pt_Eff, *Eta_Eff, *genJet_Eff, *recJet_Eff;

Pt_Eff = outplots->mkdir("Efficiency_vs_Pt");
Eta_Eff = outplots->mkdir("Efficiency_vs_Eta");
genJet_Eff = outplots->mkdir("Efficiency_vs_GenJet");
recJet_Eff = outplots->mkdir("Efficiency_vs_RecJet");

//Z electron efficiency vs eta

   Eta_Eff->cd();

   TH1D* genLeadElEta = (TH1D*) signal_file->Get("GenElectron/genLeadElEta");
   TH1D* genSecElEta = (TH1D*) signal_file->Get("GenElectron/genSecElEta");
   TH1D* genLeadElEta_Acc = (TH1D*) signal_file->Get("GenElectron/genLeadElEta_Acc");
   TH1D* genSecElEta_Acc = (TH1D*) signal_file->Get("GenElectron/genSecElEta_Acc");
   
   TH1D *recLeadElEta_Acc, *recLeadElEta_Acc_Qual, *recLeadElEta_Acc_Qual_Imp, *recLeadElEta_Acc_Qual_Imp_Iso, *recLeadElEta_Acc_Qual_Imp_Iso_EiD;
   TH1D *recSecElEta_Acc, *recSecElEta_Acc_Qual, *recSecElEta_Acc_Qual_Imp, *recSecElEta_Acc_Qual_Imp_Iso, *recSecElEta_Acc_Qual_Imp_Iso_EiD; 
   
   if(DIR=="Reco"){
    recLeadElEta_Acc = (TH1D*) signal_file->Get("RecoElectron/recLeadElEta_Acc");
    recSecElEta_Acc = (TH1D*) signal_file->Get("RecoElectron/recSecElEta_Acc");
    recLeadElEta_Acc_Qual = (TH1D*) signal_file->Get("RecoElectron/recLeadElEta_Acc_Qual");
    recSecElEta_Acc_Qual = (TH1D*) signal_file->Get("RecoElectron/recSecElEta_Acc_Qual");
    recLeadElEta_Acc_Qual_Imp = (TH1D*) signal_file->Get("RecoElectron/recLeadElEta_Acc_Qual_Imp");
    recSecElEta_Acc_Qual_Imp = (TH1D*) signal_file->Get("RecoElectron/recSecElEta_Acc_Qual_Imp");
    recLeadElEta_Acc_Qual_Imp_Iso = (TH1D*) signal_file->Get("RecoElectron/recLeadElEta_Acc_Qual_Imp_Iso");
    recSecElEta_Acc_Qual_Imp_Iso = (TH1D*) signal_file->Get("RecoElectron/recSecElEta_Acc_Qual_Imp_Iso");
    recLeadElEta_Acc_Qual_Imp_Iso_EiD = (TH1D*) signal_file->Get("RecoElectron/recLeadElEta_Acc_Qual_Imp_Iso_EiD");
    recSecElEta_Acc_Qual_Imp_Iso_EiD = (TH1D*) signal_file->Get("RecoElectron/recSecElEta_Acc_Qual_Imp_Iso_EiD");
   }
   
   if(DIR=="Plot"){
    recLeadElEta_Acc = (TH1D*) signal_file->Get("PlotElectron/recLeadElEta_Acc");
    recSecElEta_Acc = (TH1D*) signal_file->Get("PlotElectron/recSecElEta_Acc");
    recLeadElEta_Acc_Qual = (TH1D*) signal_file->Get("PlotElectron/recLeadElEta_Acc_Qual");
    recSecElEta_Acc_Qual = (TH1D*) signal_file->Get("PlotElectron/recSecElEta_Acc_Qual");
    recLeadElEta_Acc_Qual_Imp = (TH1D*) signal_file->Get("PlotElectron/recLeadElEta_Acc_Qual_Imp");
    recSecElEta_Acc_Qual_Imp = (TH1D*) signal_file->Get("PlotElectron/recSecElEta_Acc_Qual_Imp");
    recLeadElEta_Acc_Qual_Imp_Iso = (TH1D*) signal_file->Get("PlotElectron/recLeadElEta_Acc_Qual_Imp_Iso");
    recSecElEta_Acc_Qual_Imp_Iso = (TH1D*) signal_file->Get("PlotElectron/recSecElEta_Acc_Qual_Imp_Iso");
    recLeadElEta_Acc_Qual_Imp_Iso_EiD = (TH1D*) signal_file->Get("PlotElectron/recLeadElEta_Acc_Qual_Imp_Iso_EiD");
    recSecElEta_Acc_Qual_Imp_Iso_EiD = (TH1D*) signal_file->Get("PlotElectron/recSecElEta_Acc_Qual_Imp_Iso_EiD");
   }
     
   TH1D* ZLeadElectronEffvsEta_Acc_GEN = new TH1D("ZLeadElectronEffvsEta_Acc_GEN", "Z Lead Gen electron efficiency vs. #eta, Selections: Acc", 100, -2.5, 2.5);  
   TH1D* ZLeadElectronEffvsEta_Acc_REC = new TH1D("ZLeadElectronEffvsEta_Acc_REC", "Acc", 100, -2.5, 2.5);
   TH1D* ZLeadElectronEffvsEta_Acc_Qual = new TH1D("ZLeadElectronEffvsEta_Acc_Qual", "Acc+Qual", 100, -2.5, 2.5);
   TH1D* ZLeadElectronEffvsEta_Acc_Qual_Imp = new TH1D("ZLeadElectronEffvsEta_Acc_Qual_Imp", "Acc+Qual+Imp", 100, -2.5, 2.5);
   TH1D* ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso = new TH1D("ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso", "Acc+Qual+Imp+Iso", 100, -2.5, 2.5);
   TH1D* ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso_EiD = new TH1D("ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso_EiD", "Acc+Qual+Imp+Iso+EiD", 100, -2.5, 2.5);
   
 
   ZLeadElectronEffvsEta_Acc_GEN->Add(genLeadElEta_Acc);
   ZLeadElectronEffvsEta_Acc_GEN->Divide(genLeadElEta);
 
   ZLeadElectronEffvsEta_Acc_REC->Add(recLeadElEta_Acc);
   ZLeadElectronEffvsEta_Acc_REC->Divide(genLeadElEta_Acc);
   
   ZLeadElectronEffvsEta_Acc_Qual->Add(recLeadElEta_Acc_Qual);  
   ZLeadElectronEffvsEta_Acc_Qual->Divide(genLeadElEta_Acc);
   
   ZLeadElectronEffvsEta_Acc_Qual_Imp->Add(recLeadElEta_Acc_Qual_Imp);  
   ZLeadElectronEffvsEta_Acc_Qual_Imp->Divide(genLeadElEta_Acc);
   
   ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso->Add(recLeadElEta_Acc_Qual_Imp_Iso);  
   ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso->Divide(genLeadElEta_Acc);
   
   ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso_EiD->Add(recLeadElEta_Acc_Qual_Imp_Iso_EiD);  
   ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso_EiD->Divide(genLeadElEta_Acc);
	
	TCanvas *c1 = new TCanvas;
	gStyle->SetOptStat(0);
	
	ZLeadElectronEffvsEta_Acc_REC->SetMarkerStyle(20);
	ZLeadElectronEffvsEta_Acc_REC->Draw();
	ZLeadElectronEffvsEta_Acc_Qual->SetLineColor(2);
	ZLeadElectronEffvsEta_Acc_Qual->SetMarkerColor(2);
	ZLeadElectronEffvsEta_Acc_Qual->SetMarkerStyle(24);
	ZLeadElectronEffvsEta_Acc_Qual->SetMarkerSize(0.8);
	ZLeadElectronEffvsEta_Acc_Qual->Draw("sames");
	ZLeadElectronEffvsEta_Acc_Qual_Imp->SetLineColor(3);
	ZLeadElectronEffvsEta_Acc_Qual_Imp->SetMarkerColor(3);
	ZLeadElectronEffvsEta_Acc_Qual_Imp->SetMarkerStyle(26);
	ZLeadElectronEffvsEta_Acc_Qual_Imp->Draw("sames");
	ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso->SetLineColor(4);
	ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso->SetMarkerColor(4);
	ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso->SetMarkerStyle(26);
	ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso->Draw("sames");
	ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso_EiD->SetLineColor(5);
	ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso_EiD->SetMarkerColor(5);
	ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso_EiD->SetMarkerStyle(23);
	ZLeadElectronEffvsEta_Acc_Qual_Imp_Iso_EiD->Draw("sames");
	
	TLegend *leg1= c1->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	
	ZLeadElectronEffvsEta_Acc_REC->SetTitle("Selection efficiencies vs. Gen Lead electron Eta");
	
	c1->Write("ZLeadElectronEffvsEta.root");
	c1->Close();
   
   
//Z electron efficiency vs pt

   Pt_Eff->cd();
   
   TH1D* genLeadElPt = (TH1D*) signal_file->Get("GenElectron/genLeadElPt");
   TH1D* genSecElPt = (TH1D*) signal_file->Get("GenElectron/genSecElPt");
   TH1D* genLeadElPt_Acc = (TH1D*) signal_file->Get("GenElectron/genLeadElPt_Acc");
   TH1D* genSecElPt_Acc = (TH1D*) signal_file->Get("GenElectron/genSecElPt_Acc");
   
   TH1D *recLeadElPt_Acc, *recLeadElPt_Acc_Qual, *recLeadElPt_Acc_Qual_Imp, *recLeadElPt_Acc_Qual_Imp_Iso, *recLeadElPt_Acc_Qual_Imp_Iso_EiD;
   TH1D *recSecElPt_Acc, *recSecElPt_Acc_Qual, *recSecElPt_Acc_Qual_Imp, *recSecElPt_Acc_Qual_Imp_Iso, *recSecElPt_Acc_Qual_Imp_Iso_EiD; 
   
   if(DIR=="Reco"){
   recLeadElPt_Acc = (TH1D*) signal_file->Get("RecoElectron/recLeadElPt_Acc");
   recSecElPt_Acc = (TH1D*) signal_file->Get("RecoElectron/recSecElPt_Acc");
   recLeadElPt_Acc_Qual = (TH1D*) signal_file->Get("RecoElectron/recLeadElPt_Acc_Qual");
   recSecElPt_Acc_Qual = (TH1D*) signal_file->Get("RecoElectron/recSecElPt_Acc_Qual");
   recLeadElPt_Acc_Qual_Imp = (TH1D*) signal_file->Get("RecoElectron/recLeadElPt_Acc_Qual_Imp");
   recSecElPt_Acc_Qual_Imp = (TH1D*) signal_file->Get("RecoElectron/recSecElPt_Acc_Qual_Imp");
   recLeadElPt_Acc_Qual_Imp_Iso = (TH1D*) signal_file->Get("RecoElectron/recLeadElPt_Acc_Qual_Imp_Iso");
   recSecElPt_Acc_Qual_Imp_Iso = (TH1D*) signal_file->Get("RecoElectron/recSecElPt_Acc_Qual_Imp_Iso");
   recLeadElPt_Acc_Qual_Imp_Iso_EiD = (TH1D*) signal_file->Get("RecoElectron/recLeadElPt_Acc_Qual_Imp_Iso_EiD");
   recSecElPt_Acc_Qual_Imp_Iso_EiD = (TH1D*) signal_file->Get("RecoElectron/recSecElPt_Acc_Qual_Imp_Iso_EiD");
   }
   
   if(DIR=="Plot"){
   recLeadElPt_Acc = (TH1D*) signal_file->Get("PlotElectron/recLeadElPt_Acc");
   recSecElPt_Acc = (TH1D*) signal_file->Get("PlotElectron/recSecElPt_Acc");
   recLeadElPt_Acc_Qual = (TH1D*) signal_file->Get("PlotElectron/recLeadElPt_Acc_Qual");
   recSecElPt_Acc_Qual = (TH1D*) signal_file->Get("PlotElectron/recSecElPt_Acc_Qual");
   recLeadElPt_Acc_Qual_Imp = (TH1D*) signal_file->Get("PlotElectron/recLeadElPt_Acc_Qual_Imp");
   recSecElPt_Acc_Qual_Imp = (TH1D*) signal_file->Get("PlotElectron/recSecElPt_Acc_Qual_Imp");
   recLeadElPt_Acc_Qual_Imp_Iso = (TH1D*) signal_file->Get("PlotElectron/recLeadElPt_Acc_Qual_Imp_Iso");
   recSecElPt_Acc_Qual_Imp_Iso = (TH1D*) signal_file->Get("PlotElectron/recSecElPt_Acc_Qual_Imp_Iso");
   recLeadElPt_Acc_Qual_Imp_Iso_EiD = (TH1D*) signal_file->Get("PlotElectron/recLeadElPt_Acc_Qual_Imp_Iso_EiD");
   recSecElPt_Acc_Qual_Imp_Iso_EiD = (TH1D*) signal_file->Get("PlotElectron/recSecElPt_Acc_Qual_Imp_Iso_EiD");
   }
   
   TH1D* ZLeadElectronEffvsPt_Acc_GEN = new TH1D("ZLeadElectronEffvsPt_Acc_GEN", "Z Lead Gen electron efficiency vs. pt, Selections: Acc", 200, 0, 200);
   TH1D* ZLeadElectronEffvsPt_Acc_REC = new TH1D("ZLeadElectronEffvsPt_Acc_REC", "Acc", 200, 0, 200);
   TH1D* ZLeadElectronEffvsPt_Acc_Qual = new TH1D("ZLeadElectronEffvsPt_Acc_Qual", "Acc+Qual", 200, 0, 200);
   TH1D* ZLeadElectronEffvsPt_Acc_Qual_Imp = new TH1D("ZLeadElectronEffvsPt_Acc_Qual_Imp", "Acc+Qual+Imp", 200, 0, 200);
   TH1D* ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso = new TH1D("ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso", "Acc+Qual+Imp+Iso", 200, 0, 200);
   TH1D* ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso_EiD = new TH1D("ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso_EiD", "Acc+Qual+Imp+Iso+EiD", 200, 0, 200);
   
   ZLeadElectronEffvsPt_Acc_GEN->Add(genLeadElPt_Acc);
   ZLeadElectronEffvsPt_Acc_GEN->Divide(genLeadElPt); 
   
   ZLeadElectronEffvsPt_Acc_REC->Add(recLeadElPt_Acc);
   ZLeadElectronEffvsPt_Acc_REC->Divide(genLeadElPt_Acc);
   
   ZLeadElectronEffvsPt_Acc_Qual->Add(recLeadElPt_Acc_Qual);  
   ZLeadElectronEffvsPt_Acc_Qual->Divide(genLeadElPt_Acc);
   
   ZLeadElectronEffvsPt_Acc_Qual_Imp->Add(recLeadElPt_Acc_Qual_Imp);  
   ZLeadElectronEffvsPt_Acc_Qual_Imp->Divide(genLeadElPt_Acc);
   
   ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso->Add(recLeadElPt_Acc_Qual_Imp_Iso);  
   ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso->Divide(genLeadElPt_Acc);
   
   ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso_EiD->Add(recLeadElPt_Acc_Qual_Imp_Iso_EiD);  
   ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso_EiD->Divide(genLeadElPt_Acc);
	
	TCanvas *c2 = new TCanvas;
	gStyle->SetOptStat(0);
	
	ZLeadElectronEffvsPt_Acc_REC->SetMarkerStyle(20);
	ZLeadElectronEffvsPt_Acc_REC->Draw();
	ZLeadElectronEffvsPt_Acc_Qual->SetLineColor(2);
	ZLeadElectronEffvsPt_Acc_Qual->SetMarkerColor(2);
	ZLeadElectronEffvsPt_Acc_Qual->SetMarkerStyle(24);
	ZLeadElectronEffvsPt_Acc_Qual->SetMarkerSize(0.8);
	ZLeadElectronEffvsPt_Acc_Qual->Draw("sames");
	ZLeadElectronEffvsPt_Acc_Qual_Imp->SetLineColor(3);
	ZLeadElectronEffvsPt_Acc_Qual_Imp->SetMarkerColor(3);
	ZLeadElectronEffvsPt_Acc_Qual_Imp->SetMarkerStyle(26);
	ZLeadElectronEffvsPt_Acc_Qual_Imp->Draw("sames");
	ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso->SetLineColor(4);
	ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso->SetMarkerColor(4);
	ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso->SetMarkerStyle(26);
	ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso->Draw("sames");
	ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso_EiD->SetLineColor(5);
	ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso_EiD->SetMarkerColor(5);
	ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso_EiD->SetMarkerStyle(23);
	ZLeadElectronEffvsPt_Acc_Qual_Imp_Iso_EiD->Draw("sames");
	
	TLegend *leg2= c2->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	
	ZLeadElectronEffvsPt_Acc_REC->SetTitle("Selection efficiencies vs. Gen Lead electron Pt");
	
	c2->Write("ZLeadElectronEffvsPt.root");
	c2->Close();
   
 //Z electron efficiency vs Gen Jet

   genJet_Eff->cd();
   
   TH1D* GenIsoJetCounter = (TH1D*) signal_file->Get("GenElectron/GenIsoJetCounter");
   TH1D* GenIsoJetCounter_Acc = (TH1D*) signal_file->Get("GenElectron/GenIsoJetCounter_Acc");
   
   TH1D *GenIsoJetPlot_Acc = (TH1D*) signal_file->Get("PlotElectron/GenIsoJetPlot_Acc");
   TH1D *GenIsoJetPlot_Acc_Qual = (TH1D*) signal_file->Get("PlotElectron/GenIsoJetPlot_Acc_Qual");
   TH1D *GenIsoJetPlot_Acc_Qual_Imp = (TH1D*) signal_file->Get("PlotElectron/GenIsoJetPlot_Acc_Qual_Imp");
   TH1D *GenIsoJetPlot_Acc_Qual_Imp_Iso = (TH1D*) signal_file->Get("PlotElectron/GenIsoJetPlot_Acc_Qual_Imp_Iso");
   TH1D *GenIsoJetPlot_Acc_Qual_Imp_Iso_EiD = (TH1D*) signal_file->Get("PlotElectron/GenIsoJetPlot_Acc_Qual_Imp_Iso_EiD");
   
   TH1D* ZEffvsGenIsoJetNum_Acc_GEN = new TH1D("ZEffvsGenIsoJetNum_Acc_GEN", "Z Gen efficiency vs. Gen Iso Jet Number, Selections: Acc", 10, 0, 10);
   TH1D* ZEffvsGenIsoJetNum_Acc_REC = new TH1D("ZEffvsGenIsoJetNum_Acc_REC", "Acc", 10, 0, 10);
   TH1D* ZEffvsGenIsoJetNum_Acc_Qual = new TH1D("ZEffvsGenIsoJetNum_Acc_Qual", "Acc+Qual", 10, 0, 10);
   TH1D* ZEffvsGenIsoJetNum_Acc_Qual_Imp = new TH1D("ZEffvsGenIsoJetNum_Acc_Qual_Imp", "Acc+Qual+Imp", 10, 0, 10);
   TH1D* ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso = new TH1D("ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso", "Acc+Qual+Imp+Iso", 10, 0, 10);
   TH1D* ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso_EiD = new TH1D("ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso_EiD", "Acc+Qual+Imp+Iso+EiD", 10, 0, 10);
   
   ZEffvsGenIsoJetNum_Acc_GEN->Add(GenIsoJetCounter_Acc);
   ZEffvsGenIsoJetNum_Acc_GEN->Divide(GenIsoJetCounter);
   
   ZEffvsGenIsoJetNum_Acc_REC->Add(GenIsoJetPlot_Acc);
   ZEffvsGenIsoJetNum_Acc_REC->Divide(GenIsoJetCounter_Acc);
   
   ZEffvsGenIsoJetNum_Acc_Qual->Add(GenIsoJetPlot_Acc_Qual);  
   ZEffvsGenIsoJetNum_Acc_Qual->Divide(GenIsoJetCounter_Acc);
   
   ZEffvsGenIsoJetNum_Acc_Qual_Imp->Add(GenIsoJetPlot_Acc_Qual_Imp);  
   ZEffvsGenIsoJetNum_Acc_Qual_Imp->Divide(GenIsoJetCounter_Acc);
   
   ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso->Add(GenIsoJetPlot_Acc_Qual_Imp_Iso);  
   ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso->Divide(GenIsoJetCounter_Acc);
   
   ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso_EiD->Add(GenIsoJetPlot_Acc_Qual_Imp_Iso_EiD);  
   ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso_EiD->Divide(GenIsoJetCounter_Acc);
	
	TCanvas *c3 = new TCanvas;
	gStyle->SetOptStat(0);
	
	ZEffvsGenIsoJetNum_Acc_REC->SetMarkerStyle(20);
	ZEffvsGenIsoJetNum_Acc_REC->Draw();
	ZEffvsGenIsoJetNum_Acc_Qual->SetLineColor(2);
	ZEffvsGenIsoJetNum_Acc_Qual->SetMarkerColor(2);
	ZEffvsGenIsoJetNum_Acc_Qual->SetMarkerStyle(24);
	ZEffvsGenIsoJetNum_Acc_Qual->SetMarkerSize(0.8);
	ZEffvsGenIsoJetNum_Acc_Qual->Draw("sames");
	ZEffvsGenIsoJetNum_Acc_Qual_Imp->SetLineColor(3);
	ZEffvsGenIsoJetNum_Acc_Qual_Imp->SetMarkerColor(3);
	ZEffvsGenIsoJetNum_Acc_Qual_Imp->SetMarkerStyle(26);
	ZEffvsGenIsoJetNum_Acc_Qual_Imp->Draw("sames");
	ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso->SetLineColor(4);
	ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso->SetMarkerColor(4);
	ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso->SetMarkerStyle(26);
	ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso->Draw("sames");
	ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso_EiD->SetLineColor(5);
	ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso_EiD->SetMarkerColor(5);
	ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso_EiD->SetMarkerStyle(23);
	ZEffvsGenIsoJetNum_Acc_Qual_Imp_Iso_EiD->Draw("sames");
	
	TLegend *leg3= c3->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	
	ZEffvsGenIsoJetNum_Acc_REC->SetTitle("Selection efficiencies vs. Gen Jet Number");
	
	c3->Write("ZEffvsGenIsoJetNum.root");
	c3->Close();
	
	

   
	
   
       
 

  outplots->Write();
  outplots->Close();
}


