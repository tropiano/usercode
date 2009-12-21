import FWCore.ParameterSet.Config as cms

# Standard kT jets parameters
# $Id: ExclusiveJetsParameters_cfi.py,v 1.1 2009/02/19 10:32:18 lenzip Exp $
ExclusiveJetParameters = cms.PSet(
  JetFinder = cms.string('kt_algorithm'),
  #possible Modes: inclusive, exclusive (dcut/njets), inclusive is default with PtMin [GeV]
  #setting either dcut or njets !=1 will switch to the respective exclusive mode
  dcut  = cms.double(-1),
  PtMin = cms.double(0.),
  
  #possible Strategies: "Best","N2Plain","N2Tiled","N2MinHeapTiled","NlnN","NlnNCam"
  Strategy = cms.string('Best'),
  UE_Subtraction = cms.string('no')
)
        
