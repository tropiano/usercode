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

#include <vector>
#include <math.h>
#include <sstream>
#include <string>
#include <assert.h>

#include "TMatrixD.h"
#include "TVectorD.h"

static double ptmucut_leading = 20.;
static double ptmucut_second = 10.;
static double etamucut_leading = 2.1;
static double etamucut_second = 2.4;
static double zmassmin = 60.;
static double zmassmax = 110.;
static double zmassgenmin = 50.;
static double minnhit = 10.;
static double maxchi2 = 10.;
static double maxecaletinveto = 4.;
static double maxhcaletinveto = 6.;
static double dxycut = 0.2;

template <class T> bool sortByPt(const T* j1, const T* j2){ return j1->pt() > j2->pt() ; }

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

inline bool singleMu_PtEta(const pat::Muon& muon){
  return muon.pt() > ptmucut_leading && fabs(muon.eta()) < etamucut_leading;
}

inline bool leadingMu_PtEta(const pat::Muon& muon){
  return muon.pt() > ptmucut_leading && fabs(muon.eta()) < etamucut_leading;
}

inline bool secondMu_PtEta(const pat::Muon& muon){
  return muon.pt() > ptmucut_second && fabs(muon.eta()) < etamucut_second;
}

inline bool singleMu_QualityCuts(const reco::Candidate& cand){
  const pat::Muon* muon = dynamic_cast<const pat::Muon*>(&cand);
  if (!muon) throw cms::Exception("PATAnalysis:singleMu_QualityCuts") << "singleMu_QualityCuts is run on an object which is not a muon ";
  
  //reco::TrackRef gm = muon->globalTrack() ; 
 
  //is this the same as before? or this is total track hits
  return muon->isGlobalMuon() && muon->isTrackerMuon() && 
         //muon->track()->hitPattern().numberOfValidMuonHits() > 0 && muon->track()->hitPattern().numberOfValidTrackerHits() > minnhit &&
         muon->numberOfValidHits() > minnhit &&
         //muon->numberOfChambers() > 0 && 
         //muon->numberOfMatches() > 1 &&               
         muon->normChi2() < maxchi2 ;
}

inline bool singleMu_DXY(const reco::Candidate& cand){
  const pat::Muon* muon = dynamic_cast<const pat::Muon*>(&cand);
  if (!muon) throw cms::Exception("PATAnalysis:singleMu_DXY") << "singleMu_DXY is run on an object which is not a muon ";
  return muon->dB()<dxycut;
}

inline bool singleMu_Isolation(const reco::Candidate& cand){//, double isocut = 0.3){
  const pat::Muon* muon = dynamic_cast<const pat::Muon*>(&cand);
  if (!muon) throw cms::Exception("PATAnalysis:singleMu_Isolation") << "singleMu_Isolation is run on an object which is not a muon ";
  const reco::MuonIsolation& iso03 = muon->isolationR03(); 
  return (iso03.hadEt + iso03.emEt + iso03.sumPt)/muon->pt() < 0.15 ; //&&
           //iso03.emVetoEt < maxecaletinveto &&
           //iso03.hadVetoEt < maxhcaletinveto;
}


inline bool GenSelectedMuon(const std::vector<reco::CompositeCandidate>& ZGEN){
  if (ZGEN.size() == 0) return false;
  if (ZGEN.size() > 1){
    std::cout << "ERROR! Multiple Gen Z candidates found, you have to choose one before arriving here! " << std::endl;
    throw cms::Exception("PATAnalysis:RecSelectedTwoMuonsOppositeCharge_Mass") << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! ";
    return false;
  }  
  return ZGEN.size()==1 && ZGEN[0].mass() > zmassgenmin;
  //cout << "ZGEN " << ZGEN << endl; 
}

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

  return ZGEN.size()==1 && ZGEN[0].mass() > zmassmin && ZGEN[0].mass() < zmassmax 
         && leading->pt() > ptmucut_leading && fabs(leading->eta()) < etamucut_leading
         && second->pt() > ptmucut_second && fabs(second->eta()) < etamucut_second;
}



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
    if (ipath->name() == triggername) {
      if (!ipath->wasRun()){
        std::cout << "Warning! " << triggername << " was not run for this event" << std::endl;  
      }
      return ipath->wasAccept();
    }  
  }
  return false;

  //return path->wasAccept();
}

