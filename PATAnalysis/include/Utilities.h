#ifndef PATAnalysisUtilities_h
#define PATAnalysisUtilities_h

#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include <vector>
#include <math.h>
#include <sstream>
#include <string>
#include <assert.h>
#include <algorithm>

#include "TMatrixD.h"
#include "TVectorD.h"
#include "TString.h"

using namespace std;

// Kinematic and Geometric variables
static double ptmucut_leading = 20.;
static double ptmucut_second = 20.;
static double etamucut_leading = 2.4;
static double etamucut_second = 2.4;
static double zmassmin = 40.;
//static double zmassminWin = 60.;
//static double zmassmaxWin = 120.;
static double zmassgenmin = 40.;

// Single Mu Quality Cuts variables
static double maxchi2 = 10.;
static double minMuHit = 0.;
static double minMatSta = 1.;
static double minVaPiHitTr = 0.;
static double minVaTrHit = 10.;

static double minVaHit = 11.;
static double minVaPiHitInTr = 1.;
static double minMat = 2.;
static double maxPtRelErr = 0.1;

static double dxycut = 0.2;

// Single Mu Isolation (with rho) variables
static float cAecalEE = 0.041;
static float cAhcalHE = 0.032;

static double maxmuEta = 1.48;

static float cAecalEB = 0.074;
static float cAhcalEE = 0.023;

// Single Mu Tag variables
static double maxecaletinveto = 4.;
static double maxhcaletinveto = 6.;

// Trigger Wild Card
static string TrigMuStr1 = "HLT_DoubleMu6_v";
static string TrigMuStr2 = "HLT_DoubleMu7_v";
static string TrigMuStr3 = "HLT_Mu13_Mu8_v";
static string TrigMuStr4 = "HLT_Mu17_Mu8_v";

static string TrigJetStr1 = "HLT_Jet30U";

// Jet Cleaning variables
static double deltaRmin = 0.4;


template <class T> bool sortByPt(const T* j1, const T* j2){ return j1->pt() > j2->pt() ; }

// Single Mu Tag 
inline bool singleMu_Tag(const reco::Candidate& cand){
  const pat::Muon* muon = dynamic_cast<const pat::Muon*>(&cand);
  if (!muon) throw cms::Exception("PATAnalysis:singleMu_Tag") << "singleMu_Tag is run on an object which is not a muon "; 
  const reco::MuonIsolation& iso03 = muon->isolationR03();
  
  return muon->pt() > 25 && 
         fabs(muon->eta())<2.1 && 
         (iso03.hadEt + iso03.emEt + iso03.sumPt)/muon->pt() < 0.01 && 
         iso03.emVetoEt < maxecaletinveto &&
         iso03.hadVetoEt < maxhcaletinveto &&
         muon->numberOfValidHits() > 15 && 
         muon->normChi2() < 3 &&
         muon->dB() < 0.05 ;
}


// Single Mu in Pt and Eta leading region 
inline bool singleMu_PtEta(const reco::Candidate& muon){
  return muon.isGlobalMuon() && muon.pt() > ptmucut_leading && fabs(muon.eta()) < etamucut_leading;
}


// Leading Mu in Pt and Eta leading region 
inline bool leadingMu_PtEta(const reco::Candidate& muon){
  return muon.isGlobalMuon() && muon.pt() > ptmucut_leading && fabs(muon.eta()) < etamucut_leading;
}


// Second Mu in Pt and Eta second region
inline bool secondMu_PtEta(const reco::Candidate& muon){
  return muon.isGlobalMuon() && muon.pt() > ptmucut_second && fabs(muon.eta()) < etamucut_second;
}


// Single Mu Quality Cuts
inline bool singleMu_QualityCuts(const reco::Candidate& cand){
  const pat::Muon* muon = dynamic_cast<const pat::Muon*>(&cand);
  if (!muon) {
    //maybe a shallow clone
    const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&cand);
    if (scc && scc->hasMasterClone()){
        muon = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
    }
  }  
  if (!muon) throw cms::Exception("PATAnalysis:singleMu_QualityCuts") << "singleMu_QualityCuts is run on an object which is not a muon ";
  
  //reco::TrackRef gm = muon->globalTrack() ; 

   float ptRelErr = (muon->userFloat("ptError"))/muon->pt(); // è uguale!
