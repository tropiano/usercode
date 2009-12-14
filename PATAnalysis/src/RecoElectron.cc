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

recPtZ_Acc(0), recEtaZ_Acc(0), recMassZ_Acc(0), 
recPtZ_Acc_Qual(0), recEtaZ_Acc_Qual(0), recMassZ_Acc_Qual(0),
recPtZ_Acc_Qual_Imp(0), recEtaZ_Acc_Qual_Imp(0), recMassZ_Acc_Qual_Imp(0),
recPtZ_Acc_Qual_Imp_Iso(0), recEtaZ_Acc_Qual_Imp_Iso(0), recMassZ_Acc_Qual_Imp_Iso(0),
recPtZ_Acc_Qual_Imp_Iso_EiD(0), recEtaZ_Acc_Qual_Imp_Iso_EiD(0), recMassZ_Acc_Qual_Imp_Iso_EiD(0),
recTrackIsoLead_Acc_Qual_Imp(0), recEcalIsoLead_Acc_Qual_Imp(0), recHcalIsoLead_Acc_Qual_Imp(0), recRelIsoLead_Acc_Qual_Imp(0), recTrackIsoSec_Acc_Qual_Imp(0), recEcalIsoSec_Acc_Qual_Imp(0), recHcalIsoSec_Acc_Qual_Imp(0), recRelIsoSec_Acc_Qual_Imp(0), 
recLeadIsoJetPt_Acc_Qual_Imp_Iso_EiD(0), recLeadIsoJetEta_Acc_Qual_Imp_Iso_EiD(0),   RecoIsoJetPt_Acc_Qual_Imp_Iso_EiD(0), RecoJetPt_Acc_Qual_Imp_Iso_EiD(0), JetCounter_Acc_Qual_Imp_Iso_EiD(0),
IsoJetCounter_Acc(0), IsoJetCounter_Acc_Qual(0), IsoJetCounter_Acc_Qual_Imp(0), IsoJetCounter_Acc_Qual_Imp_Iso(0), IsoJetCounter_Acc_Qual_Imp_Iso_EiD(0),
HoverE_Acc_Qual_Imp_Iso(0), DeltaEtaIn_Acc_Qual_Imp_Iso(0), DeltaPhiIn_Acc_Qual_Imp_Iso(0), SigmaIEtaIEta_Acc_Qual_Imp_Iso(0), HoverE_Acc_Qual_Imp_Iso_EiD(0), DeltaEtaIn_Acc_Qual_Imp_Iso_EiD(0), DeltaPhiIn_Acc_Qual_Imp_Iso_EiD(0), SigmaIEtaIEta_Acc_Qual_Imp_Iso_EiD(0), 
recLeadElPt_Acc(0), recSecElPt_Acc(0), recLeadElEta_Acc(0), recSecElEta_Acc(0),
recLeadElPt_Acc_Qual(0), recSecElPt_Acc_Qual(0), recLeadElEta_Acc_Qual(0), recSecElEta_Acc_Qual(0),
recLeadElPt_Acc_Qual_Imp(0), recSecElPt_Acc_Qual_Imp(0), recLeadElEta_Acc_Qual_Imp(0), recSecElEta_Acc_Qual_Imp(0), 
recLeadElPt_Acc_Qual_Imp_Iso(0), recSecElPt_Acc_Qual_Imp_Iso(0), recLeadElEta_Acc_Qual_Imp_Iso(0), recSecElEta_Acc_Qual_Imp_Iso(0),
recLeadElPt_Acc_Qual_Imp_Iso_EiD(0), recSecElPt_Acc_Qual_Imp_Iso_EiD(0), recLeadElEta_Acc_Qual_Imp_Iso_EiD(0), recSecElEta_Acc_Qual_Imp_Iso_EiD(0),

DeltaRvsCharge_JetRec(0), DeltaRvsCharge_JetRec_Iso(0), DeltaRvsCharge_JetRec_NotIso(0),
HEnergy_IsoJet_ElType(0), EMEnergy_IsoJet_ElType(0), Jet_EMEnergy(0), Jet_HEnergy(0), MinDeltaR_ZDau(0), AllJetCharge(0), IsoJetCharge(0), NotIsoJetCharge(0), DeltaR_IsoJet(0), DeltaR_NotIsoJet(0), DeltaR_IsoJet_ElType(0),

