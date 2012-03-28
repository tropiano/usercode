#ifndef RecoMuonNtuple_h
#define RecoMuonNtuple_h

#include "DataFormats/FWLite/interface/Event.h"
#include "MyPatAnalysis.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "TTree.h"
#include "TList.h"
#include "TFile.h"

class RecoMuonNtuple : public MyPatAnalysis{
public :

   RecoMuonNtuple();

   virtual ~RecoMuonNtuple();

   void  process(const fwlite::Event& iEvent);
   
   virtual void begin(TFile* out, const edm::ParameterSet&);
   
   static MyPatAnalysis* create() {return new RecoMuonNtuple(); } 

   void finalize(); 

   void zero_ntuple();

   TTree *zmumutree;
   
   Int_t event, run, lumi;
   Int_t  TrgBit, OneMuTrgMatch;
   Int_t  muTrg1, muTrg2, muTrg3, muTrg4, muTrg5, muTrg6, muTrg7, muTrg8, muTrg9, muTrg10;
   
   Float_t ptzgen, etazgen, phizgen, zmassgen;
   Float_t mugenpt1, mugeneta1, mugenphi1;
   Float_t mugenpt2, mugeneta2, mugenphi2;
   Int_t gennjetsele, acc_gennjetsele;
   Float_t acc_ptzgen, acc_etazgen, acc_phizgen, acc_zmassgen;
   Float_t acc_mugenpt1, acc_mugeneta1, acc_mugenphi1;
   Float_t acc_mugenpt2, acc_mugeneta2, acc_mugenphi2;
   
   Float_t zmass, zmass_AllCuts, zpt, zeta, zphi;
  
   Int_t numberOfZ, nmusall, nmus, npfjetsele, npf2jetsele;
   Int_t genacc_npfjetsele, genacc_npf2jetsele;
   
   Float_t mupt1,mueta1,muphi1,mupt2,mueta2,muphi2;
   Float_t mucharge1,mudB1; 
   Float_t mucharge2,mudB2;
   
   Float_t acc_jetgenpt1,acc_jetgeneta1,acc_jetgenphi1,acc_jetgencharge1;
   Float_t acc_jetgenpt2,acc_jetgeneta2,acc_jetgenphi2,acc_jetgencharge2;
   Float_t acc_jetgenpt3,acc_jetgeneta3,acc_jetgenphi3,acc_jetgencharge3;
   
   Float_t pfjetet1,pfjetpt1,pfjeteta1,pfjetphi1,pfjetcharge1,pfjetDeltaRa1,pfjetDeltaRb1,pfjetEmFrac1;
   Float_t pfjetet2,pfjetpt2,pfjeteta2,pfjetphi2,pfjetcharge2,pfjetDeltaRa2,pfjetDeltaRb2,pfjetEmFrac2;
   Float_t pfjetet3,pfjetpt3,pfjeteta3,pfjetphi3,pfjetcharge3,pfjetDeltaRa3,pfjetDeltaRb3,pfjetEmFrac3;
   
   Float_t pf2jetet1,pf2jetpt1,pf2jeteta1,pf2jetphi1,pf2jecUnc1,pf2jetcharge1,pf2jetDeltaRa1,pf2jetDeltaRb1;
   Float_t pf2jetet2,pf2jetpt2,pf2jeteta2,pf2jetphi2,pf2jecUnc2,pf2jetcharge2,pf2jetDeltaRa2,pf2jetDeltaRb2;
   Float_t pf2jetet3,pf2jetpt3,pf2jeteta3,pf2jetphi3,pf2jecUnc3,pf2jetcharge3,pf2jetDeltaRa3,pf2jetDeltaRb3;
   
   Float_t DeltaR_GenJetPFJet, DeltaR_GenJetPFL1Jet;
   
   Int_t pfjetID1, pfjetID2, pfjetID3;
   Int_t pf2jetID1, pf2jetID2, pf2jetID3;
   
   Float_t rho;

   // counters
   
   Int_t noZfound, moreZfound, exactlyOneZfound, exactlyOneZfoundAcc;
   Int_t numberOfGenEvents, numberOfZgen, numberOfZgenAcc, numberOfZgenfull;
   Int_t exactlyOneZfoundAcc_OneJet,exactlyOneZfoundAcc_TwoJets, exactlyOneZfoundAcc_ThreeJets;
   Int_t OneLooseZfound;
   
   // cut flags
   
   Int_t cutAccSYM, cutAccASYM, cutTrg, cutImp, cutQualSYM, cutQualASYM, cutIsoSYM, cutIsoASYM, cutMuIDSYM, cutMuIDASYM;

private:

   std::string _sample, _selections, _NtupleFill;

  double _ptjetmin;

  double _etajetmax;
  
  double _isocut;

  Float_t _weight;
  
  double _targetLumi;
  
  double _xsec;
  
  int _entries, _EventsPerFile, _EventNumber, _ProcEvents, _Acc, _Trg, _Qual, _Imp, _Iso, _MuID, _JECUnc;
  
  std::string _JECUncFilePath;
  
  JetCorrectionUncertainty* _jecUnc;
  
  std::string _RecoCutFlags[7];
  
  std::string _muTrigVector[10];
  
  std::string _JetType;
  
  bool _Norm;
  
  TDirectory* _dir;

  TFile* _file;  


};
#endif

