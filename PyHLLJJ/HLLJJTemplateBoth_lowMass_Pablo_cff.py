import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import *
import copy
from HiggsAna.HLLJJCommon.cmdLine import options
options.parseArguments()
print options.selection 

runOnMC      = True
runOld5XGT = False
runOnFastSim = False

process = cms.Process("CMG")

###########
# Options #
###########
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents))
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200))

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
                                        #SkipEvent = cms.untracked.vstring('ProductNotFound')
                                        )
########################################################
## Conditions 
########################################################

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

process.GlobalTag.globaltag = getGlobalTag( runOnMC, runOld5XGT )
print 'Global tag       : ', process.GlobalTag.globaltag

###########
# Input   #
###########
#fullname  = "HiggsAna.HLLJJCommon.datasets." + options.infile
####fullname  = "HiggsAna.HLLJJCommon.datasets.summer12_GluGluToHToZZ4lTEST525_cff" 
#print 'Importing dataset from '
#print fullname
#process.load(fullname)
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'CMS',
    '/VBF_HToZZTo2L2Q_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM'
   )

###################BEGIN CMG SPECIFIC
from CMGTools.Common.Tools.cmsswRelease import isNewerThan

sep_line = '-'*67
print sep_line
print 'CMG PAT-tuplizer, contact Colin before any modification'
print sep_line

print sep_line
print process.source.fileNames
print sep_line 

print 'loading the main CMG sequence'

process.load('CMGTools.Common.PAT.PATCMG_cff')

if runOnMC is False:
    # removing MC stuff
    print 'removing MC stuff, as we are running on Data'

    process.patElectrons.addGenMatch = False
    process.makePatElectrons.remove( process.electronMatch )
    
    process.patMuons.addGenMatch = False
    process.makePatMuons.remove( process.muonMatch )
    
    process.PATCMGSequence.remove( process.PATCMGGenSequence )
    process.PATCMGJetSequence.remove( process.jetMCSequence )
    process.PATCMGJetSequence.remove( process.patJetFlavourId )
    process.patJets.addGenJetMatch = False
    process.patJets.addGenPartonMatch = False

    if isNewerThan('CMSSW_5_2_0'):
        process.PATCMGJetSequenceCHSpruned.remove( process.jetMCSequenceCHSpruned )
        process.patJetsCHSpruned.addGenJetMatch = False
        process.patJetsCHSpruned.addGenPartonMatch = False

    process.PATCMGTauSequence.remove( process.tauGenJets )
    process.PATCMGTauSequence.remove( process.tauGenJetsSelectorAllHadrons )
    process.PATCMGTauSequence.remove( process.tauGenJetMatch )
    process.PATCMGTauSequence.remove( process.tauMatch )
    process.patTaus.addGenJetMatch = False
    process.patTaus.addGenMatch = False

    process.patMETs.addGenMET = False 
    process.patMETsRaw.addGenMET = False 

    # adding L2L3Residual corrections
    process.patJetCorrFactors.levels.append('L2L3Residual')
    if isNewerThan('CMSSW_5_2_0'):
        process.patJetCorrFactorsCHSpruned.levels.append('L2L3Residual')


print 'cloning the jet sequence to build PU chs jets'

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
process.PATCMGJetCHSSequence = cloneProcessingSnippet(process, process.PATCMGJetSequence, 'CHS')
process.PATCMGJetCHSSequence.insert( 0, process.ak5PFJetsCHS )
from CMGTools.Common.Tools.visitorUtils import replaceSrc
replaceSrc( process.PATCMGJetCHSSequence, 'ak5PFJets', 'ak5PFJetsCHS')
replaceSrc( process.PATCMGJetCHSSequence, 'particleFlow', 'pfNoPileUp')
process.patJetCorrFactorsCHS.payload = 'AK5PFchs'
process.selectedPatJetsCHS.cut = 'pt()>10'


########################################################
## Path definition
########################################################
process.PATCMGSequence.remove(process.PATCMGTauSequence)


process.dump = cms.EDAnalyzer('EventContentAnalyzer')

process.load('CMGTools.Common.PAT.addFilterPaths_cff')
process.p = cms.Path(
    process.prePathCounter + 
    process.PATCMGSequence +
    process.PATCMGJetCHSSequence 
    )

process.p += process.postPathCounter

pathsplit = str(process.p).split("+")
for module in pathsplit:
  print module

########################################################
## Below, stuff that you probably don't want to modify
########################################################