//   float ptRelErr = (muon->innerTrack()->ptError())/muon->pt();  

   /*  
    std::cout << "isGlobalMuon = "<< muon->isGlobalMuon() << endl;
    std::cout << "normChi2 = " << muon->userFloat("normChi2") << endl;
    std::cout << "prova di normChi2 = " << fabs(muon->globalTrack()->normalizedChi2()) << endl;
    std::cout << "numberOfValidMuonHits = " << muon->userFloat("numberOfValidMuonHits") << endl;
    std::cout << "prova di numberOfValidMuonHits = " << muon->globalTrack()->hitPattern().numberOfValidMuonHits() << endl;
    std::cout << "numberOfMatchedStations = " << muon->userFloat("numberOfMatchedStations") << endl;
    std::cout << "prova di numberOfMatchedStations = " << muon->numberOfMatchedStations() << endl;
    std::cout << "numberOfValidPixelHits = " << muon->userFloat("numberOfValidPixelHits") << endl;
    std::cout << "prova di numberOfValidPixelHits = " << muon->track()->hitPattern().numberOfValidPixelHits() << endl;
    std::cout << "numberOfValidTrackerHits = " << muon->userFloat("numberOfValidTrackerHits") << endl;
    std::cout << "prova di numberOfValidTrackerHits = " << muon->track()->hitPattern().numberOfValidTrackerHits() << endl;
    std::cout << "numberOfValidHits = " << muon->userFloat("numberOfValidHits") << endl;
    std::cout << "prova di numberOfValidHits = " << muon->innerTrack()->hitPattern().numberOfValidHits() << endl;
    std::cout << "numberOfValidPixelHits = " << muon->userFloat("numberOfValidPixelHits") << endl;
    std::cout << "prova di numberOfValidPixelHits = " << muon->innerTrack()->hitPattern().numberOfValidPixelHits() << endl;
    std::cout << "numberOfMatches = " << muon->userFloat("numberOfMatches") << endl;
    std::cout << "prova di numberOfMatches = " << muon->numberOfMatches() << endl;
    std::cout << "ptErr = " << muon->userFloat("ptError")<< endl;
    std::cout << "prova di ptErr = " << muon->innerTrack()->ptError() << endl;
    std::cout << "ptRelErr = " << ptRelErr << endl;
   */

  return muon::isGoodMuon(*muon, muon::GlobalMuonPromptTight) && // should be redundant
    
         muon->isGlobalMuon() && // VBTF GlobalMuonPromptTight
         muon->userFloat("normChi2") < maxchi2 && // VBTF GlobalMuonPromptTight         
         muon->userFloat("numberOfValidMuonHits") > minMuHit && // VBTF GlobalMuonPromptTight
         muon->userFloat("numberOfMatchedStations") > minMatSta && // VBTF GlobalMuonPromptTight
         muon->userFloat("numberOfValidPixelHitsTr") > minVaPiHitTr && // VBTF GlobalMuonPromptTight
         muon->userFloat("numberOfValidTrackerHits") > minVaTrHit && // VBTF GlobalMuonPromptTight

         muon->userFloat("numberOfValidHits") >= minVaHit && // ?
         muon->userFloat("numberOfValidPixelHitsInTr") >= minVaPiHitInTr && // ??
         muon->userFloat("numberOfMatches") >= minMat && // ?
         ptRelErr <= maxPtRelErr; // ?
}


// Single Mu dxy Cut
inline bool singleMu_DXY(const reco::Candidate& cand){
  const pat::Muon* muon = dynamic_cast<const pat::Muon*>(&cand);
  if (!muon) {
    //maybe a shallow clone
    const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&cand);
    if (scc && scc->hasMasterClone()){
        muon = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
    }
  }
  if (!muon) throw cms::Exception("PATAnalysis:singleMu_DXY") << "singleMu_DXY is run on an object which is not a muon ";

  // std::cout << "dxy = " << muon->dB() << endl;

  return muon->dB() < dxycut;
}


// Single Mu Isolation (with rho) Cut
inline bool singleMu_Isolation(const reco::Candidate& cand, double rhop){//, double isocut = 0.3){
  const pat::Muon* muon = dynamic_cast<const pat::Muon*>(&cand);
  if (!muon) {
    //maybe a shallow clone
    const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&cand);
    if (scc && scc->hasMasterClone()){
        muon = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
    }
  }
  if (!muon) throw cms::Exception("PATAnalysis:singleMu_Isolation") << "singleMu_Isolation is run on an object which is not a muon ";
  const reco::MuonIsolation& iso03 = muon->isolationR03(); 

  float muEta = muon->eta(); // essentially track direction at Vtx (recommended prescription)
  float Aecal = cAecalEE;
  float Ahcal = cAhcalHE;
  if (fabs(muEta) < maxmuEta) {
  Aecal = cAecalEB;
  Ahcal = cAhcalEE;
  }

  // float muonIso = (iso03.sumPt + iso03.emEt + iso03.hadEt)/muon->pt();
  float muonIsoRho = (iso03.sumPt + max(0.,iso03.emEt-Aecal*(rhop)) + max(0.,iso03.hadEt-Ahcal*(rhop)))/muon->pt();
  // std::cout << "muonIsoRho = " << muonIsoRho << endl;
  
//  return muonIso <= 0.15;
  return muonIsoRho <= 0.15;
}


