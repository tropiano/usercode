from PhysicsTools.PatAlgos.patTemplate_cfg import *

## uncomment here
process.source.fileNames = ['file:/tmp/egallo/Wenu_test.root']
process.maxEvents.input = -1              ## (e.g. -1 to run on all events)
process.GlobalTag.globaltag = cms.string('GR_R_38X_V15::All')

#################
#my modules, adapted from the electron selection
#################
process.load("Firenze.Reduction.recZeePatAddOns_cff")
process.selectedElectrons.cut = cms.string('pt > 8. & abs(eta) < 3.')
process.selectedElectrons.src = cms.InputTag('selectedPatElectronsTriggerMatch')
# process.zeerec.cut = cms.string('50 < mass < 130')
process.load("Firenze.Reduction.recJetPatAddOns_cff")
#################

#################
#customization
#################
#switch off MC matching
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])

#add JPT and PF jet collections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(process,cms.InputTag('ak5PFJets'),
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute','L2L3Residual'])),
                 doType1MET   = True,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = True
)

from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process)
process.patMETsPF.genMETSource=cms.InputTag('')
removeSpecificPATObjects(process, ['Photons', 'Muons', 'Taus'])

#########################
#####AREA SUBTRACTION####
#########################
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.ak5PFJets.doAreaFastjet = True
process.kt6PFJets.doRhoFastjet = True
process.load('JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff')
process.load('JetMETCorrections.Configuration.JetCorrectionProducersAllAlgos_cff')
process.L1Fastjet.algorithm = cms.string('AK5Calo') #DUMMY
process.L1Fastjet.era = 'Spring10'                  #DUMMY
process.L1Fastjet.level = cms.string('L2Relative')  #DUMMY
process.L1Fastjet.useCondDB = cms.untracked.bool(False)
process.offsetCorrection = cms.Sequence(process.ak5PFJets * process.kt6PFJets * process.ak5PFJetsL1)
addJetCollection(process,cms.InputTag('ak5PFJetsL1'),
                  'AK5', 'PFL1corrected',
                  doJTA        = True,
                  doBTagging   = True,
                  jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute','L2L3Residual'])),
                  doType1MET   = True,
                  doL1Cleaning = True,
                  doL1Counters = False,
                  genJetCollection=cms.InputTag("ak5GenJets"),
                  doJetID      = True,
                  jetIdLabel   = "ak5"
 )
process.selectedJetsL1Corrected = process.selectedJets.clone(src = cms.InputTag("patJetsAK5PFL1corrected"))
process.recjetsSequence += process.selectedJetsL1Corrected
###############################################################

from PhysicsTools.PatAlgos.tools.coreTools import removeCleaning
removeCleaning( process )

##################################
## adding electron identification
##################################
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")

process.patElectrons.addElectronID     = cms.bool( True )
process.patElectrons.electronIDSources = cms.PSet(
    simpleEleId95relIso = cms.InputTag("simpleEleId95relIso"),
    simpleEleId90relIso = cms.InputTag("simpleEleId90relIso"),
    simpleEleId85relIso = cms.InputTag("simpleEleId85relIso"),
    simpleEleId80relIso = cms.InputTag("simpleEleId80relIso"),
    simpleEleId70relIso = cms.InputTag("simpleEleId70relIso"),
    simpleEleId60relIso = cms.InputTag("simpleEleId60relIso"),
    simpleEleId95cIso   = cms.InputTag("simpleEleId95cIso"),
    simpleEleId90cIso   = cms.InputTag("simpleEleId90cIso"),
    simpleEleId85cIso   = cms.InputTag("simpleEleId85cIso"),
    simpleEleId80cIso   = cms.InputTag("simpleEleId80cIso"),
    simpleEleId70cIso   = cms.InputTag("simpleEleId70cIso"),
    simpleEleId60cIso   = cms.InputTag("simpleEleId60cIso")
                                                   )

process.patElectronIDs = cms.Sequence( process.simpleEleIdSequence )
process.makePatElectrons = cms.Sequence(process.patElectronIDs       *
                                        process.patElectronIsolation *
                                        process.patElectrons
                                        )

#################
#Sequences and Paths
#################

process.recosequence = cms.Sequence(#process.recPFjetsSequence*
                                    process.offsetCorrection*
                                    process.patDefaultSequence*
                                    process.zeerecSequence*
                                    process.recjetsSequence
                                    )

#request at least two electrons
process.skimPatElectrons=cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag("zeerec"),
   minNumber = cms.uint32(1),
   filter = cms.bool(True)
 )

#process.gensequence = cms.Sequence(process.genjetsSequence*process.zeegenSequence)

# for MC
# process.pattuples = cms.Sequence(process.gensequence + process.recosequence)

# for data
process.pattuples = cms.Sequence(process.recosequence)

# for skimming
process.p = cms.Path(process.pattuples*process.skimPatElectrons)

# no skim
#process.p = cms.Path(process.pattuples)

#################
#customize Event content
#################
from Firenze.Reduction.recZeePatAddOns_cff import zeerecEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent

process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zeerecEventContent)
process.out.outputCommands.extend(jetrecEventContent)
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep *_pat*METs*_*_*', 'keep *_patTriggerEvent_*_*', 'keep *_*PatElectrons*TriggerMatch_*_*', 'keep patTriggerPaths_patTrigger_*_*','keep *_goodTracks_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'DATANEW_Run2010A_Nov4ReReco.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

#add the trigger matching, triggers taken from AN2010_425
process.electronTriggerMatchHLTElectrons = cms.EDProducer(
   "PATTriggerMatcherDRLessByR",
    src     = cms.InputTag( 'selectedPatElectrons' ),
    matched = cms.InputTag( 'patTrigger' ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring('HLT_Ele10_LW_L1R', 'HLT_Ele15_SW_L1R','HLT_Ele15_SW_CaloEleId_L1R','HLT_Ele17_SW_CaloEleId_L1R','HLT_Ele17_SW_TightEleId_L1R','HLT_Ele17_SW_TighterEleIdIsol_L1R_v2','HLT_Ele17_SW_TighterEleIdIsol_L1R_v3' ),
    collectionTags = cms.vstring( '*' ),
    maxDeltaR   = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( True ) )
   
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )

from PhysicsTools.PatAlgos.tools.trigTools import switchOnTriggerMatchEmbedding
switchOnTriggerMatchEmbedding( process , triggerMatchers = [ 'electronTriggerMatchHLTElectrons' ])

print 'Current Event content is:'
print process.out.outputCommands
