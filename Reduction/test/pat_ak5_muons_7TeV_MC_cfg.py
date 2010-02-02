from PhysicsTools.PatAlgos.patTemplate_cfg import *

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = [
'file:Zmumu7TeV_testAOD.root']
process.maxEvents.input = 2000               ## (e.g. -1 to run on all events)

##global tag for electron re-reconstruction
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'MC_31X_V3::All'

#################
#my modules
#################
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.load("Firenze.Reduction.recZeePatAddOns_cff")
process.load("Firenze.Reduction.recJetPatAddOns_cff")
process.load("Firenze.Reduction.genZmumuPatAddOns_cff")
process.load("Firenze.Reduction.genJetPatAddOns_cff")
#################


#################
#customizartion
#################
#muons
process.muonMatch.checkCharge = cms.bool(False)

#jets
# run the 3.3.x software on Summer 09 MC from 3.1.x:
#   - change the name from "ak" (3.3.x) to "antikt) (3.1.x)
#   - run jet ID (not run in 3.1.x)
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
run33xOn31xMC( process,
               jetSrc = cms.InputTag("antikt5CaloJets"),
               jetIdTag = "antikt5"
              )
process.jetCorrFactors.corrSample = 'Summer09_7TeV' ## 'Summer09' for 10TeV
process.allLayer1Jets.addTagInfos = cms.bool(False)
process.allLayer1Jets.addBTagInfo = cms.bool(False)
process.allLayer1Jets.addPartonJetMatch = cms.bool(False)
process.allLayer1Jets.embedGenPartonMatch = cms.bool(False)
process.allLayer1Jets.addDiscriminators = cms.bool(False)
process.allLayer1Muons.embedTrack = cms.bool(False)
process.allLayer1Muons.embedStandAloneMuon = cms.bool(False)
process.allLayer1Muons.embedCombinedMuon = cms.bool(False)
process.allLayer1Muons.embedPickyMuon = cms.bool(False)
process.allLayer1Muons.embedTpfmsMuon = cms.bool(False)

restrictInputToAOD31X(process)

#trigger
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )
switchOnTriggerMatchEmbedding( process )
process.patTrigger.processName='HLT8E29' #per cambiare la il processo di trigger da HLT a HLT8E29
process.patTriggerEvent.processName='HLT8E29' #per cambiare la il processo di trigger da HLT a HLT8E29:
process.muonTriggerMatchHLTMu9 = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "selectedLayer1Muons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( 'TriggerMuon' ), # 'TriggerMuon' is the enum from trigger::TriggerObjectType for HLT muons
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_Mu9' ),
    collectionTags = cms.vstring( '*' ),
     maxDPtRel = cms.double( 1.0 ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
     resolveByMatchQuality = cms.bool( False )
 ) #per fare solo il matching con HLT_Mu9 faccio questo trigger matcher
process.jetTriggerMatch = cms.EDFilter( "PATTriggerMatcherDRLessByR",
    src     = cms.InputTag( "selectedLayer1Jets" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( 'TriggerJet' ), # 'TriggerMuon' is the enum from trigger::TriggerObjectType for HLT muons
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_Jet15U', 'HLT_Jet30U' ),
    collectionTags = cms.vstring( '*' ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
 )
 
process.patTriggerMatcher = cms.Sequence(process.muonTriggerMatchHLTMu9 + process.jetTriggerMatch) # poi cambio la patTriggerMatcher sequence
process.patTriggerEvent.patTriggerMatches=cms.VInputTag('muonTriggerMatchHLTMu9', 'jetTriggerMatch') #poi cambio i triggerMatches in patTriggerEvent
process.selectedLayer1MuonsTriggerMatch = cms.EDProducer( "PATTriggerMatchMuonEmbedder",
     src     = cms.InputTag( "selectedLayer1Muons" ),
     matches = cms.VInputTag( 'muonTriggerMatchHLTMu9')
)

process.selectedLayer1JetsTriggerMatch = cms.EDProducer( "PATTriggerMatchJetEmbedder",
     src     = cms.InputTag( "selectedLayer1Jets" ),
     matches = cms.VInputTag('jetTriggerMatch')
)

process.patTriggerMatchEmbedder=cms.Sequence(process.selectedLayer1MuonsTriggerMatch + process.selectedLayer1JetsTriggerMatch) #e eseguo l'embedding solo per i muoni
process.selectedMuons.src=cms.InputTag('selectedLayer1MuonsTriggerMatch') #uso i muoni con il trigger embedding
process.selectedJets.src=cms.InputTag('selectedLayer1JetsTriggerMatch')  #uso i muoni con il trigger embedding
#################

#################
#Sequences and Paths
#################

process.pattuples = cms.Sequence(
                #process.selectedTracks+
                process.patDefaultSequence+
                process.zmumugenSequence+
                process.zmumurecSequence+
                process.genjetsSequence+
                process.recjetsSequence
            )

#process.p = cms.Path(process.skim*process.pattuples)
process.p = cms.Path(process.pattuples)
#################            

#################            
#customize Event content
#################
from Firenze.Reduction.recZmumuPatAddOns_cff import zmumurecEventContent
from Firenze.Reduction.genZmumuPatAddOns_cff import zmumugenEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from Firenze.Reduction.genJetPatAddOns_cff import jetgenEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zmumurecEventContent)
process.out.outputCommands.extend(zmumugenEventContent)
process.out.outputCommands.extend(jetrecEventContent)
process.out.outputCommands.extend(jetgenEventContent)
process.out.outputCommands.extend(patTriggerEventContent)
#process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_selectedTracks_*_*', 'keep *_layer1METs_*_*'])
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*', 'keep *_layer1METs_*_*'])
#process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'mc.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
