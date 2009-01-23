import FWCore.ParameterSet.Config as cms

process = cms.Process("NtupleMaker")



process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(

"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_1.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_100.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_11.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_12.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_14.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_15.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_16.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_18.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_19.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_2.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_20.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_22.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_23.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_24.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_25.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_26.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_27.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_28.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_29.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_31.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_33.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_34.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_36.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_37.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_38.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_39.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_4.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_40.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_41.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_44.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_46.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_47.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_5.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_51.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_52.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_53.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_56.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_59.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_6.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenPythia_full_60.root" 
   
)
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.MessageLogger = cms.Service("MessageLogger")

process.verySimplePATAnalysis = cms.EDFilter("NtupleMaker",
    electronTag = cms.untracked.InputTag("selectedLayer1Electrons"),
    tauTag      = cms.untracked.InputTag("selectedLayer1Taus"),
    muonTag     = cms.untracked.InputTag("selectedLayer1Muons"),
    jetTag      = cms.untracked.InputTag("selectedLayer1Jets"),
    jetGenTag   = cms.untracked.InputTag("sisCone5GenJets"),                                        
    photonTag   = cms.untracked.InputTag("selectedLayer1Photons"),
    metTag      = cms.untracked.InputTag("selectedLayer1METs")
)

process.TFileService = cms.Service("TFileService", fileName = cms.string('histo_PYTHIA1.root') )

process.p = cms.Path(process.verySimplePATAnalysis)