## Geometry and Detector Conditions (needed for a few patTuple production steps)

from CMGTools.Common.PAT.patCMGSchedule_cff import getSchedule
process.schedule = getSchedule(process, runOnMC, runOnFastSim)

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 10

## Options and Output Report
#process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

print sep_line

print 'Fastjet instances (dominating our processing time...):'
from CMGTools.Common.Tools.visitorUtils import SeqVisitor
v = SeqVisitor('FastjetJetProducer')
process.p.visit(v)

print sep_line

print 'starting CMSSW'

if not runOnMC and isNewerThan('CMSSW_5_2_0'):
    process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual")

####################END OF CMG SPECIFIC

###################
# JSON Filtering  #
###################
#only do this for data
if options.mcordata == "DATA" and options.json!="" :
    import PhysicsTools.PythonAnalysis.LumiList as LumiList
    import FWCore.ParameterSet.Types as CfgTypes
    myLumis = LumiList.LumiList(filename = options.json).getCMSSWString().split(',')
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    process.source.lumisToProcess.extend(myLumis)


#################
# Event filter  #
#################

process.load('HLTrigger.HLTfilters.triggerResultsFilter_cfi')
process.triggerResultsFilter.triggerConditions = cms.vstring('HLT_primaryVertexFilterPath',
                                                  'HLT_noscrapingFilterPath',
                                                  'HLT_hcalLaserEventFilterPath',
                                                  'HLT_HBHENoiseFilterPath',
                                                  #'HLT_totalKinematicsFilterPath' #only for Madgraph MC
                                                  )
process.triggerResultsFilter.l1tResults = ''                                                  

#process.badEventFilter = cms.EDFilter("HLTHighLevel",
#                                     TriggerResultsTag =
#                                      cms.InputTag("TriggerResults"), #,"","CMG"),
#                                      HLTPaths =
#                                      cms.vstring('primaryVertexFilterPath',
#                                                  'noscrapingFilterPath',
#                                                  'hcalLaserEventFilterPath',
#                                                  'HBHENoiseFilterPath',
#                                                  #'totalKinematicsFilterPath' #only for Madgraph MC
#                                                  ),
#                                      eventSetupPathsKey = cms.string(''),
#                                      # how to deal with multiple triggers: True (OR) accept if ANY is true, False
#                                      #(AND) accept if ALL are true
#                                      andOr = cms.bool(False), 
#                                      throw = cms.bool(True)    # throw exception on unknown path names
#                                      ) 

###########
# Output  #
###########
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(options.output),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_*_*_CMG'
                                                                      )
                               )
#if options.selection != "none":
process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring() )

#if options.selection == "full":
#    if options.lepton == "both" or options.lepton == "ele":
#        process.out.SelectEvents.SelectEvents.append("cmgHZZEE")
#        process.out.SelectEvents.SelectEvents.append("cmgHZZEESideband")
#    if options.lepton == "both" or options.lepton == "mu":
#        process.out.SelectEvents.SelectEvents.append("cmgHZZMM")
#        process.out.SelectEvents.SelectEvents.append("cmgHZZMMSideband")
#if options.selection == "presel":
if options.lepton == "both" or options.lepton == "ele":
  process.out.SelectEvents.SelectEvents.append("preselEle")
if options.lepton == "both" or options.lepton == "mu":
  process.out.SelectEvents.SelectEvents.append("preselMu")
    
#print process.out.SelectEvents.SelectEvents

#process.outpath = cms.EndPath(process.out)
#from CMGTools.Common.PAT.patCMGSchedule_cff import getSchedule
#process.schedule = getSchedule(process, runOnMC, runOnFastSim)
#process.schedule.append( process.outpath )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.secondaryOutput))

for content in options.content:
    
    if content == "all":
        process.out.outputCommands.append( 'keep *' )

    if content == "ele":
        process.out.outputCommands.append( 'keep *_*PatElectrons*_*_*' ) # we keep the pat::Muon

    if content == "mu":
        process.out.outputCommands.append( 'keep *_triggeredPatMuons_*_*' ) 
        process.out.outputCommands.append( 'keep *_selectedPatMuons_*_*' ) 

    if content == "gen":
        process.out.outputCommands.append( 'keep *_genParticles_*_*' ) 
        process.out.outputCommands.append( 'keep *_ak5GenJets_*_*' )
        process.out.outputCommands.append( 'keep GenEventInfoProduct*_*_*_*' )


    if content == "jets":
        process.out.outputCommands.append( 'keep *_custom*Jets*_*_*' )
        process.out.outputCommands.append( 'keep patJets_selected*_*_*' ) 
        process.out.outputCommands.append( 'keep *_*_rho_*' )
        process.out.outputCommands.append( 'keep *_patMETs*_*_*' )
        process.out.outputCommands.append( 'keep *_addPileupInfo_*_*')

    if content == "trigger":
        process.out.outputCommands.append( 'keep *_TriggerResults_*_*' )
        process.out.outputCommands.append( 'keep *_*PrimaryVertices_*_*' )

