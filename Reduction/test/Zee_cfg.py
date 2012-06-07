from PhysicsTools.PatAlgos.patTemplate_cfg import *

################################################################################
####################
# Customization
####################

isMC = False       # True: MC, False: Data - To set in recZeePatAddOns_cff.py too!
MCSkimm = True    # True: MC Skimming (no signal!)
DataSkimm = True   # True: Data Skimming (standard configuration)

nameSource = [ #'file:input_DoubleElectron_Run2011A_May10ReReco.root'
               'file:input_DYJetsToLL_M50_Fall11.root'         
               #'file:input_DoubleMu_Run2011A_May10ReReco.root' # Input file name
             ] 

numEventsToRun = -1                  # (e.g. -1 to run on all events)

#MC
#nameOutput = 'DYJetsToLL_M_50_Fall11_Ele.root' # Output file name

#nameOutput = 'TTJets_Fall11_Ele.root'
#nameOutput = 'WJetsToLNu_Fall11_Ele.root'
#nameOutput = 'QCD_Pt20to30_EMEnriched_Fall11_Ele.root'
#nameOutput = 'QCD_Pt30to80_EMEnriched_Fall11_Ele.root'
#nameOutput = 'QCD_Pt80to170_EMEnriched_Fall11_Ele.root'
#nameOutput = 'QCD_Pt20to30_BCtoE_Fall11_Ele.root'
#nameOutput = 'QCD_Pt30to80_BCtoE_Fall11_Ele.root'
#nameOutput = 'QCD_Pt80to170_BCtoE_Fall11_Ele.root'
#nameOutput = 'WWJetsTo2L2Nu_Fall11_Ele.root'

#nameOutput = 'WZJetsTo3LNu_Fall11_Ele.root'
#nameOutput = 'ZZJetsTo4L_Fall11_Ele.root'  

#DATA
#nameOutput = 'DoubleElectron_Run2011A_May10ReReco_v1.root'
#nameOutput = 'DoubleElectron_Run2011A_PromptReco_v4.root'
#nameOutput = 'DoubleElectron_Run2011A_05Aug2011_v1.root'
#nameOutput = 'DoubleElectron_Run2011A_PromptReco_v6.root'
nameOutput = 'DoubleElectron_Run2011B_PromptReco_v1.root'
                 

DataSubset = False   # True: select a Data subset
Data_firstRun = 175873
Data_Range = '175873:1-175873:32'

GloTagMC = 'START42_V17::All'              # Global Tag MC
GloTagData = 'GR_R_42_V25::All'            # Global Tag Data

pathTrigger = '(path("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v*", 1, 1) || path("HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*", 1, 1) || path("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*", 1, 1)) || (path("HLT_L1_SingleEG5_v*", 1, 1) || path("HLT_L1_SingleEG12_v*", 1, 1)) || (path("HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_v*", 1, 1) || path("HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v*", 1, 1))' # selecting the trigger objects

#type("TriggerElectron") && 
#path("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v*", 1, 1)' # secondo Ele32 presente nei dati (SingleEle)
#path("HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_v*", 1, 1)' #Data
#path("HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v*", 1, 1)' #MC
#(path("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v*", 1, 1) || path("HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*", 1, 1) || path("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*", 1, 1)) && (path("HLT_L1_SingleEG5_v*", 1, 1) || path("HLT_L1_SingleEG12_v*", 1, 1)) && (path("HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_v*", 1, 1) || path("HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v*", 1, 1))' # selecting the trigger objects

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
#process.load("RecoMuon.DetLayers.muonDetLayerGeometry_cfi")

## B-tags
process.load("RecoBTag.Configuration.RecoBTag_cff")

## goodTracks
process.load("PhysicsTools.RecoAlgos.goodTracks_cfi")
#######################



##################################
## adding electron identification
##################################
process.load("RecoLocalCalo/EcalRecAlgos/EcalSeverityLevelESProducer_cfi") 

process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")

process.patElectrons.addElectronID     = cms.bool( True )
process.patElectrons.electronIDSources = cms.PSet(
    simpleEleId95relIso = cms.InputTag("simpleEleId95relIso"),
    simpleEleId90relIso = cms.InputTag("simpleEleId90relIso"),
    simpleEleId85relIso = cms.InputTag("simpleEleId85relIso"),
    simpleEleId80relIso = cms.InputTag("simpleEleId80relIso"),
    simpleEleId95cIso   = cms.InputTag("simpleEleId95cIso"),
    simpleEleId90cIso   = cms.InputTag("simpleEleId90cIso"),
    simpleEleId85cIso   = cms.InputTag("simpleEleId85cIso"),
    simpleEleId80cIso   = cms.InputTag("simpleEleId80cIso")
                                                   )

process.patElectronIDs = cms.Sequence( process.simpleEleIdSequence )

if isMC == True:
	process.makePatElectrons = cms.Sequence (process.patElectronIDs * process.patElectronIsolation * process.electronMatch * process.patElectrons)
else:
	process.makePatElectrons = cms.Sequence (process.patElectronIDs * process.patElectronIsolation * process.patElectrons)
##################################



######################
# Firenze modules
######################
if isMC == True: 
	process.load("Firenze.Reduction.genZeePatAddOns_cff") 
	process.load("Firenze.Reduction.genJetPatAddOns_cff") 
