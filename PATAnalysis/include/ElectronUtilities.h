#ifndef PATAnalysisUtilities_h
#define PATAnalysisUtilities_h

#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"

#include <vector>
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

//Electron TRIGGER
static vector<std::string> elTrigger() {
static vector<std::string> TrgVector;

TrgVector.push_back("HLT_Ele10_LW_L1R");
TrgVector.push_back("HLT_Ele15_SW_L1R");
TrgVector.push_back("HLT_Ele15_SW_CaloEleId_L1R");
TrgVector.push_back("HLT_Ele17_SW_CaloEleId_L1R");
TrgVector.push_back("HLT_Ele17_SW_TightEleId_L1R");
TrgVector.push_back("HLT_Ele17_SW_TighterEleIdIsol_L1R_v2");
TrgVector.push_back("HLT_Ele17_SW_TighterEleIdIsol_L1R_v3"); 

//TrgVector.push_back("HLT_Ele15_LW_L1R");
//TrgVector.push_back("HLT_Ele15_SW_EleId_L1R");

return TrgVector;
}

//TrgMatch
static bool elTrgMatchReq = true;

//Jet TRIGGER
static string JetTrigger = "HLT_Jet30U";

//Common cuts
static double etaelcut = 2.5;
static double eta_el_excl_up = 1.566;               //Excluded Eta region
static double eta_el_excl_down = 1.4442;           //Excluded Eta region
static double minnhit = 11.;
static double maxchi2 = 10.;
static double dxycut = 0.02;     //cm
static double ptjetmin = 15.;   //Gev/c
static double etajetmax = 2.5;
static double isojetcut = 0.5; //Isolation jet - Z electron

//SYM cuts
static double ptelcut = 20.;    //Gev/c
static double zmassmin_sym = 50.;   //Gev/c^2
static double zmassmax_sym = 130.;  //Gev/c^2
static double sym_isocut = 0.1;   //CombRelIso
static string eID_SYM = "eidRobustLoose";

// SYM Tag cuts
static double SYM_TAG_ptelcut = 25.;    //Gev/c
static double SYM_TAG_etaelcut = 2.5;
static double SYM_TAG_eta_el_excl_up = 1.566;               //Excluded Eta region
static double SYM_TAG_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double SYM_TAG_minnhit = 15.;
static double SYM_TAG_maxchi2 = 3.;
static double SYM_TAG_dxycut = 0.02;     //cm
static double SYM_TAG_isocut = 0.05;                        //CombRelIso
static string SYM_TagEiD = "eidRobustLoose"; //if "" no EiD cut applied

//ASYM cuts
static double ptelcut0 = 20.;    //Gev/c
static double ptelcut1 = 10.;
static double zmassmin_asym = 60.;   //Gev/c^2
static double zmassmax_asym = 120.;  //Gev/c^2

//New cuts ASYM: tuned on Spring10. CombRelIso used. WP = 80 for dau0, 95 for dau1
static string eID_ASYM0 = "simpleEleId80cIso";
static string eID_ASYM1 = "simpleEleId95cIso";

// ASYM0 Tag cuts (for soft electron probe)
static double ASYM0_TAG_ptelcut = 20.;    //Gev/c
static double ASYM0_TAG_etaelcut = 2.5;
static double ASYM0_TAG_eta_el_excl_up = 1.566;               //Excluded Eta region
static double ASYM0_TAG_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double ASYM0_TAG_minnhit = 0.;
static double ASYM0_TAG_maxchi2 = 9999.;
static double ASYM0_TAG_dxycut = 0.1;     //cm
static double ASYM0_TAG_isocut = 0.05;                        //CombRelIso
static string ASYM0_TagEiD = ""; //if "" no EiD cut applied

// ASYM1 Tag cuts (for hard electron probe)
static double ASYM1_TAG_ptelcut = 10.;    //Gev/c
static double ASYM1_TAG_etaelcut = 2.5;
static double ASYM1_TAG_eta_el_excl_up = 1.566;               //Excluded Eta region
static double ASYM1_TAG_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double ASYM1_TAG_minnhit = 0.;
static double ASYM1_TAG_maxchi2 = 9999.;
static double ASYM1_TAG_dxycut = 0.1;     //cm
static double ASYM1_TAG_isocut = 0.1;                        //CombRelIso
static string ASYM1_TagEiD = ""; //if "" no EiD cut applied

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Trigger