################
# Ele Sequence #
################
process.load('CMGTools.Common.selections.cutSummaryMuon_cfi')
process.load('HiggsAna.HEEJJ.electron_cff') 
process.load('HiggsAna.HEEJJ.diElectron_cff')
process.zeesummary = process.cutSummaryMuon.clone(inputCollection = cms.InputTag("cmgDiElectron2L2Q"))
process.load('HiggsAna.HEEJJ.skims.selEventsElectron_cfi')
process.load('HiggsAna.HEEJJ.skims.selEventsZ_cff')
#process.load('HiggsAna.HLLJJCommon.histograms.recoLeptonHistos_cff')
#process.recoEleHistos = process.recoLeptonHistos.clone(src="electronPresel")

process.analysisSequenceElectrons = cms.Sequence(
    process.eleSequence2L2Q +
    process.selectedElectronSequence2L2Q +
    ##process.recoEleHistosHLLJJ +
    process.diElectronSequence2L2Q +
    process.selectedZSequence
    )


#test
#process.test = cms.Path(process.analysisSequenceElectrons)
#process.schedule = cms.Schedule(process.p)
#process.schedule.append(process.test)
#process.schedule.append(process.outpath)

#############
# MC Hack   #
#############
if options.mcordata == "DATA":
    process.genParticles = cms.EDProducer("DummyGenProducer")
    process.eleSequence2L2Q.insert(0,process.genParticles)
else:
    process.load("HiggsAna.HLLJJCommon.gen_cff")
    process.eleSequence2L2Q.insert(0,process.genHistoSequence)

##############
# PU weights #
##############
process.load('HiggsAna.HLLJJCommon.PUweights_cff')
process.eleSequence2L2Q.insert(0,process.PUseq)

##############
# Rho 2.5    #
##############
# not needed in 2012, rho for iso calculated in PAT-tuple
#process.load('RecoJets.Configuration.RecoPFJets_cff')
#process.kt6PFJetsForIsolation = process.kt6PFJets.clone(Rho_EtaMax = 2.5)
#process.RhoPath=cms.Path(process.kt6PFJetsForIsolation)

################
# Mu  Sequence #
################
process.load('CMGTools.Common.selections.cutSummaryMuon_cfi')  #needed?
process.load('HiggsAna.HMMJJ.muon_cff')
process.load('HiggsAna.HMMJJ.diMuon_cff')
process.zmumusummary = process.cutSummaryMuon.clone(inputCollection = cms.InputTag("cmgDiMuon"))
process.load('HiggsAna.HMMJJ.skims.selEventsMuon_cff')
process.load('HiggsAna.HMMJJ.skims.selEventsZ_cff')
process.load('HiggsAna.HMMJJ.higgs_cff')
#process.load('HiggsAna.HLLJJCommon.histograms.recoLeptonHistos_cff')
#process.recoMuHistos = process.recoLeptonHistos.clone(src = "muonPresel")
process.muonSequence2L2Q.insert(0,process.PUseq)

process.analysisSequenceMuons = cms.Sequence(
    process.muonSequence2L2Q
    + process.selectedMuonSequence2L2Q
#    + process.recoMuHistos
    + process.diMuonSequence2L2Q
    + process.selectedZSequence
)

################
# Jet Sequence #
################
process.load('HiggsAna.HLLJJCommon.jet_cff')
process.load('HiggsAna.HLLJJCommon.factories.cmgDiJet_cfi')
process.load('HiggsAna.HLLJJCommon.factories.cmgDiJetKinFit_cfi')
process.load('HiggsAna.HLLJJCommon.skims.selEventsPFJet_cff')
process.load('HiggsAna.HLLJJCommon.skims.selEventsZjj_cff')

process.analysisSequenceJets = cms.Sequence(
    process.jetSequence +
    process.selectedJetSequence +
    process.diJetSequence +
    process.selectedZjjSequence+
    process.cmgDiJetKinFit
    )

