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

genMassZEff_1(0), genPtZEff_1(0), genEtaZEff_1(0),
genMassZEff_12(0), genPtZEff_12(0), genEtaZEff_12(0),
genMassZEff_123(0), genPtZEff_123(0), genEtaZEff_123(0),
genMassZEff_1234(0), genPtZEff_1234(0), genEtaZEff_1234(0),
genMassZEff_12345(0), genPtZEff_12345(0), genEtaZEff_12345(0),
GenIsoJetEff_1(0), GenIsoJetEff_12(0), GenIsoJetEff_123(0), GenIsoJetEff_1234(0), GenIsoJetEff_12345(0),

genLeadElEtaEff_1(0), genLeadElEtaEff_12(0), genLeadElEtaEff_123(0), genLeadElEtaEff_1234(0), genLeadElEtaEff_12345(0),
genLeadElPtEff_1(0), genLeadElPtEff_12(0), genLeadElPtEff_123(0), genLeadElPtEff_1234(0), genLeadElPtEff_12345(0),

AccDenom_genPtZ(0), AccDenom_genMassZ(0), AccDenom_genEtaZ(0), AccDenom_genLeadElEta(0), AccDenom_genLeadElPt(0), AccDenom_GenIsoJetNumber(0),
EffDenom_genPtZ(0), EffDenom_genMassZ(0), EffDenom_genEtaZ(0), EffDenom_genLeadElEta(0), EffDenom_genLeadElPt(0), EffDenom_GenIsoJetNumber(0),

_dir(0), _electronID("eidRobustTight"), _file(0), _Acc(1), _Qual(2), _Imp(3), _Iso(4), _EiD(5), _histoVector()

{ }

void EfficiencyElectron::begin(TFile* out, const edm::ParameterSet& iConfig){
   _file = out; 
  
    std::string dirname = iConfig.getParameter<std::string>("Name");
    std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
    _electronID = iConfig.getParameter<std::string>("electronID");
    
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
   
   //Eff vs Jet properties
   string GenIsoJetEff_name = "GenIsoJetEff";
   GenIsoJetEff_name+=_RecoCutFlags[1].c_str();
   GenIsoJetEff_1 = new TH1D(GenIsoJetEff_name.c_str(), "Number of Gen Iso Jet", 10, 0, 10);
   _histoVector.push_back(GenIsoJetEff_1);
   GenIsoJetEff_name+=_RecoCutFlags[2].c_str();
   GenIsoJetEff_12 = new TH1D(GenIsoJetEff_name.c_str(), "Number of Gen Iso Jet", 10, 0, 10);
   _histoVector.push_back(GenIsoJetEff_12);
   GenIsoJetEff_name+=_RecoCutFlags[3].c_str();
   GenIsoJetEff_123 = new TH1D(GenIsoJetEff_name.c_str(), "Number of Gen Iso Jet", 10, 0, 10);
   _histoVector.push_back(GenIsoJetEff_123);
   GenIsoJetEff_name+=_RecoCutFlags[4].c_str();
   GenIsoJetEff_1234 = new TH1D(GenIsoJetEff_name.c_str(), "Number of Gen Iso Jet", 10, 0, 10);
   _histoVector.push_back(GenIsoJetEff_1234);
   GenIsoJetEff_name+=_RecoCutFlags[5].c_str();
   GenIsoJetEff_12345 = new TH1D(GenIsoJetEff_name.c_str(), "Number of Gen Iso Jet", 10, 0, 10);
   _histoVector.push_back(GenIsoJetEff_12345);
 
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
   
   //// Only first Z considered (higher pt of leading electron)
   int ZNum = 0;
   
   if(zrecHandle->size()){
   
   //Z Reco daughters
   std::vector<const pat::Electron*> zdaughters = ZDaughters((*zrecHandle)[ZNum]);
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
     if(RecoIsoJet((*zrecHandle)[ZNum],*recjets[i]))isojets.push_back(recjets[i]);
     if(!RecoIsoJet((*zrecHandle)[ZNum],*recjets[i]))notisojets.push_back(recjets[i]);
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
     
     //Events with a selected Zee 1
     if (GenSelectedInAcceptance(*zgenHandle)&&RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])){
     
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
     if (GenSelectedInAcceptance(*zgenHandle)&&RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])&&RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])){
     
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
     if (GenSelectedInAcceptance(*zgenHandle)&&RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])&&RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])&&RecSelected(_RecoCutFlags[3].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])){
     
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
     if (GenSelectedInAcceptance(*zgenHandle)&&RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])&&RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])&&RecSelected(_RecoCutFlags[3].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])&&RecSelected(_RecoCutFlags[4].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])){
     
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
     if (GenSelectedInAcceptance(*zgenHandle)&&RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])&&RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])&&RecSelected(_RecoCutFlags[3].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])&&RecSelected(_RecoCutFlags[4].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])&&RecSelected(_RecoCutFlags[5].c_str(), _electronID.c_str(), (*zrecHandle)[ZNum])){
      
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

}
 
}

void EfficiencyElectron::finalize(){
   
  _file->Write();
  
}
