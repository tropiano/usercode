#ifndef PATAnalysisUtilities_h
#define PATAnalysisUtilities_h

#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

#include <vector>
#include <math.h>
#include <sstream>
#include <string>
#include <assert.h>

#include "TMatrixD.h"
#include "TVectorD.h"

static double ptmucut = 20.;
static double etamucut = 2.4;
static double zmassmin = 60.;
static double zmassmax = 120.;
static double zmassgenmin = 50.;
static double minnhit = 11.;
static double maxchi2 = 10.;
static double maxecaletinveto = 4.;
static double maxhcaletinveto = 6.;
static double dxycut = 0.02;
/*
inline bool applyCuts(const PhysVarTreeMuon& muon, const std::vector<bool (*)(const PhysVarTreeMuon&)>& cuts) {
  std::vector<bool (*)(const PhysVarTreeMuon&)>::const_iterator begin = cuts.begin();
  std::vector<bool (*)(const PhysVarTreeMuon&)>::const_iterator end = cuts.end();
  std::vector<bool (*)(const PhysVarTreeMuon&)>::const_iterator i;
  for (i = begin; i != end; ++i){
    if ((*i)(muon) == false) return false;
  }
  return true;
}

inline bool singleMu_Tag(const PhysVarTreeMuon& muon){
  return muon._pt > 25 && 
         fabs(muon._eta)<2.1 && 
         (muon._hcalIso + muon._ecalIso + muon._trackIso)/muon._pt < 0.01 && 
         muon._etInEcalVeto < maxecaletinveto &&
         muon._etInHcalVeto < maxhcaletinveto &&
         muon._nhit > 15 && 
         muon._chi2 < 3 &&
         muon._dxy < 0.05 &&
         muon._leptonID == 1;
}

inline bool singleMu_isPositive(const PhysVarTreeMuon& muon){
  return muon._charge > 0;
}

inline bool singleMu_isNegative(const PhysVarTreeMuon& muon){
  return muon._charge < 0;
}
*/


inline bool singleMu_PtEta(const pat::Muon& muon){
  return muon.pt() > ptmucut && fabs(muon.eta())<etamucut;
}

inline bool singleMu_QualityCuts(const pat::Muon& muon){
  //is this the same as before? or this is total track hits
  return muon.numberOfValidHits() > minnhit && muon.normChi2() < maxchi2 ;
}

inline bool singleMu_DXY(const pat::Muon& muon){
  return muon.dB()<dxycut;
}

inline bool singleMu_Isolation(const pat::Muon& muon){//, double isocut = 0.3){
  const pat::IsoDeposit* hcalIso = muon.isoDeposit(pat::HCalIso);
  const pat::IsoDeposit* ecalIso = muon.isoDeposit(pat::ECalIso);
  return (muon.hcalIso() + muon.ecalIso() + muon.trackIso())/muon.pt() < 0.3 &&
           ecalIso->depositWithin(ecalIso->veto().dR, pat::IsoDeposit::Vetos(), true) < maxecaletinveto &&
           hcalIso->depositWithin(hcalIso->veto().dR, pat::IsoDeposit::Vetos(), true) < maxhcaletinveto;
}

/*
inline bool GenSelected(const std::vector<PhysVarTree>* ZGEN, const std::vector<PhysVarTreeGenParticle>* ZGENDauGenParticle){
  return ZGEN->size()==1 && ZGENDauGenParticle->size() == 2 && (*ZGEN)[0]._mass > zmassgenmin;
  //cout << "ZGEN " << ZGEN << endl; 
}

inline bool GenSelectedInAcceptance(const std::vector<PhysVarTree>* ZGEN, const std::vector<PhysVarTreeGenParticle>* ZGENDauGenParticle){
  return ZGEN->size()==1 && ZGENDauGenParticle->size() == 2 && (*ZGEN)[0]._mass > zmassmin && (*ZGEN)[0]._mass < zmassmax 
         && (*ZGENDauGenParticle)[0]._pt>ptmucut && fabs((*ZGENDauGenParticle)[0]._eta)<etamucut
         && (*ZGENDauGenParticle)[1]._pt>ptmucut && fabs((*ZGENDauGenParticle)[1]._eta)<etamucut;
    //cout << "ZGEN " << ZGEN << endl; 
}
*/


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

inline bool isMuonTriggered(const pat::TriggerEvent& triggers){
  return isTriggered(triggers, "HLT_Mu15") || isTriggered(triggers, "HLT_DoubleMu3");
}

