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

##select jets according to these criteria
import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
process.newJetSelector = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedLayer1Jets.clone()
process.newJetSelector.src = cms.InputTag("selectedLayer1Jets")
process.newJetSelector.cut = cms.string('pt > 30. & abs(eta) < 5. & nConstituents > 0')

##select muons according to these criteria
import PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi
process.newMuonSelector = PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi.selectedLayer1Muons.clone()
process.newMuonSelector.src = cms.InputTag("selectedLayer1Muons")
process.newMuonSelector.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalMuonPromptTight') & globalTrack().found()>11 & abs(globalTrack().d0())<0.2 & (trackIso()+caloIso()+ecalIso())/globalTrack().pt()<0.3")


#Z candidate combiner
process.zmumu = cms.EDFilter('NamedCandViewShallowCloneCombiner',
                              #decay = cms.string('selectedLayer1Muons@+ selectedLayer1Muons@-'),
                              decay = cms.string('newMuonSelector@+ newMuonSelector@-'),
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
                                       JetSource     = cms.InputTag('newJetSelector'), 
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

process.p = cms.Path(process.newJetSelector*process.newMuonSelector*process.zmumu*process.zee*process.zjetsanalysis) 
