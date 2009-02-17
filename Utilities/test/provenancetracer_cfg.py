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
        'file:/cms/data01/lenzip/testdata/04561ADB-38E3-DD11-AA7D-00E08178C035.root'
    )
)

process.tracer = cms.EDAnalyzer('ProvenanceTracer',
  Products = cms.InputTag("selectedLayer1Muons"),
  #Products = cms.InputTag("selectedLayer1Jets"),
  #Products = cms.InputTag("sisCone5PFJets"),
  ParentIdentifiers = cms.vstring("src", "jetSource", "blocks")
  #ParentIdentifiers = cms.vstring("src", "genJetMatch")
)


process.p = cms.Path(process.tracer)