inline bool isJetTriggered(const pat::TriggerEvent& triggersREC){
  return isTriggered(triggersREC, "HLT_Jet30");
}
/*
//to do: try to avoid the copy 
inline std::pair<bool, std::vector<pat::Muon> > RecSelected_GlobalMuons(const std::vector<pat::Muon>& MuREC, int cut) {
  std::vector<pat::Muon> output;
  //at least two muons in the acceptance with pt > 20  
  if (MuREC.size() < cut) return std::make_pair(false, output);
  unsigned int count = 0; 
  std::vector<pat::Muon>::const_iterator imu;
  for (imu = MuREC.begin(); imu != MuREC.end(); ++imu){
    //if (imu->_pt > ptmucut && fabs(imu->_eta)<etamucut) {
    if (singleMu_PtEta(*imu)){
      count++;
      output.push_back(*imu);
    }  
  }
  if (count < cut) return std::make_pair(false, output);
  else return std::make_pair(true, output);
}

inline std::pair<bool, std::vector<pat::Muon> > RecSelected_OppositeCharge(const std::vector<pat::Muon>& MuREC) {
  //if (!RecSelectedTwoMuons(MuREC)) return false;
  std::vector<pat::Muon> output;
  if (MuREC.size() == 0) return std::make_pair(false, output);
  std::vector<pat::Muon>::const_iterator imu;
  //collections are ordered so the first element is in acceptance as selected by RecSelectedTwoMuons
  double charge0 = MuREC.front().charge(); 
  for (imu = MuREC.begin()+1; imu != MuREC.end(); ++imu){
    //among the others, that are still in acceptance, check for opposite charge
    if (imu->pt() > ptmucut && fabs(imu->eta()) < etamucut && imu->charge() * charge0 < 0) {
      output = MuREC;
      return std::make_pair(true, output);
    }  
  }
  return std::make_pair(false, output);
}

//////////////////////////////////////////////
//Warning! this includes the two previous cuts
//////////////////////////////////////////////
inline bool RecSelectedTwoMuonsOppositeCharge_Mass(const std::vector<reco::CompositeCandidate>& ZREC){
  if (! ZREC.size()) return false;
  if (ZREC.size() != 1){
    std::cout << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! " << std::endl;
    throw cms::Exception("PATAnalysis:RecSelectedTwoMuonsOppositeCharge_Mass") << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! ";
    return false;
  }
  if ( !(ZREC[0].mass() > zmassmin && ZREC[0].mass() < zmassmax) ) return false;
  std::vector<pat::Muon> daughters;
  daughters.push_back(*((const pat::Muon*) ZREC[0].daughter(0)));
  daughters.push_back(*((const pat::Muon*) ZREC[0].daughter(1)));
  return RecSelected_GlobalMuons(daughters, 2).first && RecSelected_OppositeCharge(daughters).first;
}

inline std::pair<bool, std::vector<pat::Muon> > RecSelected_QualityCuts(const std::vector<pat::Muon>& MuREC, int cut){
  //if (!RecSelectedTM_OppositeCharge(MuREC)) return false;
  std::vector<pat::Muon>::const_iterator imu;
  std::vector<pat::Muon> selected;
  for (imu = MuREC.begin(); imu != MuREC.end(); ++imu){
    //if (imu->_nhit > minnhit && imu->_chi2 < maxchi2) selected.push_back(*imu);     
    if (singleMu_QualityCuts(*imu)) selected.push_back(*imu);     
  }
  if (selected.size() < cut) return std::make_pair(false, selected);
  else return std::make_pair(true, selected);
}

inline std::pair<bool, std::vector<pat::Muon> > RecSelected_DXY(const std::vector<pat::Muon>& MuREC, int cut){
  //if (!RecSelectedTM_OC_QualityCuts(MuREC)) return false;
  std::vector<pat::Muon>::const_iterator imu;
  std::vector<pat::Muon> selected;
  for (imu = MuREC.begin(); imu != MuREC.end(); ++imu){
    //if (imu->_dxy<dxycut) selected.push_back(*imu);
    if (singleMu_DXY(*imu)) selected.push_back(*imu);
  }
  if (selected.size() < cut) return std::make_pair(false, selected);
  else return std::make_pair(true, selected);
}

//here we have to put the daughters?
//we have to check the size
inline std::pair<bool, std::vector<pat::Muon> > RecSelected_Isolation(const std::vector<reco::CompositeCandidate>& ZREC, double isocut, int cut){
  std::vector<pat::Muon> selected;
  reco::CompositeCandidate::const_iterator imu;
  for (imu = ZREC[0].begin(); imu < ZREC[0].end(); ++imu){
    //if ( (imu->_hcalIso + imu->_ecalIso + imu->_trackIso)/imu->_pt < isocut && 
    //     imu->_etInEcalVeto < maxecaletinveto &&
    //     imu->_etInHcalVeto < maxhcaletinveto ) selected.push_back(*imu);
    //if (singleMu_Isolation(*imu,  isocut)) selected.push_back(*imu);
    if (singleMu_Isolation(*((const pat::Muon*) *imu)) ) selected.push_back(*((const pat::Muon*) *imu));
  }
  if (selected.size() < cut) return std::make_pair(false, selected);
  else return std::make_pair(true, selected);
}

*/


