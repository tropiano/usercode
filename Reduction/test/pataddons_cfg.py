import FWCore.ParameterSet.Config as cms

process = cms.Process('addons')
 
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

process.load('Firenze.Reduction.pattuplesV2_ZJetsMadgraph_cff')
process.source.dropDescendantsOfDroppedBranches=cms.untracked.bool(False)
process.source.inputCommands = cms.untracked.vstring('keep *',
                                                     'drop *_*_genEventScale_*',
                                                     'drop *_*_genEventWeight_*',
                                                     'drop *_*_TriggerResults_*',
                                                     'drop *_*_genEventPdfInfo_*',
                                                     'drop *_*_hltTriggerSummaryAOD_*',
                                                     'drop *_*_towerMaker_*',
                                                     'drop *_*_offlineBeamSpot_*',
                                                     'drop *_*_offlinePrimaryVertices_*',
                                                     'drop *_*_selectedLayer1Electrons_*',
                                                     'drop *_*_selectedLayer1Photons_*',
                                                     'drop *_*_selectedLayer1Taus_*')


#keep the logging output to a nice level
#process.MessageLogger = cms.Service('MessageLogger')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

process.Timing = cms.Service('Timing',
                              summaryOnly = cms.untracked.bool(True)
)

process.options = cms.untracked.PSet(
         wantSummary = cms.untracked.bool(True),
         fileMode = cms.untracked.string('NOMERGE')
)

  


##event weight 
#include "MyAnalysis/ZplusJets/data/MyAlpgenEventWeight.cfi"

####################
######GEN###########
####################

##reconstruct gen jets
process.genJetSelector = cms.EDFilter('GenJetSelector', 
                                      src = cms.InputTag('sisCone5GenJets'),
                                      cut = cms.string('pt > 30. & abs(eta) < 5. & nConstituents > 0')
                                      )
process.genjets = cms.Sequence(process.genJetSelector)

##select gen muons from Z
process.genMuMinus = cms.EDFilter('Status3MotherCombiner',
                                   src = cms.InputTag('genParticles'),
                                   motherId = cms.int32(13),
                                   motherCharge = cms.int32(-1))

process.genMuPlus = cms.EDFilter('Status3MotherCombiner',
                                   src = cms.InputTag('genParticles'),
                                   motherId = cms.int32(-13),
                                   motherCharge = cms.int32(1))

process.genMuMinusSelector = cms.EDFilter('CandViewSelector',
                                           src = cms.InputTag('genMuMinus'),
                                           #cut = cms.string('pt > 20 & abs(eta) < 2.4'))                                   
                                           cut = cms.string(''))                                   
process.genMuPlusSelector = cms.EDFilter('CandViewSelector',
                                           src = cms.InputTag('genMuPlus'),
                                           #cut = cms.string('pt > 20 & abs(eta) < 2.4'))
                                           cut = cms.string(''))

##select gen Z mumu
process.zmumugen = cms.EDFilter('CandViewShallowCloneCombiner',
                                decay = cms.string('genMuPlusSelector@+ genMuMinusSelector@-'),
                                cut   = cms.string('50 < mass < 150'),
                                name  = cms.string('ZmumuGen'),
                                roles = cms.vstring('mu1', 'mu2')
)

process.genZmumu = cms.Sequence(process.genMuPlus*process.genMuMinus*process.genMuPlusSelector*process.genMuMinusSelector*process.zmumugen)

## build plots about gen Z
import Firenze.Reduction.ZJetsPlots_cff
process.zjetsGEN = Firenze.Reduction.ZJetsPlots_cff.zjetsanalysis.clone()
process.zjetsGEN.ZSource   = cms.InputTag('zmumugen')
process.zjetsGEN.JetSource     = cms.InputTag('genJetSelector')
process.zjetsGEN.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.zjetsGEN.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.zjetsGEN.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.genLevelPlots = cms.Sequence(process.genjets*process.genZmumu*process.zjetsGEN)

####################
######RECO##########
####################


##select jets according to these criteria
#import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
#process.newJetSelector = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedLayer1Jets.clone()
#process.newJetSelector.src = cms.InputTag("selectedLayer1Jets")
#process.newJetSelector.cut = cms.string('pt > 30. & abs(eta) < 5. & nConstituents > 0')

process.newJetSelector = cms.EDFilter('PATJetRefSelector',
                                      src = cms.InputTag("selectedLayer1Jets"),
                                      cut = cms.string('pt > 30. & abs(eta) < 5. & nConstituents > 0')
)

##select muons according to these criteria
import PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi
process.newMuonSelector1 = PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi.selectedLayer1Muons.clone()
process.newMuonSelector1.src = cms.InputTag("selectedLayer1Muons")
#process.newMuonSelector.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalMuonPromptTight') & innerTrack().found()>=11 & abs(globalTrack().d0())<0.2 & (trackIso()+caloIso()+ecalIso())/pt<0.1")
process.newMuonSelector1.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalMuonPromptTight') & innerTrack().found()>=11 & abs( ((globalTrack().vx()-0.0325839)*globalTrack().py() + (globalTrack().vy()-0.000173)*globalTrack().px())/globalTrack().pt )<0.02 & (trackIso()+caloIso()+ecalIso())/pt<0.1")

