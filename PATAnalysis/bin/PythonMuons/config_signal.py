import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_signal import *

#process.load(config_background)

process.outputFile.name = cms.string("signal_FI.root")
process.zmumueff.TrainUnfolding = cms.bool(False)
process.zmumugen.ScaleFactor=cms.double(1667./5235) #inclusive cross section from MCFM
process.zmumurec.ScaleFactor=cms.double(1667./5235)
process.zmumueff.ScaleFactor=cms.double(1667./5235)

process.zmumugen.MaxEtaJet   = cms.double(2.5)

#process.zmumurec.JetSource     = cms.string("selectedPFJets")
process.zmumurec.JetSource     = cms.string("selectedJetsL1Corrected")
process.zmumurec.MaxEtaJet   = cms.double(2.5)

#process.zmumueff.JetSource     = cms.string("selectedPFJets")
process.zmumurec.JetSource     = cms.string("selectedJetsL1Corrected")
process.zmumueff.MaxEtaJet   = cms.double(2.5)

#process.zmumueffVsRec.JetSource     = cms.string("selectedPFJets")
process.zmumurec.JetSource     = cms.string("selectedJetsL1Corrected")
process.zmumueffVsRec.MaxEtaJet   = cms.double(2.5)

#process.zmumueffVsGen.JetSource     = cms.string("selectedPFJets")
process.zmumurec.JetSource     = cms.string("selectedJetsL1Corrected")
process.zmumueffVsGen.MaxEtaJet   = cms.double(2.5)

process.zmumueffVsRec.RequireGenInAcceptance=cms.bool(True)

process.zmumugen.MinPtJet = 30.
process.zmumurec.MinPtJet = 30.
process.zmumueff.MinPtJet = 30.
process.zmumueffVsRec.MinPtJet = 30.
process.zmumueffVsGen.MinPtJet = 30.
