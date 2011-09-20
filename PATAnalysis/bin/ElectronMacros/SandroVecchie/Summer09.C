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

void Summer09() {

//--------------------------------impostazioni

//estensioni files
string root = ".root";
string eps = ".eps";

//nome selezione
string selezione_name = "VBTF";

//input files
string Zee = "Zee_";
string Zee_name = Zee + selezione_name + root;

string QCD = "QCDall_";
string QCD_name = QCD + selezione_name + root;

string Wenu = "Wenu_";
string Wenu_name = Wenu + selezione_name + root;

string TTbar = "TTbar_";
string TTbar_name = TTbar + selezione_name + root;

//cartelle
string cartella1_name = "RecoElectron";
string cartella2_name = "recZ_Plots";

//grafico
string grafico_name = "recMassZ";

double normalizzazione = 0.0000216;

string Titlefix_name = "#color[2]{Su09 MC}: ";
string Titlegrafic_name = "Reconstructed Z Mass ( ";
string Titleend_name = " cuts)";

string asseX_name = "Reconstructed Z Mass (GeV/c^{2})";
string asseY_name = "Events";	

int col_Zee = 14;
int col_QCD = 435;
int col_Wenu = 802;
int col_TTbar = 50;

//output file
string output = "SU09prova";

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

//--------------------------------Zee

THStack *hs_1 = new THStack ();

TFile *Zee_1 = new TFile (Zee_name.c_str());

TDirectoryFile *Cartella1_Zee_1 = (TDirectoryFile*) Zee_1->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_Zee_Cartella2_1 = (TDirectoryFile*) Cartella1_Zee_1->Get(cartella2_name.c_str());
 
TH1D *histoZee_1 = (TH1D*) Cartella1_Zee_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());


histoZee_1->SetName("Z #rightarrow ee");
histoZee_1->SetLineColor(col_Zee);
histoZee_1->SetFillColor(col_Zee);

histoZee_1->Scale(normalizzazione);

histoZee_1->Draw();
histoZee_1->SetTitle(Title_name_1.c_str());
histoZee_1->GetYaxis()->SetRangeUser(0.,0.05);
histoZee_1->GetYaxis()->SetTitleOffset(1.38);
histoZee_1->GetXaxis()->SetTitle(asseX_name.c_str());
histoZee_1->GetYaxis()->SetTitle(asseY_name.c_str());

gPad->Update();

TPaveStats *stZee_1 = (TPaveStats*) histoZee_1->FindObject("stats");
  stZee_1->SetTextColor(col_Zee);
  stZee_1->SetLineColor(col_Zee);
  stZee_1->SetX1NDC(0.78); //new x start position
  stZee_1->SetX2NDC(0.98); //new x end position
  stZee_1->SetY1NDC(0.835); //new y start position
  stZee_1->SetY2NDC(0.995); //new y end position

gPad->Update();


//--------------------------------QCD

TFile *QCD_all_1 = new TFile (QCD_name.c_str());

TDirectoryFile *Cartella1_QCD_all_1 = (TDirectoryFile*) QCD_all_1->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_QCD_all_Cartella2_1 = (TDirectoryFile*) Cartella1_QCD_all_1->Get(cartella2_name.c_str());

TH1D *histoQCD_all_1 = (TH1D*) Cartella1_QCD_all_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());


histoQCD_all_1->SetName("QCD");
histoQCD_all_1->SetLineColor(col_QCD);
histoQCD_all_1->SetFillColor(col_QCD);

histoQCD_all_1->Scale(normalizzazione);

histoQCD_all_1->Draw("sames");

gPad->Update();

TPaveStats *stQCD_all_1 = (TPaveStats*) histoQCD_all_1->FindObject("stats");
  stQCD_all_1->SetTextColor(col_QCD);
  stQCD_all_1->SetLineColor(col_QCD);
  stQCD_all_1->SetX1NDC(0.78); //new x start position
  stQCD_all_1->SetX2NDC(0.98); //new x end position
  stQCD_all_1->SetY1NDC(0.635); //new y start position
  stQCD_all_1->SetY2NDC(0.795); //new y end position

gPad->Update();

//--------------------------------Wenu

TFile *Wenu_1 = new TFile (Wenu_name.c_str());

TDirectoryFile *Cartella1_Wenu_1 = (TDirectoryFile*) Wenu_1->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_Wenu_Cartella2_1 = (TDirectoryFile*) Cartella1_Wenu_1->Get(cartella2_name.c_str());

