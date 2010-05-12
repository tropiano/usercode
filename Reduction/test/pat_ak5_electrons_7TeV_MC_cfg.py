from PhysicsTools.PatAlgos.patTemplate_cfg import *

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = [
'file:Zee_7TeV_AOD_test.root']
process.maxEvents.input = 10000          ## (e.g. -1 to run on all events)

#################
#my modules
#################
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.load("Firenze.Reduction.recZeePatAddOns_cff")
process.load("Firenze.Reduction.recJetPatAddOns_cff")
process.load("Firenze.Reduction.genZmumuPatAddOns_cff")
process.load("Firenze.Reduction.genZeePatAddOns_cff")
process.load("Firenze.Reduction.genJetPatAddOns_cff")
#################

#egammaskims
process.load("EGamma.EGammaSkims.electronFilter_cff")
process.load("EGamma.EGammaSkims.simpleElectronSelection_cfi")
process.load("EGamma.EGammaSkims.superClusterFilter_cff")
process.load("EGamma.EGammaSkims.electronSuperClusterFilter_cff")
process.electronSuperClusterCombiner.cut='mass > 20.'
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
##################

#################
#customizartion
#################

#jets
# run the 3.3.x software on Summer 09 MC from 3.1.x:
#   - change the name from "ak" (3.3.x) to "antikt) (3.1.x)
#   - run jet ID (not run in 3.1.x)
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
run33xOn31xMC( process,
               jetSrc = cms.InputTag("antikt5CaloJets"),
               jetIdTag = "antikt5"
              )
process.jetCorrFactors.corrSample = 'Summer09_7TeV' ## 'Summer09' for 10TeV, 'Summer09_7TeV' for 7TeV
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
process.patTrigger.processName='HLT8E29' #per cambiare il processo di trigger da HLT a HLT8E29
process.patTriggerEvent.processName='HLT8E29' #per cambiare la il processo di trigger da HLT a HLT8E29:

process.electronTriggerMatch = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "selectedLayer1Electrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ), 
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_Ele10_LW_L1R' ),
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
 
process.patTriggerMatcher = cms.Sequence(process.electronTriggerMatch + process.jetTriggerMatch + process.pfjetTriggerMatch) # poi cambio la patTriggerMatcher sequence

process.patTriggerEvent.patTriggerMatches=cms.VInputTag('electronTriggerMatch', 'jetTriggerMatch') #poi cambio i triggerMatches in patTriggerEvent

process.selectedLayer1ElectronsTriggerMatch = cms.EDProducer( "PATTriggerMatchElectronEmbedder",
     src     = cms.InputTag( "selectedLayer1Electrons" ),
     matches = cms.VInputTag( 'electronTriggerMatch')
)

process.selectedLayer1JetsTriggerMatch = cms.EDProducer( "PATTriggerMatchJetEmbedder",
     src     = cms.InputTag( "selectedLayer1Jets" ),
     matches = cms.VInputTag('jetTriggerMatch')
)

process.patTriggerMatchEmbedder=cms.Sequence(process.selectedLayer1ElectronsTriggerMatch + process.selectedLayer1JetsTriggerMatch)
process.selectedElectrons.src=cms.InputTag('selectedLayer1ElectronsTriggerMatch') #uso gli elettroni con il trigger embedding
process.selectedJets.src=cms.InputTag('selectedLayer1JetsTriggerMatch')  


process.electronMatch.checkCharge = False

#load the gsfElectron sequence to rerun electron reconstrunction and make use of majority charge method
#################


#################
#Sequences and Paths
#################



process.pattuples = cms.Sequence(
                #process.selectedTracks+
                process.patDefaultSequence+
                process.zeegenSequence+
                process.zeerecSequence+
                process.genjetsSequence+
                process.recjetsSequence )
            )

#process.p = cms.Path(process.skim*process.pattuples)
process.p = cms.Path(process.pattuples)
#################            

#################            
#customize Event content
#################
from Firenze.Reduction.recZeePatAddOns_cff import zeerecEventContent
from Firenze.Reduction.genZeePatAddOns_cff import zeegenEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from Firenze.Reduction.genJetPatAddOns_cff import jetgenEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zeerecEventContent)
process.out.outputCommands.extend(zeegenEventContent)
process.out.outputCommands.extend(jetrecEventContent)
process.out.outputCommands.extend(jetgenEventContent)
process.out.outputCommands.extend(patTriggerEventContent)
#process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_selectedTracks_*_*', 'keep *_layer1METs_*_*'])
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*', 'keep *_layer1METs_*_*'])
#process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'ZeeSummer09_ak5_335.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
