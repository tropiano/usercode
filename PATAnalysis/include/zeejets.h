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
11. DYJetstoLL_All 			(not skimmed)
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
21. DYJetstoLL_All_Train 		(not skimmed)
22. TTJets_All_Train 			(skimmed)
23. WJetsToLNu_All_Train 		(skimmed)
24. QCD_EMEnriched_Pt20to30_Train	(skimmed)
25. QCD_EMEnriched_Pt30to80_Train	(skimmed)
26. QCD_EMEnriched_Pt80to170_Train	(skimmed)
27. QCD_BCtoE_Pt20to30_Train 		(skimmed)
28. QCD_BCtoE_Pt30to80_Train 		(skimmed)
29. QCD_BCtoE_Pt80to170_Train		(skimmed)
201. WWJetsTo2L2Nu_All_Train 		(skimmed)
202. WZJetsTo3LNu_Train 		(skimmed)
203. ZZJetsTo4L_Train 			(skimmed)

-3. All Sample
31. DYJetstoLL_All_Sample 		(not skimmed)
32. TTJets_All_Sample 			(skimmed)
33. WJetsToLNu_All_Sample 		(skimmed)
34. QCD_EMEnriched_Pt20to30_Sample	(skimmed)
35. QCD_EMEnriched_Pt30to80_Sample	(skimmed)
36. QCD_EMEnriched_Pt80to170_Sample	(skimmed)
37. QCD_BCtoE_Pt20to30_Sample 		(skimmed)
38. QCD_BCtoE_Pt30to80_Sample 		(skimmed)
39. QCD_BCtoE_Pt80to170_Sample		(skimmed)
301. WWJetsTo2L2Nu_All_Sample 		(skimmed)
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

	else if(i==11) SampleName="DYJetstoLL_All";
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

	else if(i==21) SampleName="DYJetstoLL_Train";
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

	else if(i==31) SampleName="DYJetstoLL_Sample";
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

		case(-9):
			//TEST
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;


		case(1):
			//Run2011A_May10ReReco_v1  (skimmed)
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(2):
			//Run2011A_PromptReco_v4  (skimmed)
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(3):
			//Run2011A_05Aug2011_v1  (skimmed)
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(4):
			//Run2011A_PromptReco_v6  (skimmed)
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(5):
			//Run2011B_PromptReco_v1  (skimmed)
			Par->_xsec = 1.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;


		case(11):
			//DYJetstoLL_All:  _EventNumber = 2446664 events -> _EventNumber/_xsec = 802.7 pb-1 (not skimmed)
			Par->_xsec = 3048.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(12):
			//TTJets_All:  _EventNumber = 51221591 events -> _EventNumber/_xsec = 310434 pb-1 (skimmed)
			Par->_xsec = 165.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 51221591;
			break;

		case(13):
			//WJetsToLNu_All:  _EventNumber = 81676286 events -> _EventNumber/_xsec = 2608 pb-1 (skimmed)
			Par->_xsec = 31314.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 81676286;
			break;

		case(14):
			//QCD_EMEnriched_Pt20to30_All:  _EventNumber = 26587740 events -> _EventNumber/(_xsec * _EventFilter) = 10,6 pb-1  (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 0.0106;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 26587740;
			break;

		case(15):
			//QCD_EMEnriched_Pt30to80_All:  _EventNumber = 26240974 events  -> _EventNumber/(_xsec * _EventFilter) = 7,24 pb-1  (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.061;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 26240974;
			break;

		case(16):
			//QCD_EMEnriched_Pt80to170_All:  _EventNumber = 7307766 events -> _EventNumber/(_xsec * _EventFilter) = 51,17 pb-1  (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.159;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 7307766;
			break;

		case(17):
			//QCD_BCtoE_Pt20to30_All:  _EventNumber = 2071133 events  -> _EventNumber/(_xsec * _EventFilter) = 14,87 pb-1  (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 5.9E-4;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 2071133;
			break;

		case(18):
			//QCD_BCtoE_Pt30to80_All:  _EventNumber = 1453740 events -> _EventNumber/(_xsec * _EventFilter) =  10,11 pb-1  (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.00242;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 1453740;
			break;

		case(19):
			//QCD_BCtoE_Pt80to170_All:  _EventNumber = 1082691 events  -> _EventNumber/(_xsec * _EventFilter) =  114,80 pb-1  (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.0105;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 1082691;
			break;

		case(101):
			//WWJetsTo2L2Nu_All:  _EventNumber = 1197558 events -> _EventNumber/_xsec = 27850 pb-1   (skimmed)
			Par->_xsec = 43.0;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 1197558;
			break;

		case(102):
			//WZJetsTo3LNu_All:   _EventNumber = 921134 events -> _EventNumber/_xsec =  50612 pb-1   (skimmed)
			Par->_xsec = 18.2;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 921134;
			break;

		case(103):
			//ZZJetsTo4L_All:   _EventNumber events = 696454 -> _EventNumber/_xsec = 118043 pb-1   (skimmed)
			Par->_xsec = 5.9;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 696454;
			break;




		case(21):
			//DYJetstoLL_Train:  _EventNumber = 1199895 events -> _EventNumber/_xsec =  393,7 pb-1 (not skimmed)
			Par->_xsec = 3048.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(22):
			//TTJets_Train:  _EventNumber = 25476773 events -> _EventNumber/_xsec = 154404  pb-1  (skimmed)
			Par->_xsec = 165.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 25476773;
			break;

		case(23):
			//WJetsToLNu_Train:  _EventNumber = 40895295 events -> _EventNumber/_xsec =  1306 pb-1  (skimmed)
			Par->_xsec = 31314.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 40895295;
			break;

		case(24):
			//QCD_EMEnriched_Pt20to30_Train:  _EventNumber = 13224194 events -> _EventNumber/(_xsec * _EventFilter) = 5,28 pb-1  (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 0.0106;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 13224194;
			break;

		case(25):
			//QCD_EMEnriched_Pt30to80_Train:  _EventNumber = 13165476 events -> _EventNumber/(_xsec * _EventFilter) = 3,63 pb-1  (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.061;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 13165476;
			break;

		case(26):
			//QCD_EMEnriched_Pt80to170_Train:  _EventNumber = 3659576 events -> _EventNumber/(_xsec * _EventFilter) = 25,62 pb-1  (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.159;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 3659576;
			break;

		case(27):
			//QCD_BCtoE_Pt20to30_Train:  _EventNumber = 971133 events -> _EventNumber/(_xsec * _EventFilter) = 6,97 pb-1  (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 5.9E-4;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 971133;
			break;

		case(28):
			//QCD_BCtoE_Pt30to80_Train:  _EventNumber = 750000 events -> _EventNumber/(_xsec * _EventFilter) = 5,21 pb-1  (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.00242;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 750000;
			break;

		case(29):
			//QCD_BCtoE_Pt80to170_Train:  _EventNumber = 575227 events -> _EventNumber/(_xsec * _EventFilter) = 61 pb-1  (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.0105;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 575227;
			break;

		case(201):
			//WWJetsTo2L2Nu_Train:  _EventNumber = 622993 events -> _EventNumber/_xsec = 14488 pb-1 (skimmed)
			Par->_xsec = 43.0;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 622993;
			break;

		case(202):
			//WZJetsTo3LNu_Train:  _EventNumber = 421134 events -> _EventNumber/_xsec =  23139 pb-1 (skimmed)
			Par->_xsec = 18.2;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 421134;
			break;

		case(203):
			//ZZJetsTo4L_Train:  _EventNumber = 346454 events -> _EventNumber/_xsec = 58721 pb-1 (skimmed)
			Par->_xsec = 5.9;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 346454;
			break;


		case(31):
			//DYJetstoLL_Sample:  _EventNumber = 1246769 events -> _EventNumber/_xsec = 409 pb-1 (not skimmed)
			Par->_xsec = 3048.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 0;
			break;

		case(32):
			//TTJets_Sample:  _EventNumber =  25744818 events -> _EventNumber/_xsec = 156029 pb-1  (skimmed)
			Par->_xsec = 165.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 25744818;
			break;

		case(33):
			//WJetsToLNu_Sample:  _EventNumber = 40780991 events -> _EventNumber/_xsec = 1302 pb-1  (skimmed)
			Par->_xsec = 31314.;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 40780991;
			break;

		case(34):
			//QCD_EMEnriched_Pt20to30_Sample:  _EventNumber = 13363546 events -> _EventNumber/(_xsec * _EventFilter) = 5,34  pb-1  (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 0.0106;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 13363546;
			break;

		case(35):
			//QCD_EMEnriched_Pt30to80_Sample:  _EventNumber = 13075498 events -> _EventNumber/(_xsec * _EventFilter) =  3,60 pb-1  (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.061;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 13075498;
			break;

		case(36):
			//QCD_EMEnriched_Pt80to170_Sample:  _EventNumber = 3648190 events -> _EventNumber/(_xsec * _EventFilter) = 25,54  pb-1  (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.159;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 3648190;
			break;

		case(37):
			//QCD_BCtoE_Pt20to30_Sample:  _EventNumber = 1100000 events -> _EventNumber/(_xsec * _EventFilter) = 7,90 pb-1  (skimmed)
			Par->_xsec = 2.361E8;
			Par->_EventFilter = 5.9E-4;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 1100000;
			break;

		case(38):
			//QCD_BCtoE_Pt30to80_Sample:  _EventNumber = 750000 events -> _EventNumber/(_xsec * _EventFilter) = 5,21 pb-1  (skimmed)
			Par->_xsec = 5.944E7;
			Par->_EventFilter = 0.00242;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 750000;
			break;

		case(39):
			//QCD_BCtoE_Pt80to170_Sample:  _EventNumber = 507464 events -> _EventNumber/(_xsec * _EventFilter) = 53,80  pb-1  (skimmed)
			Par->_xsec = 898200.0;
			Par->_EventFilter = 0.0105;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 507464;
			break;

		case(301):
			//WWJetsTo2L2Nu_Sample:  _EventNumber = 574565 events -> _EventNumber/_xsec = 13362 pb-1  (skimmed)
			Par->_xsec = 43.0;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 574565;
			break;

		case(302):
			//WZJetsTo3LNu_Sample:  _EventNumber = 500000 events -> _EventNumber/_xsec = 27472 pb-1  (skimmed)
			Par->_xsec = 18.2;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 500000;
			break;

		case(303):
			//ZZJetsTo4L_Sample:  _EventNumber = 350000 events -> _EventNumber/_xsec = vpb-1  (skimmed)
			Par->_xsec = 5.9;
			Par->_EventFilter = 1.;
			Par->_EventsPerFile = 0;
			Par->_EventNumber = 350000;
			break;


		default:
			Par->_xsec=1.;
			Par->_EventFilter=1.;
			Par->_EventsPerFile=0;
			Par->_EventNumber=0;
			break;
	}
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

#endif
