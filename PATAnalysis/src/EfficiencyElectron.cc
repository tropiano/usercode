#include "Firenze/PATAnalysis/include/EfficiencyElectron.h"
#include "Firenze/PATAnalysis/include/ElectronUtilities.h"

#include <TH2.h>
#include <TStyle.h>
#include <math.h>
#include <map> 
#include <iostream>
#include <fstream>
#include <assert.h>

#include "TList.h"
#include "TParameter.h"
#include "TFile.h"
#include "TProofOutputFile.h"
#include "TChain.h"

//CMSSW headers
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/TrackReco/interface/Track.h"


using namespace std;
using namespace edm;


EfficiencyElectron::EfficiencyElectron():

//Denominatore misura accettanza
AccDenom_GenExclJetNumber(0), AccDenom_GenInclJetNumber(0), AccDenom_RecoExclJetNumber(0), AccDenom_RecoInclJetNumber(0), AccDenom_GenSingleElPt_Barrel(0), AccDenom_GenSingleElPt_Endcap(0), 

//Denominatore misura efficienza
EffDenom_GenExclJetNumber(0), EffDenom_RecoExclJetNumber(0), EffDenom_RecoInclJetNumber(0), EffDenom_GenInclJetNumber(0), EffDenom_GenSingleElPt_Barrel(0), EffDenom_GenSingleElPt_Endcap(0), 

//Efficienza vs molteplicità dei jet
RecoInclJetEffWGA_1(0), RecoInclJetEffWGA_12(0), RecoInclJetEffWGA_123(0), RecoInclJetEffWGA_1234(0), RecoInclJetEffWGA_12345(0), RecoInclJetEffWGA_123456(0), RecoInclJetEffWGA_1234567(0),
RecoExclJetEffWGA_1(0), RecoExclJetEffWGA_12(0), RecoExclJetEffWGA_123(0), RecoExclJetEffWGA_1234(0), RecoExclJetEffWGA_12345(0), RecoExclJetEffWGA_123456(0), RecoExclJetEffWGA_1234567(0),

RecoInclJetEff_1(0), RecoInclJetEff_12(0), RecoInclJetEff_123(0), RecoInclJetEff_1234(0), RecoInclJetEff_12345(0), RecoInclJetEff_123456(0), RecoInclJetEff_1234567(0),
RecoExclJetEff_1(0), RecoExclJetEff_12(0), RecoExclJetEff_123(0), RecoExclJetEff_1234(0), RecoExclJetEff_12345(0), RecoExclJetEff_123456(0), RecoExclJetEff_1234567(0),

//Efficienza di singolo leptone vs pt
RecoSingleElPtEff_Barrel_1(0), RecoSingleElPtEff_Barrel_12(0), RecoSingleElPtEff_Barrel_123(0), RecoSingleElPtEff_Barrel_1234(0), RecoSingleElPtEff_Barrel_12345(0), RecoSingleElPtEff_Barrel_123456(0), RecoSingleElPtEff_Barrel_1234567(0),
RecoSingleElPtEff_Endcap_1(0), RecoSingleElPtEff_Endcap_12(0), RecoSingleElPtEff_Endcap_123(0), RecoSingleElPtEff_Endcap_1234(0), RecoSingleElPtEff_Endcap_12345(0), RecoSingleElPtEff_Endcap_123456(0), RecoSingleElPtEff_Endcap_1234567(0),

_sample("mc"), _selections("SYM"), _JetType("PF"), _dir(0), _file(0), _Acc(0), _Trg(0), _Qual(0), _Imp(0), _Iso(0), _ElID(0), _Conv(0), _histoVector(), _Norm(false), _norm(1.), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1)

{ }

