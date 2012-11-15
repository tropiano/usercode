#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TFileMerger.h"
#include "TSystem.h"
#include "TROOT.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Firenze/PATAnalysis/include/EfficiencyAnalyzerElectron.h"

using namespace std;

int main() {

	//Input files - Z2
	string training_sign = 
	"/gpfs/ddn/cms/user/gonzi/Analisi/rel_CMSSW_4_2_5_PhotonsPAT/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/DYJetsToLL_M50_Train_TeP_ElePhoton.root";
	string training_back = 
	"/gpfs/ddn/cms/user/gonzi/Analisi/rel_CMSSW_4_2_5_PhotonsPAT/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/Background_Train_TeP_ElePhoton.root";
	string total = 
	"/gpfs/ddn/cms/user/gonzi/Analisi/rel_CMSSW_4_2_5_PhotonsPAT/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/DYJetsToLL_M50_Sample_TeP_ElePhoton.root"; 
//	"Background_Sample_TeP_ElePhoton.root"; 
//	"Data_TeP_ElePhoton.root"; 


	//Output files
	string output_train_sign =
	"/gpfs/ddn/cms/user/gonzi/Analisi/rel_CMSSW_4_2_5_PhotonsPAT/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisElectrons/SigTrain_Pt_Barrel.root";
	string output_train_back =
	"/gpfs/ddn/cms/user/gonzi/Analisi/rel_CMSSW_4_2_5_PhotonsPAT/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisElectrons/BackTrain_Pt_Barrel.root";
	string output_total =
	"/gpfs/ddn/cms/user/gonzi/Analisi/rel_CMSSW_4_2_5_PhotonsPAT/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisElectrons/SigSample_Pt_Barrel.root";
//	"/gpfs/ddn/cms/user/gonzi/Analisi/rel_CMSSW_4_2_5_PhotonsPAT/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisElectrons/BackSample_Pt_Barrel.root";
//	"/gpfs/ddn/cms/user/gonzi/Analisi/rel_CMSSW_4_2_5_PhotonsPAT/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisElectrons/Data_Pt_Barrel.root";


	//Selections used
	string selections = "SYM";

	//Cut to evaluate
	string cut = "Global";

	//Multiplicity - "excl" = Exclusive; "incl" = Inclusive
	//Topology - "barrel" = Barrel; "endcap" = Endcap;
	string TPdir = "barrel";

	//Analyzer steps
	bool tr_sig = true;
	bool tr_back = true;
	bool total_an = true;

	//Mass bin
	int massbin = 3;

	//IF directories are different from default ones
	string nodef_dir = "";
	string nodef_secdir = "";

	//Cuts
	int _Acc =  1;
	int _Trg =  0;
	int _Qual = 0;
	int _Imp =  0;
	int _Iso =  2;
	int _ElID = 3;
	int _Conv = 0;
	
	string _RecoCutFlags[8];
	
	for(int i=0; i<8; i++){
		_RecoCutFlags[i] = "_1";
	}
 
	if(selections=="SYM"){
		_RecoCutFlags[_Acc] = "_AccSYM";
		_RecoCutFlags[_Qual] = "_QualSYM"; 
		_RecoCutFlags[_Iso] = "_IsoSYM";
		_RecoCutFlags[_ElID] = "_ElIDSYM";
		_RecoCutFlags[_Conv] = "_ConvSYM";
	}
	if(selections=="ASYM"){
		_RecoCutFlags[_Acc] = "_AccASYM";
		_RecoCutFlags[_Qual] = "_QualASYM"; 
		_RecoCutFlags[_Iso] = "_IsoASYM";
		_RecoCutFlags[_ElID] = "_ElIDASYM";
		_RecoCutFlags[_Conv] = "_ConvASYM";
	}
 
	_RecoCutFlags[_Trg] = "_Trg"; 
	_RecoCutFlags[_Imp] = "_Imp";

	TFile* train_sign = 0;
	TFile* train_back = 0;
	TFile* total_in = 0;
	TFile* out_train_sign = 0;
	TFile* out_train_back = 0;
	TFile* total_out = 0;

	if(training_sign!="") train_sign = new TFile(training_sign.c_str());
	if(training_back!="") train_back = new TFile(training_back.c_str());
	if(total!="") total_in = new TFile(total.c_str());

	if(output_train_sign!=""&&tr_sig) out_train_sign = new TFile(output_train_sign.c_str(), "RECREATE");
	if(output_train_sign!=""&&!tr_sig) out_train_sign = new TFile(output_train_sign.c_str());
	if(output_train_back!=""&&tr_back) out_train_back = new TFile(output_train_back.c_str(), "RECREATE");
	if(output_train_back!=""&&!tr_back) out_train_back = new TFile(output_train_back.c_str());
	if(output_total!=""&&total_an) total_out = new TFile(output_total.c_str(), "RECREATE");
	if(output_total!=""&&!total_an) total_out = new TFile(output_total.c_str());

	string dir = "EfficiencyElectron/";
	string sec_el_dir = "EfficiencyElectron/";

	string dataset = "datasetelectron";
	

	if(selections=="SYM" && TPdir=="excl") dir+="Tag&Probe_Excl";
	if(selections=="SYM" && TPdir=="incl") dir+="Tag&Probe_Incl";

	if(selections=="ASYM" && TPdir=="excl"){
		dir+="Tag&Probe0_Excl";
		sec_el_dir+="Tag&Probe1_Excl";
	}
	if(selections=="ASYM" && TPdir=="incl"){
		dir+="Tag&Probe0_Incl";
		sec_el_dir+="Tag&Probe1_Incl";
	}

	if(selections=="SYM" && TPdir=="barrel") dir+="Tag&Probe_Pt_Barrel";
	if(selections=="SYM" && TPdir=="endcap") dir+="Tag&Probe_Pt_Endcap";

	if(selections=="ASYM" && TPdir=="barrel"){
		dir+="Tag&Probe0_Pt_Barrel";
		sec_el_dir+="Tag&Probe1_Pt_Barrel";
	}
	if(selections=="ASYM" && TPdir=="endcap"){
		dir+="Tag&Probe0_Pt_Endcap";
		sec_el_dir+="Tag&Probe1_Pt_Endcap";
	}
	
	if(cut!="Global" && cut!="Double"){	
		int ncut = 1;	
		while(cut!=_RecoCutFlags[ncut]){
			dir+=_RecoCutFlags[ncut].c_str();
			sec_el_dir+=_RecoCutFlags[ncut].c_str();
			dataset+=_RecoCutFlags[ncut].c_str();
			ncut++;
			if(ncut>6)throw cms::Exception("PATAnalysis:zeetpana_CutNotFound") << "ERROR! Cut selected not found!";
		}
	  	dir+=_RecoCutFlags[ncut].c_str();
	  	sec_el_dir+=_RecoCutFlags[ncut].c_str();
	  	dataset+=_RecoCutFlags[ncut].c_str();
	  	dataset+="_";
	}

	else if(cut=="Global"){
		dir+="_Global";
		sec_el_dir+="_Global";
		dataset+="_Global_";
	}

	else if(cut=="Double"){
		dir+="_Double";
		sec_el_dir+="_Double";
		dataset+="_Double_";
	}

	if(nodef_dir!="") dir=nodef_dir;
	if(nodef_secdir!="") sec_el_dir=nodef_secdir;

	if(selections=="SYM") sec_el_dir="";
	
	EfficiencyAnalyzerElectron TP_TrainSign(train_sign, out_train_sign, dir.c_str(), sec_el_dir.c_str(), dataset.c_str());
	EfficiencyAnalyzerElectron TP_TrainBack(train_back, out_train_back, dir.c_str(), sec_el_dir.c_str(), dataset.c_str()); 
	EfficiencyAnalyzerElectron TP_Total(total_in, total_out, dir.c_str(), sec_el_dir.c_str(), dataset.c_str(), out_train_sign, out_train_back);
 
	if(tr_sig) TP_TrainSign.analyze(massbin,"signal");
	if(tr_back) TP_TrainBack.analyze(massbin,"background");
	if(total_an) TP_Total.analyze(massbin,"total");
 
	if(output_train_sign!="") delete out_train_sign;
	if(output_train_back!="") delete out_train_back;
	if(output_total!="") delete total_out;

	return 0;

}
