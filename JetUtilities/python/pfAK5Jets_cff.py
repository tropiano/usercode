import FWCore.ParameterSet.Config as cms

#from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *

#pat jet correcrtions
patPFJetCorrFactors = cms.EDProducer("JetCorrFactorsProducer",
      ## the use of emf in the JEC is not yet implemented
      useEMF     = cms.bool(False),
      ## input collection of jets
      jetSource  = cms.InputTag("ak5PFJets"),
      ## set of correction factors
      corrSample = cms.string("Summer09_7TeV"),
      ## correction levels
      corrLevels = cms.PSet(
        ## tags for the individual jet corrections; when
        ## not available the string should be set to 'none'    
        L1Offset   = cms.string('none'),
        L2Relative = cms.string('L2Relative_AK5PF'),
        L3Absolute = cms.string('L3Absolute_AK5PF'),
        L4EMF      = cms.string('none'),
        L5Flavor   = cms.string('none'),       # to be changed to L5Flavor   = cms.string('L5Flavor_AK5'),
        L6UE       = cms.string('none'),
        L7Parton   = cms.string('none'),       # to be changed to L7Parton   = cms.string('L7Parton_AK5'),
      ),
      ## choose sample type for flavor dependend corrections:
      sampleType = cms.string('dijet')  ##  'dijet': from dijet sample
                                       ##  'ttbar': from ttbar sample
)

#pat genjet match
patPFJetGenJetMatch = cms.EDFilter("GenJetMatcher",    # cut on deltaR, deltaPt/Pt; pick best by deltaR
     src         = cms.InputTag("ak5PFJets"),       # RECO jets (any View<Jet> is ok)
     matched     = cms.InputTag("ak5GenJets"),        # GEN jets  (must be GenJetCollection)
     mcPdgId     = cms.vint32(),                      # n/a
     mcStatus    = cms.vint32(),                      # n/a
     checkCharge = cms.bool(False),                   # n/a
     maxDeltaR   = cms.double(0.4),                   # Minimum deltaR for the match
     maxDPtRel   = cms.double(3.0),                   # Minimum deltaPt/Pt for the match
     resolveAmbiguities    = cms.bool(True),          # Forbid two RECO objects to match to the same GEN object
     resolveByMatchQuality = cms.bool(False)          # False = just match input in order; True = pick lowest deltaR pair first
)

from  PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import patJets
allLayer1PFJets = patJets.clone()
allLayer1PFJets.jetSource = cms.InputTag('ak5PFJets')
allLayer1PFJets.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patPFJetCorrFactors') )
allLayer1PFJets.addGenPartonMatch = cms.bool(False)
allLayer1PFJets.genJetMatch = cms.InputTag("patPFJetGenJetMatch")
allLayer1PFJets.getJetMCFlavour = cms.bool(False)
allLayer1PFJets.addAssociatedTracks = cms.bool(False)
allLayer1PFJets.addBTagInfo = cms.bool(False)
allLayer1PFJets.addDiscriminators = cms.bool(False)
allLayer1PFJets.addJetID = cms.bool(False)
allLayer1PFJets.addJetCharge = cms.bool(False)
allLayer1PFJets.embedGenPartonMatch = cms.bool(False)



allLayer1PFJetsSequence =  cms.Sequence(patPFJetCorrFactors*patPFJetGenJetMatch*allLayer1PFJets)
