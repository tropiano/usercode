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
// $Id: TopAnalyzerTree.cc,v 1.2 2011/05/12 13:41:44 tropiano Exp $
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
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

#include "TH1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"

#include <vector>

//
// class decleration
//

class TopAnalyzerTree : public edm::EDAnalyzer {
public:
  explicit TopAnalyzerTree(const edm::ParameterSet&);
  ~TopAnalyzerTree();
  
  
private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();
  
  // ----------member data ---------------------------
  //TH1F * z_mass;
  //canvas and histograms
  //TCanvas *canvas;
  std::string outputFile_;
  
  std::map<std::string,TH1F*> histContainer_;
  std::map<std::string,TH2F*> histContainer2d_;
  
 
  //input tags
  //edm::InputTag photonSrc_;
  
  //edm::InputTag triggerName_;
  edm::InputTag elecSrc_;
  edm::InputTag muonSrc_;
  edm::InputTag jetSrc_;
  //edm::InputTag photonSrc_;
  edm::InputTag triggerSrc_;
  std::vector<std::string> triggerName_;
  edm::InputTag vertexSrc_;
  //kinematic cuts

  float photon_ptcut_;
  float photon_etacut_;
  float jets_ptcut_;
  float jets_etacut_;
   
  float electronveto_ptcut_;
  float electronveto_etacut_;
  float muonveto_ptcut_;
  float muonveto_etacut_;


  float discr_cut1_;
  float discr_cut2_; 
  float discr_cut3_;
  float discr_cut4_; 
  std::string btag_algo1_;
  std::string btag_algo2_; 
  float relIso_;
  float jets_deltarcut_;
  //tree variables
  float jets;
  float jetpt1;
  float jeteta1;
  float jetConst1;
  float jetbDiscr1;
  
  float jetpt2;
  float jeteta2;
  float jetConst2;
  float jetbDiscr2;
  
  float jetpt3;
  float jeteta3;
  float jetConst3;
  float jetbDiscr3;
  
  float jetpt4;
  float jeteta4;
  float jetConst4;
  float jetbDiscr4;
  
  float jetpt5;
  float jeteta5;
  float jetConst5;
  float jetbDiscr5;
  
  float jetpt6;
  float jeteta6;
  float jetConst6;
  float jetbDiscr6;
  
  float EtStar1;
  float EtStar2;
  float ProdStar;
  float ProdStarNorm;
  float EtStar1Norm;
  float EtStar2Norm;
  float sphericity;
  float circularity;
  float isotropy;
  float centrality;
  float aplanarity;
  
  float C;
  float D;
  float sumEt;
  float DeltaPhi;
  float sumEt_3;
  float sumE;
  float n_vertex;
  float selected_jets;
  float bJetPtHP;
  float bJetPtHE;
  float HPdiscr;
  float HEdiscr;
  float chargedMult;
  float chargedEmEnergy;
  float chargedHadronEnergy;
  float chargedHadEnergyFrac;
  float chargedEmEnergyFrac;
  float etamoment;
  float phimoment;
  float leptonveto;  
  float NbtagHPloose;
  float NbtagHPmedium;
  float NbtagHEloose;
  float NbtagHEmedium;
  
