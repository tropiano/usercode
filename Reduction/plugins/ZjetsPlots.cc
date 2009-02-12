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
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/NamedCompositeCandidate.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Flags.h"

// pat histogram classes 
#include "PhysicsTools/StarterKit/interface/HistoComposite.h"
#include "PhysicsTools/StarterKit/interface/HistoMuon.h"
#include "PhysicsTools/StarterKit/interface/HistoElectron.h"
#include "PhysicsTools/StarterKit/interface/HistoJet.h"
#include "PhysicsTools/StarterKit/interface/PatKitHelper.h"

//root file utilities
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "PhysicsTools/UtilAlgos/interface/TFileDirectory.h"

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

      reco::NamedCompositeCandidate fixRoles(const reco::NamedCompositeCandidate& orig, std::string baserole) const;

      template <class T> void 
      AddHistoGroupVsMulti(unsigned int njet, std::vector<T*>& vGroup, 
                           std::string dir, std::string title, std::string name,
                           const PhysicsHistograms::KinAxisLimits& axis);

      template<class T> bool checkIsolation(const reco::NamedCompositeCandidate& cand) const;

      template<class T> bool isIsolated(const T* ) const;
      
      // ----------member data ---------------------------
      // Histogram server
      edm::Service<TFileService> fs;

      // Histogram objects that make "standard" plots for each object
      pat::HistoComposite* _histoZ_mumu;
      pat::HistoJet      * _histoJet_mumu;
      std::vector<pat::HistoComposite*> _histoZ_mumuVsMulti;
      std::vector<pat::HistoJet*> _histoJet_mumuVsMulti;

      pat::HistoComposite* _histoZ_ee;
      pat::HistoJet      * _histoJet_ee;
      std::vector<pat::HistoComposite*> _histoZ_eeVsMulti;
      std::vector<pat::HistoJet*> _histoJet_eeVsMulti;

      TH1D* _djr_mumu_10;
      TH1D* _djr_mumu_21;
      TH1D* _djr_mumu_32;
      std::vector<TH1D*> _djr_mumu_10VsMulti;
      std::vector<TH1D*> _djr_mumu_21VsMulti;
      std::vector<TH1D*> _djr_mumu_32VsMulti;

      TH1D* _djr_ee_10;
      TH1D* _djr_ee_21;
      TH1D* _djr_ee_32;
      std::vector<TH1D*> _djr_ee_10VsMulti;
      std::vector<TH1D*> _djr_ee_21VsMulti;
      std::vector<TH1D*> _djr_ee_32VsMulti;
      //TH1D* h_nJets;

      //configuration   
      pat::PatKitHelper _configHelper;
      edm::InputTag _zmumutag ;
      edm::InputTag _zeetag   ;
      edm::InputTag _tagrate10;
      edm::InputTag _tagrate21;
      edm::InputTag _tagrate32;
      edm::InputTag _weightTag;
      edm::InputTag _jettag;
      bool     _hasWeightProducer;  
};

template <class T> T* ZjetsPlots::createHistograms(std::string dir, std::string name, std::string title, const PhysicsHistograms::KinAxisLimits& axis) const{
  return new T(dir, name, title, axis.nBinsPt, axis.pt1, axis.pt2, axis.nBinsMass, axis.m1, axis.m2, axis.nBinsEta, axis.eta1, axis.eta2);
}

template<> bool ZjetsPlots::isIsolated(const pat::Muon* muon ) const {
  return (muon->trackIso()<2 && muon->ecalIso() < 2 && muon->hcalIso() < 2 );
}

template<> bool ZjetsPlots::isIsolated(const pat::Electron* elec ) const {
  return (elec->trackIso()<3 );
}


