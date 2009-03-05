// -*- C++ -*-
//
// Package:    Firenze/Utilities
// Class:      ProvenanceTracer
// 
/**\class ProvenanceTracer ProvenanceTracer.cc Firenze/Utilities/plugins/ProvenanceTracer.cc

 Description: <one line class summary>
 trace provenance of specified products

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Piergiulio Lenzi,,,
//         Created:  Mon Jan 26 16:02:23 CET 2009
// $Id: ProvenanceTracer.cc,v 1.2 2009/01/27 08:08:59 lenzip Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/GenericHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/Entry.h"
#include "DataFormats/Provenance/interface/ProductRegistry.h"
#include "DataFormats/Provenance/interface/ProcessHistoryRegistry.h"


//
// class decleration
//

class ProvenanceTracer : public edm::EDAnalyzer {
   public:
      explicit ProvenanceTracer(const edm::ParameterSet&);
      ~ProvenanceTracer();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      void getProvenanceHistory(const edm::Provenance& prov, const edm::Event& iEvent);
      // ----------member data ---------------------------
      //std::vector<InputTag> _VinputTag;
      edm::InputTag _inputTag;
      std::vector<std::string> _vVetoProcess;
      const edm::pset::Registry * _registry;
      std::string spacer;
      std::vector<std::string> _scannedProducts;
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
ProvenanceTracer::ProvenanceTracer(const edm::ParameterSet& iConfig)

{
  //now do what ever initialization is needed
  _inputTag  = iConfig.getParameter<edm::InputTag>("Products"); 
  _vVetoProcess = iConfig.getParameter<std::vector<std::string> >("ExcludeProcesses");

}


ProvenanceTracer::~ProvenanceTracer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
ProvenanceTracer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;

  //const ProductRegistry& reg = iEvent.productRegistry();
  //edm::ProductRegistry reg;
  //reg.print(std::cout);

  typedef std::vector< Provenance const*> Provenances;
  Provenances provenances;
  iEvent.getAllProvenance(provenances);
  
  for(Provenances::iterator itProv = provenances.begin(); itProv != provenances.end(); ++itProv){ 

    string modlabel = (*itProv)->moduleLabel();
    string prodinst = (*itProv)->productInstanceName();
    string process  = (*itProv)->processName();
    if (!( ( _inputTag.label()    == "" ? true : _inputTag.label()    == modlabel) &&
           ( _inputTag.instance() == "" ? true : _inputTag.instance() == prodinst) &&
           ( _inputTag.process()  == "" ? true : _inputTag.process()  == process)    ) ) continue;

    cout << "product is " << modlabel << ":" << prodinst << ":" << process << endl;       

    GenericHandle pIn((*itProv)->className());
    iEvent.getByLabel(_inputTag, pIn);

    if (pIn.failedToGet()){
      edm::LogWarning("ProvenanceTracer") << "Could not find product " << _inputTag ;
      return;
    }
    _registry = edm::pset::Registry::instance();

    getProvenanceHistory(**itProv, iEvent);  
  
  }     
}

void ProvenanceTracer::getProvenanceHistory(const edm::Provenance& prov, const edm::Event& iEvent) {
    using namespace edm;
    using namespace std;
    //write out the parameters
    cout << spacer << "product: " << prov.moduleLabel() << ":" << prov.productInstanceName() << ":" << prov.processName() << endl;
    edm::ParameterSetID const& psetid = prov.product().psetID();
    edm::ParameterSet pset;
    bool found = _registry->getMapped(psetid, pset);
    if (!found) {
      throw cms::Exception("ProvenanceTracer") << "Unable to retrieve original pSet " << endl;
    }
    LogInfo("ProvenanceTracer") << pset << endl;
    string fullname=prov.moduleLabel()+":"+prov.productInstanceName()+":"+prov.processName();
    //cout << "registering " << fullname << endl;
    _scannedProducts.push_back(fullname);
    //look for parents
    std::vector<BranchID> const& parents = prov.parents();
    if (parents.empty()) return;
    cout << spacer << "-->" << endl;
    //spacer += "  ";
    for (std::vector<BranchID>::const_iterator ibID = parents.begin(); ibID != parents.end(); ++ibID){
      edm::Provenance parentProv = iEvent.getProvenance(*ibID);
      //if (parentProv.processName()=="RECO" || parentProv.processName()=="HLT") continue;
      //if (parentProv.processName()=="HLT") continue;
      //exclude selected processes 
      std::vector<string>::const_iterator posProc = std::find(_vVetoProcess.begin(), _vVetoProcess.end(),
                                                                 (string)parentProv.processName());
      if (posProc != _vVetoProcess.end()){
        //cout << "vetoing " << *posProc << endl;
        continue;
      }  
      
      string parentfullname = parentProv.moduleLabel()+":"+parentProv.productInstanceName()+":"+parentProv.processName();

      std::vector<string>::const_iterator posParent = std::find(_scannedProducts.begin(), _scannedProducts.end(),
                                                                       (string)parentfullname);

      if (posParent != _scannedProducts.end()) {
        //cout << "already considered " << parentfullname << endl;
        continue;
      }         
      
      
      getProvenanceHistory(parentProv, iEvent);
      //spacer.erase(spacer.end()-3, spacer.end());
    } 
}

// ------------ method called once each job just before starting event loop  ------------
void 
ProvenanceTracer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ProvenanceTracer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(ProvenanceTracer);
