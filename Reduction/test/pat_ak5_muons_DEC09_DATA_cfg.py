from PhysicsTools.PatAlgos.patTemplate_cfg import *

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source.fileNames = [
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/FA1A8BAA-19E3-DE11-A84D-002618943951.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/F8D22798-CFE2-DE11-9135-002618943877.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/F08D3B91-D3E2-DE11-BFA7-00261894387A.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/DAE52ABF-19E3-DE11-8E7F-002618FDA28E.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/DABD5D6D-D4E2-DE11-8FFD-00261894387A.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/CA70DE6C-D4E2-DE11-A857-0026189438C4.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/C476D121-DCE2-DE11-9DBB-002618943832.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/C0BE8D91-D5E2-DE11-BD6C-002618943829.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/C070AD79-D5E2-DE11-8A74-00261894387A.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/BEE0F677-15E3-DE11-9A0A-00248C0BE014.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/B6E79B91-D5E2-DE11-96B4-002618943984.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/B67DBF91-D5E2-DE11-A457-00261894395F.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/B4DECB38-D7E2-DE11-AFD2-002618943884.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/AC65FE60-D5E2-DE11-857A-002618943954.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/A2F66B2A-DBE2-DE11-A797-002618943901.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/A0A2F242-DDE2-DE11-B1EA-00248C0BE018.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/A0215BAB-19E3-DE11-961A-0026189438ED.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/9CFCF26C-D5E2-DE11-A1BC-0026189438B3.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/9C2F4A3B-D7E2-DE11-8196-002354EF3BE0.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/9AC66043-DDE2-DE11-9E2D-0026189438DC.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/96A63ABF-19E3-DE11-A016-002618FDA28E.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/88079B43-DDE2-DE11-B5D6-00261894390C.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/8669F4A2-D8E2-DE11-BE6C-002618943884.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/72AF4044-D2E2-DE11-847B-0026189438C4.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/723DD5B1-19E3-DE11-A7EC-002618943904.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/6683E742-DDE2-DE11-AA1D-002618943906.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/6481D742-DDE2-DE11-BBDD-002354EF3BE0.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/5EAC831D-DCE2-DE11-9247-002354EF3BE0.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/5EA38391-D5E2-DE11-B4A0-002618943910.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/50EC766E-D4E2-DE11-B9C4-0026189438B3.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/50082EBF-19E3-DE11-8FEC-002618FDA28E.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/489C2F91-D3E2-DE11-9260-0026189437EC.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/44835322-DCE2-DE11-A121-0026189438DC.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/26264A85-D5E2-DE11-8047-002618943906.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/22852ABF-19E3-DE11-B549-002618FDA28E.root",
"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/06589FB1-19E3-DE11-A527-0026189438CE.root"]
process.maxEvents.input = -1               ## (e.g. -1 to run on all events)
#process.source.firstRun = cms.untracked.uint32(122314)
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('123592:2-123592:14', '') 

#################
#skim bit 40-41
#################
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND 0 AND NOT (36 OR 37 OR 38 OR 39)')

#process.load('TKDPG/BXSelect/BXSelect_cfi')
#process.bxSelect.SelectBXs = [ 51, 2724 ]

#################
#my modules
#################
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.selectedMuons.cut = cms.string('pt > 0. & abs(eta) < 3.')
process.zmumurec.cut = cms.string('0 < mass < 130') 
process.load("Firenze.Reduction.recZeePatAddOns_cff")
process.load("Firenze.Reduction.recJetPatAddOns_cff")
process.selectedJets.cut = cms.string('pt > 3. & abs(eta) < 10. & nConstituents > 0')
#################


#################
#customizartion
#################
#process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, 'All')
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
process.skim = cms.Sequence(process.hltLevel1GTSeed)

process.pattuples = cms.Sequence(
                process.selectedTracks+
                process.patDefaultSequence+
                process.zmumurecSequence+
                process.recjetsSequence
            )

process.p = cms.Path(process.skim*process.pattuples)
#process.p = cms.Path(process.pattuples)
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
process.out.fileName = 'muon.root'
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
