from PhysicsTools.PatAlgos.patTemplate_cfg import *

# load the standard PAT config 
process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.load("Firenze.Reduction.genZmumuPatAddOns_cff")
process.load("Firenze.Reduction.genZeePatAddOns_cff")
process.load("Firenze.Reduction.genJetPatAddOns_cff")
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.load("Firenze.Reduction.recZeePatAddOns_cff")
process.load("Firenze.Reduction.recJetPatAddOns_cff")

process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")

from PhysicsTools.PatAlgos.tools.jetTools import *

process.source.fileNames = [ 'file:/raid/lenzip/CMSSW/CMSSW_3_1_4/src/Firenze/Reduction/test/EWK_WMuNu_SubSkim_1.root' ]

process.eleIsoDepositEcalFromHits.ExtractorPSet.barrelEcalHits = cms.InputTag("reducedEcalRecHitsEB", "", "RECO")
process.eleIsoDepositEcalFromHits.ExtractorPSet.endcapEcalHits = cms.InputTag("reducedEcalRecHitsEE", "", "RECO")
#
#
process.eidRobustHighEnergy.reducedBarrelRecHitCollection = cms.InputTag("reducedEcalRecHitsEB", "", "RECO")
process.eidRobustHighEnergy.reducedEndcapRecHitCollection = cms.InputTag("reducedEcalRecHitsEE", "", "RECO")

# uncomment the following lines to switch the standard jet collection
# in PAT (iterativeCone5) to sisCone5
switchJetCollection(process, 
                    cms.InputTag('antikt5CaloJets'),   
                    doJTA            = True,            
                    doBTagging       = True,            
                    jetCorrLabel     = ('AK5','Calo'),  
                    doType1MET       = True,            
                    genJetCollection = cms.InputTag("antikt5GenJets")
                    ) 

process.allLayer1Jets.addJetID = cms.bool(False)
process.allLayer1Jets.addTagInfos = cms.bool(False)
process.allLayer1Jets.addBTagInfo = cms.bool(False)
process.allLayer1Jets.addPartonJetMatch = cms.bool(False)
process.allLayer1Jets.embedGenPartonMatch = cms.bool(False)
process.allLayer1Jets.addDiscriminators = cms.bool(False)
process.allLayer1Jets.addAssociatedTracks = cms.bool(False)
process.allLayer1Jets.addJetCharge = cms.bool(False)
process.cleanLayer1Jets.checkOverlaps.electrons = cms.PSet()
process.cleanLayer1Jets.checkOverlaps.tkIsoElectrons = cms.PSet()
process.cleanLayer1Jets.checkOverlaps.photons = cms.PSet()
process.cleanLayer1Jets.checkOverlaps.taus = cms.PSet()


process.allLayer1Muons.embedTrack = cms.bool(False)
process.allLayer1Muons.embedStandAloneMuon = cms.bool(False)
process.allLayer1Muons.embedCombinedMuon = cms.bool(False)
process.allLayer1Muons.embedPickyMuon = cms.bool(False)
process.allLayer1Muons.embedTpfmsMuon = cms.bool(False)
#process.allLayer1Muons.isolation.tracker.src=''
#process.allLayer1Muons.isolation.ecal.src=''
#process.allLayer1Muons.isolation.hcal.src=''
#process.allLayer1Muons.isoDeposits.tracker=''
#process.allLayer1Muons.isoDeposits.ecal=''
#process.allLayer1Muons.isoDeposits.hcal=''
process.allLayer1Muons.isolation = cms.PSet()
process.allLayer1Muons.isoDeposits = cms.PSet()

process.patTrigger.processName='HLT8E29'
process.patTriggerEvent.processName='HLT8E29'


process.myLayer1Jets = cms.Sequence(process.patJetCorrections*process.allLayer1Jets)

process.patTriggerEvent.patTriggerMatches = ['muonTriggerMatchL1Muons', 'muonTriggerMatchAll', 'muonTriggerMatchNone']
process.myTriggerSequence = cms.Sequence(process.patTrigger*process.patTriggerMuonMatcher*process.patTriggerEvent)

process.p = cms.Path(
                process.makeAllLayer1Muons+
                #process.makeAllLayer1Jets+
                process.myLayer1Jets+
                process.selectedLayer1Muons+
                process.selectedLayer1Jets+
                process.cleanLayer1Muons+
                process.cleanLayer1Jets+
                process.countLayer1Muons+
                process.countLayer1Jets+
                #process.patTriggerSequence+
                process.myTriggerSequence+
                process.zmumurecSequence+
                process.recjetsSequence
            )
from Firenze.Reduction.recZmumuPatAddOns_cff import zmumurecEventContent
from Firenze.Reduction.recZeePatAddOns_cff import zeerecEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, 'All')


process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zmumurecEventContent)
process.out.outputCommands.extend(jetrecEventContent)
process.out.outputCommands.extend(patTriggerEventContent)
process.out.dropMetaData = cms.untracked.string('DROPPED')
#process.out.dropMetaData = cms.untracked.string('ALL')

print 'Current Event content is:'
print process.out.outputCommands

process.out.fileName = 'pat_skim.root'

# In addition you usually want to change the following parameters:
#
#   process.GlobalTag.globaltag =  ...     ## (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
#   process.source.fileNames = [ ... ]     ## (e.g. 'file:AOD.root')
process.maxEvents.input = -1               ## (e.g. -1 to run on all events)
#   process.out.outputCommands = [ ... ]   ## (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
#   process.out.fileName = ...             ## (e.g. 'myTuple.root')
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

