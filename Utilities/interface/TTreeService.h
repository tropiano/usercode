#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//ROOT includes
#include "TTree.h"
#include "TFile.h"

#include <string>
#include <map>

namespace edm {
  class EventID;
  class Timestamp;
  class Event;
  class EventSetup;

    class TTreeService {
      public:
       TTreeService(const ParameterSet& iPSet, ActivityRegistry& iRegistry);
       ~TTreeService(){ }//delete _tree; _tree=0;}

       void preEventProcessing(const edm::EventID&, const edm::Timestamp&);
       void postEventProcessing(const Event&, const EventSetup&);

       TTree* tree() {return _tree;}

      private:
        TTree* _tree;
        edm::Service<TFileService> fs;
    };
}
