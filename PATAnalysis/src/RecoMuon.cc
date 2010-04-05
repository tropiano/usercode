
#include "Firenze/PATAnalysis/include/RecoMuon.h"
#include "Firenze/PATAnalysis/include/Utilities.h"
#include <TH2.h>
#include <TStyle.h>
#include <math.h>
//#include "Utilities.h"

#include "TList.h"
#include "TParameter.h"
#include "TFile.h"
#include "TProofOutputFile.h"

#include <iostream>

//CMSSW headers
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

//RooUnfold


using namespace std;
using namespace edm;


RecoMuon::RecoMuon(): 
recPtZ(0), recEtaZ(0), recMulti(0), recMassZ(0), recTrackIsoLead(0), recEcalIsoLead(0), recHcalIsoLead(0), recRelIsoLead(0),
recTrackIsoSec(0), recEcalIsoSec(0), recHcalIsoSec(0), recRelIsoSec(0),
recLeadMuPt(0), recSecMuPt(0), recLeadMuEta(0), recSecMuEta(0),
recLeadJetPt(0), recLeadJetEta(0), 
_ptjetmin(30.), _etajetmax(3.), _isocut(0.3), _norm(1.),
_file(0), _dir(0), _histoVector()
//_bayesUnfold("bayesUnfoldedMulti", "Bayes unfolded Jet Multiplicity")
//_efficiency(out, "EfficienciesTotalVsRecMulti", 10, -0.5, 9.5, 0.3, false),
{ }

