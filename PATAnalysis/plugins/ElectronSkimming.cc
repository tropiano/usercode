#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Firenze/PATAnalysis/include/ElectronUtilities.h"

//CMSSW headers
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"

#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;
using namespace edm;

class ElectronSkim : public edm::EDFilter{
public :
  
   ElectronSkim(const edm::ParameterSet&);
    
   virtual ~ElectronSkim();

private:

   virtual void beginJob(const edm::ParameterSet& iConfig);
   virtual bool filter(edm::Event&, const edm::EventSetup&);
   virtual void endJob();
   
   bool _mass;
   
   double _masscut;
   
};

ElectronSkim::ElectronSkim(const edm::ParameterSet& iConfig):
_masscut(iConfig.getParameter<double>("masscut"))
{ }

void ElectronSkim::beginJob(const edm::ParameterSet& iConfig){

  _mass = false;
    
 }

ElectronSkim::~ElectronSkim(){
  
}

bool ElectronSkim::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   edm::Handle<std::vector<reco::CompositeCandidate> > zrecHandle;
   iEvent.getByLabel("zeerec", zrecHandle);

   edm::Handle<pat::TriggerEvent> triggerHandle;
   iEvent.getByLabel("patTriggerEvent", triggerHandle);
     
   _mass = false;
   
   for(unsigned int i = 0; i!=zrecHandle->size(); i++){
     
   if((*zrecHandle)[i].mass()>_masscut){
   _mass = true;}
   
   }
      
   return _mass;
 
 }
 
 void  ElectronSkim::endJob(){

}

DEFINE_FWK_MODULE(ElectronSkim);
