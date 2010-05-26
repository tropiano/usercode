#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TPaveStats.h"
#include "TText.h"
#include "TPaveText.h"
#include <fstream>
#include "TFrame.h"
#include <iostream>


using namespace std;

void DATA_vs_MC() {

//--------------------------------impostazioni

//estensioni files
string root = ".root";
string eps = ".eps";

//nome selezione
string selezione_name = "VBTF";

//input files
string data = "Zee_MinBias_SD_Cert_132440_133928_";
string data_name = data + selezione_name + root;

string mc = "MinBias_MC_Spring10_Norm_Cert_132440_133928_";
string mc_name = mc + selezione_name + root;

//cartelle
string cartella1_name = "RecoElectron";
string cartella2_name = "recZ_Plots";

//grafico
string grafico_name = "recMassZ";

double normalizzazione = 0.0000216;

string Titlefix_name = "Min Bias #color[2]{DATA} vs #color[4]{Sp10 MC}: ";
string Titlegrafic_name = "Reconstructed Z Mass ( ";
string Titleend_name = " cuts)";

string asseX_name = "Reconstructed Z Mass (GeV/c^{2})";
string asseY_name = "Events";	

int col_data = 1;
int col_mc = 51;

//output file
string output = "DvsM";

//--------------------------------stile

gROOT->SetStyle("Plain");
gStyle->SetOptStat(1001111);

//################################ Acc

TCanvas *c_1 = new TCanvas ("c_1", "c_1",457,214,700,500);
//c_1->SetLogy();
c_1->SetRightMargin(0.246);

string taglio_name_1 = "_Acc";
string grafico_e_taglio_name_1 = grafico_name + taglio_name_1 + selezione_name;

string Titlecut_name_1 = "Acc ";
string Title_name_1 = Titlefix_name + Titlegrafic_name + Titlecut_name_1 + selezione_name + Titleend_name;

string output_name_1_root = output + taglio_name_1 + selezione_name + root;
string output_name_1_eps = output + taglio_name_1 + selezione_name + eps;

//--------------------------------dati

TFile *data_1 = new TFile (data_name.c_str());

TDirectoryFile *Cartella1_data_1 = (TDirectoryFile*) data_1->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_data_Cartella2_1 = (TDirectoryFile*) Cartella1_data_1->Get(cartella2_name.c_str());
 
TH1D *histodata_1 = (TH1D*) Cartella1_data_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());

histodata_1->SetName("DATA");
histodata_1->SetLineColor(col_data);

histodata_1->Draw();
histodata_1->SetTitle(Title_name_1.c_str());
histodata_1->GetXaxis()->SetTitle(asseX_name.c_str());
histodata_1->GetYaxis()->SetTitle(asseY_name.c_str());

gPad->Update();

TPaveStats *stdata_1 = (TPaveStats*) histodata_1->FindObject("stats");
  stdata_1->SetTextColor(col_data);
  stdata_1->SetLineColor(col_data);
  stdata_1->SetX1NDC(0.78); //new x start position
  stdata_1->SetX2NDC(0.98); //new x end position
  stdata_1->SetY1NDC(0.835); //new y start position
  stdata_1->SetY2NDC(0.995); //new y end position

gPad->Update();

//--------------------------------mc

TFile *mc_1 = new TFile (mc_name.c_str());

TDirectoryFile *Cartella1_mc_1 = (TDirectoryFile*) mc_1->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_mc_Cartella2_1 = (TDirectoryFile*) Cartella1_mc_1->Get(cartella2_name.c_str());
 
TH1D *histomc_1 = (TH1D*) Cartella1_mc_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());


histomc_1->SetName("MC");
histomc_1->SetLineColor(col_mc);
histomc_1->SetFillColor(col_mc);

histomc_1->Scale(normalizzazione);

histomc_1->Draw("hist sames");


gPad->Update();

TPaveStats *stmc_1 = (TPaveStats*) histomc_1->FindObject("stats");
  stmc_1->SetTextColor(col_mc);
  stmc_1->SetLineColor(col_mc);
  stmc_1->SetX1NDC(0.78); //new x start position
  stmc_1->SetX2NDC(0.98); //new x end position
  stmc_1->SetY1NDC(0.635); //new y start position
  stmc_1->SetY2NDC(0.795); //new y end position

gPad->Update();

//-------------------------------


   c_1->Modified();
   c_1->cd();
   c_1->SetSelected(c_1);
   c_1->SaveAs(output_name_1_root.c_str());
   c_1->SaveAs(output_name_1_eps.c_str());


//################################ Acc & Trg

