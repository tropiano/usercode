import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("TopSimple")

options = VarParsing.VarParsing ('analysis')
options.outputFile = 'test.root'
options.inputFiles
options.maxEvents = -1 # -1 means all events

options.parseArguments()

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents))

#inputfiles = cms.untracked.vstring("file:/afs/cern.ch/user/t/tropiano/scratch0/CMSSW_3_6_1_patch3/src/TopHadronic/TopAnalyzer/list.txt")
process.source = cms.Source("PoolSource",
                            fileNames      = cms.untracked.vstring(options.inputFiles),
                        #    debugVerbosity = cms.untracked.uint32(200),
                        #    debugFlag      = cms.untracked.bool(True),
                            )

process.top = cms.EDAnalyzer('TopAnalyzer',
                             vertexSrc = cms.untracked.InputTag("offlinePrimaryVerticesWithBS"),
                             electronSrc= cms.untracked.InputTag("selectedElectrons"),
                             muonSrc= cms.untracked.InputTag("selectedMuons"),
                             triggerSrc= cms.untracked.InputTag("patTriggerEvent"),
                             triggerName= cms.untracked.vstring('HLT_QuadJet25U_v1', 'HLT_QuadJet25U_v2' , 'HLT_QuadJet25U_v3'),
                             jetSrc= cms.untracked.InputTag("selectedJets"),
                             metSrc= cms.untracked.InputTag("patMETs"),
                             ElectronVeto_PtCut   = cms.double(20),
                             ElectronVeto_EtaCut  = cms.double(2.5),
                             RelIso            = cms.double(0.2),
                             MuonVeto_PtCut    = cms.double(20),
                             MuonVeto_EtaCut   = cms.double(2.5),
                             Jets_PtCut        = cms.double(40),
                             Jets_EtaCut       = cms.double(2.4),
                             #Jets_DeltaRCut    = cms.double(0.3),
                             Jets_EmFracCut    = cms.double(0.01),
                             BtagDiscrCut1        = cms.double(1.19),   #tchp loose wp
                             BtagDiscrCut2        = cms.double(1.93),   #tchp tight wp
                             BtagDiscrCut3        = cms.double(1.7),    #loose wp
                             BtagDiscrCut4        = cms.double(3.3),    #tight wp
                             BtagAlgo1            = cms.string("trackCountingHighPurBJetTags"),
                             BtagAlgo2            = cms.string("trackCountingHighEffBJetTags"),
                             MET_Cut           = cms.double(30),
                             #Mz_inf            = cms.double(76),
                             #Mz_sup            = cms.double(106) 
                             )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string (options.outputFile)
                                   )

process.p = cms.Path(process.top)
