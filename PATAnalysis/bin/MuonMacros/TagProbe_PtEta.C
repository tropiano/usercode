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

//EtaRegion: "barrel" = Barrel EtaRegion; "endcap" = Endcap EtaRegion
//Selections: "SYM" = V+jets selections (old); "ASYM" = Vector Boson Task Force (new)

void TPPlots(string selections, string EtaRegion){

        bool Fits = true;
        
        //MonteCarlo Signal file used to evaluate the Rel. MC Rel Eff. and the TP Eff. w/o background
        TFile *signal_MC_file = TFile::Open("/data/sfrosali/Zjets/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/MonteCarlo/PtEta/Train/DYtoLL_PtEta_Train.root");
        
        TFile *tp_12, *tp_123, *tp_1234, *tp_12345, *tp_123456, *tp_global;
        
        if(Fits){
        //TP files (outputs of TagAndProbeAnalyzer) whit the mass fits
        tp_12 = TFile::Open("/data/sfrosali/Zjets/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/TagProbe/PtEta/TP_PseudoData_PtEta_Iso.root");
        tp_123 = TFile::Open("/data/sfrosali/Zjets/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/TagProbe/PtEta/TP_PseudoData_PtEta_Iso.root");
        tp_1234 = TFile::Open("/data/sfrosali/Zjets/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/TagProbe/PtEta/TP_PseudoData_PtEta_Iso.root");
        tp_12345 = TFile::Open("/data/sfrosali/Zjets/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/TagProbe/PtEta/TP_PseudoData_PtEta_Iso.root");
        tp_123456 = TFile::Open("/data/sfrosali/Zjets/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/TagProbe/PtEta/TP_PseudoData_PtEta_Iso.root");
        tp_global = TFile::Open("/data/sfrosali/Zjets/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/TagProbe/PtEta/TP_PseudoData_PtEta_Global.root");
        }
        
        //Output
        string out = "TPStudy_DYtoLL_PtEta_wFits";
        string output = out+"_"+EtaRegion+".root";
        TFile* outplots = new TFile(output.c_str(), "RECREATE");
        
        ofstream tpr;
	string outtpr="TPReport_DYtoLL_PtEta_wFits";
	outtpr+="_"+EtaRegion+".txt";
	tpr.open(outtpr.c_str());
	tpr<<endl;
        
        double bin=3;
        double xmin, xmax, ymin, ymax;
        
        xmin=5;
        xmax=235;
        
        ymin=0.7;
        ymax=1.05;
        
        int cut_decimal = 10000;
	
	int _Acc  = 1;
	int _Trg  = 0;	
	int _Imp  = 2;
	int _Qual = 3;
	int _Iso  = 4;
	int _MuID  = 0;
	
	string _RecoCutFlags[7];
	
	for(int i=0; i<7; i++){
       _RecoCutFlags[i] = "_1";}
   
       if(selections=="SYM"){
       _RecoCutFlags[_Acc] =  "_AccSYM";
       _RecoCutFlags[_Qual] = "_QualSYM";
       _RecoCutFlags[_Iso] =  "_IsoSYM";
       _RecoCutFlags[_MuID] =  "_MuIDSYM";}
       if(selections=="ASYM"){
       _RecoCutFlags[_Acc] =  "_AccASYM";
       _RecoCutFlags[_Qual] = "_QualASYM";
       _RecoCutFlags[_Iso] =  "_IsoASYM";
       _RecoCutFlags[_MuID] =  "_MuIDASYM";}
     
       _RecoCutFlags[_Trg] =  "_Trg";     
       _RecoCutFlags[_Imp] =  "_Imp";
    
        if(!signal_MC_file){
	tpr<<"Error! Input files doesn't exist!"<<endl;
	return;
	}
  
  TVectorD ptBin(10);
        
  for(int i=0; i<11; i++)ptBin[i] = -1;  

  ptBin[0] = 20;		//GeV
  ptBin[1] = 50;		//GeV
  ptBin[2] = 100;		//GeV
  ptBin[3] = 200;		//GeV

TDirectory *REff, *REff_Barrel, *REff_Endcap, *TP, *TP_12, *TP_123, *TP_1234, *TP_12345, *TP_123456, *TP_global;

REff      = outplots->mkdir("Relative_Efficiency");
REff_Barrel = REff->mkdir("Relative_Efficiency_Barrel");
REff_Endcap = REff->mkdir("Relative_Efficiency_Endcap");

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

TP_global = TP->mkdir("Tag&Probe_Global");
	
   /////////////Efficiency - MC
   //Denominators
	
   TH1D* RelMCEffDenom_Barrel = (TH1D*) signal_MC_file->Get("EfficiencyMuon/EffDenom_GenSingleMuPt_Barrel");
   TH1D* RelMCEffDenom_Endcap = (TH1D*) signal_MC_file->Get("EfficiencyMuon/EffDenom_GenSingleMuPt_Endcap");

	string MCEff_Barrel_name;
	MCEff_Barrel_name = "EfficiencyMuon/RecoSingleMuPtEff_Barrel";
	
	MCEff_Barrel_name+=_RecoCutFlags[1].c_str();
        TH1D* SingleMuPtEff_Barrel_1 = (TH1D*) signal_MC_file->Get(MCEff_Barrel_name.c_str());
	MCEff_Barrel_name+=_RecoCutFlags[2].c_str();
        TH1D* SingleMuPtEff_Barrel_12 = (TH1D*) signal_MC_file->Get(MCEff_Barrel_name.c_str());
	MCEff_Barrel_name+=_RecoCutFlags[3].c_str();
	TH1D* SingleMuPtEff_Barrel_123 = (TH1D*) signal_MC_file->Get(MCEff_Barrel_name.c_str());
	MCEff_Barrel_name+=_RecoCutFlags[4].c_str();
	TH1D* SingleMuPtEff_Barrel_1234 = (TH1D*) signal_MC_file->Get(MCEff_Barrel_name.c_str());
	MCEff_Barrel_name+=_RecoCutFlags[5].c_str();
	TH1D* SingleMuPtEff_Barrel_12345 = (TH1D*) signal_MC_file->Get(MCEff_Barrel_name.c_str());
	MCEff_Barrel_name+=_RecoCutFlags[6].c_str();
	TH1D* SingleMuPtEff_Barrel_123456 = (TH1D*) signal_MC_file->Get(MCEff_Barrel_name.c_str());
	
	string MCEff_Endcap_name;
	MCEff_Endcap_name = "EfficiencyMuon/RecoSingleMuPtEff_Endcap";
	
	MCEff_Endcap_name+=_RecoCutFlags[1].c_str();
        TH1D* SingleMuPtEff_Endcap_1 = (TH1D*) signal_MC_file->Get(MCEff_Endcap_name.c_str());
	MCEff_Endcap_name+=_RecoCutFlags[2].c_str();
        TH1D* SingleMuPtEff_Endcap_12 = (TH1D*) signal_MC_file->Get(MCEff_Endcap_name.c_str());
	MCEff_Endcap_name+=_RecoCutFlags[3].c_str();
	TH1D* SingleMuPtEff_Endcap_123 = (TH1D*) signal_MC_file->Get(MCEff_Endcap_name.c_str());
	MCEff_Endcap_name+=_RecoCutFlags[4].c_str();
	TH1D* SingleMuPtEff_Endcap_1234 = (TH1D*) signal_MC_file->Get(MCEff_Endcap_name.c_str());
	MCEff_Endcap_name+=_RecoCutFlags[5].c_str();
	TH1D* SingleMuPtEff_Endcap_12345 = (TH1D*) signal_MC_file->Get(MCEff_Endcap_name.c_str());
	MCEff_Endcap_name+=_RecoCutFlags[6].c_str();
	TH1D* SingleMuPtEff_Endcap_123456 = (TH1D*) signal_MC_file->Get(MCEff_Endcap_name.c_str());

        //Relative Efficiency from MC
             
        REff_Barrel->cd();
            
        TGraphAsymmErrors MCRelEff_Barrel_1(SingleMuPtEff_Barrel_1, RelMCEffDenom_Barrel);
        TGraphAsymmErrors MCRelEff_Barrel_12(SingleMuPtEff_Barrel_12, SingleMuPtEff_Barrel_1);
        TGraphAsymmErrors MCRelEff_Barrel_123(SingleMuPtEff_Barrel_123, SingleMuPtEff_Barrel_12);
        TGraphAsymmErrors MCRelEff_Barrel_1234(SingleMuPtEff_Barrel_1234, SingleMuPtEff_Barrel_123);
        TGraphAsymmErrors MCRelEff_Barrel_12345(SingleMuPtEff_Barrel_12345, SingleMuPtEff_Barrel_1234);
        TGraphAsymmErrors MCRelEff_Barrel_123456(SingleMuPtEff_Barrel_123456, SingleMuPtEff_Barrel_12345);
      
        string MCRelEff_Barrel_name="MCRelativeEff";
        MCRelEff_Barrel_name+=_RecoCutFlags[1].c_str();
        MCRelEff_Barrel_1.SetNameTitle(MCRelEff_Barrel_name.c_str(), MCRelEff_Barrel_name.c_str());
        MCRelEff_Barrel_name+=_RecoCutFlags[2].c_str();  
        MCRelEff_Barrel_12.SetNameTitle(MCRelEff_Barrel_name.c_str(), MCRelEff_Barrel_name.c_str());
        MCRelEff_Barrel_name+=_RecoCutFlags[3].c_str();  
        MCRelEff_Barrel_123.SetNameTitle(MCRelEff_Barrel_name.c_str(), MCRelEff_Barrel_name.c_str());
        MCRelEff_Barrel_name+=_RecoCutFlags[4].c_str();  
        MCRelEff_Barrel_1234.SetNameTitle(MCRelEff_Barrel_name.c_str(), MCRelEff_Barrel_name.c_str());
        MCRelEff_Barrel_name+=_RecoCutFlags[5].c_str();  
        MCRelEff_Barrel_12345.SetNameTitle(MCRelEff_Barrel_name.c_str(), MCRelEff_Barrel_name.c_str());
        MCRelEff_Barrel_name+=_RecoCutFlags[6].c_str();  
        MCRelEff_Barrel_123456.SetNameTitle(MCRelEff_Barrel_name.c_str(), MCRelEff_Barrel_name.c_str());
        
        MCRelEff_Barrel_1.Write();
        MCRelEff_Barrel_12.Write();
        MCRelEff_Barrel_123.Write();
        MCRelEff_Barrel_1234.Write();
        MCRelEff_Barrel_123456.Write();
        
        TVectorD GraphVx(bin);
        TVectorD GraphVy(bin);
        TVectorD GraphVexl(bin);
        TVectorD GraphVexh(bin);
        TVectorD GraphVeyl(bin);
        TVectorD GraphVeyh(bin);
  
        double xg, yg; 
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Barrel_1.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Barrel_1.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Barrel_1.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors MCRelEffGraph_Barrel_1(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Barrel_1.SetNameTitle("MCRelEffGraph_Barrel_1", "MCRelEffGraph_Barrel_1");
        MCRelEffGraph_Barrel_1.Write();  
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Barrel_12.GetPoint(i, xg, yg);
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Barrel_12.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Barrel_12.GetErrorYhigh(i);      
        }
        
        TGraphAsymmErrors MCRelEffGraph_Barrel_12(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Barrel_12.SetNameTitle("MCRelEffGraph_Barrel_12", "MCRelEffGraph_Barrel_12");
        MCRelEffGraph_Barrel_12.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Barrel_123.GetPoint(i, xg, yg);
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Barrel_123.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Barrel_123.GetErrorYhigh(i);      
        }
        
        TGraphAsymmErrors MCRelEffGraph_Barrel_123(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Barrel_123.SetNameTitle("MCRelEffGraph_Barrel_123", "MCRelEffGraph_Barrel_123");
        MCRelEffGraph_Barrel_123.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Barrel_1234.GetPoint(i, xg, yg);
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Barrel_1234.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Barrel_1234.GetErrorYhigh(i);      
        }
        
        TGraphAsymmErrors MCRelEffGraph_Barrel_1234(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Barrel_1234.SetNameTitle("MCRelEffGraph_Barrel_1234", "MCRelEffGraph_Barrel_1234");
        MCRelEffGraph_Barrel_1234.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Barrel_12345.GetPoint(i, xg, yg);
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Barrel_12345.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Barrel_12345.GetErrorYhigh(i);      
        }
        
        TGraphAsymmErrors MCRelEffGraph_Barrel_12345(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Barrel_12345.SetNameTitle("MCRelEffGraph_Barrel_12345", "MCRelEffGraph_Barrel_12345");
        MCRelEffGraph_Barrel_12345.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Barrel_123456.GetPoint(i, xg, yg);
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Barrel_123456.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Barrel_123456.GetErrorYhigh(i);      
        }
        
        TGraphAsymmErrors MCRelEffGraph_Barrel_123456(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Barrel_123456.SetNameTitle("MCRelEffGraph_Barrel_123456", "MCRelEffGraph_Barrel_123456");
        MCRelEffGraph_Barrel_123456.Write(); 
               
	TCanvas *REffBarrel = new TCanvas;
	MCRelEffGraph_Barrel_1.GetYaxis()->SetRangeUser(0.6,1);
	MCRelEffGraph_Barrel_1.GetXaxis()->SetTitle("muon pt");
	MCRelEffGraph_Barrel_1.SetMarkerStyle(20);
	MCRelEffGraph_Barrel_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	MCRelEffGraph_Barrel_12.SetLineColor(2);
	MCRelEffGraph_Barrel_12.SetMarkerStyle(21);
	MCRelEffGraph_Barrel_12.SetMarkerColor(2);
	MCRelEffGraph_Barrel_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	MCRelEffGraph_Barrel_123.SetLineColor(3);
	MCRelEffGraph_Barrel_123.SetMarkerStyle(22);
	MCRelEffGraph_Barrel_123.SetMarkerColor(3);
        MCRelEffGraph_Barrel_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	MCRelEffGraph_Barrel_1234.SetLineColor(4);
	MCRelEffGraph_Barrel_1234.SetMarkerStyle(23);
	MCRelEffGraph_Barrel_1234.SetMarkerColor(4);
	MCRelEffGraph_Barrel_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
        MCRelEffGraph_Barrel_12345.SetLineColor(5);
	MCRelEffGraph_Barrel_12345.SetMarkerStyle(24);
	MCRelEffGraph_Barrel_12345.SetMarkerColor(5);
	MCRelEffGraph_Barrel_12345.Draw("PSAME");}
	if(_RecoCutFlags[6] != "_1"){
        MCRelEffGraph_Barrel_123456.SetLineColor(6);
	MCRelEffGraph_Barrel_123456.SetMarkerStyle(25);
	MCRelEffGraph_Barrel_123456.SetMarkerColor(6);
	MCRelEffGraph_Barrel_123456.Draw("PSAME");}
	TLegend *LegREffBarrel = REffBarrel->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegREffBarrel->Draw();
	MCRelEffGraph_Barrel_1.SetTitle("Rel. Single Mu Efficiency vs Mu pt");
	REffBarrel->Write("RelEff_SingleMu_VsMuPt__Barrel.root");
	REffBarrel->Close();
	
	//MC: Reco(AllCuts)/Reco(Acc) - NB: il TP si confronta con questo  
        TGraphAsymmErrors MCEff_Barrel_global(SingleMuPtEff_Barrel_123456, SingleMuPtEff_Barrel_1); 
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCEff_Barrel_global.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = MCEff_Barrel_global.GetErrorYlow(i);
        GraphVeyh[i] = MCEff_Barrel_global.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors MCRelEffGraph_Barrel_global(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Barrel_global.SetNameTitle("MCRelEffGraph_Barrel_global", "MCRelEffGraph_Barrel_global");
        MCRelEffGraph_Barrel_global.Write();
        
        REff_Endcap->cd();
            
        TGraphAsymmErrors MCRelEff_Endcap_1(SingleMuPtEff_Endcap_1, RelMCEffDenom_Endcap);
        TGraphAsymmErrors MCRelEff_Endcap_12(SingleMuPtEff_Endcap_12, SingleMuPtEff_Endcap_1);
        TGraphAsymmErrors MCRelEff_Endcap_123(SingleMuPtEff_Endcap_123, SingleMuPtEff_Endcap_12);
        TGraphAsymmErrors MCRelEff_Endcap_1234(SingleMuPtEff_Endcap_1234, SingleMuPtEff_Endcap_123);
        TGraphAsymmErrors MCRelEff_Endcap_12345(SingleMuPtEff_Endcap_12345, SingleMuPtEff_Endcap_1234);
        TGraphAsymmErrors MCRelEff_Endcap_123456(SingleMuPtEff_Endcap_123456, SingleMuPtEff_Endcap_12345);
      
        string MCRelEff_Endcap_name="MCRelativeEff";
        MCRelEff_Endcap_name+=_RecoCutFlags[1].c_str();
        MCRelEff_Endcap_1.SetNameTitle(MCRelEff_Endcap_name.c_str(), MCRelEff_Endcap_name.c_str());
        MCRelEff_Endcap_name+=_RecoCutFlags[2].c_str();  
        MCRelEff_Endcap_12.SetNameTitle(MCRelEff_Endcap_name.c_str(), MCRelEff_Endcap_name.c_str());
        MCRelEff_Endcap_name+=_RecoCutFlags[3].c_str();  
        MCRelEff_Endcap_123.SetNameTitle(MCRelEff_Endcap_name.c_str(), MCRelEff_Endcap_name.c_str());
        MCRelEff_Endcap_name+=_RecoCutFlags[4].c_str();  
        MCRelEff_Endcap_1234.SetNameTitle(MCRelEff_Endcap_name.c_str(), MCRelEff_Endcap_name.c_str());
        MCRelEff_Endcap_name+=_RecoCutFlags[5].c_str();  
        MCRelEff_Endcap_12345.SetNameTitle(MCRelEff_Endcap_name.c_str(), MCRelEff_Endcap_name.c_str());
        MCRelEff_Endcap_name+=_RecoCutFlags[6].c_str();  
        MCRelEff_Endcap_123456.SetNameTitle(MCRelEff_Endcap_name.c_str(), MCRelEff_Endcap_name.c_str());
        
        MCRelEff_Endcap_1.Write();
        MCRelEff_Endcap_12.Write();
        MCRelEff_Endcap_123.Write();
        MCRelEff_Endcap_1234.Write();
        MCRelEff_Endcap_123456.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Endcap_1.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Endcap_1.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Endcap_1.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors MCRelEffGraph_Endcap_1(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Endcap_1.SetNameTitle("MCRelEffGraph_Endcap_1", "MCRelEffGraph_Endcap_1");
        MCRelEffGraph_Endcap_1.Write();  
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Endcap_12.GetPoint(i, xg, yg);
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Endcap_12.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Endcap_12.GetErrorYhigh(i);      
        }
        
        TGraphAsymmErrors MCRelEffGraph_Endcap_12(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Endcap_12.SetNameTitle("MCRelEffGraph_Endcap_12", "MCRelEffGraph_Endcap_12");
        MCRelEffGraph_Endcap_12.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Endcap_123.GetPoint(i, xg, yg);
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Endcap_123.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Endcap_123.GetErrorYhigh(i);      
        }
        
        TGraphAsymmErrors MCRelEffGraph_Endcap_123(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Endcap_123.SetNameTitle("MCRelEffGraph_Endcap_123", "MCRelEffGraph_Endcap_123");
        MCRelEffGraph_Endcap_123.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Endcap_1234.GetPoint(i, xg, yg);
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Endcap_1234.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Endcap_1234.GetErrorYhigh(i);      
        }
        
        TGraphAsymmErrors MCRelEffGraph_Endcap_1234(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Endcap_1234.SetNameTitle("MCRelEffGraph_Endcap_1234", "MCRelEffGraph_Endcap_1234");
        MCRelEffGraph_Endcap_1234.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Endcap_12345.GetPoint(i, xg, yg);
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Endcap_12345.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Endcap_12345.GetErrorYhigh(i);      
        }
        
        TGraphAsymmErrors MCRelEffGraph_Endcap_12345(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Endcap_12345.SetNameTitle("MCRelEffGraph_Endcap_12345", "MCRelEffGraph_Endcap_12345");
        MCRelEffGraph_Endcap_12345.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCRelEff_Endcap_123456.GetPoint(i, xg, yg);
        GraphVy[i] = yg;
        GraphVeyl[i] = MCRelEff_Endcap_123456.GetErrorYlow(i);
        GraphVeyh[i] = MCRelEff_Endcap_123456.GetErrorYhigh(i);      
        }
        
        TGraphAsymmErrors MCRelEffGraph_Endcap_123456(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Endcap_123456.SetNameTitle("MCRelEffGraph_Endcap_123456", "MCRelEffGraph_Endcap_123456");
        MCRelEffGraph_Endcap_123456.Write();
               
	TCanvas *REffEndcap = new TCanvas;
	MCRelEffGraph_Endcap_1.GetYaxis()->SetRangeUser(0.6,1);
	MCRelEffGraph_Endcap_1.GetXaxis()->SetTitle("muon pt");
	MCRelEffGraph_Endcap_1.SetMarkerStyle(20);
	MCRelEffGraph_Endcap_1.Draw("AP");
	if(_RecoCutFlags[2] != "_1"){
	MCRelEffGraph_Endcap_12.SetLineColor(2);
	MCRelEffGraph_Endcap_12.SetMarkerStyle(21);
	MCRelEffGraph_Endcap_12.SetMarkerColor(2);
	MCRelEffGraph_Endcap_12.Draw("PSAME");}
	if(_RecoCutFlags[3] != "_1"){
	MCRelEffGraph_Endcap_123.SetLineColor(3);
	MCRelEffGraph_Endcap_123.SetMarkerStyle(22);
	MCRelEffGraph_Endcap_123.SetMarkerColor(3);
        MCRelEffGraph_Endcap_123.Draw("PSAME");}
	if(_RecoCutFlags[4] != "_1"){
	MCRelEffGraph_Endcap_1234.SetLineColor(4);
	MCRelEffGraph_Endcap_1234.SetMarkerStyle(23);
	MCRelEffGraph_Endcap_1234.SetMarkerColor(4);
	MCRelEffGraph_Endcap_1234.Draw("PSAME");}
	if(_RecoCutFlags[5] != "_1"){
        MCRelEffGraph_Endcap_12345.SetLineColor(5);
	MCRelEffGraph_Endcap_12345.SetMarkerStyle(24);
	MCRelEffGraph_Endcap_12345.SetMarkerColor(5);
	MCRelEffGraph_Endcap_12345.Draw("PSAME");}
	if(_RecoCutFlags[6] != "_1"){
        MCRelEffGraph_Endcap_123456.SetLineColor(6);
	MCRelEffGraph_Endcap_123456.SetMarkerStyle(25);
	MCRelEffGraph_Endcap_123456.SetMarkerColor(6);
	MCRelEffGraph_Endcap_123456.Draw("PSAME");}
	TLegend *LegREffEndcap = REffEndcap->BuildLegend(0.5,0.67,0.88,0.88,"Legenda");
	LegREffEndcap->Draw();
	MCRelEffGraph_Endcap_1.SetTitle("Rel. Single Mu Efficiency vs Mu pt");
	REffEndcap->Write("RelEff_SingleMu_VsMuPt_Endcap.root");
	REffEndcap->Close();
	
	//MC: Reco(AllCuts)/Reco(Acc) - NB: il TP si confronta con questo  
        TGraphAsymmErrors MCEff_Endcap_global(SingleMuPtEff_Endcap_123456, SingleMuPtEff_Endcap_1); 
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        MCEff_Endcap_global.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = MCEff_Endcap_global.GetErrorYlow(i);
        GraphVeyh[i] = MCEff_Endcap_global.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors MCRelEffGraph_Endcap_global(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        MCRelEffGraph_Endcap_global.SetNameTitle("MCRelEffGraph_Endcap_global", "MCRelEffGraph_Endcap_global");
        MCRelEffGraph_Endcap_global.Write();
	
        //////////////////////////////////////////////////////////////////////////////	
	
        //Tag And Probe w/o background - Get histograms 
       
        string TPDir="EfficiencyMuon/Tag&Probe_Pt";
        string TPHisto="/Muon";        
        if(selections=="ASYM")TPDir="EfficiencyMuon/Tag&Probe0_Pt";
        
        if(EtaRegion=="barrel")TPDir+="_Barrel";
        if(EtaRegion=="endcap")TPDir+="_Endcap";
 
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
             
        TPDir="EfficiencyMuon/Tag&Probe_Pt";        
        if(selections=="ASYM")TPDir="EfficiencyMuon/Tag&Probe0_Pt";
        if(EtaRegion=="barrel")TPDir+="_Barrel_Global";
        if(EtaRegion=="endcap")TPDir+="_Endcap_Global";
        TPHisto="/Muon_Global";
        
        num_TP = TPDir+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP = TPDir+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator_global = (TH1D*) signal_MC_file->Get(num_TP.c_str());
        TH1D* TP_denominator_global = (TH1D*) signal_MC_file->Get(den_TP.c_str());
      	
        TGraphAsymmErrors Eff_TP_Single_12(TP_numerator_12, TP_denominator_12);  
        TGraphAsymmErrors Eff_TP_Single_123(TP_numerator_123, TP_denominator_123);      
        TGraphAsymmErrors Eff_TP_Single_1234(TP_numerator_1234, TP_denominator_1234);        
        TGraphAsymmErrors Eff_TP_Single_12345(TP_numerator_12345, TP_denominator_12345);
        TGraphAsymmErrors Eff_TP_Single_123456(TP_numerator_123456, TP_denominator_123456);
        TGraphAsymmErrors Eff_TP_Single_global(TP_numerator_global, TP_denominator_global);
   	    
        string Eff_TP_Single_name="TP_MC-SingleEff";
        if(selections=="ASYM")Eff_TP_Single_name="TP_MC-SingleEff0";
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
        Eff_TP_Single_name="TP_MC-SingleEff_Global";
        if(selections=="ASYM")Eff_TP_Single_name="TP_MC-SingleEff0_Global";
        Eff_TP_Single_global.SetNameTitle(Eff_TP_Single_name.c_str(), Eff_TP_Single_name.c_str());
      
        string TPDir1="EfficiencyMuon/Tag&Probe1_Pt";
        if(EtaRegion=="barrel")TPDir1+="_Barrel";
        if(EtaRegion=="endcap")TPDir1+="_Endcap";
    
        TGraphAsymmErrors Eff_TP_Single1_12, Eff_TP_Single1_123, Eff_TP_Single1_1234, Eff_TP_Single1_12345, Eff_TP_Single1_123456, Eff_TP_Single1_global;
         
        if(selections=="ASYM"){
        
        TPHisto="/Muon";
        
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
        
        TPDir1="EfficiencyMuon/Tag&Probe1_Pt";
        if(EtaRegion=="barrel")TPDir1+="_Barrel_Global";
        if(EtaRegion=="endcap")TPDir1+="_Endcap_Global";
        TPHisto="/Muon_Global";
        
        num_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_numerator";
        den_TP1 = TPDir1+TPHisto+"_SingleCandTag&Probe_denominator";
        
        TH1D* TP_numerator1_global = (TH1D*) signal_MC_file->Get(num_TP1.c_str());
        TH1D* TP_denominator1_global = (TH1D*) signal_MC_file->Get(den_TP1.c_str());
        
        Eff_TP_Single1_12.BayesDivide(TP_numerator1_12, TP_denominator1_12);       
        Eff_TP_Single1_123.BayesDivide(TP_numerator1_123, TP_denominator1_123);      
        Eff_TP_Single1_1234.BayesDivide(TP_numerator1_1234, TP_denominator1_1234);        
        Eff_TP_Single1_12345.BayesDivide(TP_numerator1_12345, TP_denominator1_12345);
        Eff_TP_Single1_123456.BayesDivide(TP_numerator1_123456, TP_denominator1_123456);
        Eff_TP_Single1_global.BayesDivide(TP_numerator1_global, TP_denominator1_global);
        
        string Eff_TP_Single1_name="TP_MC-SingleEff1";
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
        Eff_TP_Single1_name="TP_MC-SingleEff1_Global";
        Eff_TP_Single1_global.SetNameTitle(Eff_TP_Single1_name.c_str(), Eff_TP_Single1_name.c_str());
        
        }
        
        //////////////////////////////////// Rebin
 
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single_12.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single_12.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single_12.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors EffTPGraph_Single_12(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single_12.SetNameTitle(("EffTPGraph_Single_12_"+EtaRegion).c_str(), ("EffTPGraph_Single_12_"+EtaRegion).c_str());
        EffTPGraph_Single_12.Write();  
 	      
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single_123.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single_123.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single_123.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors EffTPGraph_Single_123(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single_123.SetNameTitle(("EffTPGraph_Single_123_"+EtaRegion).c_str(), ("EffTPGraph_Single_123_"+EtaRegion).c_str());
        EffTPGraph_Single_123.Write(); 
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single_1234.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single_1234.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single_1234.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors EffTPGraph_Single_1234(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single_1234.SetNameTitle(("EffTPGraph_Single_1234_"+EtaRegion).c_str(), ("EffTPGraph_Single_1234_"+EtaRegion).c_str());
        EffTPGraph_Single_1234.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single_12345.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single_12345.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single_12345.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
     
        TGraphAsymmErrors EffTPGraph_Single_12345(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single_12345.SetNameTitle(("EffTPGraph_Single_12345_"+EtaRegion).c_str(), ("EffTPGraph_Single_12345_"+EtaRegion).c_str());
        EffTPGraph_Single_12345.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single_123456.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single_123456.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single_123456.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors EffTPGraph_Single_123456(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single_123456.SetNameTitle(("EffTPGraph_Single_123456_"+EtaRegion).c_str(), ("EffTPGraph_Single_123456_"+EtaRegion).c_str());
        EffTPGraph_Single_123456.Write();
      
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single_global.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single_global.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single_global.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors EffTPGraph_Single_global(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single_global.SetNameTitle(("EffTPGraph_Single_global_"+EtaRegion).c_str(), ("EffTPGraph_Single_global_"+EtaRegion).c_str());
        EffTPGraph_Single_global.Write();
 
        if(selections=="ASYM"){
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single1_12.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single1_12.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single1_12.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors EffTPGraph_Single1_12(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single1_12.SetNameTitle(("EffTPGraph_Single_12_"+EtaRegion).c_str(), ("EffTPGraph_Single_12_"+EtaRegion).c_str());
        EffTPGraph_Single1_12.Write();  
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single1_123.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single1_123.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single1_123.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors EffTPGraph_Single1_123(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single1_123.SetNameTitle(("EffTPGraph_Single_123_"+EtaRegion).c_str(), ("EffTPGraph_Single_123_"+EtaRegion).c_str());
        EffTPGraph_Single1_123.Write(); 
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single1_1234.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single1_1234.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single1_1234.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors EffTPGraph_Single1_1234(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single1_1234.SetNameTitle(("EffTPGraph_Single_1234_"+EtaRegion).c_str(), ("EffTPGraph_Single_1234_"+EtaRegion).c_str());
        EffTPGraph_Single1_1234.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single1_12345.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single1_12345.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single1_12345.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors EffTPGraph_Single1_12345(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single1_12345.SetNameTitle(("EffTPGraph_Single_12345_"+EtaRegion).c_str(), ("EffTPGraph_Single_12345_"+EtaRegion).c_str());
        EffTPGraph_Single1_12345.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single1_123456.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single1_123456.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single1_123456.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors EffTPGraph_Single1_123456(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single1_123456.SetNameTitle(("EffTPGraph_Single_123456_"+EtaRegion).c_str(), ("EffTPGraph_Single_123456_"+EtaRegion).c_str());
        EffTPGraph_Single1_123456.Write();
        
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        Eff_TP_Single1_global.GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = Eff_TP_Single1_global.GetErrorYlow(i);
        GraphVeyh[i] = Eff_TP_Single1_global.GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }
        
        TGraphAsymmErrors EffTPGraph_Single1_global(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        EffTPGraph_Single1_global.SetNameTitle(("EffTPGraph_Single_global_"+EtaRegion).c_str(), ("EffTPGraph_Single_global_"+EtaRegion).c_str());
        EffTPGraph_Single1_global.Write();
        
        }
      
        //////////////////////////////////////////////////////////////////
        
        //Get TP with background double eff.
         
        TGraphAsymmErrors *tp_eff_12, *tp_eff_123, *tp_eff_1234, *tp_eff_12345, *tp_eff_123456, *tp_eff_global;
      
        if(Fits){
        tp_eff_12 = (TGraphAsymmErrors*) tp_12->Get("SingleTag&ProbeEff");
        tp_eff_123 = (TGraphAsymmErrors*) tp_123->Get("SingleTag&ProbeEff");
        tp_eff_1234 = (TGraphAsymmErrors*) tp_1234->Get("SingleTag&ProbeEff");
        tp_eff_12345 = (TGraphAsymmErrors*) tp_12345->Get("SingleTag&ProbeEff");
        tp_eff_123456 = (TGraphAsymmErrors*) tp_123456->Get("SingleTag&ProbeEff");
        tp_eff_global = (TGraphAsymmErrors*) tp_global->Get("SingleTag&ProbeEff");}
        
        for ( int i = 0; i < 3; ++i ){ 
        GraphVx[i] = 0;  
        GraphVy[i] = 0;
        GraphVeyl[i] = 0;
        GraphVeyh[i] = 0;      
        GraphVexl[i] = 0;
        GraphVexh[i] = 0;
        }
        
        if(Fits){
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        tp_eff_12->GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = tp_eff_12->GetErrorYlow(i);
        GraphVeyh[i] = tp_eff_12->GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }}
        
        TGraphAsymmErrors tp_eff_Graph_12(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        tp_eff_Graph_12.SetNameTitle(("tp_eff_Graph_12"+EtaRegion).c_str(), ("tp_eff_Graph_12"+EtaRegion).c_str());
 	      
 	if(Fits){      
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        tp_eff_123->GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = tp_eff_123->GetErrorYlow(i);
        GraphVeyh[i] = tp_eff_123->GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }}
        
        TGraphAsymmErrors tp_eff_Graph_123(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        tp_eff_Graph_123.SetNameTitle(("tp_eff_Graph_123"+EtaRegion).c_str(), ("tp_eff_Graph_123"+EtaRegion).c_str());
        
        if(Fits){
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        tp_eff_1234->GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = tp_eff_1234->GetErrorYlow(i);
        GraphVeyh[i] = tp_eff_1234->GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }}
        
        TGraphAsymmErrors tp_eff_Graph_1234(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        tp_eff_Graph_1234.SetNameTitle(("tp_eff_Graph_1234"+EtaRegion).c_str(), ("tp_eff_Graph_1234"+EtaRegion).c_str());
        
        if(Fits){
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        tp_eff_12345->GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = tp_eff_12345->GetErrorYlow(i);
        GraphVeyh[i] = tp_eff_12345->GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }}
        
        TGraphAsymmErrors tp_eff_Graph_12345(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        tp_eff_Graph_12345.SetNameTitle(("tp_eff_Graph_12345"+EtaRegion).c_str(), ("tp_eff_Graph_12345"+EtaRegion).c_str());
        
        if(Fits){
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        tp_eff_123456->GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = tp_eff_123456->GetErrorYlow(i);
        GraphVeyh[i] = tp_eff_123456->GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }}
        
        TGraphAsymmErrors tp_eff_Graph_123456(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        tp_eff_Graph_123456.SetNameTitle(("tp_eff_Graph_123456"+EtaRegion).c_str(), ("tp_eff_Graph_123456"+EtaRegion).c_str());
        
        if(Fits){
        for ( int i = 0; i < 3; ++i ){        
        xg=0.; yg=0.;     
        tp_eff_global->GetPoint(i, xg, yg);
        GraphVx[i] = ptBin[i]+(ptBin[i+1]-ptBin[i])/2;  
        GraphVy[i] = yg;
        GraphVeyl[i] = tp_eff_global->GetErrorYlow(i);
        GraphVeyh[i] = tp_eff_global->GetErrorYhigh(i);      
        GraphVexl[i] = (ptBin[i+1]-ptBin[i])/2;
        GraphVexh[i] = (ptBin[i+1]-ptBin[i])/2;
        }}
        
        TGraphAsymmErrors tp_eff_Graph_global(GraphVx, GraphVy, GraphVexl, GraphVexh, GraphVeyl, GraphVeyh);
        tp_eff_Graph_global.SetNameTitle(("tp_eff_Graph_global"+EtaRegion).c_str(), ("tp_eff_Graph_global"+EtaRegion).c_str());
        
        //////////////////////////////////////////////////////////////////
 
        TP_12->cd();
        TP_numerator_12->Write();
        TP_denominator_12->Write();
        EffTPGraph_Single_12.Write();
        if(Fits)tp_eff_Graph_12.Write("TP_Data-Eff_12");
        TCanvas *EffTP_12 = new TCanvas;
        EffTPGraph_Single_12.SetLineColor(2);
        EffTPGraph_Single_12.SetLineWidth(2);
	EffTPGraph_Single_12.SetMarkerStyle(21);
	EffTPGraph_Single_12.SetMarkerColor(2);
	EffTPGraph_Single_12.Draw("AP");
	EffTPGraph_Single_12.SetTitle("Cut eff: MC Rel Eff (black), MC TP w/o back (red), Data (blue)");
	EffTPGraph_Single_12.GetXaxis()->SetRangeUser(xmin,xmax);
	EffTPGraph_Single_12.GetYaxis()->SetRangeUser(ymin,ymax); 
	if(Fits){ 
	tp_eff_Graph_12.SetLineColor(4);
	tp_eff_Graph_12.SetLineWidth(2);
	tp_eff_Graph_12.SetMarkerStyle(23);
	tp_eff_Graph_12.SetMarkerColor(4);
	tp_eff_Graph_12.DrawClone("PSAME");}	
	if(EtaRegion=="barrel"){	
        MCRelEffGraph_Barrel_12.SetLineColor(1);
        MCRelEffGraph_Barrel_12.SetLineWidth(2);
	MCRelEffGraph_Barrel_12.SetMarkerStyle(20);
	MCRelEffGraph_Barrel_12.SetMarkerColor(1);
	MCRelEffGraph_Barrel_12.Draw("PSAME");}
	if(EtaRegion=="endcap"){
	
        MCRelEffGraph_Endcap_12.SetLineColor(1);
        MCRelEffGraph_Endcap_12.SetLineWidth(2);
	MCRelEffGraph_Endcap_12.SetMarkerStyle(20);
	MCRelEffGraph_Endcap_12.SetMarkerColor(1);
	MCRelEffGraph_Endcap_12.Draw("PSAME");}
	string EffTP_12_name = "TPEff";
	EffTP_12_name+=_RecoCutFlags[1].c_str();
        EffTP_12_name+=_RecoCutFlags[2].c_str();
        EffTP_12_name+=".root";
	EffTP_12->Write(EffTP_12_name.c_str());
	EffTP_12->Close();

	tpr<<"Relative efficiency vs "<<EtaRegion<<" EtaRegion"<<endl;

	tpr<<endl<<"Selection: "<<_RecoCutFlags[2]<<endl<<endl;
	for(int i=0; i<bin; i++){
	double xMCeff = 0., yMCeff = 0., yMCeffErrh = 0., yMCeffErrl = 0.;
	double xMCSigTp = 0., yMCSigTp = 0.;
	double xDATA = 0., yDATA = 0.;
	if(EtaRegion=="barrel"){
	MCRelEffGraph_Barrel_12.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Barrel_12.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Barrel_12.GetErrorYlow(i);}
	if(EtaRegion=="endcap"){
	MCRelEffGraph_Endcap_12.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Endcap_12.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Endcap_12.GetErrorYlow(i);}
	EffTPGraph_Single_12.GetPoint(i,xMCSigTp,yMCSigTp);
	if(Fits)tp_eff_Graph_12.GetPoint(i,xDATA,yDATA);
	tpr<<EtaRegion<<" Mult. = "<<i<<"	MC eff. =	"<<((float)((int)(yMCeff*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(yMCeffErrh*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(yMCeffErrl*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		MC Sig TP =	"<<((float)((int)(yMCSigTp*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(EffTPGraph_Single_12.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(EffTPGraph_Single_12.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	if(Fits){tpr<<"		DATA TP =	"<<((float)((int)(yDATA*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(tp_eff_Graph_12.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(tp_eff_Graph_12.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;}
	tpr<<endl;
	}
	
	TP_123->cd();
        TP_numerator_123->Write();
        TP_denominator_123->Write();
        EffTPGraph_Single_123.Write();
        if(Fits)tp_eff_Graph_123.Write("TP_Data-Eff_123");
        TCanvas *EffTP_123 = new TCanvas;
        EffTPGraph_Single_123.SetLineColor(2);
        EffTPGraph_Single_123.SetLineWidth(2);
	EffTPGraph_Single_123.SetMarkerStyle(21);
	EffTPGraph_Single_123.SetMarkerColor(2);
	EffTPGraph_Single_123.Draw("AP");
	EffTPGraph_Single_123.SetTitle("Cut eff: MC Rel Eff (black), MC TP w/o back (red), Data (blue)");
	EffTPGraph_Single_123.GetXaxis()->SetRangeUser(xmin,xmax);
	EffTPGraph_Single_123.GetYaxis()->SetRangeUser(ymin,ymax);
	if(Fits){ 
	tp_eff_Graph_123.SetLineColor(4);
	tp_eff_Graph_123.SetLineWidth(2);
	tp_eff_Graph_123.SetMarkerStyle(23);
	tp_eff_Graph_123.SetMarkerColor(4);
	tp_eff_Graph_123.DrawClone("PSAME");}	
	if(EtaRegion=="barrel"){
        MCRelEffGraph_Barrel_123.SetLineColor(1);
        MCRelEffGraph_Barrel_123.SetLineWidth(2);
	MCRelEffGraph_Barrel_123.SetMarkerStyle(20);
	MCRelEffGraph_Barrel_123.SetMarkerColor(1);
	MCRelEffGraph_Barrel_123.Draw("PSAME");}
	if(EtaRegion=="endcap"){
        MCRelEffGraph_Endcap_123.SetLineColor(1);
        MCRelEffGraph_Endcap_123.SetLineWidth(2);
	MCRelEffGraph_Endcap_123.SetMarkerStyle(20);
	MCRelEffGraph_Endcap_123.SetMarkerColor(1);
	MCRelEffGraph_Endcap_123.Draw("PSAME");}
	string EffTP_123_name = "TPEff";
	EffTP_123_name+=_RecoCutFlags[1].c_str();
        EffTP_123_name+=_RecoCutFlags[2].c_str();
        EffTP_123_name+=_RecoCutFlags[3].c_str();
        EffTP_123_name+=".root";
	EffTP_123->Write(EffTP_123_name.c_str());
	EffTP_123->Close();
	
	tpr<<"Relative efficiency vs "<<EtaRegion<<" EtaRegion"<<endl;
	
	tpr<<endl<<"Selection: "<<_RecoCutFlags[2]<<endl<<endl;
	for(int i=0; i<bin; i++){
	double xMCeff = 0., yMCeff = 0., yMCeffErrh = 0., yMCeffErrl = 0.;
	double xMCSigTp = 0., yMCSigTp = 0.;
	double xDATA = 0., yDATA = 0.;
	if(EtaRegion=="barrel"){
	MCRelEffGraph_Barrel_123.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Barrel_123.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Barrel_123.GetErrorYlow(i);}
	if(EtaRegion=="endcap"){
	MCRelEffGraph_Endcap_123.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Endcap_123.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Endcap_123.GetErrorYlow(i);}
	EffTPGraph_Single_123.GetPoint(i,xMCSigTp,yMCSigTp);
	if(Fits)tp_eff_Graph_123.GetPoint(i,xDATA,yDATA);
	tpr<<EtaRegion<<" Mult. = "<<i<<"	MC eff. =	"<<((float)((int)(yMCeff*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(yMCeffErrh*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(yMCeffErrl*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		MC Sig TP =	"<<((float)((int)(yMCSigTp*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(EffTPGraph_Single_123.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(EffTPGraph_Single_123.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	if(Fits){tpr<<"		DATA TP =	"<<((float)((int)(yDATA*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(tp_eff_Graph_123.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(tp_eff_Graph_123.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;}
	tpr<<endl;
	}
		
	TP_1234->cd();
        TP_numerator_1234->Write();
        TP_denominator_1234->Write();
        EffTPGraph_Single_1234.Write();
        if(Fits)tp_eff_Graph_1234.Write("TP_Data-Eff_1234");
        TCanvas *EffTP_1234 = new TCanvas;
        EffTPGraph_Single_1234.SetLineColor(2);
        EffTPGraph_Single_1234.SetLineWidth(2);
	EffTPGraph_Single_1234.SetMarkerStyle(21);
	EffTPGraph_Single_1234.SetMarkerColor(2);
	EffTPGraph_Single_1234.Draw("AP");
	EffTPGraph_Single_1234.SetTitle("Cut eff: MC Rel Eff (black), MC TP w/o back (red), Data (blue)");
	EffTPGraph_Single_1234.GetXaxis()->SetRangeUser(xmin,xmax);
	EffTPGraph_Single_1234.GetYaxis()->SetRangeUser(ymin,ymax);
	if(Fits){ 
	tp_eff_Graph_1234.SetLineColor(4);
	tp_eff_Graph_1234.SetLineWidth(2);
	tp_eff_Graph_1234.SetMarkerStyle(23);
	tp_eff_Graph_1234.SetMarkerColor(4);
	tp_eff_Graph_1234.DrawClone("PSAME");}	
	if(EtaRegion=="barrel"){
        MCRelEffGraph_Barrel_1234.SetLineColor(1);
        MCRelEffGraph_Barrel_1234.SetLineWidth(2);
	MCRelEffGraph_Barrel_1234.SetMarkerStyle(20);
	MCRelEffGraph_Barrel_1234.SetMarkerColor(1);
	MCRelEffGraph_Barrel_1234.Draw("PSAME");}
	if(EtaRegion=="endcap"){
        MCRelEffGraph_Endcap_1234.SetLineColor(1);
        MCRelEffGraph_Endcap_1234.SetLineWidth(2);
	MCRelEffGraph_Endcap_1234.SetMarkerStyle(20);
	MCRelEffGraph_Endcap_1234.SetMarkerColor(1);
	MCRelEffGraph_Endcap_1234.Draw("PSAME");}
	string EffTP_1234_name = "TPEff";
	EffTP_1234_name+=_RecoCutFlags[1].c_str();
        EffTP_1234_name+=_RecoCutFlags[2].c_str();
        EffTP_1234_name+=_RecoCutFlags[3].c_str();
        EffTP_1234_name+=_RecoCutFlags[4].c_str();
        EffTP_1234_name+=".root";
	EffTP_1234->Write(EffTP_1234_name.c_str());
	EffTP_1234->Close();
	
	tpr<<"Relative efficiency vs "<<EtaRegion<<" EtaRegion"<<endl;
	
	tpr<<endl<<"Selection: "<<_RecoCutFlags[2]<<endl<<endl;
	for(int i=0; i<bin; i++){
	double xMCeff = 0., yMCeff = 0., yMCeffErrh = 0., yMCeffErrl = 0.;
	double xMCSigTp = 0., yMCSigTp = 0.;
	double xDATA = 0., yDATA = 0.;
	if(EtaRegion=="barrel"){
	MCRelEffGraph_Barrel_1234.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Barrel_1234.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Barrel_1234.GetErrorYlow(i);}
	if(EtaRegion=="endcap"){
	MCRelEffGraph_Endcap_1234.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Endcap_1234.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Endcap_1234.GetErrorYlow(i);}
	EffTPGraph_Single_1234.GetPoint(i,xMCSigTp,yMCSigTp);
	if(Fits)tp_eff_Graph_1234.GetPoint(i,xDATA,yDATA);
	tpr<<EtaRegion<<" Mult. = "<<i<<"	MC eff. =	"<<((float)((int)(yMCeff*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(yMCeffErrh*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(yMCeffErrl*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		MC Sig TP =	"<<((float)((int)(yMCSigTp*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(EffTPGraph_Single_1234.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(EffTPGraph_Single_1234.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	if(Fits){tpr<<"		DATA TP =	"<<((float)((int)(yDATA*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(tp_eff_Graph_1234.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(tp_eff_Graph_1234.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;}
	tpr<<endl;
	}
	
	TP_12345->cd();
        TP_numerator_12345->Write();
        TP_denominator_12345->Write();
        EffTPGraph_Single_12345.Write();
        if(Fits)tp_eff_Graph_12345.Write("TP_Data-Eff_12345");
        TCanvas *EffTP_12345 = new TCanvas;
        EffTPGraph_Single_12345.SetLineColor(2);
        EffTPGraph_Single_12345.SetLineWidth(2);
	EffTPGraph_Single_12345.SetMarkerStyle(21);
	EffTPGraph_Single_12345.SetMarkerColor(2);
	EffTPGraph_Single_12345.Draw("AP");
	EffTPGraph_Single_12345.SetTitle("Cut eff: MC Rel Eff (black), MC TP w/o back (red), Data (blue)");
	EffTPGraph_Single_12345.GetXaxis()->SetRangeUser(xmin,xmax);
	EffTPGraph_Single_12345.GetYaxis()->SetRangeUser(ymin,ymax);
	if(Fits){ 
	tp_eff_Graph_12345.SetLineColor(4);
	tp_eff_Graph_12345.SetLineWidth(2);
	tp_eff_Graph_12345.SetMarkerStyle(23);
	tp_eff_Graph_12345.SetMarkerColor(4);
	tp_eff_Graph_12345.DrawClone("PSAME");}	
	if(EtaRegion=="barrel"){
        MCRelEffGraph_Barrel_12345.SetLineColor(1);
        MCRelEffGraph_Barrel_12345.SetLineWidth(2);
	MCRelEffGraph_Barrel_12345.SetMarkerStyle(20);
	MCRelEffGraph_Barrel_12345.SetMarkerColor(1);
	MCRelEffGraph_Barrel_12345.Draw("PSAME");}
	if(EtaRegion=="endcap"){
        MCRelEffGraph_Endcap_12345.SetLineColor(1);
        MCRelEffGraph_Endcap_12345.SetLineWidth(2);
	MCRelEffGraph_Endcap_12345.SetMarkerStyle(20);
	MCRelEffGraph_Endcap_12345.SetMarkerColor(1);
	MCRelEffGraph_Endcap_12345.Draw("PSAME");}
	string EffTP_12345_name = "TPEff";
	EffTP_12345_name+=_RecoCutFlags[1].c_str();
        EffTP_12345_name+=_RecoCutFlags[2].c_str();
        EffTP_12345_name+=_RecoCutFlags[3].c_str();
        EffTP_12345_name+=_RecoCutFlags[4].c_str();
        EffTP_12345_name+=_RecoCutFlags[5].c_str();
        EffTP_12345_name+=".root";
	EffTP_12345->Write(EffTP_12345_name.c_str());
	EffTP_12345->Close();
	
	tpr<<"Relative efficiency vs "<<EtaRegion<<" EtaRegion"<<endl;
	
	tpr<<endl<<"Selection: "<<_RecoCutFlags[2]<<endl<<endl;
	for(int i=0; i<bin; i++){
	double xMCeff = 0., yMCeff = 0., yMCeffErrh = 0., yMCeffErrl = 0.;
	double xMCSigTp = 0., yMCSigTp = 0.;
	double xDATA = 0., yDATA = 0.;
	if(EtaRegion=="barrel"){
	MCRelEffGraph_Barrel_12345.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Barrel_12345.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Barrel_12345.GetErrorYlow(i);}
	if(EtaRegion=="endcap"){
	MCRelEffGraph_Endcap_12345.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Endcap_12345.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Endcap_12345.GetErrorYlow(i);}
	EffTPGraph_Single_12345.GetPoint(i,xMCSigTp,yMCSigTp);
	if(Fits)tp_eff_Graph_12345.GetPoint(i,xDATA,yDATA);
	tpr<<EtaRegion<<" Mult. = "<<i<<"	MC eff. =	"<<((float)((int)(yMCeff*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(yMCeffErrh*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(yMCeffErrl*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		MC Sig TP =	"<<((float)((int)(yMCSigTp*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(EffTPGraph_Single_12345.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(EffTPGraph_Single_12345.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	if(Fits){tpr<<"		DATA TP =	"<<((float)((int)(yDATA*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(tp_eff_Graph_12345.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(tp_eff_Graph_12345.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;}
	tpr<<endl;
	}
	
	TP_123456->cd();
        TP_numerator_123456->Write();
        TP_denominator_123456->Write();
        EffTPGraph_Single_123456.Write();
        if(Fits)tp_eff_Graph_123456.Write("TP_Data-Eff_123456");
        TCanvas *EffTP_123456 = new TCanvas;
        EffTPGraph_Single_123456.SetLineColor(2);
        EffTPGraph_Single_123456.SetLineWidth(2);
	EffTPGraph_Single_123456.SetMarkerStyle(21);
	EffTPGraph_Single_123456.SetMarkerColor(2);
	EffTPGraph_Single_123456.Draw("AP");
	EffTPGraph_Single_123456.SetTitle("Cut eff: MC Rel Eff (black), MC TP w/o back (red), Data (blue)");
	EffTPGraph_Single_123456.GetXaxis()->SetRangeUser(xmin,xmax);
	EffTPGraph_Single_123456.GetYaxis()->SetRangeUser(ymin,ymax);
	if(Fits){ 
	tp_eff_Graph_123456.SetLineColor(4);
	tp_eff_Graph_123456.SetLineWidth(2);
	tp_eff_Graph_123456.SetMarkerStyle(23);
	tp_eff_Graph_123456.SetMarkerColor(4);
	tp_eff_Graph_123456.DrawClone("PSAME");}	
	if(EtaRegion=="barrel"){
        MCRelEffGraph_Barrel_123456.SetLineColor(1);
        MCRelEffGraph_Barrel_123456.SetLineWidth(2);
	MCRelEffGraph_Barrel_123456.SetMarkerStyle(20);
	MCRelEffGraph_Barrel_123456.SetMarkerColor(1);
	MCRelEffGraph_Barrel_123456.Draw("PSAME");}
	if(EtaRegion=="endcap"){
        MCRelEffGraph_Endcap_123456.SetLineColor(1);
        MCRelEffGraph_Endcap_123456.SetLineWidth(2);
	MCRelEffGraph_Endcap_123456.SetMarkerStyle(20);
	MCRelEffGraph_Endcap_123456.SetMarkerColor(1);
	MCRelEffGraph_Endcap_123456.Draw("PSAME"); 
	}
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
	
	tpr<<"Relative efficiency vs "<<EtaRegion<<" EtaRegion"<<endl;
	
	tpr<<endl<<"Selection: "<<_RecoCutFlags[2]<<endl<<endl;
	for(int i=0; i<bin; i++){
	double xMCeff = 0., yMCeff = 0., yMCeffErrh = 0., yMCeffErrl = 0.;
	double xMCSigTp = 0., yMCSigTp = 0.;
	double xDATA = 0., yDATA = 0.;
	if(EtaRegion=="barrel"){
	MCRelEffGraph_Barrel_123456.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Barrel_123456.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Barrel_123456.GetErrorYlow(i);}
	if(EtaRegion=="endcap"){
	MCRelEffGraph_Endcap_123456.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Endcap_123456.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Endcap_123456.GetErrorYlow(i);}
	EffTPGraph_Single_123456.GetPoint(i,xMCSigTp,yMCSigTp);
	if(Fits)tp_eff_Graph_123456.GetPoint(i,xDATA,yDATA);
	tpr<<EtaRegion<<" Mult. = "<<i<<"	MC eff. =	"<<((float)((int)(yMCeff*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(yMCeffErrh*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(yMCeffErrl*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		MC Sig TP =	"<<((float)((int)(yMCSigTp*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(EffTPGraph_Single_123456.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(EffTPGraph_Single_123456.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	if(Fits){tpr<<"		DATA TP =	"<<((float)((int)(yDATA*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(tp_eff_Graph_123456.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(tp_eff_Graph_123456.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;}
	tpr<<endl;
	}
	
	TP_global->cd();
        TP_numerator_global->Write();
        TP_denominator_global->Write();
        EffTPGraph_Single_global.Write();
        if(Fits)tp_eff_Graph_global.Write("TP_Data-Eff_global");
        TCanvas *EffTP_global = new TCanvas;
        EffTPGraph_Single_global.SetLineColor(2);
        EffTPGraph_Single_global.SetLineWidth(2);
	EffTPGraph_Single_global.SetMarkerStyle(21);
	EffTPGraph_Single_global.SetMarkerColor(2);
	EffTPGraph_Single_global.Draw("AP");
	EffTPGraph_Single_global.SetTitle("Cut eff: MC Rel Eff (black), MC TP w/o back (red), Data (blue)");
	EffTPGraph_Single_global.GetXaxis()->SetRangeUser(xmin,xmax);
	EffTPGraph_Single_global.GetYaxis()->SetRangeUser(ymin,ymax);
	if(Fits){ 
	tp_eff_Graph_global.SetLineColor(4);
	tp_eff_Graph_global.SetLineWidth(2);
	tp_eff_Graph_global.SetMarkerStyle(23);
	tp_eff_Graph_global.SetMarkerColor(4);
	tp_eff_Graph_global.DrawClone("PSAME");}	
	if(EtaRegion=="barrel"){
        MCRelEffGraph_Barrel_global.SetLineColor(1);
        MCRelEffGraph_Barrel_global.SetLineWidth(2);
	MCRelEffGraph_Barrel_global.SetMarkerStyle(20);
	MCRelEffGraph_Barrel_global.SetMarkerColor(1);
	MCRelEffGraph_Barrel_global.Draw("PSAME");}
	if(EtaRegion=="endcap"){
        MCRelEffGraph_Endcap_global.SetLineColor(1);
        MCRelEffGraph_Endcap_global.SetLineWidth(2);
	MCRelEffGraph_Endcap_global.SetMarkerStyle(20);
	MCRelEffGraph_Endcap_global.SetMarkerColor(1);
	MCRelEffGraph_Endcap_global.Draw("PSAME");}
	string EffTP_global_name = "TPEff_Global";
        EffTP_global_name+=".root";
	EffTP_global->Write(EffTP_global_name.c_str());
	EffTP_global->Close();
	
	tpr<<"Relative efficiency vs "<<EtaRegion<<" EtaRegion"<<endl;
	
	tpr<<endl<<"Selection: "<<_RecoCutFlags[2]<<endl<<endl;
	for(int i=0; i<bin; i++){
	double xMCeff = 0., yMCeff = 0., yMCeffErrh = 0., yMCeffErrl = 0.;
	double xMCSigTp = 0., yMCSigTp = 0.;
	double xDATA = 0., yDATA = 0.;
	if(EtaRegion=="barrel"){
	MCRelEffGraph_Barrel_global.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Barrel_global.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Barrel_global.GetErrorYlow(i);}
	if(EtaRegion=="endcap"){
	MCRelEffGraph_Endcap_global.GetPoint(i,xMCeff,yMCeff);
	yMCeffErrh=MCRelEffGraph_Endcap_global.GetErrorYhigh(i);
	yMCeffErrl=MCRelEffGraph_Endcap_global.GetErrorYlow(i);}
	EffTPGraph_Single_global.GetPoint(i,xMCSigTp,yMCSigTp);
	if(Fits)tp_eff_Graph_global.GetPoint(i,xDATA,yDATA);
	tpr<<EtaRegion<<" Mult. = "<<i<<"	MC eff. =	"<<((float)((int)(yMCeff*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(yMCeffErrh*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(yMCeffErrl*cut_decimal)))/cut_decimal<<endl;
	tpr<<"		MC Sig TP =	"<<((float)((int)(yMCSigTp*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(EffTPGraph_Single_global.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(EffTPGraph_Single_global.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;
	if(Fits){tpr<<"		DATA TP =	"<<((float)((int)(yDATA*cut_decimal)))/cut_decimal<<"	Error High = "<<((float)((int)(tp_eff_Graph_global.GetErrorYhigh(i)*cut_decimal)))/cut_decimal<<"	Error Low = "<<((float)((int)(tp_eff_Graph_global.GetErrorYlow(i)*cut_decimal)))/cut_decimal<<endl;}
	tpr<<endl;
	}
	
        ////////////////////////////////////////////////////////////////////////////////////

        //Tag&Probe residuals
/*        
        TVectorD res1vx_12(n_12);
        TVectorD res1vy_12(n_12);
        TVectorD res1vexl_12(n_12);
        TVectorD res1vexh_12(n_12);
        TVectorD res1veyl_12(n_12);
        TVectorD res1veyh_12(n_12);
        TVectorD res2vx_12(n_12);
        TVectorD res2vy_12(n_12);
        TVectorD res2vexl_12(n_12);
        TVectorD res2vexh_12(n_12);
        TVectorD res2veyl_12(n_12);
        TVectorD res2veyh_12(n_12);
        
        for ( int i = 0; i < n_12; ++i ){
        double x0 = 0., y0 = 0., x1 = 0., y1 = 0., x2 = 0., y2 = 0.;
        MCRelEff_Barrel_12.GetPoint(i, x0, y0);
        Eff_TP_Double_12.GetPoint(i, x1, y1);
        if(Fits)tp_eff_12->GetPoint(i, x2, y2);
        res1vx_12(i)   = i;
        res1vexl_12(i) = MCRelEff_Barrel_12.GetErrorXlow(i);
        res1vexh_12(i) = MCRelEff_Barrel_12.GetErrorXhigh(i);
        res1vy_12(i)   = y1-y0;
        res1veyl_12(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12.GetErrorYlow(i),2)+pow(Eff_TP_Double_12.GetErrorYlow(i),2));
        res1veyh_12(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12.GetErrorYhigh(i),2)+pow(Eff_TP_Double_12.GetErrorYhigh(i),2));
        if(Fits){
        res2vx_12(i)   = i;
        res2vexl_12(i) = MCRelEff_Barrel_12.GetErrorXlow(i);
        res2vexh_12(i) = MCRelEff_Barrel_12.GetErrorXhigh(i);
        res2vy_12(i)   = y2-y0;
        res2veyl_12(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12.GetErrorYlow(i),2)+pow(tp_eff_12->GetErrorYlow(i),2));
        res2veyh_12(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12.GetErrorYhigh(i),2)+pow(tp_eff_12->GetErrorYhigh(i),2));}
        }
              
        TP_12->cd();
        TGraphAsymmErrors TP_Res1_12(res1vx_12, res1vy_12, res1vexl_12, res1vexh_12, res1veyl_12, res1veyh_12);
        TGraphAsymmErrors TP_Res2_12(res2vx_12, res2vy_12, res2vexl_12, res2vexh_12, res2veyl_12, res2veyh_12);
        TCanvas *ResTP_12 = new TCanvas;
        ResTP_12->SetGrid();
        TP_Res1_12.SetLineColor(2);
	TP_Res1_12.SetMarkerStyle(21);
	TP_Res1_12.SetLineWidth(2);
	TP_Res1_12.SetMarkerColor(2);
	TP_Res1_12.Draw("AP");
	if(Fits){
	TP_Res2_12.SetLineColor(4);
	TP_Res2_12.SetMarkerStyle(23);
	TP_Res2_12.SetLineWidth(2);
	TP_Res2_12.SetMarkerColor(4);
	TP_Res2_12.Draw("PSAME");}
	TP_Res1_12.SetTitle("TP residuals from MC Rel Eff: MC TP w/o back (red), Data (blue)");
	TP_Res1_12.GetXaxis()->SetRangeUser(xmin,xmax);
	TP_Res1_12.GetYaxis()->SetRangeUser(-0.3,0.3);
	ResTP_12->Write("ResTP_Trg.root");
	ResTP_12->Close();

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
        MCRelEff_Barrel_123.GetPoint(i, x0, y0);
        Eff_TP_Double_123.GetPoint(i, x1, y1);
        if(Fits)tp_eff_123->GetPoint(i, x2, y2);
        res1vx_123(i)   = i;
        res1vexl_123(i) = MCRelEff_Barrel_123.GetErrorXlow(i);
        res1vexh_123(i) = MCRelEff_Barrel_123.GetErrorXhigh(i);
        res1vy_123(i)   = y1-y0;
        res1veyl_123(i) = TMath::Sqrt(pow(MCRelEff_Barrel_123.GetErrorYlow(i),2)+pow(Eff_TP_Double_123.GetErrorYlow(i),2));
        res1veyh_123(i) = TMath::Sqrt(pow(MCRelEff_Barrel_123.GetErrorYhigh(i),2)+pow(Eff_TP_Double_123.GetErrorYhigh(i),2));
        if(Fits){
        res2vx_123(i)   = i;
        res2vexl_123(i) = MCRelEff_Barrel_123.GetErrorXlow(i);
        res2vexh_123(i) = MCRelEff_Barrel_123.GetErrorXhigh(i);
        res2vy_123(i)   = y2-y0;
        res2veyl_123(i) = TMath::Sqrt(pow(MCRelEff_Barrel_123.GetErrorYlow(i),2)+pow(tp_eff_123->GetErrorYlow(i),2));
        res2veyh_123(i) = TMath::Sqrt(pow(MCRelEff_Barrel_123.GetErrorYhigh(i),2)+pow(tp_eff_123->GetErrorYhigh(i),2));
        }
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
	if(Fits){
	TP_Res2_123.SetLineColor(4);
	TP_Res2_123.SetMarkerStyle(23);
	TP_Res2_123.SetLineWidth(2);
	TP_Res2_123.SetMarkerColor(4);
	TP_Res2_123.Draw("PSAME");}
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
        MCRelEff_Barrel_1234.GetPoint(i, x0, y0);
        Eff_TP_Double_1234.GetPoint(i, x1, y1);
        if(Fits)tp_eff_1234->GetPoint(i, x2, y2);
        res1vx_1234(i)   = i;
        res1vexl_1234(i) = MCRelEff_Barrel_1234.GetErrorXlow(i);
        res1vexh_1234(i) = MCRelEff_Barrel_1234.GetErrorXhigh(i);
        res1vy_1234(i)   = y1-y0;
        res1veyl_1234(i) = TMath::Sqrt(pow(MCRelEff_Barrel_1234.GetErrorYlow(i),2)+pow(Eff_TP_Double_1234.GetErrorYlow(i),2));
        res1veyh_1234(i) = TMath::Sqrt(pow(MCRelEff_Barrel_1234.GetErrorYhigh(i),2)+pow(Eff_TP_Double_1234.GetErrorYhigh(i),2));
        if(Fits){
        res2vx_1234(i)   = i;
        res2vexl_1234(i) = MCRelEff_Barrel_1234.GetErrorXlow(i);
        res2vexh_1234(i) = MCRelEff_Barrel_1234.GetErrorXhigh(i);
        res2vy_1234(i)   = y2-y0;
        res2veyl_1234(i) = TMath::Sqrt(pow(MCRelEff_Barrel_1234.GetErrorYlow(i),2)+pow(tp_eff_1234->GetErrorYlow(i),2));
        res2veyh_1234(i) = TMath::Sqrt(pow(MCRelEff_Barrel_1234.GetErrorYhigh(i),2)+pow(tp_eff_1234->GetErrorYhigh(i),2));}
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
	if(Fits){
	TP_Res2_1234.SetLineColor(4);
	TP_Res2_1234.SetMarkerStyle(23);
	TP_Res2_1234.SetLineWidth(2);
	TP_Res2_1234.SetMarkerColor(4);
	TP_Res2_1234.Draw("PSAME");}
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
        MCRelEff_Barrel_12345.GetPoint(i, x0, y0);
        Eff_TP_Double_12345.GetPoint(i, x1, y1);
        if(Fits)tp_eff_12345->GetPoint(i, x2, y2);
        res1vx_12345(i)   = i;
        res1vexl_12345(i) = MCRelEff_Barrel_12345.GetErrorXlow(i);
        res1vexh_12345(i) = MCRelEff_Barrel_12345.GetErrorXhigh(i);
        res1vy_12345(i)   = y1-y0;
        res1veyl_12345(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12345.GetErrorYlow(i),2)+pow(Eff_TP_Double_12345.GetErrorYlow(i),2));
        res1veyh_12345(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12345.GetErrorYhigh(i),2)+pow(Eff_TP_Double_12345.GetErrorYhigh(i),2));
        if(Fits){
        res2vx_12345(i)   = i;
        res2vexl_12345(i) = MCRelEff_Barrel_12345.GetErrorXlow(i);
        res2vexh_12345(i) = MCRelEff_Barrel_12345.GetErrorXhigh(i);
        res2vy_12345(i)   = y2-y0;
        res2veyl_12345(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12345.GetErrorYlow(i),2)+pow(tp_eff_12345->GetErrorYlow(i),2));
        res2veyh_12345(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12345.GetErrorYhigh(i),2)+pow(tp_eff_12345->GetErrorYhigh(i),2));}
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
	if(Fits){
	TP_Res2_12345.SetLineColor(4);
	TP_Res2_12345.SetMarkerStyle(23);
	TP_Res2_12345.SetLineWidth(2);
	TP_Res2_12345.SetMarkerColor(4);
	TP_Res2_12345.Draw("PSAME");}
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
        MCRelEff_Barrel_12345.GetPoint(i, x0, y0);
        Eff_TP_Double_123456.GetPoint(i, x1, y1);
        if(Fits)tp_eff_123456->GetPoint(i, x2, y2);
        res1vx_123456(i)   = i;
        res1vexl_123456(i) = MCRelEff_Barrel_12345.GetErrorXlow(i);
        res1vexh_123456(i) = MCRelEff_Barrel_12345.GetErrorXhigh(i);
        res1vy_123456(i)   = y1-y0;
        res1veyl_123456(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12345.GetErrorYlow(i),2)+pow(Eff_TP_Double_123456.GetErrorYlow(i),2));
        res1veyh_123456(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12345.GetErrorYhigh(i),2)+pow(Eff_TP_Double_123456.GetErrorYhigh(i),2));
        if(Fits){
        res2vx_123456(i)   = i;
        res2vexl_123456(i) = MCRelEff_Barrel_12345.GetErrorXlow(i);
        res2vexh_123456(i) = MCRelEff_Barrel_12345.GetErrorXhigh(i);
        res2vy_123456(i)   = y2-y0;
        res2veyl_123456(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12345.GetErrorYlow(i),2)+pow(tp_eff_123456->GetErrorYlow(i),2));
        res2veyh_123456(i) = TMath::Sqrt(pow(MCRelEff_Barrel_12345.GetErrorYhigh(i),2)+pow(tp_eff_123456->GetErrorYhigh(i),2));}
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
	if(Fits){
	TP_Res2_123456.SetLineColor(4);
	TP_Res2_123456.SetMarkerStyle(23);
	TP_Res2_123456.SetLineWidth(2);
	TP_Res2_123456.SetMarkerColor(4);
	TP_Res2_123456.Draw("PSAME");}
	TP_Res1_123456.SetTitle("TP residuals from MC Rel Eff: MC TP w/o back (red), Data (blue)");
	TP_Res1_123456.GetXaxis()->SetRangeUser(xmin,xmax);
	TP_Res1_123456.GetYaxis()->SetRangeUser(-0.3,0.3);
	ResTP_123456->Write("ResTP_MuID.root");
	ResTP_123456->Close();
	
	TVectorD res1vx_global(n_global);
        TVectorD res1vy_global(n_global);
        TVectorD res1vexl_global(n_global);
        TVectorD res1vexh_global(n_global);
        TVectorD res1veyl_global(n_global);
        TVectorD res1veyh_global(n_global);
        TVectorD res2vx_global(n_global);
        TVectorD res2vy_global(n_global);
        TVectorD res2vexl_global(n_global);
        TVectorD res2vexh_global(n_global);
        TVectorD res2veyl_global(n_global);
        TVectorD res2veyh_global(n_global);
       
        for ( int i = 0; i < n_global; ++i ){
        double x0 = 0., y0 = 0., x1 = 0., y1 = 0., x2 = 0., y2 = 0.;
        MCEff_global_Barrel.GetPoint(i, x0, y0);
        Eff_TP_Double_global.GetPoint(i, x1, y1);
        if(Fits)tp_eff_global->GetPoint(i, x2, y2);
        res1vx_global(i)   = i;
        res1vexl_global(i) = MCEff_global_Barrel.GetErrorXlow(i);
        res1vexh_global(i) = MCEff_global_Barrel.GetErrorXhigh(i);
        res1vy_global(i)   = y1-y0;
        res1veyl_global(i) = TMath::Sqrt(pow(MCEff_global_Barrel.GetErrorYlow(i),2)+pow(Eff_TP_Double_global.GetErrorYlow(i),2));
        res1veyh_global(i) = TMath::Sqrt(pow(MCEff_global_Barrel.GetErrorYhigh(i),2)+pow(Eff_TP_Double_global.GetErrorYhigh(i),2));
        if(Fits){
        res2vx_global(i)   = i;
        res2vexl_global(i) = MCEff_global_Barrel.GetErrorXlow(i);
        res2vexh_global(i) = MCEff_global_Barrel.GetErrorXhigh(i);
        res2vy_global(i)   = y2-y0;
        res2veyl_global(i) = TMath::Sqrt(pow(MCEff_global_Barrel.GetErrorYlow(i),2)+pow(tp_eff_global->GetErrorYlow(i),2));
        res2veyh_global(i) = TMath::Sqrt(pow(MCEff_global_Barrel.GetErrorYhigh(i),2)+pow(tp_eff_global->GetErrorYhigh(i),2));}
        }
              
        TP_global->cd();
        TGraphAsymmErrors TP_Res1_global(res1vx_global, res1vy_global, res1vexl_global, res1vexh_global, res1veyl_global, res1veyh_global);
        TGraphAsymmErrors TP_Res2_global(res2vx_global, res2vy_global, res2vexl_global, res2vexh_global, res2veyl_global, res2veyh_global);
        TCanvas *ResTP_global = new TCanvas;
        ResTP_global->SetGrid();
        TP_Res1_global.SetLineColor(2);
	TP_Res1_global.SetMarkerStyle(21);
	TP_Res1_global.SetLineWidth(2);
	TP_Res1_global.SetMarkerColor(2);
	TP_Res1_global.Draw("AP");
	if(Fits){
	TP_Res2_global.SetLineColor(4);
	TP_Res2_global.SetMarkerStyle(23);
	TP_Res2_global.SetLineWidth(2);
	TP_Res2_global.SetMarkerColor(4);
	TP_Res2_global.Draw("PSAME");}
	TP_Res1_global.SetTitle("TP residuals from MC Global Eff: MC TP w/o back (red), Data (blue)");
	TP_Res1_global.GetXaxis()->SetRangeUser(xmin,xmax);
	TP_Res1_global.GetYaxis()->SetRangeUser(-0.3,0.3);
	ResTP_global->Write("ResTP_Global.root");
	ResTP_global->Close();                                       */
	
	tpr.close();
					
  outplots->Write();
  outplots->Close();
}
