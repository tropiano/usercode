import FWCore.ParameterSet.Config as cms

 
process = cms.Process("EventContent")

### standard MessageLoggerConfiguration
#process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring("eventContent"),
                                    eventContent = cms.untracked.PSet(threshold = cms.untracked.string("INFO"))
                                    )


### Standard Configurations
process.load("Configuration.StandardSequences.Services_cff")
 
#### print event content
process.load("FWCore.Modules.printContent_cfi")

### Conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.maxEvents = cms.untracked.PSet(     
  input = cms.untracked.int32(1)     
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
              'file:/cms/data01/datasets/ZJets-madgraph/Fall08_IDEAL_V9_reco-v2/GEN-SIM-RECO/02A17B0C-6BF3-DD11-B4E5-00151715BB94.root')
) 

process.options = cms.untracked.PSet(     wantSummary = cms.untracked.bool(True) )

process.p1 = cms.Path(process.printContent)

 
