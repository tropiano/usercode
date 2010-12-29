import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_data import *

#process.load(config_background)
process.zmumurec.JetSource     = cms.string("selectedJets")
process.zmumurec.MaxEtaJet   = cms.double(2.4)

process.zmumueff.JetSource     = cms.string("selectedJets")
process.zmumueff.MaxEtaJet   = cms.double(2.4)

process.zmumurec.MinPtJet = 15.
process.zmumueff.MinPtJet = 15.