template<class T> bool ZjetsPlots::checkIsolation(const reco::NamedCompositeCandidate& cand) const {
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

   //now do what ever initialization is needed
   _histoZ_mumu = createHistograms<pat::HistoComposite>("ZmumuHistos", "Z (#mu channel)", "Zmumu", _configHelper.getAxisLimits("muon"));
   _histoJet_mumu = createHistograms<pat::HistoJet>("jetmumuHistos", "Jet (#mu channel)", "Jetmumu", _configHelper.getAxisLimits("muonJet"));

   _histoZ_ee = createHistograms<pat::HistoComposite>("ZeeHistos", "Z (e channel)", "Zee", _configHelper.getAxisLimits("electron"));
   _histoJet_ee = createHistograms<pat::HistoJet>("jeteeHistos", "Jet (e channel)", "Jetee", _configHelper.getAxisLimits("electronJet"));

   TFileDirectory misc1 = TFileDirectory(fs->mkdir("jetmumuHistos"));
   PhysicsHistograms::KinAxisLimits djrMuonAxis = _configHelper.getAxisLimits("diffJetRateMuon");
   _djr_mumu_10 = misc1.make<TH1D>("diffjetrate10mumu", "Diff Jet Rate 1->0 (#mu channel)", djrMuonAxis.nBinsPt, djrMuonAxis.pt1, djrMuonAxis.pt2);   
   _djr_mumu_21 = misc1.make<TH1D>("diffjetrate21mumu", "Diff Jet Rate 2->1 (#mu channel)", djrMuonAxis.nBinsPt, djrMuonAxis.pt1, djrMuonAxis.pt2);   
   _djr_mumu_32 = misc1.make<TH1D>("diffjetrate32mumu", "Diff Jet Rate 3->2 (#mu channel)", djrMuonAxis.nBinsPt, djrMuonAxis.pt1, djrMuonAxis.pt2);   

   TFileDirectory misc2 = TFileDirectory(fs->mkdir("jeteeHistos"));
   PhysicsHistograms::KinAxisLimits djrElectronAxis = _configHelper.getAxisLimits("diffJetRateElectron");
   _djr_ee_10 = misc2.make<TH1D>("diffjetrate10ee", "Diff Jet Rate 1->0 (e channel)", djrElectronAxis.nBinsPt, djrElectronAxis.pt1, djrElectronAxis.pt2);      
   _djr_ee_21 = misc2.make<TH1D>("diffjetrate21ee", "Diff Jet Rate 2->1 (e channel)", djrElectronAxis.nBinsPt, djrElectronAxis.pt1, djrElectronAxis.pt2);      
   _djr_ee_32 = misc2.make<TH1D>("diffjetrate32ee", "Diff Jet Rate 3->2 (e channel)", djrElectronAxis.nBinsPt, djrElectronAxis.pt1, djrElectronAxis.pt2);

   _zmumutag  = iConfig.getParameter<edm::InputTag>("ZMuMuSource");
   _zeetag    = iConfig.getParameter<edm::InputTag>("ZEESource");
   _tagrate10 = iConfig.getParameter<edm::InputTag>("DiffJetRate10");
   _tagrate21 = iConfig.getParameter<edm::InputTag>("DiffJetRate21");
   _tagrate32 = iConfig.getParameter<edm::InputTag>("DiffJetRate32");
   _jettag    = iConfig.getParameter<edm::InputTag>("JetSource");

   if (iConfig.exists("WeightProducer")) {
     _hasWeightProducer = true;
     _weightTag = iConfig.getParameter<edm::InputTag>("WeightProducer");
   } 
   else _hasWeightProducer  = false;  

}


