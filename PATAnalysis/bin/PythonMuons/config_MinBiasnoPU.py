import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_PU_background import *

#process.load(config_background)

process.outputFile.name = cms.string("MinBias_off.root")
process.zmumurec.ScaleFactor=cms.double(71260000000.0/1000000.0)

#process.zmumurec.JetSource     = cms.string("selectedJets")
process.zmumurec.JetSource     = cms.string("selectedJetsL1Corrected")
process.zmumurec.MaxEtaJet   = cms.double(2.4)

process.zmumurec.MinPtJet = 30.

