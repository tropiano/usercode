// -*- C++ -*-
//
// Package:    DiffJetRate
// Class:      DiffJetRate
// 
/**\class DiffJetRate DiffJetRate.cc MyAnalysis/DiffJetrate/src/DiffJetrate.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  pts/1
//         Created:  Tue Jun 24 18:05:01 CEST 2008
// $Id$
//
//


// system include files
#include <memory>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Common/interface/View.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/Registry.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

//
// class decleration
//

class DiffJetRate : public edm::EDProducer {
   public:
      explicit DiffJetRate(const edm::ParameterSet&);
      ~DiffJetRate();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      //double minDistance(const edm::View<reco::Jet>& pjets) const;
      double minDistance(const edm::View<reco::Candidate>& pjets, const JetCorrector* corr = 0) const;
      
      // ----------member data ---------------------------
      double _rparam;
      edm::InputTag _src;
      int _njets;
      std::string _label;
      std::string _jetcorrname;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
DiffJetRate::DiffJetRate(const edm::ParameterSet& iConfig) 
{

    _njets = iConfig.getParameter<int>("njet");
    _src = iConfig.getParameter<edm::InputTag>("src");
    if (iConfig.exists("JetCorrector")){
     _jetcorrname = iConfig.getParameter<std::string>("JetCorrector");
    } else _jetcorrname = "none"; 
   //register products    
    std::stringstream label;
    label << "DiffJetRate" << _njets << _njets-1; 
    _label = label.str();
    produces<double>(_label);

}


DiffJetRate::~DiffJetRate()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
DiffJetRate::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   //edm::Handle<edm::View <reco::Jet> > pIn;
   edm::Handle<edm::View <reco::Candidate> > pIn;
   iEvent.getByLabel(_src,pIn);

   if (pIn.failedToGet()){
      edm::LogWarning("DiffJetRate") << "No jet products for this event, DiffJetRate will return -99999" << endl;
      std::auto_ptr<double> pOut(new double(-99999));
      iEvent.put(pOut);
      return;
   }
   
   //track jet provenance
   edm::Provenance const* prov = pIn.provenance();
   //std::cout << prov->product() << std::endl;
   edm::ParameterSetID const& psetid = prov->product().psetID();
   edm::ParameterSet jetpset;
   //pset registry
   const edm::pset::Registry * registry = edm::pset::Registry::instance();
   bool found = registry->getMapped(psetid, jetpset);
   if (!found) {
      throw cms::Exception("DiffJetRate") << "Unable to retrieve original jet finder pSet " << endl; 
   }

   //std::cout << jetpset << std::endl; 
   //now check that the jet finder has been run properly for our task
   if (jetpset.getParameter<string>("@module_type") != "ExclInclFastJetProducer"){
      throw cms::Exception("DiffJetRate") << "FastJet Jets should be used for Differential Jet Rates, not " << jetpset.getParameter<string>("@module_type") << endl;
   }

   double dcut = jetpset.getParameter<double>("dcut");
   int njet    = jetpset.getParameter<int>("njets");
   if (!(njet > 0 && dcut == -1)){
      throw cms::Exception("DiffJetRate") << "FastJet Jets should be run in Exclusive mode (njet > 0 && dcut == -1) for Differential Jet Rates " << endl;
   }

   if (njet != _njets){
      throw cms::Exception("DiffJetRate") << "Exclusive number of jets " << njet << " does not match with " << _njets << "->" << _njets-1 << " jet rate calculation " << endl;
   }
   //set the R parameter to be the same as in the jet finder
   _rparam = jetpset.getParameter<double>("FJ_ktRParam");

   //get the jetcorrector from es
   const JetCorrector* corrector = 0;
   if (_jetcorrname != "none"){
     corrector = JetCorrector::getJetCorrector(_jetcorrname, iSetup);
     if (!corrector){
       throw cms::Exception("DiffJetRate") << "JetCorrector not found " << endl;
     }
   }  

   double min = -99999;
   if (pIn->size() == _njets){
      min = minDistance(*(pIn.product()), corrector);
      min = log10(sqrt(min));
   } else {
      edm::LogError("DiffJetRate") << "number of exclusive jets " << pIn->size() << " does not match the requested one " << _njets << endl;
   }
   std::auto_ptr<double> pOut(new double(min));
   iEvent.put(pOut, _label);
}

// ------------ method called once each job just before starting event loop  ------------
void 
DiffJetRate::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DiffJetRate::endJob() {
}

//double DiffJetRate::minDistance(const edm::View<reco::Jet>& pjets) const{
double DiffJetRate::minDistance(const edm::View<reco::Candidate>& pjets, const JetCorrector* corr) const{
  double min = 999999;
  //for(edm::View<reco::Jet>::const_iterator i = pjets.begin(); i != pjets.end(); ++i){ 
  for(edm::View<reco::Candidate>::const_iterator i = pjets.begin(); i != pjets.end(); ++i){
    double scale = 1.;
    if (corr) scale = corr->correction(i->p4());
    //std::cout << "scale " << scale << std::endl;
    double beamdist = i->pt()*i->pt()*scale*scale;
    if (beamdist < min) min = beamdist;
  }

  //for(edm::View<reco::Jet>::const_iterator i1 = pjets.begin(); i1 < pjets.end()-1; ++i1){
  for(edm::View<reco::Candidate>::const_iterator i1 = pjets.begin(); i1 < pjets.end()-1; ++i1){
    //for(edm::View<reco::Jet>::const_iterator i2 = i1+1; i2 < pjets.end(); ++i2){
    for(edm::View<reco::Candidate>::const_iterator i2 = i1+1; i2 < pjets.end(); ++i2){
      double scale1 = 1.;
      double scale2 = 1.;
      if (corr) {
        scale1 = corr->correction(i1->p4());
        scale2 = corr->correction(i2->p4());
      }  
      double deltaR2 = (i1->y()-i2->y())*(i1->y()-i2->y()) + 
                        deltaPhi(i1->phi(), i2->phi())*deltaPhi(i1->phi(), i2->phi());
      double jetjet = fmin(i1->pt()*i1->pt()*scale1*scale1, i2->pt()*i2->pt()*scale2*scale2) * deltaR2/(_rparam*_rparam);
      if (jetjet < min) min = jetjet;
    }
  }
  return min;
}

//define this as a plug-in
DEFINE_FWK_MODULE(DiffJetRate);
