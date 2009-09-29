
#include "Firenze/PATAnalysis/include/GenMuon.h"
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
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"



using namespace std;
using namespace edm;


GenMuon::GenMuon(TFile* proofFile, const TList* fInput):  
genPtZ(0), genEtaZ(0), genMulti(0), genMassZ(0), genLeadMuPt(0), genSecMuPt(0), genLeadMuEta(0), genSecMuEta(0),
genLeadJetPt(0), genLeadJetEta(0), genDeltayJfwdJbwd(0),
//genDjr0(0), genDjr1(0), genDjr2(0),
_ptjetmin(30.), _etajetmax(3.), _norm(1.),
_file(proofFile),  _histovector()
//_efficiency(out, "EfficienciesTotalVsRecMulti", 10, -0.5, 9.5, 0.3, false),
{       
   TIter next(fInput);
   bool factorSet = false;  
   while (TObject* obj = next()){
    const TParameter<double>* parDouble = dynamic_cast<const TParameter<double>* >(obj);
    if (parDouble){
      if (!strcmp(parDouble->GetName(), "MinPtJet")){
        _ptjetmin = parDouble->GetVal();
        cout << "set minimum pt for jets to: " << _ptjetmin << endl;
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
   }

   //_file = proofFile->OpenFile("UPDATE");
   cout << "GenMuon file name : " << _file->GetName() << endl;
   _file->cd();
   genPtZ   = new TH1D("genPtZ", "Generated Z p_{T}", 200, 0, 200);
   _histovector.push_back(genPtZ);
   genEtaZ  = new TH1D("genEtaZ", "Generated Z #eta", 100, -10, 10);
   _histovector.push_back(genEtaZ);
   genMulti = new TH1D("genMulti", "Generated jet multiplicity", 10, -0.5, 9.5);
   _histovector.push_back(genMulti);
   genMassZ = new TH1D("genMassZ", "Generated Z mass", 200, 50, 150);
   _histovector.push_back(genMassZ);
   genLeadMuPt = new TH1D("genLeadMuPt", "Generated Leading #mu p_{T}", 200, 0, 200);
   _histovector.push_back(genLeadMuPt);
   genSecMuPt  = new TH1D("genSecMuPt", "Generated Second #mu p_{T}", 200, 0, 200);
   _histovector.push_back(genSecMuPt);
   genLeadMuEta = new TH1D("genLeadMuEta", "Generated Leading #mu #eta", 50, -2.5, 2.5);
   _histovector.push_back(genLeadMuEta);
   genSecMuEta  = new TH1D("genSecMuEta", "Generated Second #mu #eta", 50, 2.5, 2.5);
   _histovector.push_back(genSecMuEta);
   genLeadJetPt = new TH1D("genLeadJetPt", "Generated Leading Jet p_{T}", 200, 0, 200);
   _histovector.push_back(genLeadJetPt);
   genLeadJetEta = new TH1D("genLeadJetEta", "Generated Leading Jet #eta", 100, -5, 5);
   _histovector.push_back(genLeadJetEta);
   genDeltayJfwdJbwd = new TH1D("genDeltayJfwdJbwd", "Generated #Deltay fw jet - bwd jet", 100, 0, 10);
   _histovector.push_back(genDeltayJfwdJbwd);
   std::vector<TH1D*>::const_iterator ibeg = _histovector.begin();
   std::vector<TH1D*>::const_iterator iend = _histovector.end();
   for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
    (*i)->Sumw2();
   }

   cout << "GenMuon Worker built." << endl;   
}

GenMuon::~GenMuon(){
  _file->ls();
  //_file->Write();
  //_file->Close();
}

void  GenMuon::process(const fwlite::ChainEvent& iEvent)
{

   _file->cd();

   double weight = 1.;

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zHandle;
   //zHandle.getByLabel(iEvent, "zmumugen");
   zHandle.getByLabel(iEvent, "zmumugenfull");

   fwlite::Handle<std::vector<reco::GenJet> > jetHandle;
   jetHandle.getByLabel(iEvent, "selectedGenJets");

   //cout << "collection taken" << endl;

   /*if (muonHandle->size()){
      recLeadMuPt->Fill((*muonHandle)[0].pt());
      
   }*/
   //we need to add the piece of code that select the Z candidate in case of multiple candidates
   if (zHandle->size() > 1) return; 


   if (GenSelected(*zHandle)){
      genPtZ->Fill((*zHandle)[0].pt(), weight);
      genEtaZ->Fill((*zHandle)[0].eta(), weight);
      genMassZ->Fill((*zHandle)[0].mass(), weight);
      double leadmupt, leadmueta, secondmupt, secondmueta;
      double pt0  = (*zHandle)[0].daughter(0)->pt(); 
      double pt1  = (*zHandle)[0].daughter(1)->pt();
      double eta0 = (*zHandle)[0].daughter(0)->eta();
      double eta1 = (*zHandle)[0].daughter(1)->eta();
      
      if (pt0 > pt1) {
        leadmupt    = pt0;
        leadmueta   = eta0;
        secondmupt  = pt1;
        secondmueta = eta1;
      } else {
        leadmupt    = pt1;
        leadmueta   = eta1;
        secondmupt  = pt0;
        secondmueta = eta0;
      }
      genLeadMuPt->Fill(leadmupt, weight);
      genLeadMuEta->Fill(leadmueta, weight);
      genSecMuPt->Fill(secondmupt, weight);
      genSecMuEta->Fill(secondmueta, weight);  

      std::vector<const reco::GenJet*> genjets = GetJets<reco::GenJet>(*jetHandle, _ptjetmin, _etajetmax);   

      genMulti->Fill(genjets.size(), weight);
      addHistosVsMulti(genjets.size(), "genJetPtIncl", " generated jet p_{T} spectrum", 200, 0, 200, genJetPtVsInclMulti);
      addHistosVsMulti(genjets.size(), "genJetEtaIncl", " generated jet #eta spectrum", 100, -5., 5., genJetEtaVsInclMulti);
      addHistosVsMulti(genjets.size(), "genZPtIncl", " generated Z p_{T} spectrum", 200, 0., 200., genZPtVsInclMulti);
      addHistosVsMulti(genjets.size(), "genZEtaIncl", " generated Z #eta spectrum", 100, -5., 5., genZEtaVsInclMulti);
      addHistosVsMulti(genjets.size(), "genZPtExcl", " generated Z p_{T} spectrum", 200, 0., 200., genZPtVsExclMulti);
      addHistosVsMulti(genjets.size(), "genZEtaExcl", " generated Z #eta spectrum", 100, -5., 5., genZEtaVsExclMulti);
      addHistosVsMulti(genjets.size(), "genMu1PtExcl", " generated lead #mu p_{T} spectrum", 200, 0., 200., genMu1PtVsExclMulti);
      addHistosVsMulti(genjets.size(), "genMu1EtaExcl", " generated lead #mu #eta spectrum", 100, -5., 5., genMu1EtaVsExclMulti);
      addHistosVsMulti(genjets.size(), "genMu2PtExcl", " generated sec #mu p_{T} spectrum", 200, 0., 200., genMu2PtVsExclMulti);
      addHistosVsMulti(genjets.size(), "genMu2EtaExcl", " generated sec #mu #eta spectrum", 100, -5., 5., genMu2EtaVsExclMulti);

      if (genjets.size()){
        genLeadJetPt->Fill(genjets[0]->pt(), weight);
        genLeadJetEta->Fill(genjets[0]->eta(), weight);
        for (unsigned int i = 0; i < genjets.size(); ++i){
          genJetPtVsInclMulti[i+1]->Fill(genjets[i]->pt(), weight);
          genJetEtaVsInclMulti[i+1]->Fill(genjets[i]->eta(), weight);
        }
      }

      //fill inclusive quantities
      for (unsigned int i = 0; i < genjets.size()+1; ++i){
        genZPtVsInclMulti[i]->Fill((*zHandle)[0].pt(), weight);
        genZEtaVsInclMulti[i]->Fill((*zHandle)[0].eta(), weight);
      }

      //fill exclusive quantities
      genZPtVsExclMulti[genjets.size()]->Fill((*zHandle)[0].pt(), weight);
      genZEtaVsExclMulti[genjets.size()]->Fill((*zHandle)[0].eta(), weight);
      genMu1PtVsExclMulti[genjets.size()]->Fill(leadmupt, weight);
      genMu1EtaVsExclMulti[genjets.size()]->Fill(leadmueta, weight);
      genMu2PtVsExclMulti[genjets.size()]->Fill(secondmupt, weight);
      genMu2EtaVsExclMulti[genjets.size()]->Fill(secondmueta, weight);


      if (genjets.size()>1){
         genDeltayJfwdJbwd->Fill(deltaYFwdBwd<reco::GenJet>(genjets), weight);
      }

   } 
   
}

void GenMuon::finalize(){
    
   _histovector.insert(_histovector.end(), genJetPtVsInclMulti.begin(), genJetPtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genJetEtaVsInclMulti.begin(), genJetEtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genZPtVsInclMulti.begin(), genZPtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genZEtaVsInclMulti.begin(), genZEtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genMu1PtVsInclMulti.begin(), genMu1PtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genMu1EtaVsInclMulti.begin(), genMu1EtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genMu2PtVsInclMulti.begin(), genMu2PtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genMu2EtaVsInclMulti.begin(), genMu2EtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genZPtVsExclMulti.begin(), genZPtVsExclMulti.end());
   _histovector.insert(_histovector.end(), genZEtaVsExclMulti.begin(), genZEtaVsExclMulti.end());

   _histovector.insert(_histovector.end(), genMu1PtVsExclMulti.begin(), genMu1PtVsExclMulti.end());
   _histovector.insert(_histovector.end(), genMu1EtaVsExclMulti.begin(), genMu1EtaVsExclMulti.end());

   _histovector.insert(_histovector.end(), genMu2PtVsExclMulti.begin(), genMu2PtVsExclMulti.end());
   _histovector.insert(_histovector.end(), genMu2EtaVsExclMulti.begin(), genMu2EtaVsExclMulti.end());
    
   std::vector<TH1D*>::const_iterator ibeg = _histovector.begin();
   std::vector<TH1D*>::const_iterator iend = _histovector.end();
   for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
      //if (*i) cout << typeid(**i).name() << endl;      
     if (*i) (*i)->Scale(_norm);
   }
   
   _file->Write();
   //_file->Close();
}