TCanvas *c_2 = new TCanvas ("c_2", "c_2",457,214,700,500);
//c_2->SetLogy();
c_2->SetRightMargin(0.246);

string taglio_name_2 = "_Trg";
string grafico_e_taglio_name_2 = grafico_e_taglio_name_1 + taglio_name_2;

string Titlecut_name_2 = "Acc & Trg ";
string Title_name_2 = Titlefix_name + Titlegrafic_name + Titlecut_name_2 + selezione_name + Titleend_name;


string output_name_2_root = output + taglio_name_1 + selezione_name + taglio_name_2 + root;
string output_name_2_eps = output + taglio_name_1 + selezione_name + taglio_name_2 + eps;

//--------------------------------dati

TFile *data_2 = new TFile (data_name.c_str());

TDirectoryFile *Cartella1_data_2 = (TDirectoryFile*) data_2->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_data_Cartella2_2 = (TDirectoryFile*) Cartella1_data_2->Get(cartella2_name.c_str());
 
TH1D *histodata_2 = (TH1D*) Cartella1_data_Cartella2_2->Get(grafico_e_taglio_name_2.c_str());

histodata_2->SetName("DATA");
histodata_2->SetLineColor(col_data);

histodata_2->Draw();
histodata_2->SetTitle(Title_name_2.c_str());
histodata_2->GetXaxis()->SetTitle(asseX_name.c_str());
histodata_2->GetYaxis()->SetTitle(asseY_name.c_str());

gPad->Update();

TPaveStats *stdata_2 = (TPaveStats*) histodata_2->FindObject("stats");
  stdata_2->SetTextColor(col_data);
  stdata_2->SetLineColor(col_data);
  stdata_2->SetX1NDC(0.78); //new x start position
  stdata_2->SetX2NDC(0.98); //new x end position
  stdata_2->SetY1NDC(0.835); //new y start position
  stdata_2->SetY2NDC(0.995); //new y end position

gPad->Update();

//--------------------------------mc

TFile *mc_2 = new TFile (mc_name.c_str());

TDirectoryFile *Cartella1_mc_2 = (TDirectoryFile*) mc_2->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_mc_Cartella2_2 = (TDirectoryFile*) Cartella1_mc_2->Get(cartella2_name.c_str());
 
TH1D *histomc_2 = (TH1D*) Cartella1_mc_Cartella2_2->Get(grafico_e_taglio_name_2.c_str());


histomc_2->SetName("MC");
histomc_2->SetLineColor(col_mc);
histomc_2->SetFillColor(col_mc);

histomc_2->Scale(normalizzazione);

histomc_2->Draw("hist sames");


gPad->Update();

TPaveStats *stmc_2 = (TPaveStats*) histomc_2->FindObject("stats");
  stmc_2->SetTextColor(col_mc);
  stmc_2->SetLineColor(col_mc);
  stmc_2->SetX1NDC(0.78); //new x start position
  stmc_2->SetX2NDC(0.98); //new x end position
  stmc_2->SetY1NDC(0.635); //new y start position
  stmc_2->SetY2NDC(0.795); //new y end position

gPad->Update();

//-------------------------------


   c_2->Modified();
   c_2->cd();
   c_2->SetSelected(c_2);
   c_2->SaveAs(output_name_2_root.c_str());
   c_2->SaveAs(output_name_2_eps.c_str());

//################################ Acc, Trg & Imp

TCanvas *c_3 = new TCanvas ("c_3", "c_3",457,214,700,500);
//c_3->SetLogy();
c_3->SetRightMargin(0.246);

string taglio_name_3 = "_Imp";
string grafico_e_taglio_name_3 = grafico_e_taglio_name_2 + taglio_name_3;

string Titlecut_name_3 = "Acc, Trg & Imp ";
string Title_name_3 = Titlefix_name + Titlegrafic_name + Titlecut_name_3 + selezione_name + Titleend_name;


string output_name_3_root = output + taglio_name_1 + selezione_name + taglio_name_2 + taglio_name_3 + root;
string output_name_3_eps = output + taglio_name_1 + selezione_name + taglio_name_2 + taglio_name_3 + eps;

//--------------------------------dati

TFile *data_3 = new TFile (data_name.c_str());

TDirectoryFile *Cartella1_data_3 = (TDirectoryFile*) data_3->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_data_Cartella2_3 = (TDirectoryFile*) Cartella1_data_3->Get(cartella2_name.c_str());
 
TH1D *histodata_3 = (TH1D*) Cartella1_data_Cartella2_3->Get(grafico_e_taglio_name_3.c_str());

histodata_3->SetName("DATA");
histodata_3->SetLineColor(col_data);

