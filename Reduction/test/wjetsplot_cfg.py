import FWCore.ParameterSet.Config as cms

process = cms.Process('WjetsPlots')
 
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#process.load('Firenze.Reduction.pattuples_ZJetsMadgraph_cff')

process.source = cms.Source('PoolSource',
                            fileNames = cms.untracked.vstring(
#'file:/cms/data01/datasets/ZJets-madgraph/Summer08_IDEAL_V9_PAT_v3/USER/1402F759-5EE7-DD11-ADE5-0015170AB26C.root',
#'file:/cms/data01/datasets/ZJets-madgraph/Summer08_IDEAL_V9_PAT_v3/USER/1E7D9992-34E3-DD11-85F4-00E08178C069.root',
#'file:/cms/data01/datasets/ZJets-madgraph/Summer08_IDEAL_V9_PAT_v3/USER/744CBFDE-38E3-DD11-90EF-00E08179189F.root' 
'file:/cms/data01/datasets/WJets-madgraph/Fall08_IDEAL_V9_v1/PATFirenze/result/WJets-madgraph_PATLayer1_sisCone_full_97.root'
                            )
#                            secondaryFileNames = cms.untracked.vstring(
#                            'file:/cms/data01/lenzip/testdata/04561ADB-38E3-DD11-AA7D-00E08178C035.root'
#                            )
)                            
#keep the logging output to a nice level
#process.MessageLogger = cms.Service('MessageLogger')


##event weight 
#include "MyAnalysis/ZplusJets/data/MyAlpgenEventWeight.cfi"

# root output file service
process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string('wjetsplots.root')
)

####################
######GEN###########
####################

process.load('Firenze.JetUtilities.NewGenJetParticles_cff')
process.load('Firenze.JetUtilities.exclusiveJets_cff')
process.load('Firenze.JetUtilities.diffJetRates_cff')
process.diffGenJets = cms.Sequence(process.newGenParticlesForJets*process.allGenExclusiveJets*process.diffGenJetRates)

##reconstruct gen jets
process.genJetSelector = cms.EDFilter('GenJetSelector', 
                                      src = cms.InputTag('sisCone5GenJets'),
                                      cut = cms.string('pt > 30. & abs(eta) < 5. & nConstituents > 0')
                                      )
process.genjets = cms.Sequence(process.genJetSelector+process.diffGenJets)

##select taus from hard interaction
process.genTauPlus = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(-15),
                                          motherCharge = cms.int32(1))
process.genTauMinus = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(15),
                                          motherCharge = cms.int32(-1))
process.genNuTau = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(16),
                                          motherCharge = cms.int32(0))
process.genAntiNuTau = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(-16),
                                          motherCharge = cms.int32(0))
                                          
##select muons from hard interaction
process.genMuPlus = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(-13),
                                          motherCharge = cms.int32(1))
process.genMuMinus = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(13),
                                          motherCharge = cms.int32(-1))
process.genNuMu = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(14),
                                          motherCharge = cms.int32(0))
process.genAntiNuMu = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(-14),
                                          motherCharge = cms.int32(0))

##select electrons from hard interaction
process.genEPlus = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(-11),
                                          motherCharge = cms.int32(1))
process.genEMinus = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(11),
                                          motherCharge = cms.int32(-1))
process.genNuE = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(12),
                                          motherCharge = cms.int32(0))
process.genAntiNuE = cms.EDFilter('Status3MotherCombiner',
                                          src = cms.InputTag('genParticles'),
                                          motherId = cms.int32(-12),
                                          motherCharge = cms.int32(0))
###Selection
process.genTauMinusSelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genTauMinus'),
                                       cut = cms.string('pdgId == 15 & pt > 20 & abs(eta) < 2.4'))
process.genTauPlusSelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genTauPlus'),
                                       cut = cms.string('pdgId == -15 & pt > 20 & abs(eta) < 2.4'))
process.genNuTauSelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genNuTau'),
                                       cut = cms.string('pdgId == 16 && abs(eta) < 5.'))
process.genAntiNuTauSelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genAntiNuTau'),
                                       cut = cms.string('pdgId == -16 && abs(eta) < 5.'))

process.genMuMinusSelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genMuMinus'),
                                       cut = cms.string('pdgId == 13 & pt > 20 & abs(eta) < 2.4'))
process.genMuPlusSelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genMuPlus'),
                                       cut = cms.string('pdgId == -13 & pt > 20 & abs(eta) < 2.4'))
process.genNuMuSelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genNuMu'),
                                       cut = cms.string('pdgId == 14 && abs(eta) < 5.'))
process.genAntiNuMuSelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genAntiNuMu'),
                                       cut = cms.string('pdgId == -14 && abs(eta) < 5.'))

process.genEMinusSelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genEMinus'),
                                       cut = cms.string('pdgId == 11 & pt > 20 & abs(eta) < 2.4'))
process.genEPlusSelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genEPlus'),
                                       cut = cms.string('pdgId == -11 & pt > 20 & abs(eta) < 2.4'))
