#ifndef RecoRapidityGaps_h
#define RecoRapidityGaps_h

#include "TFile.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "MyPatAnalysis.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "Firenze/PATAnalysis/include/RapGapPlots.h"
#include "Firenze/PATAnalysis/include/TagAndProbeFiller.h"

class RecoRapidityGaps : public MyPatAnalysis{
public :
   RecoRapidityGaps();
   
   virtual ~RecoRapidityGaps();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new RecoRapidityGaps(); } 

   //void  process(const fwlite::ChainEvent& iEvent);
   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

private:

  bool accept(const fwlite::Event& iEvent, std::string name) const;

  TFile* _file; 

  TDirectory* _dir;


  RapGapPlots<pat::Jet> _plots;

  bool _isMC;

  TagAndProbeFiller* _tpfiller2jet;
  TagAndProbeFiller* _tpfiller3jet;
  TH1F *_2jnum, *_2jden, *_3jnum, *_3jden, *_activity, *_jet15andactivity; 

};
#endif

