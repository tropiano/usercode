#ifndef zeejets_h
#define zeejets_h

#include <iostream>
#include <fstream>
#include <string>

#include "TFile.h"
#include "TParameter.h"
#include "TDSet.h"
#include "TSystem.h"

using namespace std;

/*

-9. TEST

0. All dati2011 			(skimmed)
1. Run2011A_May10ReReco_v1 		(skimmed)
2. Run2011A_PromptReco_v4 		(skimmed)
3. Run2011A_05Aug2011_v1 		(skimmed)
4. Run2011A_PromptReco_v6 		(skimmed)
5. Run2011B_PromptReco_v1		(skimmed)

-1. All
11. DYJetsToLL_M50_All 			(not skimmed)
12. TTJets_All 				(skimmed)
13. WJetsToLNu_All 			(skimmed)
14. QCD_EMEnriched_Pt20to30_All		(skimmed)
15. QCD_EMEnriched_Pt30to80_All 	(skimmed)
16. QCD_EMEnriched_Pt80to170_All	(skimmed)
17. QCD_BCtoE_Pt20to30_All 		(skimmed)
18. QCD_BCtoE_Pt30to80_All 		(skimmed)
19. QCD_BCtoE_Pt80to170_All		(skimmed)
101. WWJetsTo2L2Nu_All 			(skimmed)
102. WZJetsTo3LNu_All 			(skimmed)
103. ZZJetsTo4L_All 			(skimmed)

-2. All Train
21. DYJetsToLL_M50_Train 		(not skimmed)
22. TTJets_Train 			(skimmed)
23. WJetsToLNu_Train	 		(skimmed)
24. QCD_EMEnriched_Pt20to30_Train	(skimmed)
25. QCD_EMEnriched_Pt30to80_Train	(skimmed)
26. QCD_EMEnriched_Pt80to170_Train	(skimmed)
27. QCD_BCtoE_Pt20to30_Train 		(skimmed)
28. QCD_BCtoE_Pt30to80_Train 		(skimmed)
29. QCD_BCtoE_Pt80to170_Train		(skimmed)
201. WWJetsTo2L2Nu_Train 		(skimmed)
202. WZJetsTo3LNu_Train 		(skimmed)
203. ZZJetsTo4L_Train 			(skimmed)

-3. All Sample
31. DYJetsToLL_M50_Sample 		(not skimmed)
32. TTJets_Sample 			(skimmed)
33. WJetsToLNu_Sample 			(skimmed)
34. QCD_EMEnriched_Pt20to30_Sample	(skimmed)
35. QCD_EMEnriched_Pt30to80_Sample	(skimmed)
36. QCD_EMEnriched_Pt80to170_Sample	(skimmed)
37. QCD_BCtoE_Pt20to30_Sample 		(skimmed)
38. QCD_BCtoE_Pt30to80_Sample 		(skimmed)
39. QCD_BCtoE_Pt80to170_Sample		(skimmed)
301. WWJetsTo2L2Nu_Sample 		(skimmed)
302. WZJetsTo3LNu_Sample 		(skimmed)
303. ZZJetsTo4L_Sample 			(skimmed)

*/

static string SampleName;

