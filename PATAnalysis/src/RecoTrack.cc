
#include "Firenze/PATAnalysis/include/RecoTrack.h"
#include "Firenze/PATAnalysis/include/RecoJet.h"
#include "Firenze/PATAnalysis/include/MuonUtilities.h"

#include <TH2.h>
#include <TStyle.h>
#include <math.h>
//#include "Utilities.h"

#include "TList.h"
#include "TParameter.h"
#include "TFile.h"
#include "TProofOutputFile.h"

#include <iostream>

//CMSSW headers
#include "DataFormats/FWLite/interface/Handle.h"



using namespace std;
using namespace edm; 


RecoTrack::RecoTrack(): 
_all("All", " all Tracks"), 
_hp("hp", " High Purity tracks"), 
_pv("PV", " all tracks associated with PV"), 
_hppv("HPPV", " all HP tracks associated with PV"),
_nTrkAll(0), _nTrkPV(0), _nTrkHP(0),  _nMu(0),
_muonPt(0), _muonEta(0), _muonType(0), _muonTrkIso(0), _muonHcalIso(0), _muonEcalIso(0), _muonChi2(0), _muonNTkHits(0)
{ }

void RecoTrack::begin(TFile* out, const edm::ParameterSet& iConfig){
   _file = out; 
   std::string dirname = iConfig.getParameter<std::string>("Name");

   _ptcuttk  = iConfig.getParameter<double>("MinPtTk");
   _ptcutmu  = iConfig.getParameter<double>("MinPtMu");
   _ptcutjet  = iConfig.getParameter<double>("MinPtJet");
   _nhit    = iConfig.getParameter<double>("MinHit");
   _etamax = iConfig.getParameter<double>("MaxEta");

   //_file = proofFile->OpenFile("RECREATE");
   cout << "RecoTrack file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();
  
   _all.initialize();
   _hp.initialize();
   _pv.initialize();
   _hppv.initialize();

   _ntrackVsZ  = new TH2F("ntrackVsZ", "# tracks vs PV z", 200, -100., 100., 500, 0., 500.);
   _ntrackVsZ->Sumw2();
   _ntrackVsPz = new TH2F("ntrackVsPz", "# tracks vs PV total p_{z}", 100, 0, 10000, 500, 0., 500.);
   _ntrackVsPz->Sumw2();
   _muonPt = new TH1F("muonPt", "Muon p_{T}", 200, 0, 40); 
   _muonPt->Sumw2();
   _muonEta = new TH1F("muonEta", "Muon #eta", 100, -3., 3.);
   _muonEta->Sumw2();
   _muonType = new TH1F("muonType", "#muon type", 30, 0, 30);
   _muonType->Sumw2();
   _muonTrkIso = new TH1F("muonTrkIso", "Muon tracker isolation", 100, 0, 5);
   _muonTrkIso->Sumw2();
   _muonHcalIso = new TH1F("muonHcalIso", "Muon hcal isolation", 100, 0, 5);
   _muonHcalIso->Sumw2();
   _muonEcalIso = new TH1F("muonEcalIso", "Muon ecal isolation", 100, 0, 5);
   _muonEcalIso->Sumw2();
   _muonChi2 = new TH1F("muonChi2", "Muon #chi^{2}", 100, 0, 10);
   _muonChi2->Sumw2();
   _muonNTkHits = new TH1F("muonNTkHits", "number of tracker hits", 50, 0, 50);
   _muonNTkHits->Sumw2();
   _muonDxyPV   = new TH1F("muonDxyFromPV", "muon Dxy From PV", 1000, -10, 10);
   _muonDxyPV->Sumw2();
   _muonDzPV    = new TH1F("muonDzFromPV",  "muon Dz From PV",  1000, -10, 10);
   _muonDzPV->Sumw2();
   _nTrkAll = new TH1F("nTrkAll", "# all tracks per event", 150, 0, 150);
   _nTrkAll->Sumw2();
   _nTrkPV = new TH1F("nTrkPV", "# PV tracks per event", 150, 0, 150); 
   _nTrkPV ->Sumw2();
   _nTrkHP = new TH1F("nTrkHP", "# HP tracks per event", 150, 0, 150);   
   _nTrkHP ->Sumw2();
   _nMu  = new TH1F("nMu", "# muons per event", 10, 0, 10);
   _nMu->Sumw2(); 
   
  _dir->cd("-");
   
  cout << "RecoTrack Worker built." << endl;   
}

