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
 
_sample("mc"), _ptjetmin(30.), _etajetmax(3.), _isocut(0.1), _weight(1.), _Acc(1), _Trg(2), _Qual(3), _Imp(4), _Iso(5), _EiD(6)

{ }

void RecoElectronNtuple::begin(TFile* out, const edm::ParameterSet& iConfig){

   _file = out; 
  
   std::string dirname = iConfig.getParameter<std::string>("Name");
   std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
   _selections = iConfig.getParameter<std::string>("Selections");
   _sample = iConfig.getParameter<std::string>("Sample");
   _electronID = iConfig.getParameter<std::string>("electronID");
   _targetLumi= iConfig.getParameter<double>("targetLumi");
   _xsec      = iConfig.getParameter<double>("CrossSection");
   _Norm      = iConfig.getParameter<bool>("Norm");
   _EventsPerFile    = iConfig.getParameter<int32_t>("EventsPerFile");
   
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
   _RecoCutFlags[_Acc] =  "_Acc";
   _RecoCutFlags[_Iso] =  "_Iso";
   _RecoCutFlags[_EiD] =  "_EiD";}
   if(_selections=="VBTF"){
   _RecoCutFlags[_Acc] =  "_AccVBTF";
   _RecoCutFlags[_Iso] =  "_IsoVBTF";
   _RecoCutFlags[_EiD] =  "_EiDVBTF";}
     
   _RecoCutFlags[_Trg] =  "_Trg";
   _RecoCutFlags[_Qual] = "_Qual";
   _RecoCutFlags[_Imp] =  "_Imp";

   cout << "RecoElectronNtuple file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();

   zeetree= new TTree("ZeeTree","ZeeTree");
   valideltree = new TTree("ValidElTree","ValidElTree");
   validjettree = new TTree("ValidJetTree","ValidJetTree");
   validel_EventTree = new TTree("ValidEl_EventTree","ValidEl_EventTree");
   validjet_EventTree = new TTree("ValidJet_EventTree","ValidJet_EventTree");
   
   valideltree->Branch("run",&run,"run/I");
   valideltree->Branch("event",&event,"event/I");
   valideltree->Branch("valid_ElPt", &valid_ElPt, "valid_ElPt/F");
   valideltree->Branch("valid_ElPhi", &valid_ElPhi, "valid_ElPhi/F");
   valideltree->Branch("valid_ElEta", &valid_ElEta, "valid_ElEta/F");
   valideltree->Branch("valid_ElCharge", &valid_ElCharge, "valid_ElCharge/F");
   valideltree->Branch("valid_elehcaliso",&valid_elehcaliso,"valid_elehcaliso/F");
   valideltree->Branch("valid_eleecaliso",&valid_eleecaliso,"valid_eleecaliso/F");
   valideltree->Branch("valid_eletrackiso",&valid_eletrackiso,"valid_eletrackiso/F");
   valideltree->Branch("valid_elereliso",&valid_elereliso,"valid_elereliso/F");
   valideltree->Branch("valid_elechisq",&valid_elechisq,"valid_elechisq/F");
   valideltree->Branch("valid_elenhits",&valid_elenhits,"valid_elenhits/I");
   valideltree->Branch("valid_eledB",&valid_eledB,"valid_eledB/F");
   valideltree->Branch("valid_elefbrem",&valid_elefbrem,"valid_elefbrem/F");
   valideltree->Branch("valid_eledeltaeta",&valid_eledeltaeta,"valid_eledeltaeta/F");
   valideltree->Branch("valid_eledeltaphi",&valid_eledeltaphi,"valid_eledeltaphi/F");
   valideltree->Branch("valid_eleHadronicOvEm",&valid_eleHadronicOvEm,"valid_eleHadronicOvEm/F");
   valideltree->Branch("valid_eleHcalOvEcal",&valid_eleHcalOvEcal,"valid_eleHcalOvEcal/F");
   valideltree->Branch("valid_eleEovP",&valid_eleEovP,"valid_eleEovP/F");
   valideltree->Branch("valid_elesigmaetaeta",&valid_elesigmaetaeta,"valid_elesigmaetaeta/F");
   valideltree->Branch("valid_elesigmaietaieta",&valid_elesigmaietaieta,"valid_elesigmaietaieta/F");
   valideltree->Branch("valid_elerobusttight",&valid_elerobusttight,"valid_elerobusttight/I"); 
   valideltree->Branch("valid_elerobustloose",&valid_elerobustloose,"valid_elerobustloose/I"); 
   valideltree->Branch("valid_eletight",&valid_eletight,"valid_eletight/I"); 
   valideltree->Branch("valid_eleloose",&valid_eleloose,"valid_eleloose/I"); 
   
   validjettree->Branch("run",&run,"run/I");
   validjettree->Branch("event",&event,"event/I");
   validjettree->Branch("valid_ElMult",&valid_ElMult,"valid_ElMult/I");
   validjettree->Branch("valid_JetPt", &valid_JetPt, "valid_JetPt/F");
   validjettree->Branch("valid_JetPhi", &valid_JetPhi, "valid_JetPhi/F");
   validjettree->Branch("valid_JetEta", &valid_JetEta, "valid_JetEta/F");
   validjettree->Branch("valid_JetCharge", &valid_JetCharge, "valid_JetCharge/F"); 
   validjettree->Branch("valid_JetEl_DeltaR",valid_JetEl_DeltaR,"valid_JetEl_DeltaR[valid_ElMult]/F");
   validjettree->Branch("valid_JetEmFrac",&valid_JetEmFrac,"valid_JetEmFrac/F");
   
   validel_EventTree->Branch("run",&run,"run/I");
   validel_EventTree->Branch("event",&event,"event/I");
   validel_EventTree->Branch("valid_ElMult",&valid_ElMult,"valid_ElMult/I");
   validel_EventTree->Branch("valid_ElectronTrigger",&valid_ElectronTrigger,"valid_ElectronTrigger/I");
   
   validjet_EventTree->Branch("run",&run,"run/I");
   validjet_EventTree->Branch("event",&event,"event/I");
   validjet_EventTree->Branch("valid_JetMult",&valid_JetMult,"valid_JetMult/I");
   validjet_EventTree->Branch("valid_JetMult_Pt10Cut",&valid_JetMult_Pt10Cut,"valid_JetMult_Pt10Cut/I");
   validjet_EventTree->Branch("valid_JetTrigger",&valid_JetTrigger,"valid_JetTrigger/I"); 
   
   
   zeetree->Branch("run",&run,"run/I");
   zeetree->Branch("event",&event,"event/I");
   zeetree->Branch("weight", &_weight, "weight/F");
   //quantities at generator level 
   zeetree->Branch("ptzgen",&ptzgen,"ptzgen/F");
   zeetree->Branch("etazgen",&etazgen,"etazgen/F");
   zeetree->Branch("phizgen",&phizgen,"phizgen/F");
   zeetree->Branch("zmassgen",&zmassgen,"zmassgen/F");
   zeetree->Branch("elegenpt1",&elegenpt1,"elegenpt1/F");
   zeetree->Branch("elegeneta1",&elegeneta1,"elegeneta1/F");
   zeetree->Branch("elegenphi1",&elegenphi1,"elegenphi1/F");
   zeetree->Branch("elegenpt2",&elegenpt2,"elegenpt2/F");
   zeetree->Branch("elegeneta2",&elegeneta2,"elegeneta2/F");
   zeetree->Branch("elegenphi2",&elegenphi2,"elegenphi2/F");
   zeetree->Branch("gennjetsele",&gennjetsele,"gennjetsele/I");
   zeetree->Branch("acc_ptzgen",&acc_ptzgen,"acc_ptzgen/F");
   zeetree->Branch("acc_etazgen",&acc_etazgen,"acc_etazgen/F");
   zeetree->Branch("acc_phizgen",&acc_phizgen,"acc_phizgen/F");
   zeetree->Branch("acc_zmassgen",&acc_zmassgen,"zmassgen/F");
   zeetree->Branch("acc_elegenpt1",&acc_elegenpt1,"acc_elegenpt1/F");
   zeetree->Branch("acc_elegeneta1",&acc_elegeneta1,"acc_elegeneta1/F");
   zeetree->Branch("acc_elegenphi1",&acc_elegenphi1,"acc_elegenphi1/F");
   zeetree->Branch("acc_elegenpt2",&acc_elegenpt2,"acc_elegenpt2/F");
   zeetree->Branch("acc_elegeneta2",&acc_elegeneta2,"acc_elegeneta2/F");
   zeetree->Branch("acc_elegenphi2",&acc_elegenphi2,"acc_elegenphi2/F");
   zeetree->Branch("acc_gennjetsele",&acc_gennjetsele,"acc_gennjetsele/I");
   zeetree->Branch("acc_jetgenpt1",&acc_jetgenpt1,"acc_jetgenpt1/F");
   zeetree->Branch("acc_jetgeneta1",&acc_jetgeneta1,"acc_jetgeneta1/F");
   zeetree->Branch("acc_jetgenphi1",&acc_jetgenphi1,"acc_jetgenphi1/F");
   zeetree->Branch("acc_jetgencharge1",&acc_jetgencharge1,"acc_jetgencharge1/F");
   zeetree->Branch("acc_jetgenpt2",&acc_jetgenpt2,"acc_jetgenpt2/F");
   zeetree->Branch("acc_jetgeneta2",&acc_jetgeneta2,"acc_jetgeneta2/F");
   zeetree->Branch("acc_jetgenphi2",&acc_jetgenphi2,"acc_jetgenphi2/F");
   zeetree->Branch("acc_jetgencharge2",&acc_jetgencharge2,"acc_jetgencharge2/F");
   zeetree->Branch("acc_jetgenpt3",&acc_jetgenpt3,"acc_jetgenpt3/F");
   zeetree->Branch("acc_jetgeneta3",&acc_jetgeneta3,"acc_jetgeneta3/F");
   zeetree->Branch("acc_jetgenphi3",&acc_jetgenphi3,"acc_jetgenphi3/F");
   zeetree->Branch("acc_jetgencharge3",&acc_jetgencharge3,"acc_jetgencharge3/F");

   // reconstructed quantities
   zeetree->Branch("numberOfZ",&numberOfZ,"numberOfZ/I");
   zeetree->Branch("nelesall",&nelesall,"nelesall/I");
   zeetree->Branch("neles",&neles,"neles/I");
   zeetree->Branch("loosezmass",&loosezmass,"loosezmass/F");
   zeetree->Branch("zmass_AllCuts",&zmass_AllCuts,"zmass_AllCuts/F");
   zeetree->Branch("zpt",&zpt,"zpt/F");
   zeetree->Branch("zeta",&zeta,"zeta/F");
   zeetree->Branch("zphi",&zphi,"zphi/F");
   zeetree->Branch("elept1",&elept1,"elept1/F");
   zeetree->Branch("eleeta1",&eleeta1,"eleeta1/F");
   zeetree->Branch("elephi1",&elephi1,"elephi1/F");
   zeetree->Branch("elecharge1",&elecharge1,"elecharge1/F");
   zeetree->Branch("elehcaliso1",&elehcaliso1,"elehcaliso1/F");
   zeetree->Branch("eleecaliso1",&eleecaliso1,"eleecaliso1/F");
   zeetree->Branch("eletrackiso1",&eletrackiso1,"eletrackiso1/F");
   zeetree->Branch("elechisq1",&elechisq1,"elechisq1/F");
   zeetree->Branch("elenhits1",&elenhits1,"elenhits1/I");
   zeetree->Branch("eledB1",&eledB1,"eledB1/F");
   zeetree->Branch("elefbrem1",&elefbrem1,"elefbrem1/F");
   zeetree->Branch("eledeltaeta1",&eledeltaeta1,"eledeltaeta1/F");
   zeetree->Branch("eledeltaphi1",&eledeltaphi1,"eledeltaphi1/F");
   zeetree->Branch("eleHovE1",&eleHovE1,"eleHovE1/F");
   zeetree->Branch("eleHovE1other",&eleHovE1other,"eleHovE1other/F");
   zeetree->Branch("eleEovP1",&eleEovP1,"eleEovP1/F");
   zeetree->Branch("elesigmaetaeta1",&elesigmaetaeta1,"elesigmaetaeta1/F");
   zeetree->Branch("elesigmaietaieta1",&elesigmaietaieta1,"elesigmaietaieta1/F");
   zeetree->Branch("elerobusttight1",&elerobusttight1,"elerobusttight1/I"); 
   zeetree->Branch("elerobustloose1",&elerobustloose1,"elerobustloose1/I"); 
   zeetree->Branch("eletight1",&eletight1,"eletight1/I"); 
   zeetree->Branch("eleloose1",&eleloose1,"eleloose1/I"); 

   zeetree->Branch("elept2",&elept2,"elept2/F");
   zeetree->Branch("eleeta2",&eleeta2,"eleeta2/F");
   zeetree->Branch("elephi2",&elephi2,"elephi2/F");
   zeetree->Branch("elecharge2",&elecharge2,"elecharge2/F");
   zeetree->Branch("elehcaliso2",&elehcaliso2,"elehcaliso2/F");
   zeetree->Branch("eleecaliso2",&eleecaliso2,"eleecaliso2/F");
   zeetree->Branch("eletrackiso2",&eletrackiso2,"eletrackiso2/F");
   zeetree->Branch("elechisq2",&elechisq2,"elechisq2/F");
   zeetree->Branch("elenhits2",&elenhits2,"elenhits2/I");
   zeetree->Branch("eledB2",&eledB2,"eledB2/F");
   zeetree->Branch("elefbrem2",&elefbrem2,"elefbrem2/F");
   zeetree->Branch("eledeltaeta2",&eledeltaeta2,"eledeltaeta2/F");
   zeetree->Branch("eledeltaphi2",&eledeltaphi2,"eledeltaphi2/F");
   zeetree->Branch("eleHovE2",&eleHovE2,"eleHovE2/F");
   zeetree->Branch("eleHovE2other",&eleHovE2other,"eleHovE2other/F");
   zeetree->Branch("eleEovP2",&eleEovP2,"eleEovP2/F");
   zeetree->Branch("elesigmaetaeta2",&elesigmaetaeta2,"elesigmaetaeta2/F");
   zeetree->Branch("elesigmaietaieta2",&elesigmaietaieta2,"elesigmaietaieta2/F");
   zeetree->Branch("elerobusttight2",&elerobusttight2,"elerobusttight2/I"); 
   zeetree->Branch("elerobustloose2",&elerobustloose2,"elerobustloose2/I"); 
   zeetree->Branch("eletight2",&eletight2,"eletight2/I"); 
   zeetree->Branch("eleloose2",&eleloose2,"eleloose2/I"); 

   zeetree->Branch("njetsele",&njetsele,"njetsele/I");   
   zeetree->Branch("jetpt1",&jetpt1,"jetpt1/F");
   zeetree->Branch("jeteta1",&jeteta1,"jeteta1/F");
   zeetree->Branch("jetphi1",&jetphi1,"jetphi1/F");
   zeetree->Branch("jetcharge1",&jetcharge1,"jetcharge1/F");
   zeetree->Branch("jetDeltaRa1",&jetDeltaRa1,"jetDeltaRa1/F");
   zeetree->Branch("jetDeltaRb1",&jetDeltaRb1,"jetDeltaRb1/F");
   zeetree->Branch("jetEmFrac1",&jetEmFrac1,"jetEmFrac1/F");
   zeetree->Branch("jetpt2",&jetpt2,"jetpt2/F");
   zeetree->Branch("jeteta2",&jeteta2,"jeteta2/F");
   zeetree->Branch("jetphi2",&jetphi2,"jetphi2/F");
   zeetree->Branch("jetcharge2",&jetcharge2,"jetcharge2/F");
   zeetree->Branch("jetDeltaRa2",&jetDeltaRa2,"jetDeltaRa2/F");
   zeetree->Branch("jetDeltaRb2",&jetDeltaRb2,"jetDeltaRb2/F");
   zeetree->Branch("jetEmFrac2",&jetEmFrac2,"jetEmFrac2/F");
   zeetree->Branch("jetpt3",&jetpt3,"jetpt3/F");
   zeetree->Branch("jeteta3",&jeteta3,"jeteta3/F");
   zeetree->Branch("jetphi3",&jetphi3,"jetphi3/F");
   zeetree->Branch("jetcharge3",&jetcharge3,"jetcharge3/F");
   zeetree->Branch("jetDeltaRa3",&jetDeltaRa3,"jetDeltaRa3/F");
   zeetree->Branch("jetDeltaRb3",&jetDeltaRb3,"jetDeltaRb3/F");
   zeetree->Branch("jetEmFrac3",&jetEmFrac3,"jetEmFrac3/F");

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
   
   run=-1;
   event=-1;
   
   valid_ElPt=-99.;
   valid_ElPhi=-99.;
   valid_ElEta=-99.;
   valid_ElCharge=-99.;
   valid_elehcaliso=-99.;
   valid_eleecaliso=-99.;
   valid_eletrackiso=-99.;
   valid_elereliso=-99.;
   valid_elechisq=-99.;
   valid_elenhits=-1;
   valid_eledB=-99.;
   valid_elefbrem=-99.;
   valid_eledeltaeta=-99.;
   valid_eledeltaphi=-99.;
   valid_eleHadronicOvEm=-99.;
   valid_eleHcalOvEcal=-99.;
   valid_eleEovP=-99.;
   valid_elesigmaetaeta=-99.;
   valid_elesigmaietaieta=-99.;
   valid_elerobusttight=-1;
   valid_elerobustloose=-1;
   valid_eletight=-1;
   valid_eleloose=-1;
   
   valid_JetMult=-1;
   valid_JetMult_Pt10Cut=-1;
   valid_ElMult=-1;
   valid_ElectronTrigger=-1;
   valid_JetTrigger=-1;
   
   valid_JetPt=-99.;
   valid_JetPhi=-99.;
   valid_JetEta=-99.;
   valid_JetCharge=-99.;
   valid_JetEmFrac=-99;
   for(unsigned i = 0; i != 500; i++){
   valid_JetEl_DeltaR[i]=-99;}
   
   ptzgen=-99.;
   etazgen=-99.;
   phizgen=-99.;
   zmassgen=-99.;
   elegenpt1=-99.;
   elegeneta1=-99.;
   elegenphi1=-99.;
   elegenpt2=-99.;
   elegeneta2=-99.;
   elegenphi2=-99.;
   gennjetsele=-1;

   acc_ptzgen=-99.;
   acc_etazgen=-99.;
   acc_phizgen=-99.;
   acc_zmassgen=-99.;
   acc_elegenpt1=-99.;
   acc_elegeneta1=-99.;
   acc_elegenphi1=-99.;
   acc_elegenpt2=-99.;
   acc_elegeneta2=-99.;
   acc_elegenphi2=-99.;
   acc_gennjetsele=-1;
   acc_jetgenpt1=-99.;
   acc_jetgeneta1=-99.;
   acc_jetgenphi1=-99.;
   acc_jetgencharge1=-99.;
   acc_jetgenpt2=-99.;
   acc_jetgeneta2=-99.;
   acc_jetgenphi2=-99.;
   acc_jetgencharge2=-99.;
   acc_jetgenpt3=-99.;
   acc_jetgeneta3=-99.;
   acc_jetgenphi3=-99.;
   acc_jetgencharge3=-99.;

      loosezmass=-99.;
      zmass_AllCuts=-99.;
      zpt=-99.;
      zeta=-99.;
      zphi=-99.;
      elept1=-99.;
      eleeta1=-99.;
      elephi1 = -99.;
      elecharge1 = -99.;
      elehcaliso1=-99.;
      eleecaliso1=-99.;
      eletrackiso1=-99.;
      elechisq1=-99.;
      elenhits1=-1;
      eledB1 = -99.;
      elefbrem1=-99.;
      eledeltaeta1=-99.;
      eledeltaphi1=-99.;
      eleHovE1=-99.;
      eleHovE1other=-99.;
      eleEovP1=-99.;
      elesigmaetaeta1=-99.;
      elesigmaietaieta1=-99.;
      elerobusttight1=-1;
      elerobustloose1=-1;
      eletight1=-1;
      eleloose1=-1;
      elept2=-99.;
      eleeta2=-99.;
      elephi2 = -99.;
      elecharge2 = -99.;
      elehcaliso2=-99.;
      eleecaliso2=-99.;
      eletrackiso2=-99.;
      elechisq2=-99.;
      elenhits2=-1;
      eledB2 = -99.;
      elefbrem2=-99.;
      eledeltaeta2=-99.;
      eledeltaphi2=-99.;
      eleHovE2=-99.;
      eleHovE2other=-99.;
      eleEovP2=-99.;
      elesigmaetaeta2=-99.;
      elesigmaietaieta2=-99.;
      elerobusttight2=-1;
      elerobustloose2=-1;
      eletight2=-1;
      eleloose2=-1;
      numberOfZ = -1;
      nelesall = -1;
      neles = -1;

      njetsele = -1;
      jetpt1=-99.;
      jeteta1=-99.;
      jetphi1=-99.;
      jetcharge1=-99.;
      jetDeltaRa1=-99.;
      jetDeltaRb1=-99.;
      jetEmFrac1=-99.;
      jetpt2=-99.;
      jeteta2=-99.;
      jetphi2=-99.;
      jetcharge2=-99.;
      jetDeltaRa2=-99.;
      jetDeltaRb2=-99.;
      jetEmFrac2=-99.;
      jetpt3=-99.;
      jeteta3=-99.;
      jetphi3=-99.;
      jetcharge3=-99.;
      jetDeltaRa3=-99.;
      jetDeltaRb3=-99.;
      jetEmFrac3=-99.;


}