// A Generated Z Candidate with mass Cut
inline bool GenSelectedMuon(const std::vector<reco::CompositeCandidate>& ZGEN){
  if (ZGEN.size() == 0) return false;
  if (ZGEN.size() > 1){
    std::cout << "ERROR! Multiple Gen Z candidates found, you have to choose one before arriving here! " << std::endl;
    throw cms::Exception("PATAnalysis:RecSelectedTwoMuonsOppositeCharge_Mass") << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! ";
    return false;
  }  
  return ZGEN.size()==1 && ZGEN[0].mass() > zmassgenmin;
  // cout << "ZGEN " << ZGEN << endl; 
}


// A Generated Z Candidate with mass cut decaying in two muons in the Leading and Second Pt and Eta regions
inline bool GenSelectedInAcceptanceMuon(const std::vector<reco::CompositeCandidate>& ZGEN){
  if (ZGEN.size() == 0) return false;
  if (ZGEN.size() > 1){
    std::cout << "ERROR! Multiple Gen Z candidates found, you have to choose one before arriving here! " << std::endl;
    throw cms::Exception("PATAnalysis:RecSelectedTwoMuonsOppositeCharge_Mass") << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! ";
    return false;
  }
  
  const reco::Candidate* dau0 = ZGEN[0].daughter(0);
  const reco::Candidate* dau1 = ZGEN[0].daughter(1);

  const reco::Candidate* finaldau0 = 0; //dau0;
  const reco::Candidate* finaldau1 = 0; //dau1;

  if (dau0->numberOfDaughters()){
    bool muon0set = false;
    for (unsigned int i = 0; i < dau0->numberOfDaughters(); ++i ){
      if (fabs(dau0->daughter(i)->pdgId()) == 13){
        if (muon0set) {
          std::cout << "something wrong in GenSelectedInAcceptance: a daughter muon was already found for dau0 " << std::endl; 
        }
        finaldau0 = dau0->daughter(i);
        muon0set = true;
      }
    }
  }

  if (dau1->numberOfDaughters()){
    bool muon1set = false;
    for (unsigned int i = 0; i < dau1->numberOfDaughters(); ++i ){
      if (fabs(dau1->daughter(i)->pdgId()) == 13){
        if (muon1set) {
          std::cout << "something wrong in GenSelectedInAcceptance: a daughter muon was already found for dau1 " << std::endl;
        }
        finaldau1 = dau1->daughter(i);
        muon1set = true;
      }
    }
  }
 
  const reco::Candidate* leading = finaldau0->pt() > finaldau1->pt() ? finaldau0 : finaldau1;
  const reco::Candidate* second  = finaldau0->pt() > finaldau1->pt() ? finaldau1 : finaldau0;

  return ZGEN.size()==1
         && ZGEN[0].mass() > zmassmin 
//         && ZGEN[0].mass() > zmassminWin && ZGEN[0].mass() < zmassmaxWin 
         && leading->pt() > ptmucut_leading && fabs(leading->eta()) < etamucut_leading
         && second->pt() > ptmucut_second && fabs(second->eta()) < etamucut_second;
}


//Wild Card string Controller
inline bool in_quote(std::string quote, const std::string& s){
  typedef string::const_iterator SCI;
  SCI p = search( quote.begin(), quote.end(), s.begin(), s.end() );  //find s in quote
  return p!=quote.end();
}


// Trigger controller function (for both Mu and Jets)
inline bool isTriggered(const pat::TriggerEvent& triggers, std::string triggername){
  //std::vector<std::string>::const_iterator i;
  //pat::TriggerPathRefVector triggerRefs = triggers.acceptedPaths();
  const pat::TriggerPathCollection * paths = triggers.paths();
  //const pat::TriggerPath* path = triggers.path(triggername);
  if (!paths) {
    std::cout << "ERROR! trigger paths not found "<< std::endl;
    throw cms::Exception("PATAnalysis:isTriggered") << "ERROR! trigger paths not found ";
    return false;
  }
  for (pat::TriggerPathCollection::const_iterator ipath = paths->begin(); ipath != paths->end(); ++ipath){ 
    //std::cout << ipath->name() << std::endl; 

    bool control = in_quote(ipath->name(), triggername);
    // cout << "control = " << control << endl;

    if (control) {
    // if (ipath->name() == triggername) {
      if (!ipath->wasRun()){
        std::cout << "Warning! " << triggername << " was not run for this event" << std::endl;  
      }
      std::cout << "Trigger = " << ipath->name() << std::endl;
      return ipath->wasAccept();
    }  
  }
  return false;
  // return path->wasAccept();
}


// Muon Trigger
inline bool isMuonTriggered(const pat::TriggerEvent& triggers, const std::vector<const pat::Muon*>& MuREC){
  bool hasFired = isTriggered(triggers, TrigMuStr1) || isTriggered(triggers, TrigMuStr2) || isTriggered(triggers, TrigMuStr3) || isTriggered(triggers, TrigMuStr4);

  if (MuREC.empty()) return false;
  if (hasFired) return true; 
  return false;
}


