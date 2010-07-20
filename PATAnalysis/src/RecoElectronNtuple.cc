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
 
_sample("mc"), _selections("VBTF"), _ptjetmin(30.), _etajetmax(3.), _isocut(0.1), _weight(1.), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1), _Acc(1), _Trg(2), _Qual(3), _Imp(4), _Iso(5), _EiD(6), _Norm(false)

{ }

void RecoElectronNtuple::begin(TFile* out, const edm::ParameterSet& iConfig){

   _file = out; 
  
   std::string dirname = iConfig.getParameter<std::string>("Name");
   std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
   _selections = iConfig.getParameter<std::string>("Selections");
   _sample = iConfig.getParameter<std::string>("Sample");
   _targetLumi= iConfig.getParameter<double>("targetLumi");
   _xsec      = iConfig.getParameter<double>("CrossSection");
   _Norm      = iConfig.getParameter<bool>("Norm");
   _EventsPerFile    = iConfig.getParameter<int32_t>("EventsPerFile");
   _EventNumber    = iConfig.getParameter<int32_t>("EventNumber");
   _ProcEvents    = iConfig.getParameter<int32_t>("ProcEvents");
   
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

   cout << "RecoElectronNtuple file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();

   zeetree= new TTree("ZeeTree","ZeeTree");
   
   zeetree->Branch("run",&run,"run/I");
   zeetree->Branch("event",&event,"event/I");
   zeetree->Branch("lumi",&lumi,"lumi/I");
   
   zeetree->Branch("TrgBit",&TrgBit,"TrgBit/I");
   zeetree->Branch("OneElTrgMatch",&OneElTrgMatch,"OneElTrgMatch/I");
   zeetree->Branch("TRG_Photon15",&TRG_Photon15,"TRG_Photon15/I");
   zeetree->Branch("TRG_Ele15",&TRG_Ele15,"TRG_Ele15/I");
   
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
   zeetree->Branch("zmass",&zmass,"zmass/F");
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

   zeetree->Branch("ncalojetsele",&ncalojetsele,"ncalojetsele/I");  
   zeetree->Branch("calojetet1",&calojetet1,"calojetet1/F"); 
   zeetree->Branch("calojetpt1",&calojetpt1,"calojetpt1/F");
   zeetree->Branch("calojeteta1",&calojeteta1,"calojeteta1/F");
   zeetree->Branch("calojetphi1",&calojetphi1,"calojetphi1/F");
   zeetree->Branch("calojetcharge1",&calojetcharge1,"calojetcharge1/F");
   zeetree->Branch("calojetDeltaRa1",&calojetDeltaRa1,"calojetDeltaRa1/F");
   zeetree->Branch("calojetDeltaRb1",&calojetDeltaRb1,"calojetDeltaRb1/F");
   zeetree->Branch("calojetEmFrac1",&calojetEmFrac1,"calojetEmFrac1/F");
   zeetree->Branch("calojetet2",&calojetet2,"calojetet2/F"); 
   zeetree->Branch("calojetpt2",&calojetpt2,"calojetpt2/F");
   zeetree->Branch("calojeteta2",&calojeteta2,"calojeteta2/F");
   zeetree->Branch("calojetphi2",&calojetphi2,"calojetphi2/F");
   zeetree->Branch("calojetcharge2",&calojetcharge2,"calojetcharge2/F");
   zeetree->Branch("calojetDeltaRa2",&calojetDeltaRa2,"calojetDeltaRa2/F");
   zeetree->Branch("calojetDeltaRb2",&calojetDeltaRb2,"calojetDeltaRb2/F");
   zeetree->Branch("calojetEmFrac2",&calojetEmFrac2,"calojetEmFrac2/F");
   zeetree->Branch("calojetet3",&calojetet3,"calojetet3/F"); 
   zeetree->Branch("calojetpt3",&calojetpt3,"calojetpt3/F");
   zeetree->Branch("calojeteta3",&calojeteta3,"calojeteta3/F");
   zeetree->Branch("calojetphi3",&calojetphi3,"calojetphi3/F");
   zeetree->Branch("calojetcharge3",&calojetcharge3,"calojetcharge3/F");
   zeetree->Branch("calojetDeltaRa3",&calojetDeltaRa3,"calojetDeltaRa3/F");
   zeetree->Branch("calojetDeltaRb3",&calojetDeltaRb3,"calojetDeltaRb3/F");
   zeetree->Branch("calojetEmFrac3",&calojetEmFrac3,"calojetEmFrac3/F");
   
   zeetree->Branch("npfjetsele",&npfjetsele,"npfjetsele/I");   
   zeetree->Branch("pfjetet1",&pfjetet1,"pfjetet1/F"); 
   zeetree->Branch("pfjetpt1",&pfjetpt1,"pfjetpt1/F");
   zeetree->Branch("pfjeteta1",&pfjeteta1,"pfjeteta1/F");
   zeetree->Branch("pfjetphi1",&pfjetphi1,"pfjetphi1/F");
   zeetree->Branch("pfjetcharge1",&pfjetcharge1,"pfjetcharge1/F");
   zeetree->Branch("pfjetDeltaRa1",&pfjetDeltaRa1,"pfjetDeltaRa1/F");
   zeetree->Branch("pfjetDeltaRb1",&pfjetDeltaRb1,"pfjetDeltaRb1/F");
   zeetree->Branch("pfjetet2",&pfjetet2,"pfjetet2/F"); 
   zeetree->Branch("pfjetpt2",&pfjetpt2,"pfjetpt2/F");
   zeetree->Branch("pfjeteta2",&pfjeteta2,"pfjeteta2/F");
   zeetree->Branch("pfjetphi2",&pfjetphi2,"pfjetphi2/F");
   zeetree->Branch("pfjetcharge2",&pfjetcharge2,"pfjetcharge2/F");
   zeetree->Branch("pfjetDeltaRa2",&pfjetDeltaRa2,"pfjetDeltaRa2/F");
   zeetree->Branch("pfjetDeltaRb2",&pfjetDeltaRb2,"pfjetDeltaRb2/F");
   zeetree->Branch("pfjetet3",&pfjetet3,"pfjetet3/F"); 
   zeetree->Branch("pfjetpt3",&pfjetpt3,"pfjetpt3/F");
   zeetree->Branch("pfjeteta3",&pfjeteta3,"pfjeteta3/F");
   zeetree->Branch("pfjetphi3",&pfjetphi3,"pfjetphi3/F");
   zeetree->Branch("pfjetcharge3",&pfjetcharge3,"pfjetcharge3/F");
   zeetree->Branch("pfjetDeltaRa3",&pfjetDeltaRa3,"pfjetDeltaRa3/F");
   zeetree->Branch("pfjetDeltaRb3",&pfjetDeltaRb3,"pfjetDeltaRb3/F");

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
  
  if(_EventNumber==0 && _EventsPerFile==0)_entries = ch->GetEntries();
  
  if(_EventNumber!=0 && _EventsPerFile==0)_entries = _EventNumber;
 
  if(_EventNumber==0 && _EventsPerFile!=0)_entries = fileCounter*_EventsPerFile;
  
  if(_ProcEvents!=-1)_entries = _ProcEvents;
  
  cout<<"RecoElectron analyzing nr. file = "<<fileCounter<<endl;
  cout<<"RecoElectron analyzing nr. event = "<<_entries<<endl;
  
  delete ch;
  
  double lumi = _entries/_xsec;

  if(_Norm && lumi!=0){
   _weight = _targetLumi/lumi;
   }
  
  cout << "RecoElectron Worker built." << endl;   
 
}


 void  RecoElectronNtuple::zero_ntuple(){
 
   // zeros everything
   
   run=-1;
   event=-1;
   lumi=-1;
   TrgBit=-1;
   OneElTrgMatch=-1;
   TRG_Photon15=-1;
   TRG_Ele15=-1;
   
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

      zmass=-99.;
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

      ncalojetsele = -1;
      calojetet1=0.;
      calojetpt1=-99.;
      calojeteta1=-99.;
      calojetphi1=-99.;
      calojetcharge1=-99.;
      calojetDeltaRa1=-99.;
      calojetDeltaRb1=-99.;
      calojetEmFrac1=-99.;
      calojetet2=0.;
      calojetpt2=-99.;
      calojeteta2=-99.;
      calojetphi2=-99.;
      calojetcharge2=-99.;
      calojetDeltaRa2=-99.;
      calojetDeltaRb2=-99.;
      calojetEmFrac2=-99.;
      calojetet3=0.;
      calojetpt3=-99.;
      calojeteta3=-99.;
      calojetphi3=-99.;
      calojetcharge3=-99.;
      calojetDeltaRa3=-99.;
      calojetDeltaRb3=-99.;
      calojetEmFrac3=-99.;
      
      npfjetsele = -1;
      pfjetet1=0.;
      pfjetpt1=-99.;
      pfjeteta1=-99.;
      pfjetphi1=-99.;
      pfjetcharge1=-99.;
      pfjetDeltaRa1=-99.;
      pfjetDeltaRb1=-99.;
      pfjetet2=0.;
      pfjetpt2=-99.;
      pfjeteta2=-99.;
      pfjetphi2=-99.;
      pfjetcharge2=-99.;
      pfjetDeltaRa2=-99.;
      pfjetDeltaRb2=-99.;
      pfjetet3=0.;
      pfjetpt3=-99.;
      pfjeteta3=-99.;
      pfjetphi3=-99.;
      pfjetcharge3=-99.;
      pfjetDeltaRa3=-99.;
      pfjetDeltaRb3=-99.;

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
   lumi      = iEvent.luminosityBlock();

   // generators quantities

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zgenHandle;
   zgenHandle.getByLabel(iEvent, "zeegenfull");

   fwlite::Handle<std::vector<reco::GenJet> > jetgenHandle;
   jetgenHandle.getByLabel(iEvent, "selectedGenJets");
   
   if(_sample=="mc" && zgenHandle->size()!=0){
   
   if(zgenHandle->size()>1)throw cms::Exception("PATAnalysis:RecoElectronNtuple_MoreThanOneGENZ") << "ERROR! More than one GEN Z found!";
   
   std::vector<const reco::Candidate*> zgendaughters;
   if(zgenHandle->size())zgendaughters = ZGENDaughters((*zgenHandle)[0]);
   const reco::Candidate *gendau0 = 0;
   const reco::Candidate *gendau1 = 0;
   
     if(zgendaughters.size() != 0){        
     gendau0 = zgendaughters[0];
     gendau1 = zgendaughters[1];
     }
   
   std::vector<const reco::GenJet*> genjets = GetJets<reco::GenJet>(*jetgenHandle);   
   std::vector<const reco::GenJet*> isogenjets;
   
   for(unsigned int i = 0; i < genjets.size(); i++){
   if(IsoJet((*zgenHandle)[0],*genjets[i],"GEN"))isogenjets.push_back(genjets[i]);}

   if (GenSelected((*zgenHandle)[0], _selections)&&zgendaughters.size()!=0){   

      ptzgen=(*zgenHandle)[0].pt();
      etazgen=(*zgenHandle)[0].eta();
      phizgen=(*zgenHandle)[0].phi();
      zmassgen=(*zgenHandle)[0].mass();
      
      double genpt1= gendau0->pt();
      double genpt2= gendau1->pt();
      
      if(genpt2>genpt1)throw cms::Exception("PATAnalysis:RecoElectronNtuple_WrongElectronOrder") << "ERROR! Z electrons are in wrong order!";
 
        elegenpt1  = gendau0->pt(); 
        elegeneta1 = gendau0->eta();
        elegenphi1 = gendau0->phi();
        elegenpt2  = gendau1->pt();
        elegeneta2 = gendau1->eta();
        elegenphi2 = gendau1->phi();
        
        gennjetsele=isogenjets.size();
      
   }   // end one Z gen

   if (GenSelectedInAcceptance((*zgenHandle)[0], _selections)&&zgendaughters.size()!=0){
   
      acc_ptzgen=(*zgenHandle)[0].pt();
      acc_etazgen=(*zgenHandle)[0].eta();
      acc_phizgen=(*zgenHandle)[0].phi();
      acc_zmassgen=(*zgenHandle)[0].mass();
      
      double acc_genpt1= gendau0->pt();
      double acc_genpt2= gendau1->pt();
      
      if(acc_genpt2>acc_genpt1)throw cms::Exception("PATAnalysis:RecoElectronNtuple_WrongElectronOrder") << "ERROR! Z electrons are in wrong order!";
      
      acc_elegenpt1  = gendau0->pt(); 
      acc_elegeneta1 = gendau0->eta();
      acc_elegenphi1 = gendau0->phi();
      acc_elegenpt2  = gendau1->pt();
      acc_elegeneta2 = gendau1->eta();
      acc_elegenphi2 = gendau1->phi();
	 
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

   fwlite::Handle<std::vector<pat::Jet> > calojetrecHandle;
   calojetrecHandle.getByLabel(iEvent, "selectedJets");
   
   fwlite::Handle<std::vector<pat::Jet> > pfjetrecHandle;
   pfjetrecHandle.getByLabel(iEvent, "selectedPFJets");

   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");

   numberOfZ=zrecHandle->size();
   nelesall=electronHandle->size();    // all electrons with loose cuts
   
   // Z REC events
   
   if(zrecHandle->size()){

     std::vector<const pat::Jet*> calorecjets = GetJets<pat::Jet>(*calojetrecHandle);
     std::vector<const pat::Jet*> pfrecjets = GetJets<pat::Jet>(*pfjetrecHandle);
     
     std::vector<const pat::Jet*> caloisorecjets;
     std::vector<const pat::Jet*> pfisorecjets;
      
     for(unsigned int i = 0; i < calorecjets.size(); i++){     
     if(IsoJet((*zrecHandle)[0],*calorecjets[i],"RECO"))caloisorecjets.push_back(calorecjets[i]);}
     for(unsigned int i = 0; i < pfrecjets.size(); i++){     
     if(IsoJet((*zrecHandle)[0],*pfrecjets[i],"RECO"))pfisorecjets.push_back(pfrecjets[i]);}
   
     std::vector<const pat::Electron*> zrecdaughters = ZRECDaughters((*zrecHandle)[0]);
     const pat::Electron *recdau0 = 0;
     const pat::Electron *recdau1 = 0;
    
     if(zrecdaughters.size()){
     
     recdau0 = zrecdaughters[0];
     recdau1 = zrecdaughters[1];
     
     //Pre selections events
     
     
     
     }

   // loose cuts, only acceptance cuts
   
   if(isTriggerAvailable(*triggerHandle, "HLT_Photon15_L1R"))TRG_Photon15=1;
   if(isTriggerAvailable(*triggerHandle, "HLT_Ele15_LW_L1R"))TRG_Ele15=1;
   
   if(isElTriggerAvailable(*triggerHandle)){
   TrgBit=0;
   if(isElectronTriggered(*triggerHandle)){
   TrgBit=1;
   if(RecSelected_TrgMatch(*recdau0)||RecSelected_TrgMatch(*recdau1)){
   OneElTrgMatch=1;
   }
   }
   }
   
   if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle)){
   
      const reco::GsfTrackRef& track0 = recdau0->gsfTrack();
      const reco::GsfTrackRef& track1 = recdau1->gsfTrack();
      assert(track0.isNonnull() && track1.isNonnull());

      // fill variables for ntuple
      
      zmass=(*zrecHandle)[0].mass();
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
     
         ncalojetsele=caloisorecjets.size();
         npfjetsele=pfisorecjets.size();
         
      // fill ntuples for jets
      
      if (caloisorecjets.size()>0){
        calojetet1=caloisorecjets[0]->et();
        calojetpt1=caloisorecjets[0]->pt();
        calojeteta1=caloisorecjets[0]->eta();
        calojetphi1=caloisorecjets[0]->phi();
        calojetcharge1=caloisorecjets[0]->jetCharge();
        calojetDeltaRa1=Delta_R(*recdau0, *caloisorecjets[0]);
        calojetDeltaRb1=Delta_R(*recdau1, *caloisorecjets[0]);
	calojetEmFrac1=caloisorecjets[0]->emEnergyFraction();
      }
      if (caloisorecjets.size()>1){
        calojetet2=caloisorecjets[1]->et();
        calojetpt2=caloisorecjets[1]->pt();
        calojeteta2=caloisorecjets[1]->eta();
        calojetphi2=caloisorecjets[1]->phi();
        calojetcharge2=caloisorecjets[1]->jetCharge();
        calojetDeltaRa2=Delta_R(*recdau0, *caloisorecjets[1]);
        calojetDeltaRb2=Delta_R(*recdau1, *caloisorecjets[1]);
	calojetEmFrac2=caloisorecjets[1]->emEnergyFraction();
      }
      if (caloisorecjets.size()>2){
        calojetet3=caloisorecjets[2]->et();
        calojetpt3=caloisorecjets[2]->pt();
        calojeteta3=caloisorecjets[2]->eta();
        calojetphi3=caloisorecjets[2]->phi();
        calojetcharge3=caloisorecjets[2]->jetCharge();
        calojetDeltaRa3=Delta_R(*recdau0, *caloisorecjets[2]);
        calojetDeltaRb3=Delta_R(*recdau1, *caloisorecjets[2]);
	calojetEmFrac3=caloisorecjets[2]->emEnergyFraction();
      }
      
      if (pfisorecjets.size()>0){
        pfjetet1=pfisorecjets[0]->et();
        pfjetpt1=pfisorecjets[0]->pt();
        pfjeteta1=pfisorecjets[0]->eta();
        pfjetphi1=pfisorecjets[0]->phi();
        pfjetcharge1=pfisorecjets[0]->jetCharge();
        pfjetDeltaRa1=Delta_R(*recdau0, *pfisorecjets[0]);
        pfjetDeltaRb1=Delta_R(*recdau1, *pfisorecjets[0]);
      }
      if (pfisorecjets.size()>1){
        pfjetet2=pfisorecjets[1]->et();
        pfjetpt2=pfisorecjets[1]->pt();
        pfjeteta2=pfisorecjets[1]->eta();
        pfjetphi2=pfisorecjets[1]->phi();
        pfjetcharge2=pfisorecjets[1]->jetCharge();
        pfjetDeltaRa2=Delta_R(*recdau0, *pfisorecjets[1]);
        pfjetDeltaRb2=Delta_R(*recdau1, *pfisorecjets[1]);
      }
      if (pfisorecjets.size()>2){
        pfjetet3=pfisorecjets[2]->et();
        pfjetpt3=pfisorecjets[2]->pt();
        pfjeteta3=pfisorecjets[2]->eta();
        pfjetphi3=pfisorecjets[2]->phi();
        pfjetcharge3=pfisorecjets[2]->jetCharge();
        pfjetDeltaRa3=Delta_R(*recdau0, *pfisorecjets[2]);
        pfjetDeltaRb3=Delta_R(*recdau1, *pfisorecjets[2]);
      }
      
      } // endif loose selected Z


   // now starts the stricter cuts
      
   if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[5].c_str(), (*zrecHandle)[0], *triggerHandle)&&RecSelected(_RecoCutFlags[6].c_str(), (*zrecHandle)[0], *triggerHandle)){
  
      zmass_AllCuts=(*zrecHandle)[0].mass();
      
   }  // endif strict selected Z
   
   }
      
   zeetree->Fill();   // fill outside any loop
     
}

void RecoElectronNtuple::finalize(){
   
  _file->Write();

}
