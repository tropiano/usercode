import FWCore.ParameterSet.Config as cms

newGenParticlesForJets = cms.EDFilter("NewGenJetsParticleSelector",
    src = cms.InputTag("genParticles"),
    ignoreParticleIDs = cms.vuint32(1000022, 2000012, 2000014, 2000016, 1000039, 
        5000039, 4000012, 9900012, 9900014, 9900016, 
        39),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(True),
    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
    tausAsJets = cms.bool(False),
    checkForStatus3Mother = cms.PSet(
                            motherIds=cms.vuint32(11, 13, 12, 14, 16),
                            excludeSameFlavourDaughterOnly=cms.bool(True)),
    reverseAllSelections  = cms.bool(False)                        
)
 
newGenJetParticles = cms.Sequence(newGenParticlesForJets)

