import FWCore.ParameterSet.Config as cms

process = cms.Process('ExclusiveJets')
 
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
 
process.source = cms.Source('PoolSource',
                            fileNames = cms.untracked.vstring(
                            'file:/cms/data01/lenzip/testdata/04561ADB-38E3-DD11-AA7D-00E08178C035.root'
                            )
)
#keep the logging output to a nice level
#process.MessageLogger = cms.Service('MessageLogger')
process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring("detailedInfo"),
                                    detailedInfo = cms.untracked.PSet(threshold = cms.untracked.string("DEBUG"))
                                    )
#FastJetDefinition

process.load('Configuration.StandardSequences.GeometryECALHCAL_cff')

##event weight 
#include "MyAnalysis/ZplusJets/data/MyAlpgenEventWeight.cfi"
#process.load('Firenze.JetUtilities.exclusive1CaloJets10_cff')
#process.load('Firenze.JetUtilities.exclusive2CaloJets10_cff')
#process.load('Firenze.JetUtilities.exclusive3CaloJets10_cff')
process.load('Firenze.JetUtilities.NewGenJetParticles_cff')
process.load('Firenze.JetUtilities.exclusiveJets_cff')
process.load('Firenze.JetUtilities.diffJetRates_cff')


process.out = cms.OutputModule("PoolOutputModule",
                               outputCommands = cms.untracked.vstring('drop *', 'keep *_diff*JetRate*_*_*'),
                               fileName = cms.untracked.string('test.root'))
                               

#process.p = cms.Path(process.exclusive1CaloJets10*process.exclusive2CaloJets10*process.exclusive3CaloJets10)
process.p = cms.Path(process.newGenParticlesForJets*process.allExclusiveJets6*process.diffJetRates)
process.e = cms.EndPath(process.out) 
