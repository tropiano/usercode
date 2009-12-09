import FWCore.ParameterSet.Config as cms

process=cms.Process("test")

process.outputFile = cms.PSet(
  name = cms.string("signal.root")
)  

process.zeegen = cms.PSet (
  type        = cms.string("GenElectron"),
  Name        = cms.string("GenElectron"),
  ReportName        = cms.string("GenReport.txt"),
  sourceFileList = cms.string("file.txt"),
  CrossSection = cms.double(1944.),
  targetLumi  = cms.double(100.),
  Norm        = cms.bool(True),
  Sumw2        = cms.bool(False)
)

process.zeerec = cms.PSet (
  type        = cms.string("RecoElectron"),
  Name        = cms.string("RecoElectron"),
  sourceFileList = cms.string("file.txt"),
  electronID = cms.string("eidRobustTight"),
  CrossSection = cms.double(1944.),
  targetLumi  = cms.double(100.),
  Norm        = cms.bool(True),
  Sumw2        = cms.bool(False)
)

process.zeeeff = cms.PSet (
  type        = cms.string("EfficiencyElectron"),
  Name        = cms.string("EfficiencyElectron"),
  sourceFileList = cms.string("file.txt"),
  electronID = cms.string("eidRobustTight"),
  Sumw2        = cms.bool(False)
)

