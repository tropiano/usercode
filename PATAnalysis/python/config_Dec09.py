import FWCore.ParameterSet.Config as cms

process=cms.Process("test")

process.outputFile = cms.PSet(
  name = cms.string("tracks.root")
)  

process.track = cms.PSet (
  type        = cms.string("RecoTrack"),
  Name        = cms.string("RecoTrack"),
  MinPt       = cms.double(0.),
  MaxEta      = cms.double(3.),
  MinHit      = cms.double(5.)
)

process.jet = cms.PSet (
  type        = cms.string("RecoJet"),
  Name        = cms.string("RecoJet"),
  MinPt       = cms.double(10.),
  MaxEta      = cms.double(2.4),
)
