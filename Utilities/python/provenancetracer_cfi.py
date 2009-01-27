import FWCore.ParameterSet.Config as cms

provenancetracer = cms.EDAnalyzer('ProvenanceTracer',
    Products = cms.InputTag("selectedLayer1Jets"),
    ParentIdentifiers = cms.vstring("src")
)
