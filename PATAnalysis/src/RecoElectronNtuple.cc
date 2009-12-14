#include "Firenze/PATAnalysis/include/RecoElectronNtuple.h"
#include "Firenze/PATAnalysis/include/ElectronUtilities.h"

#include <TH2.h>
#include <TStyle.h>
#include <math.h>

#include "TList.h"
#include "TParameter.h"
#include "TFile.h"
#include "TProofOutputFile.h"
#include "TChain.h"

#include <iostream>
#include <fstream>

//CMSSW headers
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/JetReco/interface/GenJet.h"

using namespace std;
using namespace edm;


RecoElectronNtuple::RecoElectronNtuple(): 
 
_ptjetmin(30.), _etajetmax(3.), _isocut(0.1), _weight(1.)

{ }

void RecoElectronNtuple::begin(TFile* out, const edm::ParameterSet& iConfig){

   _file = out; 
  
    std::string dirname = iConfig.getParameter<std::string>("Name");
    std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
    
   _electronID = iConfig.getParameter<std::string>("electronID");
   _targetLumi= iConfig.getParameter<double>("targetLumi");
   _xsec      = iConfig.getParameter<double>("CrossSection");
   _Norm      = iConfig.getParameter<bool>("Norm");
   _EventsPerFile    = iConfig.getParameter<double>("EventsPerFile");

   cout << "RecoElectronNtuple file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();

   electrontree= new TTree("electrontree","electrontree");
   
   electrontree->Branch("weight", &_weight, "weight/F");
   
   // quantities at generator level 
   electrontree->Branch("ptzgen",&ptzgen,"ptzgen/F");
   electrontree->Branch("etazgen",&etazgen,"etazgen/F");
   electrontree->Branch("phizgen",&phizgen,"phizgen/F");
   electrontree->Branch("zmassgen",&zmassgen,"zmassgen/F");
   electrontree->Branch("elegenpt1",&elegenpt1,"elegenpt1/F");
   electrontree->Branch("elegeneta1",&elegeneta1,"elegeneta1/F");
   electrontree->Branch("elegenphi1",&elegenphi1,"elegenphi1/F");
   electrontree->Branch("elegenpt2",&elegenpt2,"elegenpt2/F");
   electrontree->Branch("elegeneta2",&elegeneta2,"elegeneta2/F");
   electrontree->Branch("elegenphi2",&elegenphi2,"elegenphi2/F");
   electrontree->Branch("gennjetsele",&gennjetsele,"gennjetsele/I");
   electrontree->Branch("acc_ptzgen",&acc_ptzgen,"acc_ptzgen/F");
   electrontree->Branch("acc_etazgen",&acc_etazgen,"acc_etazgen/F");
   electrontree->Branch("acc_phizgen",&acc_phizgen,"acc_phizgen/F");
   electrontree->Branch("acc_zmassgen",&acc_zmassgen,"zmassgen/F");
   electrontree->Branch("acc_elegenpt1",&acc_elegenpt1,"acc_elegenpt1/F");
   electrontree->Branch("acc_elegeneta1",&acc_elegeneta1,"acc_elegeneta1/F");
   electrontree->Branch("acc_elegenphi1",&acc_elegenphi1,"acc_elegenphi1/F");
   electrontree->Branch("acc_elegenpt2",&acc_elegenpt2,"acc_elegenpt2/F");
   electrontree->Branch("acc_elegeneta2",&acc_elegeneta2,"acc_elegeneta2/F");
   electrontree->Branch("acc_elegenphi2",&acc_elegenphi2,"acc_elegenphi2/F");
   electrontree->Branch("acc_gennjetsele",&acc_gennjetsele,"acc_gennjetsele/I");
   electrontree->Branch("acc_jetgenpt1",&acc_jetgenpt1,"acc_jetgenpt1/F");
   electrontree->Branch("acc_jetgeneta1",&acc_jetgeneta1,"acc_jetgeneta1/F");
   electrontree->Branch("acc_jetgenphi1",&acc_jetgenphi1,"acc_jetgenphi1/F");
   electrontree->Branch("acc_jetgencharge1",&acc_jetgencharge1,"acc_jetgencharge1/F");
   electrontree->Branch("acc_jetgenpt2",&acc_jetgenpt2,"acc_jetgenpt2/F");
   electrontree->Branch("acc_jetgeneta2",&acc_jetgeneta2,"acc_jetgeneta2/F");
   electrontree->Branch("acc_jetgenphi2",&acc_jetgenphi2,"acc_jetgenphi2/F");
   electrontree->Branch("acc_jetgencharge2",&acc_jetgencharge2,"acc_jetgencharge2/F");
   electrontree->Branch("acc_jetgenpt3",&acc_jetgenpt3,"acc_jetgenpt3/F");
   electrontree->Branch("acc_jetgeneta3",&acc_jetgeneta3,"acc_jetgeneta3/F");
   electrontree->Branch("acc_jetgenphi3",&acc_jetgenphi3,"acc_jetgenphi3/F");
   electrontree->Branch("acc_jetgencharge3",&acc_jetgencharge3,"acc_jetgencharge3/F");

   // reconstructed quantities
   electrontree->Branch("numberOfZ",&numberOfZ,"numberOfZ/I");
   electrontree->Branch("nelesall",&nelesall,"nelesall/I");
   electrontree->Branch("neles",&neles,"neles/I");
   electrontree->Branch("loosezmass",&loosezmass,"loosezmass/F");
   electrontree->Branch("zmass",&zmass,"zmass/F");
   electrontree->Branch("zpt",&zpt,"zpt/F");
   electrontree->Branch("zeta",&zeta,"zeta/F");
   electrontree->Branch("zphi",&zphi,"zphi/F");
   electrontree->Branch("elept1",&elept1,"elept1/F");
   electrontree->Branch("eleeta1",&eleeta1,"eleeta1/F");
   electrontree->Branch("elephi1",&elephi1,"elephi1/F");
   electrontree->Branch("elecharge1",&elecharge1,"elecharge1/F");
   electrontree->Branch("elehcaliso1",&elehcaliso1,"elehcaliso1/F");
   electrontree->Branch("eleecaliso1",&eleecaliso1,"eleecaliso1/F");
   electrontree->Branch("eletrackiso1",&eletrackiso1,"eletrackiso1/F");
   electrontree->Branch("elechisq1",&elechisq1,"elechisq1/F");
   electrontree->Branch("elenhits1",&elenhits1,"elenhits1/I");
   electrontree->Branch("eledB1",&eledB1,"eledB1/F");
   electrontree->Branch("elefbrem1",&elefbrem1,"elefbrem1/F");
   electrontree->Branch("eledeltaeta1",&eledeltaeta1,"eledeltaeta1/F");
   electrontree->Branch("eledeltaphi1",&eledeltaphi1,"eledeltaphi1/F");
   electrontree->Branch("eleHovE1",&eleHovE1,"eleHovE1/F");
   electrontree->Branch("eleHovE1other",&eleHovE1other,"eleHovE1other/F");
   electrontree->Branch("eleEovP1",&eleEovP1,"eleEovP1/F");
   electrontree->Branch("elesigmaetaeta1",&elesigmaetaeta1,"elesigmaetaeta1/F");
   electrontree->Branch("elesigmaietaieta1",&elesigmaietaieta1,"elesigmaietaieta1/F");
   electrontree->Branch("elerobusttight1",&elerobusttight1,"elerobusttight1/I"); 
   electrontree->Branch("elerobustloose1",&elerobustloose1,"elerobustloose1/I"); 
   electrontree->Branch("eletight1",&eletight1,"eletight1/I"); 
   electrontree->Branch("eleloose1",&eleloose1,"eleloose1/I"); 

   electrontree->Branch("elept2",&elept2,"elept2/F");
   electrontree->Branch("eleeta2",&eleeta2,"eleeta2/F");
   electrontree->Branch("elephi2",&elephi2,"elephi2/F");
   electrontree->Branch("elecharge2",&elecharge2,"elecharge2/F");
   electrontree->Branch("elehcaliso2",&elehcaliso2,"elehcaliso2/F");
   electrontree->Branch("eleecaliso2",&eleecaliso2,"eleecaliso2/F");
   electrontree->Branch("eletrackiso2",&eletrackiso2,"eletrackiso2/F");
   electrontree->Branch("elechisq2",&elechisq2,"elechisq2/F");
   electrontree->Branch("elenhits2",&elenhits2,"elenhits2/I");
   electrontree->Branch("eledB2",&eledB2,"eledB2/F");
   electrontree->Branch("elefbrem2",&elefbrem2,"elefbrem2/F");
   electrontree->Branch("eledeltaeta2",&eledeltaeta2,"eledeltaeta2/F");
   electrontree->Branch("eledeltaphi2",&eledeltaphi2,"eledeltaphi2/F");
   electrontree->Branch("eleHovE2",&eleHovE2,"eleHovE2/F");
   electrontree->Branch("eleHovE2other",&eleHovE2other,"eleHovE2other/F");
   electrontree->Branch("eleEovP2",&eleEovP2,"eleEovP2/F");
   electrontree->Branch("elesigmaetaeta2",&elesigmaetaeta2,"elesigmaetaeta2/F");
   electrontree->Branch("elesigmaietaieta2",&elesigmaietaieta2,"elesigmaietaieta2/F");
   electrontree->Branch("elerobusttight2",&elerobusttight2,"elerobusttight2/I"); 
   electrontree->Branch("elerobustloose2",&elerobustloose2,"elerobustloose2/I"); 
   electrontree->Branch("eletight2",&eletight2,"eletight2/I"); 
   electrontree->Branch("eleloose2",&eleloose2,"eleloose2/I"); 

   electrontree->Branch("njetsele",&njetsele,"njetsele/I");   
   electrontree->Branch("jetpt1",&jetpt1,"jetpt1/F");
   electrontree->Branch("jeteta1",&jeteta1,"jeteta1/F");
   electrontree->Branch("jetphi1",&jetphi1,"jetphi1/F");
   electrontree->Branch("jetcharge1",&jetcharge1,"jetcharge1/F");
   electrontree->Branch("jetDeltaRa1",&jetDeltaRa1,"jetDeltaRa1/F");
   electrontree->Branch("jetDeltaRb1",&jetDeltaRb1,"jetDeltaRb1/F");
   electrontree->Branch("jetEmFrac1",&jetEmFrac1,"jetEmFrac1/F");
   electrontree->Branch("jetpt2",&jetpt2,"jetpt2/F");
   electrontree->Branch("jeteta2",&jeteta2,"jeteta2/F");
   electrontree->Branch("jetphi2",&jetphi2,"jetphi2/F");
   electrontree->Branch("jetcharge2",&jetcharge2,"jetcharge2/F");
   electrontree->Branch("jetDeltaRa2",&jetDeltaRa2,"jetDeltaRa2/F");
   electrontree->Branch("jetDeltaRb2",&jetDeltaRb2,"jetDeltaRb2/F");
   electrontree->Branch("jetEmFrac2",&jetEmFrac2,"jetEmFrac2/F");
   electrontree->Branch("jetpt3",&jetpt3,"jetpt3/F");
   electrontree->Branch("jeteta3",&jeteta3,"jeteta3/F");
   electrontree->Branch("jetphi3",&jetphi3,"jetphi3/F");
   electrontree->Branch("jetcharge3",&jetcharge3,"jetcharge3/F");
   electrontree->Branch("jetDeltaRa3",&jetDeltaRa3,"jetDeltaRa3/F");
   electrontree->Branch("jetDeltaRb3",&jetDeltaRb3,"jetDeltaRb3/F");
   electrontree->Branch("jetEmFrac3",&jetEmFrac3,"jetEmFrac3/F");

   cout << "RecoElectronNtuple: ntuple eletree created " << endl;
   
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
  cout<<"RecoElectronNtuple analyzing nr. file = "<<fileCounter<<endl;
  cout<<"RecoElectron analyzing nr. event = "<<_entries<<endl;}
 
  if(_Norm==false){
  _entries = fileCounter*_EventsPerFile;
  cout<<"RecoElectron analyzing nr. file = "<<fileCounter<<endl;
  cout<<"RecoElectron analyzing nr. event = "<<_entries<<endl;
  }
  
  delete ch;
  
  double lumi = _entries/_xsec;

  if(lumi){
  _weight = _targetLumi/lumi;
  }
  
  cout << "RecoElectron Worker built." << endl;   
 
}


 void  RecoElectronNtuple::zero_ntuple(){
 
   // zeros everything
   ptzgen=0.;
   etazgen=0.;
   phizgen=0.;
   zmassgen=0.;
   elegenpt1=0.;
   elegeneta1=0.;
   elegenphi1=0.;
   elegenpt2=0.;
   elegeneta2=0.;
   elegenphi2=0.;
   gennjetsele=0;

   acc_ptzgen=0.;
   acc_etazgen=0.;
   acc_phizgen=0.;
   acc_zmassgen=0.;
   acc_elegenpt1=0.;
   acc_elegeneta1=0.;
   acc_elegenphi1=0.;
   acc_elegenpt2=0.;
   acc_elegeneta2=0.;
   acc_elegenphi2=0.;
   acc_gennjetsele=0;
   acc_jetgenpt1=0.;
   acc_jetgeneta1=0.;
   acc_jetgenphi1=0.;
   acc_jetgencharge1=0.;
   acc_jetgenpt2=0.;
   acc_jetgeneta2=0.;
   acc_jetgenphi2=0.;
   acc_jetgencharge2=0.;
   acc_jetgenpt3=0.;
   acc_jetgeneta3=0.;
   acc_jetgenphi3=0.;
   acc_jetgencharge3=0.;

      loosezmass=0.;
      zmass=0.;
      zpt=0.;
      zeta=0.;
      zphi=0.;
      elept1=0.;
      eleeta1=0.;
      elephi1 = 0.;
      elecharge1 = 0.;
      elehcaliso1=0.;
      eleecaliso1=0.;
      eletrackiso1=0.;
      elechisq1=0.;
      elenhits1=0;
      eledB1 = 0.;
      elefbrem1=0;
      eledeltaeta1=0.;
      eledeltaphi1=0.;
      eleHovE1=0.;
      eleHovE1other=0.;
      eleEovP1=0.;
      elesigmaetaeta1=0.;
      elesigmaietaieta1=0.;
      elerobusttight1=-9;
      elerobustloose1=-9;
      eletight1=-9;
      eleloose1=-9;
      elept2=0.;
      eleeta2=0.;
      elephi2 = 0.;
      elecharge2 = 0.;
      elehcaliso2=0.;
      eleecaliso2=0.;
      eletrackiso2=0.;
      elechisq2=0.;
      elenhits2=0;
      eledB2 = 0.;
      elefbrem2=0;
      eledeltaeta2=0.;
      eledeltaphi2=0.;
      eleHovE2=0.;
      eleHovE2other=0.;
      eleEovP2=0.;
      elesigmaetaeta2=0.;
      elesigmaietaieta2=0.;
      elerobusttight2=-9;
      elerobustloose2=-9;
      eletight2=-9;
      eleloose2=-9;
      numberOfZ = 0;
      nelesall = 0;
      neles = 0;

      njetsele = 0;
      jetpt1=0.;
      jeteta1=0.;
      jetphi1=0.;
      jetcharge1=0.;
      jetDeltaRa1=0.;
      jetDeltaRb1=0.;
      jetEmFrac1=0.;
      jetpt2=0.;
      jeteta2=0.;
      jetphi2=0.;
      jetcharge2=0.;
      jetDeltaRa2=0.;
      jetDeltaRb2=0.;
      jetEmFrac2=0.;
      jetpt3=0.;
      jeteta3=0.;
      jetphi3=0.;
      jetcharge3=0.;
      jetDeltaRa3=0.;
      jetDeltaRb3=0.;
      jetEmFrac3=0.;


}

