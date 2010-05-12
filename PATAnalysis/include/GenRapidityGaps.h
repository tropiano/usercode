#ifndef GenRapidityGaps_h
#define GenRapidityGaps_h

#include "TFile.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "MyPatAnalysis.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "Firenze/PATAnalysis/include/RapGapPlots.h"

class GenRapidityGaps : public MyPatAnalysis{
public :
   GenRapidityGaps();
   
   virtual ~GenRapidityGaps();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new GenRapidityGaps(); } 

   //void  process(const fwlite::ChainEvent& iEvent);
   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

private:

  TFile* _file; 

  TDirectory* _dir;

  RapGapPlots<reco::GenJet> _plots;

};
#endif

