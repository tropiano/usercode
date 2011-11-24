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

//JetPtMin in GeV - unfSyst = "cf" -> D6T only for CorrFact; unfSyst = "all" -> D6T for fit & CorrFact

void XSecFinal(int JetPtMin, string _unfSyst){

        int bin_incl = 5;

        string unfSyst="";
        if(_unfSyst=="cf")unfSyst="";
        if(_unfSyst=="all")unfSyst="_AllTuneCh";

        string JetPtCut;
	if(JetPtMin==15)JetPtCut="JetPt15";
	if(JetPtMin==30)JetPtCut="JetPt30";

        //Output
        string output = "InclXSecFinal_";
        output+=JetPtCut+unfSyst+".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
        
        //Report files
        ofstream finrep;
	finrep.open(("InclFinalReport_"+JetPtCut+unfSyst+".txt").c_str());
	finrep<<endl;
	
	TDirectory *UnfSystematicsDir, *JECSystematicsDir;
	UnfSystematicsDir = outplots->mkdir("UnfSystematics");
	JECSystematicsDir = outplots->mkdir("JECSystematics");
        
        string path = "/data/sfrosali/Zjets/NEW_CMSSW_3_9_7/CMSSW_3_9_7/src/Firenze/PATAnalysis/bin";
        
        ////////// GET FILES /////////
        
        //Z2 Tune       
        TFile *Z2_XSec = TFile::Open((path+"/XSec/"+JetPtCut+"/Z2/InclXSecMeas_"+JetPtCut+".root").c_str());
        
        //Z2 - JEC unc plus
        TFile *Z2_XSec_jecPlus = TFile::Open((path+"/XSec/"+JetPtCut+"/Z2/InclXSecMeas_jecUncPlus_"+JetPtCut+".root").c_str());
        
        //Z2 - JEC unc minus
        TFile *Z2_XSec_jecMinus = TFile::Open((path+"/XSec/"+JetPtCut+"/Z2/InclXSecMeas_jecUncMinus_"+JetPtCut+".root").c_str());
        
        //D6T Tune
        TFile *D6T_XSec = TFile::Open((path+"/XSec/"+JetPtCut+"/D6T/InclXSecMeas_"+JetPtCut+unfSyst+".root").c_str());
        
        UnfSystematicsDir->cd();

        ////////////////// Get Histograms UNF - Z2
        
        TGraphAsymmErrors* INCLCorrMult_Z2 = (TGraphAsymmErrors*) Z2_XSec->Get("CorrMultiplicity/UnfoldedMult/INCLCorrMult_graph");
        INCLCorrMult_Z2->Write("INCLCorrMult_Z2");
        
        ////////////////// Get Histograms UNF - D6T
        
        TGraphAsymmErrors* INCLCorrMult_D6T = (TGraphAsymmErrors*) D6T_XSec->Get("CorrMultiplicity/UnfoldedMult/INCLCorrMult_graph");
        INCLCorrMult_D6T->Write("INCLCorrMult_D6T");
        
        JECSystematicsDir->cd();
        
        ////////////////// Get Histograms JEC - Z2
        
        TGraphAsymmErrors* INCLCorrMult_Z2_jecPlus = (TGraphAsymmErrors*) Z2_XSec_jecPlus->Get("CorrMultiplicity/UnfoldedMult/INCLCorrMult_graph");
        INCLCorrMult_Z2_jecPlus->Write("INCLCorrMult_Z2_jecPlus");
        TGraphAsymmErrors* INCLCorrMult_Z2_jecMinus = (TGraphAsymmErrors*) Z2_XSec_jecMinus->Get("CorrMultiplicity/UnfoldedMult/INCLCorrMult_graph");
        INCLCorrMult_Z2_jecMinus->Write("INCLCorrMult_Z2_jecMinus");
        
        /////////////////////// INCLUSIVE
        
        UnfSystematicsDir->cd();
        TGraphAsymmErrors* INCLUnfoldingSyst_graph = new TGraphAsymmErrors();
        JECSystematicsDir->cd(); 
        TGraphAsymmErrors* INCLJECSyst_graph = new TGraphAsymmErrors();
        
        finrep<<endl<<" ### Inclusive Multiplicity ### "<<endl<<endl;
        
        double xcorr_Z2_incl, ycorr_Z2_incl;
        double xcorr_D6T_incl, ycorr_D6T_incl;
        double xcorr_Z2_jecP_incl, ycorr_Z2_jecP_incl;
        double xcorr_Z2_jecM_incl, ycorr_Z2_jecM_incl;
        double JEChigh, JEClow;
        
        for(int i=0;i<bin_incl;i++){
        
        xcorr_Z2_incl=0.; ycorr_Z2_incl=0.;
        xcorr_D6T_incl=0.; ycorr_D6T_incl=0.;
        xcorr_Z2_jecP_incl=0.; ycorr_Z2_jecP_incl=0.;
        xcorr_Z2_jecM_incl=0.; ycorr_Z2_jecM_incl=0.;
        JEChigh=0.; JEClow=0.;
        
        finrep<<endl<<"-----------> Bin n. "<<i<<", Jet# >= "<<i<<" <-----------"<<endl<<endl;
        
        INCLCorrMult_Z2->GetPoint(i, xcorr_Z2_incl, ycorr_Z2_incl);
        INCLCorrMult_D6T->GetPoint(i, xcorr_D6T_incl, ycorr_D6T_incl);
        
        finrep<<"Z2 Unf. Multiplicity =		"<<ycorr_Z2_incl<<endl<<endl;
        
        finrep<<"UNFOLDING SYSTEMATICS:"<<endl<<endl;
                     
        finrep<<"Unf. Syst. Z2-D6T =		"<<ycorr_Z2_incl-ycorr_D6T_incl<<endl;
        finrep<<"RELATIVE Unf. Syst. =		"<<TMath::Abs(ycorr_Z2_incl-ycorr_D6T_incl)/ycorr_Z2_incl<<endl<<endl;
        
        finrep<<"JEC SYSTEMATICS:"<<endl<<endl;
        
        INCLCorrMult_Z2_jecPlus->GetPoint(i, xcorr_Z2_jecP_incl, ycorr_Z2_jecP_incl);
        INCLCorrMult_Z2_jecMinus->GetPoint(i, xcorr_Z2_jecM_incl, ycorr_Z2_jecM_incl);
        
        JEChigh = (ycorr_Z2_incl-ycorr_Z2_jecP_incl)<0 ? TMath::Abs(ycorr_Z2_incl-ycorr_Z2_jecP_incl) : TMath::Abs(ycorr_Z2_incl-ycorr_Z2_jecM_incl);
        
        JEClow = (ycorr_Z2_incl-ycorr_Z2_jecP_incl)>0 ? (ycorr_Z2_incl-ycorr_Z2_jecP_incl) : (ycorr_Z2_incl-ycorr_Z2_jecM_incl);
        
        finrep<<"JEC Systematics =	(High)	"<<JEChigh<<"	(Low)	"<<JEClow<<endl;
        finrep<<"RELATIVE JEC Syst. =	(High)	"<<JEChigh/ycorr_Z2_incl<<"	(Low)	"<<JEClow/ycorr_Z2_incl<<endl<<endl;
        
        INCLUnfoldingSyst_graph->SetPoint(i, i, ycorr_Z2_incl);
        if((ycorr_Z2_incl-ycorr_D6T_incl)>0)INCLUnfoldingSyst_graph->SetPointEYlow(i, TMath::Abs(ycorr_Z2_incl-ycorr_D6T_incl));
        if((ycorr_Z2_incl-ycorr_D6T_incl)<0)INCLUnfoldingSyst_graph->SetPointEYhigh(i, TMath::Abs(ycorr_Z2_incl-ycorr_D6T_incl));
        INCLUnfoldingSyst_graph->SetPointEXhigh(i, 0.5);
        INCLUnfoldingSyst_graph->SetPointEXlow(i, 0.5);
        
        INCLJECSyst_graph->SetPoint(i, i, ycorr_Z2_incl);
        INCLJECSyst_graph->SetPointEYhigh(i, JEChigh);
        INCLJECSyst_graph->SetPointEYlow(i, JEClow);
        INCLJECSyst_graph->SetPointEXhigh(i, 0.5);
        INCLJECSyst_graph->SetPointEXlow(i, 0.5);
              
        }
        
        ///////////////// Make plots ////////////////////
        
        UnfSystematicsDir->cd();
        
        TCanvas *INCLUnfSyst = new TCanvas; 
        INCLUnfSyst->SetLogy();    
        INCLUnfoldingSyst_graph->SetLineWidth(2);
        INCLUnfoldingSyst_graph->SetMarkerStyle(23);
        INCLUnfoldingSyst_graph->GetXaxis()->SetTitle("incl. multiplicity");
        INCLUnfoldingSyst_graph->Draw("AP");  
	INCLCorrMult_Z2->SetTitle("Z2 Mult. with Unfolding Systematics");                                  
        INCLUnfSyst->Write("INCLUnfSyst.root");
        INCLUnfSyst->Close();
        
        JECSystematicsDir->cd();
        
        TCanvas *INCLJECSyst = new TCanvas; 
        INCLJECSyst->SetLogy();    
        INCLJECSyst_graph->SetLineWidth(2);
        INCLJECSyst_graph->SetMarkerStyle(23);
        INCLJECSyst_graph->GetXaxis()->SetTitle("incl. multiplicity");
        INCLJECSyst_graph->Draw("AP");  
	INCLCorrMult_Z2->SetTitle("Z2 Mult. with JEC Systematics");                                  
        INCLJECSyst->Write("INCLJECSyst.root");
        INCLJECSyst->Close();
 
        finrep.close();

        outplots->Write();
        outplots->Close();

}