_norm(1.), _dir(0), _Norm(true), _electronID("eidRobustTight"), _file(0), _histoVector()

{ }

void RecoElectron::begin(TFile* out, const edm::ParameterSet& iConfig){
   _file = out; 
  
    std::string dirname = iConfig.getParameter<std::string>("Name");
    std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
    _electronID = iConfig.getParameter<std::string>("electronID");
   _targetLumi= iConfig.getParameter<double>("targetLumi");
   _xsec      = iConfig.getParameter<double>("CrossSection");
   _Norm      = iConfig.getParameter<bool>("Norm");

   cout << "RecoElectron file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();
   
   //Z variables
   recPtZ_Acc   = new TH1D("recPtZ_Acc", "Reconstructed Z p_{T}, Selections: Acc", 200, 0, 200);
   _histoVector.push_back(recPtZ_Acc);
   recEtaZ_Acc  = new TH1D("recEtaZ_Acc", "Reconstructed Z #eta, Selections: Acc", 100, -10, 10);
   _histoVector.push_back(recEtaZ_Acc); 
   recMassZ_Acc = new TH1D("recMassZ_Acc", "Reconstructed Z mass, Selections: Acc", 200, 50, 150);
   _histoVector.push_back(recMassZ_Acc);
   recPtZ_Acc_Qual   = new TH1D("recPtZ_Acc_Qual", "Reconstructed Z p_{T}, Selections: Acc+Qual", 200, 0, 200);
   _histoVector.push_back(recPtZ_Acc_Qual);
   recEtaZ_Acc_Qual  = new TH1D("recEtaZ_Acc_Qual", "Reconstructed Z #eta, Selections: Acc+Qual", 100, -10, 10);
   _histoVector.push_back(recEtaZ_Acc_Qual); 
   recMassZ_Acc_Qual = new TH1D("recMassZ_Acc_Qual", "Reconstructed Z mass, Selections: Acc+Qual", 200, 50, 150);
   _histoVector.push_back(recMassZ_Acc_Qual);
   recPtZ_Acc_Qual_Imp  = new TH1D("recPtZ_Acc_Qual_Imp", "Reconstructed Z p_{T}, Selections: Acc+Qual+Imp", 200, 0, 200);
   _histoVector.push_back(recPtZ_Acc_Qual_Imp);
   recEtaZ_Acc_Qual_Imp  = new TH1D("recEtaZ_Acc_Qual_Imp", "Reconstructed Z #eta, Selections: Acc+Qual+Imp", 100, -10, 10);
   _histoVector.push_back(recEtaZ_Acc_Qual_Imp); 
   recMassZ_Acc_Qual_Imp = new TH1D("recMassZ_Acc_Qual_Imp", "Reconstructed Z mass, Selections: Acc+Qual+Imp", 200, 50, 150);
   _histoVector.push_back(recMassZ_Acc_Qual_Imp);
   recPtZ_Acc_Qual_Imp_Iso  = new TH1D("recPtZ_Acc_Qual_Imp_Iso", "Reconstructed Z p_{T}, Selections: Acc+Qual+Imp+Iso", 200, 0, 200);
   _histoVector.push_back(recPtZ_Acc_Qual_Imp_Iso);
   recEtaZ_Acc_Qual_Imp_Iso  = new TH1D("recEtaZ_Acc_Qual_Imp_Iso", "Reconstructed Z #eta, Selections: Acc+Qual+Imp+Iso", 100, -10, 10);
   _histoVector.push_back(recEtaZ_Acc_Qual_Imp_Iso); 
   recMassZ_Acc_Qual_Imp_Iso = new TH1D("recMassZ_Acc_Qual_Imp_Iso", "Reconstructed Z mass, Selections: Acc+Qual+Imp+Iso", 200, 50, 150);
   _histoVector.push_back(recMassZ_Acc_Qual_Imp_Iso);
   recPtZ_Acc_Qual_Imp_Iso_EiD  = new TH1D("recPtZ_Acc_Qual_Imp_Iso_EiD", "Reconstructed Z p_{T}, Selections: Acc+Qual+Imp+Iso+EiD", 200, 0, 200);
   _histoVector.push_back(recPtZ_Acc_Qual_Imp_Iso_EiD);
   recEtaZ_Acc_Qual_Imp_Iso_EiD  = new TH1D("recEtaZ_Acc_Qual_Imp_Iso_EiD", "Reconstructed Z #eta, Selections: Acc+Qual+Imp+Iso+EiD", 100, -10, 10);
   _histoVector.push_back(recEtaZ_Acc_Qual_Imp_Iso_EiD); 
   recMassZ_Acc_Qual_Imp_Iso_EiD = new TH1D("recMassZ_Acc_Qual_Imp_Iso_EiD", "Reconstructed Z mass, Selections: Acc+Qual+Imp+Iso+EiD", 200, 50, 150);
   _histoVector.push_back(recMassZ_Acc_Qual_Imp_Iso_EiD);
   
   //Z Electron variables
   recLeadElPt_Acc = new TH1D("recLeadElPt_Acc", "Reconstructed Leading electron p_{T}, Selections = Acc", 200, 0, 200);
   _histoVector.push_back(recLeadElPt_Acc);
   recSecElPt_Acc  = new TH1D("recSecElPt_Acc", "Reconstructed Second electron p_{T}, Selections = Acc", 200, 0, 200);
   _histoVector.push_back(recSecElPt_Acc);
   recLeadElEta_Acc = new TH1D("recLeadElEta_Acc", "Reconstructed Leading electron #eta, Selections = Acc", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEta_Acc);
   recSecElEta_Acc  = new TH1D("recSecElEta_Acc", "Reconstructed Second electron #eta, Selections = Acc", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEta_Acc);
   recLeadElPt_Acc_Qual = new TH1D("recLeadElPt_Acc_Qual", "Reconstructed Leading electron p_{T}, Selections = Acc+Qual", 200, 0, 200);
   _histoVector.push_back(recLeadElPt_Acc_Qual);
   recSecElPt_Acc_Qual  = new TH1D("recSecElPt_Acc_Qual", "Reconstructed Second electron p_{T}, Selections = Acc+Qual", 200, 0, 200);
   _histoVector.push_back(recSecElPt_Acc_Qual);
   recLeadElEta_Acc_Qual = new TH1D("recLeadElEta_Acc_Qual", "Reconstructed Leading electron #eta, Selections = Acc+Qual", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEta_Acc_Qual);
   recSecElEta_Acc_Qual  = new TH1D("recSecElEta_Acc_Qual", "Reconstructed Second electron #eta, Selections = Acc+Qual", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEta_Acc_Qual); 
   recLeadElPt_Acc_Qual_Imp = new TH1D("recLeadElPt_Acc_Qual_Imp", "Reconstructed Leading electron p_{T}, Selections = Acc+Qual+Imp", 200, 0, 200);
   _histoVector.push_back(recLeadElPt_Acc_Qual_Imp);
   recSecElPt_Acc_Qual_Imp  = new TH1D("recSecElPt_Acc_Qual_Imp", "Reconstructed Second electron p_{T}, Selections = Acc+Qual+Imp", 200, 0, 200);
   _histoVector.push_back(recSecElPt_Acc_Qual_Imp);
   recLeadElEta_Acc_Qual_Imp = new TH1D("recLeadElEta_Acc_Qual_Imp", "Reconstructed Leading electron #eta, Selections = Acc+Qual+Imp", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEta_Acc_Qual_Imp);
   recSecElEta_Acc_Qual_Imp  = new TH1D("recSecElEta_Acc_Qual_Imp", "Reconstructed Second electron #eta, Selections = Acc+Qual+Imp", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEta_Acc_Qual_Imp); 
   recLeadElPt_Acc_Qual_Imp_Iso = new TH1D("recLeadElPt_Acc_Qual_Imp_Iso", "Reconstructed Leading electron p_{T}, Selections = Acc+Qual+Imp+Iso", 200, 0, 200);
   _histoVector.push_back(recLeadElPt_Acc_Qual_Imp_Iso);
   recSecElPt_Acc_Qual_Imp_Iso  = new TH1D("recSecElPt_Acc_Qual_Imp_Iso", "Reconstructed Second electron p_{T}, Selections = Acc+Qual+Imp+Iso", 200, 0, 200);
   _histoVector.push_back(recSecElPt_Acc_Qual_Imp_Iso);
   recLeadElEta_Acc_Qual_Imp_Iso = new TH1D("recLeadElEta_Acc_Qual_Imp_Iso", "Reconstructed Leading electron #eta, Selections = Acc+Qual+Imp+Iso", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEta_Acc_Qual_Imp_Iso);
   recSecElEta_Acc_Qual_Imp_Iso  = new TH1D("recSecElEta_Acc_Qual_Imp_Iso", "Reconstructed Second electron #eta, Selections = Acc+Qual+Imp+Iso", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEta_Acc_Qual_Imp_Iso);  
   recLeadElPt_Acc_Qual_Imp_Iso_EiD = new TH1D("recLeadElPt_Acc_Qual_Imp_Iso_EiD", "Reconstructed Leading electron p_{T}, Selections = Acc+Qual+Imp+Iso+Eid", 200, 0, 200);
   _histoVector.push_back(recLeadElPt_Acc_Qual_Imp_Iso_EiD);
   recSecElPt_Acc_Qual_Imp_Iso_EiD  = new TH1D("recSecElPt_Acc_Qual_Imp_Iso_EiD", "Reconstructed Second electron p_{T}, Selections = Acc+Qual+Imp+Iso+Eid", 200, 0, 200);
   _histoVector.push_back(recSecElPt_Acc_Qual_Imp_Iso_EiD);
   recLeadElEta_Acc_Qual_Imp_Iso_EiD = new TH1D("recLeadElEta_Acc_Qual_Imp_Iso_EiD", "Reconstructed Leading electron #eta, Selections = Acc+Qual+Imp+Iso+Eid", 100, -2.5, 2.5);
   _histoVector.push_back(recLeadElEta_Acc_Qual_Imp_Iso_EiD);
   recSecElEta_Acc_Qual_Imp_Iso_EiD  = new TH1D("recSecElEta_Acc_Qual_Imp_Iso_EiD", "Reconstructed Second electron #eta, Selections = Acc+Qual+Imp+Iso+Eid", 100, -2.5, 2.5);
   _histoVector.push_back(recSecElEta_Acc_Qual_Imp_Iso_EiD);
   
   //Jet variables
   recLeadIsoJetPt_Acc_Qual_Imp_Iso_EiD = new TH1D("recLeadIsoJetPt_Acc_Qual_Imp_Iso_EiD", "Reconstructed Leading Iso Jet p_{T}, Selections: Acc+Qual+Imp+Iso+EiD", 200, 0, 200);
   _histoVector.push_back(recLeadIsoJetPt_Acc_Qual_Imp_Iso_EiD);
   recLeadIsoJetEta_Acc_Qual_Imp_Iso_EiD = new TH1D("recLeadIsoJetEta_Acc_Qual_Imp_Iso_EiD", "Reconstructed Leading Iso Jet #eta, Selections: Acc+Qual+Imp+Iso+EiD", 100, -5, 5); 
   _histoVector.push_back(recLeadIsoJetEta_Acc_Qual_Imp_Iso_EiD);
   RecoIsoJetPt_Acc_Qual_Imp_Iso_EiD = new TH1D("RecoIsoJetPt_Acc_Qual_Imp_Iso_EiD", "Reconstructed Iso Jet p_{T}, Selections: Acc+Qual+Imp+Iso+EiD", 250, 0, 250);
   _histoVector.push_back(RecoIsoJetPt_Acc_Qual_Imp_Iso_EiD);
   RecoJetPt_Acc_Qual_Imp_Iso_EiD = new TH1D("RecoJetPt_Acc_Qual_Imp_Iso_EiD", "Reconstructed Jet p_{T}, Selections: Acc+Qual+Imp+Iso+EiD", 250, 0, 250);
   _histoVector.push_back(RecoJetPt_Acc_Qual_Imp_Iso_EiD);
   JetCounter_Acc_Qual_Imp_Iso_EiD = new TH1D("JetCounter_Acc_Qual_Imp_Iso_EiD", "Number of Reconstructed Jet per event, Selections: Acc+Qual+Imp+Iso+EiD", 10, 0, 10);
   _histoVector.push_back(JetCounter_Acc_Qual_Imp_Iso_EiD);
   IsoJetCounter_Acc = new TH1D("IsoJetCounter_Acc", "Number of Reconstructed Iso Jet per event, Selections: Acc", 10, 0, 10);
   _histoVector.push_back(IsoJetCounter_Acc);
   IsoJetCounter_Acc_Qual = new TH1D("IsoJetCounter_Acc_Qual", "Number of Reconstructed Iso Jet per event, Selections: Acc+Qual", 10, 0, 10);
   _histoVector.push_back(IsoJetCounter_Acc_Qual);
   IsoJetCounter_Acc_Qual_Imp = new TH1D("IsoJetCounter_Acc_Qual_Imp", "Number of Reconstructed Iso Jet per event, Selections: Acc+Qual+Imp", 10, 0, 10);
   _histoVector.push_back(IsoJetCounter_Acc_Qual_Imp);
   IsoJetCounter_Acc_Qual_Imp_Iso = new TH1D("IsoJetCounter_Acc_Qual_Imp_Iso", "Number of Reconstructed Iso Jet per event, Selections: Acc+Qual+Imp+Iso", 10, 0, 10);
   _histoVector.push_back(IsoJetCounter_Acc_Qual_Imp_Iso);
   IsoJetCounter_Acc_Qual_Imp_Iso_EiD = new TH1D("IsoJetCounter_Acc_Qual_Imp_Iso_EiD", "Number of Reconstructed Iso Jet per event, Selections: Acc+Qual+Imp+Iso+EiD", 10, 0, 10);
   _histoVector.push_back(IsoJetCounter_Acc_Qual_Imp_Iso_EiD);  
   HoverE_Acc_Qual_Imp_Iso = new TH1D("HoverE_Acc_Qual_Imp_Iso", "Electron Hcal Over Ecal, Selections: Acc+Qual+Imp+Iso", 100, 0, 0.1);
   _histoVector.push_back(HoverE_Acc_Qual_Imp_Iso);
   DeltaEtaIn_Acc_Qual_Imp_Iso = new TH1D("DeltaEtaIn_Acc_Qual_Imp_Iso", "Electron Delta Eta In, Selections: Acc+Qual+Imp+Iso", 100, -0.5, 0.05);
   _histoVector.push_back(DeltaEtaIn_Acc_Qual_Imp_Iso);
   DeltaPhiIn_Acc_Qual_Imp_Iso = new TH1D("DeltaPhiIn_Acc_Qual_Imp_Iso", "Electron Delta Phi In, Selections: Acc+Qual+Imp+Iso", 100, -0.5, 0.5);
   _histoVector.push_back(DeltaPhiIn_Acc_Qual_Imp_Iso);
   SigmaIEtaIEta_Acc_Qual_Imp_Iso= new TH1D("SigmaIEtaIEta_Acc_Qual_Imp_Iso", "Electron SigmaIEtaIEta, Selections: Acc+Qual+Imp+Iso", 100, 0, 0.1);
   _histoVector.push_back(SigmaIEtaIEta_Acc_Qual_Imp_Iso);  
   HoverE_Acc_Qual_Imp_Iso_EiD = new TH1D("HoverE_Acc_Qual_Imp_Iso_EiD", "Electron Hcal Over Ecal, Selections: Acc+Qual+Imp+Iso+EiD", 100, 0, 0.1);
   _histoVector.push_back(HoverE_Acc_Qual_Imp_Iso_EiD);
   DeltaEtaIn_Acc_Qual_Imp_Iso_EiD = new TH1D("DeltaEtaIn_Acc_Qual_Imp_Iso_EiD", "Electron Delta Eta In, Selections: Acc+Qual+Imp+Iso+EiD", 100, -0.5, 0.05);
   _histoVector.push_back(DeltaEtaIn_Acc_Qual_Imp_Iso_EiD);
   DeltaPhiIn_Acc_Qual_Imp_Iso_EiD = new TH1D("DeltaPhiIn_Acc_Qual_Imp_Iso_EiD", "Electron Delta Phi In, Selections: Acc+Qual+Imp+Iso+EiD", 100, -0.5, 0.5);
   _histoVector.push_back(DeltaPhiIn_Acc_Qual_Imp_Iso_EiD);
   SigmaIEtaIEta_Acc_Qual_Imp_Iso_EiD= new TH1D("SigmaIEtaIEta_Acc_Qual_Imp_Iso_EiD", "Electron SigmaIEtaIEta, Selections: Acc+Qual+Imp+Iso+EiD", 100, 0, 0.1);
   _histoVector.push_back(SigmaIEtaIEta_Acc_Qual_Imp_Iso_EiD);
   
   //Z Electrons Isolation variables
   recTrackIsoLead_Acc_Qual_Imp = new TH1D("recTrackIsoLead_Acc_Qual_Imp", "Lead Track Isolation SumPt, Selections: Acc+Qual+Imp", 3000, 0, 30);
   _histoVector.push_back(recTrackIsoLead_Acc_Qual_Imp);
   recEcalIsoLead_Acc_Qual_Imp  = new TH1D("recEcalIsoLead_Acc_Qual_Imp", "Lead ECAL Isolation SumPt, Selections: Acc+Qual+Imp", 3000, 0, 30);
   _histoVector.push_back(recEcalIsoLead_Acc_Qual_Imp);
   recHcalIsoLead_Acc_Qual_Imp  = new TH1D("recHcalIsoLead_Acc_Qual_Imp", "Lead HCAL Isolation SumPt, Selections: Acc+Qual+Imp", 3000, 0, 30);
   _histoVector.push_back(recHcalIsoLead_Acc_Qual_Imp);
   recRelIsoLead_Acc_Qual_Imp   = new TH1D("recRelIsoLead_Acc_Qual_Imp", "Lead Comb Relative Isolation, Selections: Acc+Qual+Imp", 3000, 0, 30);
   _histoVector.push_back(recRelIsoLead_Acc_Qual_Imp);
   recTrackIsoSec_Acc_Qual_Imp = new TH1D("recTrackIsoSec_Acc_Qual_Imp", "Sec Track Isolation SumPt, Selections: Acc+Qual+Imp", 3000, 0, 30);
   _histoVector.push_back(recTrackIsoSec_Acc_Qual_Imp);
   recEcalIsoSec_Acc_Qual_Imp  = new TH1D("recEcalIsoSec_Acc_Qual_Imp", "Sec ECAL Isolation SumPt, Selections: Acc+Qual+Imp", 3000, 0, 30);
   _histoVector.push_back(recEcalIsoSec_Acc_Qual_Imp);
   recHcalIsoSec_Acc_Qual_Imp  = new TH1D("recHcalIsoSec_Acc_Qual_Imp", "Sec HCAL Isolation SumPt, Selections: Acc+Qual+Imp", 3000, 0, 30); 
   _histoVector.push_back(recHcalIsoSec_Acc_Qual_Imp);
   recRelIsoSec_Acc_Qual_Imp   = new TH1D("recRelIsoSec_Acc_Qual_Imp", "Sec Comb Relative Isolation, Selections: Acc+Qual+Imp", 3000, 0, 30);
   _histoVector.push_back(recRelIsoSec_Acc_Qual_Imp);
   
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
   
  TChain *ch = new TChain("Events");
  
  ifstream infile;
  infile.open(sourceFileList.c_str());
  string datafile;
  while(getline (infile, datafile)){
    ch->Add(datafile.c_str());
  }
  
  cout<<"RecoElectron analyzing nr. event = "<<ch->GetEntries()<<endl;
  
  _entries = ch->GetEntries();
  
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
     if (RecSelected_Acc(*zrecHandle)){
     
     //Z variables
     recPtZ_Acc->Fill((*zrecHandle)[0].pt());
     recEtaZ_Acc->Fill((*zrecHandle)[0].eta());
     recMassZ_Acc->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_Acc->Fill(dau0->eta());
     recSecElEta_Acc->Fill(dau1->eta());
     recLeadElPt_Acc->Fill(dau0->pt());
     recSecElPt_Acc->Fill(dau1->pt());
        
     //Jet variables
     IsoJetCounter_Acc->Fill(isorecjets.size());
      
     }
     
     //Events with a selected Zee Acc+Qual
     if (RecSelected_Acc_Qual(*zrecHandle)){
     
     //Z variables
     recPtZ_Acc_Qual->Fill((*zrecHandle)[0].pt());
     recEtaZ_Acc_Qual->Fill((*zrecHandle)[0].eta());
     recMassZ_Acc_Qual->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_Acc_Qual->Fill(dau0->eta());
     recSecElEta_Acc_Qual->Fill(dau1->eta());
     recLeadElPt_Acc_Qual->Fill(dau0->pt());
     recSecElPt_Acc_Qual->Fill(dau1->pt());
        
     //Jet variables
     IsoJetCounter_Acc_Qual->Fill(isorecjets.size());
     
     }
     
     //Events with a selected Zee Acc+Qual+Imp
     if (RecSelected_Acc_Qual_Imp(*zrecHandle)){
     
     //Z variables
     recPtZ_Acc_Qual_Imp->Fill((*zrecHandle)[0].pt());
     recEtaZ_Acc_Qual_Imp->Fill((*zrecHandle)[0].eta());
     recMassZ_Acc_Qual_Imp->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_Acc_Qual_Imp->Fill(dau0->eta());
     recSecElEta_Acc_Qual_Imp->Fill(dau1->eta());
     recLeadElPt_Acc_Qual_Imp->Fill(dau0->pt());
     recSecElPt_Acc_Qual_Imp->Fill(dau1->pt());
     
     recTrackIsoLead_Acc_Qual_Imp->Fill(dau0->trackIso());
     recEcalIsoLead_Acc_Qual_Imp->Fill(dau0->ecalIso());
     recHcalIsoLead_Acc_Qual_Imp->Fill(dau0->hcalIso());
     recRelIsoLead_Acc_Qual_Imp->Fill( (dau0->trackIso() + dau0->ecalIso() + dau0->hcalIso()) / dau0->pt());
     recTrackIsoSec_Acc_Qual_Imp->Fill(dau1->trackIso());
     recEcalIsoSec_Acc_Qual_Imp->Fill(dau1->ecalIso());
     recHcalIsoSec_Acc_Qual_Imp->Fill(dau1->hcalIso());
     recRelIsoSec_Acc_Qual_Imp->Fill( (dau1->trackIso() + dau1->ecalIso() + dau1->hcalIso()) / dau1->pt());
         
     //Jet variables
     IsoJetCounter_Acc_Qual_Imp->Fill(isorecjets.size());
     
     }
     
     //Events with a selected Zee Acc+Qual+Imp+Iso
     if (RecSelected_Acc_Qual_Imp_Iso(*zrecHandle)){
     
     //Z variables
     recPtZ_Acc_Qual_Imp_Iso->Fill((*zrecHandle)[0].pt());
     recEtaZ_Acc_Qual_Imp_Iso->Fill((*zrecHandle)[0].eta());
     recMassZ_Acc_Qual_Imp_Iso->Fill((*zrecHandle)[0].mass());
     
     //Z Electrons variables
     recLeadElEta_Acc_Qual_Imp_Iso->Fill(dau0->eta());
     recSecElEta_Acc_Qual_Imp_Iso->Fill(dau1->eta());
     recLeadElPt_Acc_Qual_Imp_Iso->Fill(dau0->pt());
     recSecElPt_Acc_Qual_Imp_Iso->Fill(dau1->pt());
     
     HoverE_Acc_Qual_Imp_Iso->Fill(dau0->hcalOverEcal());
     HoverE_Acc_Qual_Imp_Iso->Fill(dau1->hcalOverEcal());
     DeltaEtaIn_Acc_Qual_Imp_Iso->Fill(dau0->deltaEtaSuperClusterTrackAtVtx());
     DeltaEtaIn_Acc_Qual_Imp_Iso->Fill(dau1->deltaEtaSuperClusterTrackAtVtx());
     DeltaPhiIn_Acc_Qual_Imp_Iso->Fill(dau0->deltaPhiSuperClusterTrackAtVtx());
     DeltaPhiIn_Acc_Qual_Imp_Iso->Fill(dau1->deltaPhiSuperClusterTrackAtVtx());
     SigmaIEtaIEta_Acc_Qual_Imp_Iso->Fill(dau0->sigmaIetaIeta());
     SigmaIEtaIEta_Acc_Qual_Imp_Iso->Fill(dau1->sigmaIetaIeta());
         
     //Jet variables
     IsoJetCounter_Acc_Qual_Imp_Iso->Fill(isorecjets.size());
          
    }
    
     //Events with a selected Zee Acc+Qual+Iso+Eid
     if (RecSelected_Acc_Qual_Imp_Iso_EiD(*zrecHandle, _electronID.c_str())){
     
      //Z variables     
      recPtZ_Acc_Qual_Imp_Iso_EiD->Fill((*zrecHandle)[0].pt());
      recEtaZ_Acc_Qual_Imp_Iso_EiD->Fill((*zrecHandle)[0].eta());
      recMassZ_Acc_Qual_Imp_Iso_EiD->Fill((*zrecHandle)[0].mass());

      //Z Electrons variables      
      recLeadElEta_Acc_Qual_Imp_Iso_EiD->Fill(dau0->eta());
      recSecElEta_Acc_Qual_Imp_Iso_EiD->Fill(dau1->eta());
      recLeadElPt_Acc_Qual_Imp_Iso_EiD->Fill(dau0->pt());
      recSecElPt_Acc_Qual_Imp_Iso_EiD->Fill(dau1->pt());
   
      //Electron ID variables
      HoverE_Acc_Qual_Imp_Iso_EiD->Fill(dau0->hcalOverEcal());
      HoverE_Acc_Qual_Imp_Iso_EiD->Fill(dau1->hcalOverEcal());
      DeltaEtaIn_Acc_Qual_Imp_Iso_EiD->Fill(dau0->deltaEtaSuperClusterTrackAtVtx());
      DeltaEtaIn_Acc_Qual_Imp_Iso_EiD->Fill(dau1->deltaEtaSuperClusterTrackAtVtx());
      DeltaPhiIn_Acc_Qual_Imp_Iso_EiD->Fill(dau0->deltaPhiSuperClusterTrackAtVtx());
      DeltaPhiIn_Acc_Qual_Imp_Iso_EiD->Fill(dau1->deltaPhiSuperClusterTrackAtVtx());
      SigmaIEtaIEta_Acc_Qual_Imp_Iso_EiD->Fill(dau0->sigmaIetaIeta());
      SigmaIEtaIEta_Acc_Qual_Imp_Iso_EiD->Fill(dau1->sigmaIetaIeta());
           
           
      //Jet variables     
      JetCounter_Acc_Qual_Imp_Iso_EiD->Fill(recjets.size());
      IsoJetCounter_Acc_Qual_Imp_Iso_EiD->Fill(isorecjets.size());
    
      for(unsigned int i = 0; i < recjets.size(); i++){
      RecoJetPt_Acc_Qual_Imp_Iso_EiD->Fill(recjets[i]->pt());}
      
      for(unsigned int i = 0; i < isorecjets.size(); i++){
      RecoIsoJetPt_Acc_Qual_Imp_Iso_EiD->Fill(isorecjets[i]->pt());}
      
      if (isorecjets.size()){
        recLeadIsoJetPt_Acc_Qual_Imp_Iso_EiD->Fill(isorecjets[0]->pt());
        recLeadIsoJetEta_Acc_Qual_Imp_Iso_EiD->Fill(isorecjets[0]->eta());
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

   if(_Norm){
   _norm = _targetLumi/lumi;
   }else{
   _norm = 1.;
   }
  
   for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
     if (*i) (*i)->Scale(_norm);
   }

   
  _file->Write();

}
