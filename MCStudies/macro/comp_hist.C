{
  gROOT->Reset();
  gROOT->Clear();
  
  gStyle->SetNdivisions(5);
  gStyle->SetCanvasBorderMode(0); 
  gStyle->SetPadBorderMode(0);
  gStyle->SetOptTitle(1);
  gStyle->SetStatFont(42);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(30);
  gStyle->SetTitleFont(62,"xy");
  gStyle->SetLabelFont(62,"xy");
  gStyle->SetTitleFontSize(0.05);
  gStyle->SetTitleColor(5);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleSize(0.055,"xy");
  gStyle->SetLabelSize(0.05,"xy");
  gStyle->SetHistFillStyle(1001);
  gStyle->SetHistFillColor(4);
  gStyle->SetHistLineStyle(1);
  gStyle->SetHistLineWidth(4);
  gStyle->SetHistLineColor(1);
  gStyle->SetTitleXOffset(4.1);
  gStyle->SetTitleYOffset(1.15);
  gStyle->SetTitleX(0.08);
  gStyle->SetTitleBorderSize(0);
  //gStyle->SetOptStat(1110);
  gStyle->SetOptStat(kFALSE);
  gStyle->SetOptFit(0111);
  gStyle->SetStatH(0.1);


  //TFile f0("top_w0j_alp+pyt.root");
  //TDirectory* dir0=gDirectory;
TFile f1("hjetmult_w1j.root");
TDirectory* dir1=gDirectory;
TFile f2("hjetmult_w2j.root");
TDirectory* dir2=gDirectory;
TFile f3("hjetmult_w3j.root");
TDirectory* dir3=gDirectory;
TFile f4("hjetmult_w4j.root");
TDirectory* dir4=gDirectory;
 TFile f5("hjetmult_herwig10TeV.root");
TDirectory* dir5=gDirectory;
TFile f6("hjetmult_herwigpp10TeV.root");
TDirectory* dir6=gDirectory;
  
TH1F *jetmult_0j, *jetmult_1j, *jetmult_2j, *jetmult_3j, *jetmult_4j, *jetmult_he, *jetmult_hepp; 
TH1F jetmult_all;

//dir0->GetObject("hjetmult",jetmult_0j);
dir1->GetObject("hjetmult",jetmult_1j);
dir2->GetObject("hjetmult",jetmult_2j);
dir3->GetObject("hjetmult",jetmult_3j);
dir4->GetObject("hjetmult",jetmult_4j);
dir5->GetObject("hjetmult",jetmult_he);
dir6->GetObject("hjetmult",jetmult_hepp);
 
jetmult_1j->Sumw2();
jetmult_2j->Sumw2();
jetmult_3j->Sumw2();
jetmult_4j->Sumw2();

//jetmult_1j->Scale(1./343);
//jetmult_2j->Scale(1./333);
//jetmult_3j->Scale(1./285);
//jetmult_4j->Scale(1./15);
//jetmult_all=(*jetmult_1j)+(*jetmult_2j)+(*jetmult_3j)+(*jetmult_4j);
//jetmult_all.Scale(1./jetmult_all.Integral());
jetmult_he->Scale(1./jetmult_he->Integral());
jetmult_hepp->Scale(1./jetmult_hepp->Integral());

TCanvas *c1 = new TCanvas("c1", "c1",-20,231,700,700);
//jet multiplicity
TCanvas *c1 = new TCanvas("c1", "c1",-20,231,700,700);
c1->Divide(1,2);
c1->cd(1);
gPad->SetLogy();
//jetmult_all->GetXaxis()->SetTitle("Number of jets");
//jetmult_all->GetXaxis()->SetTitleSize(0.065);
// etmult_all->GetXaxis()->SetTitleOffset(0.65);
//jetmult_all->SetFillColor(0);
//jetmult_all->SetLineColor(4);
//jetmult_all->SetMarkerStyle(1);
//jetmult_all->SetFillStyle(3004);
//jetmult_all->Draw("e");
  
jetmult_he->SetMarkerStyle(4);
jetmult_he->SetLineColor(2);
//c1->cd(2);
jetmult_he->Draw("");
//c1->Draw();
//c1->Print("boiadeh.pdf");
//c1->Divide(1,2);

jetmult_hepp->SetMarkerStyle(4);
jetmult_hepp->SetLineColor(3);
//c1->cd(2);
jetmult_hepp->Draw("same");
l = new TLegend(0.4,0.6,0.7,0.7);
l->SetTextSize(0.026);
l->SetFillColor(0);
l->SetBorderSize(0);
//l->AddEntry(&jetmult_all,"AlpgenPythia","LPF");
l->AddEntry(jetmult_hepp,"Herwig++","LPF");
l->AddEntry(jetmult_he,"Herwig","LPF");
l->Draw();

c1->Print("jet_multiplicity.pdf");
//delete l;
//delete c1;
  
}

