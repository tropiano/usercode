from PhysicsTools.PatAlgos.patTemplate_cfg import *

#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = [
    '/store/data/Run2011A/MultiJet/AOD/PromptReco-v1/000/161/311/F8D76AF3-0558-E011-B313-003048F0258C.root',
    '/store/data/Run2011A/MultiJet/AOD/PromptReco-v1/000/161/311/F8C8DD7E-FD57-E011-93AF-003048F0258C.root',
    '/store/data/Run2011A/MultiJet/AOD/PromptReco-v1/000/161/311/EAD6D6FB-EB57-E011-883A-003048F1182E.root',
    '/store/data/Run2011A/MultiJet/AOD/PromptReco-v1/000/161/311/E4443BEB-0C58-E011-A948-0030487C6062.root',
    '/store/data/Run2011A/MultiJet/AOD/PromptReco-v1/000/161/311/E060C17D-DC57-E011-A6DB-000423D9A212.root',
    '/store/data/Run2011A/MultiJet/AOD/PromptReco-v1/000/161/311/86AA9099-DE57-E011-8288-00304879FA4A.root',
    '/store/data/Run2011A/MultiJet/AOD/PromptReco-v1/000/161/311/3402EA31-5A58-E011-834B-003048F11114.root',
    '/store/data/Run2011A/MultiJet/AOD/PromptReco-v1/000/161/311/123AF31C-E257-E011-B44C-001D09F251CC.root',
    '/store/data/Run2011A/MultiJet/AOD/PromptReco-v1/000/161/311/0AD22BBA-E057-E011-B96B-001617DBCF6A.root',
    ]

import PhysicsTools.PythonAnalysis.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
myLumis = LumiList.LumiList(filename = 'run161311.json').getCMSSWString().split(',')
process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
process.source.lumisToProcess.extend(myLumis)

process.maxEvents.input = -1        ## (e.g. -1 to run on all events)
#process.source.firstRun = cms.untracked.uint32(122314)
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('160957:1-160957:953','') 
process.GlobalTag.globaltag = cms.string('GR_R_311_V2::All')

mytrigs = ['*']

#################
#my modules
#################
#process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.load("Firenze.Reduction.recJetPatAddOnsV1_cff")
#process.load("PhysicsTools.RecoAlgos.goodTracks_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
#process.goodTracks.cut = cut = cms.string('pt > 8')
#################
process.load("Firenze.Reduction.recTTbarPatAddOns_cff")
process.countJets.minNumber = cms.uint32(4)

#################
#customization
#################
#switch off MC matching
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])
#add JPT and PF jet colelctions
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(process,cms.InputTag('ak5PFJets'),
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5PF', cms.vstring(['L1Offset', 'L2Relative', 'L3Absolute', 'L2L3Residual'])),
                 doType1MET   = True,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = True
)

from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process)
process.patMETsPF.genMETSource=cms.InputTag('')
removeSpecificPATObjects(process, ['Photons', 'Taus'])

from HLTrigger.HLTfilters.hltHighLevel_cfi import *
if mytrigs is not None :
    process.hltSelection = hltHighLevel.clone(TriggerResultsTag = 'TriggerResults::HLT', HLTPaths = mytrigs)
    process.hltSelection.throw = False

