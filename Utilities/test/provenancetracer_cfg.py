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
<<<<<<< provenancetracer_cfg.py
        'file:/cms/data01/datasets/ZJets-madgraph/Fall08_IDEAL_V9_reco-v2/PATFirenze/result/ZJets-madgraph_PATLayer1_sisCone_full_1.root'
=======
        #'file:/cms/data01/datasets/ZJets-madgraph/Fall08_IDEAL_V9_reco-v2/PATFirenze/result/ZJets-madgraph_PATLayer1_sisCone_full_1.root'
        'file:/cms/data01/lenzip/CMSSW/PattuplesProduction/CMSSW_2_2_4/src/production/ZJets-madgraph_PATLayer1_sisCone_full.root'
>>>>>>> 1.7
    )
)

process.tracer = cms.EDAnalyzer('ProvenanceTracer',
<<<<<<< provenancetracer_cfg.py
  Products = cms.InputTag("selectedLayer1Muons"),
=======
  Products = cms.InputTag("selectedLayer1Jets"),
>>>>>>> 1.7
  ExcludeProcesses = cms.vstring("RECO", "HLT")
)


process.p = cms.Path(process.tracer)
