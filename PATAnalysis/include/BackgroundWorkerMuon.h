#ifndef BackgroundWorkerMuon_h
#define BackgroundWorkerMuon_h

#include "TH1D.h"

#include "FWCore/TFWLiteSelector/interface/TFWLiteSelector.h"
#include "TH1D.h"

class BackgroundWorkerMuon {
public :
   BackgroundWorkerMuon(const TList*, TList&);

   ~BackgroundWorkerMuon();

   void  process(const edm::Event& iEvent);

   void postProcess(TList&) {};

   //Histograms
   TH1D *recPtZ, *recEtaZ, *recMulti, *recMassZ, *recTrackIsoLead, *recEcalIsoLead, *recHcalIsoLead, *recRelIsoLead,
        *recTrackIsoSec, *recEcalIsoSec, *recHcalIsoSec, *recRelIsoSec,
        *recLeadMuPt, *recSecMuPt, *recLeadMuEta, *recSecMuEta,
        *recLeadJetPt, *recLeadJetEta;
private:
  double _ptjetmin;

  double _etajetmax;
  
  double _isocut;


};
#endif

