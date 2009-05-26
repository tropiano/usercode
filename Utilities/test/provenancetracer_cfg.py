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
        'file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_1.root'
        #'file:/cms/data01/datasets/ZJets-madgraph/Fall08_IDEAL_V9_reco-v2/PATFirenze/result/ZJets-madgraph_PATLayer1_sisCone_full_1.root'
        #'file:/cms/data01/datasets/ZJets-madgraph/Fall08_IDEAL_V9_reco-v2/PATFirenze/result/ZJets-madgraph_PATLayer1_sisCone_full_1.root'
        #'file:/cms/data01/lenzip/CMSSW/PattuplesProduction/CMSSW_2_2_4/src/production/ZJets-madgraph_PATLayer1_sisCone_full.root'
    )
)

process.tracer = cms.EDAnalyzer('ProvenanceTracer',
  Products = cms.InputTag("selectedLayer1Muons"),
  ExcludeProcesses = cms.vstring()
)


process.p = cms.Path(process.tracer)
