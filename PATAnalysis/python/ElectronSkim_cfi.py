import FWCore.ParameterSet.Config as cms

ElectronSkim = cms.EDFilter("ElectronSkim",                          
		           masscut = cms.double(20)    		                    
)


ElectronSkimSequence = cms.Sequence(ElectronSkim)
