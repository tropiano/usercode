import FWCore.ParameterSet.Config as cms

process = cms.Process('ZjetsPlots')
 
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.load('Firenze.Reduction.pattuplesV2_ZJetsMadgraph_cff')

#process.source = cms.Source('PoolSource',
                            fileNames = cms.untracked.vstring(
#'file:/cms/data01/datasets/ZJets-madgraph/Summer08_IDEAL_V9_PAT_v3/USER/1402F759-5EE7-DD11-ADE5-0015170AB26C.root',
#'file:/cms/data01/datasets/ZJets-madgraph/Summer08_IDEAL_V9_PAT_v3/USER/1E7D9992-34E3-DD11-85F4-00E08178C069.root',
#'file:/cms/data01/datasets/ZJets-madgraph/Summer08_IDEAL_V9_PAT_v3/USER/744CBFDE-38E3-DD11-90EF-00E08179189F.root'
#'file:/cms/data01/datasets/ZJets-madgraph/Fall08_IDEAL_V9_reco-v2/PATFirenzeV2/result/ZJets-madgraph_PATLayer1_sisCone_full_9.root'
                            )
#                            secondaryFileNames = cms.untracked.vstring(
#                            'file:/cms/data01/lenzip/testdata/04561ADB-38E3-DD11-AA7D-00E08178C035.root'
#                            )
)                            
#keep the logging output to a nice level
#process.MessageLogger = cms.Service('MessageLogger')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)



##event weight 
#include "MyAnalysis/ZplusJets/data/MyAlpgenEventWeight.cfi"

# root output file service
process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string('ZJetsMadgraph_RUN.root')
)

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
                                           cut = cms.string('pt > 20 & abs(eta) < 2.4'))                                   
process.genMuPlusSelector = cms.EDFilter('CandViewSelector',
                                           src = cms.InputTag('genMuPlus'),
                                           cut = cms.string('pt > 20 & abs(eta) < 2.4'))

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
import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
process.newJetSelector = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedLayer1Jets.clone()
process.newJetSelector.src = cms.InputTag("selectedLayer1Jets")
process.newJetSelector.cut = cms.string('pt > 30. & abs(eta) < 5. & nConstituents > 0')

##select muons according to these criteria
import PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi
process.newMuonSelector = PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi.selectedLayer1Muons.clone()
process.newMuonSelector.src = cms.InputTag("selectedLayer1Muons")
#process.newMuonSelector.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalMuonPromptTight') & innerTrack().found()>=11 & abs(globalTrack().d0())<0.2 & (trackIso()+caloIso()+ecalIso())/pt<0.1")
process.newMuonSelector.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalMuonPromptTight') & innerTrack().found()>=11 & abs( ((globalTrack().vx()-0.0325839)*globalTrack().py() + (globalTrack().vy()-0.000173)*globalTrack().px())/globalTrack().pt )<0.02 & (trackIso()+caloIso()+ecalIso())/pt<0.1")


#Z candidate combiner
process.zmumu = cms.EDFilter('CandViewShallowCloneCombiner',
                              decay = cms.string('newMuonSelector@+ newMuonSelector@-'),
                              cut   = cms.string('50 < mass < 150'),
                              name  = cms.string('Zmumu'),
                              roles = cms.vstring('mu1', 'mu2')
)

# reco plot
import Firenze.Reduction.ZJetsPlots_cff
process.zjetsRECO = Firenze.Reduction.ZJetsPlots_cff.zjetsanalysis.clone()
process.zjetsRECO.ZSource   = cms.InputTag('zmumu')
process.zjetsRECO.JetSource     = cms.InputTag('newJetSelector')
process.zjetsRECO.DiffJetRate10 = cms.InputTag('diffCaloJetRate10', 'DiffJetRate10')
process.zjetsRECO.DiffJetRate21 = cms.InputTag('diffCaloJetRate21', 'DiffJetRate21')
process.zjetsRECO.DiffJetRate32 = cms.InputTag('diffCaloJetRate32', 'DiffJetRate32')

process.recLevelPlots = cms.Sequence(process.newJetSelector*process.newMuonSelector*process.zmumu*process.zjetsRECO)

process.out = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string('test.root'),
  #SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
  outputCommands = cms.untracked.vstring('drop *',
                                         'keep *_*_*_ZjetsPlots',
                                         'keep *_genParticles_*_*')
)


process.p1 = cms.Path(process.genLevelPlots) 
process.p2 = cms.Path(process.recLevelPlots)
#process.e  = cms.EndPath(process.out) 
