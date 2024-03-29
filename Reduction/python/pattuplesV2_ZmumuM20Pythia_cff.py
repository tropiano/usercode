import FWCore.ParameterSet.Config as cms

source = cms.Source('PoolSource',
                            fileNames = cms.untracked.vstring(
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_10.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_11.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_12.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_13.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_14.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_15.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_16.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_17.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_18.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_19.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_1.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_20.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_21.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_22.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_23.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_24.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_25.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_26.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_27.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_28.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_29.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_2.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_30.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_31.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_32.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_33.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_34.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_35.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_36.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_37.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_38.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_39.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_3.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_40.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_41.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_42.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_43.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_44.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_45.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_46.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_47.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_48.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_49.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_4.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_50.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_51.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_52.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_53.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_54.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_55.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_56.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_57.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_58.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_59.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_5.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_60.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_61.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_62.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_63.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_65.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_67.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_68.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_69.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_6.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_7.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_8.root",
"file:/cms/data01/datasets/Zmumu_M20/Summer08_IDEAL_V9_reco-v2/PATFirenzeV2/result/Zmumu_M20_PATLayer1_sisCone_full_9.root"
)
)
