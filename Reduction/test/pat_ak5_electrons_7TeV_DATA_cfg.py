from PhysicsTools.PatAlgos.patTemplate_cfg import *

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = [
'file:/data/sandro/CMSSW_3_5_6/src/Firenze/Reduction/test/FA0CE9B9-4B65-DF11-BC5F-003048D47A40.root']
process.maxEvents.input = 10000          ## (e.g. -1 to run on all events)


#################
#my modules
#################
process.load("Firenze.Reduction.recZeePatAddOns_cff")
process.selectedElectrons.cut = cms.string('pt > 5. & abs(eta) < 3.')
process.zeerec.cut = cms.string('0 < mass < 130')
process.load("Firenze.Reduction.recJetPatAddOns_cff")
process.selectedJets.cut = cms.string('pt > 10. & abs(eta) < 10. & nConstituents > 0')
process.selectedPFJets.cut = cms.string('pt > 10. & abs(eta) < 10.')

#egammaskims
#process.load("EGamma.EGammaSkims.electronFilter_cff")
#process.load("EGamma.EGammaSkims.simpleElectronSelection_cfi")
#process.load("EGamma.EGammaSkims.superClusterFilter_cff")
#process.load("EGamma.EGammaSkims.electronSuperClusterFilter_cff")
#process.electronSuperClusterCombiner.cut='mass > 20.'
#process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
##################

#################
#customizartion
#################
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
    addMuonCorrections = cms.bool(False),
    addGenMET = cms.bool(False)
)
process.patDefaultSequence.replace( process.patMETs, process.patMETs + process.patPFMETs)
#remove unneeded sequences
removeSpecificPATObjects(process, ['Photons', 'Muons', 'Taus'])

#switch off MC matching
process.patJetsPF.addGenJetMatch=False                 
process.patJetsPF.addGenPartonMatch=False
process.patJetsPF.getJetMCFlavour=False
process.patDefaultSequence.remove(process.patJetPartonMatchPF)
process.patDefaultSequence.remove(process.patJetGenJetMatchPF)
removeMCMatching(process, ['All'])

#trigger
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )
switchOnTriggerMatchEmbedding( process )


process.electronTriggerMatch = cms.EDFilter( "PATTriggerMatcherDRLessByR",
    src     = cms.InputTag( "patElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ), 
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_Photon15_L1R', 'HLT_Photon20_L1R', 'HLT_Ele15_LW_L1R' ),
    collectionTags = cms.vstring( '*' ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
 ) #per fare solo il matching con HLT_Mu9 faccio questo trigger matcher
 
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

process.patTriggerMatcher = cms.Sequence(process.electronTriggerMatch + process.jetTriggerMatch + process.pfjetTriggerMatch) # poi cambio la patTriggerMatcher sequence

process.patTriggerEvent.patTriggerMatches=cms.VInputTag('electronTriggerMatch', 'jetTriggerMatch', 'pfjetTriggerMatch') #poi cambio i triggerMatches in patTriggerEvent

process.selectedLayer1ElectronsTriggerMatch = cms.EDProducer( "PATTriggerMatchElectronEmbedder",
     src     = cms.InputTag( "patElectrons" ),
     matches = cms.VInputTag( 'electronTriggerMatch')
)

process.selectedLayer1JetsTriggerMatch = cms.EDProducer( "PATTriggerMatchJetEmbedder",
     src     = cms.InputTag( "patJets" ),
     matches = cms.VInputTag('jetTriggerMatch')
)

process.selectedLayer1PFJetsTriggerMatch = cms.EDProducer( "PATTriggerMatchJetEmbedder",
     src     = cms.InputTag( "patJetsPF" ),
     matches = cms.VInputTag('pfjetTriggerMatch')
)

process.patTriggerMatchEmbedder=cms.Sequence(process.selectedLayer1ElectronsTriggerMatch + process.selectedLayer1JetsTriggerMatch + process.selectedLayer1PFJetsTriggerMatch)
process.selectedElectrons.src=cms.InputTag('selectedLayer1ElectronsTriggerMatch') #uso gli elettroni con il trigger embedding
process.selectedJets.src=cms.InputTag('selectedLayer1JetsTriggerMatch')  
process.selectedPFJets.src=cms.InputTag('selectedLayer1PFJetsTriggerMatch')  

process.patJets.addTagInfos = cms.bool(False)
process.patJets.addBTagInfo = cms.bool(False)
process.patJets.addPartonJetMatch = cms.bool(False)
process.patJets.embedGenPartonMatch = cms.bool(False)
process.patJets.addDiscriminators = cms.bool(False)

#electrons
process.electronMatch.checkCharge = False

#################
#Sequences and Paths
#################


#process.skim = cms.Sequence(process.superClusterFilter * process.simpleElectronSelection * process.electronSuperClusterFilter)

process.recosequence = cms.Sequence(process.patDefaultSequence*
                                    process.zeerecSequence*
                                    process.recjetsSequence)

process.pattuples = cms.Sequence(process.recosequence)

#process.p = cms.Path(process.skim*process.pattuples)
process.p = cms.Path(process.pattuples)
#################            

#################            
#customize Event content
#################
from Firenze.Reduction.recZeePatAddOns_cff import zeerecEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zeerecEventContent)
process.out.outputCommands.extend(jetrecEventContent)
#process.out.outputCommands.extend(patTriggerEventContent)
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep *_pat*METs_*_*', 'keep *_patTriggerEvent_*_*', 'keep patTriggerPaths_patTrigger_*_*'])
process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'Zee_DATA.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
