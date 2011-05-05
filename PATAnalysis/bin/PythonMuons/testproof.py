import FWCore.ParameterSet.Config as cms

process=cms.Process("test")

process.outputFile = cms.PSet(
  name = cms.string("testproof.root")
)  

process.testproof = cms.PSet (
  type        = cms.string("TestAnalysis"),
  Name        = cms.string("TestAnalysis"),
)

