#ifndef __Status3MotherCombiner__
#define __Status3MotherCombiner__

/* \class Status3MotherCombiner
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
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/CandUtils/interface/AddFourMomenta.h"
#include "PhysicsTools/Utilities/interface/PtComparator.h"


class Status3MotherCombiner : public edm::EDProducer {
 public:
      
  Status3MotherCombiner(const edm::ParameterSet & ); 
  ~Status3MotherCombiner(); 
  // select object from a collection and 
  // possibly event content
  virtual void produce (edm::Event &evt, const edm::EventSetup &evtSetup);
  
 private:
  edm::InputTag _inTag;
  int _pdgId;
  int _charge;
  AddFourMomenta _combiner;
  GreaterByPt<reco::GenParticle>       pTComparator_;
   
  
};

#endif
