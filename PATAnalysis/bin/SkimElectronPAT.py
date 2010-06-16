#from PhysicsTools.PatAlgos.patTemplate_cfg import *

import FWCore.ParameterSet.Config as cms

process = cms.Process("SKIM")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.load("Firenze.PATAnalysis.list_QCDPt20to30_2_cfi")

#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring(
#      'file:/data/sandro/PATtuple/MC/Spring10/7TeV_35X/Zee/QCD_EMEnriched_Pt20to30_Spring10_7TeV_akt5_356/QCD_Pt20to30_Spring10_7TeV_ak5_356_100_2.root'
#    )
#)

#import FWCore.Utilities.FileUtils as FileUtils
#filelist = FileUtils.loadListFromFile("list.txt")
#process.source = cms.Source('PoolSource',
#fileNames = cms.untracked.vstring(*filelist)
#)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#electronskim

process.load("Firenze.PATAnalysis.ElectronSkim_cfi")
process.ElectronSkim.masscut = cms.double(30)

#################
#Sequences and Paths
#################

process.skim = cms.Sequence(process.ElectronSkimSequence)

process.skimming = cms.Path(process.skim)

#################            

#################            
#customize Event content
#################
from Firenze.Reduction.recZeePatAddOns_cff import zeerecEventContent
from Firenze.Reduction.genZeePatAddOns_cff import zeegenEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from Firenze.Reduction.genJetPatAddOns_cff import jetgenEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent

process.out = cms.OutputModule("PoolOutputModule",
fileName = cms.untracked.string('file:/data/sfrosali/PATtuple/MC/Spring10/QCD_EMEnriched_Pt20to30_Skimmed/QCD_EMEnriched_Pt20to30_Spring10_Skimmed_2.root'),
# save only events passing the full path
SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('skimming') ),
# save PAT Layer 1 output; you need a '*' to
# unpack the list of commands 'patEventContent'
outputCommands = cms.untracked.vstring('drop *', *patEventContent ) 
)
                               
process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zeerecEventContent)
process.out.outputCommands.extend(zeegenEventContent)
process.out.outputCommands.extend(jetrecEventContent)
process.out.outputCommands.extend(jetgenEventContent)
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep *_pat*METs_*_*', 'keep *_patTriggerEvent_*_*', 'keep patTriggerPaths_patTrigger_*_*'])
process.out.dropMetaData = cms.untracked.string('DROPPED')

#process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands

process.e = cms.EndPath(process.out)
