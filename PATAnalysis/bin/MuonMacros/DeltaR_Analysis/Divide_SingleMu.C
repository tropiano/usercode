//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Macro strutturata per una disposizione di cartelle simile:
//
//
//            Da qui si lancia la macro con root: 
//            --> macro/
//               
//               
//               
//               
//               
//               
//               
//               
//
// grafici/ --  
//               
//                                                                                                                                                                                  --> genJetsFalse/
//                                                                                                                                                                 
//                                                                                                                                                                  --> genJets/ --
//                
//                                                                                                                                                                                  --> genJetsTrue/
//                                                                                                                                                                  --> recJets/
//                
//             Qui vengono scritti i grafici:                  
//             --> pt_x/       -->  Address_sample/         -->  file_cartella/                           -->  Address_Vertex/             -->  Address_IsoRho/   -- 
//                 (es: pt_50/)     (es: DYtoLL_All_Fall11/)     (es: DYtoLL_All_Fall11_DeltaR_0e0_pt50/)      (es: Vertex_NoRequirement/)      (es: IsoRho_SI/)   
//                
//                                                                                                                                                                  --> SingleMu/ --> DeltaR_0ex/
//                                                                                                                                                                                    (es: DeltaR_0e0/)
//                
//                                                                                                                                                                  --> LeadSecMu/ --> DeltaR_0ex/
//                                                                                                                                                                                    (es: DeltaR_0e0/)
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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


