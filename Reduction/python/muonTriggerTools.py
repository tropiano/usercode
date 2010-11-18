from FWCore.GuiBrowsers.ConfigToolBase import *
from PhysicsTools.PatAlgos.tools.trigTools import *

class MuonTriggerTools(ConfigToolBase):
  """Switchs on trigger embedding for muons in V+jets pattuples
  """
  _label='muontTriggerTools'
  _defaultParameters=dicttypes.SortedKeysDict()
  def __init__(self): 
    ConfigToolBase.__init__(self)
    self.addParameter(self._defaultParameters, 'muonCollection', self._defaultValue, 'muon collection to be used for trigger embedding', Type=cms.InputTag)
    self.addParameter(self._defaultParameters, 'label', self._defaultValue, 'label for collection after trigger embeffing', Type=str)
    self.addParameter(self._defaultParameters, 'substituteInto', self._defaultValue, 'label for collection to be substituted into', Type=str)
    self._parameters=copy.deepcopy(self._defaultParameters)
 
  def getDefaultParameters(self):
    return self._defaultParameters

  
  def __call__(self,process, muonCollection = None, label = None, substituteInto = None):
    if muonCollection is None:
      muonCollection=self._defaultParameters['muonCollection'].value
    if label is None:
      label=self._defaultParameters['label'].value
    if substituteInto is None:
      substituteInto = self._defaultParameters['substituteInto'].value
    self.setParameter('muonCollection',muonCollection)
    self.setParameter('label',label)
    self.setParameter('substituteInto',substituteInto)
    self.apply(process) 

  def toolCode(self, process):
    matchername=self._parameters['label'].value+'muonTriggerMatch'
    setattr(process, matchername, cms.EDFilter( "PATTriggerMatcherDRLessByR",
                                                src     = cms.InputTag( 'patMuons' ),
                                                matched = cms.InputTag( "patTrigger" ),
                                                andOr          = cms.bool( False ),
                                                filterIdsEnum  = cms.vstring( 'TriggerL1Mu' ),#'TriggerMuon', 'TriggerL1Mu' ), 
                                                filterIds      = cms.vint32( 0 ),
                                                filterLabels   = cms.vstring( '*' ),
                                                pathNames      = cms.vstring( 'HLT_Mu9', 'HLT_Mu11', 'HLT_L1Mu20' ),
                                                collectionTags = cms.vstring( '*' ),
                                                maxDeltaR = cms.double( 0.2 ),
                                                resolveAmbiguities    = cms.bool( True ),
                                                resolveByMatchQuality = cms.bool( False ) ) )         
    try:
      patTriggerMatcher=getattr(process, 'NEWpatTriggerMatcher')
    except:
      patTriggerMatcher=cms.Sequence(getattr(process, matchername))
      setattr(process, 'NEWpatTriggerMatcher', patTriggerMatcher)
    else:  
      patTriggerMatcher+=getattr(process, matchername)

    patTriggerEvent=getattr(process, 'patTriggerEvent')
    matchertag=cms.InputTag(matchername)
    #patTriggerEvent.patTriggerMatches.insert(matchertag)
    patTriggerEvent.patTriggerMatches.append(matchertag)
   
    embeddername=self._parameters['label'].value+'muonTriggerEmbedder'
    setattr(process, embeddername, cms.EDProducer( "PATTriggerMatchMuonEmbedder",
                                                    src     = cms.InputTag( 'patMuons' ),
                                                    matches = cms.VInputTag( matchername) ) )
    try:
      patTriggerMatchEmbedder=getattr(process, 'NEWpatTriggerMatchEmbedder')
    except:
      patTriggerMatchEmbedder=cms.Sequence(getattr(process,embeddername))
      setattr(process, 'NEWpatTriggerMatchEmbedder', patTriggerMatchEmbedder) 
    else:  
      patTriggerMatchEmbedder+=getattr(process,embeddername)

   
    selectedMuons=getattr(process, self._parameters['substituteInto'].value)
    selectedMuons.src=cms.InputTag(embeddername)


muonTriggerTools=MuonTriggerTools()
