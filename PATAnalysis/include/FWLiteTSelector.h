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
#include <fstream>
// system include files
#include "TSelector.h"
#include "TFileMerger.h"
//#include "TFileCollection.h"

// user include files
#include "boost/shared_ptr.hpp"
//#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "RegisteredAnalyses.h"
#include "Firenze/PATAnalysis/include/tclap/CmdLine.h"
#include "Utilities.h"

// forward declarations
class TFile;
class TList;
class TTree;
class TProofOutputFile;

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

      std::vector<boost::shared_ptr<MyPatAnalysis> > _workers;
      //boost::shared_ptr<BackgroundWorker> _fwLiteTreeAnalyser;
      TProofOutputFile *_fProofFile;
      //TFile *_fFile;

};

#endif
