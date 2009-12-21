#ifndef RecoJet_h
#define RecoJet_h

#include "TFile.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

namespace myanalysis {
  class JetPlots {
    public:
    JetPlots(std::string name, std::string title);

    ~JetPlots();
    void initialize();
    void fill(const pat::Jet& jet);

    private:
    std::string _name, _title;
    bool _initialized;
    TH1F *_pt, *_eta, *_nTrk, *_emf, *_phi;
  }; 
}

class RecoJet : public MyPatAnalysis{
public :
   RecoJet();
   
   virtual ~RecoJet();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new RecoJet(); } 

   //void  process(const fwlite::ChainEvent& iEvent);
   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

private:

  TFile* _file; 

  TDirectory* _dir;

  double _ptcut, _etamax;

  myanalysis::JetPlots _jetplots;

  TH1F *_sumEt, *_metPx, *_metPy, *_met, *_nJet;
};
#endif

