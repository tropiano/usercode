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

   TTree *zeetree;
   
   Int_t event, run, lumi;
   Int_t  TrgBit, OneElTrgMatch;
   Int_t  elTrg1, elTrg2, elTrg3, elTrg4, elTrg5, elTrg6, elTrg7, elTrg8, elTrg9, elTrg10;
   
   Float_t ptzgen, etazgen, phizgen, zmassgen;
   Float_t elegenpt1, elegeneta1, elegenphi1;
   Float_t elegenpt2, elegeneta2, elegenphi2;
   Int_t gennjetsele, acc_gennjetsele;
   Float_t acc_ptzgen, acc_etazgen, acc_phizgen, acc_zmassgen;
   Float_t acc_elegenpt1, acc_elegeneta1, acc_elegenphi1;
   Float_t acc_elegenpt2, acc_elegeneta2, acc_elegenphi2;
   
   Float_t zmass, zmass_AllCuts, zpt, zeta, zphi;
  
   Int_t numberOfZ, nelesall, neles, npfjetsele, npfl1jetsele;
   
   Float_t elept1,eleeta1,elephi1,elept2,eleeta2,elephi2;
   Float_t elecharge1,eledB1,elefbrem1,eledeltaeta1,eledeltaphi1,eleHovE1,
     eleHovE1other,eleEovP1,elesigmaetaeta1,elesigmaietaieta1; 
   Float_t elecharge2,eledB2,elefbrem2,eledeltaeta2,eledeltaphi2,eleHovE2,
     eleHovE2other,eleEovP2,elesigmaetaeta2,elesigmaietaieta2;
   Float_t elehcaliso1, eleecaliso1, eletrackiso1, elereliso1, elechisq1;
   Float_t elehcaliso2, eleecaliso2, eletrackiso2, elereliso2, elechisq2;
   Int_t elerobusttight1, elerobusttight2;
   Int_t elerobustloose1, elerobustloose2;
   Int_t ele1mapID80cIso,ele2mapID95cIso;
   Int_t eletight1, eletight2;
   Int_t eleloose1, eleloose2;
   Int_t elenhits1, elenhits2;
   
   Float_t acc_jetgenpt1,acc_jetgeneta1,acc_jetgenphi1,acc_jetgencharge1;
   Float_t acc_jetgenpt2,acc_jetgeneta2,acc_jetgenphi2,acc_jetgencharge2;
   Float_t acc_jetgenpt3,acc_jetgeneta3,acc_jetgenphi3,acc_jetgencharge3;
   
   Float_t pfjetet1,pfjetpt1,pfjeteta1,pfjetphi1,pfjetcharge1,pfjetDeltaRa1,pfjetDeltaRb1,pfjetEmFrac1;
   Float_t pfjetet2,pfjetpt2,pfjeteta2,pfjetphi2,pfjetcharge2,pfjetDeltaRa2,pfjetDeltaRb2,pfjetEmFrac2;
   Float_t pfjetet3,pfjetpt3,pfjeteta3,pfjetphi3,pfjetcharge3,pfjetDeltaRa3,pfjetDeltaRb3,pfjetEmFrac3;
   
   Float_t pfl1jetet1,pfl1jetpt1,pfl1jeteta1,pfl1jetphi1,pfl1jetcharge1,pfl1jetDeltaRa1,pfl1jetDeltaRb1;
   Float_t pfl1jetet2,pfl1jetpt2,pfl1jeteta2,pfl1jetphi2,pfl1jetcharge2,pfl1jetDeltaRa2,pfl1jetDeltaRb2;
   Float_t pfl1jetet3,pfl1jetpt3,pfl1jeteta3,pfl1jetphi3,pfl1jetcharge3,pfl1jetDeltaRa3,pfl1jetDeltaRb3;
   
   Float_t DeltaR_GenJetPFJet, DeltaR_GenJetPFL1Jet;
   
   Float_t CorrLeadElPt_AllCuts, CorrSecElPt_AllCuts, CorrZMass_AllCuts, CorrZPt_AllCuts;

   // counters
   
   Int_t noZfound, moreZfound, exactlyOneZfound, exactlyOneZfoundAcc;
   Int_t numberOfGenEvents, numberOfZgen, numberOfZgenAcc, numberOfZgenfull;
   Int_t exactlyOneZfoundAcc_OneJet,exactlyOneZfoundAcc_TwoJets, exactlyOneZfoundAcc_ThreeJets;
   Int_t OneLooseZfound;
   
   // cut flags
   
   Int_t cutAccSYM, cutAccASYM, cutTrg, cutImp, cutConvASYM, cutIsoSYM, cutIsoASYM, cutEiDSYM, cutEiDASYM;

private:

   std::string _sample, _selections, _NtupleFill;

  double _ptjetmin;

  double _etajetmax;
  
  double _isocut;

  Float_t _weight;
  
  double _targetLumi;
  
  double _xsec;
  
  int _entries, _EventsPerFile, _EventNumber, _ProcEvents, _Acc, _Trg, _Conv, _Imp, _Iso, _EiD;
  
  std::string _RecoCutFlags[7];
  
  std::string _elTrigVector[10];
  
  bool _Norm;
  
  TDirectory* _dir;

  TFile* _file;  


};
#endif

