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

GenIsoJetEff_1(0), GenIsoJetEff_12(0), GenIsoJetEff_123(0), GenIsoJetEff_1234(0), GenIsoJetEff_12345(0), GenIsoJetEff_123456(0),

genLeadElEtaEff_1(0), genLeadElEtaEff_12(0), genLeadElEtaEff_123(0), genLeadElEtaEff_1234(0), genLeadElEtaEff_12345(0), genLeadElEtaEff_123456(0),
genLeadElPtEff_1(0), genLeadElPtEff_12(0), genLeadElPtEff_123(0), genLeadElPtEff_1234(0), genLeadElPtEff_12345(0), genLeadElPtEff_123456(0),

AccDenom_genPtZ(0), AccDenom_genMassZ(0), AccDenom_genEtaZ(0), AccDenom_genLeadElEta(0), AccDenom_genLeadElPt(0), AccDenom_GenIsoJetNumber(0),
EffDenom_genPtZ(0), EffDenom_genMassZ(0), EffDenom_genEtaZ(0), EffDenom_genLeadElEta(0), EffDenom_genLeadElPt(0), EffDenom_GenIsoJetNumber(0),

_sample("mc"), _selections("VBTF"), _dir(0), _electronID("eidRobustLoose"), _file(0), _Acc(1), _Trg(2), _Qual(3), _Imp(4), _Iso(5), _EiD(6), _histoVector(), _nbin(10), _xmin(-0.5), _xmax(9.5), _Norm(false), _norm(1.), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1)

{ }

