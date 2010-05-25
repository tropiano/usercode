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
process.load("Firenze.Reduction.genJetPatAddOns_cff")
process.selectedGenJets.cut = cms.string('pt > 10. & abs(eta) < 100. & nConstituents > 0')
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.selectedMuons.cut = cms.string('pt > 5. & abs(eta) < 3.')
process.zmumurec.cut = cms.string('0 < mass < 130') 
process.load("Firenze.Reduction.recJetPatAddOns_cff")
process.selectedJets.cut = cms.string('pt > 10. & abs(eta) < 10. & nConstituents > 0')
process.selectedPFJets.cut = cms.string('pt > 10. & abs(eta) < 10.')
#################


#################
#customizartion
#################
###fix missing ak5 genJets collection
process.load("RecoJets.Configuration.GenJetParticles_cff")
process.load("RecoJets.JetProducers.ak5GenJets_cfi")
process.ak5generatedJets = cms.Sequence(process.genParticlesForJets * process.ak5GenJets)
#process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.coreTools import *

from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger(process)
switchOnTriggerMatchEmbedding( process )
from PhysicsTools.PatAlgos.tools.jetTools import *
addJetCollection(process, cms.InputTag('ak5PFJets'), 'PF', '', 
                 doJTA=True, doBTagging=False,jetCorrLabel=('AK5', 'PF'), 
                 doType1MET=True, doL1Counters=False)
##add the also the PF met
process.patPFMETs = process.patMETs.clone(
    metSource = cms.InputTag("pfMet"),
    addTrigMatch = cms.bool(False),
    addMuonCorrections = cms.bool(False)
)
process.patDefaultSequence.replace( process.patMETs, process.patMETs + process.patPFMETs)
#remove unneeded sequences
removeSpecificPATObjects(process, ['Photons', 'Electrons', 'Taus'])

process.muonTriggerMatchHLTMu9 = cms.EDFilter( "PATTriggerMatcherDRLessByR",
    src     = cms.InputTag( 'patMuons' ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( 'TriggerL1Mu' ), 
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_Mu9', 'HLT_L2Mu11', 'HLT_L1Mu20' ),
    collectionTags = cms.vstring( '*' ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
     resolveByMatchQuality = cms.bool( False )
 ) #per fare solo il matching con HLT_L2Mu9 faccio questo trigger matcher
 
process.jetTriggerMatch = cms.EDFilter( "PATTriggerMatcherDRLessByR",
    src     = cms.InputTag( "patJets" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( 'TriggerJet' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_Jet15U', 'HLT_Jet30U' ),
    collectionTags = cms.vstring( '*' ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

process.pfjetTriggerMatch = cms.EDFilter( "PATTriggerMatcherDRLessByR",
    src     = cms.InputTag( "patJetsPF" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( 'TriggerJet' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_Jet15U', 'HLT_Jet30U' ),
    collectionTags = cms.vstring( '*' ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
 )


process.patTriggerMatcher = cms.Sequence(process.muonTriggerMatchHLTMu9 + process.jetTriggerMatch + process.pfjetTriggerMatch) # poi cambio la patTriggerMatcher sequence

process.patTriggerEvent.patTriggerMatches=cms.VInputTag('muonTriggerMatchHLTMu9', 'jetTriggerMatch', 'pfjetTriggerMatch') #poi cambio i triggerMatches in patTriggerEvent

process.selectedLayer1MuonsTriggerMatch = cms.EDProducer( "PATTriggerMatchMuonEmbedder",
     src     = cms.InputTag( 'patMuons' ),
     matches = cms.VInputTag( 'muonTriggerMatchHLTMu9')
)

process.selectedLayer1JetsTriggerMatch = cms.EDProducer( "PATTriggerMatchJetEmbedder",
     src     = cms.InputTag( "patJets" ),
     matches = cms.VInputTag('jetTriggerMatch')
)

process.selectedLayer1PFJetsTriggerMatch = cms.EDProducer( "PATTriggerMatchJetEmbedder",
     src     = cms.InputTag( "patJetsPF" ),
     matches = cms.VInputTag('pfjetTriggerMatch')
)

process.patTriggerMatchEmbedder=cms.Sequence(process.selectedLayer1MuonsTriggerMatch + process.selectedLayer1JetsTriggerMatch + process.selectedLayer1PFJetsTriggerMatch)
process.selectedMuons.src=cms.InputTag('selectedLayer1MuonsTriggerMatch') #uso gli elettroni con il trigger embedding
process.selectedJets.src=cms.InputTag('selectedLayer1JetsTriggerMatch')
process.selectedPFJets.src=cms.InputTag('selectedLayer1PFJetsTriggerMatch')
 
process.patJets.addTagInfos = cms.bool(False)
process.patJets.addBTagInfo = cms.bool(False)
process.patJets.addPartonJetMatch = cms.bool(False)
process.patJets.embedGenPartonMatch = cms.bool(False)
process.patJets.addDiscriminators = cms.bool(False)
process.patMuons.embedTrack = cms.bool(False)
process.patMuons.embedStandAloneMuon = cms.bool(False)
process.patMuons.embedCombinedMuon = cms.bool(False)
process.patMuons.embedPickyMuon = cms.bool(False)
process.patMuons.embedTpfmsMuon = cms.bool(False)
#################

#process.patJetCorrFactors.corrLevels.L5Flavor='none'
#################

#process.patTriggerEvent.triggerResults=cms.InputTag("TriggerResults", "", "REDIGI") 
#process.patTrigger.triggerResults=cms.InputTag("TriggerResults", "", "REDIGI") 
process.patTrigger.processName = "REDIGI"
process.patTriggerEvent.processName = "REDIGI"

################# 
#Sequences and Paths
#################
#process.skim = cms.Sequence()

process.recosequence = cms.Sequence(#process.recPFjetsSequence*
                                    process.patDefaultSequence*
                                    process.zmumurecSequence*
                                    process.recjetsSequence)
process.gensequence = cms.Sequence(process.ak5generatedJets*process.genjetsSequence*process.zmumugenSequence)                                    

process.pattuples = cms.Sequence(process.gensequence + process.recosequence)

#process.p = cms.Path(process.skim*process.pattuples)
process.p = cms.Path(process.pattuples)
#################            

#################            
#customize Event content
#################
from Firenze.Reduction.genZmumuPatAddOns_cff import zmumugenEventContent
from Firenze.Reduction.genJetPatAddOns_cff import jetgenEventContent
from Firenze.Reduction.recZmumuPatAddOns_cff import zmumurecEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zmumugenEventContent)
process.out.outputCommands.extend(jetgenEventContent)
process.out.outputCommands.extend(zmumurecEventContent)
process.out.outputCommands.extend(jetrecEventContent)
#process.out.outputCommands.extend(patTriggerEventContent)
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep *_pat*METs_*_*', 'keep *_patTriggerEvent_*_*', 'keep patTriggerPaths_patTrigger_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'Zmumu_MC.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
