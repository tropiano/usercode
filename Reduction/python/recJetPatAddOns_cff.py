import FWCore.ParameterSet.Config as cms

import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
selectedJets = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedLayer1Jets.clone()
selectedJets.src = cms.InputTag("cleanLayer1Jets")
selectedJets.cut = cms.string('pt > 20. & abs(eta) < 10. & nConstituents > 0')

recjetsSequence = cms.Sequence(selectedJets)

jetrecEventContent = [
  'keep *_selectedJets_*_*'
]  
