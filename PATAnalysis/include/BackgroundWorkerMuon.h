#ifndef BackgroundWorkerMuon_h
#define BackgroundWorkerMuon_h

#include "TFile.h"

//#include "FWCore/TFWLiteSelector/interface/TFWLiteSelector.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "TH1D.h"
#include "TList.h"
#include "TProofOutputFile.h"

class BackgroundWorkerMuon {
public :
   BackgroundWorkerMuon(TProofOutputFile* out, const TList* fInput);

   ~BackgroundWorkerMuon();

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

