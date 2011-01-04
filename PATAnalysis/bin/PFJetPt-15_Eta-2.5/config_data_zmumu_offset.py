import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_data import *

#process.load(config_background)
process.outputFile.name = cms.string("zmumu_offset.root")
process.zmumurec.JetSource     = cms.string("selectedJetsL1Corrected")
process.zmumurec.MaxEtaJet   = cms.double(2.4)

process.zmumueff.JetSource     = cms.string("selectedJetsL1Corrected")
process.zmumueff.MaxEtaJet   = cms.double(2.4)

process.zmumurec.MinPtJet = 15.
process.zmumueff.MinPtJet = 15.
