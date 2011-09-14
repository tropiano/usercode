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
#include "TLorentzVector.h"

using namespace std;
using namespace edm;


RecoElectronNtuple::RecoElectronNtuple(): 
 
  _sample("mc"), _selections("ASYM"), _NtupleFill("zcand"), _ptjetmin(30.), _etajetmax(3.), _isocut(0.1), _weight(1.), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1), _Acc(1), _Trg(2), _Conv(3), _Imp(4), _Iso(5), _EiD(6), _Norm(false)

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
   _NtupleFill      = iConfig.getParameter<std::string>("NtupleFill");
   _EventsPerFile    = iConfig.getParameter<int32_t>("EventsPerFile");
   _EventNumber    = iConfig.getParameter<int32_t>("EventNumber");
   _ProcEvents    = iConfig.getParameter<int32_t>("ProcEvents");
   
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
   
   static map<std::string, std::pair<int, int> > elTriggers = elTrigger();
   
   if(elTriggers.size()>10)throw cms::Exception("PATAnalysis:RecoElectronNtuple_TooManyTriggers") << "ERROR! Too Many Triggers! Modify elTrigVector size!";
   
   for(int i=0; i<10; i++){
   _elTrigVector[i] = "_NoTrg";}
   
   if(elTriggers.size()<10){
   int i = 0;
   static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
   for(TrgVectorIter = elTriggers.begin(); TrgVectorIter != elTriggers.end(); TrgVectorIter++){
   _elTrigVector[i] = TrgVectorIter->first.c_str();
   i++;
   }
   }
   
   std::string elTrg1_I = _elTrigVector[0]+"/I";
   zeetree->Branch(_elTrigVector[0].c_str(),&elTrg1,elTrg1_I.c_str());
   std::string elTrg2_I = _elTrigVector[1]+"/I";
   zeetree->Branch(_elTrigVector[1].c_str(),&elTrg2,elTrg2_I.c_str());
   std::string elTrg3_I = _elTrigVector[2]+"/I";
   zeetree->Branch(_elTrigVector[2].c_str(),&elTrg3,elTrg3_I.c_str());
   std::string elTrg4_I = _elTrigVector[3]+"/I";
   zeetree->Branch(_elTrigVector[3].c_str(),&elTrg4,elTrg4_I.c_str());
   std::string elTrg5_I = _elTrigVector[4]+"/I";
   zeetree->Branch(_elTrigVector[4].c_str(),&elTrg5,elTrg5_I.c_str());
   std::string elTrg6_I = _elTrigVector[5]+"/I";
   zeetree->Branch(_elTrigVector[5].c_str(),&elTrg6,elTrg6_I.c_str());
   std::string elTrg7_I = _elTrigVector[6]+"/I";
   zeetree->Branch(_elTrigVector[6].c_str(),&elTrg7,elTrg7_I.c_str());
   std::string elTrg8_I = _elTrigVector[7]+"/I";
   zeetree->Branch(_elTrigVector[7].c_str(),&elTrg8,elTrg8_I.c_str());
   std::string elTrg9_I = _elTrigVector[8]+"/I";
   zeetree->Branch(_elTrigVector[8].c_str(),&elTrg9,elTrg9_I.c_str());
   std::string elTrg10_I = _elTrigVector[9]+"/I";
   zeetree->Branch(_elTrigVector[9].c_str(),&elTrg10,elTrg10_I.c_str());
    
   zeetree->Branch("weight", &_weight, "weight/F");
   
   //cut flags
   zeetree->Branch("cutAccSYM",&cutAccSYM,"cutAccSYM/I");
   zeetree->Branch("cutAccASYM",&cutAccASYM,"cutAccASYM/I");
   zeetree->Branch("cutTrg",&cutTrg,"cutTrg/I");
   zeetree->Branch("cutImp",&cutImp,"cutImp/I");
   zeetree->Branch("cutConvASYM",&cutConvASYM,"cutConvASYM/I");
   zeetree->Branch("cutIsoSYM",&cutIsoSYM,"cutIsoSYM/I");
   zeetree->Branch("cutIsoASYM",&cutIsoASYM,"cutIsoASYM/I");
   zeetree->Branch("cutEiDSYM",&cutEiDSYM,"cutEiDSYM/I");
   zeetree->Branch("cutEiDASYM",&cutEiDASYM,"cutEiDASYM/I");
   
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

   //reconstructed quantities
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
   zeetree->Branch("elereliso1",&elereliso1,"elereliso1/F");
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
   zeetree->Branch("ele1mapID80cIso",&ele1mapID80cIso,"ele1mapID80cIso/I"); 

   zeetree->Branch("elept2",&elept2,"elept2/F");
   zeetree->Branch("eleeta2",&eleeta2,"eleeta2/F");
   zeetree->Branch("elephi2",&elephi2,"elephi2/F");
   zeetree->Branch("elecharge2",&elecharge2,"elecharge2/F");
   zeetree->Branch("elehcaliso2",&elehcaliso2,"elehcaliso2/F");
   zeetree->Branch("eleecaliso2",&eleecaliso2,"eleecaliso2/F");
   zeetree->Branch("eletrackiso2",&eletrackiso2,"eletrackiso2/F");
   zeetree->Branch("elereliso2",&elereliso2,"elereliso2/F");
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
   zeetree->Branch("ele2mapID95cIso",&ele2mapID95cIso,"ele2mapID95cIso/I"); 

   zeetree->Branch("npfjetsele",&npfjetsele,"npfjetsele/I");
   zeetree->Branch("genacc_npfjetsele",&genacc_npfjetsele,"genacc_npfjetsele/I");
   zeetree->Branch("pfjetID1",&pfjetID1,"pfjetID1/I");
   zeetree->Branch("pfjetet1",&pfjetet1,"pfjetet1/F"); 
   zeetree->Branch("pfjetpt1",&pfjetpt1,"pfjetpt1/F");
   zeetree->Branch("pfjeteta1",&pfjeteta1,"pfjeteta1/F");
   zeetree->Branch("pfjetphi1",&pfjetphi1,"pfjetphi1/F");
   zeetree->Branch("pfjetcharge1",&pfjetcharge1,"pfjetcharge1/F");
   zeetree->Branch("pfjetDeltaRa1",&pfjetDeltaRa1,"pfjetDeltaRa1/F");
   zeetree->Branch("pfjetDeltaRb1",&pfjetDeltaRb1,"pfjetDeltaRb1/F");
   zeetree->Branch("pfjetEmFrac1",&pfjetEmFrac1,"pfjetEmFrac1/F");
   zeetree->Branch("pfjetID2",&pfjetID2,"pfjetID2/I");
   zeetree->Branch("pfjetet2",&pfjetet2,"pfjetet2/F"); 
   zeetree->Branch("pfjetpt2",&pfjetpt2,"pfjetpt2/F");
   zeetree->Branch("pfjeteta2",&pfjeteta2,"pfjeteta2/F");
   zeetree->Branch("pfjetphi2",&pfjetphi2,"pfjetphi2/F");
   zeetree->Branch("pfjetcharge2",&pfjetcharge2,"pfjetcharge2/F");
   zeetree->Branch("pfjetDeltaRa2",&pfjetDeltaRa2,"pfjetDeltaRa2/F");
   zeetree->Branch("pfjetDeltaRb2",&pfjetDeltaRb2,"pfjetDeltaRb2/F");
   zeetree->Branch("pfjetEmFrac2",&pfjetEmFrac2,"pfjetEmFrac2/F");
   zeetree->Branch("pfjetID3",&pfjetID3,"pfjetID3/I");
   zeetree->Branch("pfjetet3",&pfjetet3,"pfjetet3/F"); 
   zeetree->Branch("pfjetpt3",&pfjetpt3,"pfjetpt3/F");
   zeetree->Branch("pfjeteta3",&pfjeteta3,"pfjeteta3/F");
   zeetree->Branch("pfjetphi3",&pfjetphi3,"pfjetphi3/F");
   zeetree->Branch("pfjetcharge3",&pfjetcharge3,"pfjetcharge3/F");
   zeetree->Branch("pfjetDeltaRa3",&pfjetDeltaRa3,"pfjetDeltaRa3/F");
   zeetree->Branch("pfjetDeltaRb3",&pfjetDeltaRb3,"pfjetDeltaRb3/F");
   zeetree->Branch("pfjetEmFrac3",&pfjetEmFrac3,"pfjetEmFrac3/F");
   zeetree->Branch("DeltaR_GenJetPFJet",&DeltaR_GenJetPFJet,"DeltaR_GenJetPFJet/F");
   
   zeetree->Branch("npfl1jetsele",&npfl1jetsele,"npfl1jetsele/I");
   zeetree->Branch("genacc_npfl1jetsele",&genacc_npfl1jetsele,"genacc_npfl1jetsele/I");
   zeetree->Branch("pfl1jetID1",&pfl1jetID1,"pfl1jetID1/I"); 
   zeetree->Branch("pfl1jetet1",&pfl1jetet1,"pfl1jetet1/F"); 
   zeetree->Branch("pfl1jetpt1",&pfl1jetpt1,"pfl1jetpt1/F");
   zeetree->Branch("pfl1jeteta1",&pfl1jeteta1,"pfl1jeteta1/F");
   zeetree->Branch("pfl1jetphi1",&pfl1jetphi1,"pfl1jetphi1/F");
   zeetree->Branch("pfl1jetcharge1",&pfl1jetcharge1,"pfl1jetcharge1/F");
   zeetree->Branch("pfl1jetDeltaRa1",&pfl1jetDeltaRa1,"pfl1jetDeltaRa1/F");
   zeetree->Branch("pfl1jetDeltaRb1",&pfl1jetDeltaRb1,"pfl1jetDeltaRb1/F");
   zeetree->Branch("pfl1jetID2",&pfl1jetID2,"pfl1jetID2/I");
   zeetree->Branch("pfl1jetet2",&pfl1jetet2,"pfl1jetet2/F"); 
   zeetree->Branch("pfl1jetpt2",&pfl1jetpt2,"pfl1jetpt2/F");
   zeetree->Branch("pfl1jeteta2",&pfl1jeteta2,"pfl1jeteta2/F");
   zeetree->Branch("pfl1jetphi2",&pfl1jetphi2,"pfl1jetphi2/F");
   zeetree->Branch("pfl1jetcharge2",&pfl1jetcharge2,"pfl1jetcharge2/F");
   zeetree->Branch("pfl1jetDeltaRa2",&pfl1jetDeltaRa2,"pfl1jetDeltaRa2/F");
   zeetree->Branch("pfl1jetDeltaRb2",&pfl1jetDeltaRb2,"pfl1jetDeltaRb2/F");
   zeetree->Branch("pfl1jetID3",&pfl1jetID3,"pfl1jetID3/I");
   zeetree->Branch("pfl1jetet3",&pfl1jetet3,"pfl1jetet3/F"); 
   zeetree->Branch("pfl1jetpt3",&pfl1jetpt3,"pfl1jetpt3/F");
   zeetree->Branch("pfl1jeteta3",&pfl1jeteta3,"pfl1jeteta3/F");
   zeetree->Branch("pfl1jetphi3",&pfl1jetphi3,"pfl1jetphi3/F");
   zeetree->Branch("pfl1jetcharge3",&pfl1jetcharge3,"pfl1jetcharge3/F");
   zeetree->Branch("pfl1jetDeltaRa3",&pfl1jetDeltaRa3,"pfl1jetDeltaRa3/F");
   zeetree->Branch("pfl1jetDeltaRb3",&pfl1jetDeltaRb3,"pfl1jetDeltaRb3/F");
   zeetree->Branch("DeltaR_GenJetPFL1Jet",&DeltaR_GenJetPFL1Jet,"DeltaR_GenJetPFL1Jet/F");
   
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
  
  delete ch;
  
  double lumi = _entries/_xsec;

  if(_Norm && lumi!=0){
   _weight = _targetLumi/lumi;
   }  
 
}

 void  RecoElectronNtuple::zero_ntuple(){
 
   //zeros everything
   
   run=-1;
   event=-1;
   lumi=-1;
   TrgBit=-1;
   OneElTrgMatch=-1;
   
   elTrg1=-1;
   elTrg2=-1;
   elTrg3=-1;
   elTrg4=-1;
   elTrg5=-1;
   elTrg6=-1;
   elTrg7=-1;
   elTrg8=-1;
   elTrg9=-1;
   elTrg10=-1;
   
   cutAccSYM=-1;
   cutAccASYM=-1;
   cutTrg=-1;
   cutImp=-1;
   cutConvASYM=-1;
   cutIsoSYM=-1;
   cutIsoASYM=-1;
   cutEiDSYM=-1;
   cutEiDASYM=-1;
   
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
   elereliso1=-99.;
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
   ele1mapID80cIso=-1;
   elept2=-99.;
   eleeta2=-99.;
   elephi2 = -99.;
   elecharge2 = -99.;
   elehcaliso2=-99.;
   eleecaliso2=-99.;
   eletrackiso2=-99.;
   elereliso2=-99.;
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
   ele2mapID95cIso=-1;
   numberOfZ = -1;
   nelesall = -1;
   neles = -1;

   npfjetsele = -1;
   genacc_npfjetsele = -1;
   pfjetID1=-1;
   pfjetet1=0.;
   pfjetpt1=-99.;
   pfjeteta1=-99.;
   pfjetphi1=-99.;
   pfjetcharge1=-99.;
   pfjetDeltaRa1=-99.;
   pfjetDeltaRb1=-99.;
   pfjetEmFrac1=-99.;
   pfjetID2=-1;
   pfjetet2=0.;
   pfjetpt2=-99.;
   pfjeteta2=-99.;
   pfjetphi2=-99.;
   pfjetcharge2=-99.;
   pfjetDeltaRa2=-99.;
   pfjetDeltaRb2=-99.;
   pfjetEmFrac2=-99.;
   pfjetID3=-1;
   pfjetet3=0.;
   pfjetpt3=-99.;
   pfjeteta3=-99.;
   pfjetphi3=-99.;
   pfjetcharge3=-99.;
   pfjetDeltaRa3=-99.;
   pfjetDeltaRb3=-99.;
   pfjetEmFrac3=-99.;
      
   DeltaR_GenJetPFJet=-99;
   DeltaR_GenJetPFL1Jet=-99;
      
   npfl1jetsele=-1;
   genacc_npfl1jetsele=-1;
   pfl1jetID1=-1;
   pfl1jetet1=0.;
   pfl1jetpt1=-99.;
   pfl1jeteta1=-99.;
   pfl1jetphi1=-99.;
   pfl1jetcharge1=-99.;
   pfl1jetDeltaRa1=-99.;
   pfl1jetDeltaRb1=-99.;
   pfl1jetID2=-1;
   pfl1jetet2=0.;
   pfl1jetpt2=-99.;
   pfl1jeteta2=-99.;
   pfl1jetphi2=-99.;
   pfl1jetcharge2=-99.;
   pfl1jetDeltaRa2=-99.;
   pfl1jetDeltaRb2=-99.;
   pfl1jetID3=-1;
   pfl1jetet3=0.;
   pfl1jetpt3=-99.;
   pfl1jeteta3=-99.;
   pfl1jetphi3=-99.;
   pfl1jetcharge3=-99.;
   pfl1jetDeltaRa3=-99.;
   pfl1jetDeltaRb3=-99.;

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
   if(_sample=="mc")run=-1;
   event     = iEvent.id().event();
   lumi      = iEvent.luminosityBlock();

   // generators quantities

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zgenHandle;
   zgenHandle.getByLabel(iEvent, "zeegenfull");

   fwlite::Handle<std::vector<reco::GenJet> > jetgenHandle;
   jetgenHandle.getByLabel(iEvent, "selectedGenJets");
   
   std::vector<const reco::GenJet*> isogenjets;
   
   if(_sample=="mc" && zgenHandle->size()!=0){
   
   std::vector<const reco::Candidate*> zgendaughters;
   const reco::Candidate *gendau0 = 0;
   const reco::Candidate *gendau1 = 0;
   
   zgendaughters = ZGENDaughters((*zgenHandle)[0]);
   std::vector<const reco::GenJet*> genjets = GetJets_GenJets<reco::GenJet>(*jetgenHandle);
   
   if(zgendaughters.size()){
   gendau0 = zgendaughters[0];
   gendau1 = zgendaughters[1];
   for(unsigned int i = 0; i < genjets.size(); i++){
   if(IsoJet<reco::Candidate>(zgendaughters,*genjets[i]))isogenjets.push_back(genjets[i]);}
   }else if(!zgendaughters.size()){
   for(unsigned int i = 0; i < genjets.size(); i++)isogenjets.push_back(genjets[i]);}

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
      
   }  

   if (GenSelectedInAcceptance((*zgenHandle)[0], _selections)&&zgendaughters.size()){
   
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

      if(isogenjets.size()>0){
       acc_jetgenpt1=isogenjets[0]->pt();
       acc_jetgeneta1=isogenjets[0]->eta();
       acc_jetgenphi1=isogenjets[0]->phi();
       acc_jetgencharge1=isogenjets[0]->charge();
      }
      if(isogenjets.size()>1){
       acc_jetgenpt2=isogenjets[1]->pt();
       acc_jetgeneta2=isogenjets[1]->eta();
       acc_jetgenphi2=isogenjets[1]->phi();
       acc_jetgencharge2=isogenjets[1]->charge();
      }
      if(isogenjets.size()>2){
       acc_jetgenpt3=isogenjets[2]->pt();
       acc_jetgeneta3=isogenjets[2]->eta();
       acc_jetgenphi3=isogenjets[2]->phi();
       acc_jetgencharge3=isogenjets[2]->charge();
      }

   } // end GEN Z selected in acceptance

}

   //Reconstructed quantities
   
   fwlite::Handle<std::vector<pat::Electron> > electronHandle;
   electronHandle.getByLabel(iEvent, "selectedElectrons");

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zrecHandle;
   zrecHandle.getByLabel(iEvent, "zeerec");
   
   fwlite::Handle<std::vector<pat::Jet> > pfjetrecHandle;
   pfjetrecHandle.getByLabel(iEvent, "selectedJets");
   
   fwlite::Handle<std::vector<pat::Jet> > pfl1jetrecHandle;
   pfl1jetrecHandle.getByLabel(iEvent, "selectedJetsL1Corrected");

   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");

   numberOfZ=zrecHandle->size();
   nelesall=electronHandle->size();    // all electrons with loose cuts
   
   // Z REC events
   
     std::vector<const pat::Jet*> pfrecjets = GetJets_GenJets<pat::Jet>(*pfjetrecHandle);
     std::vector<const pat::Jet*> pfl1recjets = GetJets_GenJets<pat::Jet>(*pfl1jetrecHandle);

     std::vector<const pat::Electron*> zrecdaughters; 
     
     const pat::Electron *recdau0 = 0;
     const pat::Electron *recdau1 = 0;

     std::vector<const pat::Jet*> pfisorecjets;
     std::vector<const pat::Jet*> pfl1isorecjets;
        
     if(zrecHandle->size()){
     zrecdaughters = ZRECDaughters((*zrecHandle)[0]);}
     
     if(zrecdaughters.size()){
     recdau0 = zrecdaughters[0];
     recdau1 = zrecdaughters[1];
     for(unsigned int i = 0; i < pfrecjets.size(); i++){     
     if(IsoJet<pat::Electron>(zrecdaughters,*pfrecjets[i]))pfisorecjets.push_back(pfrecjets[i]);}
     for(unsigned int i = 0; i < pfl1recjets.size(); i++){     
     if(IsoJet<pat::Electron>(zrecdaughters,*pfl1recjets[i]))pfl1isorecjets.push_back(pfl1recjets[i]);}
     }else if(!zrecdaughters.size()){
     for(unsigned int i = 0; i < pfrecjets.size(); i++)pfisorecjets.push_back(pfrecjets[i]);
     for(unsigned int i = 0; i < pfl1recjets.size(); i++)pfl1isorecjets.push_back(pfl1recjets[i]);}
     
     //Reco Jets isolated from GEN Z electrons in Gen Acceptance
     std::vector<const pat::Jet*> geniso_pfisorecjets;
     std::vector<const pat::Jet*> geniso_pfl1isorecjets;
     if(_sample=="mc" && zgenHandle->size()!=0){
     std::vector<const reco::Candidate*> zgendaughters = ZGENDaughters((*zgenHandle)[0]);
     if(zgendaughters.size()){
     for(unsigned int i = 0; i < pfrecjets.size(); i++){
     if(IsoJet<reco::Candidate>(zgendaughters,*pfrecjets[i]))geniso_pfisorecjets.push_back(pfrecjets[i]);}
     for(unsigned int i = 0; i < pfl1recjets.size(); i++){     
     if(IsoJet<reco::Candidate>(zgendaughters,*pfl1recjets[i]))geniso_pfl1isorecjets.push_back(pfl1recjets[i]);}
     }else if(!zgendaughters.size()){
     for(unsigned int i = 0; i < pfrecjets.size(); i++)geniso_pfisorecjets.push_back(pfrecjets[i]);
     for(unsigned int i = 0; i < pfl1recjets.size(); i++)geniso_pfl1isorecjets.push_back(pfl1recjets[i]);}
     
     if (GenSelectedInAcceptance((*zgenHandle)[0], _selections)&&zgendaughters.size()){
     genacc_npfjetsele=geniso_pfisorecjets.size();
     genacc_npfl1jetsele=geniso_pfl1isorecjets.size();}
     
     }//end if(GEN _sample)
   
   // loose cuts, only acceptance cuts
   
   if(zrecHandle->size()){
   
   if(isTriggerAvailable(*triggerHandle, _elTrigVector[0].c_str()))elTrg1=1;
   if(isTriggerAvailable(*triggerHandle, _elTrigVector[1].c_str()))elTrg2=1;
   if(isTriggerAvailable(*triggerHandle, _elTrigVector[2].c_str()))elTrg3=1;
   if(isTriggerAvailable(*triggerHandle, _elTrigVector[3].c_str()))elTrg4=1;
   if(isTriggerAvailable(*triggerHandle, _elTrigVector[4].c_str()))elTrg5=1;
   if(isTriggerAvailable(*triggerHandle, _elTrigVector[5].c_str()))elTrg6=1;
   if(isTriggerAvailable(*triggerHandle, _elTrigVector[6].c_str()))elTrg7=1;
   if(isTriggerAvailable(*triggerHandle, _elTrigVector[7].c_str()))elTrg8=1;
   if(isTriggerAvailable(*triggerHandle, _elTrigVector[8].c_str()))elTrg9=1;
   if(isTriggerAvailable(*triggerHandle, _elTrigVector[9].c_str()))elTrg10=1;
   
   if(isElTriggerAvailable(*triggerHandle, run)){
   TrgBit=0;
   if(isElectronTriggered(*triggerHandle, run)){
   TrgBit=1;
   if(RecSelected_TrgMatch(*recdau0, run)||RecSelected_TrgMatch(*recdau1, run))OneElTrgMatch=1;
   }
   }
   
   //Cut flags
   
   if (RecSelected("_AccSYM", (*zrecHandle)[0], *triggerHandle, run))cutAccSYM=1;
   if (RecSelected("_AccASYM", (*zrecHandle)[0], *triggerHandle, run))cutAccASYM=1;
   if (RecSelected("_Trg", (*zrecHandle)[0], *triggerHandle, run))cutTrg=1;
   if (RecSelected("_Imp", (*zrecHandle)[0], *triggerHandle, run))cutImp=1;
   if (RecSelected("_ConvASYM", (*zrecHandle)[0], *triggerHandle, run))cutConvASYM=1;
   if (RecSelected("_IsoSYM", (*zrecHandle)[0], *triggerHandle, run))cutIsoSYM=1;
   if (RecSelected("_IsoASYM", (*zrecHandle)[0], *triggerHandle, run))cutIsoASYM=1;
   if (RecSelected("_EiDSYM", (*zrecHandle)[0], *triggerHandle, run))cutEiDSYM=1;
   if (RecSelected("_EiDASYM", (*zrecHandle)[0], *triggerHandle, run))cutEiDASYM=1;
   
   if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, run)){
   
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
      elereliso1=(recdau0->trackIso() + recdau0->ecalIso() + recdau0->hcalIso()) / recdau0->pt();
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
      ele1mapID80cIso= recdau0->electronID("simpleEleId80cIso"); 

      elept2=recdau1->pt();
      eleeta2=recdau1->eta();
      elephi2=recdau1->phi();
      elecharge2=recdau1->charge();
      elehcaliso2=recdau1->hcalIso();
      eleecaliso2=recdau1->ecalIso();
      eletrackiso2=recdau1->trackIso();    
      elereliso2=(recdau1->trackIso() + recdau1->ecalIso() + recdau1->hcalIso()) / recdau1->pt();
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
      ele2mapID95cIso= recdau1->electronID("simpleEleId95cIso"); 
     
      npfjetsele=pfisorecjets.size();
      npfl1jetsele=pfl1isorecjets.size();
         
      // fill ntuples for jets
      
      if (pfisorecjets.size()>0){
        pfjetID1=jetID(pfisorecjets[0]);
        pfjetet1=pfisorecjets[0]->et();
        pfjetpt1=pfisorecjets[0]->pt();
        pfjeteta1=pfisorecjets[0]->eta();
        pfjetphi1=pfisorecjets[0]->phi();
        pfjetcharge1=pfisorecjets[0]->jetCharge();
        pfjetDeltaRa1=Delta_R(*recdau0, *pfisorecjets[0]);
        pfjetDeltaRb1=Delta_R(*recdau1, *pfisorecjets[0]);
	
	//DeltaR leading Gen jet - leading PF jet
	if(acc_jetgenphi1>-10 && pfjetphi1>-10){
	double DeltaPhi_PF = 0;
	if((TMath::Abs(acc_jetgenphi1 - pfjetphi1) < TMath::Pi()) && (TMath::Abs(acc_jetgenphi1 - pfjetphi1) > -(TMath::Pi()))){
	DeltaPhi_PF = acc_jetgenphi1 - pfjetphi1;
	}	
	else if((acc_jetgenphi1 - pfjetphi1) > TMath::Pi() ){
	DeltaPhi_PF = (2*TMath::Pi()) - (acc_jetgenphi1 - pfjetphi1);
	}	
        else if((acc_jetgenphi1 - pfjetphi1) < -(TMath::Pi())){
        DeltaPhi_PF = (2*TMath::Pi()) + (acc_jetgenphi1 - pfjetphi1);
	}
        double DeltaEta_PF = acc_jetgeneta1 - pfjeteta1;
	DeltaR_GenJetPFJet = sqrt(pow(DeltaPhi_PF,2) + pow(DeltaEta_PF,2));
	}

      }
      if (pfisorecjets.size()>1){
        pfjetID2=jetID(pfisorecjets[1]);
        pfjetet2=pfisorecjets[1]->et();
        pfjetpt2=pfisorecjets[1]->pt();
        pfjeteta2=pfisorecjets[1]->eta();
        pfjetphi2=pfisorecjets[1]->phi();
        pfjetcharge2=pfisorecjets[1]->jetCharge();
        pfjetDeltaRa2=Delta_R(*recdau0, *pfisorecjets[1]);
        pfjetDeltaRb2=Delta_R(*recdau1, *pfisorecjets[1]);
      }
      if (pfisorecjets.size()>2){
        pfjetID3=jetID(pfisorecjets[0]);
        pfjetet3=pfisorecjets[2]->et();
        pfjetpt3=pfisorecjets[2]->pt();
        pfjeteta3=pfisorecjets[2]->eta();
        pfjetphi3=pfisorecjets[2]->phi();
        pfjetcharge3=pfisorecjets[2]->jetCharge();
        pfjetDeltaRa3=Delta_R(*recdau0, *pfisorecjets[2]);
        pfjetDeltaRb3=Delta_R(*recdau1, *pfisorecjets[2]);
      }
      
      if (pfl1isorecjets.size()>0){
        pfl1jetID1=jetID(pfl1isorecjets[0]);
        pfl1jetet1=pfl1isorecjets[0]->et();
        pfl1jetpt1=pfl1isorecjets[0]->pt();
        pfl1jeteta1=pfl1isorecjets[0]->eta();
        pfl1jetphi1=pfl1isorecjets[0]->phi();
        pfl1jetcharge1=pfl1isorecjets[0]->jetCharge();
        pfl1jetDeltaRa1=Delta_R(*recdau0, *pfl1isorecjets[0]);
        pfl1jetDeltaRb1=Delta_R(*recdau1, *pfl1isorecjets[0]);
	
	//DeltaR leading Gen jet - leading PFL1 jet
	if(acc_jetgenphi1>-10 && pfl1jetphi1>-10){
	double DeltaPhi_PFL1 = 0;
	if((TMath::Abs(acc_jetgenphi1 - pfl1jetphi1) < TMath::Pi()) && (TMath::Abs(acc_jetgenphi1 - pfl1jetphi1) > -(TMath::Pi()))){
	DeltaPhi_PFL1 = acc_jetgenphi1 - pfl1jetphi1;
	}	
	else if((acc_jetgenphi1 - pfl1jetphi1) > TMath::Pi() ){
	DeltaPhi_PFL1 = (2*TMath::Pi()) - (acc_jetgenphi1 - pfl1jetphi1);
	}	
        else if((acc_jetgenphi1 - pfl1jetphi1) < -(TMath::Pi())){
        DeltaPhi_PFL1 = (2*TMath::Pi()) + (acc_jetgenphi1 - pfl1jetphi1);
	}
        double DeltaEta_PFL1 = acc_jetgeneta1 - pfl1jeteta1;
	DeltaR_GenJetPFL1Jet = sqrt(pow(DeltaPhi_PFL1,2) + pow(DeltaEta_PFL1,2));
	}
      }
      
      if (pfl1isorecjets.size()>1){
        pfl1jetID2=jetID(pfl1isorecjets[1]);
        pfl1jetet2=pfl1isorecjets[1]->et();
        pfl1jetpt2=pfl1isorecjets[1]->pt();
        pfl1jeteta2=pfl1isorecjets[1]->eta();
        pfl1jetphi2=pfl1isorecjets[1]->phi();
        pfl1jetcharge2=pfl1isorecjets[1]->jetCharge();
        pfl1jetDeltaRa2=Delta_R(*recdau0, *pfl1isorecjets[1]);
        pfl1jetDeltaRb2=Delta_R(*recdau1, *pfl1isorecjets[1]);
      }
      if (pfl1isorecjets.size()>2){
        pfl1jetID3=jetID(pfl1isorecjets[2]);
        pfl1jetet3=pfl1isorecjets[2]->et();
        pfl1jetpt3=pfl1isorecjets[2]->pt();
        pfl1jeteta3=pfl1isorecjets[2]->eta();
        pfl1jetphi3=pfl1isorecjets[2]->phi();
        pfl1jetcharge3=pfl1isorecjets[2]->jetCharge();
        pfl1jetDeltaRa3=Delta_R(*recdau0, *pfl1isorecjets[2]);
        pfl1jetDeltaRb3=Delta_R(*recdau1, *pfl1isorecjets[2]);
      }
      
      } // endif loose selected Z


   //stricter cuts
      
      if (RecSelected(_RecoCutFlags[1].c_str(), (*zrecHandle)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[2].c_str(), (*zrecHandle)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[3].c_str(), (*zrecHandle)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[4].c_str(), (*zrecHandle)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[5].c_str(), (*zrecHandle)[0], *triggerHandle, run)&&RecSelected(_RecoCutFlags[6].c_str(), (*zrecHandle)[0], *triggerHandle, run)){
  
      zmass_AllCuts=(*zrecHandle)[0].mass();
      
   }  // endif strict selected Z
   
   }
   
   if(_NtupleFill=="zcand"){
   if(zmass_AllCuts>0.)zeetree->Fill();
   }else if(_NtupleFill=="acc"){
   if(zmass>0.)zeetree->Fill();
   }else if(_NtupleFill=="all"){
   zeetree->Fill();}
   
     
}

void RecoElectronNtuple::finalize(){
   
  _file->Write();

}
