#include "Firenze/PATAnalysis/include/RecoMuon.h"
#include "Firenze/PATAnalysis/include/MuonUtilities.h"

#include <TH2.h>
#include <TStyle.h>
#include <math.h>
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
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"

#include "TLorentzVector.h"

using namespace std;
using namespace edm;


RecoMuon::RecoMuon():

recZnum(0), BestMassIndex(0),

recPtZ_1(0), recEtaZ_1(0), recMassZ_1(0), 
recPtZ_12(0), recEtaZ_12(0), recMassZ_12(0),
recPtZ_123(0), recEtaZ_123(0), recMassZ_123(0),
recPtZ_1234(0), recEtaZ_1234(0), recMassZ_1234(0),
recPtZ_12345(0), recEtaZ_12345(0), recMassZ_12345(0),
recPtZ_123456(0), recEtaZ_123456(0), recMassZ_123456(0),

recLeadMuPt_1(0), recSecMuPt_1(0), recLeadMuEta_1(0), recSecMuEta_1(0),
recLeadMuPt_12(0), recSecMuPt_12(0), recLeadMuEta_12(0), recSecMuEta_12(0),
recLeadMuPt_123(0), recSecMuPt_123(0), recLeadMuEta_123(0), recSecMuEta_123(0), 
recLeadMuPt_1234(0), recSecMuPt_1234(0), recLeadMuEta_1234(0), recSecMuEta_1234(0),
recLeadMuPt_12345(0), recSecMuPt_12345(0), recLeadMuEta_12345(0), recSecMuEta_12345(0),
recLeadMuPt_123456(0), recSecMuPt_123456(0), recLeadMuEta_123456(0), recSecMuEta_123456(0),

recMuIP_1(0),
recMuIP_12(0),
recMuIP_123(0), 
recMuIP_1234(0), 
recMuIP_12345(0),
recMuIP_123456(0),  

recTrackIsoLead_PreIso(0), recEcalIsoLead_PreIso(0), recHcalIsoLead_PreIso(0),  recTrackIsoSec_PreIso(0), recEcalIsoSec_PreIso(0), recHcalIsoSec_PreIso(0), 

recRelIso_Barrel_1(0), recRelIso_Endcap_1(0),
recRelIso_Barrel_12(0), recRelIso_Endcap_12(0),
recRelIso_Barrel_123(0), recRelIso_Endcap_123(0),
recRelIso_Barrel_1234(0), recRelIso_Endcap_1234(0),
recRelIso_Barrel_12345(0), recRelIso_Endcap_12345(0),
recRelIso_Barrel_123456(0), recRelIso_Endcap_123456(0),

RecoIsoJetPt_123456(0), RecoJetPt_123456(0), recLeadIsoJetPt_123456(0), recLeadIsoJetEta_123456(0), JetCounter_123456(0), 
IsoJetCounter_1(0), IsoJetCounter_12(0), IsoJetCounter_123(0), IsoJetCounter_1234(0), IsoJetCounter_12345(0), IsoJetCounter_123456(0),

MinDeltaR_ZDau(0), AllJetCharge(0), IsoJetCharge(0), NotIsoJetCharge(0), DeltaR_IsoJet(0), DeltaR_NotIsoJet(0), DeltaR_IsoJet_MuType(0),

_targetLumi(50.), _xsec(1.), _norm(1.), _dir(0), _Zdir(0), _Mudir(0), _Jetdir(0), _Norm(false), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1), _fileCounter(0), _Acc(0), _Trg(0), _Qual(0), _Imp(0), _Iso(0), _MuID(0), _JECUnc(0), _selections("SYM"), _JetType("PF"), _sample("data"), _file(0), _histoVector(), _histoVector2D()

{ }