inline void name(int i){

	if(i==-9) SampleName="Test";

	else if(i==1) SampleName="Run2011A_May10ReReco_v1";
	else if(i==2) SampleName="Run2011A_PromptReco_v4";
	else if(i==3) SampleName="Run2011A_05Aug2011_v1";
	else if(i==4) SampleName="Run2011A_PromptReco_v6";
	else if(i==5) SampleName="Run2011B_PromptReco_v1";

	else if(i==11) SampleName="DYJetsToLL_M50_All";
	else if(i==12) SampleName="TTJets_All";
	else if(i==13) SampleName="WJetsToLNu_All";
	else if(i==14) SampleName="QCD_EMEnriched_Pt20to30_All";
	else if(i==15) SampleName="QCD_EMEnriched_Pt30to80_All";
	else if(i==16) SampleName="QCD_EMEnriched_Pt80to170_All";
	else if(i==17) SampleName="QCD_BCtoE_Pt20to30_All";
	else if(i==18) SampleName="QCD_BCtoE_Pt30to80_All";
	else if(i==19) SampleName="QCD_BCtoE_Pt80to170_All";
	else if(i==101) SampleName="WWJetsTo2L2Nu_All";
	else if(i==102) SampleName="WZJetsTo3LNu_All";
	else if(i==103) SampleName="ZZJetsTo4L_All";

	else if(i==21) SampleName="DYJetsToLL_M50_Train";
	else if(i==22) SampleName="TTJets_Train";
	else if(i==23) SampleName="WJetsToLNu_Train";
	else if(i==24) SampleName="QCD_EMEnriched_Pt20to30_Train";
	else if(i==25) SampleName="QCD_EMEnriched_Pt30to80_Train";
	else if(i==26) SampleName="QCD_EMEnriched_Pt80to170_Train";
	else if(i==27) SampleName="QCD_BCtoE_Pt20to30_Train";
	else if(i==28) SampleName="QCD_BCtoE_Pt30to80_Train";
	else if(i==29) SampleName="QCD_BCtoE_Pt80to170_Train";
	else if(i==201) SampleName="WWJetsTo2L2Nu_Train";
	else if(i==202) SampleName="WZJetsTo3LNu_Train";
	else if(i==203) SampleName="ZZJetsTo4L_Train";

	else if(i==31) SampleName="DYJetsToLL_M50_Sample";
	else if(i==32) SampleName="TTJets_Sample";
	else if(i==33) SampleName="WJetsToLNu_Sample";
	else if(i==34) SampleName="QCD_EMEnriched_Pt20to30_Sample";
	else if(i==35) SampleName="QCD_EMEnriched_Pt30to80_Sample";
	else if(i==36) SampleName="QCD_EMEnriched_Pt80to170_Sample";
	else if(i==37) SampleName="QCD_BCtoE_Pt20to30_Sample";
	else if(i==38) SampleName="QCD_BCtoE_Pt30to80_Sample";
	else if(i==39) SampleName="QCD_BCtoE_Pt80to170_Sample";
	else if(i==301) SampleName="WWJetsTo2L2Nu_Sample";
	else if(i==302) SampleName="WZJetsTo3LNu_Sample";
	else if(i==303) SampleName="ZZJetsTo4L_Sample";

	else SampleName="";

}


typedef struct{double _xsec; double _EventFilter; int _EventsPerFile; int _EventNumber;} par;

par ParStruct = {1., 1., 0, 0}; 


