#ifndef CandUtils_AddFourMomentaTransversePlane_h
#define CandUtils_AddFourMomentaTransversePlane_h
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"

struct AddFourMomentaTransversePlane {
/// default constructor

  AddFourMomentaTransversePlane() { }
  /// constructor

  explicit AddFourMomentaTransversePlane( const edm::ParameterSet & ) { }
  /// set up a candidate

  void set( reco::Candidate& c ) const;
};

#endif

