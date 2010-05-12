#ifndef PATAnalysis_RapGapPlots_h
#define PATAnalysis_RapGapPlots_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "Firenze/PATAnalysis/include/JetUtilities.h"

#include "TH1F.h"
#include "TProfile.h"

#include <vector>
#include <string>


template <class JET> bool sortByRapidity(const JET& j1, const JET& j2){ return j1.rapidity() < j2.rapidity(); }


template<class JET>
class RapGapPlots {
  public:
  RapGapPlots();

  ~RapGapPlots();

  void init(const edm::ParameterSet&);
  
  void fill(/*const fwlite::Event& iEvent*/);

  const std::vector<JET>& getJets() const;
  
  void cacheJets(const fwlite::Event& iEvent /*const std::vector<JET>& jets*/);
  
  private:
  
  
  TProfile *_avnjets, *_angular_decor;
  TH1F *_deltaY_2jets, *_deltaY_3jets;

  std::vector<JET> _selectedJets;
  bool _jetsStored;

  double _ptcut, _etamax;
  std::string _src;
  
};

template <class JET> 
RapGapPlots<JET>::RapGapPlots():_avnjets(0), _angular_decor(0), _deltaY_2jets(0), _deltaY_3jets(0), _jetsStored(false){}

template <class JET> 
RapGapPlots<JET>::~RapGapPlots(){
  if (_avnjets) delete _avnjets;
  if (_angular_decor) delete _angular_decor;
  if (_deltaY_2jets) delete _deltaY_2jets;
  if (_deltaY_3jets) delete _deltaY_3jets;
}

template <class JET> 
void RapGapPlots<JET>::init(const edm::ParameterSet& iConfig){
  _ptcut  = iConfig.getParameter<double>("MinPt");
  _etamax = iConfig.getParameter<double>("MaxEta");
  _src = iConfig.getParameter<std::string>("jetSource");

  _avnjets       = new TProfile("avnjets_VS_rapgap", "avnjets_VS_rapgap", 10, 0., 5.);
  _angular_decor = new TProfile("angulardecor_VS_rapgap", "angulardecor_VS_rapgap", 10, 0., 5.);

  _deltaY_2jets  = new TH1F("deltaY_2jets", "deltaY_2jets", 10, 0., 5.);
  _deltaY_3jets  = new TH1F("deltaY_3jets", "deltaY_3jets", 10, 0., 5.);
}

template <class JET>
const std::vector<JET>& RapGapPlots<JET>::getJets() const{
  if (!_jetsStored) {
    throw cms::Exception("RapGapPlots") << " Jets not stored for this event, you have to call RapGapPlots::fill before!  " << std::endl;
  }
  return _selectedJets;
}

template <> 
inline void RapGapPlots<pat::Jet>::cacheJets(const fwlite::Event& iEvent/*const std::vector<pat::Jet>& jets*/){
  //std::vector<const pat::Jet*> output;


  fwlite::Handle<std::vector<pat::Jet> > jetHandle;
  jetHandle.getByLabel(iEvent, _src.c_str());

  //if (_jetsStored) {
  //  throw cms::Exception("RapGapPlots") << " Jets already stored for this event!!!  " << std::endl;
  //}
  _selectedJets.clear();       


  std::vector<pat::Jet>::const_iterator ijet;
  std::vector<pat::Jet>::const_iterator beg = jetHandle->begin();
  std::vector<pat::Jet>::const_iterator end = jetHandle->end();
  
  
  for (ijet = beg; ijet != end; ++ijet){
    if ( (fabs(ijet->rapidity()) < _etamax) && (ijet->pt() > _ptcut) ){
      if (ijet->isCaloJet()){
        if (ijet->pt() > _ptcut && fabs(ijet->eta()) < _etamax  &&
            ijet->emEnergyFraction() > 0.01 &&
            ijet->jetID().fHPD < 0.98 &&
            ijet->jetID().n90Hits > 1) {
          _selectedJets.push_back(*ijet);
        }
      } else if (ijet->isPFJet()) {
        //tight PFJetId
        if ((ijet->chargedHadronEnergyFraction() > 0.  || fabs(ijet->eta()>2.4)) &&
            ijet->neutralHadronEnergyFraction() < 0.9 &&
            (ijet->chargedMultiplicity()         > 0.  || fabs(ijet->eta()>2.4)) &&
            ijet->chargedEmEnergyFraction()     < 1.  &&
            ijet->neutralEmEnergyFraction()     < 0.9   ){
          _selectedJets.push_back(*ijet);
        }  
      } else {
        throw cms::Exception("RapGapPlots") << " not a Calo or PF jet " << std::endl;  
      } 
    }
  }
  std::sort(_selectedJets.begin(), _selectedJets.end(), sortByRapidity<pat::Jet>);
  //check that at least one jet is matched to the trigger
  /*bool matched = false;
  for (ijet = beg; ijet != end; ++ijet){
    if (isJetMatched(*ijet)){
      matched = true;
      break;
    }
  }
  //if not matched, we don't consider any jet for this event
  if (!matched) _selectedJets.clear();*/

  _jetsStored = true;
}

template <>
inline void RapGapPlots<reco::GenJet>::cacheJets(const fwlite::Event& iEvent /*const std::vector<reco::GenJet>& jets*/){
  //std::vector<const reco::GenJet*> output;
  fwlite::Handle<std::vector<reco::GenJet> > jetHandle;
  jetHandle.getByLabel(iEvent, _src.c_str());

  //if (_jetsStored) {
  //  throw cms::Exception("RapGapPlots") << " Jets already stored for this event!!!  " << std::endl;
  //}
  _selectedJets.clear();
  //_jetsStored = false;

  std::vector<reco::GenJet>::const_iterator ijet;
  std::vector<reco::GenJet>::const_iterator beg = jetHandle->begin();
  std::vector<reco::GenJet>::const_iterator end = jetHandle->end();   
  for (ijet = beg; ijet != end; ++ijet){
    if ( (fabs(ijet->rapidity()) < _etamax) && (ijet->pt() > _ptcut) ){
      _selectedJets.push_back(*ijet);
    }
  }

  std::sort(_selectedJets.begin(), _selectedJets.end(), sortByRapidity<reco::GenJet>);
  _jetsStored = true;
}

template <class JET>
void RapGapPlots<JET>::fill(/*const fwlite::Event& iEvent*/){
  //_selectedJets.clear();
  //_jetsStored = false;
  if (!_jetsStored) {
    throw cms::Exception("RapGapPlots") << " you have to call cacheJets first  " << std::endl;
  }
  

  if (_selectedJets.size()>1){
    double deltaY      = fabs(_selectedJets.front().rapidity() - _selectedJets.back().rapidity());
    double delta_phi    = deltaPhi(_selectedJets.front().phi(), _selectedJets.back().phi());
    unsigned int njets = _selectedJets.size();
    _angular_decor->Fill(deltaY, cos(M_PI - delta_phi));
    _avnjets->Fill(deltaY, njets);
    if (njets == 2) _deltaY_2jets->Fill(deltaY);
    else  _deltaY_3jets->Fill(deltaY);
  }

  _jetsStored = false;
}


#endif
