import FWCore.ParameterSet.Config as cms

process = cms.Process('plots')
 
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

#process.load('Firenze.Reduction.pattuplesV2_ZJetsMadgraph_cff')
#process.source.dropDescendantsOfDroppedBranches=cms.untracked.bool(False)
#process.source.inputCommands = cms.untracked.vstring('keep *',
#                                                     'drop *_*_genEventScale_*',
#                                                     'drop *_*_genEventWeight_*',
#                                                     'drop *_*_TriggerResults_*',
#                                                     'drop *_*_genEventPdfInfo_*',
#                                                     'drop *_*_hltTriggerSummaryAOD_*',
#                                                     'drop *_*_towerMaker_*',
#                                                     'drop *_*_offlineBeamSpot_*',
#                                                     'drop *_*_offlinePrimaryVertices_*',
#                                                     'drop *_*_selectedLayer1Electrons_*',
#                                                     'drop *_*_selectedLayer1Photons_*',
#                                                     'drop *_*_selectedLayer1Taus_*')

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:test.root'),
                            secFileNames = cms.untracked.vstring('file:/cms/data01/datasets/ZJets-madgraph/Fall08_IDEAL_V9_reco-v2/PATFirenzeV2/result/ZJets-madgraph_PATLayer1_sisCone_full_100.root')
)                            


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

# root output file service
process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string('output.root'),
                                   closeFileFast = cms.untracked.bool(True)
)


## build plots about gen Z
import Firenze.Reduction.ZJetsPlots_cff
process.zjetsGEN = Firenze.Reduction.ZJetsPlots_cff.zjetsanalysis.clone()
process.zjetsGEN.ZSource   = cms.InputTag('zmumugen')
process.zjetsGEN.JetSource     = cms.InputTag('genJetSelector')
process.zjetsGEN.DiffJetRate10 = cms.InputTag('diffGenJetRate10', 'DiffJetRate10')
process.zjetsGEN.DiffJetRate21 = cms.InputTag('diffGenJetRate21', 'DiffJetRate21')
process.zjetsGEN.DiffJetRate32 = cms.InputTag('diffGenJetRate32', 'DiffJetRate32')

process.genLevelPlots = cms.Sequence(process.zjetsGEN)

####################
######RECO##########
####################
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


process.p1 = cms.Path(process.recLevelPlots)
process.p2 = cms.Path(process.genLevelPlots)