histodata_3->Draw();
histodata_3->SetTitle(Title_name_3.c_str());
histodata_3->GetXaxis()->SetTitle(asseX_name.c_str());
histodata_3->GetYaxis()->SetTitle(asseY_name.c_str());

gPad->Update();

TPaveStats *stdata_3 = (TPaveStats*) histodata_3->FindObject("stats");
  stdata_3->SetTextColor(col_data);
  stdata_3->SetLineColor(col_data);
  stdata_3->SetX1NDC(0.78); //new x start position
  stdata_3->SetX2NDC(0.98); //new x end position
  stdata_3->SetY1NDC(0.835); //new y start position
  stdata_3->SetY2NDC(0.995); //new y end position

gPad->Update();

//--------------------------------mc

TFile *mc_3 = new TFile (mc_name.c_str());

TDirectoryFile *Cartella1_mc_3 = (TDirectoryFile*) mc_3->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_mc_Cartella2_3 = (TDirectoryFile*) Cartella1_mc_3->Get(cartella2_name.c_str());
 
TH1D *histomc_3 = (TH1D*) Cartella1_mc_Cartella2_3->Get(grafico_e_taglio_name_3.c_str());


histomc_3->SetName("MC");
histomc_3->SetLineColor(col_mc);
histomc_3->SetFillColor(col_mc);

histomc_3->Scale(normalizzazione);

histomc_3->Draw("hist sames");


gPad->Update();

TPaveStats *stmc_3 = (TPaveStats*) histomc_3->FindObject("stats");
  stmc_3->SetTextColor(col_mc);
  stmc_3->SetLineColor(col_mc);
  stmc_3->SetX1NDC(0.78); //new x start position
  stmc_3->SetX2NDC(0.98); //new x end position
  stmc_3->SetY1NDC(0.635); //new y start position
  stmc_3->SetY2NDC(0.795); //new y end position

gPad->Update();

//-------------------------------


   c_3->Modified();
   c_3->cd();
   c_3->SetSelected(c_3);
   c_3->SaveAs(output_name_3_root.c_str());
   c_3->SaveAs(output_name_3_eps.c_str());

//################################ Acc, Trg, Imp & Iso

TCanvas *c_4 = new TCanvas ("c_4", "c_4",457,214,700,500);
//c_4->SetLogy();
c_4->SetRightMargin(0.246);

string taglio_name_4 = "_Iso";
string grafico_e_taglio_name_4 = grafico_e_taglio_name_3 + taglio_name_4 + selezione_name;

string Titlecut_name_4 = "Acc, Trg, Imp & Iso ";
string Title_name_4 = Titlefix_name + Titlegrafic_name + Titlecut_name_4 + selezione_name + Titleend_name;


string output_name_4_root = output + taglio_name_1 + selezione_name + taglio_name_2 + taglio_name_3 + taglio_name_4 + selezione_name + root;
string output_name_4_eps = output + taglio_name_1 + selezione_name + taglio_name_2 + taglio_name_3 + taglio_name_4 + selezione_name + eps;

//--------------------------------dati

TFile *data_4 = new TFile (data_name.c_str());

TDirectoryFile *Cartella1_data_4 = (TDirectoryFile*) data_4->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_data_Cartella2_4 = (TDirectoryFile*) Cartella1_data_4->Get(cartella2_name.c_str());
 
TH1D *histodata_4 = (TH1D*) Cartella1_data_Cartella2_4->Get(grafico_e_taglio_name_4.c_str());

histodata_4->SetName("DATA");
histodata_4->SetLineColor(col_data);

histodata_4->Draw();
histodata_4->SetTitle(Title_name_4.c_str());
histodata_4->GetXaxis()->SetTitle(asseX_name.c_str());
histodata_4->GetYaxis()->SetTitle(asseY_name.c_str());

gPad->Update();

TPaveStats *stdata_4 = (TPaveStats*) histodata_4->FindObject("stats");
  stdata_4->SetTextColor(col_data);
  stdata_4->SetLineColor(col_data);
  stdata_4->SetX1NDC(0.78); //new x start position
  stdata_4->SetX2NDC(0.98); //new x end position
  stdata_4->SetY1NDC(0.835); //new y start position
  stdata_4->SetY2NDC(0.995); //new y end position

gPad->Update();

//--------------------------------mc

TFile *mc_4 = new TFile (mc_name.c_str());

TDirectoryFile *Cartella1_mc_4 = (TDirectoryFile*) mc_4->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_mc_Cartella2_4 = (TDirectoryFile*) Cartella1_mc_4->Get(cartella2_name.c_str());
 
TH1D *histomc_4 = (TH1D*) Cartella1_mc_Cartella2_4->Get(grafico_e_taglio_name_4.c_str());


