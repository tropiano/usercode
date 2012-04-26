//------------------------------------------------------------------------------------------------------
// Macro strutturata per una disposizione di cartelle simile:
//
//
//               Da qui si lancia la macro con root: 
//               --> macro/
//               
//
// grafici/ --
//
//               Qui vengono scritti i grafici a partire da quelli già prodotti dalla macro LeadingSecondMu*.C:                  
//               --> pt_x/       -->  Address_sample/         -->  Grafici_Insieme/
//                   (es: pt_50/)     (es: DYtoLL_All_Fall11/)     
//------------------------------------------------------------------------------------------------------

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

void AllTogetherLeadSecMuFalse_COMBO(){
        
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);

	//For the Canvas
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

	// For the Frame:
  	gStyle->SetFrameBorderMode(0);
  	gStyle->SetFrameBorderSize(1);
  	gStyle->SetFrameFillColor(0);
  	gStyle->SetFrameFillStyle(0);
  	gStyle->SetFrameLineColor(1);
  	gStyle->SetFrameLineStyle(1);
  	gStyle->SetFrameLineWidth(1);


	string root = ".root";
	string pdf = ".pdf";       
	string png = ".png";       
	string ps = ".ps";
	string C = ".C"; 
	string barra = "/"; 
	string underscore = "_";

	string Address_radice = "~/Scrivania/html/AnalysisMuons/combo/";

	string pt_x = "pt50";
	string und_pt_x = (underscore + pt_x).c_str(); 
	string pt_x_bar = (pt_x + barra).c_str(); 
	string und_pt_x_bar  = (underscore + pt_x + barra).c_str(); 

	string DeltaR_0ex = "DeltaR_0e0";
	string DeltaR_0ex_bar = (DeltaR_0ex + barra).c_str();
	string DeltaR_0ex_und = (DeltaR_0ex + underscore).c_str();
	string und_DeltaR_0ex = (underscore + DeltaR_0ex).c_str();
	float deltaR = 0.0;

	string Address_sample = "DYtoLL_All_Fall11";
//	string Address_sample = "DYtoLL_All_Summer11";
//	string Address_sample = "DYtoMuMu_All_Summer11";
	string Address_sample_und = (Address_sample + underscore).c_str();
	string Address_sample_bar = (Address_sample + barra).c_str(); 

//	string Address_Vertex = "Vertex_Less8/";
//	string Address_Vertex = "Vertex_Great8/";
	string Address_Vertex = "Vertex_NoRequirement/";

	string partial_histo_LEA_pt;
	string partial_histo_SEC_pt;
	string partial_histo_LEA_eta;
	string partial_histo_SEC_eta;
	if (Address_Vertex == "Vertex_Less8/"){
		partial_histo_LEA_pt = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Less8/genLeadMuPtFalse_VertWindA_"; 
		partial_histo_SEC_pt = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Less8/genSecondMuPtFalse_VertWindA_";
		partial_histo_LEA_eta = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Less8/genLeadMuEtaFalse_VertWindA_"; 
		partial_histo_SEC_eta = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Less8/genSecondMuEtaFalse_VertWindA_";
	}
	else if (Address_Vertex == "Vertex_Great8/"){
		partial_histo_LEA_pt = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Great8/genLeadMuPtFalse_VertWindB_"; 
		partial_histo_SEC_pt = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Great8/genSecondMuPtFalse_VertWindB_";
		partial_histo_LEA_eta = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Great8/genLeadMuEtaFalse_VertWindB_"; 
		partial_histo_SEC_eta = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Great8/genSecondMuEtaFalse_VertWindB_";
	}
	else if (Address_Vertex == "Vertex_NoRequirement/"){
		partial_histo_LEA_pt = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_No/genLeadMuPtFalse_"; 
		partial_histo_SEC_pt = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_No/genSecondMuPtFalse_";
		partial_histo_LEA_eta = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_No/genLeadMuEtaFalse_"; 
		partial_histo_SEC_eta = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_No/genSecondMuEtaFalse_";
	}
	else cout << "che sia il caso di scrivere perbenino?" << endl;

	string Address_IsoRho = "IsoRho_SI/"; 
