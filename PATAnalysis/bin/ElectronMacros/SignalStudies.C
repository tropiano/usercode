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

//sample == "mc" -> MonteCarlo , sample == "data" -> Data
//Selections: "SYM" = Symmetric selections; "ASYM" = Asymmetric selections

void Signal(string sample, string selections){

        gROOT->SetStyle("Plain");

        TFile *signal_file = TFile::Open("MC_Winter10_399/Z_Madgraph_L1FastJet_JetPt30_EffCorr.root");
        
        //Output
        string out = "SignalStudy_ZMadgraph_JetPt30_Winter10_EffCorr";
        string output = out;
        output+=".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
	
	//Selections
	int _Acc  = 1;
	int _Trg  = 2;
	int _Imp  = 3;
	int _Conv = 4;
	int _Iso  = 5;
	int _EiD  = 6;
	
	//------------------------------------------
	
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
    
        if(!signal_file){
	cout<<"Error! Input files doesn't exist!"<<endl;
	return;
	}

TDirectory *GEff, *REff, *Acc, *Pt_Eff, *Eta_Eff, *GenExclJet_Eff, *RecoInclJet_Eff, *RecoExclJet_Eff, *R_GenExclJet_Eff, *R_RecoInclJet_Eff, *R_RecoExclJet_Eff, *Mass_Eff, *Dist, *GenRec, *IsoJet, *ChargeMisID, *TP, *TP_12, *TP_123, *TP_1234, *TP_12345, *TP_123456;

GEff      = outplots->mkdir("Global_Efficiency");
REff      = outplots->mkdir("Relative_Efficiency");
Acc      = outplots->mkdir("Acceptance");
GenRec   = outplots->mkdir("GenRec_Plots");
IsoJet   = outplots->mkdir("JetIsolation");
Mass_Eff = GEff->mkdir("Efficiency_vs_ZMass");
Pt_Eff = GEff->mkdir("Efficiency_vs_ZPt");
Eta_Eff = GEff->mkdir("Efficiency_vs_ZEta");
GenExclJet_Eff = GEff->mkdir("Efficiency_vs_GenExclJet");
RecoInclJet_Eff = GEff->mkdir("Efficiency_vs_RecoInclJet");
RecoExclJet_Eff = GEff->mkdir("Efficiency_vs_RecoExclJet");
Dist    = GEff->mkdir("Distributions");
R_GenExclJet_Eff = REff->mkdir("Relative_Efficiency_vs_GenExclJet");
R_RecoInclJet_Eff = REff->mkdir("Relative_Efficiency_vs_RecoInclJet");
R_RecoExclJet_Eff = REff->mkdir("Relative_Efficiency_vs_RecoExclJet");
ChargeMisID = outplots->mkdir("ChargeMisID");
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

        ofstream effincl;
	string outeffincl="EffInclReport_";
	outeffincl+=out;
	outeffincl+=".txt";
	effincl.open(outeffincl.c_str());
	effincl<<endl;
	
	ofstream effexcl;
	string outeffexcl="EffExclReport_";
	outeffexcl+=out;
	outeffexcl+=".txt";
	effexcl.open(outeffexcl.c_str());
	effexcl<<endl;
	
   /////////////Efficiency - MC

   //Denominators
   
   TH1D* AccDenom_genMassZ = (TH1D*) signal_file->Get("EfficiencyElectron/AccDenom_genMassZ");
   TH1D* AccDenom_genPtZ = (TH1D*) signal_file->Get("EfficiencyElectron/AccDenom_genPtZ");
   TH1D* AccDenom_genEtaZ = (TH1D*) signal_file->Get("EfficiencyElectron/AccDenom_genEtaZ");
   TH1D* AccDenom_GenExclJetNumber = (TH1D*) signal_file->Get("EfficiencyElectron/AccDenom_GenExclJetNumber");
   TH1D* AccDenom_RecoInclJetNumber = (TH1D*) signal_file->Get("EfficiencyElectron/AccDenom_RecoInclJetNumber");
   TH1D* AccDenom_RecoExclJetNumber = (TH1D*) signal_file->Get("EfficiencyElectron/AccDenom_RecoExclJetNumber");
	
   TH1D* EffDenom_genMassZ = (TH1D*) signal_file->Get("EfficiencyElectron/EffDenom_genMassZ");
   TH1D* EffDenom_genPtZ = (TH1D*) signal_file->Get("EfficiencyElectron/EffDenom_genPtZ");
   TH1D* EffDenom_genEtaZ = (TH1D*) signal_file->Get("EfficiencyElectron/EffDenom_genEtaZ");
   TH1D* EffDenom_GenExclJetNumber = (TH1D*) signal_file->Get("EfficiencyElectron/EffDenom_GenExclJetNumber");
   TH1D* EffDenom_RecoInclJetNumber = (TH1D*) signal_file->Get("EfficiencyElectron/EffDenom_RecoInclJetNumber");
   TH1D* EffDenom_RecoExclJetNumber = (TH1D*) signal_file->Get("EfficiencyElectron/EffDenom_RecoExclJetNumber");
	
   //Reco in acc. efficiency
    
	string genMassZEff_name = "EfficiencyElectron/genMassZEff";
	genMassZEff_name+=_RecoCutFlags[1].c_str();
    TH1D* genMassZ_1 = (TH1D*) signal_file->Get(genMassZEff_name.c_str());
	genMassZEff_name+=_RecoCutFlags[2].c_str();
	TH1D* genMassZ_12 = (TH1D*) signal_file->Get(genMassZEff_name.c_str());
	genMassZEff_name+=_RecoCutFlags[3].c_str();
	TH1D* genMassZ_123 = (TH1D*) signal_file->Get(genMassZEff_name.c_str());
	genMassZEff_name+=_RecoCutFlags[4].c_str();
	TH1D* genMassZ_1234 = (TH1D*) signal_file->Get(genMassZEff_name.c_str());
	genMassZEff_name+=_RecoCutFlags[5].c_str();
	TH1D* genMassZ_12345 = (TH1D*) signal_file->Get(genMassZEff_name.c_str());
	genMassZEff_name+=_RecoCutFlags[6].c_str();
	TH1D* genMassZ_123456 = (TH1D*) signal_file->Get(genMassZEff_name.c_str());
	
	if(!genMassZ_12345){
	cout<<"Error! Cut sequence wrong!"<<endl;
	return;
	}
	
	string genPtZEff_name = "EfficiencyElectron/genPtZEff";
	genPtZEff_name+=_RecoCutFlags[1].c_str();
	TH1D* genPtZ_1 = (TH1D*) signal_file->Get(genPtZEff_name.c_str());
	genPtZEff_name+=_RecoCutFlags[2].c_str();
	TH1D* genPtZ_12 = (TH1D*) signal_file->Get(genPtZEff_name.c_str());
	genPtZEff_name+=_RecoCutFlags[3].c_str();
	TH1D* genPtZ_123 = (TH1D*) signal_file->Get(genPtZEff_name.c_str());
	genPtZEff_name+=_RecoCutFlags[4].c_str();
	TH1D* genPtZ_1234 = (TH1D*) signal_file->Get(genPtZEff_name.c_str());
	genPtZEff_name+=_RecoCutFlags[5].c_str();
	TH1D* genPtZ_12345 = (TH1D*) signal_file->Get(genPtZEff_name.c_str());
	genPtZEff_name+=_RecoCutFlags[6].c_str();
	TH1D* genPtZ_123456 = (TH1D*) signal_file->Get(genPtZEff_name.c_str());
	
	string genEtaZEff_name = "EfficiencyElectron/genEtaZEff";
	genEtaZEff_name+=_RecoCutFlags[1].c_str();
	TH1D* genEtaZ_1 = (TH1D*) signal_file->Get(genEtaZEff_name.c_str());
	genEtaZEff_name+=_RecoCutFlags[2].c_str();
	TH1D* genEtaZ_12 = (TH1D*) signal_file->Get(genEtaZEff_name.c_str());
	genEtaZEff_name+=_RecoCutFlags[3].c_str();
	TH1D* genEtaZ_123 = (TH1D*) signal_file->Get(genEtaZEff_name.c_str());
	genEtaZEff_name+=_RecoCutFlags[4].c_str();
	TH1D* genEtaZ_1234 = (TH1D*) signal_file->Get(genEtaZEff_name.c_str());
	genEtaZEff_name+=_RecoCutFlags[5].c_str();
	TH1D* genEtaZ_12345 = (TH1D*) signal_file->Get(genEtaZEff_name.c_str());
	genEtaZEff_name+=_RecoCutFlags[6].c_str();
	TH1D* genEtaZ_123456 = (TH1D*) signal_file->Get(genEtaZEff_name.c_str());
	
	string GenExclJetEff_name = "EfficiencyElectron/GenExclJetEff";
	GenExclJetEff_name+=_RecoCutFlags[1].c_str();
	TH1D* GenExclJetNumber_1 = (TH1D*) signal_file->Get(GenExclJetEff_name.c_str());
	GenExclJetEff_name+=_RecoCutFlags[2].c_str();
	TH1D* GenExclJetNumber_12 = (TH1D*) signal_file->Get(GenExclJetEff_name.c_str());
	GenExclJetEff_name+=_RecoCutFlags[3].c_str();
	TH1D* GenExclJetNumber_123 = (TH1D*) signal_file->Get(GenExclJetEff_name.c_str());
	GenExclJetEff_name+=_RecoCutFlags[4].c_str();
	TH1D* GenExclJetNumber_1234 = (TH1D*) signal_file->Get(GenExclJetEff_name.c_str());
	GenExclJetEff_name+=_RecoCutFlags[5].c_str();
	TH1D* GenExclJetNumber_12345 = (TH1D*) signal_file->Get(GenExclJetEff_name.c_str());
	GenExclJetEff_name+=_RecoCutFlags[6].c_str();
	TH1D* GenExclJetNumber_123456 = (TH1D*) signal_file->Get(GenExclJetEff_name.c_str());
	
	string RecoInclJetEff_name = "EfficiencyElectron/RecoInclJetEff";
	RecoInclJetEff_name+=_RecoCutFlags[1].c_str();
	TH1D* RecoInclJetNumber_1 = (TH1D*) signal_file->Get(RecoInclJetEff_name.c_str());
	RecoInclJetEff_name+=_RecoCutFlags[2].c_str();
	TH1D* RecoInclJetNumber_12 = (TH1D*) signal_file->Get(RecoInclJetEff_name.c_str());
	RecoInclJetEff_name+=_RecoCutFlags[3].c_str();
	TH1D* RecoInclJetNumber_123 = (TH1D*) signal_file->Get(RecoInclJetEff_name.c_str());
	RecoInclJetEff_name+=_RecoCutFlags[4].c_str();
	TH1D* RecoInclJetNumber_1234 = (TH1D*) signal_file->Get(RecoInclJetEff_name.c_str());
	RecoInclJetEff_name+=_RecoCutFlags[5].c_str();
	TH1D* RecoInclJetNumber_12345 = (TH1D*) signal_file->Get(RecoInclJetEff_name.c_str());
	RecoInclJetEff_name+=_RecoCutFlags[6].c_str();
	TH1D* RecoInclJetNumber_123456 = (TH1D*) signal_file->Get(RecoInclJetEff_name.c_str());
	
	string RecoExclJetEff_name = "EfficiencyElectron/RecoExclJetEff";
	RecoExclJetEff_name+=_RecoCutFlags[1].c_str();
	TH1D* RecoExclJetNumber_1 = (TH1D*) signal_file->Get(RecoExclJetEff_name.c_str());
	RecoExclJetEff_name+=_RecoCutFlags[2].c_str();
	TH1D* RecoExclJetNumber_12 = (TH1D*) signal_file->Get(RecoExclJetEff_name.c_str());
	RecoExclJetEff_name+=_RecoCutFlags[3].c_str();
	TH1D* RecoExclJetNumber_123 = (TH1D*) signal_file->Get(RecoExclJetEff_name.c_str());
	RecoExclJetEff_name+=_RecoCutFlags[4].c_str();
	TH1D* RecoExclJetNumber_1234 = (TH1D*) signal_file->Get(RecoExclJetEff_name.c_str());
	RecoExclJetEff_name+=_RecoCutFlags[5].c_str();
	TH1D* RecoExclJetNumber_12345 = (TH1D*) signal_file->Get(RecoExclJetEff_name.c_str());
	RecoExclJetEff_name+=_RecoCutFlags[6].c_str();
	TH1D* RecoExclJetNumber_123456 = (TH1D*) signal_file->Get(RecoExclJetEff_name.c_str());
		
	if(sample=="mc"){
	
   //Acceptance calculation	
	
	Acc->cd();
	
	TGraphAsymmErrors Acc_VsZMass(EffDenom_genMassZ, AccDenom_genMassZ);
	Acc_VsZMass.SetNameTitle("Acc_ZMass", "Acceptance vs gen Z Mass");
	TGraphAsymmErrors Acc_VsZPt(EffDenom_genPtZ, AccDenom_genPtZ);
	Acc_VsZPt.SetNameTitle("Acc_ZPt", "Acceptance vs gen Z Pt");
	TGraphAsymmErrors Acc_VsZEta(EffDenom_genEtaZ, AccDenom_genEtaZ);
	Acc_VsZEta.SetNameTitle("Acc_ZEta", "Acceptance vs gen Z Eta");
	TGraphAsymmErrors Acc_VsJetN(EffDenom_GenExclJetNumber, AccDenom_GenExclJetNumber);
	Acc_VsJetN.SetNameTitle("Acc_VsGenExclJetN", "Acceptance vs Gen Exclusive Iso Jet Number");
	TGraphAsymmErrors Acc_VsRecoInclJetN(EffDenom_RecoInclJetNumber, AccDenom_RecoInclJetNumber);
	Acc_VsRecoInclJetN.SetNameTitle("Acc_VsRecoInclJetN", "Acceptance vs Reco Inclusive Iso Jet Number");
	TGraphAsymmErrors Acc_VsRecoExclJetN(EffDenom_RecoExclJetNumber, AccDenom_RecoExclJetNumber);
	Acc_VsRecoExclJetN.SetNameTitle("Acc_VsRecoExclJetN", "Acceptance vs Reco Exclusive Iso Jet Number");
	
	Acc_VsZMass.Write();
	Acc_VsZPt.Write();
	Acc_VsZEta.Write();
	Acc_VsJetN.Write();
	Acc_VsRecoInclJetN.Write();
	Acc_VsRecoExclJetN.Write();
	
        effincl<<"Acceptance vs Reco Inclusive Jet number"<<endl<<endl;
        for(int i=0; i<7; i++){
        double x = 0., y = 0.;
        Acc_VsRecoInclJetN.GetPoint(i,x,y);
        effincl<<"Jet# >= "<<i<<" Acc. = "<<y<<" Error High = "<<Acc_VsRecoInclJetN.GetErrorYhigh(i)<<" Error Low = "<<Acc_VsRecoInclJetN.GetErrorYlow(i)<<endl;
        }
        effincl<<endl;
        
        effexcl<<"Acceptance vs Reco Exclusive Jet number"<<endl<<endl;
        for(int i=0; i<7; i++){
        double x = 0., y = 0.;
        Acc_VsRecoExclJetN.GetPoint(i,x,y);
        effexcl<<"Jet# = "<<i<<" Acc. = "<<y<<" Error High = "<<Acc_VsRecoExclJetN.GetErrorYhigh(i)<<" Error Low = "<<Acc_VsRecoExclJetN.GetErrorYlow(i)<<endl;
        }
        effexcl<<endl;
	
   //Global Efficiency calculation vs Z Mass
	
	Mass_Eff->cd();
	
	string EffVsZMass_name = "EffVsZMass";
	EffVsZMass_name+=_RecoCutFlags[1].c_str();
	TGraphAsymmErrors Eff_ZMass_1(genMassZ_1, EffDenom_genMassZ);
	Eff_ZMass_1.SetNameTitle(EffVsZMass_name.c_str(), _RecoCutFlags[1].c_str());
	EffVsZMass_name+=_RecoCutFlags[2].c_str();
	TGraphAsymmErrors Eff_ZMass_12(genMassZ_12, EffDenom_genMassZ);
	Eff_ZMass_12.SetNameTitle(EffVsZMass_name.c_str(), _RecoCutFlags[2].c_str());
	EffVsZMass_name+=_RecoCutFlags[3].c_str();
	TGraphAsymmErrors Eff_ZMass_123(genMassZ_123, EffDenom_genMassZ);
	Eff_ZMass_123.SetNameTitle(EffVsZMass_name.c_str(), _RecoCutFlags[3].c_str());
	EffVsZMass_name+=_RecoCutFlags[4].c_str();
	TGraphAsymmErrors Eff_ZMass_1234(genMassZ_1234, EffDenom_genMassZ);
	Eff_ZMass_1234.SetNameTitle(EffVsZMass_name.c_str(), _RecoCutFlags[4].c_str());
	EffVsZMass_name+=_RecoCutFlags[5].c_str();
	TGraphAsymmErrors Eff_ZMass_12345(genMassZ_12345, EffDenom_genMassZ);
	Eff_ZMass_12345.SetNameTitle(EffVsZMass_name.c_str(), _RecoCutFlags[5].c_str());
	EffVsZMass_name+=_RecoCutFlags[6].c_str();
	TGraphAsymmErrors Eff_ZMass_123456(genMassZ_123456, EffDenom_genMassZ);
	Eff_ZMass_123456.SetNameTitle(EffVsZMass_name.c_str(), _RecoCutFlags[6].c_str());
	
	Eff_ZMass_1.Write();
	Eff_ZMass_12.Write();
	Eff_ZMass_123.Write();
	Eff_ZMass_1234.Write();
	Eff_ZMass_12345.Write();
	Eff_ZMass_123456.Write(); 
	
		
   //Global Efficiency calculation vs Z Pt
	
	Pt_Eff->cd();
	
	string EffVsZPt_name = "EffVsZPt";
	EffVsZPt_name+=_RecoCutFlags[1].c_str();
	TGraphAsymmErrors Eff_ZPt_1(genPtZ_1, EffDenom_genPtZ);
	Eff_ZPt_1.SetNameTitle(EffVsZPt_name.c_str(), _RecoCutFlags[1].c_str());
	EffVsZPt_name+=_RecoCutFlags[2].c_str();
	TGraphAsymmErrors Eff_ZPt_12(genPtZ_12, EffDenom_genPtZ);
	Eff_ZPt_12.SetNameTitle(EffVsZPt_name.c_str(), _RecoCutFlags[2].c_str());
	EffVsZPt_name+=_RecoCutFlags[3].c_str();
	TGraphAsymmErrors Eff_ZPt_123(genPtZ_123, EffDenom_genPtZ);
	Eff_ZPt_123.SetNameTitle(EffVsZPt_name.c_str(), _RecoCutFlags[3].c_str());
	EffVsZPt_name+=_RecoCutFlags[4].c_str();
	TGraphAsymmErrors Eff_ZPt_1234(genPtZ_1234, EffDenom_genPtZ);
	Eff_ZPt_1234.SetNameTitle(EffVsZPt_name.c_str(), _RecoCutFlags[4].c_str());
	EffVsZPt_name+=_RecoCutFlags[5].c_str();
	TGraphAsymmErrors Eff_ZPt_12345(genPtZ_12345, EffDenom_genPtZ);
	Eff_ZPt_12345.SetNameTitle(EffVsZPt_name.c_str(), _RecoCutFlags[5].c_str());
	EffVsZPt_name+=_RecoCutFlags[6].c_str();
	TGraphAsymmErrors Eff_ZPt_123456(genPtZ_123456, EffDenom_genPtZ);
	Eff_ZPt_123456.SetNameTitle(EffVsZPt_name.c_str(), _RecoCutFlags[6].c_str());
	
	Eff_ZPt_1.Write();
	Eff_ZPt_12.Write();
	Eff_ZPt_123.Write();
	Eff_ZPt_1234.Write();
	Eff_ZPt_12345.Write();
	Eff_ZPt_123456.Write();
	
		
   //Global Efficiency calculation vs Z Eta
	
	Eta_Eff->cd();
	
	string EffVsZEta_name = "EffVsZEta";
	EffVsZEta_name+=_RecoCutFlags[1].c_str();
	TGraphAsymmErrors Eff_ZEta_1(genEtaZ_1, EffDenom_genEtaZ);
	Eff_ZEta_1.SetNameTitle(EffVsZEta_name.c_str(), _RecoCutFlags[1].c_str());
	EffVsZEta_name+=_RecoCutFlags[2].c_str();
	TGraphAsymmErrors Eff_ZEta_12(genEtaZ_12, EffDenom_genEtaZ);
	Eff_ZEta_12.SetNameTitle(EffVsZEta_name.c_str(), _RecoCutFlags[2].c_str());
	EffVsZEta_name+=_RecoCutFlags[3].c_str();
	TGraphAsymmErrors Eff_ZEta_123(genEtaZ_123, EffDenom_genEtaZ);
	Eff_ZEta_123.SetNameTitle(EffVsZEta_name.c_str(), _RecoCutFlags[3].c_str());
	EffVsZEta_name+=_RecoCutFlags[4].c_str();
	TGraphAsymmErrors Eff_ZEta_1234(genEtaZ_1234, EffDenom_genEtaZ);
	Eff_ZEta_1234.SetNameTitle(EffVsZEta_name.c_str(), _RecoCutFlags[4].c_str());
	EffVsZEta_name+=_RecoCutFlags[5].c_str();
	TGraphAsymmErrors Eff_ZEta_12345(genEtaZ_12345, EffDenom_genEtaZ);
	Eff_ZEta_12345.SetNameTitle(EffVsZEta_name.c_str(), _RecoCutFlags[5].c_str());
	EffVsZEta_name+=_RecoCutFlags[6].c_str();
	TGraphAsymmErrors Eff_ZEta_123456(genEtaZ_123456, EffDenom_genEtaZ);
	Eff_ZEta_123456.SetNameTitle(EffVsZEta_name.c_str(), _RecoCutFlags[6].c_str());
	
	Eff_ZEta_1.Write();
	Eff_ZEta_12.Write();
	Eff_ZEta_123.Write();
	Eff_ZEta_1234.Write();
	Eff_ZEta_12345.Write();
	Eff_ZEta_123456.Write();
	
		
   //Global Efficiency calculation vs Gen Jet Number
	
	GenExclJet_Eff->cd();
	
	string EffVsGenExclJetN_name = "EffVsGenExclJetN";
	EffVsGenExclJetN_name+=_RecoCutFlags[1].c_str();
	TGraphAsymmErrors Eff_GenExclJet_1(GenExclJetNumber_1, EffDenom_GenExclJetNumber);
	Eff_GenExclJet_1.SetNameTitle(EffVsGenExclJetN_name.c_str(), _RecoCutFlags[1].c_str());
	EffVsGenExclJetN_name+=_RecoCutFlags[2].c_str();
	TGraphAsymmErrors Eff_GenExclJet_12(GenExclJetNumber_12, EffDenom_GenExclJetNumber);
	Eff_GenExclJet_12.SetNameTitle(EffVsGenExclJetN_name.c_str(), _RecoCutFlags[2].c_str());
	EffVsGenExclJetN_name+=_RecoCutFlags[3].c_str();
	TGraphAsymmErrors Eff_GenExclJet_123(GenExclJetNumber_123, EffDenom_GenExclJetNumber);
	Eff_GenExclJet_123.SetNameTitle(EffVsGenExclJetN_name.c_str(), _RecoCutFlags[3].c_str());
	EffVsGenExclJetN_name+=_RecoCutFlags[4].c_str();
	TGraphAsymmErrors Eff_GenExclJet_1234(GenExclJetNumber_1234, EffDenom_GenExclJetNumber);
	Eff_GenExclJet_1234.SetNameTitle(EffVsGenExclJetN_name.c_str(), _RecoCutFlags[4].c_str());
	EffVsGenExclJetN_name+=_RecoCutFlags[5].c_str();
	TGraphAsymmErrors Eff_GenExclJet_12345(GenExclJetNumber_12345, EffDenom_GenExclJetNumber);
	Eff_GenExclJet_12345.SetNameTitle(EffVsGenExclJetN_name.c_str(), _RecoCutFlags[5].c_str());
	EffVsGenExclJetN_name+=_RecoCutFlags[6].c_str();
	TGraphAsymmErrors Eff_GenExclJet_123456(GenExclJetNumber_123456, EffDenom_GenExclJetNumber);
	Eff_GenExclJet_123456.SetNameTitle(EffVsGenExclJetN_name.c_str(), _RecoCutFlags[6].c_str());
	
	Eff_GenExclJet_1.Write();
	Eff_GenExclJet_12.Write();
	Eff_GenExclJet_123.Write();
	Eff_GenExclJet_1234.Write();
	Eff_GenExclJet_12345.Write();
	Eff_GenExclJet_123456.Write();
	
	//Global Efficiency calculation vs Reco Exclusive Jet Number
	
	RecoExclJet_Eff->cd();
	
	string EffVsRecoExclJetN_name = "EffVsRecoExclJetN";
	EffVsRecoExclJetN_name+=_RecoCutFlags[1].c_str();
	TGraphAsymmErrors Eff_RecoExclJet_1(RecoExclJetNumber_1, EffDenom_RecoExclJetNumber);
	Eff_RecoExclJet_1.SetNameTitle(EffVsRecoExclJetN_name.c_str(), _RecoCutFlags[1].c_str());
	EffVsRecoExclJetN_name+=_RecoCutFlags[2].c_str();
	TGraphAsymmErrors Eff_RecoExclJet_12(RecoExclJetNumber_12, EffDenom_RecoExclJetNumber);
	Eff_RecoExclJet_12.SetNameTitle(EffVsRecoExclJetN_name.c_str(), _RecoCutFlags[2].c_str());
	EffVsRecoExclJetN_name+=_RecoCutFlags[3].c_str();
	TGraphAsymmErrors Eff_RecoExclJet_123(RecoExclJetNumber_123, EffDenom_RecoExclJetNumber);
	Eff_RecoExclJet_123.SetNameTitle(EffVsRecoExclJetN_name.c_str(), _RecoCutFlags[3].c_str());
	EffVsRecoExclJetN_name+=_RecoCutFlags[4].c_str();
	TGraphAsymmErrors Eff_RecoExclJet_1234(RecoExclJetNumber_1234, EffDenom_RecoExclJetNumber);
	Eff_RecoExclJet_1234.SetNameTitle(EffVsRecoExclJetN_name.c_str(), _RecoCutFlags[4].c_str());
	EffVsRecoExclJetN_name+=_RecoCutFlags[5].c_str();
	TGraphAsymmErrors Eff_RecoExclJet_12345(RecoExclJetNumber_12345, EffDenom_RecoExclJetNumber);
	Eff_RecoExclJet_12345.SetNameTitle(EffVsRecoExclJetN_name.c_str(), _RecoCutFlags[5].c_str());
	EffVsRecoExclJetN_name+=_RecoCutFlags[6].c_str();
	TGraphAsymmErrors Eff_RecoExclJet_123456(RecoExclJetNumber_123456, EffDenom_RecoExclJetNumber);
	Eff_RecoExclJet_123456.SetNameTitle(EffVsRecoExclJetN_name.c_str(), _RecoCutFlags[6].c_str());
	
	Eff_RecoExclJet_1.Write();
	Eff_RecoExclJet_12.Write();
	Eff_RecoExclJet_123.Write();
	Eff_RecoExclJet_1234.Write();
	Eff_RecoExclJet_12345.Write();
	Eff_RecoExclJet_123456.Write();
	
	
	//Global Efficiency calculation vs Reco Inclusive Jet Number
	
	RecoInclJet_Eff->cd();
	
	string EffVsRecoInclJetN_name = "EffVsRecoInclJetN";
	EffVsRecoInclJetN_name+=_RecoCutFlags[1].c_str();
		
	TGraphAsymmErrors Eff_RecoInclJet_1(RecoInclJetNumber_1, EffDenom_RecoInclJetNumber);
	Eff_RecoInclJet_1.SetNameTitle(EffVsRecoInclJetN_name.c_str(), _RecoCutFlags[1].c_str());
	EffVsRecoInclJetN_name+=_RecoCutFlags[2].c_str();
	TGraphAsymmErrors Eff_RecoInclJet_12(RecoInclJetNumber_12, EffDenom_RecoInclJetNumber);
	Eff_RecoInclJet_12.SetNameTitle(EffVsRecoInclJetN_name.c_str(), _RecoCutFlags[2].c_str());
	EffVsRecoInclJetN_name+=_RecoCutFlags[3].c_str();
	TGraphAsymmErrors Eff_RecoInclJet_123(RecoInclJetNumber_123, EffDenom_RecoInclJetNumber);
	Eff_RecoInclJet_123.SetNameTitle(EffVsRecoInclJetN_name.c_str(), _RecoCutFlags[3].c_str());
	EffVsRecoInclJetN_name+=_RecoCutFlags[4].c_str();
	TGraphAsymmErrors Eff_RecoInclJet_1234(RecoInclJetNumber_1234, EffDenom_RecoInclJetNumber);
	Eff_RecoInclJet_1234.SetNameTitle(EffVsRecoInclJetN_name.c_str(), _RecoCutFlags[4].c_str());
	EffVsRecoInclJetN_name+=_RecoCutFlags[5].c_str();
	TGraphAsymmErrors Eff_RecoInclJet_12345(RecoInclJetNumber_12345, EffDenom_RecoInclJetNumber);
	Eff_RecoInclJet_12345.SetNameTitle(EffVsRecoInclJetN_name.c_str(), _RecoCutFlags[5].c_str());
	EffVsRecoInclJetN_name+=_RecoCutFlags[6].c_str();
	TGraphAsymmErrors Eff_RecoInclJet_123456(RecoInclJetNumber_123456, EffDenom_RecoInclJetNumber);
	Eff_RecoInclJet_123456.SetNameTitle(EffVsRecoInclJetN_name.c_str(), _RecoCutFlags[6].c_str());
	
	Eff_RecoInclJet_1.Write();
	Eff_RecoInclJet_12.Write();
	Eff_RecoInclJet_123.Write();
	Eff_RecoInclJet_1234.Write();
	Eff_RecoInclJet_12345.Write();
	Eff_RecoInclJet_123456.Write();
	
	    effincl<<"Global efficiency (Acc+Trg+Imp+Conv+Iso+EiD) vs Reco Inclusive Jet number"<<endl<<endl;
        for(int i=0; i<7; i++){
        double x = 0., y = 0.;
        Eff_RecoInclJet_123456.GetPoint(i,x,y);
        effincl<<"Jet# >= "<<i<<" Global Eff. = "<<y<<" Error High = "<<Eff_RecoInclJet_123456.GetErrorYhigh(i)<<" Error Low = "<<Eff_RecoInclJet_123456.GetErrorYlow(i)<<endl;
        }
        effincl<<endl;
        
        effexcl<<"Global efficiency (Acc+Trg+Imp+Conv+Iso+EiD) vs Reco Exclusive Jet number"<<endl<<endl;
        for(int i=0; i<7; i++){
        double x = 0., y = 0.;
        Eff_RecoExclJet_123456.GetPoint(i,x,y);
        effexcl<<"Jet# = "<<i<<" Global Eff. = "<<y<<" Error High = "<<Eff_RecoExclJet_123456.GetErrorYhigh(i)<<" Error Low = "<<Eff_RecoExclJet_123456.GetErrorYlow(i)<<endl;
        }
        effexcl<<endl;
	
     //Global Efficiency cut plots
	
	GenExclJet_Eff->cd();
	TCanvas *EffGenExclJet = new TCanvas;
	Eff_GenExclJet_1.GetYaxis()->SetRangeUser(0.3,1);
	Eff_GenExclJet_1.GetXaxis()->SetTitle("gen Jet number");
	Eff_GenExclJet_1.SetMarkerStyle(20);
	Eff_GenExclJet_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	Eff_GenExclJet_12.SetLineColor(2);
	Eff_GenExclJet_12.SetMarkerStyle(21);
	Eff_GenExclJet_12.SetMarkerColor(2);
	Eff_GenExclJet_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	Eff_GenExclJet_123.SetLineColor(3);
	Eff_GenExclJet_123.SetMarkerStyle(22);
	Eff_GenExclJet_123.SetMarkerColor(3);
	Eff_GenExclJet_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	Eff_GenExclJet_1234.SetLineColor(4);
	Eff_GenExclJet_1234.SetMarkerStyle(23);
	Eff_GenExclJet_1234.SetMarkerColor(4);
	Eff_GenExclJet_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
	Eff_GenExclJet_12345.SetLineColor(6);
	Eff_GenExclJet_12345.SetMarkerStyle(24);
	Eff_GenExclJet_12345.SetMarkerColor(6);
	Eff_GenExclJet_12345.Draw("PSAME");}
	if(_RecoCutFlags[6] != "_1"){
	Eff_GenExclJet_123456.SetLineColor(9);
	Eff_GenExclJet_123456.SetMarkerStyle(25);
	Eff_GenExclJet_123456.SetMarkerColor(9);
	Eff_GenExclJet_123456.Draw("PSAME");}
	TLegend *LegEffJ = new TLegend(0.51,0.67,0.88,0.88);
	LegEffJ->SetFillColor(0);
	LegEffJ->SetBorderSize(0);
	LegEffJ->AddEntry(&Eff_GenExclJet_1,"Acc","lp");
   	LegEffJ->AddEntry(&Eff_GenExclJet_12,"Acc+Trg","lp");
	LegEffJ->AddEntry(&Eff_GenExclJet_123,"Acc+Trg+Imp","lp");
	LegEffJ->AddEntry(&Eff_GenExclJet_1234,"Acc+Trg+Imp+Conv","lp");
	LegEffJ->AddEntry(&Eff_GenExclJet_12345,"Acc+Trg+Imp+Conv+Iso","lp");
	LegEffJ->AddEntry(&Eff_GenExclJet_123456,"Acc+Trg+Imp+Conv+Iso+EiD","lp");
	LegEffJ->Draw();
	Eff_GenExclJet_1.SetTitle("Cut Efficiency vs Gen Excl Jet Number");
	EffGenExclJet->Write("CutEfficiencyVsGenExclJet.root");
	EffGenExclJet->Close();
	
	RecoInclJet_Eff->cd();
	TCanvas *EffRecoInclJet = new TCanvas;
		
	Eff_RecoInclJet_1.GetYaxis()->SetRangeUser(0.3,1);
	Eff_RecoInclJet_1.GetXaxis()->SetTitle("reco Inclusive Jet number");
	Eff_RecoInclJet_1.SetMarkerStyle(20);
	Eff_RecoInclJet_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	Eff_RecoInclJet_12.SetLineColor(2);
	Eff_RecoInclJet_12.SetMarkerStyle(21);
	Eff_RecoInclJet_12.SetMarkerColor(2);
	Eff_RecoInclJet_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	Eff_RecoInclJet_123.SetLineColor(3);
	Eff_RecoInclJet_123.SetMarkerStyle(22);
	Eff_RecoInclJet_123.SetMarkerColor(3);
	Eff_RecoInclJet_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	Eff_RecoInclJet_1234.SetLineColor(4);
	Eff_RecoInclJet_1234.SetMarkerStyle(23);
	Eff_RecoInclJet_1234.SetMarkerColor(4);
	Eff_RecoInclJet_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
	Eff_RecoInclJet_12345.SetLineColor(6);
	Eff_RecoInclJet_12345.SetMarkerStyle(24);
	Eff_RecoInclJet_12345.SetMarkerColor(6);
	Eff_RecoInclJet_12345.Draw("PSAME");}
	if(_RecoCutFlags[6] != "_1"){
	Eff_RecoInclJet_123456.SetLineColor(9);
	Eff_RecoInclJet_123456.SetMarkerStyle(25);
	Eff_RecoInclJet_123456.SetMarkerColor(9);
	Eff_RecoInclJet_123456.Draw("PSAME");}
	TLegend *LegEffRecoInclJ = new TLegend(0.51,0.67,0.88,0.88);
	LegEffRecoInclJ->SetFillColor(0);
	LegEffRecoInclJ->SetBorderSize(0);
	LegEffRecoInclJ->AddEntry(&Eff_RecoInclJet_1,"Acc","lp");
   	LegEffRecoInclJ->AddEntry(&Eff_RecoInclJet_12,"Acc+Trg","lp");
	LegEffRecoInclJ->AddEntry(&Eff_RecoInclJet_123,"Acc+Trg+Imp","lp");
	LegEffRecoInclJ->AddEntry(&Eff_RecoInclJet_1234,"Acc+Trg+Imp+Conv","lp");
	LegEffRecoInclJ->AddEntry(&Eff_RecoInclJet_12345,"Acc+Trg+Imp+Conv+Iso","lp");
	LegEffRecoInclJ->AddEntry(&Eff_RecoInclJet_123456,"Acc+Trg+Imp+Conv+Iso+EiD","lp");
	LegEffRecoInclJ->Draw();
	Eff_RecoInclJet_1.SetTitle("Cut Efficiency vs Reco Inclusive Jet Number");
	EffRecoInclJet->Write("CutEfficiencyVsRecoInclJet.root");
	EffRecoInclJet->Close();
		
	RecoExclJet_Eff->cd();
	TCanvas *EffRecoExclJet = new TCanvas;
	Eff_RecoExclJet_1.GetYaxis()->SetRangeUser(0.3,1);
	Eff_RecoExclJet_1.GetXaxis()->SetTitle("reco Exclusive Jet number");
	Eff_RecoExclJet_1.SetMarkerStyle(20);
	Eff_RecoExclJet_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	Eff_RecoExclJet_12.SetLineColor(2);
	Eff_RecoExclJet_12.SetMarkerStyle(21);
	Eff_RecoExclJet_12.SetMarkerColor(2);
	Eff_RecoExclJet_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	Eff_RecoExclJet_123.SetLineColor(3);
	Eff_RecoExclJet_123.SetMarkerStyle(22);
	Eff_RecoExclJet_123.SetMarkerColor(3);
	Eff_RecoExclJet_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	Eff_RecoExclJet_1234.SetLineColor(4);
	Eff_RecoExclJet_1234.SetMarkerStyle(23);
	Eff_RecoExclJet_1234.SetMarkerColor(4);
	Eff_RecoExclJet_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
	Eff_RecoExclJet_12345.SetLineColor(6);
	Eff_RecoExclJet_12345.SetMarkerStyle(24);
	Eff_RecoExclJet_12345.SetMarkerColor(6);
	Eff_RecoExclJet_12345.Draw("PSAME");}
	if(_RecoCutFlags[6] != "_1"){
	Eff_RecoExclJet_123456.SetLineColor(9);
	Eff_RecoExclJet_123456.SetMarkerStyle(25);
	Eff_RecoExclJet_123456.SetMarkerColor(9);
	Eff_RecoExclJet_123456.Draw("PSAME");}
	TLegend *LegEffRecoExclJ = new TLegend(0.51,0.67,0.88,0.88);
	LegEffRecoExclJ->SetFillColor(0);
	LegEffRecoExclJ->SetBorderSize(0);
	LegEffRecoExclJ->AddEntry(&Eff_RecoExclJet_1,"Acc","lp");
   	LegEffRecoExclJ->AddEntry(&Eff_RecoExclJet_12,"Acc+Trg","lp");
	LegEffRecoExclJ->AddEntry(&Eff_RecoExclJet_123,"Acc+Trg+Imp","lp");
	LegEffRecoExclJ->AddEntry(&Eff_RecoExclJet_1234,"Acc+Trg+Imp+Conv","lp");
	LegEffRecoExclJ->AddEntry(&Eff_RecoExclJet_12345,"Acc+Trg+Imp+Conv+Iso","lp");
	LegEffRecoExclJ->AddEntry(&Eff_RecoExclJet_123456,"Acc+Trg+Imp+Conv+Iso+EiD","lp");
	LegEffRecoExclJ->Draw();
	Eff_RecoExclJet_1.SetTitle("Cut Efficiency vs Reco Exclusive Jet Number");
	EffRecoExclJet->Write("CutEfficiencyVsRecoExclJet.root");
	EffRecoExclJet->Close();
	                                                   	
	Mass_Eff->cd();
	TCanvas *EffZMass = new TCanvas;
	Eff_ZMass_1.GetYaxis()->SetRangeUser(0.2,1);
	Eff_ZMass_1.GetXaxis()->SetTitle("gen Z Mass");
	Eff_ZMass_1.SetMarkerStyle(20);
	Eff_ZMass_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	Eff_ZMass_12.SetLineColor(2);
	Eff_ZMass_12.SetMarkerStyle(21);
	Eff_ZMass_12.SetMarkerColor(2);
	Eff_ZMass_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	Eff_ZMass_123.SetLineColor(3);
	Eff_ZMass_123.SetMarkerStyle(22);
	Eff_ZMass_123.SetMarkerColor(3);
	Eff_ZMass_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	Eff_ZMass_1234.SetLineColor(4);
	Eff_ZMass_1234.SetMarkerStyle(23);
	Eff_ZMass_1234.SetMarkerColor(4);
	Eff_ZMass_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
	Eff_ZMass_12345.SetLineColor(6);
	Eff_ZMass_12345.SetMarkerStyle(24);
	Eff_ZMass_12345.SetMarkerColor(6);
	Eff_ZMass_12345.Draw("PSAME");}
	if(_RecoCutFlags[6] != "_1"){
	Eff_ZMass_123456.SetLineColor(9);
	Eff_ZMass_123456.SetMarkerStyle(25);
	Eff_ZMass_123456.SetMarkerColor(9);
	Eff_ZMass_123456.Draw("PSAME");}
	TLegend *LegEffZM = new TLegend(0.51,0.67,0.88,0.88);
	LegEffZM->SetFillColor(0);
	LegEffZM->SetBorderSize(0);
	LegEffZM->AddEntry(&Eff_ZMass_1,"Acc","lp");
   	LegEffZM->AddEntry(&Eff_ZMass_12,"Acc+Trg","lp");
	LegEffZM->AddEntry(&Eff_ZMass_123,"Acc+Trg+Imp","lp");
	LegEffZM->AddEntry(&Eff_ZMass_1234,"Acc+Trg+Imp+Conv","lp");
	LegEffZM->AddEntry(&Eff_ZMass_12345,"Acc+Trg+Imp+Conv+Iso","lp");
	LegEffZM->AddEntry(&Eff_ZMass_123456,"Acc+Trg+Imp+Conv+Iso+EiD","lp");
	LegEffZM->Draw();
	Eff_ZMass_1.SetTitle("Cut Efficiency vs Z Mass");
	EffZMass->Write("CutEfficiencyVsZMass.root");
	EffZMass->Close();  
		
	Pt_Eff->cd();
	TCanvas *EffZPt = new TCanvas;
	Eff_ZPt_1.GetYaxis()->SetRangeUser(0.2,1.2);
	Eff_ZPt_1.GetXaxis()->SetTitle("gen Z Pt");
	Eff_ZPt_1.SetMarkerStyle(20);
	Eff_ZPt_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	Eff_ZPt_12.SetLineColor(2);
	Eff_ZPt_12.SetMarkerStyle(21);
	Eff_ZPt_12.SetMarkerColor(2);
	Eff_ZPt_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	Eff_ZPt_123.SetLineColor(3);
	Eff_ZPt_123.SetMarkerStyle(22);
	Eff_ZPt_123.SetMarkerColor(3);
	Eff_ZPt_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	Eff_ZPt_1234.SetLineColor(4);
	Eff_ZPt_1234.SetMarkerStyle(23);
	Eff_ZPt_1234.SetMarkerColor(4);
	Eff_ZPt_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
	Eff_ZPt_12345.SetLineColor(6);
	Eff_ZPt_12345.SetMarkerStyle(24);
	Eff_ZPt_12345.SetMarkerColor(6);
	Eff_ZPt_12345.Draw("PSAME");}
	if(_RecoCutFlags[6] != "_1"){
	Eff_ZPt_123456.SetLineColor(9);
	Eff_ZPt_123456.SetMarkerStyle(25);
	Eff_ZPt_123456.SetMarkerColor(9);
	Eff_ZPt_123456.Draw("PSAME");}
	TLegend *LegEffZPt = new TLegend(0.51,0.67,0.88,0.88);
	LegEffZPt->SetFillColor(0);
	LegEffZPt->SetBorderSize(0);
	LegEffZPt->AddEntry(&Eff_ZPt_1,"Acc","lp");
   	LegEffZPt->AddEntry(&Eff_ZPt_12,"Acc+Trg","lp");
	LegEffZPt->AddEntry(&Eff_ZPt_123,"Acc+Trg+Imp","lp");
	LegEffZPt->AddEntry(&Eff_ZPt_1234,"Acc+Trg+Imp+Conv","lp");
	LegEffZPt->AddEntry(&Eff_ZPt_12345,"Acc+Trg+Imp+Conv+Iso","lp");
	LegEffZPt->AddEntry(&Eff_ZPt_123456,"Acc+Trg+Imp+Conv+Iso+EiD","lp");
	LegEffZPt->Draw();
	Eff_ZPt_1.SetTitle("Cut Efficiency vs Z Pt");
	EffZPt->Write("CutEfficiencyVsZPt.root");
	EffZPt->Close();
	
	Eta_Eff->cd();
	TCanvas *EffZEta = new TCanvas;
	Eff_ZEta_1.GetYaxis()->SetRangeUser(0.2,1.1);
	Eff_ZEta_1.GetXaxis()->SetTitle("gen Z Eta");
	Eff_ZEta_1.SetMarkerStyle(20);
	Eff_ZEta_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	Eff_ZEta_12.SetLineColor(2);
	Eff_ZEta_12.SetMarkerStyle(21);
	Eff_ZEta_12.SetMarkerColor(2);
	Eff_ZEta_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	Eff_ZEta_123.SetLineColor(3);
	Eff_ZEta_123.SetMarkerStyle(22);
	Eff_ZEta_123.SetMarkerColor(3);
	Eff_ZEta_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	Eff_ZEta_1234.SetLineColor(4);
	Eff_ZEta_1234.SetMarkerStyle(23);
	Eff_ZEta_1234.SetMarkerColor(4);
	Eff_ZEta_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
	Eff_ZEta_12345.SetLineColor(6);
	Eff_ZEta_12345.SetMarkerStyle(24);
	Eff_ZEta_12345.SetMarkerColor(6);
	Eff_ZEta_12345.Draw("PSAME");}
	if(_RecoCutFlags[6] != "_1"){
	Eff_ZEta_123456.SetLineColor(9);
	Eff_ZEta_123456.SetMarkerStyle(25);
	Eff_ZEta_123456.SetMarkerColor(9);
	Eff_ZEta_123456.Draw("PSAME");}
	TLegend *LegEffZEta = new TLegend(0.51,0.67,0.88,0.88);
	LegEffZEta->SetFillColor(0);
	LegEffZEta->SetBorderSize(0);
	LegEffZEta->AddEntry(&Eff_ZEta_1,"Acc","lp");
   	LegEffZEta->AddEntry(&Eff_ZEta_12,"Acc+Trg","lp");
	LegEffZEta->AddEntry(&Eff_ZEta_123,"Acc+Trg+Imp","lp");
	LegEffZEta->AddEntry(&Eff_ZEta_1234,"Acc+Trg+Imp+Conv","lp");
	LegEffZEta->AddEntry(&Eff_ZEta_12345,"Acc+Trg+Imp+Conv+Iso","lp");
	LegEffZEta->AddEntry(&Eff_ZEta_123456,"Acc+Trg+Imp+Conv+Iso+EiD","lp");
	LegEffZEta->Draw();
	Eff_ZEta_1.SetTitle("Cut Efficiency vs Z Eta");
	EffZEta->Write("CutEfficiencyVsZEta.root");
	EffZEta->Close();
	
	}
	