void RecoMuon::begin(TFile* out, const edm::ParameterSet& iConfig){
   _file = out; 
   /*TIter next(fInput);
   bool factorSet = false;  
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
      } else if (!strcmp(parDouble->GetName(), "ScaleFactor")) {
        _norm = parDouble->GetVal();
        cout << "set scale factor to: " << _norm << endl;
        factorSet = true;
      }
    }
   }  

   if (!factorSet) {
     cout << "You did not set the scale factor! " << endl;
     assert(factorSet);
   }*/
   std::string dirname = iConfig.getParameter<std::string>("Name");

   _ptjetmin  = iConfig.getParameter<double>("MinPtJet");
   _isocut    = iConfig.getParameter<double>("IsoCut");
   _etajetmax = iConfig.getParameter<double>("MaxEtaJet");
   _norm      = iConfig.getParameter<double>("ScaleFactor");
   _zcandSrc  = iConfig.getParameter<std::string>("Zsource");

   //_file = proofFile->OpenFile("RECREATE");
   cout << "RecoMuon file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();
   recPtZ   = new TH1D("recPtZ", "Reconstructed Z p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZ);
   recEtaZ  = new TH1D("recEtaZ", "Reconstructed Z #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZ);
   recMulti = new TH1D("recMulti", "Reconstructed jet multiplicity", 10, -0.5, 9.5);
   _histoVector.push_back(recMulti);
   recMassZ = new TH1D("recMassZ", "Reconstructed Z mass", 200, 50, 150);
   _histoVector.push_back(recMassZ);
   recTrackIsoLead = new TH1D("recTrackIsoLead", "Lead Track Isolation SumPt", 30, 0, 30);
   _histoVector.push_back(recTrackIsoLead);
   recEcalIsoLead  = new TH1D("recEcalIsoLead", "Lead ECAL Isolation SumPt", 30, 0, 30);
   _histoVector.push_back(recEcalIsoLead);
   recHcalIsoLead  = new TH1D("recHcalIsoLead", "Lead HCAL Isolation SumPt", 30, 0, 30);
   _histoVector.push_back(recHcalIsoLead);
   recRelIsoLead   = new TH1D("recRelIsoLead", "Lead Comb Relative Isolation", 30, 0, 30);
   _histoVector.push_back(recRelIsoLead);
   recTrackIsoSec = new TH1D("recTrackIsoSec", "Sec Track Isolation SumPt", 30, 0, 30);
   _histoVector.push_back(recTrackIsoSec);
   recEcalIsoSec  = new TH1D("recEcalIsoSec", "Sec ECAL Isolation SumPt", 30, 0, 30);
   _histoVector.push_back(recEcalIsoSec);
   recHcalIsoSec  = new TH1D("recHcalIsoSec", "Sec HCAL Isolation SumPt", 30, 0, 30); 
   _histoVector.push_back(recHcalIsoSec);
   recRelIsoSec   = new TH1D("recRelIsoSec", "Sec Comb Relative Isolation", 30, 0, 30);
   _histoVector.push_back(recRelIsoSec);
   recLeadMuPt = new TH1D("recLeadMuPt", "Reconstructed Leading #mu p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadMuPt);
   recSecMuPt  = new TH1D("recSecMuPt", "Reconstructed Second #mu p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecMuPt);
   recLeadMuEta = new TH1D("recLeadMuEta", "Reconstructed Leading #mu #eta", 50, -2.5, 2.5);
   _histoVector.push_back(recLeadMuEta);
   recSecMuEta  = new TH1D("recSecMuEta", "Reconstructed Second #mu #eta", 50, 2.5, 2.5);
   _histoVector.push_back(recSecMuEta);
   recLeadJetPt = new TH1D("recLeadJetPt", "Reconstructed Leading Jet p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadJetPt);
   recLeadJetEta = new TH1D("recLeadJetEta", "Reconstructed Leading Jet #eta", 100, -5, 5); 
   _histoVector.push_back(recLeadJetEta);
  /* std::vector<TH1D*>::const_iterator ibeg = _histoVector.begin(); 
   std::vector<TH1D*>::const_iterator iend = _histoVector.end(); 
  for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
    (*i)->Sumw2();
  }*/
  _dir->cd("-");
   
  cout << "RecoMuon Worker built." << endl;   
}

RecoMuon::~RecoMuon(){
  _file->ls();
  //_file->Write();
  //_file->Close();
}

//void  RecoMuon::process(const fwlite::ChainEvent& iEvent)
void  RecoMuon::process(const fwlite::Event& iEvent)
{


   //double weight = 1.;
   _file->cd();

   fwlite::Handle<std::vector<pat::Muon> > muonHandle;
   muonHandle.getByLabel(iEvent, "selectedMuons");

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zHandle;
   zHandle.getByLabel(iEvent, _zcandSrc.c_str());//"zmumurec");

   fwlite::Handle<std::vector<pat::Jet> > jetHandle;
   jetHandle.getByLabel(iEvent, "selectedJets");

   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");

   
   //we need to add the piece of code that select the Z candidate in case of multiple candidates
   //if (zHandle->size() > 1) return; 
   
   //we just take the first element in the collection of Z candidates.
   //That is the candidate in which the leading muon has the highest pt
   
   const pat::Muon* dau0 = 0;
   const pat::Muon* dau1 = 0;
   if (zHandle->size() > 0){
    //take the two muons with all the needed castings
    dau0 = dynamic_cast<const pat::Muon*>((*zHandle)[0].daughter(0));
    dau1 = dynamic_cast<const pat::Muon*>((*zHandle)[0].daughter(1));
    /*if (!dau0) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> ((*zHandle)[0].daughter(0));
     if (scc && scc->hasMasterClone()){
       dau0 = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
     }
    }
    if (!dau1) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> ((*zHandle)[0].daughter(1));
     if (scc && scc->hasMasterClone()){
       dau1 = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
     }
    }
    assert(dau0 && dau1);*/
   } 



   //if (RecSelected(zHandle, zHandleDauMuon, _isocut)){
   if (RecSelectedWithTrigger(*zHandle, *triggerHandle, _isocut, dau0, dau1)){
      //cout << "PASSED!" << endl;
      recPtZ->Fill((*zHandle)[0].pt(), _norm);
      recEtaZ->Fill((*zHandle)[0].eta(), _norm);
      recMassZ->Fill((*zHandle)[0].mass(), _norm);
      recLeadMuPt->Fill((*zHandle)[0].daughter(0)->pt(), _norm);
      recLeadMuEta->Fill((*zHandle)[0].daughter(0)->eta(), _norm);
      recSecMuPt->Fill((*zHandle)[0].daughter(1)->pt(), _norm);
      recSecMuEta->Fill((*zHandle)[0].daughter(1)->eta(), _norm);
   
   
      std::vector<const pat::Jet*> recjets = GetJets<pat::Jet>(*jetHandle, _ptjetmin, _etajetmax);

      _dir->cd();
      addHistosVsMulti(recjets.size(), "recJetPtIncl", " reconstructed jet p_{T} spectrum", 200, 0, 200, recJetPtVsInclMulti);
      addHistosVsMulti(recjets.size(), "recJetEtaIncl", " reconstructed jet #eta spectrum", 100, -5., 5., recJetEtaVsInclMulti);
      addHistosVsMulti(recjets.size(), "recZPtIncl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsInclMulti);
      addHistosVsMulti(recjets.size(), "recZEtaIncl", " reconstructed Z #eta spectrum", 100, -5., 5., recZEtaVsInclMulti);
      addHistosVsMulti(recjets.size(), "recZPtExcl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsExclMulti);
      addHistosVsMulti(recjets.size(), "recZEtaExcl", " reconstructed Z #eta spectrum", 100, -5., 5., recZEtaVsExclMulti);
      addHistosVsMulti(recjets.size(), "recZPtExcl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsExclMulti);
      addHistosVsMulti(recjets.size(), "recMu1PtExcl", " reconstructed lead #mu p_{T} spectrum", 200, 0., 200., recMu1PtVsExclMulti);
      addHistosVsMulti(recjets.size(), "recMu1EtaExcl", " reconstructed lead #mu #eta spectrum", 100, -5., 5., recMu1EtaVsExclMulti);
      addHistosVsMulti(recjets.size(), "recMu2PtExcl", " reconstructed sec #mu p_{T} spectrum", 200, 0., 200., recMu2PtVsExclMulti);
      addHistosVsMulti(recjets.size(), "recMu2EtaExcl", " reconstructed sec #mu #eta spectrum", 100, -5., 5., recMu2EtaVsExclMulti);   
      _dir->cd("-");

      recMulti->Fill(recjets.size(), _norm);

      if (recjets.size()){
        recLeadJetPt->Fill(recjets[0]->pt(), _norm);
        recLeadJetEta->Fill(recjets[0]->eta(), _norm);
        for (unsigned int i = 0; i < recjets.size(); ++i){
          recJetPtVsInclMulti[i+1]->Fill(recjets[i]->pt(), _norm);
          recJetEtaVsInclMulti[i+1]->Fill(recjets[i]->eta(), _norm);
        }
      }

      //fill inclusive histograms
      for (unsigned int i = 0; i < recjets.size()+1; ++i){
        recZPtVsInclMulti[i]->Fill((*zHandle)[0].pt(), _norm);
        recZEtaVsInclMulti[i]->Fill((*zHandle)[0].eta(), _norm);
      }

      //fill exclusive histograms
      recZPtVsExclMulti[recjets.size()]->Fill((*zHandle)[0].pt(), _norm);
      recZEtaVsExclMulti[recjets.size()]->Fill((*zHandle)[0].eta(), _norm);
      recMu1PtVsExclMulti[recjets.size()]->Fill(dau0->pt(), _norm);
      recMu1EtaVsExclMulti[recjets.size()]->Fill(dau0->eta(), _norm);
      recMu2PtVsExclMulti[recjets.size()]->Fill(dau1->pt(), _norm);
      recMu2EtaVsExclMulti[recjets.size()]->Fill(dau1->eta(), _norm);

      
   }

   if (RecSelectedNoIso(*zHandle, dau0, dau1)){
      recTrackIsoLead->Fill(dau0->trackIso());
      recEcalIsoLead->Fill(dau0->ecalIso());
      recHcalIsoLead->Fill(dau0->hcalIso());
      recRelIsoLead->Fill( (dau0->trackIso() + dau0->ecalIso() + dau0->hcalIso()) / dau0->pt());
      recTrackIsoSec->Fill(dau1->trackIso());
      recEcalIsoSec->Fill(dau1->ecalIso());
      recHcalIsoSec->Fill(dau1->hcalIso());
      recRelIsoSec->Fill( (dau1->trackIso() + dau1->ecalIso() + dau1->hcalIso()) / dau1->pt());
   }


}

void RecoMuon::finalize(){
   _histoVector.insert(_histoVector.end(), recJetPtVsInclMulti.begin(), recJetPtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recJetEtaVsInclMulti.begin(), recJetEtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recZPtVsInclMulti.begin(), recZPtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recZEtaVsInclMulti.begin(), recZEtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu1PtVsInclMulti.begin(), recMu1PtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu1EtaVsInclMulti.begin(), recMu1EtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu2PtVsInclMulti.begin(), recMu2PtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu2EtaVsInclMulti.begin(), recMu2EtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recZPtVsExclMulti.begin(), recZPtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recZEtaVsExclMulti.begin(), recZEtaVsExclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu1PtVsExclMulti.begin(), recMu1PtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu1EtaVsExclMulti.begin(), recMu1EtaVsExclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu2PtVsExclMulti.begin(), recMu2PtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu2EtaVsExclMulti.begin(), recMu2EtaVsExclMulti.end()); 

   std::vector<TH1D*>::const_iterator ibeg = _histoVector.begin();
   std::vector<TH1D*>::const_iterator iend = _histoVector.end();
   //use the errors as they were from real data 
   for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
     if (*i) //(*i)->Scale(_norm);
      (*i)->Sumw2();
   }

   
  _file->Write();
  //_file->Close();
}
