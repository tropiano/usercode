import FWCore.ParameterSet.Config as cms

import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
selectedJets = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedPatJets.clone()
selectedJets.src = cms.InputTag("patJets")
selectedJets.cut = cms.string('pt > 20. & abs(eta) < 10. & nConstituents > 0')

#pfjets
import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
selectedPFJets = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedPatJets.clone()
#selectedPFJets.src = cms.InputTag('allLayer1PFJets')
selectedPFJets.src = cms.InputTag('patJets')
selectedPFJets.cut = cms.string('pt > 20. & abs(eta) < 10.')

import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
selectedJPTJets = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedPatJets.clone()
#selectedPFJets.src = cms.InputTag('allLayer1PFJets')
selectedJPTJets.src = cms.InputTag('patJPTJets')
selectedJPTJets.cut = cms.string('pt > 20. & abs(eta) < 10.')

import PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi
countJets = PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi.countPatJets.clone()
#selectedPFJets.src = cms.InputTag('allLayer1PFJets')
countJets.src = cms.InputTag('selectedJets')
countJets.minNumber = cms.uint32(4)

import PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi
countPFJets = PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi.countPatJets.clone()
#selectedPFJets.src = cms.InputTag('allLayer1PFJets')
countPFJets.src = cms.InputTag('selectedPFJets')
countPFJets.minNumber = cms.uint32(4)

recjetsSequence = cms.Sequence((selectedPFJets  * countPFJets)+ (selectedJets * countJets))


jetrecEventContent = [
  'keep *_selectedJets_*_*',
  'keep *_selectedPFJets_*_*'#,
  #'keep *_selectedJPTJets_*_*'
]  
