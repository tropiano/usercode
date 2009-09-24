
#include "Firenze/PATAnalysis/include/BackgroundWorkerMuon.h"
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

using namespace std;
using namespace edm;


BackgroundWorkerMuon::BackgroundWorkerMuon(const TList* in , TList& out): 
recPtZ(0), recEtaZ(0), recMulti(0), recMassZ(0), recTrackIsoLead(0), recEcalIsoLead(0), recHcalIsoLead(0), recRelIsoLead(0),
recTrackIsoSec(0), recEcalIsoSec(0), recHcalIsoSec(0), recRelIsoSec(0),
recLeadMuPt(0), recSecMuPt(0), recLeadMuEta(0), recSecMuEta(0),
recLeadJetPt(0), recLeadJetEta(0)
//_efficiency(out, "EfficienciesTotalVsRecMulti", 10, -0.5, 9.5, 0.3, false),
{       
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
/*  delete recPtZ;
  delete recEtaZ;
  delete recMulti;
  delete recMassZ; 
  delete recTrackIsoLead; 
  delete recEcalIsoLead;  
  delete recHcalIsoLead;  
  delete recRelIsoLead ;  
  delete recTrackIsoSec;
  delete recEcalIsoSec;
  delete recHcalIsoSec;
  delete recRelIsoSec ;
  delete recLeadMuPt; 
  delete recSecMuPt;  
  delete recLeadMuEta; 
  delete recSecMuEta;  
  delete recLeadJetPt; 
  delete recLeadJetEta;
*/
  /*destroyHistosVector(recJetPtVsInclMulti);
  destroyHistosVector(recJetEtaVsInclMulti);
  destroyHistosVector(recZPtVsInclMulti);
  destroyHistosVector(recZEtaVsInclMulti);
  destroyHistosVector(recZPtVsExclMulti);
  destroyHistosVector(recZEtaVsExclMulti);
  destroyHistosVector(recMu1PtVsExclMulti);
  destroyHistosVector(recMu1EtaVsExclMulti);
  destroyHistosVector(recMu2PtVsExclMulti);
  destroyHistosVector(recMu2EtaVsExclMulti);*/
}

