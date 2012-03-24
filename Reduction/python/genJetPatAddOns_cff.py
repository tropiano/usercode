import FWCore.ParameterSet.Config as cms

########################
# Gen Jets Selector
########################
#http://cmslxr.fnal.gov/lxr/source/RecoJets/JetProducers/plugins/InputGenJetsParticleSelector.cc?v=CMSSW_4_2_5
#https://twiki.cern.ch/twiki/bin/view/Main/AVFedotovHowToCmsJets#4_InputGenJetsParticleSelector_p

################# Sandro

# Gen Particles Selector: it eliminates the Muons from Z from the Particle and some resonances BSM from the collection
GenParticlesNoMuMufromZ = cms.EDProducer('InputGenJetsParticleSelector',
               src = cms.InputTag('genParticles'),
               partonicFinalState = cms.bool(False),  
               excludeResonances = cms.bool(True), 
               excludeFromResonancePids = cms.vuint32(13),
#               excludeFromResonancePids = cms.vuint32(0),
               tausAsJets = cms.bool(False),
               ignoreParticleIDs = cms.vuint32(1000022, 2000012, 2000014,
                                                2000016, 1000039, 5000039,
                                                4000012, 9900012, 9900014,
                                               9900016, 39)
)

from RecoJets.JetProducers.GenJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *

# Gen Particles Parameters: we sobstitute the Gen Particles Selector to the standard one
GenJetParameters.src = cms.InputTag('GenParticlesNoMuMufromZ')

# We create a class of ak5GenJets without Muons from Z
ak5GenJetsmod = cms.EDProducer("FastjetJetProducer",
     GenJetParameters,
     AnomalousCellParameters,
     jetAlgorithm = cms.string("AntiKt"),
     rParam       = cms.double(0.5)
     )

# We apply some cuts to the ak5GenJet Collection
selectedGenJetsNoMuMufromZ = cms.EDFilter('GenJetSelector',
   src = cms.InputTag('ak5GenJetsmod'),
   cut = cms.string('pt > 15. & abs(eta) < 3. & nConstituents > 0')
)

genjetsNoMuMufromZSequence = cms.Sequence( (GenParticlesNoMuMufromZ * ak5GenJetsmod ) * selectedGenJetsNoMuMufromZ)

################# PierGiulio

# Gen Particles Selector Status3: it eliminates the Muons from Z from the Particle Status 3 and some resonances BSM from the collection
GenParticlesStatus3NoMuMufromZ = cms.EDProducer('NewGenJetsParticleSelector',
               src = cms.InputTag("genParticles"),
               partonicFinalState = cms.bool(False),  
               excludeResonances = cms.bool(True), 
               excludeFromResonancePids = cms.vuint32(13),
#               excludeFromResonancePids = cms.vuint32(0),
               tausAsJets = cms.bool(False),
               ignoreParticleIDs = cms.vuint32(1000022, 2000012, 2000014,
                                                2000016, 1000039, 5000039,
                                                4000012, 9900012, 9900014,
                                               9900016, 39),
               checkForStatus3Mother = cms.PSet(
                 motherIds=cms.vuint32(13),
                 excludeSameFlavourDaughterOnly=cms.bool(True)),
               reverseAllSelections  = cms.bool(True)  
)

from RecoJets.JetProducers.GenJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *

# Gen Particles Status 3 Parameters: we sobstitute the Gen Particles Selector to the standard one
GenJetParameters.src = cms.InputTag("GenParticlesStatus3NoMuMufromZ")


# We create a class of ak5GenJets Status 3 without Muons from Z
ak5GenJetsStatus3mod = cms.EDProducer("FastjetJetProducer",
     GenJetParameters,
     AnomalousCellParameters,
     jetAlgorithm = cms.string("AntiKt"),
     rParam       = cms.double(0.5)
     )

# We apply some cuts to the ak5GenJet Status 3  Collection
selectedGenJetsStatus3NoMuMufromZ = cms.EDFilter('GenJetSelector',
   src = cms.InputTag('ak5GenJetsStatus3mod'),
   cut = cms.string('pt > 15. & abs(eta) < 3. & nConstituents > 0')
)

genjetsStatus3NoMuMufromZSequence = cms.Sequence( (GenParticlesStatus3NoMuMufromZ * ak5GenJetsStatus3mod) * selectedGenJetsStatus3NoMuMufromZ)

################# Vecchia

# We apply some cuts to the Gen Jets Collection
selectedGenJetsOld = cms.EDFilter('GenJetSelector',
   src = cms.InputTag('ak5GenJets'),
   cut = cms.string('pt > 15. & abs(eta) < 3. & nConstituents > 0')
)

genjetsOldSequence = cms.Sequence(selectedGenJetsOld)

#################################
# Sequences and EventContent
#################################

genjetsSequence = cms.Sequence( genjetsNoMuMufromZSequence + genjetsStatus3NoMuMufromZSequence + genjetsOldSequence)

jetgenEventContent = [
  'keep *_selectedGenJetsOld_*_*', 
  'keep *_selectedGenJetsNoMuMufromZ_*_*',
  'keep *_selectedGenJetsStatus3NoMuMufromZ_*_*'
]
