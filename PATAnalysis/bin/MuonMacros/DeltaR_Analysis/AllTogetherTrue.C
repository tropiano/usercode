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
//               Qui vengono scritti i grafici a partire da quelli già prodotti dalle macro Divide_*.C:                  
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

void AllTogetherTrue_AN(){
        
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

	string Address_radice = "~/Scrivania/html/AnalysisMuons/grafici_AN/";

	string pt_x = "pt50";
	string und_pt_x = (underscore + pt_x).c_str(); 
	string pt_x_bar = (pt_x + barra).c_str(); 
	string und_pt_x_bar  = (underscore + pt_x + barra).c_str(); 

	string DeltaR_0e0 = "DeltaR_0e0";
	string und_DeltaR_0e0 = (underscore + DeltaR_0e0).c_str();
	string DeltaR_0e2 = "DeltaR_0e2";
	string und_DeltaR_0e2 = (underscore + DeltaR_0e2).c_str();
	string DeltaR_0e3 = "DeltaR_0e3";
	string und_DeltaR_0e3 = (underscore + DeltaR_0e3).c_str();
	string DeltaR_0e4 = "DeltaR_0e4";
	string und_DeltaR_0e4 = (underscore + DeltaR_0e4).c_str();
	string DeltaR_0e5 = "DeltaR_0e5";
	string und_DeltaR_0e5 = (underscore + DeltaR_0e5).c_str();

	string Address_sample = "DYtoLL_All_Fall11";
//	string Address_sample = "DYtoLL_All_Summer11";
//	string Address_sample = "DYtoMuMu_All_Summer11";
	string Address_sample_und = (Address_sample + underscore).c_str();
	string Address_sample_bar = (Address_sample + barra).c_str(); 

//	string Address_Vertex = "Vertex_Less8/";
//	string Address_Vertex = "Vertex_Great8/";
	string Address_Vertex = "Vertex_NoRequirement/";

	string histo_NUM_Vertex;
	string histo_DEN_Vertex;
	if (Address_Vertex == "Vertex_Less8/"){
		histo_NUM_Vertex = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Less8/numCounterTrue_VertWindA"; 
		histo_DEN_Vertex = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Less8/denCounterTrue_VertWindA"; 
	}
	else if (Address_Vertex == "Vertex_Great8/"){
		histo_NUM_Vertex = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Great8/numCounterTrue_VertWindB";
		histo_DEN_Vertex = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_Great8/denCounterTrue_VertWindB";
	}
	else if (Address_Vertex == "Vertex_NoRequirement/"){
		histo_NUM_Vertex = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_No/numCounterTrue";
		histo_DEN_Vertex = "DeltaRAnalyzerMuon/DoubleMu_Overlap/VerticesCut_No/denCounterTrue";
	}
	else cout << "che sia il caso di scrivere perbenino?" << endl;

//	string Address_IsoRho = "IsoRho_NO/"; 
	string Address_IsoRho = "IsoRho_SI/";
	if (Address_IsoRho == "IsoRho_NO/"){
		string iso = "_iso"; 
		histo_NUM_Vertex += iso;
	}


	//file Input
	string pre_Address_in = (Address_radice + pt_x_bar + Address_sample_bar).c_str();
	string post_Address_in = "genJets/genJetsTrue/";

	string file_cartella_1 = (Address_sample_und + DeltaR_0e0 + und_pt_x_bar).c_str();
	string file_cartella_2 = (Address_sample_und + DeltaR_0e2 + und_pt_x_bar).c_str();
	string file_cartella_3 = (Address_sample_und + DeltaR_0e3 + und_pt_x_bar).c_str();
	string file_cartella_4 = (Address_sample_und + DeltaR_0e4 + und_pt_x_bar).c_str();
	string file_cartella_5 = (Address_sample_und + DeltaR_0e5 + und_pt_x_bar).c_str();

	string Address_in_1 = (pre_Address_in + file_cartella_1 + Address_Vertex + Address_IsoRho + post_Address_in).c_str();
	string Address_in_2 = (pre_Address_in + file_cartella_2 + Address_Vertex + Address_IsoRho + post_Address_in).c_str();
	string Address_in_3 = (pre_Address_in + file_cartella_3 + Address_Vertex + Address_IsoRho + post_Address_in).c_str();
	string Address_in_4 = (pre_Address_in + file_cartella_4 + Address_Vertex + Address_IsoRho + post_Address_in).c_str();
	string Address_in_5 = (pre_Address_in + file_cartella_5 + Address_Vertex + Address_IsoRho + post_Address_in).c_str();

	string pre_file_in = "genJetsTrue";
	string file_in_1 = (pre_file_in + und_DeltaR_0e0 + root).c_str();
	string file_in_2 = (pre_file_in + und_DeltaR_0e2 + root).c_str();
	string file_in_3 = (pre_file_in + und_DeltaR_0e3 + root).c_str();
	string file_in_4 = (pre_file_in + und_DeltaR_0e4 + root).c_str();
	string file_in_5 = (pre_file_in + und_DeltaR_0e5 + root).c_str();

	//uno
	string percorso_1 = (Address_in_1 + file_in_1).c_str();
	TFile *file_1 = TFile::Open(percorso_1.c_str());
	//due
	string percorso_2 = (Address_in_2 + file_in_2).c_str();
	TFile *file_2 = TFile::Open(percorso_2.c_str());
	//tre
	string percorso_3 = (Address_in_3 + file_in_3).c_str();
	TFile *file_3 = TFile::Open(percorso_3.c_str());
	//quattro
	string percorso_4 = (Address_in_4 + file_in_4).c_str();
	TFile *file_4 = TFile::Open(percorso_4.c_str());
	//cinque
	string percorso_5 = (Address_in_5 + file_in_5).c_str();
	TFile *file_5 = TFile::Open(percorso_5.c_str());

	//Output
	string out_cartella = "Grafici_Insieme/";
	string Address_totale_out = (Address_radice + pt_x_bar + Address_sample_bar + out_cartella).c_str();

	string pre_file_out = "AllTogetherTrue"; 
	string file_out = (pre_file_out + root).c_str();

	string outputroot = (Address_totale_out + file_out).c_str();
	TFile* outplots = new TFile(outputroot.c_str(), "RECREATE");


	//colori	
	int col_1 = 1;
	int col_2 = 2;
	int col_3 = 3;
	int col_4 = 4;
	int col_5 = 6;

	//SetRangeUser
	double nminX = 0.0; 
	double nmaxX = 5.0;
	double nminY = 0.0; 
	double nmaxY = 0.8;

	//Legenda
	string Leg_1 = "#DeltaR = 0.0";
	string Leg_2 = "#DeltaR = 0.2";
	string Leg_3 = "#DeltaR = 0.3";
	string Leg_4 = "#DeltaR = 0.4";
	string Leg_5 = "#DeltaR = 0.5";

	string Title_Excl = "Exclusive Jet Rate: Efficiency";
	string Title_Incl = "Inclusive Jet Rate: Efficiency";

        string Insieme = "AllTogetherTrue";
	string Excl = "_Exclusive";
	string Incl = "_Inclusive";

	string histo_string_Excl = "JetRate/Exclusive/histoDiv_NUM";
	//histo 1 Exclusive
	TH1D* histo_1_Excl = (TH1D*) file_1->Get(histo_string_Excl.c_str());
	//histo 2 Exclusive
	TH1D* histo_2_Excl = (TH1D*) file_2->Get(histo_string_Excl.c_str());
	//histo 3 Exclusive
	TH1D* histo_3_Excl = (TH1D*) file_3->Get(histo_string_Excl.c_str());
	//histo 4 Exclusive
	TH1D* histo_4_Excl = (TH1D*) file_4->Get(histo_string_Excl.c_str());
	//histo 5 Exclusive
	TH1D* histo_5_Excl = (TH1D*) file_5->Get(histo_string_Excl.c_str());


	TCanvas *Canva_Excl = new TCanvas();
        Canva_Excl->Range(0,0,25,25);
        Canva_Excl->SetLeftMargin(0.160);

	histo_1_Excl->SetLineColor(col_1);
	histo_1_Excl->SetLineWidth(2);
//	histo_1_Excl->SetTitle(Title_Excl.c_str());
	histo_1_Excl->SetTitle(0);
	histo_1_Excl->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histo_1_Excl->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histo_1_Excl->GetXaxis()->SetTitle("Number of gen Jets");
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


	TLegend *Leg_Excl = new TLegend(0.30,0.63,0.88,0.88);
	Leg_Excl->SetFillColor(0);
	Leg_Excl->SetBorderSize(0);
	Leg_Excl->SetNColumns(3);
	Leg_Excl->AddEntry(histo_1_Excl, Leg_1.c_str());
   	Leg_Excl->AddEntry(histo_2_Excl, Leg_2.c_str());
   	Leg_Excl->AddEntry(histo_3_Excl, Leg_3.c_str());
   	Leg_Excl->AddEntry(histo_4_Excl, Leg_4.c_str());
   	Leg_Excl->AddEntry(histo_5_Excl, Leg_5.c_str());
   	Leg_Excl->Draw("LP");

//	TPaveText *Tex_Excl = new TPaveText(-0.01,0.05,1.96,0.11);
	TPaveText *Tex_Excl = new TPaveText(0.2118,0.129091,0.445735,0.193091,"NDC");
	Tex_Excl->SetFillColor(0);
	Tex_Excl->SetBorderSize(0);
	Tex_Excl->AddText("#splitline{CMS Simulation}{#sqrt{s} = 7 TeV}");
   	Tex_Excl->Draw();

	Canva_Excl->Write((Insieme + Excl).c_str());
        Canva_Excl->SaveAs((Address_totale_out + Insieme + Excl + pdf).c_str());
	Canva_Excl->Close();


	string histo_string_Incl = "JetRate/Inclusive/histoDiv_NUM";
	//histo 1 Inclusive
	TH1D* histo_1_Incl = (TH1D*) file_1->Get(histo_string_Incl.c_str());
	//histo 2 Inclusive
	TH1D* histo_2_Incl = (TH1D*) file_2->Get(histo_string_Incl.c_str());
	//histo 3 Inclusive
	TH1D* histo_3_Incl = (TH1D*) file_3->Get(histo_string_Incl.c_str());
	//histo 4 Inclusive
	TH1D* histo_4_Incl = (TH1D*) file_4->Get(histo_string_Incl.c_str());
	//histo 5 Inclusive
	TH1D* histo_5_Incl = (TH1D*) file_5->Get(histo_string_Incl.c_str());


	TCanvas *Canva_Incl = new TCanvas();
        Canva_Incl->Range(0,0,25,25);
        Canva_Incl->SetLeftMargin(0.160);

	histo_1_Incl->SetLineColor(col_1);
	histo_1_Incl->SetLineWidth(2);
//	histo_1_Incl->SetTitle(Title_Incl.c_str());
	histo_1_Excl->SetTitle(0);
	histo_1_Incl->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histo_1_Incl->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histo_1_Incl->GetXaxis()->SetTitle("Number of gen Jets");
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


	TLegend *Leg_Incl = new TLegend(0.30,0.63,0.88,0.88);
	Leg_Incl->SetFillColor(0);
	Leg_Incl->SetBorderSize(0);
	Leg_Incl->SetNColumns(3);
	Leg_Incl->AddEntry(histo_1_Incl, Leg_1.c_str());
   	Leg_Incl->AddEntry(histo_2_Incl, Leg_2.c_str());
   	Leg_Incl->AddEntry(histo_3_Incl, Leg_3.c_str());
   	Leg_Incl->AddEntry(histo_4_Incl, Leg_4.c_str());
   	Leg_Incl->AddEntry(histo_5_Incl, Leg_5.c_str());
   	Leg_Incl->Draw("LP");

//	TPaveText *Tex_Incl = new TPaveText(-0.01,0.05,1.96,0.11);
	TPaveText *Tex_Incl = new TPaveText(0.2118,0.129091,0.445735,0.193091,"NDC");
	Tex_Incl->SetFillColor(0);
	Tex_Incl->SetBorderSize(0);
	Tex_Incl->AddText("#splitline{CMS Simulation}{#sqrt{s} = 7 TeV}");
   	Tex_Incl->Draw();

	Canva_Incl->Write((Insieme + Incl).c_str());
        Canva_Incl->SaveAs((Address_totale_out + Insieme + Incl + pdf).c_str());
	Canva_Incl->Close();
	
  outplots->cd();
  outplots->Write();
  outplots->Close();
}

