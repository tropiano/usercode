#ifndef PATAnalysisUtilities_h
#define PATAnalysisUtilities_h

#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <vector>
#include <map>
#include <math.h>
#include <sstream>
#include <string>
#include <assert.h>
#include <stdio.h>

#include "TMatrixD.h"
#include "TVectorD.h"
#include "TMath.h"
#include "TH1D.h"

using namespace std;
using namespace pat;
using namespace pat::helper;
using namespace TMath;

////////////////////////////////// Selection Cuts ////////////////////////////////////////////////

//Muon TRIGGER

//Trigger flags

static bool muTrgMatchReq = false;
static bool TrgRange = false;

static map<std::string, std::pair<int, int> > muTrigger() {

typedef std::pair<int, int> rrange;

rrange rr1, rr2, rr3, rr4, rr5, rr6, rr7, rr8;
rr1 = make_pair(136033, 139980);
rr2 = make_pair(140058, 141882);
rr3 = make_pair(141956, 144114);
rr4 = make_pair(146428, 147116);
rr5 = make_pair(147196, 148058);
rr6 = make_pair(148819, 149064);
rr7 = make_pair(149181, 149442);
rr8 = make_pair(149181, 149442);

if(TrgRange == false){
	rr1 = make_pair(-1, 999999999);
	rr2 = make_pair(-1, 999999999);
	rr3 = make_pair(-1, 999999999);
	rr4 = make_pair(-1, 999999999);
	rr5 = make_pair(-1, 999999999);
	rr6 = make_pair(-1, 999999999);
	rr7 = make_pair(-1, 999999999);
	rr8 = make_pair(-1, 999999999);
}

static map<std::string, rrange > TrgVector;

TrgVector["HLT_DoubleMu6_v"] = rr1;
TrgVector["HLT_DoubleMu7_v"] = rr2;
TrgVector["HLT_Mu13_Mu8_v"] = rr3;
TrgVector["HLT_Mu17_Mu8_v"] = rr4;
TrgVector["HLT_Mu5"] = rr5; 
TrgVector["HLT_Mu8"] = rr6; 
TrgVector["HLT_Mu12"] = rr7; 
TrgVector["HLT_"] = rr8; //qualunque HLT

return TrgVector;
}

//Common cuts

//Muons
static double etamucut = 2.4;
static double eta_mu_excl_up = 1.499;             //Excluded Eta region
static double eta_mu_excl_down = 1.501;           //Excluded Eta region
static double dxycut = 0.2;     //cm

//Jets
static bool JetIDReq = true;
static double ptjetmin = 30.;   //Gev/c
static double etajetmax = 2.5;
static double isojetcut = 0.4; //Isolation jet - Z muon

//SYM cuts
static double ptmucut = 20.;        //Gev/c
static double zmassmin_sym = 40.;   //Gev/c^2
static double zmassmax_sym = 9999.;  //Gev/c^2

//SYM Quality cuts
static double maxchi2_SYM = 10.;
static double minMuHit_SYM = 0.;
static double minMatSta_SYM = 1.;
static double minVaPiHitTr_SYM = 0.;
static double minVaTrHit_SYM = 10.;
static double minVaHit_SYM = 11.;
static double minVaPiHitInTr_SYM = 1.;
static double minMat_SYM = 2.;
static double maxPtRelErr_SYM = 0.1;

//SYM Mu Isolation (with rho) variables
static float cAecalEE_SYM = 0.041;
static float cAhcalHE_SYM = 0.032;
static double maxmuEta_SYM = 1.48;
static float cAecalEB_SYM = 0.074;
static float cAhcalEE_SYM = 0.023;
static double muonIsoRhoCut_SYM = 0.15;

static string muID_SYM = "muonIdLoose";//questo muID non va bene, non è riconosciuto

static double SYM_TAG_ptmucut = 20.;    //Gev/c
static double SYM_TAG_etamucut = 2.4;
static double SYM_TAG_eta_mu_excl_up = 1.499;               //Excluded Eta region
static double SYM_TAG_eta_mu_excl_down = 1.501;           //Excluded Eta region
static double SYM_TAG_dxycut = 0.05;     //cm
static double SYM_TAG_maxchi2 = 9999.;
static double SYM_TAG_minMuHit = 0.;
static double SYM_TAG_minMatSta = 0.;
static double SYM_TAG_minVaPiHitTr = 0.;
static double SYM_TAG_minVaTrHit = 0.;
static double SYM_TAG_minVaHit = 0.;
static double SYM_TAG_minVaPiHitInTr = 0.;
static double SYM_TAG_minMat = 0.;
static double SYM_TAG_maxPtRelErr = 9999;
static double SYM_TAG_muonIsoRhoCut = 0.2;                        
static string SYM_TAG_MuID = ""; //if "" no MuID cut applied

//ASYM cuts
static double ptmucut0 = 20.;    //Gev/c
static double ptmucut1 = 10.;
static double zmassmin_asym = 40.;   //Gev/c^2
static double zmassmax_asym = 9999.;  //Gev/c^2

//ASYM Quality cuts
static double maxchi2_ASYM0 = 10.;
static double minMuHit_ASYM0 = 0.;
static double minMatSta_ASYM0 = 1.;
static double minVaPiHitTr_ASYM0 = 0.;
static double minVaTrHit_ASYM0 = 10.;
static double minVaHit_ASYM0 = 11.;
static double minVaPiHitInTr_ASYM0 = 1.;
static double minMat_ASYM0 = 2.;
static double maxPtRelErr_ASYM0 = 0.1;

static double maxchi2_ASYM1 = 10.;
static double minMuHit_ASYM1 = 0.;
static double minMatSta_ASYM1 = 1.;
static double minVaPiHitTr_ASYM1 = 0.;
static double minVaTrHit_ASYM1 = 10.;
static double minVaHit_ASYM1 = 11.;
static double minVaPiHitInTr_ASYM1 = 1.;
static double minMat_ASYM1 = 2.;
static double maxPtRelErr_ASYM1 = 0.1;

//ASYM Mu Isolation (with rho) variables
static float cAecalEE_ASYM0 = 0.041;
static float cAhcalHE_ASYM0 = 0.032;
static double maxmuEta_ASYM0 = 1.48;
static float cAecalEB_ASYM0 = 0.074;
static float cAhcalEE_ASYM0 = 0.023;
static double muonIsoRhoCut_ASYM0 = 0.15;

static float cAecalEE_ASYM1 = 0.041;
static float cAhcalHE_ASYM1 = 0.032;
static double maxmuEta_ASYM1 = 1.48;
static float cAecalEB_ASYM1 = 0.074;
static float cAhcalEE_ASYM1 = 0.023;
static double muonIsoRhoCut_ASYM1 = 0.15;

//Combined Iso
static string muID_ASYM0 = "muonIdLoose";//questo muID non va bene, non è riconosciuto
static string muID_ASYM1 = "muonIdLoose";//questo muID non va bene, non è riconosciuto

// ASYM0 Tag cuts (for soft muon probe)
static double ASYM0_TAG_ptmucut = 20.;    //Gev/c
static double ASYM0_TAG_etamucut = 2.4;
static double ASYM0_TAG_eta_mu_excl_up = 1.499;               //Excluded Eta region
static double ASYM0_TAG_eta_mu_excl_down = 1.501;           //Excluded Eta region
static double ASYM0_TAG_dxycut = 0.05;     //cm
static double ASYM0_TAG_maxchi2 = 10.;
static double ASYM0_TAG_minMuHit = 0.;
static double ASYM0_TAG_minMatSta = 1.;
static double ASYM0_TAG_minVaPiHitTr = 0.;
static double ASYM0_TAG_minVaTrHit = 10.;
static double ASYM0_TAG_minVaHit = 11.;
static double ASYM0_TAG_minVaPiHitInTr = 1.;
static double ASYM0_TAG_minMat = 2.;
static double ASYM0_TAG_maxPtRelErr = 0.1;
static double ASYM0_TAG_muonIsoRhoCut = 0.1;                        //CombRelIso
static string ASYM0_TAG_MuID = ""; //if "" no MuID cut applied

