import FWCore.ParameterSet.Config as cms
from Firenze.PATAnalysis.config_Dec09_MC import *

#process.load(config_background)

process.outputFile.name = cms.string("mc.root")
