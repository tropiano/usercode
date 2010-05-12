from PhysicsTools.PatAlgos.patTemplate_cfg import *

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = ['rfio:/castor/cern.ch/cms/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_356ReReco-v1/0009/FEFC70B6-F53D-DF11-B57E-003048679150.root'
#'rfio:/castor/cern.ch/cms/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/658/205F914C-B241-DF11-9FA7-003048D45FD8.root'
]
process.maxEvents.input = -1            

#################
#my modules
#################

#muons
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.selectedMuons.cut = cms.string('pt > 0. & abs(eta) < 3.')
process.zmumurec.cut = cms.string('0 < mass < 130') 
process.load("Firenze.Reduction.genZmumuPatAddOns_cff")
process.zmumugenfull.cut = cms.string('0 < mass < 10000')
process.zmumugen.cut = cms.string('0 < mass < 10000')

#electrons
process.load("Firenze.Reduction.recZeePatAddOns_cff")
process.selectedElectrons.cut = cms.string('pt > 0. & abs(eta) < 3.')
process.zeerec.cut = cms.string('0 < mass < 130')
process.load("Firenze.Reduction.genZeePatAddOns_cff")
process.zeegenfull.cut = cms.string('0 < mass < 10000')
process.zeegen.cut = cms.string('0 < mass < 10000')


#jets
process.load("Firenze.Reduction.recJetPatAddOns_cff")
process.selectedJets.cut = cms.string('pt > 3. & abs(eta) < 10. & nConstituents > 0')
process.selectedPFJets.cut = cms.string('pt > 3. & abs(eta) < 10.')
process.load("Firenze.Reduction.genJetPatAddOns_cff")
process.selectedGenJets.cut = cms.string('pt > 3. & abs(eta) < 10. & nConstituents > 0')

#################

#################
#customizartion
#################

#muons
process.muonMatch.checkCharge = cms.bool(False)

#electrons
process.electronMatch.checkCharge = cms.bool(False)

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

from PhysicsTools.PatAlgos.tools.coreTools import *

#trigger
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger(process)
switchOnTriggerMatchEmbedding( process )

process.muonTriggerMatchHLTL2Mu9 = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "patMuons" ),
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
 )
 

process.electronTriggerMatchHLTL1SingleEG8 = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "selectedPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ), 
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_L1SingleEG8' ),
    collectionTags = cms.vstring( '*' ),
     maxDPtRel = cms.double( 1.0 ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
     resolveByMatchQuality = cms.bool( False )
 ) 


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


process.patTriggerMatcher = cms.Sequence(process.muonTriggerMatchHLTL2Mu9 + process.electronTriggerMatchHLTL1SingleEG8 + process.jetTriggerMatch + process.pfjetTriggerMatch)

process.patTriggerEvent.patTriggerMatches=cms.VInputTag('muonTriggerMatchHLTL2Mu9', 'electronTriggerMatchHLTL1SingleEG8', 'jetTriggerMatch', 'pfjetTriggerMatch') 

process.selectedLayer1MuonsTriggerMatch = cms.EDProducer( "PATTriggerMatchMuonEmbedder",
     src     = cms.InputTag( "patMuons" ),
     matches = cms.VInputTag( 'muonTriggerMatchHLTL2Mu9')
)

process.selectedLayer1ElectronsTriggerMatch = cms.EDProducer( "PATTriggerMatchElectronEmbedder",
     src     = cms.InputTag( "selectedPatElectrons" ),
     matches = cms.VInputTag( 'electronTriggerMatchHLTL1SingleEG8')
)

process.selectedLayer1JetsTriggerMatch = cms.EDProducer( "PATTriggerMatchJetEmbedder",
     src     = cms.InputTag( "selectedPatJets" ),
     matches = cms.VInputTag('jetTriggerMatch')
)

process.selectedLayer1PFJetsTriggerMatch = cms.EDProducer( "PATTriggerMatchJetEmbedder",
     src     = cms.InputTag( "allLayer1PFJets" ),
     matches = cms.VInputTag('pfjetTriggerMatch')
)

process.patTriggerMatchEmbedder=cms.Sequence(process.selectedLayer1MuonsTriggerMatch + process.selectedLayer1ElectronsTriggerMatch + process.selectedLayer1JetsTriggerMatch + process.selectedLayer1PFJetsTriggerMatch)
process.selectedMuons.src=cms.InputTag('selectedLayer1MuonsTriggerMatch') 
process.selectedElectrons.src=cms.InputTag('selectedLayer1ElectronsTriggerMatch')
process.selectedJets.src=cms.InputTag('selectedLayer1JetsTriggerMatch')
process.selectedPFJets.src=cms.InputTag('selectedLayer1PFJetsTriggerMatch')

#?# 2) A che servono?
#process.eleIsoDepositEcalFromHits.ExtractorPSet.barrelEcalHits = cms.InputTag("reducedEcalRecHitsEB", "", "RECO")
#process.eleIsoDepositEcalFromHits.ExtractorPSet.endcapEcalHits = cms.InputTag("reducedEcalRecHitsEE", "", "RECO")
#process.eidRobustHighEnergy.reducedBarrelRecHitCollection = cms.InputTag("reducedEcalRecHitsEB", "", "RECO")
#process.eidRobustHighEnergy.reducedEndcapRecHitCollection = cms.InputTag("reducedEcalRecHitsEE", "", "RECO")
#################

#################
#change default jet collection
#################

process.patJetCorrFactors.corrLevels.L5Flavor='none'
process.patJetCorrFactors.corrSample='Summer09_7TeV'
#################

################# 
#Sequences and Paths
#################

process.pattuples = cms.Sequence(
                process.recPFjetsSequence*
                process.patDefaultSequence*
                process.zmumugenSequence*
                process.zmumurecSequence*
                process.zeegenSequence*
                process.zeerecSequence*
                process.genjetsSequence*
                process.recjetsSequence
            )

process.p = cms.Path(process.pattuples)
#################            

#################            
#customize Event content
#################
from Firenze.Reduction.recZmumuPatAddOns_cff import zmumurecEventContent
from Firenze.Reduction.genZmumuPatAddOns_cff import zmumugenEventContent
from Firenze.Reduction.recZeePatAddOns_cff import zeerecEventContent
from Firenze.Reduction.genZeePatAddOns_cff import zeegenEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from Firenze.Reduction.genJetPatAddOns_cff import jetgenEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zmumurecEventContent)
process.out.outputCommands.extend(zmumugenEventContent)
process.out.outputCommands.extend(zeerecEventContent)
process.out.outputCommands.extend(zeegenEventContent)
process.out.outputCommands.extend(jetrecEventContent)
process.out.outputCommands.extend(jetgenEventContent)
process.out.outputCommands.extend(patTriggerEventContent)
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*', 'keep *_patMETs_*_*'])
#process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'MC_7TeV_MinBias_Spring10_START3X_V26A_356ReReco_v1.root'
process.options.wantSummary = False       

print 'Current Event content is:'
print process.out.outputCommands