// ASYM1 Tag cuts (for hard muon probe)
static double ASYM1_TAG_ptmucut = 20.;    //Gev/c
static double ASYM1_TAG_etamucut = 2.4;
static double ASYM1_TAG_eta_mu_excl_up = 1.499;               //Excluded Eta region
static double ASYM1_TAG_eta_mu_excl_down = 1.501;           //Excluded Eta region
static double ASYM1_TAG_dxycut = 0.05;     //cm
static double ASYM1_TAG_maxchi2 = 10.;
static double ASYM1_TAG_minMuHit = 0.;
static double ASYM1_TAG_minMatSta = 1.;
static double ASYM1_TAG_minVaPiHitTr = 0.;
static double ASYM1_TAG_minVaTrHit = 10.;
static double ASYM1_TAG_minVaHit = 11.;
static double ASYM1_TAG_minVaPiHitInTr = 1.;
static double ASYM1_TAG_minMat = 2.;
static double ASYM1_TAG_maxPtRelErr = 0.1;
static double ASYM1_TAG_muonIsoRhoCut = 0.1;                        //CombRelIso
static string ASYM1_TAG_MuID = ""; //if "" no MuID cut applied

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Z Daughters Methods


// Class to sort two object by pt
template <class T> bool sortByPt(const T* j1, const T* j2){ return j1->pt() > j2->pt() ; }


// Clone Candidate: it returns the pat candidate casting a reco Candidate or its Shallow Clone 
inline const pat::Muon* CloneCandidate(const reco::Candidate& Daughter){

	const pat::Muon* dau = dynamic_cast<const pat::Muon*>(&Daughter);

	if(!dau){
		const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&Daughter);
     		if (scc && scc->hasMasterClone()){
       			dau = dynamic_cast<const pat::Muon*>(scc->masterClone().get()); 
     		}
     	}
  	return dau;
}


// it returns a vector of 2 Muons from a GEN Z sorted by pt. It require that almost a Z exists 
inline std::vector<const reco::Candidate*> ZGENDaughters(const std::vector<reco::CompositeCandidate>& ZGEN){

  std::vector<const reco::Candidate*> zdaughters;

  const reco::Candidate* dau0 = ZGEN[0].daughter(0);
  const reco::Candidate* dau1 = ZGEN[0].daughter(1);

  const reco::Candidate* finaldau0 = 0; 
  const reco::Candidate* finaldau1 = 0; 

  if (dau0->numberOfDaughters()){
    	bool mu0set = false;
    	for (unsigned int i = 0; i < dau0->numberOfDaughters(); ++i ){
      		if (fabs(dau0->daughter(i)->pdgId()) == 13){
        		if (mu0set) {
          			std::cout << "something wrong in GenSelectedInAcceptance: a daughter muon was already found for dau0 " << std::endl; 
        		}
        		finaldau0 = dau0->daughter(i);
        		mu0set = true;
      		}
    	}
  }
  else{finaldau0 = dau0;}

  if (dau1->numberOfDaughters()){
    	bool mu1set = false;
    	for (unsigned int i = 0; i < dau1->numberOfDaughters(); ++i ){
      		if (fabs(dau1->daughter(i)->pdgId()) == 13){
        		if (mu1set) {
          			std::cout << "something wrong in GenSelectedInAcceptance: a daughter muon was already found for dau1 " << std::endl;
        		}
        		finaldau1 = dau1->daughter(i);
        		mu1set = true;
      		}
    	}
  }
  else{finaldau1 = dau1;}

  const reco::Candidate* leading = finaldau0->pt() > finaldau1->pt() ? finaldau0 : finaldau1;
  const reco::Candidate* second  = finaldau0->pt() > finaldau1->pt() ? finaldau1 : finaldau0;

  zdaughters.push_back(leading);
  zdaughters.push_back(second);

  if(zdaughters.size()==1 || zdaughters.size()>2){
  	std::cout << "ERROR! Wrong Z daughters association. Z daughters number = "<<zdaughters.size()<<std::endl;
  	std::vector<const reco::Candidate*> nullzdaughters;
  	return nullzdaughters;
  }
  else{
  	return zdaughters;
  }
  
}