RecoTrack::~RecoTrack(){
  _file->ls();
  //_file->Write();
  //_file->Close();
}

//void  RecoTrack::process(const fwlite::ChainEvent& iEvent)
void  RecoTrack::process(const fwlite::Event& iEvent)
{

   _file->cd();

   fwlite::Handle<std::vector<pat::Jet> > jetHandle;
   jetHandle.getByLabel(iEvent, "selectedJets");

   fwlite::Handle<std::vector<reco::Track> > trackHandle;
   trackHandle.getByLabel(iEvent, "generalTracks");

   fwlite::Handle<std::vector<reco::Vertex> > vertexHandle;
   vertexHandle.getByLabel(iEvent, "offlinePrimaryVertices");

   fwlite::Handle<std::vector<pat::Muon> > muonHandle;
   muonHandle.getByLabel(iEvent, "selectedMuons");
   

   bool validVertex = vertexHandle->size() && vertexHandle->front().isValid() && vertexHandle->front().tracksSize() > 1; 
   
   if (validVertex){
      _ntrackVsZ->Fill(vertexHandle->front().position().z(), trackHandle->size());  
   }

   //we select events with at least one valid vertex and less than 100 tracks  
   if (!validVertex || trackHandle->size() > 100 ) return;
   //if (!validVertex ) return;
  
  
  
   //we loop ovet the tracks associated to the PV and fill histograms 
   reco::Vertex::Point p = vertexHandle->front().position();
   reco::Vertex::trackRef_iterator ir;
   reco::Vertex::trackRef_iterator beginr = vertexHandle->front().tracks_begin();
   reco::Vertex::trackRef_iterator endr   = vertexHandle->front().tracks_end();
   unsigned int countPV = 0;
   for (ir = beginr; ir != endr; ++ir ){
     if ((*ir)->pt() > _ptcuttk){ 
        _pv.fillWithPV(**ir, p);
        countPV++;  
        if ((*ir)->quality(reco::Track::qualityByName("highPurity"))){
          _hppv.fillWithPV(**ir, p);
        }
     } 
   }
   _nTrkPV->Fill(countPV);

   //we loop on all tracks in the event and fill histograms
   std::vector<reco::Track>::const_iterator it;
   std::vector<reco::Track>::const_iterator begin = trackHandle->begin();
   std::vector<reco::Track>::const_iterator end   = trackHandle->end();
   unsigned int countHP  = 0;
   unsigned int countAll = 0;
   for ( it = begin; it != end; ++it ){
      if (it->pt() > _ptcuttk){
        countAll++;
        _all.fillWithPV(*it, p);
        if(it->quality(reco::Track::qualityByName("highPurity"))){
          _hp.fillWithPV(*it, p);
          countHP++;
        }
      }  
   }
   _nTrkAll->Fill(countAll);
   _nTrkHP->Fill(countHP);

   //_nMu->Fill(muonHandle->size());

   //count jets 
   std::vector<pat::Jet>::const_iterator ijet;
   std::vector<pat::Jet>::const_iterator jetbeg = jetHandle->begin();
   std::vector<pat::Jet>::const_iterator jetend = jetHandle->end();
   unsigned int countJet = 0;
   for (ijet = jetbeg; ijet != jetend; ++ijet){
     if (ijet->pt() > _ptcutjet && fabs(ijet->eta()) < _etamax  &&
         ijet->emEnergyFraction() > 0.01 &&
         ijet->jetID().fHPD < 0.98 &&
         ijet->jetID().n90Hits > 1) {
        countJet++;
     }
   } 

   unsigned int countMu = 0; 

   std::vector<pat::Muon>::const_iterator imu;
   std::vector<pat::Muon>::const_iterator mubeg = muonHandle->begin();
   std::vector<pat::Muon>::const_iterator muend = muonHandle->end();
   for (imu = mubeg; imu != muend; ++imu){
     if (! ((imu->isGlobalMuon() || imu->isGood("TMOneStationLoose")) && imu->pt() > _ptcutmu)) continue;
     countMu++;
     _muonPt->Fill(imu->pt());
     _muonEta->Fill(imu->eta());
     //_muonChi2->Fill(imu->globalTrack()->normalizedChi2());
     _muonType->Fill(imu->type());
     const reco::MuonIsolation& iso03 = imu->isolationR03();
     _dir->cd();
     addHistosVsMulti(countJet, "TrkIso", "TrkIso", 100, 0., 5., _trkIsoVsInclusiveMulti);
     addHistosVsMulti(countJet, "HcalIso", "HcalIso", 100, 0., 5., _hcalIsoVsInclusiveMulti);
     addHistosVsMulti(countJet, "EcalIso", "EcalIso", 100, 0., 5., _ecalIsoVsInclusiveMulti);
     _muonTrkIso->Fill(iso03.sumPt);
     _muonHcalIso->Fill(iso03.hadEt);
     _muonEcalIso->Fill(iso03.emEt);
     for (unsigned int kk = 0; kk <= countJet; ++kk){
        _trkIsoVsInclusiveMulti[kk]->Fill(iso03.sumPt);
        _hcalIsoVsInclusiveMulti[kk]->Fill(iso03.hadEt);
        _ecalIsoVsInclusiveMulti[kk]->Fill(iso03.emEt);
     }
     if (imu->innerTrack().isNonnull()){
        _muonNTkHits->Fill(imu->innerTrack()->found());
        _muonChi2->Fill(imu->innerTrack()->normalizedChi2());
        _muonDxyPV->Fill(imu->innerTrack()->dxy(p));
        _muonDzPV->Fill(imu->innerTrack()->dz(p));
     } 
   }
   _nMu->Fill(countMu);
}

