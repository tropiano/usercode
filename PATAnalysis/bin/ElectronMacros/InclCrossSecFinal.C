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

//JetPtMin in GeV 

void XSecFinal(int JetPtMin){

        int bin_incl = 5;
        if(JetPtMin==30)bin_incl=4;
        
        double Luminosity = 36.162; //pb-1
        double LuminosityErr = 1.4; //pb-1
        
        double RelUncLumi = LuminosityErr/Luminosity;

        string JetPtCut;
	if(JetPtMin==15)JetPtCut="JetPt15";
	if(JetPtMin==30)JetPtCut="JetPt30";

        //Output
        string output = "InclXSecFinal_";
        output+=JetPtCut+".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
        
        //Report files
        ofstream finrep;
	finrep.open(("InclFinalReport_"+JetPtCut+".txt").c_str());
	finrep<<endl;
	
	TDirectory *UnfSystematicsDir, *JECSystematicsDir, *EffSystematicsDir, *FinalPlotsDir;
	
	UnfSystematicsDir = outplots->mkdir("UnfSystematics");
	JECSystematicsDir = outplots->mkdir("JECSystematics");
	EffSystematicsDir = outplots->mkdir("EffSystematics");
	FinalPlotsDir = outplots->mkdir("FinalPlots");
        
        string path = "/data/sfrosali/Zjets/NEW_CMSSW_3_9_7/CMSSW_3_9_7/src/Firenze/PATAnalysis/bin";
        
        /////////////////// GET FILES /////////////////////
        
        //Z2 Tune - XSec Estimation    
        TFile *Z2_XSec = TFile::Open((path+"/XSec/"+JetPtCut+"/Z2/InclXSecMeas_"+JetPtCut+".root").c_str());
        //-------------------------JEC SYSTEMATICS----------------------------
        //Z2 - JEC unc plus
        TFile *Z2_JEC_Plus = TFile::Open((path+"/XSec/"+JetPtCut+"/Z2/InclXSecMeas_jecUncPlus_"+JetPtCut+".root").c_str());
        
        //Z2 - JEC unc minus
        TFile *Z2_JEC_Minus = TFile::Open((path+"/XSec/"+JetPtCut+"/Z2/InclXSecMeas_jecUncMinus_"+JetPtCut+".root").c_str());
        
        //-------------------------UNFOLDING SYSTEMATICS----------------------------
        //D6T Tune
        TFile *D6T_Unf = TFile::Open((path+"/XSec/"+JetPtCut+"/D6T/InclXSecMeas_"+JetPtCut+".root").c_str());
        
        //-------------------------EFFICIENCY FITS SYSTEMATICS----------------------------
        //D6T Tune AllTuneChanges
        TFile *D6T_Eff = TFile::Open((path+"/XSec/"+JetPtCut+"/D6T/InclXSecMeas_"+JetPtCut+"_AllTuneCh.root").c_str());
        
        
        /////////////////////// Get Histograms //////////////////////////////

        //-------------------------- Get Histograms Z2
        
        FinalPlotsDir->cd();
        
        TGraphAsymmErrors* Multiplicity_graph = (TGraphAsymmErrors*) Z2_XSec->Get("CorrMultiplicity/Multiplicity/INCLCorrMult_graph");
        Multiplicity_graph->Write("Multiplicity_graph");
        TGraphAsymmErrors* MultRatio_graph = (TGraphAsymmErrors*) Z2_XSec->Get("CorrMultiplicity/Multiplicity/INCLUnfMultRatio_graph");
        MultRatio_graph->Write("MultRatio_graph");
        TH1D* GENMult = (TH1D*) Z2_XSec->Get("CorrMultiplicity/Multiplicity/GENMult_incl");
        GENMult->Write("GENMult");
        TH1D* GENMultPythia = (TH1D*) Z2_XSec->Get("CorrMultiplicity/Multiplicity/GENMultPythia_incl");
        GENMultPythia->Write("GENMultPythia");
        
        //-------------------------- Get Histograms UNF - D6T
        
        TGraphAsymmErrors* D6TMult_graph = (TGraphAsymmErrors*) D6T_Unf->Get("CorrMultiplicity/Multiplicity/INCLCorrMult_graph");
        D6TMult_graph->Write("D6TMult_graph");
             
        //-------------------------- Get Histograms JEC - Z2
        
        JECSystematicsDir->cd();
        
        TGraphAsymmErrors* Multiplicity_graph_jecPlus = (TGraphAsymmErrors*) Z2_JEC_Plus->Get("CorrMultiplicity/Multiplicity/INCLCorrMult_graph");
        Multiplicity_graph_jecPlus->Write("Multiplicity_graph_jecPlus");
        TGraphAsymmErrors* Multiplicity_graph_jecMinus = (TGraphAsymmErrors*) Z2_JEC_Minus->Get("CorrMultiplicity/Multiplicity/INCLCorrMult_graph");
        Multiplicity_graph_jecMinus->Write("Multiplicity_graph_jecMinus");
        
        //-------------------------- Get Histograms Efficiencies
        
        EffSystematicsDir->cd();
        
        TGraphAsymmErrors* Eff_Z2 = (TGraphAsymmErrors*) Z2_XSec->Get("CorrMultiplicity/Multiplicity/Efficiency_graph");
        Eff_Z2->Write("Efficiency_Z2");
        TGraphAsymmErrors* Eff_D6T = (TGraphAsymmErrors*) D6T_Eff->Get("CorrMultiplicity/Multiplicity/Efficiency_graph");
        Eff_D6T->Write("Efficiency_D6T");       
        
        ////////////////////////// SYSTEMATICS CALCULATION ////////////////////////////////
        
        UnfSystematicsDir->cd();
        TGraphAsymmErrors* UnfoldingSyst_graph = new TGraphAsymmErrors();
        JECSystematicsDir->cd(); 
        TGraphAsymmErrors* JECSyst_graph = new TGraphAsymmErrors();
        EffSystematicsDir->cd();
        TGraphAsymmErrors* EffSyst_graph = new TGraphAsymmErrors();
        
        finrep<<endl<<" ### Inclusive Multiplicity ### "<<endl<<endl;
        
        double xcorr_Z2_incl, ycorr_Z2_incl;
        double xcorr_D6T_incl, ycorr_D6T_incl;
        double xcorr_Z2_jecP_incl, ycorr_Z2_jecP_incl;
        double xcorr_Z2_jecM_incl, ycorr_Z2_jecM_incl;
        double JEChigh, JEClow;
        
        double xeff_Z2, yeff_Z2;
        double xeff_D6T, yeff_D6T;
        double yeff_Z2_errh, yeff_Z2_errl;
        
        for(int i=0;i<bin_incl;i++){
        
        xcorr_Z2_incl=0.; ycorr_Z2_incl=0.;
        xcorr_D6T_incl=0.; ycorr_D6T_incl=0.;
        xcorr_Z2_jecP_incl=0.; ycorr_Z2_jecP_incl=0.;
        xcorr_Z2_jecM_incl=0.; ycorr_Z2_jecM_incl=0.;
        JEChigh=0.; JEClow=0.;
        xeff_Z2 = 0.;
        yeff_Z2 = 0.;
        xeff_D6T = 0.;
        yeff_D6T = 0.;
        yeff_Z2_errh = 0.; yeff_Z2_errl = 0.;
        
        finrep<<endl<<"---------------------------> Bin n. "<<i<<", Jet# >= "<<i<<" <------------------------------"<<endl<<endl;
        
        Multiplicity_graph->GetPoint(i, xcorr_Z2_incl, ycorr_Z2_incl);
        D6TMult_graph->GetPoint(i, xcorr_D6T_incl, ycorr_D6T_incl);
        
        finrep<<"Z2 Unf. Multiplicity =		"<<ycorr_Z2_incl<<endl<<endl;
        
        finrep<<"UNFOLDING SYSTEMATICS:"<<endl<<endl;
                     
        finrep<<"Unf. Syst. Z2-D6T =		"<<ycorr_Z2_incl-ycorr_D6T_incl<<endl;
        finrep<<"RELATIVE Unf. Syst. =		"<<TMath::Abs(ycorr_Z2_incl-ycorr_D6T_incl)/ycorr_Z2_incl<<endl<<endl;
        
        finrep<<"JEC SYSTEMATICS:"<<endl<<endl;
        
        Multiplicity_graph_jecPlus->GetPoint(i, xcorr_Z2_jecP_incl, ycorr_Z2_jecP_incl);
        Multiplicity_graph_jecMinus->GetPoint(i, xcorr_Z2_jecM_incl, ycorr_Z2_jecM_incl);
        
        JEChigh = (ycorr_Z2_incl-ycorr_Z2_jecP_incl)<0 ? TMath::Abs(ycorr_Z2_incl-ycorr_Z2_jecP_incl) : TMath::Abs(ycorr_Z2_incl-ycorr_Z2_jecM_incl);
        
        JEClow = (ycorr_Z2_incl-ycorr_Z2_jecP_incl)>0 ? (ycorr_Z2_incl-ycorr_Z2_jecP_incl) : (ycorr_Z2_incl-ycorr_Z2_jecM_incl);
        
        finrep<<"JEC Systematics =	+ "<<JEChigh<<" - "<<JEClow<<endl;
        finrep<<"RELATIVE JEC Syst. =	(High)	"<<JEChigh/ycorr_Z2_incl<<"	(Low)	"<<JEClow/ycorr_Z2_incl<<endl<<endl;
        
        UnfoldingSyst_graph->SetPoint(i, i, ycorr_Z2_incl);
        if((ycorr_Z2_incl-ycorr_D6T_incl)>0)UnfoldingSyst_graph->SetPointEYlow(i, TMath::Abs(ycorr_Z2_incl-ycorr_D6T_incl));
        if((ycorr_Z2_incl-ycorr_D6T_incl)<0)UnfoldingSyst_graph->SetPointEYhigh(i, TMath::Abs(ycorr_Z2_incl-ycorr_D6T_incl));
        UnfoldingSyst_graph->SetPointEXhigh(i, 0.5);
        UnfoldingSyst_graph->SetPointEXlow(i, 0.5);
        
        JECSyst_graph->SetPoint(i, i, ycorr_Z2_incl);
        JECSyst_graph->SetPointEYhigh(i, JEChigh);
        JECSyst_graph->SetPointEYlow(i, JEClow);
        JECSyst_graph->SetPointEXhigh(i, 0.5);
        JECSyst_graph->SetPointEXlow(i, 0.5);    
        
        finrep<<"EFFICIENCY SYSTEMATICS:"<<endl<<endl; 
        
        Eff_Z2->GetPoint(i, xeff_Z2, yeff_Z2);
        yeff_Z2_errh = Eff_Z2->GetErrorYhigh(i);
        yeff_Z2_errl = Eff_Z2->GetErrorYlow(i);
        
        finrep<<"Efficiency = "<<yeff_Z2<<" + "<<yeff_Z2_errh<<" - "<<yeff_Z2_errl<<endl;
        finrep<<"Eff. Rel. Unc. =	(High)	"<<yeff_Z2_errh/yeff_Z2<<"	(Low)	"<<yeff_Z2_errl/yeff_Z2<<endl<<endl;
        
        Eff_D6T->GetPoint(i, xeff_D6T, yeff_D6T);
        
        EffSyst_graph->SetPoint(i, i, yeff_Z2);
        if((yeff_Z2-yeff_D6T)>0)EffSyst_graph->SetPointEYlow(i, TMath::Abs(yeff_Z2-yeff_D6T));
        if((yeff_Z2-yeff_D6T)<0)EffSyst_graph->SetPointEYhigh(i, TMath::Abs(yeff_Z2-yeff_D6T));
        EffSyst_graph->SetPointEXhigh(i, 0.5);
        EffSyst_graph->SetPointEXlow(i, 0.5);
        
        finrep<<"Eff. Syst. Z2-D6T =		"<<yeff_Z2-yeff_D6T<<endl;
        finrep<<"RELATIVE Eff. Syst. =		"<<TMath::Abs(yeff_Z2-yeff_D6T)/yeff_Z2<<endl<<endl;
        
        finrep<<"LUMINOSITY SYSTEMATICS:"<<endl<<endl;
        
        finrep<<"RELATIVE Lumi Syst. =		"<<RelUncLumi<<endl<<endl;
                
        }
        
        UnfSystematicsDir->cd();
        UnfoldingSyst_graph->Write("UnfoldingSyst_graph");
        JECSystematicsDir->cd();
        JECSyst_graph->Write("JECSyst_graph");
        EffSystematicsDir->cd();
        EffSyst_graph->Write("EffSyst_graph");
        
        ///////////////////////// Make Plots of Systematics ////////////////////
        
        UnfSystematicsDir->cd();
        
        TCanvas *UnfoldingSyst = new TCanvas; 
        UnfoldingSyst->SetLogy();    
        UnfoldingSyst_graph->SetLineWidth(2);
        UnfoldingSyst_graph->SetMarkerStyle(23);
        UnfoldingSyst_graph->GetXaxis()->SetTitle("inclusive multiplicity");
        UnfoldingSyst_graph->Draw("AP");  
	UnfoldingSyst_graph->SetTitle("Z2 Mult. with Unfolding Systematics");                                  
        UnfoldingSyst->Write("Unfolding_Syst.root");
        UnfoldingSyst->Close();
        
        JECSystematicsDir->cd();
        
        TCanvas *JECSyst = new TCanvas; 
        JECSyst->SetLogy();    
        JECSyst_graph->SetLineWidth(2);
        JECSyst_graph->SetMarkerStyle(23);
        JECSyst_graph->GetXaxis()->SetTitle("inclusive multiplicity");
        JECSyst_graph->Draw("AP");  
	JECSyst_graph->SetTitle("Z2 Mult. with JEC Systematics");                                  
        JECSyst->Write("JEC_Syst.root");
        JECSyst->Close();
        
        EffSystematicsDir->cd();
        
        TCanvas *INCLEffSyst = new TCanvas;     
        EffSyst_graph->SetLineWidth(2);
        EffSyst_graph->SetMarkerStyle(23);
        EffSyst_graph->GetXaxis()->SetTitle("inclusive multiplicity");
        EffSyst_graph->Draw("AP");  
	EffSyst_graph->SetTitle("Efficiency with Z2-D6T Systematic");                                  
        INCLEffSyst->Write("Eff_Syst.root");
        INCLEffSyst->Close();
        
        //////////////////// Make Plot of Multiplicity with ALL Systematics ////////////////
        
        FinalPlotsDir->cd();
        
        TGraphAsymmErrors* Systematics_graph = new TGraphAsymmErrors();
        TGraphAsymmErrors* SystematicsNoLumi_graph = new TGraphAsymmErrors();
        
        double xmult, ymult, ymult_errh, ymult_errl;
        double xeff, yeff, yeff_errh, yeff_errl;
        double xsysteff, ysysteff, ysysteff_errh, ysysteff_errl; 
        double xunf, yunf, yunf_errh, yunf_errl;
        double xjec, yjec, yjec_errh, yjec_errl;
        
        double SystRel_h, SystRel_l;
        double SystAbs_h, SystAbs_l;
        
        double SystRelNoLumi_h, SystRelNoLumi_l;
        double SystAbsNoLumi_h, SystAbsNoLumi_l;
        
        for(int i=0;i<bin_incl;i++){
        
        xmult=0.; ymult=0.; ymult_errh=0.; ymult_errl=0.;
        xeff=0.; yeff=0.; yeff_errh=0.; yeff_errl=0.;
        xsysteff=0.; ysysteff=0.; ysysteff_errh=0.; ysysteff_errl=0.;
        xunf=0.; yunf=0.; yunf_errh=0.; yunf_errl=0.;
        xjec=0.; yjec=0.; yjec_errh=0.; yjec_errl=0.;
        
        SystRel_h=0.; SystRel_l=0.;
        SystAbs_h=0.; SystAbs_l=0.;
        
        Multiplicity_graph->GetPoint(i, xmult, ymult);
        
        Systematics_graph->SetPoint(i, xmult, ymult);
        SystematicsNoLumi_graph->SetPoint(i, xmult, ymult);
        
        Eff_Z2->GetPoint(i, xeff, yeff);
        yeff_errh = Eff_Z2->GetErrorYhigh(i);
        yeff_errl = Eff_Z2->GetErrorYlow(i);
        
        EffSyst_graph->GetPoint(i, xsysteff, ysysteff);
        ysysteff_errh = EffSyst_graph->GetErrorYhigh(i);
        ysysteff_errl = EffSyst_graph->GetErrorYlow(i);
        
        UnfoldingSyst_graph->GetPoint(i, xunf, yunf);
        yunf_errh = UnfoldingSyst_graph->GetErrorYhigh(i);
        yunf_errl = UnfoldingSyst_graph->GetErrorYlow(i);
        
        JECSyst_graph->GetPoint(i, xjec, yjec);
        yjec_errh = JECSyst_graph->GetErrorYhigh(i);
        yjec_errl = JECSyst_graph->GetErrorYlow(i);
        
        //Total Relative Systematics
        
        //NB: inverto gli errori high e low dell'efficienza perchè è a denominatore
        //NB: considero anche l'errore sulla luminosità
        
        SystRel_h = sqrt(pow(yeff_errl/yeff,2)+pow(ysysteff_errl/ysysteff,2)+pow(yunf_errh/yunf,2)+pow(yjec_errh/yjec,2)+pow(RelUncLumi,2));    
        SystRel_l = sqrt(pow(yeff_errh/yeff,2)+pow(ysysteff_errh/ysysteff,2)+pow(yunf_errl/yunf,2)+pow(yjec_errl/yjec,2)+pow(RelUncLumi,2));
        
        SystRelNoLumi_h = sqrt(pow(yeff_errl/yeff,2)+pow(ysysteff_errl/ysysteff,2)+pow(yunf_errh/yunf,2)+pow(yjec_errh/yjec,2));    
        SystRelNoLumi_l = sqrt(pow(yeff_errh/yeff,2)+pow(ysysteff_errh/ysysteff,2)+pow(yunf_errl/yunf,2)+pow(yjec_errl/yjec,2));
        
        //Total Absolute Systematics
        
        SystAbs_h = SystRel_h * ymult;
        SystAbs_l = SystRel_l * ymult;
        
        SystAbsNoLumi_h = SystRelNoLumi_h * ymult;
        SystAbsNoLumi_l = SystRelNoLumi_l * ymult;
        
        Systematics_graph->SetPointEYhigh(i, SystAbs_h);
        Systematics_graph->SetPointEYlow(i, SystAbs_l);
        Systematics_graph->SetPointEXhigh(i, 0.5);
        Systematics_graph->SetPointEXlow(i, 0.5);
        
        SystematicsNoLumi_graph->SetPointEYhigh(i, SystAbsNoLumi_h);
        SystematicsNoLumi_graph->SetPointEYlow(i, SystAbsNoLumi_l);
        SystematicsNoLumi_graph->SetPointEXhigh(i, 0.5);
        SystematicsNoLumi_graph->SetPointEXlow(i, 0.5); 
   
        }
        
        FinalPlotsDir->cd();
        Systematics_graph->Write("Systematics_graph");
        SystematicsNoLumi_graph->Write("SystematicsNoLumi_graph");
        
        TCanvas *SystematicsCanv = new TCanvas;  
        SystematicsCanv->SetLogy();   
        Systematics_graph->SetLineWidth(2);
        Systematics_graph->SetMarkerStyle(23);
        Systematics_graph->GetXaxis()->SetTitle("inclusive multiplicity");
        Systematics_graph->Draw("AP");  
	Systematics_graph->SetTitle("Total Systematics");                                  
        SystematicsCanv->Write("Systematics.root");
        SystematicsCanv->Close();
        
        TCanvas *SystematicsNoLumiCanv = new TCanvas;  
        SystematicsNoLumiCanv->SetLogy();   
        SystematicsNoLumi_graph->SetLineWidth(2);
        SystematicsNoLumi_graph->SetMarkerStyle(23);
        SystematicsNoLumi_graph->GetXaxis()->SetTitle("inclusive multiplicity");
        SystematicsNoLumi_graph->Draw("AP");  
	SystematicsNoLumi_graph->SetTitle("Total Systematics (NoLumi)");                                  
        SystematicsNoLumiCanv->Write("SystematicsNoLumi.root");
        SystematicsNoLumiCanv->Close();
        
        ////////////////// Final Multiplicity Plot /////////////////////
        
        double xgen, ygen, ygen_err;
        
        TGraphAsymmErrors* GENMult_graph = new TGraphAsymmErrors();
        
        for(int i=0;i<bin_incl;i++){
        ygen=0.;
        ygen_err=0.;
        ygen = GENMult->GetBinContent(i+1);
        ygen_err = GENMult->GetBinError(i+1);
        GENMult_graph->SetPoint(i, i, ygen);
        GENMult_graph->SetPointEYhigh(i, ygen_err);
        GENMult_graph->SetPointEYlow(i, ygen_err);
        GENMult_graph->SetPointEXhigh(i, 0.5);
        GENMult_graph->SetPointEXlow(i, 0.5);}
        
        TGraphAsymmErrors* GENMultPythia_graph = new TGraphAsymmErrors();
        
        for(int i=0;i<bin_incl;i++){
        ygen=0.;
        ygen_err=0.;
        ygen = GENMultPythia->GetBinContent(i+1);
        ygen_err = GENMultPythia->GetBinError(i+1);
        GENMultPythia_graph->SetPoint(i, i, ygen);
        GENMultPythia_graph->SetPointEYhigh(i, ygen_err);
        GENMultPythia_graph->SetPointEYlow(i, ygen_err);
        GENMultPythia_graph->SetPointEXhigh(i, 0.5);
        GENMultPythia_graph->SetPointEXlow(i, 0.5);}
        
        FinalPlotsDir->cd();
        
        TCanvas *MultiplicityCanv = new TCanvas; 
        MultiplicityCanv->SetLogy();
        Systematics_graph->SetTitle("GEN Mult. (red) - Pythia Mult. (yellow) - DATA Unf.Mult. (black)");
        Systematics_graph->SetFillStyle(3004);
        Systematics_graph->SetFillColor(804);
        Systematics_graph->GetXaxis()->SetTitle("inclusive multiplicity");
        Systematics_graph->GetXaxis()->SetRangeUser(-0.7,bin_incl-0.3);
        Systematics_graph->GetXaxis()->SetNdivisions(6);
        Systematics_graph->GetYaxis()->SetTitle("N(Z + #geq n jets)");
        Systematics_graph->GetYaxis()->SetRangeUser(10,50000);
        Systematics_graph->Draw("APE2");     
        //SystematicsNoLumi_graph->SetFillStyle(3003);
        //SystematicsNoLumi_graph->SetFillColor(798);
        //SystematicsNoLumi_graph->Draw("PE2SAME");           
        GENMult_graph->SetLineColor(2);
        GENMult_graph->SetLineWidth(2);
        GENMult_graph->SetMarkerStyle(23);
        GENMult_graph->SetMarkerColor(2);
        GENMult_graph->Draw("PSAME");
        GENMultPythia_graph->SetLineColor(807);
        GENMultPythia_graph->SetLineWidth(2);
        GENMultPythia_graph->SetMarkerStyle(23);
        GENMultPythia_graph->SetMarkerColor(807);
        GENMultPythia_graph->Draw("PSAME");
        Multiplicity_graph->SetLineColor(1);
        Multiplicity_graph->SetLineWidth(2);
	Multiplicity_graph->SetMarkerStyle(20);
        Multiplicity_graph->Draw("PSAME");
        MultiplicityCanv->Write("FinalMult.root");
        MultiplicityCanv->Close();
        
        TGraphAsymmErrors* SystematicsRatio_graph = new TGraphAsymmErrors();
        TGraphAsymmErrors* SystematicsNoLumiRatio_graph = new TGraphAsymmErrors();
        
        double xratio, yratio, yratio_errh, yratio_errl;
        double xratioSyst, yratioSyst, yratioSyst_errh, yratioSyst_errl;
        double xratioSystNoL, yratioSystNoL, yratioSystNoL_errh, yratioSystNoL_errl;

        for(int i=0;i<bin_incl;i++){
        
        xratio=0.; yratio=0.; yratio_errh=0.; yratio_errl=0.;
        xratioSyst=0.; yratioSyst=0.; yratioSyst_errh=0.; yratioSyst_errl=0.;
        xratioSystNoL=0.; yratioSystNoL=0.; yratioSystNoL_errh=0.; yratioSystNoL_errl=0.;
        
        xgen=0.; ygen=0.;
   
        MultRatio_graph->GetPoint(i, xratio, yratio);
        GENMult_graph->GetPoint(i, xgen, ygen);
        
        SystematicsRatio_graph->SetPoint(i, xratio, yratio);
        SystematicsNoLumiRatio_graph->SetPoint(i, xratio, yratio);
        
        SystematicsRatio_graph->SetPointEYhigh(i, Systematics_graph->GetErrorYhigh(i)/ygen);
        SystematicsRatio_graph->SetPointEYlow(i, Systematics_graph->GetErrorYlow(i)/ygen);
        SystematicsRatio_graph->SetPointEXhigh(i, 0.5);
        SystematicsRatio_graph->SetPointEXlow(i, 0.5);
        
        SystematicsNoLumiRatio_graph->SetPointEYhigh(i, SystematicsNoLumi_graph->GetErrorYhigh(i)/ygen);
        SystematicsNoLumiRatio_graph->SetPointEYlow(i, SystematicsNoLumi_graph->GetErrorYlow(i)/ygen);
        SystematicsNoLumiRatio_graph->SetPointEXhigh(i, 0.5);
        SystematicsNoLumiRatio_graph->SetPointEXlow(i, 0.5);
        
        }
        
        FinalPlotsDir->cd();
        SystematicsRatio_graph->Write("SystematicsRatio_graph");
        SystematicsNoLumiRatio_graph->Write("SystematicsNoLumiRatio_graph");
        
        TCanvas *SystematicsRatioCanv = new TCanvas;  
        SystematicsRatioCanv->SetLogy();   
        SystematicsRatio_graph->SetLineWidth(2);
        SystematicsRatio_graph->SetMarkerStyle(23);
        SystematicsRatio_graph->GetXaxis()->SetTitle("inclusive multiplicity");
        SystematicsRatio_graph->Draw("AP");  
	SystematicsRatio_graph->SetTitle("Total Ratio Systematics");                                  
        SystematicsRatioCanv->Write("SystematicsRatio.root");
        SystematicsRatioCanv->Close();
        
        TCanvas *SystematicsNoLumiRatioCanv = new TCanvas;  
        SystematicsNoLumiRatioCanv->SetLogy();   
        SystematicsNoLumiRatio_graph->SetLineWidth(2);
        SystematicsNoLumiRatio_graph->SetMarkerStyle(23);
        SystematicsNoLumiRatio_graph->GetXaxis()->SetTitle("inclusive multiplicity");
        SystematicsNoLumiRatio_graph->Draw("AP");  
	SystematicsNoLumiRatio_graph->SetTitle("Total Ratio Systematics (NoLumi)");                                  
        SystematicsNoLumiRatioCanv->Write("SystematicsNoLumiRatio.root");
        SystematicsNoLumiRatioCanv->Close();
        
        TCanvas *RatioCanv = new TCanvas;
        RatioCanv->SetCanvasSize(600,300);
        RatioCanv->SetGridx();
        RatioCanv->SetGridy();
        SystematicsRatio_graph->SetFillStyle(3004);
        SystematicsRatio_graph->SetFillColor(804);
        SystematicsRatio_graph->GetXaxis()->SetTitle("inclusive multiplicity");
        SystematicsRatio_graph->GetXaxis()->SetRangeUser(-0.7,bin_incl-0.3);
        SystematicsRatio_graph->GetXaxis()->SetNdivisions(6);
        SystematicsRatio_graph->GetYaxis()->SetTitle("#frac{N_{Data}(Z + #geq n jets)}{N_{MC}(Z + #geq n jets)}");
        SystematicsRatio_graph->GetYaxis()->CenterTitle();
        SystematicsRatio_graph->GetYaxis()->SetTitleSize(0.05);
        SystematicsRatio_graph->GetYaxis()->SetTitleOffset(1.2);
        SystematicsRatio_graph->SetTitle("DATA Unf.Mult./GEN Mult.");
        SystematicsRatio_graph->GetYaxis()->SetRangeUser(0.4,1.6);
        SystematicsRatio_graph->Draw("APE2");   
        SystematicsNoLumiRatio_graph->SetFillStyle(1001);
        SystematicsNoLumiRatio_graph->SetFillColor(798);
        SystematicsNoLumiRatio_graph->Draw("PE2SAME");     
        MultRatio_graph->SetLineColor(1);
        MultRatio_graph->SetLineWidth(2);
        MultRatio_graph->SetMarkerStyle(20);       
        MultRatio_graph->Draw("PSAME");
        RatioCanv->Write("FinalMultRatio.root");
        RatioCanv->Close();
        
     
  
        finrep.close();

        outplots->Write();
        outplots->Close();

}