TH1D *histoWenu_1 = (TH1D*) Cartella1_Wenu_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());


histoWenu_1->SetName("W #rightarrow e #nu");
histoWenu_1->SetLineColor(col_Wenu);
histoWenu_1->SetFillColor(col_Wenu);

histoWenu_1->Scale(normalizzazione);

histoWenu_1->Draw("sames");

gPad->Update();

TPaveStats *stWenu_1 = (TPaveStats*) histoWenu_1->FindObject("stats");
  stWenu_1->SetTextColor(col_Wenu);
  stWenu_1->SetLineColor(col_Wenu);
  stWenu_1->SetX1NDC(0.78); //new x start position
  stWenu_1->SetX2NDC(0.98); //new x end position
  stWenu_1->SetY1NDC(0.435); //new y start position
  stWenu_1->SetY2NDC(0.595); //new y end position

gPad->Update();

//--------------------------------TTbar

TFile *TTbar_1 = new TFile (TTbar_name.c_str());

TDirectoryFile *Cartella1_TTbar_1 = (TDirectoryFile*) TTbar_1->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_TTbar_Cartella2_1 = (TDirectoryFile*) Cartella1_TTbar_1->Get(cartella2_name.c_str());

TH1D *histoTTbar_1 = (TH1D*) Cartella1_TTbar_Cartella2_1->Get(grafico_e_taglio_name_1.c_str());


histoTTbar_1->SetName("t #bar{t}");
histoTTbar_1->SetLineColor(col_TTbar);
histoTTbar_1->SetFillColor(col_TTbar);

histoTTbar_1->Scale(normalizzazione);

histoTTbar_1->Draw("sames");

gPad->Update();

TPaveStats *stTTbar_1 = (TPaveStats*) histoTTbar_1->FindObject("stats");
  stTTbar_1->SetTextColor(col_TTbar);
  stTTbar_1->SetLineColor(col_TTbar);
  stTTbar_1->SetX1NDC(0.78); //new x start position
  stTTbar_1->SetX2NDC(0.98); //new x end position
  stTTbar_1->SetY1NDC(0.235); //new y start position
  stTTbar_1->SetY2NDC(0.395); //new y end position

gPad->Update();


hs_1->Add(histoZee_1);

hs_1->Add(histoQCD_all_1);

hs_1->Add(histoWenu_1);

hs_1->Add(histoTTbar_1);

hs_1->Draw("hist sames");

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

//--------------------------------Zee

THStack *hs_2 = new THStack ();

TFile *Zee_2 = new TFile (Zee_name.c_str());

TDirectoryFile *Cartella1_Zee_2 = (TDirectoryFile*) Zee_2->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_Zee_Cartella2_2 = (TDirectoryFile*) Cartella1_Zee_2->Get(cartella2_name.c_str());
 
TH1D *histoZee_2 = (TH1D*) Cartella1_Zee_Cartella2_2->Get(grafico_e_taglio_name_2.c_str());


histoZee_2->SetName("Z #rightarrow ee");
histoZee_2->SetLineColor(col_Zee);
histoZee_2->SetFillColor(col_Zee);

histoZee_2->Scale(normalizzazione);

histoZee_2->Draw();
histoZee_2->SetTitle(Title_name_2.c_str());
histoZee_2->GetYaxis()->SetRangeUser(0.,0.05);
histoZee_2->GetYaxis()->SetTitleOffset(1.38);
histoZee_2->GetXaxis()->SetTitle(asseX_name.c_str());
histoZee_2->GetYaxis()->SetTitle(asseY_name.c_str());


gPad->Update();

TPaveStats *stZee_2 = (TPaveStats*) histoZee_2->FindObject("stats");
  stZee_2->SetTextColor(col_Zee);
  stZee_2->SetLineColor(col_Zee);
  stZee_2->SetX1NDC(0.78); //new x start position
  stZee_2->SetX2NDC(0.98); //new x end position
  stZee_2->SetY1NDC(0.835); //new y start position
  stZee_2->SetY2NDC(0.995); //new y end position

gPad->Update();


//--------------------------------QCD

TFile *QCD_all_2 = new TFile (QCD_name.c_str());

