#include <vector>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <math.h>

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

//#include "tdrstyle.C"

using namespace std;

void AllTogetherRec(){
        
gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);


// For the Pad:
  gStyle->SetPadBorderMode(0);
  // gStyle->SetPadBorderSize(Width_t size = 1);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);

// For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);

	//uno
	TFile *file_1 = TFile::Open("recJets_DeltaR_0e0.root");
	//due
	TFile *file_2 = TFile::Open("recJets_DeltaR_0e2.root");
	//tre
	TFile *file_3 = TFile::Open("recJets_DeltaR_0e3.root");
	//quattro
	TFile *file_4 = TFile::Open("recJets_DeltaR_0e4.root");
	//cinque
	TFile *file_5 = TFile::Open("recJets_DeltaR_0e5.root");


	//Output
	string out = "Insieme_RecJets";        
	string outputroot = out;
	outputroot+=".root";
	string outputpdf = out;
        outputpdf+=".pdf";
	string outputpng = out;
        outputpng+=".png";
	string outputC = out;
        outputC+=".C";
	TFile* outplots = new TFile(outputroot.c_str(), "RECREATE");	

	//colori	
	int col_1 = 1;
	int col_2 = 2;
	int col_3 = 3;
	int col_4 = 4;
	int col_5 = 6;

	//SetRangeUser
	double nminX = 0.0; 
	double nmaxX = 6.0;
	double nminY = 0.0; 
	double nmaxY = 0.07;

	//Legenda
	string Leg_1 = "#DeltaR = 0.0";
	string Leg_2 = "#DeltaR = 0.2";
	string Leg_3 = "#DeltaR = 0.3";
	string Leg_4 = "#DeltaR = 0.4";
	string Leg_5 = "#DeltaR = 0.5";

	string Title_Excl = "Exclusive Jet Rate: Contamination rate";
	string Title_Incl = "Inclusive Jet Rate: Contamination rate";

        string Insieme = "Insieme_RecJets";
	string Excl = "_Exclusive";
	string Incl = "_Inclusive";
	string png = ".png";

	//histo 1 Exclusive
	TH1D* histo_1_Excl = (TH1D*) file_1->Get("JetRate/Exclusive/histoDiv_NUM");
	//histo 2 Exclusive
	TH1D* histo_2_Excl = (TH1D*) file_2->Get("JetRate/Exclusive/histoDiv_NUM");
	//histo 3 Exclusive
	TH1D* histo_3_Excl = (TH1D*) file_3->Get("JetRate/Exclusive/histoDiv_NUM");
	//histo 4 Exclusive
	TH1D* histo_4_Excl = (TH1D*) file_4->Get("JetRate/Exclusive/histoDiv_NUM");
	//histo 5 Exclusive
	TH1D* histo_5_Excl = (TH1D*) file_5->Get("JetRate/Exclusive/histoDiv_NUM");


	TCanvas *Canva_Excl = new TCanvas();
        Canva_Excl->Range(0,0,25,25);
        Canva_Excl->SetLeftMargin(0.160);

	histo_1_Excl->SetLineColor(col_1);
	histo_1_Excl->SetLineWidth(2);
	histo_1_Excl->SetTitle(Title_Excl.c_str());
	histo_1_Excl->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histo_1_Excl->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histo_1_Excl->GetXaxis()->SetTitle("Number of rec Jets");
	histo_1_Excl->GetYaxis()->SetTitle("Ratio");
        histo_1_Excl->GetYaxis()->SetTitleOffset(1.5);
	histo_1_Excl->SetMarkerStyle(20);
	histo_1_Excl->SetMarkerColor(col_1);
	histo_1_Excl->Draw("histo PE");	

	histo_2_Excl->SetLineColor(col_2);
	histo_2_Excl->SetLineWidth(2);
        histo_2_Excl->SetMarkerStyle(21);
        histo_2_Excl->SetMarkerColor(col_2);
	histo_2_Excl->Draw("histo PE same");

	histo_3_Excl->SetLineColor(col_3);
	histo_3_Excl->SetLineWidth(2);
        histo_3_Excl->SetMarkerStyle(22);
        histo_3_Excl->SetMarkerColor(col_3);
	histo_3_Excl->Draw("histo PE same");

	histo_4_Excl->SetLineColor(col_4);
	histo_4_Excl->SetLineWidth(2);
        histo_4_Excl->SetMarkerStyle(23);
        histo_4_Excl->SetMarkerColor(col_4);
	histo_4_Excl->Draw("histo PE same");

	histo_5_Excl->SetLineColor(col_5);
	histo_5_Excl->SetLineWidth(2);
        histo_5_Excl->SetMarkerStyle(24);
        histo_5_Excl->SetMarkerColor(col_5);
	histo_5_Excl->Draw("histo PE same");


	TLegend *Leg_Excl = new TLegend(0.26,0.63,0.78,0.88);
	Leg_Excl->SetFillColor(0);
	Leg_Excl->SetBorderSize(0);
	Leg_Excl->SetNColumns(3);
	Leg_Excl->AddEntry(histo_1_Excl, Leg_1.c_str());
   	Leg_Excl->AddEntry(histo_2_Excl, Leg_2.c_str());
   	Leg_Excl->AddEntry(histo_3_Excl, Leg_3.c_str());
   	Leg_Excl->AddEntry(histo_4_Excl, Leg_4.c_str());
   	Leg_Excl->AddEntry(histo_5_Excl, Leg_5.c_str());
   	Leg_Excl->Draw("LP");


	Canva_Excl->Write((Insieme + Excl).c_str());
        Canva_Excl->SaveAs((Insieme + Excl + png).c_str());
	Canva_Excl->Close();

	//histo 1 Inclusive
	TH1D* histo_1_Incl = (TH1D*) file_1->Get("JetRate/Inclusive/histoDiv_NUM");
	//histo 2 Inclusive
	TH1D* histo_2_Incl = (TH1D*) file_2->Get("JetRate/Inclusive/histoDiv_NUM");
	//histo 3 Inclusive
	TH1D* histo_3_Incl = (TH1D*) file_3->Get("JetRate/Inclusive/histoDiv_NUM");
	//histo 4 Inclusive
	TH1D* histo_4_Incl = (TH1D*) file_4->Get("JetRate/Inclusive/histoDiv_NUM");
	//histo 5 Inclusive
	TH1D* histo_5_Incl = (TH1D*) file_5->Get("JetRate/Inclusive/histoDiv_NUM");


	TCanvas *Canva_Incl = new TCanvas();
        Canva_Incl->Range(0,0,25,25);
        Canva_Incl->SetLeftMargin(0.160);

	histo_1_Incl->SetLineColor(col_1);
	histo_1_Incl->SetLineWidth(2);
	histo_1_Incl->SetTitle(Title_Incl.c_str());
	histo_1_Incl->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histo_1_Incl->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histo_1_Incl->GetXaxis()->SetTitle("Number of rec Jets");
	histo_1_Incl->GetYaxis()->SetTitle("Ratio");
        histo_1_Incl->GetYaxis()->SetTitleOffset(1.5);
	histo_1_Incl->SetMarkerStyle(20);
	histo_1_Incl->SetMarkerColor(col_1);
	histo_1_Incl->Draw("histo PE");	

	histo_2_Incl->SetLineColor(col_2);
	histo_2_Incl->SetLineWidth(2);
        histo_2_Incl->SetMarkerStyle(21);
        histo_2_Incl->SetMarkerColor(col_2);
	histo_2_Incl->Draw("histo PE same");

	histo_3_Incl->SetLineColor(col_3);
	histo_3_Incl->SetLineWidth(2);
        histo_3_Incl->SetMarkerStyle(22);
        histo_3_Incl->SetMarkerColor(col_3);
	histo_3_Incl->Draw("histo PE same");

	histo_4_Incl->SetLineColor(col_4);
	histo_4_Incl->SetLineWidth(2);
        histo_4_Incl->SetMarkerStyle(23);
        histo_4_Incl->SetMarkerColor(col_4);
	histo_4_Incl->Draw("histo PE same");

	histo_5_Incl->SetLineColor(col_5);
	histo_5_Incl->SetLineWidth(2);
        histo_5_Incl->SetMarkerStyle(24);
        histo_5_Incl->SetMarkerColor(col_5);
	histo_5_Incl->Draw("histo PE same");


	TLegend *Leg_Incl = new TLegend(0.26,0.63,0.78,0.88);
	Leg_Incl->SetFillColor(0);
	Leg_Incl->SetBorderSize(0);
	Leg_Incl->SetNColumns(3);
	Leg_Incl->AddEntry(histo_1_Incl, Leg_1.c_str());
   	Leg_Incl->AddEntry(histo_2_Incl, Leg_2.c_str());
   	Leg_Incl->AddEntry(histo_3_Incl, Leg_3.c_str());
   	Leg_Incl->AddEntry(histo_4_Incl, Leg_4.c_str());
   	Leg_Incl->AddEntry(histo_5_Incl, Leg_5.c_str());
   	Leg_Incl->Draw("LP");


	Canva_Incl->Write((Insieme + Incl).c_str());
        Canva_Incl->SaveAs((Insieme + Incl + png).c_str());
	Canva_Incl->Close();


	
  outplots->cd();
  outplots->Write();
  outplots->Close();
}


