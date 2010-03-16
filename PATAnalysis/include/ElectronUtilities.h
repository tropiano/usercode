#ifndef PATAnalysisUtilities_h
#define PATAnalysisUtilities_h

#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

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

// Selection Cuts

static double ptelcut = 20.;    //Gev/c
static double etaelcut = 2.4;
static double eta_el_excl_up = 1.56;               //Excluded Eta region
static double eta_el_excl_down = 1.4442;           //Excluded Eta region
static double zmassmin = 50.;   //Gev/c^2
static double zmassmax = 130.;  //Gev/c^2
static double minnhit = 11.;
static double maxchi2 = 10.;
static double dxycut = 0.02;     //cm
static double ptjetmin = 30.;   //Gev/c
static double etajetmax = 3.0;
static double isocut = 0.1;                        //CombRelIso
static double isojetcut = 0.5;                     //Isolation jet - Z electron

// Tag & Probe cuts

static double TAG_ptelcut = 30.;    //Gev/c
static double TAG_etaelcut = 2.4;
static double TAG_eta_el_excl_up = 1.56;               //Excluded Eta region
static double TAG_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double TAG_minnhit = 15.;
static double TAG_maxchi2 = 3.;
static double TAG_dxycut = 0.02;     //cm
static double TAG_isocut = 0.05;                        //CombRelIso
static string TagEiD = "eidRobustLoose";


/////////////////////////////////////////////////////////////////////////////////


inline int nocase_cmp(const std::string & s1, const std::string & s2)
{
  std::string::const_iterator it1 = s1.begin();
  std::string::const_iterator it2 = s2.begin();
  while ((it1 != s1.end()) && (it2 != s2.end())) {
    if (::toupper(*it1) !=::toupper(*it2)) {  // < Letters differ?
      // Return -1 to indicate smaller than, 1 otherwise
      return (::toupper(*it1) < ::toupper(*it2)) ? -1 : 1;
    }
    // Proceed to the next character in each string
    ++it1;
    ++it2;
  }
  size_t size1 = s1.size(), size2 = s2.size();  // Cache lengths
  // Return -1,0 or 1 according to strings' lengths
  if (size1 == size2)
    return 0;
  return (size1 < size2) ? -1 : 1;
}

template < typename T > inline T as(const std::string & value)
{
  std::istringstream ss(value);
  T out = T();
  ss >> out;
  return out;
}

template <> inline bool as < bool > (const std::string & value) {
  if (nocase_cmp(value, "true") == 0 || nocase_cmp(value, "yes") == 0 || nocase_cmp(value, "on") == 0)
    return true;
  if (nocase_cmp(value, "false") == 0 || nocase_cmp(value, "no") == 0 || nocase_cmp(value, "off") == 0)
    return false;
  return as < int >(value);
}

inline void handleConfigStream(std::istream& in, std::map<std::string, std::vector<std::string> >& pmap) {
   std::string currentModule = "none"; 
   std::cout << "In handleConfigStream" << std::endl; 
   while(in) {
     std::string line;
     getline(in, line);
     const std::string origline = line;
     std::cout << "Input param line: " << line << std::endl;

     // Chop line after a # character
     size_t commentPosn = line.find_first_of("#");
     if (commentPosn != std::string::npos) {
       line = line.substr(0, commentPosn);
     }

     //if the line contains the keyword ANALYSIS, the config of a module begins 
     // ANALYSIS name=type 
     if (line.find("ANALYSIS") != std::string::npos){
        std::cout << "FOUND ana ANALYSIS!!!!" << std::endl; 
        size_t start = line.find_first_of(' ');
        line = line.substr(start, std::string::npos-start+1);
        std::cout << "line.substr(start, std::string::npos-start+1) " << line << std::endl;
        size_t start1 = line.find_first_of('='); 
        if (start1 == std::string::npos){
          std::cout << "Cannot get module name and type from " << line << std::endl; 
          assert(0);
        }
        line.replace(start1, 1, " ");
        const size_t first = line.find_first_not_of(' ');
        const size_t last = line.find_last_not_of(' ');
        line = line.substr(first, last-first+1);
        const size_t firstgap = line.find(' ');
        const size_t lastgap = line.rfind(' ');
        const std::string mname = line.substr(first, firstgap-first);
        const std::string mval  = line.substr(lastgap+1, last-lastgap);  
        pmap[mname].push_back(mval);
        std::cout << "name " << mname << " type " << mval << std::endl;
        currentModule=mname;
        continue;
     }

     if (currentModule == "none"){
        std::cout << "config file MUST start with the keyword ANALYSIS" << std::endl;
        assert(0);
     }  
     // Replace = signs with spaces
     if (line.find("=") != std::string::npos){
        pmap[currentModule].push_back(line);
     }  
   }
}

