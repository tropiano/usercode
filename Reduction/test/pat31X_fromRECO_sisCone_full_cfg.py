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

process.source.fileNames = [ 'file:/raid/lenzip/CMSSW/CMSSW_3_1_2/src/PhysicsTools/PatAlgos/test/zmumu-test-reco.root' ]

process.eleIsoDepositEcalFromHits.ExtractorPSet.barrelEcalHits = cms.InputTag("reducedEcalRecHitsEB", "", "RECO")
process.eleIsoDepositEcalFromHits.ExtractorPSet.endcapEcalHits = cms.InputTag("reducedEcalRecHitsEE", "", "RECO")
#
#
process.eidRobustHighEnergy.reducedBarrelRecHitCollection = cms.InputTag("reducedEcalRecHitsEB", "", "RECO")
process.eidRobustHighEnergy.reducedEndcapRecHitCollection = cms.InputTag("reducedEcalRecHitsEE", "", "RECO")

# uncomment the following lines to switch the standard jet collection
# in PAT (iterativeCone5) to sisCone5
switchJetCollection(process, 
                    cms.InputTag('sisCone5CaloJets'),   
                    doJTA            = True,            
                    doBTagging       = True,            
                    jetCorrLabel     = ('SC5','Calo'),  
                    doType1MET       = True,            
                    genJetCollection = cms.InputTag("sisCone5GenJets")
                    ) 

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


process.p = cms.Path(
                process.patDefaultSequence+
                process.patTriggerSequence+
                process.zmumugenSequence+
                process.zeegenSequence+
                process.genjetsSequence+
                process.zmumurecSequence+
                process.zeerecSequence+
                process.recjetsSequence
            )
from Firenze.Reduction.genZmumuPatAddOns_cff import zmumugenEventContent
from Firenze.Reduction.genZeePatAddOns_cff import zeegenEventContent
from Firenze.Reduction.genJetPatAddOns_cff import jetgenEventContent
from Firenze.Reduction.recZmumuPatAddOns_cff import zmumurecEventContent
from Firenze.Reduction.recZeePatAddOns_cff import zeerecEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.out.outputCommands=cms.untracked.vstring('drop *')
process.out.outputCommands.extend(zmumugenEventContent)
process.out.outputCommands.extend(zeegenEventContent)
process.out.outputCommands.extend(jetgenEventContent)
process.out.outputCommands.extend(zmumurecEventContent)
process.out.outputCommands.extend(zeerecEventContent)
process.out.outputCommands.extend(jetrecEventContent)
process.out.outputCommands.extend(patTriggerEventContent)
process.out.dropMetaData = cms.untracked.string('DROPPED')
#process.out.dropMetaData = cms.untracked.string('ALL')

print 'Current Event content is:'
print process.out.outputCommands

process.out.fileName = 'test_pat_withtrigger.root'

# In addition you usually want to change the following parameters:
#
#   process.GlobalTag.globaltag =  ...     ## (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
#   process.source.fileNames = [ ... ]     ## (e.g. 'file:AOD.root')
process.maxEvents.input = -1               ## (e.g. -1 to run on all events)
#   process.out.outputCommands = [ ... ]   ## (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
#   process.out.fileName = ...             ## (e.g. 'myTuple.root')
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

