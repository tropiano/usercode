from PhysicsTools.PatAlgos.patTemplate_cfg import *

#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = [
'file:datatest.root']
process.maxEvents.input = -1              ## (e.g. -1 to run on all events)
#process.source.firstRun = cms.untracked.uint32(122314)
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('123592:2-123592:14', '') 


#################
#my modules
#################
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.selectedMuons.cut = cms.string('pt > 5. & abs(eta) < 3.')
process.zmumurec.cut = cms.string('0 < mass < 130') 
process.load("Firenze.Reduction.recJetPatAddOns_cff")
process.selectedJets.cut = cms.string('pt > 10. & abs(eta) < 10. & nConstituents > 0')
process.selectedPFJets.cut = cms.string('pt > 10. & abs(eta) < 10.')
process.selectedJPTJets.cut = cms.string('pt > 10. & abs(eta) < 10.')
#################


#################
#customizartion
#################
#switch off MC matching
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])
#add JPT and PF jet colelctions
from PhysicsTools.PatAlgos.tools.jetTools import *
addJetCollection(process, cms.InputTag('ak5PFJets'), 
                 'PF', '', 
                 doJTA=True, 
                 doBTagging=False,
                 jetCorrLabel=('AK5', 'PF'), 
                 doType1MET=False, 
                 doL1Cleaning = False,
                 doL1Counters=False, 
                 genJetCollection=cms.InputTag(''), 
                 doJetID = True, 
                 jetIdLabel   = "ak5")
addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
                 'JPT', '',
                 doJTA        = True,
                 doBTagging   = False,
                 jetCorrLabel = ('AK5','JPT'),
                 doType1MET   = False,
                 doL1Cleaning = False,
                 doL1Counters = False,                 
                 genJetCollection = cms.InputTag(''),
                 doJetID      = True,
                 jetIdLabel   = "ak5"
                 )
#steer a few parameters for muons
process.patMuons.embedTrack = cms.bool(True)
process.patMuons.embedStandAloneMuon = cms.bool(True)
process.patMuons.embedCombinedMuon = cms.bool(True)
process.patMuons.embedPickyMuon = cms.bool(False)
process.patMuons.embedTpfmsMuon = cms.bool(False)

##add the also the PF and tc met
from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process)
addPfMET(process)
process.patMETsTC.genMETSource=cms.InputTag('')
process.patMETsPF.genMETSource=cms.InputTag('')
removeSpecificPATObjects(process, ['Photons', 'Electrons', 'Taus'])

#add the trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger(process)
switchOnTriggerMatchEmbedding( process )
process.patTriggerEvent.patTriggerMatches=cms.VInputTag()
from Firenze.Reduction.jetTriggerTools import *
from Firenze.Reduction.muonTriggerTools import *
jetTriggerTools(process, cms.InputTag('patJets'), 'calo', 'selectedJets')
jetTriggerTools(process, cms.InputTag('patJetsPF'), 'pf', 'selectedPFJets')
jetTriggerTools(process, cms.InputTag('patJetsJPT'), 'jpt', 'selectedJPTJets')
muonTriggerTools(process, cms.InputTag('patMuons'), '', 'selectedMuons')
process.patTriggerMatcher = process.NEWpatTriggerMatcher
process.patTriggerMatchEmbedder = process.NEWpatTriggerMatchEmbedder
print process.patTriggerMatcher
print process.patTriggerMatchEmbedder
#################
#################

################# 
#Sequences and Paths
#################
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.EWK_HLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.EWK_HLTFilter.HLTPaths = ["HLT_Mu9"]

process.skim = cms.Sequence(process.EWK_HLTFilter)

process.recosequence = cms.Sequence(#process.recPFjetsSequence*
                                    process.patDefaultSequence*
                                    process.zmumurecSequence*
                                    process.zmumurecSequence*
                                    process.recjetsSequence)

process.pattuples = cms.Sequence(process.recosequence)

#process.p = cms.Path(process.skim*process.pattuples)
process.p = cms.Path(process.pattuples)
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
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep *_pat*METs*_*_*', 'keep *_patTriggerEvent_*_*', 'keep patTriggerPaths_patTrigger_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'Zmumu_DATA_skim_reducedtrigger.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