// Muon Trigger Matching
inline bool isTriggerMatched(const reco::Candidate& cand){
  const pat::Muon* muon = dynamic_cast<const pat::Muon*>(&cand);
  if (!muon) {
    //maybe a shallow clone
    const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&cand);
    if (scc && scc->hasMasterClone()){
        muon = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
    }
  }

  //const pat::TriggerObjectStandAloneCollection trigMatches = muon->triggerObjectMatchesByPath("HLT_Mu15");
  //return trigMatches.size();  
  const pat::TriggerObjectStandAloneCollection trigMatchesDoMu6_v = muon->triggerObjectMatchesByPath("HLT_DoubleMu6_v1", true);
  const pat::TriggerObjectStandAloneCollection trigMatchesDoMu7_v = muon->triggerObjectMatchesByPath("HLT_DoubleMu7_v1", true);
  const pat::TriggerObjectStandAloneCollection trigMatchesMu13_Mu8_v = muon->triggerObjectMatchesByPath("HLT_Mu13_Mu8_v1", true);
  const pat::TriggerObjectStandAloneCollection trigMatchesMu17_Mu8_v = muon->triggerObjectMatchesByPath("HLT_Mu17_Mu8_v1", true);


  //std::cout << "HLT_DoubleMu6_v1 size = " << trigMatchesDoMu6_v.size() << endl;
  //std::cout << "HLT_DoubleMu7_v1 size = " << trigMatchesDoMu7_v.size() << endl;
  //std::cout << "HLT_Mu13_Mu8_v1 size = " << trigMatchesMu13_Mu8_v.size() << endl;

  bool ismatched = trigMatchesDoMu6_v.size() > 0 || trigMatchesDoMu7_v.size() > 0 || trigMatchesMu13_Mu8_v.size() > 0 || trigMatchesMu17_Mu8_v.size() > 0;

  /*
  const pat::TriggerObjectStandAloneCollection trigMatches15v1 = muon->triggerObjectMatchesByPath("HLT_Mu15_v1", true);
  const pat::TriggerObjectStandAloneCollection trigMatches13v1 = muon->triggerObjectMatchesByPath("HLT_Mu13_v1", true);
  const pat::TriggerObjectStandAloneCollection trigMatches15   = muon->triggerObjectMatchesByPath("HLT_Mu15", true);
  const pat::TriggerObjectStandAloneCollection trigMatches13   = muon->triggerObjectMatchesByPath("HLT_Mu13", true);
  const pat::TriggerObjectStandAloneCollection trigMatches11   = muon->triggerObjectMatchesByPath("HLT_Mu11",true);
  const pat::TriggerObjectStandAloneCollection trigMatches9    = muon->triggerObjectMatchesByPath("HLT_Mu9", true);
  //const pat::TriggerObjectStandAloneCollection trigMatches9    = muon->triggerObjectMatchesByFilter("hltSingleMu9L3Filtered9");

  bool ismatched = trigMatches9.size()>0 || trigMatches15v1.size()>0 || trigMatches13v1.size()>0 || trigMatches15.size()>0 || trigMatches13.size()>0 ||trigMatches11.size()>0 || trigMatches9.size()>0;
  
   if (trigMatches9.size()>0){    
    std::cout << "number of matches: " << trigMatches9.size() << std::endl;
    for (unsigned jj = 0; jj < trigMatches9.size(); ++jj){
      std::cout << "Match " << jj << ":"<< std::endl;
      std::vector< std::string > pathNames = trigMatches9[jj].pathNames();
      std::vector< std::string > filterLabels = trigMatches9[jj].filterLabels();
      std::cout << "pathNames: " << std::endl;  
      for (unsigned kk = 0; kk < pathNames.size(); ++kk){
        std::cout << pathNames[kk] << std::endl;
      }
      std::cout << "filterLabels: " << std::endl;
      for (unsigned kk = 0; kk < filterLabels.size(); ++kk){
        std::cout << filterLabels[kk] << std::endl;
      }
    }
    
    if (reco::deltaR(muon->p4(),trigMatches9[0].p4()) > 0.2) return false;
  }
 */
  
  return ismatched;
}