void EfficiencyElectron::begin(TFile* out, const edm::ParameterSet& iConfig){
   _file = out; 
    
    _sample = iConfig.getParameter<std::string>("Sample");
    std::string dirname = iConfig.getParameter<std::string>("Name");
    std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
    _selections = iConfig.getParameter<std::string>("Selections");
    _electronID = iConfig.getParameter<std::string>("electronID");
    _Norm      = iConfig.getParameter<bool>("Norm");
    _EventsPerFile    = iConfig.getParameter<int32_t>("EventsPerFile");
    _EventNumber    = iConfig.getParameter<int32_t>("EventNumber");
    _ProcEvents    = iConfig.getParameter<int32_t>("ProcEvents");
    _targetLumi= iConfig.getParameter<double>("targetLumi");
    _xsec      = iConfig.getParameter<double>("CrossSection");
    
    //Selections
   _Acc = iConfig.getParameter<int32_t>("Acc");
   _Trg = iConfig.getParameter<int32_t>("Trg");
   _Qual = iConfig.getParameter<int32_t>("Qual");
   _Imp = iConfig.getParameter<int32_t>("Imp");
   _Iso = iConfig.getParameter<int32_t>("Iso");
   _EiD = iConfig.getParameter<int32_t>("EiD");
   
   for(int i=0; i<7; i++){
   _RecoCutFlags[i] = "_1";}
   
   if(_selections=="VPJ"){
   _RecoCutFlags[_Acc] =  "_AccVPJ";
   _RecoCutFlags[_Iso] =  "_IsoVPJ";
   _RecoCutFlags[_EiD] =  "_EiDVPJ";}
   if(_selections=="VBTF"){
   _RecoCutFlags[_Acc] =  "_AccVBTF";
   _RecoCutFlags[_Iso] =  "_IsoVBTF";
   _RecoCutFlags[_EiD] =  "_EiDVBTF";}
     
   _RecoCutFlags[_Trg] =  "_Trg";
   _RecoCutFlags[_Qual] = "_Qual";
   _RecoCutFlags[_Imp] =  "_Imp";
  
    cout << "EfficiencyElectron file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();
   
   //Acceptance denominator
   AccDenom_genMassZ = new TH1D("AccDenom_genMassZ", "Generated Z Mass, No acceptance cut applied", 200, 50, 150);
   _histoVector.push_back(AccDenom_genMassZ);
   AccDenom_genPtZ = new TH1D("AccDenom_genPtZ", "Generated Z pt, No acceptance cut applied",  200, 0, 200);
   _histoVector.push_back(AccDenom_genPtZ);
   AccDenom_genEtaZ = new TH1D("AccDenom_genEtaZ", "Generated Z #eta, No acceptance cut applied",  100, -10, 10);
   _histoVector.push_back(AccDenom_genEtaZ);
   AccDenom_genLeadElEta = new TH1D("AccDenom_genLeadElEta", "Generated Leading electron #eta, No acceptance cut applied", 100, -2.5, 2.5);
   _histoVector.push_back(AccDenom_genLeadElEta);
   AccDenom_genLeadElPt = new TH1D("AccDenom_genLeadElPt", "Generated Leading electron Pt, No acceptance cut applied", 200, 0, 200);
   _histoVector.push_back(AccDenom_genLeadElPt);
   AccDenom_GenIsoJetNumber = new TH1D("AccDenom_GenIsoJetNumber", "Number of Gen Iso Jet, No acceptance cut applied", 10, 0, 10);
   _histoVector.push_back(AccDenom_GenIsoJetNumber);
   
   //Efficiency denominator
   EffDenom_genMassZ = new TH1D("EffDenom_genMassZ", "Generated Z Mass, in acceptance", 200, 50, 150);
   _histoVector.push_back(EffDenom_genMassZ);
   EffDenom_genPtZ = new TH1D("EffDenom_genPtZ", "Generated Z pt, in acceptance",  200, 0, 200);
   _histoVector.push_back(EffDenom_genPtZ);
   EffDenom_genEtaZ = new TH1D("EffDenom_genEtaZ", "Generated Z #eta, in acceptance",  100, -10, 10);
   _histoVector.push_back(EffDenom_genEtaZ);
   EffDenom_genLeadElEta = new TH1D("EffDenom_genLeadElEta", "Generated Leading electron #eta, in acceptance", 100, -2.5, 2.5);
   _histoVector.push_back(EffDenom_genLeadElEta);
   EffDenom_genLeadElPt = new TH1D("EffDenom_genLeadElPt", "Generated Leading electron Pt, in acceptance", 200, 0, 200);
   _histoVector.push_back(EffDenom_genLeadElPt);
   EffDenom_GenIsoJetNumber = new TH1D("EffDenom_GenIsoJetNumber", "Number of Gen Iso Jet, in acceptance", 10, 0, 10);
   _histoVector.push_back(EffDenom_GenIsoJetNumber);
   
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
   genMassZEff_1 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 200, 50, 150);
   _histoVector.push_back(genMassZEff_1);
   genMassZEff_name+=_RecoCutFlags[2].c_str();
   genMassZEff_12 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 200, 50, 150);
   _histoVector.push_back(genMassZEff_12);
   genMassZEff_name+=_RecoCutFlags[3].c_str();
   genMassZEff_123 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 200, 50, 150);
   _histoVector.push_back(genMassZEff_123);
   genMassZEff_name+=_RecoCutFlags[4].c_str();
   genMassZEff_1234 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 200, 50, 150);
   _histoVector.push_back(genMassZEff_1234);
   genMassZEff_name+=_RecoCutFlags[5].c_str();
   genMassZEff_12345 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 200, 50, 150);
   _histoVector.push_back(genMassZEff_12345);
   genMassZEff_name+=_RecoCutFlags[6].c_str();
   genMassZEff_123456 = new TH1D(genMassZEff_name.c_str(), "Generated Z Mass", 200, 50, 150);
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
   
   //Eff vs Jet properties
   string GenIsoJetEff_name = "GenIsoJetEff";
   GenIsoJetEff_name+=_RecoCutFlags[1].c_str();
   GenIsoJetEff_1 = new TH1D(GenIsoJetEff_name.c_str(), "Number of Gen Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(GenIsoJetEff_1);
   GenIsoJetEff_name+=_RecoCutFlags[2].c_str();
   GenIsoJetEff_12 = new TH1D(GenIsoJetEff_name.c_str(), "Number of Gen Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(GenIsoJetEff_12);
   GenIsoJetEff_name+=_RecoCutFlags[3].c_str();
   GenIsoJetEff_123 = new TH1D(GenIsoJetEff_name.c_str(), "Number of Gen Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(GenIsoJetEff_123);
   GenIsoJetEff_name+=_RecoCutFlags[4].c_str();
   GenIsoJetEff_1234 = new TH1D(GenIsoJetEff_name.c_str(), "Number of Gen Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(GenIsoJetEff_1234);
   GenIsoJetEff_name+=_RecoCutFlags[5].c_str();
   GenIsoJetEff_12345 = new TH1D(GenIsoJetEff_name.c_str(), "Number of Gen Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(GenIsoJetEff_12345);
   GenIsoJetEff_name+=_RecoCutFlags[6].c_str();
   GenIsoJetEff_123456 = new TH1D(GenIsoJetEff_name.c_str(), "Number of Gen Iso Jet", 10, -0.5, 9.5);
   _histoVector.push_back(GenIsoJetEff_123456);
   
// Tag & Probe

  //std::vector<bool (*)(const reco::Candidate&)> tag_cuts;
  tag_cuts.push_back(singleEl_Tag);
  
  if(_selections=="VPJ"){
  
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts_1;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts_12;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts_123;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts_1234;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts_12345;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts_123456;
  
  probe_cuts_1.clear();
  probe_cuts_12.clear();
  probe_cuts_123.clear();
  probe_cuts_1234.clear();
  probe_cuts_12345.clear();
  probe_cuts_123456.clear();
  
  for(unsigned int i = 1; i < 6; i++){
  if(_RecoCutFlags[i] == "_AccVPJ"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_Acc_VPJ);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_Acc_VPJ);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_Acc_VPJ);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_Acc_VPJ);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_Acc_VPJ);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_Acc_VPJ);}
  if(_RecoCutFlags[i] == "_Qual"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_Qual);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_Qual);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_Qual);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_Qual);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_Qual);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_Qual);}
  if(_RecoCutFlags[i] == "_Imp"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_Imp);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_Imp);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_Imp);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_Imp);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_Imp);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_Imp);}
  if(_RecoCutFlags[i] == "_IsoVPJ"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_Iso_VPJ);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_Iso_VPJ);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_Iso_VPJ);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_Iso_VPJ);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_Iso_VPJ);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_Iso_VPJ);}
  if(_RecoCutFlags[i] == "_EiDVPJ"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_EiD_VPJ);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_EiD_VPJ);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_EiD_VPJ);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_EiD_VPJ);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_EiD_VPJ);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_EiD_VPJ);}
  if(_RecoCutFlags[i] == "_1"){
  if(i<2)probe_cuts_1.push_back(singleEl_Probe_True);
  if(i<3)probe_cuts_12.push_back(singleEl_Probe_True);
  if(i<4)probe_cuts_123.push_back(singleEl_Probe_True);
  if(i<5)probe_cuts_1234.push_back(singleEl_Probe_True);
  if(i<6)probe_cuts_12345.push_back(singleEl_Probe_True);
  if(i<7)probe_cuts_123456.push_back(singleEl_Probe_True);}
  }
  
  string name_TagDir="Tag&Probe";
  string name_TPFiller="Electron";
  
  name_TagDir+=_RecoCutFlags[1].c_str();
  name_TPFiller+=_RecoCutFlags[1].c_str();
  name_TagDir+=_RecoCutFlags[2].c_str();
  name_TPFiller+=_RecoCutFlags[2].c_str();
  string name_TPFiller_12 = name_TPFiller+"_";
  TDirectory *TagDir_12 = _dir->mkdir(name_TagDir.c_str());
  
  _TagProbe_Electron_12 = new TagAndProbeFiller(TagDir_12, string(name_TPFiller_12.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_1, probe_cuts_12);
  
  name_TagDir+=_RecoCutFlags[3].c_str();
  name_TPFiller+=_RecoCutFlags[3].c_str();
  string name_TPFiller_123 = name_TPFiller+"_";
  TDirectory *TagDir_123 = _dir->mkdir(name_TagDir.c_str());
  
  _TagProbe_Electron_123 = new TagAndProbeFiller(TagDir_123, string(name_TPFiller_123.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_12, probe_cuts_123);
  
  name_TagDir+=_RecoCutFlags[4].c_str();
  name_TPFiller+=_RecoCutFlags[4].c_str();
  string name_TPFiller_1234 = name_TPFiller+"_";
  TDirectory *TagDir_1234 = _dir->mkdir(name_TagDir.c_str());
  
  _TagProbe_Electron_1234 = new TagAndProbeFiller(TagDir_1234, string(name_TPFiller_1234.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_123, probe_cuts_1234);
  
  name_TagDir+=_RecoCutFlags[5].c_str();
  name_TPFiller+=_RecoCutFlags[5].c_str();
  string name_TPFiller_12345 = name_TPFiller+"_";
  TDirectory *TagDir_12345 = _dir->mkdir(name_TagDir.c_str());
  
  _TagProbe_Electron_12345 = new TagAndProbeFiller(TagDir_12345, string(name_TPFiller_12345.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_1234, probe_cuts_12345);
  
  name_TagDir+=_RecoCutFlags[6].c_str();
  name_TPFiller+=_RecoCutFlags[6].c_str();
  string name_TPFiller_123456 = name_TPFiller+"_";
  TDirectory *TagDir_123456 = _dir->mkdir(name_TagDir.c_str());
  
  _TagProbe_Electron_123456 = new TagAndProbeFiller(TagDir_123456, string(name_TPFiller_123456.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts_12345, probe_cuts_123456);
  
  }
  
  if(_selections=="VBTF"){
  
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts0_1;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts0_12;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts0_123;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts0_1234;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts0_12345;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts0_123456;
  
  probe_cuts0_1.clear();
  probe_cuts0_12.clear();
  probe_cuts0_123.clear();
  probe_cuts0_1234.clear();
  probe_cuts0_12345.clear();
  probe_cuts0_123456.clear();
  
  for(unsigned int i = 1; i < 6; i++){
  if(_RecoCutFlags[i] == "_AccVBTF"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_Acc_VBTF0);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_Acc_VBTF0);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_Acc_VBTF0);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_Acc_VBTF0);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_Acc_VBTF0);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_Acc_VBTF0);}
  if(_RecoCutFlags[i] == "_Qual"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_Qual);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_Qual);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_Qual);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_Qual);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_Qual);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_Qual);}
  if(_RecoCutFlags[i] == "_Imp"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_Imp);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_Imp);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_Imp);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_Imp);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_Imp);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_Imp);}
  if(_RecoCutFlags[i] == "_IsoVBTF"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_Iso_VBTF0);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_Iso_VBTF0);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_Iso_VBTF0);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_Iso_VBTF0);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_Iso_VBTF0);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_Iso_VBTF0);}
  if(_RecoCutFlags[i] == "_EiDVBTF"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_EiD_VBTF0);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_EiD_VBTF0);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_EiD_VBTF0);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_EiD_VBTF0);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_EiD_VBTF0);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_EiD_VBTF0);}
  if(_RecoCutFlags[i] == "_1"){
  if(i<2)probe_cuts0_1.push_back(singleEl_Probe_True);
  if(i<3)probe_cuts0_12.push_back(singleEl_Probe_True);
  if(i<4)probe_cuts0_123.push_back(singleEl_Probe_True);
  if(i<5)probe_cuts0_1234.push_back(singleEl_Probe_True);
  if(i<6)probe_cuts0_12345.push_back(singleEl_Probe_True);
  if(i<7)probe_cuts0_123456.push_back(singleEl_Probe_True);}
  }
  
  string name_TagDir0="Tag&Probe0";
  string name_TPFiller0="Electron";
  
  name_TagDir0+=_RecoCutFlags[1].c_str();
  name_TPFiller0+=_RecoCutFlags[1].c_str();
  name_TagDir0+=_RecoCutFlags[2].c_str();
  name_TPFiller0+=_RecoCutFlags[2].c_str();
  string name_TPFiller0_12 = name_TPFiller0+"_";
  TDirectory *TagDir0_12 = _dir->mkdir(name_TagDir0.c_str());
  
  _TagProbe_Electron0_12 = new TagAndProbeFiller(TagDir0_12, string(name_TPFiller0_12.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts0_1, probe_cuts0_12);
  
  name_TagDir0+=_RecoCutFlags[3].c_str();
  name_TPFiller0+=_RecoCutFlags[3].c_str();
  string name_TPFiller0_123 = name_TPFiller0+"_";
  TDirectory *TagDir0_123 = _dir->mkdir(name_TagDir0.c_str());
  
  _TagProbe_Electron0_123 = new TagAndProbeFiller(TagDir0_123, string(name_TPFiller0_123.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts0_12, probe_cuts0_123);
  
  name_TagDir0+=_RecoCutFlags[4].c_str();
  name_TPFiller0+=_RecoCutFlags[4].c_str();
  string name_TPFiller0_1234 = name_TPFiller0+"_";
  TDirectory *TagDir0_1234 = _dir->mkdir(name_TagDir0.c_str());
  
  _TagProbe_Electron0_1234 = new TagAndProbeFiller(TagDir0_1234, string(name_TPFiller0_1234.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts0_123, probe_cuts0_1234);
  
  name_TagDir0+=_RecoCutFlags[5].c_str();
  name_TPFiller0+=_RecoCutFlags[5].c_str();
  string name_TPFiller0_12345 = name_TPFiller0+"_";
  TDirectory *TagDir0_12345 = _dir->mkdir(name_TagDir0.c_str());
  
  _TagProbe_Electron0_12345 = new TagAndProbeFiller(TagDir0_12345, string(name_TPFiller0_12345.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts0_1234, probe_cuts0_12345);
  
  name_TagDir0+=_RecoCutFlags[6].c_str();
  name_TPFiller0+=_RecoCutFlags[6].c_str();
  string name_TPFiller0_123456 = name_TPFiller0+"_";
  TDirectory *TagDir0_123456 = _dir->mkdir(name_TagDir0.c_str());
  
  _TagProbe_Electron0_123456 = new TagAndProbeFiller(TagDir0_123456, string(name_TPFiller0_123456.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts0_12345, probe_cuts0_123456);
  
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts1_1;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts1_12;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts1_123;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts1_1234;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts1_12345;
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts1_123456;
  
  probe_cuts1_1.clear();
  probe_cuts1_12.clear();
  probe_cuts1_123.clear();
  probe_cuts1_1234.clear();
  probe_cuts1_12345.clear();
  probe_cuts1_123456.clear();
  
  for(unsigned int i = 1; i < 6; i++){
  if(_RecoCutFlags[i] == "_AccVBTF"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_Acc_VBTF1);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_Acc_VBTF1);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_Acc_VBTF1);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_Acc_VBTF1);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_Acc_VBTF1);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_Acc_VBTF1);}
  if(_RecoCutFlags[i] == "_Qual"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_Qual);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_Qual);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_Qual);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_Qual);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_Qual);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_Qual);}
  if(_RecoCutFlags[i] == "_Imp"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_Imp);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_Imp);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_Imp);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_Imp);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_Imp);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_Imp);}
  if(_RecoCutFlags[i] == "_IsoVBTF"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_Iso_VBTF1);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_Iso_VBTF1);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_Iso_VBTF1);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_Iso_VBTF1);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_Iso_VBTF1);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_Iso_VBTF1);}
  if(_RecoCutFlags[i] == "_EiDVBTF"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_EiD_VBTF1);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_EiD_VBTF1);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_EiD_VBTF1);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_EiD_VBTF1);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_EiD_VBTF1);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_EiD_VBTF1);}
  if(_RecoCutFlags[i] == "_1"){
  if(i<2)probe_cuts1_1.push_back(singleEl_Probe_True);
  if(i<3)probe_cuts1_12.push_back(singleEl_Probe_True);
  if(i<4)probe_cuts1_123.push_back(singleEl_Probe_True);
  if(i<5)probe_cuts1_1234.push_back(singleEl_Probe_True);
  if(i<6)probe_cuts1_12345.push_back(singleEl_Probe_True);
  if(i<7)probe_cuts1_123456.push_back(singleEl_Probe_True);}
  }
  
  string name_TagDir1="Tag&Probe1";
  string name_TPFiller1="Electron";
  
  name_TagDir1+=_RecoCutFlags[1].c_str();
  name_TPFiller1+=_RecoCutFlags[1].c_str();
  name_TagDir1+=_RecoCutFlags[2].c_str();
  name_TPFiller1+=_RecoCutFlags[2].c_str();
  string name_TPFiller1_12 = name_TPFiller1+"_";
  TDirectory *TagDir1_12 = _dir->mkdir(name_TagDir1.c_str());
  
  _TagProbe_Electron1_12 = new TagAndProbeFiller(TagDir1_12, string(name_TPFiller1_12.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts1_1, probe_cuts1_12);
  
  name_TagDir1+=_RecoCutFlags[3].c_str();
  name_TPFiller1+=_RecoCutFlags[3].c_str();
  string name_TPFiller1_123 = name_TPFiller1+"_";
  TDirectory *TagDir1_123 = _dir->mkdir(name_TagDir1.c_str());
  
  _TagProbe_Electron1_123 = new TagAndProbeFiller(TagDir1_123, string(name_TPFiller1_123.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts1_12, probe_cuts1_123);
  
  name_TagDir1+=_RecoCutFlags[4].c_str();
  name_TPFiller1+=_RecoCutFlags[4].c_str();
  string name_TPFiller1_1234 = name_TPFiller1+"_";
  TDirectory *TagDir1_1234 = _dir->mkdir(name_TagDir1.c_str());
  
  _TagProbe_Electron1_1234 = new TagAndProbeFiller(TagDir1_1234, string(name_TPFiller1_1234.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts1_123, probe_cuts1_1234);
  
  name_TagDir1+=_RecoCutFlags[5].c_str();
  name_TPFiller1+=_RecoCutFlags[5].c_str();
  string name_TPFiller1_12345 = name_TPFiller1+"_";
  TDirectory *TagDir1_12345 = _dir->mkdir(name_TagDir1.c_str());
  
  _TagProbe_Electron1_12345 = new TagAndProbeFiller(TagDir1_12345, string(name_TPFiller1_12345.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts1_1234, probe_cuts1_12345);
  
  name_TagDir1+=_RecoCutFlags[6].c_str();
  name_TPFiller1+=_RecoCutFlags[6].c_str();
  string name_TPFiller1_123456 = name_TPFiller1+"_";
  TDirectory *TagDir1_123456 = _dir->mkdir(name_TagDir1.c_str());
  
  _TagProbe_Electron1_123456 = new TagAndProbeFiller(TagDir1_123456, string(name_TPFiller1_123456.c_str()), _nbin, _xmin, _xmax, tag_cuts, probe_cuts1_12345, probe_cuts1_123456);
  
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
}

void  EfficiencyElectron::process(const fwlite::Event& iEvent){

   _file->cd();

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zrecHandle;
   zrecHandle.getByLabel(iEvent, "zeerec");
   
   fwlite::Handle<std::vector<reco::CompositeCandidate> > zgenHandle;
   zgenHandle.getByLabel(iEvent, "zeegenfull");

   fwlite::Handle<std::vector<pat::Electron> > electronHandle;
   electronHandle.getByLabel(iEvent, "selectedElectrons");
  
   fwlite::Handle<std::vector<pat::Jet> > jetrecHandle;
   jetrecHandle.getByLabel(iEvent, "selectedJets");
   
   fwlite::Handle<std::vector<reco::GenJet> > jetgenHandle;
   jetgenHandle.getByLabel(iEvent, "selectedGenJets");

   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");
   
   //Z Reco daughters
   std::vector<const pat::Electron*> zdaughters;
   const pat::Electron *dau0 = 0; 
   const pat::Electron *dau1 = 0;
   
   //Reco jets
   std::vector<const pat::Jet*> recjets = GetJets<pat::Jet>(*jetrecHandle);
   std::vector<const pat::Jet*> isojets;
   std::vector<const pat::Jet*> notisojets;
   
   if(zrecHandle->size()){
   
   zdaughters = ZDaughters((*zrecHandle)[0]);
      
   for(unsigned int i = 0; i < recjets.size(); i++){
   if(RecoIsoJet((*zrecHandle)[0],*recjets[i]))isojets.push_back(recjets[i]);
   if(!RecoIsoJet((*zrecHandle)[0],*recjets[i]))notisojets.push_back(recjets[i]);
   }
   
   }
   
   //Z Rec daughters
   if(zdaughters.size() != 0){   
     
   dau0 = zdaughters[0];
   dau1 = zdaughters[1];
     
   //Pre selection events
     
     
     
     
     
   }
   
   if(_sample=="mc"){
   
   //Z Gen daughters
   std::vector<const reco::Candidate*> zgendaughters = ZGENDaughters(*zgenHandle);
   const reco::Candidate *gendau0 = 0;
   const reco::Candidate *gendau1 = 0;
   double genleadelpt = 0; 
   double genleadeleta = 0; 
   double gensecondelpt = 0;
   double gensecondeleta = 0;
   
     if(zgendaughters.size() != 0){ 
            
     gendau0 = zgendaughters[0];
     gendau1 = zgendaughters[1];
          
      double pt0  = gendau0->pt(); 
      double pt1  = gendau1->pt();
      double eta0 = gendau0->eta();
      double eta1 = gendau1->eta();
      
      if (pt0 > pt1) {
        genleadelpt    = pt0;
        genleadeleta   = eta0;
        gensecondelpt  = pt1;
        gensecondeleta = eta1;
      } else {
        genleadelpt    = pt1;
        genleadeleta   = eta1;
        gensecondelpt  = pt0;
        gensecondeleta = eta0;
      }
      
     }
     
     //Generated jets
     std::vector<const reco::GenJet*> genjets = GetJets<reco::GenJet>(*jetgenHandle);   
     std::vector<const reco::GenJet*> isogenjets;
      
     for(unsigned int i = 0; i < genjets.size(); i++){
     if(GenIsoJet(*zgenHandle,*genjets[i]))isogenjets.push_back(genjets[i]);
     }
    
     //Acceptance denominator
     if (GenSelected(*zgenHandle, _selections)&&zgendaughters.size()!=0){
     
     AccDenom_genPtZ->Fill((*zgenHandle)[0].pt());
     AccDenom_genEtaZ->Fill((*zgenHandle)[0].eta());
     AccDenom_genMassZ->Fill((*zgenHandle)[0].mass());
     AccDenom_genLeadElEta->Fill(genleadeleta);
     AccDenom_genLeadElPt->Fill(genleadelpt);
     AccDenom_GenIsoJetNumber->Fill(isogenjets.size());
     
     }
     
     //Efficiency denominator
     if (GenSelectedInAcceptance(*zgenHandle, _selections)&&zgendaughters.size()!=0){
     
     EffDenom_genPtZ->Fill((*zgenHandle)[0].pt());
     EffDenom_genEtaZ->Fill((*zgenHandle)[0].eta());
     EffDenom_genMassZ->Fill((*zgenHandle)[0].mass());
     EffDenom_genLeadElEta->Fill(genleadeleta);
     EffDenom_genLeadElPt->Fill(genleadelpt);
     EffDenom_GenIsoJetNumber->Fill(isogenjets.size());
      
     }
     
     if(zrecHandle->size()){
     
     //Events with a selected Zee 1
     if (GenSelectedInAcceptance(*zgenHandle, _selections)&&RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)){
     
     //Eff vs Z variables
     genMassZEff_1->Fill((*zgenHandle)[0].mass());
     genPtZEff_1->Fill((*zgenHandle)[0].pt());
     genEtaZEff_1->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_1->Fill(genleadeleta);
     genLeadElPtEff_1->Fill(genleadelpt);
     
     //Eff vs Jet variables
     GenIsoJetEff_1->Fill(isogenjets.size());
     
     }
     
     //Events with a selected Zee 1+2
     if (GenSelectedInAcceptance(*zgenHandle, _selections)&&RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)){
     
     //Eff vs Z variables
     genMassZEff_12->Fill((*zgenHandle)[0].mass());
     genPtZEff_12->Fill((*zgenHandle)[0].pt());
     genEtaZEff_12->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_12->Fill(genleadeleta);
     genLeadElPtEff_12->Fill(genleadelpt);
     
     //Eff vs Jet variables
     GenIsoJetEff_12->Fill(isogenjets.size());
        
     }
     
     //Events with a selected Zee 1+2+3
     if (GenSelectedInAcceptance(*zgenHandle, _selections)&&RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[3].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)){
     
     //Eff vs Z variables
     genMassZEff_123->Fill((*zgenHandle)[0].mass());
     genPtZEff_123->Fill((*zgenHandle)[0].pt());
     genEtaZEff_123->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_123->Fill(genleadeleta);
     genLeadElPtEff_123->Fill(genleadelpt);
     
     //Eff vs Jet variables
     GenIsoJetEff_123->Fill(isogenjets.size());
     
     
     }
     
     //Events with a selected Zee 1+2+3+4
     if (GenSelectedInAcceptance(*zgenHandle, _selections)&&RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[3].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[4].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)){
     
     //Eff vs Z variables
     genMassZEff_1234->Fill((*zgenHandle)[0].mass());
     genPtZEff_1234->Fill((*zgenHandle)[0].pt());
     genEtaZEff_1234->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_1234->Fill(genleadeleta);
     genLeadElPtEff_1234->Fill(genleadelpt);
     
     //Eff vs Jet variables
     GenIsoJetEff_1234->Fill(isogenjets.size());
     
     }
     
     //Events with a selected Zee 1+2+3+4+5
     if (GenSelectedInAcceptance(*zgenHandle, _selections)&&RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[3].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[4].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[5].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)){
      
      //Eff vs Z variables
      genMassZEff_12345->Fill((*zgenHandle)[0].mass());
      genPtZEff_12345->Fill((*zgenHandle)[0].pt());
      genEtaZEff_12345->Fill((*zgenHandle)[0].eta());
      
      //Eff vs Z Electrons variables
      genLeadElEtaEff_12345->Fill(genleadeleta);
      genLeadElPtEff_12345->Fill(genleadelpt);
      
      //Eff vs Jet variables
      GenIsoJetEff_12345->Fill(isogenjets.size());
      
}

//Events with a selected Zee 1+2+3+4+5+6
     if (GenSelectedInAcceptance(*zgenHandle, _selections)&&RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[3].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[4].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[5].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[6].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)){
      
      //Eff vs Z variables
      genMassZEff_123456->Fill((*zgenHandle)[0].mass());
      genPtZEff_123456->Fill((*zgenHandle)[0].pt());
      genEtaZEff_123456->Fill((*zgenHandle)[0].eta());
      
      //Eff vs Z Electrons variables
      genLeadElEtaEff_123456->Fill(genleadeleta);
      genLeadElPtEff_123456->Fill(genleadelpt);
      
      //Eff vs Jet variables
      GenIsoJetEff_123456->Fill(isogenjets.size());
      
}

}
}//end if(_sample)

