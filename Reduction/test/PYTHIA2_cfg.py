import FWCore.ParameterSet.Config as cms

process = cms.Process("ZjetsNtupleMaker")



process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(

  "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_61.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_62.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_63.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_65.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_66.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_67.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_68.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_72.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_73.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_74.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_77.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_80.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_81.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_83.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_84.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_85.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_87.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_88.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_89.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_9.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_90.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_92.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_95.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_96.root",
 "rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_98.root"


    
)
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.MessageLogger = cms.Service("MessageLogger")

process.verySimplePATAnalysis = cms.EDFilter("ZjetsNtupleMaker",
    electronTag = cms.untracked.InputTag("selectedLayer1Electrons"),
    tauTag      = cms.untracked.InputTag("selectedLayer1Taus"),
    muonTag     = cms.untracked.InputTag("selectedLayer1Muons"),
    jetTag      = cms.untracked.InputTag("selectedLayer1Jets"),
    jetGenTag   = cms.untracked.InputTag("sisCone5GenJets"),                                        
    photonTag   = cms.untracked.InputTag("selectedLayer1Photons"),
    metTag      = cms.untracked.InputTag("selectedLayer1METs")
)

process.TFileService = cms.Service("TFileService", fileName = cms.string('histo_PYTHIA2.root') )

process.p = cms.Path(process.verySimplePATAnalysis)

