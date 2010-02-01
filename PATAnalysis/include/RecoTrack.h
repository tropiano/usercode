#ifndef RecoTrack_h
#define RecoTrack_h

#include "TFile.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

namespace myanalysis {
  class TrackPlots {
    public:
    TrackPlots(std::string name, std::string title);

    ~TrackPlots();
    void initialize();
    void fill(const reco::Track& track);
    void fillWithPV(const reco::Track& track, const reco::Vertex::Point& pv);

    private:
    std::string _name, _title;
    bool _initialized;
    TH1F *_pt, *_eta, *_dxyFromPV, *_dzFromPV, *_nhits, *_phi, *_chi2;
  }; 
}

class RecoTrack : public MyPatAnalysis{
public :
   RecoTrack();
   
   virtual ~RecoTrack();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new RecoTrack(); } 

   //void  process(const fwlite::ChainEvent& iEvent);
   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

private:

  TFile* _file; 

  TDirectory* _dir;

  double _ptcuttk, _ptcutmu, _ptcutjet, _nhit, _etamax;

  myanalysis::TrackPlots _all, _hp, _pv, _hppv;

  TH2F *_ntrackVsPz, *_ntrackVsZ;
  TH1F *_nTrkAll, *_nTrkPV, *_nTrkHP, *_nMu;  
  TH1F *_muonPt, *_muonEta, *_muonType, *_muonTrkIso, *_muonHcalIso, *_muonEcalIso, 
       *_muonChi2, *_muonNTkHits, *_muonDxyPV, *_muonDzPV;
  std::vector<TH1D*> _trkIsoVsInclusiveMulti;     
  std::vector<TH1D*> _hcalIsoVsInclusiveMulti;     
  std::vector<TH1D*> _ecalIsoVsInclusiveMulti;     
       
};
#endif

