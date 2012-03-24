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

void AllTogetherSingleMu(){
        
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

  string DeltaR = "DeltaR_";
  string DeltaR_e = "0e0"; 
  float deltaR = 0.0;

  string partial_file = "Overlap_SingleMu_DeltaR";
  string underscore = "_";

  string ZeroJets = "0jets"; 
  string UnJets = "1jets";
  string DueJets = "2jets";
  string TreJets = "3jets";
  string QuattroJets = "4jets";
  string png = ".png";
  string root = ".root";

	//uno
	TFile *file_1 = TFile::Open((partial_file + underscore + DeltaR_e + underscore + ZeroJets + root).c_str());
	//due
	TFile *file_2 = TFile::Open((partial_file + underscore + DeltaR_e + underscore + UnJets + root).c_str());
	//tre
	TFile *file_3 = TFile::Open((partial_file + underscore + DeltaR_e + underscore + DueJets + root).c_str());
	//quattro
	TFile *file_4 = TFile::Open((partial_file + underscore + DeltaR_e + underscore + TreJets + root).c_str());
	//cinque
	TFile *file_5 = TFile::Open((partial_file + underscore + DeltaR_e + underscore + QuattroJets + root).c_str());


	//Output
	string out = "Insieme_";        
	string outputroot = out;
	outputroot+=".root";
	string outputpdf = out;
        outputpdf+=".pdf";
	string outputpng = out;
        outputpng+=".png";
	string outputC = out;
        outputC+=".C";
	TFile* outplots = new TFile((out + DeltaR + DeltaR_e + root).c_str(), "RECREATE");	

	//colori	
	int col_1 = 1;
	int col_2 = 2;
	int col_3 = 3;
	int col_4 = 4;
	int col_5 = 6;

	//SetRangeUser
	double nminX_pt = 0.0; 
	double nmaxX_pt = 180.0;
	double nminY_pt = 0.7; 
	double nmaxY_pt = 1.15;
	double nminX_eta = -3.0; 
	double nmaxX_eta = 3.0;
	double nminY_eta = 0.5; 
	double nmaxY_eta = 1.2;

	//Legenda
	string Leg_1 = "0 jets";
	string Leg_2 = "1 jets";
	string Leg_3 = "2 jets";
	string Leg_4 = "3 jets";
	string Leg_5 = "4 jets";


//----------------------------

TDirectory *pt = new TDirectory();
pt = outplots->mkdir("pt");
pt->cd();

	string Title_pt = "Single #mu p_{T} Efficiency";
	string pt_s = "_pt";

	//histo 1 pt
	TH1D* histo_1_pt = (TH1D*) file_1->Get("pt/histoDiv_NUM_pt");
	//histo 2 pt
	TH1D* histo_2_pt = (TH1D*) file_2->Get("pt/histoDiv_NUM_pt");
	//histo 3 pt
	TH1D* histo_3_pt = (TH1D*) file_3->Get("pt/histoDiv_NUM_pt");
	//histo 4 pt
	TH1D* histo_4_pt = (TH1D*) file_4->Get("pt/histoDiv_NUM_pt");
	//histo 5 pt
	TH1D* histo_5_pt = (TH1D*) file_5->Get("pt/histoDiv_NUM_pt");


	TCanvas *Canva_pt = new TCanvas();
        Canva_pt->Range(0,0,25,25);
        Canva_pt->SetLeftMargin(0.160);

	histo_1_pt->SetLineColor(col_1);
	histo_1_pt->SetLineWidth(2);
	histo_1_pt->SetTitle(Title_pt.c_str());
	histo_1_pt->GetXaxis()->SetRangeUser(nminX_pt, nmaxX_pt);
	histo_1_pt->GetYaxis()->SetRangeUser(nminY_pt, nmaxY_pt);
	histo_1_pt->GetXaxis()->SetTitle("gen #mu p_{T}");
	histo_1_pt->GetYaxis()->SetTitle("Ratio");
        histo_1_pt->GetYaxis()->SetTitleOffset(1.5);
	histo_1_pt->SetMarkerStyle(20);
	histo_1_pt->SetMarkerColor(col_1);
	histo_1_pt->Draw("histo PE");	

	histo_2_pt->SetLineColor(col_2);
	histo_2_pt->SetLineWidth(2);
        histo_2_pt->SetMarkerStyle(21);
        histo_2_pt->SetMarkerColor(col_2);
	histo_2_pt->Draw("histo PE same");

	histo_3_pt->SetLineColor(col_3);
	histo_3_pt->SetLineWidth(2);
        histo_3_pt->SetMarkerStyle(22);
        histo_3_pt->SetMarkerColor(col_3);
	histo_3_pt->Draw("histo PE same");

	histo_4_pt->SetLineColor(col_4);
	histo_4_pt->SetLineWidth(2);
        histo_4_pt->SetMarkerStyle(23);
        histo_4_pt->SetMarkerColor(col_4);
	histo_4_pt->Draw("histo PE same");

	histo_5_pt->SetLineColor(col_5);
	histo_5_pt->SetLineWidth(2);
        histo_5_pt->SetMarkerStyle(24);
        histo_5_pt->SetMarkerColor(col_5);
	histo_5_pt->Draw("histo PE same");

	TPaveText *Tex_pt = new TPaveText();
	Tex_pt->SetFillColor(0);
	Tex_pt->SetBorderSize(0);
	Tex_pt->AddText(Form("#Delta R = %.2f", deltaR));
   	Tex_pt->Draw("TR");

	TLegend *Leg_pt = new TLegend(0.49,0.67,0.88,0.88);
	Leg_pt->SetFillColor(0);
	Leg_pt->SetBorderSize(0);
	Leg_pt-> SetNColumns(3);
	Leg_pt->AddEntry(histo_1_pt, Leg_1.c_str());
   	Leg_pt->AddEntry(histo_2_pt, Leg_2.c_str());
   	Leg_pt->AddEntry(histo_3_pt, Leg_3.c_str());
   	Leg_pt->AddEntry(histo_4_pt, Leg_4.c_str());
   	Leg_pt->AddEntry(histo_5_pt, Leg_5.c_str());
   	Leg_pt->Draw("LP");

	Canva_pt->Write((out + DeltaR + DeltaR_e + pt_s).c_str());
        Canva_pt->SaveAs((out + DeltaR + DeltaR_e + pt_s + png).c_str());
	Canva_pt->Close();

pt->cd();

//----------------------------

TDirectory *eta = new TDirectory();
eta = outplots->mkdir("eta");
eta->cd();

	string Title_eta = "Single #mu #eta Efficiency";
	string eta_s = "_eta";

	//histo 1 eta
	TH1D* histo_1_eta = (TH1D*) file_1->Get("eta/histoDiv_NUM_eta");
	//histo 2 eta
	TH1D* histo_2_eta = (TH1D*) file_2->Get("eta/histoDiv_NUM_eta");
	//histo 3 eta
	TH1D* histo_3_eta = (TH1D*) file_3->Get("eta/histoDiv_NUM_eta");
	//histo 4 eta
	TH1D* histo_4_eta = (TH1D*) file_4->Get("eta/histoDiv_NUM_eta");
	//histo 5 eta
	TH1D* histo_5_eta = (TH1D*) file_5->Get("eta/histoDiv_NUM_eta");


	TCanvas *Canva_eta = new TCanvas();
        Canva_eta->Range(0,0,25,25);
        Canva_eta->SetLeftMargin(0.160);

	histo_1_eta->SetLineColor(col_1);
	histo_1_eta->SetLineWidth(2);
	histo_1_eta->SetTitle(Title_eta.c_str());
	histo_1_eta->GetXaxis()->SetRangeUser(nminX_eta, nmaxX_eta);
	histo_1_eta->GetYaxis()->SetRangeUser(nminY_eta, nmaxY_eta);
	histo_1_eta->GetXaxis()->SetTitle("gen #mu #eta");
	histo_1_eta->GetYaxis()->SetTitle("Ratio");
        histo_1_eta->GetYaxis()->SetTitleOffset(1.5);
	histo_1_eta->SetMarkerStyle(20);
	histo_1_eta->SetMarkerColor(col_1);
	histo_1_eta->Draw("histo PE");	

	histo_2_eta->SetLineColor(col_2);
	histo_2_eta->SetLineWidth(2);
        histo_2_eta->SetMarkerStyle(21);
        histo_2_eta->SetMarkerColor(col_2);
	histo_2_eta->Draw("histo PE same");

	histo_3_eta->SetLineColor(col_3);
	histo_3_eta->SetLineWidth(2);
        histo_3_eta->SetMarkerStyle(22);
        histo_3_eta->SetMarkerColor(col_3);
	histo_3_eta->Draw("histo PE same");

	histo_4_eta->SetLineColor(col_4);
	histo_4_eta->SetLineWidth(2);
        histo_4_eta->SetMarkerStyle(23);
        histo_4_eta->SetMarkerColor(col_4);
	histo_4_eta->Draw("histo PE same");

	histo_5_eta->SetLineColor(col_5);
	histo_5_eta->SetLineWidth(2);
        histo_5_eta->SetMarkerStyle(24);
        histo_5_eta->SetMarkerColor(col_5);
	histo_5_eta->Draw("histo PE same");

	TPaveText *Tex_eta = new TPaveText();
	Tex_eta->SetFillColor(0);
	Tex_eta->SetBorderSize(0);
	Tex_eta->AddText(Form("#Delta R = %.2f", deltaR));
   	Tex_eta->Draw("TR");

	TLegend *Leg_eta = new TLegend(0.49,0.67,0.88,0.88);
	Leg_eta->SetFillColor(0);
	Leg_eta->SetBorderSize(0);
	Leg_eta-> SetNColumns(3);
	Leg_eta->AddEntry(histo_1_eta, Leg_1.c_str());
   	Leg_eta->AddEntry(histo_2_eta, Leg_2.c_str());
   	Leg_eta->AddEntry(histo_3_eta, Leg_3.c_str());
   	Leg_eta->AddEntry(histo_4_eta, Leg_4.c_str());
   	Leg_eta->AddEntry(histo_5_eta, Leg_5.c_str());
   	Leg_eta->Draw("LP");

	Canva_eta->Write((out + DeltaR + DeltaR_e + eta_s).c_str());
        Canva_eta->SaveAs((out + DeltaR + DeltaR_e + eta_s + png).c_str());
	Canva_eta->Close();

eta->cd();

	
  outplots->cd();
  outplots->Write();
  outplots->Close();
}


