#include <TDirectory.h>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <iostream>
//#include <ostringstream>
#include <TCanvas.h> 
#include <TLegend.h>
#include <stdlib.h>
#include <stdio.h>
#include <THStack.h>
#include "TROOT.h"
#include "TStyle.h"
#include "TGraphErrors.h"

void fill_graphs(TDirectory* dir0, TDirectory* dir1, TDirectory* dir2, TGraphErrors* g[9], TGraphErrors* h[9],TGraphErrors* i[9] )
{
  //double lumi_data=444.;
  //cout<< "in fill histos"<<endl;
  dir0->GetObject("eff_l3",g[0]);
  dir1->GetObject("eff_l3",h[0]);
  dir2->GetObject("eff_l3",i[0]);

  dir0->GetObject("eff_pur",g[1]);
  dir1->GetObject("eff_pur",h[1]);
  dir2->GetObject("eff_pur",i[1]);
 
  dir0->GetObject("effl3_evts",g[2]);
  dir1->GetObject("effl3_evts",h[2]);
  dir2->GetObject("effl3_evts",i[2]);

  dir0->GetObject("effl25_evts",g[3]);
  dir1->GetObject("effl25_evts",h[3]);
  dir2->GetObject("effl25_evts",i[3]);

  dir0->GetObject("purl25_evts",g[4]);
  dir1->GetObject("purl25_evts",h[4]);
  dir2->GetObject("purl25_evts",i[4]);

  dir0->GetObject("purl25",g[5]);
  dir1->GetObject("purl25",h[5]);
  dir2->GetObject("purl25",i[5]);

  dir0->GetObject("redu_l25",g[6]);
  dir1->GetObject("redu_l25",h[6]);
  dir2->GetObject("redu_l25",i[6]);

  dir0->GetObject("redu_l3",g[7]);
  dir1->GetObject("redu_l3",h[7]);
  dir2->GetObject("redu_l3",i[7]);
  
  dir0->GetObject("eff_l25",g[8]);
  dir1->GetObject("eff_l25",h[8]);
  dir2->GetObject("eff_l25",i[8]);

  //cout<<"before the for cycle"<<endl; 
  /*for (int i=0; i<15;i++){
    h[i]->Sumw2();
    h[i]->Scale(lumi_data/lumi);
    cout << "histo number "<< i << "scaled" <<endl;
    }*/
  //cout << "histos scaled"<<endl;
  
}

void make_histos_2(){
  //gROOT->Reset();
  //gROOT->Clear();
  //gROOT->SetStyle("myStyle");
  //gROOT->SetStyle("tdrStyle");
  
  gROOT->Reset();
  gROOT->Clear();  
  //gStyle->SetNdivisions(5);
  gStyle->SetCanvasBorderMode(0); 
  gStyle->SetPadBorderMode(0);
  gStyle->SetOptTitle(1);
  gStyle->SetStatFont(42);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(30);
  gStyle->SetTitleFont(62,"xy");
  gStyle->SetLabelFont(62,"xy");
  gStyle->SetTitleFontSize(0.05);
  gStyle->SetTitleFillColor(0);
  gStyle->SetHistFillColor(4);
  gStyle->SetHistLineColor(1);
  gStyle->SetTitleXOffset(1.15);
  //gStyle->SetTitleYOffset(1.15);
  gStyle->SetTitleX(0.08);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetOptStat(kFALSE);
  gStyle->SetOptFit(0111);
  gStyle->SetStatH(0.1);
  gStyle->SetPalette(1,0);
 
  TFile f0("../skims/btagHLT_quad25_2e32.root");
  TDirectory* dir0=gDirectory;
  TFile f1("../skims/btagHLT_single50pre_2e32.root");
  TDirectory* dir1=gDirectory;
  TFile f2("../skims/btagHLT_single50pre_2e32.root");
  TDirectory* dir2=gDirectory;

  
  TGraphErrors* single_graphs[9];
  TGraphErrors* quad_graphs[9];
  TGraphErrors* old_graphs[9];
  
  TH2F* histo[6];

  /*TGraphErrors*  eff_pur_3dvertex;
  TGraphErrors*  eff_pur_onlinebspot;
  TGraphErrors*  eff_3dvertex;
  TGraphErrors*  eff_onlinebspot;*/
  fill_graphs(dir0, dir1, dir2, quad_graphs, single_graphs, old_graphs);
  
  for (int i=0; i<9; i++){
    
    TCanvas *c1 = new TCanvas("c1", "c1",700,700,700,700);
    if (i!=4) gPad->SetLogy();
    if (i==4) gPad->SetLogx();
    
    if(i==2 || i==3 ||i==4) single_graphs[i]->SetMinimum(0.001);
    single_graphs[i]->SetMarkerStyle(20);
    single_graphs[i]->SetMarkerColor(kRed);
    single_graphs[i]->Draw("Apz");
    quad_graphs[i]->SetMarkerStyle(20);
    quad_graphs[i]->GetXaxis()->SetTitle("Rate [Hz]");
    quad_graphs[i]->Draw("pz same");
    //old_graphs[i]->SetMarkerStyle(20);
    //old_graphs[i]->GetXaxis()->SetTitle("Rate [Hz]");
    //old_graphs[i]->Draw("pz same");
    //old_graphs[i]->SetMarkerColor(kBlue);
    TLegend *l = new TLegend(0.15,0.60,0.30,0.70);
    l->SetTextSize(0.030);
    l->SetFillColor(0);
    l->SetBorderSize(0);
    l->AddEntry(quad_graphs[i],"Quad25 2e32","P");
    l->AddEntry(single_graphs[i],"Single50 2e32","P");
    //l->AddEntry(old_graphs[i],"Old Vertex 6e30","P"); 
    l->Draw();
    TString name = (TString)quad_graphs[i]->GetTitle();
    c1->Print("../plots/quad_vs_single_run148952/"+name+".png");
    delete c1;
    delete l;
  }
  
  dir0->GetObject("RecovsL25Discr",histo[0]);
  dir0->GetObject("RecovsL3Discr",histo[1]);
  dir1->GetObject("RecovsL25Discr",histo[2]);
  dir1->GetObject("RecovsL3Discr",histo[3]);
  dir2->GetObject("RecovsL25Discr",histo[4]);
  dir2->GetObject("RecovsL3Discr",histo[5]);
  
  for(int i=0; i<2; i++){
    TCanvas *c1 = new TCanvas("c1", "c1",700,700,700,700);
    histo[i]->Draw("colz");
    TString name = (TString)histo[i]->GetTitle();
    c1->Print("../plots/quad_vs_single_run148952"+name+".png");
    delete c1;
}
  for(int i=2; i<4; i++){
    TCanvas *c1 = new TCanvas("c1", "c1",700,700,700,700);
    histo[i]->Draw("colz");
    TString name = (TString)histo[i]->GetTitle();
    c1->Print("../plots/"+name+".png");
    delete c1;
  }
  for(int i=4; i<6; i++){
    TCanvas *c1 = new TCanvas("c1", "c1",700,700,700,700);
    histo[i]->Draw("colz");
    TString name = (TString)histo[i]->GetTitle();
    c1->Print("../plots/"+name+".png");
    delete c1;
  }

}