void RecoTrack::finalize(){
  _file->Write();
  //_file->Close();
}

namespace myanalysis{
  TrackPlots::TrackPlots(std::string name, std::string title) : _name(name), _title(title), _initialized(false){}

  void TrackPlots::initialize(){
    std::string fullname  = "pt_"+_name;
    std::string fulltitle = "pt_"+_title;
    _pt   = new TH1F(fullname.c_str(), fulltitle.c_str(), 200, 0, 40);
    _pt->Sumw2();
    fullname  = "eta_"+_name;
    fulltitle = "eta_"+_title;  
    _eta  = new TH1F(fullname.c_str(), fulltitle.c_str(), 100, -2.5, 2.5);
    _eta->Sumw2();
    fullname   = "dxyFromPV_"+_name;
    fulltitle  = "dxyFromPV_"+_title;
    _dxyFromPV = new TH1F(fullname.c_str(), fulltitle.c_str(), 1000, -10, 10);
    _dxyFromPV->Sumw2();
    fullname  = "dzFromPV_"+_name;
    fulltitle = "dzFromPV_"+_title;
    _dzFromPV  = new TH1F(fullname.c_str(), fulltitle.c_str(), 1000, -20, 20);
    _dzFromPV->Sumw2();
    fullname  = "nhits_"+_name;
    fulltitle = "nhits_"+_title;
    _nhits = new TH1F(fullname.c_str(), fulltitle.c_str(), 50, 0, 50);
    _nhits->Sumw2();
    fullname  = "phi_"+_name;
    fulltitle = "phi_"+_title;
    _phi = new TH1F(fullname.c_str(), fulltitle.c_str(), 100, 0., 4.);
    _phi->Sumw2();
    fullname  = "chi2_"+_name;
    fulltitle = "chi2_"+_title;
    _chi2 = new TH1F(fullname.c_str(), fulltitle.c_str(), 100, 0., 10.);
    _chi2->Sumw2(); 
    
    _initialized = true;
  }

  TrackPlots::~TrackPlots(){
    if (_initialized){
      delete _pt;
      delete _eta;
      delete _dxyFromPV;
      delete _dzFromPV;
      delete _nhits;
      delete _phi;
    }  
  }

  void TrackPlots::fill(const reco::Track& track) {
    if (!_initialized) throw cms::Exception("TrackPlots") << "uninitialized histo ";
    _pt->Fill(track.pt());
    _eta->Fill(track.eta());
    _nhits->Fill(track.found());
    _phi->Fill(track.phi());
    _chi2->Fill(track.normalizedChi2());
  }
 
  void TrackPlots::fillWithPV(const reco::Track& track, const reco::Vertex::Point& pv){
    fill(track);
    _dxyFromPV->Fill(track.dxy(pv));
    _dzFromPV ->Fill(track.dz(pv));
  }
    
}
