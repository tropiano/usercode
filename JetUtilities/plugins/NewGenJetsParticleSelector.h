#ifndef __NewGenJetsParticleSelector__
#define __NewGenJetsParticleSelector__

/* \class NewGenJetsParticleSelector
 *
 adaptation from InputGenJetParticleSelector to select on status 3 particles
 * 
 * 
 */

#include "DataFormats/Provenance/interface/Provenance.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Common/interface/EDProductfwd.h"
#include "RecoParticleFlow/PFRootEvent/interface/JetRecoTypes.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class NewGenJetsParticleSelector : public edm::EDProducer {
  // collection type
 public:
  typedef std::vector<bool>     ParticleBitmap;
  typedef std::vector<const reco::GenParticle*> ParticleVector;
      
  NewGenJetsParticleSelector(const edm::ParameterSet & ); 
  ~NewGenJetsParticleSelector(); 
  // select object from a collection and 
  // possibly event content
  virtual void produce (edm::Event &evt, const edm::EventSetup &evtSetup);
      
  bool getPartonicFinalState() const { return partonicFinalState; }
  bool getExcludeResonances() const { return excludeResonances; }
  bool getTausAndJets() const { return tausAsJets; }
  double getPtMin() const { return ptMin; }
  const std::vector<unsigned int> &getIgnoredParticles() const
    { return ignoreParticleIDs; }
  
  void setPartonicFinalState(bool flag = true)
  { partonicFinalState = flag; }
  void setExcludeResonances(bool flag = true)
  { excludeResonances = flag; }
  void setTausAsJets(bool flag = true) { tausAsJets = flag; }
  void setPtMin(double ptMin) { this->ptMin = ptMin; }
  bool isParton(int pdgId) const;
  static bool isHadron(int pdgId);
  static bool isResonance(int pdgId);

  bool isIgnored(int pdgId) const;
  bool isCheckForStatus3Mother(int pdgId) const;
  bool hasPartonChildren(ParticleBitmap &invalid,
			 const ParticleVector &p,
			 const reco::GenParticle *particle) const;
  
  enum ResonanceState {
    kNo = 0,
    kDirect,
    kIndirect
  };
  ResonanceState fromResonance(ParticleBitmap &invalid,
			       const ParticleVector &p,
			       const reco::GenParticle *particle) const;
 
  bool hasStatus3Mother(const reco::GenParticle *particle) const;
  
  // iterators over selected objects: collection begin
  
 private:
  //container selected_;  //container required by selector
  NewGenJetsParticleSelector(){} //should not be used!
  
  edm::InputTag inTag;
  int testPartonChildren(ParticleBitmap &invalid,
			 const ParticleVector &p,
			 const reco::GenParticle *particle) const;
  
  std::vector<unsigned int>	ignoreParticleIDs;
  std::vector<unsigned int> excludeFromResonancePids;
  std::vector<unsigned int> checkForStatus3MotherIDs;
  void setExcludeFromResonancePids(const std::vector<unsigned int> &particleIDs);
  void setIgnoredParticles(const std::vector<unsigned int> &particleIDs);
  void setCheckForStatus3Mother(const edm::ParameterSet& config);
  bool isExcludedFromResonance(int pdgId) const;
  
  bool			partonicFinalState;
  bool			excludeResonances;
  bool			tausAsJets;
  bool                  excludeFromStatus3Mother;
  bool 	                checkForStatus3Mother_sameflavour;
  double		ptMin;
  bool                  reverseAllSelections;
  
  
  
};

#endif
