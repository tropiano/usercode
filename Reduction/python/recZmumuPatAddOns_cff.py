import FWCore.ParameterSet.Config as cms

#add user data
from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import patMuons
patMuons.userData.userFunctions = cms.vstring('? globalTrack.isNonnull ? globalTrack.hitPattern.numberOfValidPixelHits() : -1',
                                              '? globalTrack.isNonnull ? globalTrack.hitPattern.numberOfValidMuonHits() : -1',
                                              '? globalTrack.isNonnull ? globalTrack.hitPattern.numberOfValidHits() : -1',
                                              '? globalTrack.isNonnull ? globalTrack.hitPattern.numberOfValidTrackerHits() : -1',
                                              'numberOfMatches()',
                                              '? globalTrack.isNonnull ? globalTrack.normalizedChi2() : -1')
patMuons.userData.userFunctionLabels = cms.vstring('numberOfValidPixelHits',
                                                   'numberOfValidMuonHits',
                                                   'numberOfValidHits',
                                                   'numberOfValidTrackerHits',
                                                   'numberOfMatches',
                                                   'normChi2')

##select muons according to these criteria
from PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi import selectedPatMuons
selectedMuons = selectedPatMuons.clone()
selectedMuons.src = cms.InputTag("patMuons")
selectedMuons.cut = cms.string("pt > 15. & abs(eta) < 3. ")


#Z candidate combiner
zmumurec = cms.EDProducer('CandViewCombiner',
  decay = cms.string('selectedMuons@+ selectedMuons@-'),
  cut   = cms.string('50 < mass < 130'),
  name  = cms.string('Zmumu'),
  roles = cms.vstring('mu1', 'mu2')
)

zmumurecSameCharge = cms.EDProducer('CandViewCombiner',
#both ++ and -- in the same colelction
  decay = cms.string('selectedMuons@+ selectedMuons@+'),
  cut   = cms.string('50 < mass < 130'),
  name  = cms.string('Zmumu_samecharge'),
  roles = cms.vstring('mu1', 'mu2')
)

zmumurecSequence = cms.Sequence(selectedMuons*(zmumurec + zmumurecSameCharge))

zmumurecEventContent = [
  'keep *_selectedMuons_*_*',
  'keep *_zmumurec*_*_*'
]

