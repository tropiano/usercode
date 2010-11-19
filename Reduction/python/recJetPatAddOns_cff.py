import FWCore.ParameterSet.Config as cms

import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
selectedJets = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedPatJets.clone()
selectedJets.src = cms.InputTag("patJets")
selectedJets.cut = cms.string('pt > 10. & abs(eta) < 10. & nConstituents > 0')

#pfjets
#from Firenze.JetUtilities.pfAK5Jets_cff import *

#import PhysicsTools.PFCandProducer.ParticleSelectors.ptMinPFJetSelector_cfi
#import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
#selectedPFJets = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedPatJets.clone()
#selectedPFJets.src = cms.InputTag('allLayer1PFJets')
#selectedPFJets.src = cms.InputTag('patPFJets')
#selectedPFJets.cut = cms.string('pt > 20. & abs(eta) < 10.')

#import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
#selectedJPTJets = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedPatJets.clone()
##selectedPFJets.src = cms.InputTag('allLayer1PFJets')
#selectedJPTJets.src = cms.InputTag('patJPTJets')
#selectedJPTJets.cut = cms.string('pt > 20. & abs(eta) < 10.')

#recPFjetsSequence = cms.Sequence(allLayer1PFJetsSequence)

#recjetsSequence = cms.Sequence(selectedPFJets + selectedJets )#+ selectedJPTJets)
recjetsSequence = cms.Sequence( selectedJets )#+ selectedJPTJets)


jetrecEventContent = [
  'keep patJets_selectedJets*_*_*',
  #'keep *_selectedJPTJets_*_*'
]  