TDirectoryFile *Cartella1_QCD_all_2 = (TDirectoryFile*) QCD_all_2->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_QCD_all_Cartella2_2 = (TDirectoryFile*) Cartella1_QCD_all_2->Get(cartella2_name.c_str());

TH1D *histoQCD_all_2 = (TH1D*) Cartella1_QCD_all_Cartella2_2->Get(grafico_e_taglio_name_2.c_str());


histoQCD_all_2->SetName("QCD");
histoQCD_all_2->SetLineColor(col_QCD);
histoQCD_all_2->SetFillColor(col_QCD);

histoQCD_all_2->Scale(normalizzazione);

histoQCD_all_2->Draw("sames");

gPad->Update();

TPaveStats *stQCD_all_2 = (TPaveStats*) histoQCD_all_2->FindObject("stats");
  stQCD_all_2->SetTextColor(col_QCD);
  stQCD_all_2->SetLineColor(col_QCD);
  stQCD_all_2->SetX1NDC(0.78); //new x start position
  stQCD_all_2->SetX2NDC(0.98); //new x end position
  stQCD_all_2->SetY1NDC(0.635); //new y start position
  stQCD_all_2->SetY2NDC(0.795); //new y end position

gPad->Update();

//--------------------------------Wenu

TFile *Wenu_2 = new TFile (Wenu_name.c_str());

TDirectoryFile *Cartella1_Wenu_2 = (TDirectoryFile*) Wenu_2->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_Wenu_Cartella2_2 = (TDirectoryFile*) Cartella1_Wenu_2->Get(cartella2_name.c_str());

TH1D *histoWenu_2 = (TH1D*) Cartella1_Wenu_Cartella2_2->Get(grafico_e_taglio_name_2.c_str());


histoWenu_2->SetName("W #rightarrow e #nu");
histoWenu_2->SetLineColor(col_Wenu);
histoWenu_2->SetFillColor(col_Wenu);

histoWenu_2->Scale(normalizzazione);

histoWenu_2->Draw("sames");

gPad->Update();

TPaveStats *stWenu_2 = (TPaveStats*) histoWenu_2->FindObject("stats");
  stWenu_2->SetTextColor(col_Wenu);
  stWenu_2->SetLineColor(col_Wenu);
  stWenu_2->SetX1NDC(0.78); //new x start position
  stWenu_2->SetX2NDC(0.98); //new x end position
  stWenu_2->SetY1NDC(0.435); //new y start position
  stWenu_2->SetY2NDC(0.595); //new y end position

gPad->Update();

//--------------------------------TTbar

TFile *TTbar_2 = new TFile (TTbar_name.c_str());

TDirectoryFile *Cartella1_TTbar_2 = (TDirectoryFile*) TTbar_2->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_TTbar_Cartella2_2 = (TDirectoryFile*) Cartella1_TTbar_2->Get(cartella2_name.c_str());

TH1D *histoTTbar_2 = (TH1D*) Cartella1_TTbar_Cartella2_2->Get(grafico_e_taglio_name_2.c_str());


histoTTbar_2->SetName("t #bar{t}");
histoTTbar_2->SetLineColor(col_TTbar);
histoTTbar_2->SetFillColor(col_TTbar);

histoTTbar_2->Scale(normalizzazione);

histoTTbar_2->Draw("sames");

gPad->Update();

TPaveStats *stTTbar_2 = (TPaveStats*) histoTTbar_2->FindObject("stats");
  stTTbar_2->SetTextColor(col_TTbar);
  stTTbar_2->SetLineColor(col_TTbar);
  stTTbar_2->SetX1NDC(0.78); //new x start position
  stTTbar_2->SetX2NDC(0.98); //new x end position
  stTTbar_2->SetY1NDC(0.235); //new y start position
  stTTbar_2->SetY2NDC(0.395); //new y end position

gPad->Update();


hs_2->Add(histoZee_2);

hs_2->Add(histoQCD_all_2);

hs_2->Add(histoWenu_2);

hs_2->Add(histoTTbar_2);

hs_2->Draw("hist sames");

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

//--------------------------------Zee

THStack *hs_3 = new THStack ();

TFile *Zee_3 = new TFile (Zee_name.c_str());

TDirectoryFile *Cartella1_Zee_3 = (TDirectoryFile*) Zee_3->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_Zee_Cartella2_3 = (TDirectoryFile*) Cartella1_Zee_3->Get(cartella2_name.c_str());
 
