import FWCore.ParameterSet.Config as cms

process = cms.Process('PATAddOns')
 
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.load('Firenze.Reduction.pattuples_ZJetsMadgraph_cff')

#process.source = cms.Source('PoolSource',
#                            fileNames = cms.untracked.vstring(
#'file:/cms/data01/datasets/ZJets-madgraph/Summer08_IDEAL_V9_PAT_v3/USER/1402F759-5EE7-DD11-ADE5-0015170AB26C.root',
#'file:/cms/data01/datasets/ZJets-madgraph/Summer08_IDEAL_V9_PAT_v3/USER/1E7D9992-34E3-DD11-85F4-00E08178C069.root',
#'file:/cms/data01/datasets/ZJets-madgraph/Summer08_IDEAL_V9_PAT_v3/USER/744CBFDE-38E3-DD11-90EF-00E08179189F.root' 
#'file:/cms/data01/lenzip/CMSSW/PattuplesProduction/CMSSW_2_2_4/src/production/ZJets-madgraph_PATLayer1_sisCone_full.root'
#                            )
#                            secondaryFileNames = cms.untracked.vstring(
#                            'file:/cms/data01/lenzip/testdata/04561ADB-38E3-DD11-AA7D-00E08178C035.root'
#                            )
#)                            
#keep the logging output to a nice level
process.MessageLogger = cms.Service('MessageLogger')


##event weight 
#include "MyAnalysis/ZplusJets/data/MyAlpgenEventWeight.cfi"

# root output file service
process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string('zjetsplots_fixcorrections.root')
)

process.load('Configuration.StandardSequences.GeometryECALHCAL_cff')

process.load('Firenze.JetUtilities.NewGenJetParticles_cff')
process.load('Firenze.JetUtilities.exclusiveJets_cff')
process.load('Firenze.JetUtilities.diffJetRates_cff')

####################
######GEN###########
####################

##reconstruct gen jets
process.genJetSelector = cms.EDFilter('GenJetSelector', 
                                      src = cms.InputTag('sisCone5GenJets'),
                                      cut = cms.string('pt > 30. & abs(eta) < 5. & nConstituents > 0')
                                      )
process.diffGenJets = cms.Sequence(process.newGenParticlesForJets*process.allGenExclusiveJets*process.diffGenJetRates) 
process.genjets = cms.Sequence(process.genJetSelector+process.diffGenJets)


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

####################
######RECO##########
####################


##select jets according to these criteria
import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
process.newJetSelector = PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi.selectedLayer1Jets.clone()
process.newJetSelector.src = cms.InputTag("selectedLayer1Jets")
process.newJetSelector.cut = cms.string('pt > 30. & abs(eta) < 5. & nConstituents > 0')

process.diffRecoJets = cms.Sequence(process.allCaloExclusiveJets*process.diffCaloJetRates)
process.recojets = cms.Sequence(process.newJetSelector+process.diffRecoJets)

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

process.out = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string('pataddons_RUN.root'),
  #SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
  outputCommands = cms.untracked.vstring('drop *',
                                         'keep *_*_*_PATAddOns')
)

process.recZmumu = cms.Sequence(process.newMuonSelector*process.zmumu)
process.recZee   = cms.Sequence(process.zee)
process.recZ     = cms.Sequence(process.recZmumu+process.recZee)

process.p1 = cms.Path(process.genjets+process.genZ) 
process.p2 = cms.Path(process.recojets+process.recZ)

process.e  = cms.EndPath(process.out)


