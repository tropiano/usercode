#include "Firenze/Utilities/interface/TTreeService.h"
#include <iostream>

using namespace edm;

TTreeService::TTreeService (const ParameterSet& iPSet, ActivityRegistry& iRegistry){
  fs->file().cd();
  std::string treeName = iPSet.getParameter<std::string>("treeName");
  _tree = new TTree(treeName.c_str(), treeName.c_str()); 

  iRegistry.watchPreProcessEvent(this,&TTreeService::preEventProcessing);
  iRegistry.watchPostProcessEvent(this,&TTreeService::postEventProcessing);
  
}

void TTreeService::preEventProcessing(const edm::EventID&, const edm::Timestamp&) { }

void TTreeService::postEventProcessing(const Event&, const EventSetup&){
  //std::cout << "fill" << std::endl;
  _tree->Fill();
}

//declare this as a service
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"

//typedef edm::serviceregistry::AllArgsMaker<TTreeService> TTreeMaker;
//DEFINE_FWK_SERVICE_MAKER(TTreeService, TTreeMaker);
DEFINE_FWK_SERVICE(TTreeService);

