import FWCore.ParameterSet.Config as cms

process = cms.Process("ZjetsNtupleMaker")

process.load('Firenze.JetUtilities.NewGenJetParticles_cff')
process.load('RecoJets.Configuration.RecoGenJets_cff')
process.sisCone5GenJets.src=cms.InputTag('newGenParticlesForJets')
process.genjets=cms.Sequence(process.newGenParticlesForJets*process.sisCone5GenJets)


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
"file:/afs/cern.ch/user/h/hegner/scratch0/PAT/testPatTuple_recHits_221.root"
)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.MessageLogger = cms.Service("MessageLogger")

process.verySimplePATAnalysis = cms.EDFilter("ZjetsNtupleMaker",
    electronTag = cms.InputTag("selectedLayer1Electrons"),
    tauTag      = cms.InputTag("selectedLayer1Taus"),
    muonTag     = cms.InputTag("selectedLayer1Muons"),
    jetTag      = cms.InputTag("selectedLayer1Jets"),
    photonTag   = cms.InputTag("selectedLayer1Photons"),
    metTag      = cms.InputTag("selectedLayer1METs"),
    jetGenTag   = cms.InputTag("sisCone5GenJets"),
    beamSpotTag = cms.InputTag("offlineBeamSpot")
)

process.TFileService = cms.Service("TFileService", fileName = cms.string('tt_test.root') )

process.p = cms.Path(process.genjets*process.verySimplePATAnalysis)