/////////////////////Relative Efficiency from MC
	
	R_GenExclJet_Eff->cd();
	
	TGraphAsymmErrors RelEff_GenExclJet_1(GenExclJetNumber_1, EffDenom_GenExclJetNumber);
	TGraphAsymmErrors RelEff_GenExclJet_12(GenExclJetNumber_12, GenExclJetNumber_1);
	TGraphAsymmErrors RelEff_GenExclJet_123(GenExclJetNumber_123, GenExclJetNumber_12);
	TGraphAsymmErrors RelEff_GenExclJet_1234(GenExclJetNumber_1234, GenExclJetNumber_123);
	TGraphAsymmErrors RelEff_GenExclJet_12345(GenExclJetNumber_12345, GenExclJetNumber_1234);
	TGraphAsymmErrors RelEff_GenExclJet_123456(GenExclJetNumber_123456, GenExclJetNumber_12345);
	
	TGraphAsymmErrors RelEff_RecoInclJet_1(RecoInclJetNumber_1, EffDenom_RecoInclJetNumber);
	TGraphAsymmErrors RelEff_RecoInclJet_12(RecoInclJetNumber_12, RecoInclJetNumber_1);
	TGraphAsymmErrors RelEff_RecoInclJet_123(RecoInclJetNumber_123, RecoInclJetNumber_12);
	TGraphAsymmErrors RelEff_RecoInclJet_1234(RecoInclJetNumber_1234, RecoInclJetNumber_123);
	TGraphAsymmErrors RelEff_RecoInclJet_12345(RecoInclJetNumber_12345, RecoInclJetNumber_1234);
	TGraphAsymmErrors RelEff_RecoInclJet_123456(RecoInclJetNumber_123456, RecoInclJetNumber_12345);
	
	TGraphAsymmErrors RelEff_RecoExclJet_1(RecoExclJetNumber_1, EffDenom_RecoExclJetNumber);
	TGraphAsymmErrors RelEff_RecoExclJet_12(RecoExclJetNumber_12, RecoExclJetNumber_1);
	TGraphAsymmErrors RelEff_RecoExclJet_123(RecoExclJetNumber_123, RecoExclJetNumber_12);
	TGraphAsymmErrors RelEff_RecoExclJet_1234(RecoExclJetNumber_1234, RecoExclJetNumber_123);
	TGraphAsymmErrors RelEff_RecoExclJet_12345(RecoExclJetNumber_12345, RecoExclJetNumber_1234);
	TGraphAsymmErrors RelEff_RecoExclJet_123456(RecoExclJetNumber_123456, RecoExclJetNumber_12345);
	
	//Gen exclusive jet number
	
	if(sample=="mc"){
        
        string RelEff_GenExclJet_name="RelativeEff_MC";
        RelEff_GenExclJet_name+=_RecoCutFlags[1].c_str();
        RelEff_GenExclJet_1.SetNameTitle(RelEff_GenExclJet_name.c_str(), RelEff_GenExclJet_name.c_str());
        RelEff_GenExclJet_name+=_RecoCutFlags[2].c_str();  
        RelEff_GenExclJet_12.SetNameTitle(RelEff_GenExclJet_name.c_str(), RelEff_GenExclJet_name.c_str());
        RelEff_GenExclJet_name+=_RecoCutFlags[3].c_str();  
        RelEff_GenExclJet_123.SetNameTitle(RelEff_GenExclJet_name.c_str(), RelEff_GenExclJet_name.c_str());
        RelEff_GenExclJet_name+=_RecoCutFlags[4].c_str();  
        RelEff_GenExclJet_1234.SetNameTitle(RelEff_GenExclJet_name.c_str(), RelEff_GenExclJet_name.c_str());
        RelEff_GenExclJet_name+=_RecoCutFlags[5].c_str();  
        RelEff_GenExclJet_12345.SetNameTitle(RelEff_GenExclJet_name.c_str(), RelEff_GenExclJet_name.c_str());
        RelEff_GenExclJet_name+=_RecoCutFlags[6].c_str();  
        RelEff_GenExclJet_123456.SetNameTitle(RelEff_GenExclJet_name.c_str(), RelEff_GenExclJet_name.c_str());
        
        RelEff_GenExclJet_1.Write();
        RelEff_GenExclJet_12.Write();
        RelEff_GenExclJet_123.Write();
        RelEff_GenExclJet_1234.Write();
        RelEff_GenExclJet_123456.Write();
        
		TCanvas *REffJet = new TCanvas;
		RelEff_GenExclJet_1.GetYaxis()->SetRangeUser(0.3,1);
		RelEff_GenExclJet_1.GetXaxis()->SetTitle("gen Jet number");
		RelEff_GenExclJet_1.SetMarkerStyle(20);
		RelEff_GenExclJet_1.Draw("AP");
		if(_RecoCutFlags[2] != "_1"){
			RelEff_GenExclJet_12.SetLineColor(2);
			RelEff_GenExclJet_12.SetMarkerStyle(21);
			RelEff_GenExclJet_12.SetMarkerColor(2);
			RelEff_GenExclJet_12.Draw("PSAME");}
		if(_RecoCutFlags[3] != "_1"){
			RelEff_GenExclJet_123.SetLineColor(3);
			RelEff_GenExclJet_123.SetMarkerStyle(22);
			RelEff_GenExclJet_123.SetMarkerColor(3);
			RelEff_GenExclJet_123.Draw("PSAME");}
		if(_RecoCutFlags[4] != "_1"){
			RelEff_GenExclJet_1234.SetLineColor(4);
			RelEff_GenExclJet_1234.SetMarkerStyle(23);
			RelEff_GenExclJet_1234.SetMarkerColor(4);
			RelEff_GenExclJet_1234.Draw("PSAME");}
		if(_RecoCutFlags[5] != "_1"){
			RelEff_GenExclJet_12345.SetLineColor(6);
			RelEff_GenExclJet_12345.SetMarkerStyle(24);
			RelEff_GenExclJet_12345.SetMarkerColor(6);
			RelEff_GenExclJet_12345.Draw("PSAME");}
		if(_RecoCutFlags[6] != "_1"){
			RelEff_GenExclJet_123456.SetLineColor(9);
			RelEff_GenExclJet_123456.SetMarkerStyle(25);
			RelEff_GenExclJet_123456.SetMarkerColor(9);
			RelEff_GenExclJet_123456.Draw("PSAME");}
		TLegend *LegREffJ = new TLegend(0.51,0.67,0.88,0.88);
		LegREffJ->SetFillColor(0);
		LegREffJ->SetBorderSize(0);
		LegREffJ->AddEntry(&RelEff_GenExclJet_1,"Acc","lp");
		LegREffJ->AddEntry(&RelEff_GenExclJet_12,"Trg","lp");
		LegREffJ->AddEntry(&RelEff_GenExclJet_123,"Imp","lp");
		LegREffJ->AddEntry(&RelEff_GenExclJet_1234,"Conv","lp");
		LegREffJ->AddEntry(&RelEff_GenExclJet_12345,"Iso","lp");
		LegREffJ->AddEntry(&RelEff_GenExclJet_123456,"EiD","lp");
		LegREffJ->Draw();
		RelEff_GenExclJet_1.SetTitle("Cut Relative Efficiency vs Gen Excl Jet Number");
		REffJet->Write("Cut_RelativeEfficiency_VsGenExclJet.root");
		REffJet->Close();
		
		//Reco exclusive jet number
		
		R_RecoExclJet_Eff->cd();
		
        string RelEff_RecoExclJet_name="RelativeEff_MC";
        RelEff_RecoExclJet_name+=_RecoCutFlags[1].c_str();
        RelEff_RecoExclJet_1.SetNameTitle(RelEff_RecoExclJet_name.c_str(), RelEff_RecoExclJet_name.c_str());
        RelEff_RecoExclJet_name+=_RecoCutFlags[2].c_str();  
        RelEff_RecoExclJet_12.SetNameTitle(RelEff_RecoExclJet_name.c_str(), RelEff_RecoExclJet_name.c_str());
        RelEff_RecoExclJet_name+=_RecoCutFlags[3].c_str();  
        RelEff_RecoExclJet_123.SetNameTitle(RelEff_RecoExclJet_name.c_str(), RelEff_RecoExclJet_name.c_str());
        RelEff_RecoExclJet_name+=_RecoCutFlags[4].c_str();  
        RelEff_RecoExclJet_1234.SetNameTitle(RelEff_RecoExclJet_name.c_str(), RelEff_RecoExclJet_name.c_str());
        RelEff_RecoExclJet_name+=_RecoCutFlags[5].c_str();  
        RelEff_RecoExclJet_12345.SetNameTitle(RelEff_RecoExclJet_name.c_str(), RelEff_RecoExclJet_name.c_str());
        RelEff_RecoExclJet_name+=_RecoCutFlags[6].c_str();  
        RelEff_RecoExclJet_123456.SetNameTitle(RelEff_RecoExclJet_name.c_str(), RelEff_RecoExclJet_name.c_str());
        
        RelEff_RecoExclJet_1.Write();
        RelEff_RecoExclJet_12.Write();
        RelEff_RecoExclJet_123.Write();
        RelEff_RecoExclJet_1234.Write();
        RelEff_RecoExclJet_12345.Write();
        RelEff_RecoExclJet_123456.Write();
        
        TCanvas *REffRecoExclJet = new TCanvas;
		RelEff_RecoExclJet_1.GetYaxis()->SetRangeUser(0.3,1);
		RelEff_RecoExclJet_1.GetXaxis()->SetTitle("reco Exclusive Jet number");
		RelEff_RecoExclJet_1.SetMarkerStyle(20);
		RelEff_RecoExclJet_1.Draw("AP");
		if(_RecoCutFlags[2] != "_1"){
			RelEff_RecoExclJet_12.SetLineColor(2);
			RelEff_RecoExclJet_12.SetMarkerStyle(21);
			RelEff_RecoExclJet_12.SetMarkerColor(2);
			RelEff_RecoExclJet_12.Draw("PSAME");}
		if(_RecoCutFlags[3] != "_1"){
			RelEff_RecoExclJet_123.SetLineColor(3);
			RelEff_RecoExclJet_123.SetMarkerStyle(22);
			RelEff_RecoExclJet_123.SetMarkerColor(3);
			RelEff_RecoExclJet_123.Draw("PSAME");}
		if(_RecoCutFlags[4] != "_1"){
			RelEff_RecoExclJet_1234.SetLineColor(4);
			RelEff_RecoExclJet_1234.SetMarkerStyle(23);
			RelEff_RecoExclJet_1234.SetMarkerColor(4);
			RelEff_RecoExclJet_1234.Draw("PSAME");}
		if(_RecoCutFlags[5] != "_1"){
			RelEff_RecoExclJet_12345.SetLineColor(6);
			RelEff_RecoExclJet_12345.SetMarkerStyle(24);
			RelEff_RecoExclJet_12345.SetMarkerColor(6);
			RelEff_RecoExclJet_12345.Draw("PSAME");}
		if(_RecoCutFlags[6] != "_1"){
			RelEff_RecoExclJet_123456.SetLineColor(9);
			RelEff_RecoExclJet_123456.SetMarkerStyle(25);
			RelEff_RecoExclJet_123456.SetMarkerColor(9);
			RelEff_RecoExclJet_123456.Draw("PSAME");}
		TLegend *LegREffRecoExclJ = new TLegend(0.51,0.67,0.88,0.88);
		LegREffRecoExclJ->SetFillColor(0);
		LegREffRecoExclJ->SetBorderSize(0);
		LegREffRecoExclJ->AddEntry(&RelEff_RecoExclJet_1,"Acc","lp");
		LegREffRecoExclJ->AddEntry(&RelEff_RecoExclJet_12,"Trg","lp");
		LegREffRecoExclJ->AddEntry(&RelEff_RecoExclJet_123,"Imp","lp");
		LegREffRecoExclJ->AddEntry(&RelEff_RecoExclJet_1234,"Conv","lp");
		LegREffRecoExclJ->AddEntry(&RelEff_RecoExclJet_12345,"Iso","lp");
		LegREffRecoExclJ->AddEntry(&RelEff_RecoExclJet_123456,"EiD","lp");	
		LegREffRecoExclJ->Draw();
		RelEff_RecoExclJet_1.SetTitle("Cut Relative Efficiency vs Reco Exclusive Jet Number");
		REffRecoExclJet->Write("Cut_RelativeEfficiency_VsRecoExclJet.root");
		REffRecoExclJet->Close();
		
		//Reco inclusive jet number
		
		R_RecoInclJet_Eff->cd();
		
        string RelEff_RecoInclJet_name="RelativeEff_MC";
        RelEff_RecoInclJet_name+=_RecoCutFlags[1].c_str();
        RelEff_RecoInclJet_1.SetNameTitle(RelEff_RecoInclJet_name.c_str(), RelEff_RecoInclJet_name.c_str());
        RelEff_RecoInclJet_name+=_RecoCutFlags[2].c_str();  
        RelEff_RecoInclJet_12.SetNameTitle(RelEff_RecoInclJet_name.c_str(), RelEff_RecoInclJet_name.c_str());
        RelEff_RecoInclJet_name+=_RecoCutFlags[3].c_str();  
        RelEff_RecoInclJet_123.SetNameTitle(RelEff_RecoInclJet_name.c_str(), RelEff_RecoInclJet_name.c_str());
        RelEff_RecoInclJet_name+=_RecoCutFlags[4].c_str();  
        RelEff_RecoInclJet_1234.SetNameTitle(RelEff_RecoInclJet_name.c_str(), RelEff_RecoInclJet_name.c_str());
        RelEff_RecoInclJet_name+=_RecoCutFlags[5].c_str();  
        RelEff_RecoInclJet_12345.SetNameTitle(RelEff_RecoInclJet_name.c_str(), RelEff_RecoInclJet_name.c_str());
        RelEff_RecoInclJet_name+=_RecoCutFlags[6].c_str();  
        RelEff_RecoInclJet_123456.SetNameTitle(RelEff_RecoInclJet_name.c_str(), RelEff_RecoInclJet_name.c_str());
        
        RelEff_RecoInclJet_1.Write();
        RelEff_RecoInclJet_12.Write();
        RelEff_RecoInclJet_123.Write();
        RelEff_RecoInclJet_1234.Write();
        RelEff_RecoInclJet_12345.Write();
        RelEff_RecoInclJet_123456.Write();
		
        effincl<<"Relative efficiency vs Reco Inclusive Jet number"<<endl;
        effincl<<endl<<"Selection: Acc"<<endl;
        for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoInclJet_1.GetPoint(i,x,y);
			effincl<<"Jet# >= "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoInclJet_1.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoInclJet_1.GetErrorYlow(i)<<endl;}
        
        effincl<<endl<<"Selection: Trg"<<endl;
        for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoInclJet_12.GetPoint(i,x,y);
			effincl<<"Jet# >= "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoInclJet_12.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoInclJet_12.GetErrorYlow(i)<<endl;}
        
        effincl<<endl<<"Selection: Imp"<<endl;
        for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoInclJet_123.GetPoint(i,x,y);
			effincl<<"Jet# >= "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoInclJet_123.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoInclJet_123.GetErrorYlow(i)<<endl;}
        
        effincl<<endl<<"Selection: Conv"<<endl;
        for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoInclJet_1234.GetPoint(i,x,y);
			effincl<<"Jet# >= "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoInclJet_1234.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoInclJet_1234.GetErrorYlow(i)<<endl;}
        
        effincl<<endl<<"Selection: Iso"<<endl;
        for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoInclJet_12345.GetPoint(i,x,y);
			effincl<<"Jet# >= "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoInclJet_12345.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoInclJet_12345.GetErrorYlow(i)<<endl;}
		
		effincl<<endl<<"Selection: EiD"<<endl;
		for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoInclJet_123456.GetPoint(i,x,y);
			effincl<<"Jet# >= "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoInclJet_123456.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoInclJet_123456.GetErrorYlow(i)<<endl;}
        
        effexcl<<"Relative efficiency vs Reco Exclusive Jet number"<<endl;
        effexcl<<endl<<"Selection: Acc"<<endl;
        for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoExclJet_1.GetPoint(i,x,y);
			effexcl<<"Jet# = "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoExclJet_1.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoExclJet_1.GetErrorYlow(i)<<endl;}
        
        effexcl<<endl<<"Selection: Trg"<<endl;
        for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoExclJet_12.GetPoint(i,x,y);
			effexcl<<"Jet# = "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoExclJet_12.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoExclJet_12.GetErrorYlow(i)<<endl;}
        
        effexcl<<endl<<"Selection: Imp"<<endl;
        for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoExclJet_123.GetPoint(i,x,y);
			effexcl<<"Jet# = "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoExclJet_123.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoExclJet_123.GetErrorYlow(i)<<endl;}
        
        effexcl<<endl<<"Selection: Conv"<<endl;
        for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoExclJet_1234.GetPoint(i,x,y);
			effexcl<<"Jet# = "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoExclJet_1234.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoExclJet_1234.GetErrorYlow(i)<<endl;}
        
        effexcl<<endl<<"Selection: Iso"<<endl;
        for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoExclJet_12345.GetPoint(i,x,y);
			effexcl<<"Jet# = "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoExclJet_12345.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoExclJet_12345.GetErrorYlow(i)<<endl;}
		
		effexcl<<endl<<"Selection: EiD"<<endl;
		for(int i=0; i<7; i++){
			double x = 0., y = 0.;
			RelEff_RecoExclJet_123456.GetPoint(i,x,y);
			effexcl<<"Jet# = "<<i<<" Relative Eff. = "<<y<<" Error High = "<<RelEff_RecoExclJet_123456.GetErrorYhigh(i)<<" Error Low = "<<RelEff_RecoExclJet_123456.GetErrorYlow(i)<<endl;}
        
        
		TCanvas *REffRecoInclJet = new TCanvas;
		RelEff_RecoInclJet_1.GetYaxis()->SetRangeUser(0.3,1);
		RelEff_RecoInclJet_1.GetXaxis()->SetTitle("reco Inclusive Jet number");
		RelEff_RecoInclJet_1.SetMarkerStyle(20);
		RelEff_RecoInclJet_1.Draw("AP");
		if(_RecoCutFlags[2] != "_1"){
			RelEff_RecoInclJet_12.SetLineColor(2);
			RelEff_RecoInclJet_12.SetMarkerStyle(21);
			RelEff_RecoInclJet_12.SetMarkerColor(2);
			RelEff_RecoInclJet_12.Draw("PSAME");}
		if(_RecoCutFlags[3] != "_1"){
			RelEff_RecoInclJet_123.SetLineColor(3);
			RelEff_RecoInclJet_123.SetMarkerStyle(22);
			RelEff_RecoInclJet_123.SetMarkerColor(3);
			RelEff_RecoInclJet_123.Draw("PSAME");}
		if(_RecoCutFlags[4] != "_1"){
			RelEff_RecoInclJet_1234.SetLineColor(4);
			RelEff_RecoInclJet_1234.SetMarkerStyle(23);
			RelEff_RecoInclJet_1234.SetMarkerColor(4);
			RelEff_RecoInclJet_1234.Draw("PSAME");}
		if(_RecoCutFlags[5] != "_1"){
			RelEff_RecoInclJet_12345.SetLineColor(6);
			RelEff_RecoInclJet_12345.SetMarkerStyle(24);
			RelEff_RecoInclJet_12345.SetMarkerColor(6);
			RelEff_RecoInclJet_12345.Draw("PSAME");}
		if(_RecoCutFlags[6] != "_1"){
			RelEff_RecoInclJet_123456.SetLineColor(9);
			RelEff_RecoInclJet_123456.SetMarkerStyle(25);
			RelEff_RecoInclJet_123456.SetMarkerColor(9);
			RelEff_RecoInclJet_123456.Draw("PSAME");}
		TLegend *LegREffRecoInclJ = new TLegend(0.51,0.67,0.88,0.88);
		LegREffRecoInclJ->SetFillColor(0);
		LegREffRecoInclJ->SetBorderSize(0);
		LegREffRecoInclJ->AddEntry(&RelEff_RecoInclJet_1,"Acc","lp");
		LegREffRecoInclJ->AddEntry(&RelEff_RecoInclJet_12,"Trg","lp");
		LegREffRecoInclJ->AddEntry(&RelEff_RecoInclJet_123,"Imp","lp");
		LegREffRecoInclJ->AddEntry(&RelEff_RecoInclJet_1234,"Conv","lp");
		LegREffRecoInclJ->AddEntry(&RelEff_RecoInclJet_12345,"Iso","lp");
		LegREffRecoInclJ->AddEntry(&RelEff_RecoInclJet_123456,"EiD","lp");	
		LegREffRecoInclJ->Draw();
		RelEff_RecoInclJet_1.SetTitle("Cut Relative Efficiency vs Reco Inclusive Jet Number");
		REffRecoInclJet->Write("Cut_RelativeEfficiency_VsRecoInclJet.root");
		REffRecoInclJet->Close();
		
	}
	
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
	
	TH1D* TP_numerator_12 = (TH1D*) signal_file->Get(num_TP.c_str());
	TH1D* TP_denominator_12 = (TH1D*) signal_file->Get(den_TP.c_str());
	
	TPDir+=_RecoCutFlags[3].c_str();    
	TPHisto+=_RecoCutFlags[3].c_str(); 
	num_TP = TPDir+TPHisto+"_SingleCandTag&Probe_numerator";
	den_TP = TPDir+TPHisto+"_SingleCandTag&Probe_denominator";
	
	TH1D* TP_numerator_123 = (TH1D*) signal_file->Get(num_TP.c_str());
	TH1D* TP_denominator_123 = (TH1D*) signal_file->Get(den_TP.c_str());
	
	TPDir+=_RecoCutFlags[4].c_str();    
	TPHisto+=_RecoCutFlags[4].c_str(); 
	num_TP = TPDir+TPHisto+"_SingleCandTag&Probe_numerator";
	den_TP = TPDir+TPHisto+"_SingleCandTag&Probe_denominator";
	
	TH1D* TP_numerator_1234 = (TH1D*) signal_file->Get(num_TP.c_str());
	TH1D* TP_denominator_1234 = (TH1D*) signal_file->Get(den_TP.c_str());
	
	TPDir+=_RecoCutFlags[5].c_str();    
	TPHisto+=_RecoCutFlags[5].c_str(); 
	num_TP = TPDir+TPHisto+"_SingleCandTag&Probe_numerator";
	den_TP = TPDir+TPHisto+"_SingleCandTag&Probe_denominator";
	
	TH1D* TP_numerator_12345 = (TH1D*) signal_file->Get(num_TP.c_str());
	TH1D* TP_denominator_12345 = (TH1D*) signal_file->Get(den_TP.c_str());
	
	TPDir+=_RecoCutFlags[6].c_str();    
	TPHisto+=_RecoCutFlags[6].c_str(); 
	num_TP = TPDir+TPHisto+"_SingleCandTag&Probe_numerator";
	den_TP = TPDir+TPHisto+"_SingleCandTag&Probe_denominator";
	
	TH1D* TP_numerator_123456 = (TH1D*) signal_file->Get(num_TP.c_str());
	TH1D* TP_denominator_123456 = (TH1D*) signal_file->Get(den_TP.c_str());
	
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
        
        TH1D* TP_numerator1_12 = (TH1D*) signal_file->Get(num_TP1.c_str());
        TH1D* TP_denominator1_12 = (TH1D*) signal_file->Get(den_TP1.c_str());
        
        TPDir1+=_RecoCutFlags[3].c_str();    
        TPHisto+=_RecoCutFlags[3].c_str(); 
        num_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator1_123 = (TH1D*) signal_file->Get(num_TP1.c_str());
        TH1D* TP_denominator1_123 = (TH1D*) signal_file->Get(den_TP1.c_str());
        
        TPDir1+=_RecoCutFlags[4].c_str();    
        TPHisto+=_RecoCutFlags[4].c_str(); 
        num_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_denominator";
		
        TH1D* TP_numerator1_1234 = (TH1D*) signal_file->Get(num_TP1.c_str());
        TH1D* TP_denominator1_1234 = (TH1D*) signal_file->Get(den_TP1.c_str());
        
        TPDir1+=_RecoCutFlags[5].c_str();    
        TPHisto+=_RecoCutFlags[5].c_str(); 
        num_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator1_12345 = (TH1D*) signal_file->Get(num_TP1.c_str());
        TH1D* TP_denominator1_12345 = (TH1D*) signal_file->Get(den_TP1.c_str());
        
        TPDir1+=_RecoCutFlags[6].c_str();    
        TPHisto+=_RecoCutFlags[6].c_str(); 
        num_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator1_123456 = (TH1D*) signal_file->Get(num_TP1.c_str());
        TH1D* TP_denominator1_123456 = (TH1D*) signal_file->Get(den_TP1.c_str());
        
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
	if(sample=="mc"){
        TCanvas *EffTP_12 = new TCanvas;
        RelEff_GenExclJet_12.SetLineColor(1);
		RelEff_GenExclJet_12.SetMarkerStyle(20);
		RelEff_GenExclJet_12.SetMarkerColor(1);
		RelEff_GenExclJet_12.Draw("AP");
		Eff_TP_Double_12.SetLineColor(2);
		Eff_TP_Double_12.SetMarkerStyle(21);
		Eff_TP_Double_12.SetMarkerColor(2);
		Eff_TP_Double_12.Draw("PSAME");
		RelEff_GenExclJet_12.SetTitle("Cut eff: MC (black), TP w/o back (red)");
		string EffTP_12_name = "EffGenRecoVsTP";
		EffTP_12_name+=_RecoCutFlags[1].c_str();
        EffTP_12_name+=_RecoCutFlags[2].c_str();
        EffTP_12_name+=".root";
		EffTP_12->Write(EffTP_12_name.c_str());
		EffTP_12->Close();
	}
	
	TP_12->cd();
	if(sample=="mc"){
        TCanvas *EffTP_12 = new TCanvas;
        RelEff_RecoExclJet_12.SetLineColor(1);
		RelEff_RecoExclJet_12.SetMarkerStyle(20);
		RelEff_RecoExclJet_12.SetMarkerColor(1);
		RelEff_RecoExclJet_12.Draw("AP");
		Eff_TP_Double_12.Draw("PSAME");
		RelEff_RecoExclJet_12.SetTitle("Cut eff vs Reco Jet: MC (black), TP w/o back (red)");
		string EffTP_12_name = "EffRecoJetMCVsTP";
		EffTP_12_name+=_RecoCutFlags[1].c_str();
        EffTP_12_name+=_RecoCutFlags[2].c_str();
        EffTP_12_name+=".root";
		EffTP_12->Write(EffTP_12_name.c_str());
		EffTP_12->Close();
	}
	
	TP_123->cd();
	Eff_TP_Single_123.Write();
	Eff_TP_Double_123.Write();
	if(sample=="mc"){
        TCanvas *EffTP_123 = new TCanvas;
        RelEff_GenExclJet_123.SetLineColor(1);
		RelEff_GenExclJet_123.SetMarkerStyle(20);
		RelEff_GenExclJet_123.SetMarkerColor(1);
		RelEff_GenExclJet_123.Draw("AP");
		Eff_TP_Double_123.SetLineColor(2);
		Eff_TP_Double_123.SetMarkerStyle(21);
		Eff_TP_Double_123.SetMarkerColor(2);
		Eff_TP_Double_123.Draw("PSAME");
		RelEff_GenExclJet_123.SetTitle("Cut eff: MC (black), TP w/o back (red)");
		string EffTP_123_name = "EffGenRecoVsTP";
		EffTP_123_name+=_RecoCutFlags[1].c_str();
        EffTP_123_name+=_RecoCutFlags[2].c_str();
        EffTP_123_name+=_RecoCutFlags[3].c_str();
        EffTP_123_name+=".root";
		EffTP_123->Write(EffTP_123_name.c_str());
		EffTP_123->Close();
	}
	
	TP_123->cd();
	if(sample=="mc"){
        TCanvas *EffTP_123 = new TCanvas;
        RelEff_RecoExclJet_123.SetLineColor(1);
		RelEff_RecoExclJet_123.SetMarkerStyle(20);
		RelEff_RecoExclJet_123.SetMarkerColor(1);
		RelEff_RecoExclJet_123.GetXaxis()->SetRangeUser(-0.5,3.5);
		RelEff_RecoExclJet_123.GetYaxis()->SetRangeUser(0.95,0.99);
		RelEff_RecoExclJet_123.Draw("AP");
		Eff_TP_Double_123.Draw("PSAME");
		RelEff_RecoExclJet_123.SetTitle("Cut eff vs RECO Excl Jet: MC (black), TP w/o back (red)");
		string EffTP_123_name = "EffRecoJetMCVsTP";
		EffTP_123_name+=_RecoCutFlags[1].c_str();
        EffTP_123_name+=_RecoCutFlags[2].c_str();
        EffTP_123_name+=_RecoCutFlags[3].c_str();
        EffTP_123_name+=".root";
		EffTP_123->Write(EffTP_123_name.c_str());
		EffTP_123->Close();
	}
	
	TP_1234->cd();
	Eff_TP_Single_1234.Write();
	Eff_TP_Double_1234.Write();
	if(sample=="mc"){
        TCanvas *EffTP_1234 = new TCanvas;
        RelEff_GenExclJet_1234.SetLineColor(1);
		RelEff_GenExclJet_1234.SetMarkerStyle(20);
		RelEff_GenExclJet_1234.SetMarkerColor(1);
		RelEff_GenExclJet_1234.Draw("AP");
		Eff_TP_Double_1234.SetLineColor(2);
		Eff_TP_Double_1234.SetMarkerStyle(21);
		Eff_TP_Double_1234.SetMarkerColor(2);
		Eff_TP_Double_1234.Draw("PSAME");
		RelEff_GenExclJet_1234.SetTitle("Cut eff: MC (black), TP w/o back (red)");
		string EffTP_1234_name = "EffGenRecoVsTP";
		EffTP_1234_name+=_RecoCutFlags[1].c_str();
        EffTP_1234_name+=_RecoCutFlags[2].c_str();
        EffTP_1234_name+=_RecoCutFlags[3].c_str();
        EffTP_1234_name+=_RecoCutFlags[4].c_str();
        EffTP_1234_name+=".root";
		EffTP_1234->Write(EffTP_1234_name.c_str());
		EffTP_1234->Close();
	}
	
	TP_1234->cd();
	if(sample=="mc"){
        TCanvas *EffTP_1234 = new TCanvas;
        RelEff_RecoExclJet_1234.SetLineColor(1);
		RelEff_RecoExclJet_1234.SetMarkerStyle(20);
		RelEff_RecoExclJet_1234.SetMarkerColor(1);
		RelEff_RecoExclJet_1234.GetXaxis()->SetRangeUser(-0.5,3.5);
		RelEff_RecoExclJet_1234.GetYaxis()->SetRangeUser(0.84,0.93);
		RelEff_RecoExclJet_1234.Draw("AP");
		Eff_TP_Double_1234.Draw("PSAME");
		RelEff_RecoExclJet_1234.SetTitle("Cut eff vs RECO Excl #Jet: MC Reco (black), TP w/o back (red)");
		string EffTP_1234_name = "EffRecoJetMCVsTP";
		EffTP_1234_name+=_RecoCutFlags[1].c_str();
        EffTP_1234_name+=_RecoCutFlags[2].c_str();
        EffTP_1234_name+=_RecoCutFlags[3].c_str();
        EffTP_1234_name+=_RecoCutFlags[4].c_str();
        EffTP_1234_name+=".root";
		EffTP_1234->Write(EffTP_1234_name.c_str());
		EffTP_1234->Close();
	}
	
	TP_12345->cd();
	Eff_TP_Single_12345.Write();
	Eff_TP_Double_12345.Write();
	if(sample=="mc"){
        TCanvas *EffTP_12345 = new TCanvas;
        RelEff_GenExclJet_12345.SetLineColor(1);
		RelEff_GenExclJet_12345.SetMarkerStyle(20);
		RelEff_GenExclJet_12345.SetMarkerColor(1);
		RelEff_GenExclJet_12345.Draw("AP");
		Eff_TP_Double_12345.SetLineColor(2);
		Eff_TP_Double_12345.SetMarkerStyle(21);
		Eff_TP_Double_12345.SetMarkerColor(2);
		Eff_TP_Double_12345.Draw("PSAME");
		RelEff_GenExclJet_12345.SetTitle("Cut eff: MC (black), TP w/o back (red)");
		string EffTP_12345_name = "EffGenRecoVsTP";
		EffTP_12345_name+=_RecoCutFlags[1].c_str();
        EffTP_12345_name+=_RecoCutFlags[2].c_str();
        EffTP_12345_name+=_RecoCutFlags[3].c_str();
        EffTP_12345_name+=_RecoCutFlags[4].c_str();
        EffTP_12345_name+=_RecoCutFlags[5].c_str();
        EffTP_12345_name+=".root";
		EffTP_12345->Write(EffTP_12345_name.c_str());
		EffTP_12345->Close();
	}
	
	TP_12345->cd();
	if(sample=="mc"){
        TCanvas *EffTP_12345 = new TCanvas;
        RelEff_RecoExclJet_12345.SetLineColor(1);
		RelEff_RecoExclJet_12345.SetMarkerStyle(20);
		RelEff_RecoExclJet_12345.SetMarkerColor(1);
		RelEff_RecoExclJet_12345.GetXaxis()->SetRangeUser(-0.5,3.5);
		RelEff_RecoExclJet_12345.GetYaxis()->SetRangeUser(0.84,0.93);
		RelEff_RecoExclJet_12345.Draw("AP");
		Eff_TP_Double_12345.Draw("PSAME");
		RelEff_RecoExclJet_12345.SetTitle("Cut eff vs RECO Excl Jet: MC (black), TP w/o back (red)");
		string EffTP_12345_name = "EffRecoJetMCVsTP";
		EffTP_12345_name+=_RecoCutFlags[1].c_str();
        EffTP_12345_name+=_RecoCutFlags[2].c_str();
        EffTP_12345_name+=_RecoCutFlags[3].c_str();
        EffTP_12345_name+=_RecoCutFlags[4].c_str();
        EffTP_12345_name+=_RecoCutFlags[5].c_str();
        EffTP_12345_name+=".root";
		EffTP_12345->Write(EffTP_12345_name.c_str());
		EffTP_12345->Close();
	}
	
	TP_123456->cd();
	Eff_TP_Single_123456.Write();
	Eff_TP_Double_123456.Write();
	if(sample=="mc"){
        TCanvas *EffTP_123456 = new TCanvas;
        RelEff_GenExclJet_123456.SetLineColor(1);
		RelEff_GenExclJet_123456.SetMarkerStyle(20);
		RelEff_GenExclJet_123456.SetMarkerColor(1);
		RelEff_GenExclJet_123456.Draw("AP");
		Eff_TP_Double_123456.SetLineColor(2);
		Eff_TP_Double_123456.SetMarkerStyle(21);
		Eff_TP_Double_123456.SetMarkerColor(2);
		Eff_TP_Double_123456.Draw("PSAME");
		RelEff_GenExclJet_123456.SetTitle("Cut eff: MC (black), TP w/o back (red)");
		string EffTP_123456_name = "EffGenRecoVsTP";
		EffTP_123456_name+=_RecoCutFlags[1].c_str();
        EffTP_123456_name+=_RecoCutFlags[2].c_str();
        EffTP_123456_name+=_RecoCutFlags[3].c_str();
        EffTP_123456_name+=_RecoCutFlags[4].c_str();
        EffTP_123456_name+=_RecoCutFlags[5].c_str();
        EffTP_123456_name+=_RecoCutFlags[6].c_str();
        EffTP_123456_name+=".root";
		EffTP_123456->Write(EffTP_123456_name.c_str());
		EffTP_123456->Close();
	}
	
	TP_123456->cd();
	if(sample=="mc"){
        TCanvas *EffTP_123456 = new TCanvas;
		RelEff_RecoExclJet_123456.SetLineColor(1);
		RelEff_RecoExclJet_123456.SetMarkerStyle(20);
		RelEff_RecoExclJet_123456.SetMarkerColor(1);
		RelEff_RecoExclJet_123456.GetXaxis()->SetRangeUser(-0.5,3.5);
		RelEff_RecoExclJet_123456.GetYaxis()->SetRangeUser(0.84,0.93);
		RelEff_RecoExclJet_123456.Draw("AP");
		Eff_TP_Double_123456.Draw("PSAME");
		RelEff_RecoExclJet_123456.SetTitle("Cut eff vs RECO Excl Jet: MC (black), TP w/o back (red)");
		string EffTP_123456_name = "EffRecoJetMCVsTP";
		EffTP_123456_name+=_RecoCutFlags[1].c_str();
        EffTP_123456_name+=_RecoCutFlags[2].c_str();
        EffTP_123456_name+=_RecoCutFlags[3].c_str();
        EffTP_123456_name+=_RecoCutFlags[4].c_str();
        EffTP_123456_name+=_RecoCutFlags[5].c_str();
        EffTP_123456_name+=_RecoCutFlags[6].c_str();
        EffTP_123456_name+=".root";
		EffTP_123456->Write(EffTP_123456_name.c_str());
		EffTP_123456->Close();
	}
		
