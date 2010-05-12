import FWCore.ParameterSet.Config as cms

import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
selectedJets = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedPatJets.clone()
selectedJets.src = cms.InputTag("patJets")
selectedJets.cut = cms.string('pt > 20. & abs(eta) < 10. & nConstituents > 0')

#pfjets
from Firenze.JetUtilities.pfAK5Jets_cff import *

#import PhysicsTools.PFCandProducer.ParticleSelectors.ptMinPFJetSelector_cfi
import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
selectedPFJets = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedPatJets.clone()
selectedPFJets.src = cms.InputTag('allLayer1PFJets')
selectedPFJets.cut = cms.string('pt > 20. & abs(eta) < 10.')

recPFjetsSequence = cms.Sequence(allLayer1PFJetsSequence)

recjetsSequence = cms.Sequence(selectedPFJets + selectedJets)


jetrecEventContent = [
  'keep *_selectedJets_*_*',
  'keep *_selectedPFJets_*_*'
]  
