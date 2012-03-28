#include "Firenze/PATAnalysis/include/RecoMuonNtuple.h"
#include "Firenze/PATAnalysis/include/MuonUtilities.h"

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
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;


RecoMuonNtuple::RecoMuonNtuple(): 
 
  _sample("mc"), _selections("ASYM"), _NtupleFill("zcand"), _ptjetmin(30.), _etajetmax(3.), _isocut(0.1), _weight(1.), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1), _Acc(1), _Trg(0), _Qual(2), _Imp(3), _Iso(4), _MuID(0), _JECUnc(0), _JECUncFilePath("none"), _jecUnc(0), _JetType("PFL1CORRnew"), _Norm(false)

{ }

void RecoMuonNtuple::begin(TFile* out, const edm::ParameterSet& iConfig){

   _file = out; 
  
   std::string dirname = iConfig.getParameter<std::string>("Name");
   std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
   _selections = iConfig.getParameter<std::string>("Selections");
   _JetType = iConfig.getParameter<std::string>("JetType");
   _sample = iConfig.getParameter<std::string>("Sample");
   _targetLumi= iConfig.getParameter<double>("targetLumi");
   _xsec      = iConfig.getParameter<double>("CrossSection");
   _Norm      = iConfig.getParameter<bool>("Norm");
   _NtupleFill      = iConfig.getParameter<std::string>("NtupleFill");
   _EventsPerFile    = iConfig.getParameter<int32_t>("EventsPerFile");
   _EventNumber    = iConfig.getParameter<int32_t>("EventNumber");
   _ProcEvents    = iConfig.getParameter<int32_t>("ProcEvents");
   _JECUnc    = iConfig.getParameter<int32_t>("JECUnc");
   _JECUncFilePath      = iConfig.getParameter<std::string>("JECUncFilePath");
   
   if(_JECUnc!=0) _jecUnc = new JetCorrectionUncertainty(_JECUncFilePath.c_str());
   
   //Selections
   _Acc = iConfig.getParameter<int32_t>("Acc");
   _Trg = iConfig.getParameter<int32_t>("Trg");
   _Qual = iConfig.getParameter<int32_t>("Qual");
   _Imp = iConfig.getParameter<int32_t>("Imp");
   _Iso = iConfig.getParameter<int32_t>("Iso");
   _MuID = iConfig.getParameter<int32_t>("MuID");
   
   for(int i=0; i<7; i++){
   	_RecoCutFlags[i] = "_1";
   }
   
   if(_selections=="SYM"){
   	_RecoCutFlags[_Acc] =  "_AccSYM";
   	_RecoCutFlags[_Qual] = "_QualSYM";
  	_RecoCutFlags[_Iso] =  "_IsoSYM";
   	_RecoCutFlags[_MuID] =  "_MuIDSYM";
   }
   if(_selections=="ASYM"){
   	_RecoCutFlags[_Acc] =  "_AccASYM";
   	_RecoCutFlags[_Qual] = "_QualASYM"; 
   	_RecoCutFlags[_Iso] =  "_IsoASYM";
   	_RecoCutFlags[_MuID] =  "_MuIDASYM";
   }
     
   _RecoCutFlags[_Trg] =  "_Trg";  
   _RecoCutFlags[_Imp] =  "_Imp";

   cout << "RecoMuonNtuple file name : " << _file->GetName() << endl;

   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();

   zmumutree= new TTree("ZmumuTree","ZmumuTree");
   
   zmumutree->Branch("run",&run,"run/I");
   zmumutree->Branch("event",&event,"event/I");
   zmumutree->Branch("lumi",&lumi,"lumi/I");
   
   zmumutree->Branch("TrgBit",&TrgBit,"TrgBit/I");
   zmumutree->Branch("OneMuTrgMatch",&OneMuTrgMatch,"OneMuTrgMatch/I");
   
   static map<std::string, std::pair<int, int> > muTriggers = muTrigger();
   
   if(muTriggers.size()>10)throw cms::Exception("PATAnalysis:RecoMuonNtuple_TooManyTriggers") << "ERROR! Too Many Triggers! Modify muTrigVector size!";
   
   for(int i=0; i<10; i++){
   	_muTrigVector[i] = "_NoTrg";
   }
   
   if(muTriggers.size()<10){
   	int i = 0;
   	static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
   	for(TrgVectorIter = muTriggers.begin(); TrgVectorIter != muTriggers.end(); TrgVectorIter++){
   		_muTrigVector[i] = TrgVectorIter->first.c_str();
   		i++;
   	}
   }
   
   std::string muTrg1_I = _muTrigVector[0]+"/I";
   zmumutree->Branch(_muTrigVector[0].c_str(),&muTrg1,muTrg1_I.c_str());
   std::string muTrg2_I = _muTrigVector[1]+"/I";
   zmumutree->Branch(_muTrigVector[1].c_str(),&muTrg2,muTrg2_I.c_str());
   std::string muTrg3_I = _muTrigVector[2]+"/I";
   zmumutree->Branch(_muTrigVector[2].c_str(),&muTrg3,muTrg3_I.c_str());
   std::string muTrg4_I = _muTrigVector[3]+"/I";
   zmumutree->Branch(_muTrigVector[3].c_str(),&muTrg4,muTrg4_I.c_str());
   std::string muTrg5_I = _muTrigVector[4]+"/I";
   zmumutree->Branch(_muTrigVector[4].c_str(),&muTrg5,muTrg5_I.c_str());
   std::string muTrg6_I = _muTrigVector[5]+"/I";
   zmumutree->Branch(_muTrigVector[5].c_str(),&muTrg6,muTrg6_I.c_str());
   std::string muTrg7_I = _muTrigVector[6]+"/I";
   zmumutree->Branch(_muTrigVector[6].c_str(),&muTrg7,muTrg7_I.c_str());
   std::string muTrg8_I = _muTrigVector[7]+"/I";
   zmumutree->Branch(_muTrigVector[7].c_str(),&muTrg8,muTrg8_I.c_str());
   std::string muTrg9_I = _muTrigVector[8]+"/I";
   zmumutree->Branch(_muTrigVector[8].c_str(),&muTrg9,muTrg9_I.c_str());
   std::string muTrg10_I = _muTrigVector[9]+"/I";
   zmumutree->Branch(_muTrigVector[9].c_str(),&muTrg10,muTrg10_I.c_str());
    
   zmumutree->Branch("weight", &_weight, "weight/F");
   
   //cut flags
   zmumutree->Branch("cutAccSYM",&cutAccSYM,"cutAccSYM/I");
   zmumutree->Branch("cutAccASYM",&cutAccASYM,"cutAccASYM/I");
   zmumutree->Branch("cutTrg",&cutTrg,"cutTrg/I");
   zmumutree->Branch("cutImp",&cutImp,"cutImp/I");
   zmumutree->Branch("cutQualSYM",&cutQualSYM,"cutQualSYM/I");
   zmumutree->Branch("cutQualASYM",&cutQualASYM,"cutQualASYM/I");
   zmumutree->Branch("cutIsoSYM",&cutIsoSYM,"cutIsoSYM/I");
   zmumutree->Branch("cutIsoASYM",&cutIsoASYM,"cutIsoASYM/I");
   zmumutree->Branch("cutMuIDSYM",&cutMuIDSYM,"cutMuIDSYM/I");
   zmumutree->Branch("cutMuIDASYM",&cutMuIDASYM,"cutMuIDASYM/I");
   
   //quantities at generator level 
   zmumutree->Branch("ptzgen",&ptzgen,"ptzgen/F");
   zmumutree->Branch("etazgen",&etazgen,"etazgen/F");
   zmumutree->Branch("phizgen",&phizgen,"phizgen/F");
   zmumutree->Branch("zmassgen",&zmassgen,"zmassgen/F");
   zmumutree->Branch("mugenpt1",&mugenpt1,"mugenpt1/F");
   zmumutree->Branch("mugeneta1",&mugeneta1,"mugeneta1/F");
   zmumutree->Branch("mugenphi1",&mugenphi1,"mugenphi1/F");
   zmumutree->Branch("mugenpt2",&mugenpt2,"mugenpt2/F");
   zmumutree->Branch("mugeneta2",&mugeneta2,"mugeneta2/F");
   zmumutree->Branch("mugenphi2",&mugenphi2,"mugenphi2/F");
   zmumutree->Branch("gennjetsele",&gennjetsele,"gennjetsele/I");
   zmumutree->Branch("acc_ptzgen",&acc_ptzgen,"acc_ptzgen/F");
   zmumutree->Branch("acc_etazgen",&acc_etazgen,"acc_etazgen/F");
   zmumutree->Branch("acc_phizgen",&acc_phizgen,"acc_phizgen/F");
   zmumutree->Branch("acc_zmassgen",&acc_zmassgen,"zmassgen/F");
   zmumutree->Branch("acc_mugenpt1",&acc_mugenpt1,"acc_mugenpt1/F");
   zmumutree->Branch("acc_mugeneta1",&acc_mugeneta1,"acc_mugeneta1/F");
   zmumutree->Branch("acc_mugenphi1",&acc_mugenphi1,"acc_mugenphi1/F");
   zmumutree->Branch("acc_mugenpt2",&acc_mugenpt2,"acc_mugenpt2/F");
   zmumutree->Branch("acc_mugeneta2",&acc_mugeneta2,"acc_mugeneta2/F");
   zmumutree->Branch("acc_mugenphi2",&acc_mugenphi2,"acc_mugenphi2/F");
   zmumutree->Branch("acc_gennjetsele",&acc_gennjetsele,"acc_gennjetsele/I");
   zmumutree->Branch("acc_jetgenpt1",&acc_jetgenpt1,"acc_jetgenpt1/F");
   zmumutree->Branch("acc_jetgeneta1",&acc_jetgeneta1,"acc_jetgeneta1/F");
   zmumutree->Branch("acc_jetgenphi1",&acc_jetgenphi1,"acc_jetgenphi1/F");
   zmumutree->Branch("acc_jetgencharge1",&acc_jetgencharge1,"acc_jetgencharge1/F");
   zmumutree->Branch("acc_jetgenpt2",&acc_jetgenpt2,"acc_jetgenpt2/F");
   zmumutree->Branch("acc_jetgeneta2",&acc_jetgeneta2,"acc_jetgeneta2/F");
   zmumutree->Branch("acc_jetgenphi2",&acc_jetgenphi2,"acc_jetgenphi2/F");
   zmumutree->Branch("acc_jetgencharge2",&acc_jetgencharge2,"acc_jetgencharge2/F");
   zmumutree->Branch("acc_jetgenpt3",&acc_jetgenpt3,"acc_jetgenpt3/F");
   zmumutree->Branch("acc_jetgeneta3",&acc_jetgeneta3,"acc_jetgeneta3/F");
   zmumutree->Branch("acc_jetgenphi3",&acc_jetgenphi3,"acc_jetgenphi3/F");
   zmumutree->Branch("acc_jetgencharge3",&acc_jetgencharge3,"acc_jetgencharge3/F");

   //reconstructed quantities
   zmumutree->Branch("rho",&rho,"rho/F");
   zmumutree->Branch("numberOfZ",&numberOfZ,"numberOfZ/I");
   zmumutree->Branch("nmusall",&nmusall,"nmusall/I");
   zmumutree->Branch("nmus",&nmus,"nmus/I");
   zmumutree->Branch("zmass",&zmass,"zmass/F");
   zmumutree->Branch("zmass_AllCuts",&zmass_AllCuts,"zmass_AllCuts/F");
   zmumutree->Branch("zpt",&zpt,"zpt/F");
   zmumutree->Branch("zeta",&zeta,"zeta/F");
   zmumutree->Branch("zphi",&zphi,"zphi/F");
   zmumutree->Branch("mupt1",&mupt1,"mupt1/F");
   zmumutree->Branch("mueta1",&mueta1,"mueta1/F");
   zmumutree->Branch("muphi1",&muphi1,"muphi1/F");
   zmumutree->Branch("mucharge1",&mucharge1,"mucharge1/F");  
   zmumutree->Branch("mudB1",&mudB1,"mudB1/F");
   zmumutree->Branch("mupt2",&mupt2,"mupt2/F");
   zmumutree->Branch("mueta2",&mueta2,"mueta2/F");
   zmumutree->Branch("muphi2",&muphi2,"muphi2/F");
   zmumutree->Branch("mucharge2",&mucharge2,"mucharge2/F");   
   zmumutree->Branch("mudB2",&mudB2,"mudB2/F");
   
   zmumutree->Branch("npfjetsele",&npfjetsele,"npfjetsele/I");
   zmumutree->Branch("genacc_npfjetsele",&genacc_npfjetsele,"genacc_npfjetsele/I");
   zmumutree->Branch("pfjetID1",&pfjetID1,"pfjetID1/I");
   zmumutree->Branch("pfjetet1",&pfjetet1,"pfjetet1/F"); 
   zmumutree->Branch("pfjetpt1",&pfjetpt1,"pfjetpt1/F");
   zmumutree->Branch("pfjeteta1",&pfjeteta1,"pfjeteta1/F");
   zmumutree->Branch("pfjetphi1",&pfjetphi1,"pfjetphi1/F");
   zmumutree->Branch("pfjetcharge1",&pfjetcharge1,"pfjetcharge1/F");
   zmumutree->Branch("pfjetDeltaRa1",&pfjetDeltaRa1,"pfjetDeltaRa1/F");
   zmumutree->Branch("pfjetDeltaRb1",&pfjetDeltaRb1,"pfjetDeltaRb1/F");
   zmumutree->Branch("pfjetEmFrac1",&pfjetEmFrac1,"pfjetEmFrac1/F");
   zmumutree->Branch("pfjetID2",&pfjetID2,"pfjetID2/I");
   zmumutree->Branch("pfjetet2",&pfjetet2,"pfjetet2/F"); 
   zmumutree->Branch("pfjetpt2",&pfjetpt2,"pfjetpt2/F");
   zmumutree->Branch("pfjeteta2",&pfjeteta2,"pfjeteta2/F");
   zmumutree->Branch("pfjetphi2",&pfjetphi2,"pfjetphi2/F");
   zmumutree->Branch("pfjetcharge2",&pfjetcharge2,"pfjetcharge2/F");
   zmumutree->Branch("pfjetDeltaRa2",&pfjetDeltaRa2,"pfjetDeltaRa2/F");
   zmumutree->Branch("pfjetDeltaRb2",&pfjetDeltaRb2,"pfjetDeltaRb2/F");
   zmumutree->Branch("pfjetEmFrac2",&pfjetEmFrac2,"pfjetEmFrac2/F");
   zmumutree->Branch("pfjetID3",&pfjetID3,"pfjetID3/I");
   zmumutree->Branch("pfjetet3",&pfjetet3,"pfjetet3/F"); 
   zmumutree->Branch("pfjetpt3",&pfjetpt3,"pfjetpt3/F");
   zmumutree->Branch("pfjeteta3",&pfjeteta3,"pfjeteta3/F");
   zmumutree->Branch("pfjetphi3",&pfjetphi3,"pfjetphi3/F");
   zmumutree->Branch("pfjetcharge3",&pfjetcharge3,"pfjetcharge3/F");
   zmumutree->Branch("pfjetDeltaRa3",&pfjetDeltaRa3,"pfjetDeltaRa3/F");
   zmumutree->Branch("pfjetDeltaRb3",&pfjetDeltaRb3,"pfjetDeltaRb3/F");
   zmumutree->Branch("pfjetEmFrac3",&pfjetEmFrac3,"pfjetEmFrac3/F");
   zmumutree->Branch("DeltaR_GenJetPFJet",&DeltaR_GenJetPFJet,"DeltaR_GenJetPFJet/F");
   
   zmumutree->Branch("npf2jetsele",&npf2jetsele,"npf2jetsele/I");
   zmumutree->Branch("genacc_npf2jetsele",&genacc_npf2jetsele,"genacc_npf2jetsele/I");
   zmumutree->Branch("pf2jetID1",&pf2jetID1,"pf2jetID1/I"); 
   zmumutree->Branch("pf2jetet1",&pf2jetet1,"pf2jetet1/F"); 
   zmumutree->Branch("pf2jetpt1",&pf2jetpt1,"pf2jetpt1/F");
   zmumutree->Branch("pf2jeteta1",&pf2jeteta1,"pf2jeteta1/F");
   zmumutree->Branch("pf2jetphi1",&pf2jetphi1,"pf2jetphi1/F");
   zmumutree->Branch("pf2jecUnc1",&pf2jecUnc1,"pf2jecUnc1/F");
   zmumutree->Branch("pf2jetcharge1",&pf2jetcharge1,"pf2jetcharge1/F");
   zmumutree->Branch("pf2jetDeltaRa1",&pf2jetDeltaRa1,"pf2jetDeltaRa1/F");
   zmumutree->Branch("pf2jetDeltaRb1",&pf2jetDeltaRb1,"pf2jetDeltaRb1/F");
   zmumutree->Branch("pf2jetID2",&pf2jetID2,"pf2jetID2/I");
   zmumutree->Branch("pf2jetet2",&pf2jetet2,"pf2jetet2/F"); 
   zmumutree->Branch("pf2jetpt2",&pf2jetpt2,"pf2jetpt2/F");
   zmumutree->Branch("pf2jeteta2",&pf2jeteta2,"pf2jeteta2/F");
   zmumutree->Branch("pf2jetphi2",&pf2jetphi2,"pf2jetphi2/F");
   zmumutree->Branch("pf2jecUnc2",&pf2jecUnc2,"pf2jecUnc2/F");
   zmumutree->Branch("pf2jetcharge2",&pf2jetcharge2,"pf2jetcharge2/F");
   zmumutree->Branch("pf2jetDeltaRa2",&pf2jetDeltaRa2,"pf2jetDeltaRa2/F");
   zmumutree->Branch("pf2jetDeltaRb2",&pf2jetDeltaRb2,"pf2jetDeltaRb2/F");
   zmumutree->Branch("pf2jetID3",&pf2jetID3,"pf2jetID3/I");
   zmumutree->Branch("pf2jetet3",&pf2jetet3,"pf2jetet3/F"); 
   zmumutree->Branch("pf2jetpt3",&pf2jetpt3,"pf2jetpt3/F");
   zmumutree->Branch("pf2jeteta3",&pf2jeteta3,"pf2jeteta3/F");
   zmumutree->Branch("pf2jetphi3",&pf2jetphi3,"pf2jetphi3/F");
   zmumutree->Branch("pf2jecUnc3",&pf2jecUnc3,"pf2jecUnc3/F");
   zmumutree->Branch("pf2jetcharge3",&pf2jetcharge3,"pf2jetcharge3/F");
   zmumutree->Branch("pf2jetDeltaRa3",&pf2jetDeltaRa3,"pf2jetDeltaRa3/F");
   zmumutree->Branch("pf2jetDeltaRb3",&pf2jetDeltaRb3,"pf2jetDeltaRb3/F");
   zmumutree->Branch("DeltaR_GenJetPFL1Jet",&DeltaR_GenJetPFL1Jet,"DeltaR_GenJetPFL1Jet/F");
   
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

void  RecoMuonNtuple::zero_ntuple(){
 
   //zeros everything
   
   run=-1;
   event=-1;
   lumi=-1;
   TrgBit=-1;
   OneMuTrgMatch=-1;
   rho=-1.;
   
   muTrg1=-1;
   muTrg2=-1;
   muTrg3=-1;
   muTrg4=-1;
   muTrg5=-1;
   muTrg6=-1;
   muTrg7=-1;
   muTrg8=-1;
   muTrg9=-1;
   muTrg10=-1;
   
   cutAccSYM=-1;
   cutAccASYM=-1;
   cutTrg=-1;
   cutImp=-1;
   cutQualSYM=-1;
   cutQualASYM=-1;
   cutIsoSYM=-1;
   cutIsoASYM=-1;
   cutMuIDSYM=-1;
   cutMuIDASYM=-1;
   
   ptzgen=-99.;
   etazgen=-99.;
   phizgen=-99.;
   zmassgen=-99.;
   mugenpt1=-99.;
   mugeneta1=-99.;
   mugenphi1=-99.;
   mugenpt2=-99.;
   mugeneta2=-99.;
   mugenphi2=-99.;
   gennjetsele=-1;

   acc_ptzgen=-99.;
   acc_etazgen=-99.;
   acc_phizgen=-99.;
   acc_zmassgen=-99.;
   acc_mugenpt1=-99.;
   acc_mugeneta1=-99.;
   acc_mugenphi1=-99.;
   acc_mugenpt2=-99.;
   acc_mugeneta2=-99.;
   acc_mugenphi2=-99.;
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
   mupt1=-99.;
   mueta1=-99.;
   muphi1 = -99.;
   mucharge1 = -99.;  
   mudB1 = -99.;
   
   mupt2=-99.;
   mueta2=-99.;
   muphi2 = -99.;
   mucharge2 = -99.; 
   mudB2 = -99.;
  
   numberOfZ = -1;
   nmusall = -1;
   nmus = -1;

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
      
   npf2jetsele=-1;
   genacc_npf2jetsele=-1;
   pf2jetID1=-1;
   pf2jetet1=0.;
   pf2jetpt1=-99.;
   pf2jeteta1=-99.;
   pf2jetphi1=-99.;
   pf2jecUnc1=-99.;
   pf2jetcharge1=-99.;
   pf2jetDeltaRa1=-99.;
   pf2jetDeltaRb1=-99.;
   pf2jetID2=-1;
   pf2jetet2=0.;
   pf2jetpt2=-99.;
   pf2jeteta2=-99.;
   pf2jetphi2=-99.;
   pf2jecUnc2=-99.;
   pf2jetcharge2=-99.;
   pf2jetDeltaRa2=-99.;
   pf2jetDeltaRb2=-99.;
   pf2jetID3=-1;
   pf2jetet3=0.;
   pf2jetpt3=-99.;
   pf2jeteta3=-99.;
   pf2jetphi3=-99.;
   pf2jecUnc3=-99.;
   pf2jetcharge3=-99.;
   pf2jetDeltaRa3=-99.;
   pf2jetDeltaRb3=-99.;

}


RecoMuonNtuple::~RecoMuonNtuple(){
  _file->ls();
}


void  RecoMuonNtuple::process(const fwlite::Event& iEvent){

   // zero everything
   zero_ntuple();

   _file->cd();
   
   run       = iEvent.id().run();
   if(_sample=="mc") run=-1;
   event     = iEvent.id().event();
   lumi      = iEvent.luminosityBlock();

   // generators quantities

   //----richiamo la GEN Z----
   fwlite::Handle<std::vector<reco::CompositeCandidate> > zgenHandle;
   //zgenHandle.getByLabel(iEvent, "zmumugen");
   zgenHandle.getByLabel(iEvent, "zmumugenfull");
   //seleziono un vettore di 2 muoni figli della GEN Z ordinati per pt
   std::vector<const reco::Candidate*> zgendaughters;
   const reco::Candidate *gendau0 = 0;
   const reco::Candidate *gendau1 = 0;
   if(_sample=="mc" && zgenHandle->size()){  
   	zgendaughters = ZGENDaughters(*zgenHandle);
   	if(zgendaughters.size()){
   		gendau0 = zgendaughters[0];
   		gendau1 = zgendaughters[1];
		if(gendau1->pt() > gendau0->pt())throw cms::Exception("PATAnalysis:RecoMuonNtuple_WrongMuonOrder") << "ERROR! Z GEN muons are in wrong order!";
   	}
   }

   //----richiamo i GEN Jets----
   fwlite::Handle<std::vector<reco::GenJet> > jetgenHandle;
   jetgenHandle.getByLabel(iEvent, "selectedGenJetsOld");
   //seleziono un vettore di GEN Jets per Accettanza
   std::vector<const reco::GenJet*> genjets = GetJets_noJetID<reco::GenJet>(*jetgenHandle);  
   //seleziono un vettore di GEN Jets Cleaned dai 2 muoni figli della GEN Z, qualora esistano 
   std::vector<const reco::GenJet*> isogenjets;
   if(_sample=="mc" && zgendaughters.size()){
   	for(unsigned int i = 0; i < genjets.size(); i++){
   		if(IsoJet<reco::Candidate>(zgendaughters,*genjets[i])) 
   			isogenjets.push_back(genjets[i]);
   	}
   }
   else if(!zgendaughters.size()){
   	for(unsigned int i = 0; i < genjets.size(); i++) 
   		isogenjets.push_back(genjets[i]);
   }


   // inizio a riempire le Ntuple per GEN
   if(_sample=="mc" && zgenHandle->size()){
	
	// Events with a GEN Z with mass cut  
   	if (GenSelected(*zgenHandle, _selections) && 
   	    zgendaughters.size()!=0){   

      		ptzgen=(*zgenHandle)[0].pt();
   		etazgen=(*zgenHandle)[0].eta();
   		phizgen=(*zgenHandle)[0].phi();
   		zmassgen=(*zgenHandle)[0].mass();
      
   		double genpt1= gendau0->pt();
   		double genpt2= gendau1->pt();
      
   		if(genpt2>genpt1)throw cms::Exception("PATAnalysis:RecoMuonNtuple_WrongMuonOrder") << "ERROR! Z muons are in wrong order!";
 
   		mugenpt1  = gendau0->pt(); 
   		mugeneta1 = gendau0->eta();
   		mugenphi1 = gendau0->phi();
   		mugenpt2  = gendau1->pt();
   		mugeneta2 = gendau1->eta();
   		mugenphi2 = gendau1->phi();
        
   		gennjetsele=isogenjets.size();
   	}  

	// Events with a GEN Z with mass cut and 2 GEN Muons in the Acceptance (geometrical and kinematic) region
   	if (GenSelectedInAcceptance(*zgenHandle, _selections) && 
   	    zgendaughters.size()){
   
   		acc_ptzgen=(*zgenHandle)[0].pt();
   		acc_etazgen=(*zgenHandle)[0].eta();
   		acc_phizgen=(*zgenHandle)[0].phi();
   		acc_zmassgen=(*zgenHandle)[0].mass();
      
   		double acc_genpt1= gendau0->pt();
   		double acc_genpt2= gendau1->pt();
      
   		if(acc_genpt2>acc_genpt1)throw cms::Exception("PATAnalysis:RecoMuonNtuple_WrongMuonOrder") << "ERROR! Z muons are in wrong order!";
      
   		acc_mugenpt1  = gendau0->pt(); 
   		acc_mugeneta1 = gendau0->eta();
   		acc_mugenphi1 = gendau0->phi();
   		acc_mugenpt2  = gendau1->pt();
   		acc_mugeneta2 = gendau1->eta();
   		acc_mugenphi2 = gendau1->phi();
	 
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

   	} //finisco di riempire le Ntuple per GEN
   }

   //Reconstructed quantities

   //----richiamo i REC Muons----   
   fwlite::Handle<std::vector<pat::Muon> > muonHandle;
   muonHandle.getByLabel(iEvent, "selectedMuons");

   //----richiamo la REC Z----
   fwlite::Handle<std::vector<reco::CompositeCandidate> > zrecHandle;
   zrecHandle.getByLabel(iEvent, "zmumurec");
   //seleziono un vettore di 2 muoni figli della REC Z ordinati per pt
   std::vector<const pat::Muon*> zrecdaughters; 
   const pat::Muon *recdau0 = 0;
   const pat::Muon *recdau1 = 0;
   if(zrecHandle->size()){
   	zrecdaughters = ZRECDaughters(*zrecHandle);
   	if(zrecdaughters.size()){
   		recdau0 = zrecdaughters[0];
   		recdau1 = zrecdaughters[1];
   		if(recdau1->pt() > recdau0->pt())throw cms::Exception("PATAnalysis:RecoMuon_WrongMuonOrder") << "ERROR! Z muons are in wrong order!";
	}
   }

   //----richiamo i REC Jets----     
   fwlite::Handle<std::vector<pat::Jet> > pfjetrecHandle;
   pfjetrecHandle.getByLabel(iEvent, "selectedJetsL1Corrected");
   //seleziono un vettore di jets REC per Accettanza e ID
   std::vector<const pat::Jet*> pfrecjets = GetJets<pat::Jet>(*pfjetrecHandle);
   //seleziono un vettore di jets REC con Cleaning dai 2 Muoni dalla Z REC, qualora esistano
   std::vector<const pat::Jet*> pfisorecjets;
   if(zrecdaughters.size()){
   	for(unsigned int i = 0; i < pfrecjets.size(); i++){     
   		if(IsoJet<pat::Muon>(zrecdaughters,*pfrecjets[i])) 
   			pfisorecjets.push_back(pfrecjets[i]);
   	}
   }
   else if(!zrecdaughters.size()){
   	for(unsigned int i = 0; i < pfrecjets.size(); i++)
   		pfisorecjets.push_back(pfrecjets[i]);
   }
   //seleziono un vettore di jets REC con Cleaning dai 2 Muoni dalla Z GEN nella Accettanza dei GEN, qualora esistano 
   std::vector<const pat::Jet*> geniso_pfisorecjets;
   if(_sample=="mc" && zgenHandle->size()!=0){
   	if(zgendaughters.size()){
     		for(unsigned int i = 0; i < pfrecjets.size(); i++){
     			if(IsoJet<reco::Candidate>(zgendaughters,*pfrecjets[i]))
     				geniso_pfisorecjets.push_back(pfrecjets[i]);
     		}
   	}
	else if(!zgendaughters.size()){
     		for(unsigned int i = 0; i < pfrecjets.size(); i++)
   			geniso_pfisorecjets.push_back(pfrecjets[i]);
        }
   }

   //----richiamo i REC Jets----     
   fwlite::Handle<std::vector<pat::Jet> > pf2jetrecHandle;
   pf2jetrecHandle.getByLabel(iEvent, "selectedJetsL1Corrected");
   //seleziono un vettore di jets REC per Accettanza e ID corretti per le JEC Uncertainties
   std::vector<const pat::Jet*> pf2recjets = GetJets_wJECUnc<pat::Jet>(*pf2jetrecHandle, *_jecUnc, _JECUnc);
   //seleziono un vettore di jets REC con Cleaning dai 2 Muoni dalla Z REC, qualora esistano
   std::vector<const pat::Jet*> pf2isorecjets;
   if(zrecdaughters.size()){
   	for(unsigned int i = 0; i < pf2recjets.size(); i++){     
   		if(IsoJet<pat::Muon>(zrecdaughters,*pf2recjets[i]))
   			pf2isorecjets.push_back(pf2recjets[i]);
   	}
   }
   else if(!zrecdaughters.size()){
   	for(unsigned int i = 0; i < pf2recjets.size(); i++)
   		pf2isorecjets.push_back(pf2recjets[i]);
   }
   //seleziono un vettore di jets REC con Cleaning dai 2 Muoni dalla Z GEN nella Accettanza dei GEN, qualora esistano  
   std::vector<const pat::Jet*> geniso_pf2isorecjets;
   if(_sample=="mc" && zgenHandle->size()!=0){
   	if(zgendaughters.size()){
     		for(unsigned int i = 0; i < pf2recjets.size(); i++){     
     			if(IsoJet<reco::Candidate>(zgendaughters,*pf2recjets[i]))
     				geniso_pf2isorecjets.push_back(pf2recjets[i]);
     		}
   	}
	else if(!zgendaughters.size()){
     		for(unsigned int i = 0; i < pf2recjets.size(); i++)
   			geniso_pf2isorecjets.push_back(pf2recjets[i]);
        }
   }

   //----richiamo la rho----   
   fwlite::Handle<double> Rho;
   Rho.getByLabel(iEvent, "kt6PFJets", "rho");
   rho = *Rho;

   //----richiamo i Trigger paths----
   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");
 
   // Z REC events

   numberOfZ=zrecHandle->size();
   nmusall=muonHandle->size();    // all muons with loose cuts


   if(_sample=="mc" && zgenHandle->size()!=0){

   	// Events with a GEN Z with mass cut and 2 GEN Muons in the Acceptance (geometrical and kinematic) region
   	if (GenSelectedInAcceptance(*zgenHandle, _selections)&&
   	    zgendaughters.size()){
   		genacc_npfjetsele=geniso_pfisorecjets.size();
   		genacc_npf2jetsele=geniso_pf2isorecjets.size();
   	}
   }
   
 
   if(zrecHandle->size()){
   
   	if(isTriggerAvailable(*triggerHandle, _muTrigVector[0].c_str())) muTrg1=1;
   	if(isTriggerAvailable(*triggerHandle, _muTrigVector[1].c_str())) muTrg2=1;
   	if(isTriggerAvailable(*triggerHandle, _muTrigVector[2].c_str())) muTrg3=1;
   	if(isTriggerAvailable(*triggerHandle, _muTrigVector[3].c_str())) muTrg4=1;
   	if(isTriggerAvailable(*triggerHandle, _muTrigVector[4].c_str())) muTrg5=1;
   	if(isTriggerAvailable(*triggerHandle, _muTrigVector[5].c_str())) muTrg6=1;
   	if(isTriggerAvailable(*triggerHandle, _muTrigVector[6].c_str())) muTrg7=1;
   	if(isTriggerAvailable(*triggerHandle, _muTrigVector[7].c_str())) muTrg8=1;
   	if(isTriggerAvailable(*triggerHandle, _muTrigVector[8].c_str())) muTrg9=1;
   	if(isTriggerAvailable(*triggerHandle, _muTrigVector[9].c_str())) muTrg10=1;
   
   	if(isMuTriggerAvailable(*triggerHandle, run)){
   		TrgBit=0;
   		if(isMuonTriggered(*triggerHandle, run)){
   			TrgBit=1;
   			if(RecSelected_TrgMatch(*recdau0, *triggerHandle, run) || 
   			   RecSelected_TrgMatch(*recdau1, *triggerHandle, run)) 
				OneMuTrgMatch=1;
   		}
   	}
   
   //Cut flags
   
   	if(_Acc){
   		if (RecSelected("_AccSYM", *zrecHandle, *triggerHandle, run, rho)) cutAccSYM=1;
   		if (RecSelected("_AccASYM", *zrecHandle, *triggerHandle, run, rho)) cutAccASYM=1;
	}
   	if(_Trg){
   		if (RecSelected("_Trg", *zrecHandle, *triggerHandle, run, rho)) cutTrg=1;
   	}
   	if(_Imp){
   		if (RecSelected("_Imp", *zrecHandle, *triggerHandle, run, rho)) cutImp=1;
   	}
   	if(_Qual){
   		if (RecSelected("_QualSYM", *zrecHandle, *triggerHandle, run, rho)) cutQualSYM=1;
   		if (RecSelected("_QualASYM", *zrecHandle, *triggerHandle, run, rho)) cutQualASYM=1;
   	}
   	if(_Iso){
   		if (RecSelected("_IsoSYM", *zrecHandle, *triggerHandle, run, rho)) cutIsoSYM=1;
   		if (RecSelected("_IsoASYM", *zrecHandle, *triggerHandle, run, rho)) cutIsoASYM=1;
   	}
   	if(_MuID){
   		if (RecSelected("_MuIDSYM", *zrecHandle, *triggerHandle, run, rho)) cutMuIDSYM=1;
   		if (RecSelected("_MuIDASYM", *zrecHandle, *triggerHandle, run, rho)) cutMuIDASYM=1;
   	}
   
   	//looser cuts   
   	if (RecSelected(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, run, rho)){
   
	   	// fill variables for ntuple
      
   		zmass=(*zrecHandle)[0].mass();
   		zpt=(*zrecHandle)[0].pt();
  		zeta=(*zrecHandle)[0].eta();
      		zphi=(*zrecHandle)[0].phi();
      
      		mupt1=recdau0->pt();
      		mueta1=recdau0->eta();
      		muphi1=recdau0->phi();
      		mucharge1=recdau0->charge();
      		mudB1=recdau0->dB();

      		mupt2=recdau1->pt();
      		mueta2=recdau1->eta();
      		muphi2=recdau1->phi();
      		mucharge2=recdau1->charge();  
      		mudB2=recdau1->dB();
  
      		npfjetsele=pfisorecjets.size();
      		npf2jetsele=pf2isorecjets.size();

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
      
      		if (pf2isorecjets.size()>0){
        		pf2jetID1=jetID(pf2isorecjets[0]);
        		pf2jetet1=pf2isorecjets[0]->et();
        		pf2jetpt1=pf2isorecjets[0]->pt();
        		pf2jeteta1=pf2isorecjets[0]->eta();
        		pf2jetphi1=pf2isorecjets[0]->phi();
        
        		if(_JECUnc!=0){
        			_jecUnc->setJetEta(pf2isorecjets[0]->eta());
        			_jecUnc->setJetPt(pf2isorecjets[0]->pt()); 
        			pf2jecUnc1 = _jecUnc->getUncertainty(true)*_JECUnc;
   			}
        
   			pf2jetcharge1=pf2isorecjets[0]->jetCharge();
        		pf2jetDeltaRa1=Delta_R(*recdau0, *pf2isorecjets[0]);
        		pf2jetDeltaRb1=Delta_R(*recdau1, *pf2isorecjets[0]);
	
			//DeltaR leading Gen jet - leading PFL1 jet
			if(acc_jetgenphi1>-10 && pf2jetphi1>-10){
				double DeltaPhi_PFL1 = 0;
				if((TMath::Abs(acc_jetgenphi1 - pf2jetphi1) < TMath::Pi()) && 
				   (TMath::Abs(acc_jetgenphi1 - pf2jetphi1) > -(TMath::Pi()))){
					DeltaPhi_PFL1 = acc_jetgenphi1 - pf2jetphi1;
				}	
				else if((acc_jetgenphi1 - pf2jetphi1) > TMath::Pi() ){
					DeltaPhi_PFL1 = (2*TMath::Pi()) - (acc_jetgenphi1 - pf2jetphi1);
				}	
        			else if((acc_jetgenphi1 - pf2jetphi1) < -(TMath::Pi())){
        				DeltaPhi_PFL1 = (2*TMath::Pi()) + (acc_jetgenphi1 - pf2jetphi1);
				}
        			double DeltaEta_PFL1 = acc_jetgeneta1 - pf2jeteta1;
				DeltaR_GenJetPFL1Jet = sqrt(pow(DeltaPhi_PFL1,2) + pow(DeltaEta_PFL1,2));
			}
      		}
      
      		if (pf2isorecjets.size()>1){
        		pf2jetID2=jetID(pf2isorecjets[1]);
        		pf2jetet2=pf2isorecjets[1]->et();
        		pf2jetpt2=pf2isorecjets[1]->pt();
        		pf2jeteta2=pf2isorecjets[1]->eta();
        		pf2jetphi2=pf2isorecjets[1]->phi();
        
        		if(_JECUnc!=0){
        			_jecUnc->setJetEta(pf2isorecjets[1]->eta());
        			_jecUnc->setJetPt(pf2isorecjets[1]->pt()); 
        			pf2jecUnc2 = _jecUnc->getUncertainty(true)*_JECUnc;
   			}
        
       			pf2jetcharge2=pf2isorecjets[1]->jetCharge();
			pf2jetDeltaRa2=Delta_R(*recdau0, *pf2isorecjets[1]);
        		pf2jetDeltaRb2=Delta_R(*recdau1, *pf2isorecjets[1]);
      		}

      		if (pf2isorecjets.size()>2){
        		pf2jetID3=jetID(pf2isorecjets[2]);
       			pf2jetet3=pf2isorecjets[2]->et();
        		pf2jetpt3=pf2isorecjets[2]->pt();
        		pf2jeteta3=pf2isorecjets[2]->eta();
        		pf2jetphi3=pf2isorecjets[2]->phi();
        
        		if(_JECUnc!=0){
        			_jecUnc->setJetEta(pf2isorecjets[2]->eta());
        			_jecUnc->setJetPt(pf2isorecjets[2]->pt()); 
        			pf2jecUnc1 = _jecUnc->getUncertainty(true)*_JECUnc;
   			}
        
        		pf2jetcharge3=pf2isorecjets[2]->jetCharge();
        		pf2jetDeltaRa3=Delta_R(*recdau0, *pf2isorecjets[2]);
        		pf2jetDeltaRb3=Delta_R(*recdau1, *pf2isorecjets[2]);
      		}
      
   	} // endif loose selected Z


   	//stricter cuts
   	if (RecSelected(_RecoCutFlags[1].c_str(), *zrecHandle, *triggerHandle, run, rho) && 
   	    RecSelected(_RecoCutFlags[2].c_str(), *zrecHandle, *triggerHandle, run, rho) &&
   	    RecSelected(_RecoCutFlags[3].c_str(), *zrecHandle, *triggerHandle, run, rho) &&
   	    RecSelected(_RecoCutFlags[4].c_str(), *zrecHandle, *triggerHandle, run, rho) && 
   	    RecSelected(_RecoCutFlags[5].c_str(), *zrecHandle, *triggerHandle, run, rho) &&
   	    RecSelected(_RecoCutFlags[6].c_str(), *zrecHandle, *triggerHandle, run, rho)){
  
   		zmass_AllCuts=(*zrecHandle)[0].mass();
      
   	} // endif strict selected Z
   
   }
   
   if(_NtupleFill=="zcand"){
   	if(zmass_AllCuts>0.) zmumutree->Fill();
   }
   else if(_NtupleFill=="acc"){
   	if(zmass>0.) zmumutree->Fill();
   }
   else if(_NtupleFill=="all"){
   	zmumutree->Fill();
   } 
     
}

void RecoMuonNtuple::finalize(){

   delete _jecUnc; 
   
  _file->Write();

}
