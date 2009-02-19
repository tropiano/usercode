import FWCore.ParameterSet.Config as cms

# Standard kT jets parameters
# $Id: KtJetParameters_cfi.py,v 1.2 2008/04/21 03:28:56 rpw Exp $
ExclusiveJetParameters = cms.PSet(
  JetFinder = cms.string('kt_algorithm'),
  #possible Modes: inclusive, exclusive (dcut/njets), inclusive is default with PtMin [GeV]
  #setting either dcut or njets !=1 will switch to the respective exclusive mode
  dcut  = cms.double(-1),
  PtMin = cms.double(0.),
  
  #possible Strategies: "Best","N2Plain","N2Tiled","N2MinHeapTiled","NlnN","NlnNCam"
  Strategy = cms.string('Best')
)
        
