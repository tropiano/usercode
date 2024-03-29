import FWCore.ParameterSet.Config as cms

zjetsanalysis = cms.EDAnalyzer('ZjetsPlots',
                               ZSource   = cms.InputTag('z'),
                               JetSource     = cms.InputTag('newJetSelector'),
                               DiffJetRate10 = cms.InputTag('diffCaloJetRate10', 'DiffJetRate10'),
                               DiffJetRate21 = cms.InputTag('diffCaloJetRate21', 'DiffJetRate21'),
                               DiffJetRate32 = cms.InputTag('diffCaloJetRate32', 'DiffJetRate32'),
                               Z             = cms.PSet(ptBins  = cms.int32(300),
                                                        pt1     = cms.double(0.),
                                                        pt2     = cms.double(300.),
                                                        mBins   = cms.int32(100),
                                                        m1      = cms.double(50.),
                                                        m2      = cms.double(150.),
                                                        etaBins = cms.int32(200),
                                                        eta1    = cms.double(-10.),
                                                        eta2    = cms.double(10.)
                                                        ),
                              Jet            = cms.PSet(ptBins  = cms.int32(300),
                                                        pt1     = cms.double(0.),
                                                        pt2     = cms.double(300.),
                                                        mBins = cms.int32(100),
                                                        m1      = cms.double(50.),
                                                        m2      = cms.double(150.),
                                                        etaBins = cms.int32(100),
                                                        eta1    = cms.double(-10.),
                                                        eta2    = cms.double(10.)
                                                        ),
                              diffJetRate    = cms.PSet(ptBins  = cms.int32(100),
                                                        pt1     = cms.double(-1.),
                                                        pt2     = cms.double(4.),
                                                        )
)
