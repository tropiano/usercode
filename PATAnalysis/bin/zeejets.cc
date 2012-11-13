#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"
#include "TParameter.h"
#include "TFileMerger.h"
#include "TDSet.h"
#include "TChain.h"
#include "TSystem.h"
#include "TEnv.h"
#include "TROOT.h"
#include "TProof.h"
#include "TProofLog.h"
#include "Firenze/PATAnalysis/include/zeejets.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

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
11. DYJetsToLL_M50_All			(not skimmed)
12. TTJets_All				(skimmed)
13. WJetsToLNu_All			(skimmed)
14. QCD_EMEnriched_Pt20to30_All		(skimmed)
15. QCD_EMEnriched_Pt30to80_All		(skimmed)
16. QCD_EMEnriched_Pt80to170_All	(skimmed)
17. QCD_BCtoE_Pt20to30_All		(skimmed)
18. QCD_BCtoE_Pt30to80_All 		(skimmed)
19. QCD_BCtoE_Pt80to170_All		(skimmed)
101. WWJetsTo2L2Nu_All			(skimmed)
102. WZJetsTo3LNu_All			(skimmed)
103. ZZJetsTo4L_All			(skimmed)

-2. All Train
21. DYJetsToLL_M50_Train 		(not skimmed)
22. TTJets_Train			(skimmed)
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
32. TTJets_Sample			(skimmed)
33. WJetsToLNu_Sample			(skimmed)
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

//argc: sample
//argv: number of events to be processed


