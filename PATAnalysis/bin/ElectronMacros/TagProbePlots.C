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
#include "TLine.h"
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

//multiplicity: "Excl" = exclusive multiplicity; "Incl" = inclusive multiplicity
//Selections: "SYM" = V+jets selections (old); "ASYM" = Vector Boson Task Force (new)

void TPPlots(string multiplicity, string selections){

        gROOT->SetStyle("Plain");
        
        //MonteCarlo Signal file used to evaluate the Rel. MC Rel Eff. and the TP Eff. w/o background
        TFile *signal_MC_file = TFile::Open("../../../MC_Winter10_399/Z_Madgraph_L1FastJet_JetPt30_399.root");
        
        //TP files (outputs of TagAndProbeAnalyzer) whit the mass fits
        TFile *tp_123 = TFile::Open("DATA_Sig-CB_BW-Back-Exp_Imp.root");
        TFile *tp_1234 = TFile::Open("DATA_Sig-CB_BW-Back-Exp_Conv.root");
        TFile *tp_12345 = TFile::Open("DATA_Sig-CB_BW-Back-Exp_Iso.root");
        TFile *tp_123456 = TFile::Open("DATA_Sig-CB_BW-Back-Exp_EiD_NoTrBack.root");
        
        //Output
        string out = "TPStudy_TEST";
        string output = out;
        output+=".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
        
        ofstream tpr;
	string outtpr="TagProbeReport_";
	outtpr+=out;
	outtpr+=".txt";
	tpr.open(outtpr.c_str());
	tpr<<endl;
        
        double xmin, xmax;
        
        xmin=-0.5;
        xmax=3.5;
        int cut_decimal = 10000;
	
	int _Acc  = 1;
	int _Trg  = 2;	
	int _Imp  = 3;
	int _Conv = 4;
	int _Iso  = 5;
	int _EiD  = 6;
	
	string _RecoCutFlags[7];
	
	for(int i=0; i<7; i++){
       _RecoCutFlags[i] = "_1";}
   
       if(selections=="SYM"){
       _RecoCutFlags[_Acc] =  "_AccSYM";
       _RecoCutFlags[_Conv] = "_ConvSYM";
       _RecoCutFlags[_Iso] =  "_IsoSYM";
       _RecoCutFlags[_EiD] =  "_EiDSYM";}
       if(selections=="ASYM"){
       _RecoCutFlags[_Acc] =  "_AccASYM";
       _RecoCutFlags[_Conv] = "_ConvASYM";
       _RecoCutFlags[_Iso] =  "_IsoASYM";
       _RecoCutFlags[_EiD] =  "_EiDASYM";}
     
       _RecoCutFlags[_Trg] =  "_Trg";     
       _RecoCutFlags[_Imp] =  "_Imp";
    
        if(!signal_MC_file){
	cout<<"Error! Input files doesn't exist!"<<endl;
	return;
	}

TDirectory *REff, *R_Jet_Eff, *TP, *TP_12, *TP_123, *TP_1234, *TP_12345, *TP_123456;

REff      = outplots->mkdir("Relative_Efficiency");
R_Jet_Eff = REff->mkdir("Relative_Efficiency_vs_GenJet");
TP          = outplots->mkdir("Tag&Probe");
string TPdir_name = "Tag&Probe";
TPdir_name += _RecoCutFlags[1];
TPdir_name += _RecoCutFlags[2];
TP_12     = TP->mkdir(TPdir_name.c_str());
TPdir_name += _RecoCutFlags[3];
TP_123     = TP->mkdir(TPdir_name.c_str());
TPdir_name += _RecoCutFlags[4];
TP_1234     = TP->mkdir(TPdir_name.c_str());
TPdir_name += _RecoCutFlags[5];
TP_12345     = TP->mkdir(TPdir_name.c_str());
TPdir_name += _RecoCutFlags[6];
TP_123456     = TP->mkdir(TPdir_name.c_str());
	
   /////////////Efficiency - MC
   //Denominators
	
   TH1D* RelEffDenom_MC(0);
   
   if(multiplicity=="Excl")RelEffDenom_MC = (TH1D*) signal_MC_file->Get("EfficiencyElectron/EffDenom_RecoExclJetNumber");
   if(multiplicity=="Incl")RelEffDenom_MC = (TH1D*) signal_MC_file->Get("EfficiencyElectron/EffDenom_RecoInclJetNumber");

	string MCJetEff_name;
	if(multiplicity=="Excl")MCJetEff_name = "EfficiencyElectron/RecoExclJetEff";
	if(multiplicity=="Incl")MCJetEff_name = "EfficiencyElectron/RecoInclJetEff";
	
	MCJetEff_name+=_RecoCutFlags[1].c_str();
        TH1D* MCJetNumber_1 = (TH1D*) signal_MC_file->Get(MCJetEff_name.c_str());
	MCJetEff_name+=_RecoCutFlags[2].c_str();
        TH1D* MCJetNumber_12 = (TH1D*) signal_MC_file->Get(MCJetEff_name.c_str());
	MCJetEff_name+=_RecoCutFlags[3].c_str();
	TH1D* MCJetNumber_123 = (TH1D*) signal_MC_file->Get(MCJetEff_name.c_str());
	MCJetEff_name+=_RecoCutFlags[4].c_str();
	TH1D* MCJetNumber_1234 = (TH1D*) signal_MC_file->Get(MCJetEff_name.c_str());
	MCJetEff_name+=_RecoCutFlags[5].c_str();
	TH1D* MCJetNumber_12345 = (TH1D*) signal_MC_file->Get(MCJetEff_name.c_str());
	MCJetEff_name+=_RecoCutFlags[6].c_str();
	TH1D* MCJetNumber_123456 = (TH1D*) signal_MC_file->Get(MCJetEff_name.c_str());

        //Relative Efficiency from MC
             
        R_Jet_Eff->cd();
            
        TGraphAsymmErrors Eff_MC_Rel1(MCJetNumber_1, RelEffDenom_MC);
        TGraphAsymmErrors Eff_MC_Rel12(MCJetNumber_12, MCJetNumber_1);
        TGraphAsymmErrors Eff_MC_Rel123(MCJetNumber_123, MCJetNumber_12);
        TGraphAsymmErrors Eff_MC_Rel1234(MCJetNumber_1234, MCJetNumber_123);
        TGraphAsymmErrors Eff_MC_Rel12345(MCJetNumber_12345, MCJetNumber_1234);
        TGraphAsymmErrors Eff_MC_Rel123456(MCJetNumber_123456, MCJetNumber_12345);
      
        string Eff_MC_Relname="RelativeEff_MC";
        Eff_MC_Relname+=_RecoCutFlags[1].c_str();
        Eff_MC_Rel1.SetNameTitle(Eff_MC_Relname.c_str(), Eff_MC_Relname.c_str());
        Eff_MC_Relname+=_RecoCutFlags[2].c_str();  
        Eff_MC_Rel12.SetNameTitle(Eff_MC_Relname.c_str(), Eff_MC_Relname.c_str());
        Eff_MC_Relname+=_RecoCutFlags[3].c_str();  
        Eff_MC_Rel123.SetNameTitle(Eff_MC_Relname.c_str(), Eff_MC_Relname.c_str());
        Eff_MC_Relname+=_RecoCutFlags[4].c_str();  
        Eff_MC_Rel1234.SetNameTitle(Eff_MC_Relname.c_str(), Eff_MC_Relname.c_str());
        Eff_MC_Relname+=_RecoCutFlags[5].c_str();  
        Eff_MC_Rel12345.SetNameTitle(Eff_MC_Relname.c_str(), Eff_MC_Relname.c_str());
        Eff_MC_Relname+=_RecoCutFlags[6].c_str();  
        Eff_MC_Rel123456.SetNameTitle(Eff_MC_Relname.c_str(), Eff_MC_Relname.c_str());
        
        Eff_MC_Rel1.Write();
        Eff_MC_Rel12.Write();
        Eff_MC_Rel123.Write();
        Eff_MC_Rel1234.Write();
        Eff_MC_Rel123456.Write();
        
	TCanvas *REffJet = new TCanvas;
	Eff_MC_Rel1.GetYaxis()->SetRangeUser(0.3,1);
	Eff_MC_Rel1.GetXaxis()->SetTitle("gen Jet number");
	Eff_MC_Rel1.SetMarkerStyle(20);
	Eff_MC_Rel1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	Eff_MC_Rel12.SetLineColor(2);
	Eff_MC_Rel12.SetMarkerStyle(21);
	Eff_MC_Rel12.SetMarkerColor(2);
	Eff_MC_Rel12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	Eff_MC_Rel123.SetLineColor(3);
	Eff_MC_Rel123.SetMarkerStyle(22);
	Eff_MC_Rel123.SetMarkerColor(3);
        Eff_MC_Rel123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	Eff_MC_Rel1234.SetLineColor(4);
	Eff_MC_Rel1234.SetMarkerStyle(23);
	Eff_MC_Rel1234.SetMarkerColor(4);
	Eff_MC_Rel1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
        Eff_MC_Rel12345.SetLineColor(5);
	Eff_MC_Rel12345.SetMarkerStyle(24);
	Eff_MC_Rel12345.SetMarkerColor(5);
	Eff_MC_Rel12345.Draw("PSAME");}
	if(_RecoCutFlags[6] != "_1"){
        Eff_MC_Rel123456.SetLineColor(6);
	Eff_MC_Rel123456.SetMarkerStyle(25);
	Eff_MC_Rel123456.SetMarkerColor(6);
	Eff_MC_Rel123456.Draw("PSAME");}
	TLegend *LegREffJ = REffJet->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegREffJ->Draw();
	Eff_MC_Rel1.SetTitle("Cut Relative Efficiency vs Gen Jet Number");
	REffJet->Write("Cut_RelativeEfficiency_VsGenJet.root");
	REffJet->Close();
	
