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

void Ntuple_Plots(){

string Analysis_Note = "False";
if (Analysis_Note == "False") gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);

string log_scale = "True";

string Tab_cut = "True";
	
	//Signal MC
	TFile *Z_TF = TFile::Open("/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/MC_Winter10_399/Signal/JetPt15/Z_Madgraph_Z2_JetPt15_2.root");
	
	//Data
	TFile *Data_TF = TFile::Open("/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/Data_Dec22ReReco/JetPt15/Data_RUN2010A-B_JetPt15.root");
	
	//Output
	string out = "NtuplePlots_2";        
	string output = out;
	output+=".root";
	TFile* outplots = new TFile(output.c_str(), "RECREATE");
	
	//Normalization factor
	double iniLumi = 50.; //pb-1
	double targetLumi = 36.176; //pb-1
	double scale = 1.;
	if(iniLumi!=0)scale = targetLumi/iniLumi;
	
	//Define trees
	TTree* data_tree   = (TTree*) Data_TF->Get("RecoElectronNtuple/ZeeTree");
	TTree* Z_tree      = (TTree*) Z_TF->Get("RecoElectronNtuple/ZeeTree");

	//rebin
	int rebin_recMassZ = 5;
	int rebin_recPtZ = 2;
	int rebin_recEtaZ = 5;
		
	//colors	
	int col_Z = 800;
	int col_Data= 1;

	//FillStyle	
	int style = 1001;
	
	//Legenda
	string Leg_Z = "Z+jets";
	string Leg_Data	= "Data";
	
	//Define directories
	TDirectory *ZEta, *ZMass, *ZoJet_Pt, *JetRes, *Unfolding;
	ZEta = outplots->mkdir("ZEta_0-1Jets");
	ZMass = outplots->mkdir("ZMass_Barrel-Endcap");
	ZoJet_Pt = outplots->mkdir("ZoverJetPt_1Jet");
	JetRes = outplots->mkdir("JetResolution");
	Unfolding = outplots->mkdir("Unfolding");
	
	//Define Histograms
	ZEta->cd();
	TH1D* ZEta0Jet_Data = new TH1D("ZEta0Jet_Data","Z Eta Data - 0 Jet",20,-10,10);
	TH1D* ZEta0Jet_MC = new TH1D("ZEta0Jet_MC","Z Eta MC - 0 Jet",20,-10,10);
	TH1D* ZEta1Jet_Data = new TH1D("ZEta1Jet_Data","Z Eta Data - 1 Jet",20,-10,10);
	TH1D* ZEta1Jet_MC = new TH1D("ZEta1Jet_MC","Z Eta MC - 1 Jet",20,-10,10);
	
	ZoJet_Pt->cd();
	TH1D* PtZoverPtJet_1PFJet_Data = new TH1D("PtZoverPtJet_1PFJet_Data","PtZoverPtJet 1PFJet Data",24,0,3);
	TH1D* PtZoverPtJet_1PFJet_MC = new TH1D("PtZoverPtJet_1PFJet_MC","PtZoverPtJet 1PFJet MC",24,0,3);
	TH1D* PtZoverPtJet_1PFLICORRJet_Data = new TH1D("PtZoverPtJet_1PFLICORRJet_Data","PtZoverPtJet 1PFLICORRJet Data",24,0,3);
	TH1D* PtZoverPtJet_1PFLICORRJet_MC = new TH1D("PtZoverPtJet_1PFLICORRJet_MC","PtZoverPtJet 1PFLICORRJet MC",24,0,3);
	
	JetRes->cd();
	TH2D* ResJetPt_PF_MC = new TH2D("ResJetPt_PF_MC","Resolution of PF Jet Pt - MC",140,10,150,200,-1,1);
	TH2D* ResJetPt_PFLICORR_MC = new TH2D("ResJetPt_PFLICORR_MC","Resolution of PFLICORR Jet Pt - MC",140,10,150,200,-1,1);
	TH1D* ResJetPt_PF_MC_1D = new TH1D("ResJetPt_PF_MC_1D","Resolution of PF Jet Pt - MC",80,-1,1);
	TH1D* ResJetPt_PFLICORR_MC_1D = new TH1D("ResJetPt_PFLICORR_MC_1D","Resolution of PF Jet Pt - MC",80,-1,1);
	
	TH1D* DeltaPhiZJet =  new TH1D("DeltaPhiZJet","Delta Phi separation from Z and jet in 1 jet events",80,-7,7);
	
	ZMass->cd();
	TH1D* ZMass_Data = new TH1D("ZMass_Data","Z Mass, Data (black) MC (red)",200,50,150);
	TH1D* ZMass_Data_Barrel = new TH1D("ZMass_Data_Barrel","Z Mass, Data (black) MC (red) - Barrel",200,50,150);
	TH1D* ZMass_Data_Endcap = new TH1D("ZMass_Data_Endcap","Z Mass, Data (black) MC (red) - Endcap",200,50,150);
	TH1D* ZMass_MC = new TH1D("ZMass_MC","Z Mass, Data (black) MC (red)",200,50,150);
	TH1D* ZMass_MC_Barrel = new TH1D("ZMass_MC_Barrel","Z Mass, Data (black) MC (red) - Barrel",200,50,150);
	TH1D* ZMass_MC_Endcap = new TH1D("ZMass_MC_Endcap","Z Mass, Data (black) MC (red) - Endcap",200,50,150);
	
	Unfolding->cd();	
	TH2D* UnfoldingPlot_Scat_PFL1 = new TH2D("UnfoldingPlot_Scat_PFL1","Response Matrix - Gen Jet vs PFL1 Jet",6,-0.5,5.5,6,-0.5,5.5);
	TH2D* UnfoldingPlot_Scat_PF = new TH2D("UnfoldingPlot_Scat_PF","Response Matrix - Gen Jet vs PF Jet",6,-0.5,5.5,6,-0.5,5.5);
	
	//Build histograms by Tree
	
	ZEta->cd();
	TCanvas *tmp1 = new TCanvas;
	data_tree->Draw("zeta >> ZEta0Jet_Data","zmass_AllCuts>0 && npfjetsele==0");
	data_tree->Draw("zeta >> ZEta1Jet_Data","zmass_AllCuts>0 && npfjetsele==1");
	Z_tree->Draw("zeta >> ZEta0Jet_MC","(zmass_AllCuts>0 && npfjetsele==0)*weight");
	Z_tree->Draw("zeta >> ZEta1Jet_MC","(zmass_AllCuts>0 && npfjetsele==1)*weight");
	tmp1->Close();
	
	ZoJet_Pt->cd();
	TCanvas *tmp2 = new TCanvas;
	data_tree->Draw("zpt/pfjetpt1 >> PtZoverPtJet_1PFJet_Data","zmass_AllCuts>0 && npfjetsele==1 && pfjetpt1>15 && pfjeteta1<2.5");
	data_tree->Draw("zpt/pfl1jetpt1 >> PtZoverPtJet_1PFLICORRJet_Data","zmass_AllCuts>0 && npfl1jetsele==1 && pfl1jetpt1>30 && pfl1jeteta1<3");
	Z_tree->Draw("zpt/pfjetpt1 >> PtZoverPtJet_1PFJet_MC","(zmass_AllCuts>0 && npfjetsele==1 && pfjetpt1>15 && pfjeteta1<2.5)*weight");
	Z_tree->Draw("zpt/pfl1jetpt1 >> PtZoverPtJet_1PFLICORRJet_MC","(zmass_AllCuts>0 && npfl1jetsele==1 && pfl1jetpt1>30 && pfl1jeteta1<3)*weight");
	tmp2->Close();
	
	JetRes->cd();
	TCanvas *tmp3 = new TCanvas;
	Z_tree->Draw("(pfjetpt1-acc_jetgenpt1)/acc_jetgenpt1:acc_jetgenpt1 >> ResJetPt_PF_MC","npfjetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-pfjeteta1)^2+(acc_jetgenphi1-pfjetphi1)^2)<0.0625)");
	Z_tree->Draw("(pfl1jetpt1-acc_jetgenpt1)/acc_jetgenpt1:acc_jetgenpt1 >> ResJetPt_PFLICORR_MC","npfl1jetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-pfl1jeteta1)^2+(acc_jetgenphi1-pfl1jetphi1)^2)<0.0625)");
	Z_tree->Draw("(pfjetpt1-acc_jetgenpt1)/acc_jetgenpt1 >> ResJetPt_PF_MC_1D","(npfjetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-pfjeteta1)^2+(acc_jetgenphi1-pfjetphi1)^2)<0.0625))*weight");
	Z_tree->Draw("(pfl1jetpt1-acc_jetgenpt1)/acc_jetgenpt1 >> ResJetPt_PFLICORR_MC_1D","(npfl1jetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-pfl1jeteta1)^2+(acc_jetgenphi1-pfl1jetphi1)^2)<0.0625))*weight");
	Z_tree->Draw("pfjetphi1-zphi >> DeltaPhiZJet","(zmass_AllCuts>0 && npfjetsele==1)*weight");
	tmp3->Close();
	
	ZMass->cd();
	TCanvas *tmp4 = new TCanvas;
	data_tree->Draw("zmass >> ZMass_Data","cutAccASYM==1 && cutTrg==1 && cutImp==1 && cutConvASYM==1 && cutIsoASYM==1 && cutEiDASYM==1");
	data_tree->Draw("zmass >> ZMass_Data_Barrel","(elept1>20 && elept1>10 && eleeta1<1.4442 && eleeta2<1.4442) && (cutTrg==1 && cutImp==1 && cutConvASYM==1 && cutIsoASYM==1 && cutEiDASYM==1)");
	data_tree->Draw("zmass >> ZMass_Data_Endcap","(elept1>20 && elept1>10 && eleeta1<2.5 && eleeta2<2.5 && eleeta1>1.566 && eleeta2>1.566) && (cutTrg==1 && cutImp==1 && cutConvASYM==1 && cutIsoASYM==1 && cutEiDASYM==1)");
	Z_tree->Draw("zmass >> ZMass_MC","(cutAccASYM==1 && cutTrg==1 && cutImp==1 && cutConvASYM==1 && cutIsoASYM==1 && cutEiDASYM==1)*weight");
	Z_tree->Draw("zmass >> ZMass_MC_Barrel","(elept1>20 && elept1>10 && eleeta1<1.4442 && eleeta2<1.4442 && cutTrg==1 && cutImp==1 && cutConvASYM==1 && cutIsoASYM==1 && cutEiDASYM==1)*weight");
	Z_tree->Draw("zmass >> ZMass_MC_Endcap","(elept1>20 && elept1>10 && eleeta1<2.5 && eleeta2<2.5 && eleeta1>1.566 && eleeta2>1.566 && cutTrg==1 && cutImp==1 && cutConvASYM==1 && cutIsoASYM==1 && cutEiDASYM==1)*weight");
	tmp4->Close();
	
	//Unfolding Box Plot
	
	Unfolding->cd();
	TCanvas *unf_pfl1 = new TCanvas;
	Z_tree->Draw("acc_gennjetsele:genacc_npfl1jetsele >> UnfoldingPlot_Scat_PFL1","(acc_gennjetsele>-1)*weight");	
	UnfoldingPlot_Scat_PFL1->GetXaxis()->SetNdivisions(10);
	UnfoldingPlot_Scat_PFL1->GetXaxis()->SetTitle("RECO Jets Number");
	UnfoldingPlot_Scat_PFL1->GetYaxis()->SetNdivisions(10);
	UnfoldingPlot_Scat_PFL1->GetYaxis()->SetTitle("GEN Jets Number");
	UnfoldingPlot_Scat_PFL1->SetLineWidth(2);
	UnfoldingPlot_Scat_PFL1->Draw("box");
	unf_pfl1->Write("UnfoldingPlot_Box_PFL1.root");
	unf_pfl1->Close();
	
	TCanvas *unf_pf = new TCanvas;
	Z_tree->Draw("acc_gennjetsele:genacc_npfjetsele >> UnfoldingPlot_Scat_PF","(acc_gennjetsele>-1)*weight");	
	UnfoldingPlot_Scat_PF->GetXaxis()->SetNdivisions(10);
	UnfoldingPlot_Scat_PF->GetXaxis()->SetTitle("RECO Jets Number");
	UnfoldingPlot_Scat_PF->GetYaxis()->SetNdivisions(10);
	UnfoldingPlot_Scat_PF->GetYaxis()->SetTitle("GEN Jets Number");
	UnfoldingPlot_Scat_PF->SetLineWidth(2);
	UnfoldingPlot_Scat_PF->Draw("box");
	unf_pf->Write("UnfoldingPlot_Box_PF.root");
	unf_pf->Close();	
	
	//Color, style etc.
	ResJetPt_PF_MC->SetMarkerStyle(7);
	ResJetPt_PF_MC->GetXaxis()->SetTitle("p_{T,Jet}^{gen}");
	ResJetPt_PF_MC->GetYaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	ResJetPt_PF_MC->GetYaxis()->SetTitleOffset(1.15);
	ResJetPt_PFLICORR_MC->SetMarkerStyle(7);
	ResJetPt_PFLICORR_MC->GetXaxis()->SetTitle("p_{T,Jet}^{gen}");
	ResJetPt_PFLICORR_MC->GetYaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	ResJetPt_PFLICORR_MC->GetYaxis()->SetTitleOffset(1.15);
	ResJetPt_PF_MC_1D->SetLineWidth(2);
	ResJetPt_PF_MC_1D->GetXaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	ResJetPt_PFLICORR_MC_1D->SetLineWidth(2);
	ResJetPt_PFLICORR_MC_1D->GetXaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	
	//Normalization - Comparison Data/MC
	ZEta0Jet_Data->Sumw2();
	ZEta0Jet_MC->Scale(scale);
	ZEta1Jet_Data->Sumw2();
	ZEta1Jet_MC->Scale(scale);
	PtZoverPtJet_1PFJet_Data->Sumw2();
	PtZoverPtJet_1PFJet_MC->Scale(scale);
	PtZoverPtJet_1PFLICORRJet_Data->Sumw2();
	PtZoverPtJet_1PFLICORRJet_MC->Scale(scale);
	
	//ZMass_Data->Sumw2();
	ZMass_MC->Scale(scale);
	//ZMass_Data_Barrel->Sumw2();
	ZMass_MC_Barrel->Scale(scale);
	//ZMass_Data_Endcap->Sumw2();
	ZMass_MC_Endcap->Scale(scale);
	
	//Plots
	ZEta->cd();
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
	
	ZoJet_Pt->cd();
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
	
	TCanvas *tree_pfl1ptratio = new TCanvas;
	PtZoverPtJet_1PFLICORRJet_MC->SetLineColor(col_Z);
	PtZoverPtJet_1PFLICORRJet_MC->SetLineWidth(2);
	PtZoverPtJet_1PFLICORRJet_MC->SetFillColor(col_Z);
	PtZoverPtJet_1PFLICORRJet_MC->SetFillStyle(style);
	PtZoverPtJet_1PFLICORRJet_MC->SetMarkerColor(col_Z);
	PtZoverPtJet_1PFLICORRJet_MC->GetXaxis()->SetTitle("p_{T,Z}/p_{T,Jet}");
	PtZoverPtJet_1PFLICORRJet_MC->Draw("hist");
	PtZoverPtJet_1PFLICORRJet_Data->SetLineWidth(2);
	PtZoverPtJet_1PFLICORRJet_Data->SetMarkerStyle(20);
	PtZoverPtJet_1PFLICORRJet_Data->Draw("same");
	tree_pfl1ptratio->Write("PtZoverPtJet_1PFLICORRJet.root");
	tree_pfl1ptratio->Close();
	
	ZMass->cd();
	TCanvas *tree_zmass = new TCanvas;
	ZMass_MC->SetLineColor(2);
	ZMass_MC->SetLineWidth(2);
	ZMass_MC->SetMarkerColor(col_Z);
	ZMass_MC->Rebin(rebin_recMassZ);
	ZMass_MC->Draw("hist");
	ZMass_Data->SetLineWidth(2);
	ZMass_Data->SetMarkerStyle(20);
	ZMass_Data->Rebin(rebin_recMassZ);
	ZMass_Data->Draw("same");
	tree_zmass->Write("ZMass_Data-MC.root");
	tree_zmass->Close();
	
	TCanvas *tree_zmass_barrel = new TCanvas;
	ZMass_MC_Barrel->SetLineColor(2);
	ZMass_MC_Barrel->SetLineWidth(2);
	ZMass_MC_Barrel->SetMarkerColor(col_Z);
	ZMass_MC_Barrel->Rebin(rebin_recMassZ);
	ZMass_MC_Barrel->Draw("hist");
	ZMass_Data_Barrel->SetLineWidth(2);
	ZMass_Data_Barrel->SetMarkerStyle(20);
	ZMass_Data_Barrel->Rebin(rebin_recMassZ);
	ZMass_Data_Barrel->Draw("same");
	tree_zmass_barrel->Write("ZMass_Data-MC_Barrel.root");
	tree_zmass_barrel->Close();
	
	TCanvas *tree_zmass_endcap = new TCanvas;
	ZMass_MC_Endcap->SetLineColor(2);
	ZMass_MC_Endcap->SetLineWidth(2);
	ZMass_MC_Endcap->SetMarkerColor(col_Z);
	ZMass_MC_Endcap->Rebin(rebin_recMassZ);
	ZMass_MC_Endcap->Draw("hist");
	ZMass_Data_Endcap->SetLineWidth(2);
	ZMass_Data_Endcap->SetMarkerStyle(20);
	ZMass_Data_Endcap->Rebin(rebin_recMassZ);
	ZMass_Data_Endcap->Draw("same");
	tree_zmass_endcap->Write("ZMass_Data-MC_Endcap.root");
	tree_zmass_endcap->Close();
		
  outplots->Write();
  outplots->Close();

}