inline bool RecSelected(const std::vector<reco::CompositeCandidate>& ZREC, double isocut){
  //"@ZzjetsRECO.size()==1 && @ZzjetsRECODauMuon.size()==2 && ZzjetsRECO._mass>60. && ZzjetsRECO._mass<120. && ZzjetsRECODauMuon[0]._pt>20 && abs(ZzjetsRECODauMuon[0]._eta)<2.4 && ZzjetsRECODauMuon[1]._pt>20 && abs(ZzjetsRECODauMuon[1]._eta)<2.4 && (ZzjetsRECODauMuon[0]._hcalIso+ZzjetsRECODauMuon[0]._ecalIso+ZzjetsRECODauMuon[0]._trackIso)/ZzjetsRECODauMuon[0]._pt<0.3 && (ZzjetsRECODauMuon[1]._hcalIso+ZzjetsRECODauMuon[1]._ecalIso+ZzjetsRECODauMuon[1]._trackIso)/ZzjetsRECODauMuon[1]._pt<0.3 && ZzjetsRECODauMuon[0]._nhit>11 && ZzjetsRECODauMuon[1]._nhit>11"
  if (ZREC.size() > 1){
    std::cout << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! " << std::endl;
    throw cms::Exception("PATAnalysis:RecSelectedTwoMuonsOppositeCharge_Mass") << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! ";
    return false;
  }
  const pat::Muon* dau0 = (const pat::Muon*) ZREC[0].daughter(0);
  const pat::Muon* dau1 = (const pat::Muon*) ZREC[0].daughter(1);
  const pat::IsoDeposit* hcalIso0 = dau0->isoDeposit(pat::HCalIso);
  const pat::IsoDeposit* ecalIso0 = dau0->isoDeposit(pat::ECalIso);
  const pat::IsoDeposit* hcalIso1 = dau1->isoDeposit(pat::HCalIso);
  const pat::IsoDeposit* ecalIso1 = dau1->isoDeposit(pat::ECalIso);

  
  return ZREC.size()==1 && ZREC[0].mass()>zmassmin && ZREC[0].mass()<zmassmax &&
         dau0->pt() > ptmucut && fabs(dau0->eta()) < etamucut && 
         dau1->pt() > ptmucut && fabs(dau1->eta()) < etamucut &&
         (dau0->hcalIso() + dau0->ecalIso() + dau0->trackIso()) / dau0->pt() < isocut &&  
         (dau1->hcalIso() + dau1->ecalIso() + dau1->trackIso()) / dau1->pt() < isocut &&  
         dau0->numberOfValidHits() > minnhit && dau1->numberOfValidHits() > minnhit &&
         dau0->normChi2() < maxchi2 && dau1->normChi2() < maxchi2 &&
         dau0->dB() < dxycut && dau1->dB() < dxycut && 
         ecalIso0->depositWithin(ecalIso0->veto().dR, pat::IsoDeposit::Vetos(), true) < maxecaletinveto &&
         ecalIso1->depositWithin(ecalIso1->veto().dR, pat::IsoDeposit::Vetos(), true) < maxecaletinveto &&
         hcalIso0->depositWithin(hcalIso0->veto().dR, pat::IsoDeposit::Vetos(), true) < maxhcaletinveto && 
         hcalIso1->depositWithin(hcalIso1->veto().dR, pat::IsoDeposit::Vetos(), true) < maxhcaletinveto ; 
}