////////////////////////////////////////////////////////////////////////////////////	
	
        //Tag And Probe
        
        //Get Tag&Probe histograms
        
        string TPDir="EfficiencyElectron/Tag&Probe";
        string TPHisto="/Electron";
        
        if(selections=="ASYM")TPDir="EfficiencyElectron/Tag&Probe0";
 
        TPDir+=_RecoCutFlags[1].c_str();   
        TPDir+=_RecoCutFlags[2].c_str();
        TPHisto+=_RecoCutFlags[1].c_str();
        TPHisto+=_RecoCutFlags[2].c_str();
       
        string num_TP = TPDir+TPHisto+"_SingleCandTag&Probe_numerator";
        string den_TP = TPDir+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator_12 = (TH1D*) signal_MC_file->Get(num_TP.c_str());
        TH1D* TP_denominator_12 = (TH1D*) signal_MC_file->Get(den_TP.c_str());
        
        TPDir+=_RecoCutFlags[3].c_str();    
        TPHisto+=_RecoCutFlags[3].c_str(); 
        num_TP = TPDir+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP = TPDir+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator_123 = (TH1D*) signal_MC_file->Get(num_TP.c_str());
        TH1D* TP_denominator_123 = (TH1D*) signal_MC_file->Get(den_TP.c_str());
        
        TPDir+=_RecoCutFlags[4].c_str();    
        TPHisto+=_RecoCutFlags[4].c_str(); 
        num_TP = TPDir+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP = TPDir+TPHisto+"_SingleCandTag&Probe_denominator";
       
        TH1D* TP_numerator_1234 = (TH1D*) signal_MC_file->Get(num_TP.c_str());
        TH1D* TP_denominator_1234 = (TH1D*) signal_MC_file->Get(den_TP.c_str());
        
        TPDir+=_RecoCutFlags[5].c_str();    
        TPHisto+=_RecoCutFlags[5].c_str(); 
        num_TP = TPDir+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP = TPDir+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator_12345 = (TH1D*) signal_MC_file->Get(num_TP.c_str());
        TH1D* TP_denominator_12345 = (TH1D*) signal_MC_file->Get(den_TP.c_str());
        
        TPDir+=_RecoCutFlags[6].c_str();    
        TPHisto+=_RecoCutFlags[6].c_str(); 
        num_TP = TPDir+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP = TPDir+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator_123456 = (TH1D*) signal_MC_file->Get(num_TP.c_str());
        TH1D* TP_denominator_123456 = (TH1D*) signal_MC_file->Get(den_TP.c_str());
        
        TGraphAsymmErrors Eff_TP_Single_12(TP_numerator_12, TP_denominator_12);       
        TGraphAsymmErrors Eff_TP_Single_123(TP_numerator_123, TP_denominator_123);      
        TGraphAsymmErrors Eff_TP_Single_1234(TP_numerator_1234, TP_denominator_1234);        
        TGraphAsymmErrors Eff_TP_Single_12345(TP_numerator_12345, TP_denominator_12345);
        TGraphAsymmErrors Eff_TP_Single_123456(TP_numerator_123456, TP_denominator_123456);
        
        string Eff_TP_Single_name="SingleEff_TagProbe";
        if(selections=="ASYM")Eff_TP_Single_name="SingleEff_TagProbe0";
        Eff_TP_Single_name+=_RecoCutFlags[1].c_str();
        Eff_TP_Single_name+=_RecoCutFlags[2].c_str();
        Eff_TP_Single_12.SetNameTitle(Eff_TP_Single_name.c_str(), Eff_TP_Single_name.c_str());
        Eff_TP_Single_name+=_RecoCutFlags[3].c_str();
        Eff_TP_Single_123.SetNameTitle(Eff_TP_Single_name.c_str(), Eff_TP_Single_name.c_str());
        Eff_TP_Single_name+=_RecoCutFlags[4].c_str();
        Eff_TP_Single_1234.SetNameTitle(Eff_TP_Single_name.c_str(), Eff_TP_Single_name.c_str());
        Eff_TP_Single_name+=_RecoCutFlags[5].c_str();
        Eff_TP_Single_12345.SetNameTitle(Eff_TP_Single_name.c_str(), Eff_TP_Single_name.c_str());
        Eff_TP_Single_name+=_RecoCutFlags[6].c_str();
        Eff_TP_Single_123456.SetNameTitle(Eff_TP_Single_name.c_str(), Eff_TP_Single_name.c_str());
        
        string TPDir1="EfficiencyElectron/Tag&Probe1";;
        TGraphAsymmErrors Eff_TP_Single1_12, Eff_TP_Single1_123, Eff_TP_Single1_1234, Eff_TP_Single1_12345, Eff_TP_Single1_123456;
         
        if(selections=="ASYM"){
        
        TPHisto="/Electron";
        
        TPDir1+=_RecoCutFlags[1].c_str();   
        TPDir1+=_RecoCutFlags[2].c_str();
        TPHisto+=_RecoCutFlags[1].c_str();
        TPHisto+=_RecoCutFlags[2].c_str();
       
        string num_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_numerator";
        string den_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator1_12 = (TH1D*) signal_MC_file->Get(num_TP1.c_str());
        TH1D* TP_denominator1_12 = (TH1D*) signal_MC_file->Get(den_TP1.c_str());
        
        TPDir1+=_RecoCutFlags[3].c_str();    
        TPHisto+=_RecoCutFlags[3].c_str(); 
        num_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator1_123 = (TH1D*) signal_MC_file->Get(num_TP1.c_str());
        TH1D* TP_denominator1_123 = (TH1D*) signal_MC_file->Get(den_TP1.c_str());
        
        TPDir1+=_RecoCutFlags[4].c_str();    
        TPHisto+=_RecoCutFlags[4].c_str(); 
        num_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_denominator";
       
        TH1D* TP_numerator1_1234 = (TH1D*) signal_MC_file->Get(num_TP1.c_str());
        TH1D* TP_denominator1_1234 = (TH1D*) signal_MC_file->Get(den_TP1.c_str());
        
        TPDir1+=_RecoCutFlags[5].c_str();    
        TPHisto+=_RecoCutFlags[5].c_str(); 
        num_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator1_12345 = (TH1D*) signal_MC_file->Get(num_TP1.c_str());
        TH1D* TP_denominator1_12345 = (TH1D*) signal_MC_file->Get(den_TP1.c_str());
        
        TPDir1+=_RecoCutFlags[6].c_str();    
        TPHisto+=_RecoCutFlags[6].c_str(); 
        num_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator1_123456 = (TH1D*) signal_MC_file->Get(num_TP1.c_str());
        TH1D* TP_denominator1_123456 = (TH1D*) signal_MC_file->Get(den_TP1.c_str());
        
        Eff_TP_Single1_12.BayesDivide(TP_numerator1_12, TP_denominator1_12);       
        Eff_TP_Single1_123.BayesDivide(TP_numerator1_123, TP_denominator1_123);      
        Eff_TP_Single1_1234.BayesDivide(TP_numerator1_1234, TP_denominator1_1234);        
        Eff_TP_Single1_12345.BayesDivide(TP_numerator1_12345, TP_denominator1_12345);
        Eff_TP_Single1_123456.BayesDivide(TP_numerator1_123456, TP_denominator1_123456);
        
        string Eff_TP_Single1_name="SingleEff_TagProbe1";
        Eff_TP_Single1_name+=_RecoCutFlags[1].c_str();
        Eff_TP_Single1_name+=_RecoCutFlags[2].c_str();
        Eff_TP_Single1_12.SetNameTitle(Eff_TP_Single1_name.c_str(), Eff_TP_Single1_name.c_str());
        Eff_TP_Single1_name+=_RecoCutFlags[3].c_str();
        Eff_TP_Single1_123.SetNameTitle(Eff_TP_Single1_name.c_str(), Eff_TP_Single1_name.c_str());
        Eff_TP_Single1_name+=_RecoCutFlags[4].c_str();
        Eff_TP_Single1_1234.SetNameTitle(Eff_TP_Single1_name.c_str(), Eff_TP_Single1_name.c_str());
        Eff_TP_Single1_name+=_RecoCutFlags[5].c_str();
        Eff_TP_Single1_12345.SetNameTitle(Eff_TP_Single1_name.c_str(), Eff_TP_Single1_name.c_str());
        Eff_TP_Single1_name+=_RecoCutFlags[6].c_str();
        Eff_TP_Single1_123456.SetNameTitle(Eff_TP_Single1_name.c_str(), Eff_TP_Single1_name.c_str());
        
        }
        
        //Get TP w back double eff.
        
        TGraphAsymmErrors* tp_eff_123 = (TGraphAsymmErrors*) tp_123->Get("FitDoubleTag&Probe");
        TGraphAsymmErrors* tp_eff_1234 = (TGraphAsymmErrors*) tp_1234->Get("FitDoubleTag&Probe");
        TGraphAsymmErrors* tp_eff_12345 = (TGraphAsymmErrors*) tp_12345->Get("FitDoubleTag&Probe");
        TGraphAsymmErrors* tp_eff_123456 = (TGraphAsymmErrors*) tp_123456->Get("FitDoubleTag&Probe");
      
        //////////////////////////////////////////////////7
                
        int n_12 = Eff_TP_Single_12.GetN();
        TVectorD vx_12(n_12);
        TVectorD vy_12(n_12);
        TVectorD vexl_12(n_12);
        TVectorD vexh_12(n_12);
        TVectorD veyl_12(n_12);
        TVectorD veyh_12(n_12);  
        for ( int i = 0; i < n_12; ++i ){
        double x = 0., y = 0., x1=0., y1=0.;
        Eff_TP_Single_12.GetPoint(i, x, y);
        if(selections=="ASYM")Eff_TP_Single1_12.GetPoint(i, x1, y1);
        vx_12(i)   = x;
        vexl_12(i) = Eff_TP_Single_12.GetErrorXlow(i);
        vexh_12(i) = Eff_TP_Single_12.GetErrorXhigh(i);
        if(selections=="SYM")vy_12(i)   = y * y;
        if(selections=="ASYM")vy_12(i)   = y * y1;
        if(selections=="SYM")veyl_12(i) = 2 * y * Eff_TP_Single_12.GetErrorYlow(i);
        if(selections=="ASYM")veyl_12(i) = TMath::Sqrt(pow(y1*Eff_TP_Single_12.GetErrorYlow(i),2)+pow(y*Eff_TP_Single1_12.GetErrorYlow(i),2));
        if(selections=="SYM")veyh_12(i) = 2 * y * Eff_TP_Single_12.GetErrorYhigh(i);
        if(selections=="ASYM")veyh_12(i) = TMath::Sqrt(pow(y1*Eff_TP_Single_12.GetErrorYhigh(i),2)+pow(y*Eff_TP_Single1_12.GetErrorYhigh(i),2));
        }  
        
        int n_123 = Eff_TP_Single_123.GetN();
        TVectorD vx_123(n_123);
        TVectorD vy_123(n_123);
        TVectorD vexl_123(n_123);
        TVectorD vexh_123(n_123);
        TVectorD veyl_123(n_123);
        TVectorD veyh_123(n_123);
        for ( int i = 0; i < n_123; ++i ){
        double x = 0., y = 0., x1=0., y1=0.;
        Eff_TP_Single_123.GetPoint(i, x, y);
        if(selections=="ASYM")Eff_TP_Single1_123.GetPoint(i, x1, y1);
        vx_123(i)   = x;
        vexl_123(i) = Eff_TP_Single_123.GetErrorXlow(i);
        vexh_123(i) = Eff_TP_Single_123.GetErrorXhigh(i);
        if(selections=="SYM")vy_123(i)   = y * y;
        if(selections=="ASYM")vy_123(i)   = y * y1;
        if(selections=="SYM")veyl_123(i) = 2 * y * Eff_TP_Single_123.GetErrorYlow(i);
        if(selections=="ASYM")veyl_123(i) = TMath::Sqrt(pow(y1*Eff_TP_Single_123.GetErrorYlow(i),2)+pow(y*Eff_TP_Single1_123.GetErrorYlow(i),2));
        if(selections=="SYM")veyh_123(i) = 2 * y * Eff_TP_Single_123.GetErrorYhigh(i);
        if(selections=="ASYM")veyh_123(i) = TMath::Sqrt(pow(y1*Eff_TP_Single_123.GetErrorYhigh(i),2)+pow(y*Eff_TP_Single1_123.GetErrorYhigh(i),2));
        }
        
        int n_1234 = Eff_TP_Single_1234.GetN();
        TVectorD vx_1234(n_1234);
        TVectorD vy_1234(n_1234);
        TVectorD vexl_1234(n_1234);
        TVectorD vexh_1234(n_1234);
        TVectorD veyl_1234(n_1234);
        TVectorD veyh_1234(n_1234);
        for ( int i = 0; i < n_1234; ++i ){
        double x = 0., y = 0., x1=0., y1=0.;
        Eff_TP_Single_1234.GetPoint(i, x, y);
        if(selections=="ASYM")Eff_TP_Single1_1234.GetPoint(i, x1, y1);
        vx_1234(i)   = x;
        vexl_1234(i) = Eff_TP_Single_1234.GetErrorXlow(i);
        vexh_1234(i) = Eff_TP_Single_1234.GetErrorXhigh(i);
        if(selections=="SYM")vy_1234(i)   = y * y;
        if(selections=="ASYM")vy_1234(i)   = y * y1;
        if(selections=="SYM")veyl_1234(i) = 2 * y * Eff_TP_Single_1234.GetErrorYlow(i);
        if(selections=="ASYM")veyl_1234(i) = TMath::Sqrt(pow(y1*Eff_TP_Single_1234.GetErrorYlow(i),2)+pow(y*Eff_TP_Single1_1234.GetErrorYlow(i),2));
        if(selections=="SYM")veyh_1234(i) = 2 * y * Eff_TP_Single_1234.GetErrorYhigh(i);
        if(selections=="ASYM")veyh_1234(i) = TMath::Sqrt(pow(y1*Eff_TP_Single_1234.GetErrorYhigh(i),2)+pow(y*Eff_TP_Single1_1234.GetErrorYhigh(i),2));
        } 
        
        int n_12345 = Eff_TP_Single_12345.GetN();
        TVectorD vx_12345(n_12345);
        TVectorD vy_12345(n_12345);
        TVectorD vexl_12345(n_12345);
        TVectorD vexh_12345(n_12345);
        TVectorD veyl_12345(n_12345);
        TVectorD veyh_12345(n_12345);
        for ( int i = 0; i < n_12345; ++i ){
        double x = 0., y = 0., x1=0., y1=0.;
        Eff_TP_Single_12345.GetPoint(i, x, y);
        if(selections=="ASYM")Eff_TP_Single1_12345.GetPoint(i, x1, y1);
        vx_12345(i)   = x;
        vexl_12345(i) = Eff_TP_Single_12345.GetErrorXlow(i);
        vexh_12345(i) = Eff_TP_Single_12345.GetErrorXhigh(i);
        if(selections=="SYM")vy_12345(i)   = y * y;
        if(selections=="ASYM")vy_12345(i)   = y * y1;
        if(selections=="SYM")veyl_12345(i) = 2 * y * Eff_TP_Single_12345.GetErrorYlow(i);
        if(selections=="ASYM")veyl_12345(i) = TMath::Sqrt(pow(y1*Eff_TP_Single_12345.GetErrorYlow(i),2)+pow(y*Eff_TP_Single1_12345.GetErrorYlow(i),2));
        if(selections=="SYM")veyh_12345(i) = 2 * y * Eff_TP_Single_12345.GetErrorYhigh(i);
        if(selections=="ASYM")veyh_12345(i) = TMath::Sqrt(pow(y1*Eff_TP_Single_12345.GetErrorYhigh(i),2)+pow(y*Eff_TP_Single1_12345.GetErrorYhigh(i),2));
        }
        
        int n_123456 = Eff_TP_Single_123456.GetN();
        TVectorD vx_123456(n_123456);
        TVectorD vy_123456(n_123456);
        TVectorD vexl_123456(n_123456);
        TVectorD vexh_123456(n_123456);
        TVectorD veyl_123456(n_123456);
        TVectorD veyh_123456(n_123456);
        for ( int i = 0; i < n_123456; ++i ){
        double x = 0., y = 0., x1=0., y1=0.;
        Eff_TP_Single_123456.GetPoint(i, x, y);
        if(selections=="ASYM")Eff_TP_Single1_123456.GetPoint(i, x1, y1);
        vx_123456(i)   = x;
        vexl_123456(i) = Eff_TP_Single_123456.GetErrorXlow(i);
        vexh_123456(i) = Eff_TP_Single_123456.GetErrorXhigh(i);
        if(selections=="SYM")vy_123456(i)   = y * y;
        if(selections=="ASYM")vy_123456(i)   = y * y1;
        if(selections=="SYM")veyl_123456(i) = 2 * y * Eff_TP_Single_123456.GetErrorYlow(i);
        if(selections=="ASYM")veyl_123456(i) = TMath::Sqrt(pow(y1*Eff_TP_Single_123456.GetErrorYlow(i),2)+pow(y*Eff_TP_Single1_123456.GetErrorYlow(i),2));
        if(selections=="SYM")veyh_123456(i) = 2 * y * Eff_TP_Single_123456.GetErrorYhigh(i);
        if(selections=="ASYM")veyh_123456(i) = TMath::Sqrt(pow(y1*Eff_TP_Single_123456.GetErrorYhigh(i),2)+pow(y*Eff_TP_Single1_123456.GetErrorYhigh(i),2));
        } 
    
        TGraphAsymmErrors Eff_TP_Double_12(vx_12, vy_12, vexl_12, vexh_12, veyl_12, veyh_12);
        TGraphAsymmErrors Eff_TP_Double_123(vx_123, vy_123, vexl_123, vexh_123, veyl_123, veyh_123);
        TGraphAsymmErrors Eff_TP_Double_1234(vx_1234, vy_1234, vexl_1234, vexh_1234, veyl_1234, veyh_1234);
        TGraphAsymmErrors Eff_TP_Double_12345(vx_12345, vy_12345, vexl_12345, vexh_12345, veyl_12345, veyh_12345);
        TGraphAsymmErrors Eff_TP_Double_123456(vx_123456, vy_123456, vexl_123456, vexh_123456, veyl_123456, veyh_123456);
        
        string Eff_TP_Double_name="DoubleEff_TagProbe";
        Eff_TP_Double_name+=_RecoCutFlags[1].c_str();
        Eff_TP_Double_name+=_RecoCutFlags[2].c_str();
        Eff_TP_Double_12.SetNameTitle(Eff_TP_Double_name.c_str(), Eff_TP_Double_name.c_str());
        Eff_TP_Double_name+=_RecoCutFlags[3].c_str();
        Eff_TP_Double_123.SetNameTitle(Eff_TP_Double_name.c_str(), Eff_TP_Double_name.c_str());
        Eff_TP_Double_name+=_RecoCutFlags[4].c_str();
        Eff_TP_Double_1234.SetNameTitle(Eff_TP_Double_name.c_str(), Eff_TP_Double_name.c_str());
        Eff_TP_Double_name+=_RecoCutFlags[5].c_str();
        Eff_TP_Double_12345.SetNameTitle(Eff_TP_Double_name.c_str(), Eff_TP_Double_name.c_str());
        Eff_TP_Double_name+=_RecoCutFlags[6].c_str();
        Eff_TP_Double_123456.SetNameTitle(Eff_TP_Double_name.c_str(), Eff_TP_Double_name.c_str());
        
        TP_12->cd();
        Eff_TP_Single_12.Write();
        Eff_TP_Double_12.Write();
        TCanvas *EffTP_12 = new TCanvas;
        Eff_TP_Double_12.SetLineColor(2);
        Eff_TP_Double_12.SetLineWidth(2);
	Eff_TP_Double_12.SetMarkerStyle(21);
	Eff_TP_Double_12.SetMarkerColor(2);
	Eff_TP_Double_12.Draw("AP");
	Eff_TP_Double_12.SetTitle("Cut eff: MC Rel Eff (black), MC TP w/o back (red), Data (blue)");
	Eff_TP_Double_12.GetXaxis()->SetRangeUser(xmin,xmax);
	Eff_TP_Double_12.GetYaxis()->SetRangeUser(0.3,1.05); 
        Eff_MC_Rel12.SetLineColor(1);
        Eff_MC_Rel12.SetLineWidth(2);
	Eff_MC_Rel12.SetMarkerStyle(20);
	Eff_MC_Rel12.SetMarkerColor(1);
	Eff_MC_Rel12.Draw("PSAME");	
	string EffTP_12_name = "TPEff";
	EffTP_12_name+=_RecoCutFlags[1].c_str();
        EffTP_12_name+=_RecoCutFlags[2].c_str();
        EffTP_12_name+=".root";
	EffTP_12->Write(EffTP_12_name.c_str());
	EffTP_12->Close();
	
	TP_123->cd();
        Eff_TP_Single_123.Write();
        Eff_TP_Double_123.Write();
        TCanvas *EffTP_123 = new TCanvas;
        Eff_TP_Double_123.SetLineColor(2);
	Eff_TP_Double_123.SetMarkerStyle(21);
	Eff_TP_Double_123.SetLineWidth(2);
	Eff_TP_Double_123.SetMarkerColor(2);
	Eff_TP_Double_123.Draw("AP");
	Eff_TP_Double_123.SetTitle("Cut eff: MC Rel Eff (black), MC TP w/o back (red), Data (blue)");
	Eff_TP_Double_123.GetXaxis()->SetRangeUser(xmin,xmax);
	Eff_TP_Double_123.GetYaxis()->SetRangeUser(0.3,1.05);
	tp_eff_123->SetLineColor(4);
	tp_eff_123->SetLineWidth(2);
	tp_eff_123->SetMarkerStyle(23);
	tp_eff_123->SetMarkerColor(4);
	tp_eff_123->DrawClone("PSAME");	
        Eff_MC_Rel123.SetLineColor(1);
        Eff_MC_Rel123.SetLineWidth(2);
	Eff_MC_Rel123.SetMarkerStyle(20);
	Eff_MC_Rel123.SetMarkerColor(1);
	Eff_MC_Rel123.Draw("PSAME");
	string EffTP_123_name = "TPEff";
	EffTP_123_name+=_RecoCutFlags[1].c_str();
        EffTP_123_name+=_RecoCutFlags[2].c_str();
        EffTP_123_name+=_RecoCutFlags[3].c_str();
        EffTP_123_name+=".root";
	EffTP_123->Write(EffTP_123_name.c_str());
	EffTP_123->Close();
	
	tpr<<"Relative efficiency vs "<<multiplicity<<" jet multiplicity"<<endl;
	
	tpr<<endl<<"Selection: "<<_RecoCutFlags[3]<<endl<<endl;
	for(int i=0; i<4; i++){
	double xMCeff = 0., yMCeff = 0.;
	double xMCSigTp = 0., yMCSigTp = 0.;
	double xDATA = 0., yDATA = 0.;
	Eff_MC_Rel123.GetPoint(i,xMCeff,yMCeff);
	Eff_TP_Double_123.GetPoint(i,xMCSigTp,yMCSigTp);
	tp_eff_123->GetPoint(i,xDATA,yDATA);
	tpr<<multiplicity<<" Mult. = "<<i<<"	MC eff. =	"<<((float)((int)(yMCeff*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(Eff_MC_Rel123.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(Eff_MC_Rel123.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		MC Sig TP =	"<<((float)((int)(yMCSigTp*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(Eff_TP_Double_123.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(Eff_TP_Double_123.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		DATA TP =	"<<((float)((int)(yDATA*cut_decimal)))/cut_decimal<<"	ErrorHigh = "<<((float)((int)(tp_eff_123->GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(tp_eff_123->GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl<<endl;}
		
	TP_1234->cd();
        Eff_TP_Single_1234.Write();
        Eff_TP_Double_1234.Write();
        TCanvas *EffTP_1234 = new TCanvas;
        Eff_TP_Double_1234.SetLineColor(2);
        Eff_TP_Double_1234.SetLineWidth(2);
	Eff_TP_Double_1234.SetMarkerStyle(21);
	Eff_TP_Double_1234.SetMarkerColor(2);
	Eff_TP_Double_1234.Draw("AP");
	tp_eff_1234->SetLineColor(4);
	tp_eff_1234->SetLineWidth(2);
	tp_eff_1234->SetMarkerStyle(23);
	tp_eff_1234->SetMarkerColor(4);
	tp_eff_1234->DrawClone("PSAME");	
	Eff_TP_Double_1234.SetTitle("Cut eff: MC Rel Eff (black), MC TP w/o back (red), Data (blue)");
	Eff_TP_Double_1234.GetXaxis()->SetRangeUser(xmin,xmax);
	Eff_TP_Double_1234.GetYaxis()->SetRangeUser(0.3,1.05);    
        Eff_MC_Rel1234.SetLineColor(1);
        Eff_MC_Rel1234.SetLineWidth(2);
	Eff_MC_Rel1234.SetMarkerStyle(20);
	Eff_MC_Rel1234.SetMarkerColor(1);
	Eff_MC_Rel1234.Draw("PSAME");
	string EffTP_1234_name = "TPEff";
	EffTP_1234_name+=_RecoCutFlags[1].c_str();
        EffTP_1234_name+=_RecoCutFlags[2].c_str();
        EffTP_1234_name+=_RecoCutFlags[3].c_str();
        EffTP_1234_name+=_RecoCutFlags[4].c_str();
        EffTP_1234_name+=".root";
	EffTP_1234->Write(EffTP_1234_name.c_str());
	EffTP_1234->Close();
	
	tpr<<endl<<endl<<"Selection: "<<_RecoCutFlags[4]<<endl<<endl;
	for(int i=0; i<4; i++){
	double xMCeff = 0., yMCeff = 0.;
	double xMCSigTp = 0., yMCSigTp = 0.;
	double xDATA = 0., yDATA = 0.;
	Eff_MC_Rel1234.GetPoint(i,xMCeff,yMCeff);
	Eff_TP_Double_1234.GetPoint(i,xMCSigTp,yMCSigTp);
	tp_eff_1234->GetPoint(i,xDATA,yDATA);
	tpr<<multiplicity<<" Mult. = "<<i<<"	MC eff. =	"<<((float)((int)(yMCeff*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(Eff_MC_Rel1234.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(Eff_MC_Rel1234.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		MC Sig TP =	"<<((float)((int)(yMCSigTp*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(Eff_TP_Double_1234.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(Eff_TP_Double_1234.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		DATA TP =	"<<((float)((int)(yDATA*cut_decimal)))/cut_decimal<<"	ErrorHigh = "<<((float)((int)(tp_eff_1234->GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(tp_eff_1234->GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl<<endl;}
	
	TP_12345->cd();
        Eff_TP_Single_12345.Write();
        Eff_TP_Double_12345.Write();
        TCanvas *EffTP_12345 = new TCanvas;
        Eff_TP_Double_12345.SetLineColor(2);
        Eff_TP_Double_12345.SetLineWidth(2);
	Eff_TP_Double_12345.SetMarkerStyle(21);
	Eff_TP_Double_12345.SetMarkerColor(2);
	Eff_TP_Double_12345.Draw("AP");
	tp_eff_12345->SetLineColor(4);
	tp_eff_12345->SetLineWidth(2);
	tp_eff_12345->SetMarkerStyle(23);
	tp_eff_12345->SetMarkerColor(4);
	tp_eff_12345->DrawClone("PSAME");	
	Eff_TP_Double_12345.SetTitle("Cut eff: MC Rel Eff (black), MC TP w/o back (red), Data (blue)");
	Eff_TP_Double_12345.GetXaxis()->SetRangeUser(xmin,xmax);
	Eff_TP_Double_12345.GetYaxis()->SetRangeUser(0.3,1.05);      
        Eff_MC_Rel12345.SetLineColor(1);
        Eff_MC_Rel12345.SetLineWidth(2);
	Eff_MC_Rel12345.SetMarkerStyle(20);
	Eff_MC_Rel12345.SetMarkerColor(1);
	Eff_MC_Rel12345.Draw("PSAME");
	string EffTP_12345_name = "TPEff";
	EffTP_12345_name+=_RecoCutFlags[1].c_str();
        EffTP_12345_name+=_RecoCutFlags[2].c_str();
        EffTP_12345_name+=_RecoCutFlags[3].c_str();
        EffTP_12345_name+=_RecoCutFlags[4].c_str();
        EffTP_12345_name+=_RecoCutFlags[5].c_str();
        EffTP_12345_name+=".root";
	EffTP_12345->Write(EffTP_12345_name.c_str());
	EffTP_12345->Close();
	
	tpr<<endl<<endl<<"Selection: "<<_RecoCutFlags[5]<<endl<<endl;
	for(int i=0; i<4; i++){
	double xMCeff = 0., yMCeff = 0.;
	double xMCSigTp = 0., yMCSigTp = 0.;
	double xDATA = 0., yDATA = 0.;
	Eff_MC_Rel12345.GetPoint(i,xMCeff,yMCeff);
	Eff_TP_Double_12345.GetPoint(i,xMCSigTp,yMCSigTp);
	tp_eff_12345->GetPoint(i,xDATA,yDATA);
	tpr<<multiplicity<<" Mult. = "<<i<<"	MC eff. =	"<<((float)((int)(yMCeff*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(Eff_MC_Rel12345.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(Eff_MC_Rel12345.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		MC Sig TP =	"<<((float)((int)(yMCSigTp*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(Eff_TP_Double_12345.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(Eff_TP_Double_12345.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		DATA TP =	"<<((float)((int)(yDATA*cut_decimal)))/cut_decimal<<"	ErrorHigh = "<<((float)((int)(tp_eff_12345->GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(tp_eff_12345->GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl<<endl;}
	
	TP_123456->cd();
        Eff_TP_Single_123456.Write();
        Eff_TP_Double_123456.Write();
        TCanvas *EffTP_123456 = new TCanvas;
        Eff_TP_Double_123456.SetLineColor(2);
        Eff_TP_Double_123456.SetLineWidth(2);
	Eff_TP_Double_123456.SetMarkerStyle(21);
	Eff_TP_Double_123456.SetMarkerColor(2);
	Eff_TP_Double_123456.Draw("AP");
	tp_eff_123456->SetLineColor(4);
	tp_eff_123456->SetLineWidth(2);
	tp_eff_123456->SetMarkerStyle(23);
	tp_eff_123456->SetMarkerColor(4);
	tp_eff_123456->DrawClone("PSAME");
	Eff_TP_Double_123456.SetTitle("Cut eff: MC Rel Eff (black), MC TP w/o back (red), Data (blue)");
	Eff_TP_Double_123456.GetXaxis()->SetRangeUser(xmin,xmax);
	Eff_TP_Double_123456.GetYaxis()->SetRangeUser(0.3,1.05);
        Eff_MC_Rel123456.SetLineColor(1);
        Eff_MC_Rel123456.SetLineWidth(2);
	Eff_MC_Rel123456.SetMarkerStyle(20);
	Eff_MC_Rel123456.SetMarkerColor(1);
	Eff_MC_Rel123456.Draw("PSAME");	
	string EffTP_123456_name = "TPEff";
	EffTP_123456_name+=_RecoCutFlags[1].c_str();
        EffTP_123456_name+=_RecoCutFlags[2].c_str();
        EffTP_123456_name+=_RecoCutFlags[3].c_str();
        EffTP_123456_name+=_RecoCutFlags[4].c_str();
        EffTP_123456_name+=_RecoCutFlags[5].c_str();
        EffTP_123456_name+=_RecoCutFlags[6].c_str();
        EffTP_123456_name+=".root";
	EffTP_123456->Write(EffTP_123456_name.c_str());
	EffTP_123456->Close();
	
	tpr<<endl<<endl<<"Selection: "<<_RecoCutFlags[6]<<endl<<endl;
	for(int i=0; i<4; i++){
	double xMCeff = 0., yMCeff = 0.;
	double xMCSigTp = 0., yMCSigTp = 0.;
	double xDATA = 0., yDATA = 0.;
	Eff_MC_Rel123456.GetPoint(i,xMCeff,yMCeff);
	Eff_TP_Double_123456.GetPoint(i,xMCSigTp,yMCSigTp);
	tp_eff_123456->GetPoint(i,xDATA,yDATA);
	tpr<<multiplicity<<" Mult. = "<<i<<"	MC eff. =	"<<((float)((int)(yMCeff*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(Eff_MC_Rel123456.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(Eff_MC_Rel123456.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		MC Sig TP =	"<<((float)((int)(yMCSigTp*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(Eff_TP_Double_123456.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(Eff_TP_Double_123456.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		DATA TP =	"<<((float)((int)(yDATA*cut_decimal)))/cut_decimal<<"	ErrorHigh = "<<((float)((int)(tp_eff_123456->GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(tp_eff_123456->GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl<<endl;}
	
/////////////////////////////////////////////////////////////////////////////////////////

// Tag&Probe residuals

        TVectorD res1vx_123(n_123);
        TVectorD res1vy_123(n_123);
        TVectorD res1vexl_123(n_123);
        TVectorD res1vexh_123(n_123);
        TVectorD res1veyl_123(n_123);
        TVectorD res1veyh_123(n_123);
        TVectorD res2vx_123(n_123);
        TVectorD res2vy_123(n_123);
        TVectorD res2vexl_123(n_123);
        TVectorD res2vexh_123(n_123);
        TVectorD res2veyl_123(n_123);
        TVectorD res2veyh_123(n_123);
        
        for ( int i = 0; i < n_123; ++i ){
        double x0 = 0., y0 = 0., x1 = 0., y1 = 0., x2 = 0., y2 = 0.;
        Eff_MC_Rel123.GetPoint(i, x0, y0);
        Eff_TP_Double_123.GetPoint(i, x1, y1);
        tp_eff_123->GetPoint(i, x2, y2);
        res1vx_123(i)   = i;
        res1vexl_123(i) = Eff_MC_Rel123.GetErrorXlow(i);
        res1vexh_123(i) = Eff_MC_Rel123.GetErrorXhigh(i);
        res1vy_123(i)   = y1-y0;
        res1veyl_123(i) = TMath::Sqrt(pow(Eff_MC_Rel123.GetErrorYlow(i),2)+pow(Eff_TP_Double_123.GetErrorYlow(i),2));
        res1veyh_123(i) = TMath::Sqrt(pow(Eff_MC_Rel123.GetErrorYhigh(i),2)+pow(Eff_TP_Double_123.GetErrorYhigh(i),2));
        res2vx_123(i)   = i;
        res2vexl_123(i) = Eff_MC_Rel123.GetErrorXlow(i);
        res2vexh_123(i) = Eff_MC_Rel123.GetErrorXhigh(i);
        res2vy_123(i)   = y2-y0;
        res2veyl_123(i) = TMath::Sqrt(pow(Eff_MC_Rel123.GetErrorYlow(i),2)+pow(tp_eff_123->GetErrorYlow(i),2));
        res2veyh_123(i) = TMath::Sqrt(pow(Eff_MC_Rel123.GetErrorYhigh(i),2)+pow(tp_eff_123->GetErrorYhigh(i),2));
        }
              
        TP_123->cd();
        TGraphAsymmErrors TP_Res1_123(res1vx_123, res1vy_123, res1vexl_123, res1vexh_123, res1veyl_123, res1veyh_123);
        TGraphAsymmErrors TP_Res2_123(res2vx_123, res2vy_123, res2vexl_123, res2vexh_123, res2veyl_123, res2veyh_123);
        TCanvas *ResTP_123 = new TCanvas;
        ResTP_123->SetGrid();
        TP_Res1_123.SetLineColor(2);
	TP_Res1_123.SetMarkerStyle(21);
	TP_Res1_123.SetLineWidth(2);
	TP_Res1_123.SetMarkerColor(2);
	TP_Res1_123.Draw("AP");
	TP_Res2_123.SetLineColor(4);
	TP_Res2_123.SetMarkerStyle(23);
	TP_Res2_123.SetLineWidth(2);
	TP_Res2_123.SetMarkerColor(4);
	TP_Res2_123.Draw("PSAME");
	TP_Res1_123.SetTitle("TP residuals from MC Rel Eff: MC TP w/o back (red), Data (blue)");
	TP_Res1_123.GetXaxis()->SetRangeUser(xmin,xmax);
	TP_Res1_123.GetYaxis()->SetRangeUser(-0.3,0.3);
	ResTP_123->Write("ResTP_Imp.root");
	ResTP_123->Close();
	
	TVectorD res1vx_1234(n_1234);
        TVectorD res1vy_1234(n_1234);
        TVectorD res1vexl_1234(n_1234);
        TVectorD res1vexh_1234(n_1234);
        TVectorD res1veyl_1234(n_1234);
        TVectorD res1veyh_1234(n_1234);
        TVectorD res2vx_1234(n_1234);
        TVectorD res2vy_1234(n_1234);
        TVectorD res2vexl_1234(n_1234);
        TVectorD res2vexh_1234(n_1234);
        TVectorD res2veyl_1234(n_1234);
        TVectorD res2veyh_1234(n_1234);
        
        for ( int i = 0; i < n_1234; ++i ){
        double x0 = 0., y0 = 0., x1 = 0., y1 = 0., x2 = 0., y2 = 0.;
        Eff_MC_Rel1234.GetPoint(i, x0, y0);
        Eff_TP_Double_1234.GetPoint(i, x1, y1);
        tp_eff_1234->GetPoint(i, x2, y2);
        res1vx_1234(i)   = i;
        res1vexl_1234(i) = Eff_MC_Rel1234.GetErrorXlow(i);
        res1vexh_1234(i) = Eff_MC_Rel1234.GetErrorXhigh(i);
        res1vy_1234(i)   = y1-y0;
        res1veyl_1234(i) = TMath::Sqrt(pow(Eff_MC_Rel1234.GetErrorYlow(i),2)+pow(Eff_TP_Double_1234.GetErrorYlow(i),2));
        res1veyh_1234(i) = TMath::Sqrt(pow(Eff_MC_Rel1234.GetErrorYhigh(i),2)+pow(Eff_TP_Double_1234.GetErrorYhigh(i),2));
        res2vx_1234(i)   = i;
        res2vexl_1234(i) = Eff_MC_Rel1234.GetErrorXlow(i);
        res2vexh_1234(i) = Eff_MC_Rel1234.GetErrorXhigh(i);
        res2vy_1234(i)   = y2-y0;
        res2veyl_1234(i) = TMath::Sqrt(pow(Eff_MC_Rel1234.GetErrorYlow(i),2)+pow(tp_eff_1234->GetErrorYlow(i),2));
        res2veyh_1234(i) = TMath::Sqrt(pow(Eff_MC_Rel1234.GetErrorYhigh(i),2)+pow(tp_eff_1234->GetErrorYhigh(i),2));
        }
              
        TP_1234->cd();
        TGraphAsymmErrors TP_Res1_1234(res1vx_1234, res1vy_1234, res1vexl_1234, res1vexh_1234, res1veyl_1234, res1veyh_1234);
        TGraphAsymmErrors TP_Res2_1234(res2vx_1234, res2vy_1234, res2vexl_1234, res2vexh_1234, res2veyl_1234, res2veyh_1234);
        TCanvas *ResTP_1234 = new TCanvas;
        ResTP_1234->SetGrid();
        TP_Res1_1234.SetLineColor(2);
	TP_Res1_1234.SetMarkerStyle(21);
	TP_Res1_1234.SetLineWidth(2);
	TP_Res1_1234.SetMarkerColor(2);
	TP_Res1_1234.Draw("AP");
	TP_Res2_1234.SetLineColor(4);
	TP_Res2_1234.SetMarkerStyle(23);
	TP_Res2_1234.SetLineWidth(2);
	TP_Res2_1234.SetMarkerColor(4);
	TP_Res2_1234.Draw("PSAME");
	TP_Res1_1234.SetTitle("TP residuals from MC Rel Eff: MC TP w/o back (red), Data (blue)");
	TP_Res1_1234.GetXaxis()->SetRangeUser(xmin,xmax);
	TP_Res1_1234.GetYaxis()->SetRangeUser(-0.3,0.3);
	ResTP_1234->Write("ResTP_Iso.root");
	ResTP_1234->Close();
	
	TVectorD res1vx_12345(n_12345);
        TVectorD res1vy_12345(n_12345);
        TVectorD res1vexl_12345(n_12345);
        TVectorD res1vexh_12345(n_12345);
        TVectorD res1veyl_12345(n_12345);
        TVectorD res1veyh_12345(n_12345);
        TVectorD res2vx_12345(n_12345);
        TVectorD res2vy_12345(n_12345);
        TVectorD res2vexl_12345(n_12345);
        TVectorD res2vexh_12345(n_12345);
        TVectorD res2veyl_12345(n_12345);
        TVectorD res2veyh_12345(n_12345);
       
        for ( int i = 0; i < n_12345; ++i ){
        double x0 = 0., y0 = 0., x1 = 0., y1 = 0., x2 = 0., y2 = 0.;
        Eff_MC_Rel12345.GetPoint(i, x0, y0);
        Eff_TP_Double_12345.GetPoint(i, x1, y1);
        tp_eff_12345->GetPoint(i, x2, y2);
        res1vx_12345(i)   = i;
        res1vexl_12345(i) = Eff_MC_Rel12345.GetErrorXlow(i);
        res1vexh_12345(i) = Eff_MC_Rel12345.GetErrorXhigh(i);
        res1vy_12345(i)   = y1-y0;
        res1veyl_12345(i) = TMath::Sqrt(pow(Eff_MC_Rel12345.GetErrorYlow(i),2)+pow(Eff_TP_Double_12345.GetErrorYlow(i),2));
        res1veyh_12345(i) = TMath::Sqrt(pow(Eff_MC_Rel12345.GetErrorYhigh(i),2)+pow(Eff_TP_Double_12345.GetErrorYhigh(i),2));
        res2vx_12345(i)   = i;
        res2vexl_12345(i) = Eff_MC_Rel12345.GetErrorXlow(i);
        res2vexh_12345(i) = Eff_MC_Rel12345.GetErrorXhigh(i);
        res2vy_12345(i)   = y2-y0;
        res2veyl_12345(i) = TMath::Sqrt(pow(Eff_MC_Rel12345.GetErrorYlow(i),2)+pow(tp_eff_12345->GetErrorYlow(i),2));
        res2veyh_12345(i) = TMath::Sqrt(pow(Eff_MC_Rel12345.GetErrorYhigh(i),2)+pow(tp_eff_12345->GetErrorYhigh(i),2));
        }
              
        TP_12345->cd();
        TGraphAsymmErrors TP_Res1_12345(res1vx_12345, res1vy_12345, res1vexl_12345, res1vexh_12345, res1veyl_12345, res1veyh_12345);
        TGraphAsymmErrors TP_Res2_12345(res2vx_12345, res2vy_12345, res2vexl_12345, res2vexh_12345, res2veyl_12345, res2veyh_12345);
        TCanvas *ResTP_12345 = new TCanvas;
        ResTP_12345->SetGrid();
        TP_Res1_12345.SetLineColor(2);
	TP_Res1_12345.SetMarkerStyle(21);
	TP_Res1_12345.SetLineWidth(2);
	TP_Res1_12345.SetMarkerColor(2);
	TP_Res1_12345.Draw("AP");
	TP_Res2_12345.SetLineColor(4);
	TP_Res2_12345.SetMarkerStyle(23);
	TP_Res2_12345.SetLineWidth(2);
	TP_Res2_12345.SetMarkerColor(4);
	TP_Res2_12345.Draw("PSAME");
	TP_Res1_12345.SetTitle("TP residuals from MC Rel Eff: MC TP w/o back (red), Data (blue)");
	TP_Res1_12345.GetXaxis()->SetRangeUser(xmin,xmax);
	TP_Res1_12345.GetYaxis()->SetRangeUser(-0.3,0.3);
	ResTP_12345->Write("ResTP_Iso.root");
	ResTP_12345->Close();
	
	TVectorD res1vx_123456(n_123456);
        TVectorD res1vy_123456(n_123456);
        TVectorD res1vexl_123456(n_123456);
        TVectorD res1vexh_123456(n_123456);
        TVectorD res1veyl_123456(n_123456);
        TVectorD res1veyh_123456(n_123456);
        TVectorD res2vx_123456(n_123456);
        TVectorD res2vy_123456(n_123456);
        TVectorD res2vexl_123456(n_123456);
        TVectorD res2vexh_123456(n_123456);
        TVectorD res2veyl_123456(n_123456);
        TVectorD res2veyh_123456(n_123456);
       
        for ( int i = 0; i < n_123456; ++i ){
        double x0 = 0., y0 = 0., x1 = 0., y1 = 0., x2 = 0., y2 = 0.;
        Eff_MC_Rel12345.GetPoint(i, x0, y0);
        Eff_TP_Double_123456.GetPoint(i, x1, y1);
        tp_eff_123456->GetPoint(i, x2, y2);
        res1vx_123456(i)   = i;
        res1vexl_123456(i) = Eff_MC_Rel12345.GetErrorXlow(i);
        res1vexh_123456(i) = Eff_MC_Rel12345.GetErrorXhigh(i);
        res1vy_123456(i)   = y1-y0;
        res1veyl_123456(i) = TMath::Sqrt(pow(Eff_MC_Rel12345.GetErrorYlow(i),2)+pow(Eff_TP_Double_123456.GetErrorYlow(i),2));
        res1veyh_123456(i) = TMath::Sqrt(pow(Eff_MC_Rel12345.GetErrorYhigh(i),2)+pow(Eff_TP_Double_123456.GetErrorYhigh(i),2));
        res2vx_123456(i)   = i;
        res2vexl_123456(i) = Eff_MC_Rel12345.GetErrorXlow(i);
        res2vexh_123456(i) = Eff_MC_Rel12345.GetErrorXhigh(i);
        res2vy_123456(i)   = y2-y0;
        res2veyl_123456(i) = TMath::Sqrt(pow(Eff_MC_Rel12345.GetErrorYlow(i),2)+pow(tp_eff_123456->GetErrorYlow(i),2));
        res2veyh_123456(i) = TMath::Sqrt(pow(Eff_MC_Rel12345.GetErrorYhigh(i),2)+pow(tp_eff_123456->GetErrorYhigh(i),2));
        }
              
        TP_123456->cd();
        TGraphAsymmErrors TP_Res1_123456(res1vx_123456, res1vy_123456, res1vexl_123456, res1vexh_123456, res1veyl_123456, res1veyh_123456);
        TGraphAsymmErrors TP_Res2_123456(res2vx_123456, res2vy_123456, res2vexl_123456, res2vexh_123456, res2veyl_123456, res2veyh_123456);
        TCanvas *ResTP_123456 = new TCanvas;
        ResTP_123456->SetGrid();
        TP_Res1_123456.SetLineColor(2);
	TP_Res1_123456.SetMarkerStyle(21);
	TP_Res1_123456.SetLineWidth(2);
	TP_Res1_123456.SetMarkerColor(2);
	TP_Res1_123456.Draw("AP");
	TP_Res2_123456.SetLineColor(4);
	TP_Res2_123456.SetMarkerStyle(23);
	TP_Res2_123456.SetLineWidth(2);
	TP_Res2_123456.SetMarkerColor(4);
	TP_Res2_123456.Draw("PSAME");
	TP_Res1_123456.SetTitle("TP residuals from MC Rel Eff: MC TP w/o back (red), Data (blue)");
	TP_Res1_123456.GetXaxis()->SetRangeUser(xmin,xmax);
	TP_Res1_123456.GetYaxis()->SetRangeUser(-0.3,0.3);
	ResTP_123456->Write("ResTP_EiD.root");
	ResTP_123456->Close();
	
	
  outplots->Write();
  outplots->Close();
}
