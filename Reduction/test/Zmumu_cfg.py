from PhysicsTools.PatAlgos.patTemplate_cfg import *

################################################################################
####################
# Customization
####################

isMC = False        # True: MC, False: Data
MCSkimm = False    # True: MC Skimming (no signal!)
DataSkimm = True   # True: Data Skimming (standard configuration)

nameSource = [ #'file:input_DYJetsToLL_M50_Summer11.root'          # Input file name
               #'file:input_DYtoMuMu_M20_Fall11.root'
               'file:input_dati2011.root'
             ] 

numEventsToRun = 1000                       # (e.g. -1 to run on all events)
nameOutput = 'test_dati.root'
             #'output_DYJetsToLL_M50_Summer11.root'  # Output file name
             # 'output_dati2011.root'    
             # 'output_DYtoMuMu_M20_Fall11.root'        
               
DataSubset = False   # True: select a Data subset
Data_firstRun = 175873
Data_Range = '175873:1-175873:32'

GloTagMC = 'START42_V13::All'              # Global Tag MC
GloTagData = 'GR_R_42_V19::All'            # Global Tag Data

pathTrigger = 'type("TriggerMuon") && (path("HLT_DoubleMu6_v*") || path("HLT_DoubleMu7_v*") || path("HLT_Mu13_Mu8_v*") || path("HLT_Mu17_Mu8_v*"))' # selecting the trigger objects
#pathTrigger = 'type("TriggerMuon") && (path("HLT_DoubleMu6*") || path("HLT_DoubleMu7*") || path("HLT_Mu13_Mu8*"))'  

####################
################################################################################


# Module sequences rules 
# There are only two valid operators
#    the "*" or dependency operator, the operand to the right is dependent on the operand on the left (i.e. the right accesses data produced by the left)
#    the "+" or sequencing operator, the operand on the left is executed first, followed by the operand on the right, but they are not dependent on each other 

#######################
# from patTemplate
#######################
## MessageLogger
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

## Source
process.source.fileNames = nameSource

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(numEventsToRun))

## Data: Run and Block Range
if isMC == False:
	if DataSubset == True:
		process.source.firstRun = cms.untracked.uint32(Data_firstRun)
		process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange(Data_Range, '')

## Global Tag
if isMC == True: 
	process.GlobalTag.globaltag = cms.string(GloTagMC)
else: 
	process.GlobalTag.globaltag = cms.string(GloTagData)

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False), SkipEvent = cms.untracked.vstring('ProductNotFound') )
#######################



#######################
# Standard Modules
#######################
## Load standard Reco modules
process.load('Configuration.StandardSequences.Services_cff') 
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

## Generator informations
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
process.load("Configuration.StandardSequences.Generator_cff")

## Detector geometry: some of these needed for b-tag
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi")
process.load("RecoMuon.DetLayers.muonDetLayerGeometry_cfi")

## B-tags
process.load("RecoBTag.Configuration.RecoBTag_cff")

## goodTracks
process.load("PhysicsTools.RecoAlgos.goodTracks_cfi")
#######################



######################
# Firenze modules
######################
if isMC == True:
	process.load("Firenze.Reduction.genZmumuPatAddOns_cff")
	process.load("Firenze.Reduction.genJetPatAddOns_cff")
process.load("Firenze.Reduction.recZmumuPatAddOns_cff")
process.load("Firenze.Reduction.recJetPatAddOns_cff")
#process.goodTracks.cut = cut = cms.string('pt > 8') # non standard
######################



#####################
# Jets JEC & MET
#####################

from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *

# switch off MC matching in data
if isMC == False: 
	removeMCMatching(process, ['All'])

## add the PF MET
from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process, 'PF')
if isMC == False:
	process.patMETsPF.genMETSource=cms.InputTag('')

removeSpecificPATObjects(process, ['Photons', 'Electrons', 'Taus'])

# define PF jet corrections
correctionsL1L2L3 = ['L1FastJet','L2Relative','L3Absolute']
correctionsL1L2L3Residual = ['L1FastJet','L2Relative','L3Absolute', 'L2L3Residual']

if isMC == True : 
	correctionsWithL1 = correctionsL1L2L3
else : 
	correctionsWithL1 = correctionsL1L2L3Residual

## Import the JEC services
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

## Import the Jet RECO modules 
process.load('RecoJets.Configuration.RecoJets_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')

## Turn-on the FastJet density calculation 
process.kt6PFJets.doRhoFastjet = True
process.kt6PFJets.Rho_EtaMax = cms.double(5.0)

## Turn-on the FastJet jet area calculation for your favorite algorithm
process.ak5PFJets.doAreaFastjet = True
process.ak5PFJets.Rho_EtaMax = cms.double(5.0)

process.L1FastCorrection = cms.Sequence(process.kt6PFJets * process.ak5PFJets)

addJetCollection(process,cms.InputTag('ak5PFJets'),'AK5', 'PFL1corrected',
                 doJTA        = True,
                 doBTagging   = False,
                 jetCorrLabel = ('AK5PF', correctionsWithL1),
                 doType1MET   = True,
                 doL1Cleaning = True,
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = True,
                 jetIdLabel   = 'ak5'
		 )

