#ifndef Firenze_JetUtilities_ExclInclFastJetProducer_h
#define Firenze_JetUtilities_ExclInclFastJetProducer_h
/*

Copied from RecoJets/JetAlgorithms in 1_6_12.
Later releases cannot do Exclusive jets

*/

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Common/interface/EDProductfwd.h"
#include "RecoJets/JetAlgorithms/interface/JetRecoTypes.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Firenze/JetUtilities/interface/ExclInclFastJetWrapper.h"

namespace cms
{
  class ExclInclFastJetProducer : public edm::EDProducer
  {
  public:

    ExclInclFastJetProducer(const edm::ParameterSet& ps);

    /**Default destructor*/
    virtual ~ExclInclFastJetProducer();
    /**Produces the EDM products*/
    virtual void produce(edm::Event& e, const edm::EventSetup& c);
    /** jet type */
    std::string jetType () const {return mJetType;}

    /** run algorithm itself */
    virtual bool runAlgorithm (const JetReco::InputCollection& fInput, JetReco::OutputCollection* fOutput);

  private:
    edm::InputTag mSrc;
    std::string mJetType;
    bool mVerbose;
    double mEtInputCut;
    double mEInputCut;
    double mJetPtMin;

    ExclInclFastJetWrapper alg_;
  };
}

#endif