#########################
#####AREA SUBRACTION#####
#########################
## process.load('RecoJets.Configuration.RecoPFJets_cff')
## process.ak5PFJets.doAreaFastjet = True 
## process.kt6PFJets.doRhoFastjet = True 
## process.load('JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff')
## process.load('JetMETCorrections.Configuration.JetCorrectionProducersAllAlgos_cff') 
## process.L1Fastjet.algorithm = cms.string('AK5Calo') #DUMMY
## process.L1Fastjet.era = 'Spring10' #DUMMY
## process.L1Fastjet.level = cms.string('L2Relative') #DUMMY
## process.L1Fastjet.useCondDB = cms.untracked.bool(False)
## #process.ak5PFL1L2L3Residual = process.ak5PFL2L3Residual.clone()
## #process.ak5PFL1L2L3Residual.correctors.insert(0,'L1Fastjet')
## #process.ak5PFJetsL1L2L3Residual=process.ak5PFJetsL1L2L3.clone(correctors = ['ak5PFL1L2L3Residual'])
## process.offsetCorrection = cms.Sequence(process.ak5PFJets * process.kt6PFJets * process.ak5PFJetsL1) 
## addJetCollection(process,cms.InputTag('ak5PFJetsL1'),
##                  'AK5', 'PFL1corrected',
##                  doJTA        = True,
##                  doBTagging   = False,
##                  jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute', 'L2L3Residual'])),
##                  doType1MET   = True,
##                  doL1Cleaning = True,                 
##                  doL1Counters = False,
##                  genJetCollection=cms.InputTag("ak5GenJets"),
##                  doJetID      = True,
##                  jetIdLabel   = "ak5"
## )
## process.selectedJetsL1Corrected = process.selectedJets.clone(src = cms.InputTag("patJetsAK5PFL1corrected"))
## process.recjetsSequence += process.selectedJetsL1Corrected
###############################################################

#add the trigger matching
## process.muonTriggerMatchHLTMuons = cms.EDProducer(
##   "PATTriggerMatcherDRLessByR"
## , src     = cms.InputTag( 'cleanPatMuons' )
## , matched = cms.InputTag( 'patTrigger' )
## , andOr          = cms.bool( False )
## , filterIdsEnum  = cms.vstring( 'TriggerMuon' )
## , filterIds      = cms.vint32( 0 )
## , filterLabels   = cms.vstring( '*' )
## , pathNames      = cms.vstring( 'HLT_Mu9','HLT_Mu11','HLT_Mu13','HLT_Mu15','HLT_Mu17','HLT_Mu19','HLT_Mu21','HLT_Mu25',\
##                                 'HLT_Mu13_v1','HLT_Mu15_v1','HLT_Mu17_v1','HLT_Mu19_v1','HLT_Mu21_v1','HLT_Mu25_v1',\
##                                 'HLT_L1Mu7','HLT_L1Mu7_1','HLT_L2Mu7','HLT_L2Mu7_v1' )
## , collectionTags = cms.vstring( '*' )
## , maxDPtRel   = cms.double( 0.5 ) # no effect here
## , maxDeltaR   = cms.double( 0.5 )
## , maxDeltaEta = cms.double( 0.2 ) # no effect here
## , resolveAmbiguities    = cms.bool( True )
## , resolveByMatchQuality = cms.bool( True )
## )

#add the trigger matching
## process.jetTriggerMatchHLTJets = cms.EDProducer(
##   "PATTriggerMatcherDRLessByR"
## , src     = cms.InputTag( 'cleanPatJets' )
## , matched = cms.InputTag( 'patTrigger' )
## , andOr          = cms.bool( False )
## , filterIdsEnum  = cms.vstring( 'TriggerJet' )
## , filterIds      = cms.vint32( 0 )
## , filterLabels   = cms.vstring( '*' )
## , pathNames      = cms.vstring( 'HLT_QuadJet25U_v3', 'HLT_Jet30U_v3' )
## , collectionTags = cms.vstring( '*' )
## , maxDPtRel   = cms.double( 0.5 ) # no effect here
## , maxDeltaR   = cms.double( 0.5 )
## , maxDeltaEta = cms.double( 0.2 ) # no effect here
## , resolveAmbiguities    = cms.bool( True )
## , resolveByMatchQuality = cms.bool( True )
## )

# require physics declared
process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

# require scraping filter
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                    applyfilter = cms.untracked.bool(True),
                                    debugOn = cms.untracked.bool(False),
                                    numtrack = cms.untracked.uint32(10),
                                    thresh = cms.untracked.double(0.2)
                                    )
# HB + HE noise filtering
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')

from PhysicsTools.PatAlgos.tools.coreTools import removeCleaning
removeCleaning( process )


