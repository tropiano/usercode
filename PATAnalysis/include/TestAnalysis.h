#ifndef TestAnalysis_h
#define TestAnalysis_h

#include "TFile.h"
#include "TH1F.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "MyPatAnalysis.h"

class TestAnalysis : public MyPatAnalysis{
public :
   TestAnalysis();
   
   virtual ~TestAnalysis();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new TestAnalysis(); } 

   //void  process(const fwlite::ChainEvent& iEvent);
   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

private:

  TFile* _file; 

  TDirectory* _dir;

  TH1F* _plot;

};
#endif

