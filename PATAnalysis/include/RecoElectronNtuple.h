#ifndef RecoElectronNtuple_h
#define RecoElectronNtuple_h

#include "DataFormats/FWLite/interface/Event.h"
#include "MyPatAnalysis.h"
#include "TTree.h"
#include "TList.h"
#include "TFile.h"

class RecoElectronNtuple : public MyPatAnalysis{
public :

   RecoElectronNtuple();

   virtual ~RecoElectronNtuple();

   void  process(const fwlite::Event& iEvent);
   
   virtual void begin(TFile* out, const edm::ParameterSet&);
   
   static MyPatAnalysis* create() {return new RecoElectronNtuple(); } 

   void finalize(); 

   void zero_ntuple();

   // added a simple tree  EG 12.10.09
   TTree *electrontree;
   Float_t ptzgen, etazgen, phizgen, zmassgen;
   Float_t elegenpt1, elegeneta1, elegenphi1;
   Float_t elegenpt2, elegeneta2, elegenphi2;
   Int_t gennjetsele, acc_gennjetsele;
   Float_t acc_ptzgen, acc_etazgen, acc_phizgen, acc_zmassgen;
   Float_t acc_elegenpt1, acc_elegeneta1, acc_elegenphi1;
   Float_t acc_elegenpt2, acc_elegeneta2, acc_elegenphi2;
   //
   Float_t loosezmass, zmass, zpt, zeta, zphi;
   // loosezmass>0. is a loose recostructed Z, only reconstructed mass cut
   // zmass>0. is a good recostructed Z, all cuts except electron ID
   Int_t numberOfZ, nelesall, neles, njetsele;
   Float_t elept1,eleeta1,elephi1,elept2,eleeta2,elephi2;
   Float_t elecharge1,eledB1,elefbrem1,eledeltaeta1,eledeltaphi1,eleHovE1,
     eleHovE1other,eleEovP1,elesigmaetaeta1,elesigmaietaieta1; 
   Float_t elecharge2,eledB2,elefbrem2,eledeltaeta2,eledeltaphi2,eleHovE2,
     eleHovE2other,eleEovP2,elesigmaetaeta2,elesigmaietaieta2;
   Float_t elehcaliso1, eleecaliso1, eletrackiso1, elechisq1;
   Float_t elehcaliso2, eleecaliso2, eletrackiso2, elechisq2;
   Int_t elerobusttight1, elerobusttight2;
   Int_t elerobustloose1, elerobustloose2;
   Int_t eletight1, eletight2;
   Int_t eleloose1, eleloose2;
   Int_t elenhits1, elenhits2;
   
   Float_t acc_jetgenpt1,acc_jetgeneta1,acc_jetgenphi1,acc_jetgencharge1;
   Float_t acc_jetgenpt2,acc_jetgeneta2,acc_jetgenphi2,acc_jetgencharge2;
   Float_t acc_jetgenpt3,acc_jetgeneta3,acc_jetgenphi3,acc_jetgencharge3;
   Float_t jetpt1,jeteta1,jetphi1,jetcharge1,jetDeltaRa1,jetDeltaRb1,jetEmFrac1;
   Float_t jetpt2,jeteta2,jetphi2,jetcharge2,jetDeltaRa2,jetDeltaRb2,jetEmFrac2;
   Float_t jetpt3,jeteta3,jetphi3,jetcharge3,jetDeltaRa3,jetDeltaRb3,jetEmFrac3;


   // counters
   
   Int_t noZfound, moreZfound, exactlyOneZfound, exactlyOneZfoundAcc;
   Int_t numberOfGenEvents, numberOfZgen, numberOfZgenAcc, numberOfZgenfull;
   Int_t  exactlyOneZfoundAcc_OneJet,exactlyOneZfoundAcc_TwoJets, exactlyOneZfoundAcc_ThreeJets;
   Int_t OneLooseZfound;

private:

  double _ptjetmin;

  double _etajetmax;
  
  double _isocut;

  Float_t _weight;
  
  double _targetLumi;
  
  double _xsec;
  
  bool _Norm;
  
  int _entries, _EventsPerFile;
  
  std::string _electronID;
  
  TDirectory* _dir;

  TFile* _file;  


};
#endif