########################################
# Higgs Sequences (preselection level) #
########################################
process.load('HiggsAna.HEEJJ.higgs_cff')
cloneProcessingSnippet(process,process.higgsSequence, "Ele")
process.load('HiggsAna.HMMJJ.higgs_cff')
cloneProcessingSnippet(process,process.higgsSequence, "Mu")


process.analysisSequenceHZZEE = cms.Sequence(
    process.analysisSequenceElectrons +
#    process.analysisSequenceMuons +
    process.analysisSequenceJets +
    process.higgsSequenceEle
    )

process.analysisSequenceHZZMM = cms.Sequence(
#    process.analysisSequenceElectrons +
    process.analysisSequenceMuons +
    process.analysisSequenceJets +
    process.higgsSequenceMu
    )

process.badEventFilters = cms.Sequence(process.primaryVertexFilter+
                                       process.noscraping+
                                       process.hcalLaserEventFilter+
                                       process.HBHENoiseFilter)

#process.schedule = cms.Schedule(process.p)
#preselections need to have their own paths only if we want to select all events passing up to preselection level
##if options.selection == "presel":
##    process.preselEle = cms.Path(process.analysisSequenceHZZEE)
##    process.preselMu = cms.Path(process.analysisSequenceHZZMM)
##    process.schedule.append(process.preselEle)
##    process.schedule.append(process.preselMu)
if options.selection == "presel":
  #muons
  process.cmgDiMuonSel2L2Q.filter=True 
  process.ZmmCand.filter=True
  process.selectedHiggsCandFilter.minNumber=1
  process.selectedZmmCandFilter.minNumber=1
  process.selectedMuonCandFilter.minNumber=2
  #electrons
  process.cmgDiElectronSel2L2Q.filter=True
  process.ZeeCand.filter=True
  process.selectedElectronCandFilter.minNumber = 2
  process.selectedHiggsCandFilter.minNumber = 1
  process.selectedZeeCandFilter.minNumber = 1
  #jets
  process.selectedJetCandFilter.minNumber=2
  process.selectedJetCandFilter.filter=True
  process.jetCountFilter.minNumber=2
  process.selectedZjjCandFilter.minNumber=1

process.preselEle = cms.Path(process.badEventFilters+process.analysisSequenceHZZEE)
process.preselMu = cms.Path(process.badEventFilters+process.analysisSequenceHZZMM)
process.schedule.append(process.preselEle)
process.schedule.append(process.preselMu)


process.OutPath = cms.EndPath(process.triggerResultsFilter*process.out)
process.schedule.append(process.OutPath)    

##############
## Low mass ##
##############

# gen level
process.genHCompositZll.cuts.zmumu.zmumu.leg1_kinematics.pt = cms.string('(leg1().pt() > 10 && leg2().pt() > 20)||(leg1().pt() > 20 && leg2().pt() > 10)')
process.genHCompositZll.cuts.zmumu.zmumu.mass = cms.string('mass() >= 12 ') #&& mass() <= 75')
process.genHCompositZll.cuts.zee.zee.pt = cms.string('(leg1().pt() > 5 && leg2().pt() > 10)||(leg1().pt() > 10 && leg2().pt() > 5)')
process.genHCompositZll.cuts.zee.zee.mass = cms.string('mass() >= 12 ') #&& mass() <= 75')
process.genHCompositeLeptons.cuts.eKinematics.pt = cms.string('pt() > 5')
process.genHCompositeLeptons.cuts.muKinematics.pt = cms.string('pt() > 5')

# reco level
process.cmgElectron2L2Q.cuts.kinematics.pt = cms.string('pt() > 5')
process.cmgDiElectron2L2Q.cuts.zee_kinematics.mass = cms.string('mass() >= 12')
process.cmgDiElectron2L2Q.cuts.zee_kinematics.pt = cms.string('(leg1().pt() > 5 && leg2().pt() > 10)||(leg1().pt() > 10 && leg2().pt() > 5)')
process.cmgMuon2L2Q.cuts.kinematics.pt = cms.string('pt() > 5')
process.cmgDiMuon2L2Q.cuts.zmumu.mass = cms.string('mass() >= 12 ')
process.cmgDiMuon2L2Q.cuts.zmumu.leg1_kinematics = cms.string('(leg1().pt() > 5 && leg2().pt() > 10)||(leg1().pt() > 10 && leg2().pt() > 5)')

