import FWCore.ParameterSet.Config as cms

from Firenze.PATAnalysis.config_signal import *

#process.load(config_background)

process.outputFile.name = cms.string("signal.root")
process.zmumueff.TrainUnfolding = cms.bool(False)
#process.zmumugen.ScaleFactor=cms.double(50.*1300./1630220.)
#process.zmumurec.ScaleFactor=cms.double(50.*1300./1630220.)
#process.zmumueff.ScaleFactor=cms.double(50.*1300./1630220.)
