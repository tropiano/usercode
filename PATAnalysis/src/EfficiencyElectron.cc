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
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/TrackReco/interface/Track.h"


using namespace std;
using namespace edm;


EfficiencyElectron::EfficiencyElectron(): 

genMassZEff_Acc(0), genPtZEff_Acc(0), genEtaZEff_Acc(0),
genMassZEff_Acc_Qual(0), genPtZEff_Acc_Qual(0), genEtaZEff_Acc_Qual(0),
genMassZEff_Acc_Qual_Imp(0), genPtZEff_Acc_Qual_Imp(0), genEtaZEff_Acc_Qual_Imp(0),
genMassZEff_Acc_Qual_Imp_Iso(0), genPtZEff_Acc_Qual_Imp_Iso(0), genEtaZEff_Acc_Qual_Imp_Iso(0),
genMassZEff_Acc_Qual_Imp_Iso_EiD(0), genPtZEff_Acc_Qual_Imp_Iso_EiD(0), genEtaZEff_Acc_Qual_Imp_Iso_EiD(0),
GenIsoJetEff_Acc(0), GenIsoJetEff_Acc_Qual(0), GenIsoJetEff_Acc_Qual_Imp(0), GenIsoJetEff_Acc_Qual_Imp_Iso(0), GenIsoJetEff_Acc_Qual_Imp_Iso_EiD(0),

genLeadElEtaEff_Acc(0), genLeadElEtaEff_Acc_Qual(0), genLeadElEtaEff_Acc_Qual_Imp(0), genLeadElEtaEff_Acc_Qual_Imp_Iso(0), genLeadElEtaEff_Acc_Qual_Imp_Iso_EiD(0),
genLeadElPtEff_Acc(0), genLeadElPtEff_Acc_Qual(0), genLeadElPtEff_Acc_Qual_Imp(0), genLeadElPtEff_Acc_Qual_Imp_Iso(0), genLeadElPtEff_Acc_Qual_Imp_Iso_EiD(0),

AccDenom_genPtZ(0), AccDenom_genMassZ(0), AccDenom_genEtaZ(0), AccDenom_genLeadElEta(0), AccDenom_genLeadElPt(0), AccDenom_GenIsoJetNumber(0),
EffDenom_genPtZ(0), EffDenom_genMassZ(0), EffDenom_genEtaZ(0), EffDenom_genLeadElEta(0), EffDenom_genLeadElPt(0), EffDenom_GenIsoJetNumber(0),

_dir(0), _Sumw2(false), _electronID("eidRobustTight"), _file(0), _histoVector()

{ }