if(zrecHandle->size()){

// Tag & Probe

if(RecSelected_TagAndProbe((*zrecHandle)[0], _selections)){

double lumi = _entries/_xsec;

   if(_Norm && lumi!=0){
   _norm = _targetLumi/lumi;
   }

if(_selections=="VPJ"){
_TagProbe_Electron_12->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron_123->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron_1234->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron_12345->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron_123456->fill((*zrecHandle)[0], isojets.size(), _norm);
}

if(_selections=="VBTF"){
_TagProbe_Electron0_12->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron0_123->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron0_1234->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron0_12345->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron0_123456->fill((*zrecHandle)[0], isojets.size(), _norm);

_TagProbe_Electron1_12->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron1_123->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron1_1234->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron1_12345->fill((*zrecHandle)[0], isojets.size(), _norm);
_TagProbe_Electron1_123456->fill((*zrecHandle)[0], isojets.size(), _norm);
}

}
}
 
}

void EfficiencyElectron::finalize(){
  
  if(_selections=="VPJ"){
  _TagProbe_Electron_12->finalize();
  _TagProbe_Electron_123->finalize();
  _TagProbe_Electron_1234->finalize();
  _TagProbe_Electron_12345->finalize();
  _TagProbe_Electron_123456->finalize();
  }
  
  if(_selections=="VBTF"){
  _TagProbe_Electron0_12->finalize();
  _TagProbe_Electron0_123->finalize();
  _TagProbe_Electron0_1234->finalize();
  _TagProbe_Electron0_12345->finalize();
  _TagProbe_Electron0_123456->finalize();
  
  _TagProbe_Electron1_12->finalize();
  _TagProbe_Electron1_123->finalize();
  _TagProbe_Electron1_1234->finalize();
  _TagProbe_Electron1_12345->finalize();
  _TagProbe_Electron1_123456->finalize();
  }
  
  _file->Write();
  
}

