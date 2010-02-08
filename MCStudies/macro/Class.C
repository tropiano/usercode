#define Class_cxx
#include "Class.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Class::Loop(Int_t binpt, Long64_t rangept, Int_t bineta, TString suffix)
{
//   In a ROOT session, you can do:
//      Root > .L Class.C
//      Root > Class t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   TH1F *hjetmult = new TH1F("hjetmult","jet multiplicity",10,0.,10.);
   TH1F *hjetpt = new TH1F("hjetpt","jet pt",binpt,0.,rangept);
   TH1F *hjetetael = new TH1F("hjetetael","jet eta electron chan",bineta, -2.5, 2.5);
   TH1F *helectronet = new TH1F("helectronet","electron et",binpt, 0., rangept);
   TH1F *helectroneta = new TH1F("helectroneta","electron eta",bineta, -2.5, 2.5);
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      fChain->GetEntry(jentry);   
      b_jetmult->GetEntry(jentry);
      b_jetpt->GetEntry(jentry);
      b_jetetael->GetEntry(jentry);
      b_electronet->GetEntry(jentry);
      b_electroneta->GetEntry(jentry);
      hjetmult->Fill(jetmult);
      hjetpt->Fill(jetpt);
      hjetetael->Fill(jetetael);
      helectronet->Fill(electronet);
      helectroneta->Fill(electroneta);
     //std::cout<<"jet multiplicity: "<<jetmult<<endl;
      //hjetmult->Draw();
      //hjetmult->Print("hjet_mult.root");
      nb = fChain->GetEntry(jentry);   
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
   TFile f("h_"+suffix+".root","recreate");
   //hjetmult->Draw();
   //gDirectory->mkdir("jet_multiplicity");
   //gDirectory->cd("jet_multiplicity");
   hjetmult->Write();
   hjetpt->Write();
   hjetetael->Write();
   helectronet->Write(); 
   helectroneta->Write();
}
