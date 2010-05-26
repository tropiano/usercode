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
#include "THStack.h"
#include "TFrame.h"
#include <iostream>


using namespace std;

void Summer09_and_data_es() {

//--------------------------------impostazioni

//estensioni files
string root = ".root";
string eps = ".eps";

//nome selezione
string selezione_name = "VBTF";

//input files
string data = "Zee_MinBias_SD_Cert_132440_133928_";
string data_name = data + selezione_name + root;

string Zee = "Zee_";
string Zee_name = Zee + selezione_name + root;

string QCD_Pt20to30 = "QCD_Pt20to30_";
string QCD_Pt20to30_name = QCD_Pt20to30 + selezione_name + root;

string QCD_Pt30to80 = "QCD_Pt30to80_";
string QCD_Pt30to80_name = QCD_Pt30to80 + selezione_name + root;

string QCD_Pt80to170 = "QCD_Pt80to170_";
string QCD_Pt80to170_name = QCD_Pt80to170 + selezione_name + root;

string Wenu = "Wenu_";
string Wenu_name = Wenu + selezione_name + root;

string TTbar = "TTbar_";
string TTbar_name = TTbar + selezione_name + root;

//cartelle
string cartella1_name = "RecoElectron";
string cartella2_name = "recZ_Plots";

//grafico
string grafico_name = "recMassZ";

string Titlefix_name = "#color[2]{Min Bias DATA} vs #color[4]{Su09 MC}: ";
string Titlegrafic_name = "Reconstructed Z Mass ( ";
string Titleend_name = " cuts)";

string asseX_name = "Reconstructed Z Mass (GeV/c^{2})";
string asseY_name = "Events";	

int col_data = 1;
int col_Zee = 14;
int col_QCD = 435;
int col_Wenu = 802;
int col_TTbar = 50;

//output file
string output = "prova1";

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

//--------------------------------Zee

THStack *hs_1 = new THStack ();

TFile *Zee_1 = new TFile (Zee_name.c_str());

TDirectoryFile *Cartella1_Zee_1 = (TDirectoryFile*) Zee_1->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_Zee_Cartella2_1 = (TDirectoryFile*) Cartella1_Zee_1->Get(cartella2_name.c_str());
 
TH1D *histoZee_1 = (TH1D*) Cartella1_Zee_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());


histoZee_1->SetName("Z #rightarrow ee");
histoZee_1->SetLineColor(col_Zee);
histoZee_1->SetFillColor(col_Zee);

histoZee_1->Scale(0.0000216);

histoZee_1->Draw("sames");


gPad->Update();

TPaveStats *stZee_1 = (TPaveStats*) histoZee_1->FindObject("stats");
  stZee_1->SetTextColor(col_Zee);
  stZee_1->SetLineColor(col_Zee);
  stZee_1->SetX1NDC(0.78); //new x start position
  stZee_1->SetX2NDC(0.98); //new x end position
  stZee_1->SetY1NDC(0.635); //new y start position
  stZee_1->SetY2NDC(0.795); //new y end position

gPad->Update();


//--------------------------------QCD


TFile *QCD_Pt20to30_1 = new TFile (QCD_Pt20to30_name.c_str());

TDirectoryFile *Cartella1_QCD_Pt20to30_1 = (TDirectoryFile*) QCD_Pt20to30_1->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_QCD_Pt20to30_Cartella2_1 = (TDirectoryFile*) Cartella1_QCD_Pt20to30_1->Get(cartella2_name.c_str());

TH1D *histoQCD_Pt20to30_1 = (TH1D*) Cartella1_QCD_Pt20to30_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());


histoQCD_Pt20to30_1->SetName("QCD_Pt20to30");
histoQCD_Pt20to30_1->SetLineColor(2);
histoQCD_Pt20to30_1->SetFillColor(2);

histoQCD_Pt20to30_1->Scale(0.0000216);



TFile *QCD_Pt30to80_1 = new TFile (QCD_Pt30to80_name.c_str());

TDirectoryFile *Cartella1_QCD_Pt30to80_1 = (TDirectoryFile*) QCD_Pt30to80_1->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_QCD_Pt30to80_Cartella2_1 = (TDirectoryFile*) Cartella1_QCD_Pt30to80_1->Get(cartella2_name.c_str());

TH1D *histoQCD_Pt30to80_1 = (TH1D*) Cartella1_QCD_Pt30to80_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());


histoQCD_Pt30to80_1->SetName("QCD_Pt30to80");
histoQCD_Pt30to80_1->SetLineColor(3);
histoQCD_Pt30to80_1->SetFillColor(3);

histoQCD_Pt30to80_1->Scale(0.0000216);

