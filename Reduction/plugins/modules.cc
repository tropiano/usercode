#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

typedef SingleObjectSelector<
          reco::GenParticleRefVector,
          StringCutObjectSelector<reco::GenParticleRef>
        > MyGenParticleRefSelector;

DEFINE_FWK_MODULE(MyGenParticleRefSelector);

