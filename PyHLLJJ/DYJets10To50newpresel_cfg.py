import CMGTools.RootTools.fwlite.Config as cfg
from HiggsAna.PyHLLJJ.analhjjll_cff import *


DYJets10To50newpresel = cfg.MCComponent(
    name = 'DYJets10To50newpresel',
    files = [
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_0.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_1.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_10.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_11.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_12.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_13.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_14.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_15.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_16.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_17.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_18.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_19.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_2.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_20.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_21.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_22.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_23.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_24.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_25.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_26.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_27.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_28.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_29.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_3.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_30.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_31.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_32.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_33.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_34.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_35.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_36.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_37.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_38.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_39.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_4.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_40.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_41.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_42.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_43.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_44.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_5.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_6.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_7.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_8.root',
'root://eoscms//eos/cms/store/cmst3/user/lenzip/cmgTuple/2012/Summer12/newpresel/DYJets10to50/cmgTuple_9.root'
    ],
    
    xSection = 0.00114477 * 11050000, 
    nGenEvents = 6972564, # dummy 
    triggers = [],
    intLumi = 1000,
    effCorrFactor = 1 )

selectedComponents = [DYJets10To50newpresel]

DYJets10To50newpresel.splitFactor = 1
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
