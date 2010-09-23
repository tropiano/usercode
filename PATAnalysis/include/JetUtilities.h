#ifndef PATAnalysisJetUtilities_h
#define PATAnalysisJetUtilities_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include <iostream>


inline bool isJetMatched(const reco::Candidate& cand) {
  const pat::Jet* jet = dynamic_cast<const pat::Jet*>(&cand);
  if (!jet) {
    std::cout << "NOT A PAT JET IN isJetMatcjed" << std::endl;
    return false;
  }
  return jet->triggerObjectMatchesByPath("HLT_Jet30U").size() > 0 ;
  //return jet->pt()>30.;
}

inline bool tightJetPt(const reco::Candidate& cand) {
  const pat::Jet* jet = dynamic_cast<const pat::Jet*>(&cand);
  if (!jet) {
    std::cout << "NOT A PAT JET IN isJetMatcjed" << std::endl;
    return false;
  }
  return jet->pt()>25.;
}

#endif