void RecoMuon::begin(TFile* out, const edm::ParameterSet& iConfig){
    _file = out; 
  
   std::string dirname = iConfig.getParameter<std::string>("Name");
   _sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
   _selections = iConfig.getParameter<std::string>("Selections");
   _sample = iConfig.getParameter<std::string>("Sample");
   _targetLumi= iConfig.getParameter<double>("targetLumi");
   _xsec      = iConfig.getParameter<double>("CrossSection");
   _Norm      = iConfig.getParameter<bool>("Norm");
   _EventsPerFile    = iConfig.getParameter<int32_t>("EventsPerFile");
   _EventNumber    = iConfig.getParameter<int32_t>("EventNumber");
   _ProcEvents    = iConfig.getParameter<int32_t>("ProcEvents");
   _JetType = iConfig.getParameter<std::string>("JetType");
   _JECUnc    = iConfig.getParameter<int32_t>("JECUnc");
   _ReportName = iConfig.getParameter<std::string>("ReportName");
   
   //Selections
   _Acc = iConfig.getParameter<int32_t>("Acc");
   _Trg = iConfig.getParameter<int32_t>("Trg");
   _Qual = iConfig.getParameter<int32_t>("Qual");
   _Imp = iConfig.getParameter<int32_t>("Imp");
   _Iso = iConfig.getParameter<int32_t>("Iso");
   _MuID = iConfig.getParameter<int32_t>("MuID"); 
   
   for(int i=0; i<7; i++){
   _RecoCutFlags[i] = "_1";}
   
   if(_selections=="SYM"){
   _RecoCutFlags[_Acc] =  "_AccSYM";
   _RecoCutFlags[_Qual] = "_QualSYM"; 
   _RecoCutFlags[_Iso] =  "_IsoSYM";
   _RecoCutFlags[_MuID] =  "_MuIDSYM";}
   if(_selections=="ASYM"){
   _RecoCutFlags[_Acc] =  "_AccASYM";
   _RecoCutFlags[_Qual] = "_QualASYM"; 
   _RecoCutFlags[_Iso] =  "_IsoASYM";
   _RecoCutFlags[_MuID] =  "_MuIDASYM";}
     
   _RecoCutFlags[_Trg] =  "_Trg";   
   _RecoCutFlags[_Imp] =  "_Imp";
   
   for(int i=0; i<8; i++){
   _TrgList[i] = -1;}
     
   cout << "RecoMuon file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   
   //Event plots
   TDirectory *event_dir = _dir->mkdir("EventPlots");
   event_dir->cd();
   
   recZnum  = new TH1D("recZnum", "Number of reconstructed Z per event", 10, 0, 10);
   _histoVector.push_back(recZnum);
   BestMassIndex = new TH1D("BestMassIndex", "Index of the best mass Z reconstructed (-1=not Z reconstructed)", 11, -1, 10);
   _histoVector.push_back(BestMassIndex);
   
   //Z plots
   
   _Zdir = _dir->mkdir("recZ_Plots");
   _Zdir->cd();
   
   string recPtZ_name = "recPtZ";
   recPtZ_name+=_RecoCutFlags[1].c_str();
   recPtZ_1   = new TH1D(recPtZ_name.c_str(), "Reconstructed Z p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZ_1);
   recPtZ_name+=_RecoCutFlags[2].c_str();
   recPtZ_12   = new TH1D(recPtZ_name.c_str(), "Reconstructed Z p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZ_12);
   recPtZ_name+=_RecoCutFlags[3].c_str();
   recPtZ_123  = new TH1D(recPtZ_name.c_str(), "Reconstructed Z p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZ_123);
   recPtZ_name+=_RecoCutFlags[4].c_str();
   recPtZ_1234  = new TH1D(recPtZ_name.c_str(), "Reconstructed Z p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZ_1234);
   recPtZ_name+=_RecoCutFlags[5].c_str();
   recPtZ_12345  = new TH1D(recPtZ_name.c_str(), "Reconstructed Z p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZ_12345);
   recPtZ_name+=_RecoCutFlags[6].c_str();
   recPtZ_123456  = new TH1D(recPtZ_name.c_str(), "Reconstructed Z p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZ_123456);
   
   string recEtaZ_name = "recEtaZ";
   recEtaZ_name+=_RecoCutFlags[1].c_str();
   recEtaZ_1  = new TH1D(recEtaZ_name.c_str(), "Reconstructed Z #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZ_1);
   recEtaZ_name+=_RecoCutFlags[2].c_str();
   recEtaZ_12  = new TH1D(recEtaZ_name.c_str(), "Reconstructed Z #eta, Selections: Acc+Qual", 100, -10, 10);
   _histoVector.push_back(recEtaZ_12);
   recEtaZ_name+=_RecoCutFlags[3].c_str();
   recEtaZ_123  = new TH1D(recEtaZ_name.c_str(), "Reconstructed Z #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZ_123); 
   recEtaZ_name+=_RecoCutFlags[4].c_str();
   recEtaZ_1234  = new TH1D(recEtaZ_name.c_str(), "Reconstructed Z #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZ_1234); 
   recEtaZ_name+=_RecoCutFlags[5].c_str();
   recEtaZ_12345  = new TH1D(recEtaZ_name.c_str(), "Reconstructed Z #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZ_12345);
   recEtaZ_name+=_RecoCutFlags[6].c_str();
   recEtaZ_123456  = new TH1D(recEtaZ_name.c_str(), "Reconstructed Z #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZ_123456);
   
   string recMassZ_name = "recMassZ";
   recMassZ_name+=_RecoCutFlags[1].c_str();
   recMassZ_1 = new TH1D(recMassZ_name.c_str(), "Reconstructed Z mass", 100, 60, 120);
   _histoVector.push_back(recMassZ_1);
   recMassZ_name+=_RecoCutFlags[2].c_str();
   recMassZ_12 = new TH1D(recMassZ_name.c_str(), "Reconstructed Z mass", 100, 60, 120);
   _histoVector.push_back(recMassZ_12);
   recMassZ_name+=_RecoCutFlags[3].c_str();
   recMassZ_123 = new TH1D(recMassZ_name.c_str(), "Reconstructed Z mass", 100, 60, 120);
   _histoVector.push_back(recMassZ_123);
   recMassZ_name+=_RecoCutFlags[4].c_str();
   recMassZ_1234 = new TH1D(recMassZ_name.c_str(), "Reconstructed Z mass", 100, 60, 120);
   _histoVector.push_back(recMassZ_1234);  
   recMassZ_name+=_RecoCutFlags[5].c_str();
   recMassZ_12345 = new TH1D(recMassZ_name.c_str(), "Reconstructed Z mass", 100, 60, 120);
   _histoVector.push_back(recMassZ_12345);
   recMassZ_name+=_RecoCutFlags[6].c_str();
   recMassZ_123456 = new TH1D(recMassZ_name.c_str(), "Reconstructed Z mass", 100, 60, 120);
   _histoVector.push_back(recMassZ_123456);
   
   //Z Muon variables   
   _Mudir = _dir->mkdir("recZMuons_Plots");
   _Mudir->cd();
   
   string recLeadMuPt_name = "recLeadMuPt";
   recLeadMuPt_name+=_RecoCutFlags[1].c_str();
   recLeadMuPt_1 = new TH1D(recLeadMuPt_name.c_str(), "Reconstructed Leading muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadMuPt_1);
   recLeadMuPt_name+=_RecoCutFlags[2].c_str();
   recLeadMuPt_12 = new TH1D(recLeadMuPt_name.c_str(), "Reconstructed Leading muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadMuPt_12);
   recLeadMuPt_name+=_RecoCutFlags[3].c_str();
   recLeadMuPt_123 = new TH1D(recLeadMuPt_name.c_str(), "Reconstructed Leading muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadMuPt_123);
   recLeadMuPt_name+=_RecoCutFlags[4].c_str();
   recLeadMuPt_1234 = new TH1D(recLeadMuPt_name.c_str(), "Reconstructed Leading muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadMuPt_1234);
   recLeadMuPt_name+=_RecoCutFlags[5].c_str();
   recLeadMuPt_12345 = new TH1D(recLeadMuPt_name.c_str(), "Reconstructed Leading muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadMuPt_12345);
   recLeadMuPt_name+=_RecoCutFlags[6].c_str();
   recLeadMuPt_123456 = new TH1D(recLeadMuPt_name.c_str(), "Reconstructed Leading muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadMuPt_123456);
   
   string recSecMuPt_name = "recSecMuPt";
   recSecMuPt_name+=_RecoCutFlags[1].c_str();
   recSecMuPt_1  = new TH1D(recSecMuPt_name.c_str(), "Reconstructed Second muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecMuPt_1);
   recSecMuPt_name+=_RecoCutFlags[2].c_str();
   recSecMuPt_12  = new TH1D(recSecMuPt_name.c_str(), "Reconstructed Second muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecMuPt_12);
   recSecMuPt_name+=_RecoCutFlags[3].c_str();
   recSecMuPt_123  = new TH1D(recSecMuPt_name.c_str(), "Reconstructed Second muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecMuPt_123);
   recSecMuPt_name+=_RecoCutFlags[4].c_str();
   recSecMuPt_1234  = new TH1D(recSecMuPt_name.c_str(), "Reconstructed Second muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecMuPt_1234);
   recSecMuPt_name+=_RecoCutFlags[5].c_str();
   recSecMuPt_12345  = new TH1D(recSecMuPt_name.c_str(), "Reconstructed Second muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecMuPt_12345);
   recSecMuPt_name+=_RecoCutFlags[6].c_str();
   recSecMuPt_123456  = new TH1D(recSecMuPt_name.c_str(), "Reconstructed Second muon p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecMuPt_123456);
   
   string recLeadMuEta_name = "recLeadMuEta";
   recLeadMuEta_name+=_RecoCutFlags[1].c_str();  
   recLeadMuEta_1 = new TH1D(recLeadMuEta_name.c_str(), "Reconstructed Leading muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadMuEta_1);
   recLeadMuEta_name+=_RecoCutFlags[2].c_str();
   recLeadMuEta_12 = new TH1D(recLeadMuEta_name.c_str(), "Reconstructed Leading muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadMuEta_12);
   recLeadMuEta_name+=_RecoCutFlags[3].c_str();
   recLeadMuEta_123 = new TH1D(recLeadMuEta_name.c_str(), "Reconstructed Leading muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadMuEta_123);
   recLeadMuEta_name+=_RecoCutFlags[4].c_str();
   recLeadMuEta_1234 = new TH1D(recLeadMuEta_name.c_str(), "Reconstructed Leading muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadMuEta_1234);
   recLeadMuEta_name+=_RecoCutFlags[5].c_str();
   recLeadMuEta_12345 = new TH1D(recLeadMuEta_name.c_str(), "Reconstructed Leading muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadMuEta_12345);
   recLeadMuEta_name+=_RecoCutFlags[6].c_str();
   recLeadMuEta_123456 = new TH1D(recLeadMuEta_name.c_str(), "Reconstructed Leading muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadMuEta_123456);
   
   string recSecMuEta_name = "recSecMuEta";
   recSecMuEta_name+=_RecoCutFlags[1].c_str(); 
   recSecMuEta_1  = new TH1D(recSecMuEta_name.c_str(), "Reconstructed Second muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecMuEta_1);
   recSecMuEta_name+=_RecoCutFlags[2].c_str(); 
   recSecMuEta_12  = new TH1D(recSecMuEta_name.c_str(), "Reconstructed Second muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecMuEta_12);
   recSecMuEta_name+=_RecoCutFlags[3].c_str(); 
   recSecMuEta_123  = new TH1D(recSecMuEta_name.c_str(), "Reconstructed Second muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecMuEta_123); 
   recSecMuEta_name+=_RecoCutFlags[4].c_str(); 
   recSecMuEta_1234  = new TH1D(recSecMuEta_name.c_str(), "Reconstructed Second muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecMuEta_1234);
   recSecMuEta_name+=_RecoCutFlags[5].c_str();  
   recSecMuEta_12345  = new TH1D(recSecMuEta_name.c_str(), "Reconstructed Second muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecMuEta_12345);
   recSecMuEta_name+=_RecoCutFlags[6].c_str();  
   recSecMuEta_123456  = new TH1D(recSecMuEta_name.c_str(), "Reconstructed Second muon #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecMuEta_123456);
   
   string recMuIP_name = "recMuIP";
   recMuIP_name+=_RecoCutFlags[1].c_str();  
   recMuIP_1 = new TH1D(recMuIP_name.c_str(), "Reconstructed muon IP", 100, 0, 0.1);
   _histoVector.push_back(recMuIP_1);
   recMuIP_name+=_RecoCutFlags[2].c_str();  
   recMuIP_12 = new TH1D(recMuIP_name.c_str(), "Reconstructed muon IP", 100, 0, 0.1);
   _histoVector.push_back(recMuIP_12);
   recMuIP_name+=_RecoCutFlags[3].c_str();  
   recMuIP_123 = new TH1D(recMuIP_name.c_str(), "Reconstructed muon IP", 100, 0, 0.1);
   _histoVector.push_back(recMuIP_123);
   recMuIP_name+=_RecoCutFlags[4].c_str();  
   recMuIP_1234 = new TH1D(recMuIP_name.c_str(), "Reconstructed muon IP", 100, 0, 0.1);
   _histoVector.push_back(recMuIP_1234);
   recMuIP_name+=_RecoCutFlags[5].c_str();  
   recMuIP_12345 = new TH1D(recMuIP_name.c_str(), "Reconstructed muon IP", 100, 0, 0.1);
   _histoVector.push_back(recMuIP_12345);
   recMuIP_name+=_RecoCutFlags[6].c_str();  
   recMuIP_123456 = new TH1D(recMuIP_name.c_str(), "Reconstructed muon IP", 100, 0, 0.1);
   _histoVector.push_back(recMuIP_123456);
  
   //Muon identification variable
   _Mudir->cd();
  
   
   //Z Muons Isolation variables
   _Mudir->cd();
   
   recTrackIsoLead_PreIso = new TH1D("recTrackIsoLead_PreIso", "Lead Track Isolation SumPt", 1000, 0, 10);
   _histoVector.push_back(recTrackIsoLead_PreIso);
   recEcalIsoLead_PreIso  = new TH1D("recEcalIsoLead_PreIso", "Lead ECAL Isolation SumPt", 1000, 0, 10);
   _histoVector.push_back(recEcalIsoLead_PreIso);
   recHcalIsoLead_PreIso  = new TH1D("recHcalIsoLead_PreIso", "Lead HCAL Isolation SumPt", 1000, 0, 10);
   _histoVector.push_back(recHcalIsoLead_PreIso);
   recTrackIsoSec_PreIso = new TH1D("recTrackIsoSec_PreIso", "Sec Track Isolation SumPt", 1000, 0, 10);
   _histoVector.push_back(recTrackIsoSec_PreIso);
   recEcalIsoSec_PreIso  = new TH1D("recEcalIsoSec_PreIso", "Sec ECAL Isolation SumPt", 1000, 0, 10);
   _histoVector.push_back(recEcalIsoSec_PreIso);
   recHcalIsoSec_PreIso  = new TH1D("recHcalIsoSec_PreIso", "Sec HCAL Isolation SumPt", 1000, 0, 10); 
   _histoVector.push_back(recHcalIsoSec_PreIso);
   
   string recRelIso_Barrel_name = "recRelIso_Barrel";
   recRelIso_Barrel_name+=_RecoCutFlags[1].c_str();
   recRelIso_Barrel_1   = new TH1D(recRelIso_Barrel_name.c_str(), "Comb Relative Isolation - Barrel", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Barrel_1);
   recRelIso_Barrel_name+=_RecoCutFlags[2].c_str();
   recRelIso_Barrel_12   = new TH1D(recRelIso_Barrel_name.c_str(), "Comb Relative Isolation - Barrel", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Barrel_12);
   recRelIso_Barrel_name+=_RecoCutFlags[3].c_str();
   recRelIso_Barrel_123   = new TH1D(recRelIso_Barrel_name.c_str(), "Comb Relative Isolation - Barrel", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Barrel_123);
   recRelIso_Barrel_name+=_RecoCutFlags[4].c_str();
   recRelIso_Barrel_1234   = new TH1D(recRelIso_Barrel_name.c_str(), "Comb Relative Isolation - Barrel", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Barrel_1234);
   recRelIso_Barrel_name+=_RecoCutFlags[5].c_str();
   recRelIso_Barrel_12345   = new TH1D(recRelIso_Barrel_name.c_str(), "Comb Relative Isolation - Barrel", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Barrel_12345);
   recRelIso_Barrel_name+=_RecoCutFlags[6].c_str();
   recRelIso_Barrel_123456   = new TH1D(recRelIso_Barrel_name.c_str(), "Comb Relative Isolation - Barrel", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Barrel_123456);
   
   string recRelIso_Endcap_name = "recRelIso_Endcap";
   recRelIso_Endcap_name+=_RecoCutFlags[1].c_str();
   recRelIso_Endcap_1   = new TH1D(recRelIso_Endcap_name.c_str(), "Comb Relative Isolation - Endcap", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Endcap_1);
   recRelIso_Endcap_name+=_RecoCutFlags[2].c_str();
   recRelIso_Endcap_12   = new TH1D(recRelIso_Endcap_name.c_str(), "Comb Relative Isolation - Endcap", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Endcap_12);
   recRelIso_Endcap_name+=_RecoCutFlags[3].c_str();
   recRelIso_Endcap_123   = new TH1D(recRelIso_Endcap_name.c_str(), "Comb Relative Isolation - Endcap", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Endcap_123);
   recRelIso_Endcap_name+=_RecoCutFlags[4].c_str();
   recRelIso_Endcap_1234   = new TH1D(recRelIso_Endcap_name.c_str(), "Comb Relative Isolation - Endcap", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Endcap_1234);
   recRelIso_Endcap_name+=_RecoCutFlags[5].c_str();
   recRelIso_Endcap_12345   = new TH1D(recRelIso_Endcap_name.c_str(), "Comb Relative Isolation - Endcap", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Endcap_12345);
   recRelIso_Endcap_name+=_RecoCutFlags[6].c_str();
   recRelIso_Endcap_123456   = new TH1D(recRelIso_Endcap_name.c_str(), "Comb Relative Isolation - Endcap", 1000, 0, 10);
   _histoVector.push_back(recRelIso_Endcap_123456);
   
   
   //Jet variables
   _Jetdir = _dir->mkdir("recJet_Plots");
   _Jetdir->cd();
   
   string recLeadIsoJetPt_name = "recLeadIsoJetPt";
   string recLeadIsoJetEta_name = "recLeadIsoJetEta";
   string RecoIsoJetPt_name = "RecoIsoJetPt";
   string RecoJetPt_name = "RecoJetPt";
   string JetCounter_name = "JetCounter";
  
   for(int c=1;c!=7;c++){
   recLeadIsoJetPt_name+=_RecoCutFlags[c].c_str();
   recLeadIsoJetEta_name+=_RecoCutFlags[c].c_str();
   RecoIsoJetPt_name+=_RecoCutFlags[c].c_str();
   RecoJetPt_name+=_RecoCutFlags[c].c_str();
   JetCounter_name+=_RecoCutFlags[c].c_str();}
   
   recLeadIsoJetPt_123456 = new TH1D(recLeadIsoJetPt_name.c_str(), "Reconstructed Leading Iso Jet p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadIsoJetPt_123456);
   recLeadIsoJetEta_123456 = new TH1D(recLeadIsoJetEta_name.c_str(), "Reconstructed Leading Iso Jet #eta", 100, -5, 5); 
   _histoVector.push_back(recLeadIsoJetEta_123456);
   RecoIsoJetPt_123456 = new TH1D(RecoIsoJetPt_name.c_str(), "Reconstructed Iso Jet p_{T}", 250, 0, 250);
   _histoVector.push_back(RecoIsoJetPt_123456);
   RecoJetPt_123456 = new TH1D(RecoJetPt_name.c_str(), "Reconstructed Jet p_{T}", 250, 0, 250);
   _histoVector.push_back(RecoJetPt_123456);
   JetCounter_123456 = new TH1D(JetCounter_name.c_str(), "Number of Reconstructed Jet per event", 10, 0, 10);
   _histoVector.push_back(JetCounter_123456);
   
   string IsoJetCounter_name = "IsoJetCounter";  
   IsoJetCounter_name+=_RecoCutFlags[1].c_str();
   IsoJetCounter_1 = new TH1D(IsoJetCounter_name.c_str(), "Number of Reconstructed Iso Jet per event", 10, 0, 10);
   _histoVector.push_back(IsoJetCounter_1);
   IsoJetCounter_name+=_RecoCutFlags[2].c_str();
   IsoJetCounter_12 = new TH1D(IsoJetCounter_name.c_str(), "Number of Reconstructed Iso Jet per event", 10, 0, 10);
   _histoVector.push_back(IsoJetCounter_12);
   IsoJetCounter_name+=_RecoCutFlags[3].c_str();
   IsoJetCounter_123 = new TH1D(IsoJetCounter_name.c_str(), "Number of Reconstructed Iso Jet per event", 10, 0, 10);
   _histoVector.push_back(IsoJetCounter_123);
   IsoJetCounter_name+=_RecoCutFlags[4].c_str();
   IsoJetCounter_1234 = new TH1D(IsoJetCounter_name.c_str(), "Number of Reconstructed Iso Jet per event", 10, 0, 10);
   _histoVector.push_back(IsoJetCounter_1234);
   IsoJetCounter_name+=_RecoCutFlags[5].c_str();
   IsoJetCounter_12345 = new TH1D(IsoJetCounter_name.c_str(), "Number of Reconstructed Iso Jet per event", 10, 0, 10);
   _histoVector.push_back(IsoJetCounter_12345);
   IsoJetCounter_name+=_RecoCutFlags[6].c_str();
   IsoJetCounter_123456 = new TH1D(IsoJetCounter_name.c_str(), "Number of Reconstructed Iso Jet per event", 10, 0, 10);
   _histoVector.push_back(IsoJetCounter_123456);
   
   //Jet isolation properties
   TDirectory *jetiso_dir = _dir->mkdir("JetIsolation");
   jetiso_dir->cd();
   
   MinDeltaR_ZDau = new TH1D("MinDeltaR_ZDau", "Min Delta R Z Daughters All Jets", 100, 0, 10);
   _histoVector.push_back(MinDeltaR_ZDau);
   AllJetCharge = new TH1D("AllJetCharge", "All Jet Charge", 400, -2, 2);
   _histoVector.push_back(AllJetCharge);
   IsoJetCharge = new TH1D("IsoJetCharge", "Iso Jet Charge", 400, -2, 2);
   _histoVector.push_back(IsoJetCharge);
   NotIsoJetCharge = new TH1D("NotIsoJetCharge", "Not Iso Jet Charge", 400, -2, 2);
   _histoVector.push_back(NotIsoJetCharge);
   DeltaR_IsoJet = new TH1D("DeltaR_IsoJet", "Min Delta R Iso Jet - Z Muons", 100, 0, 10);
   _histoVector.push_back(DeltaR_IsoJet);
   DeltaR_NotIsoJet = new TH1D("DeltaR_NotIsoJet", "Min Delta R Not Iso Jet - Z Muons", 100, 0, 10);
   _histoVector.push_back(DeltaR_NotIsoJet);
   DeltaR_IsoJet_MuType = new TH1D("DeltaR_IsoJet_MuType", "Min Delta R Iso Jet Muon Type - Z Muons", 100, 0, 10);
   _histoVector.push_back(DeltaR_IsoJet_MuType);
   
   //////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
  _fileCounter = 0;
  
  TChain *ch = new TChain("Events");
  ifstream infile;
  infile.open(_sourceFileList.c_str());
  string datafile;
  while(getline (infile, datafile)){
    ch->Add(datafile.c_str());
    _fileCounter++;
  }
  
  if(_EventNumber==0 && _EventsPerFile==0)_entries = ch->GetEntries();
  
  if(_EventNumber!=0 && _EventsPerFile==0)_entries = _EventNumber;
 
  if(_EventNumber==0 && _EventsPerFile!=0)_entries = _fileCounter*_EventsPerFile;
  
  if(_ProcEvents!=-1)_entries = _ProcEvents;
  
  cout << "RecoMuon analyzing nr. file = "<<_fileCounter<<endl;
  cout << "RecoMuon analyzing nr. event = "<<_entries<<endl;
  
  delete ch; 
   
  cout << "RecoMuon Worker built." << endl;   
}

RecoMuon::~RecoMuon(){
  _file->ls();
  
}

void  RecoMuon::process(const fwlite::Event& iEvent)
{
   
   _run = iEvent.id().run();
   if(_sample=="mc")_run=-1;

   double weight = 1.;
   _file->cd();

   fwlite::Handle<std::vector<pat::Muon> > muonHandle;
   muonHandle.getByLabel(iEvent, "selectedMuons");

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zrecHandle;
   zrecHandle.getByLabel(iEvent, "zmumurec");
  
   fwlite::Handle<std::vector<pat::Jet> > jetrecHandle;
   jetrecHandle.getByLabel(iEvent, "selectedJetsL1Corrected");

   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");
   
   fwlite::Handle<double> Rho;
   Rho.getByLabel(iEvent, "kt6PFJets", "rho");
   
   _rho = *Rho;
   
   // Available triggers
   static map<std::string, std::pair<int, int> > TrgVector = muTrigger();
   static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
   int trg = 1;
   for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
   if(isTriggerAvailable(*triggerHandle,TrgVectorIter->first.c_str()))_TrgList[trg]=1;
   trg++;
   }
   
   //Event plots
   
   recZnum->Fill(zrecHandle->size());
   
   int BestMassZNum = -1;
   double DiffMass = 99999.;
   for(unsigned i=0; i!=zrecHandle->size(); i++){
   if(DiffMass>TMath::Abs((*zrecHandle)[i].mass()-91.1876)){
   DiffMass=TMath::Abs((*zrecHandle)[i].mass()-91.1876);
   BestMassZNum=i;}
   }
   BestMassIndex->Fill(BestMassZNum);
   
   // Isolated Jets selection
   std::vector<const pat::Jet*> recjets = GetJets<pat::Jet>(*jetrecHandle);
   std::vector<const pat::Jet*> isorecjets;
   std::vector<const pat::Jet*> notisorecjets;
   
   std::vector<const pat::Muon*> zrecdaughters;
   
   const pat::Muon *dau0 = 0;
   const pat::Muon *dau1 = 0;  
   
    if(zrecHandle->size()){
   
    zrecdaughters = ZRECDaughters(*zrecHandle);
  
     if(zrecdaughters.size()){
     
     dau0 = zrecdaughters[0];
     dau1 = zrecdaughters[1];
     
     if(dau1->pt()>dau0->pt())throw cms::Exception("PATAnalysis:RecoMuon_WrongMuonOrder") << "ERROR! Z muons are in wrong order!";
    
     }
     
     }

   if(zrecdaughters.size()){  
   for(unsigned int i = 0; i < recjets.size(); i++){     
   if(IsoJet<pat::Muon>(zrecdaughters,*recjets[i]))isorecjets.push_back(recjets[i]);
   
   if(!IsoJet<pat::Muon>(zrecdaughters,*recjets[i]))notisorecjets.push_back(recjets[i]);}
   }else if(!zrecdaughters.size()){
   for(unsigned int i = 0; i < recjets.size(); i++)isorecjets.push_back(recjets[i]);}
  
   // Z->ee events (OC) study: selections applied
   
   if(zrecHandle->size()){
     
     //Events with a selected Zee - SELECTIONS: 1
     if (RecSelected(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
   
     //Z variables
     recPtZ_1->Fill((*zrecHandle)[0].pt());
     recEtaZ_1->Fill((*zrecHandle)[0].eta());
     recMassZ_1->Fill((*zrecHandle)[0].mass());
     
     //Z Muons variables
     recLeadMuEta_1->Fill(dau0->eta());
     recSecMuEta_1->Fill(dau1->eta());
     recLeadMuPt_1->Fill(dau0->pt());
     recSecMuPt_1->Fill(dau1->pt());
     recMuIP_1->Fill(dau0->dB());
     recMuIP_1->Fill(dau1->dB());
        
     //Jet variables
     IsoJetCounter_1->Fill(isorecjets.size());
      
     }
   
     //Events with a selected Zee - SELECTIONS: 1+2
     if (RecSelected(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
    
     //Z variables
     recPtZ_12->Fill((*zrecHandle)[0].pt());
     recEtaZ_12->Fill((*zrecHandle)[0].eta());
     recMassZ_12->Fill((*zrecHandle)[0].mass());
     
     //Z Muons variables
     recLeadMuEta_12->Fill(dau0->eta());
     recSecMuEta_12->Fill(dau1->eta());
     recLeadMuPt_12->Fill(dau0->pt());
     recSecMuPt_12->Fill(dau1->pt());
     recMuIP_12->Fill(dau0->dB());
     recMuIP_12->Fill(dau1->dB());
      
     //Jet variables
     IsoJetCounter_12->Fill(isorecjets.size());
     
     }
     
     //Events with a selected Zee - SELECTIONS: 1+2+3
     if (RecSelected(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
    
     //Z variables
     recPtZ_123->Fill((*zrecHandle)[0].pt());
     recEtaZ_123->Fill((*zrecHandle)[0].eta());
     recMassZ_123->Fill((*zrecHandle)[0].mass());
     
     //Z Muons variables
     recLeadMuEta_123->Fill(dau0->eta());
     recSecMuEta_123->Fill(dau1->eta());
     recLeadMuPt_123->Fill(dau0->pt());
     recSecMuPt_123->Fill(dau1->pt());
     recMuIP_123->Fill(dau0->dB());
     recMuIP_123->Fill(dau1->dB());
         
     //Jet variables
     IsoJetCounter_123->Fill(isorecjets.size());
     
     }
    
     //Events with a selected Zee - SELECTIONS: 1+2+3+4
     if (RecSelected(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
     
     //Z variables
     recPtZ_1234->Fill((*zrecHandle)[0].pt());
     recEtaZ_1234->Fill((*zrecHandle)[0].eta());
     recMassZ_1234->Fill((*zrecHandle)[0].mass());
     
     //Z Muons variables
     recLeadMuEta_1234->Fill(dau0->eta());
     recSecMuEta_1234->Fill(dau1->eta());
     recLeadMuPt_1234->Fill(dau0->pt());
     recSecMuPt_1234->Fill(dau1->pt());
     recMuIP_1234->Fill(dau0->dB());
     recMuIP_1234->Fill(dau1->dB());
         
     //Jet variables
     IsoJetCounter_1234->Fill(isorecjets.size());
          
    }
    
    //Events with a selected Zee - SELECTIONS: 1+2+3+4+5
     if (RecSelected(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[5].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){

      //Z variables     
      recPtZ_12345->Fill((*zrecHandle)[0].pt());
      recEtaZ_12345->Fill((*zrecHandle)[0].eta());
      recMassZ_12345->Fill((*zrecHandle)[0].mass());

      //Z Muons variables      
      recLeadMuEta_12345->Fill(dau0->eta());
      recSecMuEta_12345->Fill(dau1->eta());
      recLeadMuPt_12345->Fill(dau0->pt());
      recSecMuPt_12345->Fill(dau1->pt());
      recMuIP_12345->Fill(dau0->dB());
      recMuIP_12345->Fill(dau1->dB());
           
      //Jet variables     
      IsoJetCounter_12345->Fill(isorecjets.size());
      
      }
  
     //Events with a selected Zee - SELECTIONS: 1+2+3+4+5+6
     if (RecSelected(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[5].c_str(), *zrecHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[6].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
     
      //Z variables     
      recPtZ_123456->Fill((*zrecHandle)[0].pt());
      recEtaZ_123456->Fill((*zrecHandle)[0].eta());
      recMassZ_123456->Fill((*zrecHandle)[0].mass());

      //Z Muons variables      
      recLeadMuEta_123456->Fill(dau0->eta());
      recSecMuEta_123456->Fill(dau1->eta());
      recLeadMuPt_123456->Fill(dau0->pt());
      recSecMuPt_123456->Fill(dau1->pt());
      recMuIP_123456->Fill(dau0->dB());
      recMuIP_123456->Fill(dau1->dB());
           
      //Jet variables     
      JetCounter_123456->Fill(recjets.size());
      IsoJetCounter_123456->Fill(isorecjets.size());
    
      for(unsigned int i = 0; i < recjets.size(); i++){
      RecoJetPt_123456->Fill(recjets[i]->pt());}
      
      for(unsigned int i = 0; i < isorecjets.size(); i++){
      RecoIsoJetPt_123456->Fill(isorecjets[i]->pt());}
      
      if (isorecjets.size()){
        recLeadIsoJetPt_123456->Fill(isorecjets[0]->pt());
        recLeadIsoJetEta_123456->Fill(isorecjets[0]->eta());
        }
      
      //Jet properties
     
      //All jets
      if (recjets.size()){      
      for (unsigned int i = 0; i < recjets.size(); ++i){     
          MinDeltaR_ZDau->Fill(MinDeltaRZDau<pat::Muon>(zrecdaughters,*recjets[i]));       
          AllJetCharge->Fill(recjets[i]->jetCharge());                   
      }     
      }  
    
      //Isolated jets    
      if(isorecjets.size()){      
      for (unsigned int i = 0; i < isorecjets.size(); ++i){      
        IsoJetCharge->Fill(isorecjets[i]->jetCharge());  
        DeltaR_IsoJet->Fill(MinDeltaRZDau<pat::Muon>(zrecdaughters,*isorecjets[i]));
      
        //Iso Jets muon-type
        if(isorecjets[i]->jetCharge() < -0.98 || isorecjets[i]->jetCharge() > 0.98){           
          DeltaR_IsoJet_MuType->Fill(MinDeltaRZDau<pat::Muon>(zrecdaughters,*isorecjets[i]));          
          }          
          }     
     }
     
     //Not isolated jets
     if(notisorecjets.size()){     
      for (unsigned int i = 0; i < notisorecjets.size(); ++i){      
          NotIsoJetCharge->Fill(notisorecjets[i]->jetCharge());        
          DeltaR_NotIsoJet->Fill(MinDeltaRZDau<pat::Muon>(zrecdaughters,*notisorecjets[i]));
     }
     }
     
      //Exclusive - Inclusive Histograms
      _Jetdir->cd();
      addHistosVsMulti(isorecjets.size(), "recJetPtIncl", " reconstructed jet p_{T} spectrum", 200, 0, 200, recJetPtVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recJetEtaIncl", " reconstructed jet #eta spectrum", 100, -5., 5., recJetEtaVsInclMulti);
      _Zdir->cd();
      addHistosVsMulti(isorecjets.size(), "recZPtIncl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recZEtaIncl", " reconstructed Z #eta spectrum", 100, -5., 5., recZEtaVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recZPtExcl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsExclMulti);
      addHistosVsMulti(isorecjets.size(), "recZEtaExcl", " reconstructed Z #eta spectrum", 100, -5., 5., recZEtaVsExclMulti);
      _Mudir->cd();
      addHistosVsMulti(isorecjets.size(), "recMu1PtExcl", " reconstructed lead muon p_{T} spectrum", 200, 0., 200., recMu1PtVsExclMulti);
      addHistosVsMulti(isorecjets.size(), "recMu1EtaExcl", " reconstructed lead muon #eta spectrum", 100, -5., 5., recMu1EtaVsExclMulti);
      addHistosVsMulti(isorecjets.size(), "recMu2PtExcl", " reconstructed sec muon p_{T} spectrum", 200, 0., 200., recMu2PtVsExclMulti);
      addHistosVsMulti(isorecjets.size(), "recMu2EtaExcl", " reconstructed sec muon #eta spectrum", 100, -5., 5., recMu2EtaVsExclMulti);     
      addHistosVsMulti(isorecjets.size(), "recMu1PtIncl", " reconstructed lead muon p_{T} spectrum", 200, 0., 200., recMu1PtVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recMu1EtaIncl", " reconstructed lead muon #eta spectrum", 100, -5., 5., recMu1EtaVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recMu2PtIncl", " reconstructed sec muon p_{T} spectrum", 200, 0., 200., recMu2PtVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recMu2EtaIncl", " reconstructed sec muon #eta spectrum", 100, -5., 5., recMu2EtaVsInclMulti);  

      //fill inclusive histograms
      for (unsigned int i = 0; i < isorecjets.size()+1; ++i){
        recZPtVsInclMulti[i]->Fill((*zrecHandle)[0].pt(), weight);
        recZEtaVsInclMulti[i]->Fill((*zrecHandle)[0].eta(), weight);
        recMu1PtVsInclMulti[i]->Fill(dau0->pt(), weight);
        recMu2PtVsInclMulti[i]->Fill(dau1->pt(), weight);
        recMu1EtaVsInclMulti[i]->Fill(dau0->pt(), weight);
        recMu2EtaVsInclMulti[i]->Fill(dau1->pt(), weight);
      }
      
      if (isorecjets.size()){  
        for (unsigned int i = 0; i < isorecjets.size(); ++i){
          recJetPtVsInclMulti[i+1]->Fill(isorecjets[i]->pt(), weight);
          recJetEtaVsInclMulti[i+1]->Fill(isorecjets[i]->eta(), weight);
      }
      }

      //fill exclusive histograms
      recZPtVsExclMulti[isorecjets.size()]->Fill((*zrecHandle)[0].pt(), weight);
      recZEtaVsExclMulti[isorecjets.size()]->Fill((*zrecHandle)[0].eta(), weight);
      recMu1PtVsExclMulti[isorecjets.size()]->Fill(dau0->pt(), weight);
      recMu1EtaVsExclMulti[isorecjets.size()]->Fill(dau0->eta(), weight);
      recMu2PtVsExclMulti[isorecjets.size()]->Fill(dau1->pt(), weight);
      recMu2EtaVsExclMulti[isorecjets.size()]->Fill(dau1->eta(), weight);
   
   }
   
   string IsoFlag, MuIDFlag;
   if(_selections=="SYM"){
   IsoFlag="_IsoSYM";
   MuIDFlag="_MuIDSYM";}
   if(_selections=="ASYM"){
   IsoFlag="_IsoASYM";
   MuIDFlag="_MuIDASYM";}
 
   for(int fcount = 1; fcount<7; fcount++){
   
   if(fcount != 6){
   if(_RecoCutFlags[fcount+1] == IsoFlag.c_str()){   
   bool PreIso = false;
   for(int n = 1; n < fcount+1; n++){
   if(RecSelected(_RecoCutFlags[n].c_str(), *zrecHandle, *triggerHandle, _run, _rho)){
   PreIso = true;
   }else{
   PreIso = false;
   n = fcount+1;
   }
   }
   
   if(PreIso){    
     recTrackIsoLead_PreIso->Fill(dau0->trackIso());
     recEcalIsoLead_PreIso->Fill(dau0->ecalIso());
     recHcalIsoLead_PreIso->Fill(dau0->hcalIso());
     
     recTrackIsoSec_PreIso->Fill(dau1->trackIso());
     recEcalIsoSec_PreIso->Fill(dau1->ecalIso());
     recHcalIsoSec_PreIso->Fill(dau1->hcalIso());  
     }
     
     }
     }

} 

   }
   
}


void RecoMuon::finalize(){
   
   _histoVector.insert(_histoVector.end(), recJetPtVsInclMulti.begin(), recJetPtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recJetEtaVsInclMulti.begin(), recJetEtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recZPtVsInclMulti.begin(), recZPtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recZEtaVsInclMulti.begin(), recZEtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu1PtVsInclMulti.begin(), recMu1PtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu1EtaVsInclMulti.begin(), recMu1EtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu2PtVsInclMulti.begin(), recMu2PtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu2EtaVsInclMulti.begin(), recMu2EtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recZPtVsExclMulti.begin(), recZPtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recZEtaVsExclMulti.begin(), recZEtaVsExclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu1PtVsExclMulti.begin(), recMu1PtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu1EtaVsExclMulti.begin(), recMu1EtaVsExclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu2PtVsExclMulti.begin(), recMu2PtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu2EtaVsExclMulti.begin(), recMu2EtaVsExclMulti.end());
   
   //Normalization
   
   double lumi = _entries/_xsec;

   if(_Norm && lumi!=0){
   _norm = _targetLumi/lumi;
   }

   std::vector<TH1D*>::const_iterator ibeg = _histoVector.begin();
   std::vector<TH1D*>::const_iterator iend = _histoVector.end();
   
   for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
   (*i)->Sumw2();
   }
 
   for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
   if (*i) (*i)->Scale(_norm);
   }
   
   ofstream Report;
   Report.open(_ReportName.c_str());
   Report<<endl<<endl<<"----- Sample Info -----"<<endl<<endl;
   if(_sample=="data")Report<<"Sample is: DATA"<<endl<<endl;
   if(_sample=="mc")Report<<"Sample is: MC"<<endl<<endl; 
   Report<<"Source File = "<<_sourceFileList.c_str()<<endl;
   Report<<"File number = "<<_fileCounter<<endl;
   Report<<"Number of events = "<<_entries<<endl;
   Report<<"Number of events obtained from: ";
   if(_ProcEvents==-1 && _EventNumber==0 && _EventsPerFile==0)Report<<"TChain"<<endl;
   if(_ProcEvents==-1 && _EventNumber!=0 && _EventsPerFile==0)Report<<"EventNumber"<<endl;
   if(_ProcEvents==-1 && _EventNumber==0 && _EventsPerFile!=0)Report<<"EventsPerFile = "<<_EventsPerFile<<" * fileCounter"<<endl;
   if(_ProcEvents==-1 && _EventNumber!=0 && _EventsPerFile!=0)Report<<"ERROR: EventNumber AND EventsPerFile != 0"<<endl;
   if(_ProcEvents!=-1)Report<<"ProcEvents"<<endl; 
   Report<<"Cross section = "<<_xsec<<endl;
   Report<<"Lumi = "<<lumi<<endl;
   if(_Norm && lumi!=0){ 
   Report<<"Sample is normalized to Lumi "<<_targetLumi<<" pb-1"<<endl;
   Report<<"Normalization factor = "<<_norm<<endl;}
   if(!_Norm || lumi==0)Report<<"Sample not normalized"<<endl;
   Report<<endl<<"Selections Type used = "<<_selections.c_str()<<endl;
   if(_selections=="SYM")Report<<"MuID applied = "<<muID_SYM.c_str()<<endl;
   Report<<"Selections applied:"<<endl;
   Report<<_RecoCutFlags[1].c_str()<<endl;
   Report<<_RecoCutFlags[2].c_str()<<endl;
   Report<<_RecoCutFlags[3].c_str()<<endl;
   Report<<_RecoCutFlags[4].c_str()<<endl;
   Report<<_RecoCutFlags[5].c_str()<<endl;
   Report<<_RecoCutFlags[6].c_str()<<endl<<endl;
   Report<<"Jet Type used = "<<_JetType.c_str()<<endl;
   Report<<"JEC Uncertainty = "<<_JECUnc<<" (if 0 = not applied)"<<endl<<endl;
   
   Report<<"Cut values applied:"<<endl<<endl;
   
   Report<<"Available triggers:"<<endl<<endl;
   static map<std::string, std::pair<int, int> > TrgVector = muTrigger();
   static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
   int trg = 1;
   for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
   if(_TrgList[trg]==1)Report<<"MuonTrigger = "<<TrgVectorIter->first.c_str()<<endl;
   trg++;
   }
   Report<<endl;
   
   if(_sample=="data"){
   Report<<"Run range required for each trigger (DATA sample):"<<endl<<endl;
   static map<std::string, std::pair<int, int> > TrgVector = muTrigger();
   static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
   for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
   Report<<"MuonTrigger = "<<TrgVectorIter->first.c_str()<<"		Run range = "<<TrgVectorIter->second.first<<" to "<<TrgVectorIter->second.second<<endl;}
   }
   
   if(muTrgMatchReq==true){
   Report<<"Trigger Match Required"<<endl<<endl;
   }else{
   Report<<"Trigger Match NOT Required"<<endl<<endl;}
   
   if(_selections=="SYM"){
   Report<<"ptmucut = "<<ptmucut<<endl;
   Report<<"etamucut = "<<etamucut<<endl;
   Report<<"eta_mu_excl_up = "<<eta_mu_excl_up<<endl;
   Report<<"eta_mu_excl_down = "<<eta_mu_excl_down<<endl;
   Report<<"zmassmin_sym = "<<zmassmin_sym<<endl;
   Report<<"zmassmax_sym = "<<zmassmax_sym<<endl;
   Report<<"dxycut = "<<dxycut<<endl;
   Report<<"ptjetmin = "<<ptjetmin<<endl;
   Report<<"etajetmax = "<<etajetmax<<endl;
   if(JetIDReq==true){
   Report<<endl<<"Jet ID Required"<<endl;
   }else{
   Report<<endl<<"Jet ID NOT Required"<<endl;}
   Report<<"isojetcut = "<<isojetcut<<endl;
   
   //SYM Quality cuts
   Report<<endl<<"Quality cuts:"<<endl;
   Report<<"maxchi2_SYM = "<<maxchi2_SYM<<endl;
   Report<<"minMuHit_SYM = "<<minMuHit_SYM<<endl;
   Report<<"minMatSta_SYM = "<<minMatSta_SYM<<endl;
   Report<<"minVaPiHitTr_SYM = "<<minVaPiHitTr_SYM<<endl;
   Report<<"minVaTrHit_SYM = "<<minVaTrHit_SYM<<endl;
   Report<<"minVaHit_SYM = "<<minVaHit_SYM<<endl;
   Report<<"minVaPiHitInTr_SYM = "<<minVaPiHitInTr_SYM<<endl;
   Report<<"minMat_SYM = "<<minMat_SYM<<endl;
   Report<<"maxPtRelErr_SYM = "<<maxPtRelErr_SYM<<endl;

   //SYM Mu Isolation (with rho) variables
   Report<<endl<<"Isolation cuts:"<<endl;
   Report<<"cAecalEE_SYM = "<<cAecalEE_SYM<<endl;
   Report<<"cAhcalHE_SYM = "<<cAhcalHE_SYM<<endl;
   Report<<"maxmuEta_SYM = "<<maxmuEta_SYM<<endl;
   Report<<"cAecalEB_SYM = "<<cAecalEB_SYM<<endl;
   Report<<"cAhcalEE_SYM = "<<cAhcalEE_SYM<<endl;
   Report<<"muonIsoRhoCut_SYM = "<<muonIsoRhoCut_SYM<<endl;
   
   //SYM TAG cuts
   Report<<endl<<"TAG cuts:"<<endl;
   Report<<"SYM_TAG_ptmucut = "<<SYM_TAG_ptmucut<<endl;   
   Report<<"SYM_TAG_etamucut = "<<SYM_TAG_etamucut<<endl;
   Report<<"SYM_TAG_eta_mu_excl_up = "<<SYM_TAG_eta_mu_excl_up<<endl;            
   Report<<"SYM_TAG_eta_mu_excl_down = "<<SYM_TAG_eta_mu_excl_down<<endl;          
   Report<<"SYM_TAG_dxycut = "<<SYM_TAG_dxycut<<endl;     
   Report<<"SYM_TAG_maxchi2 = "<<SYM_TAG_maxchi2<<endl;
   Report<<"SYM_TAG_minMuHit = "<<SYM_TAG_minMuHit<<endl;
   Report<<"SYM_TAG_minMatSta = "<<SYM_TAG_minMatSta<<endl;
   Report<<"SYM_TAG_minVaPiHitTr = "<<SYM_TAG_minVaPiHitTr<<endl;
   Report<<"SYM_TAG_minVaTrHit = "<<SYM_TAG_minVaTrHit<<endl;
   Report<<"SYM_TAG_minVaHit = "<<SYM_TAG_minVaHit<<endl;
   Report<<"SYM_TAG_minVaPiHitInTr = "<<SYM_TAG_minVaPiHitInTr<<endl;
   Report<<"SYM_TAG_minMat = "<<SYM_TAG_minMat<<endl;
   Report<<"SYM_TAG_maxPtRelErr = "<<SYM_TAG_maxPtRelErr<<endl;
   Report<<"SYM_TAG_muonIsoRhoCut = "<<SYM_TAG_muonIsoRhoCut<<endl;                        
   Report<<"SYM_TAG_MuID = "<<SYM_TAG_MuID.c_str()<<endl;
   
   }
   
   if(_selections=="ASYM"){
   Report<<"ptmucut0 = "<<ptmucut0<<endl;
   Report<<"ptmucut1 = "<<ptmucut1<<endl;
   Report<<"etamucut = "<<etamucut<<endl;
   Report<<"eta_mu_excl_up = "<<eta_mu_excl_up<<endl;
   Report<<"eta_mu_excl_down = "<<eta_mu_excl_down<<endl;
   Report<<"zmassmin_asym = "<<zmassmin_asym<<endl;
   Report<<"zmassmax_asym = "<<zmassmax_asym<<endl<<endl;
   Report<<"muID_ASYM0 = "<<muID_ASYM0.c_str()<<endl;
   Report<<"muID_ASYM1 = "<<muID_ASYM1.c_str()<<endl<<endl;
   Report<<"dxycut = "<<dxycut<<endl<<endl;
   Report<<"ptjetmin = "<<ptjetmin<<endl;
   Report<<"etajetmax = "<<etajetmax<<endl;
   if(JetIDReq==true){
   Report<<endl<<"Jet ID Required"<<endl;
   }else{
   Report<<endl<<"Jet ID NOT Required"<<endl;}
   Report<<"isojetcut = "<<isojetcut<<endl<<endl;
     
   //ASYM0 Quality cuts
   Report<<endl<<"Quality cuts:"<<endl;
   Report<<"maxchi2_ASYM0 = "<<maxchi2_ASYM0<<endl;
   Report<<"minMuHit_ASYM0 = "<<minMuHit_ASYM0<<endl;
   Report<<"minMatSta_ASYM0 = "<<minMatSta_ASYM0<<endl;
   Report<<"minVaPiHitTr_ASYM0 = "<<minVaPiHitTr_ASYM0<<endl;
   Report<<"minVaTrHit_ASYM0 = "<<minVaTrHit_ASYM0<<endl;
   Report<<"minVaHit_ASYM0 = "<<minVaHit_ASYM0<<endl;
   Report<<"minVaPiHitInTr_ASYM0 = "<<minVaPiHitInTr_ASYM0<<endl;
   Report<<"minMat_ASYM0 = "<<minMat_ASYM0<<endl;
   Report<<"maxPtRelErr_ASYM0 = "<<maxPtRelErr_ASYM0<<endl;

   //ASYM0 Mu Isolation (with rho) variables
   Report<<endl<<"Isolation cuts:"<<endl;
   Report<<"cAecalEE_ASYM0 = "<<cAecalEE_ASYM0<<endl;
   Report<<"cAhcalHE_ASYM0 = "<<cAhcalHE_ASYM0<<endl;
   Report<<"maxmuEta_ASYM0 = "<<maxmuEta_ASYM0<<endl;
   Report<<"cAecalEB_ASYM0 = "<<cAecalEB_ASYM0<<endl;
   Report<<"cAhcalEE_ASYM0 = "<<cAhcalEE_ASYM0<<endl;
   Report<<"muonIsoRhoCut_ASYM0 = "<<muonIsoRhoCut_ASYM0<<endl;
   
   //ASYM1 Quality cuts
   Report<<endl<<"Quality cuts:"<<endl;
   Report<<"maxchi2_ASYM1 = "<<maxchi2_ASYM1<<endl;
   Report<<"minMuHit_ASYM1 = "<<minMuHit_ASYM1<<endl;
   Report<<"minMatSta_ASYM1 = "<<minMatSta_ASYM1<<endl;
   Report<<"minVaPiHitTr_ASYM1 = "<<minVaPiHitTr_ASYM1<<endl;
   Report<<"minVaTrHit_ASYM1 = "<<minVaTrHit_ASYM1<<endl;
   Report<<"minVaHit_ASYM1 = "<<minVaHit_ASYM1<<endl;
   Report<<"minVaPiHitInTr_ASYM1 = "<<minVaPiHitInTr_ASYM1<<endl;
   Report<<"minMat_ASYM1 = "<<minMat_ASYM1<<endl;
   Report<<"maxPtRelErr_ASYM1 = "<<maxPtRelErr_ASYM1<<endl;

   //ASYM1 Mu Isolation (with rho) variables
   Report<<endl<<"Isolation cuts:"<<endl;
   Report<<"cAecalEE_ASYM1 = "<<cAecalEE_ASYM1<<endl;
   Report<<"cAhcalHE_ASYM1 = "<<cAhcalHE_ASYM1<<endl;
   Report<<"maxmuEta_ASYM1 = "<<maxmuEta_ASYM1<<endl;
   Report<<"cAecalEB_ASYM1 = "<<cAecalEB_ASYM1<<endl;
   Report<<"cAhcalEE_ASYM1 = "<<cAhcalEE_ASYM1<<endl;
   Report<<"muonIsoRhoCut_ASYM1 = "<<muonIsoRhoCut_ASYM1<<endl;
   
   //ASYM TAG cuts
   Report<<endl<<"TAG cuts:"<<endl;
   Report<<"ASYM0_TAG_ptmucut = "<<ASYM0_TAG_ptmucut<<endl;   
   Report<<"ASYM0_TAG_etamucut = "<<ASYM0_TAG_etamucut<<endl;
   Report<<"ASYM0_TAG_eta_mu_excl_up = "<<ASYM0_TAG_eta_mu_excl_up<<endl;            
   Report<<"ASYM0_TAG_eta_mu_excl_down = "<<ASYM0_TAG_eta_mu_excl_down<<endl;          
   Report<<"ASYM0_TAG_dxycut = "<<ASYM0_TAG_dxycut<<endl;     
   Report<<"ASYM0_TAG_maxchi2 = "<<ASYM0_TAG_maxchi2<<endl;
   Report<<"ASYM0_TAG_minMuHit = "<<ASYM0_TAG_minMuHit<<endl;
   Report<<"ASYM0_TAG_minMatSta = "<<ASYM0_TAG_minMatSta<<endl;
   Report<<"ASYM0_TAG_minVaPiHitTr = "<<ASYM0_TAG_minVaPiHitTr<<endl;
   Report<<"ASYM0_TAG_minVaTrHit = "<<ASYM0_TAG_minVaTrHit<<endl;
   Report<<"ASYM0_TAG_minVaHit = "<<ASYM0_TAG_minVaHit<<endl;
   Report<<"ASYM0_TAG_minVaPiHitInTr = "<<ASYM0_TAG_minVaPiHitInTr<<endl;
   Report<<"ASYM0_TAG_minMat = "<<ASYM0_TAG_minMat<<endl;
   Report<<"ASYM0_TAG_maxPtRelErr = "<<ASYM0_TAG_maxPtRelErr<<endl;
   Report<<"ASYM0_TAG_muonIsoRhoCut = "<<ASYM0_TAG_muonIsoRhoCut<<endl;                        
   Report<<"ASYM0_TAG_MuID = "<<ASYM0_TAG_MuID.c_str()<<endl<<endl;
   
   Report<<"ASYM1_TAG_ptmucut = "<<ASYM1_TAG_ptmucut<<endl;   
   Report<<"ASYM1_TAG_etamucut = "<<ASYM1_TAG_etamucut<<endl;
   Report<<"ASYM1_TAG_eta_mu_excl_up = "<<ASYM1_TAG_eta_mu_excl_up<<endl;            
   Report<<"ASYM1_TAG_eta_mu_excl_down = "<<ASYM1_TAG_eta_mu_excl_down<<endl;          
   Report<<"ASYM1_TAG_dxycut = "<<ASYM1_TAG_dxycut<<endl;     
   Report<<"ASYM1_TAG_maxchi2 = "<<ASYM1_TAG_maxchi2<<endl;
   Report<<"ASYM1_TAG_minMuHit = "<<ASYM1_TAG_minMuHit<<endl;
   Report<<"ASYM1_TAG_minMatSta = "<<ASYM1_TAG_minMatSta<<endl;
   Report<<"ASYM1_TAG_minVaPiHitTr = "<<ASYM1_TAG_minVaPiHitTr<<endl;
   Report<<"ASYM1_TAG_minVaTrHit = "<<ASYM1_TAG_minVaTrHit<<endl;
   Report<<"ASYM1_TAG_minVaHit = "<<ASYM1_TAG_minVaHit<<endl;
   Report<<"ASYM1_TAG_minVaPiHitInTr = "<<ASYM1_TAG_minVaPiHitInTr<<endl;
   Report<<"ASYM1_TAG_minMat = "<<ASYM1_TAG_minMat<<endl;
   Report<<"ASYM1_TAG_maxPtRelErr = "<<ASYM1_TAG_maxPtRelErr<<endl;
   Report<<"ASYM1_TAG_muonIsoRhoCut = "<<ASYM1_TAG_muonIsoRhoCut<<endl;                        
   Report<<"ASYM1_TAG_MuID = "<<ASYM1_TAG_MuID.c_str()<<endl;
   
   }
   
   Report.close();

  _file->Write();

}
