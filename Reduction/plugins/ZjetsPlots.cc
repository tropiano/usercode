// -*- C++ -*-
//
// Package:    ZjetsPlots
// Class:      ZjetsPlots
// 
/**\class ZjetsPlots ZjetsPlots.cc MyAnalysis/ZjetsPlots/src/ZjetsPlots.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Piergiulio Lenzi
//         Created:  Wed Apr 30 17:58:51 CEST 2008
// $Id: ZjetsPlots.cc,v 1.5 2009/03/30 08:35:06 lenzip Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Flags.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

// pat histogram classes 
#include "PhysicsTools/StarterKit/interface/HistoComposite.h"
#include "PhysicsTools/StarterKit/interface/HistoMuon.h"
#include "PhysicsTools/StarterKit/interface/HistoElectron.h"
#include "PhysicsTools/StarterKit/interface/HistoJet.h"
#include "PhysicsTools/StarterKit/interface/PatKitHelper.h"

//root file utilities
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "PhysicsTools/UtilAlgos/interface/TFileDirectory.h"
#include "Firenze/Utilities/interface/TTreeService.h"

// root
#include "TH1D.h"
#include "TH2D.h"
//
// class decleration
//

class ZjetsPlots : public edm::EDAnalyzer {
   public:
      explicit ZjetsPlots(const edm::ParameterSet&);
      ~ZjetsPlots();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      template <class T> T* createHistograms(std::string dir, std::string name, std::string title, const PhysicsHistograms::KinAxisLimits& axis) const;

      //reco::NamedCompositeCandidate fixRoles(const reco::NamedCompositeCandidate& orig, std::string baserole) const;
      reco::CompositeCandidate fixRoles(const reco::CompositeCandidate& orig) const;

      template <class T> void 
      AddHistoGroupVsMulti(unsigned int njet, std::vector<T*>& vGroup, 
                           std::string dir, std::string title, std::string name,
                           const PhysicsHistograms::KinAxisLimits& axis);

      //template<class T> bool checkIsolation(const reco::NamedCompositeCandidate& cand) const;
      template<class T> bool checkIsolation(const reco::CompositeCandidate& cand) const;

      template<class T> bool isIsolated(const T* ) const;

      template <class T> std::vector<T> vectorFromView(const edm::View<reco::Candidate>& candidates);
      
      void fillLeptons(const edm::View<reco::Candidate>& candidates);

      reco::CompositeCandidate chooseZ(const reco::CompositeCandidateCollection& Zs) const;
      
      // ----------member data ---------------------------
      // Histogram server
      edm::Service<TFileService> fs;
      edm::Service<edm::TTreeService> trees;

      // mode: can be electron, muon or gen
      std::string _mode;

      // Histogram objects that make "standard" plots for each object
      pat::HistoElectron*    _histoE;
      pat::HistoMuon*        _histoMu;
      pat::HistoGenParticle* _histoGen;
      pat::HistoComposite*   _histoZ;
      pat::HistoJet      *   _histoJet;
      //std::vector<pat::HistoComposite*> _histoZVsMulti;
      //std::vector<pat::HistoJet*> _histoJetVsMulti;
      std::vector<std::string>* _triggers; 


      //TH1D* _djr_10;
      //TH1D* _djr_21;
      //TH1D* _djr_32;
      //std::vector<TH1D*> _djr_10VsMulti;
      //std::vector<TH1D*> _djr_21VsMulti;
      //std::vector<TH1D*> _djr_32VsMulti;

      //TTree* _tree;

      //TH1D* h_nJets;

      //configuration   
      pat::PatKitHelper _configHelper;
      edm::InputTag _leptontag ;
      edm::InputTag _ztag ;
      edm::InputTag _tagrate10;
      edm::InputTag _tagrate21;
      edm::InputTag _tagrate32;
      edm::InputTag _weightTag;
      edm::InputTag _jettag;
      edm::InputTag _trigLabel;
      edm::InputTag _beamspottag;
      bool     _hasWeightProducer; 
      bool     _hasTriggerResults;
      bool     _fillLeptons;
      bool     _hasBeamSpot;
      double _massmin, _massmax;

      std::string _modLabel;
};

template <class T> T* ZjetsPlots::createHistograms(std::string dir, std::string name, std::string title, const PhysicsHistograms::KinAxisLimits& axis) const{
  return new T(dir, name, title, axis.nBinsPt, axis.pt1, axis.pt2, axis.nBinsMass, axis.m1, axis.m2, axis.nBinsEta, axis.eta1, axis.eta2, trees->tree());
}

template<> bool ZjetsPlots::isIsolated(const pat::Muon* muon ) const {
  //return (muon->trackIso()<2 && muon->ecalIso() < 2 && muon->hcalIso() < 2 );
  return (muon->trackIso()+muon->ecalIso()+muon->hcalIso())/muon->pt() < 0.1;
}

template<> bool ZjetsPlots::isIsolated(const pat::Electron* elec ) const {
  return (elec->trackIso()<3 );
}

template <class T> std::vector<T> ZjetsPlots::vectorFromView(const edm::View<reco::Candidate>& candidates){
  edm::View<reco::Candidate>::const_iterator icand;
  std::vector<T> output;
  for (icand = candidates.begin(); icand != candidates.end(); ++icand){
    const T* current = dynamic_cast<const T*>(&*icand);
    if (!current) {
      throw cms::Exception("ZjetsPlots") << "cannot cast to the concrete type predicted by mode!! ";  
    }
    //std::cout << "Ciao" << std::endl;
    output.push_back(*current);
  }
  return output;
}

void ZjetsPlots::fillLeptons(const edm::View<reco::Candidate>& candidates){
  if (_mode == "muon") {
    std::vector<pat::Muon> muons = vectorFromView<pat::Muon>(candidates);
    //std::cout << "about to fill" << std::endl;
    _histoMu->fillCollection(muons);
    //std::cout << "filled" << std::endl;
  } else if (_mode == "electron") {
    std::vector<pat::Electron> electrons = vectorFromView<pat::Electron>(candidates);
    _histoE->fillCollection(electrons);
  } else {
    std::vector<reco::GenParticle> genparticles = vectorFromView<reco::GenParticle>(candidates);
    _histoGen->fillCollection(genparticles);
  }
}

//template<class T> bool ZjetsPlots::checkIsolation(const reco::NamedCompositeCandidate& cand) const {
template<class T> bool ZjetsPlots::checkIsolation(const reco::CompositeCandidate& cand) const {
  const reco::Candidate* c0 = cand.daughter(0);
  const reco::ShallowCloneCandidate * pshallow0 = dynamic_cast<const reco::ShallowCloneCandidate *>(c0);
  if (pshallow0 && pshallow0->hasMasterClone()) c0 = pshallow0->masterClone().get();
  const T* dau0 = dynamic_cast<const T*>(c0);

  const reco::Candidate* c1 = cand.daughter(1);
  const reco::ShallowCloneCandidate * pshallow1 = dynamic_cast<const reco::ShallowCloneCandidate *>(c1);
  if (pshallow1 && pshallow1->hasMasterClone()) c1 = pshallow1->masterClone().get();
  const T* dau1 = dynamic_cast<const T*>(c1);
  if (!dau0 || ! dau1){
    //std::cout << "Error in casting to Pat::Lepton*" << std::endl;
    throw cms::Exception("ZjetsPlots") << "Error in casting to Pat::Lepton* ";
  } else{
    //std::cout << "daughter 0: role: " << cand.roles()[0] << " charge: " << dau0->threeCharge() << " isolation: " << dau0->trackIso() << std::endl;
    //std::cout << "daughter 1: role: " << cand.roles()[1] << " charge: " << dau1->threeCharge() << " isolation: " << dau1->trackIso() << std::endl;
    //return (dau0->trackIso()<3 && dau1->trackIso()<3);
    return isIsolated(dau0) && isIsolated(dau1);
  }
}


template<> void ZjetsPlots::AddHistoGroupVsMulti(unsigned int njet, std::vector<TH1D*>& vGroup,
                                                std::string dir, std::string title, std::string name,
                                                const PhysicsHistograms::KinAxisLimits& axis) {
  if (vGroup.size() < njet+1){
    for (unsigned int i = vGroup.size(); i <= njet; ++i ){
      std::stringstream fullname;
      fullname << name.c_str() << " " <<  i << "jets";
      std::stringstream fulltitle;
      fulltitle << title.c_str();//  << " " <<  i << "jets";
      std::stringstream fulldir;
      fulldir << dir.c_str() << i << "jets";
      TFileDirectory misc = TFileDirectory(fs->mkdir(fulldir.str()));
      vGroup.push_back(misc.make<TH1D>(fulltitle.str().c_str(), fullname.str().c_str(), axis.nBinsPt, axis.pt1, axis.pt2));
    }
  }
}

template <class T> void ZjetsPlots::AddHistoGroupVsMulti(unsigned int njet, std::vector<T*>& vGroup,
                                             std::string dir, std::string title, std::string name,
                                             const PhysicsHistograms::KinAxisLimits& axis) {
  if (vGroup.size() < njet+1){
    for (unsigned int i = vGroup.size(); i <= njet; ++i ){
      std::stringstream fullname;
      fullname << name.c_str() ; //<<  i << "jets";
      std::stringstream fulltitle;
      fulltitle << title.c_str() << " " << i << "jets";
      std::stringstream fulldir;
      fulldir << dir.c_str() << i << "jets";
      vGroup.push_back(new T(fulldir.str(), fulltitle.str(), fullname.str(), axis.nBinsPt, axis.pt1, axis.pt2, axis.nBinsMass, axis.m1, axis.m2, axis.nBinsEta, axis.eta1, axis.eta2));
    }
  }
}


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
ZjetsPlots::ZjetsPlots(const edm::ParameterSet& iConfig) : _configHelper(iConfig) 
{
   //fs->file().cd("..");
   _mode = iConfig.getParameter<std::string>("mode");
   fs->file().ls();
   fs->file().pwd();
   //_tree = fs->make<TTree>("tree", "tree"); 
   fs->file().cd();
   //_tree = new TTree("tree", "tree");

   //now do what ever initialization is needed
   if (iConfig.exists("LeptonSource")){
    _leptontag = iConfig.getParameter<edm::InputTag>("LeptonSource");
    _fillLeptons = true;
   } else {
    _fillLeptons = false;
   }
   _ztag      = iConfig.getParameter<edm::InputTag>("ZSource");
   _tagrate10 = iConfig.getParameter<edm::InputTag>("DiffJetRate10");
   _tagrate21 = iConfig.getParameter<edm::InputTag>("DiffJetRate21");
   _tagrate32 = iConfig.getParameter<edm::InputTag>("DiffJetRate32");
   _jettag    = iConfig.getParameter<edm::InputTag>("JetSource");
   
   if (iConfig.exists("BeamSpot")){
    _beamspottag = iConfig.getParameter<edm::InputTag>("BeamSpot");
    _hasBeamSpot = true;
   } else {
    _hasBeamSpot = false;  
   }

   _modLabel = iConfig.getParameter<std::string>("@module_label");

   if (_fillLeptons) {
    if (_mode=="muon"){
      _histoMu = createHistograms<pat::HistoMuon>("Mu"+_modLabel, "Mu " + _ztag.label(), "Mu", _configHelper.getAxisLimits("Z"));
      _histoE = 0;
      _histoGen = 0;
    } else if (_mode=="electron") {
      _histoE = createHistograms<pat::HistoElectron>("E"+_modLabel, "E " + _ztag.label(), "E", _configHelper.getAxisLimits("Z")); 
      _histoMu = 0;
      _histoGen = 0;
    } else if (_mode=="gen") {
      _histoGen = createHistograms<pat::HistoGenParticle>("genLep"+_modLabel, "genLep " + _ztag.label(), "genLep", _configHelper.getAxisLimits("Z"));
      _histoMu = 0;
      _histoE = 0;
    } else {
      throw cms::Exception("ZjetsPlots") << "Wrong mode!, mode can be: muon, electron, gen ";
    }
   } else {
     _histoMu = 0;
     _histoE = 0;
     _histoGen = 0;
   }
  
   _histoZ = createHistograms<pat::HistoComposite>("Z"+_modLabel, "Z " + _ztag.label(), "Z", 
                                                         _configHelper.getAxisLimits("Z"));
   fs->file().pwd();                                                      
   _histoJet = createHistograms<pat::HistoJet>("jet"+_modLabel, "Jet " + _ztag.label(), "Jet", 
                                                         _configHelper.getAxisLimits("Jet"));
   fs->file().pwd();                                                      

   std::string name = "jet"+_modLabel;
/*
   TFileDirectory misc1 = TFileDirectory(fs->mkdir(name.c_str()));
   PhysicsHistograms::KinAxisLimits djrAxis = _configHelper.getAxisLimits("diffJetRate");
   _djr_10 = misc1.make<TH1D>("diffjetrate10", "Diff Jet Rate 1->0 ", 
                                    djrAxis.nBinsPt, djrAxis.pt1, djrAxis.pt2);   
   _djr_21 = misc1.make<TH1D>("diffjetrate21", "Diff Jet Rate 2->1 ", 
                                    djrAxis.nBinsPt, djrAxis.pt1, djrAxis.pt2);   
   _djr_32 = misc1.make<TH1D>("diffjetrate32", "Diff Jet Rate 3->2 ", 
                                    djrAxis.nBinsPt, djrAxis.pt1, djrAxis.pt2);   
*/
   //dedicated branch for trigger names
   if (iConfig.exists("TriggerResults")){
    _trigLabel = iConfig.getParameter<edm::InputTag>("TriggerResults");
    _triggers = new std::vector<std::string>();
    std::string branchname = "triggers" + _modLabel; 
    std::cout << "Adding branch " << branchname << std::endl;
    trees->tree()->Bronch(branchname.c_str(), "std::vector<std::string>", &_triggers, 8000, 1);
    _hasTriggerResults = true;
   } else {
    _triggers = 0; 
    _hasTriggerResults = false;
   }



   if (iConfig.exists("WeightProducer")) {
     _hasWeightProducer = true;
     _weightTag = iConfig.getParameter<edm::InputTag>("WeightProducer");
   } 
   else _hasWeightProducer  = false;  

   fs->file().ls();

}


