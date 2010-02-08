//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Dec 21 12:04:34 2009 by ROOT version 5.22/00d
// from TTree TTbarTree/Tree for TTbar analysis
// found on file: top_prova.root
//////////////////////////////////////////////////////////

#ifndef Class_h
#define Class_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class Class {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           nevt;
   Int_t           jetmult;
   Float_t         jetpt;
   Float_t         electronet;
   Float_t         electroneta;
   Float_t         jetetael;
   Int_t           jetmultmu;
   Float_t         jetptmu;
   Float_t         muonet;
   Float_t         muoneta;
   Float_t         jetetamu;

   // List of branches
   TBranch        *b_nevt;   //!
   TBranch        *b_jetmult;   //!
   TBranch        *b_jetpt;   //!
   TBranch        *b_electronet;   //!
   TBranch        *b_electroneta;   //!
   TBranch        *b_jetetael;   //!
   TBranch        *b_jetmultmu;   //!
   TBranch        *b_jetptmu;   //!
   TBranch        *b_muonet;   //!
   TBranch        *b_muoneta;   //!
   TBranch        *b_jetetamu;   //!

   Class(TTree *tree=0);
   virtual ~Class();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Class_cxx
Class::Class(TString file)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  TTree *tree;
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(file);
  if (!f) {
    f = new TFile(file);
  }
  tree = (TTree*)gDirectory->Get("TTbar Tree");
  Init(tree);
}

Class::~Class()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Class::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;

   return fChain->GetEntry(entry);
}
Long64_t Class::LoadTree(Long64_t entry)
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

void Class::Init(TTree *tree)
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

   fChain->SetBranchAddress("nevt", &nevt, &b_nevt);
   fChain->SetBranchAddress("jetmult", &jetmult, &b_jetmult);
   fChain->SetBranchAddress("jetpt", &jetpt, &b_jetpt);
   fChain->SetBranchAddress("electronet", &electronet, &b_electronet);
   fChain->SetBranchAddress("electroneta", &electroneta, &b_electroneta);
   fChain->SetBranchAddress("jetetael", &jetetael, &b_jetetael);
   fChain->SetBranchAddress("jetmultmu", &jetmultmu, &b_jetmultmu);
   fChain->SetBranchAddress("jetptmu", &jetptmu, &b_jetptmu);
   fChain->SetBranchAddress("muonet", &muonet, &b_muonet);
   fChain->SetBranchAddress("muoneta", &muoneta, &b_muoneta);
   fChain->SetBranchAddress("jetetamu", &jetetamu, &b_jetetamu);
   Notify();
}

Bool_t Class::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Class::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Class::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Class_cxx
