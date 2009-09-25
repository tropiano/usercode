#ifndef PFAnalyses_VBFHTauTau_FWLiteTSelector_h
#define PFAnalyses_VBFHTauTau_FWLiteTSelector_h
// -*- C++ -*-
//
// Class  : FWLiteTSelector    
// 
/**\class FWLiteTSelector 

 Description: A ROOT TSelector which accesses data using an edm::Event
              Version updated and adapted to FWLiteAnalysis framework.

*/
//
// Original Author:  Chris Jones
//         Created:  Tue Jun 27 16:37:27 EDT 2006
// Updated by Artur Kalinowski
//                   Fri Sep 18 13:54:16 CEST 2009
//       
//


#include <string>

// system include files
#include "TSelector.h"

// user include files
#include "boost/shared_ptr.hpp"

// forward declarations
class TFile;
class TList;
class TTree;
class TProofOutputFile;

class BackgroundWorker;

#include "DataFormats/FWLite/interface/ChainEvent.h"

template <class Worker>
class FWLiteTSelector : public TSelector
{

   public:
      FWLiteTSelector();
      virtual ~FWLiteTSelector();
      
   private:
      FWLiteTSelector(const FWLiteTSelector&); // stop default

      const FWLiteTSelector& operator=(const FWLiteTSelector&); // stop default

      virtual void        Begin(TTree *) ;
      virtual void        SlaveBegin(TTree *);
      virtual void        Init(TTree*);
      virtual Bool_t      Notify() ;
      virtual Bool_t      Process(Long64_t /*entry*/) ;
      virtual void        SlaveTerminate();
      virtual void        Terminate();
      virtual Int_t Version() const { return 1; }
      
      void setupNewFile(TFile&);
      // ---------- member data --------------------------------
      int tmp;
      int _lastEntry;
      bool everythingOK_;
      boost::shared_ptr<fwlite::ChainEvent> event_;
      TTree *tree_;

      boost::shared_ptr<Worker> _fwLiteWorker;
      //boost::shared_ptr<BackgroundWorker> _fwLiteTreeAnalyser;
      TProofOutputFile *_fProofFile;
      //TFile *_fFile;

};




// system include files
#include "TProofOutputFile.h"
#include "TFile.h"
#include "TTree.h"
#include "boost/shared_ptr.hpp"


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker> FWLiteTSelector<Worker>::FWLiteTSelector() : 
  everythingOK_(false), _fwLiteWorker() { std::cout << "Building a FWLiteTSelector " << std::endl;}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker> FWLiteTSelector<Worker>::~FWLiteTSelector(){}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker> void FWLiteTSelector<Worker>::Begin(TTree * iTree) {   
  Init(iTree); 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker> void FWLiteTSelector<Worker>::SlaveBegin(TTree *iTree) { 


  Init(iTree);

  _fProofFile = new TProofOutputFile("SimpleNtuple.root","LOCAL");

  //_fwLiteTreeAnalyser =  new BackgroundWorker(_fProofFile);
   _fwLiteWorker.reset(new Worker(_fProofFile, fInput));
   
  //_fwLiteTreeAnalyser->init(myAnalysers);

}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker> void FWLiteTSelector<Worker>::Init(TTree *iTree) { 
  std::cout<<"FWLiteTSelector::Init() "<<iTree<<std::endl;
  if(iTree==0) return;
  tree_ = iTree;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker> Bool_t FWLiteTSelector<Worker>::Notify() { 

  TFile* file = tree_->GetCurrentFile();
  setupNewFile(*file);
  return everythingOK_ ? kTRUE: kFALSE; 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker> Bool_t FWLiteTSelector<Worker>::Process(Long64_t iEntry) { 

  std::cout<<"iEntry: "<<iEntry<<std::endl;
  //float miliseconds = rand()/100000000.0;
  //std::cout<<"Will wait: "<<miliseconds<<" ms"<<std::endl;
  //sleep(miliseconds);
  //std::cout<<"Event valid?: "<<event_->isValid()<<std::endl;
  //const fwlite::ChainEvent& test = event_->to(iEntry);
  for(int i=_lastEntry;i<iEntry;++i){
     ++(*event_);
     //++tmp; 
  }
  
  //std::cout<<test.id()<<std::endl;
  //if(test.id().run()>0) _fwLiteTreeAnalyser->process(const_cast<fwlite::ChainEvent&>(test));
  _fwLiteWorker->process(const_cast<fwlite::ChainEvent&>(*event_));
  _lastEntry = iEntry;

  //_fwLiteTreeAnalyser->analyse(const_cast<fwlite::ChainEvent&>(event_->to(iEntry)));
  //_fwLiteTreeAnalyser->analyse(const_cast<fwlite::ChainEvent&>(++(*event_)));

  return kTRUE; 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker> void FWLiteTSelector<Worker>::SlaveTerminate() { 

  std::cout<<"ThingsTSelector::postProcessing()"<<std::endl;
  _fwLiteWorker->finalize();
  //delete _fwLiteTreeAnalyser;
 // Write the ntuple to the file
  fOutput->Add(_fProofFile);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker> void FWLiteTSelector<Worker>::Terminate() {}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker> void FWLiteTSelector<Worker>::setupNewFile(TFile& iFile) { 

  std::vector<std::string> fileVector;
  fileVector.push_back(iFile.GetName());

  std::cout<<"fileName: "<<iFile.GetName()<<std::endl;
  event_ = boost::shared_ptr<fwlite::ChainEvent>( new fwlite::ChainEvent(fileVector));
  std::cout<<"size: "<<event_->size()<<std::endl;
  everythingOK_ = true;

}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#endif