void EfficiencyElectron::begin(TFile* out, const edm::ParameterSet& iConfig){
   _file = out; 
  
    std::string dirname = iConfig.getParameter<std::string>("Name");
    std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
    _electronID = iConfig.getParameter<std::string>("electronID");
   _Sumw2     = iConfig.getParameter<bool>("Sumw2");

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
   genLeadElEtaEff_Acc = new TH1D("genLeadElEtaEff_Acc", "Generated Leading electron #eta, Selections = Acc", 100, -2.5, 2.5);
   _histoVector.push_back(genLeadElEtaEff_Acc);
   genLeadElEtaEff_Acc_Qual = new TH1D("genLeadElEtaEff_Acc_Qual", "Generated Leading electron #eta, Selections = Acc+Qual", 100, -2.5, 2.5);
   _histoVector.push_back(genLeadElEtaEff_Acc_Qual);
   genLeadElEtaEff_Acc_Qual_Imp = new TH1D("genLeadElEtaEff_Acc_Qual_Imp", "Generated Leading electron #eta, Selections = Acc+Qual+Imp", 100, -2.5, 2.5);
   _histoVector.push_back(genLeadElEtaEff_Acc_Qual_Imp);
   genLeadElEtaEff_Acc_Qual_Imp_Iso = new TH1D("genLeadElEtaEff_Acc_Qual_Imp_Iso", "Generated Leading electron #eta, Selections = Acc+Qual+Imp+Iso", 100, -2.5, 2.5);
   _histoVector.push_back(genLeadElEtaEff_Acc_Qual_Imp_Iso);
   genLeadElEtaEff_Acc_Qual_Imp_Iso_EiD = new TH1D("genLeadElEtaEff_Acc_Qual_Imp_Iso_EiD", "Generated Leading electron #eta, Selections = Acc+Qual+Imp+Iso+EiD", 100, -2.5, 2.5);
   _histoVector.push_back(genLeadElEtaEff_Acc_Qual_Imp_Iso_EiD);   
   genLeadElPtEff_Acc = new TH1D("genLeadElPtEff_Acc", "Generated Leading electron Pt, Selections = Acc", 200, 0, 200);
   _histoVector.push_back(genLeadElPtEff_Acc);
   genLeadElPtEff_Acc_Qual = new TH1D("genLeadElPtEff_Acc_Qual", "Generated Leading electron Pt, Selections = Acc+Qual", 200, 0, 200);
   _histoVector.push_back(genLeadElPtEff_Acc_Qual);
   genLeadElPtEff_Acc_Qual_Imp = new TH1D("genLeadElPtEff_Acc_Qual_Imp", "Generated Leading electron Pt, Selections = Acc+Qual+Imp", 200, 0, 200);
   _histoVector.push_back(genLeadElPtEff_Acc_Qual_Imp);
   genLeadElPtEff_Acc_Qual_Imp_Iso = new TH1D("genLeadElPtEff_Acc_Qual_Imp_Iso", "Generated Leading electron Pt, Selections = Acc+Qual+Imp+Iso", 200, 0, 200);
   _histoVector.push_back(genLeadElPtEff_Acc_Qual_Imp_Iso);
   genLeadElPtEff_Acc_Qual_Imp_Iso_EiD = new TH1D("genLeadElPtEff_Acc_Qual_Imp_Iso_EiD", "Generated Leading electron Pt, Selections = Acc+Qual+Imp+Iso+EiD", 200, 0, 200);
   _histoVector.push_back(genLeadElPtEff_Acc_Qual_Imp_Iso_EiD);
   
   //Eff vs Z properties
   genMassZEff_Acc = new TH1D("genMassZEff_Acc", "Generated Z Mass, Selections = Acc", 200, 50, 150);
   _histoVector.push_back(genMassZEff_Acc);
   genPtZEff_Acc = new TH1D("genPtZEff_Acc", "Generated Z pt, Selections = Acc",  200, 0, 200);
   _histoVector.push_back(genPtZEff_Acc);
   genEtaZEff_Acc = new TH1D("genEtaZEff_Acc", "Generated Z #eta, Selections = Acc",  100, -10, 10);
   _histoVector.push_back(genEtaZEff_Acc);
   genMassZEff_Acc_Qual = new TH1D("genMassZEff_Acc_Qual", "Generated Z Mass, Selections = Acc+Qual", 200, 50, 150);
   _histoVector.push_back(genMassZEff_Acc_Qual);
   genPtZEff_Acc_Qual = new TH1D("genPtZEff_Acc_Qual", "Generated Z pt, Selections = Acc+Qual",  200, 0, 200);
   _histoVector.push_back(genPtZEff_Acc_Qual);
   genEtaZEff_Acc_Qual = new TH1D("genEtaZEff_Acc_Qual", "Generated Z #eta, Selections = Acc+Qual",  100, -10, 10);
   _histoVector.push_back(genEtaZEff_Acc_Qual);
   genMassZEff_Acc_Qual_Imp = new TH1D("genMassZEff_Acc_Qual_Imp", "Generated Z Mass, Selections = Acc+Qual+Imp", 200, 50, 150);
   _histoVector.push_back(genMassZEff_Acc_Qual_Imp);
   genPtZEff_Acc_Qual_Imp = new TH1D("genPtZEff_Acc_Qual_Imp", "Generated Z pt, Selections = Acc+Qual+Imp",  200, 0, 200);
   _histoVector.push_back(genPtZEff_Acc_Qual_Imp);
   genEtaZEff_Acc_Qual_Imp = new TH1D("genEtaZEff_Acc_Qual_Imp", "Generated Z #eta, Selections = Acc+Qual+Imp",  100, -10, 10);
   _histoVector.push_back(genEtaZEff_Acc_Qual_Imp);
   genMassZEff_Acc_Qual_Imp_Iso = new TH1D("genMassZEff_Acc_Qual_Imp_Iso", "Generated Z Mass, Selections = Acc+Qual+Imp+Iso", 200, 50, 150);
   _histoVector.push_back(genMassZEff_Acc_Qual_Imp_Iso);
   genPtZEff_Acc_Qual_Imp_Iso = new TH1D("genPtZEff_Acc_Qual_Imp_Iso", "Generated Z pt, Selections = Acc+Qual+Imp+Iso",  200, 0, 200);
   _histoVector.push_back(genPtZEff_Acc_Qual_Imp_Iso);
   genEtaZEff_Acc_Qual_Imp_Iso = new TH1D("genEtaZEff_Acc_Qual_Imp_Iso", "Generated Z #eta, Selections = Acc+Qual+Imp+Iso",  100, -10, 10);
   _histoVector.push_back(genEtaZEff_Acc_Qual_Imp_Iso);
   genMassZEff_Acc_Qual_Imp_Iso_EiD = new TH1D("genMassZEff_Acc_Qual_Imp_Iso_EiD", "Generated Z Mass, Selections = Acc+Qual+Imp+Iso+EiD", 200, 50, 150);
   _histoVector.push_back(genMassZEff_Acc_Qual_Imp_Iso_EiD);
   genPtZEff_Acc_Qual_Imp_Iso_EiD = new TH1D("genPtZEff_Acc_Qual_Imp_Iso_EiD", "Generated Z pt, Selections = Acc+Qual+Imp+Iso+EiD",  200, 0, 200);
   _histoVector.push_back(genPtZEff_Acc_Qual_Imp_Iso_EiD);
   genEtaZEff_Acc_Qual_Imp_Iso_EiD = new TH1D("genEtaZEff_Acc_Qual_Imp_Iso_EiD", "Generated Z #eta, Selections = Acc+Qual+Imp+Iso+EiD",  100, -10, 10);
   _histoVector.push_back(genEtaZEff_Acc_Qual_Imp_Iso_EiD);
   
   //Eff vs Jet properties
   GenIsoJetEff_Acc = new TH1D("GenIsoJetEff_Acc", "Number of Gen Iso Jet, Sel: Acc", 10, 0, 10);
   _histoVector.push_back(GenIsoJetEff_Acc);
   GenIsoJetEff_Acc_Qual = new TH1D("GenIsoJetEff_Acc_Qual", "Number of Gen Iso Jet, Sel: Acc+Qual", 10, 0, 10);
   _histoVector.push_back(GenIsoJetEff_Acc_Qual);
   GenIsoJetEff_Acc_Qual_Imp = new TH1D("GenIsoJetEff_Acc_Qual_Imp", "Number of Gen Iso Jet, Sel: Acc+Qual+Imp", 10, 0, 10);
   _histoVector.push_back(GenIsoJetEff_Acc_Qual_Imp);
   GenIsoJetEff_Acc_Qual_Imp_Iso = new TH1D("GenIsoJetEff_Acc_Qual_Imp_Iso", "Number of Gen Iso Jet, Sel: Acc+Qual+Imp+Iso", 10, 0, 10);
   _histoVector.push_back(GenIsoJetEff_Acc_Qual_Imp_Iso);
   GenIsoJetEff_Acc_Qual_Imp_Iso_EiD = new TH1D("GenIsoJetEff_Acc_Qual_Imp_Iso_EiD", "Number of Gen Iso Jet, Sel: Acc+Qual+Imp+Iso+EiD", 10, 0, 10);
   _histoVector.push_back(GenIsoJetEff_Acc_Qual_Imp_Iso_EiD);
 
   std::vector<TH1D*>::const_iterator i1beg = _histoVector.begin(); 
   std::vector<TH1D*>::const_iterator i1end = _histoVector.end(); 

   if(_Sumw2){
   for (std::vector<TH1D*>::const_iterator i = i1beg; i != i1end; ++i){
    (*i)->Sumw2();
   }
   } 
   
  cout << "EfficiencyElectron Worker built." << endl;   
}

