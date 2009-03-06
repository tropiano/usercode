import FWCore.ParameterSet.Config as cms

process = cms.Process('repat')
 
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
 
process.source = cms.Source('PoolSource',
                            fileNames = cms.untracked.vstring(
                            'file:/cms/data01/lenzip/testdata/04561ADB-38E3-DD11-AA7D-00E08178C035.root'
                            )
)                            
#keep the logging output to a nice level
process.MessageLogger = cms.Service('MessageLogger')


####################
######GEN###########
####################

##reconstruct gen jets
import PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi
process.load('Firenze.JetUtilities.NewGenJetParticles_cff')
process.load('RecoJets.Configuration.RecoGenJets_cff')
process.sisCone5GenJets.src=cms.InputTag('newGenParticlesForJets')
process.genjets = cms.Sequence(process.newGenParticlesForJets*process.sisCone5GenJets)

####################
######RECO##########
####################

#rebuild siscone5 jets
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('RecoJets.JetProducers.sisCone5CaloJets_cff')
process.load('RecoJets.JetAssociationProducers.sisCone5JTA_cff')
process.recojets = cms.Sequence(process.sisCone5CaloJets*process.sisCone5JetTracksAssociatorAtVertex)


####rebuild pat jets
#load all pat stuff
process.load("PhysicsTools.PatAlgos.patLayer0_cff")
process.load("PhysicsTools.PatAlgos.patLayer1_cff")
#load jet corrections
#For the standard calorimeter jets ( CaloJets ), Summer08 and  
#Summer08Redigi jet corrections are practically identical.
#You may use either of them. Perhaps it is more convenient to use  
#Summer08 if you are using CMSSW_2_2_X because
#it is part of the release and you don't need any additional checkout.
#However, if you want to use PFjets or JetPlusTrack, then perhaps you  
#should contact the developers
#of these objects to tell you which version of them was used for the  
#Fall08 production.
#In general, the difference between Summer08 and Summer08Redigi lies  
#exclusively in the PFjets and JPT objects.
#cheers
#Kostas
process.load('JetMETCorrections.Configuration.L2L3Corrections_Summer08_cff') ##These should be the right corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
#use standard pat tools to switch the jet collection
switchJetCollection(process, 
        'sisCone5CaloJets',    # Jet collection; must be already in the event when patLayer0 sequence is executed
        layers=[0,1],          # If you're not runnint patLayer1, set 'layers=[0]' 
        runCleaner="CaloJet",  # =None if not to clean
        doJTA=True,            # Run Jet-Track association & JetCharge
        doBTagging=True,       # Run b-tagging
        jetCorrLabel=('SC5','Calo'), # example jet correction name; set to None for no JEC
        doType1MET=True)
#manually change the jet collection to be used in the genJetMatching        
process.jetGenJetMatch.matched=cms.InputTag('sisCone5GenJets')
#since we don't have allLayer0Electrons anymode we have to disable the jet electron overlap matching in allLayer0Jets
process.allLayer0Jets.removeOverlaps = cms.PSet()
#process.allLayer1Jets.getJetMCFlavour = cms.bool(False)
process.allLayer1Jets.addTrigMatch = cms.bool(False)
process.patAODJetTracksAssociator.tracks = cms.InputTag('sisCone5JetTracksAssociatorAtVertex')

#process.load('PhysicsTools.PatAlgos.mcMatchLayer0.mcMatchSequences_cff')

process.patjets = cms.Sequence(#process.patAODJetMETCorrections*
                               process.allLayer0Jets*
                               process.patMCTruth_Jet*
                               process.patJetFlavourId*
                               process.jetCorrFactors*process.layer0JetCorrFactors*
                               process.patLayer0JetTracksCharge*
                               #process.patAODJetTracksAssociator * process.layer0JetTracksAssociator *
                               process.allLayer1Jets*
                               process.selectedLayer1Jets)

process.out = cms.OutputModule("PoolOutputModule",
                               outputCommands = cms.untracked.vstring('drop *', 
                                                                      'keep *_sisCone5GenJets_*_repat',
                                                                      'keep *_selectedLayer1Jets_*_repat'),
                               fileName = cms.untracked.string('test.root'))

###################
####PATH###########
###################
process.p1 = cms.Path(process.genjets*process.recojets*process.patjets)

process.e  = cms.EndPath(process.out)
