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

void XSecComp(int JetPtMin){

        string JetPtCut;
	if(JetPtMin==15)JetPtCut="JetPt15";
	if(JetPtMin==30)JetPtCut="JetPt30";

        //Output
        string out = "XSecComp_";
        out+=JetPtCut;
        string output = out;
        output+=".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
        
        TDirectory *UnfMultCompDir, *EffMultCompDir;
        UnfMultCompDir = outplots->mkdir("UnfMultComp");
        EffMultCompDir = outplots->mkdir("EffMultComp");

        string path = "/data/sfrosali/Zjets/CMSSW_3_9_9/src/Firenze/PATAnalysis/bin";
        
        //Z2 Tune       
        TFile *Z2_XSec = TFile::Open((path+"/XSec/"+JetPtCut+"/Z2/XSecMeas_"+JetPtCut+".root").c_str());
        
        //D6T Tune
        TFile *D6T_XSec = TFile::Open((path+"/XSec/"+JetPtCut+"/D6T/XSecMeas_"+JetPtCut+".root").c_str());

        ////////////////// Get Histograms - Z2
        
        UnfMultCompDir->cd();
        
        TGraphAsymmErrors* EXCLCorrMult_Z2 = (TGraphAsymmErrors*) Z2_XSec->Get("CorrMultiplicity/UnfoldedMult/EXCLCorrMult_graph");
        EXCLCorrMult_Z2->Write("EXCLCorrMult_Z2");
        TGraphAsymmErrors* INCLCorrMult_Z2 = (TGraphAsymmErrors*) Z2_XSec->Get("CorrMultiplicity/UnfoldedMult/INCLCorrMult_graph");
        INCLCorrMult_Z2->Write("INCLCorrMult_Z2");
        
        EffMultCompDir->cd();
        
        TGraphAsymmErrors* EXCLEffCorrMult_Z2 = (TGraphAsymmErrors*) Z2_XSec->Get("CorrMultiplicity/EffCorrMult/EXCLEffCorrMult_graph");
        EXCLCorrMult_Z2->Write("EXCLEffCorrMult_Z2");
        TGraphAsymmErrors* INCLEffCorrMult_Z2 = (TGraphAsymmErrors*) Z2_XSec->Get("CorrMultiplicity/EffCorrMult/INCLEffCorrMult_graph");
        INCLCorrMult_Z2->Write("INCLEffCorrMult_Z2");
        
        ////////////////// Get Histograms - D6T
        
        UnfMultCompDir->cd();
        
        TGraphAsymmErrors* EXCLCorrMult_D6T = (TGraphAsymmErrors*) D6T_XSec->Get("CorrMultiplicity/UnfoldedMult/EXCLCorrMult_graph");
        EXCLCorrMult_D6T->Write("EXCLCorrMult_D6T");
        TGraphAsymmErrors* INCLCorrMult_D6T = (TGraphAsymmErrors*) D6T_XSec->Get("CorrMultiplicity/UnfoldedMult/INCLCorrMult_graph");
        INCLCorrMult_D6T->Write("INCLCorrMult_D6T");
        
        EffMultCompDir->cd();
        
        TGraphAsymmErrors* EXCLEffCorrMult_D6T = (TGraphAsymmErrors*) D6T_XSec->Get("CorrMultiplicity/EffCorrMult/EXCLEffCorrMult_graph");
        EXCLCorrMult_D6T->Write("EXCLEffCorrMult_D6T");
        TGraphAsymmErrors* INCLEffCorrMult_D6T = (TGraphAsymmErrors*) D6T_XSec->Get("CorrMultiplicity/EffCorrMult/INCLEffCorrMult_graph");
        INCLCorrMult_D6T->Write("INCLEffCorrMult_D6T");
        
        ///////////////// Make plots ////////////////////
        
        UnfMultCompDir->cd();
        
        TCanvas *EXCLUnfComp = new TCanvas; 
        EXCLUnfComp->SetLogy();    
        EXCLCorrMult_Z2->SetLineColor(2);
        EXCLCorrMult_Z2->SetLineWidth(2);
        EXCLCorrMult_Z2->SetMarkerStyle(23);
        EXCLCorrMult_Z2->SetMarkerColor(2);
        EXCLCorrMult_Z2->GetXaxis()->SetTitle("excl. multiplicity");
        EXCLCorrMult_Z2->Draw("AP");  
	EXCLCorrMult_Z2->SetTitle("Z2 Mult. (red) - D6T Mult. (black)");
	EXCLCorrMult_D6T->SetLineWidth(2);
	EXCLCorrMult_D6T->SetMarkerStyle(22);
        EXCLCorrMult_D6T->Draw("PSAME");                                   
        EXCLUnfComp->Write("EXCLUnfComp.root");
        EXCLUnfComp->Close();
        
        TCanvas *INCLUnfComp = new TCanvas; 
        INCLUnfComp->SetLogy();    
        INCLCorrMult_Z2->SetLineColor(2);
        INCLCorrMult_Z2->SetLineWidth(2);
        INCLCorrMult_Z2->SetMarkerStyle(23);
        INCLCorrMult_Z2->SetMarkerColor(2);
        INCLCorrMult_Z2->GetXaxis()->SetTitle("incl. multiplicity");
        INCLCorrMult_Z2->Draw("AP");  
	INCLCorrMult_Z2->SetTitle("Z2 Mult. (red) - D6T Mult. (black)");
	INCLCorrMult_D6T->SetLineWidth(2);
	INCLCorrMult_D6T->SetMarkerStyle(22);
        INCLCorrMult_D6T->Draw("PSAME");                                   
        INCLUnfComp->Write("INCLUnfComp.root");
        INCLUnfComp->Close();
        
        EffMultCompDir->cd();

        TCanvas *EXCLEffComp = new TCanvas; 
        EXCLEffComp->SetLogy();    
        EXCLEffCorrMult_Z2->SetLineColor(2);
        EXCLEffCorrMult_Z2->SetLineWidth(2);
        EXCLEffCorrMult_Z2->SetMarkerStyle(23);
        EXCLEffCorrMult_Z2->SetMarkerColor(2);
        EXCLEffCorrMult_Z2->GetXaxis()->SetTitle("excl. multiplicity");
        EXCLEffCorrMult_Z2->Draw("AP");  
	EXCLEffCorrMult_Z2->SetTitle("Z2 Mult. (red) - D6T Mult. (black)");
	EXCLEffCorrMult_D6T->SetLineWidth(2);
	EXCLEffCorrMult_D6T->SetMarkerStyle(22);
        EXCLEffCorrMult_D6T->Draw("PSAME");                                   
        EXCLEffComp->Write("EXCLEffComp.root");
        EXCLEffComp->Close();
        
        TCanvas *INCLEffComp = new TCanvas; 
        INCLEffComp->SetLogy();    
        INCLEffCorrMult_Z2->SetLineColor(2);
        INCLEffCorrMult_Z2->SetLineWidth(2);
        INCLEffCorrMult_Z2->SetMarkerStyle(23);
        INCLEffCorrMult_Z2->SetMarkerColor(2);
        INCLEffCorrMult_Z2->GetXaxis()->SetTitle("incl. multiplicity");
        INCLEffCorrMult_Z2->Draw("AP");  
	INCLEffCorrMult_Z2->SetTitle("Z2 Mult. (red) - D6T Mult. (black)");
	INCLEffCorrMult_D6T->SetLineWidth(2);
	INCLEffCorrMult_D6T->SetMarkerStyle(22);
        INCLEffCorrMult_D6T->Draw("PSAME");                                   
        INCLEffComp->Write("INCLEffComp.root");
        INCLEffComp->Close();

        outplots->Write();
        outplots->Close();

}
