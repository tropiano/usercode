#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>

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
#include "TLatex.h"
#include "TLegendEntry.h"
#include "TGraphAsymmErrors.h"
#include "TMath.h"

using namespace std;

void XSec(){

        gROOT->SetStyle("Plain");
        
        double Luminosity = 36.; //pb-1
        double JetPtMin = 15.; //Gev/c
        
        //Output
        string out = "XSecMeas";
        string output = out;
        output+=".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
        
        //Report files
        ofstream multrep;
	multrep.open("MultReport.txt");
	multrep<<endl;

        ofstream xsec;
	xsec.open("XSecReport.txt");
	xsec<<endl;
	
	////////////////////////// Directories
	
        TDirectory *multdir, *yieldsdir, *tpeffdir, *mceffdir, *corrfactdir, *matrixdir, *xsecdir, *unfdir;
        
        multdir = outplots->mkdir("CorrMultiplicity");
        yieldsdir = multdir->mkdir("Yields");
        tpeffdir = multdir->mkdir("TPEff");
        mceffdir = multdir->mkdir("MCEffAcc");
        corrfactdir = multdir->mkdir("CorrFactors");
        matrixdir = multdir->mkdir("ResponseMatrix");
        unfdir = multdir->mkdir("UnfoldedMult");
        xsecdir = outplots->mkdir("XSec");
        
        ////////////////////////// GET FILES - Exclusive quantities
        
        //DATA: Signal and Background Yields from TP fits
        // -> DATA_Excl_Double file in TPAnalyzer
        TFile *SB_Yields_excl = TFile::Open("/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/TagProbe/JetPt15/TPAnalyzer/TestTP_ExclDouble.root");
        
        //DATA: Tag&Probe Efficiencies
        // -> TPStudy_Excl_Global file in Efficiency_TP
        TFile *TPEff_excl = TFile::Open("/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/Efficiency_TP/JetPt15/TPStudy_JetPt15_Excl_Global.root");
        
        ////////////////////////// GET FILES - Inclusive quantities
        
        //DATA: Signal and Background Yields from TP fits
        // -> DATA_Incl_Double file in TPAnalyzer
        TFile *SB_Yields_incl = TFile::Open("/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/TagProbe/JetPt15/TPAnalyzer/DATA_JetPt15_Incl_Double.root");
        
        //DATA: Tag&Probe Efficiencies
        // -> TPStudy_Incl_Global file in Efficiency_TP
        TFile *TPEff_incl = TFile::Open("/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/Efficiency_TP/JetPt15/TPStudy_JetPt15_Incl_Global_5bin.root");
        
        ////////////////////////// GET FILES - MC Efficiency (only one file)
        
        //MC: Efficiency and Acceptance
        // -> SignalStudy_ZMadgraph file in Efficiency_MC
        TFile *MCEff = TFile::Open("/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/Efficiency_MC/SignalStudy_ZMadgraph_JetPt15_2.root");
        
        //MC: Unfolding and Correction Factors
        // -> NtuplePlots file
        TFile *UNF = TFile::Open("/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/NtuplePlots/NtuplePlots_2.root");
      
        ///////////////////////// GET HISTOGRAMS - Exclusive Quantities
        
        //Signal and Background Yields
        yieldsdir->cd();
        TGraphAsymmErrors* SignalY_excl = (TGraphAsymmErrors*) SB_Yields_excl->Get("YieldPlots/SignalYield_0");
        SignalY_excl->Write("SignalYields_excl");
        TGraphAsymmErrors* BackgroundY_excl = (TGraphAsymmErrors*) SB_Yields_excl->Get("YieldPlots/BackgroundYield_0");
        BackgroundY_excl->Write("BackgYields_excl");
        
        //TP Efficiencies 
        tpeffdir->cd();
        TGraphAsymmErrors* TPData_Eff_excl = (TGraphAsymmErrors*) TPEff_excl->Get("Tag&Probe/Tag&Probe_Global/TP_Data-Eff_Global");
        TPData_Eff_excl->Write("TPDataEff_excl");
        TGraphAsymmErrors* TPMC_Eff_excl = (TGraphAsymmErrors*) TPEff_excl->Get("Tag&Probe/Tag&Probe_Global/TP_MC-DoubleEff_Global");
        TPMC_Eff_excl->Write("TPMCEff_excl");
        
        //MC Efficiency and Acceptance
        mceffdir->cd();
        TGraphAsymmErrors* MC_Eff_excl = (TGraphAsymmErrors*) MCEff->Get("Global_Efficiency/Efficiency_vs_RecoExclJet/EffVsRecoExclJetN_AccASYM_Trg_Imp_ConvASYM_IsoASYM_EiDASYM");
        MC_Eff_excl->Write("MCGlobalEff_excl");
        TGraphAsymmErrors* MC_Acc_excl = (TGraphAsymmErrors*) MCEff->Get("Acceptance/Acc_VsRecoExclJetN");
        MC_Acc_excl->Write("MCAcc_excl");
        
        //Unfolding
        corrfactdir->cd();
        TH1D* UCF_excl = (TH1D*) UNF->Get("Unfolding/CorrFactors/Exclusive_CorrFactors");
        UCF_excl->Write("CorrFactors_excl");
        unfdir->cd();
        TH1D* GENMult_excl = (TH1D*) UNF->Get("Unfolding/CorrFactors/GENJet_ExclMult");
        GENMult_excl->Write("GENMult_excl");
        matrixdir->cd();
        TH2D* Matrix_PFL1 = (TH2D*) UNF->Get("Unfolding/ResponseMatrix/UnfoldingPlot_Box_PFL1.root");
        Matrix_PFL1->Write("RespMatrix_Gen-PFL1");
        
        ///////////////////////// GET HISTOGRAMS - Inclusive Quantities
        
        //Signal and Background Yields
        yieldsdir->cd();
        TGraphAsymmErrors* SignalY_incl = (TGraphAsymmErrors*) SB_Yields_incl->Get("YieldPlots/SignalYield_0");
        SignalY_incl->Write("SignalYields_incl");
        TGraphAsymmErrors* BackgroundY_incl = (TGraphAsymmErrors*) SB_Yields_incl->Get("YieldPlots/BackgroundYield_0");
        BackgroundY_incl->Write("BackgYields_incl");
        
        //TP Efficiencies 
        tpeffdir->cd();
        TGraphAsymmErrors* TPData_Eff_incl = (TGraphAsymmErrors*) TPEff_incl->Get("Tag&Probe/Tag&Probe_Global/TP_Data-Eff_Global");
        TPData_Eff_incl->Write("TPDataEff_incl");
        TGraphAsymmErrors* TPMC_Eff_incl = (TGraphAsymmErrors*) TPEff_incl->Get("Tag&Probe/Tag&Probe_Global/TP_MC-DoubleEff_Global");
        TPMC_Eff_incl->Write("TPMCEff_incl");
        
        //MC Efficiency and Acceptance
        mceffdir->cd();
        TGraphAsymmErrors* MC_Eff_incl = (TGraphAsymmErrors*) MCEff->Get("Global_Efficiency/Efficiency_vs_RecoInclJet/EffVsRecoInclJetN_AccASYM_Trg_Imp_ConvASYM_IsoASYM_EiDASYM");
        MC_Eff_incl->Write("MCGlobalEff_incl");
        
        TGraphAsymmErrors* MC_Acc_incl = (TGraphAsymmErrors*) MCEff->Get("Acceptance/Acc_VsRecoInclJetN");
        MC_Acc_incl->Write("MCAcc_incl");
        
        //Unfolding Correction Factors
        corrfactdir->cd();
        TH1D* UCF_incl = (TH1D*) UNF->Get("Unfolding/CorrFactors/Inclusive_CorrFactors");
        UCF_incl->Write("CorrFactors_incl");
        unfdir->cd();
        TH1D* GENMult_incl = (TH1D*) UNF->Get("Unfolding/CorrFactors/GENJet_InclMult");
        GENMult_incl->Write("GENMult_incl");
        
        ////////////////////////// Corrected Jet Multiplicity
        
        unfdir->cd();
        
        TGraphAsymmErrors* CorrMult_graph = new TGraphAsymmErrors();
        TH1D* CorrMult_histo = new TH1D("CorrMult_histo","DATA - Corrected Multiplicity",5,-0.5,4.5);
        TGraphAsymmErrors* GENMult_graph = new TGraphAsymmErrors();
        TH1D* GENMult_histo = new TH1D("GENMult_histo","GEN Multiplicity",5,-0.5,4.5);
        
        double BinCorrMult;
        
        double BinUCF_excl;
        double xs_excl, ys_excl, xb_excl, yb_excl;
        double xtpdata_excl, ytpdata_excl, xtpmc_excl, ytpmc_excl;
        double xmceff_excl, ymceff_excl;
        
        double BinUCF_incl;
        double xs_incl, ys_incl, xb_incl, yb_incl;
        double xtpdata_incl, ytpdata_incl, xtpmc_incl, ytpmc_incl;
        double xmceff_incl, ymceff_incl;
        
        multrep<<" ### Unfolded Multiplicity ### "<<endl<<endl;
        multrep<<" Jet Pt >= "<<JetPtMin<<endl<<endl;
        
        for(int i=0;i<4;i++){
        
        multrep<<"---> Bin n. "<<i<<endl<<endl;
        
        BinCorrMult=0.;
        BinUCF_excl=0.;
        
        xs_excl=0.;
        ys_excl=0.;
        xb_excl=0.;
        yb_excl=0.;
        
        xtpdata_excl=0.;
        ytpdata_excl=0.;
        xtpmc_excl=0.;
        ytpmc_excl=0.;
        
        xmceff_excl=0.;
        ymceff_excl=0.;
        
        SignalY_excl->GetPoint(i, xs_excl, ys_excl);
        multrep<<"Signal Yields = "<<ys_excl<<endl;
        BackgroundY_excl->GetPoint(i, xb_excl, yb_excl);
        multrep<<"Background Yields = "<<yb_excl<<endl;
        TPData_Eff_excl->GetPoint(i, xtpdata_excl, ytpdata_excl);
        multrep<<"TP Data Efficiency = "<<ytpdata_excl<<endl;
        TPMC_Eff_excl->GetPoint(i, xtpmc_excl, ytpmc_excl);
        multrep<<"TP MC Efficiency = "<<ytpmc_excl<<endl;       
        MC_Eff_excl->GetPoint(i, xmceff_excl, ymceff_excl);
        multrep<<"MC Global Efficiency = "<<ymceff_excl<<endl;
        BinUCF_excl = UCF_excl->GetBinContent(i+1);
        multrep<<"Unf. Correction Factor = "<<BinUCF_excl<<endl<<endl;
        
        //Corrected Multiplicity calculation
        BinCorrMult = ((ys_excl-yb_excl)/(ymceff_excl*(ytpdata_excl/ytpmc_excl)))*BinUCF_excl;
        
        multrep<<"Corrected Multiplcity = "<<BinCorrMult<<endl<<endl;
        
        CorrMult_graph->SetPoint(i, i, BinCorrMult);
        CorrMult_graph->SetPointEXlow(i, 0.5);
        CorrMult_graph->SetPointEXhigh(i, 0.5);
        
        CorrMult_histo->SetBinContent(i+1, BinCorrMult);  
        
        }
        
        // Bin n.4 -> jet incl. mult. >= 4
        
        multrep<<"---> Bin n. 4 (inclusive)"<<endl<<endl;
        
        BinCorrMult=0.;
        BinUCF_incl=0.;
        
        xs_incl=0.;
        ys_incl=0.;
        xb_incl=0.;
        yb_incl=0.;
        
        xtpdata_incl=0.;
        ytpdata_incl=0.;
        xtpmc_incl=0.;
        ytpmc_incl=0.;
        
        xmceff_incl=0.;
        ymceff_incl=0.;
        
        SignalY_incl->GetPoint(4, xs_incl, ys_incl);
        multrep<<"Signal Yields = "<<ys_incl<<endl;
        BackgroundY_incl->GetPoint(4, xb_incl, yb_incl);
        multrep<<"Background Yields = "<<yb_incl<<endl;
        TPData_Eff_incl->GetPoint(4, xtpdata_incl, ytpdata_incl);
        multrep<<"TP Data Efficiency = "<<ytpdata_incl<<endl;
        TPMC_Eff_incl->GetPoint(4, xtpmc_incl, ytpmc_incl);
        multrep<<"TP MC Efficiency = "<<ytpmc_incl<<endl;       
        MC_Eff_incl->GetPoint(4, xmceff_incl, ymceff_incl);
        multrep<<"MC Global Efficiency = "<<ymceff_incl<<endl;
        BinUCF_incl = UCF_incl->GetBinContent(5);
        multrep<<"Unf. Correction Factor = "<<BinUCF_incl<<endl<<endl;
        
        //Corrected Multiplicity calculation
        BinCorrMult = ((ys_incl-yb_incl)/(ymceff_incl*(ytpdata_incl/ytpmc_incl)))*BinUCF_incl;
        
        multrep<<"Corrected Multiplcity (Inclusive) = "<<BinCorrMult<<endl<<endl;
        
        CorrMult_graph->SetPoint(4, 4, BinCorrMult);
        CorrMult_graph->SetPointEXlow(4, 0.5);
        CorrMult_graph->SetPointEXhigh(4, 0.5);
        
        CorrMult_histo->SetBinContent(5, BinCorrMult);
        
        ////////////////////////// Make Multiplicity Plots
        
	TCanvas *UnfMult = new TCanvas;
	CorrMult_graph->SetTitle("DATA - Corrected Multiplicity");
	CorrMult_graph->GetXaxis()->SetTitle("multiplicity");
	CorrMult_graph->GetXaxis()->SetNdivisions(10);
        CorrMult_graph->SetLineWidth(2);
        CorrMult_graph->SetMarkerStyle(20);
        CorrMult_graph->Draw("AP");
        TPaveText* bin4 = new TPaveText(0.715517, 0.0317797, 0.757184, 0.0911017, "BRNDC"); 
        bin4->AddText("#geq");
        bin4->SetBorderSize(0);
        bin4->SetShadowColor(0);
        bin4->SetFillColor(0);
        bin4->SetTextAlign(31);
        bin4->Draw();
        UnfMult->Write("CorrMult_graph.root");
        UnfMult->Close();
        
        //GEN Mult plot
        for(int i=1;i<5;i++){
        //histo
        GENMult_histo->SetBinContent(i, GENMult_excl->GetBinContent(i));
        GENMult_histo->SetBinError(i, GENMult_excl->GetBinError(i));
        //graph
        GENMult_graph->SetPoint(i-1, i-1, GENMult_excl->GetBinContent(i));
        GENMult_graph->SetPointEXlow(i-1, 0.5);
        GENMult_graph->SetPointEXhigh(i-1, 0.5);
        GENMult_graph->SetPointEYlow(i-1, GENMult_excl->GetBinError(i));
        GENMult_graph->SetPointEYhigh(i-1, GENMult_excl->GetBinError(i));
        }
        //histo 
        GENMult_histo->SetBinContent(5, GENMult_incl->GetBinContent(5));
        GENMult_histo->SetBinError(5, GENMult_incl->GetBinError(5));
        //graph
        GENMult_graph->SetPoint(4, 4, GENMult_incl->GetBinContent(5));
        GENMult_graph->SetPointEXlow(4, 0.5);
        GENMult_graph->SetPointEXhigh(4, 0.5);
        GENMult_graph->SetPointEYlow(4, GENMult_incl->GetBinError(5));
        GENMult_graph->SetPointEYhigh(4, GENMult_incl->GetBinError(5));
        
        TCanvas *genMult = new TCanvas;
	GENMult_graph->SetTitle("GEN Multiplicity");
	GENMult_graph->GetXaxis()->SetTitle("multiplicity");
	GENMult_graph->GetXaxis()->SetNdivisions(10);
        GENMult_graph->SetLineWidth(2);
        GENMult_graph->SetMarkerStyle(20);
        GENMult_graph->Draw("AP");
        TPaveText* binG = new TPaveText(0.715517, 0.0317797, 0.757184, 0.0911017, "BRNDC"); 
        binG->AddText("#geq");
        binG->SetBorderSize(0);
        binG->SetShadowColor(0);
        binG->SetFillColor(0);
        binG->SetTextAlign(31);
        binG->Draw();
        genMult->Write("GENMult_graph.root");
        genMult->Close();
                     
        TCanvas *UnfMultComp = new TCanvas; 
        UnfMultComp->SetLogy();    
        GENMult_graph->SetLineColor(2);
        GENMult_graph->SetLineWidth(2);
        GENMult_graph->SetMarkerStyle(20);
        GENMult_graph->SetMarkerColor(2);
        GENMult_graph->Draw("AP");  
	GENMult_graph->SetTitle("GEN Mult. (red) - DATA Unf.Mult. (black)");
        CorrMult_graph->Draw("PSAME");                                   
        TPaveText* binU = new TPaveText(0.715517, 0.0317797, 0.757184, 0.0911017, "BRNDC"); 
        binU->AddText("#geq");
        binU->SetBorderSize(0);
        binU->SetShadowColor(0);
        binU->SetFillColor(0);
        binU->SetTextAlign(31);
        binU->Draw();
        UnfMultComp->Write("UnfMultComp.root");
        UnfMultComp->Close();
        
        CorrMult_histo->Sumw2();
        GENMult_histo->Sumw2();
        
        TCanvas *multRatio = new TCanvas;
        CorrMult_histo->Divide(GENMult_histo);
        CorrMult_histo->SetLineColor(1);
        CorrMult_histo->SetLineWidth(2);
        CorrMult_histo->SetMarkerStyle(20);
        CorrMult_histo->GetXaxis()->SetNdivisions(5);
        CorrMult_histo->GetXaxis()->SetTitle("multiplicity");
        CorrMult_histo->GetXaxis()->SetTitleOffset(1.1);
        CorrMult_histo->SetTitle("DATA Unf.Mult./GEN Mult.");
        CorrMult_histo->Draw();
        TPaveText* binUR = new TPaveText(0.764368, 0.0444915, 0.807471, 0.0868644, "BRNDC"); 
        binUR->AddText("#geq");
        binUR->SetBorderSize(0);
        binUR->SetShadowColor(0);
        binUR->SetFillColor(0);
        binUR->SetTextAlign(31);
        binUR->Draw();
        multRatio->Write("UnfMultRatio.root");
        multRatio->Close();
              
        
        ////////////////////////// Cross Section
        
        xsecdir->cd();
        
        double TotEvents = 0.;
        double InclXSec = 0.;
        
        double xmult, ymult;
        double xsections;
        
        TGraphAsymmErrors* XSec = new TGraphAsymmErrors();
        
        xsec<<" ### Cross Section Measurement ### "<<endl<<endl;
        xsec<<"Jet Pt >= "<<JetPtMin<<endl<<endl;
        
	for(int i=0;i<5;i++){
	
	xmult=0.;
	ymult=0.;
	xsections=0.;
	
	CorrMult_graph->GetPoint(i, xmult, ymult);
	
	TotEvents+=ymult;
	
	xsections = ymult/Luminosity;
	
	if(i<4)xsec<<"sigma(Z + "<<i<<" jet)		---> "<<xsections<<"	pb-1"<<endl;
	if(i==4)xsec<<"sigma(Z + >= 4 jets)		---> "<<xsections<<"	pb-1"<<endl<<endl;
	
	XSec->SetPoint(i, i, xsections);
	
	}
	
	InclXSec = TotEvents/Luminosity;
	
	xsec<<"sigma(Z + >= 0 jets) in Acceptance = "<<InclXSec<<"	pb-1"<<endl<<endl;
	
	TGraphAsymmErrors* XSecRatio = new TGraphAsymmErrors();
	
	double xCrossSec, yCrossSec;
	
	for(int i=0;i<5;i++){
	
	xCrossSec=0.;
	yCrossSec=0.;
	
	XSec->GetPoint(i, xCrossSec, yCrossSec);
	
	if(i<4)xsec<<"sigma(Z + "<<i<<" jet)/sigma(Z + >= 0 jets)		---> "<<yCrossSec/InclXSec<<endl;
	if(i==4)xsec<<"sigma(Z + >= 4 jets)/sigma(Z + >= 0 jets)	---> "<<yCrossSec/InclXSec<<endl<<endl;
	
	XSecRatio->SetPoint(i, xCrossSec, yCrossSec/InclXSec);
	
	}
	
	////////////////////////// Inclusive Cross Section Ratios
	
	TGraphAsymmErrors* InclXSecRatio = new TGraphAsymmErrors();
	
	double xn, yn;
	double xsecSum;
	
	for(int i=0;i<5;i++){
	
	xsecSum=0.;
	
	for(int j=i;j<5;j++){
	
	xn=0.;
	yn=0.;
	
	XSecRatio->GetPoint(j, xn, yn);
	xsecSum+=yn;}
	
	xsec<<"sigma(Z + >= "<<i<<" jet)/sigma(Z + >= 0 jets)	---> "<<xsecSum<<endl;
	InclXSecRatio->SetPoint(i, i, xsecSum);

	}
		
	xsec<<endl;
	
	TGraphAsymmErrors* InclXSecRatioRecur = new TGraphAsymmErrors();
	
	double xn_1, yn_1;
	
	for(int i=1;i<5;i++){
	
	xn=0.;
	yn=0.;
	xn_1=0.;
	yn_1=0.;
	
	InclXSecRatio->GetPoint(i, xn, yn);
	InclXSecRatio->GetPoint(i-1, xn_1, yn_1);
	
	xsec<<"sigma(Z + >= "<<i<<" jet)/sigma(Z + >= "<<i-1<<" jets)	---> "<<yn/yn_1<<endl;
	
	InclXSecRatioRecur->SetPoint(i-1, i, yn/yn_1);
	
	}
	
	double xAcc=0.;
	double yAcc=0.;
	
	MC_Acc_incl->GetPoint(0, xAcc, yAcc);
	
	xsec<<endl<<"sigma(Z + >= 0 jets) in ALL PHASE SAPCE = "<<InclXSec/yAcc<<"	pb-1"<<endl<<endl;
		
	////////////////////////// Make Cross Section Plots
        
	TCanvas *XSecCanvas = new TCanvas;
	XSecCanvas->SetLogy();
	XSec->SetTitle("sigma(Z + n jets)");
	XSec->GetXaxis()->SetTitle("multiplicity");
	XSec->GetXaxis()->SetLimits(-0.5,4.5);
	XSec->GetXaxis()->SetNdivisions(5);
	XSec->GetXaxis()->SetTitleOffset(1.1);	
        XSec->SetLineWidth(2);
        XSec->SetMarkerStyle(20);
        XSec->Draw("AP");
        TPaveText* bin4_xsec = new TPaveText(0.765805, 0.0466102, 0.807471, 0.0889831, "BRNDC"); 
        bin4_xsec->AddText("#geq");
        bin4_xsec->SetBorderSize(0);
        bin4_xsec->SetShadowColor(0);
        bin4_xsec->SetFillColor(0);
        bin4_xsec->SetTextAlign(31);
        bin4_xsec->Draw();
        XSecCanvas->Write("XSec.root");
        XSecCanvas->Close();
        
        TCanvas *XSecRatioCanvas = new TCanvas;
        XSecRatioCanvas->SetLogy();
	XSecRatio->SetTitle("sigma(Z + n jets)/sigma(Z + >= 0 jet)");
	XSecRatio->GetXaxis()->SetTitle("multiplicity");
	XSecRatio->GetXaxis()->SetLimits(-0.5,4.5);
	XSecRatio->GetXaxis()->SetNdivisions(5);
	XSecRatio->GetXaxis()->SetTitleOffset(1.1);
        XSecRatio->SetLineWidth(2);
        XSecRatio->SetMarkerStyle(20);
        XSecRatio->Draw("AP");
        TPaveText* bin4_xsecr = new TPaveText(0.765805, 0.0466102, 0.807471, 0.0889831, "BRNDC");  
        bin4_xsecr->AddText("#geq");
        bin4_xsecr->SetBorderSize(0);
        bin4_xsecr->SetShadowColor(0);
        bin4_xsecr->SetFillColor(0);
        bin4_xsecr->SetTextAlign(31);
        bin4_xsecr->Draw();
        XSecRatioCanvas->Write("XSecRatio.root");
        XSecRatioCanvas->Close();
        
        TCanvas *InclXSecRatioCanvas = new TCanvas;
        InclXSecRatioCanvas->SetLogy();
	InclXSecRatio->SetTitle("sigma(Z + >= n jets)/sigma(Z + >= 0 jet)");
	InclXSecRatio->GetXaxis()->SetTitle("incl. multiplicity");
	InclXSecRatio->GetXaxis()->SetLimits(-0.5,4.5);
	InclXSecRatio->GetXaxis()->SetNdivisions(5);
	InclXSecRatio->GetXaxis()->SetTitleOffset(1.1);
        InclXSecRatio->SetLineWidth(2);
        InclXSecRatio->SetMarkerStyle(20);
        InclXSecRatio->Draw("AP");
        InclXSecRatioCanvas->Write("InclXSecRatio.root");
        InclXSecRatioCanvas->Close();
        
        TCanvas *InclXSecRatioRCanvas = new TCanvas;
	InclXSecRatioRecur->SetTitle("sigma(Z + >= n jets)/sigma(Z + >= (n-1) jet)");
	InclXSecRatioRecur->GetXaxis()->SetTitle("incl. multiplicity");
	InclXSecRatioRecur->GetXaxis()->SetLimits(0.5,4.5);
	InclXSecRatioRecur->GetYaxis()->SetRangeUser(0.2,0.5);
	InclXSecRatioRecur->GetXaxis()->SetNdivisions(5);
	InclXSecRatioRecur->GetXaxis()->SetTitleOffset(1.1);
        InclXSecRatioRecur->SetLineWidth(2);
        InclXSecRatioRecur->SetMarkerStyle(20);
        InclXSecRatioRecur->Draw("AP");
        InclXSecRatioRCanvas->Write("InclXSecRatioRecur.root");
        InclXSecRatioRCanvas->Close();
        
        
        
        
	
	
	        
        xsec.close();
        multrep.close();
        
        outplots->Write();
        outplots->Close();
  
}
