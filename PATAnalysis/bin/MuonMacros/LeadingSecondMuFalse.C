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


void LeadingSecondMuFalse_COMBO(){
        
       
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
	string LeadSecMu = "_LeadSecMu";
	string file_cartella = (Address_sample_und + DeltaR_0ex + und_pt_x_bar).c_str();
	string Address_totale_in = (Address_radice + pt_x_bar + Address_sample_bar + file_cartella).c_str();
	string file_in = (Address_sample_und + DeltaR_0ex + und_pt_x + LeadSecMu + root).c_str();

	TFile *file = TFile::Open((Address_totale_in + file_in).c_str());

	//colori	
	int col_LEA = 4;
	int col_SEC = 2;

	//SetRangeUser
	double nminX_pt = 0.0; 
	double nmaxX_pt = 180.0;
	double nminY_pt = 1.0; 
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
	string Leg_LEA_pt = "Leading #mu p_{T}";
	string Leg_SEC_pt = "Second #mu p_{T}";

	string Leg_LEA_eta = "Leading #mu #eta";
	string Leg_SEC_eta = "Second #mu #eta";

	// moltitudine di grafici
  	for (unsigned i = 0; i < 7; ++i){
	  	ostringstream oss;
  		oss << i;
 		string n = oss.str();

 		string njets = (duepunti + n + spazio + jets).c_str();
 		string njets_out = (n + jets).c_str();     

        
     		//file Output
		string pre_cartella_out = "LeadSecMu/";
		string cartella_out = (pre_cartella_out + DeltaR_0ex_bar).c_str();
		string Address_totale_out = (Address_totale_in + Address_Vertex + Address_IsoRho + cartella_out).c_str();

		string pre_file_out = "LeadSecMu_";
		string file_out = (pre_file_out + DeltaR_0ex_und + n + jets + root).c_str();

		string outputroot = (Address_totale_out + file_out).c_str();
		TFile* outplots = new TFile(outputroot.c_str(), "RECREATE");

//-----------------------------------

		TDirectory *pt = new TDirectory();
		pt = outplots->mkdir("pt");
		pt->cd();

		//histo LEA
		string histo_LEA_pt_string = (partial_histo_LEA_pt + njets_out).c_str();
		TH1D* histo_LEA_pt = (TH1D*) file->Get(histo_LEA_pt_string.c_str());
	        //histo SEC
		string histo_SEC_pt_string = (partial_histo_SEC_pt + njets_out).c_str();
		TH1D* histo_SEC_pt = (TH1D*) file->Get(histo_SEC_pt_string.c_str());

		string Title_pt = "2 #mu overlap = false with a rec Z";

		TCanvas *c_pt = new TCanvas();
		c_pt->SetLogy();	
	        c_pt->Range(0,0,25,25);
	        c_pt->SetLeftMargin(0.160);


		histo_LEA_pt->SetLineColor(col_LEA);

		histo_LEA_pt->SetLineWidth(2);
		histo_LEA_pt->SetTitle((Title_pt + njets).c_str());
		//histo_LEA_pt->SetTitle(0);

		histo_LEA_pt->Rebin(10);
		histo_LEA_pt->GetXaxis()->SetRangeUser(nminX_pt, nmaxX_pt);
		histo_LEA_pt->GetYaxis()->SetRangeUser(nminY_pt, nmaxY_pt);
		histo_LEA_pt->GetXaxis()->SetTitle("gen #mu p_{T}");
		histo_LEA_pt->GetYaxis()->SetTitle("Events");
		histo_LEA_pt->GetYaxis()->SetTitleOffset(1.5);
		histo_LEA_pt->Draw("hist");	
	
		histo_SEC_pt->SetLineColor(col_SEC);
		histo_SEC_pt->SetLineWidth(2);
		histo_SEC_pt->Rebin(10);
		histo_SEC_pt->Draw("hist same");

	        TH1D *histoClone_LEA_pt = (TH1D*) histo_LEA_pt->Clone("histoClone_LEA_pt");
		histoClone_LEA_pt->Sumw2();
	        TH1D *histoClone_SEC_pt = (TH1D*) histo_SEC_pt->Clone("histoClone_SEC_pt");
		histoClone_SEC_pt->Sumw2();
			
		TPaveText *Tex_pt = new TPaveText();
		Tex_pt->SetFillColor(0);
		Tex_pt->SetBorderSize(0);
		Tex_pt->AddText(Form("#Delta R = %.2f", deltaR));
	   	Tex_pt->Draw("TR");
		
		TLegend *Leg_pt = new TLegend(0.51,0.67,0.88,0.88);
		Leg_pt->SetFillColor(0);
		Leg_pt->SetBorderSize(0);
		Leg_pt->AddEntry(histo_LEA_pt,Leg_LEA_pt.c_str());
	   	Leg_pt->AddEntry(histo_SEC_pt,Leg_SEC_pt.c_str());
	   	Leg_pt->Draw("LP");
		
		string gen_pt = "LeadSec_pt";
		string gen_pt_und = (gen_pt + underscore).c_str();
		c_pt->Write(gen_pt.c_str());
	        c_pt->SaveAs((Address_totale_out + gen_pt_und + njets_out + und_DeltaR_0ex + pdf).c_str());
		c_pt->Close();
	
		pt->cd();

		//-----------------------------------
		TDirectory *eta = new TDirectory();
		eta = outplots->mkdir("eta");
		eta->cd();
		

	        //histo LEA
		string histo_LEA_eta_string = (partial_histo_LEA_eta + njets_out).c_str();
		TH1D* histo_LEA_eta = (TH1D*) file->Get(histo_LEA_eta_string.c_str());
	        //histo SEC
		string histo_SEC_eta_string = (partial_histo_SEC_eta + njets_out).c_str();
		TH1D* histo_SEC_eta = (TH1D*) file->Get(histo_SEC_eta_string.c_str());
	
		string Title_eta = "2 #mu overlap = false with a rec Z";
	
		TCanvas *c_eta = new TCanvas();
		c_eta->SetLogy();	
	        c_eta->Range(0,0,25,25);
	        c_eta->SetLeftMargin(0.160);
	
		histo_LEA_eta->SetLineColor(col_LEA);
		histo_LEA_eta->SetLineWidth(2);
		histo_LEA_eta->SetTitle((Title_eta + njets).c_str());
		//histo_LEA_eta->SetTitle(0);
		histo_LEA_eta->Rebin(2);
		histo_LEA_eta->GetXaxis()->SetRangeUser(nminX_eta, nmaxX_eta);
		histo_LEA_eta->GetYaxis()->SetRangeUser(nminY_eta, nmaxY_eta);
		histo_LEA_eta->GetXaxis()->SetTitle("gen #mu #eta");
		histo_LEA_eta->GetYaxis()->SetTitle("Events");
		histo_LEA_eta->GetYaxis()->SetTitleOffset(1.5);
		histo_LEA_eta->Draw("hist");	

		histo_SEC_eta->SetLineColor(col_SEC);
		histo_SEC_eta->SetLineWidth(2);
		histo_SEC_eta->Rebin(2);
		histo_SEC_eta->Draw("hist same");

	        TH1D *histoClone_LEA_eta = (TH1D*) histo_LEA_eta->Clone("histoClone_LEA_eta");
		histoClone_LEA_eta->Sumw2();
	        TH1D *histoClone_SEC_eta = (TH1D*) histo_SEC_eta->Clone("histoClone_SEC_eta");
		histoClone_SEC_eta->Sumw2();
			
		TPaveText *Tex_eta = new TPaveText();
		Tex_eta->SetFillColor(0);
		Tex_eta->SetBorderSize(0);
		Tex_eta->AddText(Form("#Delta R = %.2f", deltaR));
   		Tex_eta->Draw("TR");
		
		TLegend *Leg_eta = new TLegend(0.51,0.67,0.88,0.88);
		Leg_eta->SetFillColor(0);
		Leg_eta->SetBorderSize(0);
		Leg_eta->AddEntry(histo_LEA_eta,Leg_LEA_eta.c_str());
   		Leg_eta->AddEntry(histo_SEC_eta,Leg_SEC_eta.c_str());
   		Leg_eta->Draw("LP");
		
		string gen_eta = "LeadSec_eta";
		string gen_eta_und = (gen_eta + underscore).c_str();
		c_eta->Write(gen_eta.c_str());
        	c_eta->SaveAs((Address_totale_out + gen_eta_und + njets_out + und_DeltaR_0ex + pdf).c_str());
		c_eta->Close();

		eta->cd();


  		outplots->cd();
  		outplots->Write();
  		outplots->Close();
	}
	
}
	