from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process ) # This is optional and can be omitted.
## #switchOnTriggerMatching( process, triggerMatchers = [ 'muonTriggerMatchHLTMuons' ] )
## switchOnTriggerMatchEmbedding( process , triggerMatchers = [ 'jetTriggerMatchHLTJets' ])
## # Switch to selected PAT objects in the trigger matching
## removeCleaningFromTriggerMatching( process )
## process.jetTriggerMatchHLTJets.src     = cms.InputTag( 'patJets' )
## #process.selectedPatMuonsTriggerMatch.src = cms.InputTag( 'patMuons' )
## #process.selectedPatMuonsTriggerMatch.matches=cms.VInputTag('muonTriggerMatchHLTMuons')
# process.patTriggerSequence=cms.Sequence(process.patTrigger)#+process.selectedPatMuonsTriggerMatch)

#process.selectedMuons.src=cms.InputTag('selectedPatMuonsTriggerMatch')

#################
#################

## process.patMuons.embedTpfmsMuon = cms.bool(False)
## process.patMuons.embedHighLevelSelection = cms.bool(True)
## process.patMuons.embedCaloMETMuonCorrs = cms.bool(False)
## process.patMuons.embedPFCandidate = cms.bool(False)
## process.patMuons.embedTcMETMuonCorrs = cms.bool(False)
## process.patMuons.embedCombinedMuon = cms.bool(True)
## process.patMuons.embedStandAloneMuon = cms.bool(False)
## process.patMuons.embedPickyMuon = cms.bool(False)

################# 
#Sequences and Paths
#################
#request at least two muons
#from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import countPatMuons
#process.skimPatMuons = countPatMuons.clone()
#process.skimPatMuons.minNumber = cms.uint32(2)
#process.skimPatMuons.filter = cms.bool(True)
#process.skimPatMuons.src = 'selectedMuons'
## process.skimPatMuons=cms.EDFilter("CandViewCountFilter",
##   src = cms.InputTag("zmumurec"),
##   minNumber = cms.uint32(1),
##   filter = cms.bool(True)
## )


process.recosequence = cms.Sequence(#process.recPFjetsSequence*
                                    process.patDefaultSequence*
                                    process.ttbarrecSequence*
                                    #process.zmumurecSequence*
                                    process.recjetsSequence)

#process.pattuples = cms.Sequence(process.goodTracks+process.offsetCorrection*process.recosequence*process.skimPatMuons)
process.pattuples = cms.Sequence(process.patTrigger*
                                 process.recosequence)


process.p = cms.Path(process.pattuples)
#process.p = cms.Path(process.pattuples)
#################            

#################            
#customize Event content
#################
from Firenze.Reduction.recTTbarPatAddOns_cff import ttbarrecEventContent
#from Firenze.Reduction.recZmumuPatAddOns_cff import zmumurecEventContent
from Firenze.Reduction.recJetPatAddOnsV1_cff import jetrecEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('p'))

#process.out.outputCommands=cms.untracked.vstring('drop *')
#process.out.outputCommands.extend(zmumurecEventContent)

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
process.out.outputCommands = patEventContentNoCleaning
process.out.outputCommands += patExtraAodEventContent
process.out.outputCommands += ttbarrecEventContent
process.out.outputCommands += jetrecEventContent
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += [
    'drop patJets_selectedPatJets_*_*',
    'keep patJets_goodPatJets_*_*',    
    'keep recoPFCandidates_selectedPatJets*_*_*',
    'keep *_patTriggerEvent_*_*'
    ]


## process.out.outputCommands.extend(ttbarrecEventContent)
## process.out.outputCommands.extend(jetrecEventContent)
## process.out.outputCommands.extend(patTriggerEventContent)

## process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*',
##                                    'keep *_pat*METs*_*_*',
##                                    'keep *_patTriggerEvent_*_*',
##                                    'keep patTriggerPaths_patTrigger_*_*',
##                                    'keep *_goodTracks_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'Top_hadronic.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
