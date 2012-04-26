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


void Divide_genJetsTrue_AN(){
       
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

	string DeltaR_0ex = "DeltaR_0e5";
	string und_DeltaR_0ex = (underscore + DeltaR_0ex).c_str();
	float deltaR = 0.5;

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
	string file_cartella = (Address_sample_und + DeltaR_0ex + und_pt_x_bar).c_str();
	string Address_totale_in = (Address_radice + pt_x_bar + Address_sample_bar + file_cartella).c_str();
	string file_in = (Address_sample_und + DeltaR_0ex + und_pt_x + root).c_str();

	TFile *file = TFile::Open((Address_totale_in + file_in).c_str());
             
	//file Output
	string out_cartella = "genJets/genJetsTrue/";
	string Address_totale_out = (Address_totale_in + Address_Vertex + Address_IsoRho + out_cartella).c_str();
	string pre_file_out = "genJetsTrue";
	string file_out = (pre_file_out + und_DeltaR_0ex + root).c_str();

	string outputroot = (Address_totale_out + file_out).c_str();
	TFile* outplots = new TFile(outputroot.c_str(), "RECREATE");	
	

	//colori	
	int col_NUM = 4;
	int col_DEN = 2;
	int col_DIV = 1;

	//SetRangeUser
	double nminX = 0.0; 
	double nmaxX = 6.0;
//	double nminY = 0.001; 
//	double nmaxY = 100.0;
	
	//Legenda
	string Leg_NUM = "#splitline{2 #mu overlap = true}{with a rec Z}";
	string Leg_DEN = "2 #mu overlap = true";

//-----------------------------------
TDirectory *Jet = new TDirectory();
Jet = outplots->mkdir("JetRate");
Jet->cd();

        //histo NUM
	string histo_NUM_string = histo_NUM_Vertex;
	TH1D* histo_NUM = (TH1D*) file->Get(histo_NUM_string.c_str());
        //histo DEN
	string histo_DEN_string = histo_DEN_Vertex;
	TH1D* histo_DEN = (TH1D*) file->Get(histo_DEN_string.c_str());

 string cartella;
 string cartella_exclusive_name = "Exclusive";
 string cartella_inclusive_name = "Inclusive";

 string Title;
 string Title_exclusive = "Exclusive Jet rate";
 string Title_inclusive = "Inclusive Jet rate";


for(int a=1; a<3; a++){

	if(a==1){cartella = cartella_exclusive_name;
		 Title = Title_exclusive;}
	if(a==2){cartella = cartella_inclusive_name;
		 Title = Title_inclusive;
     
		for(int n=1; n<10; n++){
                
                double err_NUM, err_DEN;        
		histo_NUM->SetBinContent(n, histo_NUM->IntegralAndError(n, 10, err_NUM));
		histo_NUM->SetBinError(n, err_NUM);
		histo_DEN->SetBinContent(n, histo_DEN->IntegralAndError(n, 10, err_DEN));
		histo_DEN->SetBinError(n, err_DEN);
		}
	}

TDirectory *Dir = new TDirectory;
Dir = Jet->mkdir(cartella.c_str());
Dir->cd();

	TCanvas *GenJets = new TCanvas();
	GenJets->SetLogy();	
        GenJets->Range(0,0,25,25);
        GenJets->SetLeftMargin(0.160);

	histo_NUM->SetLineColor(col_NUM);
	histo_NUM->SetLineWidth(2);
	//histo_NUM->SetTitle(Title.c_str());
	histo_NUM->SetTitle(0);
	histo_NUM->GetXaxis()->SetRangeUser(nminX, nmaxX);
	//histo_NUM->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histo_NUM->GetXaxis()->SetTitle("Number of gen Jets");
	histo_NUM->GetYaxis()->SetTitle("Events");
        histo_NUM->GetYaxis()->SetTitleOffset(1.5);
	histo_NUM->Draw("hist");	

	histo_DEN->SetLineColor(col_DEN);
	histo_DEN->SetLineWidth(2);
	histo_DEN->Draw("hist same");
		
	TPaveText *TexJet = new TPaveText();
	TexJet->SetFillColor(0);
	TexJet->SetBorderSize(0);
	TexJet->AddText(Form("#Delta R = %.2f", deltaR));
   	TexJet->Draw("TR");
	
	TLegend *LegJet = new TLegend(0.51,0.67,0.88,0.88);
	LegJet->SetFillColor(0);
	LegJet->SetBorderSize(0);
	LegJet->AddEntry(histo_NUM,Leg_NUM.c_str());
   	LegJet->AddEntry(histo_DEN,Leg_DEN.c_str());
   	LegJet->Draw("LP");
	
	string rate = "rate";
	string und_rate_und = (underscore + rate + underscore).c_str();

	GenJets->SaveAs((Address_totale_out + pre_file_out + und_rate_und + cartella + und_DeltaR_0ex + pdf).c_str());
	GenJets->Close();

	TCanvas *Divide = new TCanvas();
        Divide->Range(0,0,25,25);
        Divide->SetLeftMargin(0.160);

//	string divideplot = ": Efficiency";

        TH1D *histoDiv_NUM = (TH1D*) histo_NUM->Clone("histoDiv_NUM");
	histoDiv_NUM->Sumw2();

        TH1D *histoDiv_DEN = (TH1D*) histo_DEN->Clone("histoDiv_DEN");
	histoDiv_DEN->Sumw2();

gStyle->SetOptStat(0);
	histoDiv_NUM->SetLineColor(col_DIV);
	histoDiv_NUM->SetLineWidth(2);
	//histoDiv_NUM->SetTitle((Title + divideplot).c_str());
	histoDiv_NUM->SetTitle(0);
	histoDiv_NUM->GetXaxis()->SetRangeUser(nminX, nmaxX);
	//histo_NUM->GetYaxis()->SetRangeUser(nminY, nmaxY);
	histoDiv_NUM->GetXaxis()->SetTitle("Number of gen Jets");
	histoDiv_NUM->GetYaxis()->SetTitle("Ratio");
        histoDiv_NUM->GetYaxis()->SetTitleOffset(1.5);

	histoDiv_NUM->Divide(histoDiv_NUM,histoDiv_DEN, 1, 1, "B");
	histoDiv_NUM->Draw("E");	

	TPaveText *TexDiv = new TPaveText();
	TexDiv->SetFillColor(0);
	TexDiv->SetBorderSize(0);
	TexDiv->AddText(Form("#Delta R = %.2f", deltaR));
   	TexDiv->Draw("TR");
	
	string divide = "Efficiency";
	string und_divide_und = (underscore + divide + underscore).c_str();

	Divide->Write(divide.c_str());
	Divide->SaveAs((Address_totale_out + pre_file_out + und_divide_und + cartella + und_DeltaR_0ex + pdf).c_str());
	Divide->Close();

Jet->cd();

	}
  
  outplots->cd();
  outplots->Write();
  outplots->Close();
}
	
