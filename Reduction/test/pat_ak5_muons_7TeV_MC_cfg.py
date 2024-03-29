from PhysicsTools.PatAlgos.patTemplate_cfg import *

#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = [
'file:/tmp/lenzip/mcpileuptest.root']
process.maxEvents.input = -1              ## (e.g. -1 to run on all events)
#process.source.firstRun = cms.untracked.uint32(122314)
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('123592:2-123592:14', '') 
process.GlobalTag.globaltag = cms.string('START38_V14::All')

#################
#my modules
#################
process.load("Firenze.Reduction.genZmumuPatAddOns_cff")
process.load("Firenze.Reduction.genJetPatAddOns_cff")
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.load("Firenze.Reduction.recJetPatAddOns_cff")
process.load("PhysicsTools.RecoAlgos.goodTracks_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.goodTracks.cut = cut = cms.string('pt > 8')
#################


#################
#customizartion
#################
from PhysicsTools.PatAlgos.tools.coreTools import *
#add JPT and PF jet collections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(process,cms.InputTag('ak5PFJets'),
                 doJTA=True,
                 doBTagging=True,
                 jetCorrLabel=('AK5PF', cms.vstring(['L2Relative', 'L3Absolute'])),
                 doType1MET=True,
		 genJetCollection=cms.InputTag("ak5GenJets"),
		 doJetID      = True
		 )

#steer a few parameters for muons
#process.patMuons.embedTrack = cms.bool(True)
#process.patMuons.embedStandAloneMuon = cms.bool(True)
#process.patMuons.embedCombinedMuon = cms.bool(True)
#process.patMuons.embedPickyMuon = cms.bool(False)
#process.patMuons.embedTpfmsMuon = cms.bool(False)


##add the also the PF and tc met
from PhysicsTools.PatAlgos.tools.metTools import *
#addTcMET(process)
addPfMET(process)
removeSpecificPATObjects(process, ['Photons', 'Electrons', 'Taus'])

#########################
#####AREA SUBRACTION#####
#########################
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.ak5PFJets.doAreaFastjet = True
process.kt6PFJets.doRhoFastjet = True
process.load('JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff')
process.load('JetMETCorrections.Configuration.JetCorrectionProducersAllAlgos_cff')
process.L1Fastjet.algorithm = cms.string('AK5Calo') #DUMMY
process.L1Fastjet.era = 'Spring10' #DUMMY
process.L1Fastjet.level = cms.string('L2Relative') #DUMMY
process.L1Fastjet.useCondDB = cms.untracked.bool(False)
#process.ak5PFL1L2L3Residual = process.ak5PFL2L3Residual.clone()
#process.ak5PFL1L2L3Residual.correctors.insert(0,'L1Fastjet')
#process.ak5PFJetsL1L2L3Residual=process.ak5PFJetsL1L2L3.clone(correctors = ['ak5PFL1L2L3Residual'])
process.offsetCorrection = cms.Sequence(process.ak5PFJets * process.kt6PFJets * process.ak5PFJetsL1)
addJetCollection(process,cms.InputTag('ak5PFJetsL1'),'AK5', 'PFL1corrected',
                 doJTA        = True,
                 doBTagging   = False,
                 jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute'])),
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


#add the trigger matching
process.muonTriggerMatchHLTMuons = cms.EDProducer(
  "PATTriggerMatcherDRLessByR"
, src     = cms.InputTag( 'cleanPatMuons' )
, matched = cms.InputTag( 'patTrigger' )
, andOr          = cms.bool( False )
, filterIdsEnum  = cms.vstring( 'TriggerMuon' )
, filterIds      = cms.vint32( 0 )
, filterLabels   = cms.vstring( '*' )
, pathNames      = cms.vstring( 'HLT_Mu9','HLT_Mu11','HLT_Mu13','HLT_Mu15','HLT_Mu17','HLT_Mu19','HLT_Mu21','HLT_Mu25',\
                                'HLT_Mu13_v1','HLT_Mu15_v1','HLT_Mu17_v1','HLT_Mu19_v1','HLT_Mu21_v1','HLT_Mu25_v1',\
                                'HLT_L1Mu7','HLT_L1Mu7_1','HLT_L2Mu7','HLT_L2Mu7_v1' )
, collectionTags = cms.vstring( '*' )
, maxDPtRel   = cms.double( 0.5 ) # no effect here
, maxDeltaR   = cms.double( 0.5 )
, maxDeltaEta = cms.double( 0.2 ) # no effect here
, resolveAmbiguities    = cms.bool( True )
, resolveByMatchQuality = cms.bool( True )
)

from PhysicsTools.PatAlgos.tools.coreTools import removeCleaning
removeCleaning( process )

from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process ) # This is optional and can be omitted.
#switchOnTriggerMatching( process, triggerMatchers = [ 'muonTriggerMatchHLTMuons' ] )
switchOnTriggerMatchEmbedding( process , triggerMatchers = [ 'muonTriggerMatchHLTMuons' ])
# Switch to selected PAT objects in the trigger matching
removeCleaningFromTriggerMatching( process )
process.muonTriggerMatchHLTMuons.src     = cms.InputTag( 'patMuons' )
process.selectedPatMuonsTriggerMatch.src = cms.InputTag( 'patMuons' )
process.selectedPatMuonsTriggerMatch.matches=cms.VInputTag('muonTriggerMatchHLTMuons')
process.patTriggerSequence=cms.Sequence(process.patTrigger+process.muonTriggerMatchHLTMuons+process.selectedPatMuonsTriggerMatch)

