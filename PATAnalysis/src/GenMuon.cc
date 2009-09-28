
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
_file(proofFile)
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
   genEtaZ  = new TH1D("genEtaZ", "Generated Z #eta", 100, -10, 10);
   genMulti = new TH1D("genMulti", "Generated jet multiplicity", 10, -0.5, 9.5);
   genMassZ = new TH1D("genMassZ", "Generated Z mass", 200, 50, 150);
   genLeadMuPt = new TH1D("genLeadMuPt", "Generated Leading #mu p_{T}", 200, 0, 200);
   genSecMuPt  = new TH1D("genSecMuPt", "Generated Second #mu p_{T}", 200, 0, 200);
   genLeadMuEta = new TH1D("genLeadMuEta", "Generated Leading #mu #eta", 50, -2.5, 2.5);
   genSecMuEta  = new TH1D("genSecMuEta", "Generated Second #mu #eta", 50, 2.5, 2.5);
   genLeadJetPt = new TH1D("genLeadJetPt", "Generated Leading Jet p_{T}", 200, 0, 200);
   genLeadJetEta = new TH1D("genLeadJetEta", "Generated Leading Jet #eta", 100, -5, 5);
   genDeltayJfwdJbwd = new TH1D("genDeltayJfwdJbwd", "Generated #Deltay fw jet - bwd jet", 100, 0, 10);
   cout << "GenMuon Worker built." << endl;   
}

GenMuon::~GenMuon(){
  _file->ls();
  //_file->Write();
  //_file->Close();
}

void  GenMuon::process(const fwlite::ChainEvent& iEvent)
{

   double weight = 1.;

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zHandle;
   zHandle.getByLabel(iEvent, "zmumugen");

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

      std::vector<const reco::GenJet*> recjets = GetJets<reco::GenJet>(*jetHandle, _ptjetmin, _etajetmax);   

      genMulti->Fill(recjets.size(), weight);

      if (recjets.size()){
        genLeadJetPt->Fill(recjets[0]->pt(), weight);
        genLeadJetEta->Fill(recjets[0]->eta(), weight);
      }

      if (recjets.size()>1){
         genDeltayJfwdJbwd->Fill(deltaYFwdBwd<reco::GenJet>(recjets), weight);
      }

   } 
   
}
