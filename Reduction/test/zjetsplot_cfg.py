import FWCore.ParameterSet.Config as cms

process = cms.Process('ZjetsPlots')
 
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
 
process.source = cms.Source('PoolSource',
                            fileNames = cms.untracked.vstring(
                            'file:/cms/data01/lenzip/CMSSW/ZjetsAnalysis/CMSSW_2_2_4/src/Firenze/JetUtilities/test/test.root'
                            ),
                            secondaryFileNames = cms.untracked.vstring(
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

####################
######GEN###########
####################

##reconstruct gen jets
import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
process.load('Firenze.JetUtilities.NewGenJetParticles_cff')
process.load('RecoJets.Configuration.RecoGenJets_cff')
process.sisCone5GenJets.src=cms.InputTag('newGenParticlesForJets')
process.genJetSelector = cms.EDFilter('GenJetSelector', 
                                      src = cms.InputTag('sisCone5GenJets'),
                                      cut = cms.string('pt > 30. & abs(eta) < 5. & nConstituents > 0')
                                      )
process.genjets = cms.Sequence(process.newGenParticlesForJets*process.sisCone5GenJets*process.genJetSelector)

##select gen muons from Z
import Firenze.Reduction.genZleptonSelector_cff
process.genMuonsFromZ = Firenze.Reduction.genZleptonSelector_cff.genZleptonSelector.clone()
process.genMuonsFromZ.excludeFromResonancePids = cms.vuint32(13)
process.genMuonsFromZ.checkForStatus3Mother.motherIds=cms.vuint32(13)
process.genMuonSelector = cms.EDFilter('MyGenParticleRefSelector',
                                       src = cms.InputTag('genMuonsFromZ'),
                                       cut = cms.string('pt > 20 & abs(eta) < 2.4'))

##select gen electrons from Z
import Firenze.Reduction.genZleptonSelector_cff
process.genElectronFromZ = Firenze.Reduction.genZleptonSelector_cff.genZleptonSelector.clone()
process.genElectronFromZ.excludeFromResonancePids = cms.vuint32(11)
process.genElectronFromZ.checkForStatus3Mother.motherIds=cms.vuint32(11)
process.genElectronSelector = cms.EDFilter('MyGenParticleRefSelector',
                                           src = cms.InputTag('genElectronFromZ'),
                                           cut = cms.string('pt > 20 & abs(eta) < 2.4'))

##select gen Z mumu
process.zmumugen = cms.EDFilter('CandViewShallowCloneCombiner',
                                decay = cms.string('genMuonSelector@+ genMuonSelector@-'),
                                cut   = cms.string('50 < mass < 150'),
                                name  = cms.string('ZmumuGen'),
                                roles = cms.vstring('mu1', 'mu2')
)

##select gen Z ee
process.zeegen = cms.EDFilter('CandViewShallowCloneCombiner',
                                decay = cms.string('genElectronSelector@+ genElectronSelector@-'),
                                cut   = cms.string('50 < mass < 150'),
                                name  = cms.string('ZeeGen'),
                                roles = cms.vstring('e1', 'e2')
)

process.genZmumu = cms.Sequence(process.genMuonsFromZ*process.genMuonSelector*process.zmumugen)
process.genZee   = cms.Sequence(process.genElectronFromZ*process.genElectronSelector*process.zeegen)
process.genZ     = cms.Sequence(process.genZmumu+process.genZee)

## build plots about gen Z
import Firenze.Reduction.ZJetsPlots_cff
process.zjetsGEN = Firenze.Reduction.ZJetsPlots_cff.zjetsanalysis.clone()
process.zjetsGEN.ZMuMuSource   = cms.InputTag('zmumugen')
process.zjetsGEN.ZEESource     = cms.InputTag('zeegen')
process.zjetsGEN.JetSource     = cms.InputTag('genJetSelector')
process.zjetsGEN.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.zjetsGEN.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.zjetsGEN.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.genLevelPlots = cms.Sequence(process.genjets*process.genZ*process.zjetsGEN)

####################
######RECO##########
####################


##select jets according to these criteria
import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
process.newJetSelector = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedLayer1Jets.clone()
process.newJetSelector.src = cms.InputTag("selectedLayer1Jets")
process.newJetSelector.cut = cms.string('pt > 30. & abs(eta) < 5. & nConstituents > 0')

##select muons according to these criteria
import PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi
process.newMuonSelector = PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi.selectedLayer1Muons.clone()
process.newMuonSelector.src = cms.InputTag("selectedLayer1Muons")
process.newMuonSelector.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalMuonPromptTight') & innerTrack().found()>=11 & abs(globalTrack().d0())<0.2 & (trackIso()+caloIso()+ecalIso())/pt<0.1")


#Z candidate combiner
process.zmumu = cms.EDFilter('CandViewShallowCloneCombiner',
                              decay = cms.string('newMuonSelector@+ newMuonSelector@-'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('Zmumu'),
                              roles = cms.vstring('mu1', 'mu2')
)

process.zee = cms.EDFilter('CandViewShallowCloneCombiner',
                              decay = cms.string('selectedLayer1Electrons@+ selectedLayer1Electrons@-'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('Zee'),
                              roles = cms.vstring('e1', 'e2')
)

# reco plot
import Firenze.Reduction.ZJetsPlots_cff
process.zjetsRECO = Firenze.Reduction.ZJetsPlots_cff.zjetsanalysis.clone()
process.zjetsRECO.ZMuMuSource   = cms.InputTag('zmumu')
process.zjetsRECO.ZEESource     = cms.InputTag('zee')
process.zjetsRECO.JetSource     = cms.InputTag('newJetSelector')
process.zjetsRECO.DiffJetRate10 = cms.InputTag('diffCaloJetRate10', 'DiffJetRate10')
process.zjetsRECO.DiffJetRate21 = cms.InputTag('diffCaloJetRate21', 'DiffJetRate21')
process.zjetsRECO.DiffJetRate32 = cms.InputTag('diffCaloJetRate32', 'DiffJetRate32')

process.recLevelPlots = cms.Sequence(process.newJetSelector*process.newMuonSelector*process.zmumu*process.zee*process.zjetsRECO)

#process.p = cms.Path(process.newJetSelector*process.newMuonSelector*process.zmumu*process.zee*process.zjetsanalysis) 
process.p1 = cms.Path(process.genLevelPlots) 
process.p2 = cms.Path(process.recLevelPlots) 
