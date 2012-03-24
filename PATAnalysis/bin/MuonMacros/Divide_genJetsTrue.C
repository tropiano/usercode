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


void Divide_genJetsTrue(){
        
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


	//DeltaR
	string DeltaR = "_0e5";
	float deltaR = 0.5;

	//file
	string pre_file = "DYJetsToLL_M_50_Fall11";
	string DeltaR_w = "_DeltaR";
	string root = ".root";
	TFile *file = TFile::Open((pre_file + DeltaR_w + DeltaR + root).c_str()) ;
             
	//Output
	string pre_out = "genJetsTrue";
	string out = (pre_out + DeltaR_w + DeltaR).c_str();
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
	int col_NUM = 4;
	int col_DEN = 2;
	int col_DIV = 1;

	//SetRangeUser
	double nminX = 0.0; 
	double nmaxX = 6.0;
	double nminY = 0.5; 
	double nmaxY = 150000.0;
	
	//Legenda
	string Leg_NUM = "#splitline{2 #mu overlap = true}{with a rec Z}";
	string Leg_DEN = "2 #mu overlap = true";

//-----------------------------------
TDirectory *Jet = new TDirectory();
Jet = outplots->mkdir("JetRate");
Jet->cd();

        //histo NUM
	TH1D* histo_NUM = (TH1D*) file->Get("RecoMuon/DoubleMu_Overlap/numCounterTrue");
        //histo DEN
	TH1D* histo_DEN = (TH1D*) file->Get("RecoMuon/DoubleMu_Overlap/denCounterTrue");

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
	histo_NUM->SetTitle(Title.c_str());
	histo_NUM->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histo_NUM->GetYaxis()->SetRangeUser(nminY, nmaxY);
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
	
//        GenJets->SaveAs((outputpdf.c_str()));
//        GenJets->SaveAs((outputpng.c_str()));
//        GenJets->SaveAs((outputroot.c_str()));
//        GenJets->SaveAs((outputC.c_str()));

	string genjets = "genJetsTrue_";
        string rate = "rate_";
        string png = ".png";
	GenJets->Write(genjets.c_str());
        GenJets->SaveAs((genjets + rate + cartella + DeltaR + png).c_str());
	GenJets->Close();

	TCanvas *Divide = new TCanvas();
        Divide->Range(0,0,25,25);
        Divide->SetLeftMargin(0.160);

	string divideplot = ": Efficiency";

        TH1D *histoDiv_NUM = (TH1D*) histo_NUM->Clone("histoDiv_NUM");
	histoDiv_NUM->Sumw2();

        TH1D *histoDiv_DEN = (TH1D*) histo_DEN->Clone("histoDiv_DEN");
	histoDiv_DEN->Sumw2();

gStyle->SetOptStat(0);
	histoDiv_NUM->SetLineColor(col_DIV);
	histoDiv_NUM->SetLineWidth(2);
	histoDiv_NUM->SetTitle((Title + divideplot).c_str());
	histoDiv_NUM->GetXaxis()->SetRangeUser(nminX, nmaxX);
	histo_NUM->GetYaxis()->SetRangeUser(nminY, nmaxY);
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
	
	string divide = "Efficiency_";
	Divide->Write(divide.c_str());
        Divide->SaveAs((genjets + divide + cartella +  DeltaR + png).c_str());
	Divide->Close();

Jet->cd();

	}
  
  outplots->cd();
  outplots->Write();
  outplots->Close();
}
	
