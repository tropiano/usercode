import FWCore.ParameterSet.Config as cms

#add user data
from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import patMuons
patMuons.userData.userFunctions = cms.vstring(
                                              '? globalTrack.isNonnull ? globalTrack.normalizedChi2() : -1',
                                              '? globalTrack.isNonnull ? globalTrack.hitPattern.numberOfValidMuonHits() : -1',
                                              'numberOfMatchedStations()',
                                              '? track.isNonnull ? track.hitPattern.numberOfValidPixelHits() : -1',
#                                              '? globalTrack.isNonnull ? globalTrack.hitPattern.numberOfValidPixelHits() : -1',
                                              '? track.isNonnull ? track.hitPattern.numberOfValidTrackerHits() : -1',
#                                              '? globalTrack.isNonnull ? globalTrack.hitPattern.numberOfValidTrackerHits() : -1',
                                              '? innerTrack.isNonnull ? innerTrack.hitPattern.numberOfValidHits() : -1',
#                                              '? globalTrack.isNonnull ? globalTrack.hitPattern.numberOfValidHits() : -1',
                                              '? innerTrack.isNonnull ? innerTrack.hitPattern.numberOfValidPixelHits() : -1',
                                              'numberOfMatches()',
                                              '? innerTrack.isNonnull ? innerTrack.ptError() : -1')

patMuons.userData.userFunctionLabels = cms.vstring(
                                                   'normChi2',
                                                   'numberOfValidMuonHits',
                                                   'numberOfMatchedStations',
                                                   'numberOfValidPixelHitsTr',
                                                   'numberOfValidTrackerHits',
                                                   'numberOfValidHits',
                                                   'numberOfValidPixelHitsInTr',
                                                   'numberOfMatches',
                                                   'ptError')

##select muons according to these criteria
from PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi import selectedPatMuons
selectedMuons = selectedPatMuons.clone()
selectedMuons.src = cms.InputTag("patMuons")
selectedMuons.cut = cms.string("pt > 15. & abs(eta) < 3. ")

# Muon MC Matcher
from PhysicsTools.PatAlgos.mcMatchLayer0.muonMatch_cfi import *
muonMatch.src = cms.InputTag("muons") # RECO objects to match
muonMatch.matched = cms.InputTag("genParticles")   # mc-truth particle collection
muonMatch.mcStatus = cms.vint32(1)

selectedMuons.addGenMatch = cms.bool(True)
selectedMuons.embedGenMatch = cms.bool(True)
selectedMuons.genParticleMatch = cms.InputTag("muonMatch")

# new class of selected Muons
selectedMuons1 = cms.EDFilter("LargestPtCandViewSelector",
  src = cms.InputTag("selectedMuons"),
  maxNumber = cms.uint32(2)
)

#selectedMuons1 = selectedMuons.clone()
 
#Z candidate combiner
zmumurec = cms.EDProducer('CandViewCombiner',
  decay = cms.string('selectedMuons1@+ selectedMuons1@-'),
  cut   = cms.string('60 < mass < 120'),
  name  = cms.string('Zmumu'),
  roles = cms.vstring('mu1', 'mu2')
)

zmumurecSameCharge = cms.EDProducer('CandViewCombiner',
#both ++ and -- in the same collection
  decay = cms.string('selectedMuons1@+ selectedMuons1@+'),
  cut   = cms.string('60 < mass < 120'),
  name  = cms.string('Zmumu_samecharge'),
  roles = cms.vstring('mu1', 'mu2')
)

zmumurecSequence = cms.Sequence(muonMatch*selectedMuons*(selectedMuons1*(zmumurec + zmumurecSameCharge)))

zmumurecEventContent = [
  'keep *_selectedMuons_*_*',
#  'keep *_selectedMuons1_*_*', non funge
  'keep *_zmumurec*_*_*'
]

