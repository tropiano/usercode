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
// $Id: ProvenanceTracer.cc,v 1.1 2009/01/26 19:35:30 lenzip Exp $
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

      // ----------member data ---------------------------
      //std::vector<InputTag> _VinputTag;
      edm::InputTag _inputTag;
      std::vector<std::string> _vSrcNames;
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
  _vSrcNames = iConfig.getParameter<std::vector<std::string> >("ParentIdentifiers");

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

    GenericHandle pIn((*itProv)->className());
    iEvent.getByLabel(_inputTag, pIn);

    if (pIn.failedToGet()){
      edm::LogWarning("ProvenanceTracer") << "Could not find product " << _inputTag ;
      return;
    }

    //track jet provenance
    edm::Provenance const* prov = pIn.provenance();
    //std::cout << prov->product() << std::endl;
    edm::ParameterSetID const& psetid = prov->product().psetID();
    edm::ParameterSet pset;
    //pset registry
    const edm::pset::Registry * registry = edm::pset::Registry::instance();
    bool found = registry->getMapped(psetid, pset);
    if (!found) {
      throw cms::Exception("ProvenanceTracer") << "Unable to retrieve original pSet " << endl;
    }

    ProcessHistoryRegistry* phr = ProcessHistoryRegistry::instance();
    ProcessHistory ph;
    if (!phr->getMapped(iEvent.processHistoryID(), ph)){
      throw cms::Exception("ProvenanceTracer") << "Unable to find process history ";
    }


    LogInfo("ProvenanceTracer") << pset << std::endl;
    bool hasParentSource = true;
    while (hasParentSource){
      //check the possible src names, and take the first for the moment
      string src_name = "none";
      for (vector<string>::const_iterator iName = _vSrcNames.begin(); iName != _vSrcNames.end(); ++iName){
        if (pset.exists(*iName)) {
          src_name = *iName;
          break;
        }

      }
      /*if (!pset.exists("src")) {
        hasParentSource = false; 
        break;
      }*/
      if (src_name == "none") {
        hasParentSource = false; 
        break;
      }
      cout << "Parent source for " << pset.getParameter<string>("@module_label") << " exists with name ..." ;
      InputTag parentTag;
      try {
        parentTag = pset.getParameter<InputTag>(src_name);
        cout << parentTag.label() << endl;
      } catch (cms::Exception&) {
        throw cms::Exception("ProvenanceTracer") << "src parameter is not an InputTag, cannot handle this for the moment ";
        break;
      }
      bool foundParent = false;
      ParameterSet parentPset;
      //loop on all processes
      for (ProcessHistory::const_iterator iproc = ph.begin(); iproc != ph.end() && foundParent == false; ++iproc){
        //get the current process Config
        ProcessConfiguration currentProc = *iproc;  
        //get the current process whole Pset
        ParameterSet WholeProcessPset;
        bool foundwppset = registry->getMapped(currentProc.parameterSetID(), WholeProcessPset);
        if (!foundwppset){
          throw cms::Exception("ProvenanceTracer") << "Unable to find the process config for process " << currentProc.processName(); 
        }
        //get the parent module pset
        //ParameterSetEntry const* parentModulePSet = WholeProcessPset.retrieveParameterSet(parentTag.label());
        //if (parentModulePSet == 0) continue;
        if (!WholeProcessPset.exists(parentTag.label())) continue;

        parentPset = WholeProcessPset.retrieve(parentTag.label()).getPSet();

        cout << "Found parent Provenance info" << endl;
        foundParent = true; 
      }
      if (!foundParent) {
        throw cms::Exception("ProvenanceTracer") << "Unable to find parent configuation for module " << parentTag.label() << endl;
        break;
      }
      LogInfo("ProvenanceTracer") << "\nCorresponding source was generated according to: " << endl;
      LogInfo("ProvenanceTracer") << parentPset;
      pset = parentPset;
    }
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
