import FWCore.ParameterSet.Config as cms

##select muons according to these criteria
import PhysicsTools.PatAlgos.selectionLayer1.electronSelector_cfi
selectedElectrons = PhysicsTools.PatAlgos.selectionLayer1.electronSelector_cfi.selectedPatElectrons.clone()
selectedElectrons.src = cms.InputTag("patElectrons")
#selectedElectrons.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalElectronPromptTight') & innerTrack().found()>=11 & abs(globalTrack().d0())<0.2 & (trackIso()+caloIso()+ecalIso())/pt<0.1")
selectedElectrons.cut = cms.string("pt > 10. & abs(eta) < 3.")

import PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi
selectedMuons = PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi.selectedPatMuons.clone()
selectedMuons.src = cms.InputTag("patMuons")
#selectedElectrons.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalElectronPromptTight') & innerTrack().found()>=11 & abs(globalTrack().d0())<0.2 & (trackIso()+caloIso()+ecalIso())/pt<0.1")
selectedElectrons.cut = cms.string("pt > 5. & abs(eta) < 3.")


#Z candidate combiner
## zeerec = cms.EDFilter('CandViewShallowCloneCombiner',
##   decay = cms.string('selectedElectrons@+ selectedElectrons@-'),
##   cut   = cms.string('50 < mass < 130'),
##   name  = cms.string('Zeerec'),
##   roles = cms.vstring('e1', 'e2')
## )

## zeerecSameChargePlus = cms.EDFilter('CandViewShallowCloneCombiner',
##   decay = cms.string('selectedElectrons@+ selectedElectrons@+'),
##   cut   = cms.string('50 < mass < 130'),
##   name  = cms.string('Zeerec_samechargeplus'),
##   roles = cms.vstring('e1', 'e2')
## )

## zeerecSameChargeMinus = cms.EDFilter('CandViewShallowCloneCombiner',
##   decay = cms.string('selectedElectrons@- selectedElectrons@-'),
##   cut   = cms.string('50 < mass < 130'),
##   name  = cms.string('Zeerec_samechargeminus'),
##   roles = cms.vstring('e1', 'e2')
## )

ttbarrecSequence = cms.Sequence(selectedElectrons*selectedMuons)

ttbarrecEventContent = [
    'keep *_selectedMuons_*_*',
    'keep *_selectedElectrons_*_*'
]
