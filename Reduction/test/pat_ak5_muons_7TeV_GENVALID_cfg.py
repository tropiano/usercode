from PhysicsTools.PatAlgos.patTemplate_cfg import *

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = [
'file:zmumupythiatest.root']
process.maxEvents.input = -1              ## (e.g. -1 to run on all events)
#process.source.firstRun = cms.untracked.uint32(122314)
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('123592:2-123592:14', '') 


#################
#my modules
#################
process.load("Firenze.Reduction.genZmumuPatAddOns_cff")
process.load("Firenze.Reduction.genWmunuPatAddOns_cff")
process.load("Firenze.Reduction.genJetPatAddOns_cff")
process.selectedGenJets.cut = cms.string('pt > 10. & abs(eta) < 100. & nConstituents > 0')
#################


#################
#customizartion
#################
###fix missing ak5 genJets collection
process.load("RecoJets.Configuration.GenJetParticles_cff")
process.load("RecoJets.JetProducers.ak5GenJets_cfi")
process.ak5generatedJets = cms.Sequence(process.genParticlesForJets * process.ak5GenJets)

process.gensequence = cms.Sequence(process.ak5generatedJets*process.genjetsSequence*process.zmumugenSequence*process.wMunugenSequence)

process.pattuples = cms.Sequence(process.gensequence)

process.p = cms.Path(process.pattuples)
#################            

#################            
#customize Event content
#################
from Firenze.Reduction.genZmumuPatAddOns_cff import zmumugenEventContent
from Firenze.Reduction.genWmunuPatAddOns_cff import wMunugenEventContent
from Firenze.Reduction.genJetPatAddOns_cff import jetgenEventContent

process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zmumugenEventContent)
process.out.outputCommands.extend(wMunugenEventContent)
process.out.outputCommands.extend(jetgenEventContent)

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'Zmumu_MC.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
