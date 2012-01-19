import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_signal import *

#process.load(config_background)

process.outputFile.name = cms.string("signalMadgraph.root")
process.zmumueff.TrainUnfolding = cms.bool(False)
process.zmumugen.ScaleFactor=cms.double(3048./2565097) #inclusive cross section from https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSections
process.zmumurec.ScaleFactor=cms.double(3048./2565097)
process.zmumueff.ScaleFactor=cms.double(3048./2565097)

process.zmumurec.JetSource     = cms.string("selectedJets")
process.zmumurec.MaxEtaJet   = cms.double(2.4)

process.zmumueff.JetSource     = cms.string("selectedJets")
process.zmumueff.MaxEtaJet   = cms.double(2.4)

process.zmumueffVsRec.JetSource     = cms.string("selectedJets")
process.zmumueffVsRec.MaxEtaJet   = cms.double(2.4)

process.zmumueffVsGen.JetSource     = cms.string("selectedJets")
process.zmumueffVsGen.MaxEtaJet   = cms.double(2.4)

process.zmumueffVsRec.RequireGenInAcceptance=cms.bool(True)

process.zmumugen.MinPtJet = 30.
process.zmumurec.MinPtJet = 30.
process.zmumueff.MinPtJet = 30.
process.zmumueffVsRec.MinPtJet = 30.
process.zmumueffVsGen.MinPtJet = 30.