EfficiencyElectron::~EfficiencyElectron(){
  _file->ls();
}

void  EfficiencyElectron::process(const fwlite::Event& iEvent)
{

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
   std::vector<const pat::Electron*> zdaughters = ZDaughters(*zrecHandle);
   const pat::Electron *dau0, *dau1;
   
   //Z Gen daughters
   std::vector<const reco::Candidate*> zgendaughters = ZGENDaughters(*zgenHandle);
   const reco::Candidate *gendau0, *gendau1;
   double genleadelpt, genleadeleta, gensecondelpt, gensecondeleta;
   
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
     
     //Reco jets
     std::vector<const pat::Jet*> recjets = GetJets<pat::Jet>(*jetrecHandle);
     std::vector<const pat::Jet*> isojets;
     std::vector<const pat::Jet*> notisojets;
      
     for(unsigned int i = 0; i < recjets.size(); i++){
     if(RecoIsoJet(*zrecHandle,*recjets[i]))isojets.push_back(recjets[i]);
     if(!RecoIsoJet(*zrecHandle,*recjets[i]))notisojets.push_back(recjets[i]);
     }
     
     if(zdaughters.size() != 0){   
     
     dau0 = zdaughters[0];
     dau1 = zdaughters[1];
     
     //Pre selection events
     
     
     
     
     
     }
    
     //Acceptance denominator
     if (GenSelected(*zgenHandle)&&zgendaughters.size()!=0){
     
     AccDenom_genPtZ->Fill((*zgenHandle)[0].pt());
     AccDenom_genEtaZ->Fill((*zgenHandle)[0].eta());
     AccDenom_genMassZ->Fill((*zgenHandle)[0].mass());
     AccDenom_genLeadElEta->Fill(genleadeleta);
     AccDenom_genLeadElPt->Fill(genleadelpt);
     AccDenom_GenIsoJetNumber->Fill(isogenjets.size());
     
     }
     
     //Efficiency denominator
     if (GenSelectedInAcceptance(*zgenHandle)&&zgendaughters.size()!=0){
     
     EffDenom_genPtZ->Fill((*zgenHandle)[0].pt());
     EffDenom_genEtaZ->Fill((*zgenHandle)[0].eta());
     EffDenom_genMassZ->Fill((*zgenHandle)[0].mass());
     EffDenom_genLeadElEta->Fill(genleadeleta);
     EffDenom_genLeadElPt->Fill(genleadelpt);
     EffDenom_GenIsoJetNumber->Fill(isogenjets.size());
      
     }
     
     //Events with a selected Zee Acc
     if (GenSelectedInAcceptance(*zgenHandle)&&RecSelected_Acc(*zrecHandle)){
     
     //Eff vs Z variables
     genMassZEff_Acc->Fill((*zgenHandle)[0].mass());
     genPtZEff_Acc->Fill((*zgenHandle)[0].pt());
     genEtaZEff_Acc->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_Acc->Fill(genleadeleta);
     genLeadElPtEff_Acc->Fill(genleadelpt);
     
     //Eff vs Jet variables
     GenIsoJetEff_Acc->Fill(isogenjets.size());
     
     }
     
     //Events with a selected Zee Acc+Qual
     if (GenSelectedInAcceptance(*zgenHandle)&&RecSelected_Acc_Qual(*zrecHandle)){
     
     //Eff vs Z variables
     genMassZEff_Acc_Qual->Fill((*zgenHandle)[0].mass());
     genPtZEff_Acc_Qual->Fill((*zgenHandle)[0].pt());
     genEtaZEff_Acc_Qual->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_Acc_Qual->Fill(genleadeleta);
     genLeadElPtEff_Acc_Qual->Fill(genleadelpt);
     
     //Eff vs Jet variables
     GenIsoJetEff_Acc_Qual->Fill(isogenjets.size());
        
     }
     
     //Events with a selected Zee Acc+Qual+Imp
     if (GenSelectedInAcceptance(*zgenHandle)&&RecSelected_Acc_Qual_Imp(*zrecHandle)){
     
     //Eff vs Z variables
     genMassZEff_Acc_Qual_Imp->Fill((*zgenHandle)[0].mass());
     genPtZEff_Acc_Qual_Imp->Fill((*zgenHandle)[0].pt());
     genEtaZEff_Acc_Qual_Imp->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_Acc_Qual_Imp->Fill(genleadeleta);
     genLeadElPtEff_Acc_Qual_Imp->Fill(genleadelpt);
     
     //Eff vs Jet variables
     GenIsoJetEff_Acc_Qual_Imp->Fill(isogenjets.size());
     
     
     }
     
     //Events with a selected Zee Acc+Qual+Imp+Iso
     if (GenSelectedInAcceptance(*zgenHandle)&&RecSelected_Acc_Qual_Imp_Iso(*zrecHandle)){
     
     //Eff vs Z variables
     genMassZEff_Acc_Qual_Imp_Iso->Fill((*zgenHandle)[0].mass());
     genPtZEff_Acc_Qual_Imp_Iso->Fill((*zgenHandle)[0].pt());
     genEtaZEff_Acc_Qual_Imp_Iso->Fill((*zgenHandle)[0].eta());
     
     //Eff vs Z Electrons variables
     genLeadElEtaEff_Acc_Qual_Imp_Iso->Fill(genleadeleta);
     genLeadElPtEff_Acc_Qual_Imp_Iso->Fill(genleadelpt);
     
     //Eff vs Jet variables
     GenIsoJetEff_Acc_Qual_Imp_Iso->Fill(isogenjets.size());
     
     }
     
     //Events with a selected Zee Acc+Qual+Iso+EiD
     if (GenSelectedInAcceptance(*zgenHandle)&&RecSelected_Acc_Qual_Imp_Iso_EiD(*zrecHandle, _electronID.c_str())){
      
      //Eff vs Z variables
      genMassZEff_Acc_Qual_Imp_Iso_EiD->Fill((*zgenHandle)[0].mass());
      genPtZEff_Acc_Qual_Imp_Iso_EiD->Fill((*zgenHandle)[0].pt());
      genEtaZEff_Acc_Qual_Imp_Iso_EiD->Fill((*zgenHandle)[0].eta());
      
      //Eff vs Z Electrons variables
      genLeadElEtaEff_Acc_Qual_Imp_Iso_EiD->Fill(genleadeleta);
      genLeadElPtEff_Acc_Qual_Imp_Iso_EiD->Fill(genleadelpt);
      
      //Eff vs Jet variables
      GenIsoJetEff_Acc_Qual_Imp_Iso_EiD->Fill(isogenjets.size());
      
}
 
}

void EfficiencyElectron::finalize(){
   
  _file->Write();
  
}
