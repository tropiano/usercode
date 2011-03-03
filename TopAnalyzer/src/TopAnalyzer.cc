// -*- C++ -*-
//
// Package:    TopAnalyzer
// Class:      TopAnalyzer
// 
/**\class TopAnalyzer TopAnalyzer.cc TopBTag/TopAnalyzer/src/TopAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Obluraski
//         Created:  Fri Jul  3 15:48:26 CEST 2009
// $Id$
//
//

// system include files
#include <memory>
#include <map>
#include <string>

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
#include "PhysicsTools/CandUtils/interface/EventShapeVariables.h"
#include "TFile.h"

//pat objects
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"

#include "TH1.h"
#include "TH1F.h"
#include "TH2F.h"

#include "TH1D.h"
#include "TH2F.h"
#include <vector>

//
// class decleration
//

class TopAnalyzer : public edm::EDAnalyzer {
public:
  explicit TopAnalyzer(const edm::ParameterSet&);
  ~TopAnalyzer();
  
  
private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();
  
  // ----------member data ---------------------------
  //TH1F * z_mass;
  //canvas and histograms
  //TCanvas *canvas;
  std::map<std::string,TH1F*> histContainer_;
  std::map<std::string,TH2F*> histContainer2d_;

  //input tags
  //edm::InputTag photonSrc_;
  
  //edm::InputTag triggerName_;
  edm::InputTag elecSrc_;
  edm::InputTag muonSrc_;
  edm::InputTag jetSrc_;
  edm::InputTag metSrc_;
  edm::InputTag triggerSrc_;
  std::vector<std::string> triggerName_;
  edm::InputTag vertexSrc_;
  //kinematic cuts
  
  float electronveto_ptcut_;
  float electronveto_etacut_;
  float muonveto_ptcut_;
  float muonveto_etacut_;
  float jets_ptcut_;
  float jets_etacut_;
  float jets_emfraccut_;
  float discr_cut1_;
  float discr_cut2_; 
  float discr_cut3_;
  float discr_cut4_; 
  std::string btag_algo1_;
  std::string btag_algo2_; 
  float met_cut_;
  //float mz_sup_;
  //float mz_inf_;
  float relIso_;
  //float d0Cut_;
  float jets_deltarcut_;
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

std::vector<math::XYZVector> makeVecForEventShape(std::vector<pat::Jet> jets, double scale = 1.) {
  std::vector<math::XYZVector> p;
  unsigned int i=1;
  for (std::vector<pat::Jet>::const_iterator jet = jets.begin(); jet != jets.end(); ++jet) {
    math::XYZVector Vjet(jet->px() * scale, jet->py() * scale, jet->pz() * scale);
    p.push_back(Vjet);
  }
  return p;
}

TopAnalyzer::TopAnalyzer(const edm::ParameterSet& iConfig):
  histContainer_(),
  histContainer2d_(),
  elecSrc_(iConfig.getUntrackedParameter<edm::InputTag>("electronSrc")),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" )),
  metSrc_(iConfig.getUntrackedParameter<edm::InputTag>("metSrc" )),
  triggerSrc_(iConfig.getUntrackedParameter<edm::InputTag>("triggerSrc" )),
  triggerName_(iConfig.getUntrackedParameter<std::vector<std::string> >("triggerName")),
  vertexSrc_(iConfig.getUntrackedParameter<edm::InputTag>("vertexSrc" )),
  electronveto_ptcut_(iConfig.getParameter<double>("ElectronVeto_PtCut")),
  electronveto_etacut_(iConfig.getParameter<double>("ElectronVeto_EtaCut")),
  muonveto_ptcut_(iConfig.getParameter<double>("MuonVeto_PtCut")),
  muonveto_etacut_(iConfig.getParameter<double>("MuonVeto_EtaCut")),
  jets_ptcut_(iConfig.getParameter<double>("Jets_PtCut")),
  jets_etacut_(iConfig.getParameter<double>("Jets_EtaCut")),
  //jets_deltarcut_(iConfig.getParameter<double>("Jets_DeltaRCut")),
  jets_emfraccut_(iConfig.getParameter<double>("Jets_EmFracCut")),
  discr_cut1_(iConfig.getParameter<double>("BtagDiscrCut1")),
  discr_cut2_(iConfig.getParameter<double>("BtagDiscrCut2")),
  discr_cut3_(iConfig.getParameter<double>("BtagDiscrCut3")),
  discr_cut4_(iConfig.getParameter<double>("BtagDiscrCut4")), 
  btag_algo1_(iConfig.getParameter<std::string>("BtagAlgo1")),
  btag_algo2_(iConfig.getParameter<std::string>("BtagAlgo2")),
  met_cut_(iConfig.getParameter<double>("MET_Cut")),
  //mz_sup_(iConfig.getParameter<double>("Mz_sup")),
  //mz_inf_(iConfig.getParameter<double>("Mz_inf")),
  relIso_(iConfig.getParameter<double>("RelIso"))
{
  //now do what ever initialization is needed

}

TopAnalyzer::~TopAnalyzer()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}
//
// member functions
//

/*bool eleId(edm::View<pat::Electron>::const_iterator ielectron, float hoe_b, float dphi_b, float deta_b, float sigma_b, float hoe_e, float dphi_e, float deta_e, float sigma_e)
{
  if(ielectron->isEB())
    {
      if((ielectron->hadronicOverEm()<hoe_b)
	 &&(ielectron->deltaPhiSuperClusterTrackAtVtx()<dphi_b)
	 &&(ielectron->deltaEtaSuperClusterTrackAtVtx()<deta_b)
	 &&(ielectron->sigmaIetaIeta()<sigma_b))
	return true;
      else return false;
    }
  else if(ielectron->isEE())
    {
	 if((ielectron->hadronicOverEm()<hoe_e)
	    && (ielectron->deltaPhiSuperClusterTrackAtVtx()<dphi_e)
	    && (ielectron->deltaEtaSuperClusterTrackAtVtx()<deta_e)
	    && (ielectron->sigmaIetaIeta()<sigma_e))
	   return true;
	 else return false;
       }
  else return false;
}
 
bool isEcalCrack(edm::View<pat::Electron>::const_iterator ielectron, float etamin, float etamax)
{
  if( (fabs(ielectron->superCluster()->eta())>etamin) && (fabs(ielectron->superCluster()->eta())<etamax) )
    return true;
  else return false;
}
*/

