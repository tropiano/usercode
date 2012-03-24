import FWCore.ParameterSet.Config as cms


#################################
# Z mumu with QED FS photons
#################################

## select gen muons from Z clustering with the possible QED Final State photon

# Gen Mu Minus Status 3 producer
genMuMinusStatus3 = cms.EDProducer('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(13),
  requestedStaus3Daughters = cms.vint32(),
  motherCharge = cms.int32(-1)
)

# Gen Mu Plus Status 3 producer
genMuPlusStatus3 = cms.EDProducer('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(-13),
  requestedStaus3Daughters = cms.vint32(),
  motherCharge = cms.int32(1)
)

# Gen Mu Minus Status 3 selector
genMuMinusStatus3Selector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genMuMinusStatus3'),
   cut = cms.string('pt > 15 & abs(eta) < 3.')
)

# Gen Mu Plus Status 3 selector
genMuPlusStatus3Selector = cms.EDFilter('CandViewSelector',
  src = cms.InputTag('genMuPlusStatus3'),
  cut = cms.string('pt > 15 & abs(eta) < 3.')
)

####
# Gen Mu Minus Status 3 Highest Pt selector
genMuMinusStatus3SelectorFirst = cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("genMuMinusStatus3Selector"),
    maxNumber = cms.uint32(1)
  )

# Gen Mu Plus Status 3 Highest Pt selector
genMuPlusStatus3SelectorFirst = cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("genMuPlusStatus3Selector"),
    maxNumber = cms.uint32(1)
  )
####

## Z mumu with QED FS photons
#zmumugenfull = cms.EDFilter('CandViewShallowCloneCombiner',
zmumugenfull = cms.EDProducer('CandViewCombiner',
   decay = cms.string('genMuPlusStatus3SelectorFirst@+ genMuMinusStatus3SelectorFirst@-'),
   cut   = cms.string('60 < mass < 120'),
   name  = cms.string('ZmumuGenFull'),
   roles = cms.vstring('mu1', 'mu2')
)

zmumugenSequenceQEDFS = cms.Sequence(((genMuMinusStatus3 + genMuPlusStatus3) * (genMuMinusStatus3Selector + genMuPlusStatus3Selector) * (genMuMinusStatus3SelectorFirst + genMuPlusStatus3SelectorFirst)) * zmumugenfull)

####################################
# Z mumu without QED FS photons
####################################

## select gen muons from Z clustering without the possible QED Final State photon

# Gen Mu Minus selector
genMuMinusSelector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genParticles'),
   cut = cms.string('status == 1 & pdgId == 13 & pt > 15 & abs(eta) < 3.')
)

# Gen Mu Plus selector
genMuPlusSelector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genParticles'),
   cut = cms.string('status == 1 & pdgId == -13 & pt > 15 & abs(eta) < 3.')
)

# Gen Mu selector
#genMuSelector = cms.Sequence(genMuMinusSelector + genMuPlusSelector)

####
# Gen Mu Minus Highest Pt selector
genMuMinusSelectorFirst = cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("genMuMinusSelector"),
    maxNumber = cms.uint32(1)
  )

# Gen Mu Plus Highest Pt selector
genMuPlusSelectorFirst = cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("genMuPlusSelector"),
    maxNumber = cms.uint32(1)
  )

#genMuSelectorFirst = cms.Sequence(genMuMinusSelectorFirst + genMuPlusSelectorFirst)

####

## Z mumu without QED FS photons
#zmumugen = cms.EDFilter('CandViewShallowCloneCombiner',
zmumugen = cms.EDProducer('CandViewCombiner',
   decay = cms.string('genMuPlusSelectorFirst@+ genMuMinusSelectorFirst@-'),
   cut   = cms.string('60 < mass < 120'),
   name  = cms.string('ZmumuGen'),
   roles = cms.vstring('mu1', 'mu2')
)

zmumugenSequenceNoQEDFS = cms.Sequence((genMuMinusSelector + genMuPlusSelector) * (genMuMinusSelectorFirst + genMuPlusSelectorFirst) * zmumugen)


#################################
# Sequences and EventContent
#################################

zmumugenSequence = cms.Sequence( zmumugenSequenceQEDFS + zmumugenSequenceNoQEDFS )

zmumugenEventContent = [
  'keep *_genMuMinusStatus3Selector_*_*',
  'keep *_genMuPlusStatus3Selector_*_*',
  'keep *_zmumugenfull_*_*',
  'keep *_genMuMinusSelector_*_*',
  'keep *_genMuPlusSelector_*_*',
#  'keep *_genMuSelector_*_*',
#  'keep *_genMuSelectorFirst_*_*',
  'keep *_zmumugen_*_*'
]
