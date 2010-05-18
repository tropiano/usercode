from PhysicsTools.PatAlgos.patTemplate_cfg import *

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = [
'rfio:/castor/cern.ch/cms/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/658/205F914C-B241-DF11-9FA7-003048D45FD8.root']
process.maxEvents.input = -1              ## (e.g. -1 to run on all events)
#process.source.firstRun = cms.untracked.uint32(122314)
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('123592:2-123592:14', '') 

#################
#skim bit 40-41
#################
#process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
#process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
#process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND 0 AND NOT (36 OR 37 OR 38 OR 39)')

#process.load('TKDPG/BXSelect/BXSelect_cfi')
#process.bxSelect.SelectBXs = [ 51, 2724 ]

#################
#my modules
#################
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.selectedMuons.cut = cms.string('pt > 5. & abs(eta) < 3.')
process.zmumurec.cut = cms.string('0 < mass < 130') 
process.load("Firenze.Reduction.recZeePatAddOns_cff")
process.load("Firenze.Reduction.recJetPatAddOns_cff")
process.selectedJets.cut = cms.string('pt > 10. & abs(eta) < 10. & nConstituents > 0')
process.selectedPFJets.cut = cms.string('pt > 10. & abs(eta) < 10.')
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
    src     = cms.InputTag( "selectedPatMuons" ),
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
 
process.patTriggerMatcher = cms.Sequence(process.muonTriggerMatchHLTL2Mu9 + process.jetTriggerMatch) # poi cambio la patTriggerMatcher sequence

process.patTriggerEvent.patTriggerMatches=cms.VInputTag('muonTriggerMatchHLTL2Mu9', 'jetTriggerMatch') #poi cambio i triggerMatches in patTriggerEvent

process.selectedLayer1MuonsTriggerMatch = cms.EDProducer( "PATTriggerMatchMuonEmbedder",
     src     = cms.InputTag( "selectedPatMuons" ),
     matches = cms.VInputTag( 'muonTriggerMatchHLTL2Mu9')
)

process.selectedLayer1JetsTriggerMatch = cms.EDProducer( "PATTriggerMatchJetEmbedder",
     src     = cms.InputTag( "selectedPatJets" ),
     matches = cms.VInputTag('jetTriggerMatch')
)

process.patTriggerMatchEmbedder=cms.Sequence(process.selectedLayer1MuonsTriggerMatch + process.selectedLayer1JetsTriggerMatch)
process.selectedMuons.src=cms.InputTag('selectedLayer1MuonsTriggerMatch') #uso gli elettroni con il trigger embedding
process.selectedJets.src=cms.InputTag('selectedLayer1JetsTriggerMatch')


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

#################
#change default jet collection
#################
#from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJetCollection(process, 
#                    cms.InputTag('sisCone5CaloJets'),   
#                    doJTA            = True,            
#                    doBTagging       = True,            
#                    jetCorrLabel     = ('SC5','Calo900'),  
#                    doType1MET       = True,            
#                    genJetCollection = cms.InputTag("sisCone5GenJets")
#                    )
process.patJetCorrFactors.corrLevels.L5Flavor='none'
process.patJetCorrFactors.corrSample='Summer09_7TeV'
#################

#################
#Tracks
#################
from PhysicsTools.RecoAlgos.recoTrackSelector_cfi import recoTrackSelector
process.selectedTracks = recoTrackSelector.clone();
#process.selectedTracks.ptMin = 1. #select tracks with pt > 1 GeV
process.selectedTracks.quality = cms.vstring('highPurity')
#################

################# 
#Sequences and Paths
#################
#process.skim = cms.Sequence(process.superClusterFilter * process.simpleElectronSelection * process.electronSuperClusterFilter)
#process.skim = cms.Sequence(process.hltLevel1GTSeed)

process.pattuples = cms.Sequence(
                #process.selectedTracks+
                process.patDefaultSequence+
                process.zmumurecSequence+
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
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*', 'keep *_layer1METs_*_*'])
#process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'Zmumu_MinB_Comm10_May6thPDSkim2_SD_Mu_v1_Run_132440_134987.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