///////////////////// Distributions
	
	Dist->cd();
	
	string recMassZ_name = "RecoElectron/recZ_Plots/recMassZ";
	recMassZ_name+=_RecoCutFlags[1].c_str();
	TH1D* recMassZ_1 = (TH1D*) signal_file->Get(recMassZ_name.c_str());
	recMassZ_name+=_RecoCutFlags[2].c_str();
	TH1D* recMassZ_12 = (TH1D*) signal_file->Get(recMassZ_name.c_str());
	recMassZ_name+=_RecoCutFlags[3].c_str();
	TH1D* recMassZ_123 = (TH1D*) signal_file->Get(recMassZ_name.c_str());
	recMassZ_name+=_RecoCutFlags[4].c_str();
	TH1D* recMassZ_1234 = (TH1D*) signal_file->Get(recMassZ_name.c_str());
	recMassZ_name+=_RecoCutFlags[5].c_str();
	TH1D* recMassZ_12345 = (TH1D*) signal_file->Get(recMassZ_name.c_str());
	recMassZ_name+=_RecoCutFlags[6].c_str();
	TH1D* recMassZ_123456 = (TH1D*) signal_file->Get(recMassZ_name.c_str());
	
	TCanvas *ZMass = new TCanvas;
	recMassZ_1->SetLineColor(1);
	recMassZ_1->GetXaxis()->SetTitle("rec Z Mass");
	recMassZ_1->SetTitle("Reconstructed Z Mass vs cuts applied");
	recMassZ_1->Draw();
	if(_RecoCutFlags[2] != "_1"){
	recMassZ_12->SetLineColor(2);
	recMassZ_12->Draw("sames");}
	if(_RecoCutFlags[3] != "_1"){
	recMassZ_123->SetLineColor(3);
	recMassZ_123->Draw("sames");}
	if(_RecoCutFlags[4] != "_1"){
	recMassZ_1234->SetLineColor(4);
	recMassZ_1234->Draw("sames");}
	if(_RecoCutFlags[5] != "_1"){
	recMassZ_12345->SetLineColor(6);
	recMassZ_12345->Draw("sames");}
	if(_RecoCutFlags[6] != "_1"){
	recMassZ_123456->SetLineColor(9);
	recMassZ_123456->Draw("sames");}
	ZMass->Write("ZMass.root");
	ZMass->Close();
	
	string recPtZ_name = "RecoElectron/recZ_Plots/recPtZ";
	recPtZ_name+=_RecoCutFlags[1].c_str();
	TH1D* recPtZ_1 = (TH1D*) signal_file->Get(recPtZ_name.c_str());
	recPtZ_name+=_RecoCutFlags[2].c_str();
	TH1D* recPtZ_12 = (TH1D*) signal_file->Get(recPtZ_name.c_str());
	recPtZ_name+=_RecoCutFlags[3].c_str();
	TH1D* recPtZ_123 = (TH1D*) signal_file->Get(recPtZ_name.c_str());
	recPtZ_name+=_RecoCutFlags[4].c_str();
	TH1D* recPtZ_1234 = (TH1D*) signal_file->Get(recPtZ_name.c_str());
	recPtZ_name+=_RecoCutFlags[5].c_str();
	TH1D* recPtZ_12345 = (TH1D*) signal_file->Get(recPtZ_name.c_str());
	recPtZ_name+=_RecoCutFlags[6].c_str();
	TH1D* recPtZ_123456 = (TH1D*) signal_file->Get(recPtZ_name.c_str());
	
	TCanvas *ZPt = new TCanvas;
	recPtZ_1->SetLineColor(1);
	recPtZ_1->GetXaxis()->SetTitle("rec Z Pt");
	recPtZ_1->SetTitle("Reconstructed Z Pt vs cuts applied");
	recPtZ_1->Draw();
	if(_RecoCutFlags[2] != "_1"){
	recPtZ_12->SetLineColor(2);
	recPtZ_12->Draw("sames");}
	if(_RecoCutFlags[3] != "_1"){
	recPtZ_123->SetLineColor(3);
	recPtZ_123->Draw("sames");}
	if(_RecoCutFlags[4] != "_1"){
	recPtZ_1234->SetLineColor(4);
	recPtZ_1234->Draw("sames");}
	if(_RecoCutFlags[5] != "_1"){
	recPtZ_12345->SetLineColor(6);
	recPtZ_12345->Draw("sames");}
	if(_RecoCutFlags[6] != "_1"){
	recPtZ_123456->SetLineColor(9);
	recPtZ_123456->Draw("sames");}
	ZPt->Write("ZPt.root");
	ZPt->Close();
	
	string recEtaZ_name = "RecoElectron/recZ_Plots/recEtaZ";
	recEtaZ_name+=_RecoCutFlags[1].c_str();
	TH1D* recEtaZ_1 = (TH1D*) signal_file->Get(recEtaZ_name.c_str());
	recEtaZ_name+=_RecoCutFlags[2].c_str();
	TH1D* recEtaZ_12 = (TH1D*) signal_file->Get(recEtaZ_name.c_str());
	recEtaZ_name+=_RecoCutFlags[3].c_str();
	TH1D* recEtaZ_123 = (TH1D*) signal_file->Get(recEtaZ_name.c_str());
	recEtaZ_name+=_RecoCutFlags[4].c_str();
	TH1D* recEtaZ_1234 = (TH1D*) signal_file->Get(recEtaZ_name.c_str());
	recEtaZ_name+=_RecoCutFlags[5].c_str();
	TH1D* recEtaZ_12345 = (TH1D*) signal_file->Get(recEtaZ_name.c_str());
	recEtaZ_name+=_RecoCutFlags[6].c_str();
	TH1D* recEtaZ_123456 = (TH1D*) signal_file->Get(recEtaZ_name.c_str());
	
	TCanvas *ZEta = new TCanvas;
	recEtaZ_1->SetLineColor(1);
	recEtaZ_1->GetXaxis()->SetTitle("rec Z Eta");
	recEtaZ_1->SetTitle("Reconstructed Z Eta vs cuts applied");
	recEtaZ_1->Draw();
	if(_RecoCutFlags[2] != "_1"){
	recEtaZ_12->SetLineColor(2);
	recEtaZ_12->Draw("sames");}
	if(_RecoCutFlags[3] != "_1"){
	recEtaZ_123->SetLineColor(3);
	recEtaZ_123->Draw("sames");}
	if(_RecoCutFlags[4] != "_1"){
	recEtaZ_1234->SetLineColor(4);
	recEtaZ_1234->Draw("sames");}
	if(_RecoCutFlags[5] != "_1"){
	recEtaZ_12345->SetLineColor(6);
	recEtaZ_12345->Draw("sames");}
	if(_RecoCutFlags[6] != "_1"){
	recEtaZ_123456->SetLineColor(9);
	recEtaZ_123456->Draw("sames");}
	ZEta->Write("ZEta.root");
	ZEta->Close();
		
	string recExclJetN_name = "RecoElectron/recJet_Plots/IsoJetCounter";
	recExclJetN_name+=_RecoCutFlags[1].c_str();
	TH1D* recExclJetN_1 = (TH1D*) signal_file->Get(recExclJetN_name.c_str());
	recExclJetN_name+=_RecoCutFlags[2].c_str();
	TH1D* recExclJetN_12 = (TH1D*) signal_file->Get(recExclJetN_name.c_str());
	recExclJetN_name+=_RecoCutFlags[3].c_str();
	TH1D* recExclJetN_123 = (TH1D*) signal_file->Get(recExclJetN_name.c_str());
	recExclJetN_name+=_RecoCutFlags[4].c_str();
	TH1D* recExclJetN_1234 = (TH1D*) signal_file->Get(recExclJetN_name.c_str());
	recExclJetN_name+=_RecoCutFlags[5].c_str();
	TH1D* recExclJetN_12345 = (TH1D*) signal_file->Get(recExclJetN_name.c_str());
	recExclJetN_name+=_RecoCutFlags[6].c_str();
	TH1D* recExclJetN_123456 = (TH1D*) signal_file->Get(recExclJetN_name.c_str());

	TCanvas *JetN = new TCanvas;
	recExclJetN_1->SetLineColor(1);
	recExclJetN_1->GetXaxis()->SetTitle("rec Jet number");
	recExclJetN_1->SetTitle("Reconstructed Jet number vs cuts applied");
	recExclJetN_1->Draw();
	if(_RecoCutFlags[2] != "_1"){
	recExclJetN_12->SetLineColor(2);
	recExclJetN_12->Draw("sames");}
	if(_RecoCutFlags[3] != "_1"){
	recExclJetN_123->SetLineColor(3);
	recExclJetN_123->Draw("sames");}
	if(_RecoCutFlags[4] != "_1"){
	recExclJetN_1234->SetLineColor(4);
	recExclJetN_1234->Draw("sames");}
	if(_RecoCutFlags[5] != "_1"){
	recExclJetN_12345->SetLineColor(6);
	recExclJetN_12345->Draw("sames");}
	if(_RecoCutFlags[6] != "_1"){
	recExclJetN_123456->SetLineColor(9);
	recExclJetN_123456->Draw("sames");}
	JetN->Write("JetN.root");
	JetN->Close();

	if(sample=="mc"){
////////////////////////// Gen - Rec Plots
	GenRec->cd();
	
	TH1D* GENMassZ_Acc = (TH1D*) signal_file->Get("GenElectron/genZ_Plots/genMassZ_Acc");
	TH1D* GENPtZ_Acc = (TH1D*) signal_file->Get("GenElectron/genZ_Plots/genPtZ_Acc");
	TH1D* GENEtaZ_Acc = (TH1D*) signal_file->Get("GenElectron/genZ_Plots/genEtaZ_Acc");
	TH1D* GENExclJetN_Acc = (TH1D*) signal_file->Get("GenElectron/genJet_Plots/GenIsoJetCounter_Acc");
	
	TCanvas *GenRec_ZMass = new TCanvas;
	recMassZ_123456->SetLineColor(1);
	recMassZ_123456->GetXaxis()->SetTitle("Z Mass");
	recMassZ_123456->SetTitle("Reconstructed (black) and Generated (red) Z Mass");
	recMassZ_123456->Draw();
	GENMassZ_Acc->SetLineColor(2);
	GENMassZ_Acc->Draw("sames");
	GenRec_ZMass->Write("GenRec_ZMass.root");
	GenRec_ZMass->Close();
	
	TCanvas *GenRec_ZPt = new TCanvas;
	recPtZ_123456->SetLineColor(1);
	recPtZ_123456->GetXaxis()->SetTitle("Z Pt");
	recPtZ_123456->SetTitle("Reconstructed (black) and Generated (red) Z Pt");
	recPtZ_123456->Draw();
	GENPtZ_Acc->SetLineColor(2);
	GENPtZ_Acc->Draw("sames");
	GenRec_ZPt->Write("GenRec_ZPt.root");
	GenRec_ZPt->Close();
	
	TCanvas *GenRec_ZEta = new TCanvas;
	recEtaZ_123456->SetLineColor(1);
	recEtaZ_123456->GetXaxis()->SetTitle("Z Eta");
	recEtaZ_123456->SetTitle("Reconstructed (black) and Generated (red) Z Eta");
	recEtaZ_123456->Draw();
	GENEtaZ_Acc->SetLineColor(2);
	GENEtaZ_Acc->Draw("sames");
	GenRec_ZEta->Write("GenRec_ZEta.root");
	GenRec_ZEta->Close();
		
	TCanvas *GenRec_ExclJetN = new TCanvas;
	recExclJetN_123456->SetLineColor(1);
	recExclJetN_123456->GetXaxis()->SetTitle("Jet number");
	recExclJetN_123456->SetTitle("Reconstructed (black) and Generated (red) Exclusive Jet Number");
	recExclJetN_123456->Draw();
	GENExclJetN_Acc->SetLineColor(2);
	GENExclJetN_Acc->Draw("sames");
	GenRec_ExclJetN->Write("GenRec_ExclJetN.root");
	GenRec_ExclJetN->Close();
	
/////////////////////// Jet Isolation
		
	IsoJet->cd();
	
	TH1D* GENExclJetPt = (TH1D*) signal_file->Get("GenElectron/genJet_Plots/GenJetPt_Acc");
	TH1D* GENIsoJetPt = (TH1D*) signal_file->Get("GenElectron/genJet_Plots/GenIsoJetPt_Acc");
	
	string RECJetPt_name = "RecoElectron/recJet_Plots/RecoJetPt";
	RECJetPt_name+=_RecoCutFlags[1].c_str();
	RECJetPt_name+=_RecoCutFlags[2].c_str();
	RECJetPt_name+=_RecoCutFlags[3].c_str();
	RECJetPt_name+=_RecoCutFlags[4].c_str();
	RECJetPt_name+=_RecoCutFlags[5].c_str();
	RECJetPt_name+=_RecoCutFlags[6].c_str();
	TH1D* RECJetPt_123456 = (TH1D*) signal_file->Get(RECJetPt_name.c_str());
	
	string RECIsoJetPt_name = "RecoElectron/recJet_Plots/RecoIsoJetPt";
	RECIsoJetPt_name+=_RecoCutFlags[1].c_str();
	RECIsoJetPt_name+=_RecoCutFlags[2].c_str();
	RECIsoJetPt_name+=_RecoCutFlags[3].c_str();
	RECIsoJetPt_name+=_RecoCutFlags[4].c_str();
	RECIsoJetPt_name+=_RecoCutFlags[5].c_str();
	RECIsoJetPt_name+=_RecoCutFlags[6].c_str();
	TH1D* RECIsoJetPt_123456 = (TH1D*) signal_file->Get(RECIsoJetPt_name.c_str());
	
	TCanvas *AllJetPt = new TCanvas;
	RECJetPt_123456->GetXaxis()->SetTitle("Jet Pt");
	RECJetPt_123456->SetTitle("Reconstructed (black) and Generated (red) Jet Pt, NO jet-isolation cut applied");
	RECJetPt_123456->Draw();
	GENExclJetPt->SetLineColor(2);
	GENExclJetPt->Draw("sames");
	AllJetPt->Write("AllJetPt.root");
	AllJetPt->Close();
	
	TCanvas *IsoJetPt = new TCanvas;
	RECIsoJetPt_123456->GetXaxis()->SetTitle("Jet Pt");
	RECIsoJetPt_123456->SetTitle("Reconstructed (black) and Generated (red) Jet Pt, jet-isolation cut applied");
	RECIsoJetPt_123456->Draw();
	GENIsoJetPt->SetLineColor(2);
	GENIsoJetPt->Draw("sames");
	IsoJetPt->Write("IsoJetPt.root");
	IsoJetPt->Close();
	
	}
  
