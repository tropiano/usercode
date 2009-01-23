import FWCore.ParameterSet.Config as cms

process = cms.Process("ZjetsNtupleMaker")



process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(

"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_33.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_34.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_35.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_36.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_37.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_38.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_39.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_40.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_41.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_42.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_43.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_44.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_45.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_46.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_47.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_48.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_49.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_50.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_51.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_52.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_53.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_54.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_55.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_56.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_57.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_58.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_59.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_60.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_61.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_62.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_63.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_64.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_65.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_66.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_67.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_68.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_69.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_70.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_71.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_72.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_73.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_74.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_75.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_76.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_77.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_78.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_79.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_80.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_81.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_82.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_83.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_84.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_85.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_86.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_87.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_88.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_89.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_90.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_91.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_92.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_93.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_94.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_98.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_99.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_100.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_101.root",
"rfio:/castor/cern.ch/user/e/egallo/Zmumu/SisCone100/PATLayer1_Output.fromAOD_sisConegenMadgraph_full_102.root"

)
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000))

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

process.TFileService = cms.Service("TFileService", fileName = cms.string('histo_5000_MADGRAPH.root') )

process.p = cms.Path(process.verySimplePATAnalysis)


                              