// Trigger

inline bool isTriggered(const pat::TriggerEvent& triggers, std::string triggername){
  std::vector<std::string>::const_iterator i;
  const pat::TriggerPath* path = triggers.path(triggername);
  if (!path) {
    std::cout << "ERROR! trigger path " << triggername << " not found " << std::endl;
    throw cms::Exception("PATAnalysis:isTriggered") << "ERROR! trigger path " << triggername << " not found ";
    return false;
  }
  return path->wasAccept();
}

inline bool isElectronTriggered(const pat::TriggerEvent& triggers){
  return isTriggered(triggers, "HLT_Ele10_LW_L1R");
}

inline bool isJetTriggered(const pat::TriggerEvent& triggersREC){
  return isTriggered(triggersREC, "HLT_Jet30");
}

/////////////////////////////////////////////////////////////////////////////////////////

// Z Daughters Methods

inline std::vector<const reco::Candidate*> ZGENDaughters(const std::vector<reco::CompositeCandidate>& ZGEN){

std::vector<const reco::Candidate*> zdaughters;

const reco::Candidate *dau = 0;
const reco::Candidate *daudau = 0;
  
  if(ZGEN.size() == 1){
  
  for(unsigned i = 0; i != ZGEN[0].numberOfDaughters(); i++){ 
  dau = ZGEN[0].daughter(i);
  
  if(!dau->numberOfDaughters()){
  if((dau->pdgId() == 11) || (dau->pdgId() == -11))zdaughters.push_back(dau);
  }
  
  if(dau->numberOfDaughters()){
  for(unsigned i = 0; i != dau->numberOfDaughters(); i++){
  daudau = dau->daughter(i);
  if((daudau->pdgId() == 11) || (daudau->pdgId() == -11))zdaughters.push_back(daudau);
  }
  }
  
  }
  }
  
  if(zdaughters.size()==1 || zdaughters.size()>2){
  std::cout<<"ERROR! Wrong Z daughters association. Z daughters number = "<<zdaughters.size()<<std::endl;
  std::vector<const reco::Candidate*> nullzdaughters;
  return nullzdaughters;
  }else{
  return zdaughters;
  }
  
}

inline bool isZdaughtersAvailable(const reco::CompositeCandidate ZREC){

  const pat::Electron* dau0 = dynamic_cast<const pat::Electron*>(ZREC.daughter(0));
  const pat::Electron* dau1 = dynamic_cast<const pat::Electron*>(ZREC.daughter(1));

if (!dau0) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (ZREC.daughter(0));
     if (scc && scc->hasMasterClone()){
       dau0 = dynamic_cast<const pat::Electron*>(scc->masterClone().get()); 
     }
    }
if (!dau1) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (ZREC.daughter(1));
     if (scc && scc->hasMasterClone()){
       dau1 = dynamic_cast<const pat::Electron*>(scc->masterClone().get());
     }
}

  if(dau0 && dau1){
    return true;
  }else{
    return false;
  }
  
}

inline std::vector<const pat::Electron*> ZDaughters(const reco::CompositeCandidate ZREC){

std::vector<const pat::Electron*> zdaughters;

if(isZdaughtersAvailable(ZREC)){

  const pat::Electron* dau0 = dynamic_cast<const pat::Electron*>(ZREC.daughter(0));
  const pat::Electron* dau1 = dynamic_cast<const pat::Electron*>(ZREC.daughter(1));

if (!dau0) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (ZREC.daughter(0));
     if (scc && scc->hasMasterClone()){
       dau0 = dynamic_cast<const pat::Electron*>(scc->masterClone().get()); 
     }
    }
if (!dau1) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (ZREC.daughter(1));
     if (scc && scc->hasMasterClone()){
       dau1 = dynamic_cast<const pat::Electron*>(scc->masterClone().get());
     }
}
  zdaughters.push_back(dau0);
  zdaughters.push_back(dau1);
  
  }

  return zdaughters;
  
}

