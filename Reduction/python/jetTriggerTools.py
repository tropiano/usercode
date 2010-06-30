from FWCore.GuiBrowsers.ConfigToolBase import *
from PhysicsTools.PatAlgos.tools.trigTools import *

class JetTriggerTools(ConfigToolBase):
  """Switchs on trigger embedding for jets in V+jets pattuples
  """
  _label='jetTriggerTools'
  _defaultParameters=dicttypes.SortedKeysDict()
  def __init__(self): 
    ConfigToolBase.__init__(self)
    self.addParameter(self._defaultParameters, 'jetCollection', self._defaultValue, 'jet collection to be used for trigger embedding', Type=cms.InputTag)
    self.addParameter(self._defaultParameters, 'label', self._defaultValue, 'label for collection after trigger embeffing', Type=str)
    self.addParameter(self._defaultParameters, 'substituteInto', self._defaultValue, 'label for collection to be substituted into', Type=str)
    self._parameters=copy.deepcopy(self._defaultParameters)
 
  def getDefaultParameters(self):
    return self._defaultParameters

  
  def __call__(self,process, jetCollection = None, label = None, substituteInto = None):
    if jetCollection is None:
      jetCollection=self._defaultParameters['jetCollection'].value
    if label is None:
      label=self._defaultParameters['label'].value
    if substituteInto is None:
      substituteInto = self._defaultParameters['substituteInto'].value
    self.setParameter('jetCollection',jetCollection)
    self.setParameter('label',label)
    self.setParameter('substituteInto',substituteInto)
    self.apply(process) 

  def toolCode(self, process):
    matchername=self._parameters['label'].value+'jetTriggerMatch'
    setattr(process, matchername, cms.EDFilter( "PATTriggerMatcherDRLessByR",
                                                      src     = self._parameters['jetCollection'].value,
                                                      matched = cms.InputTag( "patTrigger" ),
                                                      andOr          = cms.bool( False ),
                                                      filterIdsEnum  = cms.vstring( 'TriggerJet' ),
                                                      filterIds      = cms.vint32( 0 ),
                                                      filterLabels   = cms.vstring( '*' ),
                                                      pathNames      = cms.vstring( 'HLT_Jet15U', 'HLT_Jet30U' ),
                                                      collectionTags = cms.vstring( '*' ),
                                                      maxDeltaR = cms.double( 0.2 ),
                                                      resolveAmbiguities    = cms.bool( True ),
                                                      resolveByMatchQuality = cms.bool( False )) )

    try:
      patTriggerMatcher=getattr(process, 'NEWpatTriggerMatcher')
    except:
      patTriggerMatcher=cms.Sequence(getattr(process, matchername))
      setattr(process,'NEWpatTriggerMatcher', patTriggerMatcher)
    else:  
      patTriggerMatcher+=getattr(process, matchername)

    patTriggerEvent=getattr(process, 'patTriggerEvent')
    matchertag=cms.InputTag(matchername)
    #patTriggerEvent.patTriggerMatches.insert(matchertag)
    patTriggerEvent.patTriggerMatches.append(matchertag)
   
    embeddername=self._parameters['label'].value+'jetTriggerEmbedder'
    setattr(process, embeddername, cms.EDProducer( "PATTriggerMatchJetEmbedder", 
                                                    src     = self._parameters['jetCollection'].value,
                                                    matches = cms.VInputTag( matchername )) ) 
    try:
      patTriggerMatchEmbedder=getattr(process, 'NEWpatTriggerMatchEmbedder')
    except:
      patTriggerMatchEmbedder=cms.Sequence(getattr(process,embeddername))
      setattr(process,'NEWpatTriggerMatchEmbedder',patTriggerMatchEmbedder)
    else:  
      patTriggerMatchEmbedder+=getattr(process,embeddername)

   
    selectedJets=getattr(process, self._parameters['substituteInto'].value)
    selectedJets.src=cms.InputTag(embeddername)

#process.selectedJets.src=cms.InputTag('selectedLayer1JetsTriggerMatch')
#process.selectedPFJets.src=cms.InputTag('selectedLayer1PFJetsTriggerMatch')
#process.selectedJPTJets.src=cms.InputTag('selectedLayer1JPTJetsTriggerMatch')

jetTriggerTools=JetTriggerTools()