ZjetsPlots::~ZjetsPlots()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   delete _histoZ;
   delete _histoJet;
/*
    for (std::vector<pat::HistoComposite*>::const_iterator i = _histoZVsMulti.begin(); 
        i != _histoZVsMulti.end(); ++i){
      delete *i; 
    }     
    for (std::vector<pat::HistoJet*>::const_iterator i = _histoJetVsMulti.begin();
        i != _histoJetVsMulti.end(); ++i){
      delete *i;
    }               
*/
}



//
// member functions
//

// ------------ method called to for each event  ------------
void
ZjetsPlots::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;

   _histoZ->clearTreeVariables();
   _histoJet->clearTreeVariables();
   if (_histoMu) _histoMu->clearTreeVariables();
   if (_histoE) _histoE->clearTreeVariables();
   if (_histoGen) _histoGen->clearTreeVariables();

   // if the beams pot is pecified we use it
   //otherwise (0,0,0) is used
   if (_hasBeamSpot){
     edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
     iEvent.getByLabel(_beamspottag, recoBeamSpotHandle);
     reco::TrackBase::Point beamSpot(recoBeamSpotHandle->position());
     _histoZ->setBeamSpot(beamSpot);
     if (_histoMu) _histoMu->setBeamSpot(beamSpot);
     if (_histoE)  _histoE ->setBeamSpot(beamSpot);
   }  

   //get the lepton candidates
   //if we fill the leptons info 
   // we fill the whole tree only if 
   // there is at least one lepton
   if (_fillLeptons){
    Handle<edm::View<reco::Candidate> > lepHandle;
    iEvent.getByLabel(_leptontag, lepHandle);
    const edm::View<reco::Candidate>& leptons = *lepHandle;
    if (leptons.size() == 0) return;
    fillLeptons(leptons);
   } 

   double weight = 1.;
   

   //fill trigger info
   if (_hasTriggerResults){
    _triggers->clear();
    edm::Handle<edm::TriggerResults> HLTR;
    iEvent.getByLabel(_trigLabel, HLTR);
    TriggerNames nameservice(*HLTR);
    for ( unsigned int iHlt=0; iHlt < HLTR->size(); iHlt++ ) {
     // if the trigegr is passed store the info
     if (HLTR->accept(iHlt)==1){
       _triggers->push_back(nameservice.triggerName(iHlt));
     }
    }
   } 


   // get the Z candidates
   Handle<reco::CompositeCandidateCollection> ZHandle;
   iEvent.getByLabel(_ztag, ZHandle);
   const reco::CompositeCandidateCollection& Zs = *ZHandle;
   
   // get the jets
   Handle<edm::View<reco::Jet> > jetHandle;
   iEvent.getByLabel(_jettag, jetHandle);   
   const edm::View<reco::Jet>& alljets = *jetHandle;

   //remove jets overlapping with electrons
   std::vector<const reco::Jet*> jets;
   edm::View<reco::Jet>::const_iterator ijet;
   for (ijet = alljets.begin(); ijet != alljets.end(); ++ijet){
     const pat::Jet* patjet = dynamic_cast<const pat::Jet*>(&*ijet); 
     if (patjet){
      if (pat::Flags::test(*ijet, pat::Flags::Overlap::Electrons)){
        //std::cout << "Removing jet overlapping with electron" << std::endl;  
        jets.push_back(&*ijet); 
      }
     } else jets.push_back(&*ijet); 
   } 

   unsigned int jetsize = jets.size(); 
    
   if (Zs.size() > 0){
     CompositeCandidate cand = chooseZ(Zs);
     bool isolatedMuons = true;

     if (!isolatedMuons) throw cms::Exception("ZjetsPlots") << "ERROR: isolation check failed " << std::endl;

     if (isolatedMuons){
       _histoZ->fill(cand);
     }   
   }
    // fill the jet histos
    _histoJet->fillCollection(jets, weight);
}


