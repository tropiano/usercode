//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 18 15:24:15 2009 by ROOT version 5.18/00b
// from TTree Background/tree
// found on file: ../data/ZMM_M20Pythia_full.root
//////////////////////////////////////////////////////////

#ifndef BackgroundWorker_h
#define BackgroundWorker_h

#include "TH1D.h"

#include "FWCore/TFWLiteSelector/interface/TFWLiteSelector.h"
#include "TH1D.h"

class BackgroundWorker {
public :
   BackgroundWorker(const TList*, TList&);

   ~BackgroundWorker();

   void  process(const edm::Event& iEvent);

   void postProcess(TList&) {};

   //Histograms
   TH1D *recPtZ, *recEtaZ, *recMulti, *recMassZ, *recTrackIsoLead, *recEcalIsoLead, *recHcalIsoLead, *recRelIsoLead,
        *recTrackIsoSec, *recEcalIsoSec, *recHcalIsoSec, *recRelIsoSec,
        *recLeadMuPt, *recSecMuPt, *recLeadMuEta, *recSecMuEta,
        *recLeadJetPt, *recLeadJetEta;

};
#endif

