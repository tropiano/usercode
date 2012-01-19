import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_background import *

#process.load(config_background)

process.outputFile.name = cms.string("qcd.root")
process.zmumurec.ScaleFactor=cms.double(296600000.*0.0002855/24520391)
process.zmumueff.ScaleFactor=cms.double(296900000.*0.0002855/24520391)
process.zmumueff.TrainUnfolding = cms.bool(False)

process.zmumurec.JetSource     = cms.string("selectedJets")
process.zmumurec.MaxEtaJet   = cms.double(2.4)

process.zmumueff.JetSource     = cms.string("selectedJets")
process.zmumueff.MaxEtaJet   = cms.double(2.4)

process.zmumurec.MinPtJet = 30.
process.zmumueff.MinPtJet = 30.