inline bool RecSelectedWithTrigger(const std::vector<reco::CompositeCandidate>& ZREC, const pat::TriggerEvent& triggers, double isocut){
  //"@ZzjetsRECO.size()==1 && @ZzjetsRECODauMuon.size()==2 && ZzjetsRECO._mass>60. && ZzjetsRECO._mass<120. && ZzjetsRECODauMuon[0]._pt>20 && abs(ZzjetsRECODauMuon[0]._eta)<2.4 && ZzjetsRECODauMuon[1]._pt>20 && abs(ZzjetsRECODauMuon[1]._eta)<2.4 && (ZzjetsRECODauMuon[0]._hcalIso+ZzjetsRECODauMuon[0]._ecalIso+ZzjetsRECODauMuon[0]._trackIso)/ZzjetsRECODauMuon[0]._pt<0.3 && (ZzjetsRECODauMuon[1]._hcalIso+ZzjetsRECODauMuon[1]._ecalIso+ZzjetsRECODauMuon[1]._trackIso)/ZzjetsRECODauMuon[1]._pt<0.3 && ZzjetsRECODauMuon[0]._nhit>11 && ZzjetsRECODauMuon[1]._nhit>11"
  
  bool isTriggered = isMuonTriggered(triggers);
  if (!isTriggered) return false;
  return RecSelected(ZREC, isocut);
}
/*
inline bool RecSelectedNoIso(const std::vector<PhysVarTree>* ZREC, const std::vector<PhysVarTreeMuon>* ZRECDauMuon){
  //"@ZzjetsRECO.size()==1 && @ZzjetsRECODauMuon.size()==2 && ZzjetsRECO._mass>60. && ZzjetsRECO._mass<120. && ZzjetsRECODauMuon[0]._pt>20 && abs(ZzjetsRECODauMuon[0]._eta)<2.4 && ZzjetsRECODauMuon[1]._pt>20 && abs(ZzjetsRECODauMuon[1]._eta)<2.4 && (ZzjetsRECODauMuon[0]._hcalIso+ZzjetsRECODauMuon[0]._ecalIso+ZzjetsRECODauMuon[0]._trackIso)/ZzjetsRECODauMuon[0]._pt<0.3 && (ZzjetsRECODauMuon[1]._hcalIso+ZzjetsRECODauMuon[1]._ecalIso+ZzjetsRECODauMuon[1]._trackIso)/ZzjetsRECODauMuon[1]._pt<0.3 && ZzjetsRECODauMuon[0]._nhit>11 && ZzjetsRECODauMuon[1]._nhit>11"

  return ZREC->size()==1 && ZRECDauMuon->size() == 2 && (*ZREC)[0]._mass>60. && (*ZREC)[0]._mass<120. &&
         (*ZRECDauMuon)[0]._pt>20. && fabs((*ZRECDauMuon)[0]._eta)<2.4 && (*ZRECDauMuon)[1]._pt>20. && fabs((*ZRECDauMuon)[1]._eta)<2.4 &&
         (*ZRECDauMuon)[0]._leptonID == 1 && (*ZRECDauMuon)[1]._leptonID == 1 && 
         (*ZRECDauMuon)[0]._nhit>11 && (*ZRECDauMuon)[1]._nhit>11;
}

inline bool GenAndRecSelected(const std::vector<PhysVarTree>* ZGEN, const std::vector<PhysVarTreeGenParticle>* ZGENGenParticle,
                       const std::vector<PhysVarTree>* ZREC, const std::vector<PhysVarTreeMuon>* ZRECDauMuon, double isocut) {
  return GenSelected(ZGEN, ZGENGenParticle) && RecSelected(ZREC, ZRECDauMuon, isocut);
} 

inline std::vector<PhysVarTreeJet> GetJets(const std::vector<PhysVarTreeJet>* jets, double ptmin, double etamax){
  std::vector<PhysVarTreeJet> selectedjets;
  std::vector<PhysVarTreeJet>::const_iterator ijet;
  for (ijet = jets->begin(); ijet != jets->end(); ++ijet){
    if (ijet->_pt > ptmin && fabs(ijet->_eta) < etamax) selectedjets.push_back(*ijet);
  }
  return selectedjets;
}

inline double deltaYFwdBwd(const std::vector<PhysVarTreeJet>& jets){
  assert(jets.size()>1);
  double maxy = -999999;
  double miny =  999999;
  for (int i = 0; i < jets.size(); ++i){
    if (jets[i]._y > maxy){
      maxy = jets[i]._y;
    }
    if (jets[i]._y < miny){
      miny = jets[i]._y;
    }
  }
  return maxy-miny ;   
}

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


//book histograms according to multiplicity
inline void addHistosVsMulti(int multi, std::string name, std::string title, int nbin, double min, double max, std::vector<TH1D*>& array){
  if (array.size() < multi+1){
    for (int i = array.size(); i < multi+1; ++i){
      std::stringstream fullname;
      fullname << name << i << "jets";
      std::stringstream fulltitle;
      std::string ext;
      if (i == 1) ext = "st";
      else if (i == 2) ext = "nd";
      else if (i == 3 ) ext = "rd";
      else ext = "th";
      fulltitle << i << "^{" << ext << "} jet " << title;

      array.push_back(new TH1D(fullname.str().c_str(), fulltitle.str().c_str(), nbin, min, max));

    }
  }
}

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