// Muon Trigger + Muon Trigger Matching (leading muon)
inline bool isMuonTriggeredAndMatched(const pat::TriggerEvent& triggers, const std::vector<const pat::Muon*>& MuREC){
 
 bool HasFired = isMuonTriggered(triggers, MuREC);
 bool Ismatched = isTriggerMatched(*MuREC[0]);

  /* inline bool isMuonTriggered(const pat::TriggerEvent& triggers, const std::vector<const pat::Muon*>& MuREC){
  bool hasFired = isTriggered(triggers, "HLT_Mu9") || isTriggered(triggers, "HLT_Mu11") || isTriggered(triggers, "HLT_Mu13_v1") || isTriggered(triggers, "HLT_Mu15_v1");
  if (MuREC.empty()) return false;

  //now we check that one of the muons is matched to that trigger
  //std::vector<const pat::Muon*>::const_iterator imu;
  //for (imu = MuREC.begin(); imu != MuREC.end(); ++imu){
  //check that the leading muon is matched
  //const pat::TriggerObjectStandAloneCollection trigMatches15v1 = MuREC[0]->triggerObjectMatchesByPath("HLT_Mu15_v1", true);
  //const pat::TriggerObjectStandAloneCollection trigMatches13v1 = MuREC[0]->triggerObjectMatchesByPath("HLT_Mu13_v1", true);
  //const pat::TriggerObjectStandAloneCollection trigMatches15   = MuREC[0]->triggerObjectMatchesByPath("HLT_Mu15", true);
  //const pat::TriggerObjectStandAloneCollection trigMatches13   = MuREC[0]->triggerObjectMatchesByPath("HLT_Mu13", true);
  //const pat::TriggerObjectStandAloneCollection trigMatches11   = MuREC[0]->triggerObjectMatchesByPath("HLT_Mu11", true);
  //const pat::TriggerObjectStandAloneCollection trigMatches9    = MuREC[0]->triggerObjectMatchesByPath("HLT_Mu9", true);
  //const pat::TriggerObjectStandAloneCollection trigMatches9    = MuREC[0]->triggerObjectMatchesByFilter("hltSingleMu9L3Filtered9");
  //bool ismatched = trigMatches15v1.size()>0 || trigMatches13v1.size()>0 || trigMatches15.size()>0 || trigMatches13.size()>0 ||trigMatches11.size()>0 || trigMatches9.size()>0;
  */

  if (HasFired && Ismatched ) return true; 
  return false;
}


// Jet Trigger
inline bool isJetTriggered(const pat::TriggerEvent& triggersREC){
  return isTriggered(triggersREC, TrigJetStr1);
}


// Reconstructed Selected Global Muons: at least two muons in the acceptance with pt > 20 
//to do: try to avoid the copy 
//inline std::pair<bool, std::vector<pat::Muon> > RecSelected_GlobalMuons(const std::vector<pat::Muon>& MuREC, int cut) {
inline std::pair<bool, std::vector<const pat::Muon*> > RecSelected_GlobalMuons(const std::vector<const pat::Muon*>& MuREC, unsigned int cut) {
  std::vector<const pat::Muon*> output;
  //at least two muons in the acceptance with pt > 20  
  if (MuREC.size() < cut) return std::make_pair(false, output);
  unsigned int count = 0; 
  std::vector<const pat::Muon*>::const_iterator imu;
  bool isleadfound = false;
  for (imu = MuREC.begin(); imu != MuREC.end(); ++imu){
    if (!isleadfound){
      if (leadingMu_PtEta(**imu)){
        count++;
        output.push_back(*imu);
        isleadfound = true;
      }  
    } else {
      if (secondMu_PtEta(**imu)){
        count++;
        output.push_back(*imu);
      } 
    }
  }
  if (count < cut) return std::make_pair(false, output);
  else return std::make_pair(true, output);
}


// Reconstructed Selected Muons with Opposite Charge
inline std::pair<bool, std::vector<const pat::Muon*> > RecSelected_Charge(const std::vector<const pat::Muon*>& MuREC, int product = -1.) {
  //if (!RecSelectedTwoMuons(MuREC)) return false;
  std::vector<const pat::Muon*> output;
  if (MuREC.size() == 0) return std::make_pair(false, output);
  std::vector<const pat::Muon*>::const_iterator imu;
  //collections are ordered so the first element is in acceptance as selected by RecSelectedTwoMuons
  double charge0 = MuREC.front()->charge(); 
  for (imu = MuREC.begin()+1; imu != MuREC.end(); ++imu){
    if ((*imu)->charge() * charge0  == product) {
      output = MuREC;
      return std::make_pair(true, output);
    }  
  }
  return std::make_pair(false, output);
}

//////////////////////////////////////////////
//Warning! this includes the two previous cuts
//////////////////////////////////////////////

// Reconstructed Z with mass cut
inline bool RecSelected_MuonMass(const std::vector<reco::CompositeCandidate>& ZREC){
  if (! ZREC.size()) return false;
  /*if (ZREC.size() != 1){
    std::cout << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! " << std::endl;
    throw cms::Exception("PATAnalysis:RecSelectedTwoMuonsOppositeCharge_Mass") << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! ";
    return false;
  }*/
  if ( ZREC[0].mass() > zmassmin 
//       ZREC[0].mass() > zmassminWin && ZREC[0].mass() < zmassmaxWin 
     ) return true;
  return false;
}


// At least 2 Reconstructed Muons passing Quality Cuts
inline std::pair<bool, std::vector<const pat::Muon*> > RecSelected_QualityCuts(const std::vector<const pat::Muon*>& MuREC, unsigned int cut){
  //if (!RecSelectedTM_OppositeCharge(MuREC)) return false;
  std::vector<const pat::Muon*>::const_iterator imu;
  std::vector<const pat::Muon*> selected;
  for (imu = MuREC.begin(); imu != MuREC.end(); ++imu){
    if (singleMu_QualityCuts(**imu)) selected.push_back(*imu);     
  }
  if (selected.size() < cut) return std::make_pair(false, selected);
  else return std::make_pair(true, selected);
}