//	string Address_IsoRho = "IsoRho_NO/";
	if (Address_IsoRho == "IsoRho_NO/"){
		string iso = "iso_"; 
		partial_histo_LEA_pt += iso;
		partial_histo_SEC_pt += iso;
		partial_histo_LEA_eta += iso;
		partial_histo_SEC_eta += iso;
	}


	//file Input
	string file_cartella = (Address_sample_und + DeltaR_0ex + und_pt_x_bar).c_str();
	string pre_Address_totale_in = (Address_radice + pt_x_bar + Address_sample_bar + file_cartella).c_str();

	string pre_cartella_in = "LeadSecMu/";
	string cartella_in = (pre_cartella_in + DeltaR_0ex_bar).c_str();
	string Address_totale_in = (pre_Address_totale_in + Address_Vertex + Address_IsoRho + cartella_in).c_str();

	string pre_in = "LeadSecMu_";
	string pre_file_in = (pre_in + DeltaR_0ex_und).c_str();

	string ZeroJets = "0jets"; 
	string UnJets = "1jets";
	string DueJets = "2jets";
	string TreJets = "3jets";
	string QuattroJets = "4jets";

	string file_in_1 = (pre_file_in + ZeroJets + root).c_str();
	string file_in_2 = (pre_file_in + UnJets + root).c_str();
	string file_in_3 = (pre_file_in + DueJets + root).c_str();
	string file_in_4 = (pre_file_in + TreJets + root).c_str();
	string file_in_5 = (pre_file_in + QuattroJets + root).c_str();

	//uno
	TFile *file_1 = TFile::Open((Address_totale_in + file_in_1).c_str());
	//due
	TFile *file_2 = TFile::Open((Address_totale_in + file_in_2).c_str());
	//tre
	TFile *file_3 = TFile::Open((Address_totale_in + file_in_3).c_str());
	//quattro
	TFile *file_4 = TFile::Open((Address_totale_in + file_in_4).c_str());
	//cinque
	TFile *file_5 = TFile::Open((Address_totale_in + file_in_5).c_str());


	//Output
	string out_cartella = "Grafici_Insieme/";
	string Address_totale_out = (Address_radice + out_cartella).c_str();
             
	string pre_file_out = "AllTogetherLeadSecMuFalse_";        
	string file_out = (pre_file_out + DeltaR_0ex + und_pt_x ).c_str();
	string outputroot = (Address_totale_out + file_out + root).c_str();

	TFile* outplots = new TFile(outputroot.c_str(), "RECREATE");	


	//colori	
	int col_1 = 1;
	int col_2 = 2;
	int col_3 = 3;
	int col_4 = 4;
	int col_5 = 6;

	//SetRangeUser
	double nminX_pt = 0.0; 
	double nmaxX_pt = 180.0;
	double nminY_pt = 0.9; 
	double nmaxY_pt = 1000000000;
	double nminX_eta = -3.0; 
	double nmaxX_eta = 3.0;
	double nminY_eta = 0.9; 
	double nmaxY_eta = 1000000000;

	//Legenda
	string Leg_1 = "0 jets";
	string Leg_2 = "1 jets";
	string Leg_3 = "2 jets";
	string Leg_4 = "3 jets";
	string Leg_5 = "4 jets";

        string Insieme = "AllTogetherLeadSecMuFalse";
//----------------------------