TH1D *histoZee_3 = (TH1D*) Cartella1_Zee_Cartella2_3->Get(grafico_e_taglio_name_3.c_str());


histoZee_3->SetName("Z #rightarrow ee");
histoZee_3->SetLineColor(col_Zee);
histoZee_3->SetFillColor(col_Zee);

histoZee_3->Scale(normalizzazione);

histoZee_3->Draw();
histoZee_3->SetTitle(Title_name_3.c_str());
histoZee_3->GetYaxis()->SetRangeUser(0.,0.05);
histoZee_3->GetYaxis()->SetTitleOffset(1.38);
histoZee_3->GetXaxis()->SetTitle(asseX_name.c_str());
histoZee_3->GetYaxis()->SetTitle(asseY_name.c_str());


gPad->Update();

TPaveStats *stZee_3 = (TPaveStats*) histoZee_3->FindObject("stats");
  stZee_3->SetTextColor(col_Zee);
  stZee_3->SetLineColor(col_Zee);
  stZee_3->SetX1NDC(0.78); //new x start position
  stZee_3->SetX2NDC(0.98); //new x end position
  stZee_3->SetY1NDC(0.835); //new y start position
  stZee_3->SetY2NDC(0.995); //new y end position

gPad->Update();


//--------------------------------QCD

TFile *QCD_all_3 = new TFile (QCD_name.c_str());

TDirectoryFile *Cartella1_QCD_all_3 = (TDirectoryFile*) QCD_all_3->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_QCD_all_Cartella2_3 = (TDirectoryFile*) Cartella1_QCD_all_3->Get(cartella2_name.c_str());

TH1D *histoQCD_all_3 = (TH1D*) Cartella1_QCD_all_Cartella2_3->Get(grafico_e_taglio_name_3.c_str());


histoQCD_all_3->SetName("QCD");
histoQCD_all_3->SetLineColor(col_QCD);
histoQCD_all_3->SetFillColor(col_QCD);

histoQCD_all_3->Scale(normalizzazione);

histoQCD_all_3->Draw("sames");

gPad->Update();

TPaveStats *stQCD_all_3 = (TPaveStats*) histoQCD_all_3->FindObject("stats");
  stQCD_all_3->SetTextColor(col_QCD);
  stQCD_all_3->SetLineColor(col_QCD);
  stQCD_all_3->SetX1NDC(0.78); //new x start position
  stQCD_all_3->SetX2NDC(0.98); //new x end position
  stQCD_all_3->SetY1NDC(0.635); //new y start position
  stQCD_all_3->SetY2NDC(0.795); //new y end position

gPad->Update();

//--------------------------------Wenu

TFile *Wenu_3 = new TFile (Wenu_name.c_str());

TDirectoryFile *Cartella1_Wenu_3 = (TDirectoryFile*) Wenu_3->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_Wenu_Cartella2_3 = (TDirectoryFile*) Cartella1_Wenu_3->Get(cartella2_name.c_str());

TH1D *histoWenu_3 = (TH1D*) Cartella1_Wenu_Cartella2_3->Get(grafico_e_taglio_name_3.c_str());


histoWenu_3->SetName("W #rightarrow e #nu");
histoWenu_3->SetLineColor(col_Wenu);
histoWenu_3->SetFillColor(col_Wenu);

histoWenu_3->Scale(normalizzazione);

histoWenu_3->Draw("sames");

gPad->Update();

TPaveStats *stWenu_3 = (TPaveStats*) histoWenu_3->FindObject("stats");
  stWenu_3->SetTextColor(col_Wenu);
  stWenu_3->SetLineColor(col_Wenu);
  stWenu_3->SetX1NDC(0.78); //new x start position
  stWenu_3->SetX2NDC(0.98); //new x end position
  stWenu_3->SetY1NDC(0.435); //new y start position
  stWenu_3->SetY2NDC(0.595); //new y end position

gPad->Update();

//--------------------------------TTbar

TFile *TTbar_3 = new TFile (TTbar_name.c_str());

TDirectoryFile *Cartella1_TTbar_3 = (TDirectoryFile*) TTbar_3->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_TTbar_Cartella2_3 = (TDirectoryFile*) Cartella1_TTbar_3->Get(cartella2_name.c_str());

TH1D *histoTTbar_3 = (TH1D*) Cartella1_TTbar_Cartella2_3->Get(grafico_e_taglio_name_3.c_str());


