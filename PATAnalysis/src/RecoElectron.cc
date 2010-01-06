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
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

using namespace std;
using namespace edm;


RecoElectron::RecoElectron():

recPtZ_1(0), recEtaZ_1(0), recMassZ_1(0), 
recPtZ_12(0), recEtaZ_12(0), recMassZ_12(0),
recPtZ_123(0), recEtaZ_123(0), recMassZ_123(0),
recPtZ_1234(0), recEtaZ_1234(0), recMassZ_1234(0),
recPtZ_12345(0), recEtaZ_12345(0), recMassZ_12345(0),

recLeadIsoJetPt_12345(0), recLeadIsoJetEta_12345(0),   RecoIsoJetPt_12345(0), RecoJetPt_12345(0), JetCounter_12345(0),
IsoJetCounter_1(0), IsoJetCounter_12(0), IsoJetCounter_123(0), IsoJetCounter_1234(0), IsoJetCounter_12345(0),
HoverE_PreEiD(0), DeltaEtaIn_PreEiD(0), DeltaPhiIn_PreEiD(0), SigmaIEtaIEta_PreEiD(0), HoverE_EiD(0), DeltaEtaIn_EiD(0), DeltaPhiIn_EiD(0), SigmaIEtaIEta_EiD(0), 
recLeadElPt_1(0), recSecElPt_1(0), recLeadElEta_1(0), recSecElEta_1(0),
recLeadElPt_12(0), recSecElPt_12(0), recLeadElEta_12(0), recSecElEta_12(0),
recLeadElPt_123(0), recSecElPt_123(0), recLeadElEta_123(0), recSecElEta_123(0), 
recLeadElPt_1234(0), recSecElPt_1234(0), recLeadElEta_1234(0), recSecElEta_1234(0),
recLeadElPt_12345(0), recSecElPt_12345(0), recLeadElEta_12345(0), recSecElEta_12345(0),

recTrackIsoLead_PreIso(0), recEcalIsoLead_PreIso(0), recHcalIsoLead_PreIso(0), recRelIsoLead_PreIso(0), recTrackIsoSec_PreIso(0), recEcalIsoSec_PreIso(0), recHcalIsoSec_PreIso(0), recRelIsoSec_PreIso(0), 

DeltaRvsCharge_JetRec(0), DeltaRvsCharge_JetRec_Iso(0), DeltaRvsCharge_JetRec_NotIso(0),
HEnergy_IsoJet_ElType(0), EMEnergy_IsoJet_ElType(0), Jet_EMEnergy(0), Jet_HEnergy(0), MinDeltaR_ZDau(0), AllJetCharge(0), IsoJetCharge(0), NotIsoJetCharge(0), DeltaR_IsoJet(0), DeltaR_NotIsoJet(0), DeltaR_IsoJet_ElType(0),

_norm(1.), _dir(0), _Norm(true), _entries(0), _EventsPerFile(0), _electronID("eidRobustTight"), _file(0), _Acc(1), _Qual(2), _Imp(3), _Iso(4), _EiD(5), _histoVector()

{ }