// it returns a vector of 2 Muons from a REC Z sorted by pt
inline std::vector<const pat::Muon*> ZRECDaughters(const std::vector<reco::CompositeCandidate>& ZREC){
  
  std::vector<const pat::Muon*> zdaughters;
 
  const pat::Muon* dau0 = CloneCandidate(*(ZREC[0].daughter(0)));
  const pat::Muon* dau1 = CloneCandidate(*(ZREC[0].daughter(1)));
  
  const pat::Muon* leading = 0;
  const pat::Muon* second = 0;
  
  if(dau0 && dau1){ 
  	leading = dau0->pt() > dau1->pt() ? dau0 : dau1;
  	second  = dau0->pt() > dau1->pt() ? dau1 : dau0;
  }

 zdaughters.push_back(leading);
 zdaughters.push_back(second);
  
 return zdaughters;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//  GEN SELECTION

// GEN Z Candidate: it is true if there is 1 GEN Z with Mass cut
inline bool GenSelected(const std::vector<reco::CompositeCandidate>& ZGEN, string selections){
     
  if(selections=="SYM") 
  	return ZGEN[0].mass() > zmassmin_sym && 
  	       ZGEN[0].mass() < zmassmax_sym;
  if(selections=="ASYM") 
  	return ZGEN[0].mass() > zmassmin_asym && 
  	       ZGEN[0].mass() < zmassmax_asym;
  
  else return false;
}


// GEN Z Candidate: it is true if there is 1 GEN Z with Mass cut decaying in two GEN Muons in the Acceptance (geometrical and kinematic) region
inline bool GenSelectedInAcceptance(const std::vector<reco::CompositeCandidate>& ZGEN, string selections){

  if (ZGEN.size() == 0) 
  	return false;
  if (ZGEN.size() > 1){
    std::cout << "ERROR! Multiple Gen Z candidates found, you have to choose one before arriving here! " << std::endl;
    throw cms::Exception("PATAnalysis:RecSelectedTwoMuonsOppositeCharge_Mass") << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! ";
    return false;
  }

  std::vector<const reco::Candidate*> zgendaughters = ZGENDaughters(ZGEN);
  const reco::Candidate *dau0 = 0;
  const reco::Candidate *dau1 = 0;
  
  if(zgendaughters.size() != 0){      
    
  	dau0 = zgendaughters[0];
  	dau1 = zgendaughters[1];
  
  	if(selections=="SYM"){
  		return ZGEN[0].mass() > zmassmin_sym && ZGEN[0].mass() < zmassmax_sym &&
  		       dau0->pt() >= ptmucut && fabs(dau0->eta()) <= etamucut &&
  		       dau1->pt() >= ptmucut && fabs(dau1->eta()) <= etamucut &&
  		       (fabs(dau0->eta())<eta_mu_excl_down || fabs(dau0->eta())>eta_mu_excl_up) && 
  		       (fabs(dau1->eta())<eta_mu_excl_down || fabs(dau1->eta())>eta_mu_excl_up);
  	}
  	else if(selections=="ASYM"){
  		return ZGEN[0].mass() > zmassmin_asym && ZGEN[0].mass() < zmassmax_asym && 
  		       dau0->pt() >= ptmucut0 && fabs(dau0->eta()) <= etamucut &&
  		       dau1->pt() >= ptmucut1 && fabs(dau1->eta()) <= etamucut &&
  		       (fabs(dau0->eta())<eta_mu_excl_down || fabs(dau0->eta())>eta_mu_excl_up) && 
  		       (fabs(dau1->eta())<eta_mu_excl_down || fabs(dau1->eta())>eta_mu_excl_up);
  	}
  	else{
 		return false;
  	}
  }
  else{
  	return false;
  }
}

inline bool SingleMu_GenSelectedInAcceptance_SYM(const reco::Candidate* mu){
 return    mu->pt() > ptmucut && fabs(mu->eta()) < etamucut
           && (fabs(mu->eta())<eta_mu_excl_down || fabs(mu->eta())>eta_mu_excl_up);
}

inline bool SingleMu_GenSelectedInAcceptance_ASYM0(const reco::Candidate* mu){
 return    mu->pt() > ptmucut0 && fabs(mu->eta()) < etamucut
           && (fabs(mu->eta())<eta_mu_excl_down || fabs(mu->eta())>eta_mu_excl_up);
}

inline bool SingleMu_GenSelectedInAcceptance_ASYM1(const reco::Candidate* mu){
 return    mu->pt() > ptmucut1 && fabs(mu->eta()) < etamucut
           && (fabs(mu->eta())<eta_mu_excl_down || fabs(mu->eta())>eta_mu_excl_up);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Trigger

//Wild Card string Controller
inline bool in_quote(std::string quote, const std::string& s){
  typedef string::const_iterator SCI;
  SCI p = search( quote.begin(), quote.end(), s.begin(), s.end() );  
  return p!=quote.end();
}

inline bool isTriggerAvailable(const pat::TriggerEvent& triggers, std::string triggername){
  const pat::TriggerPathCollection * paths = triggers.paths();  
  bool TrgAv = false;
  if (!paths) {
    std::cout << "ERROR! trigger paths not found "<< std::endl;
    throw cms::Exception("PATAnalysis:isTriggered") << "ERROR! trigger paths not found ";
    return false;}    
  for(pat::TriggerPathCollection::const_iterator ipath = paths->begin(); ipath != paths->end(); ++ipath){ 
    bool control = in_quote(ipath->name(), triggername);
    if(control){
    if(ipath->wasRun())TrgAv=true;}
}
  return TrgAv;
}

inline bool isMuTriggerAvailable(const pat::TriggerEvent& triggers, int run){
  bool Triggered = false;
  static map<std::string, std::pair<int, int> > TrgVector = muTrigger();
  static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;    
  for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
  if(run!=-1){
  if(isTriggerAvailable(triggers, TrgVectorIter->first.c_str()) && (TrgVectorIter->second.first<=run && run<=TrgVectorIter->second.second))Triggered = true;
  }else{
  if(isTriggerAvailable(triggers, TrgVectorIter->first.c_str()))Triggered = true;
  }
  }
  return Triggered;
}

inline bool isTriggered(const pat::TriggerEvent& triggers, std::string triggername){
  const pat::TriggerPathCollection * paths = triggers.paths(); 
  bool TrgFired = false;
  if (!paths) {
    std::cout << "ERROR! trigger paths not found "<< std::endl;
    throw cms::Exception("PATAnalysis:isTriggered") << "ERROR! trigger paths not found ";
    return false;}    
  for(pat::TriggerPathCollection::const_iterator ipath = paths->begin(); ipath != paths->end(); ++ipath){ 
    bool control = in_quote(ipath->name(), triggername);
    if(control){
    if(ipath->wasAccept())TrgFired=true;}
}
  return TrgFired;
}

inline bool isMuonTriggered(const pat::TriggerEvent& triggers, int run){
  bool Triggered = false;
  static map<std::string, std::pair<int, int> > TrgVector = muTrigger();
  static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
  for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
  if(run!=-1){
  if( isTriggered(triggers, TrgVectorIter->first.c_str()) && (TrgVectorIter->second.first<=run && run<=TrgVectorIter->second.second) )Triggered = true;
  }else{
  if(isTriggered(triggers, TrgVectorIter->first.c_str()))Triggered = true;
  }
  }
  return Triggered;
}

inline bool RecSelected_TrgMatch(const pat::Muon& Muon, const pat::TriggerEvent& triggers, int run){
bool trigmatch = false;

const pat::TriggerPathCollection * paths = triggers.paths(); 

static map<std::string, std::pair<int, int> > TrgVector = muTrigger();
static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;

for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
for(pat::TriggerPathCollection::const_iterator ipath = paths->begin(); ipath != paths->end(); ++ipath){ 
if(in_quote(ipath->name(), TrgVectorIter->first.c_str())){
const TriggerObjectStandAloneCollection MatchMuon = Muon.triggerObjectMatchesByPath(ipath->name().c_str(), true);
if(TrgRange==true && run!=-1){
if(MatchMuon.size() && (TrgVectorIter->second.first<=run && run<=TrgVectorIter->second.second))trigmatch = true;
}else{
if(MatchMuon.size())trigmatch = true;
}
}}
}
return trigmatch;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Z REC SELECTION

