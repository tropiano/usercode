#include "Firenze/PATAnalysis/include/RecoElectron.h"
#include "Firenze/PATAnalysis/include/ElectronUtilities.h"

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
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"

#include "TLorentzVector.h"

using namespace std;
using namespace edm;


RecoElectron::RecoElectron():

recZnum(0), recZSCnum(0), BestMassIndex(0),

recPtZ_1(0), recEtaZ_1(0), recMassZ_1(0), 
recPtZ_12(0), recEtaZ_12(0), recMassZ_12(0),
recPtZ_123(0), recEtaZ_123(0), recMassZ_123(0),
recPtZ_1234(0), recEtaZ_1234(0), recMassZ_1234(0),
recPtZ_12345(0), recEtaZ_12345(0), recMassZ_12345(0),
recPtZ_123456(0), recEtaZ_123456(0), recMassZ_123456(0),

recLeadElPt_1(0), recSecElPt_1(0), recLeadElEta_1(0), recSecElEta_1(0),
recLeadElPt_12(0), recSecElPt_12(0), recLeadElEta_12(0), recSecElEta_12(0),
recLeadElPt_123(0), recSecElPt_123(0), recLeadElEta_123(0), recSecElEta_123(0), 
recLeadElPt_1234(0), recSecElPt_1234(0), recLeadElEta_1234(0), recSecElEta_1234(0),
recLeadElPt_12345(0), recSecElPt_12345(0), recLeadElEta_12345(0), recSecElEta_12345(0),
recLeadElPt_123456(0), recSecElPt_123456(0), recLeadElEta_123456(0), recSecElEta_123456(0),

recElIP_1(0), recLeadElfBrem_1(0), recSecElfBrem_1(0),
recElIP_12(0), recLeadElfBrem_12(0), recSecElfBrem_12(0),
recElIP_123(0), recLeadElfBrem_123(0), recSecElfBrem_123(0), 
recElIP_1234(0), recLeadElfBrem_1234(0), recSecElfBrem_1234(0),
recElIP_12345(0), recLeadElfBrem_12345(0), recSecElfBrem_12345(0),
recElIP_123456(0), recLeadElfBrem_123456(0), recSecElfBrem_123456(0),

recLeadElConvMissHit_1(0), recLeadElConvCotDist_1(0),
recLeadElConvMissHit_12(0), recLeadElConvCotDist_12(0), 
recLeadElConvMissHit_123(0), recLeadElConvCotDist_123(0), 
recLeadElConvMissHit_1234(0), recLeadElConvCotDist_1234(0),
recLeadElConvMissHit_12345(0), recLeadElConvCotDist_12345(0),
recLeadElConvMissHit_123456(0), recLeadElConvCotDist_123456(0), 

recSecElConvMissHit_1(0), recSecElConvCotDist_1(0),
recSecElConvMissHit_12(0), recSecElConvCotDist_12(0), 
recSecElConvMissHit_123(0), recSecElConvCotDist_123(0), 
recSecElConvMissHit_1234(0), recSecElConvCotDist_1234(0), 
recSecElConvMissHit_12345(0), recSecElConvCotDist_12345(0),
recSecElConvMissHit_123456(0), recSecElConvCotDist_123456(0), 

recPtZSC_1(0), recEtaZSC_1(0), recMassZSC_1(0), 
recPtZSC_12(0), recEtaZSC_12(0), recMassZSC_12(0),
recPtZSC_123(0), recEtaZSC_123(0), recMassZSC_123(0),
recPtZSC_1234(0), recEtaZSC_1234(0), recMassZSC_1234(0),
recPtZSC_12345(0), recEtaZSC_12345(0), recMassZSC_12345(0),
recPtZSC_123456(0), recEtaZSC_123456(0), recMassZSC_123456(0),

recLeadElPtSC_1(0), recSecElPtSC_1(0), recLeadElEtaSC_1(0), recSecElEtaSC_1(0),
recLeadElPtSC_12(0), recSecElPtSC_12(0), recLeadElEtaSC_12(0), recSecElEtaSC_12(0),
recLeadElPtSC_123(0), recSecElPtSC_123(0), recLeadElEtaSC_123(0), recSecElEtaSC_123(0), 
recLeadElPtSC_1234(0), recSecElPtSC_1234(0), recLeadElEtaSC_1234(0), recSecElEtaSC_1234(0),
recLeadElPtSC_12345(0), recSecElPtSC_12345(0), recLeadElEtaSC_12345(0), recSecElEtaSC_12345(0),
recLeadElPtSC_123456(0), recSecElPtSC_123456(0), recLeadElEtaSC_123456(0), recSecElEtaSC_123456(0),

recLeadElIPSC_1(0), recSecElIPSC_1(0), recLeadElfBremSC_1(0), recSecElfBremSC_1(0),
recLeadElIPSC_12(0), recSecElIPSC_12(0), recLeadElfBremSC_12(0), recSecElfBremSC_12(0),
recLeadElIPSC_123(0), recSecElIPSC_123(0), recLeadElfBremSC_123(0), recSecElfBremSC_123(0), 
recLeadElIPSC_1234(0), recSecElIPSC_1234(0), recLeadElfBremSC_1234(0), recSecElfBremSC_1234(0),
recLeadElIPSC_12345(0), recSecElIPSC_12345(0), recLeadElfBremSC_12345(0), recSecElfBremSC_12345(0),
recLeadElIPSC_123456(0), recSecElIPSC_123456(0), recLeadElfBremSC_123456(0), recSecElfBremSC_123456(0),

recTrackIsoLead_PreIso(0), recEcalIsoLead_PreIso(0), recHcalIsoLead_PreIso(0),  recTrackIsoSec_PreIso(0), recEcalIsoSec_PreIso(0), recHcalIsoSec_PreIso(0), 

recRelIso_Barrel_PreIso(0), recRelIso_Endcap_PreIso(0),

HoverE_Barrel_PreEiD(0), DeltaEtaIn_Barrel_PreEiD(0), DeltaPhiIn_Barrel_PreEiD(0), SigmaIEtaIEta_Barrel_PreEiD(0), HoverE_Endcap_PreEiD(0), DeltaEtaIn_Endcap_PreEiD(0), DeltaPhiIn_Endcap_PreEiD(0), SigmaIEtaIEta_Endcap_PreEiD(0), 

RecoIsoJetPt_123456(0), RecoJetPt_123456(0), recLeadIsoJetPt_123456(0), recLeadIsoJetEta_123456(0), JetCounter_123456(0), 
IsoJetCounter_1(0), IsoJetCounter_12(0), IsoJetCounter_123(0), IsoJetCounter_1234(0), IsoJetCounter_12345(0), IsoJetCounter_123456(0),

MinDeltaR_ZDau(0), AllJetCharge(0), IsoJetCharge(0), NotIsoJetCharge(0), DeltaR_IsoJet(0), DeltaR_NotIsoJet(0), DeltaR_IsoJet_ElType(0),

ChargeMisID_Pt_Acc(0), ChargeMisID_Eta_Acc(0), ChargeMisID_Hit_Acc(0), ChargeMisID_fBrem_Acc(0), ChargeMisID_IP_Acc(0), ChargeMisID_RecoExclJet_Acc(0), ChargeMisID_RecoInclJet_Acc(0),

CorrectCharge_Pt_Acc(0), CorrectCharge_Eta_Acc(0), CorrectCharge_Hit_Acc(0), CorrectCharge_fBrem_Acc(0), CorrectCharge_IP_Acc(0), CorrectCharge_RecoExclJet_Acc(0), CorrectCharge_RecoInclJet_Acc(0),

AllEl_Pt_Acc(0), AllEl_Eta_Acc(0), AllEl_Hit_Acc(0), AllEl_fBrem_Acc(0), AllEl_IP_Acc(0), AllEl_RecoExclJet_Acc(0), AllEl_RecoInclJet_Acc(0),

PixelHit_OC(0), PixelHit_SC(0), FirstPixelBarrelHit_OC(0), FirstPixelBarrelHit_SC(0),

DeltaRvsCharge_JetRec(0), DeltaRvsCharge_JetRec_Iso(0), DeltaRvsCharge_JetRec_NotIso(0),

_targetLumi(50.), _xsec(1.), _norm(1.), _dir(0), _charge_dir(0), _Zdir(0), _Eldir(0), _Jetdir(0), _Norm(false), _GenParticleMatch(false), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1), _fileCounter(0), _Acc(1), _Trg(2), _Conv(3), _Imp(4), _Iso(5), _EiD(6), _JECUnc(0), _selections("ASYM"), _JetType("PF"), _sample("data"), _file(0), _histoVector(), _histoVector2D()

{ }