// At least 2 Reconstructed Muons passing dxy Cut
inline std::pair<bool, std::vector<const pat::Muon*> > RecSelected_DXY(const std::vector<const pat::Muon*>& MuREC, unsigned int cut){
  std::vector<const pat::Muon*>::const_iterator imu;
  std::vector<const pat::Muon*> selected;
  for (imu = MuREC.begin(); imu != MuREC.end(); ++imu){
    if (singleMu_DXY(**imu)) selected.push_back(*imu);
  }
  if (selected.size() < cut) return std::make_pair(false, selected);
  else return std::make_pair(true, selected);
}


// At least 2 Reconstructed Muons passing Single Mu Isolation (with rho) Cut
//we have to check the size
inline std::pair<bool, std::vector<const pat::Muon*> > RecSelected_Isolation(const std::vector<const pat::Muon*>& Muons, double isocut, unsigned int cut, double rho_RSI){
  std::vector<const pat::Muon*> selected;
  std::vector<const pat::Muon*>::const_iterator imu;

  // std::cout << "rho_RSI = " << rho_RSI << endl;

  for (imu = Muons.begin(); imu != Muons.end(); ++imu){
    if ( singleMu_Isolation(**imu, rho_RSI) ) selected.push_back(*imu);
  }
  if (selected.size() < cut) return std::make_pair(false, selected);
  else return std::make_pair(true, selected);
}


// Reconstructed Muon Selection: a Z with mass cut; leading and second Mu: Pt-Eta acceptance, Single Mu Quality Cuts, Single Mu dxy Cut, Single Mu Isolation (with rho) Cut
inline bool RecSelectedMuon(const std::vector<reco::CompositeCandidate>& ZREC, double isocut, double rho_RSM, const pat::Muon* extdau0 = 0,  const pat::Muon* estdau1 = 0 ){

  if (ZREC.size() == 0) return false;
  const pat::Muon* dau0 = 0;
  const pat::Muon* dau1 = 0;
  if (extdau0 && estdau1){
    dau0 = extdau0;
    dau1 = estdau1;
  } else {
    dau0 = dynamic_cast<const pat::Muon*>(ZREC[0].daughter(0));
    dau1 = dynamic_cast<const pat::Muon*>(ZREC[0].daughter(1));
  }  
  if (!dau0) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (ZREC[0].daughter(0));
     if (scc && scc->hasMasterClone()){
       dau0 = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
     }
  }
  if (!dau1) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (ZREC[0].daughter(1));
     if (scc && scc->hasMasterClone()){
       dau1 = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
     }
  }
  assert(dau0 && dau1);
  const pat::Muon* leading = dau0->pt() > dau1->pt() ? dau0 : dau1;
  const pat::Muon* second  = dau0->pt() > dau1->pt() ? dau1 : dau0;
  //const reco::MuonIsolation& iso_leading = leading->isolationR03();
  //const reco::MuonIsolation& iso_second  = second ->isolationR03();

   // std::cout << "rho_RSM = " << rho_RSM << endl;

  return ZREC.size() > 0 &&
         ZREC[0].mass() > zmassmin &&
//         ZREC[0].mass() > zmassminWin && ZREC[0].mass() < zmassmaxWin &&
         leadingMu_PtEta(*leading) && secondMu_PtEta(*second) &&
         singleMu_QualityCuts(*leading) && singleMu_QualityCuts(*second) &&
         singleMu_DXY(*leading) && singleMu_DXY(*second) && 
         singleMu_Isolation(*leading, rho_RSM) && singleMu_Isolation(*second, rho_RSM);               
}


// Reconstructed Muon Selection with Trigger: it returns the Reconstructed Muon Selection value if Leading and Second Mu pass the Muon Trigger Matching controller selection
inline bool RecSelectedMuonWithTrigger(const std::vector<reco::CompositeCandidate>& ZREC, const pat::TriggerEvent& triggers, double isocut, double rho_T, const pat::Muon* extdau0 = 0, const pat::Muon* extdau1 = 0){

  if (ZREC.size() == 0) return false;
  const pat::Muon* dau0 = 0;
  const pat::Muon* dau1 = 0;
  if (extdau0 && extdau1){
    dau0 = extdau0;
    dau1 = extdau1;
  } else {
    dau0 = dynamic_cast<const pat::Muon*>(ZREC[0].daughter(0));
    dau1 = dynamic_cast<const pat::Muon*>(ZREC[0].daughter(1));
  }
  if (!dau0) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (ZREC[0].daughter(0));
     if (scc && scc->hasMasterClone()){
       dau0 = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
     }
  }
  if (!dau1) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (ZREC[0].daughter(1));
     if (scc && scc->hasMasterClone()){
       dau1 = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
     }
  }
  assert(dau0 && dau1);
  const pat::Muon* leading = dau0->pt() > dau1->pt() ? dau0 : dau1;
  const pat::Muon* second  = dau0->pt() > dau1->pt() ? dau1 : dau0;

  std::vector<const pat::Muon*> muons;
  muons.push_back(leading);
  muons.push_back(second);
 
  bool isTriggered = isMuonTriggered(triggers, muons);

 // std::cout << "rho_T = " << rho_T << endl;
 // std::cout << "RecSelectedMuon = " << RecSelectedMuon(ZREC, isocut, rho_T, extdau0, extdau1) << endl;

  if (!isTriggered) return false;

  return RecSelectedMuon(ZREC, isocut, rho_T, extdau0, extdau1);
}