process.newMuonSelector2 = PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi.selectedLayer1Muons.clone()
process.newMuonSelector2.src = cms.InputTag("selectedLayer1Muons")
#process.newMuonSelector.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalMuonPromptTight') & innerTrack().found()>=11 & abs(globalTrack().d0())<0.2 & (trackIso()+caloIso()+ecalIso())/pt<0.1")
process.newMuonSelector2.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalMuonPromptTight') & innerTrack().found()>=11 & (trackIso()+caloIso()+ecalIso())/pt<0.1")

process.newMuonSelector3 = PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi.selectedLayer1Muons.clone()
process.newMuonSelector3.src = cms.InputTag("selectedLayer1Muons")
#process.newMuonSelector.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalMuonPromptTight') & innerTrack().found()>=11 & abs(globalTrack().d0())<0.2 & (trackIso()+caloIso()+ecalIso())/pt<0.1")
process.newMuonSelector3.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalMuonPromptTight') & innerTrack().found()>=11")


#Z candidate combiner
process.zmumuIsoDxy = cms.EDFilter('CandViewShallowCloneCombiner',
                              decay = cms.string('newMuonSelector1@+ newMuonSelector1@-'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('Zmumu'),
                              roles = cms.vstring('mu1', 'mu2')
)

process.zmumuIso = cms.EDFilter('CandViewShallowCloneCombiner',
                              decay = cms.string('newMuonSelector2@+ newMuonSelector2@-'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('Zmumu'),
                              roles = cms.vstring('mu1', 'mu2')
)

process.zmumu = cms.EDFilter('CandViewShallowCloneCombiner',
                              decay = cms.string('newMuonSelector3@+ newMuonSelector3@-'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('Zmumu'),
                              roles = cms.vstring('mu1', 'mu2')
)

process.recz1 = cms.Sequence(process.newMuonSelector1*process.zmumuIsoDxy)
process.recz2 = cms.Sequence(process.newMuonSelector2*process.zmumuIso)
process.recz3 = cms.Sequence(process.newMuonSelector3*process.zmumu)

process.recz = cms.Sequence(process.recz1+process.recz2+process.recz3)

##MET selection
process.metfilter = cms.EDFilter('PATMETSelector',
                           src    = cms.InputTag('selectedLayer1METs'),
                           cut    = cms.string('et < 50.'),
                           filter = cms.bool(True)
)


# reco plot
import Firenze.Reduction.ZJetsPlots_cff
process.plotsIsoDxy = Firenze.Reduction.ZJetsPlots_cff.zjetsanalysis.clone()
process.plotsIsoDxy.ZSource   = cms.InputTag('zmumuIsoDxy')
process.plotsIsoDxy.JetSource     = cms.InputTag('newJetSelector')
process.plotsIsoDxy.DiffJetRate10 = cms.InputTag('diffCaloJetRate10', 'DiffJetRate10')
process.plotsIsoDxy.DiffJetRate21 = cms.InputTag('diffCaloJetRate21', 'DiffJetRate21')
process.plotsIsoDxy.DiffJetRate32 = cms.InputTag('diffCaloJetRate32', 'DiffJetRate32')

process.plotsIso = Firenze.Reduction.ZJetsPlots_cff.zjetsanalysis.clone()
process.plotsIso.ZSource   = cms.InputTag('zmumuIso')
process.plotsIso.JetSource     = cms.InputTag('newJetSelector')
process.plotsIso.DiffJetRate10 = cms.InputTag('diffCaloJetRate10', 'DiffJetRate10')
process.plotsIso.DiffJetRate21 = cms.InputTag('diffCaloJetRate21', 'DiffJetRate21')
process.plotsIso.DiffJetRate32 = cms.InputTag('diffCaloJetRate32', 'DiffJetRate32')

process.plots = Firenze.Reduction.ZJetsPlots_cff.zjetsanalysis.clone()
process.plots.ZSource   = cms.InputTag('zmumu')
process.plots.JetSource     = cms.InputTag('newJetSelector')
process.plots.DiffJetRate10 = cms.InputTag('diffCaloJetRate10', 'DiffJetRate10')
process.plots.DiffJetRate21 = cms.InputTag('diffCaloJetRate21', 'DiffJetRate21')
process.plots.DiffJetRate32 = cms.InputTag('diffCaloJetRate32', 'DiffJetRate32')

process.recLevelPlots = cms.Sequence(process.plotsIsoDxy+process.plotsIso+process.plots)



process.out = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string('test.root'),
  #SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
  outputCommands = cms.untracked.vstring('drop *',
                                         'keep *_*_*_addons')
)



process.p1 = cms.Path(process.newJetSelector*process.recz)
process.p2 = cms.Path(process.genjets*process.genZmumu)
#process.p = cms.Path(process.genjets*process.genZmumu+process.newJetSelector*process.recz)
#process.p = cms.Path(process.genJetSelector*process.genMuMinus*process.genMuPlus*process.genMuPlusSelector*process.genMuMinusSelector*process.zmumugen)
#process.s = cms.Schedule(process.p1,process.p2)
process.e  = cms.EndPath(process.out) 