process.genNuESelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genNuE'),
                                       cut = cms.string('pdgId == 12 && abs(eta) < 5.'))
process.genAntiNuESelector = cms.EDFilter('CandViewSelector',
                                       src = cms.InputTag('genAntiNuE'),
                                       cut = cms.string('pdgId == -12 && abs(eta) < 5.'))

##select gen W- numu mu
process.wminustomugen = cms.EDFilter('CandViewTransverseCombiner',
                                decay = cms.string('genMuMinusSelector@- genAntiNuMuSelector'),
                                cut   = cms.string('50 < mass < 150'),
                                name  = cms.string('WminusTomuGen'),
                                roles = cms.vstring('mu', 'numu')
)

##select gen W- nue e
process.wminustoegen = cms.EDFilter('CandViewTransverseCombiner',
                                decay = cms.string('genEMinusSelector@- genAntiNuESelector'),
                                cut   = cms.string('50 < mass < 150'),
                                name  = cms.string('WminusToeGen'),
                                roles = cms.vstring('e', 'nue')
)

##select gen W+ numu mu
process.wplustomugen = cms.EDFilter('CandViewTransverseCombiner',
                                decay = cms.string('genMuPlusSelector@+ genNuMuSelector'),
                                cut   = cms.string('50 < mass < 150'),
                                name  = cms.string('WplusTomuGen'),
                                roles = cms.vstring('mu', 'numu')
)

##select gen W- nue e
process.wplustoegen = cms.EDFilter('CandViewTransverseCombiner',
                                decay = cms.string('genEPlusSelector@+ genNuESelector'),
                                cut   = cms.string('50 < mass < 150'),
                                name  = cms.string('WplusToeGen'),
                                roles = cms.vstring('e', 'nue')
)


##select gen W+ nutau tau
process.wplustotaugen = cms.EDFilter('CandViewTransverseCombiner',
                                decay = cms.string('genTauPlusSelector@+ genNuTauSelector'),
                                cut   = cms.string('50 < mass < 150'),
                                name  = cms.string('WplusTotauGen'),
                                roles = cms.vstring('tau', 'nutau')
)

##select gen W- nutau tau
process.wminustotaugen = cms.EDFilter('CandViewTransverseCombiner',
                                decay = cms.string('genTauMinusSelector@+ genAntiNuTauSelector'),
                                cut   = cms.string('50 < mass < 150'),
                                name  = cms.string('WminusTotauGen'),
                                roles = cms.vstring('tau', 'nutau')
)
process.genWmu = cms.Sequence(process.genMuMinus*process.genMuPlus*process.genNuMu*process.genAntiNuMu*
                              process.genMuMinusSelector*process.genMuPlusSelector*process.genNuMuSelector*process.genAntiNuMuSelector*
                              process.wplustomugen*process.wminustomugen)


process.genWe  = cms.Sequence(process.genEMinus*process.genEPlus*process.genNuE*process.genAntiNuE*
                              process.genEMinusSelector*process.genEPlusSelector*process.genNuESelector*process.genAntiNuESelector*
                              process.wplustoegen*process.wminustoegen)


process.genWtau  = cms.Sequence(process.genTauMinus*process.genTauPlus*process.genNuTau*process.genAntiNuTau*
                                process.genTauMinusSelector*process.genTauPlusSelector*process.genNuTauSelector*process.genAntiNuTauSelector*
                                process.wplustotaugen*process.wminustotaugen)


process.genW   = cms.Sequence(process.genWmu*
                              process.genWe*
                              process.genWtau)

## build plots about gen W 
import Firenze.Reduction.WJetsPlots_cff
process.wplustomuGEN = Firenze.Reduction.WJetsPlots_cff.wjetsanalysis.clone()
process.wplustomuGEN.WSource   = cms.InputTag('wplustomugen')
process.wplustomuGEN.JetSource     = cms.InputTag('genJetSelector')
process.wplustomuGEN.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.wplustomuGEN.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.wplustomuGEN.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.wminustomuGEN = Firenze.Reduction.WJetsPlots_cff.wjetsanalysis.clone()
process.wminustomuGEN.WSource   = cms.InputTag('wminustomugen')
process.wminustomuGEN.JetSource     = cms.InputTag('genJetSelector')
process.wminustomuGEN.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.wminustomuGEN.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.wminustomuGEN.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.wplustoeGEN = Firenze.Reduction.WJetsPlots_cff.wjetsanalysis.clone()
process.wplustoeGEN.WSource   = cms.InputTag('wplustoegen')
process.wplustoeGEN.JetSource     = cms.InputTag('genJetSelector')
process.wplustoeGEN.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.wplustoeGEN.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.wplustoeGEN.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.wminustoeGEN = Firenze.Reduction.WJetsPlots_cff.wjetsanalysis.clone()
process.wminustoeGEN.WSource   = cms.InputTag('wminustoegen')
process.wminustoeGEN.JetSource     = cms.InputTag('genJetSelector')
process.wminustoeGEN.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.wminustoeGEN.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.wminustoeGEN.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.wplustotauGEN = Firenze.Reduction.WJetsPlots_cff.wjetsanalysis.clone()
process.wplustotauGEN.WSource   = cms.InputTag('wplustotaugen')
process.wplustotauGEN.JetSource     = cms.InputTag('genJetSelector')
process.wplustotauGEN.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.wplustotauGEN.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.wplustotauGEN.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.wminustotauGEN = Firenze.Reduction.WJetsPlots_cff.wjetsanalysis.clone()
process.wminustotauGEN.WSource   = cms.InputTag('wminustotaugen')
process.wminustotauGEN.JetSource     = cms.InputTag('genJetSelector')
process.wminustotauGEN.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.wminustotauGEN.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.wminustotauGEN.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.genLevelPlots = cms.Sequence(process.genjets*process.genW*
                                     process.wplustomuGEN*process.wplustoeGEN*process.wminustomuGEN*process.wminustoeGEN*process.wplustotauGEN*process.wminustotauGEN)