// ------------  method called to for each event  ------------
void
TopAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  // get electron collection
  Handle<edm::View<pat::Electron> > electronsHandle;
  iEvent.getByLabel(elecSrc_,electronsHandle);
  edm::View<pat::Electron> electrons = *electronsHandle;

  // get muon collection
  Handle<edm::View<pat::Muon> > muonsHandle;
  iEvent.getByLabel(muonSrc_,muonsHandle);
  edm::View<pat::Muon> muons = *muonsHandle;

  // get tau collection  
  //Handle<edm::View<pat::Tau> > tausHandle;
  //iEvent.getByLabel(tauSrc_,tausHandle);
  //edm::View<pat::Tau> taus = *tausHandle;

  // get jet collection
  Handle<edm::View<pat::Jet> > jetsHandle;
  iEvent.getByLabel(jetSrc_,jetsHandle);
  edm::View<pat::Jet> jets = *jetsHandle;

  // get vertex
  Handle<edm::View<reco::Vertex> > vertexHandle;
  iEvent.getByLabel(vertexSrc_,vertexHandle);
  edm::View<reco::Vertex> vertex = *vertexHandle;

  // get met collection  
  Handle<edm::View<pat::MET> > metHandle;
  iEvent.getByLabel(metSrc_,metHandle);
  edm::View<pat::MET> met = *metHandle;
  
  edm::Handle<pat::TriggerEvent> triggerHandle;
  iEvent.getByLabel(triggerSrc_, triggerHandle);

  //get trigger
  //Handle<edm::View<pat::TriggerPath> > triggerHandle;
  //iEvent.getByLabel(triggerSrc_,triggerHandle);
  pat::TriggerEvent trigger = *triggerHandle;

  
  std::vector<pat::Jet> all_jets;
  std::vector<pat::Jet> selec_jets;
  bool triggered=false;
  //std::vector<pat::Jet> selec_jets_electrons;
  //std::cout<<"trigger path "<<*(trigger.acceptedPaths().begin())<<std::endl;
  
  /*for(std::vector<pat::TriggerPath>::const_iterator ipath = trigger.paths()->begin(); ipath!=trigger.paths()->end(); ipath++)
    {
      cout<<"trigger path "<<ipath->name()<<endl;
      }*/
  
  //cout<<"trigger paths: "<<trigger.paths()<<endl;
  
  for(std::vector<std::string>::const_iterator iname = triggerName_.begin(); iname!=triggerName_.end(); iname++)
    {
      //cout << "trigger name " << *iname << endl;
      if(trigger.path(*iname) && trigger.path(*iname)->wasRun() && trigger.path(*iname)->wasAccept()){
	//if(trigger.path(*iname)->wasAccept()) 
	triggered=true;
      }  
    }
  if(triggered)
    {
      histContainer_["trigger"]->Fill(1);
      //vertex requirement
      if(vertex.size()<1) return;  
      reco::Vertex const & pv = vertex.at(0);
      if((!(pv.isFake())) 
	 && (pv.ndof()>4)
	 && (fabs(pv.z())<15) 
	 && (fabs(pv.position().Rho())<2.0))
	{
	  histContainer_["vertex"]->Fill(1);
	  
	  for(edm::View<pat::Jet>::const_iterator ijet=jets.begin(); ijet!=jets.end(); ++ijet)
	    {
	      //cout << "this jet has a discriminator of: " << ijet->bDiscriminator("trackCountingHighEffBJetTags") << endl; 
	      //all_jets.push_back(*ijet); 
	      //Double_t deltaR=reco::deltaR2(ijet->eta(),ijet->phi(),selec_electrons.front().eta(),selec_electrons.front().phi());
	      if(ijet->pt()>jets_ptcut_                           //pt cut
		 && fabs(ijet->eta())<jets_etacut_                //eta cut
		 //&& deltaR>jets_deltarcut_                      //electron rejection cut
		 //&& ijet->chargedEmEnergyFraction()>jets_emfraccut_      //em fraction
		 //&& ijet->jetID().n90Hits>1
		 //&& ijet->jetID().fHPD<0.98
		 )
		{
		  histContainer_["chargedMult"]->Fill(ijet->chargedMultiplicity());
		  histContainer_["chargedEmEnergy"]->Fill(ijet->chargedEmEnergy());
		  histContainer_["chargedHadronEnergy"]->Fill(ijet->chargedHadronEnergy());
		  histContainer_["chargedHadEnergyFrac"]->Fill(ijet->chargedHadronEnergyFraction());
		  //cout<<"neutral energy fraction: " << ijet->neutralHadronEnergyFraction()<<endl;
		  //cout<<"em energy fraction: " << 
		  histContainer_["chargedEmEnergyFrac"]->Fill(ijet->chargedEmEnergyFraction());
		  //cout<<"neutral em energy fraction: " << ijet->neutralEmEnergyFraction()<<endl;
		  selec_jets.push_back(*ijet);
		  //histContainer_["deltaR"]->Fill(deltaR);
		} 
	    }
	  bool vetoelectron = false;
	  bool vetomuon = false; 
	  
	  for(edm::View<pat::Electron>::const_iterator ielectron=electrons.begin(); ielectron!=electrons.end(); ++ielectron)
	    {
	      //int eid = (int) ielectron->electronID("eidRobustLoose"); 
	      //bool id1(eid & 0x1);
	      //bool id3(eid & 0x3);
	      if((ielectron->p4().Pt()>electronveto_ptcut_)                                             
		 && (fabs(ielectron->eta())<electronveto_etacut_)
		 //&& !(isEcalCrack(ielectron,1.4442,1.5660))
		 //&& eleId(ielectron,0.025,0.03,0.004,0.01,0.025,0.02,0.005,0.03)
		 && (((ielectron->dr03TkSumPt()+ielectron->dr03EcalRecHitSumEt()+ielectron->dr03HcalTowerSumEt())/ielectron->p4().Pt())<relIso_) //isolation
		 //&& (fabs(ielectron->dB())<d0Cut_) 
		 )
		vetoelectron=true;
	      
	      /*selec_electrons.push_back(*ielectron);
		else if(
		(ielectron->et()>20)                                              pt cut
		&& (fabs(ielectron->eta())<2.5)
		&& (((ielectron->dr03TkSumPt()+ielectron->dr03EcalRecHitSumEt()+ielectron->dr03HcalTowerSumEt())/ielectron->et())<0.1) //isolation
		&& (id1 || id3)
		)
		loose_electrons.push_back(*ielectron);*/
	    }
	  /*		histContainer_["selected_electrons"]->Fill(selec_electrons.size());
	  //exactly one electron
	  if(selec_electrons.size()==1) histContainer_["cut_3"]->Fill(1);
	  else return;
		//impact parameter                                                    
		if(selec_electrons.front().gsfTrack()->trackerExpectedHitsInner().numberOfHits()==0) histContainer_["cut_4"]->Fill(1);
		else return;*/
	  for(edm::View<pat::Muon>::const_iterator imuon=muons.begin(); imuon!=muons.end(); ++imuon)
	    {
	      if (//(imuon->isGood("AllGlobalMuons")>0)
		  (imuon->pt()>muonveto_ptcut_)
		  &&(fabs(imuon->eta())<muonveto_etacut_)
		  &&(((imuon->hcalIso()+imuon->ecalIso()+imuon->trackIso())/imuon->pt())<relIso_) )
		//((imuon->isolationR03()->sumPt()+imuon->isolationR03()->emEt()+imuon->isolationR03()->hadEt())/imuon->pt()<0.2))
		vetomuon=true;
	    }
	  
	  if(selec_jets.size()>5){
	    histContainer_["6jets"]->Fill(1);
	    if(vetoelectron || vetomuon)	  histContainer_["leptonveto"]->Fill(1);
	  }
	  
	  if(selec_jets.size()>5){
	    //kinematical selection
	    EventShapeVariables eventshape(makeVecForEventShape(selec_jets));
	    double aplanarity  = eventshape.aplanarity();
	    double sphericity  = eventshape.sphericity();
	    double circularity = eventshape.circularity();
	    double isotropy    = eventshape.isotropy();
	    double C           = eventshape.C();
	    double D           = eventshape.D();
	    double DeltaPhi    = reco::deltaPhi(selec_jets[0].phi(), selec_jets[1].phi());
	    double sumEt       = 0;
	    double sumE        = 0;
	    
	    for(std::vector<pat::Jet>::iterator iseljets = selec_jets.begin();iseljets != selec_jets.end();++iseljets)
	      {
		sumEt          += iseljets->et();
		sumE           += iseljets->energy();
	      }
	    double centrality  = sumEt/sumE;
	    double sumEt_3     = sumEt - selec_jets[0].et() - selec_jets[1].et();
	    
	    histContainer_["aplanarity"]->Fill(aplanarity);
	    histContainer_["circularity"]->Fill(circularity);
	    histContainer_["sphericity"]->Fill(sphericity);
	    histContainer_["isotropy"]->Fill(isotropy);
	    histContainer_["centrality"]->Fill(centrality);
	    histContainer_["C"]->Fill(C);
	    histContainer_["D"]->Fill(D);
	    histContainer_["sumEt"]->Fill(sumEt);
	    histContainer_["sumE"]->Fill(sumE);
	    histContainer_["DeltaPhi"]->Fill(DeltaPhi);
	    histContainer_["sumEt_3"]->Fill(sumEt_3);
	    
	    //b-tagging
	    int btaggedmedium1=0;
	    int btaggedloose1=0;
	    for(std::vector<pat::Jet>::iterator iseljets = selec_jets.begin();iseljets != selec_jets.end();++iseljets)
	      {
		histContainer_["HPdiscr"]->Fill(iseljets->bDiscriminator(btag_algo1_));
		if(iseljets->bDiscriminator(btag_algo1_) > discr_cut1_)
		  {
		    btaggedloose1++;
		    histContainer_["bJetPtHP"]->Fill(iseljets->pt());
		  }
		if(iseljets->bDiscriminator(btag_algo1_) > discr_cut2_)
		  {
		    btaggedmedium1++;
		  }
	      }
	    histContainer_["NbtagHP"]->Fill(btaggedmedium1);
	    
	    int btaggedmedium2=0;
	    int btaggedloose2=0;
	    for(std::vector<pat::Jet>::iterator iseljets = selec_jets.begin();iseljets != selec_jets.end();++iseljets)
	      {
		histContainer_["HEdiscr"]->Fill(iseljets->bDiscriminator(btag_algo2_));
		if(iseljets->bDiscriminator(btag_algo2_) > discr_cut3_){
		  btaggedloose2++;
		  histContainer_["bJetPtHE"]->Fill(iseljets->pt());
		}
		if(iseljets->bDiscriminator(btag_algo2_) > discr_cut4_){
		  btaggedmedium2++;
		}
		//cout<<"eta moment"<<endl;
		histContainer_["etamoment"]->Fill(iseljets->etaetaMoment());
		//cout<<"phi moment"<<endl;
		histContainer_["phimoment"]->Fill(iseljets->phiphiMoment());
		//cout<<"eta moment et"<<endl;
		histContainer2d_["etamoment_et"]->Fill(iseljets->pt(),iseljets->etaetaMoment());
		//cout<<"phi moment et"<<endl;
		histContainer2d_["phimoment_et"]->Fill(iseljets->pt(),iseljets->phiphiMoment());
		
	      }
	    histContainer_["NbtagHE"]->Fill(btaggedmedium2);
	    //histContainer_["jetpt"]->Fill(selec_jets.front().pt());
	    //histContainer_["jeteta"]->Fill(selec_jets.front().eta());
	    bool kin=false;
	    if (aplanarity>=0.09 && 
		sumEt>=280 &&
		sumEt_3>=150) kin=true;
	      
	    if(kin) histContainer_["kin"]->Fill(1); 
	    
	    if(btaggedloose1) histContainer_["1btag_tchp_loose"]->Fill(1);
	    if(btaggedloose1>1) histContainer_["2btags_tchp_loose"]->Fill(1);
	    if(btaggedmedium1) histContainer_["1btag_tchp_medium"]->Fill(1);
	    if(btaggedmedium1>1) histContainer_["2btags_tchp_medium"]->Fill(1);
	    
	    if(btaggedloose2) histContainer_["1btag_tche_loose"]->Fill(1);
	    if(btaggedloose2>1) histContainer_["2btags_tche_loose"]->Fill(1);
	    if(btaggedmedium2) histContainer_["1btag_tche_medium"]->Fill(1);
	    if(btaggedmedium2>1) histContainer_["2btags_tche_medium"]->Fill(1);
	    
	    
	    std::vector<pat::Jet>::iterator ijet = selec_jets.begin();
	    histContainer_["1stjetpt"]->Fill(ijet->pt());
	    histContainer_["1stjeteta"]->Fill(ijet->eta());
	    histContainer_["1stjetConst"]->Fill(ijet->associatedTracks().size());
	    
	    //if(ijet->associatedTracks().size())
	    //cout<<"tracce con dz rispetto al vertice: "<< ijet->associatedTracks()[0]->dz(pv.position()) <<endl;
	    
	    if(ijet!=selec_jets.end())
	      {
		ijet++;
		histContainer_["2ndjetpt"]->Fill(ijet->pt());
		histContainer_["2ndjeteta"]->Fill(ijet->eta());
		histContainer_["2ndjetConst"]->Fill(ijet->associatedTracks().size());
		if(ijet!=selec_jets.end())
		  {
		    ijet++;
		    histContainer_["3rdjetpt"]->Fill(ijet->pt());
		    histContainer_["3rdjeteta"]->Fill(ijet->eta());
		    histContainer_["3rdjetConst"]->Fill(ijet->associatedTracks().size());
		    if(ijet!=selec_jets.end())
		      {
			ijet++;
			histContainer_["4thjetpt"]->Fill(ijet->pt());
			histContainer_["4thjeteta"]->Fill(ijet->eta());
			histContainer_["4thjetConst"]->Fill(ijet->associatedTracks().size());
		      }
		    
		    if(ijet!=selec_jets.end())
		      {
			ijet++;
			histContainer_["5thjetpt"]->Fill(ijet->pt());
			histContainer_["5thjeteta"]->Fill(ijet->eta());
			histContainer_["5thjetConst"]->Fill(ijet->associatedTracks().size());
		      }
		    
		    if(ijet!=selec_jets.end())
		      {
			ijet++;
			histContainer_["6thjetpt"]->Fill(ijet->pt());
			histContainer_["6thjeteta"]->Fill(ijet->eta());
			histContainer_["6thjetConst"]->Fill(ijet->associatedTracks().size());
		      }
		    
		  }
	      }
	  }
	}
      histContainer_["selected_jets"]->Fill(selec_jets.size());
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
TopAnalyzer::beginJob()
{
  //register to the TFile Service
  edm::Service<TFileService> fs;
  
  //histContainer_["electronet"]=fs->make<TH1F>("electronet", "Et of electrons", 100, 0, 300);
  //histContainer_["elecs"]=fs->make<TH1F>("elecs",   "electron multiplicity", 10, 0,  10);
  //histContainer_["electroneta"]=fs->make<TH1F>("electroneta",   "Electron Eta", 100, -2.5, 2.5);
  histContainer_["jets"]=fs->make<TH1F>("jets",    "jet multiplicity",      10, 0,  10);
  histContainer_["1stjetpt"]=fs->make<TH1F>("1stjetpt", "1st jet Pt", 100, 0, 300);
  histContainer_["1stjeteta"]=fs->make<TH1F>("1stjeteta", "1st jet Eta", 100, -2.5, 2.5);
  histContainer_["1stjetConst"]=fs->make<TH1F>("1stjetConst", "1st jet number of tracks", 20, 0, 20);
  
  histContainer_["2ndjetpt"]=fs->make<TH1F>("2ndjetpt", "2nd jet Pt", 100, 0, 300);
  histContainer_["2ndjeteta"]=fs->make<TH1F>("2ndjeteta", "2nd jet Eta", 100, -2.5, 2.5);
  histContainer_["2ndjetConst"]=fs->make<TH1F>("2ndjetConst", "2nd jet number of tracks", 20, 0, 20);
  
  histContainer_["3rdjetpt"]=fs->make<TH1F>("3rdjetpt", "3rd jet Pt", 100, 0, 300);
  histContainer_["3rdjeteta"]=fs->make<TH1F>("3rdjeteta", "3rd jet Eta", 100, -2.5, 2.5);
  histContainer_["3rdjetConst"]=fs->make<TH1F>("3rdjetConst", "3rd jet number of tracks", 20, 0, 20);
  
  histContainer_["4thjetpt"]=fs->make<TH1F>("4thjetpt", "4th jet Pt", 100, 0, 300);
  histContainer_["4thjeteta"]=fs->make<TH1F>("4thjeteta", "4th jet Eta", 100, -2.5, 2.5);
  histContainer_["4thjetConst"]=fs->make<TH1F>("4thjetConst", "4th jet number of tracks", 20, 0, 20);
  
  histContainer_["5thjetpt"]=fs->make<TH1F>("5thjetpt", "5th jet Pt", 100, 0, 300);
  histContainer_["5thjeteta"]=fs->make<TH1F>("5thjeteta", "5th jet Eta", 100, -2.5, 2.5);
  histContainer_["5thjetConst"]=fs->make<TH1F>("5thjetConst", "5th jet number of tracks", 20, 0, 20);
   
  histContainer_["6thjetpt"]=fs->make<TH1F>("6thjetpt", "6th jet Pt", 100, 0, 300);
  histContainer_["6thjeteta"]=fs->make<TH1F>("6thjeteta", "6th jet Eta", 100, -2.5, 2.5);
  histContainer_["6thjetConst"]=fs->make<TH1F>("6thjetConst", "6th jet number of tracks", 20, 0, 20);
  
  histContainer_["selected_jets"]=fs->make<TH1F>("selected_jets", "selected jets multiplicity", 10, 0,  10);
  histContainer_["bJetPtHP"]=fs->make<TH1F>("bJetPtHP", "b jet Pt (high purity)", 100, 0, 300);
  histContainer_["bJetPtHE"]=fs->make<TH1F>("bJetPtHE", "b jet Pt (high efficiency)", 100, 0, 300);
 
  histContainer_["HPdiscr"]=fs->make<TH1F>("HPdiscr", "discriminator (high purity)", 100, -10, 10);
  histContainer_["HEdiscr"]=fs->make<TH1F>("HEdiscr", "discriminator (high efficiency)", 100, -10, 10);
 
  histContainer_["chargedMult"]=fs->make<TH1F>("chargedMult",     "charged multiplicity",         20, 0, 20);
  histContainer_["chargedEmEnergy"]=fs->make<TH1F>("chargedEmEnergy", "charged em energy", 300, 0,  300);
  histContainer_["chargedHadronEnergy"]=fs->make<TH1F>("chargedHadronEnergy", "charged had energy", 300, 0, 300);
  histContainer_["chargedHadEnergyFrac"]=fs->make<TH1F>("chargedHadEnergyFrac","charged had energy fraction",100, 0,  1);
  histContainer_["chargedEmEnergyFrac"]=fs->make<TH1F>("chargedEmEnergyFrac","charged em energy fraction",100, 0,  1);

  histContainer_["NbtagHP"]=fs->make<TH1F>("NbtagHP",     "no of btagged jets (high purity)", 5, 0, 5); 
  histContainer_["NbtagHE"]=fs->make<TH1F>("NbtagHE",     "no of btagged jets (high efficiency)", 5, 0, 5); 
 
  histContainer_["trigger"]=fs->make<TH1F>("trigger",     "Trigger", 10, 0, 10);
  histContainer_["6jets"]=fs->make<TH1F>("6jets",     "6 jets", 10, 0, 10);
  histContainer_["kin"]=fs->make<TH1F>("kin",   "kinematical cuts", 10, 0, 10);
  histContainer_["1btag_tche_medium"]=fs->make<TH1F>("1btag_tche_medium",   "1 btag TCHE medium", 10, 0, 10);
  histContainer_["2btags_tche_medium"]=fs->make<TH1F>("2btags_tche_medium",   "2 btags TCHE medium", 10, 0, 10);
  histContainer_["1btag_tchp_medium"]=fs->make<TH1F>("1btag_tchp_medium",   "1 btag TCHP medium", 10, 0, 10);
  histContainer_["2btags_tchp_medium"]=fs->make<TH1F>("2btags_tchp_medium",   "2 btags TCHP medium", 10, 0, 10);
  histContainer_["1btag_tche_loose"]=fs->make<TH1F>("1btag_tche_loose",   "1 btag TCHE loose", 10, 0, 10);
  histContainer_["2btags_tche_loose"]=fs->make<TH1F>("2btags_tche_loose",   "2 btags TCHE loose", 10, 0, 10);
  histContainer_["1btag_tchp_loose"]=fs->make<TH1F>("1btag_tchp_loose",   "1 btag TCHP loose", 10, 0, 10);
  histContainer_["2btags_tchp_loose"]=fs->make<TH1F>("2btags_tchp_loose",   "2 btags TCHP loose", 10, 0, 10);
  histContainer_["vertex"]=fs->make<TH1F>("vertex",    "Vertex", 10, 0, 10);
  histContainer_["leptonveto"]=fs->make<TH1F>("leptonveto",    "Lepton Veto", 10, 0, 10);

  histContainer_["aplanarity"]=fs->make<TH1F>("aplanarity",     "aplanarity", 100, 0, 0.5);
  histContainer_["sphericity"]=fs->make<TH1F>("sphericity",     "sphericity", 100, 0, 1);
  histContainer_["circularity"]=fs->make<TH1F>("circularity",   "circularity", 100, 0, 1);
  histContainer_["isotropy"]=fs->make<TH1F>("isotropy",   "isotropy", 100, 0, 1);
  histContainer_["centrality"]=fs->make<TH1F>("centrality",   "centrality", 100, 0, 1);
  histContainer_["C"]=fs->make<TH1F>("C",   "C", 100, 0, 1);
  histContainer_["D"]=fs->make<TH1F>("D",   "D", 100, 0, 1);
  histContainer_["sumEt"]=fs->make<TH1F>("sumEt",   "sumEt", 100, 0, 1000);
  histContainer_["DeltaPhi"]=fs->make<TH1F>("DeltaPhi",   "DeltaPhi", 320, 0, 3.2);
  histContainer_["sumEt_3"]=fs->make<TH1F>("sumEt_3",   "sumEt_3", 100, 0, 1000);
  histContainer_["sumE"]=fs->make<TH1F>("sumE",   "sumE", 100, 0, 1000);
  histContainer2d_["etamoment_et"]=fs->make<TH2F>("etamoment_et",   "etamoment_et", 100, 0, 500, 100, 0, .2);
  histContainer2d_["phimoment_et"]=fs->make<TH2F>("phimoment_et",   "phimoment_et", 100, 0, 500, 100, 0, .2);
  
  histContainer_["etamoment"]=fs->make<TH1F>("etamoment",   "etamoment", 100, 0, 0.2);
  histContainer_["phimoment"]=fs->make<TH1F>("phimoment",   "phimoment", 100, 0, 0.2);
  
}
// ------------ method called once each job just after ending the event loop  ------------
void 
TopAnalyzer::endJob() {
  using namespace std;
  //cout << "Cut flow table" << endl;
  //cout << "Cut 1:   "      << histContainer_["cut_1"]->GetEntries() << endl;
  //cout << "Cut 2:   "      << histContainer_["cut_2"]->GetEntries() << endl;
  //cout << "Cut 3:   "    << histContainer_["cut_3"]->GetEntries() << endl;
  //cout << "Cut 4:   "    << histContainer_["cut_4"]->GetEntries() << endl;
  //cout << "Cut 5:   "    << histContainer_["cut_5"]->GetEntries() << endl;
  //cout << "Cut 3a:  "      << histContainer_["cut_3a"]->GetEntries() << endl;
  //cout << "Cut 3b:  "      << histContainer_["cut_3b"]->GetEntries() << endl;
  //cout << "Cut 3c:  "      << histContainer_["cut_3c"]->GetEntries() << endl;
  //cout << "Cut 3d:  " << histContainer_["cut_3d"]->GetEntries() << endl;
  //cout << "Cut 7:   " << histContainer_["cut_7"]->GetEntries() << endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(TopAnalyzer);
