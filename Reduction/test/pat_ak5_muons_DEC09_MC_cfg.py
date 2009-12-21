from PhysicsTools.PatAlgos.patTemplate_cfg import *

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

#/MinBiasDW/Summer09-DESIGN_3X_V8A_900GeV_AODSIM-v1/AODSIM
process.source.fileNames = [
'file:FE6A620D-9BE2-DE11-9A3F-002618943960.root']
process.maxEvents.input = -1               ## (e.g. -1 to run on all events)
#process.source.firstRun = cms.untracked.uint32(122314)
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('122314:24-122314:37') 

#################
#skim bit 40-41
#################
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('40 OR 41')


#################
#my modules
#################
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.selectedMuons.cut = cms.string("pt > 0. & abs(eta) < 3.")
process.zmumurec.cut = cms.string('0 < mass < 130') 
process.load("Firenze.Reduction.recZeePatAddOns_cff")
process.load("Firenze.Reduction.recJetPatAddOns_cff")
process.selectedJets.cut = cms.string('pt > 5. & abs(eta) < 10. & nConstituents > 0')
process.load("Firenze.Reduction.genZmumuPatAddOns_cff")
process.load("Firenze.Reduction.genJetPatAddOns_cff")
process.selectedGenJets.cut = cms.string('pt > 5. & abs(eta) < 10. & nConstituents > 0')
process.chargedGenParticles = cms.EDFilter('CandViewSelector',
   src = cms.InputTag('genParticles'),
   cut = cms.string('status == 1 && charge() != 0')
)
#################


#################
#customizartion
#################
#process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.coreTools import *
#removeMCMatching(process, 'All')
restrictInputToAOD(process, 'All')
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger(process)
process.eleIsoDepositEcalFromHits.ExtractorPSet.barrelEcalHits = cms.InputTag("reducedEcalRecHitsEB", "", "RECO")
process.eleIsoDepositEcalFromHits.ExtractorPSet.endcapEcalHits = cms.InputTag("reducedEcalRecHitsEE", "", "RECO")
process.eidRobustHighEnergy.reducedBarrelRecHitCollection = cms.InputTag("reducedEcalRecHitsEB", "", "RECO")
process.eidRobustHighEnergy.reducedEndcapRecHitCollection = cms.InputTag("reducedEcalRecHitsEE", "", "RECO")
process.allLayer1Jets.addJetID = cms.bool(False)
process.allLayer1Jets.addTagInfos = cms.bool(False)
process.allLayer1Jets.addBTagInfo = cms.bool(False)
process.allLayer1Jets.addPartonJetMatch = cms.bool(False)
process.allLayer1Jets.embedGenPartonMatch = cms.bool(False)
process.allLayer1Jets.addDiscriminators = cms.bool(False)
process.allLayer1Jets.addAssociatedTracks = cms.bool(False)
process.allLayer1Muons.embedTrack = cms.bool(False)
process.allLayer1Muons.embedStandAloneMuon = cms.bool(False)
process.allLayer1Muons.embedCombinedMuon = cms.bool(False)
process.allLayer1Muons.embedPickyMuon = cms.bool(False)
process.allLayer1Muons.embedTpfmsMuon = cms.bool(False)
process.load("JetMETCorrections.Configuration.L2L3Corrections_900GeV_cff") ##JEC for 900 GeV collisions
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
process.jetCorrFactors.corrLevels.L5Flavor='none'
process.jetCorrFactors.corrSample='900GeV'
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

process.pattuples = cms.Sequence(
                #process.selectedTracks+
                process.chargedGenParticles+
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
process.out.outputCommands.extend(['keep *_chargedGenParticles_*_*', 'keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*', 'keep *_layer1METs_*_*'])
#process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep recoTracks_generalTracks_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = 'mc.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
