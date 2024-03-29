#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/CandAlgos/interface/CandCombiner.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "Firenze/BosonUtilities/interface/AddFourMomentaTransversePlane.h"

typedef reco::modules::CandCombiner<
                         StringCutObjectSelector<reco::Candidate>,
                         AnyPairSelector,
                         ::combiner::helpers::ShallowClone,
                         reco::CompositeCandidateCollection,
                         AddFourMomentaTransversePlane
                       > CandViewTransverseCombiner;

DEFINE_FWK_MODULE( CandViewTransverseCombiner );