// ------------ method called once each job just before starting event loop  ------------
void 
ZjetsPlots::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ZjetsPlots::endJob() {
  //_tree->Write();
}


//reco::NamedCompositeCandidate ZjetsPlots::fixRoles(const reco::NamedCompositeCandidate& orig, std::string baserole) const{
reco::CompositeCandidate ZjetsPlots::fixRoles(const reco::CompositeCandidate& orig) const{
  reco::CompositeCandidate::role_collection roles;
  reco::CompositeCandidate cand = orig;
  std::string namep = "lp"+_modLabel;
  std::string namem = "lm"+_modLabel;
  if (cand.daughter(0)->threeCharge()> 0) {
    roles.push_back(namep);
    roles.push_back(namem);
  } else {
    roles.push_back(namem);
    roles.push_back(namep);
  }
  //std::cout << typeid(*(cand.daughter(0)->masterClone())).name() << std::endl;
  //const pat::Electron* ele = dynamic_cast<const pat::Electron*>(cand.daughter(0)->masterClone().get());
  //if (ele) std::cout << "ele id Tight " << ele->electronID("eidRobustTight") << std::endl;
  cand.setRoles(roles);
  cand.applyRoles();
  return cand;
}

reco::CompositeCandidate ZjetsPlots::chooseZ(const reco::CompositeCandidateCollection& Zs) const {
  reco::CompositeCandidate chosen;
  reco::CompositeCandidateCollection::const_iterator iter;
  double distance = 10000;
  for (iter = Zs.begin(); iter != Zs.end(); ++iter){
    if (fabs(iter->mass()-91.1876) < distance){
      distance = fabs(iter->mass()-91.1876);
      chosen = *iter;
    }
  }
  return chosen;
}


//define this as a plug-in
DEFINE_FWK_MODULE(ZjetsPlots);
