import FWCore.ParameterSet.Config as cms


#################################
# Z ee with QED FS photons
#################################

## select gen electrons from Z clustering with the possible QED Final State photon

# Gen E Minus Status 3 producer
genEMinusStatus3 = cms.EDProducer('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(11),
  requestedStaus3Daughters = cms.vint32(),
  motherCharge = cms.int32(-1)
)

# Gen E Plus Status 3 producer
genEPlusStatus3 = cms.EDProducer('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(-11),
  requestedStaus3Daughters = cms.vint32(),
  motherCharge = cms.int32(1)
)

# Gen E Minus Status 3 selector
genEMinusStatus3Selector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genEMinusStatus3'),
   cut = cms.string('pt > 10 & abs(eta) < 3.')
)

# Gen E Plus Status 3 selector
genEPlusStatus3Selector = cms.EDFilter('CandViewSelector',
  src = cms.InputTag('genEPlusStatus3'),
  cut = cms.string('pt > 10 & abs(eta) < 3.')
)

####
# Gen E Minus Status 3 Highest Pt selector
genEMinusStatus3SelectorFirst = cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("genEMinusStatus3Selector"),
    maxNumber = cms.uint32(1)
  )

# Gen E Plus Status 3 Highest Pt selector
genEPlusStatus3SelectorFirst = cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("genEPlusStatus3Selector"),
    maxNumber = cms.uint32(1)
  )
####

## Z ee with QED FS photons
#zeegenfull = cms.EDFilter('CandViewShallowCloneCombiner',
zeegenfull = cms.EDProducer('CandViewCombiner',
   decay = cms.string('genEPlusStatus3SelectorFirst@+ genEMinusStatus3SelectorFirst@-'),
   cut   = cms.string('60 < mass < 120'),
   name  = cms.string('ZeeGenFull'),
   roles = cms.vstring('e1', 'e2')
)

zeegenSequenceQEDFS = cms.Sequence(((genEMinusStatus3 + genEPlusStatus3) * (genEMinusStatus3Selector + genEPlusStatus3Selector) * (genEMinusStatus3SelectorFirst + genEPlusStatus3SelectorFirst)) * zeegenfull)

####################################
# Z ee without QED FS photons
####################################

## select gen electrons from Z clustering without the possible QED Final State photon

# Gen E Minus selector
genEMinusSelector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genParticles'),
   cut = cms.string('status == 1 & pdgId == 11 & pt > 10 & abs(eta) < 3.')
)

# Gen E Plus selector
genEPlusSelector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genParticles'),
   cut = cms.string('status == 1 & pdgId == -11 & pt > 10 & abs(eta) < 3.')
)

# Gen E selector
#genESelector = cms.Sequence(genEMinusSelector + genEPlusSelector)

####
# Gen E Minus Highest Pt selector
genEMinusSelectorFirst = cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("genEMinusSelector"),
    maxNumber = cms.uint32(1)
  )

# Gen E Plus Highest Pt selector
genEPlusSelectorFirst = cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("genEPlusSelector"),
    maxNumber = cms.uint32(1)
  )

#genESelectorFirst = cms.Sequence(genEMinusSelectorFirst + genEPlusSelectorFirst)

####

## Z ee without QED FS photons
#zeegen = cms.EDFilter('CandViewShallowCloneCombiner',
zeegen = cms.EDProducer('CandViewCombiner',
   decay = cms.string('genEPlusSelectorFirst@+ genEMinusSelectorFirst@-'),
   cut   = cms.string('60 < mass < 120'),
   name  = cms.string('ZeeGen'),
   roles = cms.vstring('e1', 'e2')
)

zeegenSequenceNoQEDFS = cms.Sequence((genEMinusSelector + genEPlusSelector) * (genEMinusSelectorFirst + genEPlusSelectorFirst) * zeegen)


#################################
# Sequences and EventContent
#################################

zeegenSequence = cms.Sequence( zeegenSequenceQEDFS + zeegenSequenceNoQEDFS )

zeegenEventContent = [
  'keep *_genEMinusStatus3Selector_*_*',
  'keep *_genEPlusStatus3Selector_*_*',
  'keep *_zeegenfull_*_*',
  'keep *_genEMinusSelector_*_*',
  'keep *_genEPlusSelector_*_*',
  'keep *_zeegen_*_*'
]
