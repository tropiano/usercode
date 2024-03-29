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

//JetPtMin in GeV; Tune Z2 or D6T; jecUnc: "0" = NotApplied & Not Norm, "0N" = NotApplied & Norm, "+" = Added, "-" = Subtracted 

void Ntuple_Plots(int JetPtMin, string tune, string jecUnc){ 

string log_scale = "True";

string Tab_cut = "True";

string path = "/data/sfrosali/Zjets/NEW_CMSSW_3_9_7/CMSSW_3_9_7/src/Firenze/PATAnalysis/bin";
string JetPtCut;
if(JetPtMin==15)JetPtCut="JetPt15";
if(JetPtMin==30)JetPtCut="JetPt30";

//JEC Uncertainty
string JEC="";
if(jecUnc=="0N")JEC="";
if(jecUnc=="0")JEC="_NotNorm";
if(jecUnc=="+")JEC="_jecUncPlus";
if(jecUnc=="-")JEC="_jecUncMinus";
	
	//Signal MC
	TFile *Z_TF = TFile::Open((path+"/MC_Winter10/Signal/"+JetPtCut+"/Z_Madgraph_"+tune+"_"+JetPtCut+JEC+".root").c_str());
	
	//Data
	TFile *Data_TF = TFile::Open((path+"/Data_Dec22ReReco/"+JetPtCut+"/Data_RUN2010A-B_"+JetPtCut+".root").c_str());
	
	//Output
	string JECout;
	if(jecUnc=="0") JECout="";
	if(jecUnc=="+") JECout="_jecUncPlus";
        if(jecUnc=="-") JECout="_jecUncMinus";
        
	string out = "NtuplePlots"+JECout+"_";
	out+=JetPtCut;     
	string output = out;
	output+=".root";
	TFile* outplots = new TFile(output.c_str(), "RECREATE");
	
	//Report files
        ofstream ntuplerep;
	ntuplerep.open(("NtupleReport_"+JetPtCut+JECout+".txt").c_str());
	ntuplerep<<endl;
	
	//Normalization factor
	double iniLumi;
	if(jecUnc=="0" && tune=="Z2") iniLumi = 1593.52; //pb-1 Z2
	if(jecUnc=="0" && tune=="D6T") iniLumi = 492.126; //pb-1 D6T
	if(jecUnc=="0N") iniLumi = 50.; //pb-1
	double targetLumi = 36.162; //pb-1
	double scale = 1.;
	if(iniLumi!=0)scale = targetLumi/iniLumi;
	
	ostringstream scale_stream;
        scale_stream << scale;
        string scale_str = scale_stream.str();
	
	//Define trees
	TTree* data_tree   = (TTree*) Data_TF->Get("RecoElectronNtuple/ZeeTree");
	TTree* Z_tree      = (TTree*) Z_TF->Get("RecoElectronNtuple/ZeeTree");

	//rebin
	int rebin_recMassZ = 1;
	int rebin_recPtZ = 3;
	int rebin_recEtaZ = 2;
		
	//colors	
	int col_Z = 800;
	int col_Data= 1;

	//FillStyle	
	int style = 1001;
	
	//Legenda
	string Leg_Z = "Z+jets";
	string Leg_Data	= "Data";
	
	//Define directories
	TDirectory *ZEta, *ZMass, *ZoJet_Pt, *JetRes, *Unfolding, *Matrix, *CorrFact;
	ZEta = outplots->mkdir("ZEta_0-1Jets");
	ZMass = outplots->mkdir("ZMass_Barrel-Endcap");
	ZoJet_Pt = outplots->mkdir("JetOverZPt_1Jet");
	JetRes = outplots->mkdir("JetResolution");
	Unfolding = outplots->mkdir("Unfolding");
	Matrix = Unfolding->mkdir("ResponseMatrix");
	CorrFact = Unfolding->mkdir("CorrFactors");
	
	//Define Histograms
	ZEta->cd();
	TH1D* ZEta0Jet_Data = new TH1D("ZEta0Jet_Data","Z Eta Data - 0 Jet",200,-10,10);
	TH1D* ZEta0Jet_MC = new TH1D("ZEta0Jet_MC","Z Eta MC - 0 Jet",200,-10,10);
	TH1D* ZEta1Jet_Data = new TH1D("ZEta1Jet_Data","Z Eta Data - 1 Jet",200,-10,10);
	TH1D* ZEta1Jet_MC = new TH1D("ZEta1Jet_MC","Z Eta MC - 1 Jet",200,-10,10);
	
	ZoJet_Pt->cd();
	TH1D* PtJetOverPtZ_1PFJet_Data = new TH1D("PtJetOverPtZ_1PFJet_Data","PtJetOverPtZ 1PFJet Data",300,0,3);
	TH1D* PtJetOverPtZ_1PFJet_MC = new TH1D("PtJetOverPtZ_1PFJet_MC","PtJetOverPtZ 1PFJet MC",300,0,3);
	TH1D* PtJetOverPtZ_1PFL1CORRJet_Data = new TH1D("PtJetOverPtZ_1PFL1CORRJet_Data","PtJetOverPtZ 1PFL1CORRJet Data",300,0,3);
	TH1D* PtJetOverPtZ_1PFL1CORRJet_MC = new TH1D("PtJetOverPtZ_1PFL1CORRJet_MC","PtJetOverPtZ 1PFL1CORRJet MC",300,0,3);
	
	JetRes->cd();
	TH2D* ResJetPt_PF_MC = new TH2D("ResJetPt_PF_MC","Resolution of PF Jet Pt - MC",140,10,150,200,-1,1);
	TH2D* ResJetPt_PFL1CORR_MC = new TH2D("ResJetPt_PFL1CORR_MC","Resolution of PFL1CORR Jet Pt - MC",140,10,150,200,-1,1);
	TH1D* ResJetPt_PF_MC_1D = new TH1D("ResJetPt_PF_MC_1D","Resolution of PF Jet Pt - MC",80,-1,1);
	TH1D* ResJetPt_PFL1CORR_MC_1D = new TH1D("ResJetPt_PFL1CORR_MC_1D","Resolution of PF Jet Pt - MC",80,-1,1);
	
	TH1D* DeltaPhiZJet =  new TH1D("DeltaPhiZJet","Delta Phi separation from Z and jet in 1 jet events",80,-7,7);
	
	ZMass->cd();
	TH1D* ZMass_Data = new TH1D("ZMass_Data","Z Mass, Data (black) MC (red)",200,50,150);
	TH1D* ZMass_Data_Barrel = new TH1D("ZMass_Data_Barrel","Z Mass, Data (black) MC (red) - Barrel",200,50,150);
	TH1D* ZMass_Data_Endcap = new TH1D("ZMass_Data_Endcap","Z Mass, Data (black) MC (red) - Endcap",200,50,150);
	TH1D* ZMass_MC = new TH1D("ZMass_MC","Z Mass, Data (black) MC (red)",200,50,150);
	TH1D* ZMass_MC_Barrel = new TH1D("ZMass_MC_Barrel","Z Mass, Data (black) MC (red) - Barrel",200,50,150);
	TH1D* ZMass_MC_Endcap = new TH1D("ZMass_MC_Endcap","Z Mass, Data (black) MC (red) - Endcap",200,50,150);
	
	Matrix->cd();	
	TH2D* UnfoldingPlot_Scat_PFL1 = new TH2D("UnfoldingPlot_Scat_PFL1","Response Matrix - Gen Jet vs PFL1 Jet",5,-0.5,4.5,5,-0.5,4.5);
	CorrFact->cd();
	TH1D* GENJet_ExclMult = new TH1D("GENJet_ExclMult","Gen Jet Excl Mult.",5,-0.5,4.5);
        TH1D* RECOJet_ExclMult = new TH1D("RECOJet_ExclMult","Reco Jet Excl Mult.",5,-0.5,4.5);
        TH1D* GENJet_InclMult = new TH1D("GENJet_InclMult","Gen Jet Incl Mult.",5,-0.5,4.5);
        TH1D* RECOJet_InclMult = new TH1D("RECOJet_InclMult","Reco Jet Incl Mult.",5,-0.5,4.5);
	
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
	data_tree->Draw("pfjetpt1/zpt >> PtJetOverPtZ_1PFJet_Data","zmass_AllCuts>0 && npfjetsele==1");
	data_tree->Draw("pfl1jetpt1/zpt >> PtJetOverPtZ_1PFL1CORRJet_Data","zmass_AllCuts>0 && npfl1jetsele==1");
	Z_tree->Draw("pfjetpt1/zpt >> PtJetOverPtZ_1PFJet_MC","(zmass_AllCuts>0 && npfjetsele==1)*weight");
	Z_tree->Draw("pfl1jetpt1/zpt >> PtJetOverPtZ_1PFL1CORRJet_MC","(zmass_AllCuts>0 && npfl1jetsele==1)*weight");
	tmp2->Close();
	
	JetRes->cd();
	TCanvas *tmp3 = new TCanvas;
	Z_tree->Draw("(pfjetpt1-acc_jetgenpt1)/acc_jetgenpt1:acc_jetgenpt1 >> ResJetPt_PF_MC","npfjetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-pfjeteta1)^2+(acc_jetgenphi1-pfjetphi1)^2)<0.0625)");
	Z_tree->Draw("(pfl1jetpt1-acc_jetgenpt1)/acc_jetgenpt1:acc_jetgenpt1 >> ResJetPt_PFL1CORR_MC","npfl1jetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-pfl1jeteta1)^2+(acc_jetgenphi1-pfl1jetphi1)^2)<0.0625)");
	Z_tree->Draw("(pfjetpt1-acc_jetgenpt1)/acc_jetgenpt1 >> ResJetPt_PF_MC_1D","(npfjetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-pfjeteta1)^2+(acc_jetgenphi1-pfjetphi1)^2)<0.0625))*weight");
	Z_tree->Draw("(pfl1jetpt1-acc_jetgenpt1)/acc_jetgenpt1 >> ResJetPt_PFL1CORR_MC_1D","(npfl1jetsele>0 && acc_gennjetsele>0 && (((acc_jetgeneta1-pfl1jeteta1)^2+(acc_jetgenphi1-pfl1jetphi1)^2)<0.0625))*weight");
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
	
	//Color, style etc.
	ResJetPt_PF_MC->SetMarkerStyle(7);
	ResJetPt_PF_MC->GetXaxis()->SetTitle("p_{T,Jet}^{gen}");
	ResJetPt_PF_MC->GetYaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	ResJetPt_PF_MC->GetYaxis()->SetTitleOffset(1.15);
	ResJetPt_PFL1CORR_MC->SetMarkerStyle(7);
	ResJetPt_PFL1CORR_MC->GetXaxis()->SetTitle("p_{T,Jet}^{gen}");
	ResJetPt_PFL1CORR_MC->GetYaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	ResJetPt_PFL1CORR_MC->GetYaxis()->SetTitleOffset(1.15);
	ResJetPt_PF_MC_1D->SetLineWidth(2);
	ResJetPt_PF_MC_1D->GetXaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	ResJetPt_PFL1CORR_MC_1D->SetLineWidth(2);
	ResJetPt_PFL1CORR_MC_1D->GetXaxis()->SetTitle("(p_{T,Jet}^{reco}-p_{T,Jet}^{gen})/p_{T,Jet}^{gen}");
	
	//Normalization - Comparison Data/MC
	ZEta0Jet_Data->Sumw2();
	ZEta0Jet_MC->Scale(scale);
	ZEta1Jet_Data->Sumw2();
	ZEta1Jet_MC->Scale(scale);
	PtJetOverPtZ_1PFJet_Data->Sumw2();
	PtJetOverPtZ_1PFJet_MC->Scale(scale);
	PtJetOverPtZ_1PFL1CORRJet_Data->Sumw2();
	PtJetOverPtZ_1PFL1CORRJet_MC->Scale(scale);
	
	//ZMass_Data->Sumw2();
	ZMass_MC->Scale(scale);
	//ZMass_Data_Barrel->Sumw2();
	ZMass_MC_Barrel->Scale(scale);
	//ZMass_Data_Endcap->Sumw2();
	ZMass_MC_Endcap->Scale(scale);
	
	//////////////////////// Unfolding
	
	Unfolding->cd();
	
	//Unfolding Box Plot
	
	Matrix->cd();	
	
	TCanvas *unf_pfl1 = new TCanvas;
	Z_tree->Draw("acc_gennjetsele:genacc_npfl1jetsele >> UnfoldingPlot_Scat_PFL1","acc_gennjetsele>-1");	
	UnfoldingPlot_Scat_PFL1->GetXaxis()->SetNdivisions(5);
	UnfoldingPlot_Scat_PFL1->GetXaxis()->SetTitle("RECO Jets");
	UnfoldingPlot_Scat_PFL1->GetYaxis()->SetNdivisions(5);
	UnfoldingPlot_Scat_PFL1->GetYaxis()->SetTitle("GEN Jets");
	UnfoldingPlot_Scat_PFL1->SetLineWidth(2);	
	UnfoldingPlot_Scat_PFL1->Draw("box");
	unf_pfl1->Write("UnfoldingPlot_Box_PFL1.root");
	unf_pfl1->Close();
	
	TH1D *projY = UnfoldingPlot_Scat_PFL1->ProjectionY();
	TH2D* Norm_UnfoldingPlot_Scat_PFL1 = new TH2D("Norm_UnfoldingPlot_Scat_PFL1","Response Matrix - Gen Jet vs PFL1 Jet",5,-0.5,4.5,5,-0.5,4.5);
	int BinNorm=0;
	for(int i=1;i<6;i++){
	BinNorm = projY->GetBinContent(i);
	for(int j=1;j<6;j++){
	Norm_UnfoldingPlot_Scat_PFL1->SetBinContent(j,i,UnfoldingPlot_Scat_PFL1->GetBinContent(j,i)/BinNorm);
	}}
	
	TCanvas *normunf_pfl1 = new TCanvas;
	Norm_UnfoldingPlot_Scat_PFL1->GetXaxis()->SetNdivisions(5);
	Norm_UnfoldingPlot_Scat_PFL1->GetXaxis()->SetTitle("RECO Jets");
	Norm_UnfoldingPlot_Scat_PFL1->GetYaxis()->SetNdivisions(5);
	Norm_UnfoldingPlot_Scat_PFL1->GetYaxis()->SetTitle("GEN Jets");
	Norm_UnfoldingPlot_Scat_PFL1->SetMarkerSize(1.5);
	Norm_UnfoldingPlot_Scat_PFL1->SetLineWidth(2);	
	Norm_UnfoldingPlot_Scat_PFL1->Draw("textcolz");
	normunf_pfl1->Write("Norm_UnfoldingPlot_Box_PFL1.root");
	normunf_pfl1->Close();
       
        //Make Multiplicity histograms
        
        CorrFact->cd();
        
        TCanvas *tmp5 = new TCanvas;
        Z_tree->Draw("acc_gennjetsele >> GENJet_ExclMult","acc_gennjetsele>-1");
        Z_tree->Draw("genacc_npfl1jetsele >> RECOJet_ExclMult","acc_gennjetsele>-1");
        tmp5->Close();
        
        double errGen, errReco;
	
	for(int n=1; n<6; n++){
	
	errGen=0.;
	errReco=0.;
	
	GENJet_InclMult->SetBinContent(n, GENJet_ExclMult->IntegralAndError(n, 10, errGen));
	GENJet_InclMult->SetBinError(n, errGen);
	RECOJet_InclMult->SetBinContent(n, RECOJet_ExclMult->IntegralAndError(n, 10, errReco));
	RECOJet_InclMult->SetBinError(n, errReco);
	
	}
	
        // Correction Factors
        
        GENJet_ExclMult->Write("GENJet_ExclMult");
        
        TCanvas *GenRec_ExclMult = new TCanvas;
        GENJet_ExclMult->GetXaxis()->SetTitle("Jet multiplicity");
	GENJet_ExclMult->SetTitle("Reconstructed (black) and Generated (red) Exclusive Multiplicity");
        GENJet_ExclMult->SetLineColor(2);
	GENJet_ExclMult->SetLineWidth(2);
	GENJet_ExclMult->Draw("hist");
	RECOJet_ExclMult->SetLineColor(1);
	RECOJet_ExclMult->SetLineWidth(2);	
	RECOJet_ExclMult->Draw("hist sames");	
	GenRec_ExclMult->Write("GenRec_ExclMult.root");
	GenRec_ExclMult->Close();
		
	GENJet_ExclMult->Sumw2();
	RECOJet_ExclMult->Sumw2();
	
	GENJet_ExclMult->Divide(RECOJet_ExclMult);        
        GENJet_ExclMult->SetName("Exclusive_CorrFactors");
        GENJet_ExclMult->SetTitle("Exclusive Correction Factors");
        GENJet_ExclMult->SetLineColor(1);
        GENJet_ExclMult->SetLineWidth(2);
        GENJet_ExclMult->SetMarkerStyle(20);
	GENJet_ExclMult->GetXaxis()->SetTitle("Exclusive Multiplicity");
	GENJet_ExclMult->GetYaxis()->SetTitle("Correction Factors");
	GENJet_ExclMult->GetYaxis()->SetRangeUser(0.4,1.3);
	
	//Inclusive Mult.	
	
	GENJet_InclMult->Write("GENJet_InclMult");
        
        TCanvas *GenRec_InclMult = new TCanvas;
        GENJet_InclMult->GetXaxis()->SetTitle("Jet multiplicity");
	GENJet_InclMult->SetTitle("Reconstructed (black) and Generated (red) Inclusive Multiplicity");
        GENJet_InclMult->SetLineColor(2);
	GENJet_InclMult->SetLineWidth(2);
	GENJet_InclMult->Draw("hist");
	RECOJet_InclMult->SetLineColor(1);
	RECOJet_InclMult->SetLineWidth(2);	
	RECOJet_InclMult->Draw("hist sames");	
	GenRec_InclMult->Write("GenRec_InclMult.root");
	GenRec_InclMult->Close();
		
	GENJet_InclMult->Sumw2();
	RECOJet_InclMult->Sumw2();
	
	GENJet_InclMult->Divide(RECOJet_InclMult);        
        GENJet_InclMult->SetName("Inclusive_CorrFactors");
        GENJet_InclMult->SetTitle("Inclusive Correction Factors");
        GENJet_InclMult->SetLineColor(1);
        GENJet_InclMult->SetLineWidth(2);
        GENJet_InclMult->SetMarkerStyle(20);
	GENJet_InclMult->GetXaxis()->SetTitle("Inclusive Multiplicity");
	GENJet_InclMult->GetYaxis()->SetTitle("Correction Factors");
	GENJet_InclMult->GetYaxis()->SetRangeUser(0.4,1.3);

	//Plots
	ZEta->cd();
	TCanvas *tree_eta0jet = new TCanvas;
	ZEta0Jet_MC->SetLineColor(col_Z);
	ZEta0Jet_MC->SetLineWidth(2);
	ZEta0Jet_MC->SetFillColor(col_Z);
	ZEta0Jet_MC->SetFillStyle(style);
	ZEta0Jet_MC->SetMarkerColor(col_Z);
	ZEta0Jet_MC->GetXaxis()->SetTitle("#eta");
	ZEta0Jet_MC->Rebin(rebin_recEtaZ);
	ZEta0Jet_MC->Draw("hist");
	ZEta0Jet_Data->SetLineWidth(2);
	ZEta0Jet_Data->SetMarkerStyle(20);
	ZEta0Jet_Data->Rebin(rebin_recEtaZ);
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
	ZEta1Jet_MC->Rebin(rebin_recEtaZ);
	ZEta1Jet_MC->Draw("hist");
	ZEta1Jet_Data->SetLineWidth(2);
	ZEta1Jet_Data->SetMarkerStyle(20);
	ZEta1Jet_Data->Rebin(rebin_recEtaZ);
	ZEta1Jet_Data->Draw("same");
	tree_eta1jet->Write("Conf_ZEta1Jet.root");
	tree_eta1jet->Close();
		
	ZoJet_Pt->cd();
	TCanvas *tree_pfptratio = new TCanvas;
	PtJetOverPtZ_1PFJet_MC->SetLineColor(col_Z);
	PtJetOverPtZ_1PFJet_MC->SetLineWidth(2);
	PtJetOverPtZ_1PFJet_MC->SetFillColor(col_Z);
	PtJetOverPtZ_1PFJet_MC->SetFillStyle(style);
	PtJetOverPtZ_1PFJet_MC->SetMarkerColor(col_Z);
	PtJetOverPtZ_1PFJet_MC->GetXaxis()->SetTitle("p_{T,Jet}/p_{T,Z}");
	PtJetOverPtZ_1PFJet_MC->Rebin(rebin_recPtZ);
	PtJetOverPtZ_1PFJet_MC->Draw("hist");
	PtJetOverPtZ_1PFJet_Data->SetLineWidth(2);
	PtJetOverPtZ_1PFJet_Data->SetMarkerStyle(20);
	PtJetOverPtZ_1PFJet_Data->Rebin(rebin_recPtZ);
	PtJetOverPtZ_1PFJet_Data->Draw("same");
	tree_pfptratio->Write("PtJetOverPtZ_1PFJet.root");
	tree_pfptratio->Close();
	
	ntuplerep<<"PtJet/PtZ - L2L3 - MC Mean = "<<PtJetOverPtZ_1PFJet_MC->GetMean()<<" +/- "<<PtJetOverPtZ_1PFJet_MC->GetMeanError()<<endl;
	ntuplerep<<"PtJet/PtZ - L2L3 - DATA Mean = "<<PtJetOverPtZ_1PFJet_Data->GetMean()<<" +/- "<<PtJetOverPtZ_1PFJet_Data->GetMeanError()<<endl<<endl;
	
	TCanvas *tree_pfl1ptratio = new TCanvas;
	PtJetOverPtZ_1PFL1CORRJet_MC->SetLineColor(col_Z);
	PtJetOverPtZ_1PFL1CORRJet_MC->SetLineWidth(2);
	PtJetOverPtZ_1PFL1CORRJet_MC->SetFillColor(col_Z);
	PtJetOverPtZ_1PFL1CORRJet_MC->SetFillStyle(style);
	PtJetOverPtZ_1PFL1CORRJet_MC->SetMarkerColor(col_Z);
	PtJetOverPtZ_1PFL1CORRJet_MC->GetXaxis()->SetTitle("p_{T,Jet}/p_{T,Z}");
	PtJetOverPtZ_1PFL1CORRJet_MC->Rebin(rebin_recPtZ);
	PtJetOverPtZ_1PFL1CORRJet_MC->Draw("hist");
	PtJetOverPtZ_1PFL1CORRJet_Data->SetLineWidth(2);
	PtJetOverPtZ_1PFL1CORRJet_Data->SetMarkerStyle(20);
	PtJetOverPtZ_1PFL1CORRJet_Data->Rebin(rebin_recPtZ);
	PtJetOverPtZ_1PFL1CORRJet_Data->Draw("same");
	tree_pfl1ptratio->Write("PtJetOverPtZ_1PFL1CORRJet.root");
	tree_pfl1ptratio->Close();
	
	ntuplerep<<"PtJet/PtZ - L1L2L3 - MC Mean = "<<PtJetOverPtZ_1PFL1CORRJet_MC->GetMean()<<" +/- "<<PtJetOverPtZ_1PFL1CORRJet_MC->GetMeanError()<<endl;
	ntuplerep<<"PtJet/PtZ - L1L2L3 - DATA Mean = "<<PtJetOverPtZ_1PFL1CORRJet_Data->GetMean()<<" +/- "<<PtJetOverPtZ_1PFL1CORRJet_Data->GetMeanError()<<endl<<endl;
	
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
	
	ntuplerep.close();
		
  outplots->Write();
  outplots->Close();

}