process.eKinematics.pt = cms.string('pt() > 5')
process.zee_kinematics.pt = cms.string('(leg1().pt() > 5 && leg2().pt() > 10)||(leg1().pt() > 10 && leg2().pt() > 5)')
process.zee_kinematics.mass = cms.string('mass() >= 12 ')
process.zee.leg1_kinematics.pt = cms.string('pt() > 5')
process.zee.mass = cms.string('mass() >= 12 ')

process.muKinematics.pt = cms.string('pt() > 5')
process.zmumu.leg1_kinematics.pt = cms.string('(leg1().pt() > 5 && leg2().pt() > 10)||(leg1().pt() > 10 && leg2().pt() > 5)')
process.zmumu.mass = cms.string('mass() >= 12 ')


########################################
# Final Selection Sequence             #
########################################
###
####default is electrons
###process.load("HiggsAna.HLLJJCommon.FinalSelection_cff")
###cloneProcessingSnippet(process,process.cmgSeq, "Ele")
###cloneProcessingSnippet(process,process.cmg0Seqtag, "Ele")
###cloneProcessingSnippet(process,process.cmg1Seqtag, "Ele")
###cloneProcessingSnippet(process,process.cmg2Seqtag, "Ele")
###    
####muons need filter types + inputs adjusted
###cloneProcessingSnippet(process,process.cmgSeq, "Mu")
###massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","CmgDiElectronDiJetHiggsSelector","CmgDiMuonDiJetHiggsSelector")
###massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","DiElectronDiJetHiggsLDProducer","DiMuonDiJetHiggsLDProducer")
###massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","DiElectronDiJetHiggsQGLDSelector","DiMuonDiJetHiggsQGLDSelector")
###massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","CmgDiElectronDiJetHiggsMerger","CmgDiMuonDiJetHiggsMerger")
###massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","DiElectronDiJetHiggsBestCandidateSelector","DiMuonDiJetHiggsBestCandidateSelector")
###massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","DiElectronDiJetHiggsTagger","DiMuonDiJetHiggsTagger")
###massSearchReplaceAnyInputTag(process.cmgSeqMu,cms.InputTag("cmgHiggsSelKinFitEle"),cms.InputTag("cmgHiggsSelKinFitMu"))
###massSearchReplaceAnyInputTag(process.cmgSeqMu,cms.InputTag("cmgHiggsSelEle"),cms.InputTag("cmgHiggsSelMu"))
###cloneProcessingSnippet(process,process.cmg0Seqtag, "Mu")
###cloneProcessingSnippet(process,process.cmg1Seqtag, "Mu")
###cloneProcessingSnippet(process,process.cmg2Seqtag, "Mu")
###massSearchReplaceAnyInputTag(process.cmg0SeqtagMu,cms.InputTag("BestSelectorKinFitEle"),cms.InputTag("BestSelectorKinFitMu"),verbose=False,moduleLabelOnly=True)
###massSearchReplaceAnyInputTag(process.cmg1SeqtagMu,cms.InputTag("BestSelectorKinFitEle"),cms.InputTag("BestSelectorKinFitMu"),verbose=False,moduleLabelOnly=True)
###massSearchReplaceAnyInputTag(process.cmg2SeqtagMu,cms.InputTag("BestSelectorKinFitEle"),cms.InputTag("BestSelectorKinFitMu"),verbose=False,moduleLabelOnly=True)
###
###
####collect adjusted sequences into paths
###if options.lepton == "both" or options.lepton == "ele":
###    process.cmgHZZEE = cms.Path(process.badEventFilter+process.analysisSequenceHZZEE     + process.cmgSeqEle )
###    process.cmgHZZEE0Tag = cms.Path(process.badEventFilter+process.analysisSequenceHZZEE + process.cmgSeqEle + process.cmg0SeqtagEle)
###    process.cmgHZZEE1Tag = cms.Path(process.badEventFilter+process.analysisSequenceHZZEE + process.cmgSeqEle + process.cmg1SeqtagEle)
###    process.cmgHZZEE2Tag = cms.Path(process.badEventFilter+process.analysisSequenceHZZEE + process.cmgSeqEle + process.cmg2SeqtagEle)
###
###if options.lepton == "both" or options.lepton == "mu":
###    process.cmgHZZMM = cms.Path(process.badEventFilter+process.analysisSequenceHZZMM  + process.cmgSeqMu  )
###    process.cmgHZZMM0Tag = cms.Path(process.badEventFilter+process.analysisSequenceHZZMM + process.cmgSeqMu + process.cmg0SeqtagMu)
###    process.cmgHZZMM1Tag = cms.Path(process.badEventFilter+process.analysisSequenceHZZMM + process.cmgSeqMu + process.cmg1SeqtagMu)
###    process.cmgHZZMM2Tag = cms.Path(process.badEventFilter+process.analysisSequenceHZZMM + process.cmgSeqMu + process.cmg2SeqtagMu)
###
###
###
###########################################
#### Sideband Sequence                    #
###########################################
###
####default is electrons
###process.load("HiggsAna.HLLJJCommon.sideband_cff")
###cloneProcessingSnippet(process,process.sidebandSequence, "Ele")
###
####muons need filter types + inputs adjusted
###cloneProcessingSnippet(process,process.sidebandSequence, "Mu")
###massSearchReplaceParam(process.sidebandSequenceMu,"_TypedParameterizable__type","CmgDiElectronDiJetHiggsSelector","CmgDiMuonDiJetHiggsSelector")
###massSearchReplaceParam(process.sidebandSequenceMu,"_TypedParameterizable__type","DiElectronDiJetHiggsBestCandidateSelector","DiMuonDiJetHiggsBestCandidateSelector")
###massSearchReplaceAnyInputTag(process.sidebandSequenceMu,cms.InputTag("BestSelectorKinFitEle","primary"),cms.InputTag("BestSelectorKinFitMu","primary"))
###massSearchReplaceAnyInputTag(process.sidebandSequenceMu,cms.InputTag("VBFTaggerEle"),cms.InputTag("VBFTaggerMu"))
###massSearchReplaceAnyInputTag(process.sidebandSequenceMu,cms.InputTag("VBFTaggerSecondaryEle"),cms.InputTag("VBFTaggerSecondaryMu"))
###
####collect adjusted sequences into paths ---> final paths are defined here !!!!
###if options.lepton == "both" or options.lepton == "ele":
###    process.cmgHZZEESideband = cms.Path(process.badEventFilter+process.analysisSequenceHZZEE + process.cmgSeqEle + process.sidebandSequenceEle)
###    process.cmgHZZEESideband.replace(process.FinalFilterEle,~process.FinalFilterEle) # invert signal band selection
###    process.schedule.append(process.cmgHZZEESideband)
###
###if options.lepton == "both" or options.lepton == "mu":
###    process.cmgHZZMMSideband = cms.Path(process.badEventFilter+process.analysisSequenceHZZMM + process.cmgSeqMu + process.sidebandSequenceMu)
###    process.cmgHZZMMSideband.replace(process.FinalFilterMu,~process.FinalFilterMu) # invert signal band selection
###    process.schedule.append(process.cmgHZZMMSideband)
###
###
###
###########################################
#### Input switch                         #
###########################################
###if options.runinspain  :
###    process.cmgElectron.cuts.wp95c.iso='test_bit(sourcePtr().electronID(\"eidVBTFRel95\"),1)'
###    process.cmgElectron.cuts.wp95c.conversionVeto='test_bit(sourcePtr().electronID(\"eidVBTFRel95\"),2)'
###    
###    #remove duplicate jet-ide sequence
###    process.jetSequence.remove(process.puJetIdSequence)
###    process.jetSequence.remove(process.qglikelihood)
###
###    # replace input for MVA Id remove MC matching
###    process.cmgJetRaw.cfg.PFJetFactory.puMvas=cms.VInputTag()
###    process.cmgJetRaw.cfg.PFJetFactory.puIds=cms.VInputTag()
###
###    for seqname in process.sequences_():
###        #print seqname
###        #print process.sequences_()[seqname]
###        # replace input electrons 
###        massSearchReplaceAnyInputTag( process.sequences_()[seqname] , cms.InputTag("patElectronsWithTrigger"),cms.InputTag("selectedPatElectronsAK5"))
###        #replace muons
###        massSearchReplaceAnyInputTag( process.sequences_()[seqname] , cms.InputTag("patMuonsWithTrigger"),cms.InputTag("selectedPatMuonsAK5"))
###        #replace jets
###        massSearchReplaceAnyInputTag( process.sequences_()[seqname] , cms.InputTag("selectedPatJets"),cms.InputTag("customPFJetsNoPUSub"))
###        #replace MET
###        massSearchReplaceAnyInputTag( process.sequences_()[seqname] , cms.InputTag("patMETsRaw"),cms.InputTag("patMETsAK5"))
