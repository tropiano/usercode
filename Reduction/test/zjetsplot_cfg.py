import FWCore.ParameterSet.Config as cms

process = cms.Process('ZjetsPlots')
 
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
 
process.source = cms.Source('PoolSource',
                            fileNames = cms.untracked.vstring(
                            'file:/cms/data01/lenzip/testdata/04561ADB-38E3-DD11-AA7D-00E08178C035.root'
                            )
)
#keep the logging output to a nice level
process.MessageLogger = cms.Service('MessageLogger')


##event weight 
#include "MyAnalysis/ZplusJets/data/MyAlpgenEventWeight.cfi"

# root output file service
process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string('zjetsplots.root')
)

#Z candidate combiner
process.zmumu = cms.EDFilter('NamedCandViewShallowCloneCombiner',
                              decay = cms.string('selectedLayer1Muons@+ selectedLayer1Muons@-'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('Zmumu'),
                              roles = cms.vstring('mu+', 'mu-')
)

process.zee = cms.EDFilter('NamedCandViewShallowCloneCombiner',
                              decay = cms.string('selectedLayer1Electrons@+ selectedLayer1Electrons@-'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('Zee'),
                              roles = cms.vstring('e+', 'e-')
)

#plots

process.zjetsanalysis = cms.EDAnalyzer('ZjetsPlots',
                                       ZMuMuSource   = cms.InputTag('zmumu'), 
                                       ZEESource     = cms.InputTag('zee'), 
                                       JetSource     = cms.InputTag('selectedLayer1Jets'), 
                                       DiffJetRate10 = cms.InputTag('diffCaloJetRate10', 'DiffJetRate10'), 
                                       DiffJetRate21 = cms.InputTag('diffCaloJetRate21', 'DiffJetRate21'), 
                                       DiffJetRate32 = cms.InputTag('diffCaloJetRate32', 'DiffJetRate32'),
                                       muon          = cms.PSet(ptBins  = cms.int32(300),
                                                                pt1     = cms.double(0.),
                                                                pt2     = cms.double(300.),
                                                                mBins   = cms.int32(100),
                                                                m1      = cms.double(50.),
                                                                m2      = cms.double(150.),
                                                                etaBins = cms.int32(100),
                                                                eta1    = cms.double(-2.5),
                                                                eta2    = cms.double(2.5) 
                                                                ),
                                       muonJet       = cms.PSet(ptBins  = cms.int32(300),
                                                                pt1     = cms.double(0.),
                                                                pt2     = cms.double(300.),
                                                                mBins = cms.int32(100),
                                                                m1      = cms.double(50.),
                                                                m2      = cms.double(150.),
                                                                etaBins = cms.int32(100),
                                                                eta1    = cms.double(-5.),
                                                                eta2    = cms.double(5.)
                                                                ),                          
                                       electron      = cms.PSet(ptBins  = cms.int32(300),
                                                                pt1     = cms.double(0.),
                                                                pt2     = cms.double(300.),
                                                                mBins   = cms.int32(100),
                                                                m1      = cms.double(50.),
                                                                m2      = cms.double(150.),
                                                                etaBins = cms.int32(100),
                                                                eta1    = cms.double(-2.5),
                                                                eta2    = cms.double(2.5) 
                                                                ),
                                       electronJet   = cms.PSet(ptBins  = cms.int32(300),
                                                                pt1     = cms.double(0.),
                                                                pt2     = cms.double(300.),
                                                                mBins   = cms.int32(100),
                                                                m1      = cms.double(50.),
                                                                m2      = cms.double(150.),
                                                                etaBins = cms.int32(100),
                                                                eta1    = cms.double(-5.),
                                                                eta2    = cms.double(5.)
                                                                ),
                                diffJetRateMuon      = cms.PSet(ptBins  = cms.int32(100),
                                                                pt1     = cms.double(-1.),
                                                                pt2     = cms.double(4.),
                                                                ),
                                diffJetRateElectron  = cms.PSet(ptBins  = cms.int32(100),
                                                                pt1     = cms.double(-1.),
                                                                pt2     = cms.double(4.),
                                                                ) 
)

process.p = cms.Path(process.zmumu*process.zee*process.zjetsanalysis) 
