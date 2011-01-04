import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_signal import *

#process.load(config_background)

process.outputFile.name = cms.string("signalMadgraphOffset.root")
process.zmumueff.TrainUnfolding = cms.bool(False)
process.zmumugen.ScaleFactor=cms.double(3048./2565097) #inclusive cross section from https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSections
process.zmumurec.ScaleFactor=cms.double(3048./2565097)
process.zmumueff.ScaleFactor=cms.double(3048./2565097)

process.zmumurec.JetSource     = cms.string("selectedJetsL1Corrected")
process.zmumurec.MaxEtaJet   = cms.double(2.5)

process.zmumueff.JetSource     = cms.string("selectedJetsL1Corrected")
process.zmumueff.MaxEtaJet   = cms.double(2.5)

process.zmumueffVsRec.JetSource     = cms.string("selectedJetsL1Corrected")
process.zmumueffVsRec.MaxEtaJet   = cms.double(2.5)

process.zmumueffVsGen.JetSource     = cms.string("selectedJetsL1Corrected")
process.zmumueffVsGen.MaxEtaJet   = cms.double(2.5)

process.zmumueffVsRec.RequireGenInAcceptance=cms.bool(True)

process.zmumugen.MinPtJet = 15.
process.zmumurec.MinPtJet = 15.
process.zmumueff.MinPtJet = 15.
process.zmumueffVsRec.MinPtJet = 15.
process.zmumueffVsGen.MinPtJet = 15.

