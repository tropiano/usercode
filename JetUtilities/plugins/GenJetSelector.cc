#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "DataFormats/JetReco/interface/GenJet.h"

typedef SingleObjectSelector<
          reco::GenJetCollection,
          StringCutObjectSelector<reco::GenJet>
        > GenJetSelector;

DEFINE_FWK_MODULE(GenJetSelector);

