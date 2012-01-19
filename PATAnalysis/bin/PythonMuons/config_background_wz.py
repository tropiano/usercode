import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_background import *

#process.load(config_background)

process.outputFile.name = cms.string("wz.root")
process.zmumurec.ScaleFactor=cms.double(10.4/1935664) #inclusive cross section from MCFM
process.zmumueff.ScaleFactor=cms.double(10.4/1935664)
process.zmumueff.TrainUnfolding = cms.bool(False)

process.zmumurec.JetSource     = cms.string("selectedJets")
process.zmumurec.MaxEtaJet   = cms.double(2.4)

process.zmumueff.JetSource     = cms.string("selectedJets")
process.zmumueff.MaxEtaJet   = cms.double(2.4)

process.zmumurec.MinPtJet = 30.
process.zmumueff.MinPtJet = 30.