  //file and tree
  TFile* output;
  TTree* TopTree;


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

std::vector<math::XYZVector> makeVecForEventShapeTree(std::vector<pat::Jet> jets, double scale = 1.) {
  std::vector<math::XYZVector> p;
  //unsigned int i=1;
  for (std::vector<pat::Jet>::const_iterator jet = jets.begin(); jet != jets.end(); ++jet) {
    math::XYZVector Vjet(jet->px() * scale, jet->py() * scale, jet->pz() * scale);
    p.push_back(Vjet);
  }
  return p;
}

double etaetaMomentHome(pat::Jet jet) {
  //std::cout<< "in etaetaMoment " << std::endl;
  std::vector<const reco::Candidate*> towers = jet.getJetConstituentsQuick();
  //std::cout<< "got the constituents " << std::endl;
  double sumw = 0;
  double sum = 0;
  double sum2 = 0;
  int i = towers.size();
  while (--i >= 0) {
    double value = towers[i]->eta();
    double weight = towers[i]->et();
    sumw += weight;
    sum += value * weight;
    sum2 += value * value * weight;
  }
  return sumw > 0 ? (sum2 - sum*sum/sumw ) / sumw : 0;
}

TopAnalyzerTree::TopAnalyzerTree(const edm::ParameterSet& iConfig):
  outputFile_(iConfig.getUntrackedParameter<std::string>("outputFile")),
  histContainer_(),
  histContainer2d_(),
  elecSrc_(iConfig.getUntrackedParameter<edm::InputTag>("electronSrc")),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  //photonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("photonSrc")),
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" )),
  triggerSrc_(iConfig.getUntrackedParameter<edm::InputTag>("triggerSrc" )),
  triggerName_(iConfig.getUntrackedParameter<std::vector<std::string> >("triggerName")),
  vertexSrc_(iConfig.getUntrackedParameter<edm::InputTag>("vertexSrc" )),
  electronveto_ptcut_(iConfig.getParameter<double>("ElectronVeto_PtCut")),
  electronveto_etacut_(iConfig.getParameter<double>("ElectronVeto_EtaCut")),
  muonveto_ptcut_(iConfig.getParameter<double>("MuonVeto_PtCut")),
  muonveto_etacut_(iConfig.getParameter<double>("MuonVeto_EtaCut")),
  jets_ptcut_(iConfig.getParameter<double>("Jets_PtCut")),
  jets_etacut_(iConfig.getParameter<double>("Jets_EtaCut")),
  discr_cut1_(iConfig.getParameter<double>("BtagDiscrCut1")),
  discr_cut2_(iConfig.getParameter<double>("BtagDiscrCut2")),
  discr_cut3_(iConfig.getParameter<double>("BtagDiscrCut3")),
  discr_cut4_(iConfig.getParameter<double>("BtagDiscrCut4")), 
  btag_algo1_(iConfig.getParameter<std::string>("BtagAlgo1")),
  btag_algo2_(iConfig.getParameter<std::string>("BtagAlgo2")),
  relIso_(iConfig.getParameter<double>("RelIso"))
{
  //now do what ever initialization is needed
  
  
  
}

TopAnalyzerTree::~TopAnalyzerTree()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}
//
// member functions
//

