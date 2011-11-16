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

//JetPtMin in GeV; Tune Z2 or D6T; jecUnc: "0" = NotApplied, "+" = Added, "-" = Subtracted 

void XSec(int JetPtMin, string tune, string jecUnc){
        
        double Luminosity = 36.; //pb-1
        double LuminosityErr = 1.4; //pb-1
        
        //Normalization factor
	double iniLumi = 50.; //pb-1
	double scale = 1.;
	if(iniLumi!=0)scale = Luminosity/iniLumi;
	
	double scaleGENRECO;
	if(tune=="Z2")scaleGENRECO = 0.044; // Z2 
	if(tune=="D6T")scaleGENRECO = 0.043; // D6T
	
	string JetPtCut;
	if(JetPtMin==15)JetPtCut="JetPt15";
	if(JetPtMin==30)JetPtCut="JetPt30";
	
	int bin_excl = 4;
	int bin_incl = 5;
	
	//JEC Uncertainty
	string JEC="";
	if(jecUnc=="0")JEC="";
	if(jecUnc=="+")JEC="jecUncPlus_";
	if(jecUnc=="-")JEC="jecUncMinus_";
        
        //Output
        string out = "XSecMeas_";
        out+=JEC;
        out+=JetPtCut;
        string output = out;
        output+=".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
        
        //Report files
        ofstream multrep;
	multrep.open(("MultReport_"+JEC+JetPtCut+".txt").c_str());
	multrep<<endl;

        ofstream xsec;
	xsec.open(("XSecReport_"+JEC+JetPtCut+".txt").c_str());
	xsec<<endl;
	
	////////////////////////// Directories /////////////////////////////////////////////
	
        TDirectory *multdir, *yieldsdir, *tpeffdir, *mceffdir, *corrfactdir, *matrixdir, *xsecdir, *unfdir, *effcorrdir;
        
        multdir = outplots->mkdir("CorrMultiplicity");
        yieldsdir = multdir->mkdir("Yields");
        tpeffdir = multdir->mkdir("TPEff");
        mceffdir = multdir->mkdir("MCEffAcc");
        corrfactdir = multdir->mkdir("CorrFactors");
        matrixdir = multdir->mkdir("ResponseMatrix");
        unfdir = multdir->mkdir("UnfoldedMult");
        effcorrdir = multdir->mkdir("EffCorrMult");
        xsecdir = outplots->mkdir("XSec");
        
        ////////////////////////// GET FILES - Exclusive quantities ////////////////////////
        
        string path = "/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin";
        
        //DATA: Signal and Background Yields from TP fits
        // -> DATA_Excl_Double file in TPAnalyzer        
        TFile *SB_Yields_excl = TFile::Open((path+"/TagProbe/New_Analyzer/"+JetPtCut+"/Z2/DATA_"+JetPtCut+"_Excl_Double.root").c_str());
        
        //DATA: Tag&Probe Efficiencies
        // -> TPStudy_Excl_Global file in Efficiency_TP
        TFile *TPEff_excl = TFile::Open((path+"/Efficiency_TP/New_Analyzer/"+JetPtCut+"/Z2/TPStudy_"+JetPtCut+"_Excl_Global.root").c_str());
        
        ////////////////////////// GET FILES - Inclusive quantities
        
        //DATA: Signal and Background Yields from TP fits
        // -> DATA_Incl_Double file in TPAnalyzer
        TFile *SB_Yields_incl = TFile::Open((path+"/TagProbe/New_Analyzer/"+JetPtCut+"/Z2/DATA_"+JetPtCut+"_Incl_Double.root").c_str());
        
        //DATA: Tag&Probe Efficiencies
        // -> TPStudy_Incl_Global file in Efficiency_TP
        TFile *TPEff_incl = TFile::Open((path+"/Efficiency_TP/New_Analyzer/"+JetPtCut+"/Z2/TPStudy_"+JetPtCut+"_Incl_Global.root").c_str());
        
        ////////////////////////// GET FILES - MC 
        
        //MC: Efficiency and Acceptance
        // -> SignalStudy_ZMadgraph file in Efficiency_MC
        TFile *MCEff = TFile::Open((path+"/Efficiency_MC/"+JetPtCut+"/Z2/SignalStudy_ZMadgraph_Z2_"+JetPtCut+".root").c_str());
        
        //MC: Unfolding and Correction Factors
        // -> NtuplePlots file
        TFile *UNF = TFile::Open((path+"/Ntuple/"+JetPtCut+"/"+tune+"/NtuplePlots_"+JEC+JetPtCut+".root").c_str());
        
        //MC: WZ - ZZ background from MC
        // -> WZ-ZZ_Pythia file
        TFile *WZ_ZZ = TFile::Open((path+"/MC_Winter10/Background/"+JetPtCut+"/WZ-ZZ_Pythia_"+JetPtCut+".root").c_str());
      
        ///////////////////////// GET HISTOGRAMS - Exclusive Quantities ///////////////////////
        
        //Signal and Background Yields
        yieldsdir->cd();
        TGraphAsymmErrors* SignalY_excl = (TGraphAsymmErrors*) SB_Yields_excl->Get("YieldPlots/SignalYield_0");
        SignalY_excl->Write("SignalYields_excl");
        TGraphAsymmErrors* BackgroundY_excl = (TGraphAsymmErrors*) SB_Yields_excl->Get("YieldPlots/BackgroundYield_0");
        BackgroundY_excl->Write("BackgYields_excl");
        
        //WZ, ZZ Background from MC
        TH1D* WZ_ZZ_excl = (TH1D*) WZ_ZZ->Get("RecoElectron/recJet_Plots/IsoJetCounter_AccASYM_Trg_Imp_ConvASYM_IsoASYM_EiDASYM");
        WZ_ZZ_excl->Scale(scale);
        WZ_ZZ_excl->Write("WZ_ZZ_excl");
        
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
        GENMult_excl->Sumw2();
        GENMult_excl->Scale(scaleGENRECO);
        GENMult_excl->Write("GENMult_excl");
        effcorrdir->cd();
        TH1D* RECOMult_excl = (TH1D*) UNF->Get("Unfolding/CorrFactors/RECOJet_ExclMult");
        RECOMult_excl->Sumw2();
        RECOMult_excl->Scale(scaleGENRECO);
        RECOMult_excl->Write("RECOMult_excl");
        matrixdir->cd();
        TH2D* Matrix_PFL1 = (TH2D*) UNF->Get("Unfolding/ResponseMatrix/UnfoldingPlot_Box_PFL1.root");
        Matrix_PFL1->Write("RespMatrix_Gen-PFL1");
        TH2D* NormMatrix_PFL1 = (TH2D*) UNF->Get("Unfolding/ResponseMatrix/Norm_UnfoldingPlot_Box_PFL1.root");
        NormMatrix_PFL1->Write("NormRespMatrix_Gen-PFL1");
        
        ///////////////////////// GET HISTOGRAMS - Inclusive Quantities /////////////////////////////
        
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
        GENMult_incl->Sumw2();
        GENMult_incl->Scale(scaleGENRECO);
        GENMult_incl->Write("GENMult_incl");
        effcorrdir->cd();
        TH1D* RECOMult_incl = (TH1D*) UNF->Get("Unfolding/CorrFactors/RECOJet_InclMult");
        RECOMult_incl->Sumw2();
        RECOMult_incl->Scale(scaleGENRECO);
        RECOMult_incl->Write("RECOMult_incl");
        
        multrep<<" Jet Pt >=	"<<JetPtMin<<endl<<endl;
        multrep<<" Tune =	"<<tune.c_str()<<endl<<endl;
        multrep<<" Jec Unc =	"<<jecUnc.c_str()<<endl<<endl; 
        
        ////////////////////////// Corrected Jet Multiplicity - EXCLUSIVE /////////////////////////////
        
        unfdir->cd();       
        TGraphAsymmErrors* EXCLCorrMult_graph = new TGraphAsymmErrors();
        
        effcorrdir->cd();
        TGraphAsymmErrors* EXCLEffCorrMult_graph = new TGraphAsymmErrors();
        
        double BinCorrMult_excl; 
        double EffCorrMult_excl;       
        double BinUCF_excl, BinWZ_ZZ_excl;
        
        double xs_excl, ys_excl, xb_excl, yb_excl;
        double xtpdata_excl, ytpdata_excl; 
        double xtpmc_excl, ytpmc_excl;
        double xmceff_excl, ymceff_excl;
        
        double BinCorrMult_excl_errh;
        double BinCorrMult_excl_errl;       
        double BinWZ_ZZ_excl_err;
        
        double ys_excl_err, yb_excl_err;
        double ytpdata_excl_errh, ytpdata_excl_errl; 
        double ytpmc_excl_errh, ytpmc_excl_errl;
        double ymceff_excl_errh, ymceff_excl_errl;
        
        multrep<<endl<<" ### Unfolded Exclusive Multiplicity ### "<<endl<<endl;       
        
        for(int i=0;i<bin_excl;i++){
        
        multrep<<"-----------> Bin n. "<<i<<", Jet# == "<<i<<" <-----------"<<endl<<endl;
        
        BinCorrMult_excl=0.;
        EffCorrMult_excl=0.;
        BinUCF_excl=0.;
        BinWZ_ZZ_excl=0.;
        
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
        
        BinCorrMult_excl_errh=0.; BinCorrMult_excl_errl=0.;  
        BinWZ_ZZ_excl_err =0.;     
        ys_excl_err=0.; 
        yb_excl_err=0.;
        ytpdata_excl_errh=0.; ytpdata_excl_errl=0.;
        ytpmc_excl_errh=0.; ytpmc_excl_errl=0.;
        ymceff_excl_errh=0.; ymceff_excl_errl=0.;
        
        SignalY_excl->GetPoint(i, xs_excl, ys_excl);
        ys_excl_err = SignalY_excl->GetErrorY(i);
        multrep<<"Signal Yields = "<<ys_excl<<" +/- "<<ys_excl_err<<endl;
        
        BackgroundY_excl->GetPoint(i, xb_excl, yb_excl);
        yb_excl_err = BackgroundY_excl->GetErrorY(i);
        multrep<<"Background Yields = "<<yb_excl<<" +/- "<<yb_excl_err<<endl;
        
        BinWZ_ZZ_excl = WZ_ZZ_excl->GetBinContent(i+1);
        BinWZ_ZZ_excl_err = WZ_ZZ_excl->GetBinError(i+1);
        multrep<<"WZ-ZZ Bkg Yields from MC = "<<BinWZ_ZZ_excl<<" +/- "<<BinWZ_ZZ_excl_err<<endl;
        
        TPData_Eff_excl->GetPoint(i, xtpdata_excl, ytpdata_excl);
        ytpdata_excl_errh = TPData_Eff_excl->GetErrorYhigh(i);
        ytpdata_excl_errl = TPData_Eff_excl->GetErrorYlow(i);
        multrep<<"TP Data Efficiency = "<<ytpdata_excl<<" (+) "<<ytpdata_excl_errh<< " (-) "<<ytpdata_excl_errl<<endl;
        
        TPMC_Eff_excl->GetPoint(i, xtpmc_excl, ytpmc_excl);
        ytpmc_excl_errh = TPMC_Eff_excl->GetErrorYhigh(i);
        ytpmc_excl_errl = TPMC_Eff_excl->GetErrorYlow(i);
        multrep<<"TP MC Efficiency = "<<ytpmc_excl<<" (+) "<<ytpmc_excl_errh<< " (-) "<<ytpmc_excl_errl<<endl;
               
        MC_Eff_excl->GetPoint(i, xmceff_excl, ymceff_excl);
        ymceff_excl_errh = MC_Eff_excl->GetErrorYhigh(i);
        ymceff_excl_errl = MC_Eff_excl->GetErrorYlow(i);
        multrep<<"MC Global Efficiency = "<<ymceff_excl<<" (+) "<<ymceff_excl_errh<< " (-) "<<ymceff_excl_errl<<endl;
        
        multrep<<"(MCGlobalEff)*Rho = "<<ymceff_excl*(ytpdata_excl/ytpmc_excl)<<endl;
        
        BinUCF_excl = UCF_excl->GetBinContent(i+1);
        multrep<<"Unf. Correction Factor = "<<BinUCF_excl<<endl<<endl;
        
        //Corrected Multiplicity calculation
        EffCorrMult_excl = ((ys_excl-BinWZ_ZZ_excl)/(ymceff_excl*(ytpdata_excl/ytpmc_excl)));  
        BinCorrMult_excl = EffCorrMult_excl*BinUCF_excl;
        
        //Statistical relative uncertainty on Nsig 
        BinCorrMult_excl_errh = BinCorrMult_excl*(ys_excl_err/(ys_excl-BinWZ_ZZ_excl));        
        BinCorrMult_excl_errl = BinCorrMult_excl*(ys_excl_err/(ys_excl-BinWZ_ZZ_excl));
        
        multrep<<"Corrected Excl. Multiplcity = "<<BinCorrMult_excl<<" (+) "<<BinCorrMult_excl_errh<<" (-) "<<BinCorrMult_excl_errl<<endl<<endl;
        
        multrep<<"STATISTICAL RELATIVE unc. (Nsig) = 	"<<ys_excl_err/(ys_excl-BinWZ_ZZ_excl)<<endl<<endl;
        
        multrep<<"SYSTEMATIC RELATIVE uncertainties:"<<endl;
        multrep<<"WZ-ZZ background =			"<<BinWZ_ZZ_excl_err/(ys_excl-BinWZ_ZZ_excl)<<endl;
        multrep<<"TP Eff. Data =			"<<" (High) "<<ytpdata_excl_errh/ytpdata_excl<<" (Low) "<<ytpdata_excl_errl/ytpdata_excl<<endl;
        multrep<<"TP Eff. MC =			"<<" (High) "<<ytpmc_excl_errh/ytpmc_excl<<" (Low) "<<ytpmc_excl_errl/ytpmc_excl<<endl;         
        multrep<<"MC Global Eff. =		"<<" (High) "<<ymceff_excl_errh/ymceff_excl<<" (Low) "<<ymceff_excl_errl/ymceff_excl<<endl;
        multrep<<"Luminosity =				"<<LuminosityErr/Luminosity<<endl<<endl;     
        
        EXCLCorrMult_graph->SetPoint(i, i, BinCorrMult_excl);
        EXCLCorrMult_graph->SetPointEYhigh(i, BinCorrMult_excl_errh);
        EXCLCorrMult_graph->SetPointEYlow(i, BinCorrMult_excl_errl);
        EXCLCorrMult_graph->SetPointEXhigh(i, 0.5);
        EXCLCorrMult_graph->SetPointEXlow(i, 0.5);
        
        EXCLEffCorrMult_graph->SetPoint(i, i, EffCorrMult_excl);
        EXCLEffCorrMult_graph->SetPointEYhigh(i, BinCorrMult_excl_errh);
        EXCLEffCorrMult_graph->SetPointEYlow(i, BinCorrMult_excl_errl);
        EXCLEffCorrMult_graph->SetPointEXhigh(i, 0.5);
        EXCLEffCorrMult_graph->SetPointEXlow(i, 0.5);
        }
        
        unfdir->cd();
        EXCLCorrMult_graph->Write("EXCLCorrMult_graph");
        effcorrdir->cd();
        EXCLEffCorrMult_graph->Write("EXCLEffCorrMult_graph");
        
        ////////////////////////// Corrected Jet Multiplicity - INCLUSIVE /////////////////////////////
        
        unfdir->cd();
        TGraphAsymmErrors* INCLCorrMult_graph = new TGraphAsymmErrors();
        effcorrdir->cd();
        TGraphAsymmErrors* INCLEffCorrMult_graph = new TGraphAsymmErrors();
        
        double BinCorrMult_incl;
        double EffCorrMult_incl;       
        double BinUCF_incl, BinWZ_ZZ_incl;
        
        double xs_incl, ys_incl, xb_incl, yb_incl;
        double xtpdata_incl, ytpdata_incl; 
        double xtpmc_incl, ytpmc_incl;
        double xmceff_incl, ymceff_incl;
        
        double BinCorrMult_incl_errh;
        double BinCorrMult_incl_errl;   
        double BinWZ_ZZ_incl_err;   

        double ys_incl_err, yb_incl_err;
        double ytpdata_incl_errh, ytpdata_incl_errl; 
        double ytpmc_incl_errh, ytpmc_incl_errl;
        double ymceff_incl_errh, ymceff_incl_errl;
        
        multrep<<endl<<" ### Unfolded Inclusive Multiplicity ### "<<endl<<endl;       
        
        for(int i=0;i<bin_incl;i++){
        
        multrep<<"-----------> Bin n. "<<i<<", Jet# >= "<<i<<" <-----------"<<endl<<endl;
        
        BinCorrMult_incl=0.;
        EffCorrMult_incl=0.;
        BinUCF_incl=0.;
        BinWZ_ZZ_incl=0.;
        
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
        
        BinCorrMult_incl_errh=0.; BinCorrMult_incl_errl=0.;       
        BinWZ_ZZ_incl_err=0.;
        ys_incl_err=0.; 
        yb_incl_err=0.;
        ytpdata_incl_errh=0.; ytpdata_incl_errl=0.;
        ytpmc_incl_errh=0.; ytpmc_incl_errl=0.;
        ymceff_incl_errh=0.; ymceff_incl_errl=0.;
        
        SignalY_incl->GetPoint(i, xs_incl, ys_incl);
        ys_incl_err = SignalY_incl->GetErrorY(i);
        multrep<<"Signal Yields = "<<ys_incl<<" +/- "<<ys_incl_err<<endl;
        
        BackgroundY_incl->GetPoint(i, xb_incl, yb_incl);
        yb_incl_err = BackgroundY_incl->GetErrorY(i);
        multrep<<"Background Yields = "<<yb_incl<<" +/- "<<yb_incl_err<<endl;
        
        BinWZ_ZZ_incl = WZ_ZZ_excl->IntegralAndError(i+1, 10, BinWZ_ZZ_incl_err);
        multrep<<"WZ-ZZ Bkg Yields from MC = "<<BinWZ_ZZ_incl<<" +/- "<<BinWZ_ZZ_incl_err<<endl;
        
        TPData_Eff_incl->GetPoint(i, xtpdata_incl, ytpdata_incl);
        ytpdata_incl_errh = TPData_Eff_incl->GetErrorYhigh(i);
        ytpdata_incl_errl = TPData_Eff_incl->GetErrorYlow(i);
        multrep<<"TP Data Efficiency = "<<ytpdata_incl<<" (+) "<<ytpdata_incl_errh<< " (-) "<<ytpdata_incl_errl<<endl;
        
        TPMC_Eff_incl->GetPoint(i, xtpmc_incl, ytpmc_incl);
        ytpmc_incl_errh = TPMC_Eff_incl->GetErrorYhigh(i);
        ytpmc_incl_errl = TPMC_Eff_incl->GetErrorYlow(i);
        multrep<<"TP MC Efficiency = "<<ytpmc_incl<<" (+) "<<ytpmc_incl_errh<< " (-) "<<ytpmc_incl_errl<<endl;
               
        MC_Eff_incl->GetPoint(i, xmceff_incl, ymceff_incl);
        ymceff_incl_errh = MC_Eff_incl->GetErrorYhigh(i);
        ymceff_incl_errl = MC_Eff_incl->GetErrorYlow(i);
        multrep<<"MC Global Efficiency = "<<ymceff_incl<<" (+) "<<ymceff_incl_errh<< " (-) "<<ymceff_incl_errl<<endl;
        
        multrep<<"(MCGlobalEff)*Rho = "<<ymceff_incl*(ytpdata_incl/ytpmc_incl)<<endl;
        
        BinUCF_incl = UCF_incl->GetBinContent(i+1);
        multrep<<"Unf. Correction Factor = "<<BinUCF_incl<<endl<<endl;
        
        //Corrected Multiplicity calculation
        EffCorrMult_incl = ((ys_incl-BinWZ_ZZ_incl)/(ymceff_incl*(ytpdata_incl/ytpmc_incl)));
        BinCorrMult_incl = EffCorrMult_incl*BinUCF_incl;
        
        //Statistical relative uncertainty on Nsig 
        BinCorrMult_incl_errh = BinCorrMult_incl*(ys_incl_err/(ys_incl-BinWZ_ZZ_incl));        
        BinCorrMult_incl_errl = BinCorrMult_incl*(ys_incl_err/(ys_incl-BinWZ_ZZ_incl));
        
        multrep<<"Corrected Incl. Multiplcity = "<<BinCorrMult_incl<<" (+) "<<BinCorrMult_incl_errh<<" (-) "<<BinCorrMult_incl_errl<<endl<<endl;
        
        multrep<<"STATISTICAL RELATIVE unc. (Nsig) = 	"<<ys_excl_err/(ys_excl-BinWZ_ZZ_excl)<<endl<<endl;
        
        multrep<<"SYSTEMATIC RELATIVE uncertainties:"<<endl;
        multrep<<"WZ-ZZ background =			"<<BinWZ_ZZ_incl_err/(ys_incl-BinWZ_ZZ_incl)<<endl;
        multrep<<"TP Eff. Data =			"<<" (High) "<<ytpdata_incl_errh/ytpdata_incl<<" (Low) "<<ytpdata_incl_errl/ytpdata_incl<<endl;
        multrep<<"TP Eff. MC =			"<<" (High) "<<ytpmc_incl_errh/ytpmc_incl<<" (Low) "<<ytpmc_incl_errl/ytpmc_incl<<endl;
        multrep<<"MC Global Eff. =		"<<" (High) "<<ymceff_incl_errh/ymceff_incl<<" (Low) "<<ymceff_incl_errl/ymceff_incl<<endl;
        multrep<<"Luminosity =				"<<LuminosityErr/Luminosity<<endl<<endl;
                
        INCLCorrMult_graph->SetPoint(i, i, BinCorrMult_incl);
        INCLCorrMult_graph->SetPointEYhigh(i, BinCorrMult_incl_errh);
        INCLCorrMult_graph->SetPointEYlow(i, BinCorrMult_incl_errl);
        INCLCorrMult_graph->SetPointEXhigh(i, 0.5);
        INCLCorrMult_graph->SetPointEXlow(i, 0.5);
        
        INCLEffCorrMult_graph->SetPoint(i, i, EffCorrMult_incl);
        INCLEffCorrMult_graph->SetPointEYhigh(i, BinCorrMult_incl_errh);
        INCLEffCorrMult_graph->SetPointEYlow(i, BinCorrMult_incl_errl);
        INCLEffCorrMult_graph->SetPointEXhigh(i, 0.5);
        INCLEffCorrMult_graph->SetPointEXlow(i, 0.5);
        }
        
        unfdir->cd();
        INCLCorrMult_graph->Write("INCLCorrMult_graph");
        effcorrdir->cd();
        INCLEffCorrMult_graph->Write("INCLEffCorrMult_graph");
        
        ///////////////// Make UNFOLDED Exclusive Multiplicity final Graphs and Histos ///////////////////
        
        unfdir->cd();
        
        TGraphAsymmErrors* EXCLGENMult_graph = new TGraphAsymmErrors();
        
        double ygmult, yerr;
        
        for(int i=0;i<bin_excl;i++){
        ygmult=0.;
        yerr=0.;
        ygmult = GENMult_excl->GetBinContent(i+1);
        yerr = GENMult_excl->GetBinError(i+1);
        EXCLGENMult_graph->SetPoint(i, i, ygmult);
        EXCLGENMult_graph->SetPointEYhigh(i, yerr);
        EXCLGENMult_graph->SetPointEYlow(i, yerr);
        EXCLGENMult_graph->SetPointEXhigh(i, 0.5);
        EXCLGENMult_graph->SetPointEXlow(i, 0.5);}
        
        TGraphAsymmErrors* EXCLUnfMultRatio_graph = new TGraphAsymmErrors();
        
        double xunf, yunf, yunf_errh, yunf_errl;
        double xgen, ygen, ygen_err;
        double ratio_errh, ratio_errl;
        
        for(int i=0;i<bin_excl;i++){
        
        xunf=0.;
        yunf=0.;
        yunf_errh=0.;
        yunf_errl=0.;
        xgen=0.;
        ygen=0.;
        ygen_err=0.;
        ratio_errh=0.;
        ratio_errl=0.;
        
        EXCLCorrMult_graph->GetPoint(i, xunf, yunf);
        yunf_errh = EXCLCorrMult_graph->GetErrorYhigh(i);
        yunf_errl = EXCLCorrMult_graph->GetErrorYlow(i);
        EXCLGENMult_graph->GetPoint(i, xgen, ygen);
        ygen_err = EXCLGENMult_graph->GetErrorY(i);
       
        EXCLUnfMultRatio_graph->SetPoint(i, i, yunf/ygen);
        ratio_errh = (yunf/ygen)*sqrt(pow((yunf_errh/yunf),2)+pow((ygen_err/ygen),2));
        ratio_errl = (yunf/ygen)*sqrt(pow((yunf_errl/yunf),2)+pow((ygen_err/ygen),2));
        EXCLUnfMultRatio_graph->SetPointEYhigh(i, ratio_errh);
        EXCLUnfMultRatio_graph->SetPointEYlow(i, ratio_errl);
        EXCLUnfMultRatio_graph->SetPointEXhigh(i, 0.5);
        EXCLUnfMultRatio_graph->SetPointEXlow(i, 0.5);       
        }
        
        ///////////////////// Make UNFOLDED Exclusive Multiplicity Plots /////////////////////////////
               
        TCanvas *EXCLUnfMultComp = new TCanvas; 
        EXCLUnfMultComp->SetLogy();    
        EXCLGENMult_graph->SetLineColor(2);
        EXCLGENMult_graph->SetLineWidth(2);
        EXCLGENMult_graph->SetMarkerStyle(23);
        EXCLGENMult_graph->SetMarkerColor(2);
        EXCLGENMult_graph->GetXaxis()->SetTitle("excl. multiplicity");
        EXCLGENMult_graph->Draw("AP");  
	EXCLGENMult_graph->SetTitle("GEN Mult. (red) - DATA Unf.Mult. (black)");
	EXCLCorrMult_graph->SetLineWidth(2);
	EXCLCorrMult_graph->SetMarkerStyle(22);
        EXCLCorrMult_graph->Draw("PSAME");                                   
        EXCLUnfMultComp->Write("EXCLUnfMultComp.root");
        EXCLUnfMultComp->Close();
              
        TCanvas *EXCLmultRatio = new TCanvas;
        EXCLUnfMultRatio_graph->SetLineColor(1);
        EXCLUnfMultRatio_graph->SetLineWidth(2);
        EXCLUnfMultRatio_graph->SetMarkerStyle(22);
        EXCLUnfMultRatio_graph->GetXaxis()->SetTitle("excl. multiplicity");
        EXCLUnfMultRatio_graph->GetXaxis()->SetTitleOffset(1.1);
        EXCLUnfMultRatio_graph->SetName("EXCLUnfMultRatio_graph");
        EXCLUnfMultRatio_graph->SetTitle("DATA Unf.Mult./GEN Mult.");
        EXCLUnfMultRatio_graph->Draw("AP");
        EXCLmultRatio->Write("EXCLUnfMultRatio.root");
        EXCLmultRatio->Close();
        
        ///////////////// Make UNFOLDED Inclusive Multiplicity final Graphs and Histos ///////////////////
        
        unfdir->cd();
        
        TGraphAsymmErrors* INCLGENMult_graph = new TGraphAsymmErrors();
        
        for(int i=0;i<bin_incl;i++){
        ygmult=0.;
        yerr=0.;
        ygmult = GENMult_incl->GetBinContent(i+1);
        yerr = GENMult_incl->GetBinError(i+1);
        INCLGENMult_graph->SetPoint(i, i, ygmult);
        INCLGENMult_graph->SetPointEYhigh(i, yerr);
        INCLGENMult_graph->SetPointEYlow(i, yerr);
        INCLGENMult_graph->SetPointEXhigh(i, 0.5);
        INCLGENMult_graph->SetPointEXlow(i, 0.5);}
        
        TGraphAsymmErrors* INCLUnfMultRatio_graph = new TGraphAsymmErrors();
        
        for(int i=0;i<bin_incl;i++){
        
        xunf=0.;
        yunf=0.;
        yunf_errh=0.;
        yunf_errl=0.;
        xgen=0.;
        ygen=0.;
        ygen_err=0.;
        ratio_errh=0.;
        ratio_errl=0.;
        
        INCLCorrMult_graph->GetPoint(i, xunf, yunf);
        yunf_errh = INCLCorrMult_graph->GetErrorYhigh(i);
        yunf_errl = INCLCorrMult_graph->GetErrorYlow(i);
        INCLGENMult_graph->GetPoint(i, xgen, ygen);
        ygen_err = INCLGENMult_graph->GetErrorY(i);
       
        INCLUnfMultRatio_graph->SetPoint(i, i, yunf/ygen);
        ratio_errh = (yunf/ygen)*sqrt(pow((yunf_errh/yunf),2)+pow((ygen_err/ygen),2));
        ratio_errl = (yunf/ygen)*sqrt(pow((yunf_errl/yunf),2)+pow((ygen_err/ygen),2));
        INCLUnfMultRatio_graph->SetPointEYhigh(i, ratio_errh);
        INCLUnfMultRatio_graph->SetPointEYlow(i, ratio_errl);
        INCLUnfMultRatio_graph->SetPointEXhigh(i, 0.5);
        INCLUnfMultRatio_graph->SetPointEXlow(i, 0.5);       
        }
        
        ///////////////////// Make UNFOLDED Inclusive Multiplicity Plots /////////////////////////////
               
        TCanvas *INCLUnfMultComp = new TCanvas; 
        INCLUnfMultComp->SetLogy();    
        INCLGENMult_graph->SetLineColor(2);
        INCLGENMult_graph->SetLineWidth(2);
        INCLGENMult_graph->SetMarkerStyle(23);
        INCLGENMult_graph->SetMarkerColor(2);
        INCLGENMult_graph->GetXaxis()->SetTitle("incl. multiplicity");
        INCLGENMult_graph->Draw("AP");  
	INCLGENMult_graph->SetTitle("GEN Mult. (red) - DATA Unf.Mult. (black)");
	INCLCorrMult_graph->SetLineWidth(2);
	INCLCorrMult_graph->SetMarkerStyle(22);
        INCLCorrMult_graph->Draw("PSAME");                                   
        INCLUnfMultComp->Write("INCLUnfMultComp.root");
        INCLUnfMultComp->Close();
              
        TCanvas *INCLmultRatio = new TCanvas;
        INCLUnfMultRatio_graph->SetLineColor(1);
        INCLUnfMultRatio_graph->SetLineWidth(2);
        INCLUnfMultRatio_graph->SetMarkerStyle(22);
        INCLUnfMultRatio_graph->GetXaxis()->SetTitle("incl. multiplicity");
        INCLUnfMultRatio_graph->GetXaxis()->SetTitleOffset(1.1);
        INCLUnfMultRatio_graph->SetName("INCLUnfMultRatio_graph");
        INCLUnfMultRatio_graph->SetTitle("DATA Unf.Mult./GEN Mult.");
        INCLUnfMultRatio_graph->Draw("AP");
        INCLmultRatio->Write("INCLUnfMultRatio.root");
        INCLmultRatio->Close();
        
        ////////////////////// Make EFFCORR Excl. Multiplicity final Graphs and Histos ///////////////////
        
        effcorrdir->cd();
        
        TGraphAsymmErrors* EXCLRECOMult_graph = new TGraphAsymmErrors();
      
        for(int i=0;i<bin_excl;i++){
        ygmult=0.;
        yerr=0.;
        ygmult = RECOMult_excl->GetBinContent(i+1);
        yerr = RECOMult_excl->GetBinError(i+1);
        EXCLRECOMult_graph->SetPoint(i, i, ygmult);
        EXCLRECOMult_graph->SetPointEYhigh(i, yerr);
        EXCLRECOMult_graph->SetPointEYlow(i, yerr);
        EXCLRECOMult_graph->SetPointEXhigh(i, 0.5);
        EXCLRECOMult_graph->SetPointEXlow(i, 0.5);}
        
        TGraphAsymmErrors* EXCLEffMultRatio_graph = new TGraphAsymmErrors();
        
        for(int i=0;i<bin_excl;i++){
        
        xunf=0.;
        yunf=0.;
        yunf_errh=0.;
        yunf_errl=0.;
        xgen=0.;
        ygen=0.;
        ygen_err=0.;
        ratio_errh=0.;
        ratio_errl=0.;
        
        EXCLEffCorrMult_graph->GetPoint(i, xunf, yunf);
        yunf_errh = EXCLEffCorrMult_graph->GetErrorYhigh(i);
        yunf_errl = EXCLEffCorrMult_graph->GetErrorYlow(i);
        EXCLRECOMult_graph->GetPoint(i, xgen, ygen);
        ygen_err = EXCLRECOMult_graph->GetErrorY(i);
       
        EXCLEffMultRatio_graph->SetPoint(i, i, yunf/ygen);
        ratio_errh = (yunf/ygen)*sqrt(pow((yunf_errh/yunf),2)+pow((ygen_err/ygen),2));
        ratio_errl = (yunf/ygen)*sqrt(pow((yunf_errl/yunf),2)+pow((ygen_err/ygen),2));
        EXCLEffMultRatio_graph->SetPointEYhigh(i, ratio_errh);
        EXCLEffMultRatio_graph->SetPointEYlow(i, ratio_errl);
        EXCLEffMultRatio_graph->SetPointEXhigh(i, 0.5);
        EXCLEffMultRatio_graph->SetPointEXlow(i, 0.5);        
        }
        
        ////////////////////////// Make EFFCORR Excl. Multiplicity Plots ////////////////////////////////
                          
        TCanvas *EXCLEffMultComp = new TCanvas; 
        EXCLEffMultComp->SetLogy();    
        EXCLRECOMult_graph->SetLineColor(4);
        EXCLRECOMult_graph->SetLineWidth(2);
        EXCLRECOMult_graph->SetMarkerStyle(23);
        EXCLRECOMult_graph->SetMarkerColor(4);
        EXCLRECOMult_graph->GetXaxis()->SetTitle("excl. multiplicity");
        EXCLRECOMult_graph->Draw("AP");  
	EXCLRECOMult_graph->SetTitle("RECO Mult. (blue) - DATA Unf.Mult. (black)");
	EXCLEffCorrMult_graph->SetLineWidth(2);
	EXCLEffCorrMult_graph->SetMarkerStyle(22);
        EXCLEffCorrMult_graph->Draw("PSAME");                                   
        EXCLEffMultComp->Write("EXCLEffMultComp.root");
        EXCLEffMultComp->Close();
              
        TCanvas *EXCLEffmultRatio = new TCanvas;
        EXCLEffMultRatio_graph->SetLineColor(1);
        EXCLEffMultRatio_graph->SetLineWidth(2);
        EXCLEffMultRatio_graph->SetMarkerStyle(22);
        EXCLEffMultRatio_graph->GetXaxis()->SetTitle("excl. multiplicity");
        EXCLEffMultRatio_graph->GetXaxis()->SetTitleOffset(1.1);
        EXCLEffMultRatio_graph->SetName("ExclEffMultRatio_graph");
        EXCLEffMultRatio_graph->SetTitle("DATA Unf.Mult./RECO Mult.");
        EXCLEffMultRatio_graph->Draw("AP");
        EXCLEffmultRatio->Write("EXCLEffMultRatio.root");
        EXCLEffmultRatio->Close();
                     
        ////////////////////// Make EFFCORR Incl. Multiplicity final Graphs and Histos ///////////////////
        
        effcorrdir->cd();
        
        TGraphAsymmErrors* INCLRECOMult_graph = new TGraphAsymmErrors();
      
        for(int i=0;i<bin_incl;i++){
        ygmult=0.;
        yerr=0.;
        ygmult = RECOMult_incl->GetBinContent(i+1);
        yerr = RECOMult_incl->GetBinError(i+1);
        INCLRECOMult_graph->SetPoint(i, i, ygmult);
        INCLRECOMult_graph->SetPointEYhigh(i, yerr);
        INCLRECOMult_graph->SetPointEYlow(i, yerr);
        INCLRECOMult_graph->SetPointEXhigh(i, 0.5);
        INCLRECOMult_graph->SetPointEXlow(i, 0.5);}
        
        TGraphAsymmErrors* INCLEffMultRatio_graph = new TGraphAsymmErrors();
        
        for(int i=0;i<bin_incl;i++){
        
        xunf=0.;
        yunf=0.;
        yunf_errh=0.;
        yunf_errl=0.;
        xgen=0.;
        ygen=0.;
        ygen_err=0.;
        ratio_errh=0.;
        ratio_errl=0.;
        
        INCLEffCorrMult_graph->GetPoint(i, xunf, yunf);
        yunf_errh = INCLEffCorrMult_graph->GetErrorYhigh(i);
        yunf_errl = INCLEffCorrMult_graph->GetErrorYlow(i);
        INCLRECOMult_graph->GetPoint(i, xgen, ygen);
        ygen_err = INCLRECOMult_graph->GetErrorY(i);
       
        INCLEffMultRatio_graph->SetPoint(i, i, yunf/ygen);
        ratio_errh = (yunf/ygen)*sqrt(pow((yunf_errh/yunf),2)+pow((ygen_err/ygen),2));
        ratio_errl = (yunf/ygen)*sqrt(pow((yunf_errl/yunf),2)+pow((ygen_err/ygen),2));
        INCLEffMultRatio_graph->SetPointEYhigh(i, ratio_errh);
        INCLEffMultRatio_graph->SetPointEYlow(i, ratio_errl);
        INCLEffMultRatio_graph->SetPointEXhigh(i, 0.5);
        INCLEffMultRatio_graph->SetPointEXlow(i, 0.5);        
        }
        
        ////////////////////////// Make EFFCORR Incl. Multiplicity Plots ///////////////////////////////
                          
        TCanvas *INCLEffMultComp = new TCanvas; 
        INCLEffMultComp->SetLogy();    
        INCLRECOMult_graph->SetLineColor(4);
        INCLRECOMult_graph->SetLineWidth(2);
        INCLRECOMult_graph->SetMarkerStyle(23);
        INCLRECOMult_graph->SetMarkerColor(4);
        INCLRECOMult_graph->GetXaxis()->SetTitle("incl. multiplicity");
        INCLRECOMult_graph->Draw("AP");  
	INCLRECOMult_graph->SetTitle("RECO Mult. (blue) - DATA Unf.Mult. (black)");
	INCLEffCorrMult_graph->SetLineWidth(2);
	INCLEffCorrMult_graph->SetMarkerStyle(22);
        INCLEffCorrMult_graph->Draw("PSAME");                                   
        INCLEffMultComp->Write("INCLEffMultComp.root");
        INCLEffMultComp->Close();
              
        TCanvas *INCLEffmultRatio = new TCanvas;
        INCLEffMultRatio_graph->SetLineColor(1);
        INCLEffMultRatio_graph->SetLineWidth(2);
        INCLEffMultRatio_graph->SetMarkerStyle(22);
        INCLEffMultRatio_graph->GetXaxis()->SetTitle("incl. multiplicity");
        INCLEffMultRatio_graph->GetXaxis()->SetTitleOffset(1.1);
        INCLEffMultRatio_graph->SetName("InclEffMultRatio_graph");
        INCLEffMultRatio_graph->SetTitle("DATA Unf.Mult./RECO Mult.");
        INCLEffMultRatio_graph->Draw("AP");
        INCLEffmultRatio->Write("INCLEffMultRatio.root");
        INCLEffmultRatio->Close();
        
///////////////////////////////// Cross Sections //////////////////////////////////////////
        
        xsecdir->cd();
               
        double xmult, ymult, ymult_errh, ymult_errl;
        double xsections, xsections_errh, xsections_errl;
        
        TGraphAsymmErrors* EXCLXSec = new TGraphAsymmErrors();
        
        xsec<<" ### Cross Section Measurement ### "<<endl<<endl;
        xsec<<"Jet Pt >= "<<JetPtMin<<endl<<endl;
        
	for(int i=0;i<bin_excl;i++){
	
	xmult=0.;
	ymult=0.;
        ymult_errh=0.;
        ymult_errl=0.;
	xsections=0.;
	xsections_errh=0.;
	xsections_errl=0.;
	
	EXCLCorrMult_graph->GetPoint(i, xmult, ymult);
	ymult_errh = EXCLCorrMult_graph->GetErrorYhigh(i);
	ymult_errl = EXCLCorrMult_graph->GetErrorYlow(i);

	xsections = ymult/Luminosity;
	xsections_errh = xsections*sqrt(pow((ymult_errh/ymult),2)+pow((LuminosityErr/Luminosity),2));
	xsections_errl = xsections*sqrt(pow((ymult_errl/ymult),2)+pow((LuminosityErr/Luminosity),2));
	
	xsec<<"sigma(Z + "<<i<<" jet)		---> "<<xsections<<" (+) "<<xsections_errh<<" (-) "<<xsections_errl<<"	pb"<<endl;
	
	EXCLXSec->SetPoint(i, i, xsections);
	EXCLXSec->SetPointEYhigh(i, xsections_errh);
	EXCLXSec->SetPointEYlow(i, xsections_errl);
	}
	
	xsec<<endl;
	
	TGraphAsymmErrors* INCLXSec = new TGraphAsymmErrors();
        
	for(int i=0;i<bin_incl;i++){
	
	xmult=0.;
	ymult=0.;
        ymult_errh=0.;
        ymult_errl=0.;
	xsections=0.;
	xsections_errh=0.;
	xsections_errl=0.;
	
	INCLCorrMult_graph->GetPoint(i, xmult, ymult);
	ymult_errh = INCLCorrMult_graph->GetErrorYhigh(i);
	ymult_errl = INCLCorrMult_graph->GetErrorYlow(i);
	
	xsections = ymult/Luminosity;
	xsections_errh = xsections*sqrt(pow((ymult_errh/ymult),2)+pow((LuminosityErr/Luminosity),2));
	xsections_errl = xsections*sqrt(pow((ymult_errl/ymult),2)+pow((LuminosityErr/Luminosity),2));
	
	xsec<<"sigma(Z + >= "<<i<<" jet)		---> "<<xsections<<" (+) "<<xsections_errh<<" (-) "<<xsections_errl<<"	pb"<<endl;
	
	INCLXSec->SetPoint(i, i, xsections);
	INCLXSec->SetPointEYhigh(i, xsections_errh);
	INCLXSec->SetPointEYlow(i, xsections_errl);	
	}
	
	xsec<<endl;
	
	/////////////////////////////// Cross Section Ratios
	
	TGraphAsymmErrors* EXCLXSecRatio = new TGraphAsymmErrors();
	
	double INCL_xmult=0.;
	double INCL_ymult=0.;
	double INCL_ymult_errh=0.;
	double INCL_ymult_errl=0.;
	
	INCLCorrMult_graph->GetPoint(0, INCL_xmult, INCL_ymult);
	INCL_ymult_errh = INCLCorrMult_graph->GetErrorYhigh(0);
	INCL_ymult_errl = INCLCorrMult_graph->GetErrorYlow(0);
	
	double xsec_ratio_errh, xsec_ratio_errl;
	
	for(int i=0;i<bin_excl;i++){

	xsec_ratio_errh=0.;
	xsec_ratio_errl=0.;
	xmult=0.;
	ymult=0.;
	ymult_errh=0.;
	ymult_errl=0.;

	EXCLCorrMult_graph->GetPoint(i, xmult, ymult);
	ymult_errh = EXCLCorrMult_graph->GetErrorYhigh(i);
	ymult_errl = EXCLCorrMult_graph->GetErrorYlow(i);
	
	xsec_ratio_errh=(ymult/INCL_ymult)*sqrt(pow((ymult_errh/ymult),2)+pow((INCL_ymult_errh/INCL_ymult),2));
	xsec_ratio_errl=(ymult/INCL_ymult)*sqrt(pow((ymult_errl/ymult),2)+pow((INCL_ymult_errl/INCL_ymult),2));
	
	xsec<<"sigma(Z + "<<i<<" jet)/sigma(Z + >= 0 jets)		---> "<<ymult/INCL_ymult<<" (+) "<<xsec_ratio_errh<<" (-) "<<xsec_ratio_errl<<endl;
	
	EXCLXSecRatio->SetPoint(i, i, ymult/INCL_ymult);
	EXCLXSecRatio->SetPointEYhigh(i, xsec_ratio_errh);
	EXCLXSecRatio->SetPointEYlow(i, xsec_ratio_errl);
	}
	
	xsec<<endl;
	
	TGraphAsymmErrors* INCLXSecRatio = new TGraphAsymmErrors();
	
	for(int i=0;i<bin_incl;i++){
	
	xsec_ratio_errh=0.;
	xsec_ratio_errl=0.;
	xmult=0.;
	ymult=0.;
	ymult_errh=0.;
	ymult_errl=0.;
	
	INCLCorrMult_graph->GetPoint(i, xmult, ymult);
	ymult_errh = INCLCorrMult_graph->GetErrorYhigh(i);
	ymult_errl = INCLCorrMult_graph->GetErrorYlow(i);
	
	xsec_ratio_errh=(ymult/INCL_ymult)*sqrt(pow((ymult_errh/ymult),2)+pow((INCL_ymult_errh/INCL_ymult),2));
	xsec_ratio_errl=(ymult/INCL_ymult)*sqrt(pow((ymult_errl/ymult),2)+pow((INCL_ymult_errl/INCL_ymult),2));
	
	xsec<<"sigma(Z + >= "<<i<<" jet)/sigma(Z + >= 0 jets)		---> "<<ymult/INCL_ymult<<" (+) "<<xsec_ratio_errh<<" (-) "<<xsec_ratio_errl<<endl;
	
	INCLXSecRatio->SetPoint(i, i, ymult/INCL_ymult);
	INCLXSecRatio->SetPointEYhigh(i, xsec_ratio_errh);
	INCLXSecRatio->SetPointEYlow(i, xsec_ratio_errl);
	}
	
	//////////////////////////////// Tot XSec in ALL PHASE SPACE
	
	double TotXSec = 0.; double xTotXSec = 0.;
	double TotXSec_errh = 0.; double TotXSec_errl = 0.;
	INCLXSec->GetPoint(0, xTotXSec, TotXSec);
	TotXSec_errh = INCLXSec->GetErrorYhigh(0);
	TotXSec_errl = INCLXSec->GetErrorYlow(0);
	
	double xAcc=0.;
	double yAcc=0.;
	
	MC_Acc_incl->GetPoint(0, xAcc, yAcc);
	
	xsec<<endl<<"sigma(Z + >= 0 jets) in ALL PHASE SPACE = "<<TotXSec/yAcc<<"	pb"<<endl<<endl;
		
	////////////////////////// Make Cross Section Plots ////////////////////////////////
        
	TCanvas *EXCLXSecCanvas = new TCanvas;
	EXCLXSecCanvas->SetLogy();
	EXCLXSec->SetTitle("sigma(Z + n jets)");
	EXCLXSec->GetXaxis()->SetTitle("excl. multiplicity");
	EXCLXSec->GetXaxis()->SetLimits(-0.5,bin_excl-0.5);
	EXCLXSec->GetXaxis()->SetNdivisions(bin_excl);
	EXCLXSec->GetXaxis()->SetTitleOffset(1.1);	
        EXCLXSec->SetLineWidth(2);
        EXCLXSec->SetMarkerStyle(22);
        EXCLXSec->Draw("AP");
        EXCLXSecCanvas->Write("EXCLXSec.root");
        EXCLXSecCanvas->Close();
        
        TCanvas *INCLXSecCanvas = new TCanvas;
	INCLXSecCanvas->SetLogy();
	INCLXSec->SetTitle("sigma(Z + >= n jets)");
	INCLXSec->GetXaxis()->SetTitle("incl. multiplicity");
	INCLXSec->GetXaxis()->SetLimits(-0.5,bin_incl-0.5);
	INCLXSec->GetXaxis()->SetNdivisions(bin_incl);
	INCLXSec->GetXaxis()->SetTitleOffset(1.1);	
        INCLXSec->SetLineWidth(2);
        INCLXSec->SetMarkerStyle(22);
        INCLXSec->Draw("AP");
        INCLXSecCanvas->Write("INCLXSec.root");
        INCLXSecCanvas->Close();
        
        TCanvas *EXCLXSecRatioCanvas = new TCanvas;
        EXCLXSecRatioCanvas->SetLogy();
	EXCLXSecRatio->SetTitle("sigma(Z + n jets)/sigma(Z + >= 0 jet)");
	EXCLXSecRatio->GetXaxis()->SetTitle("excl. multiplicity");
	EXCLXSecRatio->GetXaxis()->SetLimits(-0.5,bin_excl-0.5);
	EXCLXSecRatio->GetXaxis()->SetNdivisions(bin_excl);
	EXCLXSecRatio->GetXaxis()->SetTitleOffset(1.1);
        EXCLXSecRatio->SetLineWidth(2);
        EXCLXSecRatio->SetMarkerStyle(22);
        EXCLXSecRatio->Draw("AP");
        EXCLXSecRatioCanvas->Write("EXCLXSecRatio.root");
        EXCLXSecRatioCanvas->Close();
        
        TCanvas *INCLXSecRatioCanvas = new TCanvas;
        INCLXSecRatioCanvas->SetLogy();
	INCLXSecRatio->SetTitle("sigma(Z + >= n jets)/sigma(Z + >= 0 jet)");
	INCLXSecRatio->GetXaxis()->SetTitle("incl. multiplicity");
	INCLXSecRatio->GetXaxis()->SetLimits(0.5,bin_incl-0.5);
	INCLXSecRatio->GetYaxis()->SetRangeUser(0,0.25);
	INCLXSecRatio->GetXaxis()->SetNdivisions(bin_incl);
	INCLXSecRatio->GetXaxis()->SetTitleOffset(1.1);
        INCLXSecRatio->SetLineWidth(2);
        INCLXSecRatio->SetMarkerStyle(22);
        INCLXSecRatio->Draw("AP");
        INCLXSecRatioCanvas->Write("INCLXSecRatio.root");
        INCLXSecRatioCanvas->Close();
                
      
        xsec.close();
        multrep.close();
        
        outplots->Write();
        outplots->Close();
        
        /*TPaveText* binUR = new TPaveText(0.716954, 0.0466102, 0.758621, 0.0889831, "BRNDC"); 
        binUR->AddText("#geq");
        binUR->SetBorderSize(0);
        binUR->SetShadowColor(0);
        binUR->SetFillColor(0);
        binUR->SetTextAlign(31);
        binUR->Draw();*/
  
}