void EfficiencyElectron::begin(TFile* out, const edm::ParameterSet& iConfig){

	_file = out; 
	_sample = iConfig.getParameter<std::string>("Sample");
	std::string dirname = iConfig.getParameter<std::string>("Name");
	std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
	_selections = iConfig.getParameter<std::string>("Selections");
	_Norm = iConfig.getParameter<bool>("Norm");
	_EventsPerFile	= iConfig.getParameter<int32_t>("EventsPerFile");
	_EventNumber	= iConfig.getParameter<int32_t>("EventNumber");
	_ProcEvents	= iConfig.getParameter<int32_t>("ProcEvents");
	_targetLumi= iConfig.getParameter<double>("targetLumi");
	_xsec = iConfig.getParameter<double>("CrossSection");
	_JetType = iConfig.getParameter<std::string>("JetType");
	
	//Selections
	_Acc = iConfig.getParameter<int32_t>("Acc");
	_Trg = iConfig.getParameter<int32_t>("Trg");
	_Qual = iConfig.getParameter<int32_t>("Qual");
	_Imp = iConfig.getParameter<int32_t>("Imp");
	_Iso = iConfig.getParameter<int32_t>("Iso");
	_ElID = iConfig.getParameter<int32_t>("ElID");
	_Conv = iConfig.getParameter<int32_t>("Conv");	

	for(int i=0; i<8; i++){
		_RecoCutFlags[i] = "_1";
	}
	
	if(_selections=="SYM"){
		_RecoCutFlags[_Acc] = "_AccSYM";
		_RecoCutFlags[_Qual] = "_QualSYM"; 
		_RecoCutFlags[_Iso] = "_IsoSYM";
//		_RecoCutFlags[_Iso] = "_IsoPhotonSYM";
		_RecoCutFlags[_ElID] = "_ElIDSYM";
//		_RecoCutFlags[_ElID] = "_ElIDPhotonSYM";
		_RecoCutFlags[_Conv] = "_ConvSYM";
	}
	if(_selections=="ASYM"){
		_RecoCutFlags[_Acc] = "_AccASYM";
		_RecoCutFlags[_Qual] = "_QualASYM"; 
		_RecoCutFlags[_Iso] = "_IsoASYM";
//		_RecoCutFlags[_Iso] = "_IsoPhotonASYM";
		_RecoCutFlags[_ElID] = "_ElIDASYM";
//		_RecoCutFlags[_ElID] = "_ElIDPhotonASYM";
		_RecoCutFlags[_Conv] = "_ConvASYM";
	}
	
	_RecoCutFlags[_Trg] = "_Trg";
	_RecoCutFlags[_Imp] = "_Imp";

	cout << "EfficiencyElectron file name : " << _file->GetName() << endl;
	_file->cd();
	_dir = _file->mkdir(dirname.c_str(), dirname.c_str());
	_dir->cd();
	
	// Bins for efficiency measurement
	
	int nbin_mult = 10;
	int nbin_pt = 3;
	double mult_xmin = -0.5;
	double mult_xmax = 9.5;
	double pt_xmin = 0;
	double pt_xmax = 3;

	///////////////////////////////////
	
	// Acceptance denominator
	AccDenom_GenExclJetNumber = new TH1D("AccDenom_GenExclJetNumber", "Number of Gen Iso Jet, No acceptance cut applied", 10, -0.5, 9.5);
	_histoVector.push_back(AccDenom_GenExclJetNumber);
	AccDenom_RecoExclJetNumber = new TH1D("AccDenom_RecoExclJetNumber", "Number of Exclusive Reco Jet, No acceptance cut applied", 10, -0.5, 9.5);
	_histoVector.push_back(AccDenom_RecoExclJetNumber);
	AccDenom_RecoInclJetNumber = new TH1D("AccDenom_RecoInclJetNumber", "Number of Inclusive Reco Jet, No acceptance cut applied", 10, -0.5, 9.5);
	_histoVector.push_back(AccDenom_RecoInclJetNumber);
	AccDenom_GenInclJetNumber = new TH1D("AccDenom_GenInclJetNumber", "Number of Inclusive Gen Jet, No acceptance cut applied", 10, -0.5, 9.5);
	_histoVector.push_back(AccDenom_GenInclJetNumber);
	AccDenom_GenSingleElPt_Barrel = new TH1D("AccDenom_GenSingleElPt_Barrel", "Reco electron Pt, No acceptance cut applied - Barrel", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(AccDenom_GenSingleElPt_Barrel);
	AccDenom_GenSingleElPt_Endcap = new TH1D("AccDenom_GenSingleElPt_Endcap", "Reco electron Pt, No acceptance cut applied - Endcap", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(AccDenom_GenSingleElPt_Endcap);
	
	// Efficiency denominator
	_histoVector.push_back(EffDenom_GenSingleElPt_Endcap);
	EffDenom_GenExclJetNumber = new TH1D("EffDenom_GenExclJetNumber", "Number of Gen Iso Jet, in acceptance", 10, -0.5, 9.5);
	_histoVector.push_back(EffDenom_GenExclJetNumber);
	EffDenom_RecoExclJetNumber = new TH1D("EffDenom_RecoExclJetNumber", "Number of Exclusive Reco Iso Jet, in acceptance", 10, -0.5, 9.5);
	_histoVector.push_back(EffDenom_RecoExclJetNumber);
	EffDenom_RecoInclJetNumber = new TH1D("EffDenom_RecoInclJetNumber", "Number of Inclusive Reco Iso Jet, in acceptance", 10, -0.5, 9.5);
	_histoVector.push_back(EffDenom_RecoInclJetNumber);
	EffDenom_GenInclJetNumber = new TH1D("EffDenom_GenInclJetNumber", "Number of Inclusive Gen Iso Jet, in acceptance", 10, -0.5, 9.5);
	_histoVector.push_back(EffDenom_GenInclJetNumber);
	EffDenom_GenSingleElPt_Barrel = new TH1D("EffDenom_GenSingleElPt_Barrel", "Generated Leading electron #eta, in acceptance", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(EffDenom_GenSingleElPt_Barrel);
	EffDenom_GenSingleElPt_Endcap = new TH1D("EffDenom_GenSingleElPt_Endcap", "Generated Leading electron Pt, in acceptance", nbin_pt, pt_xmin, pt_xmax);
	
	// Eff vs Z electrons properties
	string RecoSingleElPtEff_Barrel_name = "RecoSingleElPtEff_Barrel";
	RecoSingleElPtEff_Barrel_name += _RecoCutFlags[1].c_str();
	RecoSingleElPtEff_Barrel_1 = new TH1D(RecoSingleElPtEff_Barrel_name.c_str(), "Reco electron #eta", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Barrel_1);
	RecoSingleElPtEff_Barrel_name += _RecoCutFlags[2].c_str();
	RecoSingleElPtEff_Barrel_12 = new TH1D(RecoSingleElPtEff_Barrel_name.c_str(), "Reco electron #eta", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Barrel_12);
	RecoSingleElPtEff_Barrel_name += _RecoCutFlags[3].c_str();
	RecoSingleElPtEff_Barrel_123 = new TH1D(RecoSingleElPtEff_Barrel_name.c_str(), "Reco electron #eta", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Barrel_123);
	RecoSingleElPtEff_Barrel_name += _RecoCutFlags[4].c_str();
	RecoSingleElPtEff_Barrel_1234 = new TH1D(RecoSingleElPtEff_Barrel_name.c_str(), "Reco electron #eta", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Barrel_1234);
	RecoSingleElPtEff_Barrel_name += _RecoCutFlags[5].c_str();
	RecoSingleElPtEff_Barrel_12345 = new TH1D(RecoSingleElPtEff_Barrel_name.c_str(), "Reco electron #eta", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Barrel_12345);
	RecoSingleElPtEff_Barrel_name += _RecoCutFlags[6].c_str();
	RecoSingleElPtEff_Barrel_123456 = new TH1D(RecoSingleElPtEff_Barrel_name.c_str(), "Reco electron #eta", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Barrel_123456);
	RecoSingleElPtEff_Barrel_name += _RecoCutFlags[7].c_str();
	RecoSingleElPtEff_Barrel_1234567 = new TH1D(RecoSingleElPtEff_Barrel_name.c_str(), "Reco electron #eta", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Barrel_1234567);

	
	string RecoSingleElPtEff_Endcap_name = "RecoSingleElPtEff_Endcap";
	RecoSingleElPtEff_Endcap_name += _RecoCutFlags[1].c_str();
	RecoSingleElPtEff_Endcap_1 = new TH1D(RecoSingleElPtEff_Endcap_name.c_str(), "Reco electron Pt", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Endcap_1);
	RecoSingleElPtEff_Endcap_name += _RecoCutFlags[2].c_str();
	RecoSingleElPtEff_Endcap_12 = new TH1D(RecoSingleElPtEff_Endcap_name.c_str(), "Reco electron Pt", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Endcap_12);
	RecoSingleElPtEff_Endcap_name += _RecoCutFlags[3].c_str();
	RecoSingleElPtEff_Endcap_123 = new TH1D(RecoSingleElPtEff_Endcap_name.c_str(), "Reco electron Pt", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Endcap_123);
	RecoSingleElPtEff_Endcap_name += _RecoCutFlags[4].c_str();
	RecoSingleElPtEff_Endcap_1234 = new TH1D(RecoSingleElPtEff_Endcap_name.c_str(), "Reco electron Pt", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Endcap_1234);
	RecoSingleElPtEff_Endcap_name += _RecoCutFlags[5].c_str();
	RecoSingleElPtEff_Endcap_12345 = new TH1D(RecoSingleElPtEff_Endcap_name.c_str(), "Reco electron Pt", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Endcap_12345);
	RecoSingleElPtEff_Endcap_name += _RecoCutFlags[6].c_str();
	RecoSingleElPtEff_Endcap_123456 = new TH1D(RecoSingleElPtEff_Endcap_name.c_str(), "Reco electron Pt", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Endcap_123456);
	RecoSingleElPtEff_Endcap_name += _RecoCutFlags[7].c_str();
	RecoSingleElPtEff_Endcap_1234567 = new TH1D(RecoSingleElPtEff_Endcap_name.c_str(), "Reco electron Pt", nbin_pt, pt_xmin, pt_xmax);
	_histoVector.push_back(RecoSingleElPtEff_Endcap_1234567);

	// Eff vs Reco Excl Jet (iso) number With Gen(Acc) Req.
	string RecoExclJetEffWGA_name = "RecoExclJetEffWGenAcc";
	RecoExclJetEffWGA_name += _RecoCutFlags[1].c_str();
	RecoExclJetEffWGA_1 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEffWGA_1);
	RecoExclJetEffWGA_name += _RecoCutFlags[2].c_str();
	RecoExclJetEffWGA_12 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEffWGA_12);
	RecoExclJetEffWGA_name += _RecoCutFlags[3].c_str();
	RecoExclJetEffWGA_123 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEffWGA_123);
	RecoExclJetEffWGA_name += _RecoCutFlags[4].c_str();
	RecoExclJetEffWGA_1234 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEffWGA_1234);
	RecoExclJetEffWGA_name += _RecoCutFlags[5].c_str();
	RecoExclJetEffWGA_12345 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEffWGA_12345);
	RecoExclJetEffWGA_name += _RecoCutFlags[6].c_str();
	RecoExclJetEffWGA_123456 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEffWGA_123456);
	RecoExclJetEffWGA_name += _RecoCutFlags[7].c_str();
	RecoExclJetEffWGA_1234567 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEffWGA_1234567);	

	// Eff vs Reco Incl Jet (iso) number With Gen(Acc) Req.
	string RecoInclJetEffWGA_name = "RecoInclJetEffWGenAcc";
	RecoInclJetEffWGA_name += _RecoCutFlags[1].c_str();
	RecoInclJetEffWGA_1 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEffWGA_1);
	RecoInclJetEffWGA_name += _RecoCutFlags[2].c_str();
	RecoInclJetEffWGA_12 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEffWGA_12);
	RecoInclJetEffWGA_name += _RecoCutFlags[3].c_str();
	RecoInclJetEffWGA_123 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEffWGA_123);
	RecoInclJetEffWGA_name += _RecoCutFlags[4].c_str();
	RecoInclJetEffWGA_1234 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEffWGA_1234);
	RecoInclJetEffWGA_name += _RecoCutFlags[5].c_str();
	RecoInclJetEffWGA_12345 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEffWGA_12345);
	RecoInclJetEffWGA_name += _RecoCutFlags[6].c_str();
	RecoInclJetEffWGA_123456 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEffWGA_123456); 
	RecoInclJetEffWGA_name += _RecoCutFlags[7].c_str();
	RecoInclJetEffWGA_1234567 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEffWGA_1234567); 
	
	// Eff vs Reco Incl (Iso) Jet number
	string RecoInclJetEff_name = "RecoInclJetEff";
	RecoInclJetEff_name += _RecoCutFlags[1].c_str();
	RecoInclJetEff_1 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEff_1);
	RecoInclJetEff_name += _RecoCutFlags[2].c_str();
	RecoInclJetEff_12 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEff_12);
	RecoInclJetEff_name += _RecoCutFlags[3].c_str();
	RecoInclJetEff_123 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEff_123);
	RecoInclJetEff_name += _RecoCutFlags[4].c_str();
	RecoInclJetEff_1234 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEff_1234);
	RecoInclJetEff_name += _RecoCutFlags[5].c_str();
	RecoInclJetEff_12345 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEff_12345);
	RecoInclJetEff_name += _RecoCutFlags[6].c_str();
	RecoInclJetEff_123456 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEff_123456);
	RecoInclJetEff_name += _RecoCutFlags[7].c_str();
	RecoInclJetEff_1234567 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoInclJetEff_1234567);

	
	// Eff vs Reco Excl (Iso) Jet number
	string RecoExclJetEff_name = "RecoExclJetEff";
	RecoExclJetEff_name += _RecoCutFlags[1].c_str();
	RecoExclJetEff_1 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEff_1);
	RecoExclJetEff_name += _RecoCutFlags[2].c_str();
	RecoExclJetEff_12 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEff_12);
	RecoExclJetEff_name += _RecoCutFlags[3].c_str();
	RecoExclJetEff_123 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEff_123);
	RecoExclJetEff_name += _RecoCutFlags[4].c_str();
	RecoExclJetEff_1234 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEff_1234);
	RecoExclJetEff_name += _RecoCutFlags[5].c_str();
	RecoExclJetEff_12345 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEff_12345);
	RecoExclJetEff_name += _RecoCutFlags[6].c_str();
	RecoExclJetEff_123456 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEff_123456);
	RecoExclJetEff_name += _RecoCutFlags[7].c_str();
	RecoExclJetEff_1234567 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
	_histoVector.push_back(RecoExclJetEff_1234567);

	
	// Tag & Probe

	if(_selections=="SYM"){
		tag_cuts.push_back(singleEl_Tag_SYM);
		tag_cuts_AllSel.push_back(singleEl_Tag_AllSel_SYM);
	}
	if(_selections=="ASYM"){
		tag_cuts_0.push_back(singleEl_Tag_ASYM0);
		tag_cuts_1.push_back(singleEl_Tag_ASYM1);
		tag_cuts_0_AllSel.push_back(singleEl_Tag_AllSel_ASYM0);
		tag_cuts_1_AllSel.push_back(singleEl_Tag_AllSel_ASYM1);
	}

	if(_selections=="SYM"){
		probe_cuts_1.clear();
		probe_cuts_12.clear();
		probe_cuts_123.clear();
		probe_cuts_1234.clear();
		probe_cuts_12345.clear();
		probe_cuts_123456.clear();
		probe_cuts_1234567.clear();

	for(unsigned int i = 1; i < 8; i++){
		if(_RecoCutFlags[i] == "_AccSYM"){
			if(i<2) probe_cuts_1.push_back(singleEl_Probe_Acc_SYM);
			if(i<3) probe_cuts_12.push_back(singleEl_Probe_Acc_SYM);
			if(i<4) probe_cuts_123.push_back(singleEl_Probe_Acc_SYM);
			if(i<5) probe_cuts_1234.push_back(singleEl_Probe_Acc_SYM);
			if(i<6) probe_cuts_12345.push_back(singleEl_Probe_Acc_SYM);
			if(i<7) probe_cuts_123456.push_back(singleEl_Probe_Acc_SYM);
			if(i<8) probe_cuts_1234567.push_back(singleEl_Probe_Acc_SYM);
		}
		if(_RecoCutFlags[i] == "_Trg"){
			std::cout<<"filling trigger cuts"<<std::endl;
			if(i<2)probe_cuts_1.push_back(singleEl_Probe_Trg_SYM);
			if(i<3)probe_cuts_12.push_back(singleEl_Probe_Trg_SYM);
			if(i<4)probe_cuts_123.push_back(singleEl_Probe_Trg_SYM);
			if(i<5)probe_cuts_1234.push_back(singleEl_Probe_Trg_SYM);
			if(i<6)probe_cuts_12345.push_back(singleEl_Probe_Trg_SYM);
			if(i<7)probe_cuts_123456.push_back(singleEl_Probe_Trg_SYM);
			if(i<8)probe_cuts_1234567.push_back(singleEl_Probe_Trg_SYM);
		}
		if(_RecoCutFlags[i] == "_QualSYM"){
			if(i<2) probe_cuts_1.push_back(singleEl_Probe_Qual_SYM);
			if(i<3) probe_cuts_12.push_back(singleEl_Probe_Qual_SYM);
			if(i<4) probe_cuts_123.push_back(singleEl_Probe_Qual_SYM);
			if(i<5) probe_cuts_1234.push_back(singleEl_Probe_Qual_SYM);
			if(i<6) probe_cuts_12345.push_back(singleEl_Probe_Qual_SYM);
			if(i<7) probe_cuts_123456.push_back(singleEl_Probe_Qual_SYM);
			if(i<8) probe_cuts_1234567.push_back(singleEl_Probe_Qual_SYM);
		}
		if(_RecoCutFlags[i] == "_Imp"){
			if(i<2) probe_cuts_1.push_back(singleEl_Probe_Imp_SYM);
			if(i<3) probe_cuts_12.push_back(singleEl_Probe_Imp_SYM);
			if(i<4) probe_cuts_123.push_back(singleEl_Probe_Imp_SYM);
			if(i<5) probe_cuts_1234.push_back(singleEl_Probe_Imp_SYM);
			if(i<6) probe_cuts_12345.push_back(singleEl_Probe_Imp_SYM);
			if(i<7) probe_cuts_123456.push_back(singleEl_Probe_Imp_SYM);
			if(i<8) probe_cuts_1234567.push_back(singleEl_Probe_Imp_SYM);
		}

		if(_RecoCutFlags[i] == "_IsoSYM"){
			if(i<2) probe_cuts_1.push_back(singleEl_Probe_Iso_SYM);
			if(i<3) probe_cuts_12.push_back(singleEl_Probe_Iso_SYM);
			if(i<4) probe_cuts_123.push_back(singleEl_Probe_Iso_SYM);
			if(i<5) probe_cuts_1234.push_back(singleEl_Probe_Iso_SYM);
			if(i<6) probe_cuts_12345.push_back(singleEl_Probe_Iso_SYM);
			if(i<7) probe_cuts_123456.push_back(singleEl_Probe_Iso_SYM);
			if(i<8) probe_cuts_1234567.push_back(singleEl_Probe_Iso_SYM);
		}
/*
		if(_RecoCutFlags[i] == "_IsoPhotonSYM"){
			if(i<2) probe_cuts_1.push_back(singleEl_Probe_IsoPhoton_SYM);
			if(i<3) probe_cuts_12.push_back(singleEl_Probe_IsoPhoton_SYM);
			if(i<4) probe_cuts_123.push_back(singleEl_Probe_IsoPhoton_SYM);
			if(i<5) probe_cuts_1234.push_back(singleEl_Probe_IsoPhoton_SYM);
			if(i<6) probe_cuts_12345.push_back(singleEl_Probe_IsoPhoton_SYM);
			if(i<7) probe_cuts_123456.push_back(singleEl_Probe_IsoPhoton_SYM);
			if(i<8) probe_cuts_1234567.push_back(singleEl_Probe_IsoPhoton_SYM);
		}
*/
		if(_RecoCutFlags[i] == "_ElIDSYM"){
			if(i<2) probe_cuts_1.push_back(singleEl_Probe_ElID_SYM);
			if(i<3) probe_cuts_12.push_back(singleEl_Probe_ElID_SYM);
			if(i<4) probe_cuts_123.push_back(singleEl_Probe_ElID_SYM);
			if(i<5) probe_cuts_1234.push_back(singleEl_Probe_ElID_SYM);
			if(i<6) probe_cuts_12345.push_back(singleEl_Probe_ElID_SYM);
			if(i<7) probe_cuts_123456.push_back(singleEl_Probe_ElID_SYM);
			if(i<8) probe_cuts_1234567.push_back(singleEl_Probe_ElID_SYM);
		}
/*
		if(_RecoCutFlags[i] == "_ElIDPhotonSYM"){
			if(i<2) probe_cuts_1.push_back(singleEl_Probe_ElIDPhoton_SYM);
			if(i<3) probe_cuts_12.push_back(singleEl_Probe_ElIDPhoton_SYM);
			if(i<4) probe_cuts_123.push_back(singleEl_Probe_ElIDPhoton_SYM);
			if(i<5) probe_cuts_1234.push_back(singleEl_Probe_ElIDPhoton_SYM);
			if(i<6) probe_cuts_12345.push_back(singleEl_Probe_ElIDPhoton_SYM);
			if(i<7) probe_cuts_123456.push_back(singleEl_Probe_ElIDPhoton_SYM);
			if(i<8) probe_cuts_1234567.push_back(singleEl_Probe_ElIDPhoton_SYM);
		}
*/
		if(_RecoCutFlags[i] == "_ConvSYM"){
			if(i<2) probe_cuts_1.push_back(singleEl_Probe_Conv_SYM);
			if(i<3) probe_cuts_12.push_back(singleEl_Probe_Conv_SYM);
			if(i<4) probe_cuts_123.push_back(singleEl_Probe_Conv_SYM);
			if(i<5) probe_cuts_1234.push_back(singleEl_Probe_Conv_SYM);
			if(i<6) probe_cuts_12345.push_back(singleEl_Probe_Conv_SYM);
			if(i<7) probe_cuts_123456.push_back(singleEl_Probe_Conv_SYM);
			if(i<8) probe_cuts_1234567.push_back(singleEl_Probe_Conv_SYM);
		}
		if(_RecoCutFlags[i] == "_1"){
			if(i<2) probe_cuts_1.push_back(singleEl_Probe_True);
			if(i<3) probe_cuts_12.push_back(singleEl_Probe_True);
			if(i<4) probe_cuts_123.push_back(singleEl_Probe_True);
			if(i<5) probe_cuts_1234.push_back(singleEl_Probe_True);
			if(i<6) probe_cuts_12345.push_back(singleEl_Probe_True);
			if(i<7) probe_cuts_123456.push_back(singleEl_Probe_True);
			if(i<8) probe_cuts_1234567.push_back(singleEl_Probe_True);
		}
	}

	string name_TagDir_Excl = "Tag&Probe_Excl";
	string name_TagDir_Incl = "Tag&Probe_Incl";
	string name_TPFiller = "electron";

	name_TagDir_Excl += _RecoCutFlags[1].c_str();
	name_TagDir_Excl += _RecoCutFlags[2].c_str();
	name_TagDir_Incl += _RecoCutFlags[1].c_str();
	name_TagDir_Incl += _RecoCutFlags[2].c_str();
	name_TPFiller += _RecoCutFlags[1].c_str();
	name_TPFiller += _RecoCutFlags[2].c_str();

	string name_TPFiller_12 = name_TPFiller+"_";
	TDirectory *TagDir_Excl_12 = _dir->mkdir(name_TagDir_Excl.c_str());
	TDirectory *TagDir_Incl_12 = _dir->mkdir(name_TagDir_Incl.c_str());

	_TagProbe_Electron_Excl_12 = new TagAndProbeFillerElectron(TagDir_Excl_12, string(name_TPFiller_12.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_1, probe_cuts_12);
	_TagProbe_Electron_Incl_12 = new TagAndProbeFillerElectron(TagDir_Incl_12, string(name_TPFiller_12.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_1, probe_cuts_12);

	name_TPFiller += _RecoCutFlags[3].c_str();
	string name_TPFiller_123 = name_TPFiller+"_"; 
	name_TagDir_Excl += _RecoCutFlags[3].c_str();
	TDirectory *TagDir_Excl_123 = _dir->mkdir(name_TagDir_Excl.c_str());
	name_TagDir_Incl += _RecoCutFlags[3].c_str();
	TDirectory *TagDir_Incl_123 = _dir->mkdir(name_TagDir_Incl.c_str());

	_TagProbe_Electron_Excl_123 = new TagAndProbeFillerElectron(TagDir_Excl_123, string(name_TPFiller_123.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_12, probe_cuts_123);
	_TagProbe_Electron_Incl_123 = new TagAndProbeFillerElectron(TagDir_Incl_123, string(name_TPFiller_123.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_12, probe_cuts_123);

	name_TPFiller += _RecoCutFlags[4].c_str();
	string name_TPFiller_1234 = name_TPFiller+"_";
	name_TagDir_Excl += _RecoCutFlags[4].c_str();
	TDirectory *TagDir_Excl_1234 = _dir->mkdir(name_TagDir_Excl.c_str()); 
	name_TagDir_Incl += _RecoCutFlags[4].c_str();
	TDirectory *TagDir_Incl_1234 = _dir->mkdir(name_TagDir_Incl.c_str());

	_TagProbe_Electron_Excl_1234 = new TagAndProbeFillerElectron(TagDir_Excl_1234, string(name_TPFiller_1234.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_123, probe_cuts_1234);
	_TagProbe_Electron_Incl_1234 = new TagAndProbeFillerElectron(TagDir_Incl_1234, string(name_TPFiller_1234.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_123, probe_cuts_1234);

	name_TPFiller += _RecoCutFlags[5].c_str();
	string name_TPFiller_12345 = name_TPFiller+"_";
	name_TagDir_Excl += _RecoCutFlags[5].c_str();
	TDirectory *TagDir_Excl_12345 = _dir->mkdir(name_TagDir_Excl.c_str());
	name_TagDir_Incl += _RecoCutFlags[5].c_str();
	TDirectory *TagDir_Incl_12345 = _dir->mkdir(name_TagDir_Incl.c_str());

	_TagProbe_Electron_Excl_12345 = new TagAndProbeFillerElectron(TagDir_Excl_12345, string(name_TPFiller_12345.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_1234, probe_cuts_12345);
	_TagProbe_Electron_Incl_12345 = new TagAndProbeFillerElectron(TagDir_Incl_12345, string(name_TPFiller_12345.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_1234, probe_cuts_12345);

	name_TPFiller += _RecoCutFlags[6].c_str();
	string name_TPFiller_123456 = name_TPFiller+"_";
	name_TagDir_Excl += _RecoCutFlags[6].c_str();
	TDirectory *TagDir_Excl_123456 = _dir->mkdir(name_TagDir_Excl.c_str());
	name_TagDir_Incl += _RecoCutFlags[6].c_str();
	TDirectory *TagDir_Incl_123456 = _dir->mkdir(name_TagDir_Incl.c_str());

	_TagProbe_Electron_Excl_123456 = new TagAndProbeFillerElectron(TagDir_Excl_123456, string(name_TPFiller_123456.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_12345, probe_cuts_123456);
	_TagProbe_Electron_Incl_123456 = new TagAndProbeFillerElectron(TagDir_Incl_123456, string(name_TPFiller_123456.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_12345, probe_cuts_123456);

	name_TPFiller += _RecoCutFlags[7].c_str();
	string name_TPFiller_1234567 = name_TPFiller+"_";
	name_TagDir_Excl += _RecoCutFlags[7].c_str();
	TDirectory *TagDir_Excl_1234567 = _dir->mkdir(name_TagDir_Excl.c_str());
	name_TagDir_Incl += _RecoCutFlags[7].c_str();
	TDirectory *TagDir_Incl_1234567 = _dir->mkdir(name_TagDir_Incl.c_str());

	_TagProbe_Electron_Excl_1234567 = new TagAndProbeFillerElectron(TagDir_Excl_1234567, string(name_TPFiller_1234567.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_123456, probe_cuts_1234567);
	_TagProbe_Electron_Incl_1234567 = new TagAndProbeFillerElectron(TagDir_Incl_1234567, string(name_TPFiller_1234567.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_123456, probe_cuts_1234567);


	string name_TPFiller_Global = "electron_Global_";
	TDirectory *TagDir_Excl_Global = _dir->mkdir("Tag&Probe_Excl_Global");
	TDirectory *TagDir_Incl_Global = _dir->mkdir("Tag&Probe_Incl_Global");

	_TagProbe_Electron_Excl_Global = new TagAndProbeFillerElectron(TagDir_Excl_Global, string(name_TPFiller_Global.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_1, probe_cuts_1234567);
	_TagProbe_Electron_Incl_Global = new TagAndProbeFillerElectron(TagDir_Incl_Global, string(name_TPFiller_Global.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts, probe_cuts_1, probe_cuts_1234567);

	//Fit del plot di massa di double electron
	string name_TPFiller_Double = "electron_Double_";
	TDirectory *TagDir_Excl_Double = _dir->mkdir("Tag&Probe_Excl_Double");
	TDirectory *TagDir_Incl_Double = _dir->mkdir("Tag&Probe_Incl_Double");

	_TagProbe_Electron_Excl_Double = new TagAndProbeFillerElectron(TagDir_Excl_Double, string(name_TPFiller_Double.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_AllSel, probe_cuts_1, probe_cuts_1234567);
	_TagProbe_Electron_Incl_Double = new TagAndProbeFillerElectron(TagDir_Incl_Double, string(name_TPFiller_Double.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_AllSel, probe_cuts_1, probe_cuts_1234567);



	//Efficienza di singolo leptone in funzione di pt ed eta

	for(int i=0; i<3; i++) _ptBinTP[i] = -1;

	_ptBinTP[0] = 100;	//GeV
	_ptBinTP[1] = 150;	//GeV
	_ptBinTP[2] = 300;	//GeV
//	_ptBinTP[3] = ;	        //GeV

	string name_TagDir_Pt_Barrel = "Tag&Probe_Pt_Barrel";
	string name_TagDir_Pt_Endcap = "Tag&Probe_Pt_Endcap";
	string name_TPFiller_PtEta = "electron";

	name_TagDir_Pt_Barrel += _RecoCutFlags[1].c_str();
	name_TagDir_Pt_Barrel += _RecoCutFlags[2].c_str();
	name_TagDir_Pt_Endcap += _RecoCutFlags[1].c_str();
	name_TagDir_Pt_Endcap += _RecoCutFlags[2].c_str();
	name_TPFiller_PtEta += _RecoCutFlags[1].c_str();
	name_TPFiller_PtEta += _RecoCutFlags[2].c_str();

	string name_TPFiller_PtEta_12 = name_TPFiller_PtEta+"_";
	TDirectory *TagDir_Pt_Barrel_12 = _dir->mkdir(name_TagDir_Pt_Barrel.c_str());
	TDirectory *TagDir_Pt_Endcap_12 = _dir->mkdir(name_TagDir_Pt_Endcap.c_str());

	_TagProbe_Electron_Pt_Barrel_12 = new TagAndProbeFillerElectron(TagDir_Pt_Barrel_12, string(name_TPFiller_PtEta_12.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_1, probe_cuts_12);
	_TagProbe_Electron_Pt_Endcap_12 = new TagAndProbeFillerElectron(TagDir_Pt_Endcap_12, string(name_TPFiller_PtEta_12.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_1, probe_cuts_12);

	name_TPFiller_PtEta += _RecoCutFlags[3].c_str();
	string name_TPFiller_PtEta_123 = name_TPFiller_PtEta+"_"; 
	name_TagDir_Pt_Barrel += _RecoCutFlags[3].c_str();
	TDirectory *TagDir_Pt_Barrel_123 = _dir->mkdir(name_TagDir_Pt_Barrel.c_str());
	name_TagDir_Pt_Endcap += _RecoCutFlags[3].c_str();
	TDirectory *TagDir_Pt_Endcap_123 = _dir->mkdir(name_TagDir_Pt_Endcap.c_str());

	_TagProbe_Electron_Pt_Barrel_123 = new TagAndProbeFillerElectron(TagDir_Pt_Barrel_123, string(name_TPFiller_PtEta_123.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_12, probe_cuts_123);
	_TagProbe_Electron_Pt_Endcap_123 = new TagAndProbeFillerElectron(TagDir_Pt_Endcap_123, string(name_TPFiller_PtEta_123.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_12, probe_cuts_123);

	name_TPFiller_PtEta += _RecoCutFlags[4].c_str();
	string name_TPFiller_PtEta_1234 = name_TPFiller_PtEta+"_"; 
	name_TagDir_Pt_Barrel += _RecoCutFlags[4].c_str();
	TDirectory *TagDir_Pt_Barrel_1234 = _dir->mkdir(name_TagDir_Pt_Barrel.c_str());
	name_TagDir_Pt_Endcap += _RecoCutFlags[4].c_str();
	TDirectory *TagDir_Pt_Endcap_1234 = _dir->mkdir(name_TagDir_Pt_Endcap.c_str());

	_TagProbe_Electron_Pt_Barrel_1234 = new TagAndProbeFillerElectron(TagDir_Pt_Barrel_1234, string(name_TPFiller_PtEta_1234.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_123, probe_cuts_1234);
	_TagProbe_Electron_Pt_Endcap_1234 = new TagAndProbeFillerElectron(TagDir_Pt_Endcap_1234, string(name_TPFiller_PtEta_1234.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_123, probe_cuts_1234);

	name_TPFiller_PtEta += _RecoCutFlags[5].c_str();
	string name_TPFiller_PtEta_12345 = name_TPFiller_PtEta+"_"; 
	name_TagDir_Pt_Barrel += _RecoCutFlags[5].c_str();
	TDirectory *TagDir_Pt_Barrel_12345 = _dir->mkdir(name_TagDir_Pt_Barrel.c_str());
	name_TagDir_Pt_Endcap += _RecoCutFlags[5].c_str();
	TDirectory *TagDir_Pt_Endcap_12345 = _dir->mkdir(name_TagDir_Pt_Endcap.c_str());

	_TagProbe_Electron_Pt_Barrel_12345 = new TagAndProbeFillerElectron(TagDir_Pt_Barrel_12345, string(name_TPFiller_PtEta_12345.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_1234, probe_cuts_12345);
	_TagProbe_Electron_Pt_Endcap_12345 = new TagAndProbeFillerElectron(TagDir_Pt_Endcap_12345, string(name_TPFiller_PtEta_12345.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_1234, probe_cuts_12345);

	name_TPFiller_PtEta += _RecoCutFlags[6].c_str();
	string name_TPFiller_PtEta_123456 = name_TPFiller_PtEta+"_"; 
	name_TagDir_Pt_Barrel += _RecoCutFlags[6].c_str();
	TDirectory *TagDir_Pt_Barrel_123456 = _dir->mkdir(name_TagDir_Pt_Barrel.c_str());
	name_TagDir_Pt_Endcap += _RecoCutFlags[6].c_str();
	TDirectory *TagDir_Pt_Endcap_123456 = _dir->mkdir(name_TagDir_Pt_Endcap.c_str());

	_TagProbe_Electron_Pt_Barrel_123456 = new TagAndProbeFillerElectron(TagDir_Pt_Barrel_123456, string(name_TPFiller_PtEta_123456.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_12345, probe_cuts_123456);
	_TagProbe_Electron_Pt_Endcap_123456 = new TagAndProbeFillerElectron(TagDir_Pt_Endcap_123456, string(name_TPFiller_PtEta_123456.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_12345, probe_cuts_123456);

	name_TPFiller_PtEta += _RecoCutFlags[7].c_str();
	string name_TPFiller_PtEta_1234567 = name_TPFiller_PtEta+"_"; 
	name_TagDir_Pt_Barrel += _RecoCutFlags[7].c_str();
	TDirectory *TagDir_Pt_Barrel_1234567 = _dir->mkdir(name_TagDir_Pt_Barrel.c_str());
	name_TagDir_Pt_Endcap += _RecoCutFlags[7].c_str();
	TDirectory *TagDir_Pt_Endcap_1234567 = _dir->mkdir(name_TagDir_Pt_Endcap.c_str());

	_TagProbe_Electron_Pt_Barrel_1234567 = new TagAndProbeFillerElectron(TagDir_Pt_Barrel_1234567, string(name_TPFiller_PtEta_1234567.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_123456, probe_cuts_1234567);
	_TagProbe_Electron_Pt_Endcap_1234567 = new TagAndProbeFillerElectron(TagDir_Pt_Endcap_1234567, string(name_TPFiller_PtEta_1234567.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_123456, probe_cuts_1234567);


	string name_TPFiller_PtEta_Global = "electron_Global_";
	TDirectory *TagDir_Pt_Barrel_Global = _dir->mkdir("Tag&Probe_Pt_Barrel_Global");
	TDirectory *TagDir_Pt_Endcap_Global = _dir->mkdir("Tag&Probe_Pt_Endcap_Global");

	_TagProbe_Electron_Pt_Barrel_Global = new TagAndProbeFillerElectron(TagDir_Pt_Barrel_Global, string(name_TPFiller_PtEta_Global.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_1, probe_cuts_1234567);
	_TagProbe_Electron_Pt_Endcap_Global = new TagAndProbeFillerElectron(TagDir_Pt_Endcap_Global, string(name_TPFiller_PtEta_Global.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts, probe_cuts_1, probe_cuts_1234567);

	}

	if(_selections=="ASYM"){

		probe_cuts0_1.clear();
		probe_cuts0_12.clear();
		probe_cuts0_123.clear();
		probe_cuts0_1234.clear();
		probe_cuts0_12345.clear();
		probe_cuts0_123456.clear();
		probe_cuts0_1234567.clear();

		for(unsigned int i = 1; i < 8; i++){
			if(_RecoCutFlags[i] == "_AccASYM"){
				if(i<2) probe_cuts0_1.push_back(singleEl_Probe_Acc_ASYM0);
				if(i<3) probe_cuts0_12.push_back(singleEl_Probe_Acc_ASYM0);
				if(i<4) probe_cuts0_123.push_back(singleEl_Probe_Acc_ASYM0);
				if(i<5) probe_cuts0_1234.push_back(singleEl_Probe_Acc_ASYM0);
				if(i<6) probe_cuts0_12345.push_back(singleEl_Probe_Acc_ASYM0);
				if(i<7) probe_cuts0_123456.push_back(singleEl_Probe_Acc_ASYM0);
				if(i<8) probe_cuts0_1234567.push_back(singleEl_Probe_Acc_ASYM0);
			}
			if(_RecoCutFlags[i] == "_Trg"){
				if(i<2) probe_cuts0_1.push_back(singleEl_Probe_Trg_ASYM0);
				if(i<3) probe_cuts0_12.push_back(singleEl_Probe_Trg_ASYM0);
				if(i<4) probe_cuts0_123.push_back(singleEl_Probe_Trg_ASYM0);
				if(i<5) probe_cuts0_1234.push_back(singleEl_Probe_Trg_ASYM0);
				if(i<6) probe_cuts0_12345.push_back(singleEl_Probe_Trg_ASYM0);
				if(i<7) probe_cuts0_123456.push_back(singleEl_Probe_Trg_ASYM0);
				if(i<8) probe_cuts0_1234567.push_back(singleEl_Probe_Trg_ASYM0);
			}
			if(_RecoCutFlags[i] == "_QualASYM"){
				if(i<2) probe_cuts0_1.push_back(singleEl_Probe_Qual_ASYM0);
				if(i<3) probe_cuts0_12.push_back(singleEl_Probe_Qual_ASYM0);
				if(i<4) probe_cuts0_123.push_back(singleEl_Probe_Qual_ASYM0);
				if(i<5) probe_cuts0_1234.push_back(singleEl_Probe_Qual_ASYM0);
				if(i<6) probe_cuts0_12345.push_back(singleEl_Probe_Qual_ASYM0);
				if(i<7) probe_cuts0_123456.push_back(singleEl_Probe_Qual_ASYM0);
				if(i<8) probe_cuts0_1234567.push_back(singleEl_Probe_Qual_ASYM0);
			} 
			if(_RecoCutFlags[i] == "_Imp"){
				if(i<2) probe_cuts0_1.push_back(singleEl_Probe_Imp_ASYM0);
				if(i<3) probe_cuts0_12.push_back(singleEl_Probe_Imp_ASYM0);
				if(i<4) probe_cuts0_123.push_back(singleEl_Probe_Imp_ASYM0);
				if(i<5) probe_cuts0_1234.push_back(singleEl_Probe_Imp_ASYM0);
				if(i<6) probe_cuts0_12345.push_back(singleEl_Probe_Imp_ASYM0);
				if(i<7) probe_cuts0_123456.push_back(singleEl_Probe_Imp_ASYM0);
				if(i<8) probe_cuts0_1234567.push_back(singleEl_Probe_Imp_ASYM0);
			}

			if(_RecoCutFlags[i] == "_IsoASYM"){
				if(i<2) probe_cuts0_1.push_back(singleEl_Probe_Iso_ASYM0);
				if(i<3) probe_cuts0_12.push_back(singleEl_Probe_Iso_ASYM0);
				if(i<4) probe_cuts0_123.push_back(singleEl_Probe_Iso_ASYM0);
				if(i<5) probe_cuts0_1234.push_back(singleEl_Probe_Iso_ASYM0);
				if(i<6) probe_cuts0_12345.push_back(singleEl_Probe_Iso_ASYM0);
				if(i<7) probe_cuts0_123456.push_back(singleEl_Probe_Iso_ASYM0);
				if(i<8) probe_cuts0_1234567.push_back(singleEl_Probe_Iso_ASYM0);
			}
/*
			if(_RecoCutFlags[i] == "_IsoPhotonASYM"){
				if(i<2) probe_cuts0_1.push_back(singleEl_Probe_IsoPhoton_ASYM0);
				if(i<3) probe_cuts0_12.push_back(singleEl_Probe_IsoPhoton_ASYM0);
				if(i<4) probe_cuts0_123.push_back(singleEl_Probe_IsoPhoton_ASYM0);
				if(i<5) probe_cuts0_1234.push_back(singleEl_Probe_IsoPhoton_ASYM0);
				if(i<6) probe_cuts0_12345.push_back(singleEl_Probe_IsoPhoton_ASYM0);
				if(i<7) probe_cuts0_123456.push_back(singleEl_Probe_IsoPhoton_ASYM0);
				if(i<8) probe_cuts0_1234567.push_back(singleEl_Probe_IsoPhoton_ASYM0);
			}
*/
			if(_RecoCutFlags[i] == "_ElIDASYM"){
				if(i<2) probe_cuts0_1.push_back(singleEl_Probe_ElID_ASYM0);
				if(i<3) probe_cuts0_12.push_back(singleEl_Probe_ElID_ASYM0);
				if(i<4) probe_cuts0_123.push_back(singleEl_Probe_ElID_ASYM0);
				if(i<5) probe_cuts0_1234.push_back(singleEl_Probe_ElID_ASYM0);
				if(i<6) probe_cuts0_12345.push_back(singleEl_Probe_ElID_ASYM0);
				if(i<7) probe_cuts0_123456.push_back(singleEl_Probe_ElID_ASYM0);
				if(i<8) probe_cuts0_1234567.push_back(singleEl_Probe_ElID_ASYM0);
			}
/*
			if(_RecoCutFlags[i] == "_ElIDPhotonASYM"){
				if(i<2) probe_cuts0_1.push_back(singleEl_Probe_ElIDPhoton_ASYM0);
				if(i<3) probe_cuts0_12.push_back(singleEl_Probe_ElIDPhoton_ASYM0);
				if(i<4) probe_cuts0_123.push_back(singleEl_Probe_ElIDPhoton_ASYM0);
				if(i<5) probe_cuts0_1234.push_back(singleEl_Probe_ElIDPhoton_ASYM0);
				if(i<6) probe_cuts0_12345.push_back(singleEl_Probe_ElIDPhoton_ASYM0);
				if(i<7) probe_cuts0_123456.push_back(singleEl_Probe_ElIDPhoton_ASYM0);
				if(i<8) probe_cuts0_1234567.push_back(singleEl_Probe_ElIDPhoton_ASYM0);
			}
*/
			if(_RecoCutFlags[i] == "_ConvASYM"){
				if(i<2) probe_cuts0_1.push_back(singleEl_Probe_Conv_ASYM0);
				if(i<3) probe_cuts0_12.push_back(singleEl_Probe_Conv_ASYM0);
				if(i<4) probe_cuts0_123.push_back(singleEl_Probe_Conv_ASYM0);
				if(i<5) probe_cuts0_1234.push_back(singleEl_Probe_Conv_ASYM0);
				if(i<6) probe_cuts0_12345.push_back(singleEl_Probe_Conv_ASYM0);
				if(i<7) probe_cuts0_123456.push_back(singleEl_Probe_Conv_ASYM0);
				if(i<8) probe_cuts0_1234567.push_back(singleEl_Probe_Conv_ASYM0);
			}
			if(_RecoCutFlags[i] == "_1"){
				if(i<2) probe_cuts0_1.push_back(singleEl_Probe_True);
				if(i<3) probe_cuts0_12.push_back(singleEl_Probe_True);
				if(i<4) probe_cuts0_123.push_back(singleEl_Probe_True);
				if(i<5) probe_cuts0_1234.push_back(singleEl_Probe_True);
				if(i<6) probe_cuts0_12345.push_back(singleEl_Probe_True);
				if(i<7) probe_cuts0_123456.push_back(singleEl_Probe_True);
				if(i<8) probe_cuts0_1234567.push_back(singleEl_Probe_True);			}
		}

		string name_TagDir0_Excl = "Tag&Probe0_Excl";
		string name_TagDir0_Incl = "Tag&Probe0_Incl";
		string name_TPFiller0 = "electron";

		name_TagDir0_Excl += _RecoCutFlags[1].c_str();
		name_TagDir0_Excl += _RecoCutFlags[2].c_str();
		name_TagDir0_Incl += _RecoCutFlags[1].c_str();
		name_TagDir0_Incl += _RecoCutFlags[2].c_str();
		name_TPFiller0 += _RecoCutFlags[1].c_str();
		name_TPFiller0 += _RecoCutFlags[2].c_str();

		string name_TPFiller0_12 = name_TPFiller0+"_";
		TDirectory *TagDir0_Excl_12 = _dir->mkdir(name_TagDir0_Excl.c_str());
		TDirectory *TagDir0_Incl_12 = _dir->mkdir(name_TagDir0_Incl.c_str());

		_TagProbe_Electron0_Excl_12 = new TagAndProbeFillerElectron(TagDir0_Excl_12, string(name_TPFiller0_12.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_12, "hard");
		_TagProbe_Electron0_Incl_12 = new TagAndProbeFillerElectron(TagDir0_Incl_12, string(name_TPFiller0_12.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_12, "hard");

		name_TPFiller0 += _RecoCutFlags[3].c_str();
		string name_TPFiller0_123 = name_TPFiller0+"_";
		name_TagDir0_Excl += _RecoCutFlags[3].c_str(); 
		TDirectory *TagDir0_Excl_123 = _dir->mkdir(name_TagDir0_Excl.c_str());
		name_TagDir0_Incl += _RecoCutFlags[3].c_str(); 
		TDirectory *TagDir0_Incl_123 = _dir->mkdir(name_TagDir0_Incl.c_str());

		_TagProbe_Electron0_Excl_123 = new TagAndProbeFillerElectron(TagDir0_Excl_123, string(name_TPFiller0_123.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_12, probe_cuts0_123, "hard");
		_TagProbe_Electron0_Incl_123 = new TagAndProbeFillerElectron(TagDir0_Incl_123, string(name_TPFiller0_123.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_12, probe_cuts0_123, "hard");

		name_TPFiller0 += _RecoCutFlags[4].c_str();
		string name_TPFiller0_1234 = name_TPFiller0+"_";
		name_TagDir0_Excl += _RecoCutFlags[4].c_str();
		TDirectory *TagDir0_Excl_1234 = _dir->mkdir(name_TagDir0_Excl.c_str());
		name_TagDir0_Incl += _RecoCutFlags[4].c_str();
		TDirectory *TagDir0_Incl_1234 = _dir->mkdir(name_TagDir0_Incl.c_str());

		_TagProbe_Electron0_Excl_1234 = new TagAndProbeFillerElectron(TagDir0_Excl_1234, string(name_TPFiller0_1234.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_123, probe_cuts0_1234, "hard");
		_TagProbe_Electron0_Incl_1234 = new TagAndProbeFillerElectron(TagDir0_Incl_1234, string(name_TPFiller0_1234.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_123, probe_cuts0_1234, "hard");

		name_TPFiller0 += _RecoCutFlags[5].c_str();
		string name_TPFiller0_12345 = name_TPFiller0+"_";
		name_TagDir0_Excl += _RecoCutFlags[5].c_str();
		TDirectory *TagDir0_Excl_12345 = _dir->mkdir(name_TagDir0_Excl.c_str());
		name_TagDir0_Incl += _RecoCutFlags[5].c_str();
		TDirectory *TagDir0_Incl_12345 = _dir->mkdir(name_TagDir0_Incl.c_str());

		_TagProbe_Electron0_Excl_12345 = new TagAndProbeFillerElectron(TagDir0_Excl_12345, string(name_TPFiller0_12345.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_1234, probe_cuts0_12345, "hard");
		_TagProbe_Electron0_Incl_12345 = new TagAndProbeFillerElectron(TagDir0_Incl_12345, string(name_TPFiller0_12345.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_1234, probe_cuts0_12345, "hard");

		name_TPFiller0 += _RecoCutFlags[6].c_str();
		string name_TPFiller0_123456 = name_TPFiller0+"_";
		name_TagDir0_Excl += _RecoCutFlags[6].c_str(); 
		TDirectory *TagDir0_Excl_123456 = _dir->mkdir(name_TagDir0_Excl.c_str());
		name_TagDir0_Incl += _RecoCutFlags[6].c_str(); 
		TDirectory *TagDir0_Incl_123456 = _dir->mkdir(name_TagDir0_Incl.c_str());

		_TagProbe_Electron0_Excl_123456 = new TagAndProbeFillerElectron(TagDir0_Excl_123456, string(name_TPFiller0_123456.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_12345, probe_cuts0_123456, "hard");
		_TagProbe_Electron0_Incl_123456 = new TagAndProbeFillerElectron(TagDir0_Incl_123456, string(name_TPFiller0_123456.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_12345, probe_cuts0_123456, "hard");


		name_TPFiller0 += _RecoCutFlags[7].c_str();
		string name_TPFiller0_1234567 = name_TPFiller0+"_";
		name_TagDir0_Excl += _RecoCutFlags[7].c_str(); 
		TDirectory *TagDir0_Excl_1234567 = _dir->mkdir(name_TagDir0_Excl.c_str());
		name_TagDir0_Incl += _RecoCutFlags[7].c_str(); 
		TDirectory *TagDir0_Incl_1234567 = _dir->mkdir(name_TagDir0_Incl.c_str());

		_TagProbe_Electron0_Excl_1234567 = new TagAndProbeFillerElectron(TagDir0_Excl_1234567, string(name_TPFiller0_1234567.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_123456, probe_cuts0_1234567, "hard");
		_TagProbe_Electron0_Incl_123456 = new TagAndProbeFillerElectron(TagDir0_Incl_1234567, string(name_TPFiller0_1234567.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_123456, probe_cuts0_1234567, "hard");


		string name_TPFiller0_Global = "electron_Global_";
		TDirectory *TagDir0_Excl_Global = _dir->mkdir("Tag&Probe0_Excl_Global");
		TDirectory *TagDir0_Incl_Global = _dir->mkdir("Tag&Probe0_Incl_Global");

		_TagProbe_Electron0_Excl_Global = new TagAndProbeFillerElectron(TagDir0_Excl_Global, string(name_TPFiller0_Global.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_1234567, "hard");
		_TagProbe_Electron0_Incl_Global = new TagAndProbeFillerElectron(TagDir0_Incl_Global, string(name_TPFiller0_Global.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_1234567, "hard");

		//Fit del plot di massa di double electron
		string name_TPFiller0_Double = "electron_Double_";
		TDirectory *TagDir0_Excl_Double = _dir->mkdir("Tag&Probe0_Excl_Double");
		TDirectory *TagDir0_Incl_Double = _dir->mkdir("Tag&Probe0_Incl_Double");

		_TagProbe_Electron0_Excl_Double = new TagAndProbeFillerElectron(TagDir0_Excl_Double, string(name_TPFiller0_Double.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1_AllSel, probe_cuts0_1, probe_cuts0_1234567, "hard");
		_TagProbe_Electron0_Incl_Double = new TagAndProbeFillerElectron(TagDir0_Incl_Double, string(name_TPFiller0_Double.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_1_AllSel, probe_cuts0_1, probe_cuts0_1234567, "hard");

		//Efficienza di singolo leptone in funzione di pt ed eta

		for(int i=0; i<3; i++) _ptBinTP0[i] = -1;

		_ptBinTP0[0] = 100;	//GeV
		_ptBinTP0[1] = 150;	//GeV
		_ptBinTP0[2] = 300;	//GeV
//		_ptBinTP0[3] = ;	//GeV

		string name_TagDir0_Pt_Barrel = "Tag&Probe0_Pt_Barrel";
		string name_TagDir0_Pt_Endcap = "Tag&Probe0_Pt_Endcap";
		string name_TPFiller0_PtEta = "electron";

		name_TagDir0_Pt_Barrel += _RecoCutFlags[1].c_str();
		name_TagDir0_Pt_Barrel += _RecoCutFlags[2].c_str();
		name_TagDir0_Pt_Endcap += _RecoCutFlags[1].c_str();
		name_TagDir0_Pt_Endcap += _RecoCutFlags[2].c_str();
		name_TPFiller0_PtEta += _RecoCutFlags[1].c_str();
		name_TPFiller0_PtEta += _RecoCutFlags[2].c_str();

		string name_TPFiller0_PtEta_12 = name_TPFiller0_PtEta+"_";
		TDirectory *TagDir0_Pt_Barrel_12 = _dir->mkdir(name_TagDir0_Pt_Barrel.c_str());
		TDirectory *TagDir0_Pt_Endcap_12 = _dir->mkdir(name_TagDir0_Pt_Endcap.c_str());

		_TagProbe_Electron0_Pt_Barrel_12 = new TagAndProbeFillerElectron(TagDir0_Pt_Barrel_12, string(name_TPFiller0_PtEta_12.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_12, "hard");
		_TagProbe_Electron0_Pt_Endcap_12 = new TagAndProbeFillerElectron(TagDir0_Pt_Endcap_12, string(name_TPFiller0_PtEta_12.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_12, "hard");

		name_TPFiller0_PtEta += _RecoCutFlags[3].c_str();
		string name_TPFiller0_PtEta_123 = name_TPFiller0_PtEta+"_"; 
		name_TagDir0_Pt_Barrel += _RecoCutFlags[3].c_str();
		TDirectory *TagDir0_Pt_Barrel_123 = _dir->mkdir(name_TagDir0_Pt_Barrel.c_str());
		name_TagDir0_Pt_Endcap += _RecoCutFlags[3].c_str();
		TDirectory *TagDir0_Pt_Endcap_123 = _dir->mkdir(name_TagDir0_Pt_Endcap.c_str());

		_TagProbe_Electron0_Pt_Barrel_123 = new TagAndProbeFillerElectron(TagDir0_Pt_Barrel_123, string(name_TPFiller0_PtEta_123.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_12, probe_cuts0_123, "hard");
		_TagProbe_Electron0_Pt_Endcap_123 = new TagAndProbeFillerElectron(TagDir0_Pt_Endcap_123, string(name_TPFiller0_PtEta_123.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_12, probe_cuts0_123, "hard");

		name_TPFiller0_PtEta += _RecoCutFlags[4].c_str();
		string name_TPFiller0_PtEta_1234 = name_TPFiller0_PtEta+"_"; 
		name_TagDir0_Pt_Barrel += _RecoCutFlags[4].c_str();
		TDirectory *TagDir0_Pt_Barrel_1234 = _dir->mkdir(name_TagDir0_Pt_Barrel.c_str());
		name_TagDir0_Pt_Endcap += _RecoCutFlags[4].c_str();
		TDirectory *TagDir0_Pt_Endcap_1234 = _dir->mkdir(name_TagDir0_Pt_Endcap.c_str());

		_TagProbe_Electron0_Pt_Barrel_1234 = new TagAndProbeFillerElectron(TagDir0_Pt_Barrel_1234, string(name_TPFiller0_PtEta_1234.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_123, probe_cuts0_1234, "hard");
		_TagProbe_Electron0_Pt_Endcap_1234 = new TagAndProbeFillerElectron(TagDir0_Pt_Endcap_1234, string(name_TPFiller0_PtEta_1234.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_123, probe_cuts0_1234, "hard");

		name_TPFiller0_PtEta += _RecoCutFlags[5].c_str();
		string name_TPFiller0_PtEta_12345 = name_TPFiller0_PtEta+"_"; 
		name_TagDir0_Pt_Barrel += _RecoCutFlags[5].c_str();
		TDirectory *TagDir0_Pt_Barrel_12345 = _dir->mkdir(name_TagDir0_Pt_Barrel.c_str());
		name_TagDir0_Pt_Endcap += _RecoCutFlags[5].c_str();
		TDirectory *TagDir0_Pt_Endcap_12345 = _dir->mkdir(name_TagDir0_Pt_Endcap.c_str());

		_TagProbe_Electron0_Pt_Barrel_12345 = new TagAndProbeFillerElectron(TagDir0_Pt_Barrel_12345, string(name_TPFiller0_PtEta_12345.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_1234, probe_cuts0_12345, "hard");
		_TagProbe_Electron0_Pt_Endcap_12345 = new TagAndProbeFillerElectron(TagDir0_Pt_Endcap_12345, string(name_TPFiller0_PtEta_12345.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_1234, probe_cuts0_12345, "hard");

		name_TPFiller0_PtEta += _RecoCutFlags[6].c_str();
		string name_TPFiller0_PtEta_123456 = name_TPFiller0_PtEta+"_"; 
		name_TagDir0_Pt_Barrel += _RecoCutFlags[6].c_str();
		TDirectory *TagDir0_Pt_Barrel_123456 = _dir->mkdir(name_TagDir0_Pt_Barrel.c_str());
		name_TagDir0_Pt_Endcap += _RecoCutFlags[6].c_str();
		TDirectory *TagDir0_Pt_Endcap_123456 = _dir->mkdir(name_TagDir0_Pt_Endcap.c_str());

		_TagProbe_Electron0_Pt_Barrel_123456 = new TagAndProbeFillerElectron(TagDir0_Pt_Barrel_123456, string(name_TPFiller0_PtEta_123456.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_12345, probe_cuts0_123456, "hard");
		_TagProbe_Electron0_Pt_Endcap_123456 = new TagAndProbeFillerElectron(TagDir0_Pt_Endcap_123456, string(name_TPFiller0_PtEta_123456.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_12345, probe_cuts0_123456, "hard");


		name_TPFiller0_PtEta += _RecoCutFlags[7].c_str();
		string name_TPFiller0_PtEta_1234567 = name_TPFiller0_PtEta+"_"; 
		name_TagDir0_Pt_Barrel += _RecoCutFlags[7].c_str();
		TDirectory *TagDir0_Pt_Barrel_1234567 = _dir->mkdir(name_TagDir0_Pt_Barrel.c_str());
		name_TagDir0_Pt_Endcap += _RecoCutFlags[7].c_str();
		TDirectory *TagDir0_Pt_Endcap_1234567 = _dir->mkdir(name_TagDir0_Pt_Endcap.c_str());

		_TagProbe_Electron0_Pt_Barrel_1234567 = new TagAndProbeFillerElectron(TagDir0_Pt_Barrel_1234567, string(name_TPFiller0_PtEta_1234567.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_123456, probe_cuts0_1234567, "hard");
		_TagProbe_Electron0_Pt_Endcap_1234567 = new TagAndProbeFillerElectron(TagDir0_Pt_Endcap_1234567, string(name_TPFiller0_PtEta_1234567.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_123456, probe_cuts0_1234567, "hard");


		string name_TPFiller0_PtEta_Global = "electron_Global_";
		TDirectory *TagDir0_Pt_Barrel_Global = _dir->mkdir("Tag&Probe0_Pt_Barrel_Global");
		TDirectory *TagDir0_Pt_Endcap_Global = _dir->mkdir("Tag&Probe0_Pt_Endcap_Global");

		_TagProbe_Electron0_Pt_Barrel_Global = new TagAndProbeFillerElectron(TagDir0_Pt_Barrel_Global, string(name_TPFiller0_PtEta_Global.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_1234567, "hard");
		_TagProbe_Electron0_Pt_Endcap_Global = new TagAndProbeFillerElectron(TagDir0_Pt_Endcap_Global, string(name_TPFiller0_PtEta_Global.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_1234567, "hard");


		probe_cuts1_1.clear();
		probe_cuts1_12.clear();
		probe_cuts1_123.clear();
		probe_cuts1_1234.clear();
		probe_cuts1_12345.clear();
		probe_cuts1_123456.clear();
		probe_cuts1_1234567.clear();

		for(unsigned int i = 1; i < 8; i++){
			if(_RecoCutFlags[i] == "_AccASYM"){
				if(i<2) probe_cuts1_1.push_back(singleEl_Probe_Acc_ASYM1);
				if(i<3) probe_cuts1_12.push_back(singleEl_Probe_Acc_ASYM1);
				if(i<4) probe_cuts1_123.push_back(singleEl_Probe_Acc_ASYM1);
				if(i<5) probe_cuts1_1234.push_back(singleEl_Probe_Acc_ASYM1);
				if(i<6) probe_cuts1_12345.push_back(singleEl_Probe_Acc_ASYM1);
				if(i<7) probe_cuts1_123456.push_back(singleEl_Probe_Acc_ASYM1);
				if(i<8) probe_cuts1_1234567.push_back(singleEl_Probe_Acc_ASYM1);
			}
			if(_RecoCutFlags[i] == "_Trg"){
				if(i<2) probe_cuts1_1.push_back(singleEl_Probe_Trg_ASYM1);
				if(i<3) probe_cuts1_12.push_back(singleEl_Probe_Trg_ASYM1);
				if(i<4) probe_cuts1_123.push_back(singleEl_Probe_Trg_ASYM1);
				if(i<5) probe_cuts1_1234.push_back(singleEl_Probe_Trg_ASYM1);
				if(i<6) probe_cuts1_12345.push_back(singleEl_Probe_Trg_ASYM1);
				if(i<7) probe_cuts1_123456.push_back(singleEl_Probe_Trg_ASYM1);
				if(i<8) probe_cuts1_1234567.push_back(singleEl_Probe_Trg_ASYM1);
			}
			if(_RecoCutFlags[i] == "_QualASYM"){
				if(i<2) probe_cuts1_1.push_back(singleEl_Probe_Qual_ASYM1);
				if(i<3) probe_cuts1_12.push_back(singleEl_Probe_Qual_ASYM1);
				if(i<4) probe_cuts1_123.push_back(singleEl_Probe_Qual_ASYM1);
				if(i<5) probe_cuts1_1234.push_back(singleEl_Probe_Qual_ASYM1);
				if(i<6) probe_cuts1_12345.push_back(singleEl_Probe_Qual_ASYM1);
				if(i<7) probe_cuts1_123456.push_back(singleEl_Probe_Qual_ASYM1);
				if(i<8) probe_cuts1_1234567.push_back(singleEl_Probe_Qual_ASYM1);
			}
			if(_RecoCutFlags[i] == "_Imp"){
				if(i<2) probe_cuts1_1.push_back(singleEl_Probe_Imp_ASYM1);
				if(i<3) probe_cuts1_12.push_back(singleEl_Probe_Imp_ASYM1);
				if(i<4) probe_cuts1_123.push_back(singleEl_Probe_Imp_ASYM1);
				if(i<5) probe_cuts1_1234.push_back(singleEl_Probe_Imp_ASYM1);
				if(i<6) probe_cuts1_12345.push_back(singleEl_Probe_Imp_ASYM1);
				if(i<7) probe_cuts1_123456.push_back(singleEl_Probe_Imp_ASYM1);
				if(i<8) probe_cuts1_1234567.push_back(singleEl_Probe_Imp_ASYM1);
			}

			if(_RecoCutFlags[i] == "_IsoASYM"){
				if(i<2) probe_cuts1_1.push_back(singleEl_Probe_Iso_ASYM1);
				if(i<3) probe_cuts1_12.push_back(singleEl_Probe_Iso_ASYM1);
				if(i<4) probe_cuts1_123.push_back(singleEl_Probe_Iso_ASYM1);
				if(i<5) probe_cuts1_1234.push_back(singleEl_Probe_Iso_ASYM1);
				if(i<6) probe_cuts1_12345.push_back(singleEl_Probe_Iso_ASYM1);
				if(i<7) probe_cuts1_123456.push_back(singleEl_Probe_Iso_ASYM1);
				if(i<8) probe_cuts1_1234567.push_back(singleEl_Probe_Iso_ASYM1);
			}
/*
			if(_RecoCutFlags[i] == "_IsoPhotonASYM"){
				if(i<2) probe_cuts1_1.push_back(singleEl_Probe_IsoPhoton_ASYM1);
				if(i<3) probe_cuts1_12.push_back(singleEl_Probe_IsoPhoton_ASYM1);
				if(i<4) probe_cuts1_123.push_back(singleEl_Probe_IsoPhoton_ASYM1);
				if(i<5) probe_cuts1_1234.push_back(singleEl_Probe_IsoPhoton_ASYM1);
				if(i<6) probe_cuts1_12345.push_back(singleEl_Probe_IsoPhoton_ASYM1);
				if(i<7) probe_cuts1_123456.push_back(singleEl_Probe_IsoPhoton_ASYM1);
				if(i<8) probe_cuts1_1234567.push_back(singleEl_Probe_IsoPhoton_ASYM1);
			}
*/
			if(_RecoCutFlags[i] == "_ElIDASYM"){
				if(i<2) probe_cuts1_1.push_back(singleEl_Probe_ElID_ASYM1);
				if(i<3) probe_cuts1_12.push_back(singleEl_Probe_ElID_ASYM1);
				if(i<4) probe_cuts1_123.push_back(singleEl_Probe_ElID_ASYM1);
				if(i<5) probe_cuts1_1234.push_back(singleEl_Probe_ElID_ASYM1);
				if(i<6) probe_cuts1_12345.push_back(singleEl_Probe_ElID_ASYM1);
				if(i<7) probe_cuts1_123456.push_back(singleEl_Probe_ElID_ASYM1);
				if(i<8) probe_cuts1_1234567.push_back(singleEl_Probe_ElID_ASYM1);
			}
/*
			if(_RecoCutFlags[i] == "_ElIDPhotonASYM"){
				if(i<2) probe_cuts1_1.push_back(singleEl_Probe_ElIDPhoton_ASYM1);
				if(i<3) probe_cuts1_12.push_back(singleEl_Probe_ElIDPhoton_ASYM1);
				if(i<4) probe_cuts1_123.push_back(singleEl_Probe_ElIDPhoton_ASYM1);
				if(i<5) probe_cuts1_1234.push_back(singleEl_Probe_ElIDPhoton_ASYM1);
				if(i<6) probe_cuts1_12345.push_back(singleEl_Probe_ElIDPhoton_ASYM1);
				if(i<7) probe_cuts1_123456.push_back(singleEl_Probe_ElIDPhoton_ASYM1);
				if(i<8) probe_cuts1_1234567.push_back(singleEl_Probe_ElIDPhoton_ASYM1);
			}
*/
			if(_RecoCutFlags[i] == "_ConvASYM"){
				if(i<2) probe_cuts1_1.push_back(singleEl_Probe_Conv_ASYM1);
				if(i<3) probe_cuts1_12.push_back(singleEl_Probe_Conv_ASYM1);
				if(i<4) probe_cuts1_123.push_back(singleEl_Probe_Conv_ASYM1);
				if(i<5) probe_cuts1_1234.push_back(singleEl_Probe_Conv_ASYM1);
				if(i<6) probe_cuts1_12345.push_back(singleEl_Probe_Conv_ASYM1);
				if(i<7) probe_cuts1_123456.push_back(singleEl_Probe_Conv_ASYM1);
				if(i<8) probe_cuts1_1234567.push_back(singleEl_Probe_Conv_ASYM1);
			}
			if(_RecoCutFlags[i] == "_1"){
				if(i<2) probe_cuts1_1.push_back(singleEl_Probe_True);
				if(i<3) probe_cuts1_12.push_back(singleEl_Probe_True);
				if(i<4) probe_cuts1_123.push_back(singleEl_Probe_True);
				if(i<5) probe_cuts1_1234.push_back(singleEl_Probe_True);
				if(i<6) probe_cuts1_12345.push_back(singleEl_Probe_True);
				if(i<7) probe_cuts1_123456.push_back(singleEl_Probe_True);
				if(i<8) probe_cuts1_1234567.push_back(singleEl_Probe_True);
			}
		}

		string name_TagDir1_Excl = "Tag&Probe1_Excl";
		string name_TagDir1_Incl = "Tag&Probe1_Incl";
		string name_TPFiller1 = "electron";

		name_TPFiller1 += _RecoCutFlags[1].c_str();
		name_TPFiller1 += _RecoCutFlags[2].c_str();
		name_TagDir1_Excl += _RecoCutFlags[1].c_str();
		name_TagDir1_Excl += _RecoCutFlags[2].c_str();
		name_TagDir1_Incl += _RecoCutFlags[1].c_str();
		name_TagDir1_Incl += _RecoCutFlags[2].c_str();

		string name_TPFiller1_12 = name_TPFiller1+"_";
		TDirectory *TagDir1_Excl_12 = _dir->mkdir(name_TagDir1_Excl.c_str());
		TDirectory *TagDir1_Incl_12 = _dir->mkdir(name_TagDir1_Incl.c_str());

		_TagProbe_Electron1_Excl_12 = new TagAndProbeFillerElectron(TagDir1_Excl_12, string(name_TPFiller1_12.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_12, "soft");
		_TagProbe_Electron1_Incl_12 = new TagAndProbeFillerElectron(TagDir1_Incl_12, string(name_TPFiller1_12.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_12, "soft");
	
		name_TPFiller1 += _RecoCutFlags[3].c_str();
		string name_TPFiller1_123 = name_TPFiller1+"_";
		name_TagDir1_Excl += _RecoCutFlags[3].c_str();
		TDirectory *TagDir1_Excl_123 = _dir->mkdir(name_TagDir1_Excl.c_str());
		name_TagDir1_Incl += _RecoCutFlags[3].c_str();
		TDirectory *TagDir1_Incl_123 = _dir->mkdir(name_TagDir1_Incl.c_str());

		_TagProbe_Electron1_Excl_123 = new TagAndProbeFillerElectron(TagDir1_Excl_123, string(name_TPFiller1_123.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_12, probe_cuts1_123, "soft");
		_TagProbe_Electron1_Incl_123 = new TagAndProbeFillerElectron(TagDir1_Incl_123, string(name_TPFiller1_123.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_12, probe_cuts1_123, "soft");

		name_TPFiller1 += _RecoCutFlags[4].c_str();
		string name_TPFiller1_1234 = name_TPFiller1+"_";
		name_TagDir1_Excl += _RecoCutFlags[4].c_str();
		TDirectory *TagDir1_Excl_1234 = _dir->mkdir(name_TagDir1_Excl.c_str());
		name_TagDir1_Incl += _RecoCutFlags[4].c_str();
		TDirectory *TagDir1_Incl_1234 = _dir->mkdir(name_TagDir1_Incl.c_str());

		_TagProbe_Electron1_Excl_1234 = new TagAndProbeFillerElectron(TagDir1_Excl_1234, string(name_TPFiller1_1234.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_123, probe_cuts1_1234, "soft");
		_TagProbe_Electron1_Incl_1234 = new TagAndProbeFillerElectron(TagDir1_Incl_1234, string(name_TPFiller1_1234.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_123, probe_cuts1_1234, "soft");

		name_TPFiller1 += _RecoCutFlags[5].c_str();
		string name_TPFiller1_12345 = name_TPFiller1+"_";
		name_TagDir1_Excl += _RecoCutFlags[5].c_str();
		TDirectory *TagDir1_Excl_12345 = _dir->mkdir(name_TagDir1_Excl.c_str());
		name_TagDir1_Incl += _RecoCutFlags[5].c_str();
		TDirectory *TagDir1_Incl_12345 = _dir->mkdir(name_TagDir1_Incl.c_str());

		_TagProbe_Electron1_Excl_12345 = new TagAndProbeFillerElectron(TagDir1_Excl_12345, string(name_TPFiller1_12345.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_1234, probe_cuts1_12345, "soft");
		_TagProbe_Electron1_Incl_12345 = new TagAndProbeFillerElectron(TagDir1_Incl_12345, string(name_TPFiller1_12345.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_1234, probe_cuts1_12345, "soft");

		name_TPFiller1 += _RecoCutFlags[6].c_str();
		string name_TPFiller1_123456 = name_TPFiller1+"_";
		name_TagDir1_Excl += _RecoCutFlags[6].c_str();
		TDirectory *TagDir1_Excl_123456 = _dir->mkdir(name_TagDir1_Excl.c_str());
		name_TagDir1_Incl += _RecoCutFlags[6].c_str();
		TDirectory *TagDir1_Incl_123456 = _dir->mkdir(name_TagDir1_Incl.c_str());

		_TagProbe_Electron1_Excl_123456 = new TagAndProbeFillerElectron(TagDir1_Excl_123456, string(name_TPFiller1_123456.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_12345, probe_cuts1_123456, "soft");
		_TagProbe_Electron1_Incl_123456 = new TagAndProbeFillerElectron(TagDir1_Incl_123456, string(name_TPFiller1_123456.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_12345, probe_cuts1_123456, "soft");

		name_TPFiller1 += _RecoCutFlags[7].c_str();
		string name_TPFiller1_1234567 = name_TPFiller1+"_";
		name_TagDir1_Excl += _RecoCutFlags[7].c_str();
		TDirectory *TagDir1_Excl_1234567 = _dir->mkdir(name_TagDir1_Excl.c_str());
		name_TagDir1_Incl += _RecoCutFlags[7].c_str();
		TDirectory *TagDir1_Incl_1234567 = _dir->mkdir(name_TagDir1_Incl.c_str());

		_TagProbe_Electron1_Excl_1234567 = new TagAndProbeFillerElectron(TagDir1_Excl_1234567, string(name_TPFiller1_1234567.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_123456, probe_cuts1_1234567, "soft");
		_TagProbe_Electron1_Incl_1234567 = new TagAndProbeFillerElectron(TagDir1_Incl_1234567, string(name_TPFiller1_1234567.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_123456, probe_cuts1_1234567, "soft");


		string name_TPFiller1_Global = "electron_Global_";
		TDirectory *TagDir1_Excl_Global = _dir->mkdir("Tag&Probe1_Excl_Global");
		TDirectory *TagDir1_Incl_Global = _dir->mkdir("Tag&Probe1_Incl_Global");

		_TagProbe_Electron1_Excl_Global = new TagAndProbeFillerElectron(TagDir1_Excl_Global, string(name_TPFiller1_Global.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_1234567, "soft");
		_TagProbe_Electron1_Incl_Global = new TagAndProbeFillerElectron(TagDir1_Incl_Global, string(name_TPFiller1_Global.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_1234567, "soft");

		//Fit del plot di massa di double electron

		string name_TPFiller1_Double = "electron_Double_";
		TDirectory *TagDir1_Excl_Double = _dir->mkdir("Tag&Probe1_Excl_Double");
		TDirectory *TagDir1_Incl_Double = _dir->mkdir("Tag&Probe1_Incl_Double");

		_TagProbe_Electron1_Excl_Double = new TagAndProbeFillerElectron(TagDir1_Excl_Double, string(name_TPFiller1_Double.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0_AllSel, probe_cuts1_1, probe_cuts1_1234567, "soft");
		_TagProbe_Electron1_Incl_Double = new TagAndProbeFillerElectron(TagDir1_Incl_Double, string(name_TPFiller1_Double.c_str()), nbin_mult, mult_xmin, mult_xmax, tag_cuts_0_AllSel, probe_cuts1_1, probe_cuts1_1234567, "soft");

		for(int i=0; i<3; i++) _ptBinTP1[i] = -1;

		_ptBinTP1[0] = 100;	//GeV
		_ptBinTP1[1] = 150;	//GeV
		_ptBinTP1[2] = 300;	//GeV
//		_ptBinTP1[3] = ;	//GeV

		string name_TagDir1_Pt_Barrel = "Tag&Probe1_Pt_Barrel";
		string name_TagDir1_Pt_Endcap = "Tag&Probe1_Pt_Endcap";
		string name_TPFiller1_PtEta = "electron";

		name_TagDir1_Pt_Barrel += _RecoCutFlags[1].c_str();
		name_TagDir1_Pt_Barrel += _RecoCutFlags[2].c_str();
		name_TagDir1_Pt_Endcap += _RecoCutFlags[1].c_str();
		name_TagDir1_Pt_Endcap += _RecoCutFlags[2].c_str();
		name_TPFiller1_PtEta += _RecoCutFlags[1].c_str();
		name_TPFiller1_PtEta += _RecoCutFlags[2].c_str();

		string name_TPFiller1_PtEta_12 = name_TPFiller1_PtEta+"_";
		TDirectory *TagDir1_Pt_Barrel_12 = _dir->mkdir(name_TagDir1_Pt_Barrel.c_str());
		TDirectory *TagDir1_Pt_Endcap_12 = _dir->mkdir(name_TagDir1_Pt_Endcap.c_str());

		_TagProbe_Electron1_Pt_Barrel_12 = new TagAndProbeFillerElectron(TagDir1_Pt_Barrel_12, string(name_TPFiller1_PtEta_12.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_12, "soft");
		_TagProbe_Electron1_Pt_Endcap_12 = new TagAndProbeFillerElectron(TagDir1_Pt_Endcap_12, string(name_TPFiller1_PtEta_12.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_12, "soft");

		name_TPFiller1_PtEta += _RecoCutFlags[3].c_str();
		string name_TPFiller1_PtEta_123 = name_TPFiller1_PtEta+"_"; 
		name_TagDir1_Pt_Barrel += _RecoCutFlags[3].c_str();
		TDirectory *TagDir1_Pt_Barrel_123 = _dir->mkdir(name_TagDir1_Pt_Barrel.c_str());
		name_TagDir1_Pt_Endcap += _RecoCutFlags[3].c_str();
		TDirectory *TagDir1_Pt_Endcap_123 = _dir->mkdir(name_TagDir1_Pt_Endcap.c_str());

		_TagProbe_Electron1_Pt_Barrel_123 = new TagAndProbeFillerElectron(TagDir1_Pt_Barrel_123, string(name_TPFiller1_PtEta_123.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_12, probe_cuts1_123, "soft");
		_TagProbe_Electron1_Pt_Endcap_123 = new TagAndProbeFillerElectron(TagDir1_Pt_Endcap_123, string(name_TPFiller1_PtEta_123.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_12, probe_cuts1_123, "soft");

		name_TPFiller1_PtEta += _RecoCutFlags[4].c_str();
		string name_TPFiller1_PtEta_1234 = name_TPFiller1_PtEta+"_"; 
		name_TagDir1_Pt_Barrel += _RecoCutFlags[4].c_str();
		TDirectory *TagDir1_Pt_Barrel_1234 = _dir->mkdir(name_TagDir1_Pt_Barrel.c_str());
		name_TagDir1_Pt_Endcap += _RecoCutFlags[4].c_str();
		TDirectory *TagDir1_Pt_Endcap_1234 = _dir->mkdir(name_TagDir1_Pt_Endcap.c_str());

		_TagProbe_Electron1_Pt_Barrel_1234 = new TagAndProbeFillerElectron(TagDir1_Pt_Barrel_1234, string(name_TPFiller1_PtEta_1234.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_123, probe_cuts1_1234, "soft");
		_TagProbe_Electron1_Pt_Endcap_1234 = new TagAndProbeFillerElectron(TagDir1_Pt_Endcap_1234, string(name_TPFiller1_PtEta_1234.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_123, probe_cuts1_1234, "soft");

		name_TPFiller1_PtEta += _RecoCutFlags[5].c_str();
		string name_TPFiller1_PtEta_12345 = name_TPFiller1_PtEta+"_"; 
		name_TagDir1_Pt_Barrel += _RecoCutFlags[5].c_str();
		TDirectory *TagDir1_Pt_Barrel_12345 = _dir->mkdir(name_TagDir1_Pt_Barrel.c_str());
		name_TagDir1_Pt_Endcap += _RecoCutFlags[5].c_str();
		TDirectory *TagDir1_Pt_Endcap_12345 = _dir->mkdir(name_TagDir1_Pt_Endcap.c_str());

		_TagProbe_Electron1_Pt_Barrel_12345 = new TagAndProbeFillerElectron(TagDir1_Pt_Barrel_12345, string(name_TPFiller1_PtEta_12345.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_1234, probe_cuts1_12345, "soft");
		_TagProbe_Electron1_Pt_Endcap_12345 = new TagAndProbeFillerElectron(TagDir1_Pt_Endcap_12345, string(name_TPFiller1_PtEta_12345.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_1234, probe_cuts1_12345, "soft");

		name_TPFiller1_PtEta += _RecoCutFlags[6].c_str();
		string name_TPFiller1_PtEta_123456 = name_TPFiller1_PtEta+"_"; 
		name_TagDir1_Pt_Barrel += _RecoCutFlags[6].c_str();
		TDirectory *TagDir1_Pt_Barrel_123456 = _dir->mkdir(name_TagDir1_Pt_Barrel.c_str());
		name_TagDir1_Pt_Endcap += _RecoCutFlags[6].c_str();
		TDirectory *TagDir1_Pt_Endcap_123456 = _dir->mkdir(name_TagDir1_Pt_Endcap.c_str());

		_TagProbe_Electron1_Pt_Barrel_123456 = new TagAndProbeFillerElectron(TagDir1_Pt_Barrel_123456, string(name_TPFiller1_PtEta_123456.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_12345, probe_cuts1_123456, "soft");
		_TagProbe_Electron1_Pt_Endcap_123456 = new TagAndProbeFillerElectron(TagDir1_Pt_Endcap_123456, string(name_TPFiller1_PtEta_123456.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_12345, probe_cuts1_123456, "soft");

		name_TPFiller1_PtEta += _RecoCutFlags[7].c_str();
		string name_TPFiller1_PtEta_1234567 = name_TPFiller1_PtEta+"_"; 
		name_TagDir1_Pt_Barrel += _RecoCutFlags[7].c_str();
		TDirectory *TagDir1_Pt_Barrel_1234567 = _dir->mkdir(name_TagDir1_Pt_Barrel.c_str());
		name_TagDir1_Pt_Endcap += _RecoCutFlags[7].c_str();
		TDirectory *TagDir1_Pt_Endcap_1234567 = _dir->mkdir(name_TagDir1_Pt_Endcap.c_str());

		_TagProbe_Electron1_Pt_Barrel_1234567 = new TagAndProbeFillerElectron(TagDir1_Pt_Barrel_1234567, string(name_TPFiller1_PtEta_1234567.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_123456, probe_cuts1_1234567, "soft");
		_TagProbe_Electron1_Pt_Endcap_1234567 = new TagAndProbeFillerElectron(TagDir1_Pt_Endcap_1234567, string(name_TPFiller1_PtEta_1234567.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_123456, probe_cuts1_1234567, "soft");


		string name_TPFiller1_PtEta_Global = "electron_Global_";
		TDirectory *TagDir1_Pt_Barrel_Global = _dir->mkdir("Tag&Probe1_Pt_Barrel_Global");
		TDirectory *TagDir1_Pt_Endcap_Global = _dir->mkdir("Tag&Probe1_Pt_Endcap_Global");

		_TagProbe_Electron1_Pt_Barrel_Global = new TagAndProbeFillerElectron(TagDir1_Pt_Barrel_Global, string(name_TPFiller1_PtEta_Global.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_1234567, "soft");
		_TagProbe_Electron1_Pt_Endcap_Global = new TagAndProbeFillerElectron(TagDir1_Pt_Endcap_Global, string(name_TPFiller1_PtEta_Global.c_str()), nbin_pt, pt_xmin, pt_xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_1234567, "soft");

	}

	int fileCounter = 0;

	TChain *ch = new TChain("Events");
	ifstream infile;
	infile.open(sourceFileList.c_str());
	string datafile;
	while(getline (infile, datafile)){
		ch->Add(datafile.c_str());
		fileCounter++;
	}

	if(_EventNumber==0 && _EventsPerFile==0) _entries = ch->GetEntries();

	if(_EventNumber!=0 && _EventsPerFile==0) _entries = _EventNumber;
 
	if(_EventNumber==0 && _EventsPerFile!=0) _entries = fileCounter*_EventsPerFile;

	if(_ProcEvents!=-1) _entries = _ProcEvents;

	cout<<"RecoElectron analyzing nr. file = "<<fileCounter<<endl;
	cout<<"RecoElectron analyzing nr. event = "<<_entries<<endl;

	delete ch;

	cout << "EfficiencyElectron Worker built." << endl;	

}

EfficiencyElectron::~EfficiencyElectron(){

	_file->ls();

	std::vector<TH1D*>::const_iterator ibeg = _histoVector.begin();
	std::vector<TH1D*>::const_iterator iend = _histoVector.end();
	
	for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
		delete (*i);
	}
	
}

void EfficiencyElectron::process(const fwlite::Event& iEvent){

	_file->cd();
	
	_run = iEvent.id().run();
	if(_sample=="mc") _run=-1;

	fwlite::Handle<std::vector<pat::Electron> > electronHandle;
	electronHandle.getByLabel(iEvent, "selectedElectrons");

	fwlite::Handle<std::vector<reco::CompositeCandidate> > zrecHandle;
	zrecHandle.getByLabel(iEvent, "zeerec");
	
	fwlite::Handle<std::vector<pat::Jet> > jetrecHandle;
	jetrecHandle.getByLabel(iEvent, "selectedJetsL1Corrected");

	fwlite::Handle<double> Rho;
	Rho.getByLabel(iEvent, "kt6PFJets", "rho");
	_rho = *Rho;

	fwlite::Handle<pat::TriggerEvent> triggerHandle;
	triggerHandle.getByLabel(iEvent, "patTriggerEvent");

	fwlite::Handle<std::vector<reco::CompositeCandidate> > zgenHandle;
	zgenHandle.getByLabel(iEvent, "zeegen");

	fwlite::Handle<std::vector<reco::GenJet> > jetgenHandle;
	jetgenHandle.getByLabel(iEvent, "selectedGenJetsOld");
	
	//Z Gen daughters
	std::vector<const pat::Electron*> zgendaughters;
	const pat::Electron *gendau0 = 0;
	const pat::Electron *gendau1 = 0;
	
	//Z Reco daughters
	std::vector<const pat::Electron*> zrecdaughters;
	const pat::Electron *recdau0 = 0; 
	const pat::Electron *recdau1 = 0;
	
	//Reco jets
	std::vector<const pat::Jet*> recjets = GetJets<pat::Jet>(*jetrecHandle);
	std::vector<const pat::Jet*> geniso_recjets;
	std::vector<const pat::Jet*> reciso_recjets;
	
	//Reco Jets isolated from GEN Z electrons
	if(_sample=="mc"){
		if(zgenHandle->size()){
			zgendaughters = ZGENDaughters(*zgenHandle);
		}	
		if(zgendaughters.size()){
			for(unsigned int i = 0; i < recjets.size(); i++){
				if(IsoJet<pat::Electron>(zgendaughters,*recjets[i])){
					geniso_recjets.push_back(recjets[i]);
				}
			}
		}
		else if(!zgendaughters.size()){
			for(unsigned int i = 0; i < recjets.size(); i++){
				geniso_recjets.push_back(recjets[i]);
			}
		}
	}//end if(_sample)
	
	//Z Rec daughters
	if(zrecHandle->size()){
		zrecdaughters = ZRECDaughters(*zrecHandle);
	}	
	if(zrecdaughters.size()){	
		recdau0 = zrecdaughters[0];
		recdau1 = zrecdaughters[1];
		//Pre selection events 
	}
 
	//Reco Jets isolated from RECO Z electrons
	if(zrecdaughters.size()){
		for(unsigned int i = 0; i < recjets.size(); i++){
			if(IsoJet<pat::Electron>(zrecdaughters,*recjets[i])){
				reciso_recjets.push_back(recjets[i]);
			}
		}
	}
	else if(!zrecdaughters.size()){
		for(unsigned int i = 0; i < recjets.size(); i++){
			reciso_recjets.push_back(recjets[i]);
		}
	}
 
	if(_sample=="mc" && zgenHandle->size()!=0){
	
		//Z Gen daughters order
	
		double genleadelpt = 0; 
		double genleadeleta = 0; 
		double gensecondelpt = 0;
		double gensecondeleta = 0;
	
		if(zgendaughters.size() != 0){ 
			
			gendau0 = zgendaughters[0];
			gendau1 = zgendaughters[1];
		
			genleadelpt = gendau0->pt(); 
			gensecondelpt = gendau1->pt();
			genleadeleta = gendau0->eta();
			gensecondeleta = gendau1->eta();
	
			if(gensecondelpt>genleadelpt){
				throw cms::Exception("PATAnalysis:EfficiencyElectron_WrongElectronOrder") << "ERROR! Z electrons are in wrong order!";
			}		
		}
		//Generated jets
		std::vector<const reco::GenJet*> genjets = GetJets_noJetID<reco::GenJet>(*jetgenHandle);	
		std::vector<const reco::GenJet*> isogenjets;

		if(zgendaughters.size()){
			for(unsigned int i = 0; i < genjets.size(); i++){
				if(IsoJet<pat::Electron>(zgendaughters,*genjets[i])){
					isogenjets.push_back(genjets[i]);
				}
			}
		}
		else if(!zgendaughters.size()){
			for(unsigned int i = 0; i < genjets.size(); i++){
				isogenjets.push_back(genjets[i]);
			}
		}
	
		//Acceptance denominator
		if (GenSelected(*zgenHandle, _selections) && zgendaughters.size()!=0){

			AccDenom_GenExclJetNumber->Fill(isogenjets.size());
			AccDenom_RecoExclJetNumber->Fill(geniso_recjets.size());
			for(unsigned int i = 0; (i < isogenjets.size()+1) && ((i+1)<10); i++){
				AccDenom_GenInclJetNumber->AddBinContent(i+1,1);
			}
			for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
				AccDenom_RecoInclJetNumber->AddBinContent(i+1,1);
			}	
		}
	
		//Efficiency denominator
		if (GenSelectedInAcceptance((*zgenHandle), _selections) && zgendaughters.size()!=0){
	
			EffDenom_GenExclJetNumber->Fill(isogenjets.size());	
			EffDenom_RecoExclJetNumber->Fill(geniso_recjets.size());
			for(unsigned int i = 0; (i < isogenjets.size()+1) && ((i+1)<10); i++){
				EffDenom_GenInclJetNumber->AddBinContent(i+1,1);
			}
			for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
				EffDenom_RecoInclJetNumber->AddBinContent(i+1,1);
			}		
		}

		if(zrecHandle->size()){
	
			//Events with a selected Zee 1
			if (RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEff_1->AddBinContent(i+1,1);
				}
				RecoExclJetEff_1->Fill(geniso_recjets.size());
			}
	
			//Events with a selected Zee 1 With Gen(Acc) Req.
			if (GenSelectedInAcceptance((*zgenHandle), _selections) && 
			    RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables With Gen(Acc)
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEffWGA_1->AddBinContent(i+1,1);
				}
				RecoExclJetEffWGA_1->Fill(geniso_recjets.size());
			}

			//Events with a selected Zee 1+2
			if (RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEff_12->AddBinContent(i+1,1);
				}
				RecoExclJetEff_12->Fill(geniso_recjets.size());
			}
	
			//Events with a selected Zee 1+2 With Gen(Acc) Req.
			if (GenSelectedInAcceptance((*zgenHandle), _selections) && 
			    RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables With Gen(Acc)
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEffWGA_12->AddBinContent(i+1,1);
				}
				RecoExclJetEffWGA_12->Fill(geniso_recjets.size());
			}
	
			//Events with a selected Zee 1+2+3
			if (RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables
				for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++){
					RecoInclJetEff_123->AddBinContent(i+1,1);
				}
				RecoExclJetEff_123->Fill(geniso_recjets.size());
			}
	
			//Events with a selected Zee 1+2+3 With Gen(Acc) Req.
			if (GenSelectedInAcceptance((*zgenHandle), _selections) && 
			    RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables With Gen(Acc)
				for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++){
					RecoInclJetEffWGA_123->AddBinContent(i+1,1);
				}
				RecoExclJetEffWGA_123->Fill(geniso_recjets.size());
			}
	
			//Events with a selected Zee 1+2+3+4
			if (RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEff_1234->AddBinContent(i+1,1);
				}
				RecoExclJetEff_1234->Fill(geniso_recjets.size());
			}
	
			//Events with a selected Zee 1+2+3+4 With Gen(Acc) Req.
			if (GenSelectedInAcceptance((*zgenHandle), _selections) && 
			    RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho) &&
			    RecSelected_ele(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables With Gen(Acc)
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEffWGA_1234->AddBinContent(i+1,1);
				}
				RecoExclJetEffWGA_1234->Fill(geniso_recjets.size());
			}
	
			//Events with a selected Zee 1+2+3+4+5
			if (RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho) &&
			    RecSelected_ele(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[5].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEff_12345->AddBinContent(i+1,1);
				}
				RecoExclJetEff_12345->Fill(geniso_recjets.size());
			}
	
			//Events with a selected Zee 1+2+3+4+5 With Gen(Acc) Req.
			if (GenSelectedInAcceptance((*zgenHandle), _selections) && 
			    RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[5].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables With Gen(Acc)
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEffWGA_12345->AddBinContent(i+1,1);
				}
				RecoExclJetEffWGA_12345->Fill(geniso_recjets.size());
			}
	
			//Events with a selected Zee 1+2+3+4+5+6
			if (RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[5].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[6].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEff_123456->AddBinContent(i+1,1);
				}
				RecoExclJetEff_123456->Fill(geniso_recjets.size());
			}
	
			//Events with a selected Zee 1+2+3+4+5+6 With Gen(Acc) Req.
			if (GenSelectedInAcceptance((*zgenHandle), _selections) && 
			    RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[5].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[6].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables With Gen(Acc)
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEffWGA_123456->AddBinContent(i+1,1);
				}
				RecoExclJetEffWGA_123456->Fill(geniso_recjets.size());
			}	

			//Events with a selected Zee 1+2+3+4+5+6+7
			if (RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[5].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[6].c_str(), *zrecHandle, *triggerHandle, _run, _rho) &&
			    RecSelected_ele(_RecoCutFlags[7].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEff_1234567->AddBinContent(i+1,1);
				}
				RecoExclJetEff_1234567->Fill(geniso_recjets.size());
			}
	
			//Events with a selected Zee 1+2+3+4+5+6+7 With Gen(Acc) Req.
			if (GenSelectedInAcceptance((*zgenHandle), _selections) && 
			    RecSelected_ele(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[5].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_ele(_RecoCutFlags[6].c_str(), *zrecHandle, *triggerHandle, _run, _rho) &&
			    RecSelected_ele(_RecoCutFlags[7].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
				//Eff vs Reco Jet variables With Gen(Acc)
				for(unsigned int i = 0; (i < geniso_recjets.size()+1) && ((i+1)<10); i++){
					RecoInclJetEffWGA_1234567->AddBinContent(i+1,1);
				}
				RecoExclJetEffWGA_1234567->Fill(geniso_recjets.size());
			}	


		}

		if(zgendaughters.size() != 0){
	
			int genpt0 = -1;
			int genpt1 = -1;
	
			if(gendau0->pt()>_ptBinTP[0] && gendau0->pt()<_ptBinTP[1]) genpt0 = 0;
			if(gendau0->pt()>_ptBinTP[1] && gendau0->pt()<_ptBinTP[2]) genpt0 = 1;
			if(gendau0->pt()>_ptBinTP[2])                              genpt0 = 2;
/*
			if(gendau0->pt()>_ptBinTP[2] && gendau0->pt()<_ptBinTP[3]) genpt0 = 2;
			if(gendau0->pt()>_ptBinTP[3] && gendau0->pt()<_ptBinTP[4]) genpt0 = 3;
			if(gendau0->pt()>_ptBinTP[4] && gendau0->pt()<_ptBinTP[5]) genpt0 = 4;
			if(gendau0->pt()>_ptBinTP[5] && gendau0->pt()<_ptBinTP[6]) genpt0 = 5;
			if(gendau0->pt()>_ptBinTP[6] && gendau0->pt()<_ptBinTP[7]) genpt0 = 6;
			if(gendau0->pt()>_ptBinTP[7] && gendau0->pt()<_ptBinTP[8]) genpt0 = 7;
			if(gendau0->pt()>_ptBinTP[8] && gendau0->pt()<_ptBinTP[9]) genpt0 = 8;
			if(gendau0->pt()>_ptBinTP[9] && gendau0->pt()<_ptBinTP[10]) genpt0 = 9;
*/	
			if(gendau1->pt()>_ptBinTP[0] && gendau1->pt()<_ptBinTP[1]) genpt1 = 0;
			if(gendau1->pt()>_ptBinTP[1] && gendau1->pt()<_ptBinTP[2]) genpt1 = 1;
			if(gendau1->pt()>_ptBinTP[2])                              genpt1 = 2;
/*
			if(gendau1->pt()>_ptBinTP[2] && gendau1->pt()<_ptBinTP[3]) genpt1 = 2;
			if(gendau1->pt()>_ptBinTP[3] && gendau1->pt()<_ptBinTP[4]) genpt1 = 3;
			if(gendau1->pt()>_ptBinTP[4] && gendau1->pt()<_ptBinTP[5]) genpt1 = 4;
			if(gendau1->pt()>_ptBinTP[5] && gendau1->pt()<_ptBinTP[6]) genpt1 = 5;
			if(gendau1->pt()>_ptBinTP[6] && gendau1->pt()<_ptBinTP[7]) genpt1 = 6;
			if(gendau1->pt()>_ptBinTP[7] && gendau1->pt()<_ptBinTP[8]) genpt1 = 7;
			if(gendau1->pt()>_ptBinTP[8] && gendau1->pt()<_ptBinTP[9]) genpt1 = 8;
			if(gendau1->pt()>_ptBinTP[9] && gendau1->pt()<_ptBinTP[10]) genpt1 = 9;
*/	
			if(MassWindow(*zgenHandle, _selections)){
	
				if(gendau0->isEB()) AccDenom_GenSingleElPt_Barrel->Fill(genpt0);	
				if(gendau0->isEE()) AccDenom_GenSingleElPt_Endcap->Fill(genpt0);
				if(gendau1->isEB()) AccDenom_GenSingleElPt_Barrel->Fill(genpt1);	
				if(gendau1->isEE()) AccDenom_GenSingleElPt_Endcap->Fill(genpt1);
	
				if(_selections=="SYM"){	
					if(SingleEl_GenSelectedInAcceptance_SYM(gendau0)){
						if(gendau0->isEB()) EffDenom_GenSingleElPt_Barrel->Fill(genpt0);
						if(gendau0->isEE()) EffDenom_GenSingleElPt_Endcap->Fill(genpt0);
					}
					if(SingleEl_GenSelectedInAcceptance_SYM(gendau1)){
						if(gendau1->isEB())EffDenom_GenSingleElPt_Barrel->Fill(genpt1);
						if(gendau1->isEE())EffDenom_GenSingleElPt_Endcap->Fill(genpt1);
					}
				}
	
				if(_selections=="ASYM"){	
					if(SingleEl_GenSelectedInAcceptance_ASYM0(gendau0)){
						if(gendau0->isEB()) EffDenom_GenSingleElPt_Barrel->Fill(genpt0);
						if(gendau0->isEE()) EffDenom_GenSingleElPt_Endcap->Fill(genpt0);
					}
					if(SingleEl_GenSelectedInAcceptance_ASYM1(gendau1)){
						if(gendau1->isEB()) EffDenom_GenSingleElPt_Barrel->Fill(genpt1);
						if(gendau1->isEE()) EffDenom_GenSingleElPt_Endcap->Fill(genpt1);
					}
				}
			}
		}
	
		if(zrecdaughters.size() != 0){
	
			int recpt0 = -1;
			int recpt1 = -1;
	
			if(recdau0->pt()>_ptBinTP[0] && recdau0->pt()<_ptBinTP[1]) recpt0 = 0;
			if(recdau0->pt()>_ptBinTP[1] && recdau0->pt()<_ptBinTP[2]) recpt0 = 1;
			if(recdau0->pt()>_ptBinTP[2])                              recpt0 = 2;
/*
			if(recdau0->pt()>_ptBinTP[2] && recdau0->pt()<_ptBinTP[3]) recpt0 = 2;
			if(recdau0->pt()>_ptBinTP[3] && recdau0->pt()<_ptBinTP[4]) recpt0 = 3;
			if(recdau0->pt()>_ptBinTP[4] && recdau0->pt()<_ptBinTP[5]) recpt0 = 4;
			if(recdau0->pt()>_ptBinTP[5] && recdau0->pt()<_ptBinTP[6]) recpt0 = 5;
			if(recdau0->pt()>_ptBinTP[6] && recdau0->pt()<_ptBinTP[7]) recpt0 = 6;
			if(recdau0->pt()>_ptBinTP[7] && recdau0->pt()<_ptBinTP[8]) recpt0 = 7;
			if(recdau0->pt()>_ptBinTP[8] && recdau0->pt()<_ptBinTP[9]) recpt0 = 8;
			if(recdau0->pt()>_ptBinTP[9] && recdau0->pt()<_ptBinTP[10]) recpt0 = 9;
*/	
			if(recdau1->pt()>_ptBinTP[0] && recdau1->pt()<_ptBinTP[1]) recpt1 = 0;
			if(recdau1->pt()>_ptBinTP[1] && recdau1->pt()<_ptBinTP[2]) recpt1 = 1;
			if(recdau1->pt()>_ptBinTP[2])                              recpt1 = 2;
/*
			if(recdau1->pt()>_ptBinTP[2] && recdau1->pt()<_ptBinTP[3]) recpt1 = 2;
			if(recdau1->pt()>_ptBinTP[3] && recdau1->pt()<_ptBinTP[4]) recpt1 = 3;
			if(recdau1->pt()>_ptBinTP[4] && recdau1->pt()<_ptBinTP[5]) recpt1 = 4;
			if(recdau1->pt()>_ptBinTP[5] && recdau1->pt()<_ptBinTP[6]) recpt1 = 5;
			if(recdau1->pt()>_ptBinTP[6] && recdau1->pt()<_ptBinTP[7]) recpt1 = 6;
			if(recdau1->pt()>_ptBinTP[7] && recdau1->pt()<_ptBinTP[8]) recpt1 = 7;
			if(recdau1->pt()>_ptBinTP[8] && recdau1->pt()<_ptBinTP[9]) recpt1 = 8;
			if(recdau1->pt()>_ptBinTP[9] && recdau1->pt()<_ptBinTP[10]) recpt1 = 9;
*/	
			if(MassWindow(*zrecHandle, _selections)){
	
				if(_selections=="SYM"){ 

					std::vector<bool (*)(const reco::Candidate&, int run, double rho)>::const_iterator cut1 = probe_cuts_1234567.begin();
	
					if((*cut1)(*recdau0, _run, _rho)){
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_1->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_1->Fill(recpt0);
					}

					if((*cut1)(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_1->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_1->Fill(recpt1);
					}
 
					if((*cut1)(*recdau0, _run, _rho) && 
					   (*(cut1+1))(*recdau0, _run, _rho)){
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_12->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_12->Fill(recpt0);
					}

					if((*cut1)(*recdau1, _run, _rho) && 
					   (*(cut1+1))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_12->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_12->Fill(recpt1);
					}
		
					if((*cut1)(*recdau0, _run, _rho) && 
					   (*(cut1+1))(*recdau0, _run, _rho) && 
					   (*(cut1+2))(*recdau0, _run, _rho)){	
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_123->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_123->Fill(recpt0);
					}

					if((*cut1)(*recdau1, _run, _rho) && 
					   (*(cut1+1))(*recdau1, _run, _rho) && 
					   (*(cut1+2))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_123->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_123->Fill(recpt1);
					}
	
					if((*cut1)(*recdau0, _run, _rho) && 
					   (*(cut1+1))(*recdau0, _run, _rho) && 
					   (*(cut1+2))(*recdau0, _run, _rho) && 
					   (*(cut1+3))(*recdau0, _run, _rho)){
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_1234->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_1234->Fill(recpt0);
					}

					if((*cut1)(*recdau1, _run, _rho) && 
					   (*(cut1+1))(*recdau1, _run, _rho) && 
					   (*(cut1+2))(*recdau1, _run, _rho) && 
					   (*(cut1+3))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_1234->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_1234->Fill(recpt1);
					}
	
					if((*cut1)(*recdau0, _run, _rho) && 
					   (*(cut1+1))(*recdau0, _run, _rho) && 
					   (*(cut1+2))(*recdau0, _run, _rho) && 
					   (*(cut1+3))(*recdau0, _run, _rho) &&
					   (*(cut1+4))(*recdau0, _run, _rho)){
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_12345->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_12345->Fill(recpt0);
					}

					if((*cut1)(*recdau1, _run, _rho) && 
					   (*(cut1+1))(*recdau1, _run, _rho) && 
					   (*(cut1+2))(*recdau1, _run, _rho) && 
					   (*(cut1+3))(*recdau1, _run, _rho) && 
					   (*(cut1+4))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_12345->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_12345->Fill(recpt1);
					}

					if((*cut1)(*recdau0, _run, _rho) && 
					   (*(cut1+1))(*recdau0, _run, _rho) && 
					   (*(cut1+2))(*recdau0, _run, _rho) && 
					   (*(cut1+3))(*recdau0, _run, _rho) && 
					   (*(cut1+4))(*recdau0, _run, _rho) && 
					   (*(cut1+5))(*recdau0, _run, _rho)){	
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_123456->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_123456->Fill(recpt0);
					}
					if((*cut1)(*recdau1, _run, _rho) && 
					   (*(cut1+1))(*recdau1, _run, _rho) && 
					   (*(cut1+2))(*recdau1, _run, _rho) && 
					   (*(cut1+3))(*recdau1, _run, _rho) && 
					   (*(cut1+4))(*recdau1, _run, _rho) && 
					   (*(cut1+5))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_123456->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_123456->Fill(recpt1);
					}

					if((*cut1)(*recdau0, _run, _rho) && 
					   (*(cut1+1))(*recdau0, _run, _rho) && 
					   (*(cut1+2))(*recdau0, _run, _rho) && 
					   (*(cut1+3))(*recdau0, _run, _rho) && 
					   (*(cut1+4))(*recdau0, _run, _rho) && 
					   (*(cut1+5))(*recdau0, _run, _rho) && 
					   (*(cut1+6))(*recdau0, _run, _rho)){	
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_1234567->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_1234567->Fill(recpt0);
					}
					if((*cut1)(*recdau1, _run, _rho) && 
					   (*(cut1+1))(*recdau1, _run, _rho) && 
					   (*(cut1+2))(*recdau1, _run, _rho) && 
					   (*(cut1+3))(*recdau1, _run, _rho) && 
					   (*(cut1+4))(*recdau1, _run, _rho) && 
					   (*(cut1+5))(*recdau1, _run, _rho) &&
					   (*(cut1+6))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_1234567->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_1234567->Fill(recpt1);
					}


				}
	
				if(_selections=="ASYM"){ 
	
					std::vector<bool (*)(const reco::Candidate&, int run, double rho)>::const_iterator cut1_tight = probe_cuts0_1234567.begin();
					std::vector<bool (*)(const reco::Candidate&, int run, double rho)>::const_iterator cut1_loose = probe_cuts1_1234567.begin();
	
					if((*cut1_tight)(*recdau0, _run, _rho)){
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_1->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_1->Fill(recpt0);
					}
					if((*cut1_loose)(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_1->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_1->Fill(recpt1);
					}
 
					if((*cut1_tight)(*recdau0, _run, _rho) && 
					   (*(cut1_tight+1))(*recdau0, _run, _rho)){
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_12->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_12->Fill(recpt0);
					}
					if((*cut1_loose)(*recdau1, _run, _rho) && 
					   (*(cut1_loose+1))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_12->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_12->Fill(recpt1);
					}
	
					if((*cut1_tight)(*recdau0, _run, _rho) && 
					   (*(cut1_tight+1))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+2))(*recdau0, _run, _rho)){
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_123->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_123->Fill(recpt0);
					}
					if((*cut1_loose)(*recdau1, _run, _rho) && 
					   (*(cut1_loose+1))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+2))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_123->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_123->Fill(recpt1);
					}
	
					if((*cut1_tight)(*recdau0, _run, _rho) && 
					   (*(cut1_tight+1))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+2))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+3))(*recdau0, _run, _rho)){
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_1234->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_1234->Fill(recpt0);
					}
					if((*cut1_loose)(*recdau1, _run, _rho) && 
					   (*(cut1_loose+1))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+2))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+3))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_1234->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_1234->Fill(recpt1);
					}
	
					if((*cut1_tight)(*recdau0, _run, _rho) && 
					   (*(cut1_tight+1))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+2))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+3))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+4))(*recdau0, _run, _rho)){
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_12345->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_12345->Fill(recpt0);
					}
					if((*cut1_loose)(*recdau1, _run, _rho) && 
					   (*(cut1_loose+1))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+2))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+3))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+4))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_12345->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_12345->Fill(recpt1);
					}
	
					if((*cut1_tight)(*recdau0, _run, _rho) && 
					   (*(cut1_tight+1))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+2))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+3))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+4))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+5))(*recdau0, _run, _rho)){
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_123456->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_123456->Fill(recpt0);
					}
					if((*cut1_loose)(*recdau1, _run, _rho) && 
					   (*(cut1_loose+1))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+2))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+3))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+4))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+5))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_123456->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_123456->Fill(recpt1);
					}
					if((*cut1_tight)(*recdau0, _run, _rho) && 
					   (*(cut1_tight+1))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+2))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+3))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+4))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+5))(*recdau0, _run, _rho) && 
					   (*(cut1_tight+6))(*recdau0, _run, _rho)){
						if(recdau0->isEB()) RecoSingleElPtEff_Barrel_1234567->Fill(recpt0);
						if(recdau0->isEE()) RecoSingleElPtEff_Endcap_1234567->Fill(recpt0);
					}
					if((*cut1_loose)(*recdau1, _run, _rho) && 
					   (*(cut1_loose+1))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+2))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+3))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+4))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+5))(*recdau1, _run, _rho) && 
					   (*(cut1_loose+6))(*recdau1, _run, _rho)){
						if(recdau1->isEB()) RecoSingleElPtEff_Barrel_1234567->Fill(recpt1);
						if(recdau1->isEE()) RecoSingleElPtEff_Endcap_1234567->Fill(recpt1);
					}


				}
			}	
		}
	}//end if(_sample)


	// Tag & Probe
	if(zrecHandle->size()){
		//Conditions required to fill TagAndProbe
		if(MassWindow(*zrecHandle, _selections)){

			double lumi = _entries/_xsec;

			if(_Norm && lumi!=0){
				_norm = _targetLumi/lumi;
			}
	
			if(_selections=="SYM"){
				_TagProbe_Electron_Excl_12->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron_Excl_123->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron_Excl_1234->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron_Excl_12345->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron_Excl_123456->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron_Excl_1234567->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron_Excl_Global->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron_Excl_Double->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
			}
			if(_selections=="ASYM"){
				_TagProbe_Electron0_Excl_12->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron0_Excl_123->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron0_Excl_1234->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron0_Excl_12345->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron0_Excl_123456->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron0_Excl_1234567->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron0_Excl_Global->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron0_Excl_Double->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);

				_TagProbe_Electron1_Excl_12->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron1_Excl_123->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron1_Excl_1234->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron1_Excl_12345->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron1_Excl_123456->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron1_Excl_1234567->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron1_Excl_Global->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
				_TagProbe_Electron1_Excl_Double->fill((*zrecHandle)[0], _run, _rho, reciso_recjets.size(), _norm);
			}
	
			for(unsigned int i = 0; i < reciso_recjets.size()+1; i++){
				if(_selections=="SYM"){
					_TagProbe_Electron_Incl_12->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron_Incl_123->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron_Incl_1234->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron_Incl_12345->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron_Incl_123456->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron_Incl_1234567->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron_Incl_Global->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron_Incl_Double->fill((*zrecHandle)[0], _run, _rho, i, _norm);
				}
				if(_selections=="ASYM"){
					_TagProbe_Electron0_Incl_12->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron0_Incl_123->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron0_Incl_1234->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron0_Incl_12345->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron0_Incl_123456->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron0_Incl_1234567->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron0_Incl_Global->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron0_Incl_Double->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron1_Incl_12->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron1_Incl_123->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron1_Incl_1234->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron1_Incl_12345->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron1_Incl_123456->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron1_Incl_1234567->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron1_Incl_Global->fill((*zrecHandle)[0], _run, _rho, i, _norm);
					_TagProbe_Electron1_Incl_Double->fill((*zrecHandle)[0], _run, _rho, i, _norm);
				}
			}

			if(_selections=="SYM"){
				_TagProbe_Electron_Pt_Barrel_12->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 0);
				_TagProbe_Electron_Pt_Barrel_123->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 0);
				_TagProbe_Electron_Pt_Barrel_1234->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 0);
				_TagProbe_Electron_Pt_Barrel_12345->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 0);
				_TagProbe_Electron_Pt_Barrel_123456->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 0);
				_TagProbe_Electron_Pt_Barrel_1234567->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 0);
				_TagProbe_Electron_Pt_Barrel_Global->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 0);
				_TagProbe_Electron_Pt_Endcap_12->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 1);
				_TagProbe_Electron_Pt_Endcap_123->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 1);
				_TagProbe_Electron_Pt_Endcap_1234->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 1);
				_TagProbe_Electron_Pt_Endcap_12345->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 1);
				_TagProbe_Electron_Pt_Endcap_123456->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 1);
				_TagProbe_Electron_Pt_Endcap_1234567->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 1);
				_TagProbe_Electron_Pt_Endcap_Global->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP, 1);
			}
			if(_selections=="ASYM"){
				_TagProbe_Electron0_Pt_Barrel_12->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 0);
				_TagProbe_Electron0_Pt_Barrel_123->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 0);
				_TagProbe_Electron0_Pt_Barrel_1234->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 0);
				_TagProbe_Electron0_Pt_Barrel_12345->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 0);
				_TagProbe_Electron0_Pt_Barrel_123456->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 0);
				_TagProbe_Electron0_Pt_Barrel_1234567->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 0);
				_TagProbe_Electron0_Pt_Barrel_Global->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 0);
				_TagProbe_Electron0_Pt_Endcap_12->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 1);
				_TagProbe_Electron0_Pt_Endcap_123->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 1);
				_TagProbe_Electron0_Pt_Endcap_1234->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 1);
				_TagProbe_Electron0_Pt_Endcap_12345->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 1);
				_TagProbe_Electron0_Pt_Endcap_123456->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 1);
				_TagProbe_Electron0_Pt_Endcap_1234567->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 1);
				_TagProbe_Electron0_Pt_Endcap_Global->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP0, 1);
	
				_TagProbe_Electron1_Pt_Barrel_12->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 0);
				_TagProbe_Electron1_Pt_Barrel_123->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 0);
				_TagProbe_Electron1_Pt_Barrel_1234->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 0);
				_TagProbe_Electron1_Pt_Barrel_12345->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 0);
				_TagProbe_Electron1_Pt_Barrel_123456->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 0);
				_TagProbe_Electron1_Pt_Barrel_1234567->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 0);
				_TagProbe_Electron1_Pt_Barrel_Global->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 0);
				_TagProbe_Electron1_Pt_Endcap_12->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 1);
				_TagProbe_Electron1_Pt_Endcap_123->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 1);
				_TagProbe_Electron1_Pt_Endcap_1234->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 1);
				_TagProbe_Electron1_Pt_Endcap_12345->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 1);
				_TagProbe_Electron1_Pt_Endcap_123456->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 1);
				_TagProbe_Electron1_Pt_Endcap_1234567->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 1);
				_TagProbe_Electron1_Pt_Endcap_Global->fill((*zrecHandle)[0], _run, _rho, 0, _norm, _ptBinTP1, 1);
			}
		}
	}
}