TDirectory *pt = new TDirectory();
pt = outplots->mkdir("pt");
pt->cd();

	string Title_LEA_pt = "Leading #mu p_{T} vs Jets rate";

	string histo_string_LEA_pt = "pt/histoClone_LEA_pt";
	//histo 1 LEA pt
	TH1D* histo_1_LEA_pt = (TH1D*) file_1->Get(histo_string_LEA_pt.c_str());
	//histo 2 LEA pt
	TH1D* histo_2_LEA_pt = (TH1D*) file_2->Get(histo_string_LEA_pt.c_str());
	//histo 3 LEA pt
	TH1D* histo_3_LEA_pt = (TH1D*) file_3->Get(histo_string_LEA_pt.c_str());
	//histo 4 LEA pt
	TH1D* histo_4_LEA_pt = (TH1D*) file_4->Get(histo_string_LEA_pt.c_str());
	//histo 5 LEA pt
	TH1D* histo_5_LEA_pt = (TH1D*) file_5->Get(histo_string_LEA_pt.c_str());



	TCanvas *Canva_LEA_pt = new TCanvas();
	Canva_LEA_pt->SetLogy();
        Canva_LEA_pt->Range(0,0,25,25);
        Canva_LEA_pt->SetLeftMargin(0.160);

	histo_1_LEA_pt->SetLineColor(col_1);
	histo_1_LEA_pt->SetLineWidth(2);
	histo_1_LEA_pt->SetTitle(Title_LEA_pt.c_str());
	//histo_1_LEA_pt->SetTitle(0);
	histo_1_LEA_pt->GetXaxis()->SetRangeUser(nminX_pt, nmaxX_pt);
	histo_1_LEA_pt->GetYaxis()->SetRangeUser(nminY_pt, nmaxY_pt);
	histo_1_LEA_pt->GetXaxis()->SetTitle("gen #mu p_{T}");
	histo_1_LEA_pt->GetYaxis()->SetTitle("Events");
        histo_1_LEA_pt->GetYaxis()->SetTitleOffset(1.5);
	histo_1_LEA_pt->SetMarkerStyle(20);
	histo_1_LEA_pt->SetMarkerColor(col_1);
	histo_1_LEA_pt->Draw("histo PE");	

	histo_2_LEA_pt->SetLineColor(col_2);
	histo_2_LEA_pt->SetLineWidth(2);
        histo_2_LEA_pt->SetMarkerStyle(21);
        histo_2_LEA_pt->SetMarkerColor(col_2);
	histo_2_LEA_pt->Draw("histo PE same");

	histo_3_LEA_pt->SetLineColor(col_3);
	histo_3_LEA_pt->SetLineWidth(2);
        histo_3_LEA_pt->SetMarkerStyle(22);
        histo_3_LEA_pt->SetMarkerColor(col_3);
	histo_3_LEA_pt->Draw("histo PE same");

	histo_4_LEA_pt->SetLineColor(col_4);
	histo_4_LEA_pt->SetLineWidth(2);
        histo_4_LEA_pt->SetMarkerStyle(23);
        histo_4_LEA_pt->SetMarkerColor(col_4);
	histo_4_LEA_pt->Draw("histo PE same");

	histo_5_LEA_pt->SetLineColor(col_5);
	histo_5_LEA_pt->SetLineWidth(2);
        histo_5_LEA_pt->SetMarkerStyle(24);
        histo_5_LEA_pt->SetMarkerColor(col_5);
	histo_5_LEA_pt->Draw("histo PE same");

	TPaveText *Tex_Delta_LEA_pt = new TPaveText();
	Tex_Delta_LEA_pt ->SetFillColor(0);
	Tex_Delta_LEA_pt ->SetBorderSize(0);
	Tex_Delta_LEA_pt ->AddText(Form("#Delta R = %.2f", deltaR));
   	Tex_Delta_LEA_pt ->Draw("TR");

	TLegend *Leg_LEA_pt  = new TLegend(0.49,0.67,0.88,0.88);
	Leg_LEA_pt ->SetFillColor(0);
	Leg_LEA_pt ->SetBorderSize(0);
	Leg_LEA_pt -> SetNColumns(3);
	Leg_LEA_pt ->AddEntry(histo_1_LEA_pt, Leg_1.c_str());
   	Leg_LEA_pt ->AddEntry(histo_2_LEA_pt, Leg_2.c_str());
   	Leg_LEA_pt ->AddEntry(histo_3_LEA_pt, Leg_3.c_str());
   	Leg_LEA_pt ->AddEntry(histo_4_LEA_pt, Leg_4.c_str());
   	Leg_LEA_pt ->AddEntry(histo_5_LEA_pt, Leg_5.c_str());
   	Leg_LEA_pt ->Draw("LP");

	TPaveText *Tex_LEA_pt = new TPaveText(0.207742,0.782667,0.441677,0.846667,"NDC");
	Tex_LEA_pt->SetFillColor(0);
	Tex_LEA_pt->SetBorderSize(0);
	Tex_LEA_pt->AddText("#splitline{CMS Simulation}{#sqrt{s} = 7 TeV}");
   	Tex_LEA_pt->Draw();

	string und_LEA_pt = "_LEA_pt";

	Canva_LEA_pt->Write((Insieme + und_DeltaR_0ex + und_LEA_pt).c_str());
        Canva_LEA_pt->SaveAs((Address_totale_out + Insieme + und_DeltaR_0ex + und_LEA_pt + pdf).c_str());
	Canva_LEA_pt->Close();