///////////////////////////////////////////

//  GEN SELECTION

inline bool GenSelected(const std::vector<reco::CompositeCandidate>& ZGEN){
  if (ZGEN.size() == 0) return false;
  if (ZGEN.size() > 1){
    std::cout << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! " << std::endl;
    return false;
  }  
  return ZGEN.size()==1 && ZGEN[0].mass() > zmassmin && ZGEN[0].mass() < zmassmax;
}

inline bool GenSelectedInAcceptance(const std::vector<reco::CompositeCandidate>& ZGEN){
  if (ZGEN.size() == 0) return false;
  if (ZGEN.size() > 1){
    std::cout << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! " << std::endl;
    return false;
  }
  
  std::vector<const reco::Candidate*> zgendaughters = ZGENDaughters(ZGEN);
  const reco::Candidate *dau0 = 0;
  const reco::Candidate *dau1 = 0;
  
  if(zgendaughters.size() != 0){      
    
  dau0 = zgendaughters[0];
  dau1 = zgendaughters[1];
  
  return ZGEN.size()==1 && ZGEN[0].mass() > zmassmin && ZGEN[0].mass() < zmassmax 
         && dau0->pt() > ptelcut && fabs(dau0->eta()) < etaelcut
         && dau1->pt() > ptelcut && fabs(dau1->eta()) < etaelcut
         && (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) && 
	    (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
 }else{
 return false;
 }
}

////////////////////////////////////////////

// REC SELECTION
  
inline bool RecSelected(string Flag, string EID, const reco::CompositeCandidate ZREC){
  
  std::vector<const pat::Electron*> zdaughters = ZDaughters(ZREC);
  
  if(!zdaughters.size()){
  return false;
  }else{
  
  const pat::Electron* dau0 = zdaughters[0];
  const pat::Electron* dau1 = zdaughters[1];
  
  const reco::GsfTrack track0 = *(dau0->gsfTrack());
  const reco::GsfTrack track1 = *(dau1->gsfTrack());
  assert(&track0 && &track1);
  
  bool electron_ID0 = false;
  bool electron_ID1 = false;
  
  if(dau0->isElectronIDAvailable(EID.c_str()) && dau1->isElectronIDAvailable(EID.c_str())){
  if(dau0->electronID(EID.c_str())==1.0)electron_ID0 = true;
  if(dau1->electronID(EID.c_str())==1.0)electron_ID1 = true;
  }
 
  if(Flag=="_Acc"){
  return ZREC.mass()>zmassmin && ZREC.mass()<zmassmax
         && dau0->pt() > ptelcut && fabs(dau0->eta()) < etaelcut 
         && dau1->pt() > ptelcut && fabs(dau1->eta()) < etaelcut
         && (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) &&
            (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
            }
  else if(Flag=="_Qual"){
  return track0.numberOfValidHits() > minnhit && track1.numberOfValidHits() > minnhit &&
         track0.normalizedChi2() < maxchi2 && track1.normalizedChi2() < maxchi2;
         }  
  else if(Flag=="_Imp"){
  return dau0->dB() < dxycut && dau1->dB() < dxycut;
  }
  else if(Flag=="_Iso"){
  return (dau0->hcalIso() + dau0->ecalIso() + dau0->trackIso()) / dau0->pt() < isocut &&  
         (dau1->hcalIso() + dau1->ecalIso() + dau1->trackIso()) / dau1->pt() < isocut;
         }
  else if(Flag=="_EiD"){
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

inline bool RecSelectedWithTrigger(const std::vector<reco::CompositeCandidate>& ZREC, string EID, const pat::TriggerEvent& triggers, const pat::Electron* extdau0 = 0, const pat::Electron* extdau1 = 0){
  bool isTriggered = isElectronTriggered(triggers);
  if(!isTriggered){
  return false;
  }else{
  return true;
  }
}

/////////////////////////////////////////////////

// Geometrical Methods - GEN

inline double Delta_Phi_GEN(const reco::Candidate& Electron, const reco::Jet& jet){
	
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

inline double Delta_Eta_GEN(const reco::Candidate& Electron, const reco::Jet& jet){

	double deltaEta = Electron.eta() - jet.eta();
	
	return deltaEta;
	
}

inline double Delta_R_GEN(const reco::Candidate& Electron, const reco::Jet& jet){

	double deltaR = sqrt(pow(Delta_Phi_GEN(Electron,jet),2) + pow(Delta_Eta_GEN(Electron,jet),2));
	
	return deltaR;
	
}

inline double MinDeltaRZDau_GEN(const std::vector<reco::CompositeCandidate>& ZGEN, const reco::Jet& jet){

        double minDeltaRZDau = -999999;
        
        std::vector<const reco::Candidate*> zdaughters = ZGENDaughters(ZGEN);
        const reco::Candidate *dau0 = 0;
        const reco::Candidate *dau1 = 0;

        if(zdaughters.size()){
        
        dau0 = zdaughters[0];
        dau1 = zdaughters[1];

	minDeltaRZDau = (Delta_R_GEN(*dau0,jet) < Delta_R_GEN(*dau1,jet)) ? Delta_R_GEN(*dau0,jet) : Delta_R_GEN(*dau1,jet);		
	}
	
	return minDeltaRZDau;
	
}

////////////////////////////////////////

// Geometrical Methods - RECO

inline double Delta_Phi(const pat::Electron& Electron, const reco::Jet& jet){
	
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

inline double Delta_Eta(const pat::Electron& Electron, const reco::Jet& jet){
	
	double deltaEta = Electron.eta() - jet.eta();
	
	return deltaEta;
	
}

inline double Delta_R(const pat::Electron& Electron, const reco::Jet& jet){

	double deltaR = sqrt(pow(Delta_Phi(Electron,jet),2) + pow(Delta_Eta(Electron,jet),2));
	
	return deltaR;
	
}

inline double MinDeltaRElectron(const std::vector<pat::Electron>& Electrons, const reco::Jet& jet){
	
	double DeltaR, minDeltaR;
	DeltaR = 0;
        minDeltaR = 999999;
	
	for(unsigned int i = 0; i < Electrons.size(); ++i){
		
	DeltaR = Delta_R(Electrons[i],jet);
		
	if(DeltaR < minDeltaR){
		minDeltaR = DeltaR;
	}
	}
	
	return minDeltaR;
	
}

inline double MinDeltaRZDau(const reco::CompositeCandidate ZREC, const reco::Jet& jet){

        double minDeltaRZDau = -999999;
        
        std::vector<const pat::Electron*> zdaughters = ZDaughters(ZREC);
        const pat::Electron *dau0 = 0;
        const pat::Electron *dau1 = 0;

        if(zdaughters.size()){
        
        dau0 = zdaughters[0];
        dau1 = zdaughters[1];

	minDeltaRZDau = (Delta_R(*dau0,jet) < Delta_R(*dau1,jet)) ? Delta_R(*dau0,jet) : Delta_R(*dau1,jet);		
	}
	
	return minDeltaRZDau;
	
}

///////////////////////////////////////////

// Jet Isolation - GEN

inline bool GenIsoJet(const std::vector<reco::CompositeCandidate>& ZGEN, const reco::Jet& jet){
	
	bool iso_jet = true;
	
	if(MinDeltaRZDau_GEN(ZGEN,jet) < isojetcut) iso_jet = false;
	
	return iso_jet;
	
}

////////////////////////////////////////////

// Jet Isolation - RECO

inline bool RecoIsoJet(const reco::CompositeCandidate ZREC, const reco::Jet& jet){
	
	bool iso_jet = true;
	
	if(MinDeltaRZDau(ZREC,jet) < isojetcut) iso_jet = false;
	
	return iso_jet;
	
}

///////////////////////////////////////////////

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

////////////////////////////////////////////////

// Histogram Methods

//book histograms according to multiplicity
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

/////////////////////////////////////////////////////////////////////////////////////

// Tag & Probe

//Conditions required to fill TagAndProbe
inline bool RecSelected_TagAndProbe(const reco::CompositeCandidate ZREC){
  if ( ZREC.mass() > zmassmin && ZREC.mass() < zmassmax ){
  return true;
  }else{
  return false;
  }
}

//Cuts applied on Tag electron
inline bool singleEl_Tag(const reco::Candidate& cand){
  const pat::Electron* electron = dynamic_cast<const pat::Electron*>(&cand);
  if (!electron) {
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&cand);
     if (scc && scc->hasMasterClone()){
       electron = dynamic_cast<const pat::Electron*>(scc->masterClone().get()); 
     }
    }
  if(electron){
  const reco::GsfTrack track = *(electron->gsfTrack());
  assert(&track);
  bool TAG_EiD = false;
  if(electron->isElectronIDAvailable(TagEiD.c_str())){
  if(electron->electronID(TagEiD.c_str())==1.0)TAG_EiD = true;
  }
  return electron->pt() > TAG_ptelcut && 
  	 fabs(electron->eta()) < TAG_etaelcut &&
         (fabs(electron->eta())<TAG_eta_el_excl_down || fabs(electron->eta())>TAG_eta_el_excl_up) &&
         track.numberOfValidHits() > TAG_minnhit && 
         track.normalizedChi2() < TAG_maxchi2 &&
         electron->dB() < TAG_dxycut &&
         (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < TAG_isocut &&  
         TAG_EiD;
 }else{
 return false;}
}

//Probe cuts
inline bool singleEl_Probe_Acc(const reco::Candidate& cand){
  const pat::Electron* electron = dynamic_cast<const pat::Electron*>(&cand);
  if (!electron) {
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&cand);
     if (scc && scc->hasMasterClone()){
       electron = dynamic_cast<const pat::Electron*>(scc->masterClone().get()); 
     }
    }
  if(electron){
  return electron->pt() > ptelcut && fabs(electron->eta()) < etaelcut &&
         (fabs(electron->eta())<eta_el_excl_down || fabs(electron->eta())>eta_el_excl_up);
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_Qual(const reco::Candidate& cand){
  const pat::Electron* electron = dynamic_cast<const pat::Electron*>(&cand);
  if (!electron) {
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&cand);
     if (scc && scc->hasMasterClone()){
       electron = dynamic_cast<const pat::Electron*>(scc->masterClone().get()); 
     }
    }
  if(electron){
  const reco::GsfTrack track = *(electron->gsfTrack());
  assert(&track);
  return track.numberOfValidHits() > minnhit && track.normalizedChi2() < maxchi2;
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_Imp(const reco::Candidate& cand){
  const pat::Electron* electron = dynamic_cast<const pat::Electron*>(&cand);
  if (!electron) {
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&cand);
     if (scc && scc->hasMasterClone()){
       electron = dynamic_cast<const pat::Electron*>(scc->masterClone().get()); 
     }
    }
  if(electron){
  return electron->dB() < dxycut;
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_Iso(const reco::Candidate& cand){
  const pat::Electron* electron = dynamic_cast<const pat::Electron*>(&cand);
  if (!electron) {
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&cand);
     if (scc && scc->hasMasterClone()){
       electron = dynamic_cast<const pat::Electron*>(scc->masterClone().get()); 
     }
    }
  if(electron){
  const reco::GsfTrack track = *(electron->gsfTrack());
  assert(&track);
  return (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < isocut;
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_EiD(const reco::Candidate& cand){
  const pat::Electron* electron = dynamic_cast<const pat::Electron*>(&cand);
  if (!electron) {
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&cand);
     if (scc && scc->hasMasterClone()){
       electron = dynamic_cast<const pat::Electron*>(scc->masterClone().get()); 
     }
    }
  if(electron){
  bool electron_ID = false;
  if(electron->isElectronIDAvailable(TagEiD.c_str())){
  if(electron->electronID(TagEiD.c_str())==1.0)electron_ID = true;}
  return electron_ID;
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_True(const reco::Candidate& cand){
return true;
}
  
  
#endif