// Reconstructed Muon Selection without Isolation: a Z with mass cut; leading and second Mu: Pt-Eta acceptance, Single Mu Quality Cuts, Single Mu dxy Cut
inline bool RecSelectedNoIso(const std::vector<reco::CompositeCandidate>& ZREC, const pat::Muon* extdau0 = 0, const pat::Muon* extdau1 = 0 ){
  if (ZREC.size() == 0) return false;

  const pat::Muon* dau0 = 0;
  const pat::Muon* dau1 = 0;
  if (extdau0 && extdau1){
    dau0 = extdau0;
    dau1 = extdau1;
  } else {
    dau0 = dynamic_cast<const pat::Muon*>(ZREC[0].daughter(0));
    dau1 = dynamic_cast<const pat::Muon*>(ZREC[0].daughter(1));
  }  

  assert(dau0 && dau1);

  const pat::Muon* leading = dau0->pt() > dau1->pt() ? dau0 : dau1;
  const pat::Muon* second  = dau0->pt() > dau1->pt() ? dau1 : dau0;
 
  return ZREC.size() > 0 && 
         ZREC[0].mass() > zmassmin &&
//         ZREC[0].mass() > zmassminWin && ZREC[0].mass() < zmassmaxWin &&
         leadingMu_PtEta(*leading) && secondMu_PtEta(*second) &&
         singleMu_QualityCuts(*leading) && singleMu_QualityCuts(*second) &&
         singleMu_DXY(*leading) && singleMu_DXY(*second);
}


// It Creates a vector of Cleaned Jets
template<class JET>
void CleanJets(const std::vector<JET>& jets, const std::vector<const pat::Muon*> muons, std::vector<JET>& cleanedjets){
  for (unsigned int i = 0; i < jets.size();  ++i){
    bool close = false; 
    for (unsigned int j = 0; j < muons.size();  ++j){
      if (reco::deltaR(jets[i].p4(), muons[j]->p4()) < deltaRmin){
        close = true;
        break;
      }
    }
    if (!close) cleanedjets.push_back(jets[i]);
  }
}


// It Creates a vector of Selected Jets in the Jet Pt and Eta acceptance
template<class JET> 
std::vector<const JET*> 
GetJets(const std::vector<JET>& jets, double ptmin, double etamax){
  std::vector<const JET*> selectedjets;
  for (unsigned int i = 0; i < jets.size();  ++i){
    if (jets[i].pt() > ptmin && fabs(jets[i].eta()) < etamax) selectedjets.push_back(&jets[i]);
  }
  return selectedjets;
}


// It returns the Forward-Backward rapidity difference
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



