import FWCore.ParameterSet.Config as cms

process=cms.Process("test")

process.outputFile = cms.PSet(
  name = cms.string("tracks.root")
)  

process.track = cms.PSet (
  type        = cms.string("RecoTrack"),
  Name        = cms.string("RecoTrack"),
  MinPtTk     = cms.double(0.5),
  MinPtMu     = cms.double(1.),
  MinPtJet    = cms.double(20.),
  MaxEta      = cms.double(3.),
  MinHit      = cms.double(5.)
)

process.jet = cms.PSet (
  type        = cms.string("RecoJet"),
  Name        = cms.string("RecoJet"),
  MinPt       = cms.double(20.),
  MaxEta      = cms.double(3.),
  JetSource   = cms.string("selectedPFJets")
)

process.rapgap = cms.PSet (
  type        = cms.string("RecoRapidityGaps"),
  Name        = cms.string("RecoRapidityGaps"),
  jetSource   = cms.string("selectedPFJets"),
  MinPt       = cms.double(20.),
  MaxEta      = cms.double(3.),
  isMC        = cms.bool(True)
)

process.genrapgap = cms.PSet (
  type        = cms.string("GenRapidityGaps"),
  Name        = cms.string("GenRapidityGaps"),
  jetSource   = cms.string("selectedGenJets"),
  MinPt       = cms.double(20.),
  MaxEta      = cms.double(3.)
)
