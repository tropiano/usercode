#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

typedef SingleObjectSelector<
          reco::GenParticleRefVector,
          StringCutObjectSelector<reco::GenParticleRef>
        > MyGenParticleRefSelector;

DEFINE_FWK_MODULE(MyGenParticleRefSelector);

