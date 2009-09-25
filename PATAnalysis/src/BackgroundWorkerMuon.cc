
#include "Firenze/PATAnalysis/include/BackgroundWorkerMuon.h"
#include "Firenze/PATAnalysis/include/Utilities.h"
#include <TH2.h>
#include <TStyle.h>
#include <math.h>
//#include "Utilities.h"

#include "TList.h"
#include "TParameter.h"
#include "TFile.h"

#include <iostream>

//CMSSW headers
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

using namespace std;
using namespace edm;


BackgroundWorkerMuon::BackgroundWorkerMuon(const TList* in , TList& out): 
recPtZ(0), recEtaZ(0), recMulti(0), recMassZ(0), recTrackIsoLead(0), recEcalIsoLead(0), recHcalIsoLead(0), recRelIsoLead(0),
recTrackIsoSec(0), recEcalIsoSec(0), recHcalIsoSec(0), recRelIsoSec(0),
recLeadMuPt(0), recSecMuPt(0), recLeadMuEta(0), recSecMuEta(0),
recLeadJetPt(0), recLeadJetEta(0), _ptjetmin(30.), _etajetmax(3.), _isocut(0.3)
//_efficiency(out, "EfficienciesTotalVsRecMulti", 10, -0.5, 9.5, 0.3, false),
{       
   TIter next(in);
   while (TObject* obj = next()){
    const TParameter<double>* parDouble = dynamic_cast<const TParameter<double>* >(obj);
    if (parDouble){
      if (!strcmp(parDouble->GetName(), "MinPtJet")){
        _ptjetmin = parDouble->GetVal();
        cout << "set minimum pt for jets to: " << _ptjetmin << endl;
      } else if (!strcmp(parDouble->GetName(), "IsoCut")){
        _isocut = parDouble->GetVal();
        cout << "set isolation cut to: " << _isocut << endl;
      } else if (!strcmp(parDouble->GetName(), "MaxEtaJet")) {
        _etajetmax = parDouble->GetVal();
        cout << "set maximim eta for jets to: " << _etajetmax << endl;
      }
    }
   }  

   recPtZ   = new TH1D("recPtZ", "Reconstructed Z p_{T}", 200, 0, 200);
   out.Add(recPtZ); 
   recEtaZ  = new TH1D("recEtaZ", "Reconstructed Z #eta", 100, -10, 10);
   out.Add(recEtaZ);
   recMulti = new TH1D("recMulti", "Reconstructed jet multiplicity", 10, -0.5, 9.5);
   out.Add(recMulti);
   recMassZ = new TH1D("recMassZ", "Reconstructed Z mass", 200, 50, 150);
   out.Add(recMassZ);
   recTrackIsoLead = new TH1D("recTrackIsoLead", "Lead Track Isolation SumPt", 30, 0, 30);
   out.Add(recTrackIsoLead);
   recEcalIsoLead  = new TH1D("recEcalIsoLead", "Lead ECAL Isolation SumPt", 30, 0, 30);
   out.Add(recEcalIsoLead);
   recHcalIsoLead  = new TH1D("recHcalIsoLead", "Lead HCAL Isolation SumPt", 30, 0, 30);
   out.Add(recHcalIsoLead);
   recRelIsoLead   = new TH1D("recRelIsoLead", "Lead Comb Relative Isolation", 30, 0, 30);
   out.Add(recRelIsoLead);
   recTrackIsoSec = new TH1D("recTrackIsoSec", "Sec Track Isolation SumPt", 30, 0, 30);
   out.Add(recTrackIsoSec);
   recEcalIsoSec  = new TH1D("recEcalIsoSec", "Sec ECAL Isolation SumPt", 30, 0, 30);
   out.Add(recEcalIsoSec);
   recHcalIsoSec  = new TH1D("recHcalIsoSec", "Sec HCAL Isolation SumPt", 30, 0, 30); 
   out.Add(recHcalIsoSec);
   recRelIsoSec   = new TH1D("recRelIsoSec", "Sec Comb Relative Isolation", 30, 0, 30);
   out.Add(recRelIsoSec);
   recLeadMuPt = new TH1D("recLeadMuPt", "Reconstructed Leading #mu p_{T}", 200, 0, 200);
   out.Add(recLeadMuPt);
   recSecMuPt  = new TH1D("recSecMuPt", "Reconstructed Second #mu p_{T}", 200, 0, 200);
   out.Add(recSecMuPt);
   recLeadMuEta = new TH1D("recLeadMuEta", "Reconstructed Leading #mu #eta", 50, -2.5, 2.5);
   out.Add(recLeadMuEta);
   recSecMuEta  = new TH1D("recSecMuEta", "Reconstructed Second #mu #eta", 50, 2.5, 2.5);
   out.Add(recSecMuEta);
   recLeadJetPt = new TH1D("recLeadJetPt", "Reconstructed Leading Jet p_{T}", 200, 0, 200);
   out.Add(recLeadJetPt);
   recLeadJetEta = new TH1D("recLeadJetEta", "Reconstructed Leading Jet #eta", 100, -5, 5); 
   out.Add(recLeadJetEta);
  cout << "Background Worker built." << endl;   
}

