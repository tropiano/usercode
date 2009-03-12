
#include "FWCore/Framework/interface/MakerMacros.h"
#include "Status3MotherCombiner.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include <iostream>
#include <memory>
#include "PhysicsTools/CandUtils/interface/pdgIdUtils.h"
#include "DataFormats/Candidate/interface/CompositeCandidateFwd.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "PhysicsTools/HepMCCandAlgos/interface/GenParticlesHelper.h"

using namespace std;
using namespace reco;

Status3MotherCombiner::Status3MotherCombiner(const edm::ParameterSet &params ):
  _inTag(params.getParameter<edm::InputTag>("src")),
  _pdgId(params.getParameter<int>("motherId")),
  _charge(params.getParameter<int>("motherCharge")),
  _combiner(params){
  
  produces <reco::CompositeCandidateCollection> ();
      
}

Status3MotherCombiner::~Status3MotherCombiner(){}

void Status3MotherCombiner::produce (edm::Event &evt, const edm::EventSetup &evtSetup){

  //cout << "CIAO" << endl;

 
  std::auto_ptr<reco::CompositeCandidateCollection> output (new reco::CompositeCandidateCollection);

  std::map<int,  CompositeCandidate* > mothers_products;
    
  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByLabel(_inTag, genParticles );
  const reco::GenParticleCollection& particles = *genParticles;
  
  unsigned int size = particles.size();
  
  for(unsigned int i = 0; i < size; i++) {
    //const reco::GenParticle *particle = particles[i];
    GenParticleRef particle( &particles, i );
    if (particle->pdgId()  != _pdgId) continue;
    if (particle->status() != 3) continue;
    //cout << "Particle " << particle->pdgId() << endl;
    reco::GenParticleRefVector descendents;
    GenParticlesHelper::findDescendents(particle, descendents, 1);
    //fill a vector with descents (needed for sort)
    std::map<int, unsigned int > id_Index;
    GenParticleCollection descendentsV;
    for (reco::GenParticleRefVector::const_iterator id = descendents.begin(); id != descendents.end(); ++id){
      descendentsV.push_back(**id);
      std::map<int, unsigned int >::const_iterator ifind = id_Index.find((*id)->pdgId());
      if (ifind == id_Index.end()) id_Index[(*id)->pdgId()]=0;
    }
    //sort descents in pt
    std::sort(descendentsV.begin(), descendentsV.end(), pTComparator_);
    CompositeCandidate mother(0, Particle::LorentzVector(0,0,0,0));
    reco::GenParticleCollection::const_iterator iter;
    CompositeCandidate::role_collection roles;
    for (iter = descendentsV.begin(); iter != descendentsV.end(); ++iter){
      //std::cout << "Adding daughter ID " << (*iter)->pdgId() << endl;
      //mother.addDaughter(**iter);
      mother.addDaughter(*iter);
      stringstream s;
      //s << (*iter)->pdgId() << "_" << c;
      s << iter->pdgId() << "_" << id_Index[iter->pdgId()];
      roles.push_back(s.str());
      id_Index[iter->pdgId()]++;
    } 
    _combiner.set(mother);
    mother.setPdgId(_pdgId);
    mother.setRoles(roles);
    //cout << "final momentum " << mother.momentum() << endl;
    output->push_back(mother);
    assert(mother.charge()==_charge);
  }

  evt.put(output);
}
DEFINE_FWK_MODULE(Status3MotherCombiner);      
      
