import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.GenJetParameters_cfi import *
GenJetParameters.src = cms.InputTag("newGenParticlesForJets")
GenJetParameters.jetPtMin = cms.double(0.)
from RecoJets.JetProducers.FastjetParameters_cfi import *
from Firenze.JetUtilities.ExclusiveJetsParameters_cfi import *
exclusive2GenJets6 = cms.EDProducer("ExclInclFastJetProducer",
    FastjetNoPU,
    ExclusiveJetParameters,
    GenJetParameters,

    njets = cms.int32(2), 
    
    alias = cms.untracked.string('exclusive2GenJets6'),
    FJ_ktRParam = cms.double(0.6)
)