histoTTbar_3->SetName("t #bar{t}");
histoTTbar_3->SetLineColor(col_TTbar);
histoTTbar_3->SetFillColor(col_TTbar);

histoTTbar_3->Scale(normalizzazione);

histoTTbar_3->Draw("sames");

gPad->Update();

TPaveStats *stTTbar_3 = (TPaveStats*) histoTTbar_3->FindObject("stats");
  stTTbar_3->SetTextColor(col_TTbar);
  stTTbar_3->SetLineColor(col_TTbar);
  stTTbar_3->SetX1NDC(0.78); //new x start position
  stTTbar_3->SetX2NDC(0.98); //new x end position
  stTTbar_3->SetY1NDC(0.235); //new y start position
  stTTbar_3->SetY2NDC(0.395); //new y end position

gPad->Update();


hs_3->Add(histoZee_3);

hs_3->Add(histoQCD_all_3);

hs_3->Add(histoWenu_3);

hs_3->Add(histoTTbar_3);

hs_3->Draw("hist sames");

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

//--------------------------------Zee

THStack *hs_4 = new THStack ();

TFile *Zee_4 = new TFile (Zee_name.c_str());

TDirectoryFile *Cartella1_Zee_4 = (TDirectoryFile*) Zee_4->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_Zee_Cartella2_4 = (TDirectoryFile*) Cartella1_Zee_4->Get(cartella2_name.c_str());
 
TH1D *histoZee_4 = (TH1D*) Cartella1_Zee_Cartella2_4->Get(grafico_e_taglio_name_4.c_str());


histoZee_4->SetName("Z #rightarrow ee");
histoZee_4->SetLineColor(col_Zee);
histoZee_4->SetFillColor(col_Zee);

histoZee_4->Scale(normalizzazione);

histoZee_4->Draw();
histoZee_4->SetTitle(Title_name_4.c_str());
histoZee_4->GetYaxis()->SetRangeUser(0.,0.05);
histoZee_4->GetYaxis()->SetTitleOffset(1.38);
histoZee_4->GetXaxis()->SetTitle(asseX_name.c_str());
histoZee_4->GetYaxis()->SetTitle(asseY_name.c_str());


gPad->Update();

TPaveStats *stZee_4 = (TPaveStats*) histoZee_4->FindObject("stats");
  stZee_4->SetTextColor(col_Zee);
  stZee_4->SetLineColor(col_Zee);
  stZee_4->SetX1NDC(0.78); //new x start position
  stZee_4->SetX2NDC(0.98); //new x end position
  stZee_4->SetY1NDC(0.835); //new y start position
  stZee_4->SetY2NDC(0.995); //new y end position

gPad->Update();


//--------------------------------QCD

TFile *QCD_all_4 = new TFile (QCD_name.c_str());

TDirectoryFile *Cartella1_QCD_all_4 = (TDirectoryFile*) QCD_all_4->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_QCD_all_Cartella2_4 = (TDirectoryFile*) Cartella1_QCD_all_4->Get(cartella2_name.c_str());

TH1D *histoQCD_all_4 = (TH1D*) Cartella1_QCD_all_Cartella2_4->Get(grafico_e_taglio_name_4.c_str());


histoQCD_all_4->SetName("QCD");
histoQCD_all_4->SetLineColor(col_QCD);
histoQCD_all_4->SetFillColor(col_QCD);

histoQCD_all_4->Scale(normalizzazione);

histoQCD_all_4->Draw("sames");

gPad->Update();

TPaveStats *stQCD_all_4 = (TPaveStats*) histoQCD_all_4->FindObject("stats");
  stQCD_all_4->SetTextColor(col_QCD);
  stQCD_all_4->SetLineColor(col_QCD);
  stQCD_all_4->SetX1NDC(0.78); //new x start position
  stQCD_all_4->SetX2NDC(0.98); //new x end position
  stQCD_all_4->SetY1NDC(0.635); //new y start position
  stQCD_all_4->SetY2NDC(0.795); //new y end position

gPad->Update();

//--------------------------------Wenu

TFile *Wenu_4 = new TFile (Wenu_name.c_str());

TDirectoryFile *Cartella1_Wenu_4 = (TDirectoryFile*) Wenu_4->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_Wenu_Cartella2_4 = (TDirectoryFile*) Cartella1_Wenu_4->Get(cartella2_name.c_str());

