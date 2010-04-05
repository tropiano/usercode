import FWCore.ParameterSet.Config as cms

process=cms.Process("test")

process.outputFile = cms.PSet(
  name = cms.string("signal.root")
)  

process.zmumugen = cms.PSet (
  type        = cms.string("GenMuon"),
  Name        = cms.string("GenMuon"),
  MinPtJet    = cms.double(30.),
  MaxEtaJet   = cms.double(3.),
  ScaleFactor = cms.double(1.)
)

process.zmumurec = cms.PSet (
  type        = cms.string("RecoMuon"),
  Name        = cms.string("RecoMuon"),
  MinPtJet    = cms.double(30.),
  MaxEtaJet   = cms.double(3.),
  ScaleFactor = cms.double(1.),
  IsoCut      = cms.double(0.3),
  Zsource     = cms.string("zmumurec")
)

process.zmumueff = cms.PSet (
  type        = cms.string("EfficiencyMuon"),
  Name        = cms.string("EfficiencyMuon"),
  MinPtJet    = cms.double(30.),
  MaxEtaJet   = cms.double(3.),
  ScaleFactor = cms.double(1.),
  RequireGenInAcceptance = cms.bool(True),
  VsGenMulti  = cms.bool(True),
  TrainUnfolding = cms.bool(True),
  NBin        = cms.uint32(10),
  XMin        = cms.double(-0.5),
  XMax        = cms.double(9.5),
  IsoCut      = cms.double(0.3),
  Zsource     = cms.string("zmumurec"),
  OppositeCharge = cms.bool(True)
)