void RecoElectron::begin(TFile* out, const edm::ParameterSet& iConfig){
   _file = out; 
  
    std::string dirname = iConfig.getParameter<std::string>("Name");
    std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
    _electronID = iConfig.getParameter<std::string>("electronID");
   _targetLumi= iConfig.getParameter<double>("targetLumi");
   _xsec      = iConfig.getParameter<double>("CrossSection");
   _Norm      = iConfig.getParameter<bool>("Norm");
   _EventsPerFile    = iConfig.getParameter<double>("EventsPerFile");
   
   //Selections
   _Acc = iConfig.getParameter<double>("Acc");
   _Qual = iConfig.getParameter<double>("Qual");
   _Imp = iConfig.getParameter<double>("Imp");
   _Iso = iConfig.getParameter<double>("Iso");
   _EiD = iConfig.getParameter<double>("EiD");
   
   for(int i=0; i<6; i++){
   _RecoCutFlags[i] = "_1";}
   
   _RecoCutFlags[_Acc] = "_Acc";
   _RecoCutFlags[_Qual] = "_Qual";
   _RecoCutFlags[_Imp] = "_Imp";
   _RecoCutFlags[_Iso] = "_Iso";
   _RecoCutFlags[_EiD] = "_EiD";
   
   cout << "RecoElectron file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();
   
   //Z variables
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
   
   string recMassZ_name = "recMassZ";
   recMassZ_name+=_RecoCutFlags[1].c_str();
   recMassZ_1 = new TH1D(recMassZ_name.c_str(), "Reconstructed Z mass", 200, 50, 150);
   _histoVector.push_back(recMassZ_1);
   recMassZ_name+=_RecoCutFlags[2].c_str();
   recMassZ_12 = new TH1D(recMassZ_name.c_str(), "Reconstructed Z mass", 200, 50, 150);
   _histoVector.push_back(recMassZ_12);
   recMassZ_name+=_RecoCutFlags[3].c_str();
   recMassZ_123 = new TH1D(recMassZ_name.c_str(), "Reconstructed Z mass", 200, 50, 150);
   _histoVector.push_back(recMassZ_123);
   recMassZ_name+=_RecoCutFlags[4].c_str();
   recMassZ_1234 = new TH1D(recMassZ_name.c_str(), "Reconstructed Z mass", 200, 50, 150);
   _histoVector.push_back(recMassZ_1234);  
   recMassZ_name+=_RecoCutFlags[5].c_str();
   recMassZ_12345 = new TH1D(recMassZ_name.c_str(), "Reconstructed Z mass", 200, 50, 150);
   _histoVector.push_back(recMassZ_12345);
   
   //Z Electron variables
   
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
   
   string recSecElEta_name = "recSecElEta";
   recSecElEta_name+=_RecoCutFlags[1].c_str(); 
   recSecElEta_1  = new TH1D(recSecElEta_name.c_str(), "Reconstructed Second electron #eta,", 100, -2.5, 2.5);
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
   
   //Electron identification variable
   
   HoverE_PreEiD = new TH1D("HoverE_PreEiD", "Electron Hcal Over Ecal", 100, 0, 0.1);
   _histoVector.push_back(HoverE_PreEiD);
   DeltaEtaIn_PreEiD = new TH1D("DeltaEtaIn_PreEiD", "Electron Delta Eta In", 100, -0.5, 0.05);
   _histoVector.push_back(DeltaEtaIn_PreEiD);
   DeltaPhiIn_PreEiD = new TH1D("DeltaPhiIn_PreEiD", "Electron Delta Phi In", 100, -0.5, 0.5);
   _histoVector.push_back(DeltaPhiIn_PreEiD);
   SigmaIEtaIEta_PreEiD= new TH1D("SigmaIEtaIEta_PreEiD", "Electron SigmaIEtaIEta", 100, 0, 0.1);
   _histoVector.push_back(SigmaIEtaIEta_PreEiD);
   
   HoverE_EiD = new TH1D("HoverE_EiD", "Electron Hcal Over Ecal", 100, 0, 0.1);
   _histoVector.push_back(HoverE_EiD);
   DeltaEtaIn_EiD = new TH1D("DeltaEtaIn_EiD", "Electron Delta Eta In", 100, -0.5, 0.05);
   _histoVector.push_back(DeltaEtaIn_EiD);
   DeltaPhiIn_EiD = new TH1D("DeltaPhiIn_EiD", "Electron Delta Phi In", 100, -0.5, 0.5);
   _histoVector.push_back(DeltaPhiIn_EiD);
   SigmaIEtaIEta_EiD= new TH1D("SigmaIEtaIEta_EiD", "Electron SigmaIEtaIEta", 100, 0, 0.1);
   _histoVector.push_back(SigmaIEtaIEta_EiD);
   
   //Z Electrons Isolation variables
   recTrackIsoLead_PreIso = new TH1D("recTrackIsoLead_PreIso", "Lead Track Isolation SumPt", 3000, 0, 30);
   _histoVector.push_back(recTrackIsoLead_PreIso);
   recEcalIsoLead_PreIso  = new TH1D("recEcalIsoLead_PreIso", "Lead ECAL Isolation SumPt", 3000, 0, 30);
   _histoVector.push_back(recEcalIsoLead_PreIso);
   recHcalIsoLead_PreIso  = new TH1D("recHcalIsoLead_PreIso", "Lead HCAL Isolation SumPt", 3000, 0, 30);
   _histoVector.push_back(recHcalIsoLead_PreIso);
   recRelIsoLead_PreIso   = new TH1D("recRelIsoLead_PreIso", "Lead Comb Relative Isolation", 3000, 0, 30);
   _histoVector.push_back(recRelIsoLead_PreIso);
   recTrackIsoSec_PreIso = new TH1D("recTrackIsoSec_PreIso", "Sec Track Isolation SumPt", 3000, 0, 30);
   _histoVector.push_back(recTrackIsoSec_PreIso);
   recEcalIsoSec_PreIso  = new TH1D("recEcalIsoSec_PreIso", "Sec ECAL Isolation SumPt", 3000, 0, 30);
   _histoVector.push_back(recEcalIsoSec_PreIso);
   recHcalIsoSec_PreIso  = new TH1D("recHcalIsoSec_PreIso", "Sec HCAL Isolation SumPt", 3000, 0, 30); 
   _histoVector.push_back(recHcalIsoSec_PreIso);
   recRelIsoSec_PreIso   = new TH1D("recRelIsoSec_PreIso", "Sec Comb Relative Isolation", 3000, 0, 30);
   _histoVector.push_back(recRelIsoSec_PreIso);
   
   //Jet variables
   
   string recLeadIsoJetPt_name = "recLeadIsoJetPt";
   string recLeadIsoJetEta_name = "recLeadIsoJetEta";
   string RecoIsoJetPt_name = "RecoIsoJetPt";
   string RecoJetPt_name = "RecoJetPt";
   string JetCounter_name = "JetCounter";
  
   for(int c=1;c!=6;c++){
   recLeadIsoJetPt_name+=_RecoCutFlags[c].c_str();
   recLeadIsoJetEta_name+=_RecoCutFlags[c].c_str();
   RecoIsoJetPt_name+=_RecoCutFlags[c].c_str();
   RecoJetPt_name+=_RecoCutFlags[c].c_str();
   JetCounter_name+=_RecoCutFlags[c].c_str();}
   
   recLeadIsoJetPt_12345 = new TH1D(recLeadIsoJetPt_name.c_str(), "Reconstructed Leading Iso Jet p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadIsoJetPt_12345);
   recLeadIsoJetEta_12345 = new TH1D(recLeadIsoJetEta_name.c_str(), "Reconstructed Leading Iso Jet #eta", 100, -5, 5); 
   _histoVector.push_back(recLeadIsoJetEta_12345);
   RecoIsoJetPt_12345 = new TH1D(RecoIsoJetPt_name.c_str(), "Reconstructed Iso Jet p_{T}", 250, 0, 250);
   _histoVector.push_back(RecoIsoJetPt_12345);
   RecoJetPt_12345 = new TH1D(RecoJetPt_name.c_str(), "Reconstructed Jet p_{T}", 250, 0, 250);
   _histoVector.push_back(RecoJetPt_12345);
   JetCounter_12345 = new TH1D(JetCounter_name.c_str(), "Number of Reconstructed Jet per event", 10, 0, 10);
   _histoVector.push_back(JetCounter_12345);
   
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
   
   //Jet isolation properties
   
   TDirectory *jetiso_dir = _dir->mkdir("JetIsolation");
   jetiso_dir->cd();
   
   DeltaRvsCharge_JetRec = new TH2D("DeltaRvsCharge_JetRec", "Delta R Reco Jets - Reco Z-Electrons", 400, -2, 2, 1000, 0, 6);
   _histoVector2D.push_back(DeltaRvsCharge_JetRec);
   DeltaRvsCharge_JetRec_Iso = new TH2D("DeltaRvsCharge_JetRec_Iso", "Delta R Reco ISO Jets - Reco Z-Electrons", 400, -2, 2, 1000, 0, 6);
   _histoVector2D.push_back(DeltaRvsCharge_JetRec);
   DeltaRvsCharge_JetRec_NotIso = new TH2D("DeltaRvsCharge_JetRec_NotIso", "Delta R Reco NOT ISO Jets - Reco Z-Electrons", 400, -2, 2, 1000, 0, 6);
   _histoVector2D.push_back(DeltaRvsCharge_JetRec);
   Jet_HEnergy = new TH1D("Jet_HEnergy", "Hadronic Jet Energy Fraction", 100, -0.5, 1.5);
   _histoVector.push_back(Jet_HEnergy);
   Jet_EMEnergy = new TH1D("Jet_EMEnergy", "EM Jet Energy Fraction", 100, -0.5, 1.5);
   _histoVector.push_back(Jet_EMEnergy);
   HEnergy_IsoJet_ElType = new TH1D("HEnergy_IsoJet_ElType", "Hadronic Energy Fraction for Iso Jets Electron Type", 100, -0.5, 1.5);
   _histoVector.push_back(HEnergy_IsoJet_ElType);
   EMEnergy_IsoJet_ElType = new TH1D("EMEnergy_IsoJet_ElType", "EM Energy Fraction for Iso Jets Electron Type", 100, -0.5, 1.5);
   _histoVector.push_back(EMEnergy_IsoJet_ElType);  
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
   
   std::vector<TH1D*>::const_iterator ibeg = _histoVector.begin(); 
   std::vector<TH1D*>::const_iterator iend = _histoVector.end();
 
  int fileCounter = 0;
  
  TChain *ch = new TChain("Events");
  ifstream infile;
  infile.open(sourceFileList.c_str());
  string datafile;
  while(getline (infile, datafile)){
    ch->Add(datafile.c_str());
    fileCounter++;
  }
  
  if(_Norm==true){
  _entries = ch->GetEntries();
  cout<<"RecoElectron analyzing nr. file = "<<fileCounter<<endl;
  cout<<"RecoElectron analyzing nr. event = "<<_entries<<endl;}
 
  if(_Norm==false){
  _entries = fileCounter*_EventsPerFile;
  cout<<"RecoElectron analyzing nr. file = "<<fileCounter<<endl;
  cout<<"RecoElectron analyzing nr. event = "<<_entries<<endl;
  }
  
  delete ch; 
   
  cout << "RecoElectron Worker built." << endl;   
}

RecoElectron::~RecoElectron(){
  _file->ls();
}

void  RecoElectron::process(const fwlite::Event& iEvent)
{


   double weight = 1.;
   _file->cd();

   fwlite::Handle<std::vector<pat::Electron> > electronHandle;
   electronHandle.getByLabel(iEvent, "selectedElectrons");

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zrecHandle;
   zrecHandle.getByLabel(iEvent, "zeerec");
  
   fwlite::Handle<std::vector<pat::Jet> > jetrecHandle;
   jetrecHandle.getByLabel(iEvent, "selectedJets");

   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");

   
     //we need to add the piece of code that select the Z candidate in case of multiple candidates
     
     std::vector<const pat::Jet*> recjets = GetJets<pat::Jet>(*jetrecHandle);
     std::vector<const pat::Jet*> isorecjets;
     std::vector<const pat::Jet*> notisorecjets;
      
     for(unsigned int i = 0; i < recjets.size(); i++){     
     if(RecoIsoJet(*zrecHandle,*recjets[i]))isorecjets.push_back(recjets[i]);
     if(!RecoIsoJet(*zrecHandle,*recjets[i]))notisorecjets.push_back(recjets[i]);}
   
     std::vector<const pat::Electron*> zdaughters = ZDaughters(*zrecHandle);
     const pat::Electron *dau0, *dau1;
     
     if(zdaughters.size()){
     
     dau0 = zdaughters[0];
     dau1 = zdaughters[1];
     
     //Pre selections events
     
     
     
     }
     
     //Events with a selected Zee Acc
     if (RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), *zrecHandle)){
     
     //Z variables
     recPtZ_1->Fill((*zrecHandle)[0].pt());
     recEtaZ_1->Fill((*zrecHandle)[0].eta());
     recMassZ_1->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_1->Fill(dau0->eta());
     recSecElEta_1->Fill(dau1->eta());
     recLeadElPt_1->Fill(dau0->pt());
     recSecElPt_1->Fill(dau1->pt());
        
     //Jet variables
     IsoJetCounter_1->Fill(isorecjets.size());
      
     }
     
   
     //Events with a selected Zee Acc+Qual
     if (RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), *zrecHandle) && RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), *zrecHandle)){
    
     //Z variables
     recPtZ_12->Fill((*zrecHandle)[0].pt());
     recEtaZ_12->Fill((*zrecHandle)[0].eta());
     recMassZ_12->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_12->Fill(dau0->eta());
     recSecElEta_12->Fill(dau1->eta());
     recLeadElPt_12->Fill(dau0->pt());
     recSecElPt_12->Fill(dau1->pt());
        
     //Jet variables
     IsoJetCounter_12->Fill(isorecjets.size());
     
     }
     
     //Events with a selected Zee Acc+Qual+Imp
     if (RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), *zrecHandle) && RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), *zrecHandle) && RecSelected(_RecoCutFlags[3].c_str(), _electronID.c_str(), *zrecHandle)){
    
     //Z variables
     recPtZ_123->Fill((*zrecHandle)[0].pt());
     recEtaZ_123->Fill((*zrecHandle)[0].eta());
     recMassZ_123->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_123->Fill(dau0->eta());
     recSecElEta_123->Fill(dau1->eta());
     recLeadElPt_123->Fill(dau0->pt());
     recSecElPt_123->Fill(dau1->pt());
         
     //Jet variables
     IsoJetCounter_123->Fill(isorecjets.size());
     
     }
    
     //Events with a selected Zee Acc+Qual+Imp+Iso
     if (RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), *zrecHandle) && RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), *zrecHandle) && RecSelected(_RecoCutFlags[3].c_str(), _electronID.c_str(), *zrecHandle) && RecSelected(_RecoCutFlags[4].c_str(), _electronID.c_str(), *zrecHandle)){
     
     //Z variables
     recPtZ_1234->Fill((*zrecHandle)[0].pt());
     recEtaZ_1234->Fill((*zrecHandle)[0].eta());
     recMassZ_1234->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_1234->Fill(dau0->eta());
     recSecElEta_1234->Fill(dau1->eta());
     recLeadElPt_1234->Fill(dau0->pt());
     recSecElPt_1234->Fill(dau1->pt());
         
     //Jet variables
     IsoJetCounter_1234->Fill(isorecjets.size());
          
    }
  
     //Events with a selected Zee Acc+Qual+Iso+Eid
     if (RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), *zrecHandle) && RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), *zrecHandle) && RecSelected(_RecoCutFlags[3].c_str(), _electronID.c_str(), *zrecHandle) && RecSelected(_RecoCutFlags[4].c_str(), _electronID.c_str(), *zrecHandle) && RecSelected(_RecoCutFlags[5].c_str(), _electronID.c_str(), *zrecHandle)){
     
      //Z variables     
      recPtZ_12345->Fill((*zrecHandle)[0].pt());
      recEtaZ_12345->Fill((*zrecHandle)[0].eta());
      recMassZ_12345->Fill((*zrecHandle)[0].mass());

      //Z Electrons variables      
      recLeadElEta_12345->Fill(dau0->eta());
      recSecElEta_12345->Fill(dau1->eta());
      recLeadElPt_12345->Fill(dau0->pt());
      recSecElPt_12345->Fill(dau1->pt());
           
      //Jet variables     
      JetCounter_12345->Fill(recjets.size());
      IsoJetCounter_12345->Fill(isorecjets.size());
    
      for(unsigned int i = 0; i < recjets.size(); i++){
      RecoJetPt_12345->Fill(recjets[i]->pt());}
      
      for(unsigned int i = 0; i < isorecjets.size(); i++){
      RecoIsoJetPt_12345->Fill(isorecjets[i]->pt());}
      
      if (isorecjets.size()){
        recLeadIsoJetPt_12345->Fill(isorecjets[0]->pt());
        recLeadIsoJetEta_12345->Fill(isorecjets[0]->eta());
        }
      
      //Jet properties      
      
      //All jets
      if (recjets.size()){      
      for (unsigned int i = 0; i < recjets.size(); ++i){     
          Jet_HEnergy->Fill(recjets[i]->energyFractionHadronic());
          Jet_EMEnergy->Fill(recjets[i]->emEnergyFraction());      
          MinDeltaR_ZDau->Fill(MinDeltaRZDau(*zrecHandle,*recjets[i]));          
          DeltaRvsCharge_JetRec->Fill(recjets[i]->jetCharge(), MinDeltaRZDau(*zrecHandle,*recjets[i]));
          AllJetCharge->Fill(recjets[i]->jetCharge());                   
      }     
      }  
    
      //Isolated jets    
      if(isorecjets.size()){      
      for (unsigned int i = 0; i < isorecjets.size(); ++i){      
        DeltaRvsCharge_JetRec_Iso->Fill(isorecjets[i]->jetCharge(), MinDeltaRZDau(*zrecHandle,*isorecjets[i]));
        IsoJetCharge->Fill(isorecjets[i]->jetCharge());  
        DeltaR_IsoJet->Fill(MinDeltaRZDau(*zrecHandle,*isorecjets[i]));
      
        //Iso Jets electron-type
        if(isorecjets[i]->jetCharge() < -0.98 || isorecjets[i]->jetCharge() > 0.98){        
          HEnergy_IsoJet_ElType->Fill(isorecjets[i]->energyFractionHadronic());
          EMEnergy_IsoJet_ElType->Fill(isorecjets[i]->emEnergyFraction());    
          DeltaR_IsoJet_ElType->Fill(MinDeltaRZDau(*zrecHandle,*isorecjets[i]));          
          }          
          }     
     }
     
     //Not isolated jets
     if(notisorecjets.size()){     
      for (unsigned int i = 0; i < notisorecjets.size(); ++i){      
          NotIsoJetCharge->Fill(notisorecjets[i]->jetCharge());        
          DeltaR_NotIsoJet->Fill(MinDeltaRZDau(*zrecHandle,*notisorecjets[i]));
          DeltaRvsCharge_JetRec_NotIso->Fill(recjets[i]->jetCharge(), MinDeltaRZDau(*zrecHandle,*notisorecjets[i]));        
     }
     }
     
     
      //Exclusive - Inclusive Histograms
      _dir->cd();
      addHistosVsMulti(isorecjets.size(), "recJetPtIncl", " reconstructed jet p_{T} spectrum", 200, 0, 200, recJetPtVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recJetEtaIncl", " reconstructed jet #eta spectrum", 100, -5., 5., recJetEtaVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recZPtIncl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recZEtaIncl", " reconstructed Z #eta spectrum", 100, -5., 5., recZEtaVsInclMulti);
      addHistosVsMulti(isorecjets.size(), "recZPtExcl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsExclMulti);
      addHistosVsMulti(isorecjets.size(), "recZEtaExcl", " reconstructed Z #eta spectrum", 100, -5., 5., recZEtaVsExclMulti);
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
 
   for(int fcount = 1; fcount<6; fcount++){
   
   if(fcount != 5){
   if(_RecoCutFlags[fcount+1] == "_Iso"){   
   bool PreIso = false;
   for(int n = 1; n < fcount+1; n++){
   if(RecSelected(_RecoCutFlags[n].c_str(), _electronID.c_str(), *zrecHandle)){
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
     recRelIsoLead_PreIso->Fill( (dau0->trackIso() + dau0->ecalIso() + dau0->hcalIso()) / dau0->pt());
     recTrackIsoSec_PreIso->Fill(dau1->trackIso());
     recEcalIsoSec_PreIso->Fill(dau1->ecalIso());
     recHcalIsoSec_PreIso->Fill(dau1->hcalIso());
     recRelIsoSec_PreIso->Fill( (dau1->trackIso() + dau1->ecalIso() + dau1->hcalIso()) / dau1->pt());     
     }
     
     }
     }
     
   if(fcount != 5){
   if(_RecoCutFlags[fcount+1] == "_EiD"){   
   bool PreEiD = false;
   for(int n = 1; n < fcount+1; n++){
   if(RecSelected(_RecoCutFlags[n].c_str(), _electronID.c_str(), *zrecHandle)){
   PreEiD = true;
   }else{
   PreEiD = false;
   n = fcount+1;
   }
   }     
   
   if(PreEiD){     
     HoverE_PreEiD->Fill(dau0->hcalOverEcal());
     HoverE_PreEiD->Fill(dau1->hcalOverEcal());
     DeltaEtaIn_PreEiD->Fill(dau0->deltaEtaSuperClusterTrackAtVtx());
     DeltaEtaIn_PreEiD->Fill(dau1->deltaEtaSuperClusterTrackAtVtx());
     DeltaPhiIn_PreEiD->Fill(dau0->deltaPhiSuperClusterTrackAtVtx());
     DeltaPhiIn_PreEiD->Fill(dau1->deltaPhiSuperClusterTrackAtVtx());
     SigmaIEtaIEta_PreEiD->Fill(dau0->sigmaIetaIeta());
     SigmaIEtaIEta_PreEiD->Fill(dau1->sigmaIetaIeta());     
     }
     
   if(PreEiD && RecSelected(_RecoCutFlags[fcount+1].c_str(), _electronID.c_str(), *zrecHandle)){
     HoverE_EiD->Fill(dau0->hcalOverEcal());
     HoverE_EiD->Fill(dau1->hcalOverEcal());
     DeltaEtaIn_EiD->Fill(dau0->deltaEtaSuperClusterTrackAtVtx());
     DeltaEtaIn_EiD->Fill(dau1->deltaEtaSuperClusterTrackAtVtx());
     DeltaPhiIn_EiD->Fill(dau0->deltaPhiSuperClusterTrackAtVtx());
     DeltaPhiIn_EiD->Fill(dau1->deltaPhiSuperClusterTrackAtVtx());
     SigmaIEtaIEta_EiD->Fill(dau0->sigmaIetaIeta());
     SigmaIEtaIEta_EiD->Fill(dau1->sigmaIetaIeta());
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

   std::vector<TH1D*>::const_iterator ibeg = _histoVector.begin();
   std::vector<TH1D*>::const_iterator iend = _histoVector.end();
   
   //Normalization
   
   double lumi = _entries/_xsec;

   if(lumi){
   _norm = _targetLumi/lumi;
   }
  
   for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
     if (*i) (*i)->Scale(_norm);
   }

   
  _file->Write();

}