void Parameters(int i, par* Par){

	switch(i){

	//test
		case(-9):
			//TEST
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

	//data: total pixel luminosity (pixelLumiCalc.py) = 4.849365 /fb (skimmed)
		case(1):
			//Run2011A_May10ReReco_v1: pixel luminosity (pixelLumiCalc.py) = 212.372 /pb (skimmed)
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(2):
			//Run2011A_PromptReco_v4: pixel luminosity (pixelLumiCalc.py) =  927.398 /pb (skimmed)
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(3):
			//Run2011A_05Aug2011_v1: pixel luminosity (pixelLumiCalc.py) = 389.876 /pb (skimmed)
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(4):
			//Run2011A_PromptReco_v6: pixel luminosity (pixelLumiCalc.py) = 706.719 /pb (skimmed)
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(5):
			//Run2011B_PromptReco_v1: pixel luminosity (pixelLumiCalc.py) =  2.613 /fb (skimmed)
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

	//MC All
		case(11):
			//DYJetsToLL_M50_All:  _EventNumber = 35560196 events -> _EventNumber/_xsec = 11667 pb-1 (not skimmed)
			Par->_xsec = 3048.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(12):
			//TTJets_All:  _EventNumber = 52286646 events -> _EventNumber/_xsec = 316889 pb-1 (skimmed)
			Par->_xsec = 165.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 52286646;
			break;

		case(13):
			//WJetsToLNu_All:  _EventNumber = 65911904 events -> _EventNumber/_xsec = 2104 pb-1 (skimmed)
			Par->_xsec = 31314.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 65911904;
			break;

		case(14):
			//QCD_EMEnriched_Pt20to30_All:  _EventNumber = 34103622 events -> _EventNumber/(_xsec * _EventFilter) = 13.6 pb-1 (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 0.0106;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 34103622;
			break;

		case(15):
			//QCD_EMEnriched_Pt30to80_All:  _EventNumber = 41122923 events  -> _EventNumber/(_xsec * _EventFilter) = 11.3 pb-1 (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.061;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 41122923;
			break;

		case(16):
			//QCD_EMEnriched_Pt80to170_All:  _EventNumber = 8150672 events -> _EventNumber/(_xsec * _EventFilter) = 57.07 pb-1 (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.159;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 8150672;
			break;

		case(17):
			//QCD_BCtoE_Pt20to30_All:  _EventNumber = 2071133 events  -> _EventNumber/(_xsec * _EventFilter) = 14.87 pb-1 (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 5.9E-4;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 2071133;
			break;

		case(18):
			//QCD_BCtoE_Pt30to80_All:  _EventNumber = 1980033 events -> _EventNumber/(_xsec * _EventFilter) =  13.76 pb-1 (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.00242;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 1980033;
			break;

		case(19):
			//QCD_BCtoE_Pt80to170_All:  _EventNumber = 1082691 events  -> _EventNumber/(_xsec * _EventFilter) =  114.80 pb-1 (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.0105;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 1082691;
			break;

		case(101):
			//WWJetsTo2L2Nu_All:  _EventNumber = 1197558 events -> _EventNumber/_xsec = 27850 pb-1 (skimmed)
			Par->_xsec = 43.0;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 1197558;
			break;

		case(102):
			//WZJetsTo3LNu_All:   _EventNumber = 1221134 events -> _EventNumber/_xsec =  67095 pb-1 (skimmed)
			Par->_xsec = 18.2;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 1221134;
			break;

		case(103):
			//ZZJetsTo4L_All:   _EventNumber events = 888734 -> _EventNumber/_xsec = 150633 pb-1 (skimmed)
			Par->_xsec = 5.9;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 888734;
			break;



	//MC Train
		case(21):
			//DYJetsToLL_M50_Train:  _EventNumber = 17854257 events -> _EventNumber/_xsec =  5858 pb-1 (not skimmed)
			Par->_xsec = 3048.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(22):
			//TTJets_Train:  _EventNumber = 26177765 events -> _EventNumber/_xsec = 158653  pb-1 (skimmed)
			Par->_xsec = 165.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 26177765;
			break;

		case(23):
			//WJetsToLNu_Train:  _EventNumber = 35232820 events -> _EventNumber/_xsec = 1125 pb-1 (skimmed)
			Par->_xsec = 31314.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 35232820;
			break;

		case(24):
			//QCD_EMEnriched_Pt20to30_Train:  _EventNumber = 16770177 events -> _EventNumber/(_xsec * _EventFilter) = 6.70 pb-1 (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 0.0106;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 16770177;
			break;

		case(25):
			//QCD_EMEnriched_Pt30to80_Train:  _EventNumber = 19421503 events -> _EventNumber/(_xsec * _EventFilter) = 5.36 pb-1 (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.061;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 19421503;
			break;

		case(26):
			//QCD_EMEnriched_Pt80to170_Train:  _EventNumber = 4054660 events -> _EventNumber/(_xsec * _EventFilter) = 28.39 pb-1 (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.159;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 4054660;
			break;

		case(27):
			//QCD_BCtoE_Pt20to30_Train:  _EventNumber = 1050000 events -> _EventNumber/(_xsec * _EventFilter) = 7.54 pb-1 (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 5.9E-4;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 1050000;
			break;

		case(28):
			//QCD_BCtoE_Pt30to80_Train:  _EventNumber = 926293 events -> _EventNumber/(_xsec * _EventFilter) = 6.44 pb-1 (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.00242;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 926293;
			break;

		case(29):
			//QCD_BCtoE_Pt80to170_Train:  _EventNumber = 550000 events -> _EventNumber/(_xsec * _EventFilter) = 58.3 pb-1 (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.0105;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 550000;
			break;

		case(201):
			//WWJetsTo2L2Nu_Train:  _EventNumber = 574565 events -> _EventNumber/_xsec = 13362 pb-1 (skimmed)
			Par->_xsec = 43.0;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 574565;
			break;

		case(202):
			//WZJetsTo3LNu_Train:  _EventNumber = 600000 events -> _EventNumber/_xsec =  32967 pb-1 (skimmed)
			Par->_xsec = 18.2;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 600000;
			break;

		case(203):
			//ZZJetsTo4L_Train:  _EventNumber = 438734 events -> _EventNumber/_xsec = 74362 pb-1 (skimmed)
			Par->_xsec = 5.9;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 438734;
			break;


	//MC Sample
		case(31):
			//DYJetsToLL_M50_Sample:  _EventNumber = 17705939 events -> _EventNumber/_xsec = 5809 pb-1 (not skimmed)
			Par->_xsec = 3048.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(32):
			//TTJets_Sample:  _EventNumber = 26108881 events -> _EventNumber/_xsec = 158236 pb-1 (skimmed)
			Par->_xsec = 165.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 26108881;
			break;

		case(33):
			//WJetsToLNu_Sample:  _EventNumber = 30679084 events -> _EventNumber/_xsec = 980 pb-1 (skimmed)
			Par->_xsec = 31314.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 30679084;
			break;

		case(34):
			//QCD_EMEnriched_Pt20to30_Sample:  _EventNumber = 17333445 events -> _EventNumber/(_xsec * _EventFilter) =  6.93 pb-1 (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 0.0106;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 17333445;
			break;

		case(35):
			//QCD_EMEnriched_Pt30to80_Sample:  _EventNumber = 21701420 events -> _EventNumber/(_xsec * _EventFilter) =  5.99 pb-1 (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.061;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 21701420;
			break;

		case(36):
			//QCD_EMEnriched_Pt80to170_Sample:  _EventNumber = 4096012 events -> _EventNumber/(_xsec * _EventFilter) = 28.69 pb-1 (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.159;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 4096012;
			break;

		case(37):
			//QCD_BCtoE_Pt20to30_Sample:  _EventNumber = 1021133 events -> _EventNumber/(_xsec * _EventFilter) = 7.33 pb-1 (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 5.9E-4;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 1021133;
			break;

		case(38):
			//QCD_BCtoE_Pt30to80_Sample:  _EventNumber = 1053740 events -> _EventNumber/(_xsec * _EventFilter) = 7.33 pb-1 (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.00242;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 1053740;
			break;

		case(39):
			//QCD_BCtoE_Pt80to170_Sample:  _EventNumber = 532691 events -> _EventNumber/(_xsec * _EventFilter) = 56.48  pb-1 (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.0105;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 532691;
			break;

		case(301):
			//WWJetsTo2L2Nu_Sample:  _EventNumber = 622993 events -> _EventNumber/_xsec = 14488 pb-1 (skimmed)
			Par->_xsec = 43.0;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 622993;
			break;

		case(302):
			//WZJetsTo3LNu_Sample:  _EventNumber = 621134 events -> _EventNumber/_xsec = 34128 pb-1 (skimmed)
			Par->_xsec = 18.2;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 621134;
			break;

		case(303):
			//ZZJetsTo4L_Sample:  _EventNumber = 450000 events -> _EventNumber/_xsec = 76271 pb-1 (skimmed)
			Par->_xsec = 5.9;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 450000;
			break;


		default:
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;
	}
}


