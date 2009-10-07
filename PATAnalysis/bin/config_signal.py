import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_signal import *

#process.load(config_background)

process.outputFile.name = cms.string("signal.root")