histomc_4->SetName("MC");
histomc_4->SetLineColor(col_mc);
histomc_4->SetFillColor(col_mc);

histomc_4->Scale(normalizzazione);

histomc_4->Draw("hist sames");


gPad->Update();

TPaveStats *stmc_4 = (TPaveStats*) histomc_4->FindObject("stats");
  stmc_4->SetTextColor(col_mc);
  stmc_4->SetLineColor(col_mc);
  stmc_4->SetX1NDC(0.78); //new x start position
  stmc_4->SetX2NDC(0.98); //new x end position
  stmc_4->SetY1NDC(0.635); //new y start position
  stmc_4->SetY2NDC(0.795); //new y end position

gPad->Update();

//-------------------------------


   c_4->Modified();
   c_4->cd();
   c_4->SetSelected(c_4);
   c_4->SaveAs(output_name_4_root.c_str());
   c_4->SaveAs(output_name_4_eps.c_str());

//################################ Acc, Trg, Imp, Iso & EiD

TCanvas *c_5 = new TCanvas ("c_5", "c_5",457,214,700,500);
//c_5->SetLogy();
c_5->SetRightMargin(0.246);

string taglio_name_5 = "_EiD";
string grafico_e_taglio_name_5 = grafico_e_taglio_name_4 + taglio_name_5 + selezione_name;

string Titlecut_name_5 = "Acc, Trg, Imp, Iso & EiD ";
string Title_name_5 = Titlefix_name + Titlegrafic_name + Titlecut_name_5 + selezione_name + Titleend_name;


string output_name_5_root = output + taglio_name_1 + selezione_name + taglio_name_2 + taglio_name_3 + taglio_name_4 + selezione_name + taglio_name_5 + selezione_name + root;
string output_name_5_eps = output + taglio_name_1 + selezione_name + taglio_name_2 + taglio_name_3 + taglio_name_4 + selezione_name + taglio_name_5 + selezione_name + eps;

//--------------------------------dati

TFile *data_5 = new TFile (data_name.c_str());

TDirectoryFile *Cartella1_data_5 = (TDirectoryFile*) data_5->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_data_Cartella2_5 = (TDirectoryFile*) Cartella1_data_5->Get(cartella2_name.c_str());
 
TH1D *histodata_5 = (TH1D*) Cartella1_data_Cartella2_5->Get(grafico_e_taglio_name_5.c_str());

histodata_5->SetName("DATA");
histodata_5->SetLineColor(col_data);

histodata_5->Draw();
histodata_5->SetTitle(Title_name_5.c_str());
histodata_5->GetXaxis()->SetTitle(asseX_name.c_str());
histodata_5->GetYaxis()->SetTitle(asseY_name.c_str());

gPad->Update();

TPaveStats *stdata_5 = (TPaveStats*) histodata_5->FindObject("stats");
  stdata_5->SetTextColor(col_data);
  stdata_5->SetLineColor(col_data);
  stdata_5->SetX1NDC(0.78); //new x start position
  stdata_5->SetX2NDC(0.98); //new x end position
  stdata_5->SetY1NDC(0.835); //new y start position
  stdata_5->SetY2NDC(0.995); //new y end position

gPad->Update();

//--------------------------------mc

TFile *mc_5 = new TFile (mc_name.c_str());

TDirectoryFile *Cartella1_mc_5 = (TDirectoryFile*) mc_5->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_mc_Cartella2_5 = (TDirectoryFile*) Cartella1_mc_5->Get(cartella2_name.c_str());
 
TH1D *histomc_5 = (TH1D*) Cartella1_mc_Cartella2_5->Get(grafico_e_taglio_name_5.c_str());


histomc_5->SetName("MC");
histomc_5->SetLineColor(col_mc);
histomc_5->SetFillColor(col_mc);

histomc_5->Scale(normalizzazione);

histomc_5->Draw("hist sames");


gPad->Update();

TPaveStats *stmc_5 = (TPaveStats*) histomc_5->FindObject("stats");
  stmc_5->SetTextColor(col_mc);
  stmc_5->SetLineColor(col_mc);
  stmc_5->SetX1NDC(0.78); //new x start position
  stmc_5->SetX2NDC(0.98); //new x end position
  stmc_5->SetY1NDC(0.635); //new y start position
  stmc_5->SetY2NDC(0.795); //new y end position

gPad->Update();

//-------------------------------


   c_5->Modified();
   c_5->cd();
   c_5->SetSelected(c_5);
   c_5->SaveAs(output_name_5_root.c_str());
   c_5->SaveAs(output_name_5_eps.c_str());


}
