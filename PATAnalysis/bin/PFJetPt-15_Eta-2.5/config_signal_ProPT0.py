import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_signal import *

#process.load(config_background)

process.outputFile.name = cms.string("signalProPT0.root")
process.zmumueff.TrainUnfolding = cms.bool(False)
process.zmumugen.ScaleFactor=cms.double(1667./2174000) #inclusive cross section from MCFM
process.zmumurec.ScaleFactor=cms.double(1667./2174000)
process.zmumueff.ScaleFactor=cms.double(1667./2174000)

process.zmumurec.JetSource     = cms.string("selectedPFJets")
process.zmumurec.MaxEtaJet   = cms.double(2.5)

process.zmumueff.JetSource     = cms.string("selectedPFJets")
process.zmumueff.MaxEtaJet   = cms.double(2.5)

process.zmumueffVsRec.JetSource     = cms.string("selectedPFJets")
process.zmumueffVsRec.MaxEtaJet   = cms.double(2.5)

process.zmumueffVsGen.JetSource     = cms.string("selectedPFJets")
process.zmumueffVsGen.MaxEtaJet   = cms.double(2.5)

process.zmumueffVsRec.RequireGenInAcceptance=cms.bool(True)

process.zmumugen.MinPtJet = 15.
process.zmumurec.MinPtJet = 15.
process.zmumueff.MinPtJet = 15.
process.zmumueffVsRec.MinPtJet = 15.
process.zmumueffVsGen.MinPtJet = 15.