void RecoElectron::begin(TFile* out, const edm::ParameterSet& iConfig){
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
   _GenParticleMatch = iConfig.getParameter<bool>("GenParticleMatch");
   _JetType = iConfig.getParameter<std::string>("JetType");
   _JECUnc    = iConfig.getParameter<int32_t>("JECUnc");
   _ReportName = iConfig.getParameter<std::string>("ReportName");
   
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
   
   for(int i=0; i<8; i++){
   _TrgList[i] = -1;}
     
   cout << "RecoElectron file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   
   //Event plots
   TDirectory *event_dir = _dir->mkdir("EventPlots");
   event_dir->cd();
   
   recZnum  = new TH1D("recZnum", "Number of reconstructed Z per event", 10, 0, 10);
   _histoVector.push_back(recZnum);
   recZSCnum = new TH1D("recZSCnum", "Number of reconstructed Z SC per event", 10, 0, 10);
   _histoVector.push_back(recZSCnum);
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
   
   //Z variables - SC
   TDirectory *SC_dir = _dir->mkdir("SameSign");
   SC_dir->cd();
   
   string recPtZSC_name = "recPtZSC";
   recPtZSC_name+=_RecoCutFlags[1].c_str();
   recPtZSC_1   = new TH1D(recPtZSC_name.c_str(), "Reconstructed ZSC p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZSC_1);
   recPtZSC_name+=_RecoCutFlags[2].c_str();
   recPtZSC_12   = new TH1D(recPtZSC_name.c_str(), "Reconstructed ZSC p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZSC_12);
   recPtZSC_name+=_RecoCutFlags[3].c_str();
   recPtZSC_123  = new TH1D(recPtZSC_name.c_str(), "Reconstructed ZSC p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZSC_123);
   recPtZSC_name+=_RecoCutFlags[4].c_str();
   recPtZSC_1234  = new TH1D(recPtZSC_name.c_str(), "Reconstructed ZSC p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZSC_1234);
   recPtZSC_name+=_RecoCutFlags[5].c_str();
   recPtZSC_12345  = new TH1D(recPtZSC_name.c_str(), "Reconstructed ZSC p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZSC_12345);
   recPtZSC_name+=_RecoCutFlags[6].c_str();
   recPtZSC_123456  = new TH1D(recPtZSC_name.c_str(), "Reconstructed ZSC p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZSC_123456);
   
   string recEtaZSC_name = "recEtaZSC";
   recEtaZSC_name+=_RecoCutFlags[1].c_str();
   recEtaZSC_1  = new TH1D(recEtaZSC_name.c_str(), "Reconstructed ZSC #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZSC_1);
   recEtaZSC_name+=_RecoCutFlags[2].c_str();
   recEtaZSC_12  = new TH1D(recEtaZSC_name.c_str(), "Reconstructed ZSC #eta, Selections: Acc+Qual", 100, -10, 10);
   _histoVector.push_back(recEtaZSC_12);
   recEtaZSC_name+=_RecoCutFlags[3].c_str();
   recEtaZSC_123  = new TH1D(recEtaZSC_name.c_str(), "Reconstructed ZSC #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZSC_123); 
   recEtaZSC_name+=_RecoCutFlags[4].c_str();
   recEtaZSC_1234  = new TH1D(recEtaZSC_name.c_str(), "Reconstructed ZSC #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZSC_1234); 
   recEtaZSC_name+=_RecoCutFlags[5].c_str();
   recEtaZSC_12345  = new TH1D(recEtaZSC_name.c_str(), "Reconstructed ZSC #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZSC_12345);
   recEtaZSC_name+=_RecoCutFlags[6].c_str();
   recEtaZSC_123456  = new TH1D(recEtaZSC_name.c_str(), "Reconstructed ZSC #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZSC_123456);
   
   string recMassZSC_name = "recMassZSC";
   recMassZSC_name+=_RecoCutFlags[1].c_str();
   recMassZSC_1 = new TH1D(recMassZSC_name.c_str(), "Reconstructed ZSC mass", 100, 60, 120);
   _histoVector.push_back(recMassZSC_1);
   recMassZSC_name+=_RecoCutFlags[2].c_str();
   recMassZSC_12 = new TH1D(recMassZSC_name.c_str(), "Reconstructed ZSC mass", 100, 60, 120);
   _histoVector.push_back(recMassZSC_12);
   recMassZSC_name+=_RecoCutFlags[3].c_str();
   recMassZSC_123 = new TH1D(recMassZSC_name.c_str(), "Reconstructed ZSC mass", 100, 60, 120);
   _histoVector.push_back(recMassZSC_123);
   recMassZSC_name+=_RecoCutFlags[4].c_str();
   recMassZSC_1234 = new TH1D(recMassZSC_name.c_str(), "Reconstructed ZSC mass", 100, 60, 120);
   _histoVector.push_back(recMassZSC_1234);  
   recMassZSC_name+=_RecoCutFlags[5].c_str();
   recMassZSC_12345 = new TH1D(recMassZSC_name.c_str(), "Reconstructed ZSC mass", 100, 60, 120);
   _histoVector.push_back(recMassZSC_12345);
   recMassZSC_name+=_RecoCutFlags[6].c_str();
   recMassZSC_123456 = new TH1D(recMassZSC_name.c_str(), "Reconstructed ZSC mass", 100, 60, 120);
   _histoVector.push_back(recMassZSC_123456);
   
   //Z Electron variables   
   _Eldir = _dir->mkdir("recZElectrons_Plots");
   _Eldir->cd();
   
   string recLeadElPt_name = "recLeadElPt";
   recLeadElPt_name+=_RecoCutFlags[1].c_str();
   recLeadElPt_1 = new TH1D(recLeadElPt_name.c_str(), "Reconstructed Leading electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadElPt_1);
   recLeadElPt_name+=_RecoCutFlags[2].c_str();
   recLeadElPt_12 = new TH1D(recLeadElPt_name.c_str(), "Reconstructed Leading electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadElPt_12);
   recLeadElPt_name+=_RecoCutFlags[3].c_str();
   recLeadElPt_123 = new TH1D(recLeadElPt_name.c_str(), "Reconstructed Leading electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadElPt_123);
   recLeadElPt_name+=_RecoCutFlags[4].c_str();
   recLeadElPt_1234 = new TH1D(recLeadElPt_name.c_str(), "Reconstructed Leading electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadElPt_1234);
   recLeadElPt_name+=_RecoCutFlags[5].c_str();
   recLeadElPt_12345 = new TH1D(recLeadElPt_name.c_str(), "Reconstructed Leading electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadElPt_12345);
   recLeadElPt_name+=_RecoCutFlags[6].c_str();
   recLeadElPt_123456 = new TH1D(recLeadElPt_name.c_str(), "Reconstructed Leading electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadElPt_123456);
   
   string recSecElPt_name = "recSecElPt";
   recSecElPt_name+=_RecoCutFlags[1].c_str();
   recSecElPt_1  = new TH1D(recSecElPt_name.c_str(), "Reconstructed Second electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecElPt_1);
   recSecElPt_name+=_RecoCutFlags[2].c_str();
   recSecElPt_12  = new TH1D(recSecElPt_name.c_str(), "Reconstructed Second electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecElPt_12);
   recSecElPt_name+=_RecoCutFlags[3].c_str();
   recSecElPt_123  = new TH1D(recSecElPt_name.c_str(), "Reconstructed Second electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecElPt_123);
   recSecElPt_name+=_RecoCutFlags[4].c_str();
   recSecElPt_1234  = new TH1D(recSecElPt_name.c_str(), "Reconstructed Second electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecElPt_1234);
   recSecElPt_name+=_RecoCutFlags[5].c_str();
   recSecElPt_12345  = new TH1D(recSecElPt_name.c_str(), "Reconstructed Second electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecElPt_12345);
   recSecElPt_name+=_RecoCutFlags[6].c_str();
   recSecElPt_123456  = new TH1D(recSecElPt_name.c_str(), "Reconstructed Second electron p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecElPt_123456);
   
   string recLeadElEta_name = "recLeadElEta";
   recLeadElEta_name+=_RecoCutFlags[1].c_str();  
   recLeadElEta_1 = new TH1D(recLeadElEta_name.c_str(), "Reconstructed Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEta_1);
   recLeadElEta_name+=_RecoCutFlags[2].c_str();
   recLeadElEta_12 = new TH1D(recLeadElEta_name.c_str(), "Reconstructed Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEta_12);
   recLeadElEta_name+=_RecoCutFlags[3].c_str();
   recLeadElEta_123 = new TH1D(recLeadElEta_name.c_str(), "Reconstructed Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEta_123);
   recLeadElEta_name+=_RecoCutFlags[4].c_str();
   recLeadElEta_1234 = new TH1D(recLeadElEta_name.c_str(), "Reconstructed Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEta_1234);
   recLeadElEta_name+=_RecoCutFlags[5].c_str();
   recLeadElEta_12345 = new TH1D(recLeadElEta_name.c_str(), "Reconstructed Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEta_12345);
   recLeadElEta_name+=_RecoCutFlags[6].c_str();
   recLeadElEta_123456 = new TH1D(recLeadElEta_name.c_str(), "Reconstructed Leading electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEta_123456);
   
   string recSecElEta_name = "recSecElEta";
   recSecElEta_name+=_RecoCutFlags[1].c_str(); 
   recSecElEta_1  = new TH1D(recSecElEta_name.c_str(), "Reconstructed Second electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEta_1);
   recSecElEta_name+=_RecoCutFlags[2].c_str(); 
   recSecElEta_12  = new TH1D(recSecElEta_name.c_str(), "Reconstructed Second electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEta_12);
   recSecElEta_name+=_RecoCutFlags[3].c_str(); 
   recSecElEta_123  = new TH1D(recSecElEta_name.c_str(), "Reconstructed Second electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEta_123); 
   recSecElEta_name+=_RecoCutFlags[4].c_str(); 
   recSecElEta_1234  = new TH1D(recSecElEta_name.c_str(), "Reconstructed Second electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEta_1234);
   recSecElEta_name+=_RecoCutFlags[5].c_str();  
   recSecElEta_12345  = new TH1D(recSecElEta_name.c_str(), "Reconstructed Second electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEta_12345);
   recSecElEta_name+=_RecoCutFlags[6].c_str();  
   recSecElEta_123456  = new TH1D(recSecElEta_name.c_str(), "Reconstructed Second electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEta_123456);
   
   string recElIP_name = "recElIP";
   recElIP_name+=_RecoCutFlags[1].c_str();  
   recElIP_1 = new TH1D(recElIP_name.c_str(), "Reconstructed electron IP", 100, 0, 0.1);
   _histoVector.push_back(recElIP_1);
   recElIP_name+=_RecoCutFlags[2].c_str();  
   recElIP_12 = new TH1D(recElIP_name.c_str(), "Reconstructed electron IP", 100, 0, 0.1);
   _histoVector.push_back(recElIP_12);
   recElIP_name+=_RecoCutFlags[3].c_str();  
   recElIP_123 = new TH1D(recElIP_name.c_str(), "Reconstructed electron IP", 100, 0, 0.1);
   _histoVector.push_back(recElIP_123);
   recElIP_name+=_RecoCutFlags[4].c_str();  
   recElIP_1234 = new TH1D(recElIP_name.c_str(), "Reconstructed electron IP", 100, 0, 0.1);
   _histoVector.push_back(recElIP_1234);
   recElIP_name+=_RecoCutFlags[5].c_str();  
   recElIP_12345 = new TH1D(recElIP_name.c_str(), "Reconstructed electron IP", 100, 0, 0.1);
   _histoVector.push_back(recElIP_12345);
   recElIP_name+=_RecoCutFlags[6].c_str();  
   recElIP_123456 = new TH1D(recElIP_name.c_str(), "Reconstructed electron IP", 100, 0, 0.1);
   _histoVector.push_back(recElIP_123456);
   
   string recLeadElfBrem_name = "recLeadElfBrem";
   recLeadElfBrem_name+=_RecoCutFlags[1].c_str();  
   recLeadElfBrem_1 = new TH1D(recLeadElfBrem_name.c_str(), "Reconstructed Leading electron fBrem", 100, 0, 2);
   _histoVector.push_back(recLeadElfBrem_1);
   recLeadElfBrem_name+=_RecoCutFlags[2].c_str();  
   recLeadElfBrem_12 = new TH1D(recLeadElfBrem_name.c_str(), "Reconstructed Leading electron fBrem", 100, 0, 2);
   _histoVector.push_back(recLeadElfBrem_12);
   recLeadElfBrem_name+=_RecoCutFlags[3].c_str();  
   recLeadElfBrem_123 = new TH1D(recLeadElfBrem_name.c_str(), "Reconstructed Leading electron fBrem", 100, 0, 2);
   _histoVector.push_back(recLeadElfBrem_123);
   recLeadElfBrem_name+=_RecoCutFlags[4].c_str();  
   recLeadElfBrem_1234 = new TH1D(recLeadElfBrem_name.c_str(), "Reconstructed Leading electron fBrem", 100, 0, 2);
   _histoVector.push_back(recLeadElfBrem_1234);
   recLeadElfBrem_name+=_RecoCutFlags[5].c_str();  
   recLeadElfBrem_12345 = new TH1D(recLeadElfBrem_name.c_str(), "Reconstructed Leading electron fBrem", 100, 0, 2);
   _histoVector.push_back(recLeadElfBrem_12345);
   recLeadElfBrem_name+=_RecoCutFlags[6].c_str();  
   recLeadElfBrem_123456 = new TH1D(recLeadElfBrem_name.c_str(), "Reconstructed Leading electron fBrem", 100, 0, 2);
   _histoVector.push_back(recLeadElfBrem_123456);
   
   string recSecElfBrem_name = "recSecElfBrem";
   recSecElfBrem_name+=_RecoCutFlags[1].c_str();  
   recSecElfBrem_1 = new TH1D(recSecElfBrem_name.c_str(), "Reconstructed Second electron fBrem", 100, 0, 2);
   _histoVector.push_back(recSecElfBrem_1);
   recSecElfBrem_name+=_RecoCutFlags[2].c_str();  
   recSecElfBrem_12 = new TH1D(recSecElfBrem_name.c_str(), "Reconstructed Second electron fBrem", 100, 0, 2);
   _histoVector.push_back(recSecElfBrem_12);
   recSecElfBrem_name+=_RecoCutFlags[3].c_str();  
   recSecElfBrem_123 = new TH1D(recSecElfBrem_name.c_str(), "Reconstructed Second electron fBrem", 100, 0, 2);
   _histoVector.push_back(recSecElfBrem_123);
   recSecElfBrem_name+=_RecoCutFlags[4].c_str();  
   recSecElfBrem_1234 = new TH1D(recSecElfBrem_name.c_str(), "Reconstructed Second electron fBrem", 100, 0, 2);
   _histoVector.push_back(recSecElfBrem_1234);
   recSecElfBrem_name+=_RecoCutFlags[5].c_str();  
   recSecElfBrem_12345 = new TH1D(recSecElfBrem_name.c_str(), "Reconstructed Second electron fBrem", 100, 0, 2);
   _histoVector.push_back(recSecElfBrem_12345);
   recSecElfBrem_name+=_RecoCutFlags[6].c_str();  
   recSecElfBrem_123456 = new TH1D(recSecElfBrem_name.c_str(), "Reconstructed Second electron fBrem", 100, 0, 2);
   _histoVector.push_back(recSecElfBrem_123456);
   
   string recLeadElConvMissHit_name = "recLeadElConvMissHit";
   recLeadElConvMissHit_name+=_RecoCutFlags[1].c_str();
   recLeadElConvMissHit_1 = new TH1D(recLeadElConvMissHit_name.c_str(), "Reconstructed Leading Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recLeadElConvMissHit_1);
   recLeadElConvMissHit_name+=_RecoCutFlags[2].c_str();
   recLeadElConvMissHit_12 = new TH1D(recLeadElConvMissHit_name.c_str(), "Reconstructed Leading Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recLeadElConvMissHit_12);
   recLeadElConvMissHit_name+=_RecoCutFlags[3].c_str();
   recLeadElConvMissHit_123 = new TH1D(recLeadElConvMissHit_name.c_str(), "Reconstructed Leading Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recLeadElConvMissHit_123);
   recLeadElConvMissHit_name+=_RecoCutFlags[4].c_str();
   recLeadElConvMissHit_1234 = new TH1D(recLeadElConvMissHit_name.c_str(), "Reconstructed Leading Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recLeadElConvMissHit_1234);
   recLeadElConvMissHit_name+=_RecoCutFlags[5].c_str();
   recLeadElConvMissHit_12345 = new TH1D(recLeadElConvMissHit_name.c_str(), "Reconstructed Leading Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recLeadElConvMissHit_12345);
   recLeadElConvMissHit_name+=_RecoCutFlags[6].c_str();
   recLeadElConvMissHit_123456 = new TH1D(recLeadElConvMissHit_name.c_str(), "Reconstructed Leading Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recLeadElConvMissHit_123456);
   
   string recSecElConvMissHit_name = "recSecElConvMissHit";
   recSecElConvMissHit_name+=_RecoCutFlags[1].c_str();
   recSecElConvMissHit_1 = new TH1D(recSecElConvMissHit_name.c_str(), "Reconstructed Second Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recSecElConvMissHit_1);
   recSecElConvMissHit_name+=_RecoCutFlags[2].c_str();
   recSecElConvMissHit_12 = new TH1D(recSecElConvMissHit_name.c_str(), "Reconstructed Second Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recSecElConvMissHit_12);
   recSecElConvMissHit_name+=_RecoCutFlags[3].c_str();
   recSecElConvMissHit_123 = new TH1D(recSecElConvMissHit_name.c_str(), "Reconstructed Second Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recSecElConvMissHit_123);
   recSecElConvMissHit_name+=_RecoCutFlags[4].c_str();
   recSecElConvMissHit_1234 = new TH1D(recSecElConvMissHit_name.c_str(), "Reconstructed Second Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recSecElConvMissHit_1234);
   recSecElConvMissHit_name+=_RecoCutFlags[5].c_str();
   recSecElConvMissHit_12345 = new TH1D(recSecElConvMissHit_name.c_str(), "Reconstructed Second Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recSecElConvMissHit_12345);
   recSecElConvMissHit_name+=_RecoCutFlags[6].c_str();
   recSecElConvMissHit_123456 = new TH1D(recSecElConvMissHit_name.c_str(), "Reconstructed Second Electron Missing Hits", 7, 0, 7);
   _histoVector.push_back(recSecElConvMissHit_123456);
   
   string recLeadElConvCotDist_name = "recLeadElConvCotDist";
   recLeadElConvCotDist_name+=_RecoCutFlags[1].c_str();
   recLeadElConvCotDist_1 = new TH1D(recLeadElConvCotDist_name.c_str(), "Reco Leading Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recLeadElConvCotDist_1);
   recLeadElConvCotDist_name+=_RecoCutFlags[2].c_str();
   recLeadElConvCotDist_12 = new TH1D(recLeadElConvCotDist_name.c_str(), "Reco Leading Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recLeadElConvCotDist_12);
   recLeadElConvCotDist_name+=_RecoCutFlags[3].c_str();
   recLeadElConvCotDist_123 = new TH1D(recLeadElConvCotDist_name.c_str(), "Reco Leading Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recLeadElConvCotDist_123);
   recLeadElConvCotDist_name+=_RecoCutFlags[4].c_str();
   recLeadElConvCotDist_1234 = new TH1D(recLeadElConvCotDist_name.c_str(), "Reco Leading Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recLeadElConvCotDist_1234);
   recLeadElConvCotDist_name+=_RecoCutFlags[5].c_str();
   recLeadElConvCotDist_12345 = new TH1D(recLeadElConvCotDist_name.c_str(), "Reco Leading Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recLeadElConvCotDist_12345);
   recLeadElConvCotDist_name+=_RecoCutFlags[6].c_str();
   recLeadElConvCotDist_123456 = new TH1D(recLeadElConvCotDist_name.c_str(), "Reco Leading Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recLeadElConvCotDist_123456);
   
   string recSecElConvCotDist_name = "recSecElConvCotDist";
   recSecElConvCotDist_name+=_RecoCutFlags[1].c_str();
   recSecElConvCotDist_1 = new TH1D(recSecElConvCotDist_name.c_str(), "Reco Second Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recSecElConvCotDist_1);
   recSecElConvCotDist_name+=_RecoCutFlags[2].c_str();
   recSecElConvCotDist_12 = new TH1D(recSecElConvCotDist_name.c_str(), "Reco Second Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recSecElConvCotDist_12);
   recSecElConvCotDist_name+=_RecoCutFlags[3].c_str();
   recSecElConvCotDist_123 = new TH1D(recSecElConvCotDist_name.c_str(), "Reco Second Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recSecElConvCotDist_123);
   recSecElConvCotDist_name+=_RecoCutFlags[4].c_str();
   recSecElConvCotDist_1234 = new TH1D(recSecElConvCotDist_name.c_str(), "Reco Second Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recSecElConvCotDist_1234);
   recSecElConvCotDist_name+=_RecoCutFlags[5].c_str();
   recSecElConvCotDist_12345 = new TH1D(recSecElConvCotDist_name.c_str(), "Reco Second Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recSecElConvCotDist_12345);
   recSecElConvCotDist_name+=_RecoCutFlags[6].c_str();
   recSecElConvCotDist_123456 = new TH1D(recSecElConvCotDist_name.c_str(), "Reco Second Electron Conv Var - sqrt(Cot2+Dist2)", 100, 0., 0.2);
   _histoVector.push_back(recSecElConvCotDist_123456);
   
   //Z Electron variables - SC   
   SC_dir->cd();
   
   string recLeadElPtSC_name = "recLeadElPtSC";
   recLeadElPtSC_name+=_RecoCutFlags[1].c_str();
   recLeadElPtSC_1 = new TH1D(recLeadElPtSC_name.c_str(), "Reconstructed Leading electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recLeadElPtSC_1);
   recLeadElPtSC_name+=_RecoCutFlags[2].c_str();
   recLeadElPtSC_12 = new TH1D(recLeadElPtSC_name.c_str(), "Reconstructed Leading electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recLeadElPtSC_12);
   recLeadElPtSC_name+=_RecoCutFlags[3].c_str();
   recLeadElPtSC_123 = new TH1D(recLeadElPtSC_name.c_str(), "Reconstructed Leading electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recLeadElPtSC_123);
   recLeadElPtSC_name+=_RecoCutFlags[4].c_str();
   recLeadElPtSC_1234 = new TH1D(recLeadElPtSC_name.c_str(), "Reconstructed Leading electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recLeadElPtSC_1234);
   recLeadElPtSC_name+=_RecoCutFlags[5].c_str();
   recLeadElPtSC_12345 = new TH1D(recLeadElPtSC_name.c_str(), "Reconstructed Leading electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recLeadElPtSC_12345);
   recLeadElPtSC_name+=_RecoCutFlags[6].c_str();
   recLeadElPtSC_123456 = new TH1D(recLeadElPtSC_name.c_str(), "Reconstructed Leading electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recLeadElPtSC_123456);
   
   string recSecElPtSC_name = "recSecElPtSC";
   recSecElPtSC_name+=_RecoCutFlags[1].c_str();
   recSecElPtSC_1  = new TH1D(recSecElPtSC_name.c_str(), "Reconstructed Second electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recSecElPtSC_1);
   recSecElPtSC_name+=_RecoCutFlags[2].c_str();
   recSecElPtSC_12  = new TH1D(recSecElPtSC_name.c_str(), "Reconstructed Second electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recSecElPtSC_12);
   recSecElPtSC_name+=_RecoCutFlags[3].c_str();
   recSecElPtSC_123  = new TH1D(recSecElPtSC_name.c_str(), "Reconstructed Second electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recSecElPtSC_123);
   recSecElPtSC_name+=_RecoCutFlags[4].c_str();
   recSecElPtSC_1234  = new TH1D(recSecElPtSC_name.c_str(), "Reconstructed Second electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recSecElPtSC_1234);
   recSecElPtSC_name+=_RecoCutFlags[5].c_str();
   recSecElPtSC_12345  = new TH1D(recSecElPtSC_name.c_str(), "Reconstructed Second electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recSecElPtSC_12345);
   recSecElPtSC_name+=_RecoCutFlags[6].c_str();
   recSecElPtSC_123456  = new TH1D(recSecElPtSC_name.c_str(), "Reconstructed Second electron p_{T} - SameCharge", 200, 0, 200);
   _histoVector.push_back(recSecElPtSC_123456);
   
   string recLeadElEtaSC_name = "recLeadElEtaSC";
   recLeadElEtaSC_name+=_RecoCutFlags[1].c_str();  
   recLeadElEtaSC_1 = new TH1D(recLeadElEtaSC_name.c_str(), "Reconstructed Leading electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEtaSC_1);
   recLeadElEtaSC_name+=_RecoCutFlags[2].c_str();
   recLeadElEtaSC_12 = new TH1D(recLeadElEtaSC_name.c_str(), "Reconstructed Leading electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEtaSC_12);
   recLeadElEtaSC_name+=_RecoCutFlags[3].c_str();
   recLeadElEtaSC_123 = new TH1D(recLeadElEtaSC_name.c_str(), "Reconstructed Leading electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEtaSC_123);
   recLeadElEtaSC_name+=_RecoCutFlags[4].c_str();
   recLeadElEtaSC_1234 = new TH1D(recLeadElEtaSC_name.c_str(), "Reconstructed Leading electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEtaSC_1234);
   recLeadElEtaSC_name+=_RecoCutFlags[5].c_str();
   recLeadElEtaSC_12345 = new TH1D(recLeadElEtaSC_name.c_str(), "Reconstructed Leading electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEtaSC_12345);
   recLeadElEtaSC_name+=_RecoCutFlags[6].c_str();
   recLeadElEtaSC_123456 = new TH1D(recLeadElEtaSC_name.c_str(), "Reconstructed Leading electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEtaSC_123456);
   
   string recSecElEtaSC_name = "recSecElEtaSC";
   recSecElEtaSC_name+=_RecoCutFlags[1].c_str(); 
   recSecElEtaSC_1  = new TH1D(recSecElEtaSC_name.c_str(), "Reconstructed Second electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEtaSC_1);
   recSecElEtaSC_name+=_RecoCutFlags[2].c_str(); 
   recSecElEtaSC_12  = new TH1D(recSecElEtaSC_name.c_str(), "Reconstructed Second electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEtaSC_12);
   recSecElEtaSC_name+=_RecoCutFlags[3].c_str(); 
   recSecElEtaSC_123  = new TH1D(recSecElEtaSC_name.c_str(), "Reconstructed Second electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEtaSC_123); 
   recSecElEtaSC_name+=_RecoCutFlags[4].c_str(); 
   recSecElEtaSC_1234  = new TH1D(recSecElEtaSC_name.c_str(), "Reconstructed Second electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEtaSC_1234);
   recSecElEtaSC_name+=_RecoCutFlags[5].c_str();  
   recSecElEtaSC_12345  = new TH1D(recSecElEtaSC_name.c_str(), "Reconstructed Second electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEtaSC_12345);
   recSecElEtaSC_name+=_RecoCutFlags[6].c_str();  
   recSecElEtaSC_123456  = new TH1D(recSecElEtaSC_name.c_str(), "Reconstructed Second electron #eta - SameCharge", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEtaSC_123456);
   
   string recLeadElIPSC_name = "recLeadElIPSC";
   recLeadElIPSC_name+=_RecoCutFlags[1].c_str();  
   recLeadElIPSC_1 = new TH1D(recLeadElIPSC_name.c_str(), "Reconstructed Leading electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recLeadElIPSC_1);
   recLeadElIPSC_name+=_RecoCutFlags[2].c_str();  
   recLeadElIPSC_12 = new TH1D(recLeadElIPSC_name.c_str(), "Reconstructed Leading electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recLeadElIPSC_12);
   recLeadElIPSC_name+=_RecoCutFlags[3].c_str();  
   recLeadElIPSC_123 = new TH1D(recLeadElIPSC_name.c_str(), "Reconstructed Leading electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recLeadElIPSC_123);
   recLeadElIPSC_name+=_RecoCutFlags[4].c_str();  
   recLeadElIPSC_1234 = new TH1D(recLeadElIPSC_name.c_str(), "Reconstructed Leading electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recLeadElIPSC_1234);
   recLeadElIPSC_name+=_RecoCutFlags[5].c_str();  
   recLeadElIPSC_12345 = new TH1D(recLeadElIPSC_name.c_str(), "Reconstructed Leading electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recLeadElIPSC_12345);
   recLeadElIPSC_name+=_RecoCutFlags[6].c_str();  
   recLeadElIPSC_123456 = new TH1D(recLeadElIPSC_name.c_str(), "Reconstructed Leading electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recLeadElIPSC_123456);
   
   string recSecElIPSC_name = "recSecElIPSC";
   recSecElIPSC_name+=_RecoCutFlags[1].c_str();  
   recSecElIPSC_1 = new TH1D(recSecElIPSC_name.c_str(), "Reconstructed Second electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recSecElIPSC_1);
   recSecElIPSC_name+=_RecoCutFlags[2].c_str();  
   recSecElIPSC_12 = new TH1D(recSecElIPSC_name.c_str(), "Reconstructed Second electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recSecElIPSC_12);
   recSecElIPSC_name+=_RecoCutFlags[3].c_str();  
   recSecElIPSC_123 = new TH1D(recSecElIPSC_name.c_str(), "Reconstructed Second electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recSecElIPSC_123);
   recSecElIPSC_name+=_RecoCutFlags[4].c_str();  
   recSecElIPSC_1234 = new TH1D(recSecElIPSC_name.c_str(), "Reconstructed Second electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recSecElIPSC_1234);
   recSecElIPSC_name+=_RecoCutFlags[5].c_str();  
   recSecElIPSC_12345 = new TH1D(recSecElIPSC_name.c_str(), "Reconstructed Second electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recSecElIPSC_12345);
   recSecElIPSC_name+=_RecoCutFlags[6].c_str();  
   recSecElIPSC_123456 = new TH1D(recSecElIPSC_name.c_str(), "Reconstructed Second electron IP - SameCharge", 100, 0, 0.1);
   _histoVector.push_back(recSecElIPSC_123456);
   
   string recLeadElfBremSC_name = "recLeadElfBremSC";
   recLeadElfBremSC_name+=_RecoCutFlags[1].c_str();  
   recLeadElfBremSC_1 = new TH1D(recLeadElfBremSC_name.c_str(), "Reconstructed Leading electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recLeadElfBremSC_1);
   recLeadElfBremSC_name+=_RecoCutFlags[2].c_str();  
   recLeadElfBremSC_12 = new TH1D(recLeadElfBremSC_name.c_str(), "Reconstructed Leading electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recLeadElfBremSC_12);
   recLeadElfBremSC_name+=_RecoCutFlags[3].c_str();  
   recLeadElfBremSC_123 = new TH1D(recLeadElfBremSC_name.c_str(), "Reconstructed Leading electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recLeadElfBremSC_123);
   recLeadElfBremSC_name+=_RecoCutFlags[4].c_str();  
   recLeadElfBremSC_1234 = new TH1D(recLeadElfBremSC_name.c_str(), "Reconstructed Leading electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recLeadElfBremSC_1234);
   recLeadElfBremSC_name+=_RecoCutFlags[5].c_str();  
   recLeadElfBremSC_12345 = new TH1D(recLeadElfBremSC_name.c_str(), "Reconstructed Leading electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recLeadElfBremSC_12345);
   recLeadElfBremSC_name+=_RecoCutFlags[6].c_str();  
   recLeadElfBremSC_123456 = new TH1D(recLeadElfBremSC_name.c_str(), "Reconstructed Leading electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recLeadElfBremSC_123456);
   
   string recSecElfBremSC_name = "recSecElfBremSC";
   recSecElfBremSC_name+=_RecoCutFlags[1].c_str();  
   recSecElfBremSC_1 = new TH1D(recSecElfBremSC_name.c_str(), "Reconstructed Second electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recSecElfBremSC_1);
   recSecElfBremSC_name+=_RecoCutFlags[2].c_str();  
   recSecElfBremSC_12 = new TH1D(recSecElfBremSC_name.c_str(), "Reconstructed Second electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recSecElfBremSC_12);
   recSecElfBremSC_name+=_RecoCutFlags[3].c_str();  
   recSecElfBremSC_123 = new TH1D(recSecElfBremSC_name.c_str(), "Reconstructed Second electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recSecElfBremSC_123);
   recSecElfBremSC_name+=_RecoCutFlags[4].c_str();  
   recSecElfBremSC_1234 = new TH1D(recSecElfBremSC_name.c_str(), "Reconstructed Second electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recSecElfBremSC_1234);
   recSecElfBremSC_name+=_RecoCutFlags[5].c_str();  
   recSecElfBremSC_12345 = new TH1D(recSecElfBremSC_name.c_str(), "Reconstructed Second electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recSecElfBremSC_12345);
   recSecElfBremSC_name+=_RecoCutFlags[6].c_str();  
   recSecElfBremSC_123456 = new TH1D(recSecElfBremSC_name.c_str(), "Reconstructed Second electron fBrem - SameCharge", 100, 0, 2);
   _histoVector.push_back(recSecElfBremSC_123456);
   
   //Electron identification variable
   _Eldir->cd();
   
   HoverE_Barrel_PreEiD = new TH1D("HoverE_Barrel_PreEiD", "Electron Hcal Over Ecal - Barrel", 100, 0, 0.1);
   _histoVector.push_back(HoverE_Barrel_PreEiD);
   DeltaEtaIn_Barrel_PreEiD = new TH1D("DeltaEtaIn_Barrel_PreEiD", "Electron Delta Eta In - Barrel", 100, -0.1, 0.1);
   _histoVector.push_back(DeltaEtaIn_Barrel_PreEiD);
   DeltaPhiIn_Barrel_PreEiD = new TH1D("DeltaPhiIn_Barrel_PreEiD", "Electron Delta Phi In - Barrel", 100, -0.4, 0.4);
   _histoVector.push_back(DeltaPhiIn_Barrel_PreEiD);
   SigmaIEtaIEta_Barrel_PreEiD= new TH1D("SigmaIEtaIEta_Barrel_PreEiD", "Electron SigmaIEtaIEta - Barrel", 100, 0, 0.1);
   _histoVector.push_back(SigmaIEtaIEta_Barrel_PreEiD);
   
   HoverE_Endcap_PreEiD = new TH1D("HoverE_Endcap_PreEiD", "Electron Hcal Over Ecal - Endcap", 100, 0, 0.1);
   _histoVector.push_back(HoverE_Endcap_PreEiD);
   DeltaEtaIn_Endcap_PreEiD = new TH1D("DeltaEtaIn_Endcap_PreEiD", "Electron Delta Eta In - Endcap", 100, -0.1, 0.1);
   _histoVector.push_back(DeltaEtaIn_Endcap_PreEiD);
   DeltaPhiIn_Endcap_PreEiD = new TH1D("DeltaPhiIn_Endcap_PreEiD", "Electron Delta Phi In - Endcap", 100, -0.4, 0.4);
   _histoVector.push_back(DeltaPhiIn_Endcap_PreEiD);
   SigmaIEtaIEta_Endcap_PreEiD= new TH1D("SigmaIEtaIEta_Endcap_PreEiD", "Electron SigmaIEtaIEta - Endcap", 100, 0, 0.1);
   _histoVector.push_back(SigmaIEtaIEta_Endcap_PreEiD);
   
   //Z Electrons Isolation variables
   _Eldir->cd();
   
   recTrackIsoLead_PreIso = new TH1D("recTrackIsoLead_PreIso", "Lead Track Isolation SumPt", 3000, 0, 30);
   _histoVector.push_back(recTrackIsoLead_PreIso);
   recEcalIsoLead_PreIso  = new TH1D("recEcalIsoLead_PreIso", "Lead ECAL Isolation SumPt", 3000, 0, 30);
   _histoVector.push_back(recEcalIsoLead_PreIso);
   recHcalIsoLead_PreIso  = new TH1D("recHcalIsoLead_PreIso", "Lead HCAL Isolation SumPt", 3000, 0, 30);
   _histoVector.push_back(recHcalIsoLead_PreIso);
   recTrackIsoSec_PreIso = new TH1D("recTrackIsoSec_PreIso", "Sec Track Isolation SumPt", 3000, 0, 30);
   _histoVector.push_back(recTrackIsoSec_PreIso);
   recEcalIsoSec_PreIso  = new TH1D("recEcalIsoSec_PreIso", "Sec ECAL Isolation SumPt", 3000, 0, 30);
   _histoVector.push_back(recEcalIsoSec_PreIso);
   recHcalIsoSec_PreIso  = new TH1D("recHcalIsoSec_PreIso", "Sec HCAL Isolation SumPt", 3000, 0, 30); 
   _histoVector.push_back(recHcalIsoSec_PreIso);
   
   
   recRelIso_Barrel_PreIso   = new TH1D("recRelIso_Barrel_PreIso", "Comb Relative Isolation - Barrel", 3000, 0, 30);
   _histoVector.push_back(recRelIso_Barrel_PreIso);
   recRelIso_Endcap_PreIso   = new TH1D("recRelIso_Endcap_PreIso", "Comb Relative Isolation - Endcap", 3000, 0, 30);
   _histoVector.push_back(recRelIso_Endcap_PreIso);
   
   
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
   
   DeltaRvsCharge_JetRec = new TH2D("DeltaRvsCharge_JetRec", "Delta R Reco Jets - Reco Z-Electrons", 400, -2, 2, 1000, 0, 6);
   _histoVector2D.push_back(DeltaRvsCharge_JetRec);
   DeltaRvsCharge_JetRec_Iso = new TH2D("DeltaRvsCharge_JetRec_Iso", "Delta R Reco ISO Jets - Reco Z-Electrons", 400, -2, 2, 1000, 0, 6);
   _histoVector2D.push_back(DeltaRvsCharge_JetRec);
   DeltaRvsCharge_JetRec_NotIso = new TH2D("DeltaRvsCharge_JetRec_NotIso", "Delta R Reco NOT ISO Jets - Reco Z-Electrons", 400, -2, 2, 1000, 0, 6);
   _histoVector2D.push_back(DeltaRvsCharge_JetRec);
   MinDeltaR_ZDau = new TH1D("MinDeltaR_ZDau", "Min Delta R Z Daughters All Jets", 100, 0, 10);
   _histoVector.push_back(MinDeltaR_ZDau);
   AllJetCharge = new TH1D("AllJetCharge", "All Jet Charge", 400, -2, 2);
   _histoVector.push_back(AllJetCharge);
   IsoJetCharge = new TH1D("IsoJetCharge", "Iso Jet Charge", 400, -2, 2);
   _histoVector.push_back(IsoJetCharge);
   NotIsoJetCharge = new TH1D("NotIsoJetCharge", "Not Iso Jet Charge", 400, -2, 2);
   _histoVector.push_back(NotIsoJetCharge);
   DeltaR_IsoJet = new TH1D("DeltaR_IsoJet", "Min Delta R Iso Jet - Z Electrons", 100, 0, 10);
   _histoVector.push_back(DeltaR_IsoJet);
   DeltaR_NotIsoJet = new TH1D("DeltaR_NotIsoJet", "Min Delta R Not Iso Jet - Z Electrons", 100, 0, 10);
   _histoVector.push_back(DeltaR_NotIsoJet);
   DeltaR_IsoJet_ElType = new TH1D("DeltaR_IsoJet_ElType", "Min Delta R Iso Jet Electron Type - Z Electrons", 100, 0, 10);
   _histoVector.push_back(DeltaR_IsoJet_ElType);
   
   //Electron charge MisID
   _charge_dir = _dir->mkdir("ChargeMisID");
   _charge_dir->cd();
   
   ChargeMisID_Pt_Acc = new TH1D("ChargeMisID_Pt_Acc", "ChargeMisID - gen electron p_{T}", 200, 0, 200);
   _histoVector.push_back(ChargeMisID_Pt_Acc);
   ChargeMisID_Eta_Acc = new TH1D("ChargeMisID_Eta_Acc", "ChargeMisID - gen electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(ChargeMisID_Eta_Acc);
   ChargeMisID_Hit_Acc = new TH1D("ChargeMisID_Hit_Acc", "ChargeMisID - track hits", 30, 0, 30);
   _histoVector.push_back(ChargeMisID_Hit_Acc);  
   ChargeMisID_fBrem_Acc = new TH1D("ChargeMisID_fBrem_Acc", "ChargeMisID - fBrem", 100, 0, 2);
   _histoVector.push_back(ChargeMisID_fBrem_Acc);
   ChargeMisID_IP_Acc = new TH1D("ChargeMisID_IP_Acc", "ChargeMisID - IP", 100, 0, 0.1);
   _histoVector.push_back(ChargeMisID_IP_Acc);
   ChargeMisID_RecoExclJet_Acc = new TH1D("ChargeMisID_RecoExclJet_Acc", "ChargeMisID - Reco Jet exclusive multiplicity", 10, -0.5, 9.5);
   _histoVector.push_back(ChargeMisID_RecoExclJet_Acc);
   ChargeMisID_RecoInclJet_Acc = new TH1D("ChargeMisID_RecoInclJet_Acc", "ChargeMisID - Reco Jet inclusive multiplicity", 10, -0.5, 9.5);
   _histoVector.push_back(ChargeMisID_RecoInclJet_Acc); 
   
   CorrectCharge_Pt_Acc = new TH1D("CorrectCharge_Pt_Acc", "CorrectCharge - gen electron p_{T}", 200, 0, 200);
   _histoVector.push_back(CorrectCharge_Pt_Acc);
   CorrectCharge_Eta_Acc = new TH1D("CorrectCharge_Eta_Acc", "CorrectCharge - gen electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(CorrectCharge_Eta_Acc);
   CorrectCharge_Hit_Acc = new TH1D("CorrectCharge_Hit_Acc", "CorrectCharge - track hits", 30, 0, 30);
   _histoVector.push_back(CorrectCharge_Hit_Acc);
   CorrectCharge_fBrem_Acc = new TH1D("CorrectCharge_fBrem_Acc", "CorrectCharge - fBrem", 100, 0, 2);
   _histoVector.push_back(CorrectCharge_fBrem_Acc);
   CorrectCharge_IP_Acc = new TH1D("CorrectCharge_IP_Acc", "CorrectCharge - IP", 100, 0, 0.1);
   _histoVector.push_back(CorrectCharge_IP_Acc);
   CorrectCharge_RecoExclJet_Acc = new TH1D("CorrectCharge_RecoExclJet_Acc", "CorrectCharge - Reco Jet exclusive multiplicity", 10, -0.5, 9.5);
   _histoVector.push_back(CorrectCharge_RecoExclJet_Acc);
   CorrectCharge_RecoInclJet_Acc = new TH1D("CorrectCharge_RecoInclJet_Acc", "CorrectCharge - Reco Jet inclusive multiplicity", 10, -0.5, 9.5);
   _histoVector.push_back(CorrectCharge_RecoInclJet_Acc);

   AllEl_Pt_Acc = new TH1D("AllEl_Pt_Acc", "AllEl - gen electron p_{T}", 200, 0, 200);
   _histoVector.push_back(AllEl_Pt_Acc);
   AllEl_Eta_Acc = new TH1D("AllEl_Eta_Acc", "AllEl - gen electron #eta", 100, -2.5, 2.5);
   _histoVector.push_back(AllEl_Eta_Acc);
   AllEl_Hit_Acc = new TH1D("AllEl_Hit_Acc", "AllEl - track hits", 30, 0, 30);
   _histoVector.push_back(AllEl_Hit_Acc);
   AllEl_fBrem_Acc = new TH1D("AllEl_fBrem_Acc", "AllEl - fBrem", 100, 0, 2);
   _histoVector.push_back(AllEl_fBrem_Acc);
   AllEl_IP_Acc = new TH1D("AllEl_IP_Acc", "AllEl - IP", 100, 0, 0.1);
   _histoVector.push_back(AllEl_IP_Acc);
   AllEl_RecoExclJet_Acc = new TH1D("AllEl_RecoExclJet_Acc", "AllEl - Reco Jet exclusive multiplicity", 10, -0.5, 9.5);
   _histoVector.push_back(AllEl_RecoExclJet_Acc);
   AllEl_RecoInclJet_Acc = new TH1D("AllEl_RecoInclJet_Acc", "AllEl - Reco Jet inclusive multiplicity", 10, -0.5, 9.5);
   _histoVector.push_back(AllEl_RecoInclJet_Acc);
   
   PixelHit_OC = new TH1D("PixelHit_OC", "Number of Pixel Hit - ZOC", 10,0,10);
   _histoVector.push_back(PixelHit_OC);
   PixelHit_SC = new TH1D("PixelHit_SC", "Number of Pixel Hit - ZSC", 10,0,10);
   _histoVector.push_back(PixelHit_SC);
   FirstPixelBarrelHit_OC = new TH1D("FirstPixelBarrelHit_OC", "Hit in first pixel barrel layer - ZOC", 4,-2,2);
   _histoVector.push_back(FirstPixelBarrelHit_OC);
   FirstPixelBarrelHit_SC = new TH1D("FirstPixelBarrelHit_SC", "Hit in first pixel barrel layer - ZSC", 4,-2,2);
   _histoVector.push_back(FirstPixelBarrelHit_SC);
   
   HitVsEta_CorrCharge = new TProfile("HitVsEta_CorrCharge", "Track hits vs eta - CorrCharge", 120,-3,3);
   HitVsEta_MisIDCharge = new TProfile("HitVsEta_MisIDCharge", "Track hits vs eta - MisIDCharge", 120,-3,3);
   HitVsEta_AllEl = new TProfile("HitVsEta_AllEl", "Track hits vs eta - AllEl", 120,-3,3);
   fBremVsEta = new TProfile("fBremVsEta", "fBrem Vs Eta", 120,-3,3);
   
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
  
  cout << "RecoElectron analyzing nr. file = "<<_fileCounter<<endl;
  cout << "RecoElectron analyzing nr. event = "<<_entries<<endl;
  
  delete ch; 
   
  cout << "RecoElectron Worker built." << endl;   
}

RecoElectron::~RecoElectron(){
  _file->ls();
  
}

void  RecoElectron::process(const fwlite::Event& iEvent)
{
   
   run = iEvent.id().run();
   if(_sample=="mc")run=-1;

   double weight = 1.;
   _file->cd();

   fwlite::Handle<std::vector<pat::Electron> > electronHandle;
   electronHandle.getByLabel(iEvent, "selectedElectrons");

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zrecHandle;
   zrecHandle.getByLabel(iEvent, "zeerec");
   
   fwlite::Handle<std::vector<reco::CompositeCandidate> > zrecHandleSC;
   zrecHandleSC.getByLabel(iEvent, "zeerecSameChargePlus");
  
   fwlite::Handle<std::vector<pat::Jet> > jetrecHandle;
   if(_JetType=="PF")jetrecHandle.getByLabel(iEvent, "selectedJets");
   if(_JetType=="PFL1CORR")jetrecHandle.getByLabel(iEvent, "selectedJetsL1Corrected");

   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");
   
   // Available triggers
   static map<std::string, std::pair<int, int> > TrgVector = elTrigger();
   static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
   int trg = 1;
   for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
   if(isTriggerAvailable(*triggerHandle,TrgVectorIter->first.c_str()))_TrgList[trg]=1;
   trg++;
   }
   
   //Event plots
   
   recZnum->Fill(zrecHandle->size());
   
   recZSCnum->Fill(zrecHandleSC->size());
   
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
   
   std::vector<const pat::Electron*> zdaughters;
   
   const pat::Electron *dau0 = 0;
   const pat::Electron *dau1 = 0;  
   
    if(zrecHandle->size()){
   
    zdaughters = ZRECDaughters((*zrecHandle)[0]);
  
     if(zdaughters.size()){
     
     dau0 = zdaughters[0];
     dau1 = zdaughters[1];
     
     if(dau1->pt()>dau0->pt())throw cms::Exception("PATAnalysis:RecoElectron_WrongElectronOrder") << "ERROR! Z electrons are in wrong order!";
    
     }
     
     }

   if(zdaughters.size()){  
   for(unsigned int i = 0; i < recjets.size(); i++){     
   if(IsoJet<pat::Electron>(zdaughters,*recjets[i]))isorecjets.push_back(recjets[i]);
   if(!IsoJet<pat::Electron>(zdaughters,*recjets[i]))notisorecjets.push_back(recjets[i]);}
   }else if(!zdaughters.size()){
   for(unsigned int i = 0; i < recjets.size(); i++)isorecjets.push_back(recjets[i]);}
   
   // Same Charge Z study
   
   if(zrecHandleSC->size()){
     
     std::vector<const pat::Electron*> zdaughtersSC = ZRECDaughters((*zrecHandleSC)[0]);
     
     const pat::Electron *dau0SC = 0;
     const pat::Electron *dau1SC = 0;
     
     if(zdaughtersSC.size()){
    
     
     dau0SC = zdaughtersSC[0];
     dau1SC = zdaughtersSC[1];
     
     //Pre selections events
    
   
    
     }
   
     //Events with a selected Zee SC - SELECTIONS: 1
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)){
     
     //Z variables
     recPtZSC_1->Fill((*zrecHandleSC)[0].pt());
     recEtaZSC_1->Fill((*zrecHandleSC)[0].eta());
     recMassZSC_1->Fill((*zrecHandleSC)[0].mass());
     
     //Z Electrons variables
     recLeadElEtaSC_1->Fill(dau0SC->eta());
     recSecElEtaSC_1->Fill(dau1SC->eta());
     recLeadElPtSC_1->Fill(dau0SC->pt());
     recSecElPtSC_1->Fill(dau1SC->pt());
     recLeadElIPSC_1->Fill(dau0SC->dB());
     recSecElIPSC_1->Fill(dau1SC->dB());
     recLeadElfBremSC_1->Fill(dau0SC->fbrem());
     recSecElfBremSC_1->Fill(dau1SC->fbrem());
    
     }
     
     //Events with a selected Zee SC - SELECTIONS: 1+2
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)){
     
     //Z variables
     recPtZSC_12->Fill((*zrecHandleSC)[0].pt());
     recEtaZSC_12->Fill((*zrecHandleSC)[0].eta());
     recMassZSC_12->Fill((*zrecHandleSC)[0].mass());
     
     //Z Electrons variables
     recLeadElEtaSC_12->Fill(dau0SC->eta());
     recSecElEtaSC_12->Fill(dau1SC->eta());
     recLeadElPtSC_12->Fill(dau0SC->pt());
     recSecElPtSC_12->Fill(dau1SC->pt());
     recLeadElIPSC_12->Fill(dau0SC->dB());
     recSecElIPSC_12->Fill(dau1SC->dB());
     recLeadElfBremSC_12->Fill(dau0SC->fbrem());
     recSecElfBremSC_12->Fill(dau1SC->fbrem());
    
     }
   
     //Events with a selected Zee SC - SELECTIONS: 1+2+3
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)){
     
     //Z variables
     recPtZSC_123->Fill((*zrecHandleSC)[0].pt());
     recEtaZSC_123->Fill((*zrecHandleSC)[0].eta());
     recMassZSC_123->Fill((*zrecHandleSC)[0].mass());
     
     //Z Electrons variables
     recLeadElEtaSC_123->Fill(dau0SC->eta());
     recSecElEtaSC_123->Fill(dau1SC->eta());
     recLeadElPtSC_123->Fill(dau0SC->pt());
     recSecElPtSC_123->Fill(dau1SC->pt());
     recLeadElIPSC_123->Fill(dau0SC->dB());
     recSecElIPSC_123->Fill(dau1SC->dB());
     recLeadElfBremSC_123->Fill(dau0SC->fbrem());
     recSecElfBremSC_123->Fill(dau1SC->fbrem());
    
     }
   
     //Events with a selected Zee SC - SELECTIONS: 1+2+3+4
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)){
     
     //Z variables
     recPtZSC_1234->Fill((*zrecHandleSC)[0].pt());
     recEtaZSC_1234->Fill((*zrecHandleSC)[0].eta());
     recMassZSC_1234->Fill((*zrecHandleSC)[0].mass());
    
     //Z Electrons variables
     recLeadElEtaSC_1234->Fill(dau0SC->eta());
     recSecElEtaSC_1234->Fill(dau1SC->eta());
     recLeadElPtSC_1234->Fill(dau0SC->pt());
     recSecElPtSC_1234->Fill(dau1SC->pt());
     
     recLeadElIPSC_1234->Fill(dau0SC->dB());
     recSecElIPSC_1234->Fill(dau1SC->dB());
     recLeadElfBremSC_1234->Fill(dau0SC->fbrem());
     recSecElfBremSC_1234->Fill(dau1SC->fbrem());
  
     }
     
     //Events with a selected Zee SC - SELECTIONS: 1+2+3+4+5
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[5].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)){
     
     //Z variables
     recPtZSC_12345->Fill((*zrecHandleSC)[0].pt());
     recEtaZSC_12345->Fill((*zrecHandleSC)[0].eta());
     recMassZSC_12345->Fill((*zrecHandleSC)[0].mass());
     
     //Z Electrons variables
     recLeadElEtaSC_12345->Fill(dau0SC->eta());
     recSecElEtaSC_12345->Fill(dau1SC->eta());
     recLeadElPtSC_12345->Fill(dau0SC->pt());
     recSecElPtSC_12345->Fill(dau1SC->pt());
     recLeadElIPSC_12345->Fill(dau0SC->dB());
     recSecElIPSC_12345->Fill(dau1SC->dB());
     recLeadElfBremSC_12345->Fill(dau0SC->fbrem());
     recSecElfBremSC_12345->Fill(dau1SC->fbrem());
  
     }
     
     //Events with a selected Zee SC - SELECTIONS: 1+2+3+4+5+6
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[5].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[6].c_str(), (*zrecHandleSC)[0], *triggerHandle, run)){
     
     //Z variables
     recPtZSC_123456->Fill((*zrecHandleSC)[0].pt());
     recEtaZSC_123456->Fill((*zrecHandleSC)[0].eta());
     recMassZSC_123456->Fill((*zrecHandleSC)[0].mass());
     
     //Z Electrons variables
     recLeadElEtaSC_123456->Fill(dau0SC->eta());
     recSecElEtaSC_123456->Fill(dau1SC->eta());
     recLeadElPtSC_123456->Fill(dau0SC->pt());
     recSecElPtSC_123456->Fill(dau1SC->pt());
     recLeadElIPSC_123456->Fill(dau0SC->dB());
     recSecElIPSC_123456->Fill(dau1SC->dB());
     recLeadElfBremSC_123456->Fill(dau0SC->fbrem());
     recSecElfBremSC_123456->Fill(dau1SC->fbrem());
   
     }
   
    } 
     
   // End of Same Charge Z study 

   // Electron Charge MisID study
   
   std::vector<pat::Electron> selectedelectrons;  
   for(unsigned i = 0; i != electronHandle->size(); i++){
   if((*electronHandle)[i].pt() > ptelcut && fabs((*electronHandle)[i].eta()) < etaelcut && (fabs((*electronHandle)[i].eta())<eta_el_excl_down || fabs((*electronHandle)[i].eta())>eta_el_excl_up)) selectedelectrons.push_back((*electronHandle)[i]);
   }
   
   bool ZdauOCinAcc, ZdauSCinAcc;
  
   for(unsigned i = 0; i != selectedelectrons.size(); i++){
   
   reco::GenParticleRef genCorrElectron = selectedelectrons[i].genParticleById(11,0,true);
   reco::GenParticleRef genMisElectron = selectedelectrons[i].genParticleById(-11,0,true);
      
   const reco::GsfTrack eltrack = *(selectedelectrons[i].gsfTrack());
   const reco::HitPattern& hp = eltrack.hitPattern();

   ZdauOCinAcc = false; 
   ZdauSCinAcc = false;
  
   if(_GenParticleMatch){
 
   const reco::Candidate *mom;
     
   if(genCorrElectron.isNonnull()){
   if(genCorrElectron->pt() > ptelcut && fabs(genCorrElectron->eta()) < etaelcut && (fabs(genCorrElectron->eta())<eta_el_excl_down || fabs(genCorrElectron->eta())>eta_el_excl_up)){
     if(genCorrElectron->numberOfMothers()){
       mom = genCorrElectron->mother();
       if(mom->pdgId()==23)ZdauOCinAcc=true;
       while(mom->numberOfMothers()){
	 mom = mom->mother();
	 if(mom->pdgId()==23)ZdauOCinAcc=true;
    }
    }
    }
     }
 
   if(genMisElectron.isNonnull()){
   if(genMisElectron->pt() > ptelcut && fabs(genMisElectron->eta()) < etaelcut && (fabs(genMisElectron->eta())<eta_el_excl_down || fabs(genMisElectron->eta())>eta_el_excl_up)){
     if(genMisElectron->numberOfMothers()){
       mom = genMisElectron->mother();
       if(mom->pdgId()==23)ZdauSCinAcc=true; 
       while(mom->numberOfMothers()){
	 mom = mom->mother();
	 if(mom->pdgId()==23)ZdauSCinAcc=true;
    }
    }
    }
     }
  
   }else{
  
   if(genCorrElectron.isNonnull()){
   if(genCorrElectron->pt() > ptelcut && fabs(genCorrElectron->eta()) < etaelcut && (fabs(genCorrElectron->eta())<eta_el_excl_down || fabs(genCorrElectron->eta())>eta_el_excl_up)){   
     ZdauOCinAcc=true; 
    }}

   if(genMisElectron.isNonnull()){
   if(genMisElectron->pt() > ptelcut && fabs(genMisElectron->eta()) < etaelcut && (fabs(genMisElectron->eta())<eta_el_excl_down || fabs(genMisElectron->eta())>eta_el_excl_up)){
      ZdauSCinAcc=true; 
    }}
    
    }
 
   if(ZdauOCinAcc){

   fBremVsEta->Fill(selectedelectrons[i].eta(),selectedelectrons[i].fbrem());
   
   PixelHit_OC->Fill(hp.numberOfValidPixelBarrelHits()+hp.numberOfValidPixelEndcapHits());
 
   if(hp.hasValidHitInFirstPixelBarrel()){
   FirstPixelBarrelHit_OC->Fill(1);
   }else{
   FirstPixelBarrelHit_OC->Fill(-1);}
  
   _charge_dir->cd();
   addHistosVsMulti(isorecjets.size(), "ElCorrChargePtExcl", "Correct Charge electron p_{T} spectrum", 200, 0., 200., ElCorrChargePtExclMulti);
   addHistosVsMulti(isorecjets.size(), "ElCorrChargeEtaExcl", "Correct Charge electron #eta spectrum", 100, -5, 5, ElCorrChargeEtaExclMulti);
   addHistosVsMulti(isorecjets.size(), "ElCorrChargeHitExcl", "Correct Charge electron track hit number", 30, 0., 30., ElCorrChargeHitExclMulti);
   addHistosVsMulti(isorecjets.size(), "ElCorrChargefBremExcl", "Correct Charge electron fBrem", 100, 0., 2., ElCorrChargefBremExclMulti);
  
   CorrectCharge_Pt_Acc->Fill(genCorrElectron->pt());
   CorrectCharge_Eta_Acc->Fill(genCorrElectron->eta());
   CorrectCharge_Hit_Acc->Fill(selectedelectrons[i].gsfTrack()->numberOfValidHits());
   CorrectCharge_fBrem_Acc->Fill(selectedelectrons[i].fbrem());
   CorrectCharge_IP_Acc->Fill(selectedelectrons[i].dB());
   CorrectCharge_RecoExclJet_Acc->Fill(isorecjets.size());
 
   for(unsigned int n = 0; (n < isorecjets.size()+1)&&((n+1)<10); n++)CorrectCharge_RecoInclJet_Acc->AddBinContent(n+1,1);
   
   HitVsEta_CorrCharge->Fill(genCorrElectron->eta(),selectedelectrons[i].gsfTrack()->numberOfValidHits());
   
   ElCorrChargePtExclMulti[isorecjets.size()]->Fill(genCorrElectron->pt(), weight);
   ElCorrChargeEtaExclMulti[isorecjets.size()]->Fill(genCorrElectron->eta(), weight);
   ElCorrChargeHitExclMulti[isorecjets.size()]->Fill(selectedelectrons[i].gsfTrack()->numberOfValidHits(), weight);
   ElCorrChargefBremExclMulti[isorecjets.size()]->Fill(selectedelectrons[i].fbrem(), weight);
 
   _charge_dir->cd();
   addHistosVsMulti(isorecjets.size(), "AllElPtExcl", "All electron p_{T} spectrum", 200, 0., 200., AllElPtExclMulti);
   addHistosVsMulti(isorecjets.size(), "AllElEtaExcl", "All electron #eta spectrum", 100, -5, 5, AllElEtaExclMulti);
   addHistosVsMulti(isorecjets.size(), "AllElHitExcl", "All electron track hit number", 30, 0., 30., AllElHitExclMulti);
   addHistosVsMulti(isorecjets.size(), "AllElfBremExcl", "All electron fBrem", 100, 0., 2., AllElfBremExclMulti);
 
   AllEl_Eta_Acc->Fill(genCorrElectron->eta());
   AllEl_Hit_Acc->Fill(selectedelectrons[i].gsfTrack()->numberOfValidHits());
   AllEl_fBrem_Acc->Fill(selectedelectrons[i].fbrem());
   AllEl_IP_Acc->Fill(selectedelectrons[i].dB());
   AllEl_RecoExclJet_Acc->Fill(isorecjets.size());
   
   for(unsigned int n = 0; (n < isorecjets.size()+1)&&((n+1)<10); n++)AllEl_RecoInclJet_Acc->AddBinContent(n+1,1);
   
   HitVsEta_AllEl->Fill(genCorrElectron->eta(),selectedelectrons[i].gsfTrack()->numberOfValidHits());
   
   AllElPtExclMulti[isorecjets.size()]->Fill(genCorrElectron->pt(), weight);
   AllElEtaExclMulti[isorecjets.size()]->Fill(genCorrElectron->eta(), weight);
   AllElHitExclMulti[isorecjets.size()]->Fill(selectedelectrons[i].gsfTrack()->numberOfValidHits(), weight);
   AllElfBremExclMulti[isorecjets.size()]->Fill(selectedelectrons[i].fbrem(), weight);

   }
   
   if(ZdauSCinAcc){

   fBremVsEta->Fill(selectedelectrons[i].eta(),selectedelectrons[i].fbrem());
   
   PixelHit_SC->Fill(hp.numberOfValidPixelBarrelHits()+hp.numberOfValidPixelEndcapHits());
   
   if(hp.hasValidHitInFirstPixelBarrel()){
   FirstPixelBarrelHit_SC->Fill(1);
   }else{
   FirstPixelBarrelHit_SC->Fill(-1);}
   
   _charge_dir->cd();
   addHistosVsMulti(isorecjets.size(), "ElMisIDChargePtExcl", "MisID Charge electron p_{T} spectrum", 200, 0., 200., ElMisIDChargePtExclMulti);
   addHistosVsMulti(isorecjets.size(), "ElMisIDChargeEtaExcl", "MisID Charge electron #eta spectrum", 100, -5, 5, ElMisIDChargeEtaExclMulti);
   addHistosVsMulti(isorecjets.size(), "ElMisIDChargeHitExcl", "MisID Charge electron track hit number", 30, 0., 30., ElMisIDChargeHitExclMulti);
   addHistosVsMulti(isorecjets.size(), "ElMisIDChargefBremExcl", "MisID Charge electron fBrem", 100, 0., 2., ElMisIDChargefBremExclMulti);
   
   ChargeMisID_Pt_Acc->Fill(genMisElectron->pt());
   ChargeMisID_Eta_Acc->Fill(genMisElectron->eta());
   ChargeMisID_Hit_Acc->Fill(selectedelectrons[i].gsfTrack()->numberOfValidHits());
   ChargeMisID_fBrem_Acc->Fill(selectedelectrons[i].fbrem());
   ChargeMisID_IP_Acc->Fill(selectedelectrons[i].dB());
   ChargeMisID_RecoExclJet_Acc->Fill(isorecjets.size());
   for(unsigned int n = 0; (n < isorecjets.size()+1)&&((n+1)<10); n++)ChargeMisID_RecoInclJet_Acc->AddBinContent(n+1,1);
   
   HitVsEta_MisIDCharge->Fill(genMisElectron->eta(),selectedelectrons[i].gsfTrack()->numberOfValidHits());
   
   ElMisIDChargePtExclMulti[isorecjets.size()]->Fill(genMisElectron->pt(), weight);
   ElMisIDChargeEtaExclMulti[isorecjets.size()]->Fill(genMisElectron->eta(), weight);
   ElMisIDChargeHitExclMulti[isorecjets.size()]->Fill(selectedelectrons[i].gsfTrack()->numberOfValidHits(), weight);
   ElMisIDChargefBremExclMulti[isorecjets.size()]->Fill(selectedelectrons[i].fbrem(), weight);

   _charge_dir->cd();
   addHistosVsMulti(isorecjets.size(), "AllElPtExcl", "All electron p_{T} spectrum", 200, 0., 200., AllElPtExclMulti);
   addHistosVsMulti(isorecjets.size(), "AllElEtaExcl", "All electron #eta spectrum", 100, -5, 5, AllElEtaExclMulti);
   addHistosVsMulti(isorecjets.size(), "AllElHitExcl", "All electron track hit number", 30, 0., 30., AllElHitExclMulti);
   addHistosVsMulti(isorecjets.size(), "AllElfBremExcl", "All electron fBrem", 100, 0., 2., AllElfBremExclMulti);
   
   AllEl_Pt_Acc->Fill(genMisElectron->pt());
   AllEl_Eta_Acc->Fill(genMisElectron->eta());
   AllEl_Hit_Acc->Fill(selectedelectrons[i].gsfTrack()->numberOfValidHits());
   AllEl_fBrem_Acc->Fill(selectedelectrons[i].fbrem());
   AllEl_IP_Acc->Fill(selectedelectrons[i].dB());
   AllEl_RecoExclJet_Acc->Fill(isorecjets.size());
   for(unsigned int n = 0; (n < isorecjets.size()+1)&&((n+1)<10); n++)AllEl_RecoInclJet_Acc->AddBinContent(n+1,1);
   
   HitVsEta_AllEl->Fill(genMisElectron->eta(),selectedelectrons[i].gsfTrack()->numberOfValidHits());
   
   AllElPtExclMulti[isorecjets.size()]->Fill(genMisElectron->pt(), weight);
   AllElEtaExclMulti[isorecjets.size()]->Fill(genMisElectron->eta(), weight);
   AllElHitExclMulti[isorecjets.size()]->Fill(selectedelectrons[i].gsfTrack()->numberOfValidHits(), weight);
   AllElfBremExclMulti[isorecjets.size()]->Fill(selectedelectrons[i].fbrem(), weight);
   
   }

   } 
   
   // End of charge MisID study     
   
   // Z->ee events (OC) study: selections applied
   
   if(zrecHandle->size()){
     
     //Events with a selected Zee - SELECTIONS: 1
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, run)){
     
     //Z variables
     recPtZ_1->Fill((*zrecHandle)[0].pt());
     recEtaZ_1->Fill((*zrecHandle)[0].eta());
     recMassZ_1->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_1->Fill(dau0->eta());
     recSecElEta_1->Fill(dau1->eta());
     recLeadElPt_1->Fill(dau0->pt());
     recSecElPt_1->Fill(dau1->pt());
     recElIP_1->Fill(dau0->dB());
     recElIP_1->Fill(dau1->dB());
     recLeadElfBrem_1->Fill(dau0->fbrem());
     recSecElfBrem_1->Fill(dau1->fbrem());
     recLeadElConvCotDist_1->Fill(sqrt(pow(dau0->convDcot(),2)+pow(dau0->convDist(),2)));
     recSecElConvCotDist_1->Fill(sqrt(pow(dau1->convDcot(),2)+pow(dau1->convDist(),2)));
     
     const reco::Track *track0 = (const reco::Track*)(dau0->gsfTrack().get()); 
     const reco::Track *track1 = (const reco::Track*)(dau1->gsfTrack().get());      
     const reco::HitPattern& p0_inner = track0->trackerExpectedHitsInner(); 
     const reco::HitPattern& p1_inner = track1->trackerExpectedHitsInner();
     
     recLeadElConvMissHit_1->Fill(p0_inner.numberOfHits());
     recSecElConvMissHit_1->Fill(p1_inner.numberOfHits()); 
        
     //Jet variables
     IsoJetCounter_1->Fill(isorecjets.size());
      
     }
   
     //Events with a selected Zee - SELECTIONS: 1+2
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, run)){
    
     //Z variables
     recPtZ_12->Fill((*zrecHandle)[0].pt());
     recEtaZ_12->Fill((*zrecHandle)[0].eta());
     recMassZ_12->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_12->Fill(dau0->eta());
     recSecElEta_12->Fill(dau1->eta());
     recLeadElPt_12->Fill(dau0->pt());
     recSecElPt_12->Fill(dau1->pt());
     recElIP_12->Fill(dau0->dB());
     recElIP_12->Fill(dau1->dB());
     recLeadElfBrem_12->Fill(dau0->fbrem());
     recSecElfBrem_12->Fill(dau1->fbrem());
     recLeadElConvCotDist_12->Fill(sqrt(pow(dau0->convDcot(),2)+pow(dau0->convDist(),2)));
     recSecElConvCotDist_12->Fill(sqrt(pow(dau1->convDcot(),2)+pow(dau1->convDist(),2)));
     
     const reco::Track *track0 = (const reco::Track*)(dau0->gsfTrack().get()); 
     const reco::Track *track1 = (const reco::Track*)(dau1->gsfTrack().get());      
     const reco::HitPattern& p0_inner = track0->trackerExpectedHitsInner(); 
     const reco::HitPattern& p1_inner = track1->trackerExpectedHitsInner();
     
     recLeadElConvMissHit_12->Fill(p0_inner.numberOfHits());
     recSecElConvMissHit_12->Fill(p1_inner.numberOfHits()); 
        
     //Jet variables
     IsoJetCounter_12->Fill(isorecjets.size());
     
     }
     
     //Events with a selected Zee - SELECTIONS: 1+2+3
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, run)){
    
     //Z variables
     recPtZ_123->Fill((*zrecHandle)[0].pt());
     recEtaZ_123->Fill((*zrecHandle)[0].eta());
     recMassZ_123->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_123->Fill(dau0->eta());
     recSecElEta_123->Fill(dau1->eta());
     recLeadElPt_123->Fill(dau0->pt());
     recSecElPt_123->Fill(dau1->pt());
     recElIP_123->Fill(dau0->dB());
     recElIP_123->Fill(dau1->dB());
     recLeadElfBrem_123->Fill(dau0->fbrem());
     recSecElfBrem_123->Fill(dau1->fbrem());
     recLeadElConvCotDist_123->Fill(sqrt(pow(dau0->convDcot(),2)+pow(dau0->convDist(),2)));
     recSecElConvCotDist_123->Fill(sqrt(pow(dau1->convDcot(),2)+pow(dau1->convDist(),2)));
     
     const reco::Track *track0 = (const reco::Track*)(dau0->gsfTrack().get()); 
     const reco::Track *track1 = (const reco::Track*)(dau1->gsfTrack().get());      
     const reco::HitPattern& p0_inner = track0->trackerExpectedHitsInner(); 
     const reco::HitPattern& p1_inner = track1->trackerExpectedHitsInner();
     
     recLeadElConvMissHit_123->Fill(p0_inner.numberOfHits());
     recSecElConvMissHit_123->Fill(p1_inner.numberOfHits()); 
         
     //Jet variables
     IsoJetCounter_123->Fill(isorecjets.size());
     
     }
    
     //Events with a selected Zee - SELECTIONS: 1+2+3+4
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandle)[0], *triggerHandle, run)){
     
     //Z variables
     recPtZ_1234->Fill((*zrecHandle)[0].pt());
     recEtaZ_1234->Fill((*zrecHandle)[0].eta());
     recMassZ_1234->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_1234->Fill(dau0->eta());
     recSecElEta_1234->Fill(dau1->eta());
     recLeadElPt_1234->Fill(dau0->pt());
     recSecElPt_1234->Fill(dau1->pt());
     recElIP_1234->Fill(dau0->dB());
     recElIP_1234->Fill(dau1->dB());
     recLeadElfBrem_1234->Fill(dau0->fbrem());
     recSecElfBrem_1234->Fill(dau1->fbrem());
     recLeadElConvCotDist_1234->Fill(sqrt(pow(dau0->convDcot(),2)+pow(dau0->convDist(),2)));
     recSecElConvCotDist_1234->Fill(sqrt(pow(dau1->convDcot(),2)+pow(dau1->convDist(),2)));
     
     const reco::Track *track0 = (const reco::Track*)(dau0->gsfTrack().get()); 
     const reco::Track *track1 = (const reco::Track*)(dau1->gsfTrack().get());      
     const reco::HitPattern& p0_inner = track0->trackerExpectedHitsInner(); 
     const reco::HitPattern& p1_inner = track1->trackerExpectedHitsInner();
     
     recLeadElConvMissHit_1234->Fill(p0_inner.numberOfHits());
     recSecElConvMissHit_1234->Fill(p1_inner.numberOfHits()); 
         
     //Jet variables
     IsoJetCounter_1234->Fill(isorecjets.size());
          
    }
    
    //Events with a selected Zee - SELECTIONS: 1+2+3+4+5
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[5].c_str(), (*zrecHandle)[0], *triggerHandle, run)){

      //Z variables     
      recPtZ_12345->Fill((*zrecHandle)[0].pt());
      recEtaZ_12345->Fill((*zrecHandle)[0].eta());
      recMassZ_12345->Fill((*zrecHandle)[0].mass());

      //Z Electrons variables      
      recLeadElEta_12345->Fill(dau0->eta());
      recSecElEta_12345->Fill(dau1->eta());
      recLeadElPt_12345->Fill(dau0->pt());
      recSecElPt_12345->Fill(dau1->pt());
      recElIP_12345->Fill(dau0->dB());
      recElIP_12345->Fill(dau1->dB());
      recLeadElfBrem_12345->Fill(dau0->fbrem());
      recSecElfBrem_12345->Fill(dau1->fbrem());
      recLeadElConvCotDist_12345->Fill(sqrt(pow(dau0->convDcot(),2)+pow(dau0->convDist(),2)));
      recSecElConvCotDist_12345->Fill(sqrt(pow(dau1->convDcot(),2)+pow(dau1->convDist(),2)));
     
      const reco::Track *track0 = (const reco::Track*)(dau0->gsfTrack().get()); 
      const reco::Track *track1 = (const reco::Track*)(dau1->gsfTrack().get());      
      const reco::HitPattern& p0_inner = track0->trackerExpectedHitsInner(); 
      const reco::HitPattern& p1_inner = track1->trackerExpectedHitsInner();
     
      recLeadElConvMissHit_12345->Fill(p0_inner.numberOfHits());
      recSecElConvMissHit_12345->Fill(p1_inner.numberOfHits()); 
           
      //Jet variables     
      IsoJetCounter_12345->Fill(isorecjets.size());
      
      }
  
     //Events with a selected Zee - SELECTIONS: 1+2+3+4+5+6
     if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[5].c_str(), (*zrecHandle)[0], *triggerHandle, run) && RecSelected(_RecoCutFlags[6].c_str(), (*zrecHandle)[0], *triggerHandle, run)){
     
      //Z variables     
      recPtZ_123456->Fill((*zrecHandle)[0].pt());
      recEtaZ_123456->Fill((*zrecHandle)[0].eta());
      recMassZ_123456->Fill((*zrecHandle)[0].mass());

      //Z Electrons variables      
      recLeadElEta_123456->Fill(dau0->eta());
      recSecElEta_123456->Fill(dau1->eta());
      recLeadElPt_123456->Fill(dau0->pt());
      recSecElPt_123456->Fill(dau1->pt());
      recElIP_123456->Fill(dau0->dB());
      recElIP_123456->Fill(dau1->dB());
      recLeadElfBrem_123456->Fill(dau0->fbrem());
      recSecElfBrem_123456->Fill(dau1->fbrem());
      recLeadElConvCotDist_123456->Fill(sqrt(pow(dau0->convDcot(),2)+pow(dau0->convDist(),2)));
      recSecElConvCotDist_123456->Fill(sqrt(pow(dau1->convDcot(),2)+pow(dau1->convDist(),2)));
     
      const reco::Track *track0 = (const reco::Track*)(dau0->gsfTrack().get()); 
      const reco::Track *track1 = (const reco::Track*)(dau1->gsfTrack().get());      
      const reco::HitPattern& p0_inner = track0->trackerExpectedHitsInner(); 
      const reco::HitPattern& p1_inner = track1->trackerExpectedHitsInner();
     
      recLeadElConvMissHit_123456->Fill(p0_inner.numberOfHits());
      recSecElConvMissHit_123456->Fill(p1_inner.numberOfHits()); 
           
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
          MinDeltaR_ZDau->Fill(MinDeltaRZDau<pat::Electron>(zdaughters,*recjets[i]));       
          DeltaRvsCharge_JetRec->Fill(recjets[i]->jetCharge(), MinDeltaRZDau<pat::Electron>(zdaughters,*recjets[i]));
          AllJetCharge->Fill(recjets[i]->jetCharge());                   
      }     
      }  
    
      //Isolated jets    
      if(isorecjets.size()){      
      for (unsigned int i = 0; i < isorecjets.size(); ++i){      
        DeltaRvsCharge_JetRec_Iso->Fill(isorecjets[i]->jetCharge(), MinDeltaRZDau<pat::Electron>(zdaughters,*isorecjets[i]));
        IsoJetCharge->Fill(isorecjets[i]->jetCharge());  
        DeltaR_IsoJet->Fill(MinDeltaRZDau<pat::Electron>(zdaughters,*isorecjets[i]));
      
        //Iso Jets electron-type
        if(isorecjets[i]->jetCharge() < -0.98 || isorecjets[i]->jetCharge() > 0.98){           
          DeltaR_IsoJet_ElType->Fill(MinDeltaRZDau<pat::Electron>(zdaughters,*isorecjets[i]));          
          }          
          }     
     }
     
     //Not isolated jets
     if(notisorecjets.size()){     
      for (unsigned int i = 0; i < notisorecjets.size(); ++i){      
          NotIsoJetCharge->Fill(notisorecjets[i]->jetCharge());        
          DeltaR_NotIsoJet->Fill(MinDeltaRZDau<pat::Electron>(zdaughters,*notisorecjets[i]));
          DeltaRvsCharge_JetRec_NotIso->Fill(recjets[i]->jetCharge(), MinDeltaRZDau<pat::Electron>(zdaughters,*notisorecjets[i]));        
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
      _Eldir->cd();
      addHistosVsMulti(isorecjets.size(), "recEl1PtExcl", " reconstructed lead electron p_{T} spectrum", 200, 0., 200., recEl1PtVsExclMulti);
      addHistosVsMulti(isorecjets.size(), "recEl1EtaExcl", " reconstructed lead electron #eta spectrum", 100, -5., 5., recEl1EtaVsExclMulti);
      addHistosVsMulti(isorecjets.size(), "recEl2PtExcl", " reconstructed sec electron p_{T} spectrum", 200, 0., 200., recEl2PtVsExclMulti);
      addHistosVsMulti(isorecjets.size(), "recEl2EtaExcl", " reconstructed sec electron #eta spectrum", 100, -5., 5., recEl2EtaVsExclMulti);     
      addHistosVsMulti(isorecjets.size(), "recEl1PtIncl", " reconstructed lead electron p_{T} spectrum", 200, 0., 200., recEl1PtVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recEl1EtaIncl", " reconstructed lead electron #eta spectrum", 100, -5., 5., recEl1EtaVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recEl2PtIncl", " reconstructed sec electron p_{T} spectrum", 200, 0., 200., recEl2PtVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recEl2EtaIncl", " reconstructed sec electron #eta spectrum", 100, -5., 5., recEl2EtaVsInclMulti);  

      //fill inclusive histograms
      for (unsigned int i = 0; i < isorecjets.size()+1; ++i){
        recZPtVsInclMulti[i]->Fill((*zrecHandle)[0].pt(), weight);
        recZEtaVsInclMulti[i]->Fill((*zrecHandle)[0].eta(), weight);
        recEl1PtVsInclMulti[i]->Fill(dau0->pt(), weight);
        recEl2PtVsInclMulti[i]->Fill(dau1->pt(), weight);
        recEl1EtaVsInclMulti[i]->Fill(dau0->pt(), weight);
        recEl2EtaVsInclMulti[i]->Fill(dau1->pt(), weight);
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
      recEl1PtVsExclMulti[isorecjets.size()]->Fill(dau0->pt(), weight);
      recEl1EtaVsExclMulti[isorecjets.size()]->Fill(dau0->eta(), weight);
      recEl2PtVsExclMulti[isorecjets.size()]->Fill(dau1->pt(), weight);
      recEl2EtaVsExclMulti[isorecjets.size()]->Fill(dau1->eta(), weight);
   
   }
   
   string IsoFlag, EiDFlag;
   if(_selections=="SYM"){
   IsoFlag="_IsoSYM";
   EiDFlag="_EiDSYM";}
   if(_selections=="ASYM"){
   IsoFlag="_IsoASYM";
   EiDFlag="_EiDASYM";}
 
   for(int fcount = 1; fcount<7; fcount++){
   
   if(fcount != 6){
   if(_RecoCutFlags[fcount+1] == IsoFlag.c_str()){   
   bool PreIso = false;
   for(int n = 1; n < fcount+1; n++){
   if(RecSelected(_RecoCutFlags[n].c_str(), (*zrecHandle)[0], *triggerHandle, run)){
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
     
     
     //Barrel
     if(fabs(dau0->eta())<eta_el_excl_down)recRelIso_Barrel_PreIso->Fill( (dau0->trackIso() + dau0->ecalIso() + dau0->hcalIso()) / dau0->pt());
     if(fabs(dau1->eta())<eta_el_excl_down)recRelIso_Barrel_PreIso->Fill( (dau1->trackIso() + dau1->ecalIso() + dau1->hcalIso()) / dau1->pt());
     
     //Endcap
     if(fabs(dau0->eta())>eta_el_excl_up && fabs(dau0->eta()) < etaelcut)recRelIso_Endcap_PreIso->Fill( (dau0->trackIso() + dau0->ecalIso() + dau0->hcalIso()) / dau0->pt());
     if(fabs(dau1->eta())>eta_el_excl_up && fabs(dau1->eta()) < etaelcut)recRelIso_Endcap_PreIso->Fill( (dau1->trackIso() + dau1->ecalIso() + dau1->hcalIso()) / dau1->pt());     
     }
     
     }
     }
     
   if(fcount != 6){
   if(_RecoCutFlags[fcount+1] == EiDFlag.c_str()){   
   bool PreEiD = false;
   for(int n = 1; n < fcount+1; n++){
   if(RecSelected(_RecoCutFlags[n].c_str(), (*zrecHandle)[0], *triggerHandle, run)){
   PreEiD = true;
   }else{
   PreEiD = false;
   n = fcount+1;
   }
   } 
   
   if(PreEiD){  
     //Barrel
     if(fabs(dau0->eta())<eta_el_excl_down){
     HoverE_Barrel_PreEiD->Fill(dau0->hcalOverEcal());
     DeltaEtaIn_Barrel_PreEiD->Fill(dau0->deltaEtaSuperClusterTrackAtVtx());
     DeltaPhiIn_Barrel_PreEiD->Fill(dau0->deltaPhiSuperClusterTrackAtVtx());
     SigmaIEtaIEta_Barrel_PreEiD->Fill(dau0->sigmaIetaIeta());}   
     if(fabs(dau1->eta())<eta_el_excl_down){
     HoverE_Barrel_PreEiD->Fill(dau1->hcalOverEcal());    
     DeltaEtaIn_Barrel_PreEiD->Fill(dau1->deltaEtaSuperClusterTrackAtVtx());    
     DeltaPhiIn_Barrel_PreEiD->Fill(dau1->deltaPhiSuperClusterTrackAtVtx());   
     SigmaIEtaIEta_Barrel_PreEiD->Fill(dau1->sigmaIetaIeta());}
     //Endcap
     if(fabs(dau0->eta())>eta_el_excl_up && fabs(dau0->eta()) < etaelcut){
     HoverE_Endcap_PreEiD->Fill(dau0->hcalOverEcal());
     DeltaEtaIn_Endcap_PreEiD->Fill(dau0->deltaEtaSuperClusterTrackAtVtx());
     DeltaPhiIn_Endcap_PreEiD->Fill(dau0->deltaPhiSuperClusterTrackAtVtx());
     SigmaIEtaIEta_Endcap_PreEiD->Fill(dau0->sigmaIetaIeta());}
     if(fabs(dau1->eta())>eta_el_excl_up && fabs(dau1->eta()) < etaelcut){   
     HoverE_Endcap_PreEiD->Fill(dau1->hcalOverEcal());    
     DeltaEtaIn_Endcap_PreEiD->Fill(dau1->deltaEtaSuperClusterTrackAtVtx());    
     DeltaPhiIn_Endcap_PreEiD->Fill(dau1->deltaPhiSuperClusterTrackAtVtx());    
     SigmaIEtaIEta_Endcap_PreEiD->Fill(dau1->sigmaIetaIeta());}  
     
     }   
   }
   }

} 

   }
   
}


void RecoElectron::finalize(){
   
   _histoVector.insert(_histoVector.end(), recJetPtVsInclMulti.begin(), recJetPtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recJetEtaVsInclMulti.begin(), recJetEtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recZPtVsInclMulti.begin(), recZPtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recZEtaVsInclMulti.begin(), recZEtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recEl1PtVsInclMulti.begin(), recEl1PtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recEl1EtaVsInclMulti.begin(), recEl1EtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recEl2PtVsInclMulti.begin(), recEl2PtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recEl2EtaVsInclMulti.begin(), recEl2EtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recZPtVsExclMulti.begin(), recZPtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recZEtaVsExclMulti.begin(), recZEtaVsExclMulti.end());

   _histoVector.insert(_histoVector.end(), recEl1PtVsExclMulti.begin(), recEl1PtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recEl1EtaVsExclMulti.begin(), recEl1EtaVsExclMulti.end());

   _histoVector.insert(_histoVector.end(), recEl2PtVsExclMulti.begin(), recEl2PtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recEl2EtaVsExclMulti.begin(), recEl2EtaVsExclMulti.end());

   _histoVector.insert(_histoVector.end(), ElCorrChargePtExclMulti.begin(), ElCorrChargePtExclMulti.end());
   _histoVector.insert(_histoVector.end(), ElCorrChargeEtaExclMulti.begin(), ElCorrChargeEtaExclMulti.end());
   _histoVector.insert(_histoVector.end(), ElCorrChargeHitExclMulti.begin(), ElCorrChargeHitExclMulti.end());
   _histoVector.insert(_histoVector.end(), ElCorrChargefBremExclMulti.begin(), ElCorrChargefBremExclMulti.end());

   _histoVector.insert(_histoVector.end(), ElMisIDChargePtExclMulti.begin(), ElMisIDChargePtExclMulti.end());
   _histoVector.insert(_histoVector.end(), ElMisIDChargeEtaExclMulti.begin(), ElMisIDChargeEtaExclMulti.end());
   _histoVector.insert(_histoVector.end(), ElMisIDChargeHitExclMulti.begin(), ElMisIDChargeHitExclMulti.end());
   _histoVector.insert(_histoVector.end(), ElMisIDChargefBremExclMulti.begin(), ElMisIDChargefBremExclMulti.end());

   _histoVector.insert(_histoVector.end(), AllElPtExclMulti.begin(), AllElPtExclMulti.end());
   _histoVector.insert(_histoVector.end(), AllElEtaExclMulti.begin(), AllElEtaExclMulti.end());
   _histoVector.insert(_histoVector.end(), AllElHitExclMulti.begin(), AllElHitExclMulti.end());
   _histoVector.insert(_histoVector.end(), AllElfBremExclMulti.begin(), AllElfBremExclMulti.end());
   
   
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
   if(_selections=="SYM")Report<<"EiD applied = "<<eID_SYM.c_str()<<endl;
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
   static map<std::string, std::pair<int, int> > TrgVector = elTrigger();
   static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
   int trg = 1;
   for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
   if(_TrgList[trg]==1)Report<<"ElectronTrigger = "<<TrgVectorIter->first.c_str()<<endl;
   trg++;
   }
   Report<<endl;
   
   if(_sample=="data"){
   Report<<"Run range required for each trigger (DATA sample):"<<endl<<endl;
   static map<std::string, std::pair<int, int> > TrgVector = elTrigger();
   static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
   for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
   Report<<"ElectronTrigger = "<<TrgVectorIter->first.c_str()<<"		Run range = "<<TrgVectorIter->second.first<<" to "<<TrgVectorIter->second.second<<endl;}
   }
   
   if(elTrgMatchReq==true){
   Report<<"Trigger Match Required"<<endl<<endl;
   }else{
   Report<<"Trigger Match NOT Required"<<endl<<endl;}
   
   if(_selections=="SYM"){
   Report<<"ptelcut = "<<ptelcut<<endl;
   Report<<"etaelcut = "<<etaelcut<<endl;
   Report<<"eta_el_excl_up = "<<eta_el_excl_up<<endl;
   Report<<"eta_el_excl_down = "<<eta_el_excl_down<<endl;
   Report<<"zmassmin_sym = "<<zmassmin_sym<<endl;
   Report<<"zmassmax_sym = "<<zmassmax_sym<<endl;
   Report<<"dxycut = "<<dxycut<<endl;
   Report<<"ptjetmin = "<<ptjetmin<<endl;
   Report<<"etajetmax = "<<etajetmax<<endl;
   Report<<"isocut = "<<sym_isocut<<endl;
   if(JetIDReq==true){
   Report<<endl<<"Jet ID Required"<<endl;
   }else{
   Report<<endl<<"Jet ID NOT Required"<<endl;}
   Report<<"isojetcut = "<<isojetcut<<endl;
 
   Report<<"SYM_TAG_ptelcut = "<<SYM_TAG_ptelcut<<endl;
   Report<<"SYM_TAG_etaelcut = "<<SYM_TAG_etaelcut<<endl;
   Report<<"SYM_TAG_eta_el_excl_up = "<<SYM_TAG_eta_el_excl_up<<endl;
   Report<<"SYM_TAG_eta_el_excl_down = "<<SYM_TAG_eta_el_excl_down<<endl;
   Report<<"SYM_TAG_minnhit = "<<SYM_TAG_minnhit<<endl;
   Report<<"SYM_TAG_maxchi2 = "<<SYM_TAG_maxchi2<<endl;
   Report<<"SYM_TAG_dxycut = "<<SYM_TAG_dxycut<<endl;
   Report<<"SYM_TAG_isocut = "<<SYM_TAG_isocut<<endl;
   Report<<"SYM_TagEiD = "<<SYM_TagEiD.c_str()<<endl;
   }
   
   if(_selections=="ASYM"){
   Report<<"ptelcut0 = "<<ptelcut0<<endl;
   Report<<"ptelcut1 = "<<ptelcut1<<endl;
   Report<<"etaelcut = "<<etaelcut<<endl;
   Report<<"eta_el_excl_up = "<<eta_el_excl_up<<endl;
   Report<<"eta_el_excl_down = "<<eta_el_excl_down<<endl;
   Report<<"zmassmin_asym = "<<zmassmin_asym<<endl;
   Report<<"zmassmax_asym = "<<zmassmax_asym<<endl<<endl;
   Report<<"eID_ASYM0 = "<<eID_ASYM0.c_str()<<endl;
   Report<<"eID_ASYM1 = "<<eID_ASYM0.c_str()<<endl<<endl;
   Report<<"dxycut = "<<dxycut<<endl<<endl;
   Report<<"ptjetmin = "<<ptjetmin<<endl;
   Report<<"etajetmax = "<<etajetmax<<endl;
   if(JetIDReq==true){
   Report<<endl<<"Jet ID Required"<<endl;
   }else{
   Report<<endl<<"Jet ID NOT Required"<<endl;}
   Report<<"isojetcut = "<<isojetcut<<endl<<endl;
   
   Report<<"ASYM0_TAG_ptelcut = "<<ASYM0_TAG_ptelcut<<endl;
   Report<<"ASYM0_TAG_etaelcut = "<<ASYM0_TAG_etaelcut<<endl;
   Report<<"ASYM0_TAG_eta_el_excl_up = "<<ASYM0_TAG_eta_el_excl_up<<endl;
   Report<<"ASYM0_TAG_eta_el_excl_down = "<<ASYM0_TAG_eta_el_excl_down<<endl;
   Report<<"ASYM0_TAG_minnhit = "<<ASYM0_TAG_minnhit<<endl;
   Report<<"ASYM0_TAG_maxchi2 = "<<ASYM0_TAG_maxchi2<<endl;
   Report<<"ASYM0_TAG_dxycut = "<<ASYM0_TAG_dxycut<<endl;
   Report<<"ASYM0_TAG_isocut = "<<ASYM0_TAG_isocut<<endl;
   Report<<"ASYM0_TagEiD = "<<ASYM0_TagEiD.c_str()<<endl<<endl;
   Report<<"ASYM1_TAG_ptelcut = "<<ASYM1_TAG_ptelcut<<endl;
   Report<<"ASYM1_TAG_etaelcut = "<<ASYM1_TAG_etaelcut<<endl;
   Report<<"ASYM1_TAG_eta_el_excl_up = "<<ASYM1_TAG_eta_el_excl_up<<endl;
   Report<<"ASYM1_TAG_eta_el_excl_down = "<<ASYM1_TAG_eta_el_excl_down<<endl;
   Report<<"ASYM1_TAG_minnhit = "<<ASYM1_TAG_minnhit<<endl;
   Report<<"ASYM1_TAG_maxchi2 = "<<ASYM1_TAG_maxchi2<<endl;
   Report<<"ASYM1_TAG_dxycut = "<<ASYM1_TAG_dxycut<<endl;
   Report<<"ASYM1_TAG_isocut = "<<ASYM1_TAG_isocut<<endl; 
   Report<<"ASYM1_TagEiD = "<<ASYM1_TagEiD.c_str()<<endl;
   }
   
   Report.close();

  _file->Write();

}