/*
inline TMatrixD createInclusiveTransform(int nrows){
  //create matrix with 1 in the upper triangle
  TMatrixD transform(nrows, nrows);
  for (int i = 0; i < transform.GetNrows(); ++i){
    for (int j = 0; j < transform.GetNcols(); ++j){
      if (j>=i) transform(i, j) = 1;
    }
  }
  return transform;
}

inline TVectorD inclusiveMultiplicities(const TVectorD& exclusive){
  return createInclusiveTransform(exclusive.GetNrows())*exclusive;
}

inline TMatrixD inclusiveCovariance(const TMatrixD& exclusiveCovariance){
  TMatrixD A = createInclusiveTransform(exclusiveCovariance.GetNrows());
  return A*exclusiveCovariance*A.Transpose(A);
}

inline TVectorD exclusiveRatios(const TVectorD& exclusive){
  double sum = exclusive.Sum();
  TVectorD ratios(exclusive.GetNrows());
  for (int i = 0; i < exclusive.GetNrows(); ++i){
    ratios(i) = exclusive(i)/sum;
  }
  return ratios;
}

inline TMatrixD exclusiveRatiosCovariance(const TVectorD& exclusive, const TMatrixD& exclusiveCovariance){
  TMatrixD A(exclusiveCovariance.GetNrows(), exclusiveCovariance.GetNcols());
  for (int i = 0; i < exclusiveCovariance.GetNrows(); ++i){
    for (int k = 0; k < exclusiveCovariance.GetNcols(); ++k){
      if (i==k) A(i, k) = (exclusive.Sum() - exclusive(i)) / (exclusive.Sum()*exclusive.Sum());
      else A(i, k) = -1/(exclusive.Sum()*exclusive.Sum());
    }
  }
  return A*exclusiveCovariance*A.Transpose(A);
}

inline TVectorD inclusiveRatios(const TVectorD& inclusive){
  TVectorD ratios(inclusive.GetNrows());
  for (int i = 0; i < inclusive.GetNrows(); ++i){
    ratios(i) = inclusive(i)/inclusive(0);
  }
  return ratios;
}

inline TMatrixD inclusiveRatiosCovariance(const TVectorD& inclusive, const TMatrixD& inclusiveCovariance){
  TMatrixD A(inclusiveCovariance.GetNrows(), inclusiveCovariance.GetNcols());
  for (int i = 0; i < inclusiveCovariance.GetNrows(); ++i){
    for (int k = 0; k < inclusiveCovariance.GetNcols(); ++k){
      if (i == k && i == 0) A(i, k) = 0;
      else if (i == k) A(i, k) = 1/inclusive(0);
      else A(i, k) = 0;
    }
  }
  return A*inclusiveCovariance*A.Transpose(A);
}

inline TVectorD errorsFromCovariance(const TMatrixD& covariance){
  TVectorD errors(covariance.GetNrows());
  for (int i = 0; i < covariance.GetNrows(); ++i){
    for (int j = 0; j < covariance.GetNcols(); ++j){
      if (i==j) errors(i) = sqrt(covariance(i,j));
    }
  }
  return errors;
}

inline TVectorD systematicErrors(const TVectorD& orig, const TVectorD& varied){
  TVectorD error(orig.GetNrows());
  for (int i = 0; i < error.GetNrows(); ++i){
    error(i) = varied(i) - orig(i);
  }
  return error;
}

*/


// It books histograms according to Jet multiplicity
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

/*
inline TGraphAsymmErrors multiplyTGraphs(const std::vector<TGraphAsymmErrors*>& graphs, const char* name) {
  if (graphs.empty()){
    cout << "ERROR!!!!! in multiplyTGraphs: no graphs to add" << endl;
    TGraphAsymmErrors out;
    out.SetNameTitle(name, name);
    return out; 
  }
  if (!graphs[0]){
    cout << "ERROR!!!!! in multiplyTGraphs: invalid pointer to the first graph" << endl;
    TGraphAsymmErrors out;     
    out.SetNameTitle(name, name);
    return out;
  }
  for (int i = 1; i < graphs.size(); ++i){
    if (!graphs[i]){
      cout << "ERROR!!!!! in multiplyTGraphs: invalid pointer to the graph " << i << endl;
      TGraphAsymmErrors out;
      out.SetNameTitle(name, name);
      return out;
    } //else if (graphs[i]->GetN() != graphs[0]->GetN()) {
        //cout << "ERROR!!!!! in multiplyTGraphs: different number of points " << i << endl;
        //TGraphAsymmErrors out;
        //out.SetNameTitle(name, name);
        //return out;
  }
  int n = graphs[0]->GetN();
  TVectorD vx(n);
  TVectorD vy(n);
  TVectorD vexl(n);
  TVectorD vexh(n);
  TVectorD veyl(n);
  TVectorD veyh(n);
  for ( int i = 0; i < n; ++i ){
    vy(i) = 1.;
    //we are assuming that the error on x is the binning and that it is the same
    vexl(i) = graphs[0]->GetErrorXlow(i);
    vexh(i) = graphs[0]->GetErrorXhigh(i);
    for (int j = 0; j < graphs.size(); ++j){
      double x = 0., y = 1.;
      graphs[j]->GetPoint(i, x, y);
      vx(i)   = x;
      vy(i)   *= y;
      double product = 1.;
      for (int k = 0; k < graphs.size(); ++k){
        double x2 = 0., y2 = 1.;
        graphs[k]->GetPoint(i, x2, y2);
        if (k != j) product *= y2;
      }
      veyl(i) += product * graphs[j]->GetErrorYlow(i);
      veyh(i) += product * graphs[j]->GetErrorYhigh(i);
    }
  }

  TGraphAsymmErrors out(vx, vy, vexl, vexh, veyl, veyh);
  out.SetNameTitle(name, name);
  return out;
}


//NEED TO CHECK THIS!!
inline void correctByEfficiency(TH1D* histo, TGraphAsymmErrors* eff){
  for (int i = 1; i <= histo->GetNbinsX(); ++i){
    double x = 0., y = 1.;
    eff->GetPoint(i-1, x, y);
    //numeric protection
    y = (y>0.01) ? y : 1.;
    histo->SetBinContent(i, histo->GetBinContent(i)/y);
    double error = histo->GetBinContent(i)/(y*y) * eff->GetErrorYlow(i) + histo->GetBinError(i)/y;
    histo->SetBinError(i, error);
  }
}
*/
#endif