int main(int argc, char *argv[]) {

	int PreDefName, ProcEvents;
	string sourceList, sample, outNameSuf;

	if (argc <3) {
		cout << "Usage is: " << endl << "zeejets.exe PreDefName ProcEvents" << endl << "Stop" << endl;
		return 0;
	}
	else

	PreDefName = atoi(argv[1]);
 
 	//Number of events to be processed
	ProcEvents = atoi(argv[2]);

	//List of Source Files
	string SourceFilesDir = "SourceFilesElectrons/";

	string Run2011A_May10ReReco_v1 = "Run2011A_May10ReReco_v1.list";
	string Run2011A_PromptReco_v4 = "Run2011A_PromptReco_v4.list";
	string Run2011A_05Aug2011_v1 = "Run2011A_05Aug2011_v1.list";
	string Run2011A_PromptReco_v6 = "Run2011A_PromptReco_v6.list";
	string Run2011B_PromptReco_v1 = "Run2011B_PromptReco_v1.list";

	string DYJetsToLL_M50_All = "DYJetsToLL_M50_All.list"; 
	string TTJets_All = "TTJets_All.list";
	string WJetsToLNu_All = "WJetsToLNu_All.list";
	string QCD_EMEnriched_Pt20to30_All = "QCD_EMEnriched_Pt20to30_All.list";
	string QCD_EMEnriched_Pt30to80_All = "QCD_EMEnriched_Pt30to80_All.list";
	string QCD_EMEnriched_Pt80to170_All = "QCD_EMEnriched_Pt80to170_All.list";
	string QCD_BCtoE_Pt20to30_All = "QCD_BCtoE_Pt20to30_All.list";
	string QCD_BCtoE_Pt30to80_All = "QCD_BCtoE_Pt30to80_All.list";
	string QCD_BCtoE_Pt80to170_All = "QCD_BCtoE_Pt80to170_All.list";
	string WWJetsTo2L2Nu_All = "WWJetsTo2L2Nu_All.list";
	string WZJetsTo3LNu_All = "WZJetsTo3LNu_All.list";
	string ZZJetsTo4L_All = "ZZJetsTo4L_All.list";	

	string DYJetsToLL_M50_Train = "DYJetsToLL_M50_Train.list"; 
	string TTJets_Train = "TTJets_Train.list";
	string WJetsToLNu_Train = "WJetsToLNu_Train.list";
	string QCD_EMEnriched_Pt20to30_Train = "QCD_EMEnriched_Pt20to30_Train.list";
	string QCD_EMEnriched_Pt30to80_Train = "QCD_EMEnriched_Pt30to80_Train.list";
	string QCD_EMEnriched_Pt80to170_Train = "QCD_EMEnriched_Pt80to170_Train.list";
	string QCD_BCtoE_Pt20to30_Train = "QCD_BCtoE_Pt20to30_Train.list";
	string QCD_BCtoE_Pt30to80_Train = "QCD_BCtoE_Pt30to80_Train.list";
	string QCD_BCtoE_Pt80to170_Train = "QCD_BCtoE_Pt80to170_Train.list";
	string WWJetsTo2L2Nu_Train = "WWJetsTo2L2Nu_Train.list";
	string WZJetsTo3LNu_Train = "WZJetsTo3LNu_Train.list";
	string ZZJetsTo4L_Train = "ZZJetsTo4L_Train.list";	

	string DYJetsToLL_M50_Sample = "DYJetsToLL_M50_Sample.list"; 
	string TTJets_Sample = "TTJets_Sample.list";
	string WJetsToLNu_Sample = "WJetsToLNu_Sample.list";
	string QCD_EMEnriched_Pt20to30_Sample = "QCD_EMEnriched_Pt20to30_Sample.list";
	string QCD_EMEnriched_Pt30to80_Sample = "QCD_EMEnriched_Pt30to80_Sample.list";
	string QCD_EMEnriched_Pt80to170_Sample = "QCD_EMEnriched_Pt80to170_Sample.list";
	string QCD_BCtoE_Pt20to30_Sample = "QCD_BCtoE_Pt20to30_Sample.list";
	string QCD_BCtoE_Pt30to80_Sample = "QCD_BCtoE_Pt30to80_Sample.list";
	string QCD_BCtoE_Pt80to170_Sample = "QCD_BCtoE_Pt80to170_Sample.list";
	string WWJetsTo2L2Nu_Sample = "WWJetsTo2L2Nu_Sample.list";
	string WZJetsTo3LNu_Sample = "WZJetsTo3LNu_Sample.list";
	string ZZJetsTo4L_Sample = "ZZJetsTo4L_Sample.list";	

	string Test = "Test.list";


	//sample: "data" or "mc"
	sample = "mc";
 

	//tag to recognize the analysis in the output file 
	string analysis = "_TeP_ElePhoton";

	
	//Path of PATAnalysis dir - DO NOT FORGET THE SLASH AT THE END OF THE PATH (complete path)
	string path="/data/sandro/Analisi/rel_CMSSW_4_2_5_PhotonsPAT_TEP/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisElectrons/";


	//Modules
	bool GEN = false;
	bool RECO = false;
	bool EFF = true;
	bool NTUPLE = false;
	

	//Ntuple - "zcand" = saves only z candidates; "acc" = save only events with Z in the acceptance; "all" = saves all the events
	string NtupleFill = "all";

	
	//Cuts: if number = 0, the cut doesn't affect the Z REC selection
	int Acc	=  1;
	int Trg	=  4;
	int Qual = 0;
	int Imp =  0;
	int Iso =  2;
	int ElID = 3;
	int Conv = 0;


	//Selections: "SYM" = V+jets selections (symmetric); "ASYM" = Vector Boson Task Force (a-symmetric)
	string selections = "SYM";

	//RECO Jet Type 
	string JetType = "selectedJetsL1Corrected";

	//JEC Uncertainty applied to RecoMuonNtuple: 0 = NotApplied, 1 = Added, -1 = Subtracted
	int JECUnc = 0; //default value = 0
	string JECUncFilePath = "/data/sfrosali/Zjets/Commit/CMSSW_4_2_25/src/Firenze/PATAnalysis/bin/JECUncertainty/Jec10V1_Uncertainty_AK5PF.txt";

	//Normalization
	string Norm = "True";
	double targetLumi = 100.; //pb-1
	if(PreDefName==0 || 
	   PreDefName==1 || 
	   PreDefName==2 || 
	   PreDefName==3 || 
	   PreDefName==4 || 
	   PreDefName==5) Norm="False"; // Do not normalize for data

	//Number of CPUs
	int CPU = 7;

	//Log (non funziona)
	bool Log = false;
	
	//Gen Particle Matching
	//string GenParticleMatch = "False";
	

	//definition of source List and sample (data or mc)

	// -> data
	if(PreDefName==-9) sourceList = (SourceFilesDir + Test).c_str();
 
	if(PreDefName==1){
		sourceList = (SourceFilesDir + Run2011A_May10ReReco_v1).c_str();
		sample = "data";
	}
	else if (PreDefName==2){
		sourceList = (SourceFilesDir + Run2011A_PromptReco_v4).c_str();
		sample = "data";
	}
	else if (PreDefName==3){
		sourceList = (SourceFilesDir + Run2011A_05Aug2011_v1).c_str();
		sample = "data";
	}
	else if (PreDefName==4){
		sourceList = (SourceFilesDir + Run2011A_PromptReco_v6).c_str();
		sample = "data";
	}
	else if (PreDefName==5){
		sourceList = (SourceFilesDir + Run2011B_PromptReco_v1).c_str();
		sample = "data";
	}

	// -> MC All
	else if (PreDefName==11){
		sourceList = (SourceFilesDir + DYJetsToLL_M50_All).c_str();
		sample = "mc";
	}
	else if (PreDefName==12){
		sourceList = (SourceFilesDir + TTJets_All).c_str();
		sample = "mc";
	}
	else if (PreDefName==13){
		sourceList = (SourceFilesDir + WJetsToLNu_All).c_str();
		sample = "mc";
	}
	else if (PreDefName==14){
		sourceList = (SourceFilesDir + QCD_EMEnriched_Pt20to30_All).c_str();
		sample = "mc";
	}
	else if (PreDefName==15){
		sourceList = (SourceFilesDir + QCD_EMEnriched_Pt30to80_All).c_str();
		sample = "mc";
	}
	else if (PreDefName==16){
		sourceList = (SourceFilesDir + QCD_EMEnriched_Pt80to170_All).c_str();
		sample = "mc";
	}
	else if (PreDefName==17){
		sourceList = (SourceFilesDir + QCD_BCtoE_Pt20to30_All).c_str();
		sample = "mc";
	}
	else if (PreDefName==18){
		sourceList = (SourceFilesDir + QCD_BCtoE_Pt30to80_All).c_str();
		sample = "mc";
	}
	else if (PreDefName==19){
		sourceList = (SourceFilesDir + QCD_BCtoE_Pt80to170_All).c_str();
		sample = "mc";
	}
	else if (PreDefName==101){
		sourceList = (SourceFilesDir + WWJetsTo2L2Nu_All).c_str();
		sample = "mc";
	}
	else if (PreDefName==102){
		sourceList = (SourceFilesDir + WZJetsTo3LNu_All).c_str();
		sample = "mc";
	}
	else if (PreDefName==103){
		sourceList = (SourceFilesDir + ZZJetsTo4L_All).c_str();
		sample = "mc";
	}

	// -> MC Train
	else if (PreDefName==21){
		sourceList = (SourceFilesDir + DYJetsToLL_M50_Train).c_str();
		sample = "mc";
	}
	else if (PreDefName==22){
		sourceList = (SourceFilesDir + TTJets_Train).c_str();
		sample = "mc";
	}
	else if (PreDefName==23){
		sourceList = (SourceFilesDir + WJetsToLNu_Train).c_str();
		sample = "mc";
	}
	else if (PreDefName==24){
		sourceList = (SourceFilesDir + QCD_EMEnriched_Pt20to30_Train).c_str();
		sample = "mc";
	}
	else if (PreDefName==25){
		sourceList = (SourceFilesDir + QCD_EMEnriched_Pt30to80_Train).c_str();
		sample = "mc";
	}
	else if (PreDefName==26){
		sourceList = (SourceFilesDir + QCD_EMEnriched_Pt80to170_Train).c_str();
		sample = "mc";
	}
	else if (PreDefName==27){
		sourceList = (SourceFilesDir + QCD_BCtoE_Pt20to30_Train).c_str();
		sample = "mc";
	}
	else if (PreDefName==28){
		sourceList = (SourceFilesDir + QCD_BCtoE_Pt30to80_Train).c_str();
		sample = "mc";
	}
	else if (PreDefName==29){
		sourceList = (SourceFilesDir + QCD_BCtoE_Pt80to170_Train).c_str();
		sample = "mc";
	}
	else if (PreDefName==201){
		sourceList = (SourceFilesDir + WWJetsTo2L2Nu_Train).c_str();
		sample = "mc";
	}
	else if (PreDefName==202){
		sourceList = (SourceFilesDir + WZJetsTo3LNu_Train).c_str();
		sample = "mc";
	}
	else if (PreDefName==203){
		sourceList = (SourceFilesDir + ZZJetsTo4L_Train).c_str();
		sample = "mc";
	}

	// -> MC Sample
	else if (PreDefName==31){
		sourceList = (SourceFilesDir + DYJetsToLL_M50_Sample).c_str();
		sample = "mc";
	}
	else if (PreDefName==32){
		sourceList = (SourceFilesDir + TTJets_Sample).c_str();
		sample = "mc";
	}
	else if (PreDefName==33){
		sourceList = (SourceFilesDir + WJetsToLNu_Sample).c_str();
		sample = "mc";
	}
	else if (PreDefName==34){
		sourceList = (SourceFilesDir + QCD_EMEnriched_Pt20to30_Sample).c_str();
		sample = "mc";
	}
	else if (PreDefName==35){
		sourceList = (SourceFilesDir + QCD_EMEnriched_Pt30to80_Sample).c_str();
		sample = "mc";
	}
	else if (PreDefName==36){
		sourceList = (SourceFilesDir + QCD_EMEnriched_Pt80to170_Sample).c_str();
		sample = "mc";
	}
	else if (PreDefName==37){
		sourceList = (SourceFilesDir + QCD_BCtoE_Pt20to30_Sample).c_str();
		sample = "mc";
	}
	else if (PreDefName==38){
		sourceList = (SourceFilesDir + QCD_BCtoE_Pt30to80_Sample).c_str();
		sample = "mc";
	}
	else if (PreDefName==39){
		sourceList = (SourceFilesDir + QCD_BCtoE_Pt80to170_Sample).c_str();
		sample = "mc";
	}
	else if (PreDefName==301){
		sourceList = (SourceFilesDir + WWJetsTo2L2Nu_Sample).c_str();
		sample = "mc";
	}
	else if (PreDefName==302){
		sourceList = (SourceFilesDir + WZJetsTo3LNu_Sample).c_str();
		sample = "mc";
	}
	else if (PreDefName==303){
		sourceList = (SourceFilesDir + ZZJetsTo4L_Sample).c_str();
		sample = "mc";
	}


	cout << "RUNNING ON " << sourceList << endl;
	cout << "PreDefName " << PreDefName << endl;


	// output file

	name(PreDefName);
	string outputName = SampleName;
	outputName+=analysis+=outNameSuf;


	// Parmeters setting and cfg creation

	double xsec = 1.0; 
	double EventFilter = 1.0;
	int EventsPerFile = 0;
	int EventNumber = 0;

	if(sample=="data"){
		//GenParticleMatch = "False";
		GEN = false;
	}
	
	// ->  Single Sample or Test 
	if(PreDefName>0 || PreDefName==-9){
		
		//Parameters
		Parameters(PreDefName, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
				
		makeCfg(sample, selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), sourceList.c_str(), outputName.c_str(), Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
	}

	// -> All data
	if(PreDefName==0){
		
		Parameters(1, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("data", selections, JetType, false, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), Run2011A_May10ReReco_v1.c_str(), "Run2011A_May10ReReco_v1", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(2, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("data", selections, JetType, false, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), Run2011A_PromptReco_v4.c_str(), "Run2011A_PromptReco_v4", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(3, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("data", selections, JetType, false, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), Run2011A_05Aug2011_v1.c_str(), "Run2011A_05Aug2011_v1", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(4, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("data", selections, JetType, false, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), Run2011A_PromptReco_v6.c_str(), "Run2011A_PromptReco_v6", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(5, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("data", selections, JetType, false, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), Run2011B_PromptReco_v1.c_str(), "Run2011B_PromptReco_v1", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
	}

	// -> MC All
	if(PreDefName==-1){

		Parameters(11, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), DYJetsToLL_M50_All.c_str(), "DYJetsToLL_M50_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(12, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), TTJets_All.c_str(), "TTJets_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(13, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), WJetsToLNu_All.c_str(), "WJetsToLNu_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(14, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_EMEnriched_Pt20to30_All.c_str(), "QCD_EMEnriched_Pt20to30_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(15, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_EMEnriched_Pt30to80_All.c_str(), "QCD_EMEnriched_Pt30to80_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(16, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_EMEnriched_Pt80to170_All.c_str(), "QCD_EMEnriched_Pt80to170_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(17, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_BCtoE_Pt20to30_All.c_str(), "QCD_BCtoE_Pt20to30_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(18, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_BCtoE_Pt30to80_All.c_str(), "QCD_BCtoE_Pt30to80_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(19, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_BCtoE_Pt80to170_All.c_str(), "QCD_BCtoE_Pt80to170_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(101, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), WWJetsTo2L2Nu_All.c_str(), "WWJetsTo2L2Nu_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(102, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), WZJetsTo3LNu_All.c_str(), "WZJetsTo3LNu_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(103, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), ZZJetsTo4L_All.c_str(), "ZZJetsTo4L_All", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

	}

	// -> MC Train
	if(PreDefName==-2){
		
		Parameters(21, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), DYJetsToLL_M50_Train.c_str(), "DYJetsToLL_M50_Train", "False", EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);//Il Train del segnale non è norm. per avere un conf. corretto con effMC (altrimenti avrei le barre d'errore sbagliate)
		
		Parameters(22, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), TTJets_Train.c_str(), "TTJets_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(23, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), WJetsToLNu_Train.c_str(), "WJetsToLNu_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(24, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_EMEnriched_Pt20to30_Train.c_str(), "QCD_EMEnriched_Pt20to30_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(25, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_EMEnriched_Pt30to80_Train.c_str(), "QCD_EMEnriched_Pt30to80_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(26, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_EMEnriched_Pt80to170_Train.c_str(), "QCD_EMEnriched_Pt80to170_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(27, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_BCtoE_Pt20to30_Train.c_str(), "QCD_BCtoE_Pt20to30_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(28, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_BCtoE_Pt30to80_Train.c_str(), "QCD_BCtoE_Pt30to80_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(29, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_BCtoE_Pt80to170_Train.c_str(), "QCD_BCtoE_Pt80to170_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(201, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), WWJetsTo2L2Nu_Train.c_str(), "WWJetsTo2L2Nu_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(202, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), WZJetsTo3LNu_Train.c_str(), "WZJetsTo3LNu_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(203, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), ZZJetsTo4L_Train.c_str(), "ZZJetsTo4L_Train", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
	}


	// -> MC Sample
	if(PreDefName==-3){
		
		Parameters(31, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), DYJetsToLL_M50_Sample.c_str(), "DYJetsToLL_M50_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(32, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), TTJets_Sample.c_str(), "TTJets_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(33, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), WJetsToLNu_Sample.c_str(), "WJetsToLNu_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(34, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_EMEnriched_Pt20to30_Sample.c_str(), "QCD_EMEnriched_Pt20to30_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(35, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_EMEnriched_Pt30to80_Sample.c_str(), "QCD_EMEnriched_Pt30to80_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(36, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_EMEnriched_Pt80to170_Sample.c_str(), "QCD_EMEnriched_Pt80to170_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(37, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_BCtoE_Pt20to30_Sample.c_str(), "QCD_BCtoE_Pt20to30_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
		Parameters(38, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_BCtoE_Pt30to80_Sample.c_str(), "QCD_BCtoE_Pt30to80_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(39, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), QCD_BCtoE_Pt80to170_Sample.c_str(), "QCD_BCtoE_Pt80to170_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(301, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), WWJetsTo2L2Nu_Sample.c_str(), "WWJetsTo2L2Nu_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(302, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), WZJetsTo3LNu_Sample.c_str(), "WZJetsTo3LNu_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);

		Parameters(303, &ParStruct);
		xsec=ParStruct._xsec;
		EventFilter=ParStruct._EventFilter;
		EventsPerFile=ParStruct._EventsPerFile;
		EventNumber=ParStruct._EventNumber;
		makeCfg("mc", selections, JetType, GEN, RECO, EFF, NTUPLE, Acc, Trg, Qual, Imp, Iso, ElID, Conv, path.c_str(), ZZJetsTo4L_Sample.c_str(), "ZZJetsTo4L_Sample", Norm.c_str(), EventsPerFile, EventNumber, ProcEvents, xsec*EventFilter, targetLumi, NtupleFill, JECUnc, JECUncFilePath);
		
	}


	//TProof settings
	gEnv->SetValue("Proof.Sandbox", "/data/sandro/.proof");

	TProof * p = TProof::Open("");

	p->SetParallel(CPU);
	p->SetParameter( "PROOF_UseTreeCache", ( Int_t ) 0 );


	gSystem->Load("libFWCoreFWLite");
	AutoLibraryLoader::enable();
	gSystem->Load("libFirenzePATAnalysis");
	p->Exec(".x /data/sandro/Analisi/rel_CMSSW_4_2_5_PhotonsPAT_TEP/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/remote.C");


	//Process

	// ->  Single Sample or Test 
	if(PreDefName>0 || PreDefName==-9){
	  TDSet* SetDS = getDS(sourceList.c_str());
	  
	  string cfgPath = path;
	  cfgPath+=outputName;
	  cfgPath+=".py";
	  TNamed* configfile = new TNamed("ConfigFile", cfgPath.c_str());
	  p->AddInput(configfile);
	  p->Process(SetDS, "FWLiteTSelector","",ProcEvents);
	  p->ClearInput();
	  delete SetDS;
	}

	// -> All data
	if(PreDefName==0){ 

		string cfgPath_0_1=path+"Run2011A_May10ReReco_v1.py";
		TDSet* SetDS_0_1 = getDS(Run2011A_May10ReReco_v1.c_str());
		TNamed* configfile_0_1 = new TNamed("ConfigFile", cfgPath_0_1.c_str());
		p->AddInput(configfile_0_1);
		p->Process(SetDS_0_1, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_0_1;

		string cfgPath_0_2=path+"Run2011A_PromptReco_v4.py";
		TDSet* SetDS_0_2 = getDS(Run2011A_PromptReco_v4.c_str());
		TNamed* configfile_0_2 = new TNamed("ConfigFile", cfgPath_0_2.c_str());
		p->AddInput(configfile_0_2);
		p->Process(SetDS_0_2, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_0_2;

		string cfgPath_0_3=path+"Run2011A_05Aug2011_v1.py";
		TDSet* SetDS_0_3 = getDS(Run2011A_05Aug2011_v1.c_str());
		TNamed* configfile_0_3 = new TNamed("ConfigFile", cfgPath_0_3.c_str());
		p->AddInput(configfile_0_3);
		p->Process(SetDS_0_3, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_0_3;

		string cfgPath_0_4=path+"Run2011A_PromptReco_v6.py";
		TDSet* SetDS_0_4 = getDS(Run2011A_PromptReco_v6.c_str());
		TNamed* configfile_0_4 = new TNamed("ConfigFile", cfgPath_0_4.c_str());
		p->AddInput(configfile_0_4);
		p->Process(SetDS_0_4, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_0_4;

		string cfgPath_0_5=path+"Run2011B_PromptReco_v1.py";
		TDSet* SetDS_0_5 = getDS(Run2011B_PromptReco_v1.c_str());
		TNamed* configfile_0_5 = new TNamed("ConfigFile", cfgPath_0_5.c_str());
		p->AddInput(configfile_0_5);
		p->Process(SetDS_0_5, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_0_5;

	}

	// -> MC All
	if(PreDefName==-1){ 

		string cfgPath_1_11=path+"DYJetsToLL_M50_All.py";
		TDSet* SetDS_1_11 = getDS(DYJetsToLL_M50_All.c_str());
		TNamed* configfile_1_11 = new TNamed("ConfigFile", cfgPath_1_11.c_str());
		p->AddInput(configfile_1_11);
		p->Process(SetDS_1_11, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_11;

		string cfgPath_1_12=path+"TTJets_All.py";
		TDSet* SetDS_1_12 = getDS(TTJets_All.c_str());
		TNamed* configfile_1_12 = new TNamed("ConfigFile", cfgPath_1_12.c_str());
		p->AddInput(configfile_1_12);
		p->Process(SetDS_1_12, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_12;

		string cfgPath_1_13=path+"WJetsToLNu_All.py";
		TDSet* SetDS_1_13 = getDS(WJetsToLNu_All.c_str());
		TNamed* configfile_1_13 = new TNamed("ConfigFile", cfgPath_1_13.c_str());
		p->AddInput(configfile_1_13);
		p->Process(SetDS_1_13, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_13;

		string cfgPath_1_14=path+"QCD_EMEnriched_Pt20to30_All.py";
		TDSet* SetDS_1_14 = getDS(QCD_EMEnriched_Pt20to30_All.c_str());
		TNamed* configfile_1_14 = new TNamed("ConfigFile", cfgPath_1_14.c_str());
		p->AddInput(configfile_1_14);
		p->Process(SetDS_1_14, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_14;

		string cfgPath_1_15=path+"QCD_EMEnriched_Pt30to80_All.py";
		TDSet* SetDS_1_15 = getDS(QCD_EMEnriched_Pt30to80_All.c_str());
		TNamed* configfile_1_15 = new TNamed("ConfigFile", cfgPath_1_15.c_str());
		p->AddInput(configfile_1_15);
		p->Process(SetDS_1_15, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_15;

		string cfgPath_1_16=path+"QCD_EMEnriched_Pt80to170_All.py";
		TDSet* SetDS_1_16 = getDS(QCD_EMEnriched_Pt80to170_All.c_str());
		TNamed* configfile_1_16 = new TNamed("ConfigFile", cfgPath_1_16.c_str());
		p->AddInput(configfile_1_16);
		p->Process(SetDS_1_16, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_16;

		string cfgPath_1_17=path+"QCD_BCtoE_Pt20to30_All.py";
		TDSet* SetDS_1_17 = getDS(QCD_BCtoE_Pt20to30_All.c_str());
		TNamed* configfile_1_17 = new TNamed("ConfigFile", cfgPath_1_17.c_str());
		p->AddInput(configfile_1_17);
		p->Process(SetDS_1_17, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_17;

		string cfgPath_1_18=path+"QCD_BCtoE_Pt30to80_All.py";
		TDSet* SetDS_1_18 = getDS(QCD_BCtoE_Pt30to80_All.c_str());
		TNamed* configfile_1_18 = new TNamed("ConfigFile", cfgPath_1_18.c_str());
		p->AddInput(configfile_1_18);
		p->Process(SetDS_1_18, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_18;

		string cfgPath_1_19=path+"QCD_BCtoE_Pt80to170_All.py";
		TDSet* SetDS_1_19 = getDS(QCD_BCtoE_Pt80to170_All.c_str());
		TNamed* configfile_1_19 = new TNamed("ConfigFile", cfgPath_1_19.c_str());
		p->AddInput(configfile_1_19);
		p->Process(SetDS_1_19, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_19;

		string cfgPath_1_101=path+"WWJetsTo2L2Nu_All.py";
		TDSet* SetDS_1_101 = getDS(WWJetsTo2L2Nu_All.c_str());
		TNamed* configfile_1_101 = new TNamed("ConfigFile", cfgPath_1_101.c_str());
		p->AddInput(configfile_1_101);
		p->Process(SetDS_1_101, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_101;

		string cfgPath_1_102=path+"WZJetsTo3LNu_All.py";
		TDSet* SetDS_1_102 = getDS(WZJetsTo3LNu_All.c_str());
		TNamed* configfile_1_102 = new TNamed("ConfigFile", cfgPath_1_102.c_str());
		p->AddInput(configfile_1_102);
		p->Process(SetDS_1_102, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_102;

		string cfgPath_1_103=path+"ZZJetsTo4L_All.py";
		TDSet* SetDS_1_103 = getDS(ZZJetsTo4L_All.c_str());
		TNamed* configfile_1_103 = new TNamed("ConfigFile", cfgPath_1_103.c_str());
		p->AddInput(configfile_1_103);
		p->Process(SetDS_1_103, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_1_103;

	}

	// -> MC Train
	if(PreDefName==-2){ 

		string cfgPath_2_21=path+"DYJetsToLL_M50_Train.py";
		TDSet* SetDS_2_21 = getDS(DYJetsToLL_M50_Train.c_str());
		TNamed* configfile_2_21 = new TNamed("ConfigFile", cfgPath_2_21.c_str());
		p->AddInput(configfile_2_21);
		p->Process(SetDS_2_21, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_21;

		string cfgPath_2_22=path+"TTJets_Train.py";
		TDSet* SetDS_2_22 = getDS(TTJets_Train.c_str());
		TNamed* configfile_2_22 = new TNamed("ConfigFile", cfgPath_2_22.c_str());
		p->AddInput(configfile_2_22);
		p->Process(SetDS_2_22, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_22;

		string cfgPath_2_23=path+"WJetsToLNu_Train.py";
		TDSet* SetDS_2_23 = getDS(WJetsToLNu_Train.c_str());
		TNamed* configfile_2_23 = new TNamed("ConfigFile", cfgPath_2_23.c_str());
		p->AddInput(configfile_2_23);
		p->Process(SetDS_2_23, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_23;

		string cfgPath_2_24=path+"QCD_EMEnriched_Pt20to30_Train.py";
		TDSet* SetDS_2_24 = getDS(QCD_EMEnriched_Pt20to30_Train.c_str());
		TNamed* configfile_2_24 = new TNamed("ConfigFile", cfgPath_2_24.c_str());
		p->AddInput(configfile_2_24);
		p->Process(SetDS_2_24, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_24;

		string cfgPath_2_25=path+"QCD_EMEnriched_Pt30to80_Train.py";
		TDSet* SetDS_2_25 = getDS(QCD_EMEnriched_Pt30to80_Train.c_str());
		TNamed* configfile_2_25 = new TNamed("ConfigFile", cfgPath_2_25.c_str());
		p->AddInput(configfile_2_25);
		p->Process(SetDS_2_25, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_25;

		string cfgPath_2_26=path+"QCD_EMEnriched_Pt80to170_Train.py";
		TDSet* SetDS_2_26 = getDS(QCD_EMEnriched_Pt80to170_Train.c_str());
		TNamed* configfile_2_26 = new TNamed("ConfigFile", cfgPath_2_26.c_str());
		p->AddInput(configfile_2_26);
		p->Process(SetDS_2_26, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_26;

		string cfgPath_2_27=path+"QCD_BCtoE_Pt20to30_Train.py";
		TDSet* SetDS_2_27 = getDS(QCD_BCtoE_Pt20to30_Train.c_str());
		TNamed* configfile_2_27 = new TNamed("ConfigFile", cfgPath_2_27.c_str());
		p->AddInput(configfile_2_27);
		p->Process(SetDS_2_27, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_27;

		string cfgPath_2_28=path+"QCD_BCtoE_Pt30to80_Train.py";
		TDSet* SetDS_2_28 = getDS(QCD_BCtoE_Pt30to80_Train.c_str());
		TNamed* configfile_2_28 = new TNamed("ConfigFile", cfgPath_2_28.c_str());
		p->AddInput(configfile_2_28);
		p->Process(SetDS_2_28, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_28;

		string cfgPath_2_29=path+"QCD_BCtoE_Pt80to170_Train.py";
		TDSet* SetDS_2_29 = getDS(QCD_BCtoE_Pt80to170_Train.c_str());
		TNamed* configfile_2_29 = new TNamed("ConfigFile", cfgPath_2_29.c_str());
		p->AddInput(configfile_2_29);
		p->Process(SetDS_2_29, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_29;

		string cfgPath_2_201=path+"WWJetsTo2L2Nu_Train.py";
		TDSet* SetDS_2_201 = getDS(WWJetsTo2L2Nu_Train.c_str());
		TNamed* configfile_2_201 = new TNamed("ConfigFile", cfgPath_2_201.c_str());
		p->AddInput(configfile_2_201);
		p->Process(SetDS_2_201, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_201;

		string cfgPath_2_202=path+"WZJetsTo3LNu_Train.py";
		TDSet* SetDS_2_202 = getDS(WZJetsTo3LNu_Train.c_str());
		TNamed* configfile_2_202 = new TNamed("ConfigFile", cfgPath_2_202.c_str());
		p->AddInput(configfile_2_202);
		p->Process(SetDS_2_202, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_202;

		string cfgPath_2_203=path+"ZZJetsTo4L_Train.py";
		TDSet* SetDS_2_203 = getDS(ZZJetsTo4L_Train.c_str());
		TNamed* configfile_2_203 = new TNamed("ConfigFile", cfgPath_2_203.c_str());
		p->AddInput(configfile_2_203);
		p->Process(SetDS_2_203, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_2_203;

	}

	// -> MC Sample
	if(PreDefName==-3){ 

		string cfgPath_3_31=path+"DYJetsToLL_M50_Sample.py";
		TDSet* SetDS_3_31 = getDS(DYJetsToLL_M50_Sample.c_str());
		TNamed* configfile_3_31 = new TNamed("ConfigFile", cfgPath_3_31.c_str());
		p->AddInput(configfile_3_31);
		p->Process(SetDS_3_31, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_31;

		string cfgPath_3_32=path+"TTJets_Sample.py";
		TDSet* SetDS_3_32 = getDS(TTJets_Sample.c_str());
		TNamed* configfile_3_32 = new TNamed("ConfigFile", cfgPath_3_32.c_str());
		p->AddInput(configfile_3_32);
		p->Process(SetDS_3_32, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_32;

		string cfgPath_3_33=path+"WJetsToLNu_Sample.py";
		TDSet* SetDS_3_33 = getDS(WJetsToLNu_Sample.c_str());
		TNamed* configfile_3_33 = new TNamed("ConfigFile", cfgPath_3_33.c_str());
		p->AddInput(configfile_3_33);
		p->Process(SetDS_3_33, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_33;

		string cfgPath_3_34=path+"QCD_EMEnriched_Pt20to30_Sample.py";
		TDSet* SetDS_3_34 = getDS(QCD_EMEnriched_Pt20to30_Sample.c_str());
		TNamed* configfile_3_34 = new TNamed("ConfigFile", cfgPath_3_34.c_str());
		p->AddInput(configfile_3_34);
		p->Process(SetDS_3_34, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_34;

		string cfgPath_3_35=path+"QCD_EMEnriched_Pt30to80_Sample.py";
		TDSet* SetDS_3_35 = getDS(QCD_EMEnriched_Pt30to80_Sample.c_str());
		TNamed* configfile_3_35 = new TNamed("ConfigFile", cfgPath_3_35.c_str());
		p->AddInput(configfile_3_35);
		p->Process(SetDS_3_35, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_35;

		string cfgPath_3_36=path+"QCD_EMEnriched_Pt80to170_Sample.py";
		TDSet* SetDS_3_36 = getDS(QCD_EMEnriched_Pt80to170_Sample.c_str());
		TNamed* configfile_3_36 = new TNamed("ConfigFile", cfgPath_3_36.c_str());
		p->AddInput(configfile_3_36);
		p->Process(SetDS_3_36, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_36;

		string cfgPath_3_37=path+"QCD_BCtoE_Pt20to30_Sample.py";
		TDSet* SetDS_3_37 = getDS(QCD_BCtoE_Pt20to30_Sample.c_str());
		TNamed* configfile_3_37 = new TNamed("ConfigFile", cfgPath_3_37.c_str());
		p->AddInput(configfile_3_37);
		p->Process(SetDS_3_37, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_37;

		string cfgPath_3_38=path+"QCD_BCtoE_Pt30to80_Sample.py";
		TDSet* SetDS_3_38 = getDS(QCD_BCtoE_Pt30to80_Sample.c_str());
		TNamed* configfile_3_38 = new TNamed("ConfigFile", cfgPath_3_38.c_str());
		p->AddInput(configfile_3_38);
		p->Process(SetDS_3_38, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_38;

		string cfgPath_3_39=path+"QCD_BCtoE_Pt80to170_Sample.py";
		TDSet* SetDS_3_39 = getDS(QCD_BCtoE_Pt80to170_Sample.c_str());
		TNamed* configfile_3_39 = new TNamed("ConfigFile", cfgPath_3_39.c_str());
		p->AddInput(configfile_3_39);
		p->Process(SetDS_3_39, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_39;

		string cfgPath_3_301=path+"WWJetsTo2L2Nu_Sample.py";
		TDSet* SetDS_3_301 = getDS(WWJetsTo2L2Nu_Sample.c_str());
		TNamed* configfile_3_301 = new TNamed("ConfigFile", cfgPath_3_301.c_str());
		p->AddInput(configfile_3_301);
		p->Process(SetDS_3_301, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_301;

		string cfgPath_3_302=path+"WZJetsTo3LNu_Sample.py";
		TDSet* SetDS_3_302 = getDS(WZJetsTo3LNu_Sample.c_str());
		TNamed* configfile_3_302 = new TNamed("ConfigFile", cfgPath_3_302.c_str());
		p->AddInput(configfile_3_302);
		p->Process(SetDS_3_302, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_302;

		string cfgPath_3_303=path+"ZZJetsTo4L_Sample.py";
		TDSet* SetDS_3_303 = getDS(ZZJetsTo4L_Sample.c_str());
		TNamed* configfile_3_303 = new TNamed("ConfigFile", cfgPath_3_303.c_str());
		p->AddInput(configfile_3_303);
		p->Process(SetDS_3_303, "FWLiteTSelector","",ProcEvents);
		p->ClearInput();
		delete SetDS_3_303;

	}


	if(Log){ 
		TProofLog *pl = TProof::Mgr("")->GetSessionLogs();
		pl->Save("0.1",path.c_str());
	}

	p->Close();
		 
	return 0;
}
