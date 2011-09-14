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

genMassZEff_1(0), genPtZEff_1(0), genEtaZEff_1(0),
genMassZEff_12(0), genPtZEff_12(0), genEtaZEff_12(0),
genMassZEff_123(0), genPtZEff_123(0), genEtaZEff_123(0),
genMassZEff_1234(0), genPtZEff_1234(0), genEtaZEff_1234(0),
genMassZEff_12345(0), genPtZEff_12345(0), genEtaZEff_12345(0),
genMassZEff_123456(0), genPtZEff_123456(0), genEtaZEff_123456(0),

RecoInclJetEffWGA_1(0), RecoInclJetEffWGA_12(0), RecoInclJetEffWGA_123(0), RecoInclJetEffWGA_1234(0), RecoInclJetEffWGA_12345(0), RecoInclJetEffWGA_123456(0),
RecoExclJetEffWGA_1(0), RecoExclJetEffWGA_12(0), RecoExclJetEffWGA_123(0), RecoExclJetEffWGA_1234(0), RecoExclJetEffWGA_12345(0), RecoExclJetEffWGA_123456(0),

RecoInclJetEff_1(0), RecoInclJetEff_12(0), RecoInclJetEff_123(0), RecoInclJetEff_1234(0), RecoInclJetEff_12345(0), RecoInclJetEff_123456(0),
RecoExclJetEff_1(0), RecoExclJetEff_12(0), RecoExclJetEff_123(0), RecoExclJetEff_1234(0), RecoExclJetEff_12345(0), RecoExclJetEff_123456(0),

genLeadElEtaEff_1(0), genLeadElEtaEff_12(0), genLeadElEtaEff_123(0), genLeadElEtaEff_1234(0), genLeadElEtaEff_12345(0), genLeadElEtaEff_123456(0),
genLeadElPtEff_1(0), genLeadElPtEff_12(0), genLeadElPtEff_123(0), genLeadElPtEff_1234(0), genLeadElPtEff_12345(0), genLeadElPtEff_123456(0),

AccDenom_genPtZ(0), AccDenom_genMassZ(0), AccDenom_genEtaZ(0), AccDenom_genLeadElEta(0), AccDenom_genLeadElPt(0), AccDenom_GenExclJetNumber(0), AccDenom_RecoExclJetNumber(0), AccDenom_RecoInclJetNumber(0), AccDenom_GenInclJetNumber(0), 
EffDenom_genPtZ(0), EffDenom_genMassZ(0), EffDenom_genEtaZ(0), EffDenom_genLeadElEta(0), EffDenom_genLeadElPt(0), EffDenom_GenExclJetNumber(0), EffDenom_RecoExclJetNumber(0), EffDenom_RecoInclJetNumber(0), EffDenom_GenInclJetNumber(0), 

_sample("mc"), _selections("ASYM"), _JetType("PF"), _dir(0), _file(0), _Acc(1), _Trg(2), _Conv(3), _Imp(4), _Iso(5), _EiD(6), _histoVector(), _nbin(10), _xmin(-0.5), _xmax(9.5), _Norm(false), _norm(1.), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1)

{ }

