#ifndef Firenze_JetAlgorithms_ExclInclFastJetWrapper_h
#define Firenze_JetAlgorithms_ExclInclFastJetWrapper_h

/*

Copied from RecoJets/JetAlgorithms in 1_6_12.
Later releases cannot do Exclusive jets

*/


#include "RecoParticleFlow/PFRootEvent/interface/ProtoJet.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>

class ExclInclFastJetWrapper
{
  //typedef reco::CandidateRef FJCand;
  //typedef const reco::CandidateRef constFJCand;
 public:
  ExclInclFastJetWrapper();
  ExclInclFastJetWrapper(const edm::ParameterSet& ps);
  ~ExclInclFastJetWrapper();
  void run (const JetReco::InputCollection& fInput, JetReco::OutputCollection* fOutput);
 private:
  //fastjet::JetDefinition jet_def;
  struct JetConfig;
  int theMode;
  JetConfig *theJetConfig;
  double thePtMin;
  double theDcut;
  int theNjets;
  // Subtraction-Parameters:
  bool theDoSubtraction;
  double theGhost_EtaMax;
  int theActive_Area_Repeats;
  //fastjet::ActiveAreaSpec theArea_Spec;
  double theGhostArea;
  double theMedian_Pt_Per_Area;
};


#endif