###################
######RECO#########
###################

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

##select electrons according to these criteria
import PhysicsTools.PatAlgos.selectionLayer1.electronSelector_cfi
process.newElectronSelector = PhysicsTools.PatAlgos.selectionLayer1.electronSelector_cfi.selectedLayer1Electrons.clone()
process.newElectronSelector.src = cms.InputTag("selectedLayer1Electrons")
process.newElectronSelector.cut = cms.string("pt > 20. & abs(eta) < 2.4 & electronID('eidRobustTight') & abs(gsfTrack().d0())<0.2 & (trackIso()+caloIso()+ecalIso())/pt<0.1")
#Z candidate combiner
process.wplustomurec = cms.EDFilter('CandViewTransverseCombiner',
                              decay = cms.string('newMuonSelector@+ selectedLayer1METs'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('wplustomuREC'),
                              roles = cms.vstring('mu', 'numu')
)

process.wplustoerec = cms.EDFilter('CandViewTransverseCombiner',
                              decay = cms.string('newElectronSelector@+ selectedLayer1METs'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('wplustoeREC'),
                              roles = cms.vstring('e', 'nue')
)

process.wminustomurec = cms.EDFilter('CandViewTransverseCombiner',
                              decay = cms.string('newMuonSelector@- selectedLayer1METs'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('wminustomuREC'),
                              roles = cms.vstring('mu', 'numu')
)

process.wminustoerec = cms.EDFilter('CandViewTransverseCombiner',
                              decay = cms.string('newElectronSelector@- selectedLayer1METs'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('wminustoeREC'),
                              roles = cms.vstring('e', 'nue')
)

process.recWmu = cms.Sequence(process.newMuonSelector*process.wplustomurec*process.wminustomurec)
process.recWe  = cms.Sequence(process.newElectronSelector*process.wplustoerec*process.wminustoerec)
process.recW   = cms.Sequence(process.recWmu*process.recWe)


# reco plot
import Firenze.Reduction.WJetsPlots_cff
process.wplustomuREC = Firenze.Reduction.WJetsPlots_cff.wjetsanalysis.clone()
process.wplustomuREC.WSource   = cms.InputTag('wplustomurec')
process.wplustomuREC.JetSource     = cms.InputTag('newJetSelector')
process.wplustomuREC.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.wplustomuREC.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.wplustomuREC.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.wminustomuREC = Firenze.Reduction.WJetsPlots_cff.wjetsanalysis.clone()
process.wminustomuREC.WSource   = cms.InputTag('wminustomurec')
process.wminustomuREC.JetSource     = cms.InputTag('newJetSelector')
process.wminustomuREC.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.wminustomuREC.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.wminustomuREC.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.wplustoeREC = Firenze.Reduction.WJetsPlots_cff.wjetsanalysis.clone()
process.wplustoeREC.WSource   = cms.InputTag('wplustoerec')
process.wplustoeREC.JetSource     = cms.InputTag('newJetSelector')
process.wplustoeREC.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.wplustoeREC.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.wplustoeREC.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.wminustoeREC = Firenze.Reduction.WJetsPlots_cff.wjetsanalysis.clone()
process.wminustoeREC.WSource   = cms.InputTag('wminustoerec')
process.wminustoeREC.JetSource     = cms.InputTag('newJetSelector')
process.wminustoeREC.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.wminustoeREC.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.wminustoeREC.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.recLevelPlots = cms.Sequence(process.newJetSelector*process.recW*process.wplustomuREC*process.wplustoeREC*process.wminustomuREC*process.wminustoeREC)

process.out = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string('pataddons_RUN.root'),
  #SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
  outputCommands = cms.untracked.vstring('drop *',
                                         'keep *_*_*_WjetsPlots',
                                         'keep *_genParticles_*_*')
)

process.p1 = cms.Path(process.genLevelPlots) 
#process.p2 = cms.Path(process.recLevelPlots) 
#process.e = cms.EndPath(process.out)
