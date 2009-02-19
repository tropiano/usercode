import FWCore.ParameterSet.Config as cms

from Firenze.JetUtilities.exclusive1CaloJets10_cff import *
from Firenze.JetUtilities.exclusive2CaloJets10_cff import *
from Firenze.JetUtilities.exclusive3CaloJets10_cff import *
from Firenze.JetUtilities.exclusive1GenJets10_cff import *
from Firenze.JetUtilities.exclusive2GenJets10_cff import *
from Firenze.JetUtilities.exclusive3GenJets10_cff import *

from Firenze.JetUtilities.exclusive1CaloJets6_cff import *
from Firenze.JetUtilities.exclusive2CaloJets6_cff import *
from Firenze.JetUtilities.exclusive3CaloJets6_cff import *
from Firenze.JetUtilities.exclusive1GenJets6_cff import *
from Firenze.JetUtilities.exclusive2GenJets6_cff import *
from Firenze.JetUtilities.exclusive3GenJets6_cff import *

allExclusiveJets10 = cms.Sequence(exclusive1GenJets10+exclusive2GenJets10+exclusive3GenJets10+exclusive1CaloJets10+exclusive2CaloJets10+exclusive3CaloJets10)
genExclusiveJets10 = cms.Sequence(exclusive1GenJets10+exclusive2GenJets10+exclusive3GenJets10)
caloExclusiveJets10 = cms.Sequence(exclusive1CaloJets10+exclusive2CaloJets10+exclusive3CaloJets10)

allExclusiveJets6 = cms.Sequence(exclusive1GenJets6+exclusive2GenJets6+exclusive3GenJets6+exclusive1CaloJets6+exclusive2CaloJets6+exclusive3CaloJets6)
genExclusiveJets6 = cms.Sequence(exclusive1GenJets6+exclusive2GenJets6+exclusive3GenJets6)
caloExclusiveJets6 = cms.Sequence(exclusive1CaloJets6+exclusive2CaloJets6+exclusive3CaloJets6)

allExclusiveJets = cms.Sequence(allExclusiveJets6+allExclusiveJets10)
allGenExclusiveJets = cms.Sequence(genExclusiveJets6+genExclusiveJets10)
allCaloExclusiveJets = cms.Sequence(caloExclusiveJets6+caloExclusiveJets10)