RecoElectronNtuple::~RecoElectronNtuple(){
  _file->ls();
}

void  RecoElectronNtuple::process(const fwlite::Event& iEvent)
{

   // zero everything
   zero_ntuple();

   _file->cd();

   // generators quantities

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zgenHandle;
   zgenHandle.getByLabel(iEvent, "zeegenfull");

   fwlite::Handle<std::vector<reco::GenJet> > jetgenHandle;
   jetgenHandle.getByLabel(iEvent, "selectedGenJets");

   if (zgenHandle->size() > 1) return;
   
   std::vector<const reco::Candidate*> zgendaughters = ZGENDaughters(*zgenHandle);
   const reco::Candidate *gendau0, *gendau1;
   
     if(zgendaughters.size() != 0){        
     gendau0 = zgendaughters[0];
     gendau1 = zgendaughters[1];
     }
   
   std::vector<const reco::GenJet*> genjets = GetJets<reco::GenJet>(*jetgenHandle);   
   std::vector<const reco::GenJet*> isogenjets;
   
   for(unsigned int i = 0; i < genjets.size(); i++){
   if(GenIsoJet(*zgenHandle,*genjets[i]))isogenjets.push_back(genjets[i]);}

   if (GenSelected(*zgenHandle)&&zgendaughters.size()!=0){   

      ptzgen=(*zgenHandle)[0].pt();
      etazgen=(*zgenHandle)[0].eta();
      phizgen=(*zgenHandle)[0].phi();
      zmassgen=(*zgenHandle)[0].mass();
      double genpt1= gendau0->pt();
      double genpt2= gendau1->pt();
      if(genpt1>=genpt2){
        elegenpt1  = gendau0->pt(); 
        elegeneta1 = gendau0->eta();
        elegenphi1 = gendau0->phi();
        elegenpt2  = gendau1->pt();
        elegeneta2 = gendau1->eta();
        elegenphi2 = gendau1->phi();
      }else{
        elegenpt1  = gendau1->pt(); 
        elegeneta1 = gendau1->eta();
        elegenphi1 = gendau1->phi();
        elegenpt2  = gendau0->pt();
        elegeneta2 = gendau0->eta();
        elegenphi2 = gendau0->phi();
        } 
        
        gennjetsele=isogenjets.size();
      
   }   // end one Z gen

   if (GenSelectedInAcceptance(*zgenHandle)&&zgendaughters.size()!=0){
   
      acc_ptzgen=(*zgenHandle)[0].pt();
      acc_etazgen=(*zgenHandle)[0].eta();
      acc_phizgen=(*zgenHandle)[0].phi();
      acc_zmassgen=(*zgenHandle)[0].mass();
      double acc_genpt1= gendau0->pt();
      double acc_genpt2= gendau1->pt();
      if(acc_genpt1>=acc_genpt2){
      acc_elegenpt1  = gendau0->pt(); 
      acc_elegeneta1 = gendau0->eta();
      acc_elegenphi1 = gendau0->phi();
      acc_elegenpt2  = gendau1->pt();
      acc_elegeneta2 = gendau1->eta();
      acc_elegenphi2 = gendau1->phi();
      }else{
      acc_elegenpt1  = gendau1->pt(); 
      acc_elegeneta1 = gendau1->eta();
      acc_elegenphi1 = gendau1->phi();
      acc_elegenpt2  = gendau0->pt();
      acc_elegeneta2 = gendau0->eta();
      acc_elegenphi2 = gendau0->phi();
      }
	 
      acc_gennjetsele=isogenjets.size();

      if(genjets.size()>0){
       acc_jetgenpt1=genjets[0]->pt();
       acc_jetgeneta1=genjets[0]->eta();
       acc_jetgenphi1=genjets[0]->phi();
       acc_jetgencharge1=genjets[0]->charge();
      }
      if(genjets.size()>1){
       acc_jetgenpt2=genjets[1]->pt();
       acc_jetgeneta2=genjets[1]->eta();
       acc_jetgenphi2=genjets[1]->phi();
       acc_jetgencharge2=genjets[1]->charge();
      }
      if(genjets.size()>2){
       acc_jetgenpt3=genjets[2]->pt();
       acc_jetgeneta3=genjets[2]->eta();
       acc_jetgenphi3=genjets[2]->phi();
       acc_jetgencharge3=genjets[2]->charge();
      }

   } // end selected in acceptance


   //reconstructed quantities
   fwlite::Handle<std::vector<pat::Electron> > electronHandle;
   electronHandle.getByLabel(iEvent, "selectedElectrons");

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zrecHandle;
   zrecHandle.getByLabel(iEvent, "zeerec");

   fwlite::Handle<std::vector<pat::Jet> > jetrecHandle;
   jetrecHandle.getByLabel(iEvent, "selectedJets");

   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");

   numberOfZ=zrecHandle->size();
   nelesall=electronHandle->size();    // all electrons with loose cuts

     std::vector<const pat::Jet*> recjets = GetJets<pat::Jet>(*jetrecHandle);
     std::vector<const pat::Jet*> isorecjets;
     std::vector<const pat::Jet*> notisorecjets;
      
     for(unsigned int i = 0; i < recjets.size(); i++){     
     if(RecoIsoJet(*zrecHandle,*recjets[i]))isorecjets.push_back(recjets[i]);
     if(!RecoIsoJet(*zrecHandle,*recjets[i]))notisorecjets.push_back(recjets[i]);}
   
     std::vector<const pat::Electron*> zrecdaughters = ZDaughters(*zrecHandle);
     const pat::Electron *recdau0, *recdau1;
    
     if(zrecdaughters.size()){
     
     recdau0 = zrecdaughters[0];
     recdau1 = zrecdaughters[1];
     
     //Pre selections events
     
     
     
     }

  // loose cuts, only acceptance cuts
   if (RecSelected_Acc(*zrecHandle)){
   
   const reco::GsfTrack track0 = *(recdau0->gsfTrack());
   const reco::GsfTrack track1 = *(recdau1->gsfTrack());
   assert(&track0 && &track1);

      // fill variables for ntuple
      loosezmass=(*zrecHandle)[0].mass();
      zpt=(*zrecHandle)[0].pt();
      zeta=(*zrecHandle)[0].eta();
      zphi=(*zrecHandle)[0].phi();
      //
      elept1=recdau0->pt();
      eleeta1=recdau0->eta();
      elephi1=recdau0->phi();
      elecharge1=recdau0->charge();
      elehcaliso1=recdau0->hcalIso();
      eleecaliso1=recdau0->ecalIso();
      eletrackiso1=recdau0->trackIso();
      elenhits1=track0.numberOfValidHits();
      elechisq1=track0.normalizedChi2();
      eledB1=recdau0->dB();
      elefbrem1=recdau0->fbrem();
      eledeltaeta1=recdau0->deltaEtaSuperClusterTrackAtVtx();
      eledeltaphi1=recdau0->deltaPhiSuperClusterTrackAtVtx();
      eleHovE1=recdau0->hadronicOverEm();
      eleHovE1other=recdau0->hcalOverEcal();
      eleEovP1=recdau0->eSuperClusterOverP();
      elesigmaetaeta1=recdau0->sigmaEtaEta();
      elesigmaietaieta1=recdau0->sigmaIetaIeta();
      
      if(recdau0->isElectronIDAvailable("eidRobustTight") && 
	  recdau0->electronID("eidRobustTight")) elerobusttight1=1;
      if(recdau0->isElectronIDAvailable("eidRobustLoose") && 
	  recdau0->electronID("eidRobustLoose")) elerobustloose1=1;
      if(recdau0->isElectronIDAvailable("eidTight") && 
	  recdau0->electronID("eidTight")) eletight1=1;
      if(recdau0->isElectronIDAvailable("eidLoose") && 
	  recdau0->electronID("eidLoose")) eleloose1=1;

      elept2=recdau1->pt();
      eleeta2=recdau1->eta();
      elephi2=recdau1->phi();
      elecharge2=recdau1->charge();
      elehcaliso2=recdau1->hcalIso();
      eleecaliso2=recdau1->ecalIso();
      eletrackiso2=recdau1->trackIso();
      eledB2=recdau1->dB();
      elenhits2=track1.numberOfValidHits();
      elechisq2=track1.normalizedChi2();
      elefbrem2=recdau1->fbrem();
      eledeltaeta2=recdau1->deltaEtaSuperClusterTrackAtVtx();
      eledeltaphi2=recdau1->deltaPhiSuperClusterTrackAtVtx();
      eleHovE2=recdau1->hadronicOverEm();
      eleHovE2other=recdau1->hcalOverEcal();
      eleEovP2=recdau1->eSuperClusterOverP();
      elesigmaetaeta2=recdau1->sigmaEtaEta();
      elesigmaietaieta2=recdau1->sigmaIetaIeta();
      
      if(recdau1->isElectronIDAvailable("eidRobustTight") && 
	  recdau1->electronID("eidRobustTight")) elerobusttight2=1;
      if(recdau1->isElectronIDAvailable("eidRobustLoose") && 
	  recdau1->electronID("eidRobustLoose")) elerobustloose2=1;
      if(recdau1->isElectronIDAvailable("eidTight") && 
	  recdau1->electronID("eidTight")) eletight2=1;
      if(recdau1->isElectronIDAvailable("eidLoose") && 
	  recdau1->electronID("eidLoose")) eleloose2=1;
     
         njetsele=isorecjets.size();
         
      // fill ntuples for jets
      if (isorecjets.size()>0){
        jetpt1=isorecjets[0]->pt();
        jeteta1=isorecjets[0]->eta();
        jetphi1=isorecjets[0]->phi();
        jetcharge1=isorecjets[0]->jetCharge();
        jetDeltaRa1=Delta_R(*recdau0, *isorecjets[0]);
        jetDeltaRb1=Delta_R(*recdau1, *isorecjets[0]);
	jetEmFrac1=isorecjets[0]->emEnergyFraction();
      }
      if (isorecjets.size()>1){
        jetpt2=isorecjets[1]->pt();
        jeteta2=isorecjets[1]->eta();
        jetphi2=isorecjets[1]->phi();
        jetcharge2=isorecjets[1]->jetCharge();
        jetDeltaRa2=Delta_R(*recdau0, *isorecjets[1]);
        jetDeltaRb2=Delta_R(*recdau1, *isorecjets[1]);
	jetEmFrac2=isorecjets[1]->emEnergyFraction();
      }
      if (isorecjets.size()>2){
        jetpt3=isorecjets[2]->pt();
        jeteta3=isorecjets[2]->eta();
        jetphi3=isorecjets[2]->phi();
        jetcharge3=isorecjets[2]->jetCharge();
        jetDeltaRa3=Delta_R(*recdau0, *isorecjets[2]);
        jetDeltaRb3=Delta_R(*recdau1, *isorecjets[2]);
	jetEmFrac3=isorecjets[2]->emEnergyFraction();

      }


      // now starts the stricter cuts
   if (RecSelected_Acc_Qual_Imp_Iso_EiD(*zrecHandle, _electronID.c_str())){
  
      zmass=(*zrecHandle)[0].mass();
      

   }  // endif strict selected Z     
      
   }  // endif loose selected Z


   electrontree->Fill();   // fill outside any loop
     

}

void RecoElectronNtuple::finalize(){
   
  _file->Write();

}
