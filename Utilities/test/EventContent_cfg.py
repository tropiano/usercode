import FWCore.ParameterSet.Config as cms

 
process = cms.Process("EventContent")

### standard MessageLoggerConfiguration
#process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring("detailedInfo"),
                                    detailedInfo = cms.untracked.PSet(threshold = cms.untracked.string("INFO"))
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
              'file:/home/lenzip/CMSSW/CMSSW_2_2_3/src/SHERPA112_Zll_0j1incl_10TeV_QCUT20_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT.root')
) 

process.options = cms.untracked.PSet(     wantSummary = cms.untracked.bool(True) )

process.p1 = cms.Path(process.printContent)

 