void EfficiencyElectron::begin(TFile* out, const edm::ParameterSet& iConfig){
   _file = out; 
    
    _sample = iConfig.getParameter<std::string>("Sample");
    std::string dirname = iConfig.getParameter<std::string>("Name");
    std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
    _selections = iConfig.getParameter<std::string>("Selections");
    _Norm      = iConfig.getParameter<bool>("Norm");
    _EventsPerFile    = iConfig.getParameter<int32_t>("EventsPerFile");
    _EventNumber    = iConfig.getParameter<int32_t>("EventNumber");
    _ProcEvents    = iConfig.getParameter<int32_t>("ProcEvents");
    _targetLumi= iConfig.getParameter<double>("targetLumi");
    _xsec      = iConfig.getParameter<double>("CrossSection");
    _JetType = iConfig.getParameter<std::string>("JetType");
    
    //Selections
   _Acc = iConfig.getParameter<int32_t>("Acc");
   _Trg = iConfig.getParameter<int32_t>("Trg");
   _Conv = iConfig.getParameter<int32_t>("Conv");
   _Imp = iConfig.getParameter<int32_t>("Imp");
   _Iso = iConfig.getParameter<int32_t>("Iso");
   _EiD = iConfig.getParameter<int32_t>("EiD");
   
   for(int i=0; i<7; i++){
   _RecoCutFlags[i] = "_1";}
   
   if(_selections=="SYM"){
   _RecoCutFlags[_Acc] =  "_AccSYM";
   _RecoCutFlags[_Conv] = "_ConvSYM"; 
   _RecoCutFlags[_Iso] =  "_IsoSYM";
   _RecoCutFlags[_EiD] =  "_EiDSYM";}
   if(_selections=="ASYM"){
   _RecoCutFlags[_Acc] =  "_AccASYM";
   _RecoCutFlags[_Conv] = "_ConvASYM"; 
   _RecoCutFlags[_Iso] =  "_IsoASYM";
   _RecoCutFlags[_EiD] =  "_EiDASYM";}
     
   _RecoCutFlags[_Trg] =  "_Trg";  
   _RecoCutFlags[_Imp] =  "_Imp";
  
    cout << "EfficiencyElectron file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();
   
   //Acceptance denominator
   AccDenom_genMassZ = new TH1D("AccDenom_genMassZ", "Generated Z Mass, No acceptance cut applied", 100, 60, 120);
   _histoVector.push_back(AccDenom_genMassZ);
   AccDenom_genPtZ = new TH1D("AccDenom_genPtZ", "Generated Z pt, No acceptance cut applied",  200, 0, 200);
   _histoVector.push_back(AccDenom_genPtZ);
   AccDenom_genEtaZ = new TH1D("AccDenom_genEtaZ", "Generated Z #eta, No acceptance cut applied",  100, -10, 10);
   _histoVector.push_back(AccDenom_genEtaZ);
   AccDenom_genLeadElEta = new TH1D("AccDenom_genLeadElEta", "Generated Leading electron #eta, No acceptance cut applied", 100, -2.5, 2.5);
   _histoVector.push_back(AccDenom_genLeadElEta);
   AccDenom_genLeadElPt = new TH1D("AccDenom_genLeadElPt", "Generated Leading electron Pt, No acceptance cut applied", 200, 0, 200);
   _histoVector.push_back(AccDenom_genLeadElPt);
   AccDenom_GenExclJetNumber = new TH1D("AccDenom_GenExclJetNumber", "Number of Gen Iso Jet, No acceptance cut applied", 10, -0.5, 9.5);
   _histoVector.push_back(AccDenom_GenExclJetNumber);
   AccDenom_RecoExclJetNumber = new TH1D("AccDenom_RecoExclJetNumber", "Number of Exclusive Reco Jet, No acceptance cut applied", 10, -0.5, 9.5);
   _histoVector.push_back(AccDenom_RecoExclJetNumber);
   AccDenom_RecoInclJetNumber = new TH1D("AccDenom_RecoInclJetNumber", "Number of Inclusive Reco Jet, No acceptance cut applied", 10, -0.5, 9.5);
   _histoVector.push_back(AccDenom_RecoInclJetNumber);
   AccDenom_GenInclJetNumber = new TH1D("AccDenom_GenInclJetNumber", "Number of Inclusive Gen Jet, No acceptance cut applied", 10, -0.5, 9.5);
   _histoVector.push_back(AccDenom_GenInclJetNumber);
   
   //Efficiency denominator
   EffDenom_genMassZ = new TH1D("EffDenom_genMassZ", "Generated Z Mass, in acceptance", 100, 60, 120);
   _histoVector.push_back(EffDenom_genMassZ);
   EffDenom_genPtZ = new TH1D("EffDenom_genPtZ", "Generated Z pt, in acceptance",  200, 0, 200);
   _histoVector.push_back(EffDenom_genPtZ);
   EffDenom_genEtaZ = new TH1D("EffDenom_genEtaZ", "Generated Z #eta, in acceptance",  100, -10, 10);
   _histoVector.push_back(EffDenom_genEtaZ);
   EffDenom_genLeadElEta = new TH1D("EffDenom_genLeadElEta", "Generated Leading electron #eta, in acceptance", 100, -2.5, 2.5);
   _histoVector.push_back(EffDenom_genLeadElEta);
   EffDenom_genLeadElPt = new TH1D("EffDenom_genLeadElPt", "Generated Leading electron Pt, in acceptance", 200, 0, 200);
   _histoVector.push_back(EffDenom_genLeadElPt);
   EffDenom_GenExclJetNumber = new TH1D("EffDenom_GenExclJetNumber", "Number of Gen Iso Jet, in acceptance", 10, -0.5, 9.5);
   _histoVector.push_back(EffDenom_GenExclJetNumber);
   EffDenom_RecoExclJetNumber = new TH1D("EffDenom_RecoExclJetNumber", "Number of Exclusive Reco Iso Jet, in acceptance", 10, -0.5, 9.5);
   _histoVector.push_back(EffDenom_RecoExclJetNumber);
   EffDenom_RecoInclJetNumber = new TH1D("EffDenom_RecoInclJetNumber", "Number of Inclusive Reco Iso Jet, in acceptance", 10, -0.5, 9.5);
   _histoVector.push_back(EffDenom_RecoInclJetNumber);
   EffDenom_GenInclJetNumber = new TH1D("EffDenom_GenInclJetNumber", "Number of Inclusive Gen Iso Jet, in acceptance", 10, -0.5, 9.5);
   _histoVector.push_back(EffDenom_GenInclJetNumber);
   
   //Eff vs Z electrons properties
   
   string genLeadElEtaEff_name = "genLeadElEtaEff";
   genLeadElEtaEff_name+=_RecoCutFlags[1].c_str();
   genLeadElEtaEff_1 = new TH1D(genLeadElEtaEff_name.c_str(), "Generated Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(genLeadElEtaEff_1);
   genLeadElEtaEff_name+=_RecoCutFlags[2].c_str();
   genLeadElEtaEff_12 = new TH1D(genLeadElEtaEff_name.c_str(), "Generated Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(genLeadElEtaEff_12);
   genLeadElEtaEff_name+=_RecoCutFlags[3].c_str();
   genLeadElEtaEff_123 = new TH1D(genLeadElEtaEff_name.c_str(), "Generated Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(genLeadElEtaEff_123);
   genLeadElEtaEff_name+=_RecoCutFlags[4].c_str();
   genLeadElEtaEff_1234 = new TH1D(genLeadElEtaEff_name.c_str(), "Generated Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(genLeadElEtaEff_1234);
   genLeadElEtaEff_name+=_RecoCutFlags[5].c_str();
   genLeadElEtaEff_12345 = new TH1D(genLeadElEtaEff_name.c_str(), "Generated Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(genLeadElEtaEff_12345);
   genLeadElEtaEff_name+=_RecoCutFlags[6].c_str();
   genLeadElEtaEff_123456 = new TH1D(genLeadElEtaEff_name.c_str(), "Generated Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(genLeadElEtaEff_123456);  
   
   string genLeadElPtEff_name = "genLeadElPtEff";
   genLeadElPtEff_name+=_RecoCutFlags[1].c_str();
   genLeadElPtEff_1 = new TH1D(genLeadElPtEff_name.c_str(), "Generated Leading electron Pt", 200, 0, 200);
   _histoVector.push_back(genLeadElPtEff_1);
   genLeadElPtEff_name+=_RecoCutFlags[2].c_str();
   genLeadElPtEff_12 = new TH1D(genLeadElPtEff_name.c_str(), "Generated Leading electron Pt", 200, 0, 200);
   _histoVector.push_back(genLeadElPtEff_12);
   genLeadElPtEff_name+=_RecoCutFlags[3].c_str();
   genLeadElPtEff_123 = new TH1D(genLeadElPtEff_name.c_str(), "Generated Leading electron Pt", 200, 0, 200);
   _histoVector.push_back(genLeadElPtEff_123);
   genLeadElPtEff_name+=_RecoCutFlags[4].c_str();
   genLeadElPtEff_1234 = new TH1D(genLeadElPtEff_name.c_str(), "Generated Leading electron Pt", 200, 0, 200);
   _histoVector.push_back(genLeadElPtEff_1234);
   genLeadElPtEff_name+=_RecoCutFlags[5].c_str();
   genLeadElPtEff_12345 = new TH1D(genLeadElPtEff_name.c_str(), "Generated Leading electron Pt", 200, 0, 200);
   _histoVector.push_back(genLeadElPtEff_12345);
   genLeadElPtEff_name+=_RecoCutFlags[6].c_str();
   genLeadElPtEff_123456 = new TH1D(genLeadElPtEff_name.c_str(), "Generated Leading electron Pt", 200, 0, 200);
   _histoVector.push_back(genLeadElPtEff_123456);
   
   //Eff vs Z properties
   
   string genMassZEff_name = "genMassZEff";
   genMassZEff_name+=_RecoCutFlags[1].c_str();   
   genMassZEff_1 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 100, 60, 120);
   _histoVector.push_back(genMassZEff_1);
   genMassZEff_name+=_RecoCutFlags[2].c_str();
   genMassZEff_12 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 100, 60, 120);
   _histoVector.push_back(genMassZEff_12);
   genMassZEff_name+=_RecoCutFlags[3].c_str();
   genMassZEff_123 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 100, 60, 120);
   _histoVector.push_back(genMassZEff_123);
   genMassZEff_name+=_RecoCutFlags[4].c_str();
   genMassZEff_1234 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 100, 60, 120);
   _histoVector.push_back(genMassZEff_1234);
   genMassZEff_name+=_RecoCutFlags[5].c_str();
   genMassZEff_12345 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 100, 60, 120);
   _histoVector.push_back(genMassZEff_12345);
   genMassZEff_name+=_RecoCutFlags[6].c_str();
   genMassZEff_123456 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 100, 60, 120);
   _histoVector.push_back(genMassZEff_123456);
   
   string genPtZEff_name = "genPtZEff";
   genPtZEff_name+=_RecoCutFlags[1].c_str();
   genPtZEff_1 = new TH1D(genPtZEff_name.c_str(), "Generated Z pt",  200, 0, 200);
   _histoVector.push_back(genPtZEff_1);
   genPtZEff_name+=_RecoCutFlags[2].c_str();
   genPtZEff_12 = new TH1D(genPtZEff_name.c_str(), "Generated Z pt",  200, 0, 200);
   _histoVector.push_back(genPtZEff_12);
   genPtZEff_name+=_RecoCutFlags[3].c_str();
   genPtZEff_123 = new TH1D(genPtZEff_name.c_str(), "Generated Z pt",  200, 0, 200);
   _histoVector.push_back(genPtZEff_123);
   genPtZEff_name+=_RecoCutFlags[4].c_str();
   genPtZEff_1234 = new TH1D(genPtZEff_name.c_str(), "Generated Z pt",  200, 0, 200);
   _histoVector.push_back(genPtZEff_1234);
   genPtZEff_name+=_RecoCutFlags[5].c_str();
   genPtZEff_12345 = new TH1D(genPtZEff_name.c_str(), "Generated Z pt",  200, 0, 200);
   _histoVector.push_back(genPtZEff_12345);
   genPtZEff_name+=_RecoCutFlags[6].c_str();
   genPtZEff_123456 = new TH1D(genPtZEff_name.c_str(), "Generated Z pt",  200, 0, 200);
   _histoVector.push_back(genPtZEff_123456);
   
   string genEtaZEff_name = "genEtaZEff";
   genEtaZEff_name+=_RecoCutFlags[1].c_str();
   genEtaZEff_1 = new TH1D(genEtaZEff_name.c_str(), "Generated Z #eta",  100, -10, 10);
   _histoVector.push_back(genEtaZEff_1);
   genEtaZEff_name+=_RecoCutFlags[2].c_str();
   genEtaZEff_12 = new TH1D(genEtaZEff_name.c_str(), "Generated Z #eta",  100, -10, 10);
   _histoVector.push_back(genEtaZEff_12);
   genEtaZEff_name+=_RecoCutFlags[3].c_str();
   genEtaZEff_123 = new TH1D(genEtaZEff_name.c_str(), "Generated Z #eta",  100, -10, 10);
   _histoVector.push_back(genEtaZEff_123);
   genEtaZEff_name+=_RecoCutFlags[4].c_str();
   genEtaZEff_1234 = new TH1D(genEtaZEff_name.c_str(), "Generated Z #eta",  100, -10, 10);
   _histoVector.push_back(genEtaZEff_1234);
   genEtaZEff_name+=_RecoCutFlags[5].c_str();
   genEtaZEff_12345 = new TH1D(genEtaZEff_name.c_str(), "Generated Z #eta",  100, -10, 10);
   _histoVector.push_back(genEtaZEff_12345);
   genEtaZEff_name+=_RecoCutFlags[6].c_str();
   genEtaZEff_123456 = new TH1D(genEtaZEff_name.c_str(), "Generated Z #eta",  100, -10, 10);
   _histoVector.push_back(genEtaZEff_123456);
   
   //Eff vs Reco Excl Jet (iso) number With Gen(Acc) Req.
   string RecoExclJetEffWGA_name = "RecoExclJetEffWGenAcc";
   RecoExclJetEffWGA_name+=_RecoCutFlags[1].c_str();
   RecoExclJetEffWGA_1 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEffWGA_1);
   RecoExclJetEffWGA_name+=_RecoCutFlags[2].c_str();
   RecoExclJetEffWGA_12 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEffWGA_12);
   RecoExclJetEffWGA_name+=_RecoCutFlags[3].c_str();
   RecoExclJetEffWGA_123 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEffWGA_123);
   RecoExclJetEffWGA_name+=_RecoCutFlags[4].c_str();
   RecoExclJetEffWGA_1234 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEffWGA_1234);
   RecoExclJetEffWGA_name+=_RecoCutFlags[5].c_str();
   RecoExclJetEffWGA_12345 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEffWGA_12345);
   RecoExclJetEffWGA_name+=_RecoCutFlags[6].c_str();
   RecoExclJetEffWGA_123456 = new TH1D(RecoExclJetEffWGA_name.c_str(), "Number of Exclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEffWGA_123456);
   
   //Eff vs Reco Incl Jet (iso) number With Gen(Acc) Req.
   string RecoInclJetEffWGA_name = "RecoInclJetEffWGenAcc";
   RecoInclJetEffWGA_name+=_RecoCutFlags[1].c_str();
   RecoInclJetEffWGA_1 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEffWGA_1);
   RecoInclJetEffWGA_name+=_RecoCutFlags[2].c_str();
   RecoInclJetEffWGA_12 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEffWGA_12);
   RecoInclJetEffWGA_name+=_RecoCutFlags[3].c_str();
   RecoInclJetEffWGA_123 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEffWGA_123);
   RecoInclJetEffWGA_name+=_RecoCutFlags[4].c_str();
   RecoInclJetEffWGA_1234 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEffWGA_1234);
   RecoInclJetEffWGA_name+=_RecoCutFlags[5].c_str();
   RecoInclJetEffWGA_12345 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEffWGA_12345);
   RecoInclJetEffWGA_name+=_RecoCutFlags[6].c_str();
   RecoInclJetEffWGA_123456 = new TH1D(RecoInclJetEffWGA_name.c_str(), "Number of Inclusive Reco Iso Jet With Gen(Acc) Req.", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEffWGA_123456); 
   
   //Eff vs Reco Incl (Iso) Jet number
   string RecoInclJetEff_name = "RecoInclJetEff";
   RecoInclJetEff_name+=_RecoCutFlags[1].c_str();
   RecoInclJetEff_1 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEff_1);
   RecoInclJetEff_name+=_RecoCutFlags[2].c_str();
   RecoInclJetEff_12 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEff_12);
   RecoInclJetEff_name+=_RecoCutFlags[3].c_str();
   RecoInclJetEff_123 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEff_123);
   RecoInclJetEff_name+=_RecoCutFlags[4].c_str();
   RecoInclJetEff_1234 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEff_1234);
   RecoInclJetEff_name+=_RecoCutFlags[5].c_str();
   RecoInclJetEff_12345 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEff_12345);
   RecoInclJetEff_name+=_RecoCutFlags[6].c_str();
   RecoInclJetEff_123456 = new TH1D(RecoInclJetEff_name.c_str(), "Number of Inclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoInclJetEff_123456);
   
   //Eff vs Reco Excl (Iso) Jet number
   string RecoExclJetEff_name = "RecoExclJetEff";
   RecoExclJetEff_name+=_RecoCutFlags[1].c_str();
   RecoExclJetEff_1 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEff_1);
   RecoExclJetEff_name+=_RecoCutFlags[2].c_str();
   RecoExclJetEff_12 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEff_12);
   RecoExclJetEff_name+=_RecoCutFlags[3].c_str();
   RecoExclJetEff_123 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEff_123);
   RecoExclJetEff_name+=_RecoCutFlags[4].c_str();
   RecoExclJetEff_1234 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEff_1234);
   RecoExclJetEff_name+=_RecoCutFlags[5].c_str();
   RecoExclJetEff_12345 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEff_12345);
   RecoExclJetEff_name+=_RecoCutFlags[6].c_str();
   RecoExclJetEff_123456 = new TH1D(RecoExclJetEff_name.c_str(), "Number of Exclusive Reco Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(RecoExclJetEff_123456);
   
// Tag & Probe

  if(_selections=="SYM"){
  tag_cuts.push_back(singleEl_Tag_SYM);
  tag_cuts_AllSel.push_back(singleEl_Tag_AllSel_SYM);}
  if(_selections=="ASYM"){
  tag_cuts_0.push_back(singleEl_Tag_ASYM0);
  tag_cuts_1.push_back(singleEl_Tag_ASYM1);
  tag_cuts_0_AllSel.push_back(singleEl_Tag_AllSel_ASYM0);
  tag_cuts_1_AllSel.push_back(singleEl_Tag_AllSel_ASYM1);}
  
  if(_selections=="SYM"){
  
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts_1;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts_12;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts_123;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts_1234;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts_12345;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts_123456;
  
  probe_cuts_1.clear();
  probe_cuts_12.clear();
  probe_cuts_123.clear();
  probe_cuts_1234.clear();
  probe_cuts_12345.clear();
  probe_cuts_123456.clear();
  
  for(unsigned int i = 1; i < 7; i++){
  if(_RecoCutFlags[i] == "_AccSYM"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_Acc_SYM);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_Acc_SYM);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_Acc_SYM);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_Acc_SYM);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_Acc_SYM);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_Acc_SYM);}
  if(_RecoCutFlags[i] == "_ConvSYM"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_Conv_SYM);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_Conv_SYM);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_Conv_SYM);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_Conv_SYM);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_Conv_SYM);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_Conv_SYM);}
  if(_RecoCutFlags[i] == "_Imp"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_Imp);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_Imp);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_Imp);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_Imp);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_Imp);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_Imp);}
  if(_RecoCutFlags[i] == "_IsoSYM"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_Iso_SYM);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_Iso_SYM);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_Iso_SYM);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_Iso_SYM);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_Iso_SYM);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_Iso_SYM);}
  if(_RecoCutFlags[i] == "_EiDSYM"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_EiD_SYM);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_EiD_SYM);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_EiD_SYM);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_EiD_SYM);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_EiD_SYM);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_EiD_SYM);}
  if(_RecoCutFlags[i] == "_1"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_True);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_True);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_True);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_True);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_True);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_True);}
  }
  
  string name_TagDir_Excl="Tag&Probe_Excl";
  string name_TagDir_Incl="Tag&Probe_Incl";
  string name_TPFiller="Electron";
  
  name_TagDir_Excl+=_RecoCutFlags[1].c_str();
  name_TagDir_Excl+=_RecoCutFlags[2].c_str();  
  name_TagDir_Incl+=_RecoCutFlags[1].c_str();
  name_TagDir_Incl+=_RecoCutFlags[2].c_str();
  name_TPFiller+=_RecoCutFlags[1].c_str();
  name_TPFiller+=_RecoCutFlags[2].c_str();
  
  string name_TPFiller_12 = name_TPFiller+"_";
  TDirectory *TagDir_Excl_12 = _dir->mkdir(name_TagDir_Excl.c_str());
  TDirectory *TagDir_Incl_12 = _dir->mkdir(name_TagDir_Incl.c_str());
  
  _TagProbe_Electron_Excl_12 = new TagAndProbeFillerElectron(TagDir_Excl_12, string(name_TPFiller_12.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_1, probe_cuts_12);
  _TagProbe_Electron_Incl_12 = new TagAndProbeFillerElectron(TagDir_Incl_12, string(name_TPFiller_12.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_1, probe_cuts_12);
  
  name_TPFiller+=_RecoCutFlags[3].c_str();
  string name_TPFiller_123 = name_TPFiller+"_"; 
  name_TagDir_Excl+=_RecoCutFlags[3].c_str();
  TDirectory *TagDir_Excl_123 = _dir->mkdir(name_TagDir_Excl.c_str());
  name_TagDir_Incl+=_RecoCutFlags[3].c_str();
  TDirectory *TagDir_Incl_123 = _dir->mkdir(name_TagDir_Incl.c_str());
  
  _TagProbe_Electron_Excl_123 = new TagAndProbeFillerElectron(TagDir_Excl_123, string(name_TPFiller_123.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_12, probe_cuts_123);
  _TagProbe_Electron_Incl_123 = new TagAndProbeFillerElectron(TagDir_Incl_123, string(name_TPFiller_123.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_12, probe_cuts_123);
  
  name_TPFiller+=_RecoCutFlags[4].c_str();
  string name_TPFiller_1234 = name_TPFiller+"_";
  name_TagDir_Excl+=_RecoCutFlags[4].c_str();
  TDirectory *TagDir_Excl_1234 = _dir->mkdir(name_TagDir_Excl.c_str()); 
  name_TagDir_Incl+=_RecoCutFlags[4].c_str();
  TDirectory *TagDir_Incl_1234 = _dir->mkdir(name_TagDir_Incl.c_str());
  
  _TagProbe_Electron_Excl_1234 = new TagAndProbeFillerElectron(TagDir_Excl_1234, string(name_TPFiller_1234.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_123, probe_cuts_1234);
  _TagProbe_Electron_Incl_1234 = new TagAndProbeFillerElectron(TagDir_Incl_1234, string(name_TPFiller_1234.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_123, probe_cuts_1234);
  
  name_TPFiller+=_RecoCutFlags[5].c_str();
  string name_TPFiller_12345 = name_TPFiller+"_";
  name_TagDir_Excl+=_RecoCutFlags[5].c_str();
  TDirectory *TagDir_Excl_12345 = _dir->mkdir(name_TagDir_Excl.c_str());
  name_TagDir_Incl+=_RecoCutFlags[5].c_str();
  TDirectory *TagDir_Incl_12345 = _dir->mkdir(name_TagDir_Incl.c_str());
  
  _TagProbe_Electron_Excl_12345 = new TagAndProbeFillerElectron(TagDir_Excl_12345, string(name_TPFiller_12345.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_1234, probe_cuts_12345);
  _TagProbe_Electron_Incl_12345 = new TagAndProbeFillerElectron(TagDir_Incl_12345, string(name_TPFiller_12345.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_1234, probe_cuts_12345);
  
  name_TPFiller+=_RecoCutFlags[6].c_str();
  string name_TPFiller_123456 = name_TPFiller+"_";
  name_TagDir_Excl+=_RecoCutFlags[6].c_str();  
  TDirectory *TagDir_Excl_123456 = _dir->mkdir(name_TagDir_Excl.c_str());
  name_TagDir_Incl+=_RecoCutFlags[6].c_str();
  TDirectory *TagDir_Incl_123456 = _dir->mkdir(name_TagDir_Incl.c_str());
  
  _TagProbe_Electron_Excl_123456 = new TagAndProbeFillerElectron(TagDir_Excl_123456, string(name_TPFiller_123456.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_12345, probe_cuts_123456);
  _TagProbe_Electron_Incl_123456 = new TagAndProbeFillerElectron(TagDir_Incl_123456, string(name_TPFiller_123456.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_12345, probe_cuts_123456);
  
  string name_TPFiller_Global = "Electron_Global_";
  TDirectory *TagDir_Excl_Global = _dir->mkdir("Tag&Probe_Excl_Global");
  TDirectory *TagDir_Incl_Global = _dir->mkdir("Tag&Probe_Incl_Global");
  
  _TagProbe_Electron_Excl_Global = new TagAndProbeFillerElectron(TagDir_Excl_Global, string(name_TPFiller_Global.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_1, probe_cuts_123456);
  _TagProbe_Electron_Incl_Global = new TagAndProbeFillerElectron(TagDir_Incl_Global, string(name_TPFiller_Global.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_1, probe_cuts_123456);
  
  //Fit del plot di massa di double electron
  string name_TPFiller_Double = "Electron_Double_";
  TDirectory *TagDir_Excl_Double = _dir->mkdir("Tag&Probe_Excl_Double");
  TDirectory *TagDir_Incl_Double = _dir->mkdir("Tag&Probe_Incl_Double");
  
  _TagProbe_Electron_Excl_Double = new TagAndProbeFillerElectron(TagDir_Excl_Double, string(name_TPFiller_Double.c_str()), _nbin, _xmin, _xmax, tag_cuts_AllSel, probe_cuts_1, probe_cuts_123456);
  _TagProbe_Electron_Incl_Double = new TagAndProbeFillerElectron(TagDir_Incl_Double, string(name_TPFiller_Double.c_str()), _nbin, _xmin, _xmax, tag_cuts_AllSel, probe_cuts_1, probe_cuts_123456);
  
  }
  
  if(_selections=="ASYM"){
  
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts0_1;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts0_12;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts0_123;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts0_1234;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts0_12345;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts0_123456;
  
  probe_cuts0_1.clear();
  probe_cuts0_12.clear();
  probe_cuts0_123.clear();
  probe_cuts0_1234.clear();
  probe_cuts0_12345.clear();
  probe_cuts0_123456.clear();
  
  for(unsigned int i = 1; i < 7; i++){
  if(_RecoCutFlags[i] == "_AccASYM"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_Acc_ASYM0);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_Acc_ASYM0);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_Acc_ASYM0);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_Acc_ASYM0);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_Acc_ASYM0);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_Acc_ASYM0);}
  if(_RecoCutFlags[i] == "_Trg"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_Trg_ASYM0);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_Trg_ASYM0);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_Trg_ASYM0);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_Trg_ASYM0);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_Trg_ASYM0);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_Trg_ASYM0);}
  if(_RecoCutFlags[i] == "_Imp"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_Imp);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_Imp);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_Imp);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_Imp);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_Imp);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_Imp);}
  if(_RecoCutFlags[i] == "_ConvASYM"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_Conv_ASYM0);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_Conv_ASYM0);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_Conv_ASYM0);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_Conv_ASYM0);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_Conv_ASYM0);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_Conv_ASYM0);} 
  if(_RecoCutFlags[i] == "_IsoASYM"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_Iso_ASYM0);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_Iso_ASYM0);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_Iso_ASYM0);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_Iso_ASYM0);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_Iso_ASYM0);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_Iso_ASYM0);}
  if(_RecoCutFlags[i] == "_EiDASYM"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_EiD_ASYM0);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_EiD_ASYM0);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_EiD_ASYM0);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_EiD_ASYM0);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_EiD_ASYM0);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_EiD_ASYM0);}
  if(_RecoCutFlags[i] == "_1"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_True);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_True);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_True);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_True);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_True);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_True);}
  }
  
  string name_TagDir0_Excl="Tag&Probe0_Excl";
  string name_TagDir0_Incl="Tag&Probe0_Incl";
  string name_TPFiller0="Electron";
  
  name_TagDir0_Excl+=_RecoCutFlags[1].c_str();  
  name_TagDir0_Excl+=_RecoCutFlags[2].c_str();
  name_TagDir0_Incl+=_RecoCutFlags[1].c_str();  
  name_TagDir0_Incl+=_RecoCutFlags[2].c_str();
  name_TPFiller0+=_RecoCutFlags[1].c_str();
  name_TPFiller0+=_RecoCutFlags[2].c_str();
  
  string name_TPFiller0_12 = name_TPFiller0+"_";
  TDirectory *TagDir0_Excl_12 = _dir->mkdir(name_TagDir0_Excl.c_str());
  TDirectory *TagDir0_Incl_12 = _dir->mkdir(name_TagDir0_Incl.c_str());
  
  _TagProbe_Electron0_Excl_12 = new TagAndProbeFillerElectron(TagDir0_Excl_12, string(name_TPFiller0_12.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_12, "hard");
  _TagProbe_Electron0_Incl_12 = new TagAndProbeFillerElectron(TagDir0_Incl_12, string(name_TPFiller0_12.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_12, "hard");
  
  name_TPFiller0+=_RecoCutFlags[3].c_str();
  string name_TPFiller0_123 = name_TPFiller0+"_";
  name_TagDir0_Excl+=_RecoCutFlags[3].c_str(); 
  TDirectory *TagDir0_Excl_123 = _dir->mkdir(name_TagDir0_Excl.c_str());
  name_TagDir0_Incl+=_RecoCutFlags[3].c_str(); 
  TDirectory *TagDir0_Incl_123 = _dir->mkdir(name_TagDir0_Incl.c_str());
  
  _TagProbe_Electron0_Excl_123 = new TagAndProbeFillerElectron(TagDir0_Excl_123, string(name_TPFiller0_123.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_12, probe_cuts0_123, "hard");
  _TagProbe_Electron0_Incl_123 = new TagAndProbeFillerElectron(TagDir0_Incl_123, string(name_TPFiller0_123.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_12, probe_cuts0_123, "hard");
  
  name_TPFiller0+=_RecoCutFlags[4].c_str();
  string name_TPFiller0_1234 = name_TPFiller0+"_";
  name_TagDir0_Excl+=_RecoCutFlags[4].c_str();
  TDirectory *TagDir0_Excl_1234 = _dir->mkdir(name_TagDir0_Excl.c_str());
  name_TagDir0_Incl+=_RecoCutFlags[4].c_str();
  TDirectory *TagDir0_Incl_1234 = _dir->mkdir(name_TagDir0_Incl.c_str());
  
  _TagProbe_Electron0_Excl_1234 = new TagAndProbeFillerElectron(TagDir0_Excl_1234, string(name_TPFiller0_1234.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_123, probe_cuts0_1234, "hard");
  _TagProbe_Electron0_Incl_1234 = new TagAndProbeFillerElectron(TagDir0_Incl_1234, string(name_TPFiller0_1234.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_123, probe_cuts0_1234, "hard");
  
  name_TPFiller0+=_RecoCutFlags[5].c_str();
  string name_TPFiller0_12345 = name_TPFiller0+"_";
  name_TagDir0_Excl+=_RecoCutFlags[5].c_str();  
  TDirectory *TagDir0_Excl_12345 = _dir->mkdir(name_TagDir0_Excl.c_str());
  name_TagDir0_Incl+=_RecoCutFlags[5].c_str();  
  TDirectory *TagDir0_Incl_12345 = _dir->mkdir(name_TagDir0_Incl.c_str());
  
  _TagProbe_Electron0_Excl_12345 = new TagAndProbeFillerElectron(TagDir0_Excl_12345, string(name_TPFiller0_12345.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_1234, probe_cuts0_12345, "hard");
  _TagProbe_Electron0_Incl_12345 = new TagAndProbeFillerElectron(TagDir0_Incl_12345, string(name_TPFiller0_12345.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_1234, probe_cuts0_12345, "hard");
  
  name_TPFiller0+=_RecoCutFlags[6].c_str();
  string name_TPFiller0_123456 = name_TPFiller0+"_";
  name_TagDir0_Excl+=_RecoCutFlags[6].c_str(); 
  TDirectory *TagDir0_Excl_123456 = _dir->mkdir(name_TagDir0_Excl.c_str());
  name_TagDir0_Incl+=_RecoCutFlags[6].c_str(); 
  TDirectory *TagDir0_Incl_123456 = _dir->mkdir(name_TagDir0_Incl.c_str());
  
  _TagProbe_Electron0_Excl_123456 = new TagAndProbeFillerElectron(TagDir0_Excl_123456, string(name_TPFiller0_123456.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_12345, probe_cuts0_123456, "hard");
  _TagProbe_Electron0_Incl_123456 = new TagAndProbeFillerElectron(TagDir0_Incl_123456, string(name_TPFiller0_123456.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_12345, probe_cuts0_123456, "hard");
  
  string name_TPFiller0_Global = "Electron_Global_";
  TDirectory *TagDir0_Excl_Global = _dir->mkdir("Tag&Probe0_Excl_Global");
  TDirectory *TagDir0_Incl_Global = _dir->mkdir("Tag&Probe0_Incl_Global");
  
  _TagProbe_Electron0_Excl_Global = new TagAndProbeFillerElectron(TagDir0_Excl_Global, string(name_TPFiller0_Global.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_123456, "hard");
  _TagProbe_Electron0_Incl_Global = new TagAndProbeFillerElectron(TagDir0_Incl_Global, string(name_TPFiller0_Global.c_str()), _nbin, _xmin, _xmax, tag_cuts_1, probe_cuts0_1, probe_cuts0_123456, "hard");
  
  //Fit del plot di massa di double electron
  string name_TPFiller0_Double = "Electron_Double_";
  TDirectory *TagDir0_Excl_Double = _dir->mkdir("Tag&Probe0_Excl_Double");
  TDirectory *TagDir0_Incl_Double = _dir->mkdir("Tag&Probe0_Incl_Double");
  
  _TagProbe_Electron0_Excl_Double = new TagAndProbeFillerElectron(TagDir0_Excl_Double, string(name_TPFiller0_Double.c_str()), _nbin, _xmin, _xmax, tag_cuts_1_AllSel, probe_cuts0_1, probe_cuts0_123456, "hard");
  _TagProbe_Electron0_Incl_Double = new TagAndProbeFillerElectron(TagDir0_Incl_Double, string(name_TPFiller0_Double.c_str()), _nbin, _xmin, _xmax, tag_cuts_1_AllSel, probe_cuts0_1, probe_cuts0_123456, "hard");
  
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts1_1;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts1_12;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts1_123;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts1_1234;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts1_12345;
  std::vector<bool (*)(const reco::Candidate&, int run)> probe_cuts1_123456;
  
  probe_cuts1_1.clear();
  probe_cuts1_12.clear();
  probe_cuts1_123.clear();
  probe_cuts1_1234.clear();
  probe_cuts1_12345.clear();
  probe_cuts1_123456.clear();
  
  for(unsigned int i = 1; i < 7; i++){
  if(_RecoCutFlags[i] == "_AccASYM"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_Acc_ASYM1);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_Acc_ASYM1);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_Acc_ASYM1);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_Acc_ASYM1);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_Acc_ASYM1);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_Acc_ASYM1);}
  if(_RecoCutFlags[i] == "_Trg"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_Trg_ASYM1);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_Trg_ASYM1);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_Trg_ASYM1);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_Trg_ASYM1);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_Trg_ASYM1);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_Trg_ASYM1);}
  if(_RecoCutFlags[i] == "_Imp"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_Imp);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_Imp);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_Imp);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_Imp);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_Imp);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_Imp);}
  if(_RecoCutFlags[i] == "_ConvASYM"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_Conv_ASYM1);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_Conv_ASYM1);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_Conv_ASYM1);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_Conv_ASYM1);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_Conv_ASYM1);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_Conv_ASYM1);}
  if(_RecoCutFlags[i] == "_IsoASYM"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_Iso_ASYM1);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_Iso_ASYM1);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_Iso_ASYM1);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_Iso_ASYM1);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_Iso_ASYM1);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_Iso_ASYM1);}
  if(_RecoCutFlags[i] == "_EiDASYM"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_EiD_ASYM1);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_EiD_ASYM1);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_EiD_ASYM1);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_EiD_ASYM1);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_EiD_ASYM1);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_EiD_ASYM1);}
  if(_RecoCutFlags[i] == "_1"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_True);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_True);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_True);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_True);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_True);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_True);}
  }
  
  string name_TagDir1_Excl="Tag&Probe1_Excl";
  string name_TagDir1_Incl="Tag&Probe1_Incl";
  string name_TPFiller1="Electron";
  
  name_TPFiller1+=_RecoCutFlags[1].c_str();
  name_TPFiller1+=_RecoCutFlags[2].c_str();
  name_TagDir1_Excl+=_RecoCutFlags[1].c_str();
  name_TagDir1_Excl+=_RecoCutFlags[2].c_str();
  name_TagDir1_Incl+=_RecoCutFlags[1].c_str();
  name_TagDir1_Incl+=_RecoCutFlags[2].c_str();
  
  string name_TPFiller1_12 = name_TPFiller1+"_";
  TDirectory *TagDir1_Excl_12 = _dir->mkdir(name_TagDir1_Excl.c_str());
  TDirectory *TagDir1_Incl_12 = _dir->mkdir(name_TagDir1_Incl.c_str());
  
  _TagProbe_Electron1_Excl_12 = new TagAndProbeFillerElectron(TagDir1_Excl_12, string(name_TPFiller1_12.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_12, "soft");
  _TagProbe_Electron1_Incl_12 = new TagAndProbeFillerElectron(TagDir1_Incl_12, string(name_TPFiller1_12.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_12, "soft");
  
  name_TPFiller1+=_RecoCutFlags[3].c_str();
  string name_TPFiller1_123 = name_TPFiller1+"_";
  name_TagDir1_Excl+=_RecoCutFlags[3].c_str();  
  TDirectory *TagDir1_Excl_123 = _dir->mkdir(name_TagDir1_Excl.c_str());
  name_TagDir1_Incl+=_RecoCutFlags[3].c_str();  
  TDirectory *TagDir1_Incl_123 = _dir->mkdir(name_TagDir1_Incl.c_str());
  
  _TagProbe_Electron1_Excl_123 = new TagAndProbeFillerElectron(TagDir1_Excl_123, string(name_TPFiller1_123.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_12, probe_cuts1_123, "soft");
  _TagProbe_Electron1_Incl_123 = new TagAndProbeFillerElectron(TagDir1_Incl_123, string(name_TPFiller1_123.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_12, probe_cuts1_123, "soft");
  
  name_TPFiller1+=_RecoCutFlags[4].c_str();
  string name_TPFiller1_1234 = name_TPFiller1+"_";
  name_TagDir1_Excl+=_RecoCutFlags[4].c_str();  
  TDirectory *TagDir1_Excl_1234 = _dir->mkdir(name_TagDir1_Excl.c_str());
  name_TagDir1_Incl+=_RecoCutFlags[4].c_str();  
  TDirectory *TagDir1_Incl_1234 = _dir->mkdir(name_TagDir1_Incl.c_str());
  
  _TagProbe_Electron1_Excl_1234 = new TagAndProbeFillerElectron(TagDir1_Excl_1234, string(name_TPFiller1_1234.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_123, probe_cuts1_1234, "soft");
  _TagProbe_Electron1_Incl_1234 = new TagAndProbeFillerElectron(TagDir1_Incl_1234, string(name_TPFiller1_1234.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_123, probe_cuts1_1234, "soft");
  
  name_TPFiller1+=_RecoCutFlags[5].c_str();
  string name_TPFiller1_12345 = name_TPFiller1+"_";
  name_TagDir1_Excl+=_RecoCutFlags[5].c_str();  
  TDirectory *TagDir1_Excl_12345 = _dir->mkdir(name_TagDir1_Excl.c_str());
  name_TagDir1_Incl+=_RecoCutFlags[5].c_str();  
  TDirectory *TagDir1_Incl_12345 = _dir->mkdir(name_TagDir1_Incl.c_str());
  
  _TagProbe_Electron1_Excl_12345 = new TagAndProbeFillerElectron(TagDir1_Excl_12345, string(name_TPFiller1_12345.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_1234, probe_cuts1_12345, "soft");
  _TagProbe_Electron1_Incl_12345 = new TagAndProbeFillerElectron(TagDir1_Incl_12345, string(name_TPFiller1_12345.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_1234, probe_cuts1_12345, "soft");
  
  name_TPFiller1+=_RecoCutFlags[6].c_str();
  string name_TPFiller1_123456 = name_TPFiller1+"_";
  name_TagDir1_Excl+=_RecoCutFlags[6].c_str();  
  TDirectory *TagDir1_Excl_123456 = _dir->mkdir(name_TagDir1_Excl.c_str());
  name_TagDir1_Incl+=_RecoCutFlags[6].c_str();  
  TDirectory *TagDir1_Incl_123456 = _dir->mkdir(name_TagDir1_Incl.c_str());
  
  _TagProbe_Electron1_Excl_123456 = new TagAndProbeFillerElectron(TagDir1_Excl_123456, string(name_TPFiller1_123456.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_12345, probe_cuts1_123456, "soft");
  _TagProbe_Electron1_Incl_123456 = new TagAndProbeFillerElectron(TagDir1_Incl_123456, string(name_TPFiller1_123456.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_12345, probe_cuts1_123456, "soft");
  
  string name_TPFiller1_Global = "Electron_Global_";
  TDirectory *TagDir1_Excl_Global = _dir->mkdir("Tag&Probe1_Excl_Global");
  TDirectory *TagDir1_Incl_Global = _dir->mkdir("Tag&Probe1_Incl_Global");
  
  _TagProbe_Electron1_Excl_Global = new TagAndProbeFillerElectron(TagDir1_Excl_Global, string(name_TPFiller1_Global.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_123456, "soft");
  _TagProbe_Electron1_Incl_Global = new TagAndProbeFillerElectron(TagDir1_Incl_Global, string(name_TPFiller1_Global.c_str()), _nbin, _xmin, _xmax, tag_cuts_0, probe_cuts1_1, probe_cuts1_123456, "soft");
  
  //Fit del plot di massa di double electron
  string name_TPFiller1_Double = "Electron_Double_";
  TDirectory *TagDir1_Excl_Double = _dir->mkdir("Tag&Probe1_Excl_Double");
  TDirectory *TagDir1_Incl_Double = _dir->mkdir("Tag&Probe1_Incl_Double");
  
  _TagProbe_Electron1_Excl_Double = new TagAndProbeFillerElectron(TagDir1_Excl_Double, string(name_TPFiller1_Double.c_str()), _nbin, _xmin, _xmax, tag_cuts_0_AllSel, probe_cuts1_1, probe_cuts1_123456, "soft");
  _TagProbe_Electron1_Incl_Double = new TagAndProbeFillerElectron(TagDir1_Incl_Double, string(name_TPFiller1_Double.c_str()), _nbin, _xmin, _xmax, tag_cuts_0_AllSel, probe_cuts1_1, probe_cuts1_123456, "soft");
  
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
  
  if(_EventNumber==0 && _EventsPerFile==0)_entries = ch->GetEntries();
  
  if(_EventNumber!=0 && _EventsPerFile==0)_entries = _EventNumber;
 
  if(_EventNumber==0 && _EventsPerFile!=0)_entries = fileCounter*_EventsPerFile;
  
  if(_ProcEvents!=-1)_entries = _ProcEvents;
  
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

void  EfficiencyElectron::process(const fwlite::Event& iEvent){

   _file->cd();
   
   _run = iEvent.id().run();
   if(_sample=="mc")_run=-1;

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zrecHandle;
   zrecHandle.getByLabel(iEvent, "zeerec");
   
   fwlite::Handle<std::vector<reco::CompositeCandidate> > zgenHandle;
   zgenHandle.getByLabel(iEvent, "zeegenfull");

   fwlite::Handle<std::vector<pat::Electron> > electronHandle;
   electronHandle.getByLabel(iEvent, "selectedElectrons");
  
   fwlite::Handle<std::vector<pat::Jet> > jetrecHandle;
   if(_JetType=="PF")jetrecHandle.getByLabel(iEvent, "selectedJets");
   if(_JetType=="PFL1CORR")jetrecHandle.getByLabel(iEvent, "selectedJetsL1Corrected");
   
   fwlite::Handle<std::vector<reco::GenJet> > jetgenHandle;
   jetgenHandle.getByLabel(iEvent, "selectedGenJets");

   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");
   
   //Z Gen daughters
   std::vector<const reco::Candidate*> zgendaughters;
   const reco::Candidate *gendau0 = 0;
   const reco::Candidate *gendau1 = 0;
   
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
   if(zgenHandle->size())zgendaughters = ZGENDaughters((*zgenHandle)[0]);
   if(zgendaughters.size()){
   for(unsigned int i = 0; i < recjets.size(); i++){
   if(IsoJet<reco::Candidate>(zgendaughters,*recjets[i]))geniso_recjets.push_back(recjets[i]);}
   }else if(!zgendaughters.size()){
   for(unsigned int i = 0; i < recjets.size(); i++)geniso_recjets.push_back(recjets[i]);}
   }//end if(_sample)
   
   //Z Rec daughters
   if(zrecHandle->size())zrecdaughters = ZRECDaughters((*zrecHandle)[0]);
   
   if(zrecdaughters.size()){   
     
   recdau0 = zrecdaughters[0];
   recdau1 = zrecdaughters[1];
     
   //Pre selection events
     
   }
 
   //Reco Jets isolated from RECO Z electrons
   if(zrecdaughters.size()){  
   for(unsigned int i = 0; i < recjets.size(); i++){     
   if(IsoJet<pat::Electron>(zrecdaughters,*recjets[i]))reciso_recjets.push_back(recjets[i]);}
   }else if(!zrecdaughters.size()){
   for(unsigned int i = 0; i < recjets.size(); i++)reciso_recjets.push_back(recjets[i]);}
 
   if(_sample=="mc" && zgenHandle->size()!=0){
   
   //Z Gen daughters order
   
   double genleadelpt = 0; 
   double genleadeleta = 0; 
   double gensecondelpt = 0;
   double gensecondeleta = 0;
   
     if(zgendaughters.size() != 0){ 
            
      gendau0 = zgendaughters[0];
      gendau1 = zgendaughters[1];
          
      genleadelpt  = gendau0->pt(); 
      gensecondelpt  = gendau1->pt();
      genleadeleta = gendau0->eta();
      gensecondeleta = gendau1->eta();
      
      if(gensecondelpt>genleadelpt)throw cms::Exception("PATAnalysis:EfficiencyElectron_WrongElectronOrder") << "ERROR! Z electrons are in wrong order!";
          
     }
     
     //Generated jets
     std::vector<const reco::GenJet*> genjets = GetJets_GenJets<reco::GenJet>(*jetgenHandle);   
     std::vector<const reco::GenJet*> isogenjets;
     
     if(zgendaughters.size()){
     for(unsigned int i = 0; i < genjets.size(); i++){
     if(IsoJet<reco::Candidate>(zgendaughters,*genjets[i]))isogenjets.push_back(genjets[i]);}
     }else if(!zgendaughters.size()){
     for(unsigned int i = 0; i < genjets.size(); i++)isogenjets.push_back(genjets[i]);}
     
     //Acceptance denominator
     if (GenSelected((*zgenHandle)[0], _selections)&&zgendaughters.size()!=0){
     
     AccDenom_genPtZ->Fill((*zgenHandle)[0].pt());
     AccDenom_genEtaZ->Fill((*zgenHandle)[0].eta());
     AccDenom_genMassZ->Fill((*zgenHandle)[0].mass());
     AccDenom_genLeadElEta->Fill(genleadeleta);
     AccDenom_genLeadElPt->Fill(genleadelpt);
     AccDenom_GenExclJetNumber->Fill(isogenjets.size());          
     AccDenom_RecoExclJetNumber->Fill(geniso_recjets.size());
     for(unsigned int i = 0; (i < isogenjets.size()+1)&&((i+1)<10); i++)AccDenom_GenInclJetNumber->AddBinContent(i+1,1);
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)AccDenom_RecoInclJetNumber->AddBinContent(i+1,1);
     
     }
     
     //Efficiency denominator
     if (GenSelectedInAcceptance((*zgenHandle)[0], _selections)&&zgendaughters.size()!=0){
     
     EffDenom_genPtZ->Fill((*zgenHandle)[0].pt());
     EffDenom_genEtaZ->Fill((*zgenHandle)[0].eta());
     EffDenom_genMassZ->Fill((*zgenHandle)[0].mass());
     EffDenom_genLeadElEta->Fill(genleadeleta);
     EffDenom_genLeadElPt->Fill(genleadelpt);
     EffDenom_GenExclJetNumber->Fill(isogenjets.size());    
     EffDenom_RecoExclJetNumber->Fill(geniso_recjets.size());
     for(unsigned int i = 0; (i < isogenjets.size()+1)&&((i+1)<10); i++)EffDenom_GenInclJetNumber->AddBinContent(i+1,1);
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)EffDenom_RecoInclJetNumber->AddBinContent(i+1,1);
      
     }
  
     if(zrecHandle->size()){
     
     //Events with a selected Zee 1
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){
     
     //Eff vs Z variables
     genMassZEff_1->Fill((*zgenHandle)[0].mass());
     genPtZEff_1->Fill((*zgenHandle)[0].pt());
     genEtaZEff_1->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_1->Fill(genleadeleta);
     genLeadElPtEff_1->Fill(genleadelpt);
     
     //Eff vs Reco Jet variables
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEff_1->AddBinContent(i+1,1);
     RecoExclJetEff_1->Fill(geniso_recjets.size());
     
     }
     
     //Events with a selected Zee 1 With Gen(Acc) Req.
     if (GenSelectedInAcceptance((*zgenHandle)[0], _selections)&&RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){
     
     //Eff vs Reco Jet variables With Gen(Acc)
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEffWGA_1->AddBinContent(i+1,1);
     RecoExclJetEffWGA_1->Fill(geniso_recjets.size());
     
     }
     
     
     
     //Events with a selected Zee 1+2
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){
     
     //Eff vs Z variables
     genMassZEff_12->Fill((*zgenHandle)[0].mass());
     genPtZEff_12->Fill((*zgenHandle)[0].pt());
     genEtaZEff_12->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_12->Fill(genleadeleta);
     genLeadElPtEff_12->Fill(genleadelpt);
     
     //Eff vs Reco Jet variables
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEff_12->AddBinContent(i+1,1);
     RecoExclJetEff_12->Fill(geniso_recjets.size());
        
     }
     
     //Events with a selected Zee 1+2 With Gen(Acc) Req.
     if (GenSelectedInAcceptance((*zgenHandle)[0], _selections)&&RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){
     
     //Eff vs Reco Jet variables With Gen(Acc)
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEffWGA_12->AddBinContent(i+1,1);
     RecoExclJetEffWGA_12->Fill(geniso_recjets.size());
     
     }
      
     
     //Events with a selected Zee 1+2+3
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){
     
     //Eff vs Z variables
     genMassZEff_123->Fill((*zgenHandle)[0].mass());
     genPtZEff_123->Fill((*zgenHandle)[0].pt());
     genEtaZEff_123->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_123->Fill(genleadeleta);
     genLeadElPtEff_123->Fill(genleadelpt);
     
     //Eff vs Reco Jet variables
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEff_123->AddBinContent(i+1,1);
     RecoExclJetEff_123->Fill(geniso_recjets.size());
     
     }
     
     //Events with a selected Zee 1+2+3 With Gen(Acc) Req.
     if (GenSelectedInAcceptance((*zgenHandle)[0], _selections)&&RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){
     
      //Eff vs Reco Jet variables With Gen(Acc)
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEffWGA_123->AddBinContent(i+1,1);
     RecoExclJetEffWGA_123->Fill(geniso_recjets.size());
     
     }
     
     
     //Events with a selected Zee 1+2+3+4
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){
     
     //Eff vs Z variables
     genMassZEff_1234->Fill((*zgenHandle)[0].mass());
     genPtZEff_1234->Fill((*zgenHandle)[0].pt());
     genEtaZEff_1234->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_1234->Fill(genleadeleta);
     genLeadElPtEff_1234->Fill(genleadelpt);
     
     //Eff vs Reco Jet variables
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEff_1234->AddBinContent(i+1,1);
     RecoExclJetEff_1234->Fill(geniso_recjets.size());
     
     }
     
     //Events with a selected Zee 1+2+3+4 With Gen(Acc) Req.
     if (GenSelectedInAcceptance((*zgenHandle)[0], _selections)&&RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){
     
     //Eff vs Reco Jet variables With Gen(Acc)
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEffWGA_1234->AddBinContent(i+1,1);
     RecoExclJetEffWGA_1234->Fill(geniso_recjets.size());
     
     }
     
  
     //Events with a selected Zee 1+2+3+4+5
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[5].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){
      
      //Eff vs Z variables
      genMassZEff_12345->Fill((*zgenHandle)[0].mass());
      genPtZEff_12345->Fill((*zgenHandle)[0].pt());
      genEtaZEff_12345->Fill((*zgenHandle)[0].eta());
      
      //Eff vs Z Electrons variables
      genLeadElEtaEff_12345->Fill(genleadeleta);
      genLeadElPtEff_12345->Fill(genleadelpt);
      
      //Eff vs Reco Jet variables
      for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEff_12345->AddBinContent(i+1,1);
      RecoExclJetEff_12345->Fill(geniso_recjets.size());
      
      }
      
      //Events with a selected Zee 1+2+3+4+5 With Gen(Acc) Req.
     if (GenSelectedInAcceptance((*zgenHandle)[0], _selections)&&RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[5].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){

      //Eff vs Reco Jet variables With Gen(Acc)
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEffWGA_12345->AddBinContent(i+1,1);
     RecoExclJetEffWGA_12345->Fill(geniso_recjets.size());
     
     }
     
     
     //Events with a selected Zee 1+2+3+4+5+6
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[5].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[6].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){
      
      //Eff vs Z variables
      genMassZEff_123456->Fill((*zgenHandle)[0].mass());
      genPtZEff_123456->Fill((*zgenHandle)[0].pt());
      genEtaZEff_123456->Fill((*zgenHandle)[0].eta());
      
      //Eff vs Z Electrons variables
      genLeadElEtaEff_123456->Fill(genleadeleta);
      genLeadElPtEff_123456->Fill(genleadelpt);
      
      //Eff vs Reco Jet variables
      for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEff_123456->AddBinContent(i+1,1);
      RecoExclJetEff_123456->Fill(geniso_recjets.size());
      
      }
      
      //Events with a selected Zee 1+2+3+4+5+6 With Gen(Acc) Req.
     if (GenSelectedInAcceptance((*zgenHandle)[0], _selections)&&RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[5].c_str(), (*zrecHandle)[0], *triggerHandle, _run)&&RecSelected(_RecoCutFlags[6].c_str(), (*zrecHandle)[0], *triggerHandle, _run)){
     
     //Eff vs Reco Jet variables With Gen(Acc)
     for(unsigned int i = 0; (i < geniso_recjets.size()+1)&&((i+1)<10); i++)RecoInclJetEffWGA_123456->AddBinContent(i+1,1);
     RecoExclJetEffWGA_123456->Fill(geniso_recjets.size());
     
     }
     
}
}//end if(_sample)

if(zrecHandle->size()){

// Tag & Probe

if(RecSelected_TagAndProbe((*zrecHandle)[0], _selections, *triggerHandle, _run)){

double lumi = _entries/_xsec;

   if(_Norm && lumi!=0){
   _norm = _targetLumi/lumi;
   }
   
   if(_selections=="SYM"){
   _TagProbe_Electron_Excl_12->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron_Excl_123->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron_Excl_1234->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron_Excl_12345->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron_Excl_123456->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron_Excl_Global->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron_Excl_Double->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   }
   if(_selections=="ASYM"){
   _TagProbe_Electron0_Excl_12->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron0_Excl_123->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron0_Excl_1234->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron0_Excl_12345->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron0_Excl_123456->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron0_Excl_Global->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron0_Excl_Double->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);

   _TagProbe_Electron1_Excl_12->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron1_Excl_123->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron1_Excl_1234->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron1_Excl_12345->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron1_Excl_123456->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron1_Excl_Global->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   _TagProbe_Electron1_Excl_Double->fill((*zrecHandle)[0], _run, reciso_recjets.size(), _norm);
   }
   
   for(unsigned int i = 0; i < reciso_recjets.size()+1; i++){  
   if(_selections=="SYM"){
   _TagProbe_Electron_Incl_12->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron_Incl_123->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron_Incl_1234->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron_Incl_12345->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron_Incl_123456->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron_Incl_Global->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron_Incl_Double->fill((*zrecHandle)[0], _run, i, _norm);
   }
   if(_selections=="ASYM"){
   _TagProbe_Electron0_Incl_12->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron0_Incl_123->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron0_Incl_1234->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron0_Incl_12345->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron0_Incl_123456->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron0_Incl_Global->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron0_Incl_Double->fill((*zrecHandle)[0], _run, i, _norm);

   _TagProbe_Electron1_Incl_12->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron1_Incl_123->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron1_Incl_1234->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron1_Incl_12345->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron1_Incl_123456->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron1_Incl_Global->fill((*zrecHandle)[0], _run, i, _norm);
   _TagProbe_Electron1_Incl_Double->fill((*zrecHandle)[0], _run, i, _norm);
   }
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
  _TagProbe_Electron_Excl_Global->finalize();
  _TagProbe_Electron_Excl_Double->finalize();
  
  _TagProbe_Electron_Incl_12->finalize();
  _TagProbe_Electron_Incl_123->finalize();
  _TagProbe_Electron_Incl_1234->finalize();
  _TagProbe_Electron_Incl_12345->finalize();
  _TagProbe_Electron_Incl_123456->finalize();
  _TagProbe_Electron_Incl_Global->finalize();
  _TagProbe_Electron_Incl_Double->finalize();
  }
  
  if(_selections=="ASYM"){
  _TagProbe_Electron0_Excl_12->finalize();
  _TagProbe_Electron0_Excl_123->finalize();
  _TagProbe_Electron0_Excl_1234->finalize();
  _TagProbe_Electron0_Excl_12345->finalize();
  _TagProbe_Electron0_Excl_123456->finalize();
  _TagProbe_Electron0_Excl_Global->finalize();
  _TagProbe_Electron0_Excl_Double->finalize();
  
  _TagProbe_Electron0_Incl_12->finalize();
  _TagProbe_Electron0_Incl_123->finalize();
  _TagProbe_Electron0_Incl_1234->finalize();
  _TagProbe_Electron0_Incl_12345->finalize();
  _TagProbe_Electron0_Incl_123456->finalize();
  _TagProbe_Electron0_Incl_Global->finalize();
  _TagProbe_Electron0_Incl_Double->finalize();
  
  _TagProbe_Electron1_Excl_12->finalize();
  _TagProbe_Electron1_Excl_123->finalize();
  _TagProbe_Electron1_Excl_1234->finalize();
  _TagProbe_Electron1_Excl_12345->finalize();
  _TagProbe_Electron1_Excl_123456->finalize();
  _TagProbe_Electron1_Excl_Global->finalize();
  _TagProbe_Electron1_Excl_Double->finalize();
  
  _TagProbe_Electron1_Incl_12->finalize();
  _TagProbe_Electron1_Incl_123->finalize();
  _TagProbe_Electron1_Incl_1234->finalize();
  _TagProbe_Electron1_Incl_12345->finalize();
  _TagProbe_Electron1_Incl_123456->finalize();
  _TagProbe_Electron1_Incl_Global->finalize();
  _TagProbe_Electron1_Incl_Double->finalize();
  }
  
  _file->Write();

}