TH1D *histoWenu_4 = (TH1D*) Cartella1_Wenu_Cartella2_4->Get(grafico_e_taglio_name_4.c_str());


histoWenu_4->SetName("W #rightarrow e #nu");
histoWenu_4->SetLineColor(col_Wenu);
histoWenu_4->SetFillColor(col_Wenu);

histoWenu_4->Scale(normalizzazione);

histoWenu_4->Draw("sames");

gPad->Update();

TPaveStats *stWenu_4 = (TPaveStats*) histoWenu_4->FindObject("stats");
  stWenu_4->SetTextColor(col_Wenu);
  stWenu_4->SetLineColor(col_Wenu);
  stWenu_4->SetX1NDC(0.78); //new x start position
  stWenu_4->SetX2NDC(0.98); //new x end position
  stWenu_4->SetY1NDC(0.435); //new y start position
  stWenu_4->SetY2NDC(0.595); //new y end position

gPad->Update();

//--------------------------------TTbar

TFile *TTbar_4 = new TFile (TTbar_name.c_str());

TDirectoryFile *Cartella1_TTbar_4 = (TDirectoryFile*) TTbar_4->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_TTbar_Cartella2_4 = (TDirectoryFile*) Cartella1_TTbar_4->Get(cartella2_name.c_str());

TH1D *histoTTbar_4 = (TH1D*) Cartella1_TTbar_Cartella2_4->Get(grafico_e_taglio_name_4.c_str());


histoTTbar_4->SetName("t #bar{t}");
histoTTbar_4->SetLineColor(col_TTbar);
histoTTbar_4->SetFillColor(col_TTbar);

histoTTbar_4->Scale(normalizzazione);

histoTTbar_4->Draw("sames");

gPad->Update();

TPaveStats *stTTbar_4 = (TPaveStats*) histoTTbar_4->FindObject("stats");
  stTTbar_4->SetTextColor(col_TTbar);
  stTTbar_4->SetLineColor(col_TTbar);
  stTTbar_4->SetX1NDC(0.78); //new x start position
  stTTbar_4->SetX2NDC(0.98); //new x end position
  stTTbar_4->SetY1NDC(0.235); //new y start position
  stTTbar_4->SetY2NDC(0.395); //new y end position

gPad->Update();


hs_4->Add(histoZee_4);

hs_4->Add(histoQCD_all_4);

hs_4->Add(histoWenu_4);

hs_4->Add(histoTTbar_4);

hs_4->Draw("hist sames");

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

//--------------------------------Zee

THStack *hs_5 = new THStack ();

TFile *Zee_5 = new TFile (Zee_name.c_str());

TDirectoryFile *Cartella1_Zee_5 = (TDirectoryFile*) Zee_5->Get(cartella1_name.c_str());
TDirectoryFile *Cartella1_Zee_Cartella2_5 = (TDirectoryFile*) Cartella1_Zee_5->Get(cartella2_name.c_str());
 
TH1D *histoZee_5 = (TH1D*) Cartella1_Zee_Cartella2_5->Get(grafico_e_taglio_name_5.c_str());


histoZee_5->SetName("Z #rightarrow ee");
histoZee_5->SetLineColor(col_Zee);
histoZee_5->SetFillColor(col_Zee);

histoZee_5->Scale(normalizzazione);

histoZee_5->Draw();
histoZee_5->SetTitle(Title_name_5.c_str());
histoZee_5->GetYaxis()->SetRangeUser(0.,0.05);
histoZee_5->GetYaxis()->SetTitleOffset(1.38);
histoZee_5->GetXaxis()->SetTitle(asseX_name.c_str());
histoZee_5->GetYaxis()->SetTitle(asseY_name.c_str());


gPad->Update();

TPaveStats *stZee_5 = (TPaveStats*) histoZee_5->FindObject("stats");
  stZee_5->SetTextColor(col_Zee);
  stZee_5->SetLineColor(col_Zee);
  stZee_5->SetX1NDC(0.78); //new x start position
  stZee_5->SetX2NDC(0.98); //new x end position
  stZee_5->SetY1NDC(0.835); //new y start position
  stZee_5->SetY2NDC(0.995); //new y end position

gPad->Update();


//--------------------------------QCD

TFile *QCD_all_5 = new TFile (QCD_name.c_str());

