
// -*- C++ -*-
//
// Package:    TTbarSelection
// Class:      TTbarSelection
// 
/**\class TTbarSelection TTbarSelection.cc BtagHLT/TTbarSelection/src/TTbarSelection.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Antonio Tropiano,40 3-B32,+41227671551,
//         Created:  Wed Oct 27 11:10:54 CEST 2010
// $Id$
//
//


// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"
#include "TFile.h"

//pat objects
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/MuonReco/interface/Muon.h" 
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/VertexReco/interface/Vertex.h" 
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h" 

#include "TH1.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include <vector>


// class declaration
//

class TTbarSelection : public edm::EDAnalyzer {
   public:
      explicit TTbarSelection(const edm::ParameterSet&);
      ~TTbarSelection();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
  std::map<std::string,TH1F*> histContainer_;
  //std::map<std::string,TH2F*> histContainer2d_;

  //input tags
  //edm::InputTag photonSrc_;
  
  //edm::InputTag triggerName_;
  edm::InputTag elecSrc_;
  edm::InputTag muonSrc_;
  edm::InputTag jetSrc_;
  edm::InputTag metSrc_;
  edm::InputTag triggerSrc_;
  std::string triggerName_;
  edm::InputTag vertexSrc_;

  float electronveto_ptcut_;
  float electronveto_etacut_;
  float relIso_;
  float muonveto_ptcut_;
  float muonveto_etacut_;
  float jets_ptcut_;
  float jets_etacut_;
  float discr_cut1_;
  float discr_cut2_; 
  std::string btag_algo1_;
  std::string btag_algo2_; 

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TTbarSelection::TTbarSelection(const edm::ParameterSet& iConfig):
  triggerSrc_(iConfig.getUntrackedParameter<edm::InputTag>("triggerSrc" )),
  triggerName_(iConfig.getUntrackedParameter<std::string>("triggerName")),
  elecSrc_(iConfig.getUntrackedParameter<edm::InputTag>("electronSrc" )),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" )),
  vertexSrc_(iConfig.getUntrackedParameter<edm::InputTag>("vertexSrc" )),
  electronveto_ptcut_(iConfig.getParameter<double>("ElectronVeto_PtCut")),
  electronveto_etacut_(iConfig.getParameter<double>("ElectronVeto_EtaCut")),
  muonveto_ptcut_(iConfig.getParameter<double>("MuonVeto_PtCut")),
  muonveto_etacut_(iConfig.getParameter<double>("MuonVeto_EtaCut")),
  jets_ptcut_(iConfig.getParameter<double>("Jets_PtCut")),
  jets_etacut_(iConfig.getParameter<double>("Jets_EtaCut")),
  discr_cut1_(iConfig.getParameter<double>("BtagDiscrCut1")),
  discr_cut2_(iConfig.getParameter<double>("BtagDiscrCut2")) 
  //btag_algo1_(iConfig.getParameter<std::string>("BtagAlgo1")),
  //btag_algo2_(iConfig.getParameter<std::string>("BtagAlgo2"))
			      //_(iConfig.getUntrackedParameter<std::string>("triggerName"))
  
{
  //now do what ever initialization is needed
  
}


TTbarSelection::~TTbarSelection()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TTbarSelection::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)

{
  using namespace edm;
  using namespace std;
  // get electron collection
  Handle<View<reco::GsfElectron> > electronsHandle;
  iEvent.getByLabel(elecSrc_,electronsHandle);
  View<reco::GsfElectron> electrons = *electronsHandle;
  
  // get muon collection
  Handle<View<reco::Muon> > muonsHandle;
  iEvent.getByLabel(muonSrc_,muonsHandle);
  View<reco::Muon> muons = *muonsHandle;

  // get jet collection
  Handle<View<reco::PFJet> > jetsHandle;
  iEvent.getByLabel(jetSrc_,jetsHandle);
  View<reco::PFJet> jets = *jetsHandle;

  // get vertex
  Handle<View<reco::Vertex> > vertexHandle;
  iEvent.getByLabel(vertexSrc_,vertexHandle);
  View<reco::Vertex> vertex = *vertexHandle;
  
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("trackCountingHighEffBJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  //trigger
  //edm::Handle<hlt::TriggerEvent> triggerHandle;
  //iEvent.getByLabel(triggerSrc_, triggerHandle);
  //hlt::TriggerEvent trigger = *triggerHandle;

  vector<reco::Muon> selec_muons;
  vector<reco::Electron> selec_electrons;
  //vector<reco::Jet> selec_jets_muons;
  //vector<reco::Jet> selec_jets_electrons;
  vector<reco::JetTag> all_jets;
  vector<reco::JetTag> selec_jets;
  bool leptonveto = false;
  /*if (trigger.path(triggerName_)->wasAccept())
  {
  histContainer_["trigger"]->Fill(1);*/
    //vertex requirement
  if(vertex.size()<1) return;  
  reco::Vertex const & pv = vertex.at(0);
  if((!(pv.isFake())) 
     && (pv.ndof()>4)
     && (fabs(pv.z())<15) 
     && (fabs(pv.position().Rho())<2.0))
    {
      histContainer_["vertex"]->Fill(1);
      if (leptonveto){
      for(edm::View<reco::GsfElectron>::const_iterator ielectron=electrons.begin(); ielectron!=electrons.end(); ++ielectron)
	{
	  if((ielectron->p4().Pt()>electronveto_ptcut_)                                             
	     && (fabs(ielectron->eta())<electronveto_etacut_)
	     //&& (((ielectron->dr03TkSumPt()+ielectron->dr03EcalRecHitSumEt()+ielectron->dr03HcalTowerSumEt())/ielectron->p4().Pt())<relIso_) //isolation 
	     )
	    return;
	}		
      for(edm::View<reco::Muon>::const_iterator imuon=muons.begin(); imuon!=muons.end(); ++imuon)
	{
	  if ((imuon->pt()>muonveto_ptcut_)
	      &&(fabs(imuon->eta())<muonveto_etacut_)
	      //&&(((imuon->hcalIso()+imuon->ecalIso()+imuon->trackIso())/imuon->pt())<relIso_) )
	      )
	    return;
	}
      }
      histContainer_["lepton_veto"]->Fill(1);
      
      //for(edm::View<reco::Jet>::const_iterator ijet=jets.begin(); ijet!=jets.end(); ++ijet)
      for (unsigned int i = 0; i != bTags.size(); ++i) {
	//cout<<" Jet "<< i 
	//  <<" has b tag discriminator = "<<bTags[i].second
	//  << " and jet Pt = "<<bTags[i].first->pt()<<endl;
	
	if(bTags[i].first->pt()>jets_ptcut_                           //pt cut
	   && fabs(bTags[i].first->eta())<jets_etacut_                //eta cut
	   )
	  //{
	  //if(ijet->pt()>jets_ptcut_ 
	  // && fabs(ijet->eta())<jets_etacut_)
	  selec_jets.push_back(bTags[i]);
	//histContainer_["deltaR"]->Fill(deltaR);
	//  } 
	//}
      }
      int btagged1=0;
      if(selec_jets.size()>5){
	histContainer_["6jets"]->Fill(1);
	for(std::vector<reco::JetTag>::iterator iseljets = selec_jets.begin();iseljets != selec_jets.end();++iseljets)
	  {
	    if(iseljets->second > discr_cut1_)
	      btagged1++;
	  }
	if(btagged1==1) histContainer_["6jets_1btag"]->Fill(1);
	else if (btagged1>1) {
	   histContainer_["6jets_1btag"]->Fill(1);
	   histContainer_["6jets_2btag"]->Fill(1);
	  }
      }
	  
      /*int btagged2=0;
	for(std::vector<reco::JetTag>::iterator iseljets = selec_jets.begin();iseljets != selec_jets.end();++iseljets)
	  {
	    if(iseljets->second > discr_cut2_)
	      btagged2++;
	    histContainer_["6jets_2btag"]->Fill(1);
	    }
	  }*/
    }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
      Handle<ExampleData> pIn;
      iEvent.getByLabel("example",pIn);
#endif
      
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
      ESHandle<SetupData> pSetup;
      iSetup.get<SetupRecord>().get(pSetup);
#endif
      
}

  
  // ------------ method called once each job just before starting event loop  ------------
void 
TTbarSelection::beginJob()
{
  //register to the TFile Service
  edm::Service<TFileService> fs;  
  histContainer_["trigger"]=fs->make<TH1F>("trigger", "trigger cut",               2, 0, 2);
  histContainer_["vertex"]=fs->make<TH1F>("vertex",   "vertex cut",                2, 0, 2);
  histContainer_["lepton_veto"]=fs->make<TH1F>("lepton_veto",   "lepton veto cut", 2, 0, 2);
  histContainer_["6jets"]=fs->make<TH1F>("6jets",    "6 jets cut",                 2, 0, 2);
  histContainer_["6jets_1btag"]=fs->make<TH1F>("6jets1btag", "6 jets 1 btag cut",  2, 0, 2);
  histContainer_["6jets_2btag"]=fs->make<TH1F>("6jets2btag", "6 jets 2 btag cut",  2, 0, 2);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TTbarSelection::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TTbarSelection);
