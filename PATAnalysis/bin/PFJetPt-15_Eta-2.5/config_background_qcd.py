import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_background import *

#process.load(config_background)

process.outputFile.name = cms.string("qcd.root")
process.zmumurec.ScaleFactor=cms.double(296900000.*2.684e-4/5120334)
process.zmumueff.ScaleFactor=cms.double(296900000.*2.684e-4/5120334)
process.zmumueff.TrainUnfolding = cms.bool(False)

process.zmumurec.JetSource     = cms.string("selectedPFJets")
process.zmumurec.MaxEtaJet   = cms.double(2.5)

process.zmumueff.JetSource     = cms.string("selectedPFJets")
process.zmumueff.MaxEtaJet   = cms.double(2.5)

process.zmumurec.MinPtJet = 15.
process.zmumueff.MinPtJet = 15.

