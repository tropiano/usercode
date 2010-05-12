import FWCore.ParameterSet.Config as cms
from Firenze.PATAnalysis.config_Dec09_DATA import *

#process.load(config_background)

process.outputFile.name = cms.string("data.root")
#process.rapgap.jetSource   = cms.string("selectedJets")