// ------------  method called to for each event  ------------
void
TopAnalyzerTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  // get electron collection
  Handle<edm::View<pat::Electron> > electronsHandle;
  iEvent.getByLabel(elecSrc_,electronsHandle);
  edm::View<pat::Electron> electrons = *electronsHandle;

  //cout<< "electrons found" << endl;
  // get muon collection
  Handle<edm::View<pat::Muon> > muonsHandle;
  iEvent.getByLabel(muonSrc_,muonsHandle);
  edm::View<pat::Muon> muons = *muonsHandle;
  //cout<< "muons found" << endl;

  // get photon collection
  /*Handle<edm::View<pat::Photon> > photonsHandle;
  iEvent.getByLabel(photonSrc_,photonsHandle);
  edm::View<pat::Photon> photons = *photonsHandle;*/

  // get tau collection  
  //Handle<edm::View<pat::Tau> > tausHandle;
  //iEvent.getByLabel(tauSrc_,tausHandle);
  //edm::View<pat::Tau> taus = *tausHandle;

  // get jet collection
  Handle<edm::View<pat::Jet> > jetsHandle;
  iEvent.getByLabel(jetSrc_,jetsHandle);
  edm::View<pat::Jet> jets = *jetsHandle;
  //cout<< "jets found" << endl;

  // get vertex
  Handle<edm::View<reco::Vertex> > vertexHandle;
  iEvent.getByLabel(vertexSrc_,vertexHandle);
  edm::View<reco::Vertex> vertex = *vertexHandle;
  //cout<< "vertex found" << endl;

  //get trigger
  edm::Handle<pat::TriggerEvent> triggerHandle;
  iEvent.getByLabel(triggerSrc_, triggerHandle);
  pat::TriggerEvent trigger = *triggerHandle;
  

  std::vector<pat::Jet> all_jets;
  std::vector<pat::Jet> selec_jets;
  bool triggered = false;
  int nvertex    = 0;
  
  std::vector<pat::Jet> selec_jets_electrons;
  //std::cout<<"trigger path "<<*(trigger.acceptedPaths().begin())<<std::endl;
  
  /*for(std::vector<pat::TriggerPath>::const_iterator ipath = trigger.paths()->begin(); ipath!=trigger.paths()->end(); ipath++)
    {
      cout<<"trigger path "<<ipath->name()<<endl;
      }*/
  
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

      //cout<< "triggered" << endl;
      //histContainer_["trigger"]->Fill(1);
      //vertex requirement
      if(vertex.size()<1) return;  

      if (vertex.size()<99){
	for(edm::View<reco::Vertex>::const_iterator ivertex=vertex.begin(); ivertex!=vertex.end(); ++ivertex){
	  
	  if((!(ivertex->isFake())) 
	     && (ivertex->ndof()>4)
	     && (fabs(ivertex->z())<15) 
	     && (fabs(ivertex->position().Rho())<2.0))
	    {
	      //isVertex = true;
	      nvertex++;
	      //cout<< "found one vertex" << endl;
	    }
	}
      }
      
      if(nvertex){
	n_vertex=nvertex;
	for(edm::View<pat::Jet>::const_iterator ijet=jets.begin(); ijet!=jets.end(); ++ijet)
	  {
	    if(ijet->pt()>jets_ptcut_                           //pt cut
	       && fabs(ijet->eta())<jets_etacut_                //eta cut
	       //&& deltaR>jets_deltarcut_                      //electron rejection cut
	       //&& ijet->chargedEmEnergyFraction()>jets_emfraccut_      //em fraction
	       //&& ijet->jetID().n90Hits>1
		 //&& ijet->jetID().fHPD<0.98
	       )
	      {
		chargedMult=ijet->chargedMultiplicity();
		chargedEmEnergy=ijet->chargedEmEnergy();
		chargedHadronEnergy=ijet->chargedHadronEnergy();
		chargedHadEnergyFrac=ijet->chargedHadronEnergyFraction();
		chargedEmEnergyFrac=ijet->chargedEmEnergyFraction();
		//cout<< "energy fraction variables filled" << endl;
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
	      //cout<< "electron found" << endl;
	      vetoelectron=true;
	   
	  }
	 
	for(edm::View<pat::Muon>::const_iterator imuon=muons.begin(); imuon!=muons.end(); ++imuon)
	  {
	    if (//(imuon->isGood("AllGlobalMuons")>0)
		(imuon->pt()>muonveto_ptcut_)
		&&(fabs(imuon->eta())<muonveto_etacut_)
		&&(((imuon->hcalIso()+imuon->ecalIso()+imuon->trackIso())/imuon->pt())<relIso_) )
	      
	      //((imuon->isolationR03()->sumPt()+imuon->isolationR03()->emEt()+imuon->isolationR03()->hadEt())/imuon->pt()<0.2))
	      //cout<< "muon found" << endl;
	      vetomuon=true;
	  }	
	selected_jets=selec_jets.size();

	if(selec_jets.size()>5){
	  
	  if(vetoelectron || vetomuon)	  leptonveto=1;
	  //jet related variables
	  std::vector<double> EtStar;
	  for(std::vector<pat::Jet>::iterator iseljets = selec_jets.begin();iseljets != selec_jets.end();++iseljets)
	    {
	      double pz = iseljets->pz();
	      double E  = iseljets->energy();
	      double Et = iseljets->et();
	      EtStar.push_back( Et*(1-(pz*pz)/(E*E)) );
	      //cout<< "retrieved pz, Et, E of the jets" << endl;
	    }
	  double ProdStar = TMath::GeomMean(EtStar.begin()+2,EtStar.end());
	  
	  EtStar1=EtStar[0];
	  EtStar2=EtStar[1];
	  
	  //kinematical selection
	  EventShapeVariables eventshape(makeVecForEventShapeTree(selec_jets));
	  //cout<< "event shapes made" << endl;
	  aplanarity  = eventshape.aplanarity();
	  sphericity  = eventshape.sphericity();
	  circularity = eventshape.circularity();
	  isotropy    = eventshape.isotropy();
	  C           = eventshape.C();
	  D           = eventshape.D();
	  DeltaPhi    = reco::deltaPhi(selec_jets[0].phi(), selec_jets[1].phi());
	  sumEt       = 0;
	  sumE        = 0;
	  //cout<< "event shapes filled" << endl;
	  for(std::vector<pat::Jet>::iterator iseljets = selec_jets.begin();iseljets != selec_jets.end();++iseljets)
	    {
	      sumEt          += iseljets->et();
	      sumE           += iseljets->energy();
	    }
	  centrality  = sumEt/sumE;
	  sumEt_3     = sumEt - selec_jets[0].et() - selec_jets[1].et();
	  
	  
	  EtStar1Norm=(EtStar[0]/sumEt);
	  EtStar2Norm=(EtStar[1]/sumEt);
	  ProdStarNorm=(ProdStar/sumEt);
	  
	  //b-tagging
	  int btaggedmedium1=0;
	  int btaggedloose1=0;
	  for(std::vector<pat::Jet>::iterator iseljets = selec_jets.begin();iseljets != selec_jets.end();++iseljets)
	    {
	      HPdiscr=iseljets->bDiscriminator(btag_algo1_);
	      //cout<< "b discriminator filled" << endl;
	      if(iseljets->bDiscriminator(btag_algo1_) > discr_cut1_)
		{
		  btaggedloose1++;
		  bJetPtHP=iseljets->pt();
		}
	      if(iseljets->bDiscriminator(btag_algo1_) > discr_cut2_)
		{
		  btaggedmedium1++;
		}
	    }
	  
	  NbtagHPmedium=btaggedmedium1;
	  NbtagHPloose=btaggedloose1;
	  
	  int btaggedmedium2=0;
	  int btaggedloose2=0;
	  std::vector<double> etamoment_norm;
	  std::vector<double> phimoment_norm;
	  for(std::vector<pat::Jet>::iterator iseljets = selec_jets.begin();iseljets != selec_jets.end();++iseljets)
	    {
	      HEdiscr=iseljets->bDiscriminator(btag_algo2_);
	      //cout<< "2nd b discriminator filled" << endl;
	      if(iseljets->bDiscriminator(btag_algo2_) > discr_cut3_){
		btaggedloose2++;
		bJetPtHE=iseljets->pt();
	      }
	      if(iseljets->bDiscriminator(btag_algo2_) > discr_cut4_){
		btaggedmedium2++;
	      }
	      
	      
	      //if(!btaggedloose1){
		//fill jet shapes only if it's not b-tagged
		//etamoment=etaetaMomentHome(*iseljets);
		//phimoment=iseljets->phiphiMoment();
		//cout<< "eta moments, phi moments filled " << etamoment << endl;
	      //}
	    }
	  
	  NbtagHEmedium=btaggedmedium2;
	  NbtagHEloose=btaggedloose2;
	   
	  std::vector<pat::Jet>::iterator ijet = selec_jets.begin();
	  jetpt1=ijet->pt();
	  jeteta1=ijet->eta();
	  jetConst1=ijet->associatedTracks().size();
	  //cout<< "associated tracks filled" << endl;
	  jetbDiscr1=ijet->bDiscriminator(btag_algo2_);
	  //if(ijet->associatedTracks().size())
	  //cout<<"tracce con dz rispetto al vertice: "<< ijet->associatedTracks()[0]->dz(pv.position()) <<endl;
	  
	  if(ijet!=selec_jets.end())
	    {
	      ijet++;
	      jetpt2=ijet->pt();
	      jeteta2=ijet->eta();
	      jetConst2=ijet->associatedTracks().size();
	      jetbDiscr2=ijet->bDiscriminator(btag_algo2_);
	 
	    }
	  if(ijet!=selec_jets.end())
	    {
	      ijet++;
		jetpt3=ijet->pt();
		jeteta3=ijet->eta();
		jetConst3=ijet->associatedTracks().size();
		jetbDiscr3=ijet->bDiscriminator(btag_algo2_);
	 
	    }
	  if(ijet!=selec_jets.end())
	    {
		ijet++;
		jetpt4=ijet->pt();
		jeteta4=ijet->eta();
		jetConst4=ijet->associatedTracks().size();
		jetbDiscr4=ijet->bDiscriminator(btag_algo2_);
	 
	    }
	  
	  if(ijet!=selec_jets.end())
	    {
	      ijet++;
	      jetpt5=ijet->pt();
	      jeteta5=ijet->eta();
	      jetConst5=ijet->associatedTracks().size();
	      jetbDiscr5=ijet->bDiscriminator(btag_algo2_);
	 
	    }
	  if(ijet!=selec_jets.end())
	    {
	      ijet++;
	      jetpt6=ijet->pt();
	      jeteta6=ijet->eta();
	      jetConst6=ijet->associatedTracks().size();
	      jetbDiscr6=ijet->bDiscriminator(btag_algo2_);
	 
	    }
	  TopTree->Fill();
  
	}
      }
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
TopAnalyzerTree::beginJob()
{

  output = new TFile(outputFile_.c_str(),"RECREATE");
  int bufsize=64000;
  TopTree = new TTree("TopTree","Top Tree");
  
  //register to the TFile Service
  //edm::Service<TFileService> fs;
  
  //pt, eta, number of constituents of the jets
  TopTree->Branch("jets", &jets, "jets/F", bufsize);
   
  TopTree->Branch("jetpt1",     &jetpt1,          "jetpt1/F",    bufsize);
  TopTree->Branch("jeteta1",    &jeteta1,         "jeteta1/F",   bufsize);
  TopTree->Branch("jetConst1",  &jetConst1,       "jetConst1/F", bufsize);
  TopTree->Branch("jetbDiscr1", &jetbDiscr1,      "jetbDiscr1/F", bufsize);
  
  TopTree->Branch("jetpt2",     &jetpt2,          "jetpt2/F",    bufsize);
  TopTree->Branch("jeteta2",    &jeteta2,         "jeteta2/F",   bufsize);
  TopTree->Branch("jetConst2",  &jetConst2,       "jetConst2/F", bufsize);
  TopTree->Branch("jetbDiscr2", &jetbDiscr2,      "jetbDiscr2/F", bufsize);
  
  TopTree->Branch("jetpt3",     &jetpt3,          "jetpt3/F",    bufsize);
  TopTree->Branch("jeteta3",    &jeteta3,         "jeteta3/F",   bufsize);
  TopTree->Branch("jetConst3",  &jetConst3,       "jetConst3/F", bufsize);
  TopTree->Branch("jetbDiscr3", &jetbDiscr3,      "jetbDiscr3/F", bufsize);
  
  TopTree->Branch("jetpt4",     &jetpt4,          "jetpt4/F",    bufsize);
  TopTree->Branch("jeteta4",    &jeteta4,         "jeteta4/F",   bufsize);
  TopTree->Branch("jetConst4",  &jetConst4,       "jetConst4/F", bufsize);
  TopTree->Branch("jetbDiscr4", &jetbDiscr4,      "jetbDiscr4/F", bufsize);
  
  TopTree->Branch("jetpt5",     &jetpt5,          "jetpt5/F",    bufsize);
  TopTree->Branch("jeteta5",    &jeteta5,         "jeteta5/F",   bufsize);
  TopTree->Branch("jetConst5",  &jetConst5,       "jetConst5/F", bufsize);
  TopTree->Branch("jetbDiscr5", &jetbDiscr5,      "jetbDiscr5/F", bufsize);
  
  TopTree->Branch("jetpt6",     &jetpt6,          "jetpt6/F",    bufsize);
  TopTree->Branch("jeteta6",    &jeteta6,         "jeteta6/F",   bufsize);
  TopTree->Branch("jetConst6",  &jetConst6,       "jetConst6/F", bufsize);
  TopTree->Branch("jetbDiscr6", &jetbDiscr6,      "jetbDiscr6/F", bufsize);
  
  //jet shapes
  TopTree->Branch("EtStar1",      &EtStar1,      "EtStar1/F",      bufsize);
  TopTree->Branch("EtStar2",      &EtStar2,      "EtStar2/F",      bufsize);
  TopTree->Branch("ProdStar",     &ProdStar,     "ProdStar/F",     bufsize);
  TopTree->Branch("EtStar1Norm",  &EtStar1Norm,  "EtStar1Norm/F",  bufsize);
  TopTree->Branch("EtStar2Norm",  &EtStar2Norm,  "EtStar2Norm/F",  bufsize);
  TopTree->Branch("ProdStarNorm", &ProdStarNorm, "ProdStarNorm/F", bufsize);
  
  TopTree->Branch("aplanarity",   &aplanarity,   "aplanarity/F",   bufsize);
  TopTree->Branch("sphericity",   &sphericity,   "sphericity/F",   bufsize);
  TopTree->Branch("circularity",  &circularity,  "circularity/F",  bufsize);
  TopTree->Branch("isotropy",     &isotropy,     "isotropy/F",     bufsize);
  TopTree->Branch("centrality",   &centrality,   "centrality/F",   bufsize);
  TopTree->Branch("C",            &C,            "C/F",            bufsize);
  TopTree->Branch("D",            &D,            "D/F",            bufsize);
  TopTree->Branch("sumEt",        &sumEt,        "sumEt/F",        bufsize);
  TopTree->Branch("DeltaPhi",     &DeltaPhi,     "DeltaPhi/F",     bufsize);
  TopTree->Branch("sumEt_3",      &sumEt_3,      "sumEt_3/F",      bufsize);
  TopTree->Branch("sumE",         &sumE,         "sumE/F",         bufsize);
  TopTree->Branch("n_vertex",     &n_vertex,     "n_vertex/F",     bufsize);
  TopTree->Branch("selected_jets",&selected_jets,"selected_jets/F",bufsize);
  TopTree->Branch("bJetPtHP",     &bJetPtHP,     "bJetPtHP/F",     bufsize);
  TopTree->Branch("bJetPtHE",     &bJetPtHE,     "bJetPtHE/F",     bufsize); 
  TopTree->Branch("HPdiscr",      &HPdiscr,      "HPdiscr/F",      bufsize);
  TopTree->Branch("HEdiscr",      &HEdiscr,      "HEdiscr/F",      bufsize);
  
  TopTree->Branch("chargedMult",           &chargedMult,            "chargedMult/F",         bufsize);
  TopTree->Branch("chargedEmEnergy",       &chargedEmEnergy,        "chargedEmEnergy/F",     bufsize);
  TopTree->Branch("chargedHadronEnergy",   &chargedHadronEnergy,    "chargedHadronEnergy/F", bufsize);
  TopTree->Branch("chargedHadEnergyFrac",  &chargedHadEnergyFrac,   "chargedHadEnergyFrac/F",bufsize);
  TopTree->Branch("chargedEmEnergyFrac",   &chargedEmEnergyFrac,    "chargedEmEnergyFrac/F", bufsize);
  TopTree->Branch("etamoment",             &etamoment,              "etamoment/F",             bufsize);
  TopTree->Branch("phimoment",             &phimoment,              "phimoment/F",             bufsize);
  TopTree->Branch("leptonveto",            &leptonveto,             "leptonveto/F",             bufsize);
  
  TopTree->Branch("NbtagHPloose",            &NbtagHPloose,             "NbtagHPloose/F",             bufsize);
  TopTree->Branch("NbtagHPmedium",           &NbtagHPmedium,            "NbtagHPmedium/F",             bufsize);
  TopTree->Branch("NbtagHEloose",            &NbtagHEloose,             "NbtagHEloose/F",             bufsize);
  TopTree->Branch("NbtagHEmedium",           &NbtagHEmedium,            "NbtagHEmedium/F",             bufsize);
  
}
// ------------ method called once each job just after ending the event loop  ------------
void 
TopAnalyzerTree::endJob() {
  using namespace std;
  output->Write();
  output->Close();
}

//define this as a plug-in
DEFINE_FWK_MODULE(TopAnalyzerTree);
