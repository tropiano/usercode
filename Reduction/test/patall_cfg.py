import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
    PATSummaryTables = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# source
process.source = cms.Source("PoolSource", 
     fileNames = cms.untracked.vstring(
     'file:/cms/data01/datasets/ZJets-madgraph/Fall08_IDEAL_V9_reco-v2/GEN-SIM-RECO/02A17B0C-6BF3-DD11-B4E5-00151715BB94.root'
     )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )


# PAT Layer 0+1
process.load("PhysicsTools.PatAlgos.patLayer0_cff")
process.load("PhysicsTools.PatAlgos.patLayer1_cff")

## Load additional RECO config
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('IDEAL_V9::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

from PhysicsTools.PatAlgos.tools.jetTools import *
##Load the right jet corrections
#process.load('JetMETCorrections.Configuration.L2L3Corrections_Summer08_cff')
## ==== Example with CaloJets
switchJetCollection(process, 
        'sisCone5CaloJets',    # Jet collection; must be already in the event when patLayer0 sequence is executed
        doJTA=True,            # Run Jet-Track association & JetCharge
        doBTagging=True,       # Run b-tagging
        jetCorrLabel=('SC5','Calo'), # example jet correction name; set to None for no JEC
        doType1MET=True,       # recompute Type1 MET using these jets
        genJetCollection=cms.InputTag("sisCone5GenJets")) 

#manually change the jet collection to be used in the genJetMatching        
process.jetGenJetMatch.matched=cms.InputTag('sisCone5GenJets')
process.patAODJetTracksAssociator.tracks = cms.InputTag('sisCone5JetTracksAssociatorAtVertex')


#process.content = cms.EDAnalyzer("EventContentAnalyzer")
process.p = cms.Path(
                ## process.iterativeCone5BasicJets +  ## Turn on this to run tests on BasicJets
                process.patLayer0  
                #process.patLayer0_patTuple_withoutPFTau  
                #+ process.content    # uncomment to get a dump 
                + process.patLayer1
                #+ process.layer1Muons*process.layer1Electrons*process.countLayer1Leptons*process.layer1Photons*process.layer1Jets*process.layer1METs*process.layer1Hemispheres
)


# Output module configuration
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('DATASETNAME_PATLayer1_sisCone_full.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = cms.untracked.vstring('drop *')
)
process.outpath = cms.EndPath(process.out)
# save PAT Layer 1 output
process.load("PhysicsTools.PatAlgos.patLayer1_EventContent_cff")
process.out.outputCommands.extend(process.patLayer1EventContent.outputCommands)
sisConeEventContent = cms.PSet(
  outputCommands = cms.untracked.vstring('keep *_sisCone5GenJets_*_*')
)
process.out.outputCommands.extend(sisConeEventContent.outputCommands)
## Necessary fixes to run 2.2.X on 2.1.X data
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import run22XonSummer08AODSIM
run22XonSummer08AODSIM(process)

