####################
# Customization
####################
isMC = True
####################

import FWCore.ParameterSet.Config as cms

##select electrons according to these criteria

from PhysicsTools.PatAlgos.selectionLayer1.electronSelector_cfi import selectedPatElectrons
selectedElectrons = selectedPatElectrons.clone()
selectedElectrons.src = cms.InputTag("patElectrons")
selectedElectrons.cut = cms.string("pt > 10. & abs(eta) < 3. ")

if isMC == True:
	# Electron MC Matcher
	from PhysicsTools.PatAlgos.mcMatchLayer0.electronMatch_cfi import *
	electronMatch.src = cms.InputTag("gsfElectrons") # RECO objects to match
	electronMatch.matched = cms.InputTag("genParticles")   # mc-truth particle collection
	electronMatch.mcStatus = cms.vint32(1)

	selectedElectrons.addGenMatch = cms.bool(True)
	selectedElectrons.embedGenMatch = cms.bool(True)
	selectedElectrons.genParticleMatch = cms.InputTag("electronMatch")

# new class of selected Electrons
selectedElectrons1 = cms.EDFilter("LargestPtCandViewSelector",
  src = cms.InputTag("selectedElectrons"),
  maxNumber = cms.uint32(2)
)
 
#Z candidate combiner
zeerec = cms.EDProducer('CandViewCombiner',
  decay = cms.string('selectedElectrons1@+ selectedElectrons1@-'),
  cut   = cms.string('60 < mass < 120'),
  name  = cms.string('Zee'),
  roles = cms.vstring('e1', 'e2')
)

#both ++ and -- in the same collection
zeerecSameChargePlus = cms.EDProducer('CandViewCombiner',
  decay = cms.string('selectedElectrons1@+ selectedElectrons1@+'),
  cut   = cms.string('60 < mass < 120'),
  name  = cms.string('Zee_samechargeplus'),
  roles = cms.vstring('e1', 'e2')
)

zeerecSameChargeMinus = cms.EDProducer('CandViewCombiner',
  decay = cms.string('selectedElectrons1@- selectedElectrons1@-'),
  cut   = cms.string('60 < mass < 120'),
  name  = cms.string('Zee_samechargeminus'),
  roles = cms.vstring('e1', 'e2')
)

if isMC == True:
	zeerecSequence = cms.Sequence(electronMatch * selectedElectrons * (selectedElectrons1 * (zeerec + zeerecSameChargePlus + zeerecSameChargeMinus)))
else:
	zeerecSequence = cms.Sequence(selectedElectrons * (selectedElectrons1 * (zeerec + zeerecSameChargePlus + zeerecSameChargeMinus)))

zeerecEventContent = [
  'keep *_selectedElectrons_*_*',
  'keep *_zeerec*_*_*'
]

