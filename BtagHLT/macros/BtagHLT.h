//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Sep  6 09:34:30 2010 by ROOT version 5.18/00b
// from TTree btagInfo/btagInfo
// found on file: test/btagtest.root
//////////////////////////////////////////////////////////

#ifndef BtagHLT_h
#define BtagHLT_h
#include "TH2.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include <math.h>

class BtagHLT {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         L25Reco_reco_discriminator;
   Float_t         L25Reco_l25_discriminator;
   Float_t         L25Reco_reco_trackmult;
   Float_t         L25Reco_l25_trackmult;
   Float_t         L25Reco_reco_ip2ndtrack;
   Float_t         L25Reco_l25_ip2ndtrack;
   Float_t         L25Reco_reco_sigmaip2ndtrack;
   Float_t         L25Reco_l25_sigmaip2ndtrack;
   Float_t         L25Reco_reco_ip3rdtrack;
   Float_t         L25Reco_l25_ip3rdtrack;
   Float_t         L25Reco_reco_sigmaip3rdtrack;
   Float_t         L25Reco_l25_sigmaip3rdtrack;
   Float_t         L25Reco_reco_deltar;
   Float_t         L25Reco_l25_deltar;
   Float_t         L25Reco_reco_etajet;
   Float_t         L25Reco_l25_etajet;
   Float_t         L25Reco_reco_Etjet;
   Float_t         L25Reco_l25_Etjet;
   Float_t         L25Reco_reco_phijet;
   Float_t         L25Reco_l25_phijet;
   Float_t         L3Reco_reco_discriminator;
   Float_t         L3Reco_l3_discriminator;
   Float_t         L3Reco_reco_trackmult;
   Float_t         L3Reco_l3_trackmult;
   Float_t         L3Reco_reco_ip2ndtrack;
   Float_t         L3Reco_l3_ip2ndtrack;
   Float_t         L3Reco_reco_sigmaip2ndtrack;
   Float_t         L3Reco_l3_sigmaip2ndtrack;
   Float_t         L3Reco_reco_ip3rdtrack;
   Float_t         L3Reco_l3_ip3rdtrack;
   Float_t         L3Reco_reco_sigmaip3rdtrack;
   Float_t         L3Reco_l3_sigmaip3rdtrack;

   Float_t         L3Reco_reco_deltar;
   Float_t         L3Reco_l3_deltar;
   Float_t         L3Reco_reco_etajet;
   Float_t         L3Reco_l3_etajet;
   Float_t         L3Reco_reco_Etjet;
   Float_t         L3Reco_l3_Etjet;
   Float_t         L3Reco_reco_phijet;
   Float_t         L3Reco_l3_phijet;
   Float_t         L3Reco_l3_dz;
   Float_t         L3Reco_reco_dz;
   Float_t         L3Reco_l3_sigmadz;
   Float_t         L3Reco_reco_sigmadz;

   Float_t         L3Reco_l3_dxy;
   Float_t         L3Reco_reco_dxy;
   Float_t         L3Reco_l3_sigmadxy;
   Float_t         L3Reco_reco_sigmadxy;
   UInt_t         eventNum;
   
   UInt_t          prev_eventNum;
 
