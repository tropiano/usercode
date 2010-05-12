import FWCore.ParameterSet.Config as cms

selectedGenJets = cms.EDFilter('GenJetSelector',
   src = cms.InputTag('ak5GenJets'),
   cut = cms.string('pt > 20. & abs(eta) < 100. & nConstituents > 0')
)
genjetsSequence = cms.Sequence(selectedGenJets)

jetgenEventContent = [
  'keep *_selectedGenJets_*_*'
]
