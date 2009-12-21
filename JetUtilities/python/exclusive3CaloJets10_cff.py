import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.CaloJetParameters_cfi import *
CaloJetParameters.jetPtMin = cms.double(0.)
#from RecoJets.JetProducers.FastjetParameters_cfi import *
from Firenze.JetUtilities.ExclusiveJetsParameters_cfi import *
exclusive3CaloJets10 = cms.EDProducer("ExclInclFastJetProducer",
    #FastjetNoPU,
    ExclusiveJetParameters,
    CaloJetParameters,
   
    njets = cms.int32(3),
   
    alias = cms.untracked.string('exclusive3CaloJets10'),
    FJ_ktRParam = cms.double(1.0)
)
