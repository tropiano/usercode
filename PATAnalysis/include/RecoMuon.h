#ifndef RecoMuon_h
#define RecoMuon_h

#include "TFile.h"

//#include "FWCore/TFWLiteSelector/interface/TFWLiteSelector.h"
//#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TList.h"
//#include "TProofOutputFile.h"
#include "TFile.h"
#include "MyPatAnalysis.h"

class RecoMuon : public MyPatAnalysis{
public :
   RecoMuon();
   
   virtual ~RecoMuon();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new RecoMuon(); } 

   //void  process(const fwlite::ChainEvent& iEvent);
   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

   //Histograms
   TH1D *recPtZ, *recEtaZ, *recMulti, *recMassZ, *recTrackIsoLead, *recEcalIsoLead, *recHcalIsoLead, *recRelIsoLead,
        *recTrackIsoSec, *recEcalIsoSec, *recHcalIsoSec, *recRelIsoSec,
        *recLeadMuPt, *recSecMuPt, *recLeadMuEta, *recSecMuEta,
        *recLeadJetPt, *recLeadJetEta;

   std::vector<TH1D*> recJetPtVsInclMulti;
   std::vector<TH1D*> recJetEtaVsInclMulti;

   std::vector<TH1D*> recZPtVsInclMulti;
   std::vector<TH1D*> recZEtaVsInclMulti;

   std::vector<TH1D*> recMu1PtVsInclMulti;
   std::vector<TH1D*> recMu1EtaVsInclMulti;

   std::vector<TH1D*> recMu2PtVsInclMulti;
   std::vector<TH1D*> recMu2EtaVsInclMulti;

   std::vector<TH1D*> recZPtVsExclMulti;
   std::vector<TH1D*> recZEtaVsExclMulti;

   std::vector<TH1D*> recMu1PtVsExclMulti;
   std::vector<TH1D*> recMu1EtaVsExclMulti;

   std::vector<TH1D*> recMu2PtVsExclMulti;
   std::vector<TH1D*> recMu2EtaVsExclMulti;    
 
private:
  double _ptjetmin;

  double _etajetmax;
  
  double _isocut;

  double _norm;

  TFile* _file; 

  TDirectory* _dir;

  std::vector<TH1D*> _histoVector;

};
#endif
