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
// $Id: ZjetsPlots.cc,v 1.4 2009/03/24 08:33:28 lenzip Exp $
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
//#include "DataFormats/Candidate/interface/NamedCompositeCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
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
      
      // ----------member data ---------------------------
      // Histogram server
      edm::Service<TFileService> fs;
      edm::Service<edm::TTreeService> trees;

      // Histogram objects that make "standard" plots for each object
      pat::HistoComposite* _histoZ;
      pat::HistoJet      * _histoJet;
      std::vector<pat::HistoComposite*> _histoZVsMulti;
      std::vector<pat::HistoJet*> _histoJetVsMulti;


      TH1D* _djr_10;
      TH1D* _djr_21;
      TH1D* _djr_32;
      std::vector<TH1D*> _djr_10VsMulti;
      std::vector<TH1D*> _djr_21VsMulti;
      std::vector<TH1D*> _djr_32VsMulti;

      //TTree* _tree;

      //TH1D* h_nJets;

      //configuration   
      pat::PatKitHelper _configHelper;
      edm::InputTag _ztag ;
      edm::InputTag _tagrate10;
      edm::InputTag _tagrate21;
      edm::InputTag _tagrate32;
      edm::InputTag _weightTag;
      edm::InputTag _jettag;
      bool     _hasWeightProducer; 

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
   fs->file().ls();
   fs->file().pwd();
   //_tree = fs->make<TTree>("tree", "tree"); 
   fs->file().cd();
   //_tree = new TTree("tree", "tree");

   //now do what ever initialization is needed
   _ztag  = iConfig.getParameter<edm::InputTag>("ZSource");
   _tagrate10 = iConfig.getParameter<edm::InputTag>("DiffJetRate10");
   _tagrate21 = iConfig.getParameter<edm::InputTag>("DiffJetRate21");
   _tagrate32 = iConfig.getParameter<edm::InputTag>("DiffJetRate32");
   _jettag    = iConfig.getParameter<edm::InputTag>("JetSource");

   _modLabel = iConfig.getParameter<std::string>("@module_label");
  
   _histoZ = createHistograms<pat::HistoComposite>("Z"+_modLabel, "Z " + _ztag.label(), "Z", 
                                                         _configHelper.getAxisLimits("Z"));
   fs->file().pwd();                                                      
   _histoJet = createHistograms<pat::HistoJet>("jet"+_modLabel, "Jet " + _ztag.label(), "Jet", 
                                                         _configHelper.getAxisLimits("Jet"));
   fs->file().pwd();                                                      

   std::string name = "jet"+_modLabel;

   TFileDirectory misc1 = TFileDirectory(fs->mkdir(name.c_str()));
   PhysicsHistograms::KinAxisLimits djrAxis = _configHelper.getAxisLimits("diffJetRate");
   _djr_10 = misc1.make<TH1D>("diffjetrate10", "Diff Jet Rate 1->0 ", 
                                    djrAxis.nBinsPt, djrAxis.pt1, djrAxis.pt2);   
   _djr_21 = misc1.make<TH1D>("diffjetrate21", "Diff Jet Rate 2->1 ", 
                                    djrAxis.nBinsPt, djrAxis.pt1, djrAxis.pt2);   
   _djr_32 = misc1.make<TH1D>("diffjetrate32", "Diff Jet Rate 3->2 ", 
                                    djrAxis.nBinsPt, djrAxis.pt1, djrAxis.pt2);   


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

    for (std::vector<pat::HistoComposite*>::const_iterator i = _histoZVsMulti.begin(); 
        i != _histoZVsMulti.end(); ++i){
      delete *i; 
    }     
    for (std::vector<pat::HistoJet*>::const_iterator i = _histoJetVsMulti.begin();
        i != _histoJetVsMulti.end(); ++i){
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

   _histoZ->clearTreeVariables();
   _histoJet->clearTreeVariables();

   // get the W candidates
   Handle<reco::CompositeCandidateCollection> ZHandle;
   iEvent.getByLabel(_ztag, ZHandle);
   const reco::CompositeCandidateCollection& Zs = *ZHandle;

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
   //Handle<std::vector<pat::Jet> > jetHandle;
   Handle<edm::View<reco::Jet> > jetHandle;
   iEvent.getByLabel(_jettag, jetHandle);   
   //const std::vector<pat::Jet>& alljets = *jetHandle;
   const edm::View<reco::Jet>& alljets = *jetHandle;

   //remove jets overlapping with electrons
   //std::vector<pat::Jet> jets;
   std::vector<const reco::Jet*> jets;
   //std::vector<pat::Jet>::const_iterator ijet;
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
    
   if (Zs.size() == 1){
     CompositeCandidate cand = fixRoles(Zs.front());
     //std::cout << "Z pt " << cand.pt() << std::endl;



     //bool isolatedMuons = checkIsolation<pat::Muon>(cand);
     bool isolatedMuons = true;

     if (!isolatedMuons) throw cms::Exception("ZjetsPlots") << "ERROR: isolation check failed " << std::endl;

     if (isolatedMuons){
      // fill W histos
      _histoZ->fill(cand);

      AddHistoGroupVsMulti<pat::HistoComposite>(jets.size(), _histoZVsMulti, 
                                                "Z"+_modLabel, "Z " + _ztag.label() , "Z", 
                                                _configHelper.getAxisLimits("Z"));
      _histoZVsMulti[jetsize]->fill(cand);

      // fill the jet histos
      _histoJet->fillCollection(jets, weight);

    

      AddHistoGroupVsMulti<pat::HistoJet>(jets.size(), _histoJetVsMulti, 
                                          "jet"+_modLabel, "Jet " + _ztag.label(), "Jet", 
                                          _configHelper.getAxisLimits("Jet"));
  
      PhysicsHistograms::KinAxisLimits djrAxis = _configHelper.getAxisLimits("diffJetRate");
      AddHistoGroupVsMulti<TH1D>(jets.size(), _djr_10VsMulti, 
                                 "jetHistos", "diffjetrate10", "Diff Jet Rate 1->0", 
                                 djrAxis);
      AddHistoGroupVsMulti<TH1D>(jets.size(), _djr_21VsMulti, 
                                 "jetHistos", "diffjetrate21", "Diff Jet Rate 2->1", 
                                 djrAxis);
      AddHistoGroupVsMulti<TH1D>(jets.size(), _djr_32VsMulti, 
                                 "jetHistos", "diffjetrate32", "Diff Jet Rate 3->2", 
                                 djrAxis);
      _histoJetVsMulti[jets.size()]->fillCollection(jets, weight); 
      if (found10) {
        _djr_10->Fill(rate10, weight);
        _djr_10VsMulti[jetsize]->Fill(rate10, weight);   
      }
      if (found21) {
        _djr_21->Fill(rate21, weight);
        _djr_21VsMulti[jetsize]->Fill(rate21, weight);
      }
      if (found32) {
        _djr_32->Fill(rate32, weight);
        _djr_32VsMulti[jetsize]->Fill(rate32, weight);
      }
     }
     //_tree->Fill();
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

//define this as a plug-in
DEFINE_FWK_MODULE(ZjetsPlots);
