from PhysicsTools.PatAlgos.patTemplate_cfg import *

#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = [
'file:runBtest.root']
process.maxEvents.input = -1              ## (e.g. -1 to run on all events)
#process.source.firstRun = cms.untracked.uint32(122314)
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('123592:2-123592:14', '') 


#################
#my modules
#################
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.load("Firenze.Reduction.recJetPatAddOns_cff")
#################


#################
#customizartion
#################
#switch off MC matching
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])
#add JPT and PF jet colelctions
from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJetCollection(process, cms.InputTag('ak5PFJets'),
#                 doJTA=False,
#                 doBTagging=False,
#                 jetCorrLabel=('AK5', 'PF'), 
#                 doType1MET=False, 
#                 genJetCollection=cms.InputTag(''), 
#                 doJetID = True, 
#                 jetIdLabel   = "ak5")
switchJetCollection(process,cms.InputTag('ak5PFJets'),
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute', 'L2L3Residual'])),
                 doType1MET   = True,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = True
)

from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process)
process.patMETsPF.genMETSource=cms.InputTag('')
removeSpecificPATObjects(process, ['Photons', 'Electrons', 'Taus'])

#########################
#####AREA SUBRACTION#####
#########################
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.ak5PFJets.doAreaFastjet = True 
process.kt6PFJets.doRhoFastjet = True 
process.load('JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff')
process.load('JetMETCorrections.Configuration.JetCorrectionProducersAllAlgos_cff') 
process.L1Fastjet.algorithm = cms.string('AK5Calo') #DUMMY
process.L1Fastjet.era = 'Spring10' #DUMMY
process.L1Fastjet.level = cms.string('L2Relative') #DUMMY
process.L1Fastjet.useCondDB = cms.untracked.bool(False)
process.ak5PFL1L2L3Residual = process.ak5PFL2L3Residual.clone()
process.ak5PFL1L2L3Residual.correctors.insert(0,'L1Fastjet')
process.ak5PFJetsL1L2L3Residual=process.ak5PFJetsL1L2L3.clone(correctors = ['ak5PFL1L2L3Residual'])
process.offsetCorrection = cms.Sequence(process.ak5PFJets * process.kt6PFJets * process.ak5PFJetsL1L2L3Residual)  
###############################################################

#add the trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger(process)
switchOnTriggerMatchEmbedding( process )
process.cleanPatMuonsTriggerMatch.matches=cms.VInputTag('cleanMuonTriggerMatchHLTMu9')
process.cleanMuonTriggerMatchHLTMu9.src = cms.InputTag("patMuons")
process.cleanPatMuonsTriggerMatch.src = cms.InputTag("patMuons")
process.patTriggerSequence=cms.Sequence(process.patTrigger+process.cleanMuonTriggerMatchHLTMu9+process.cleanPatMuonsTriggerMatch)
process.selectedMuons.src=cms.InputTag('cleanPatMuonsTriggerMatch')
#################
#################

process.patMuons.embedTpfmsMuon = cms.bool(False)
process.patMuons.embedHighLevelSelection = cms.bool(True)
process.patMuons.embedCaloMETMuonCorrs = cms.bool(False)
process.patMuons.embedPFCandidate = cms.bool(False)
process.patMuons.embedTcMETMuonCorrs = cms.bool(False)
process.patMuons.embedCombinedMuon = cms.bool(True)
process.patMuons.embedStandAloneMuon = cms.bool(False)
process.patMuons.embedPickyMuon = cms.bool(False)

################# 
#Sequences and Paths
#################
#request at least two muons
from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import countPatMuons
process.skimPatMuons = countPatMuons.clone()
process.skimPatMuons.minNumber = cms.uint32(2)
process.skimPatMuons.filter = cms.bool(True)
process.skimPatMuons.src = 'selectedMuons'

process.recosequence = cms.Sequence(#process.recPFjetsSequence*
                                    process.patDefaultSequence*
                                    process.zmumurecSequence*
                                    process.recjetsSequence)

process.pattuples = cms.Sequence(process.offsetCorrection*process.recosequence*process.skimPatMuons)

process.p = cms.Path(process.pattuples)
#process.p = cms.Path(process.pattuples)
#################            

#################            
#customize Event content
#################
from Firenze.Reduction.recZmumuPatAddOns_cff import zmumurecEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('p'))

process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zmumurecEventContent)
process.out.outputCommands.extend(jetrecEventContent)
#process.out.outputCommands.extend(patTriggerEventContent)
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep *_pat*METs*_*_*', 'keep *_patTriggerEvent_*_*', 'keep patTriggerPaths_patTrigger_*_*', 'keep *_ak5PFJets*L2L3*_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'Zmumu_DATA_skim.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