inline bool isMuonTriggered(const pat::TriggerEvent& triggers){
  return isTriggered(triggers, "HLT_Mu5");
}

inline bool isJetTriggered(const pat::TriggerEvent& triggersREC){
  return isTriggered(triggersREC, "HLT_Jet30U");
}

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
inline bool RecSelected_MuonMass(const std::vector<reco::CompositeCandidate>& ZREC){
  if (! ZREC.size()) return false;
  /*if (ZREC.size() != 1){
    std::cout << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! " << std::endl;
    throw cms::Exception("PATAnalysis:RecSelectedTwoMuonsOppositeCharge_Mass") << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! ";
    return false;
  }*/
  if ( ZREC[0].mass() > zmassmin && ZREC[0].mass() < zmassmax ) return true;
  return false;
}

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

inline std::pair<bool, std::vector<const pat::Muon*> > RecSelected_DXY(const std::vector<const pat::Muon*>& MuREC, unsigned int cut){
  std::vector<const pat::Muon*>::const_iterator imu;
  std::vector<const pat::Muon*> selected;
  for (imu = MuREC.begin(); imu != MuREC.end(); ++imu){
    if (singleMu_DXY(**imu)) selected.push_back(*imu);
  }
  if (selected.size() < cut) return std::make_pair(false, selected);
  else return std::make_pair(true, selected);
}

//we have to check the size
inline std::pair<bool, std::vector<const pat::Muon*> > RecSelected_Isolation(const std::vector<const pat::Muon*>& Muons, double isocut, unsigned int cut){
  std::vector<const pat::Muon*> selected;
  std::vector<const pat::Muon*>::const_iterator imu;
  for (imu = Muons.begin(); imu != Muons.end(); ++imu){
    if ( singleMu_Isolation(**imu) ) selected.push_back(*imu);
  }
  if (selected.size() < cut) return std::make_pair(false, selected);
  else return std::make_pair(true, selected);
}



inline bool RecSelectedMuon(const std::vector<reco::CompositeCandidate>& ZREC, double isocut, const pat::Muon* extdau0 = 0,  const pat::Muon* estdau1 = 0){
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

  
  return ZREC.size() > 0 && ZREC[0].mass() > zmassmin && ZREC[0].mass() < zmassmax &&
         leadingMu_PtEta(*leading) && secondMu_PtEta(*second) &&
         singleMu_QualityCuts(*leading) && singleMu_QualityCuts(*second) &&
         singleMu_DXY(*leading) && singleMu_DXY(*second) && 
         singleMu_Isolation(*leading);               
}

inline bool RecSelectedMuonWithTrigger(const std::vector<reco::CompositeCandidate>& ZREC, const pat::TriggerEvent& triggers, double isocut, 
                                   const pat::Muon* extdau0 = 0, const pat::Muon* extdau1 = 0){
  
  bool isTriggered = isMuonTriggered(triggers);
  if (!isTriggered) return false;
  return RecSelectedMuon(ZREC, isocut, extdau0, extdau1);
}

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
 
  return ZREC.size() > 0 && ZREC[0].mass() > zmassmin && ZREC[0].mass() < zmassmax &&
         leadingMu_PtEta(*leading) && secondMu_PtEta(*second) &&
         singleMu_QualityCuts(*leading) && singleMu_QualityCuts(*second) &&
         singleMu_DXY(*leading) && singleMu_DXY(*second);
 
}

template<class JET> 
std::vector<const JET*> 
GetJets(const std::vector<JET>& jets, double ptmin, double etamax){
  std::vector<const JET*> selectedjets;
  for (unsigned int i = 0; i < jets.size();  ++i){
    if (jets[i].pt() > ptmin && fabs(jets[i].eta()) < etamax) selectedjets.push_back(&jets[i]);
  }
  return selectedjets;
}

template<class JET>
void
CleanJets(const std::vector<JET>& jets, const std::vector<const pat::Muon*> muons,  std::vector<JET>& cleanedjets, double deltaRmin = 0.5){
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