process.selectedMuons.src=cms.InputTag('selectedPatMuonsTriggerMatch')

#################
#################

#################
#################
process.patMuons.embedTpfmsMuon = cms.bool(False)
process.patMuons.embedHighLevelSelection = cms.bool(True)
process.patMuons.embedCaloMETMuonCorrs = cms.bool(False)
process.patMuons.embedPFCandidate = cms.bool(False)
process.patMuons.embedTcMETMuonCorrs = cms.bool(False)
process.patMuons.embedCombinedMuon = cms.bool(True)
process.patMuons.embedStandAloneMuon = cms.bool(False)
process.patMuons.embedPickyMuon = cms.bool(False)

#################

process.patTrigger.processName = "REDIGI38XPU"  ###WHAT DO WE NEED TO PUT HERE FOR THE 38X SAMPLES???????????????????????????
process.patTriggerEvent.processName = "REDIGI38XPU"

################# 
#Sequences and Paths
#################
#process.skim = cms.Sequence()
#from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import countPatMuons
#process.skimPatMuons = countPatMuons.clone()
#process.skimPatMuons.minNumber = cms.uint32(2)
#process.skimPatMuons.filter = cms.bool(True)
#process.skimPatMuons.src = 'selectedMuons'
process.skimPatMuons=cms.EDFilter("CandViewCountFilter",
  src = cms.InputTag("zmumurec"),
  minNumber = cms.uint32(1),
  filter = cms.bool(True)
)


process.recosequence = cms.Sequence(#process.recPFjetsSequence*
				    process.offsetCorrection*
                                    process.patDefaultSequence*
                                    process.zmumurecSequence*
                                    process.recjetsSequence)

#process.pattuples = cms.Sequence(process.recosequence*process.skimPatMuons)

process.gensequence = cms.Sequence(process.genjetsSequence*process.zmumugenSequence)                                    

process.pattuples = cms.Sequence(process.goodTracks + process.gensequence + process.recosequence)

#process.p = cms.Path(process.pattuples*process.skimPatMuons)
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
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep *_pat*METs*_*_*', 'keep *_patTriggerEvent_*_*', 'keep patTriggerPaths_patTrigger_*_*', 'keep *_goodTracks_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'Zmumu_MC.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
