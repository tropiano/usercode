import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring("detailedInfo"),
                                    detailedInfo = cms.untracked.PSet(threshold = cms.untracked.string("INFO"))
                                    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        #'file:/cms/data01/datasets/ZJets-madgraph/Summer08_IDEAL_V9_PAT_v3/USER/1402F759-5EE7-DD11-ADE5-0015170AB26C.root'
        'file:/cms/data01/lenzip/CMSSW/PattuplesProduction/CMSSW_2_2_4/src/production/TTjets-madgraph_PATLayer1_sisCone_full.root'
    )
)

process.tracer = cms.EDAnalyzer('ProvenanceTracer',
  #Products = cms.InputTag("selectedLayer1Muons"),
  Products = cms.InputTag("selectedLayer1Muons"),
  #Products = cms.InputTag("sisCone5PFJets"),
  #ParentIdentifiers = cms.vstring("src", "jetSource", "blocks")
  ExcludeProcesses = cms.vstring("RECO", "HLT")
)


process.p = cms.Path(process.tracer)