process.load("Firenze.Reduction.recZeePatAddOns_cff")
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

removeSpecificPATObjects(process, ['Photons', 'Muons', 'Taus'])

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
#process.kt6PFJets.Rho_EtaMax = cms.double(5.0)

## Turn-on the FastJet jet area calculation for your favorite algorithm
process.ak5PFJets.doAreaFastjet = True
#process.ak5PFJets.Rho_EtaMax = cms.double(5.0)

## Slimming the PFJet collection by raising the pt cut
process.ak5PFJets.jetPtMin = cms.double(15.0)

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
process.electronTriggerMatchHLTElectrons = cms.EDProducer(
   # matching in DeltaR, sorting by best DeltaR
  "PATTriggerMatcherDRLessByR" 
   # matcher input collections
, src     = cms.InputTag( 'cleanPatElectrons' )
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

#switchOnTriggerMatching( process, triggerMatchers = [ 'electronTriggerMatchHLTElectrons' ] )
switchOnTriggerMatchEmbedding( process , triggerMatchers = [ 'electronTriggerMatchHLTElectrons' ] )

# Switch to selected PAT objects in the trigger matching
removeCleaningFromTriggerMatching( process )

process.electronTriggerMatchHLTElectrons.src     = cms.InputTag( 'patElectrons' )
process.selectedPatElectronsTriggerMatch.src = cms.InputTag( 'patElectrons' )
process.selectedPatElectronsTriggerMatch.matches = cms.VInputTag('electronTriggerMatchHLTElectrons')
process.patTriggerSequence = cms.Sequence(process.patTrigger + process.electronTriggerMatchHLTElectrons + process.selectedPatElectronsTriggerMatch)

process.selectedElectrons.src = cms.InputTag('selectedPatElectronsTriggerMatch')

#######################



###########################
# patElectrons Preferences
###########################
#embedding objects 
#?process.patElectrons.embedCombinedElectron = cms.bool(True) 
#?process.patElectrons.embedStandAloneElectron = cms.bool(False)
#?process.patElectrons.embedPickyElectron = cms.bool(False)
#?process.patElectrons.embedTpfmsElectron = cms.bool(False)

process.patElectrons.embedPFCandidate = cms.bool(True)

# embedding of track info

process.patElectrons.embedTrack = cms.bool(True)

# embedding of electron MET corrections for caloMET
#?process.patElectrons.embedCaloMETElectronCorrs = cms.bool(False)

# embedding of electron MET corrections for tcMET
#?process.patElectrons.embedTcMETElectronCorrs = cms.bool(False)

# high level selections
process.patElectrons.embedHighLevelSelection = cms.bool(True)

###########################



########################## 
# Sequences and Paths
##########################
process.skimPatElectrons=cms.EDFilter("CandViewCountFilter",
  src = cms.InputTag("zeerec"),
  minNumber = cms.uint32(1),
  filter = cms.bool(True)
)


process.recosequence = cms.Sequence( (process.L1FastCorrection * process.patDefaultSequence) * (process.zeerecSequence + process.recjetsSequence) )
                                 
if isMC == True:
	process.gensequence = cms.Sequence(process.zeegenSequence + process.genjetsSequence)
	process.pattuples = cms.Sequence(process.gensequence + process.recosequence)
	if MCSkimm == True: 		
		process.p = cms.Path(process.pattuples * process.skimPatElectrons)
	else:
		process.p = cms.Path(process.pattuples)
else:
	process.pattuples = cms.Sequence(process.recosequence)
	if DataSkimm == True:	
		process.p = cms.Path(process.pattuples * process.skimPatElectrons)
	else:
		process.p = cms.Path(process.pattuples)
##########################            



##############################            
# Customize Event content
##############################
if isMC == True:
	from Firenze.Reduction.genZeePatAddOns_cff import zeegenEventContent 
	from Firenze.Reduction.genJetPatAddOns_cff import jetgenEventContent 
from Firenze.Reduction.recZeePatAddOns_cff import zeerecEventContent
from Firenze.Reduction.recJetPatAddOns_cff import jetrecEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.out.outputCommands=cms.untracked.vstring('drop *')
if isMC == True:
	process.out.outputCommands.extend(zeegenEventContent)
	process.out.outputCommands.extend(jetgenEventContent)
process.out.outputCommands.extend(zeerecEventContent)
process.out.outputCommands.extend(jetrecEventContent)
#process.out.outputCommands.extend(patTriggerEventContent)
process.out.outputCommands.extend(['keep *_offlinePrimaryVertices*_*_*', 'keep *_pat*METs*_*_*', 'keep *_patTriggerEvent_*_*', 'keep patTriggerPaths_patTrigger_*_*', 'keep *_goodTracks_*_*', 'keep double_kt6PFJets_rho_PAT*', 'keep PileupSummaryInfos_*_*_*','keep recoGenParticles_genParticles_*_*'])

process.out.dropMetaData = cms.untracked.string('DROPPED')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
process.out.fileName = nameOutput
process.options.wantSummary = False        ## (to suppress the long output at the end of the job)

print 'Current Event content is:'
print process.out.outputCommands
##############################
