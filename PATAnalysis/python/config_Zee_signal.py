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
  EventsPerFile = cms.double(0),
  Norm        = cms.bool(True)
)

process.zeerec = cms.PSet (
  type        = cms.string("RecoElectron"),
  Name        = cms.string("RecoElectron"),
  sourceFileList = cms.string("file.txt"),
  electronID = cms.string("eidRobustTight"),
  Acc        = cms.double(1),
  Qual       = cms.double(2),
  Imp        = cms.double(3),
  Iso        = cms.double(4),
  EiD        = cms.double(5),
  CrossSection = cms.double(1944.),
  targetLumi  = cms.double(100.),
  EventsPerFile = cms.double(0),
  Norm        = cms.bool(True)
)

process.zeeeff = cms.PSet (
  type        = cms.string("EfficiencyElectron"),
  Name        = cms.string("EfficiencyElectron"),
  sourceFileList = cms.string("file.txt"),
  electronID = cms.string("eidRobustTight"),
  Acc        = cms.double(1),
  Qual       = cms.double(2),
  Imp        = cms.double(3),
  Iso        = cms.double(4),
  EiD        = cms.double(5)
)

process.zeentuple = cms.PSet (
  type        = cms.string("RecoElectronNtuple"),
  Name        = cms.string("RecoElectronNtuple"),
  sourceFileList = cms.string("file.txt"),
  electronID = cms.string("eidRobustTight"),
  Acc        = cms.double(1),
  Qual       = cms.double(2),
  Imp        = cms.double(3),
  Iso        = cms.double(4),
  EiD        = cms.double(5),
  CrossSection = cms.double(1944.),
  targetLumi  = cms.double(100.),
  EventsPerFile = cms.double(0),
  Norm        = cms.bool(True)
)

