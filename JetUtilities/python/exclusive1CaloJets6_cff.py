import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.CaloJetParameters_cfi import *
CaloJetParameters.jetPtMin = cms.double(0.)
#from RecoJets.JetProducers.FastjetParameters_cfi import *
from Firenze.JetUtilities.ExclusiveJetsParameters_cfi import *
exclusive1CaloJets6 = cms.EDProducer("ExclInclFastJetProducer",
    #FastjetNoPU,
    ExclusiveJetParameters,
    CaloJetParameters,
    
    njets = cms.int32(1),
   
    alias = cms.untracked.string('exclusive1CaloJets6'),
    FJ_ktRParam = cms.double(0.6)
)
