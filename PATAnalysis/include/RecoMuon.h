#ifndef RecoMuon_h
#define RecoMuon_h

#include "TFile.h"

//#include "FWCore/TFWLiteSelector/interface/TFWLiteSelector.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "TH1D.h"
#include "TList.h"
//#include "TProofOutputFile.h"
#include "TFile.h"

class RecoMuon {
public :
   RecoMuon(TFile* out, const TList* fInput);

   ~RecoMuon();

   void  process(const fwlite::ChainEvent& iEvent);

   void finalize() {
     _file->Write();
     _file->Close();
   }

   //Histograms
   TH1D *recPtZ, *recEtaZ, *recMulti, *recMassZ, *recTrackIsoLead, *recEcalIsoLead, *recHcalIsoLead, *recRelIsoLead,
        *recTrackIsoSec, *recEcalIsoSec, *recHcalIsoSec, *recRelIsoSec,
        *recLeadMuPt, *recSecMuPt, *recLeadMuEta, *recSecMuEta,
        *recLeadJetPt, *recLeadJetEta;
private:
  double _ptjetmin;

  double _etajetmax;
  
  double _isocut;

  double _norm;

  TFile* _file;  


};
#endif

