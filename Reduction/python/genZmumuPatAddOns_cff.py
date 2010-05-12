import FWCore.ParameterSet.Config as cms

##select gen muons from Z clustering the possible QED final state photon
genMuMinusStatus3 = cms.EDFilter('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(13),
  requestedStaus3Daughters = cms.vint32(),
  motherCharge = cms.int32(-1)
)

genMuPlusStatus3 = cms.EDFilter('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(-13),
  requestedStaus3Daughters = cms.vint32(),
  motherCharge = cms.int32(1)
)

genMuMinusStatus3Selector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genMuMinusStatus3'),
   cut = cms.string('pt > 0 & abs(eta) < 100.')
)

genMuPlusStatus3Selector = cms.EDFilter('CandViewSelector',
  src = cms.InputTag('genMuPlusStatus3'),
  cut = cms.string('pt > 0 & abs(eta) < 100.')
)

##Z mumu with QED FS photons
#zmumugenfull = cms.EDFilter('CandViewShallowCloneCombiner',
zmumugenfull = cms.EDFilter('CandViewCombiner',
   decay = cms.string('genMuPlusStatus3Selector@+ genMuMinusStatus3Selector@-'),
   cut   = cms.string('20 < mass < 10000'),
   name  = cms.string('ZmumuGenFull'),
   roles = cms.vstring('mu1', 'mu2')
)

#Gen Mu Minus selector
genMuMinusSelector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genParticles'),
   cut = cms.string('status == 1 & pdgId == 13 & pt > 0 & abs(eta) < 100.')
)
#Gen Mu Plus selector
genMuPlusSelector = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genParticles'),
   cut = cms.string('status == 1 & pdgId == -13 & pt > 0 & abs(eta) < 100.')
)
## Z mumu wothout QED FS photons
#zmumugen = cms.EDFilter('CandViewShallowCloneCombiner',
zmumugen = cms.EDFilter('CandViewCombiner',
   decay = cms.string('genMuPlusSelector@+ genMuMinusSelector@-'),
   cut   = cms.string('20 < mass < 10000'),
   name  = cms.string('ZmumuGen'),
   roles = cms.vstring('mu1', 'mu2')
)

zmumugenSequence = cms.Sequence(genMuMinusStatus3+genMuPlusStatus3+genMuMinusStatus3Selector+genMuPlusStatus3Selector+zmumugenfull+genMuMinusSelector+genMuPlusSelector+zmumugen)

zmumugenEventContent = [
  'keep *_genMuMinusStatus3Selector_*_*',
  'keep *_genMuPlusStatus3Selector_*_*',
  'keep *_zmumugenfull_*_*',
  'keep *_genMuMinusSelector_*_*',
  'keep *_genMuPlusSelector_*_*',
  'keep *_zmumugen_*_*'
]
