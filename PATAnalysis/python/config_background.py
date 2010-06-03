import FWCore.ParameterSet.Config as cms

process=cms.Process("test")

process.outputFile = cms.PSet(
  name = cms.string("background.root")
)  

process.zmumurec = cms.PSet (
  type        = cms.string("RecoMuon"),
  Name        = cms.string("RecoMuon"),
  MinPtJet    = cms.double(30.),
  MaxEtaJet   = cms.double(3.),
  ScaleFactor = cms.double(1.),
  IsoCut      = cms.double(0.3),
  Zsource     = cms.string("zmumurec"),
  JetSource     = cms.string("selectedJets")
)

process.zmumueff = cms.PSet (
  type        = cms.string("EfficiencyMuon"),
  Name        = cms.string("EfficiencyMuonVSRecoMulti"),
  MinPtJet    = cms.double(30.),
  MaxEtaJet   = cms.double(3.),
  ScaleFactor = cms.double(1.),
  VsGenMulti  = cms.bool(False),
  RequireGenInAcceptance = cms.bool(False),
  TrainUnfolding = cms.bool(False),
  NBin        = cms.uint32(10),
  XMin        = cms.double(-0.5),
  XMax        = cms.double(9.5),
  IsoCut      = cms.double(0.3),
  Zsource     = cms.string("zmumurec"),
  JetSource     = cms.string("selectedJets"),
  OppositeCharge = cms.bool(True)
)
