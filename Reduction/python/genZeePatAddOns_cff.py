import FWCore.ParameterSet.Config as cms


##select gen eons from Z clustering the possible QED final state photon
# genEMinusStatus3 = cms.EDFilter('Status3MotherCombiner',
genEMinusStatus3 = cms.EDProducer('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(11),
  requestedStaus3Daughters = cms.vint32(),
  motherCharge = cms.int32(-1)
)

# genEPlusStatus3 = cms.EDFilter('Status3MotherCombiner',
genEPlusStatus3 = cms.EDProducer('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(-11),
  requestedStaus3Daughters = cms.vint32(),
  motherCharge = cms.int32(1)
)

genEMinusStatus3Selector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genEMinusStatus3'),
   cut = cms.string('pt > 0 & abs(eta) < 100.')
)

genEPlusStatus3Selector = cms.EDFilter('CandViewSelector',
  src = cms.InputTag('genEPlusStatus3'),
  cut = cms.string('pt > 0 & abs(eta) < 100.')
)

##Z ee with QED FS photons
# zeegenfull = cms.EDFilter('CandViewShallowCloneCombiner',
zeegenfull = cms.EDProducer('CandViewCombiner',
   decay = cms.string('genEPlusStatus3Selector@+ genEMinusStatus3Selector@-'),
   cut   = cms.string('20 < mass < 10000'),
   name  = cms.string('ZeeGenFull'),
   roles = cms.vstring('e1', 'e2')
)

#Gen E Minus selector
genEMinusSelector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genParticles'),
   cut = cms.string('status == 1 & pdgId == 11 & pt > 0 & abs(eta) < 100.')
)
#Gen E Plus selector
genEPlusSelector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genParticles'),
   cut = cms.string('status == 1 & pdgId == -11 & pt > 0 & abs(eta) < 100.')
)
## Z ee wothout QED FS photons
#zeegen = cms.EDFilter('CandViewShallowCloneCombiner',
zeegen = cms.EDProducer('CandViewCombiner',
   decay = cms.string('genEPlusSelector@+ genEMinusSelector@-'),
   cut   = cms.string('20 < mass < 10000'),
   name  = cms.string('ZeeGen'),
   roles = cms.vstring('e1', 'e2')
)

zeegenSequence = cms.Sequence(genEMinusStatus3+genEPlusStatus3+genEMinusStatus3Selector+genEPlusStatus3Selector+zeegenfull+genEMinusSelector+genEPlusSelector+zeegen)

zeegenEventContent = [
  'keep *_genEMinusStatus3Selector_*_*',
  'keep *_genEPlusStatus3Selector_*_*',
  'keep *_zeegenfull_*_*',
  'keep *_genEMinusSelector_*_*',
  'keep *_genEPlusSelector_*_*',
  'keep *_zeegen_*_*'
]
