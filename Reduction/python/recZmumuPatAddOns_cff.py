import FWCore.ParameterSet.Config as cms

##select muons according to these criteria
import PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi
selectedMuons = PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi.selectedPatMuons.clone()
selectedMuons.src = cms.InputTag("patMuons")
#selectedMuons.cut = cms.string("pt > 20. & abs(eta) < 2.4 & isGood('GlobalMuonPromptTight') & innerTrack().found()>=11 & abs(globalTrack().d0())<0.2 & (trackIso()+caloIso()+ecalIso())/pt<0.1")
selectedMuons.cut = cms.string("pt > 10. & abs(eta) < 3. & isGlobalMuon()")


#Z candidate combiner
zmumurec = cms.EDFilter('CandViewShallowCloneCombiner',
#zmumurec = cms.EDFilter('CandViewCombiner',
  decay = cms.string('selectedMuons@+ selectedMuons@-'),
  cut   = cms.string('50 < mass < 130'),
  name  = cms.string('Zmumu'),
  roles = cms.vstring('mu1', 'mu2')
)

zmumurecSameChargePlus = cms.EDFilter('CandViewShallowCloneCombiner',
#zmumurecSameChargePlus = cms.EDFilter('CandViewCombiner',
  decay = cms.string('selectedMuons@+ selectedMuons@+'),
  cut   = cms.string('50 < mass < 130'),
  name  = cms.string('Zmumu_samechargeplus'),
  roles = cms.vstring('mu1', 'mu2')
)

zmumurecSameChargeMinus = cms.EDFilter('CandViewShallowCloneCombiner',
#zmumurecSameChargeMinus = cms.EDFilter('CandViewCombiner',
  decay = cms.string('selectedMuons@- selectedMuons@-'),
  cut   = cms.string('50 < mass < 130'),
  name  = cms.string('Zmumu_samechargeminus'),
  roles = cms.vstring('mu1', 'mu2')
)

zmumurecSequence = cms.Sequence(selectedMuons*(zmumurec + zmumurecSameChargePlus + zmumurecSameChargeMinus))

zmumurecEventContent = [
  'keep *_selectedMuons_*_*',
  'keep *_zmumurec*_*_*'
]