//////////////


	string Title_SEC_pt  = "Second #mu p_{T} vs Jets rate";

	string histo_string_SEC_pt  = "pt/histoClone_SEC_pt";
	//histo 1 LEA pt
	TH1D* histo_1_SEC_pt  = (TH1D*) file_1->Get(histo_string_SEC_pt .c_str());
	//histo 2 LEA pt
	TH1D* histo_2_SEC_pt  = (TH1D*) file_2->Get(histo_string_SEC_pt .c_str());
	//histo 3 LEA pt
	TH1D* histo_3_SEC_pt  = (TH1D*) file_3->Get(histo_string_SEC_pt .c_str());
	//histo 4 LEA pt
	TH1D* histo_4_SEC_pt  = (TH1D*) file_4->Get(histo_string_SEC_pt .c_str());
	//histo 5 LEA pt
	TH1D* histo_5_SEC_pt  = (TH1D*) file_5->Get(histo_string_SEC_pt .c_str());



	TCanvas *Canva_SEC_pt  = new TCanvas();
	Canva_SEC_pt ->SetLogy();
        Canva_SEC_pt ->Range(0,0,25,25);
        Canva_SEC_pt ->SetLeftMargin(0.160);

	histo_1_SEC_pt ->SetLineColor(col_1);
	histo_1_SEC_pt ->SetLineWidth(2);
	histo_1_SEC_pt ->SetTitle(Title_SEC_pt.c_str());
	//histo_1_SEC_pt ->SetTitle(0);
	histo_1_SEC_pt ->GetXaxis()->SetRangeUser(nminX_pt, nmaxX_pt);
	histo_1_SEC_pt ->GetYaxis()->SetRangeUser(nminY_pt, nmaxY_pt);
	histo_1_SEC_pt ->GetXaxis()->SetTitle("gen #mu p_{T}");
	histo_1_SEC_pt ->GetYaxis()->SetTitle("Events");
        histo_1_SEC_pt ->GetYaxis()->SetTitleOffset(1.5);
	histo_1_SEC_pt ->SetMarkerStyle(20);
	histo_1_SEC_pt ->SetMarkerColor(col_1);
	histo_1_SEC_pt ->Draw("histo PE");	

	histo_2_SEC_pt ->SetLineColor(col_2);
	histo_2_SEC_pt ->SetLineWidth(2);
        histo_2_SEC_pt ->SetMarkerStyle(21);
        histo_2_SEC_pt ->SetMarkerColor(col_2);
	histo_2_SEC_pt ->Draw("histo PE same");

	histo_3_SEC_pt ->SetLineColor(col_3);
	histo_3_SEC_pt ->SetLineWidth(2);
        histo_3_SEC_pt ->SetMarkerStyle(22);
        histo_3_SEC_pt ->SetMarkerColor(col_3);
	histo_3_SEC_pt ->Draw("histo PE same");

	histo_4_SEC_pt ->SetLineColor(col_4);
	histo_4_SEC_pt ->SetLineWidth(2);
        histo_4_SEC_pt ->SetMarkerStyle(23);
        histo_4_SEC_pt ->SetMarkerColor(col_4);
	histo_4_SEC_pt ->Draw("histo PE same");

	histo_5_SEC_pt ->SetLineColor(col_5);
	histo_5_SEC_pt ->SetLineWidth(2);
        histo_5_SEC_pt ->SetMarkerStyle(24);
        histo_5_SEC_pt ->SetMarkerColor(col_5);
	histo_5_SEC_pt ->Draw("histo PE same");

	TPaveText *Tex_Delta_SEC_pt  = new TPaveText();
	Tex_Delta_SEC_pt  ->SetFillColor(0);
	Tex_Delta_SEC_pt  ->SetBorderSize(0);
	Tex_Delta_SEC_pt  ->AddText(Form("#Delta R = %.2f", deltaR));
   	Tex_Delta_SEC_pt  ->Draw("TR");

	TLegend *Leg_SEC_pt   = new TLegend(0.49,0.67,0.88,0.88);
	Leg_SEC_pt  ->SetFillColor(0);
	Leg_SEC_pt  ->SetBorderSize(0);
	Leg_SEC_pt  -> SetNColumns(3);
	Leg_SEC_pt  ->AddEntry(histo_1_SEC_pt , Leg_1.c_str());
   	Leg_SEC_pt  ->AddEntry(histo_2_SEC_pt , Leg_2.c_str());
   	Leg_SEC_pt  ->AddEntry(histo_3_SEC_pt , Leg_3.c_str());
   	Leg_SEC_pt  ->AddEntry(histo_4_SEC_pt , Leg_4.c_str());
   	Leg_SEC_pt  ->AddEntry(histo_5_SEC_pt , Leg_5.c_str());
   	Leg_SEC_pt  ->Draw("LP");

	TPaveText *Tex_SEC_pt  = new TPaveText(0.207742,0.782667,0.441677,0.846667,"NDC");
	Tex_SEC_pt ->SetFillColor(0);
	Tex_SEC_pt ->SetBorderSize(0);
	Tex_SEC_pt ->AddText("#splitline{CMS Simulation}{#sqrt{s} = 7 TeV}");
   	Tex_SEC_pt ->Draw();

	string und_SEC_pt  = "_SEC_pt ";

	Canva_SEC_pt ->Write((Insieme + und_DeltaR_0ex + und_SEC_pt ).c_str());
        Canva_SEC_pt ->SaveAs((Address_totale_out + Insieme + und_DeltaR_0ex + und_SEC_pt  + pdf).c_str());
	Canva_SEC_pt ->Close();


