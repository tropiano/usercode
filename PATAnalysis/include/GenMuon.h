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

   void finalize() {
     _file->Write();
     _file->Close();
   }

   //Histograms
   TH1D *genPtZ, *genEtaZ, *genMulti, *genMassZ, *genLeadMuPt, *genSecMuPt, *genLeadMuEta, *genSecMuEta,
        *genLeadJetPt, *genLeadJetEta, *genDeltayJfwdJbwd;
        //*genDjr0, *genDjr1, *genDjr2; 
   
private:
  double _ptjetmin;

  double _etajetmax;
  
  double _norm;

  TFile* _file;  


};
#endif