void makeCfg(string sample, string selections, string JetType, bool GEN, bool RECO, bool EFF, bool NTUPLE, int Acc, int Trg, int Qual, int Imp, int Iso, int ElID, int Conv, const char* path, const char* sourceList, const char *outFile, const char* Norm, int EventsPerFile, int EventNumber, int ProcEvents, double xsec, double targetLumi, string NtupleFill, int JECUnc, string JECUncFilePath){

	//cfg
	string cfgPath = path;
	cfgPath+= outFile;
	cfgPath+=".py";

	//source
	string Source = path;
	Source+=sourceList;

	//Ouput File Name
	string outputFile = outFile;
	outputFile+=".root";

	//ReportName
	string RepName = path;
	RepName+="Report_";
	RepName+=outFile;
	RepName+=".txt";

	ofstream cfg;
	cfg.open(cfgPath.c_str());

	cfg<<"import FWCore.ParameterSet.Config as cms"<<std::endl<<std::endl;

	cfg<<"process = cms.Process(\"test\")"<<std::endl<<std::endl;

	cfg<<"process.outputFile = cms.PSet(name = cms.string(\""<<outputFile.c_str()<<"\"))"<<std::endl<<std::endl;

	if(GEN){
		cfg<<"process.zeegen = cms.PSet ("<<std::endl;
		cfg<<"type        = cms.string(\"GenElectron\"),"<<std::endl;
		cfg<<"Name        = cms.string(\"GenElectron\"),"<<std::endl;
		cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
		cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
		cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
		cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
		cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
		cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
		cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
		cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<")"<<std::endl;
		cfg<<")"<<std::endl<<std::endl;
	}

	if(RECO){
		cfg<<"process.zeerec = cms.PSet ("<<std::endl;
		cfg<<"type        = cms.string(\"RecoElectron\"),"<<std::endl;
		cfg<<"Name        = cms.string(\"RecoElectron\"),"<<std::endl;
		cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
		cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
		cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
		cfg<<"JetType = cms.string(\""<<JetType.c_str()<<"\"),"<<std::endl;
		cfg<<"ReportName        = cms.string(\""<<RepName.c_str()<<"\"),"<<std::endl;
		cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
		cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
		cfg<<"Qual = cms.int32("<<Qual<<"),"<<std::endl;
		cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
		cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
		cfg<<"ElID = cms.int32("<<ElID<<"),"<<std::endl;
		cfg<<"Conv = cms.int32("<<Conv<<"),"<<std::endl;
		cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
		cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
		cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
		//cfg<<"GenParticleMatch = cms.bool("<<GenParticleMatch<<"),"<<std::endl;
		cfg<<"JECUnc  = cms.int32("<<JECUnc<<"),"<<std::endl;
		cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
		cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
		cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<")"<<std::endl;
		cfg<<")"<<std::endl<<std::endl;
	}

	if(EFF){
		cfg<<"process.zeeeff = cms.PSet ("<<std::endl;
		cfg<<"type        = cms.string(\"EfficiencyElectron\"),"<<std::endl;
		cfg<<"Name        = cms.string(\"EfficiencyElectron\"),"<<std::endl;
		cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
		cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
		cfg<<"JetType = cms.string(\""<<JetType.c_str()<<"\"),"<<std::endl;
		cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
		cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
		cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
		cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
		cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
		cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
		cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<"),"<<std::endl;
		cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
		cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
		cfg<<"Qual = cms.int32("<<Qual<<"),"<<std::endl;
		cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
		cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
		cfg<<"ElID = cms.int32("<<ElID<<"),"<<std::endl;
		cfg<<"Conv = cms.int32("<<Conv<<"),"<<std::endl;
		cfg<<")"<<std::endl<<std::endl;
	}

	if(NTUPLE){
		cfg<<"process.zeentuple = cms.PSet ("<<std::endl;
		cfg<<"type        = cms.string(\"RecoElectronNtuple\"),"<<std::endl;
		cfg<<"Name        = cms.string(\"RecoElectronNtuple\"),"<<std::endl;
		cfg<<"sourceFileList = cms.string(\""<<Source.c_str()<<"\"),"<<std::endl;
		cfg<<"Selections = cms.string(\""<<selections.c_str()<<"\"),"<<std::endl;
		cfg<<"JetType = cms.string(\""<<JetType.c_str()<<"\"),"<<std::endl;
		cfg<<"Sample = cms.string(\""<<sample.c_str()<<"\"),"<<std::endl;
		cfg<<"Acc = cms.int32("<<Acc<<"),"<<std::endl;
		cfg<<"Trg = cms.int32("<<Trg<<"),"<<std::endl;
		cfg<<"Qual = cms.int32("<<Qual<<"),"<<std::endl;
		cfg<<"Imp = cms.int32("<<Imp<<"),"<<std::endl;
		cfg<<"Iso = cms.int32("<<Iso<<"),"<<std::endl;
		cfg<<"ElID = cms.int32("<<ElID<<"),"<<std::endl;
		cfg<<"Conv = cms.int32("<<Conv<<"),"<<std::endl;
		cfg<<"CrossSection = cms.double("<<xsec<<"),"<<std::endl;
		cfg<<"targetLumi  = cms.double("<<targetLumi<<"),"<<std::endl;
		cfg<<"Norm        = cms.bool("<<Norm<<"),"<<std::endl;
		cfg<<"NtupleFill        = cms.string(\""<<NtupleFill<<"\"),"<<std::endl;
		cfg<<"JECUnc  = cms.int32("<<JECUnc<<"),"<<std::endl;
		cfg<<"JECUncFilePath        = cms.string(\""<<JECUncFilePath<<"\"),"<<std::endl;
		cfg<<"EventsPerFile  = cms.int32("<<EventsPerFile<<"),"<<std::endl;
		cfg<<"EventNumber  = cms.int32("<<EventNumber<<"),"<<std::endl;
		cfg<<"ProcEvents  = cms.int32("<<ProcEvents<<")"<<std::endl;
		cfg<<")"<<std::endl<<std::endl;
	}

	cfg.close();
}


TDSet* getDS(const char* filename){

	TDSet* out = new TDSet("dataset", "Events");
	ifstream infile;
	infile.open(filename);
	string datafile;
	while(getline (infile, datafile)){
		out->Add(datafile.c_str());
	}
	return out; 
}

#endif