histoQCD_Pt30to80_1->Add(histoQCD_Pt20to30_1);


TFile *QCD_Pt80to170_1 = new TFile (QCD_Pt80to170_name.c_str());

TDirectoryFile *Cartella1_QCD_Pt80to170_1 = (TDirectoryFile*) QCD_Pt80to170_1->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_QCD_Pt80to170_Cartella2_1 = (TDirectoryFile*) Cartella1_QCD_Pt80to170_1->Get(cartella2_name.c_str());

TH1D *histoQCD_Pt80to170_1 = (TH1D*) Cartella1_QCD_Pt80to170_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());


histoQCD_Pt80to170_1->SetName("QCD");
histoQCD_Pt80to170_1->SetLineColor(col_QCD);
histoQCD_Pt80to170_1->SetFillColor(col_QCD);

histoQCD_Pt80to170_1->Scale(0.0000216);

histoQCD_Pt80to170_1->Add(histoQCD_Pt30to80_1);

histoQCD_Pt80to170_1->Draw("sames");

gPad->Update();

TPaveStats *stQCD_Pt80to170_1 = (TPaveStats*) histoQCD_Pt80to170_1->FindObject("stats");
  stQCD_Pt80to170_1->SetTextColor(col_QCD);
  stQCD_Pt80to170_1->SetLineColor(col_QCD);
  stQCD_Pt80to170_1->SetX1NDC(0.78); //new x start position
  stQCD_Pt80to170_1->SetX2NDC(0.98); //new x end position
  stQCD_Pt80to170_1->SetY1NDC(0.435); //new y start position
  stQCD_Pt80to170_1->SetY2NDC(0.595); //new y end position

gPad->Update();


//--------------------------------Wenu

TFile *Wenu_1 = new TFile (Wenu_name.c_str());

TDirectoryFile *Cartella1_Wenu_1 = (TDirectoryFile*) Wenu_1->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_Wenu_Cartella2_1 = (TDirectoryFile*) Cartella1_Wenu_1->Get(cartella2_name.c_str());

TH1D *histoWenu_1 = (TH1D*) Cartella1_Wenu_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());


histoWenu_1->SetName("W #rightarrow e #nu");
histoWenu_1->SetLineColor(col_Wenu);
histoWenu_1->SetFillColor(col_Wenu);

histoWenu_1->Scale(0.0000216);

histoWenu_1->Draw("sames");

gPad->Update();

TPaveStats *stWenu_1 = (TPaveStats*) histoWenu_1->FindObject("stats");
  stWenu_1->SetTextColor(col_Wenu);
  stWenu_1->SetLineColor(col_Wenu);
  stWenu_1->SetX1NDC(0.78); //new x start position
  stWenu_1->SetX2NDC(0.98); //new x end position
  stWenu_1->SetY1NDC(0.235); //new y start position
  stWenu_1->SetY2NDC(0.395); //new y end position

gPad->Update();

//--------------------------------TTbar

TFile *TTbar_1 = new TFile (TTbar_name.c_str());

TDirectoryFile *Cartella1_TTbar_1 = (TDirectoryFile*) TTbar_1->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_TTbar_Cartella2_1 = (TDirectoryFile*) Cartella1_TTbar_1->Get(cartella2_name.c_str());

TH1D *histoTTbar_1 = (TH1D*) Cartella1_TTbar_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());


histoTTbar_1->SetName("t #bar{t}");
histoTTbar_1->SetLineColor(col_TTbar);
histoTTbar_1->SetFillColor(col_TTbar);

histoTTbar_1->Scale(0.0000216);

histoTTbar_1->Draw("sames");

gPad->Update();

TPaveStats *stTTbar_1 = (TPaveStats*) histoTTbar_1->FindObject("stats");
  stTTbar_1->SetTextColor(col_TTbar);
  stTTbar_1->SetLineColor(col_TTbar);
  stTTbar_1->SetX1NDC(0.78); //new x start position
  stTTbar_1->SetX2NDC(0.98); //new x end position
  stTTbar_1->SetY1NDC(0.035); //new y start position
  stTTbar_1->SetY2NDC(0.195); //new y end position

gPad->Update();


hs_1->Add(histoZee_1);

hs_1->Add(histoQCD_Pt80to170_1);

hs_1->Add(histoWenu_1);

hs_1->Add(histoTTbar_1);

hs_1->Draw("hist sames");

//-------------------------------


   c_1->Modified();
   c_1->cd();
   c_1->SetSelected(c_1);
   c_1->SaveAs(output_name_1_root.c_str());
   c_1->SaveAs(output_name_1_eps.c_str());
}
