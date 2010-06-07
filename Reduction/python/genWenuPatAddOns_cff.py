import FWCore.ParameterSet.Config as cms

##select gen muons from Z clustering the possible QED final state photon
wEnuMinusgenfull = cms.EDFilter('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(-24),
  requestedStaus3Daughters = cms.vint32(11),
  motherCharge = cms.int32(-1)
) 

wEnuPlusgenfull = cms.EDFilter('Status3MotherCombiner',
  src = cms.InputTag('genParticles'),
  motherId = cms.int32(24),
  requestedStaus3Daughters = cms.vint32(11),
  motherCharge = cms.int32(1)
)

wEnugenSequence = cms.Sequence(wEnuMinusgenfull+wEnuPlusgenfull)

wEnugenEventContent = [
  'keep *_wEnuMinusgenfull_*_*',
  'keep *_wEnuPlusgenfull_*_*'
]