   // List of branches
   TBranch        *b_L25Reco_reco_discriminator;   //!
   TBranch        *b_L25Reco_l25_discriminator;   //!
   TBranch        *b_L25Reco_reco_trackmult;   //!
   TBranch        *b_L25Reco_l25_trackmult;   //!
   TBranch        *b_L25Reco_reco_ip2ndtrack;   //!
   TBranch        *b_L25Reco_l25_ip2ndtrack;   //!
   TBranch        *b_L25Reco_reco_sigmaip2ndtrack;   //!
   TBranch        *b_L25Reco_l25_sigmaip2ndtrack;   //!
   TBranch        *b_L25Reco_reco_ip3rdtrack;   //!
   TBranch        *b_L25Reco_l25_ip3rdtrack;   //!
   TBranch        *b_L25Reco_reco_sigmaip3rdtrack;   //!
   TBranch        *b_L25Reco_l25_sigmaip3rdtrack;   //!
   TBranch        *b_L25Reco_reco_deltar;   //!
   TBranch        *b_L25Reco_l25_deltar;   //!
   TBranch        *b_L25Reco_reco_etajet;   //!
   TBranch        *b_L25Reco_l25_etajet;   //!
   TBranch        *b_L25Reco_reco_Etjet;   //!
   TBranch        *b_L25Reco_l25_Etjet;   //!
   TBranch        *b_L25Reco_reco_phijet;   //!
   TBranch        *b_L25Reco_l25_phijet;   //!
   TBranch        *b_L3Reco_reco_discriminator;   //!
   TBranch        *b_L3Reco_l3_discriminator;   //!
   TBranch        *b_L3Reco_reco_trackmult;   //!
   TBranch        *b_L3Reco_l3_trackmult;   //!
   TBranch        *b_L3Reco_reco_ip2ndtrack;   //!
   TBranch        *b_L3Reco_l3_ip2ndtrack;   //!
   TBranch        *b_L3Reco_reco_sigmaip2ndtrack;   //!
   TBranch        *b_L3Reco_l3_sigmaip2ndtrack;   //!
   TBranch        *b_L3Reco_reco_ip3rdtrack;   //!
   TBranch        *b_L3Reco_l3_ip3rdtrack;   //!
   TBranch        *b_L3Reco_reco_sigmaip3rdtrack;   //!
   TBranch        *b_L3Reco_l3_sigmaip3rdtrack;   //!
   TBranch        *b_L3Reco_reco_deltar;   //!
   TBranch        *b_L3Reco_l3_deltar;   //!
   TBranch        *b_L3Reco_reco_etajet;   //!
   TBranch        *b_L3Reco_l3_etajet;   //!
   TBranch        *b_L3Reco_reco_Etjet;   //!
   TBranch        *b_L3Reco_l3_Etjet;   //!
   TBranch        *b_L3Reco_reco_phijet;   //!
   TBranch        *b_L3Reco_l3_phijet;   //!
   TBranch        *b_L3Reco_reco_dz;   //!
   TBranch        *b_L3Reco_l3_dz;   //!
   TBranch        *b_L3Reco_reco_sigmadz;   //!
   TBranch        *b_L3Reco_l3_sigmadz;   //!

   TBranch        *b_L3Reco_reco_dxy;   //!
   TBranch        *b_L3Reco_l3_dxy;   //!
   TBranch        *b_L3Reco_reco_sigmadxy;   //!
   TBranch        *b_L3Reco_l3_sigmadxy;   //!
   
   TBranch        *b_eventNum;   //!
   //TBranch        *b_eventNum;   //!
   

   BtagHLT(TTree *tree=0);
   virtual ~BtagHLT();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual void     makeProfile(TH2F *hist1, int rebin, TProfile *profile);
   virtual void     changeStyle(TProfile *profile, const char *labelX, const char *labelY, int color, int style, float min, float max);
};

#endif

#ifdef BtagHLT_cxx
BtagHLT::BtagHLT(TTree *tree)
{
  // if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  bool offline = false;
  bool online  = false;
  bool vertex  = false;
  bool quad    = true;
  if(quad){
    if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../data/run148952/btaghlt_quad25_2e32.root");
      if (!f) {
	f = new TFile("../data/run148952/btaghlt_quad25_2e32.root");
      }
    }
  }
  if(offline){
    if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../data/run144112/btag_offline.root");
      if (!f) {
	f = new TFile("../data/run144112/btag_offline.root");
      }
    }
  }
  else if(online){
    if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../data/run148952/btaghlt_single50pre.root");
      if (!f) {
	f = new TFile("../data/run148952/btaghlt_single50pre.root");
      }
      
    }
  }
  else if(vertex)
    {
      if (tree == 0) {
	TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../data/run144112/btag_online3d.root");
	if (!f) {
	  f = new TFile("../data/run144112/btag_online3d.root");
	}
      }
    }
  tree = (TTree*)gDirectory->Get("btagInfo");
  Init(tree);
}