BackgroundWorkerMuon::~BackgroundWorkerMuon(){
}

void  BackgroundWorkerMuon::process(const edm::Event& iEvent)
{

   edm::Handle<std::vector<pat::Muon> > muonHandle;
   iEvent.getByLabel("selectedMuons", muonHandle);

   edm::Handle<std::vector<reco::CompositeCandidate> > zHandle;
   iEvent.getByLabel("zmumurec", zHandle);

   edm::Handle<std::vector<pat::Jet> > jetHandle;
   iEvent.getByLabel("selectedJets", jetHandle);

   edm::Handle<pat::TriggerEvent> triggerHandle;
   iEvent.getByLabel("patTriggerEvent", triggerHandle);

   
   //cout << "collection taken" << endl;

   /*if (muonHandle->size()){
      recLeadMuPt->Fill((*muonHandle)[0].pt());
      
   }*/
   //we need to add the piece of code that select the Z candidate in case of multiple candidates
   
   //if (RecSelected(zHandle, zHandleDauMuon, _isocut)){
   if (RecSelectedWithTrigger(*zHandle, *triggerHandle, _isocut)){
      recPtZ->Fill((*zHandle)[0].pt());
      recEtaZ->Fill((*zHandle)[0].eta());
      recMassZ->Fill((*zHandle)[0].mass());
      recLeadMuPt->Fill((*zHandle)[0].daughter(0)->pt());
      recLeadMuEta->Fill((*zHandle)[0].daughter(0)->eta());
      recSecMuPt->Fill((*zHandle)[0].daughter(1)->pt());
      recSecMuEta->Fill((*zHandle)[0].daughter(1)->eta());
      //std::vector<PhysVarTreeJet> selectedjets = GetJets(jetREC, _ptjetmin, _etajetmax);
      //std::vector<pat::Jet> recjets = GetJets(*jetHandle, _ptjetmin, _etajetmax);
      /*
      for (int i = 0; i < recjets.size()+1; ++i){
        recZPtVsInclMulti[i]->Fill((*zHandle)[0]._pt);
        recZEtaVsInclMulti[i]->Fill((*zHandle)[0]._eta);
      }
      */

      //if (recjets.size()){
      //  recLeadJetPt->Fill(recjets[0]._pt);
      //  recLeadJetEta->Fill(recjets[0]._eta);
        /*
        for (int i = 0; i < recjets.size(); ++i){
          recJetPtVsInclMulti[i+1]->Fill(recjets[i]._pt);
          recJetEtaVsInclMulti[i+1]->Fill(recjets[i]._eta);
        }*/
      //}
   }
/*
   const pat::Muon* dau0 = (const pat::Muon*) zHandle->daughter(0);  
   const pat::Muon* dau1 = (const pat::Muon*) zHandle->daughter(1);  

   if (RecSelectedNoIso(zHandle, zHandleDauMuon)){
      recTrackIsoLead->Fill(dau0->trackIso());
      recEcalIsoLead->Fill(dau0->ecalIso());
      recHcalIsoLead->Fill(dau0->hcalIso());
      recRelIsoLead->Fill( (dau0->trackIso() + dau0->ecalIso() + dau0->hcalIso()) / dau0->pt());
      recTrackIsoSec->Fill(dau1->trackIso());
      recEcalIsoSec->Fill(dau1->ecalIso());
      recHcalIsoSec->Fill(dau1->hcalIso());
      recRelIsoSec->Fill( (dau1->trackIso() + dau1->ecalIso() + dau1->hcalIso()) / dau1->pt());
   }
*/

}