////////////////////////////////////////////////////////////////////          
	
	// Charge Misidentification
	
	ChargeMisID->cd();
	
        TH1D* AllEl_Pt_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_Pt_Acc");
	TH1D* AllEl_Eta_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_Eta_Acc");
	TH1D* AllEl_Hit_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_Hit_Acc");
	TH1D* AllEl_fBrem_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_fBrem_Acc");
	TH1D* AllEl_IP_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_IP_Acc");
	TH1D* AllEl_RecoExclJet_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_RecoExclJet_Acc");
	TH1D* AllEl_RecoInclJet_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/AllEl_RecoInclJet_Acc");
	TH1D* ChargeMisID_Pt_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_Pt_Acc");
	TH1D* ChargeMisID_Eta_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_Eta_Acc");
	TH1D* ChargeMisID_Hit_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_Hit_Acc");
	TH1D* ChargeMisID_fBrem_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_fBrem_Acc");
	TH1D* ChargeMisID_IP_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_IP_Acc");
	TH1D* ChargeMisID_RecoExclJet_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_RecoExclJet_Acc");
	TH1D* ChargeMisID_RecoInclJet_Acc = (TH1D*) signal_file->Get("RecoElectron/ChargeMisID/ChargeMisID_RecoInclJet_Acc");
	
		
	ChargeMisID_Eta_Acc->Divide(AllEl_Eta_Acc);
	ChargeMisID_Eta_Acc->GetXaxis()->SetTitle("gen Eta");
	ChargeMisID_Eta_Acc->SetName("ChargeMisID_Eta");
	ChargeMisID_Eta_Acc->Write();
	ChargeMisID_Pt_Acc->Divide(AllEl_Pt_Acc);
	ChargeMisID_Pt_Acc->GetXaxis()->SetTitle("gen Pt");
	ChargeMisID_Pt_Acc->SetName("ChargeMisID_Pt");
	ChargeMisID_Pt_Acc->Write();
	ChargeMisID_Hit_Acc->Divide(AllEl_Hit_Acc);
	ChargeMisID_Hit_Acc->GetXaxis()->SetTitle("Number of Hits on track");
	ChargeMisID_Hit_Acc->SetName("ChargeMisID_Hit");
	ChargeMisID_Hit_Acc->Write();
	ChargeMisID_IP_Acc->Divide(AllEl_IP_Acc);
	ChargeMisID_IP_Acc->GetXaxis()->SetTitle("Impact Parameter");
	ChargeMisID_IP_Acc->SetName("ChargeMisID_IP");
	ChargeMisID_IP_Acc->Write();
	ChargeMisID_fBrem_Acc->Divide(AllEl_fBrem_Acc);
	ChargeMisID_fBrem_Acc->GetXaxis()->SetTitle("fBrem");
	ChargeMisID_fBrem_Acc->SetName("ChargeMisID_fBrem");
	ChargeMisID_fBrem_Acc->Write();
	ChargeMisID_RecoExclJet_Acc->Divide(AllEl_RecoExclJet_Acc);
	ChargeMisID_RecoExclJet_Acc->GetXaxis()->SetTitle("RecoExclJet");
	ChargeMisID_RecoExclJet_Acc->SetName("ChargeMisID_RecoExclJet");
	ChargeMisID_RecoExclJet_Acc->Write();
	ChargeMisID_RecoInclJet_Acc->Divide(AllEl_RecoInclJet_Acc);
	ChargeMisID_RecoInclJet_Acc->GetXaxis()->SetTitle("RecoInclJet");
	ChargeMisID_RecoInclJet_Acc->SetName("ChargeMisID_RecoInclJet");
	ChargeMisID_RecoInclJet_Acc->Write();	
	
	effincl.close();	

  outplots->Write();
  outplots->Close();
}
