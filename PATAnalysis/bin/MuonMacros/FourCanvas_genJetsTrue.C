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
//               Qui vengono scritti i grafici a partire da quelli già prodotti dalla macro Divide_*.C:                  
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
#include "TLatex.h"

//#include "tdrstyle.C"

using namespace std;

void FourCanvas_genJetsTrue(){
        
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
	float pt = 50;

	string DeltaR_0ex = "DeltaR_0e0";
	string und_DeltaR_0ex = (underscore + DeltaR_0ex).c_str();
	float deltaR = 0.0;

	string Address_sample1 = "DYtoLL_All_Fall11";
	string Address_sample1_und = (Address_sample1 + underscore).c_str();
	string Address_sample1_bar = (Address_sample1 + barra).c_str(); 
	string Address_sample2 = "DYtoLL_All_Summer11";
	string Address_sample2_und = (Address_sample2 + underscore).c_str();
	string Address_sample2_bar = (Address_sample2 + barra).c_str(); 
	string Address_sample3 = "DYtoMuMu_All_Summer11";
	string Address_sample3_und = (Address_sample3 + underscore).c_str();
	string Address_sample3_bar = (Address_sample3 + barra).c_str(); 

	string cartella1 = (Address_sample1_und + DeltaR_0ex + und_pt_x_bar).c_str();
	string cartella2 = (Address_sample2_und + DeltaR_0ex + und_pt_x_bar).c_str();
	string cartella3 = (Address_sample3_und + DeltaR_0ex + und_pt_x_bar).c_str();

	string pre_Address1 = (Address_radice + pt_x_bar + Address_sample1_bar + cartella1).c_str();
	string pre_Address2 = (Address_radice + pt_x_bar + Address_sample2_bar + cartella2).c_str();
	string pre_Address3 = (Address_radice + pt_x_bar + Address_sample3_bar + cartella3).c_str();

	string Address_Vertex1 = "Vertex_Less8/"; 
	string Address_Vertex2 = "Vertex_Great8/"; 
	string Address_Vertex3 = "Vertex_NoRequirement/"; 

	string Address_IsoRho1 = "IsoRho_NO/"; 
	string Address_IsoRho2 = "IsoRho_SI/"; 

	string percorso_post = "genJets/genJetsTrue/";
	string file_pre = "genJetsTrue_";
	string file = (file_pre + DeltaR_0ex + root).c_str();

	//VERTEX 1
	//MAD Fall11 Iso rho no
	TFile *file_Ver1_1a = TFile::Open((pre_Address1 + Address_Vertex1 + Address_IsoRho1 + percorso_post + file).c_str());
	//MAD Fall11 Iso rho si
	TFile *file_Ver1_1b = TFile::Open((pre_Address1 + Address_Vertex1 + Address_IsoRho2 + percorso_post + file).c_str());
	//MAD Summer11 Iso rho no
	TFile *file_Ver1_2a = TFile::Open((pre_Address2 + Address_Vertex1 + Address_IsoRho1 + percorso_post + file).c_str());
	//MAD Summer11 Iso rho si
	TFile *file_Ver1_2b = TFile::Open((pre_Address2 + Address_Vertex1 + Address_IsoRho2 + percorso_post + file).c_str());
	//PYT Summer11 Iso rho no
	TFile *file_Ver1_3a = TFile::Open((pre_Address3 + Address_Vertex1 + Address_IsoRho1 + percorso_post + file).c_str());
	//PYT Summer11 Iso rho si
	TFile *file_Ver1_3b = TFile::Open((pre_Address3 + Address_Vertex1 + Address_IsoRho2 + percorso_post + file).c_str());

	//VERTEX 2
	//MAD Fall11 Iso rho no
	TFile *file_Ver2_1a = TFile::Open((pre_Address1 + Address_Vertex2 + Address_IsoRho1 + percorso_post + file).c_str());
	//MAD Fall11 Iso rho si
	TFile *file_Ver2_1b = TFile::Open((pre_Address1 + Address_Vertex2 + Address_IsoRho2 + percorso_post + file).c_str());
	//MAD Summer11 Iso rho no
	TFile *file_Ver2_2a = TFile::Open((pre_Address2 + Address_Vertex2 + Address_IsoRho1 + percorso_post + file).c_str());
	//MAD Summer11 Iso rho si
	TFile *file_Ver2_2b = TFile::Open((pre_Address2 + Address_Vertex2 + Address_IsoRho2 + percorso_post + file).c_str());
	//PYT Summer11 Iso rho no
	TFile *file_Ver2_3a = TFile::Open((pre_Address3 + Address_Vertex2 + Address_IsoRho1 + percorso_post + file).c_str());
	//PYT Summer11 Iso rho si
	TFile *file_Ver2_3b = TFile::Open((pre_Address3 + Address_Vertex2 + Address_IsoRho2 + percorso_post + file).c_str());

	//VERTEX 3
	//MAD Fall11 Iso rho no
	TFile *file_Ver3_1a = TFile::Open((pre_Address1 + Address_Vertex3 + Address_IsoRho1 + percorso_post + file).c_str());
	//MAD Fall11 Iso rho si
	TFile *file_Ver3_1b = TFile::Open((pre_Address1 + Address_Vertex3 + Address_IsoRho2 + percorso_post + file).c_str());
	//MAD Summer11 Iso rho no
	TFile *file_Ver3_2a = TFile::Open((pre_Address2 + Address_Vertex3 + Address_IsoRho1 + percorso_post + file).c_str());
	//MAD Summer11 Iso rho si
	TFile *file_Ver3_2b = TFile::Open((pre_Address2 + Address_Vertex3 + Address_IsoRho2 + percorso_post + file).c_str());
	//PYT Summer11 Iso rho no
	TFile *file_Ver3_3a = TFile::Open((pre_Address3 + Address_Vertex3 + Address_IsoRho1 + percorso_post + file).c_str());
	//PYT Summer11 Iso rho si
	TFile *file_Ver3_3b = TFile::Open((pre_Address3 + Address_Vertex3 + Address_IsoRho2 + percorso_post + file).c_str());

	//Output
	string out_cartella = "Grafici_Insieme/";
	string Address_totale_out = (Address_radice + out_cartella).c_str();
             
	string pre_file_out = "genJetsTrue_";        
	string file_out = (pre_file_out + DeltaR_0ex + und_pt_x ).c_str();
	string outputroot = (Address_totale_out + file_out + root).c_str();

	TFile* outplots = new TFile(outputroot.c_str(), "RECREATE");	

	//colori	
	int col_1a = 1;
	int col_1b = 1;
	int col_2a = 2;
	int col_2b = 2;
	int col_3a = 3;
	int col_3b = 3;

	//SetRangeUser
	double nminX = 0.0; 
	double nmaxX = 4.0;
	double nminY = 0.0; 
	double nmaxY = 1.05;

	//Legenda
	string Leg_1a = "#splitline{MAD F11}{Iso}";
	string Leg_1b = "#splitline{MAD F11}{Iso rho}";
	string Leg_2a = "#splitline{MAD S11}{Iso}";
	string Leg_2b = "#splitline{MAD S11}{Iso rho}";
	string Leg_3a = "#splitline{PYT S11}{Iso}";
	string Leg_3b = "#splitline{PYT S11}{Iso rho}";

	string Title_c1 = "n vertices #leq 8";
	string Title_c2 = "n vertices > 8";
	string Title_c3 = "n vertices no requirement";


	//VERTEX 1
	string histo_string_Incl = "JetRate/Inclusive/histoDiv_NUM";
	// histo MAD Fall11 Iso rho no  
	TH1D* histo_Ver1_1a = (TH1D*) file_Ver1_1a->Get(histo_string_Incl.c_str());
	// histo MAD Fall11 Iso rho si  
	TH1D* histo_Ver1_1b = (TH1D*) file_Ver1_1b->Get(histo_string_Incl.c_str());
	// histo MAD Summer11 Iso rho no  
	TH1D* histo_Ver1_2a = (TH1D*) file_Ver1_2a->Get(histo_string_Incl.c_str());
	// histo MAD Summer11 Iso rho si  
	TH1D* histo_Ver1_2b = (TH1D*) file_Ver1_2b->Get(histo_string_Incl.c_str());
	// histo PYT Summer11 Iso rho no  
	TH1D* histo_Ver1_3a = (TH1D*) file_Ver1_3a->Get(histo_string_Incl.c_str());
	// histo PYT Summer11 Iso rho si    
	TH1D* histo_Ver1_3b = (TH1D*) file_Ver1_3b->Get(histo_string_Incl.c_str());

	//VERTEX 2
	// histo MAD Fall11 Iso rho no  
	TH1D* histo_Ver2_1a = (TH1D*) file_Ver2_1a->Get(histo_string_Incl.c_str());
	// histo MAD Fall11 Iso rho si  
	TH1D* histo_Ver2_1b = (TH1D*) file_Ver2_1b->Get(histo_string_Incl.c_str());
	// histo MAD Summer11 Iso rho no  
	TH1D* histo_Ver2_2a = (TH1D*) file_Ver2_2a->Get(histo_string_Incl.c_str());
	// histo MAD Summer11 Iso rho si  
	TH1D* histo_Ver2_2b = (TH1D*) file_Ver2_2b->Get(histo_string_Incl.c_str());
	// histo PYT Summer11 Iso rho no  
	TH1D* histo_Ver2_3a = (TH1D*) file_Ver2_3a->Get(histo_string_Incl.c_str());
	// histo PYT Summer11 Iso rho si    
	TH1D* histo_Ver2_3b = (TH1D*) file_Ver2_3b->Get(histo_string_Incl.c_str());

	//VERTEX 3
	// histo MAD Fall11 Iso rho no  
	TH1D* histo_Ver3_1a = (TH1D*) file_Ver3_1a->Get(histo_string_Incl.c_str());
	// histo MAD Fall11 Iso rho si  
	TH1D* histo_Ver3_1b = (TH1D*) file_Ver3_1b->Get(histo_string_Incl.c_str());
	// histo MAD Summer11 Iso rho no  
	TH1D* histo_Ver3_2a = (TH1D*) file_Ver3_2a->Get(histo_string_Incl.c_str());
	// histo MAD Summer11 Iso rho si  
	TH1D* histo_Ver3_2b = (TH1D*) file_Ver3_2b->Get(histo_string_Incl.c_str());
	// histo PYT Summer11 Iso rho no  
	TH1D* histo_Ver3_3a = (TH1D*) file_Ver3_3a->Get(histo_string_Incl.c_str());
	// histo PYT Summer11 Iso rho si    
	TH1D* histo_Ver3_3b = (TH1D*) file_Ver3_3b->Get(histo_string_Incl.c_str());


	TCanvas *Canva_c1 = new TCanvas();
//        Canva_Incl->Range(0,0,25,25);
//        Canva_c1->SetLeftMargin(0.160);
        Canva_c1->Divide(2,2);

        Canva_c1->cd(1);
	histo_Ver1_1a->SetTitle(Title_c1.c_str());
	histo_Ver1_1a->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histo_Ver1_1a->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histo_Ver1_1a->GetXaxis()->SetTitle("Number of gen Jets");
	histo_Ver1_1a->GetYaxis()->SetTitle("Ratio");
        histo_Ver1_1a->GetYaxis()->SetTitleOffset(1.3);
	histo_Ver1_1a->SetLineColor(col_1a);
	histo_Ver1_1a->SetLineWidth(2);
	histo_Ver1_1a->SetLineStyle(2);
	histo_Ver1_1a->SetMarkerStyle(24);
	histo_Ver1_1a->SetMarkerColor(col_1a);
	histo_Ver1_1a->Draw("histo PE");	

	histo_Ver1_1b->SetLineColor(col_1b);
	histo_Ver1_1b->SetLineWidth(2);
	histo_Ver1_1b->SetLineStyle(1);
        histo_Ver1_1b->SetMarkerStyle(20);
        histo_Ver1_1b->SetMarkerColor(col_1b);
	histo_Ver1_1b->Draw("histo PE same");

	histo_Ver1_2a->SetLineColor(col_2a);
	histo_Ver1_2a->SetLineWidth(2);
	histo_Ver1_2a->SetLineStyle(2);
        histo_Ver1_2a->SetMarkerStyle(25);
        histo_Ver1_2a->SetMarkerColor(col_2a);
	histo_Ver1_2a->Draw("histo PE same");

	histo_Ver1_2b->SetLineColor(col_2b);
	histo_Ver1_2b->SetLineWidth(2);
	histo_Ver1_2b->SetLineStyle(1);
        histo_Ver1_2b->SetMarkerStyle(21);
        histo_Ver1_2b->SetMarkerColor(col_2b);
	histo_Ver1_2b->Draw("histo PE same");

	histo_Ver1_3a->SetLineColor(col_3a);
	histo_Ver1_3a->SetLineWidth(2);
	histo_Ver1_3a->SetLineStyle(2);
        histo_Ver1_3a->SetMarkerStyle(26);
        histo_Ver1_3a->SetMarkerColor(col_3a);
	histo_Ver1_3a->Draw("histo PE same");

	histo_Ver1_3b->SetLineColor(col_3b);
	histo_Ver1_3b->SetLineWidth(2);
	histo_Ver1_3b->SetLineStyle(1);
        histo_Ver1_3b->SetMarkerStyle(22);
        histo_Ver1_3b->SetMarkerColor(col_3b);
	histo_Ver1_3b->Draw("histo PE same");

/*
	TLegend *Leg_c1 = new TLegend(0.160,0.65,0.65,0.89);
	Leg_c1->SetFillColor(0);
	Leg_c1->SetBorderSize(0);
	Leg_c1->SetNColumns(3);
	Leg_c1->AddEntry(histo_Ver1_1a, Leg_1a.c_str());
   	Leg_c1->AddEntry(histo_Ver1_2a, Leg_2a.c_str());
   	Leg_c1->AddEntry(histo_Ver1_3a, Leg_3a.c_str());
   	Leg_c1->AddEntry(histo_Ver1_1b, Leg_1b.c_str());
   	Leg_c1->AddEntry(histo_Ver1_2b, Leg_2b.c_str());
   	Leg_c1->AddEntry(histo_Ver1_3b, Leg_3b.c_str());
   	Leg_c1->Draw("LP");
*/
//

        Canva_c1->cd(2);
	histo_Ver2_1a->SetTitle(Title_c2.c_str());
	histo_Ver2_1a->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histo_Ver2_1a->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histo_Ver2_1a->GetXaxis()->SetTitle("Number of gen Jets");
	histo_Ver2_1a->GetYaxis()->SetTitle("Ratio");
        histo_Ver2_1a->GetYaxis()->SetTitleOffset(1.3);
	histo_Ver2_1a->SetLineColor(col_1a);
	histo_Ver2_1a->SetLineWidth(2);
	histo_Ver2_1a->SetLineStyle(2);
	histo_Ver2_1a->SetMarkerStyle(24);
	histo_Ver2_1a->SetMarkerColor(col_1a);
	histo_Ver2_1a->Draw("histo PE");	

	histo_Ver2_1b->SetLineColor(col_1b);
	histo_Ver2_1b->SetLineWidth(2);
	histo_Ver2_1b->SetLineStyle(1);
        histo_Ver2_1b->SetMarkerStyle(20);
        histo_Ver2_1b->SetMarkerColor(col_1b);
	histo_Ver2_1b->Draw("histo PE same");

	histo_Ver2_2a->SetLineColor(col_2a);
	histo_Ver2_2a->SetLineWidth(2);
	histo_Ver2_2a->SetLineStyle(2);
        histo_Ver2_2a->SetMarkerStyle(25);
        histo_Ver2_2a->SetMarkerColor(col_2a);
	histo_Ver2_2a->Draw("histo PE same");

	histo_Ver2_2b->SetLineColor(col_2b);
	histo_Ver2_2b->SetLineWidth(2);
	histo_Ver2_2b->SetLineStyle(1);
        histo_Ver2_2b->SetMarkerStyle(21);
        histo_Ver2_2b->SetMarkerColor(col_2b);
	histo_Ver2_2b->Draw("histo PE same");

	histo_Ver2_3a->SetLineColor(col_3a);
	histo_Ver2_3a->SetLineWidth(2);
	histo_Ver2_3a->SetLineStyle(2);
        histo_Ver2_3a->SetMarkerStyle(26);
        histo_Ver2_3a->SetMarkerColor(col_3a);
	histo_Ver2_3a->Draw("histo PE same");

	histo_Ver2_3b->SetLineColor(col_3b);
	histo_Ver2_3b->SetLineWidth(2);
	histo_Ver2_3b->SetLineStyle(1);
        histo_Ver2_3b->SetMarkerStyle(22);
        histo_Ver2_3b->SetMarkerColor(col_3b);
	histo_Ver2_3b->Draw("histo PE same");

/*
	TLegend *Leg_c2 = new TLegend(0.160,0.65,0.65,0.89);
	Leg_c2->SetFillColor(0);
	Leg_c2->SetBorderSize(0);
	Leg_c2->SetNColumns(3);
	Leg_c2->AddEntry(histo_Ver2_1a, Leg_1a.c_str());
   	Leg_c2->AddEntry(histo_Ver2_2a, Leg_2a.c_str());
   	Leg_c2->AddEntry(histo_Ver2_3a, Leg_3a.c_str());
   	Leg_c2->AddEntry(histo_Ver2_1b, Leg_1b.c_str());
   	Leg_c2->AddEntry(histo_Ver2_2b, Leg_2b.c_str());
   	Leg_c2->AddEntry(histo_Ver2_3b, Leg_3b.c_str());
   	Leg_c2->Draw("LP");
*/

//

        Canva_c1->cd(3);
	histo_Ver3_1a->SetTitle(Title_c3.c_str());
	histo_Ver3_1a->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histo_Ver3_1a->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histo_Ver3_1a->GetXaxis()->SetTitle("Number of gen Jets");
	histo_Ver3_1a->GetYaxis()->SetTitle("Ratio");
        histo_Ver3_1a->GetYaxis()->SetTitleOffset(1.3);
	histo_Ver3_1a->SetLineColor(col_1a);
	histo_Ver3_1a->SetLineWidth(2);
	histo_Ver3_1a->SetLineStyle(2);
	histo_Ver3_1a->SetMarkerStyle(24);
	histo_Ver3_1a->SetMarkerColor(col_1a);
	histo_Ver3_1a->Draw("histo PE");	

	histo_Ver3_1b->SetLineColor(col_1b);
	histo_Ver3_1b->SetLineWidth(2);
	histo_Ver3_1b->SetLineStyle(1);
        histo_Ver3_1b->SetMarkerStyle(20);
        histo_Ver3_1b->SetMarkerColor(col_1b);
	histo_Ver3_1b->Draw("histo PE same");

	histo_Ver3_2a->SetLineColor(col_2a);
	histo_Ver3_2a->SetLineWidth(2);
	histo_Ver3_2a->SetLineStyle(2);
        histo_Ver3_2a->SetMarkerStyle(25);
        histo_Ver3_2a->SetMarkerColor(col_2a);
	histo_Ver3_2a->Draw("histo PE same");

	histo_Ver3_2b->SetLineColor(col_2b);
	histo_Ver3_2b->SetLineWidth(2);
	histo_Ver3_2b->SetLineStyle(1);
        histo_Ver3_2b->SetMarkerStyle(21);
        histo_Ver3_2b->SetMarkerColor(col_2b);
	histo_Ver3_2b->Draw("histo PE same");

	histo_Ver3_3a->SetLineColor(col_3a);
	histo_Ver3_3a->SetLineWidth(2);
	histo_Ver3_3a->SetLineStyle(2);
        histo_Ver3_3a->SetMarkerStyle(26);
        histo_Ver3_3a->SetMarkerColor(col_3a);
	histo_Ver3_3a->Draw("histo PE same");

	histo_Ver3_3b->SetLineColor(col_3b);
	histo_Ver3_3b->SetLineWidth(2);
	histo_Ver3_3b->SetLineStyle(1);
        histo_Ver3_3b->SetMarkerStyle(22);
        histo_Ver3_3b->SetMarkerColor(col_3b);
	histo_Ver3_3b->Draw("histo PE same");

/*
	TLegend *Leg_c3 = new TLegend(0.160,0.65,0.65,0.89);
	Leg_c3->SetFillColor(0);
	Leg_c3->SetBorderSize(0);
	Leg_c3->SetNColumns(3);
	Leg_c3->AddEntry(histo_Ver3_1a, Leg_1a.c_str());
   	Leg_c3->AddEntry(histo_Ver3_2a, Leg_2a.c_str());
   	Leg_c3->AddEntry(histo_Ver3_3a, Leg_3a.c_str());
   	Leg_c3->AddEntry(histo_Ver3_1b, Leg_1b.c_str());
   	Leg_c3->AddEntry(histo_Ver3_2b, Leg_2b.c_str());
   	Leg_c3->AddEntry(histo_Ver3_3b, Leg_3b.c_str());
   	Leg_c3->Draw("LP");
*/
//
        Canva_c1->cd(4);

	TLegend *Leg_c4 = new TLegend(0.160,0.65,0.8,0.89);
	Leg_c4->SetFillColor(0);
//	Leg_c4->SetBorderSize(0);
	Leg_c4->SetNColumns(3);
	Leg_c4->AddEntry(histo_Ver1_1a, Leg_1a.c_str());
   	Leg_c4->AddEntry(histo_Ver1_2a, Leg_2a.c_str());
   	Leg_c4->AddEntry(histo_Ver1_3a, Leg_3a.c_str());
   	Leg_c4->AddEntry(histo_Ver1_1b, Leg_1b.c_str());
   	Leg_c4->AddEntry(histo_Ver1_2b, Leg_2b.c_str());
   	Leg_c4->AddEntry(histo_Ver1_3b, Leg_3b.c_str());
   	Leg_c4->Draw("LP");

	TPaveText *Tex_c4 = new TPaveText(0.160,0.129091,0.7,0.3,"NDC");
	Tex_c4->SetFillColor(0);
	Tex_c4->SetBorderSize(0);
	Tex_c4->AddText(Form("#Delta R = %.2f", deltaR));
	Tex_c4->AddText(Form("p_{T} = %.2f GeV/c", pt));
   	Tex_c4->Draw();

//	TLatex *Latex = new TLatex();
//	Latex->DrawLatex(.2,.2,"#splitline{#DeltaR = 0.0}{p_{T} = 50 GeV/c}");

	string combo = "combo";
	Canva_c1->Write(combo.c_str());
        Canva_c1->SaveAs((Address_totale_out + file_out + pdf).c_str());
	Canva_c1->Close();


	
  outplots->cd();
  outplots->Write();
  outplots->Close();
}