process.selectedJetsL1Corrected = process.selectedJets.clone(src = cms.InputTag("patJetsAK5PFL1corrected"))
process.recjetsSequence += process.selectedJetsL1Corrected
#####################



#######################
# Trigger Matching
#######################
## add the trigger matching
process.muonTriggerMatchHLTMuons = cms.EDProducer(
   # matching in DeltaR, sorting by best DeltaR
  "PATTriggerMatcherDRLessByR" 
   # matcher input collections
, src     = cms.InputTag( 'cleanPatMuons' )
, matched = cms.InputTag( 'patTrigger' )
   # selections of trigger objects
, matchedCuts = cms.string( pathTrigger )
   # selection of matches
, maxDPtRel   = cms.double( 0.5 ) # no effect here
, maxDeltaR   = cms.double( 0.5 )
, maxDeltaEta = cms.double( 0.2 ) # no effect here
   # definition of matcher output
, resolveAmbiguities    = cms.bool( True )
, resolveByMatchQuality = cms.bool( True )
)

from PhysicsTools.PatAlgos.tools.coreTools import removeCleaning
removeCleaning( process )

from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process ) # This is optional and can be omitted.

#switchOnTriggerMatching( process, triggerMatchers = [ 'muonTriggerMatchHLTMuons' ] )
switchOnTriggerMatchEmbedding( process , triggerMatchers = [ 'muonTriggerMatchHLTMuons' ] )

# Switch to selected PAT objects in the trigger matching
removeCleaningFromTriggerMatching( process )

process.muonTriggerMatchHLTMuons.src     = cms.InputTag( 'patMuons' )
process.selectedPatMuonsTriggerMatch.src = cms.InputTag( 'patMuons' )
process.selectedPatMuonsTriggerMatch.matches = cms.VInputTag('muonTriggerMatchHLTMuons')
process.patTriggerSequence = cms.Sequence(process.patTrigger + process.muonTriggerMatchHLTMuons + process.selectedPatMuonsTriggerMatch)

process.selectedMuons.src = cms.InputTag('selectedPatMuonsTriggerMatch')

#######################



###########################
# patMuons Preferences
###########################
#embedding objects 
process.patMuons.embedCombinedMuon = cms.bool(True) 
process.patMuons.embedStandAloneMuon = cms.bool(False)
process.patMuons.embedPickyMuon = cms.bool(False)
process.patMuons.embedTpfmsMuon = cms.bool(False)
process.patMuons.embedPFCandidate = cms.bool(True)

# embeding of track info
process.patMuons.embedTrack = cms.bool(True)

# embedding of muon MET corrections for caloMET
process.patMuons.embedCaloMETMuonCorrs = cms.bool(False)

# embedding of muon MET corrections for tcMET
process.patMuons.embedTcMETMuonCorrs = cms.bool(False)

# high level selections
process.patMuons.embedHighLevelSelection = cms.bool(True)
###########################



########################## 
# Sequences and Paths
##########################
process.skimPatMuons=cms.EDFilter("CandViewCountFilter",
  src = cms.InputTag("zmumurec"),
  minNumber = cms.uint32(1),
  filter = cms.bool(True)
)


process.recosequence = cms.Sequence( (process.L1FastCorrection * process.patDefaultSequence) * (process.zmumurecSequence + process.recjetsSequence) )
                                 
if isMC == True:
	process.gensequence = cms.Sequence(process.genjetsSequence + process.zmumugenSequence)
	process.pattuples = cms.Sequence(process.gensequence + process.recosequence)
	if MCSkimm == True: 		
		process.p = cms.Path(process.pattuples * process.skimPatMuons)
	else:
		process.p = cms.Path(process.pattuples)
else:
	process.pattuples = cms.Sequence(process.recosequence)
	if DataSkimm == True:	
		process.p = cms.Path(process.pattuples * process.skimPatMuons)
	else:
		process.p = cms.Path(process.pattuples)
##########################            



##############################            
# Customize Event content
##############################
if isMC == True:
	from Firenze.Reduction.genZmumuPatAddOns_cff import zmumugenEventContent
	from Firenze.Reduction.genJetPatAddOns_cff import jetgenEventContent
from Firenze.Reduction.recZmumuPatAddOns_cff import zmumurecEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.out.outputCommands=cms.untracked.vstring('drop *')
if isMC == True:
	process.out.outputCommands.extend(zmumugenEventContent)
	process.out.outputCommands.extend(jetgenEventContent)
process.out.outputCommands.extend(zmumurecEventContent)
process.out.outputCommands.extend(jetrecEventContent)
#process.out.outputCommands.extend(patTriggerEventContent)
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep *_pat*METs*_*_*', 'keep *_patTriggerEvent_*_*', 'keep patTriggerPaths_patTrigger_*_*', 'keep *_goodTracks_*_*', 'keep double_kt6PFJets_rho_PAT*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = nameOutput
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
##############################
