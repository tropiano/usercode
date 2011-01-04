import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_background import *

#process.load(config_background)

process.outputFile.name = cms.string("ttbar.root")
process.zmumurec.ScaleFactor=cms.double(95./1095950)
process.zmumueff.ScaleFactor=cms.double(95./1095950)
process.zmumueff.TrainUnfolding = cms.bool(False)

process.zmumurec.JetSource     = cms.string("selectedJets")
process.zmumurec.MaxEtaJet   = cms.double(2.4)

process.zmumueff.JetSource     = cms.string("selectedJets")
process.zmumueff.MaxEtaJet   = cms.double(2.4)

process.zmumurec.MinPtJet = 30.
process.zmumueff.MinPtJet = 30.