RecoElectronNtuple::~RecoElectronNtuple(){
  _file->ls();
}

void  RecoElectronNtuple::process(const fwlite::Event& iEvent)
{

   // zero everything
   
   zero_ntuple();

   _file->cd();
   
   run       = iEvent.id().run();
   event     = iEvent.id().event();

   // generators quantities

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zgenHandle;
   zgenHandle.getByLabel(iEvent, "zeegenfull");

   fwlite::Handle<std::vector<reco::GenJet> > jetgenHandle;
   jetgenHandle.getByLabel(iEvent, "selectedGenJets");
   
   if(_sample=="mc"){

   if (zgenHandle->size() > 1) return;
   
   std::vector<const reco::Candidate*> zgendaughters = ZGENDaughters(*zgenHandle);
   const reco::Candidate *gendau0 = 0;
   const reco::Candidate *gendau1 = 0;
   
     if(zgendaughters.size() != 0){        
     gendau0 = zgendaughters[0];
     gendau1 = zgendaughters[1];
     }
   
   std::vector<const reco::GenJet*> genjets = GetJets<reco::GenJet>(*jetgenHandle);   
   std::vector<const reco::GenJet*> isogenjets;
   
   for(unsigned int i = 0; i < genjets.size(); i++){
   if(GenIsoJet(*zgenHandle,*genjets[i]))isogenjets.push_back(genjets[i]);}

   if (GenSelected(*zgenHandle, _selections)&&zgendaughters.size()!=0){   

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

   if (GenSelectedInAcceptance(*zgenHandle, _selections)&&zgendaughters.size()!=0){
   
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

}

   //Reconstructed quantities
   
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
   
   //Validation Trees
   
   std::vector<const pat::Jet*> jetspt10cut;
   for(unsigned int i = 0; i < jetrecHandle->size(); i++){     
   if((*jetrecHandle)[i].pt())jetspt10cut.push_back(&(*jetrecHandle)[i]);}
   
   valid_ElMult = electronHandle->size();
   valid_JetMult = jetrecHandle->size();
   valid_JetMult_Pt10Cut = jetspt10cut.size();
   
   if(isElectronTriggerAvailable(*triggerHandle)){
   valid_ElectronTrigger=0;
   if(isElectronTriggered(*triggerHandle))valid_ElectronTrigger = 1;
   }
   
   if(isJetTriggerAvailable(*triggerHandle)){
   valid_JetTrigger=0;
   if(isJetTriggered(*triggerHandle))valid_JetTrigger = 1;
   }
   
   validel_EventTree->Fill();
   validjet_EventTree->Fill();
   
   for(unsigned i = 0; i != electronHandle->size(); i++){
   
   const reco::GsfTrackRef valid_eltrack_ref = (*electronHandle)[i].gsfTrack();
   assert(valid_eltrack_ref.isNonnull());
   const reco::GsfTrack valid_eltrack = *((*electronHandle)[i].gsfTrack());
   
   valid_ElPt = (*electronHandle)[i].pt();
   valid_ElPhi = (*electronHandle)[i].phi();
   valid_ElEta = (*electronHandle)[i].eta();
   valid_ElCharge = (*electronHandle)[i].charge();
   valid_elehcaliso=(*electronHandle)[i].hcalIso();
   valid_eleecaliso=(*electronHandle)[i].ecalIso();
   valid_eletrackiso=(*electronHandle)[i].trackIso();
   valid_elereliso=((*electronHandle)[i].trackIso() + (*electronHandle)[i].ecalIso() + (*electronHandle)[i].hcalIso()) / (*electronHandle)[i].pt();
   valid_elenhits=valid_eltrack.numberOfValidHits();
   valid_elechisq=valid_eltrack.normalizedChi2();
   valid_eledB=(*electronHandle)[i].dB();
   valid_elefbrem=(*electronHandle)[i].fbrem();
   valid_eledeltaeta=(*electronHandle)[i].deltaEtaSuperClusterTrackAtVtx();
   valid_eledeltaphi=(*electronHandle)[i].deltaPhiSuperClusterTrackAtVtx();
   valid_eleHadronicOvEm=(*electronHandle)[i].hadronicOverEm();
   valid_eleHcalOvEcal=(*electronHandle)[i].hcalOverEcal();
   valid_eleEovP=(*electronHandle)[i].eSuperClusterOverP();
   valid_elesigmaetaeta=(*electronHandle)[i].sigmaEtaEta();
   valid_elesigmaietaieta=(*electronHandle)[i].sigmaIetaIeta();
   if((*electronHandle)[i].isElectronIDAvailable("eidRobustTight") && (*electronHandle)[i].electronID("eidRobustTight")) valid_elerobusttight=1;
   if((*electronHandle)[i].isElectronIDAvailable("eidRobustLoose") && (*electronHandle)[i].electronID("eidRobustLoose")) valid_elerobustloose=1;
   if((*electronHandle)[i].isElectronIDAvailable("eidTight") && (*electronHandle)[i].electronID("eidTight")) valid_eletight=1;
   if((*electronHandle)[i].isElectronIDAvailable("eidLoose") && (*electronHandle)[i].electronID("eidLoose")) valid_eleloose=1;
      
   valideltree->Fill();
   }
   
   for(unsigned i = 0; i != jetrecHandle->size(); i++){
   
   valid_JetPt = (*jetrecHandle)[i].pt();
   valid_JetPhi = (*jetrecHandle)[i].phi();
   valid_JetEta = (*jetrecHandle)[i].eta();
   valid_JetCharge = (*jetrecHandle)[i].charge();
   valid_JetEmFrac= (*jetrecHandle)[i].emEnergyFraction();
   for(unsigned n = 0; n != electronHandle->size(); n++){valid_JetEl_DeltaR[n]=Delta_R((*electronHandle)[n], (*jetrecHandle)[i]);}
   
   validjettree->Fill();
   }
   
   // Z REC events
   
   if(zrecHandle->size()){

     std::vector<const pat::Jet*> recjets = GetJets<pat::Jet>(*jetrecHandle);
     std::vector<const pat::Jet*> isorecjets;
     std::vector<const pat::Jet*> notisorecjets;
      
     for(unsigned int i = 0; i < recjets.size(); i++){     
     if(RecoIsoJet((*zrecHandle)[0],*recjets[i]))isorecjets.push_back(recjets[i]);
     if(!RecoIsoJet((*zrecHandle)[0],*recjets[i]))notisorecjets.push_back(recjets[i]);}
   
     std::vector<const pat::Electron*> zrecdaughters = ZDaughters((*zrecHandle)[0]);
     const pat::Electron *recdau0 = 0;
     const pat::Electron *recdau1 = 0;
    
     if(zrecdaughters.size()){
     
     recdau0 = zrecdaughters[0];
     recdau1 = zrecdaughters[1];
     
     //Pre selections events
     
     
     
     }

   // loose cuts, only acceptance cuts
   
   if (RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)){
   
      const reco::GsfTrackRef& track0 = recdau0->gsfTrack();
      const reco::GsfTrackRef& track1 = recdau1->gsfTrack();
      assert(track0.isNonnull() && track1.isNonnull());

      // fill variables for ntuple
      
      loosezmass=(*zrecHandle)[0].mass();
      zpt=(*zrecHandle)[0].pt();
      zeta=(*zrecHandle)[0].eta();
      zphi=(*zrecHandle)[0].phi();
      
      elept1=recdau0->pt();
      eleeta1=recdau0->eta();
      elephi1=recdau0->phi();
      elecharge1=recdau0->charge();
      elehcaliso1=recdau0->hcalIso();
      eleecaliso1=recdau0->ecalIso();
      eletrackiso1=recdau0->trackIso();
      elenhits1=track0->numberOfValidHits();
      elechisq1=track0->normalizedChi2();
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
      elenhits2=track1->numberOfValidHits();
      elechisq2=track1->normalizedChi2();
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
      
      } // endif loose selected Z


   // now starts the stricter cuts
      
   if (RecSelected(_RecoCutFlags[1].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[2].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[3].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[4].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[5].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[6].c_str(), _electronID.c_str(), (*zrecHandle)[0], *triggerHandle)){
  
      zmass_AllCuts=(*zrecHandle)[0].mass();
      
   }  // endif strict selected Z
   
   }
      
   zeetree->Fill();   // fill outside any loop
     
}

void RecoElectronNtuple::finalize(){
   
  _file->Write();

}
