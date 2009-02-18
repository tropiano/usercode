import FWCore.ParameterSet.Config as cms

process = cms.Process("ZjetsNtupleMaker")

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_1.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_2.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_3.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_4.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_5.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_6.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_7.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_8.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_9.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_10.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_11.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_12.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_13.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_14.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_15.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_16.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_17.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_18.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_19.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_20.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_21.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_22.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_23.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_24.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_25.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_26.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_27.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_28.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_29.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_30.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_31.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_32.root"
)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10))

process.MessageLogger = cms.Service("MessageLogger")

process.verySimplePATAnalysis = cms.EDFilter("ZjetsNtupleMaker",
    electronTag = cms.InputTag("selectedLayer1Electrons"),
    tauTag      = cms.InputTag("selectedLayer1Taus"),
    muonTag     = cms.InputTag("selectedLayer1Muons"),
    jetTag      = cms.InputTag("selectedLayer1Jets"),
    jetGenTag   = cms.InputTag("sisCone5GenJets"),                                        
    photonTag   = cms.InputTag("selectedLayer1Photons"),
    metTag      = cms.InputTag("selectedLayer1METs"),
    beamSpotTag = cms.InputTag("hltOfflineBeamSpot")
)

process.TFileService = cms.Service("TFileService", fileName = cms.string('histo_1M_MADGRAPH_ISO1.root') )

process.p = cms.Path(process.verySimplePATAnalysis)