void Divide_SingleMu_AN(){
        
       
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
 	string duepunti = ": ";
 	string spazio = " ";
 	string jets = "jets";

	string Address_radice = "~/Scrivania/html/AnalysisMuons/grafici_AN/";

	string pt_x = "pt50";
	string und_pt_x = (underscore + pt_x).c_str(); 
	string pt_x_bar = (pt_x + barra).c_str(); 
	string und_pt_x_bar  = (underscore + pt_x + barra).c_str(); 

	string DeltaR_0ex = "DeltaR_0e4";
	string DeltaR_0ex_bar = (DeltaR_0ex + barra).c_str();
	string DeltaR_0ex_und = (DeltaR_0ex + underscore).c_str();
	string und_DeltaR_0ex = (underscore + DeltaR_0ex).c_str();
	float deltaR = 0.4;

	string Address_sample = "DYtoLL_All_Fall11";
//	string Address_sample = "DYtoLL_All_Summer11";
//	string Address_sample = "DYtoMuMu_All_Summer11";
	string Address_sample_und = (Address_sample + underscore).c_str();
	string Address_sample_bar = (Address_sample + barra).c_str(); 

//	string Address_Vertex = "Vertex_Less8/";
//	string Address_Vertex = "Vertex_Great8/";
	string Address_Vertex = "Vertex_NoRequirement/";

	string partial_histo_NUM_pt;
	string partial_histo_DEN_pt;
	string partial_histo_NUM_eta;
	string partial_histo_DEN_eta;
	if (Address_Vertex == "Vertex_Less8/"){
		partial_histo_NUM_pt = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_Less8/genMuPtinDeltaRMCMatch_VertWindA_"; 
		partial_histo_DEN_pt = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_Less8/genMuPtinDeltaR_VertWindA_";
		partial_histo_NUM_eta = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_Less8/genMuEtainDeltaRMCMatch_VertWindA_"; 
		partial_histo_DEN_eta = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_Less8/genMuEtainDeltaR_VertWindA_";
	}
	else if (Address_Vertex == "Vertex_Great8/"){
		partial_histo_NUM_pt = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_Great8/genMuPtinDeltaRMCMatch_VertWindB_"; 
		partial_histo_DEN_pt = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_Great8/genMuPtinDeltaR_VertWindB_";
		partial_histo_NUM_eta = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_Great8/genMuEtainDeltaRMCMatch_VertWindB_"; 
		partial_histo_DEN_eta = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_Great8/genMuEtainDeltaR_VertWindB_";
	}
	else if (Address_Vertex == "Vertex_NoRequirement/"){
		partial_histo_NUM_pt = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_No/genMuPtinDeltaRMCMatch_"; 
		partial_histo_DEN_pt = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_No/genMuPtinDeltaR_";
		partial_histo_NUM_eta = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_No/genMuEtainDeltaRMCMatch_"; 
		partial_histo_DEN_eta = "DeltaRAnalyzerMuon/SingleMu_Overlap/VerticesCut_No/genMuEtainDeltaR_";
	}
	else cout << "che sia il caso di scrivere perbenino?" << endl;

//	string Address_IsoRho = "IsoRho_NO/"; 
	string Address_IsoRho = "IsoRho_SI/";
	if (Address_IsoRho == "IsoRho_NO/"){
		string iso = "iso_"; 
		partial_histo_NUM_pt += iso;
		partial_histo_NUM_eta += iso;
	}

	//file Input
	string file_cartella = (Address_sample_und + DeltaR_0ex + und_pt_x_bar).c_str();
	string Address_totale_in = (Address_radice + pt_x_bar + Address_sample_bar + file_cartella).c_str();
	string file_in = (Address_sample_und + DeltaR_0ex + und_pt_x + root).c_str();

	TFile *file = TFile::Open((Address_totale_in + file_in).c_str());

	//colori	
	int col_NUM = 4;
	int col_DEN = 2;
	int col_DIV = 1;

	//SetRangeUser
	double nminX_pt = 0.0; 
	double nmaxX_pt = 200.0;
	double nminY_pt = 1; 
	double nmaxY_pt = 10000000.0;
	//double nminYDiv_pt = 0.0; 
	//double nmaxYDiv_pt = 1.0;

	double nminX_eta = -3.0; 
	double nmaxX_eta = 3.0;
	double nminY_eta = 0.1; 
	double nmaxY_eta = 100000000.0;
	//double nminYDiv_eta = 0.0; 
	//double nmaxYDiv_eta = 1.0;

	//Legenda
	string Leg_NUM_pt = "#splitline{1 #mu overlap = false}{with MC match}";
	string Leg_DEN_pt = "1 #mu overlap = false";

	string Leg_NUM_eta = "#splitline{1 #mu overlap = false}{with MC match}";
	string Leg_DEN_eta = "1 #mu overlap = false";


	// moltitudine di grafici
  	for (unsigned i = 0; i < 7; ++i){
	  	ostringstream oss;
  		oss << i;
 		string n = oss.str();

 		string njets = (duepunti + n + spazio + jets).c_str();
 		string njets_out = (n + jets).c_str();     

        
     		//file Output
		string pre_cartella_out = "SingleMu/";
		string cartella_out = (pre_cartella_out + DeltaR_0ex_bar).c_str();
		string Address_totale_out = (Address_totale_in + Address_Vertex + Address_IsoRho + cartella_out).c_str();

		string pre_file_out = "Overlap_SingleMu_";
		string file_out = (pre_file_out + DeltaR_0ex_und + n + jets + root).c_str();

		string outputroot = (Address_totale_out + file_out).c_str();
		TFile* outplots = new TFile(outputroot.c_str(), "RECREATE");

//-----------------------------------

		TDirectory *pt = new TDirectory();
		pt = outplots->mkdir("pt");
		pt->cd();

		//histo NUM
		string histo_NUM_pt_string = (partial_histo_NUM_pt + njets_out).c_str();
		TH1D* histo_NUM_pt = (TH1D*) file->Get(histo_NUM_pt_string.c_str());
	        //histo DEN
		string histo_DEN_pt_string = (partial_histo_DEN_pt + njets_out).c_str();
		TH1D* histo_DEN_pt = (TH1D*) file->Get(histo_DEN_pt_string.c_str());

		string Title_pt = "Single #mu p_{T}";

		TCanvas *c_pt = new TCanvas();
		c_pt->SetLogy();	
	        c_pt->Range(0,0,25,25);
	        c_pt->SetLeftMargin(0.160);

		histo_NUM_pt->SetLineColor(col_NUM);
		histo_NUM_pt->SetLineWidth(2);
		//histo_NUM_pt->SetTitle((Title_pt + njets).c_str());
		histo_NUM_pt->SetTitle(0);
		histo_NUM_pt->Rebin(10);
		histo_NUM_pt->GetXaxis()->SetRangeUser(nminX_pt, nmaxX_pt);
		histo_NUM_pt->GetYaxis()->SetRangeUser(nminY_pt, nmaxY_pt);
		histo_NUM_pt->GetXaxis()->SetTitle("gen #mu p_{T}");
		histo_NUM_pt->GetYaxis()->SetTitle("Events");
		histo_NUM_pt->GetYaxis()->SetTitleOffset(1.5);
		histo_NUM_pt->Draw("hist");	
	
		histo_DEN_pt->SetLineColor(col_DEN);
		histo_DEN_pt->SetLineWidth(2);
		histo_DEN_pt->Rebin(10);
		histo_DEN_pt->Draw("hist same");
			
		TPaveText *Tex_pt = new TPaveText();
		Tex_pt->SetFillColor(0);
		Tex_pt->SetBorderSize(0);
		Tex_pt->AddText(Form("#Delta R = %.2f", deltaR));
	   	Tex_pt->Draw("TR");
		
		TLegend *Leg_pt = new TLegend(0.51,0.67,0.88,0.88);
		Leg_pt->SetFillColor(0);
		Leg_pt->SetBorderSize(0);
		Leg_pt->AddEntry(histo_NUM_pt,Leg_NUM_pt.c_str());
	   	Leg_pt->AddEntry(histo_DEN_pt,Leg_DEN_pt.c_str());
	   	Leg_pt->Draw("LP");
		
		string gen_pt = "SingleMu_pt";
		string gen_pt_und = (gen_pt + underscore).c_str();
		c_pt->Write(gen_pt.c_str());
	        c_pt->SaveAs((Address_totale_out + gen_pt_und + njets_out + und_DeltaR_0ex + pdf).c_str());
		c_pt->Close();


		TCanvas *c_Divide_pt = new TCanvas();
	        c_Divide_pt->Range(0,0,25,25);
	        c_Divide_pt->SetLeftMargin(0.160);
	
		string divideplot_pt = " Efficiency";
	
	        TH1D *histoDiv_NUM_pt = (TH1D*) histo_NUM_pt->Clone("histoDiv_NUM_pt");
		histoDiv_NUM_pt->Sumw2();
	
	        TH1D *histoDiv_DEN_pt = (TH1D*) histo_DEN_pt->Clone("histoDiv_DEN_pt");
		histoDiv_DEN_pt->Sumw2();
	
		gStyle->SetOptStat(0);
		histoDiv_NUM_pt->SetLineColor(col_DIV);
		histoDiv_NUM_pt->SetLineWidth(2);
	//	histoDiv_NUM_pt->SetTitle((Title_pt + divideplot_pt + njets).c_str());
		histoDiv_NUM_pt->SetTitle(0);
		histoDiv_NUM_pt->GetXaxis()->SetRangeUser(nminX_pt, nmaxX_pt);
	//	histo_NUM_pt->GetYaxis()->SetRangeUser(nminYDiv_pt, nmaxYDiv_pt);
		histoDiv_NUM_pt->GetXaxis()->SetTitle("gen #mu p_{T}");
		histoDiv_NUM_pt->GetYaxis()->SetTitle("Ratio");
	        histoDiv_NUM_pt->GetYaxis()->SetTitleOffset(1.5);
	
		histoDiv_NUM_pt->Divide(histoDiv_NUM_pt,histoDiv_DEN_pt, 1, 1, "B");
		histoDiv_NUM_pt->Draw("E");	
	
		TPaveText *TexDiv_pt = new TPaveText();
		TexDiv_pt->SetFillColor(0);
		TexDiv_pt->SetBorderSize(0);
		TexDiv_pt->AddText(Form("#Delta R = %.2f", deltaR));
	   	TexDiv_pt->Draw("TR");
	
		string divide_pt = "Divide_pt";
		string divide_pt_und = (divide_pt + underscore).c_str();
	
		c_Divide_pt->Write(divide_pt.c_str());
	        c_Divide_pt->SaveAs((Address_totale_out + divide_pt_und + njets_out + und_DeltaR_0ex + pdf).c_str());
		c_Divide_pt->Close();
	
		pt->cd();

		//-----------------------------------
		TDirectory *eta = new TDirectory();
		eta = outplots->mkdir("eta");
		eta->cd();
		

	        //histo NUM
		string histo_NUM_eta_string = (partial_histo_NUM_eta + njets_out).c_str();
		TH1D* histo_NUM_eta = (TH1D*) file->Get(histo_NUM_eta_string.c_str());
	        //histo DEN
		string histo_DEN_eta_string = (partial_histo_DEN_eta + njets_out).c_str();
		TH1D* histo_DEN_eta = (TH1D*) file->Get(histo_DEN_eta_string.c_str());
	
		string Title_eta = "Single #mu #eta";
	
		TCanvas *c_eta = new TCanvas();
		c_eta->SetLogy();	
	        c_eta->Range(0,0,25,25);
	        c_eta->SetLeftMargin(0.160);
	
		histo_NUM_eta->SetLineColor(col_NUM);
		histo_NUM_eta->SetLineWidth(2);
		//histo_NUM_eta->SetTitle((Title_eta + njets).c_str());
		histo_NUM_eta->SetTitle(0);
		histo_NUM_eta->Rebin(2);
		histo_NUM_eta->GetXaxis()->SetRangeUser(nminX_eta, nmaxX_eta);
		histo_NUM_eta->GetYaxis()->SetRangeUser(nminY_eta, nmaxY_eta);
		histo_NUM_eta->GetXaxis()->SetTitle("gen #mu #eta");
		histo_NUM_eta->GetYaxis()->SetTitle("Events");
		histo_NUM_eta->GetYaxis()->SetTitleOffset(1.5);
		histo_NUM_eta->Draw("hist");	

		histo_DEN_eta->SetLineColor(col_DEN);
		histo_DEN_eta->SetLineWidth(2);
		histo_DEN_eta->Rebin(2);
		histo_DEN_eta->Draw("hist same");
			
		TPaveText *Tex_eta = new TPaveText();
		Tex_eta->SetFillColor(0);
		Tex_eta->SetBorderSize(0);
		Tex_eta->AddText(Form("#Delta R = %.2f", deltaR));
   		Tex_eta->Draw("TR");
		
		TLegend *Leg_eta = new TLegend(0.51,0.67,0.88,0.88);
		Leg_eta->SetFillColor(0);
		Leg_eta->SetBorderSize(0);
		Leg_eta->AddEntry(histo_NUM_eta,Leg_NUM_eta.c_str());
   		Leg_eta->AddEntry(histo_DEN_eta,Leg_DEN_eta.c_str());
   		Leg_eta->Draw("LP");
		
		string gen_eta = "SingleMu_eta";
		string gen_eta_und = (gen_eta + underscore).c_str();
		c_eta->Write(gen_eta.c_str());
        	c_eta->SaveAs((Address_totale_out + gen_eta_und + njets_out + und_DeltaR_0ex + pdf).c_str());
		c_eta->Close();



		TCanvas *c_Divide_eta = new TCanvas();
		c_Divide_eta->Range(0,0,25,25);
	        c_Divide_eta->SetLeftMargin(0.160);

		string divideplot_eta = " Efficiency";
	
	        TH1D *histoDiv_NUM_eta = (TH1D*) histo_NUM_eta->Clone("histoDiv_NUM_eta");
		histoDiv_NUM_eta->Sumw2();
	
	        TH1D *histoDiv_DEN_eta = (TH1D*) histo_DEN_eta->Clone("histoDiv_DEN_eta");
		histoDiv_DEN_eta->Sumw2();
	
		gStyle->SetOptStat(0);
		histoDiv_NUM_eta->SetLineColor(col_DIV);
		histoDiv_NUM_eta->SetLineWidth(2);
		//histoDiv_NUM_eta->SetTitle((Title_eta + divideplot_eta + njets).c_str());
		histoDiv_NUM_eta->SetTitle(0);
		histoDiv_NUM_eta->GetXaxis()->SetRangeUser(nminX_eta, nmaxX_eta);
		//histo_NUM_eta->GetYaxis()->SetRangeUser(nminYDiv_eta, nmaxYDiv_eta);
		histoDiv_NUM_eta->GetXaxis()->SetTitle("gen #mu #eta");
		histoDiv_NUM_eta->GetYaxis()->SetTitle("Ratio");
        	histoDiv_NUM_eta->GetYaxis()->SetTitleOffset(1.5);

		histoDiv_NUM_eta->Divide(histoDiv_NUM_eta,histoDiv_DEN_eta, 1, 1, "B");
		histoDiv_NUM_eta->Draw("E");	

		TPaveText *TexDiv_eta = new TPaveText();
		TexDiv_eta->SetFillColor(0);
		TexDiv_eta->SetBorderSize(0);
		TexDiv_eta->AddText(Form("#Delta R = %.2f", deltaR));
   		TexDiv_eta->Draw("TR");
	
		string divide_eta = "Divide_eta";
		string divide_eta_und = (divide_eta + underscore).c_str();
		c_Divide_eta->Write(divide_eta.c_str());
        	c_Divide_eta->SaveAs((Address_totale_out + divide_eta_und + njets_out + und_DeltaR_0ex + pdf).c_str());
		c_Divide_eta->Close();

		eta->cd();


  		outplots->cd();
  		outplots->Write();
  		outplots->Close();
	}
	
}
	