ZjetsPlots::~ZjetsPlots()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   delete _histoZ_mumu;
   delete _histoJet_mumu;
   delete _histoZ_ee;
   delete _histoJet_ee;

    for (std::vector<pat::HistoComposite*>::const_iterator i = _histoZ_mumuVsMulti.begin(); 
        i != _histoZ_mumuVsMulti.end(); ++i){
      delete *i; 
    }     
    for (std::vector<pat::HistoJet*>::const_iterator i = _histoJet_mumuVsMulti.begin();
        i != _histoJet_mumuVsMulti.end(); ++i){
      delete *i;
    }               

    for (std::vector<pat::HistoComposite*>::const_iterator i = _histoZ_eeVsMulti.begin();
        i != _histoZ_eeVsMulti.end(); ++i){
      delete *i;
    }
    for (std::vector<pat::HistoJet*>::const_iterator i = _histoJet_eeVsMulti.begin();
        i != _histoJet_eeVsMulti.end(); ++i){
      delete *i;
    }
   

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

   // get the Z candidates
   Handle<reco::NamedCompositeCandidateCollection> zmumuHandle;
   iEvent.getByLabel(_zmumutag, zmumuHandle);
   const reco::NamedCompositeCandidateCollection& Zmumus = *zmumuHandle;

   Handle<reco::NamedCompositeCandidateCollection> zeeHandle;
   iEvent.getByLabel(_zeetag, zeeHandle);
   const reco::NamedCompositeCandidateCollection& Zees = *zeeHandle;

   double weight = 1.;
   Handle<double> eventWeight;
   if (_hasWeightProducer){
    iEvent.getByLabel(_weightTag, eventWeight);
    weight = *eventWeight;
   }

   double rate10 = -9999;
   double rate21 = -9999;
   double rate32 = -9999;
   bool found10=false, found21=false, found32=false;
   Handle<double> hrate10, hrate21, hrate32;
   try {
    iEvent.getByLabel(_tagrate10, hrate10);
    found10=true;
    rate10=*hrate10;
   } catch (cms::Exception& e){
     LogDebug("ZplusJet") << "DiffJetRate10 not found for this event" << std::endl;
     //std::cout << "DiffJetRate10 not found for this event" << std::endl;
   }
   try{
    iEvent.getByLabel(_tagrate21, hrate21);
    found21=true;
    rate21=*hrate21;
   } catch (cms::Exception& e) {
     LogDebug("ZplusJet") << "DiffJetRate21 not found for this event" << std::endl; 
     //std::cout << "DiffJetRate21 not found for this event" << std::endl; 
   }
   try {
    iEvent.getByLabel(_tagrate32, hrate32);
    found32=true;
    rate32=*hrate32;
   } catch (cms::Exception& e) {
    LogDebug("ZplusJet") << "DiffJetRate32 not found for this event" << std::endl;
    //std::cout << "DiffJetRate32 not found for this event" << std::endl;
   }
   

   //std::cout << "weight is " << weight << std::endl;

   // get the jets
   Handle<std::vector<pat::Jet> > jetHandle;
   iEvent.getByLabel(_jettag, jetHandle);   
   const std::vector<pat::Jet>& alljets = *jetHandle;

   //remove jets overlapping with electrons
   std::vector<pat::Jet> jets;
   std::vector<pat::Jet>::const_iterator ijet;
   for (ijet = alljets.begin(); ijet != alljets.end(); ++ijet){
     if (pat::Flags::test(*ijet, pat::Flags::Overlap::Electrons)){
       //std::cout << "Removing jet overlapping with electron" << std::endl;  
       jets.push_back(*ijet); 
     }
   } 

   unsigned int jetsize = jets.size(); 
    
   if (Zmumus.size() == 1){
     NamedCompositeCandidate cand = fixRoles(Zmumus.front(), "#mu");

     bool isolatedMuons = checkIsolation<pat::Muon>(cand); 

     if (isolatedMuons){
      // fill Z histos
      //_histoZ_mumu->fillCollection(Zmumus);
      _histoZ_mumu->fill(cand);

      AddHistoGroupVsMulti<pat::HistoComposite>(jets.size(), _histoZ_mumuVsMulti, "ZmumuHistos", "Z (#mu channel)", "Zmumu", _configHelper.getAxisLimits("muon"));
      _histoZ_mumuVsMulti[jetsize]->fill(cand);

      // fill the jet histos
      _histoJet_mumu->fillCollection(jets, weight);

    

      AddHistoGroupVsMulti<pat::HistoJet>(jets.size(), _histoJet_mumuVsMulti, "jetmumuHistos", "Jet (#mu channel)", "Jetmumu", _configHelper.getAxisLimits("muonJet"));
  
      PhysicsHistograms::KinAxisLimits djrMuonAxis = _configHelper.getAxisLimits("diffJetRateMuon");
      AddHistoGroupVsMulti<TH1D>(jets.size(), _djr_mumu_10VsMulti, "jetmumuHistos", "diffjetrate10mumu", "Diff Jet Rate 1->0 (#mu channel)", djrMuonAxis);
      AddHistoGroupVsMulti<TH1D>(jets.size(), _djr_mumu_21VsMulti, "jetmumuHistos", "diffjetrate21mumu", "Diff Jet Rate 2->1 (#mu channel)", djrMuonAxis);
      AddHistoGroupVsMulti<TH1D>(jets.size(), _djr_mumu_32VsMulti, "jetmumuHistos", "diffjetrate32mumu", "Diff Jet Rate 3->2 (#mu channel)", djrMuonAxis);
      _histoJet_mumuVsMulti[jets.size()]->fillCollection(jets, weight); 
      if (found10) {
        _djr_mumu_10->Fill(rate10, weight);
        _djr_mumu_10VsMulti[jetsize]->Fill(rate10, weight);   
      }
      if (found21) {
        _djr_mumu_21->Fill(rate21, weight);
        _djr_mumu_21VsMulti[jetsize]->Fill(rate21, weight);
      }
      if (found32) {
        _djr_mumu_32->Fill(rate32, weight);
        _djr_mumu_32VsMulti[jetsize]->Fill(rate32, weight);
      }
     }
   }

   if (Zees.size() == 1){
     NamedCompositeCandidate cand = fixRoles(Zees.front(), "e"); 
 
     // fill Z histos
     _histoZ_ee->fill(cand);

     bool isolatedElectrons = checkIsolation<pat::Electron>(cand);

     //debug
     //if (cand.p4().mass()>120 && cand.p4().mass()<140){
     // std::cout << "Massa sospetta evento: " << iEvent.id().event() << " run: " << iEvent.id().run() << std::endl;
     //}

     
     if (isolatedElectrons){
      AddHistoGroupVsMulti<pat::HistoComposite>(jets.size(), _histoZ_eeVsMulti, "ZeeHistos", "Z (e channel)", "Zee", _configHelper.getAxisLimits("electron"));
      _histoZ_eeVsMulti[jets.size()]->fill(cand);

      // fill the jet histos
      _histoJet_ee->fillCollection(jets, weight);

      AddHistoGroupVsMulti<pat::HistoJet>(jets.size(), _histoJet_eeVsMulti, "jeteeHistos", "Jet (e channel)", "Jetee", _configHelper.getAxisLimits("electronJet"));

      PhysicsHistograms::KinAxisLimits djrElectronAxis = _configHelper.getAxisLimits("diffJetRateElectron");
      AddHistoGroupVsMulti<TH1D>(jets.size(), _djr_ee_10VsMulti, "jeteeHistos", "diffjetrate10ee", "Diff Jet Rate 1->0 (e channel)", djrElectronAxis);
      AddHistoGroupVsMulti<TH1D>(jets.size(), _djr_ee_21VsMulti, "jeteeHistos", "diffjetrate21ee", "Diff Jet Rate 2->1 (e channel)", djrElectronAxis);
      AddHistoGroupVsMulti<TH1D>(jets.size(), _djr_ee_32VsMulti, "jeteeHistos", "diffjetrate32ee", "Diff Jet Rate 3->2 (e channel)", djrElectronAxis);
      _histoJet_eeVsMulti[jets.size()]->fillCollection(jets, weight);
      if (found10) {
        _djr_ee_10->Fill(rate10, weight);
        _djr_ee_10VsMulti[jetsize]->Fill(rate10, weight);
      }
      if (found21) {
        _djr_ee_21->Fill(rate21, weight);
        _djr_ee_21VsMulti[jetsize]->Fill(rate21, weight);
      }
      if (found32) {
       _djr_ee_32->Fill(rate32, weight);
       _djr_ee_32VsMulti[jetsize]->Fill(rate32, weight);
      }

     }
   }
}


// ------------ method called once each job just before starting event loop  ------------
void 
ZjetsPlots::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ZjetsPlots::endJob() {
}


reco::NamedCompositeCandidate ZjetsPlots::fixRoles(const reco::NamedCompositeCandidate& orig, std::string baserole) const{
  reco::NamedCompositeCandidate::role_collection roles;
  reco::NamedCompositeCandidate cand = orig;
  if (cand.daughter(0)->threeCharge() > 0) {
    roles.push_back(baserole + "^{+}");
    roles.push_back(baserole + "^{-}");
  } else {
    roles.push_back(baserole + "^{-}");
    roles.push_back(baserole + "^{+}");
  }
  cand.setRoles(roles);
  cand.applyRoles();
  return cand;
}

//define this as a plug-in
DEFINE_FWK_MODULE(ZjetsPlots);
