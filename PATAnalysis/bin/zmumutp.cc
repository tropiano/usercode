#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TFileMerger.h"
#include "TSystem.h"
#include "TROOT.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Firenze/PATAnalysis/include/MuonUtilities.h"
#include "Firenze/PATAnalysis/include/EfficiencyAnalyzerMuon.h"

using namespace std;

int main() {

	//Input files
	string training_sign = 
	"/raid/sandro/Analisi/rel_CMSSW_4_2_5_TeP/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisMuons/DYtoLL_PtEta_Train_Small.root";
	string training_back = 
	"/raid/sandro/Analisi/rel_CMSSW_4_2_5_TeP/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisMuons/Background_PtEta_Train.root";
	string total = 
	"/raid/sandro/Analisi/rel_CMSSW_4_2_5_TeP/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisMuons/Data_input_TeP.root"; 

	//Output files
	string output_train_sign =
	"/raid/sandro/Analisi/rel_CMSSW_4_2_5_TeP/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisMuons/TrainSig_Small_PtEta_endcap_Global_1.root";
	string output_train_back =
	"/raid/sandro/Analisi/rel_CMSSW_4_2_5_TeP/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisMuons/TrainBack_PtEta_endcap_Global_1.root";
	string output_total =
	"/raid/sandro/Analisi/rel_CMSSW_4_2_5_TeP/CMSSW_4_2_5/src/Firenze/PATAnalysis/bin/AnalysisMuons/TP_Data_PtEta_endcap_Global_1.root";

	//Cuts
	int _Acc = 1;
	int _Trg = 0;
	int _Imp = 2;
	int _Qual = 3;
	int _Iso = 4;
	int _MuID = 0;

	//TPDirectory - "excl" = Exclusive Mult.; "incl" = Inclusive Mult.; "barrel" = Pt-Barrel; "endcap" = Pt-Endcap
	string TPdir = "endcap";

	//Selections used
	string selections = "SYM";

	//Cut to evaluate = Trg, Imp, Qual, Iso, MuID, Global (AllCuts/Acc), Double (double selection applied for signal and background extraction from fits)

	string cut = "Global";

	//Analyzer steps
	bool tr_sig = true;
	bool tr_back = true;
	bool total_an = true;

	//Mass bin
	int massbin = 3;

	//IF directories are different from default ones
	string nodef_dir = "";
	string nodef_secdir = "";
	
	string _RecoCutFlags[7];
	
	for(int i=0; i<7; i++){
		_RecoCutFlags[i] = "_1";
	}
	 
	if(selections=="SYM"){
		 _RecoCutFlags[_Acc] = "_AccSYM";
		 _RecoCutFlags[_Qual] = "_QualSYM"; 
		 _RecoCutFlags[_Iso] = "_IsoSYM";
		 _RecoCutFlags[_MuID] = "_MuIDSYM";
	}
	 if(selections=="ASYM"){
		 _RecoCutFlags[_Acc] = "_AccASYM";
		 _RecoCutFlags[_Qual] = "_QualASYM"; 
		 _RecoCutFlags[_Iso] = "_IsoASYM";
		 _RecoCutFlags[_MuID] = "_MuIDASYM";
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

	string dir = "EfficiencyMuon/";
	string sec_mu_dir = "EfficiencyMuon/";

	string dataset = "datasetMuon";
	

	if(selections=="SYM" && TPdir=="excl") dir+="Tag&Probe_Excl";

	if(selections=="SYM" && TPdir=="incl") dir+="Tag&Probe_Incl";

	if(selections=="ASYM" && TPdir=="excl"){
		dir+="Tag&Probe0_Excl";
		sec_mu_dir+="Tag&Probe1_Excl";
	}

	if(selections=="ASYM" && TPdir=="incl"){
		dir+="Tag&Probe0_Incl";
		sec_mu_dir+="Tag&Probe1_Incl";
	}

	if(selections=="SYM" && TPdir=="barrel") dir+="Tag&Probe_Pt_Barrel";

	if(selections=="SYM" && TPdir=="endcap") dir+="Tag&Probe_Pt_Endcap";

	if(selections=="ASYM" && TPdir=="barrel"){
		dir+="Tag&Probe0_Pt_Barrel";
		sec_mu_dir+="Tag&Probe1_Pt_Barrel";}

	if(selections=="ASYM" && TPdir=="endcap"){
		dir+="Tag&Probe0_Pt_Endcap";
		sec_mu_dir+="Tag&Probe1_Pt_Endcap";
	}

	if(cut!="Global" && cut!="Double"){	 
		int ncut = 1;	
		while(!in_quote(_RecoCutFlags[ncut], cut.c_str())){
			dir+=_RecoCutFlags[ncut].c_str();
			sec_mu_dir+=_RecoCutFlags[ncut].c_str();
			dataset+=_RecoCutFlags[ncut].c_str();
			ncut++;
			if(ncut>6) throw cms::Exception("PATAnalysis:zmumutp_CutNotFound") << "ERROR! Cut selected not found!";
		}

		dir+=_RecoCutFlags[ncut].c_str();
		sec_mu_dir+=_RecoCutFlags[ncut].c_str();
		dataset+=_RecoCutFlags[ncut].c_str();
		dataset+="_";
	}

	else if(cut=="Global"){
		dir+="_Global";
		sec_mu_dir+="_Global";
		dataset+="_Global_";
	}

	else if(cut=="Double"){
		dir+="_Double";
		sec_mu_dir+="_Double";
		dataset+="_Double_";
	}
	
	if(nodef_dir!="") dir = nodef_dir;

	if(nodef_secdir!="") sec_mu_dir = nodef_secdir;

	if(selections=="SYM") sec_mu_dir="";

	EfficiencyAnalyzerMuon TP_TrainSign(train_sign, out_train_sign, dir.c_str(), sec_mu_dir.c_str(), dataset.c_str());
	EfficiencyAnalyzerMuon TP_TrainBack(train_back, out_train_back, dir.c_str(), sec_mu_dir.c_str(), dataset.c_str()); 
	EfficiencyAnalyzerMuon TP_Total(total_in, total_out, dir.c_str(), sec_mu_dir.c_str(), dataset.c_str(), out_train_sign, out_train_back);
 
	if(tr_sig) TP_TrainSign.analyze(massbin,"signal");
	if(tr_back) TP_TrainBack.analyze(massbin,"background");
	if(total_an) TP_Total.analyze(massbin,"total");
 
	if(output_train_sign!="") delete out_train_sign;
	if(output_train_back!="") delete out_train_back;
	if(output_total!="") delete total_out;
 
	return 0;

}