BtagHLT::~BtagHLT()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t BtagHLT::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t BtagHLT::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void BtagHLT::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("L25Reco_reco_discriminator", &L25Reco_reco_discriminator, &b_L25Reco_reco_discriminator);
   fChain->SetBranchAddress("L25Reco_l25_discriminator", &L25Reco_l25_discriminator, &b_L25Reco_l25_discriminator);
   fChain->SetBranchAddress("L25Reco_reco_trackmult", &L25Reco_reco_trackmult, &b_L25Reco_reco_trackmult);
   fChain->SetBranchAddress("L25Reco_l25_trackmult", &L25Reco_l25_trackmult, &b_L25Reco_l25_trackmult);
   fChain->SetBranchAddress("L25Reco_reco_ip2ndtrack", &L25Reco_reco_ip2ndtrack, &b_L25Reco_reco_ip2ndtrack);
   fChain->SetBranchAddress("L25Reco_l25_ip2ndtrack", &L25Reco_l25_ip2ndtrack, &b_L25Reco_l25_ip2ndtrack);
   fChain->SetBranchAddress("L25Reco_reco_sigmaip2ndtrack", &L25Reco_reco_sigmaip2ndtrack, &b_L25Reco_reco_sigmaip2ndtrack);
   fChain->SetBranchAddress("L25Reco_l25_sigmaip2ndtrack", &L25Reco_l25_sigmaip2ndtrack, &b_L25Reco_l25_sigmaip2ndtrack);
   fChain->SetBranchAddress("L25Reco_reco_ip3rdtrack", &L25Reco_reco_ip3rdtrack, &b_L25Reco_reco_ip3rdtrack);
   fChain->SetBranchAddress("L25Reco_l25_ip3rdtrack", &L25Reco_l25_ip3rdtrack, &b_L25Reco_l25_ip3rdtrack);
   fChain->SetBranchAddress("L25Reco_reco_sigmaip3rdtrack", &L25Reco_reco_sigmaip3rdtrack, &b_L25Reco_reco_sigmaip3rdtrack);
   fChain->SetBranchAddress("L25Reco_l25_sigmaip3rdtrack", &L25Reco_l25_sigmaip3rdtrack, &b_L25Reco_l25_sigmaip3rdtrack);
   fChain->SetBranchAddress("L25Reco_reco_deltar", &L25Reco_reco_deltar, &b_L25Reco_reco_deltar);
   fChain->SetBranchAddress("L25Reco_l25_deltar", &L25Reco_l25_deltar, &b_L25Reco_l25_deltar);
   fChain->SetBranchAddress("L25Reco_reco_etajet", &L25Reco_reco_etajet, &b_L25Reco_reco_etajet);
   fChain->SetBranchAddress("L25Reco_l25_etajet", &L25Reco_l25_etajet, &b_L25Reco_l25_etajet);
   fChain->SetBranchAddress("L25Reco_reco_Etjet", &L25Reco_reco_Etjet, &b_L25Reco_reco_Etjet);
   fChain->SetBranchAddress("L25Reco_l25_Etjet", &L25Reco_l25_Etjet, &b_L25Reco_l25_Etjet);
   fChain->SetBranchAddress("L25Reco_reco_phijet", &L25Reco_reco_phijet, &b_L25Reco_reco_phijet);
   fChain->SetBranchAddress("L25Reco_l25_phijet", &L25Reco_l25_phijet, &b_L25Reco_l25_phijet);
   fChain->SetBranchAddress("L3Reco_reco_discriminator", &L3Reco_reco_discriminator, &b_L3Reco_reco_discriminator);
   fChain->SetBranchAddress("L3Reco_l3_discriminator", &L3Reco_l3_discriminator, &b_L3Reco_l3_discriminator);
   fChain->SetBranchAddress("L3Reco_reco_trackmult", &L3Reco_reco_trackmult, &b_L3Reco_reco_trackmult);
   fChain->SetBranchAddress("L3Reco_l3_trackmult", &L3Reco_l3_trackmult, &b_L3Reco_l3_trackmult);
   
   fChain->SetBranchAddress("L3Reco_reco_ip2ndtrack", &L3Reco_reco_ip2ndtrack, &b_L3Reco_reco_ip2ndtrack);
   fChain->SetBranchAddress("L3Reco_l3_ip2ndtrack", &L3Reco_l3_ip2ndtrack, &b_L3Reco_l3_ip2ndtrack);
   fChain->SetBranchAddress("L3Reco_reco_sigmaip2ndtrack", &L3Reco_reco_sigmaip2ndtrack, &b_L3Reco_reco_sigmaip2ndtrack);
   fChain->SetBranchAddress("L3Reco_l3_sigmaip2ndtrack", &L3Reco_l3_sigmaip2ndtrack, &b_L3Reco_l3_sigmaip2ndtrack);
   fChain->SetBranchAddress("L3Reco_reco_ip3rdtrack", &L3Reco_reco_ip3rdtrack, &b_L3Reco_reco_ip3rdtrack);
   fChain->SetBranchAddress("L3Reco_l3_ip3rdtrack", &L3Reco_l3_ip3rdtrack, &b_L3Reco_l3_ip3rdtrack);
   fChain->SetBranchAddress("L3Reco_reco_sigmaip3rdtrack", &L3Reco_reco_sigmaip3rdtrack, &b_L3Reco_reco_sigmaip3rdtrack);
   fChain->SetBranchAddress("L3Reco_l3_sigmaip3rdtrack", &L3Reco_l3_sigmaip3rdtrack, &b_L3Reco_l3_sigmaip3rdtrack);
   
   fChain->SetBranchAddress("L3Reco_reco_deltar", &L3Reco_reco_deltar, &b_L3Reco_reco_deltar);
   fChain->SetBranchAddress("L3Reco_l3_deltar", &L3Reco_l3_deltar, &b_L3Reco_l3_deltar);
   fChain->SetBranchAddress("L3Reco_reco_etajet", &L3Reco_reco_etajet, &b_L3Reco_reco_etajet);
   fChain->SetBranchAddress("L3Reco_l3_etajet", &L3Reco_l3_etajet, &b_L3Reco_l3_etajet);
   fChain->SetBranchAddress("L3Reco_reco_Etjet", &L3Reco_reco_Etjet, &b_L3Reco_reco_Etjet);
   fChain->SetBranchAddress("L3Reco_l3_Etjet", &L3Reco_l3_Etjet, &b_L3Reco_l3_Etjet);
   fChain->SetBranchAddress("L3Reco_reco_phijet", &L3Reco_reco_phijet, &b_L3Reco_reco_phijet);
   fChain->SetBranchAddress("L3Reco_l3_phijet", &L3Reco_l3_phijet, &b_L3Reco_l3_phijet);

   fChain->SetBranchAddress("L3Reco_reco_dz", &L3Reco_reco_dz, &b_L3Reco_reco_dz);
   fChain->SetBranchAddress("L3Reco_l3_dz", &L3Reco_l3_dz, &b_L3Reco_l3_dz);
   fChain->SetBranchAddress("L3Reco_reco_sigmadz", &L3Reco_reco_sigmadz, &b_L3Reco_reco_sigmadz);
   fChain->SetBranchAddress("L3Reco_l3_sigmadz", &L3Reco_l3_sigmadz, &b_L3Reco_l3_sigmadz);
   
   fChain->SetBranchAddress("L3Reco_reco_dxy", &L3Reco_reco_dxy, &b_L3Reco_reco_dxy);
   fChain->SetBranchAddress("L3Reco_l3_dxy", &L3Reco_l3_dxy, &b_L3Reco_l3_dxy);
   fChain->SetBranchAddress("L3Reco_reco_sigmadxy", &L3Reco_reco_sigmadxy, &b_L3Reco_reco_sigmadxy);
   fChain->SetBranchAddress("L3Reco_l3_sigmadxy", &L3Reco_l3_sigmadxy, &b_L3Reco_l3_sigmadxy);
   
   fChain->SetBranchAddress("eventNum", &eventNum, &b_eventNum);
   //fChain->SetBranchAddress("prev_eventNum", &prev_eventNum, &b_eventNum);
   
   Notify();
}

Bool_t BtagHLT::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void BtagHLT::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t BtagHLT::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef BtagHLT_cxx
