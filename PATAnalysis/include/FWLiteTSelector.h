#ifndef Firenze_PATAnalysis_FWLiteTSelector_h
#define Firenze_PATAnalysis_FWLiteTSelector_h
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
//#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Event.h"

// forward declarations
class TFile;
class TList;
class TTree;
class TProofOutputFile;

class DoNothingWorker {
  public:
  DoNothingWorker(TFile* file, const TList* fInput){}
   ~DoNothingWorker(){};

   //void  process(const fwlite::ChainEvent& iEvent){};
   void  process(const fwlite::Event& iEvent){};

   void finalize() {}

};




template <class Worker1, class Worker2=DoNothingWorker, class Worker3=DoNothingWorker, class Worker4=DoNothingWorker, class Worker5=DoNothingWorker >
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
      //boost::shared_ptr<fwlite::ChainEvent> event_;
      boost::shared_ptr<fwlite::Event> event_;
      TTree *tree_;

      boost::shared_ptr<Worker1> _fwLiteWorker1;
      boost::shared_ptr<Worker2> _fwLiteWorker2;
      boost::shared_ptr<Worker3> _fwLiteWorker3;
      boost::shared_ptr<Worker4> _fwLiteWorker4;
      boost::shared_ptr<Worker5> _fwLiteWorker5;
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
template <class Worker1, class Worker2, class Worker3, class Worker4, class Worker5> 
FWLiteTSelector<Worker1, Worker2, Worker3, Worker4, Worker5>::
FWLiteTSelector() : 
  everythingOK_(false), _fwLiteWorker1(), _fwLiteWorker2(), _fwLiteWorker3(), _fwLiteWorker4(), _fwLiteWorker5() 
{ std::cout << "Building a FWLiteTSelector " << std::endl;}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker1, class Worker2, class Worker3, class Worker4, class Worker5> 
FWLiteTSelector<Worker1, Worker2, Worker3, Worker4, Worker5>::
~FWLiteTSelector(){}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker1, class Worker2, class Worker3, class Worker4, class Worker5>
void
FWLiteTSelector<Worker1, Worker2, Worker3, Worker4, Worker5>::
Begin(TTree * iTree) {   
  Init(iTree); 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker1, class Worker2, class Worker3, class Worker4, class Worker5> 
void
FWLiteTSelector<Worker1, Worker2, Worker3, Worker4, Worker5>::
SlaveBegin(TTree *iTree) { 


  Init(iTree);
  TIter next(fInput);
  bool fileSet(false);
  while (TObject* obj = next()){
    TNamed* parFile   = dynamic_cast<TNamed*>(obj);
    if (parFile) {
      if (!strcmp(parFile->GetName(), "OutputFile")){
        _fProofFile = new TProofOutputFile(parFile->GetTitle(),"LOCAL");
        std::cout << "set output file to: " << parFile->GetTitle() << std::endl;
        fileSet = true;
      }  
    }
  }
  //check that everything was set       
  if (!fileSet) {
    std::cout << "You did not set the output file! " << std::endl;
    assert(fileSet);
  }

  TFile* unique = _fProofFile->OpenFile("RECREATE");

  //_fwLiteTreeAnalyser =  new BackgroundWorker(_fProofFile);
   _fwLiteWorker1.reset(new Worker1(unique, fInput));
   _fwLiteWorker2.reset(new Worker2(unique, fInput));
   _fwLiteWorker3.reset(new Worker3(unique, fInput));
   _fwLiteWorker4.reset(new Worker4(unique, fInput));
   _fwLiteWorker5.reset(new Worker5(unique, fInput));
   
  //_fwLiteTreeAnalyser->init(myAnalysers);

}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker1, class Worker2, class Worker3, class Worker4, class Worker5> 
void
FWLiteTSelector<Worker1, Worker2, Worker3, Worker4, Worker5>::
Init(TTree *iTree) { 
  std::cout<<"FWLiteTSelector::Init() "<<iTree<<std::endl;
  if(iTree==0) return;
  tree_ = iTree;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker1, class Worker2, class Worker3, class Worker4, class Worker5> 
Bool_t
FWLiteTSelector<Worker1, Worker2, Worker3, Worker4, Worker5>::
Notify() { 

  TFile* file = tree_->GetCurrentFile();
  setupNewFile(*file);
  return everythingOK_ ? kTRUE: kFALSE; 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker1, class Worker2, class Worker3, class Worker4, class Worker5> 
Bool_t
FWLiteTSelector<Worker1, Worker2, Worker3, Worker4, Worker5>::
Process(Long64_t iEntry) { 
/*
  for(int i=_lastEntry;i<iEntry;++i){
     ++(*event_);
     //++tmp; 
  }
*/  
  const fwlite::Event& test = event_->to(iEntry);
/*  
  //std::cout<<test.id()<<std::endl;
  //if(test.id().run()>0) _fwLiteTreeAnalyser->process(const_cast<fwlite::ChainEvent&>(test));
  _fwLiteWorker1->process(const_cast<fwlite::ChainEvent&>(*event_));
  _fwLiteWorker2->process(const_cast<fwlite::ChainEvent&>(*event_));
  _fwLiteWorker3->process(const_cast<fwlite::ChainEvent&>(*event_));
  _fwLiteWorker4->process(const_cast<fwlite::ChainEvent&>(*event_));
  _fwLiteWorker5->process(const_cast<fwlite::ChainEvent&>(*event_));
  _lastEntry = iEntry;
*/
  _fwLiteWorker1->process(const_cast<fwlite::Event&>(test));
  _fwLiteWorker2->process(const_cast<fwlite::Event&>(test));
  _fwLiteWorker3->process(const_cast<fwlite::Event&>(test));
  _fwLiteWorker4->process(const_cast<fwlite::Event&>(test));
  _fwLiteWorker5->process(const_cast<fwlite::Event&>(test)); 
  
  //_fwLiteTreeAnalyser->analyse(const_cast<fwlite::ChainEvent&>(event_->to(iEntry)));
  //_fwLiteTreeAnalyser->analyse(const_cast<fwlite::ChainEvent&>(++(*event_)));

  return kTRUE; 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker1, class Worker2, class Worker3, class Worker4, class Worker5> 
void
FWLiteTSelector<Worker1, Worker2, Worker3, Worker4, Worker5>::
SlaveTerminate() { 

  std::cout<<"ThingsTSelector::postProcessing()"<<std::endl;
  _fwLiteWorker1->finalize();
  _fwLiteWorker2->finalize();
  _fwLiteWorker3->finalize();
  _fwLiteWorker4->finalize();
  _fwLiteWorker5->finalize();
  //delete _fwLiteTreeAnalyser;
 // Write the ntuple to the file
  fOutput->Add(_fProofFile);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker1, class Worker2, class Worker3, class Worker4, class Worker5> 
void
FWLiteTSelector<Worker1, Worker2, Worker3, Worker4, Worker5>::
Terminate() {}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class Worker1, class Worker2, class Worker3, class Worker4, class Worker5> 
void
FWLiteTSelector<Worker1, Worker2, Worker3, Worker4, Worker5>::
setupNewFile(TFile& iFile) { 

  //std::vector<std::string> fileVector;
  //fileVector.push_back(iFile.GetName());

  std::cout<<"fileName: "<<iFile.GetName()<<std::endl;
  //event_ = boost::shared_ptr<fwlite::ChainEvent>( new fwlite::ChainEvent(fileVector));
  event_ = boost::shared_ptr<fwlite::Event>( new fwlite::Event(&iFile));
  std::cout<<"size: "<<event_->size()<<std::endl;
  everythingOK_ = true;

}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#endif
