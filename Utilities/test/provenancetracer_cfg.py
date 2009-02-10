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
        #'file:/home/lenzip/CMSSW/CMSSW_2_2_3/src/SHERPA112_Zll_0j1incl_10TeV_QCUT20_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT.root'
        'file:/cms/data01/lenzip/CMSSW/ZjetsAnalysis/CMSSW_2_2_4/src/Firenze/Utilities/test/PATLayer1_Output.fromAOD_sisConegenMadgraphBS_full_82.root'
    )
)

process.tracer = cms.EDAnalyzer('ProvenanceTracer',
  Products = cms.InputTag("sisCone5GenJets"),
  #Products = cms.InputTag("selectedLayer1Jets"),
  #Products = cms.InputTag("sisCone5PFJets"),
  ParentIdentifiers = cms.vstring("src", "jetSource", "blocks")
  #ParentIdentifiers = cms.vstring("src", "genJetMatch")
)


process.p = cms.Path(process.tracer)
