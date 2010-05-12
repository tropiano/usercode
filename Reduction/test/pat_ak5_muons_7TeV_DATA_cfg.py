from PhysicsTools.PatAlgos.patTemplate_cfg import *

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = [
'file:7CBA6E3E-874F-DF11-B955-003048D47A0C.root']
process.maxEvents.input = -1              ## (e.g. -1 to run on all events)
#process.source.firstRun = cms.untracked.uint32(122314)
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('123592:2-123592:14', '') 


#################
#my modules
#################
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.selectedMuons.cut = cms.string('pt > 0. & abs(eta) < 3.')
process.zmumurec.cut = cms.string('0 < mass < 130') 
process.load("Firenze.Reduction.recJetPatAddOns_cff")
process.selectedJets.cut = cms.string('pt > 3. & abs(eta) < 10. & nConstituents > 0')
process.selectedPFJets.cut = cms.string('pt > 3. & abs(eta) < 10.')
#################


#################
#customizartion
#################
#process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])
process.allLayer1PFJets.addGenJetMatch = cms.bool(False)
process.allLayer1PFJetsSequence.remove(process.patPFJetGenJetMatch)
#restrictInputToAOD(process, ['All'])
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger(process)
switchOnTriggerMatchEmbedding( process )

process.muonTriggerMatchHLTL2Mu9 = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( 'patMuons' ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( 'TriggerMuon' ), 
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_L2Mu9' ),
    collectionTags = cms.vstring( '*' ),
     maxDPtRel = cms.double( 1.0 ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
     resolveByMatchQuality = cms.bool( False )
 ) #per fare solo il matching con HLT_L2Mu9 faccio questo trigger matcher
 
process.jetTriggerMatch = cms.EDFilter( "PATTriggerMatcherDRLessByR",
    src     = cms.InputTag( "selectedPatJets" ),
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
    src     = cms.InputTag( "allLayer1PFJets" ),
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


process.patTriggerMatcher = cms.Sequence(process.muonTriggerMatchHLTL2Mu9 + process.jetTriggerMatch + process.pfjetTriggerMatch) # poi cambio la patTriggerMatcher sequence

process.patTriggerEvent.patTriggerMatches=cms.VInputTag('muonTriggerMatchHLTL2Mu9', 'jetTriggerMatch', 'pfjetTriggerMatch') #poi cambio i triggerMatches in patTriggerEvent

process.selectedLayer1MuonsTriggerMatch = cms.EDProducer( "PATTriggerMatchMuonEmbedder",
     src     = cms.InputTag( 'patMuons' ),
     matches = cms.VInputTag( 'muonTriggerMatchHLTL2Mu9')
)

process.selectedLayer1JetsTriggerMatch = cms.EDProducer( "PATTriggerMatchJetEmbedder",
     src     = cms.InputTag( "selectedPatJets" ),
     matches = cms.VInputTag('jetTriggerMatch')
)

process.selectedLayer1PFJetsTriggerMatch = cms.EDProducer( "PATTriggerMatchJetEmbedder",
     src     = cms.InputTag( "allLayer1PFJets" ),
     matches = cms.VInputTag('pfjetTriggerMatch')
)

process.patTriggerMatchEmbedder=cms.Sequence(process.selectedLayer1MuonsTriggerMatch + process.selectedLayer1JetsTriggerMatch + process.selectedLayer1PFJetsTriggerMatch)
process.selectedMuons.src=cms.InputTag('selectedLayer1MuonsTriggerMatch') #uso gli elettroni con il trigger embedding
process.selectedJets.src=cms.InputTag('selectedLayer1JetsTriggerMatch')
process.selectedPFJets.src=cms.InputTag('selectedLayer1PFJetsTriggerMatch')
 
process.eleIsoDepositEcalFromHits.ExtractorPSet.barrelEcalHits = cms.InputTag("reducedEcalRecHitsEB", "", "RECO")
process.eleIsoDepositEcalFromHits.ExtractorPSet.endcapEcalHits = cms.InputTag("reducedEcalRecHitsEE", "", "RECO")
process.eidRobustHighEnergy.reducedBarrelRecHitCollection = cms.InputTag("reducedEcalRecHitsEB", "", "RECO")
process.eidRobustHighEnergy.reducedEndcapRecHitCollection = cms.InputTag("reducedEcalRecHitsEE", "", "RECO")
#process.patJets.addJetID = cms.bool(False)
process.patJets.addTagInfos = cms.bool(False)
process.patJets.addBTagInfo = cms.bool(False)
process.patJets.addPartonJetMatch = cms.bool(False)
process.patJets.embedGenPartonMatch = cms.bool(False)
process.patJets.addDiscriminators = cms.bool(False)
#process.patJets.addAssociatedTracks = cms.bool(False)
process.patMuons.embedTrack = cms.bool(False)
process.patMuons.embedStandAloneMuon = cms.bool(False)
process.patMuons.embedCombinedMuon = cms.bool(False)
process.patMuons.embedPickyMuon = cms.bool(False)
process.patMuons.embedTpfmsMuon = cms.bool(False)
#################

process.patJetCorrFactors.corrLevels.L5Flavor='none'
process.patJetCorrFactors.corrSample='Summer09_7TeV'
#################

process.patTriggerEvent.triggerResults=cms.InputTag("TriggerResults", "", "HLT") 
process.patTrigger.triggerResults=cms.InputTag("TriggerResults", "", "HLT") 

################# 
#Sequences and Paths
#################
#process.skim = cms.Sequence(process.superClusterFilter * process.simpleElectronSelection * process.electronSuperClusterFilter)
#process.skim = cms.Sequence(process.hltLevel1GTSeed)

process.pattuples = cms.Sequence(
                #process.selectedTracks+
                process.recPFjetsSequence*
                process.patDefaultSequence*
                process.zmumurecSequence*
                process.recjetsSequence
            )

#process.p = cms.Path(process.skim*process.pattuples)
process.p = cms.Path(process.pattuples)
#################            

#################            
#customize Event content
#################
from Firenze.Reduction.recZmumuPatAddOns_cff import zmumurecEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zmumurecEventContent)
process.out.outputCommands.extend(jetrecEventContent)
process.out.outputCommands.extend(patTriggerEventContent)
#process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_selectedTracks_*_*', 'keep *_layer1METs_*_*'])
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*', 'keep *_patMETs_*_*'])
#process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'Zmumu_MinBias_Comm_10_GOODCOLL_v8.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