//REC Z Candidate: it is true if there is 1 REC Z with Mass cut decaying in two REC Muons with these cuts: Acceptance (geometrical and kinematic), Quality, Impact Parameter, Isolation with rho.  
inline bool RecSelected(string Flag, const std::vector<reco::CompositeCandidate>& ZREC, const pat::TriggerEvent& triggers, int run, double rho){

  std::vector<const pat::Muon*> zdaughters; 
  
  if(ZREC.size()!=0){
  	zdaughters = ZRECDaughters(ZREC);
  }
  else{
  	return false;
  }
  
  if(!zdaughters.size()){
  	return false;
  }
  else{
  
  	const pat::Muon* dau0 = zdaughters[0];
  	const pat::Muon* dau1 = zdaughters[1];
	 
	  bool iso0 = false;	
	  bool iso1 = false;
				
	  bool muon_ID0 = false;
	  bool muon_ID1 = false;
	  
	  bool qual0 = false;
	  bool qual1 = false;
	
	  if(Flag=="_AccSYM"){
	  	return ZREC[0].mass()>zmassmin_sym && ZREC[0].mass()<zmassmax_sym &&
	               dau0->pt() >= ptmucut && fabs(dau0->eta()) <= etamucut &&
	               dau1->pt() >= ptmucut && fabs(dau1->eta()) <= etamucut &&
	               (fabs(dau0->eta())<eta_mu_excl_down || fabs(dau0->eta())>eta_mu_excl_up) &&
	               (fabs(dau1->eta())<eta_mu_excl_down || fabs(dau1->eta())>eta_mu_excl_up);
	  }
	
	  else if(Flag=="_AccASYM"){
	  	return ZREC[0].mass()>zmassmin_asym && ZREC[0].mass()<zmassmax_asym &&
	  	       dau0->pt() >= ptmucut0 && fabs(dau0->eta()) <= etamucut &&
	  	       dau1->pt() >= ptmucut1 && fabs(dau1->eta()) <= etamucut &&
	  	       (fabs(dau0->eta())<eta_mu_excl_down || fabs(dau0->eta())>eta_mu_excl_up) &&
	               (fabs(dau1->eta())<eta_mu_excl_down || fabs(dau1->eta())>eta_mu_excl_up);
	  }
	
	  else if(Flag=="_Trg"){
	  	bool cutTrg = false;
	  	if(muTrgMatchReq==true){
	  		if(isMuonTriggered(triggers, run) && (RecSelected_TrgMatch(*dau0, triggers, run))) 
				cutTrg=true;
	  	}
		else if(muTrgMatchReq==false){
	  		if(isMuonTriggered(triggers, run)) 
				cutTrg=true;
	  	}
	  	return cutTrg;
	  }
	
	  else if(Flag=="_QualSYM"){
	  
	  	float ptRelErr0 = (dau0->userFloat("ptError"))/dau0->pt();
	  	float ptRelErr1 = (dau1->userFloat("ptError"))/dau1->pt();
	  
	  	if(muon::isGoodMuon(*dau0, muon::GlobalMuonPromptTight) &&   
	           dau0->isGlobalMuon() && 
	           dau0->userFloat("normChi2") < maxchi2_SYM &&         
	           dau0->userFloat("numberOfValidMuonHits") > minMuHit_SYM && 
	           dau0->userFloat("numberOfMatchedStations") > minMatSta_SYM && 
	           dau0->userFloat("numberOfValidPixelHitsTr") > minVaPiHitTr_SYM && 
	           dau0->userFloat("numberOfValidTrackerHits") > minVaTrHit_SYM && 
	           dau0->userFloat("numberOfValidHits") >= minVaHit_SYM && 
	           dau0->userFloat("numberOfValidPixelHitsInTr") >= minVaPiHitInTr_SYM && 
	           dau0->userFloat("numberOfMatches") >= minMat_SYM && 
	           ptRelErr0 <= maxPtRelErr_SYM)
	  		qual0 = true;
	         
	  	if(muon::isGoodMuon(*dau1, muon::GlobalMuonPromptTight) && 
	  	   dau1->isGlobalMuon() && 
	  	   dau1->userFloat("normChi2") < maxchi2_SYM &&          
	  	   dau1->userFloat("numberOfValidMuonHits") > minMuHit_SYM && 
	  	   dau1->userFloat("numberOfMatchedStations") > minMatSta_SYM && 
	  	   dau1->userFloat("numberOfValidPixelHitsTr") > minVaPiHitTr_SYM &&
	  	   dau1->userFloat("numberOfValidTrackerHits") > minVaTrHit_SYM && 
	  	   dau1->userFloat("numberOfValidHits") >= minVaHit_SYM && 
	  	   dau1->userFloat("numberOfValidPixelHitsInTr") >= minVaPiHitInTr_SYM && 
	  	   dau1->userFloat("numberOfMatches") >= minMat_SYM && 
	  	   ptRelErr1 <= maxPtRelErr_SYM)
	  		qual1 = true;
	  
	  	return qual0 && qual1;
	  } 
	  else if(Flag=="_QualASYM"){
	  
	  	float ptRelErr0 = (dau0->userFloat("ptError"))/dau0->pt();
	  	float ptRelErr1 = (dau1->userFloat("ptError"))/dau1->pt();
	  
	  	if(muon::isGoodMuon(*dau0, muon::GlobalMuonPromptTight) &&    
	  	   dau0->isGlobalMuon() && 
	  	   dau0->userFloat("normChi2") < maxchi2_ASYM0 &&         
	  	   dau0->userFloat("numberOfValidMuonHits") > minMuHit_ASYM0 && 
	  	   dau0->userFloat("numberOfMatchedStations") > minMatSta_ASYM0 && 
	  	   dau0->userFloat("numberOfValidPixelHitsTr") > minVaPiHitTr_ASYM0 && 
	  	   dau0->userFloat("numberOfValidTrackerHits") > minVaTrHit_ASYM0 && 
	  	   dau0->userFloat("numberOfValidHits") >= minVaHit_ASYM0 && 
	  	   dau0->userFloat("numberOfValidPixelHitsInTr") >= minVaPiHitInTr_ASYM0 && 
	  	   dau0->userFloat("numberOfMatches") >= minMat_ASYM0 && 
	  	   ptRelErr0 <= maxPtRelErr_ASYM0)
	  		qual0 = true;
	         
	  	if(muon::isGoodMuon(*dau1, muon::GlobalMuonPromptTight) &&    
	  	   dau1->isGlobalMuon() && 
	  	   dau1->userFloat("normChi2") < maxchi2_ASYM1 &&        
	  	   dau1->userFloat("numberOfValidMuonHits") > minMuHit_ASYM1 && 
	  	   dau1->userFloat("numberOfMatchedStations") > minMatSta_ASYM1 && 
	  	   dau1->userFloat("numberOfValidPixelHitsTr") > minVaPiHitTr_ASYM1 && 
	  	   dau1->userFloat("numberOfValidTrackerHits") > minVaTrHit_ASYM1 && 
	  	   dau1->userFloat("numberOfValidHits") >= minVaHit_ASYM1 && 
	  	   dau1->userFloat("numberOfValidPixelHitsInTr") >= minVaPiHitInTr_ASYM1 && 
	  	   dau1->userFloat("numberOfMatches") >= minMat_ASYM1 && 
	  	   ptRelErr1 <= maxPtRelErr_ASYM1) 
	  		qual1 = true;
	  
	  	return qual0 && qual1;
	  }  
	
	  else if(Flag=="_Imp"){
	  	return dau0->dB() < dxycut && dau1->dB() < dxycut;
	  }
	
	  else if(Flag=="_IsoSYM"){
	  
	  	const reco::MuonIsolation& iso03_0 = dau0->isolationR03(); 
	  	const reco::MuonIsolation& iso03_1 = dau1->isolationR03();
	  
	  	float muEta0 = dau0->eta(); 
	  	float muEta1 = dau1->eta(); 
	  
	  	float Aecal0 = cAecalEE_SYM;
	 	float Ahcal0 = cAhcalHE_SYM;
	  	float Aecal1 = cAecalEE_SYM;
	  	float Ahcal1 = cAhcalHE_SYM;
	  
	  	if (fabs(muEta0) < maxmuEta_SYM) {
	  		Aecal0 = cAecalEB_SYM;
	  		Ahcal0 = cAhcalEE_SYM;
	  	}
	  
	  	if (fabs(muEta1) < maxmuEta_SYM) {
	  		Aecal1 = cAecalEB_SYM;
	  		Ahcal1 = cAhcalEE_SYM;
	  	}
	  
	  	float muonIsoRho0 = (iso03_0.sumPt + max(0.,iso03_0.emEt-Aecal0*(rho)) + max(0.,iso03_0.hadEt-Ahcal0*(rho)))/dau0->pt();
	  
	  	float muonIsoRho1 = (iso03_1.sumPt + max(0.,iso03_1.emEt-Aecal1*(rho)) + max(0.,iso03_1.hadEt-Ahcal1*(rho)))/dau1->pt();
	  
	  	if(muonIsoRho0 <= muonIsoRhoCut_SYM) iso0 = true;
	  	if(muonIsoRho1 <= muonIsoRhoCut_SYM) iso1 = true;
	         
	  	return iso0 && iso1;       
	  
	  }
	
	  else if(Flag=="_IsoASYM"){
	 
	  	const reco::MuonIsolation& iso03_0 = dau0->isolationR03(); 
	  	const reco::MuonIsolation& iso03_1 = dau1->isolationR03();
	  
	  	float muEta0 = dau0->eta();
	  	float muEta1 = dau1->eta(); 
	  
	  	float Aecal0 = cAecalEE_ASYM0;
	  	float Ahcal0 = cAhcalHE_ASYM0;
	  	float Aecal1 = cAecalEE_ASYM1;
	  	float Ahcal1 = cAhcalHE_ASYM1;
	  
	  	if (fabs(muEta0) < maxmuEta_ASYM0) {
	  		Aecal0 = cAecalEB_ASYM0;
	  		Ahcal0 = cAhcalEE_ASYM0;
	  	}
	  
	  	if (fabs(muEta1) < maxmuEta_ASYM1) {
	  		Aecal1 = cAecalEB_ASYM1;
	  		Ahcal1 = cAhcalEE_ASYM1;
	  	}
	  
	  	float muonIsoRho0 = (iso03_0.sumPt + max(0.,iso03_0.emEt-Aecal0*(rho)) + max(0.,iso03_0.hadEt-Ahcal0*(rho)))/dau0->pt();
	  
	  	float muonIsoRho1 = (iso03_1.sumPt + max(0.,iso03_1.emEt-Aecal1*(rho)) + max(0.,iso03_1.hadEt-Ahcal1*(rho)))/dau1->pt();
	  
	  	if(muonIsoRho0 <= muonIsoRhoCut_ASYM0) iso0 = true;
	  	if(muonIsoRho1 <= muonIsoRhoCut_ASYM1) iso1 = true;
	         
	  	return iso0 && iso1;       
	  
	  }
	
	  else if(Flag=="_MuIDSYM"){
	  	if(dau0->muonID(muID_SYM.c_str())==1.0) muon_ID0 = true;
	  	if(dau1->muonID(muID_SYM.c_str())==1.0) muon_ID1 = true;
	  	return muon_ID0 && muon_ID1;
	  }
	
	  else if(Flag=="_MuIDASYM"){
	  	if(dau0->muonID(muID_ASYM0.c_str())==7.0 || dau0->muonID(muID_ASYM0.c_str())==1.0 || dau0->muonID(muID_ASYM0.c_str())==3.0 || dau0->muonID(muID_ASYM0.c_str())==5.0) 
			muon_ID0 = true;
	  	if(dau1->muonID(muID_ASYM1.c_str())==7.0 || dau1->muonID(muID_ASYM1.c_str())==1.0 || dau1->muonID(muID_ASYM1.c_str())==3.0 || dau1->muonID(muID_ASYM1.c_str())==5.0) 
			muon_ID1 = true;
	  	return muon_ID0 && muon_ID1;
	  }
	
	  else if(Flag=="_1"){
	  	return true;
	  }
	
	  else{
	  	return false;
	  }
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Geometrical Methods

// Delta Phi between a Muon and a Jet
template<class MU> double Delta_Phi(const MU& Muon, const reco::Jet& jet){
	
	double deltaPhi = 0;

	if((TMath::Abs(Muon.phi() - jet.phi()) < TMath::Pi()) && (TMath::Abs(Muon.phi() - jet.phi()) > -(TMath::Pi()))){
	deltaPhi = Muon.phi() - jet.phi();
	}
	
	if((Muon.phi() - jet.phi()) > TMath::Pi() ){
	deltaPhi = (2*TMath::Pi()) - (Muon.phi() - jet.phi());
	}
	
        if((Muon.phi() - jet.phi()) < -(TMath::Pi())){
        deltaPhi = (2*TMath::Pi()) + (Muon.phi() - jet.phi());
	}
	
	return deltaPhi;
	
}


// Delta Eta between a Muon and a Jet
template<class MU> double Delta_Eta(const MU& Muon, const reco::Jet& jet){

	double deltaEta = Muon.eta() - jet.eta();
	
	return deltaEta;
	
}


// Delta R between a Muon and a Jet
template<class MU> double Delta_R(const MU& Muon, const reco::Jet& jet){

	double deltaR = sqrt(pow(Delta_Phi<MU>(Muon,jet),2) + pow(Delta_Eta<MU>(Muon,jet),2));
	
	return deltaR;
	
}


// Delta R between 2 Muons and a Jet: it returns the Delta R value of the Muon-Jet couple for wich the Delta R is minimum
template<class MUON> double MinDeltaRZDau(const std::vector<const MUON*>& muons, const reco::Jet& jet){

        double minDeltaRZDau = -999999;
	
	minDeltaRZDau = (Delta_R<MUON>(*(muons[0]),jet) < Delta_R<MUON>(*(muons[1]),jet)) ? Delta_R<MUON>(*(muons[0]),jet) : Delta_R<MUON>(*(muons[1]),jet);
	
	return minDeltaRZDau;
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Jet Methods


// Jets Selector Utility: it returns a vector of Jets, selected for Acceptance (geometrical and kinematic)
template<class JET> std::vector<const JET*> GetJets_noJetID(const std::vector<JET>& jets){

  std::vector<const JET*> selectedjets;
  for (unsigned int i = 0; i < jets.size();  ++i){
    if (jets[i].pt() > ptjetmin && fabs(jets[i].eta()) < etajetmax) 
  	selectedjets.push_back(&jets[i]);
  }
  return selectedjets;
}

// Single Jet ID Selector Utility: it is true if the Jet pass the ID selection
template<class JET> int jetID(const JET& jet){

   int jetID = -1;
   
   if (jet->isPFJet()) {
             
   	int   chm = jet->chargedHadronMultiplicity();
   	int   npr = jet->chargedMultiplicity() + jet->neutralMultiplicity();
   	float nhf = (jet->neutralHadronEnergy() + jet->HFHadronEnergy())/jet->energy();
   	float phf = jet->photonEnergyFraction();
   	float chf = jet->chargedHadronEnergyFraction();
   	float elf = (jet->electronEnergy())/(jet->energy());
   
   	if(npr>1 && phf<0.99 && nhf<0.99 && ((fabs(jet->eta())<=2.4 && nhf<0.9 && phf<0.9 && elf<0.99 && chf>0 && chm>0) || fabs(jet->eta())>2.4)) 
   		jetID = 1;
             
   }

   return jetID;
}


//Jets Selector Utility: it returns a vector of Jets, if you want selected for ID, selected for Acceptance (geometrical and kinematic) corrected for the JEC Uncertainties
template<class JET> std::vector<const JET*> GetJets_wJECUnc(const std::vector<JET>& jets, JetCorrectionUncertainty& jecUnc, int JECUnc){

  std::vector<const JET*> selectedjets;
  
  bool jetIDflag = true;
  
  for (unsigned int i = 0; i < jets.size();  ++i){
  	if(JetIDReq){
  		jetIDflag=false;
  		if(jetID(&jets[i])==1)
  			jetIDflag=true;
  	}
  
  	double unc = 0;
  
  	if(JECUnc!=0){
  		jecUnc.setJetEta(jets[i].eta());
  		jecUnc.setJetPt(jets[i].pt()); 
  		unc = jecUnc.getUncertainty(true)*JECUnc;
  	}
  
  	if (jets[i].pt()+(jets[i].pt()*unc) > ptjetmin && 
      	    fabs(jets[i].eta()) < etajetmax && 
      	    jetIDflag) 
  		selectedjets.push_back(&jets[i]);
  }
  return selectedjets;
}


// Jets Selector Utility: it returns a vector of Jets, selected for Acceptance (geometrical and kinematic) and ID
template<class JET> std::vector<const JET*> GetJets(const std::vector<JET>& jets){
  std::vector<const JET*> selectedjets;
  
  bool jetIDflag = true;
  
  for (unsigned int i = 0; i < jets.size();  ++i){
  	if(JetIDReq){
  		jetIDflag=false;
  		if(jetID(&jets[i])==1) jetIDflag=true;
  	}
  	if (jets[i].pt() > ptjetmin && fabs(jets[i].eta()) < etajetmax && jetIDflag)
		selectedjets.push_back(&jets[i]);
  }
  
  return selectedjets;
}

// Jet Selector Utility: it returns a vector of Jets, cleaned from a vector of muons
template<class JET> std::vector<const JET*> CleanJets(std::vector<const JET*> jets, const std::vector<const pat::Muon*> muons){
  std::vector<const JET*> cleanedjets;

  for (unsigned int i = 0; i < jets.size();  ++i){
    bool close = false; 
    for (unsigned int j = 0; j < muons.size();  ++j){
      if (deltaR(jets[i]->p4(), muons[j]->p4()) < isojetcut){
        close = true;
        break;
      }
    }
    if (!close) cleanedjets.push_back(jets[i]);
  }
  return cleanedjets;
}


//  Single Jet Selector Utility: it is true if the single Jet is Cleaned from a vector of Muons (2 Muons, because of the MinDeltaRZDau method)
template<class MUON> bool IsoJet(const std::vector<const MUON*>& muons, const reco::Jet& jet){
	
   bool iso_jet = true;
	
   if(MinDeltaRZDau<MUON>(muons,jet) < isojetcut) iso_jet = false;
	
   return iso_jet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Histogram Methods - book histograms according to multiplicity

inline void addHistosVsMulti(unsigned int multi, std::string name, std::string title, int nbin, double min, double max, std::vector<TH1D*>& array){
  if (array.size() < multi+1){
    for (unsigned int i = array.size(); i < multi+1; ++i){
      std::stringstream fullname;
      fullname << name << i << "jets";
      std::stringstream fulltitle;
      std::string ext;
      if (i == 1) ext = "st";
      else if (i == 2) ext = "nd";
      else if (i == 3 ) ext = "rd";
      else ext = "th";
      fulltitle << i << "^{" << ext << "} jet " << title;

      TH1D* newhisto = new TH1D(fullname.str().c_str(), fulltitle.str().c_str(), nbin, min, max);
      newhisto->Sumw2();

      array.push_back(newhisto);

    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////


// Tag & Probe

//Conditions required to fill TagAndProbe
inline bool MassWindow(const std::vector<reco::CompositeCandidate>& Z, string selections){
  
  if(selections=="SYM")return Z[0].mass() > zmassmin_sym && Z[0].mass() < zmassmax_sym;
  if(selections=="ASYM")return Z[0].mass() > zmassmin_asym && Z[0].mass() < zmassmax_asym;
  else return false;
  
}

//Cuts applied on Tag muon
inline bool singleMu_Tag_SYM(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* muon = CloneCandidate(cand);
  
  bool TAG_qual = false;
  bool TAG_iso  = false;
  bool TAG_MuID = true;
  
  if(muon){
  
  float ptRelErr = (muon->userFloat("ptError"))/muon->pt();
  if(muon::isGoodMuon(*muon, muon::GlobalMuonPromptTight) &&   
  muon->isGlobalMuon() && 
  muon->userFloat("normChi2") < SYM_TAG_maxchi2 &&         
  muon->userFloat("numberOfValidMuonHits") > SYM_TAG_minMuHit && 
  muon->userFloat("numberOfMatchedStations") > SYM_TAG_minMatSta && 
  muon->userFloat("numberOfValidPixelHitsTr") > SYM_TAG_minVaPiHitTr && 
  muon->userFloat("numberOfValidTrackerHits") > SYM_TAG_minVaTrHit && 
  muon->userFloat("numberOfValidHits") >= SYM_TAG_minVaHit && 
  muon->userFloat("numberOfValidPixelHitsInTr") >= SYM_TAG_minVaPiHitInTr && 
  muon->userFloat("numberOfMatches") >= SYM_TAG_minMat && 
  ptRelErr <= SYM_TAG_maxPtRelErr)TAG_qual = true;
         
  const reco::MuonIsolation& iso03 = muon->isolationR03();   
  float muEta = muon->eta();   
  float Aecal = cAecalEE_SYM;
  float Ahcal = cAhcalHE_SYM;  
  if (fabs(muEta) < maxmuEta_SYM) {
  Aecal = cAecalEB_SYM;
  Ahcal = cAhcalEE_SYM;}
  
  float muonIsoRho = (iso03.sumPt + max(0.,iso03.emEt-Aecal*(rho)) + max(0.,iso03.hadEt-Ahcal*(rho)))/muon->pt();
  if(muonIsoRho <= SYM_TAG_muonIsoRhoCut)TAG_iso = true;
 
  if(SYM_TAG_MuID!=""){
  TAG_MuID = false;
  if(muon->muonID(SYM_TAG_MuID.c_str())==1.0)TAG_MuID = true;}
  
  return muon->pt() >= SYM_TAG_ptmucut && 
  	 fabs(muon->eta()) <= SYM_TAG_etamucut &&
         (fabs(muon->eta()) < SYM_TAG_eta_mu_excl_down || fabs(muon->eta()) > SYM_TAG_eta_mu_excl_up) &&
         muon->dB() < SYM_TAG_dxycut &&
         TAG_qual &&
         TAG_iso  &&  
         TAG_MuID;
 }else{
 return false;}
}

inline bool singleMu_Tag_ASYM0(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* muon = CloneCandidate(cand);
  
  bool TAG_qual = false;
  bool TAG_iso  = false;
  bool TAG_MuID = true;
  
  if(muon){
  
  float ptRelErr = (muon->userFloat("ptError"))/muon->pt();
  if(muon::isGoodMuon(*muon, muon::GlobalMuonPromptTight) &&   
  muon->isGlobalMuon() && 
  muon->userFloat("normChi2") < ASYM0_TAG_maxchi2 &&         
  muon->userFloat("numberOfValidMuonHits") > ASYM0_TAG_minMuHit && 
  muon->userFloat("numberOfMatchedStations") > ASYM0_TAG_minMatSta && 
  muon->userFloat("numberOfValidPixelHitsTr") > ASYM0_TAG_minVaPiHitTr && 
  muon->userFloat("numberOfValidTrackerHits") > ASYM0_TAG_minVaTrHit && 
  muon->userFloat("numberOfValidHits") >= ASYM0_TAG_minVaHit && 
  muon->userFloat("numberOfValidPixelHitsInTr") >= ASYM0_TAG_minVaPiHitInTr && 
  muon->userFloat("numberOfMatches") >= ASYM0_TAG_minMat && 
  ptRelErr <= ASYM0_TAG_maxPtRelErr)TAG_qual = true;
         
  const reco::MuonIsolation& iso03 = muon->isolationR03();   
  float muEta = muon->eta();   
  float Aecal = cAecalEE_ASYM0;
  float Ahcal = cAhcalHE_ASYM0;  
  if (fabs(muEta) < maxmuEta_ASYM0) {
  Aecal = cAecalEB_ASYM0;
  Ahcal = cAhcalEE_ASYM0;}
  
  float muonIsoRho = (iso03.sumPt + max(0.,iso03.emEt-Aecal*(rho)) + max(0.,iso03.hadEt-Ahcal*(rho)))/muon->pt();
  if(muonIsoRho <= ASYM0_TAG_muonIsoRhoCut)TAG_iso = true;
 
  if(ASYM0_TAG_MuID!=""){
  TAG_MuID = false;
  if(muon->muonID(ASYM0_TAG_MuID.c_str())==1.0)TAG_MuID = true;}
  
  return muon->pt() >= ASYM0_TAG_ptmucut && 
  	 fabs(muon->eta()) <= ASYM0_TAG_etamucut &&
         (fabs(muon->eta()) < ASYM0_TAG_eta_mu_excl_down || fabs(muon->eta()) > ASYM0_TAG_eta_mu_excl_up) &&
         muon->dB() < ASYM0_TAG_dxycut &&
         TAG_qual &&
         TAG_iso  &&  
         TAG_MuID;
 }else{
 return false;}
}

inline bool singleMu_Tag_ASYM1(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* muon = CloneCandidate(cand);
  
  bool TAG_qual = false;
  bool TAG_iso  = false;
  bool TAG_MuID = true;
  
  if(muon){
  
  float ptRelErr = (muon->userFloat("ptError"))/muon->pt();
  if(muon::isGoodMuon(*muon, muon::GlobalMuonPromptTight) &&   
  muon->isGlobalMuon() && 
  muon->userFloat("normChi2") < ASYM1_TAG_maxchi2 &&         
  muon->userFloat("numberOfValidMuonHits") > ASYM1_TAG_minMuHit && 
  muon->userFloat("numberOfMatchedStations") > ASYM1_TAG_minMatSta && 
  muon->userFloat("numberOfValidPixelHitsTr") > ASYM1_TAG_minVaPiHitTr && 
  muon->userFloat("numberOfValidTrackerHits") > ASYM1_TAG_minVaTrHit && 
  muon->userFloat("numberOfValidHits") >= ASYM1_TAG_minVaHit && 
  muon->userFloat("numberOfValidPixelHitsInTr") >= ASYM1_TAG_minVaPiHitInTr && 
  muon->userFloat("numberOfMatches") >= ASYM1_TAG_minMat && 
  ptRelErr <= ASYM1_TAG_maxPtRelErr)TAG_qual = true;
         
  const reco::MuonIsolation& iso03 = muon->isolationR03();   
  float muEta = muon->eta();   
  float Aecal = cAecalEE_ASYM1;
  float Ahcal = cAhcalHE_ASYM1;  
  if (fabs(muEta) < maxmuEta_ASYM1) {
  Aecal = cAecalEB_ASYM1;
  Ahcal = cAhcalEE_ASYM1;}
  
  float muonIsoRho = (iso03.sumPt + max(0.,iso03.emEt-Aecal*(rho)) + max(0.,iso03.hadEt-Ahcal*(rho)))/muon->pt();
  if(muonIsoRho <= ASYM1_TAG_muonIsoRhoCut)TAG_iso = true;
 
  if(ASYM1_TAG_MuID!=""){
  TAG_MuID = false;
  if(muon->muonID(ASYM1_TAG_MuID.c_str())==1.0)TAG_MuID = true;}
  
  return muon->pt() >= ASYM1_TAG_ptmucut && 
  	 fabs(muon->eta()) <= ASYM1_TAG_etamucut &&
         (fabs(muon->eta()) < ASYM1_TAG_eta_mu_excl_down || fabs(muon->eta()) > ASYM1_TAG_eta_mu_excl_up) &&
         muon->dB() < ASYM1_TAG_dxycut &&
         TAG_qual &&
         TAG_iso  &&  
         TAG_MuID;
 }else{
 return false;}
}


//Probe cuts SYM
// Single Muon cut: Acceptance (geometrical and kinematic)
inline bool singleMu_Probe_Acc_SYM(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* muon = CloneCandidate(cand);
  
  if(muon){
  	return (muon->pt() >= ptmucut &&
                fabs(muon->eta()) <= etamucut && 
                (fabs(muon->eta())<eta_mu_excl_down || fabs(muon->eta())>eta_mu_excl_up));
  }
  else{
  	return false;
  }
}


inline bool singleMu_Probe_Trg_SYM(const reco::Candidate& cand, int run, double rho){
  
  const pat::Muon* mu0 = CloneCandidate(cand);
  bool TPTrgMatch=true;
  
  if(muTrgMatchReq){
  TPTrgMatch=false;
  if(mu0){
  //if(RecSelected_TrgMatch(*mu0,  run))
  TPTrgMatch=true;
  }else{
  TPTrgMatch=false;}
  }
 
  return TPTrgMatch; 
  
 }


// Single Muon cut: Impact Parameter    
inline bool singleMu_Probe_Imp(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* muon = CloneCandidate(cand);
  
  if(muon){
  	return muon->dB() < dxycut;
  }
  else{
  	return false;
  }
}

  
// Single Muon cut: Quality 
inline bool singleMu_Probe_Qual_SYM(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* mu = CloneCandidate(cand);
  
  bool qual = false;
  
  if(mu){
  
  	float ptRelErr = (mu->userFloat("ptError"))/mu->pt();
  
  	if(muon::isGoodMuon(*mu, muon::GlobalMuonPromptTight) &&   
         	mu->isGlobalMuon() && 
         	mu->userFloat("normChi2") < maxchi2_SYM &&         
         	mu->userFloat("numberOfValidMuonHits") > minMuHit_SYM && 
         	mu->userFloat("numberOfMatchedStations") > minMatSta_SYM && 
         	mu->userFloat("numberOfValidPixelHitsTr") > minVaPiHitTr_SYM && 
         	mu->userFloat("numberOfValidTrackerHits") > minVaTrHit_SYM && 
         	mu->userFloat("numberOfValidHits") >= minVaHit_SYM && 
         	mu->userFloat("numberOfValidPixelHitsInTr") >= minVaPiHitInTr_SYM && 
         	mu->userFloat("numberOfMatches") >= minMat_SYM && 
         	ptRelErr <= maxPtRelErr_SYM)
  			qual = true;
  }       
  return qual;
}

  
// Single Muon cut: Isolation with rho
inline bool singleMu_Probe_Iso_SYM(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* muon = CloneCandidate(cand);
  
  bool iso = false;
  
  if(muon){
 
  	const reco::MuonIsolation& iso03 = muon->isolationR03(); 
  
  	float muEta = muon->eta(); 
  
  	float Aecal = cAecalEE_SYM;
  	float Ahcal = cAhcalHE_SYM;
  
  	if (fabs(muEta) < maxmuEta_SYM) {
  		Aecal = cAecalEB_SYM;
  		Ahcal = cAhcalEE_SYM;
  	}
  
  	float muonIsoRho = (iso03.sumPt + max(0.,iso03.emEt-Aecal*(rho)) + max(0.,iso03.hadEt-Ahcal*(rho)))/muon->pt();
  
  	if(muonIsoRho <= muonIsoRhoCut_SYM)
		iso = true;
  }
         
  return iso;
  
}
  

inline bool singleMu_Probe_MuID_SYM(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* muon = CloneCandidate(cand);
  
  if(muon){
  bool muon_ID = false; 
  if(muon->muonID(muID_SYM.c_str())==1.0)muon_ID = true;
  return muon_ID;
  }else{
  return false;}
}
  
//Probe cuts ASYM
inline bool singleMu_Probe_Acc_ASYM0(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* mu0 = CloneCandidate(cand);
  
  if(mu0){
  return (          
          mu0->pt() >= ptmucut0
          && fabs(mu0->eta()) <= etamucut 
          && (fabs(mu0->eta())<eta_mu_excl_down || fabs(mu0->eta())>eta_mu_excl_up));
  }else{
  return false;}
  }
  
inline bool singleMu_Probe_Acc_ASYM1(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* mu1 = CloneCandidate(cand);
  
  if(mu1){
  return (          
          mu1->pt() >= ptmucut1
          && fabs(mu1->eta()) <= etamucut 
          && (fabs(mu1->eta())<eta_mu_excl_down || fabs(mu1->eta())>eta_mu_excl_up));
  }else{
  return false;}
  }
  
inline bool singleMu_Probe_Trg_ASYM0(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* mu0 = CloneCandidate(cand);
  bool TPTrgMatch=true;
  
  if(muTrgMatchReq){
  TPTrgMatch=false;
  if(mu0){
  //if(RecSelected_TrgMatch(*mu0, run))
  TPTrgMatch=true;
  }else{
  TPTrgMatch=false;}
  }
 
  return TPTrgMatch; 
  
}  

//Per il momento non applico il probe Trg al second muon
inline bool singleMu_Probe_Trg_ASYM1(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* mu1 = CloneCandidate(cand);
  
  if(mu1){
  return true;
  }else{
  return false;}
  }
  
inline bool singleMu_Probe_Qual_ASYM0(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* mu0 = CloneCandidate(cand);
  
  bool qual0 = false;
  
  if(mu0){
  
  float ptRelErr0 = (mu0->userFloat("ptError"))/mu0->pt();
  
  if(muon::isGoodMuon(*mu0, muon::GlobalMuonPromptTight) &&   
         mu0->isGlobalMuon() && 
         mu0->userFloat("normChi2") < maxchi2_ASYM0 &&         
         mu0->userFloat("numberOfValidMuonHits") > minMuHit_ASYM0 && 
         mu0->userFloat("numberOfMatchedStations") > minMatSta_ASYM0 && 
         mu0->userFloat("numberOfValidPixelHitsTr") > minVaPiHitTr_ASYM0 && 
         mu0->userFloat("numberOfValidTrackerHits") > minVaTrHit_ASYM0 && 
         mu0->userFloat("numberOfValidHits") >= minVaHit_ASYM0 && 
         mu0->userFloat("numberOfValidPixelHitsInTr") >= minVaPiHitInTr_ASYM0 && 
         mu0->userFloat("numberOfMatches") >= minMat_ASYM0 && 
         ptRelErr0 <= maxPtRelErr_ASYM0)qual0 = true;
         
  }       
  return qual0;
}

inline bool singleMu_Probe_Qual_ASYM1(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* mu1 = CloneCandidate(cand);
  
  bool qual1 = false;
  
  if(mu1){
  
  float ptRelErr1 = (mu1->userFloat("ptError"))/mu1->pt();
  
  if(muon::isGoodMuon(*mu1, muon::GlobalMuonPromptTight) &&   
         mu1->isGlobalMuon() && 
         mu1->userFloat("normChi2") < maxchi2_ASYM1 &&         
         mu1->userFloat("numberOfValidMuonHits") > minMuHit_ASYM1 && 
         mu1->userFloat("numberOfMatchedStations") > minMatSta_ASYM1 && 
         mu1->userFloat("numberOfValidPixelHitsTr") > minVaPiHitTr_ASYM1 && 
         mu1->userFloat("numberOfValidTrackerHits") > minVaTrHit_ASYM1 && 
         mu1->userFloat("numberOfValidHits") >= minVaHit_ASYM1 && 
         mu1->userFloat("numberOfValidPixelHitsInTr") >= minVaPiHitInTr_ASYM1 && 
         mu1->userFloat("numberOfMatches") >= minMat_ASYM1 && 
         ptRelErr1 <= maxPtRelErr_ASYM1)qual1 = true;
         
  }       
  return qual1;
}

inline bool singleMu_Probe_Iso_ASYM0(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* muon = CloneCandidate(cand);
  
  bool iso0 = false;
  
  if(muon){
 
  const reco::MuonIsolation& iso03_0 = muon->isolationR03(); 
  
  float muEta0 = muon->eta(); 
  
  float Aecal0 = cAecalEE_ASYM0;
  float Ahcal0 = cAhcalHE_ASYM0;
  
  if (fabs(muEta0) < maxmuEta_ASYM0) {
  Aecal0 = cAecalEB_ASYM0;
  Ahcal0 = cAhcalEE_ASYM0;
  }
  
  float muonIsoRho0 = (iso03_0.sumPt + max(0.,iso03_0.emEt-Aecal0*(rho)) + max(0.,iso03_0.hadEt-Ahcal0*(rho)))/muon->pt();
  
  if(muonIsoRho0 <= muonIsoRhoCut_ASYM0)iso0 = true;}
         
  return iso0;
  
}

inline bool singleMu_Probe_Iso_ASYM1(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* muon = CloneCandidate(cand);
  
  bool iso1 = false;
  
  if(muon){
 
  const reco::MuonIsolation& iso03_1 = muon->isolationR03(); 
  
  float muEta1 = muon->eta(); 
  
  float Aecal1 = cAecalEE_ASYM1;
  float Ahcal1 = cAhcalHE_ASYM1;
  
  if (fabs(muEta1) < maxmuEta_ASYM1) {
  Aecal1 = cAecalEB_ASYM1;
  Ahcal1 = cAhcalEE_ASYM1;
  }
  
  float muonIsoRho1 = (iso03_1.sumPt + max(0.,iso03_1.emEt-Aecal1*(rho)) + max(0.,iso03_1.hadEt-Ahcal1*(rho)))/muon->pt();
  
  if(muonIsoRho1 <= muonIsoRhoCut_ASYM1)iso1 = true;}
         
  return iso1;
  
}

inline bool singleMu_Probe_MuID_ASYM0(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* mu0 = CloneCandidate(cand);
  
  if(mu0){
  bool mu0_ID = false;
   
  if(mu0->muonID(muID_ASYM0.c_str())==7.0 || mu0->muonID(muID_ASYM0.c_str())==1.0 || mu0->muonID(muID_ASYM0.c_str())==3.0 || mu0->muonID(muID_ASYM0.c_str())==5.0)mu0_ID = true;
  
  return mu0_ID;
  }else{
  return false;}
}

inline bool singleMu_Probe_MuID_ASYM1(const reco::Candidate& cand, int run, double rho){

  const pat::Muon* mu1 = CloneCandidate(cand);
  
  if(mu1){
  bool mu1_ID = false;
  
  if(mu1->muonID(muID_ASYM1.c_str())==7.0 || mu1->muonID(muID_ASYM1.c_str())==1.0 || mu1->muonID(muID_ASYM1.c_str())==3.0 || mu1->muonID(muID_ASYM1.c_str())==5.0)mu1_ID = true;
  
  return mu1_ID;
  }else{
  return false;}
}

inline bool singleMu_Probe_True(const reco::Candidate& cand, int run, double rho){
return true;
}

inline bool singleMu_Tag_AllSel_SYM(const reco::Candidate& cand, int run, double rho){
   
  return singleMu_Probe_Acc_SYM(cand, run, rho) && 
         //singleMu_Probe_Trg_SYM(cand, run, rho) && 
         singleMu_Probe_Imp(cand, run, rho) && 
         singleMu_Probe_Qual_SYM(cand, run, rho) && 
         singleMu_Probe_Iso_SYM(cand, run, rho) ;
         //singleMu_Probe_MuID_SYM(cand, run, rho); 
}

inline bool singleMu_Tag_AllSel_ASYM0(const reco::Candidate& cand, int run, double rho){
   
  return singleMu_Probe_Acc_ASYM0(cand, run, rho) && 
         //singleMu_Probe_Trg_ASYM0(cand, run, rho) && 
         singleMu_Probe_Imp(cand, run, rho) && 
         singleMu_Probe_Qual_ASYM0(cand, run, rho) && 
         singleMu_Probe_Iso_ASYM0(cand, run, rho) ;
         //singleMu_Probe_MuID_ASYM0(cand, run, rho);
}

inline bool singleMu_Tag_AllSel_ASYM1(const reco::Candidate& cand, int run, double rho){
   
  return singleMu_Probe_Acc_ASYM1(cand, run, rho) && 
         //singleMu_Probe_Trg_ASYM1(cand, run, rho) && 
         singleMu_Probe_Imp(cand, run, rho) && 
         singleMu_Probe_Qual_ASYM1(cand, run, rho) && 
         singleMu_Probe_Iso_ASYM1(cand, run, rho) ; 
         //singleMu_Probe_MuID_ASYM1(cand, run, rho); 
}
   
#endif
