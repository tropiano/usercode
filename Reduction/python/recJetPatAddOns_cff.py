import FWCore.ParameterSet.Config as cms

import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
selectedJets = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedLayer1Jets.clone()
selectedJets.src = cms.InputTag("cleanLayer1Jets")
selectedJets.cut = cms.string('pt > 20. & abs(eta) < 10. & nConstituents > 0')

import PhysicsTools.PFCandProducer.ParticleSelectors.ptMinPFJetSelector_cfi
selectedPFJets = PhysicsTools.PFCandProducer.ParticleSelectors.ptMinPFJetSelector_cfi.ptMinPFJets.clone()
selectedPFJets.src = cms.InputTag('antikt5PFJets')
selectedPFJets.ptMin = 20.

recjetsSequence = cms.Sequence(selectedJets + selectedPFJets)


jetrecEventContent = [
  'keep *_selectedJets_*_*',
  'keep *_selectedPFJets_*_*'
]  