TDirectoryFile *Cartella1_QCD_all_5 = (TDirectoryFile*) QCD_all_5->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_QCD_all_Cartella2_5 = (TDirectoryFile*) Cartella1_QCD_all_5->Get(cartella2_name.c_str());

TH1D *histoQCD_all_5 = (TH1D*) Cartella1_QCD_all_Cartella2_5->Get(grafico_e_taglio_name_5.c_str());


histoQCD_all_5->SetName("QCD");
histoQCD_all_5->SetLineColor(col_QCD);
histoQCD_all_5->SetFillColor(col_QCD);

histoQCD_all_5->Scale(normalizzazione);

histoQCD_all_5->Draw("sames");

gPad->Update();

TPaveStats *stQCD_all_5 = (TPaveStats*) histoQCD_all_5->FindObject("stats");
  stQCD_all_5->SetTextColor(col_QCD);
  stQCD_all_5->SetLineColor(col_QCD);
  stQCD_all_5->SetX1NDC(0.78); //new x start position
  stQCD_all_5->SetX2NDC(0.98); //new x end position
  stQCD_all_5->SetY1NDC(0.635); //new y start position
  stQCD_all_5->SetY2NDC(0.795); //new y end position

gPad->Update();

//--------------------------------Wenu

TFile *Wenu_5 = new TFile (Wenu_name.c_str());

TDirectoryFile *Cartella1_Wenu_5 = (TDirectoryFile*) Wenu_5->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_Wenu_Cartella2_5 = (TDirectoryFile*) Cartella1_Wenu_5->Get(cartella2_name.c_str());

TH1D *histoWenu_5 = (TH1D*) Cartella1_Wenu_Cartella2_5->Get(grafico_e_taglio_name_5.c_str());


histoWenu_5->SetName("W #rightarrow e #nu");
histoWenu_5->SetLineColor(col_Wenu);
histoWenu_5->SetFillColor(col_Wenu);

histoWenu_5->Scale(normalizzazione);

histoWenu_5->Draw("sames");

gPad->Update();

TPaveStats *stWenu_5 = (TPaveStats*) histoWenu_5->FindObject("stats");
  stWenu_5->SetTextColor(col_Wenu);
  stWenu_5->SetLineColor(col_Wenu);
  stWenu_5->SetX1NDC(0.78); //new x start position
  stWenu_5->SetX2NDC(0.98); //new x end position
  stWenu_5->SetY1NDC(0.435); //new y start position
  stWenu_5->SetY2NDC(0.595); //new y end position

gPad->Update();

//--------------------------------TTbar

TFile *TTbar_5 = new TFile (TTbar_name.c_str());

TDirectoryFile *Cartella1_TTbar_5 = (TDirectoryFile*) TTbar_5->Get(cartella1_name.c_str());

TDirectoryFile *Cartella1_TTbar_Cartella2_5 = (TDirectoryFile*) Cartella1_TTbar_5->Get(cartella2_name.c_str());

TH1D *histoTTbar_5 = (TH1D*) Cartella1_TTbar_Cartella2_5->Get(grafico_e_taglio_name_5.c_str());


histoTTbar_5->SetName("t #bar{t}");
histoTTbar_5->SetLineColor(col_TTbar);
histoTTbar_5->SetFillColor(col_TTbar);

histoTTbar_5->Scale(normalizzazione);

histoTTbar_5->Draw("sames");

gPad->Update();

TPaveStats *stTTbar_5 = (TPaveStats*) histoTTbar_5->FindObject("stats");
  stTTbar_5->SetTextColor(col_TTbar);
  stTTbar_5->SetLineColor(col_TTbar);
  stTTbar_5->SetX1NDC(0.78); //new x start position
  stTTbar_5->SetX2NDC(0.98); //new x end position
  stTTbar_5->SetY1NDC(0.235); //new y start position
  stTTbar_5->SetY2NDC(0.395); //new y end position

gPad->Update();


hs_5->Add(histoZee_5);

hs_5->Add(histoQCD_all_5);

hs_5->Add(histoWenu_5);

hs_5->Add(histoTTbar_5);

hs_5->Draw("hist sames");

//-------------------------------


   c_5->Modified();
   c_5->cd();
   c_5->SetSelected(c_5);
   c_5->SaveAs(output_name_5_root.c_str());
   c_5->SaveAs(output_name_5_eps.c_str());


}