void EfficiencyElectron::finalize(){


	if(_selections=="SYM"){
		_TagProbe_Electron_Excl_12->finalize();
		_TagProbe_Electron_Excl_123->finalize();
		_TagProbe_Electron_Excl_1234->finalize();
		_TagProbe_Electron_Excl_12345->finalize();
		_TagProbe_Electron_Excl_123456->finalize();
		_TagProbe_Electron_Excl_1234567->finalize();
		_TagProbe_Electron_Excl_Global->finalize();
		_TagProbe_Electron_Excl_Double->finalize();

		_TagProbe_Electron_Incl_12->finalize();
		_TagProbe_Electron_Incl_123->finalize();
		_TagProbe_Electron_Incl_1234->finalize();
		_TagProbe_Electron_Incl_12345->finalize();
		_TagProbe_Electron_Incl_123456->finalize();
		_TagProbe_Electron_Incl_1234567->finalize();
		_TagProbe_Electron_Incl_Global->finalize();
		_TagProbe_Electron_Incl_Double->finalize();
	}

	if(_selections=="ASYM"){
		_TagProbe_Electron0_Excl_12->finalize();
		_TagProbe_Electron0_Excl_123->finalize();
		_TagProbe_Electron0_Excl_1234->finalize();
		_TagProbe_Electron0_Excl_12345->finalize();
		_TagProbe_Electron0_Excl_123456->finalize();
		_TagProbe_Electron0_Excl_1234567->finalize();
		_TagProbe_Electron0_Excl_Global->finalize();
		_TagProbe_Electron0_Excl_Double->finalize();
	
		_TagProbe_Electron0_Incl_12->finalize();
		_TagProbe_Electron0_Incl_123->finalize();
		_TagProbe_Electron0_Incl_1234->finalize();
		_TagProbe_Electron0_Incl_12345->finalize();
		_TagProbe_Electron0_Incl_123456->finalize();
		_TagProbe_Electron0_Incl_1234567->finalize();
		_TagProbe_Electron0_Incl_Global->finalize();
		_TagProbe_Electron0_Incl_Double->finalize();

		_TagProbe_Electron1_Excl_12->finalize();
		_TagProbe_Electron1_Excl_123->finalize();
		_TagProbe_Electron1_Excl_1234->finalize();
		_TagProbe_Electron1_Excl_12345->finalize();
		_TagProbe_Electron1_Excl_123456->finalize();
		_TagProbe_Electron1_Excl_1234567->finalize();
		_TagProbe_Electron1_Excl_Global->finalize();
		_TagProbe_Electron1_Excl_Double->finalize();
 	
 		_TagProbe_Electron1_Incl_12->finalize();
 		_TagProbe_Electron1_Incl_123->finalize();
 		_TagProbe_Electron1_Incl_1234->finalize();
		_TagProbe_Electron1_Incl_12345->finalize();
		_TagProbe_Electron1_Incl_123456->finalize();
		_TagProbe_Electron1_Incl_1234567->finalize();
		_TagProbe_Electron1_Incl_Global->finalize();
		_TagProbe_Electron1_Incl_Double->finalize();
	}

	if(_selections=="SYM"){
		_TagProbe_Electron_Pt_Barrel_12->finalize();
		_TagProbe_Electron_Pt_Barrel_123->finalize();
		_TagProbe_Electron_Pt_Barrel_1234->finalize();
		_TagProbe_Electron_Pt_Barrel_12345->finalize();
		_TagProbe_Electron_Pt_Barrel_123456->finalize();
		_TagProbe_Electron_Pt_Barrel_1234567->finalize();
		_TagProbe_Electron_Pt_Barrel_Global->finalize();
 
		_TagProbe_Electron_Pt_Endcap_12->finalize();
		_TagProbe_Electron_Pt_Endcap_123->finalize();
		_TagProbe_Electron_Pt_Endcap_1234->finalize();
		_TagProbe_Electron_Pt_Endcap_12345->finalize();
		_TagProbe_Electron_Pt_Endcap_123456->finalize();
		_TagProbe_Electron_Pt_Endcap_1234567->finalize();
		_TagProbe_Electron_Pt_Endcap_Global->finalize();
	}

	if(_selections=="ASYM"){
		_TagProbe_Electron0_Pt_Barrel_12->finalize();
		_TagProbe_Electron0_Pt_Barrel_123->finalize();
		_TagProbe_Electron0_Pt_Barrel_1234->finalize();
		_TagProbe_Electron0_Pt_Barrel_12345->finalize();
		_TagProbe_Electron0_Pt_Barrel_123456->finalize();
		_TagProbe_Electron0_Pt_Barrel_1234567->finalize();
		_TagProbe_Electron0_Pt_Barrel_Global->finalize();
 
		_TagProbe_Electron0_Pt_Endcap_12->finalize();
		_TagProbe_Electron0_Pt_Endcap_123->finalize();
		_TagProbe_Electron0_Pt_Endcap_1234->finalize();
		_TagProbe_Electron0_Pt_Endcap_12345->finalize();
		_TagProbe_Electron0_Pt_Endcap_123456->finalize();
		_TagProbe_Electron0_Pt_Endcap_1234567->finalize();
		_TagProbe_Electron0_Pt_Endcap_Global->finalize();

		_TagProbe_Electron1_Pt_Barrel_12->finalize();
		_TagProbe_Electron1_Pt_Barrel_123->finalize();
		_TagProbe_Electron1_Pt_Barrel_1234->finalize();
		_TagProbe_Electron1_Pt_Barrel_12345->finalize();
		_TagProbe_Electron1_Pt_Barrel_123456->finalize();
		_TagProbe_Electron1_Pt_Barrel_1234567->finalize();
		_TagProbe_Electron1_Pt_Barrel_Global->finalize();
 
		_TagProbe_Electron1_Pt_Endcap_12->finalize();
		_TagProbe_Electron1_Pt_Endcap_123->finalize();
		_TagProbe_Electron1_Pt_Endcap_1234->finalize();
		_TagProbe_Electron1_Pt_Endcap_12345->finalize();
		_TagProbe_Electron1_Pt_Endcap_123456->finalize();
		_TagProbe_Electron1_Pt_Endcap_1234567->finalize();
		_TagProbe_Electron1_Pt_Endcap_Global->finalize();
	}

	_file->Write();

}

