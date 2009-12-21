
#include "Firenze/PATAnalysis/include/RecoJet.h"
#include "Firenze/PATAnalysis/include/Utilities.h"

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
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

using namespace std;
using namespace edm; 


RecoJet::RecoJet(): 
_jetplots("Jets", " jets"),
_sumEt(0), _metPx(0), _metPy(0), _met(0), _nJet(0)
{ }

void RecoJet::begin(TFile* out, const edm::ParameterSet& iConfig){
   _file = out; 
   std::string dirname = iConfig.getParameter<std::string>("Name");

   _ptcut  = iConfig.getParameter<double>("MinPt");
   _etamax = iConfig.getParameter<double>("MaxEta");

   //_file = proofFile->OpenFile("RECREATE");
   cout << "RecoJet file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();
  
   _jetplots.initialize();

   _sumEt = new TH1F("sumEt", "event sumEt", 100, 0., 100.);
   _sumEt->Sumw2();
   _metPx = new TH1F("metPx", "event MET Px", 100, -10., 10.);
   _metPx->Sumw2();
   _metPy = new TH1F("metPy", "event MET Py", 100, -10., 10.);
   _metPy->Sumw2();
   _met   = new TH1F("met",   "event MET",   100, 0., 10.); 
   _met->Sumw2();
   _nJet = new TH1F("nJet", "# jets", 20, 0, 20);
   _nJet->Sumw2();
  
   
  _dir->cd("-");
   
  cout << "RecoJet Worker built." << endl;   
}

RecoJet::~RecoJet(){
  _file->ls();
}

//void  RecoJet::process(const fwlite::ChainEvent& iEvent)
void  RecoJet::process(const fwlite::Event& iEvent)
{

   _file->cd();

   fwlite::Handle<std::vector<pat::Jet> > jetHandle;
   jetHandle.getByLabel(iEvent, "selectedJets");

   fwlite::Handle<std::vector<pat::MET> > metHandle;
   metHandle.getByLabel(iEvent, "layer1METs");

   fwlite::Handle<std::vector<reco::Vertex> > vertexHandle;
   vertexHandle.getByLabel(iEvent, "offlinePrimaryVertices");

   fwlite::Handle<std::vector<reco::Track> > trackHandle;
   trackHandle.getByLabel(iEvent, "generalTracks");

   bool validVertex = vertexHandle->size() && 
                      vertexHandle->front().isValid() && 
                      vertexHandle->front().tracksSize() > 1; 
   
   //we select events with at least one valid vertex and less than 100 tracks  
   if (!validVertex || trackHandle->size() > 100 ) return;
   //if (!validVertex ) return;

  
   std::vector<pat::Jet>::const_iterator ijet;
   std::vector<pat::Jet>::const_iterator jetbeg = jetHandle->begin();
   std::vector<pat::Jet>::const_iterator jetend = jetHandle->end();
   int countJet = 0;
   for (ijet = jetbeg; ijet != jetend; ++ijet){
     if (ijet->pt() > _ptcut && fabs(ijet->eta()) < _etamax  && ijet->emEnergyFraction() > 0.01 && ijet->energyFractionHadronic() < 0.98 ) {
        _jetplots.fill(*ijet);
        countJet++;
     }
   }
   _nJet->Fill(countJet);

   _sumEt->Fill(metHandle->front().sumEt());
   _metPx->Fill(metHandle->front().px());
   _metPy->Fill(metHandle->front().py());
   _met  ->Fill(metHandle->front().et());

}

void RecoJet::finalize(){
  _file->Write();
}

namespace myanalysis{
  JetPlots::JetPlots(std::string name, std::string title) : _name(name), _title(title), _initialized(false){}

  void JetPlots::initialize(){
    std::string fullname  = "pt_"+_name;
    std::string fulltitle = "pt_"+_title;
    _pt   = new TH1F(fullname.c_str(), fulltitle.c_str(), 100, 0, 30);
    _pt->Sumw2();
    fullname  = "eta_"+_name;
    fulltitle = "eta_"+_title;  
    _eta  = new TH1F(fullname.c_str(), fulltitle.c_str(), 100, -5, 5);
    _eta->Sumw2();
    fullname  = "nTrk_"+_name;
    fulltitle = "nTrk_"+_title;
    _nTrk  = new TH1F(fullname.c_str(), fulltitle.c_str(), 50, 0, 50);
    _nTrk->Sumw2();
    fullname  = "phi_"+_name;
    fulltitle = "phi_"+_title;
    _phi = new TH1F(fullname.c_str(), fulltitle.c_str(), 100, 0., 4.);
    _phi->Sumw2();
    fullname  = "emf_"+_name;
    fulltitle = "emf_"+_title;
    _emf = new TH1F(fullname.c_str(), fulltitle.c_str(), 110, 0., 1.1);
    _emf->Sumw2();
    _initialized = true;
  }

  JetPlots::~JetPlots(){
    if (_initialized){
      delete _pt;
      delete _eta;
      delete _nTrk;
      delete _phi;
      delete _emf;
    }  
  }

  void JetPlots::fill(const pat::Jet& jet) {
    if (!_initialized) throw cms::Exception("JetPlots") << "uninitialized histo ";
    _pt->Fill(jet.pt());
    _eta->Fill(jet.eta());
    _nTrk->Fill(jet.associatedTracks().size());
    _phi->Fill(jet.phi());
    _emf->Fill(jet.emEnergyFraction());
  }
 
}