pt->cd();

//----------------------------

TDirectory *eta = new TDirectory();
eta = outplots->mkdir("eta");
eta->cd();

	string Title_LEA_eta = "Leading #mu #eta vs Jets rate";

	string histo_string_LEA_eta = "eta/histoClone_LEA_eta";
	//histo 1 LEA eta
	TH1D* histo_1_LEA_eta = (TH1D*) file_1->Get(histo_string_LEA_eta.c_str());
	//histo 2 LEA eta
	TH1D* histo_2_LEA_eta = (TH1D*) file_2->Get(histo_string_LEA_eta.c_str());
	//histo 3 LEA eta
	TH1D* histo_3_LEA_eta = (TH1D*) file_3->Get(histo_string_LEA_eta.c_str());
	//histo 4 LEA eta
	TH1D* histo_4_LEA_eta = (TH1D*) file_4->Get(histo_string_LEA_eta.c_str());
	//histo 5 LEA eta
	TH1D* histo_5_LEA_eta = (TH1D*) file_5->Get(histo_string_LEA_eta.c_str());



	TCanvas *Canva_LEA_eta = new TCanvas();
	Canva_LEA_eta->SetLogy();
        Canva_LEA_eta->Range(0,0,25,25);
        Canva_LEA_eta->SetLeftMargin(0.160);

	histo_1_LEA_eta->SetLineColor(col_1);
	histo_1_LEA_eta->SetLineWidth(2);
	histo_1_LEA_eta->SetTitle(Title_LEA_eta.c_str());
	//histo_1_LEA_eta->SetTitle(0);
	histo_1_LEA_eta->GetXaxis()->SetRangeUser(nminX_eta, nmaxX_eta);
	histo_1_LEA_eta->GetYaxis()->SetRangeUser(nminY_eta, nmaxY_eta);
	histo_1_LEA_eta->GetXaxis()->SetTitle("gen #mu #eta");
	histo_1_LEA_eta->GetYaxis()->SetTitle("Events");
        histo_1_LEA_eta->GetYaxis()->SetTitleOffset(1.5);
	histo_1_LEA_eta->SetMarkerStyle(20);
	histo_1_LEA_eta->SetMarkerColor(col_1);
	histo_1_LEA_eta->Draw("histo PE");	

	histo_2_LEA_eta->SetLineColor(col_2);
	histo_2_LEA_eta->SetLineWidth(2);
        histo_2_LEA_eta->SetMarkerStyle(21);
        histo_2_LEA_eta->SetMarkerColor(col_2);
	histo_2_LEA_eta->Draw("histo PE same");

	histo_3_LEA_eta->SetLineColor(col_3);
	histo_3_LEA_eta->SetLineWidth(2);
        histo_3_LEA_eta->SetMarkerStyle(22);
        histo_3_LEA_eta->SetMarkerColor(col_3);
	histo_3_LEA_eta->Draw("histo PE same");

	histo_4_LEA_eta->SetLineColor(col_4);
	histo_4_LEA_eta->SetLineWidth(2);
        histo_4_LEA_eta->SetMarkerStyle(23);
        histo_4_LEA_eta->SetMarkerColor(col_4);
	histo_4_LEA_eta->Draw("histo PE same");

	histo_5_LEA_eta->SetLineColor(col_5);
	histo_5_LEA_eta->SetLineWidth(2);
        histo_5_LEA_eta->SetMarkerStyle(24);
        histo_5_LEA_eta->SetMarkerColor(col_5);
	histo_5_LEA_eta->Draw("histo PE same");

	TPaveText *Tex_Delta_LEA_eta = new TPaveText();
	Tex_Delta_LEA_eta ->SetFillColor(0);
	Tex_Delta_LEA_eta ->SetBorderSize(0);
	Tex_Delta_LEA_eta ->AddText(Form("#Delta R = %.2f", deltaR));
   	Tex_Delta_LEA_eta ->Draw("TR");

	TLegend *Leg_LEA_eta  = new TLegend(0.49,0.67,0.88,0.88);
	Leg_LEA_eta ->SetFillColor(0);
	Leg_LEA_eta ->SetBorderSize(0);
	Leg_LEA_eta -> SetNColumns(3);
	Leg_LEA_eta ->AddEntry(histo_1_LEA_eta, Leg_1.c_str());
   	Leg_LEA_eta ->AddEntry(histo_2_LEA_eta, Leg_2.c_str());
   	Leg_LEA_eta ->AddEntry(histo_3_LEA_eta, Leg_3.c_str());
   	Leg_LEA_eta ->AddEntry(histo_4_LEA_eta, Leg_4.c_str());
   	Leg_LEA_eta ->AddEntry(histo_5_LEA_eta, Leg_5.c_str());
   	Leg_LEA_eta ->Draw("LP");

	TPaveText *Tex_LEA_eta = new TPaveText(0.207742,0.782667,0.441677,0.846667,"NDC");
	Tex_LEA_eta->SetFillColor(0);
	Tex_LEA_eta->SetBorderSize(0);
	Tex_LEA_eta->AddText("#splitline{CMS Simulation}{#sqrt{s} = 7 TeV}");
   	Tex_LEA_eta->Draw();

	string und_LEA_eta = "_LEA_eta";

	Canva_LEA_eta->Write((Insieme + und_DeltaR_0ex + und_LEA_eta).c_str());
        Canva_LEA_eta->SaveAs((Address_totale_out + Insieme + und_DeltaR_0ex + und_LEA_eta + pdf).c_str());
	Canva_LEA_eta->Close();


