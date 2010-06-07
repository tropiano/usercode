import FWCore.ParameterSet.Config as cms

##select gen muons from Z clustering the possible QED final state photon
wMunuMinusgenfull = cms.EDFilter('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(-24),
  requestedStaus3Daughters = cms.vint32(13),
  motherCharge = cms.int32(-1)
) 

wMunuPlusgenfull = cms.EDFilter('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(24),
  requestedStaus3Daughters = cms.vint32(13),
  motherCharge = cms.int32(1)
)

wMunugenSequence = cms.Sequence(wMunuMinusgenfull+wMunuPlusgenfull)

wMunugenEventContent = [
  'keep *_wMunuMinusgenfull_*_*',
  'keep *_wMunuPlusgenfull_*_*'
]