inline bool isTriggerAvailable(const pat::TriggerEvent& triggers, std::string triggername){
  const pat::TriggerPath* path = triggers.path(triggername);
  if(!path){
  return false;
  }else{
  return true;}
}

inline bool isElTriggerAvailable(const pat::TriggerEvent& triggers){
  bool Triggered = false;
  static vector<std::string> TrgVector = elTrigger();
  static vector<std::string>::iterator TrgVectorIter;
  for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
  const pat::TriggerPath* path = triggers.path(TrgVectorIter->c_str());
  if(path)Triggered = true;
  }
  return Triggered;
}

inline bool isTriggered(const pat::TriggerEvent& triggers, std::string triggername){
  const pat::TriggerPath* path = triggers.path(triggername);
  if (!path) {
    std::cout << "ERROR! trigger path " << triggername << " not found " << std::endl;
    return false;
  }
  return path->wasAccept();
}

inline bool isElectronTriggered(const pat::TriggerEvent& triggers){
  bool Triggered = false;
  static vector<std::string> TrgVector = elTrigger();
  static vector<std::string>::iterator TrgVectorIter;
  for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
  if(isTriggered(triggers, TrgVectorIter->c_str()))Triggered = true;}
  return Triggered;
}

inline bool isJetTriggered(const pat::TriggerEvent& triggers){
  return isTriggered(triggers, JetTrigger.c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Z Daughters Methods

inline const pat::Electron* CloneCandidate(const reco::Candidate& Daughter){

const pat::Electron* dau = dynamic_cast<const pat::Electron*>(&Daughter);

if(!dau){
const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&Daughter);
     if (scc && scc->hasMasterClone()){
       dau = dynamic_cast<const pat::Electron*>(scc->masterClone().get()); 
     }
     }
  return dau;
}

inline std::vector<const reco::Candidate*> ZGENDaughters(const reco::CompositeCandidate& ZGEN){

  std::vector<const reco::Candidate*> zdaughters;
  
  const reco::Candidate* dau0 = ZGEN.daughter(0);
  const reco::Candidate* dau1 = ZGEN.daughter(1);

  const reco::Candidate* finaldau0 = 0; 
  const reco::Candidate* finaldau1 = 0; 

  if (dau0->numberOfDaughters()){
    bool el0set = false;
    for (unsigned int i = 0; i < dau0->numberOfDaughters(); ++i ){
      if (fabs(dau0->daughter(i)->pdgId()) == 11){
        if (el0set) {
          std::cout << "something wrong in GenSelectedInAcceptance: a daughter electron was already found for dau0 " << std::endl; 
        }
        finaldau0 = dau0->daughter(i);
        el0set = true;
      }
    }
  }

  if (dau1->numberOfDaughters()){
    bool el1set = false;
    for (unsigned int i = 0; i < dau1->numberOfDaughters(); ++i ){
      if (fabs(dau1->daughter(i)->pdgId()) == 11){
        if (el1set) {
          std::cout << "something wrong in GenSelectedInAcceptance: a daughter electron was already found for dau1 " << std::endl;
        }
        finaldau1 = dau1->daughter(i);
        el1set = true;
      }
    }
  }
 
  const reco::Candidate* leading = finaldau0->pt() > finaldau1->pt() ? finaldau0 : finaldau1;
  const reco::Candidate* second  = finaldau0->pt() > finaldau1->pt() ? finaldau1 : finaldau0;
  
  zdaughters.push_back(leading);
  zdaughters.push_back(second);
  
  if(zdaughters.size()==1 || zdaughters.size()>2){
  std::cout << "ERROR! Wrong Z daughters association. Z daughters number = "<<zdaughters.size()<<std::endl;
  std::vector<const reco::Candidate*> nullzdaughters;
  return nullzdaughters;
  }else{
  return zdaughters;
  }
  
}

inline std::vector<const pat::Electron*> ZRECDaughters(const reco::CompositeCandidate& ZREC){

 std::vector<const pat::Electron*> zdaughters;
 
  const pat::Electron* dau0 = CloneCandidate(*(ZREC.daughter(0)));
  const pat::Electron* dau1 = CloneCandidate(*(ZREC.daughter(1)));
  
  const pat::Electron* leading = 0;
  const pat::Electron* second = 0;
  
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

inline bool GenSelected(const reco::CompositeCandidate& ZGEN, string selections){
     
  if(selections=="SYM")return ZGEN.mass() > zmassmin_sym && ZGEN.mass() < zmassmax_sym;
  if(selections=="ASYM")return ZGEN.mass() > zmassmin_asym && ZGEN.mass() < zmassmax_asym;
  
  else return false;
}

inline bool GenSelectedInAcceptance(const reco::CompositeCandidate& ZGEN, string selections){
  
  std::vector<const reco::Candidate*> zgendaughters = ZGENDaughters(ZGEN);
  const reco::Candidate *dau0 = 0;
  const reco::Candidate *dau1 = 0;
  
  if(zgendaughters.size() != 0){      
    
  dau0 = zgendaughters[0];
  dau1 = zgendaughters[1];
  
  if(selections=="SYM"){
  return ZGEN.mass() > zmassmin_sym && ZGEN.mass() < zmassmax_sym 
         && dau0->pt() > ptelcut && fabs(dau0->eta()) < etaelcut
         && dau1->pt() > ptelcut && fabs(dau1->eta()) < etaelcut
         && (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) && 
	    (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
 }else if(selections=="ASYM"){
 return ZGEN.mass() > zmassmin_asym && ZGEN.mass() < zmassmax_asym 
         && dau0->pt() > ptelcut0 && fabs(dau0->eta()) < etaelcut
         && dau1->pt() > ptelcut1 && fabs(dau1->eta()) < etaelcut
         && (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) && 
	    (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
 }else{
 return false;
 }
 }else{
 return false;
 }
 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// REC SELECTION

inline bool RecSelected_TrgMatch(const pat::Electron& Electron){
bool trigmatch = false;
static vector<std::string> TrgVector = elTrigger();
static vector<std::string>::iterator TrgVectorIter;
for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
const TriggerObjectStandAloneCollection MatchElectron = Electron.triggerObjectMatchesByPath(TrgVectorIter->c_str());
if(MatchElectron.size()) trigmatch = true;
}
return trigmatch;
}
  
inline bool RecSelected(string Flag, const reco::CompositeCandidate& ZREC, const pat::TriggerEvent& triggers){

  std::vector<const pat::Electron*> zdaughters = ZRECDaughters(ZREC);
  
  if(!zdaughters.size()){
  return false;
  }else{
  
  const pat::Electron* dau0 = zdaughters[0];
  const pat::Electron* dau1 = zdaughters[1];
  
  const reco::GsfTrackRef track0 = dau0->gsfTrack();
  const reco::GsfTrackRef track1 = dau1->gsfTrack();
  assert (track0.isNonnull() && track1.isNonnull());
 
  bool iso0 = false;	
  bool iso1 = false;
			
  bool electron_ID0 = false;
  bool electron_ID1 = false;
  
  bool conv0 = false;
  bool conv1 = false;

  if(Flag=="_AccSYM"){
  return ZREC.mass()>zmassmin_sym && ZREC.mass()<zmassmax_sym
         && dau0->pt() > ptelcut && fabs(dau0->eta()) < etaelcut 
         && dau1->pt() > ptelcut && fabs(dau1->eta()) < etaelcut
         && (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) &&
            (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
         }
  else if(Flag=="_AccASYM"){
  return ZREC.mass()>zmassmin_asym && ZREC.mass()<zmassmax_asym
         && dau0->pt() > ptelcut0 && fabs(dau0->eta()) < etaelcut 
         && dau1->pt() > ptelcut1 && fabs(dau1->eta()) < etaelcut
         && (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) &&
            (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
         }
  else if(Flag=="_Trg"){
  bool cutTrg = false;
  if(elTrgMatchReq==true){
  if(isElectronTriggered(triggers)&&(RecSelected_TrgMatch(*dau0)||RecSelected_TrgMatch(*dau1)))cutTrg=true;
  }else if(elTrgMatchReq==false){
  if(isElectronTriggered(triggers))cutTrg=true;
  }
  return cutTrg;
         }
  else if(Flag=="_ConvASYM"){
  if(dau0->isElectronIDAvailable(eID_ASYM0.c_str()) && dau1->isElectronIDAvailable(eID_ASYM1.c_str())){
  if(dau0->electronID(eID_ASYM0.c_str())==7.0 || dau0->electronID(eID_ASYM0.c_str())==4.0 || dau0->electronID(eID_ASYM0.c_str())==5.0 || dau0->electronID(eID_ASYM0.c_str())==6.0)conv0 = true;
  if(dau1->electronID(eID_ASYM1.c_str())==7.0 || dau1->electronID(eID_ASYM1.c_str())==4.0 || dau1->electronID(eID_ASYM1.c_str())==5.0 || dau1->electronID(eID_ASYM1.c_str())==6.0)conv1 = true;}
  return conv0 && conv1;
  }  
  else if(Flag=="_Imp"){
  return dau0->dB() < dxycut && dau1->dB() < dxycut;
         }
  else if(Flag=="_IsoSYM"){
  return (dau0->hcalIso() + dau0->ecalIso() + dau0->trackIso()) / dau0->pt() < sym_isocut &&  
         (dau1->hcalIso() + dau1->ecalIso() + dau1->trackIso()) / dau1->pt() < sym_isocut;
         }
  else if(Flag=="_IsoASYM"){
  if(dau0->isElectronIDAvailable(eID_ASYM0.c_str()) && dau1->isElectronIDAvailable(eID_ASYM1.c_str())){
  if(dau0->electronID(eID_ASYM0.c_str())==7.0 || dau0->electronID(eID_ASYM0.c_str())==2.0 || dau0->electronID(eID_ASYM0.c_str())==3.0 || dau0->electronID(eID_ASYM0.c_str())==6.0)iso0 = true;  
  if(dau1->electronID(eID_ASYM1.c_str())==7.0 || dau1->electronID(eID_ASYM1.c_str())==2.0 || dau1->electronID(eID_ASYM1.c_str())==3.0 || dau1->electronID(eID_ASYM1.c_str())==6.0)iso1 = true;
  }
  return iso0 && iso1;    
  }
  else if(Flag=="_EiDSYM"){
  if(dau0->isElectronIDAvailable(eID_SYM.c_str()) && dau1->isElectronIDAvailable(eID_SYM.c_str())){
  if(dau0->electronID(eID_SYM.c_str())==1.0)electron_ID0 = true;
  if(dau1->electronID(eID_SYM.c_str())==1.0)electron_ID1 = true;}
  return electron_ID0 && electron_ID1;
  }
  else if(Flag=="_EiDASYM"){
  if(dau0->isElectronIDAvailable(eID_ASYM0.c_str()) && dau1->isElectronIDAvailable(eID_ASYM1.c_str())){
  if(dau0->electronID(eID_ASYM0.c_str())==7.0 || dau0->electronID(eID_ASYM0.c_str())==1.0 || dau0->electronID(eID_ASYM0.c_str())==3.0 || dau0->electronID(eID_ASYM0.c_str())==5.0)electron_ID0 = true;
  if(dau1->electronID(eID_ASYM1.c_str())==7.0 || dau1->electronID(eID_ASYM1.c_str())==1.0 || dau1->electronID(eID_ASYM1.c_str())==3.0 || dau1->electronID(eID_ASYM1.c_str())==5.0)electron_ID1 = true;}
  return electron_ID0 && electron_ID1;
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

template<class EL> double Delta_Phi(const EL& Electron, const reco::Jet& jet){
	
	double deltaPhi = 0;

	if((TMath::Abs(Electron.phi() - jet.phi()) < TMath::Pi()) && (TMath::Abs(Electron.phi() - jet.phi()) > -(TMath::Pi()))){
	deltaPhi = Electron.phi() - jet.phi();
	}
	
	if((Electron.phi() - jet.phi()) > TMath::Pi() ){
	deltaPhi = (2*TMath::Pi()) - (Electron.phi() - jet.phi());
	}
	
        if((Electron.phi() - jet.phi()) < -(TMath::Pi())){
        deltaPhi = (2*TMath::Pi()) + (Electron.phi() - jet.phi());
	}
	
	return deltaPhi;
	
}

template<class EL> double Delta_Eta(const EL& Electron, const reco::Jet& jet){

	double deltaEta = Electron.eta() - jet.eta();
	
	return deltaEta;
	
}

template<class EL> double Delta_R(const EL& Electron, const reco::Jet& jet){

	double deltaR = sqrt(pow(Delta_Phi<EL>(Electron,jet),2) + pow(Delta_Eta<EL>(Electron,jet),2));
	
	return deltaR;
	
}

template<class ELECTRON> double MinDeltaRZDau(const std::vector<const ELECTRON*>& electrons, const reco::Jet& jet){

        double minDeltaRZDau = -999999;
	
	minDeltaRZDau = (Delta_R<ELECTRON>(*(electrons[0]),jet) < Delta_R<ELECTRON>(*(electrons[1]),jet)) ? Delta_R<ELECTRON>(*(electrons[0]),jet) : Delta_R<ELECTRON>(*(electrons[1]),jet);
	
	return minDeltaRZDau;
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Jet Methods

template<class JET> std::vector<const JET*> GetJets(const std::vector<JET>& jets){
  std::vector<const JET*> selectedjets;
  for (unsigned int i = 0; i < jets.size();  ++i){
    if (jets[i].pt() > ptjetmin && fabs(jets[i].eta()) < etajetmax) selectedjets.push_back(&jets[i]);
  }
  return selectedjets;
}

template<class JET> double deltaYFwdBwd(const std::vector<const JET*>& jets){
  assert(jets.size()>1);
  double maxy = -999999;
  double miny =  999999;
  for (unsigned int i = 0; i < jets.size(); ++i){
    if (jets[i]->rapidity() > maxy){
      maxy = jets[i]->rapidity();
    }
    if (jets[i]->rapidity() < miny){
      miny = jets[i]->rapidity();
    }
  }
  return maxy-miny ;   
}

template<class ELECTRON> bool IsoJet(const std::vector<const ELECTRON*>& electrons, const reco::Jet& jet){
	
	bool iso_jet = true;
	
	if(MinDeltaRZDau<ELECTRON>(electrons,jet) < isojetcut) iso_jet = false;
	
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
inline bool RecSelected_TagAndProbe(const reco::CompositeCandidate& ZREC, string selections){
  if(selections=="SYM")return ZREC.mass() > zmassmin_sym && ZREC.mass() < zmassmax_sym;
  if(selections=="ASYM")return ZREC.mass() > zmassmin_asym && ZREC.mass() < zmassmax_asym;
  else return false;
}

//Cuts applied on Tag electron
inline bool singleEl_Tag_SYM(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);

  if(electron){
  const reco::GsfTrackRef track = electron->gsfTrack();
  assert(track.isNonnull());
  bool TAG_EiD = true;
  if(electron->isElectronIDAvailable(SYM_TagEiD.c_str())){
  TAG_EiD = false;
  if(electron->electronID(SYM_TagEiD.c_str())==1.0)TAG_EiD = true;
  }
  return electron->pt() > SYM_TAG_ptelcut && 
  	 fabs(electron->eta()) < SYM_TAG_etaelcut &&
         (fabs(electron->eta()) < SYM_TAG_eta_el_excl_down || fabs(electron->eta()) > SYM_TAG_eta_el_excl_up) &&
         track->numberOfValidHits() > SYM_TAG_minnhit && 
         track->normalizedChi2() < SYM_TAG_maxchi2 &&
         electron->dB() < SYM_TAG_dxycut &&
         (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < SYM_TAG_isocut &&  
         TAG_EiD;
 }else{
 return false;}
}

inline bool singleEl_Tag_ASYM0(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  const reco::GsfTrackRef track = electron->gsfTrack();
  assert(track.isNonnull());
  bool TAG_EiD = true;
  if(electron->isElectronIDAvailable(ASYM0_TagEiD.c_str())){
  TAG_EiD = false;
  if(electron->electronID(ASYM0_TagEiD.c_str())==1.0)TAG_EiD = true;
  }
  return electron->pt() > ASYM0_TAG_ptelcut && 
  	 fabs(electron->eta()) < ASYM0_TAG_etaelcut &&
         (fabs(electron->eta()) < ASYM0_TAG_eta_el_excl_down || fabs(electron->eta()) > ASYM0_TAG_eta_el_excl_up) &&
         track->numberOfValidHits() > ASYM0_TAG_minnhit && 
         track->normalizedChi2() < ASYM0_TAG_maxchi2 &&
         electron->dB() < ASYM0_TAG_dxycut &&
         (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < ASYM0_TAG_isocut &&  
         TAG_EiD;
 }else{
 return false;}
}

inline bool singleEl_Tag_ASYM1(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){ 
  const reco::GsfTrackRef track = electron->gsfTrack();
  assert(track.isNonnull());
  bool TAG_EiD = true;
  if(electron->isElectronIDAvailable(ASYM1_TagEiD.c_str())){
  TAG_EiD = false;
  if(electron->electronID(ASYM1_TagEiD.c_str())==1.0)TAG_EiD = true;
  }
  
  return electron->pt() > ASYM1_TAG_ptelcut && 
  	 fabs(electron->eta()) < ASYM1_TAG_etaelcut &&
         (fabs(electron->eta()) < ASYM1_TAG_eta_el_excl_down || fabs(electron->eta()) > ASYM1_TAG_eta_el_excl_up) &&
         track->numberOfValidHits() > ASYM1_TAG_minnhit && 
         track->normalizedChi2() < ASYM1_TAG_maxchi2 &&
         electron->dB() < ASYM1_TAG_dxycut &&
         (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < ASYM1_TAG_isocut &&  
         TAG_EiD;
 }else{
 return false;}
}

//Probe cuts SYM
inline bool singleEl_Probe_Acc_SYM(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  return (          
          electron->pt() > ptelcut 
          && fabs(electron->eta()) < etaelcut 
          && (fabs(electron->eta())<eta_el_excl_down || fabs(electron->eta())>eta_el_excl_up));
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_Qual(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  const reco::GsfTrackRef track = electron->gsfTrack();
  assert(track.isNonnull());
  return track->numberOfValidHits() > minnhit && track->normalizedChi2() < maxchi2;
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_Imp(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  return electron->dB() < dxycut;
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_Iso_SYM(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  return (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < sym_isocut;
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_EiD_SYM(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  bool electron_ID = false;
  if(electron->isElectronIDAvailable(SYM_TagEiD.c_str())){
  if(electron->electronID(SYM_TagEiD.c_str())==1.0)electron_ID = true;}
  return electron_ID;
  }else{
  return false;}
}
  
inline bool singleEl_Probe_True(const reco::Candidate& cand){
return true;
}

//Probe cuts ASYM
inline bool singleEl_Probe_Acc_ASYM0(const reco::Candidate& cand){

  const pat::Electron* el0 = CloneCandidate(cand);
  
  if(el0){
  return (          
          el0->pt() > ptelcut0
          && fabs(el0->eta()) < etaelcut 
          && (fabs(el0->eta())<eta_el_excl_down || fabs(el0->eta())>eta_el_excl_up));
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_Acc_ASYM1(const reco::Candidate& cand){

  const pat::Electron* el1 = CloneCandidate(cand);
  
  if(el1){
  return (          
          el1->pt() > ptelcut1
          && fabs(el1->eta()) < etaelcut 
          && (fabs(el1->eta())<eta_el_excl_down || fabs(el1->eta())>eta_el_excl_up));
  }else{
  return false;}
  }

inline bool singleEl_Probe_Iso_ASYM0(const reco::Candidate& cand){

  const pat::Electron* el0 = CloneCandidate(cand);
  
  if(el0){
  bool el0_ID = false; 
  if(el0->isElectronIDAvailable(eID_ASYM0.c_str())){
  if(el0->electronID(eID_ASYM0.c_str())==2.0)el0_ID = true;}
  return el0_ID;
  }else{
  return false;}
}

inline bool singleEl_Probe_Iso_ASYM1(const reco::Candidate& cand){

  const pat::Electron* el1 = CloneCandidate(cand);
  
  if(el1){
  bool el1_ID = false; 
  if(el1->isElectronIDAvailable(eID_ASYM1.c_str())){
  if(el1->electronID(eID_ASYM1.c_str())==2.0)el1_ID = true;}
  return el1_ID;
  }else{
  return false;}
}

inline bool singleEl_Probe_EiD_ASYM0(const reco::Candidate& cand){

  const pat::Electron* el0 = CloneCandidate(cand);
  
  if(el0){
  bool el0_ID = false; 
  if(el0->isElectronIDAvailable(eID_ASYM0.c_str())){
  if(el0->electronID(eID_ASYM0.c_str())==7.0)el0_ID = true;}
  return el0_ID;
  }else{
  return false;}
}

inline bool singleEl_Probe_EiD_ASYM1(const reco::Candidate& cand){

  const pat::Electron* el1 = CloneCandidate(cand);
  
  if(el1){
  bool el1_ID = false; 
  if(el1->isElectronIDAvailable(eID_ASYM1.c_str())){
  if(el1->electronID(eID_ASYM1.c_str())==7.0)el1_ID = true;}
  return el1_ID;
  }else{
  return false;}
}

    
#endif