//////////////


	string Title_SEC_eta  = "Second #mu #eta vs Jets rate";

	string histo_string_SEC_eta  = "eta/histoClone_SEC_eta";
	//histo 1 SEC pt
	TH1D* histo_1_SEC_eta  = (TH1D*) file_1->Get(histo_string_SEC_eta .c_str());
	//histo 2 SEC pt
	TH1D* histo_2_SEC_eta  = (TH1D*) file_2->Get(histo_string_SEC_eta .c_str());
	//histo 3 SEC pt
	TH1D* histo_3_SEC_eta  = (TH1D*) file_3->Get(histo_string_SEC_eta .c_str());
	//histo 4 SEC pt
	TH1D* histo_4_SEC_eta  = (TH1D*) file_4->Get(histo_string_SEC_eta .c_str());
	//histo 5 SEC pt
	TH1D* histo_5_SEC_eta  = (TH1D*) file_5->Get(histo_string_SEC_eta .c_str());



	TCanvas *Canva_SEC_eta  = new TCanvas();
	Canva_SEC_eta ->SetLogy();
        Canva_SEC_eta ->Range(0,0,25,25);
        Canva_SEC_eta ->SetLeftMargin(0.160);

	histo_1_SEC_eta ->SetLineColor(col_1);
	histo_1_SEC_eta ->SetLineWidth(2);
	histo_1_SEC_eta ->SetTitle(Title_SEC_eta.c_str());
	//histo_1_SEC_eta ->SetTitle(0);
	histo_1_SEC_eta ->GetXaxis()->SetRangeUser(nminX_eta, nmaxX_eta);
	histo_1_SEC_eta ->GetYaxis()->SetRangeUser(nminY_eta, nmaxY_eta);
	histo_1_SEC_eta ->GetXaxis()->SetTitle("gen #mu #eta");
	histo_1_SEC_eta ->GetYaxis()->SetTitle("Events");
        histo_1_SEC_eta ->GetYaxis()->SetTitleOffset(1.5);
	histo_1_SEC_eta ->SetMarkerStyle(20);
	histo_1_SEC_eta ->SetMarkerColor(col_1);
	histo_1_SEC_eta ->Draw("histo PE");	

	histo_2_SEC_eta ->SetLineColor(col_2);
	histo_2_SEC_eta ->SetLineWidth(2);
        histo_2_SEC_eta ->SetMarkerStyle(21);
        histo_2_SEC_eta ->SetMarkerColor(col_2);
	histo_2_SEC_eta ->Draw("histo PE same");

	histo_3_SEC_eta ->SetLineColor(col_3);
	histo_3_SEC_eta ->SetLineWidth(2);
        histo_3_SEC_eta ->SetMarkerStyle(22);
        histo_3_SEC_eta ->SetMarkerColor(col_3);
	histo_3_SEC_eta ->Draw("histo PE same");

	histo_4_SEC_eta ->SetLineColor(col_4);
	histo_4_SEC_eta ->SetLineWidth(2);
        histo_4_SEC_eta ->SetMarkerStyle(23);
        histo_4_SEC_eta ->SetMarkerColor(col_4);
	histo_4_SEC_eta ->Draw("histo PE same");

	histo_5_SEC_eta ->SetLineColor(col_5);
	histo_5_SEC_eta ->SetLineWidth(2);
        histo_5_SEC_eta ->SetMarkerStyle(24);
        histo_5_SEC_eta ->SetMarkerColor(col_5);
	histo_5_SEC_eta ->Draw("histo PE same");

	TPaveText *Tex_Delta_SEC_eta  = new TPaveText();
	Tex_Delta_SEC_eta  ->SetFillColor(0);
	Tex_Delta_SEC_eta  ->SetBorderSize(0);
	Tex_Delta_SEC_eta  ->AddText(Form("#Delta R = %.2f", deltaR));
   	Tex_Delta_SEC_eta  ->Draw("TR");

	TLegend *Leg_SEC_eta   = new TLegend(0.49,0.67,0.88,0.88);
	Leg_SEC_eta  ->SetFillColor(0);
	Leg_SEC_eta  ->SetBorderSize(0);
	Leg_SEC_eta  -> SetNColumns(3);
	Leg_SEC_eta  ->AddEntry(histo_1_SEC_eta , Leg_1.c_str());
   	Leg_SEC_eta  ->AddEntry(histo_2_SEC_eta , Leg_2.c_str());
   	Leg_SEC_eta  ->AddEntry(histo_3_SEC_eta , Leg_3.c_str());
   	Leg_SEC_eta  ->AddEntry(histo_4_SEC_eta , Leg_4.c_str());
   	Leg_SEC_eta  ->AddEntry(histo_5_SEC_eta , Leg_5.c_str());
   	Leg_SEC_eta  ->Draw("LP");

	TPaveText *Tex_SEC_eta  = new TPaveText(0.207742,0.782667,0.441677,0.846667,"NDC");
	Tex_SEC_eta ->SetFillColor(0);
	Tex_SEC_eta ->SetBorderSize(0);
	Tex_SEC_eta ->AddText("#splitline{CMS Simulation}{#sqrt{s} = 7 TeV}");
   	Tex_SEC_eta ->Draw();

	string und_SEC_eta  = "_SEC_eta ";

	Canva_SEC_eta ->Write((Insieme + und_DeltaR_0ex + und_SEC_eta ).c_str());
        Canva_SEC_eta ->SaveAs((Address_totale_out + Insieme + und_DeltaR_0ex + und_SEC_eta  + pdf).c_str());
	Canva_SEC_eta ->Close();


eta->cd();

	
  outplots->cd();
  outplots->Write();
  outplots->Close();
}

