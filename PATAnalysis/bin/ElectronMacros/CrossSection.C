#include <vector>
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
#include "TGraphAsymmErrors.h"
#include "TMath.h"

using namespace std;

void XSec(){

        gROOT->SetStyle("Plain");
        
        //GET FILES
        
        //DATA: Signal and Background Yields from TP fits
        TFile *SB_Yields = TFile::Open("/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/TagProbe/JetPt15/TPAnalyzer/TestTP_ExclDouble.root");
        
        //DATA: Tag&Probe Efficiencies
        TFile *TPEff = TFile::Open("/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/Efficienze_TP/JetPt15/TPStudy_JetPt15_Excl_Global.root");
        
        //MC: Efficiency and Unfolding Correction Factors
        TFile *MCEff = TFile::Open("/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin/Efficienze_MC/SignalStudy_ZMadgraph_JetPt15_2.root");
        
        //Output
        string out = "XSecMeas";
        string output = out;
        output+=".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
      
        //GET HISTOGRAMS
        
        //Signal and Background Yields
        TGraphAsymmErrors* SignalY = (TGraphAsymmErrors*) SB_Yields->Get("YieldPlots/SignalYield_0");
        TGraphAsymmErrors* BackgroundY = (TGraphAsymmErrors*) SB_Yields->Get("YieldPlots/BackgroundYield_0");
        
        //TP Efficiencies 
        TGraphAsymmErrors* TPData_Eff = (TGraphAsymmErrors*) TPEff->Get("Tag&Probe/Tag&Probe_Global/TP_Data-Eff_Global");
        TGraphAsymmErrors* TPMC_Eff = (TGraphAsymmErrors*) TPEff->Get("Tag&Probe/Tag&Probe_Global/TP_MC-DoubleEff_Global");
        
        //MC Efficiency
        TGraphAsymmErrors* MC_Eff = (TGraphAsymmErrors*) MCEff->Get("Global_Efficiency/Efficiency_vs_RecoExclJet/EffVsRecoExclJetN_AccASYM_Trg_Imp_ConvASYM_IsoASYM_EiDASYM");
        
        //Unfolding Correction Factors
        TH1D* UCF = (TH1D*) MCEff->Get("Unfolding_CorrFactors/Unfolding_CorrFactors");
        
        //Report file
        ofstream xsec;
	string outxsec="XSecReport.txt";
	xsec.open(outxsec.c_str());
	xsec<<endl;
        
        ///////////////Corrected Jet Multiplicity
        
        TGraphAsymmErrors* CorrMult = new TGraphAsymmErrors();
        
        double BinCorrMult, BinUCF;
        double xs, ys, xb, yb;
        double xtpdata, ytpdata, xtpmc, ytpmc;
        double xmceff, ymceff;
        
        xsec<<"### Unfolded Multiplicity ###"<<endl<<endl;
        
        for(int i=0;i<5;i++){
        
        xsec<<"---> Bin n. "<<i<<endl<<endl;
        
        BinCorrMult=0.;
        BinUCF=0.;
        
        xs=0.;
        ys=0.;
        xb=0.;
        yb=0.;
        
        xtpdata=0.;
        ytpdata=0.;
        xtpmc=0.;
        ytpmc=0.;
        
        xmceff=0.;
        ymceff=0.;
        
        SignalY->GetPoint(i, xs, ys);
        xsec<<"Signal Yields = "<<ys<<endl;
        BackgroundY->GetPoint(i, xb, yb);
        xsec<<"Background Yields = "<<yb<<endl;
        TPData_Eff->GetPoint(i, xtpdata, ytpdata);
        xsec<<"TP Data Efficiency = "<<ytpdata<<endl;
        TPMC_Eff->GetPoint(i, xtpmc, ytpmc);
        xsec<<"TP MC Efficiency = "<<ytpmc<<endl;       
        MC_Eff->GetPoint(i, xmceff, ymceff);
        xsec<<"MC Global Efficiency = "<<ymceff<<endl;
        BinUCF = UCF->GetBinContent(i+1);
        xsec<<"Unf. Correction Factor = "<<BinUCF<<endl<<endl;
        
        //Corrected Multiplicity calculation
        BinCorrMult = ((ys -yb)/(ymceff*(ytpdata/ytpmc)))*BinUCF;
        
        xsec<<"Corrected Multiplcity = "<<BinCorrMult<<endl<<endl;
        
        CorrMult->SetPoint(i, i, BinCorrMult);
        CorrMult->SetPointEXlow(i, 0.5);
        CorrMult->SetPointEXhigh(i, 0.5);   
        
        }
        
        
	TCanvas *UnfMult = new TCanvas;
	CorrMult->SetTitle("DATA - Corrected Exclusive Multiplicity");
	CorrMult->GetXaxis()->SetTitle("Exclusive Jet Multiplicity");
        CorrMult->SetLineWidth(2);
        CorrMult->SetMarkerStyle(21);
        CorrMult->Draw("AP");
        UnfMult->Write("Corrected_Multiplicity.root");
        UnfMult->Close();
	
        
        
        xsec.close();
        
        outplots->Write();
        outplots->Close();
  
}