void  BackgroundWorkerMuon::process(const edm::Event& iEvent)
{
   //cout << "fChain "<< fChain << " with " << fChain->GetEntriesFast() << " entries" << endl;

   //if (!(entry%100000)) cout << ">>>>>>>>Processing Event " << entry << endl;

   //cout << "Processing event" << endl;

   edm::Handle<std::vector<pat::Muon> > muonHandle;
   iEvent.getByLabel("selectedMuons", muonHandle);

   //cout << "collection taken" << endl;

   if (muonHandle->size()){
    recLeadMuPt->Fill((*muonHandle)[0].pt()); 
   }
/*   
   if (MuREC->size() > 0){
      //_efficiency.init(*MuREC, *ZREC, *ZRECDauMuon, *triggersREC);

      //_efficiency.fill(jetREC->size(), norm);
   }   

   //if (RecSelected(ZREC, ZRECDauMuon, _isocut)){
   if (RecSelectedWithTrigger(ZREC, ZRECDauMuon, triggersREC, _isocut)){
      recPtZ->Fill((*ZREC)[0]._pt);
      recEtaZ->Fill((*ZREC)[0]._eta);
      recMassZ->Fill((*ZREC)[0]._mass);
      recLeadMuPt->Fill((*ZRECDauMuon)[0]._pt);
      recLeadMuEta->Fill((*ZRECDauMuon)[0]._eta);
      recSecMuPt->Fill((*ZRECDauMuon)[1]._pt);
      recSecMuEta->Fill((*ZRECDauMuon)[1]._eta);
      //std::vector<PhysVarTreeJet> selectedjets = GetJets(jetREC, _ptjetmin, _etajetmax);
      std::vector<PhysVarTreeJet> recjets = GetJets(jetREC, _ptjetmin, _etajetmax);
      addHistosVsMulti(recjets.size(), "recJetPtIncl", " reconstructed jet p_{T} spectrum", 200, 0, 200, recJetPtVsInclMulti);
      addHistosVsMulti(recjets.size(), "recJetEtaIncl", " reconstructed jet #eta spectrum", 100, -5., 5., recJetEtaVsInclMulti);
      addHistosVsMulti(recjets.size(), "recZPtIncl", " reconstructed Z p_{T} spectrum", 200., 0., 200., recZPtVsInclMulti);
      addHistosVsMulti(recjets.size(), "recZEtaIncl", " reconstructed Z #eta spectrum", 100, -5., 5., recZEtaVsInclMulti);
      addHistosVsMulti(recjets.size(), "recZPtExcl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsExclMulti);
      addHistosVsMulti(recjets.size(), "recZEtaExcl", " reconstructed Z #eta spectrum", 100, -5., 5., recZEtaVsExclMulti);
      addHistosVsMulti(recjets.size(), "recZPtExcl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsExclMulti);
      addHistosVsMulti(recjets.size(), "recMu1PtExcl", " reconstructed lead #mu p_{T} spectrum", 200, 0., 200., recMu1PtVsExclMulti);
      addHistosVsMulti(recjets.size(), "recMu1EtaExcl", " reconstructed lead #mu #eta spectrum", 100, -5., 5., recMu1EtaVsExclMulti);
      addHistosVsMulti(recjets.size(), "recMu2PtExcl", " reconstructed sec #mu p_{T} spectrum", 200, 0., 200., recMu2PtVsExclMulti);
      addHistosVsMulti(recjets.size(), "recMu2EtaExcl", " reconstructed sec #mu #eta spectrum", 100, -5., 5., recMu2EtaVsExclMulti);
      recMulti->Fill(recjets.size());

      for (int i = 0; i < recjets.size()+1; ++i){
        recZPtVsInclMulti[i]->Fill((*ZREC)[0]._pt);
        recZEtaVsInclMulti[i]->Fill((*ZREC)[0]._eta);
      }

      //fill exclusive histograms
      recZPtVsExclMulti[recjets.size()]->Fill((*ZREC)[0]._pt);
      recZEtaVsExclMulti[recjets.size()]->Fill((*ZREC)[0]._eta);
      recMu1PtVsExclMulti[recjets.size()]->Fill((*ZRECDauMuon)[0]._pt);
      recMu1EtaVsExclMulti[recjets.size()]->Fill((*ZRECDauMuon)[0]._eta);
      recMu2PtVsExclMulti[recjets.size()]->Fill((*ZRECDauMuon)[1]._pt);
      recMu2EtaVsExclMulti[recjets.size()]->Fill((*ZRECDauMuon)[1]._eta);

      if (recjets.size()){
        recLeadJetPt->Fill(recjets[0]._pt);
        recLeadJetEta->Fill(recjets[0]._eta);
        for (int i = 0; i < recjets.size(); ++i){
          recJetPtVsInclMulti[i+1]->Fill(recjets[i]._pt);
          recJetEtaVsInclMulti[i+1]->Fill(recjets[i]._eta);
        }
      }
   }

   if (RecSelectedNoIso(ZREC, ZRECDauMuon)){
      recTrackIsoLead->Fill((*ZRECDauMuon)[0]._trackIso);
      recEcalIsoLead->Fill((*ZRECDauMuon)[0]._ecalIso);
      recHcalIsoLead->Fill((*ZRECDauMuon)[0]._hcalIso);
      recRelIsoLead->Fill(( (*ZRECDauMuon)[0]._trackIso+(*ZRECDauMuon)[0]._ecalIso+(*ZRECDauMuon)[0]._hcalIso )/(*ZRECDauMuon)[0]._pt);
      recTrackIsoSec->Fill((*ZRECDauMuon)[1]._trackIso);
      recEcalIsoSec->Fill((*ZRECDauMuon)[1]._ecalIso);
      recHcalIsoSec->Fill((*ZRECDauMuon)[1]._hcalIso);
      recRelIsoSec->Fill(( (*ZRECDauMuon)[1]._trackIso+(*ZRECDauMuon)[1]._ecalIso+(*ZRECDauMuon)[1]._hcalIso )/(*ZRECDauMuon)[1]._pt);
   }


   return kTRUE;*/
}
