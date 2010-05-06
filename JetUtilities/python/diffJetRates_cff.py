import FWCore.ParameterSet.Config as cms

from JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_ReReco332_cff import *
#from Firenze.JetUtilities.exclusiveJets_cff import *

diffCaloJetRate10 = cms.EDFilter('DiffJetRate',
                                  njet         = cms.int32(1),
                                  src          = cms.InputTag('exclusive1CaloJets6'),
                                  JetCorrector = cms.string('L2L3JetCorrectorKT6Calo'))

diffCaloJetRate21 = cms.EDFilter('DiffJetRate',
                                  njet         = cms.int32(2),
                                  src          = cms.InputTag('exclusive2CaloJets6'),
                                  JetCorrector = cms.string('L2L3JetCorrectorKT6Calo'))

diffCaloJetRate32 = cms.EDFilter('DiffJetRate',
                                  njet         = cms.int32(3),
                                  src          = cms.InputTag('exclusive3CaloJets6'),
                                  JetCorrector = cms.string('L2L3JetCorrectorKT6Calo'))


diffGenJetRate10 = cms.EDFilter('DiffJetRate',
                                        njet         = cms.int32(1),
                                        src          = cms.InputTag('exclusive1GenJets10'))
                                        
diffGenJetRate21 = cms.EDFilter('DiffJetRate',
                                 njet         = cms.int32(2),
                                 src          = cms.InputTag('exclusive2GenJets10'))

diffGenJetRate32 = cms.EDFilter('DiffJetRate',
                                 njet         = cms.int32(3),
                                 src          = cms.InputTag('exclusive3GenJets10'))

diffCaloJetRates = cms.Sequence(diffCaloJetRate10+diffCaloJetRate21+diffCaloJetRate32)
diffGenJetRates  = cms.Sequence(diffGenJetRate10+diffGenJetRate21+diffGenJetRate32)
diffJetRates     = cms.Sequence(diffCaloJetRates+diffGenJetRates)

