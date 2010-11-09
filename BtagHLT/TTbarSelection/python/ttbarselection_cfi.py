import FWCore.ParameterSet.Config as cms

ttbarselection = cms.EDAnalyzer('TTbarSelection',
                                vertexSrc = cms.untracked.InputTag("offlinePrimaryVerticesWithBS"),
                                electronSrc= cms.untracked.InputTag("selectedElectrons"),
                                muonSrc= cms.untracked.InputTag("selectedMuons"),
                                triggerSrc= cms.untracked.InputTag("TriggerEvent"),
                                triggerName= cms.untracked.string("HLT_BTagIP_Jet50U"),
                                jetSrc= cms.untracked.InputTag("selectedJets"),
                                metSrc= cms.untracked.InputTag("patMETs"),
                                ElectronVeto_PtCut   = cms.double(0),
                                ElectronVeto_EtaCut  = cms.double(99999),
                                RelIso            = cms.double(0.2),
                                MuonVeto_PtCut    = cms.double(0),
                                MuonVeto_EtaCut   = cms.double(99999),
                                Jets_PtCut        = cms.double(30),
                                Jets_EtaCut       = cms.double(2.4),
                                Jets_EmFracCut    = cms.double(0.01),
                                MET_Cut           = cms.double(30),
                                BtagDiscrCut1     = cms.double(4.),
                                BtagDiscrCut2     = cms.double(4.),
                                )


