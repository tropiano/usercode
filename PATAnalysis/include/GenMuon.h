#ifndef GenMuon_h
#define GenMuon_h

#include "TFile.h"

//#include "FWCore/TFWLiteSelector/interface/TFWLiteSelector.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "TH1D.h"
#include "TList.h"
//#include "TProofOutputFile.h"
#include "TFile.h"

class GenMuon {
public :
   GenMuon(TFile* out, const TList* fInput);

   ~GenMuon();

   void  process(const fwlite::ChainEvent& iEvent);

   void finalize(); 

   //Histograms
   TH1D *genPtZ, *genEtaZ, *genMulti, *genMassZ, *genLeadMuPt, *genSecMuPt, *genLeadMuEta, *genSecMuEta,
        *genLeadJetPt, *genLeadJetEta, *genDeltayJfwdJbwd;
        //*genDjr0, *genDjr1, *genDjr2;

   std::vector<TH1D*> genJetPtVsInclMulti;
   std::vector<TH1D*> genJetEtaVsInclMulti;

   std::vector<TH1D*> genZPtVsInclMulti;
   std::vector<TH1D*> genZEtaVsInclMulti;

   std::vector<TH1D*> genMu1PtVsInclMulti;
   std::vector<TH1D*> genMu1EtaVsInclMulti;

   std::vector<TH1D*> genMu2PtVsInclMulti;
   std::vector<TH1D*> genMu2EtaVsInclMulti;

   std::vector<TH1D*> genZPtVsExclMulti;
   std::vector<TH1D*> genZEtaVsExclMulti;

   std::vector<TH1D*> genMu1PtVsExclMulti;
   std::vector<TH1D*> genMu1EtaVsExclMulti;

   std::vector<TH1D*> genMu2PtVsExclMulti;
   std::vector<TH1D*> genMu2EtaVsExclMulti;
   
private:
  double _ptjetmin;

  double _etajetmax;
  
  double _norm;

  TFile* _file;  

  std::vector<TH1D*> _histovector;

};
#endif

