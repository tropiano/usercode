import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_data import *

#process.load(config_background)
process.zmumurec.JetSource     = cms.string("selectedPFJets")
process.zmumurec.MaxEtaJet   = cms.double(2.5)

process.zmumueff.JetSource     = cms.string("selectedPFJets")
process.zmumueff.MaxEtaJet   = cms.double(2.5)

process.zmumurec.MinPtJet = 15.
process.zmumueff.MinPtJet = 15.
