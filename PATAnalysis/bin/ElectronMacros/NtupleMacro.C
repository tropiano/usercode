#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "TH1D.h"
#include "TH2D.h"
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
#include "TTree.h"

using namespace std;

//Selections: "SYM" = V+jets selections (old); "ASYM" = Vector Boson Task Force (new)

void Ntuple_Plots(){

string Analysis_Note = "False";
if (Analysis_Note == "False") gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);

string log_scale = "True";

string Tab_cut = "True";
        
                
        //Background MC
        TFile* QCD_EMEnriched_all_TF = TFile::Open("../Simulazioni/PF30Jets/QCD_EMEnriched_Pythia_ALL_Spring10_PF30Eta2e5_Norm50_ALL.root");
        TFile* QCD_BCtoE_all_TF = TFile::Open("../Simulazioni/PF30Jets/QCD_BCtoE_Pythia_ALL_Spring10_PF30Eta2e5_Norm50_ALL.root");
        TFile* TTbar_TF = TFile::Open("../Simulazioni/PF30Jets/TTbarJets_Madgraph_Spring10_PF30Eta2e5_Norm50_ALL.root");
        TFile* W_TF = TFile::Open("../Simulazioni/PF30Jets/WJets_Madgraph_Spring10_PF30Eta2e5_Norm50_ALL.root");
        
	//Signal MC
        TFile *Z_TF = TFile::Open("../Simulazioni/PF30Jets/ZJets_Madgraph_Spring10_PF30Eta2e5_Norm50_ALL.root");

        //Data
        TFile *Data_TF = TFile::Open("../Dati/PF30Jets/Data_Sep8_Photon15_Cleaned_Ele15_SW_EleID_ALL.root");
        
        //Output
        string out = "Ntuple_Plots";        
        string output = out;
        output+=".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
	
	//Define trees
	TTree* data_tree   = (TTree*) Data_TF->Get("RecoElectronNtuple/ZeeTree");
	TTree* Z_tree    = (TTree*) Z_TF->Get("RecoElectronNtuple/ZeeTree");
	TTree* QCDbce_tree = (TTree*) QCD_BCtoE_all_TF->Get("RecoElectronNtuple/ZeeTree");
	TTree* QCDem_tree  = (TTree*) QCD_EMEnriched_all_TF->Get("RecoElectronNtuple/ZeeTree");
	TTree* TT_tree     = (TTree*) TTbar_TF->Get("RecoElectronNtuple/ZeeTree");
	TTree* W_tree      = (TTree*) W_TF->Get("RecoElectronNtuple/ZeeTree");
	      
        //Normalization factor
        double iniLumi = 50.; //pb-1
        double targetLumi = 2.343; //pb-1
        double scale = 1.;
        if(iniLumi!=0)scale = targetLumi/iniLumi;

	//rebin
	int rebin_recMassZ = 5;
	int rebin_recPtZ = 2;
	int rebin_recEtaZ = 5;
		
	//colors	
	int col_Z = 800;
	int col_QCD_EMEnriched_all= 616+3;
	int col_QCD_BCtoE_all= 800+7;
	int col_TTbar= 632;
	int col_W= 800+3;
	int col_Data= 1;

	//FillStyle	
	int style = 1001;
	
	//Legenda
	string Leg_QCD_EMEnriched_all = "QCD EM Enriched";
	string Leg_QCD_BCtoE_all = "QCD b,c #rightarrow e";
	string Leg_TTbar = "t #bar{t}+jets";
	string Leg_W = "W+jets";
	string Leg_Z = "Z+jets";
	string Leg_Data	= "Data";
	
	//Define Histograms
	TH1D* ZEta0Jet_Data = new TH1D("ZEta0Jet_Data","Z Eta Data - 0 Jet",20,-10,10);
	TH1D* ZEta0Jet_MC = new TH1D("ZEta0Jet_MC","Z Eta MC - 0 Jet",20,-10,10);
	TH1D* ZEta1Jet_Data = new TH1D("ZEta1Jet_Data","Z Eta Data - 1 Jet",20,-10,10);
	TH1D* ZEta1Jet_MC = new TH1D("ZEta1Jet_MC","Z Eta MC - 1 Jet",20,-10,10);
	TH1D* PtZoverPtJet_1PFJet_Data = new TH1D("PtZoverPtJet_1PFJet_Data","PtZoverPtJet 1PFJet Data",24,0,3);
	TH1D* PtZoverPtJet_1PFJet_MC = new TH1D("PtZoverPtJet_1PFJet_MC","PtZoverPtJet 1PFJet MC",24,0,3);
	TH1D* PtZoverPtJet_1CALOJet_Data = new TH1D("PtZoverPtJet_1CALOJet_Data","PtZoverPtJet 1CALOJet Data",24,0,3);
	TH1D* PtZoverPtJet_1CALOJet_MC = new TH1D("PtZoverPtJet_1CALOJet_MC","PtZoverPtJet 1CALOJet MC",24,0,3);
	TH2D* ResJetPt_PF_MC = new TH2D("ResJetPt_PF_MC","Resolution of PF Jet Pt - MC",140,10,150,200,-1,1);
	TH2D* ResJetPt_CALO_MC = new TH2D("ResJetPt_CALO_MC","Resolution of CALO Jet Pt - MC",140,10,150,200,-1,1);
	TH1D* ResJetPt_PF_MC_1D = new TH1D("ResJetPt_PF_MC_1D","Resolution of PF Jet Pt - MC",80,-1,1);
	TH1D* ResJetPt_CALO_MC_1D = new TH1D("ResJetPt_CALO_MC_1D","Resolution of PF Jet Pt - MC",80,-1,1);
	
	
	//Build histograms by Tree
	TCanvas *tmp = new TCanvas;
	
	data_tree->Draw("zeta >> ZEta0Jet_Data","zmass_AllCuts>0 && npfjetsele==0");
	data_tree->Draw("zeta >> ZEta1Jet_Data","zmass_AllCuts>0 && npfjetsele==1");
	data_tree->Draw("zpt/pfjetpt1 >> PtZoverPtJet_1PFJet_Data","zmass_AllCuts>0 && npfjetsele==1");
	data_tree->Draw("zpt/calojetpt1 >> PtZoverPtJet_1CALOJet_Data","zmass_AllCuts>0 && ncalojetsele==1");
	
	Z_tree->Draw("zeta >> ZEta0Jet_MC","(zmass_AllCuts>0 && npfjetsele==0)*weight");
	Z_tree->Draw("zeta >> ZEta1Jet_MC","(zmass_AllCuts>0 && npfjetsele==1)*weight");
	Z_tree->Draw("zpt/pfjetpt1 >> PtZoverPtJet_1PFJet_MC","(zmass_AllCuts>0 && npfjetsele==1)*weight");
	Z_tree->Draw("zpt/calojetpt1 >> PtZoverPtJet_1CALOJet_MC","(zmass_AllCuts>0 && ncalojetsele==1)*weight");
	Z_tree->Draw("(pfjetpt1-acc_jetgenpt1)/acc_jetgenpt1:acc_jetgenpt1 >> ResJetPt_PF_MC","npfjetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-pfjeteta1)^2+(acc_jetgenphi1-pfjetphi1)^2)<0.0625)");
	Z_tree->Draw("(calojetpt1-acc_jetgenpt1)/acc_jetgenpt1:acc_jetgenpt1 >> ResJetPt_CALO_MC","ncalojetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-calojeteta1)^2+(acc_jetgenphi1-calojetphi1)^2)<0.0625)");
	Z_tree->Draw("(pfjetpt1-acc_jetgenpt1)/acc_jetgenpt1 >> ResJetPt_PF_MC_1D","(npfjetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-pfjeteta1)^2+(acc_jetgenphi1-pfjetphi1)^2)<0.0625))*weight");
	Z_tree->Draw("(calojetpt1-acc_jetgenpt1)/acc_jetgenpt1 >> ResJetPt_CALO_MC_1D","(ncalojetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-calojeteta1)^2+(acc_jetgenphi1-calojetphi1)^2)<0.0625))*weight");
	
	tmp->Close();
	
	//Color, style etc.
	ResJetPt_PF_MC->SetMarkerStyle(7);
	ResJetPt_PF_MC->GetXaxis()->SetTitle("p_{T,Jet}^{gen}");
	ResJetPt_PF_MC->GetYaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	ResJetPt_PF_MC->GetYaxis()->SetTitleOffset(1.15);
	ResJetPt_CALO_MC->SetMarkerStyle(7);
	ResJetPt_CALO_MC->GetXaxis()->SetTitle("p_{T,Jet}^{gen}");
	ResJetPt_CALO_MC->GetYaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	ResJetPt_CALO_MC->GetYaxis()->SetTitleOffset(1.15);
	ResJetPt_PF_MC_1D->SetLineWidth(2);
	ResJetPt_PF_MC_1D->GetXaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	ResJetPt_CALO_MC_1D->SetLineWidth(2);
	ResJetPt_CALO_MC_1D->GetXaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	
	//Normalization - Comparison Data/MC
	ZEta0Jet_Data->Sumw2();
	ZEta0Jet_MC->Scale(scale);
	ZEta1Jet_Data->Sumw2();
	ZEta1Jet_MC->Scale(scale);
	PtZoverPtJet_1PFJet_Data->Sumw2();
	PtZoverPtJet_1PFJet_MC->Scale(scale);
	PtZoverPtJet_1CALOJet_Data->Sumw2();
	PtZoverPtJet_1CALOJet_MC->Scale(scale);
	
	//Plots
	TCanvas *tree_eta0jet = new TCanvas;
	ZEta0Jet_MC->SetLineColor(col_Z);
	ZEta0Jet_MC->SetLineWidth(2);
	ZEta0Jet_MC->SetFillColor(col_Z);
	ZEta0Jet_MC->SetFillStyle(style);
	ZEta0Jet_MC->SetMarkerColor(col_Z);
	ZEta0Jet_MC->GetXaxis()->SetTitle("#eta");
	ZEta0Jet_MC->Draw("hist");
	ZEta0Jet_Data->SetLineWidth(2);
	ZEta0Jet_Data->SetMarkerStyle(20);
	ZEta0Jet_Data->Draw("same");
	tree_eta0jet->Write("Conf_ZEta0Jet.root");
	tree_eta0jet->Close();
	
	TCanvas *tree_eta1jet = new TCanvas;
	ZEta1Jet_MC->SetLineColor(col_Z);
	ZEta1Jet_MC->SetLineWidth(2);
	ZEta1Jet_MC->SetFillColor(col_Z);
	ZEta1Jet_MC->SetFillStyle(style);
	ZEta1Jet_MC->SetMarkerColor(col_Z);
	ZEta1Jet_MC->GetXaxis()->SetTitle("#eta");
	ZEta1Jet_MC->Draw("hist");
	ZEta1Jet_Data->SetLineWidth(2);
	ZEta1Jet_Data->SetMarkerStyle(20);
	ZEta1Jet_Data->Draw("same");
	tree_eta1jet->Write("Conf_ZEta1Jet.root");
	tree_eta1jet->Close();
	
	TCanvas *tree_pfptratio = new TCanvas;
	PtZoverPtJet_1PFJet_MC->SetLineColor(col_Z);
	PtZoverPtJet_1PFJet_MC->SetLineWidth(2);
	PtZoverPtJet_1PFJet_MC->SetFillColor(col_Z);
	PtZoverPtJet_1PFJet_MC->SetFillStyle(style);
	PtZoverPtJet_1PFJet_MC->SetMarkerColor(col_Z);
	PtZoverPtJet_1PFJet_MC->GetXaxis()->SetTitle("p_{T,Z}/p_{T,Jet}");
	PtZoverPtJet_1PFJet_MC->Draw("hist");
	PtZoverPtJet_1PFJet_Data->SetLineWidth(2);
	PtZoverPtJet_1PFJet_Data->SetMarkerStyle(20);
	PtZoverPtJet_1PFJet_Data->Draw("same");
	tree_pfptratio->Write("PtZoverPtJet_1PFJet.root");
	tree_pfptratio->Close();
	
	TCanvas *tree_caloptratio = new TCanvas;
	PtZoverPtJet_1CALOJet_MC->SetLineColor(col_Z);
	PtZoverPtJet_1CALOJet_MC->SetLineWidth(2);
	PtZoverPtJet_1CALOJet_MC->SetFillColor(col_Z);
	PtZoverPtJet_1CALOJet_MC->SetFillStyle(style);
	PtZoverPtJet_1CALOJet_MC->SetMarkerColor(col_Z);
	PtZoverPtJet_1CALOJet_MC->GetXaxis()->SetTitle("p_{T,Z}/p_{T,Jet}");
	PtZoverPtJet_1CALOJet_MC->Draw("hist");
	PtZoverPtJet_1CALOJet_Data->SetLineWidth(2);
	PtZoverPtJet_1CALOJet_Data->SetMarkerStyle(20);
	PtZoverPtJet_1CALOJet_Data->Draw("same");
	tree_caloptratio->Write("PtZoverPtJet_1CALOJet.root");
	tree_caloptratio->Close();
		
  outplots->Write();
  outplots->Close();

}
