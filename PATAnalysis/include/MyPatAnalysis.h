#ifndef FirenzeMyPatAnalysis
#define FirenzeMyPatAnalysis

#include "TFile.h"
#include "TList.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class MyPatAnalysis {
  public:
   //virtual void begin(TFile* out, TList* list)  = 0;
   virtual void begin(TFile* out, const edm::ParameterSet&)  = 0;

   virtual void  process(const fwlite::Event& iEvent) = 0;

   virtual void finalize() = 0;
};

#endif
