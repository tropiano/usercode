#include "TMath.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "PhysicsTools/PatExamples/plugins/PatTriggerAnalyzer.h"

#include <iostream> // DEBUG
#include <map>
#include <string>

#include "TH1D.h"
#include "TH2D.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
//#include "CondFormats/JetMETObjects/interface/TriggerEvent.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

class PFjetAnalyzer : public edm::EDAnalyzer {
  
 public:
  /// default constructor  
  explicit PFjetAnalyzer( const edm::ParameterSet & iConfig );
  /// default destructor
  ~PFjetAnalyzer();
  
 private:
  /// everythin that needs to be done before the event loop
  virtual void beginJob();
  /// everythin that needs to be done during the event loop
  virtual void analyze( const edm::Event & iEvent, const edm::EventSetup & iSetup );
  /// everythin that needs to be done after the event loop
  virtual void endJob();

  /// input for patTrigger  
  edm::InputTag trigger_;
  /// input for patTriggerEvent
  edm::InputTag triggerEvent_;
  /// input for patTriggerEvent
  edm::InputTag triggerTag_;
  
  /// input for muons
  edm::InputTag muons_;
  /// input for trigger match objects
  std::string   muonMatch_;
  /// input for muons
  edm::InputTag jets_;
  /// input for trigger match objects
  std::string  jetMatch_;
  //trigger name 
  std::string triggerName_;
  
  /// minimal id for meanPt plot
  //unsigned minID_;
  /// maximal id for meanPt plot
  //unsigned maxID_;
  //bool emulation
  //bool emulateHLT_;
  //jes shift bool

  //skim on a hlt path
  bool hltSkim_;

  edm::InputTag jetsPF_;
  edm::InputTag jetsCalo_;
  edm::InputTag jetsMatchedPF_;
  edm::InputTag jetsMatchedCalo_;
  
  
  /// histogram management
  std::map< std::string, TH1D* > histos1D_;
  std::map< std::string, TH2D* > histos2D_;
  
  /// internals for meanPt histogram calculation
  std::map< unsigned, unsigned > sumN_;
  std::map< unsigned, double >   sumPt_;
};

using namespace pat;
using namespace std;


PFjetAnalyzer::PFjetAnalyzer( const edm::ParameterSet & iConfig ) :
  triggerTag_( iConfig.getParameter< edm::InputTag >( "triggerTag" ) ),
  
  // input collection reco jets
  jetsPF_( iConfig.getParameter< edm::InputTag >( "jetsPF" ) ),
  //calo jets 
  jetsCalo_( iConfig.getParameter< edm::InputTag >( "jetsCalo") ),
  // input collection reco jets
  jetsMatchedPF_( iConfig.getParameter< edm::InputTag >( "jetsMatchedPF" ) ),
  // input collection reco jets
  jetsMatchedCalo_( iConfig.getParameter< edm::InputTag >( "jetsMatchedCalo" ) ),
  //hlt emiulation
  //emulateHLT_( iConfig.getParameter<bool>( "emulateHLT" ) ),
  //hlt skimming
  hltSkim_( iConfig.getParameter<bool>( "hltSkim" ) ),
  //jes shift emulation 
  //JESshift_( iConfig.getParameter<bool>( "JESshift" ) ),
  //trigger name 
  triggerName_( iConfig.getParameter<std::string>( "triggerName" ) ),
  histos1D_(), histos2D_()
{
}

PFjetAnalyzer::~PFjetAnalyzer()
{
}

void PFjetAnalyzer::beginJob()
{
  edm::Service< TFileService > fileService;
  //
  histos1D_[ "PFjetPt" ] = fileService->make< TH1D >( "PFjetPt", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "PFjetPt" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "PFjetPt" ]->SetYTitle( "# of objects" );
  //
  /*histos1D_[ "PFjetPt2" ] = fileService->make< TH1D >( "PFjetPt2", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "PFjetPt2" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "PFjetPt2" ]->SetYTitle( "# of objects" );
  
  histos1D_[ "PFjetPt3" ] = fileService->make< TH1D >( "PFjetPt3", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "PFjetPt3" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "PFjetPt3" ]->SetYTitle( "# of objects" );
  
  histos1D_[ "PFjetPt4" ] = fileService->make< TH1D >( "PFjetPt4", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "PFjetPt4" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "PFjetPt4" ]->SetYTitle( "# of objects" );
  
  histos1D_[ "PFjetPt5" ] = fileService->make< TH1D >( "PFjetPt5", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "PFjetPt5" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "PFjetPt5" ]->SetYTitle( "# of objects" );
  
  histos1D_[ "PFjetPt6" ] = fileService->make< TH1D >( "PFjetPt6", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "PFjetPt6" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "PFjetPt6" ]->SetYTitle( "# of objects" );*/
  
  histos1D_[ "matchedPt" ] = fileService->make< TH1D >( "matchedPt", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "matchedPt" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "matchedPt" ]->SetYTitle( "# of objects" );
  
  /* histos1D_[ "matchedPt2" ] = fileService->make< TH1D >( "matchedPt2", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "matchedPt2" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "matchedPt2" ]->SetYTitle( "# of objects" );
  
  histos1D_[ "matchedPt3" ] = fileService->make< TH1D >( "matchedPt3", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "matchedPt3" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "matchedPt3" ]->SetYTitle( "# of objects" );
  
  histos1D_[ "matchedPt4" ] = fileService->make< TH1D >( "matchedPt4", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "matchedPt4" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "matchedPt4" ]->SetYTitle( "# of objects" );
  
  histos1D_[ "matchedPt5" ] = fileService->make< TH1D >( "matchedPt5", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "matchedPt5" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "matchedPt5" ]->SetYTitle( "# of objects" );
  
  histos1D_[ "matchedPt6" ] = fileService->make< TH1D >( "matchedPt6", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  histos1D_[ "matchedPt6" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  histos1D_[ "matchedPt6" ]->SetYTitle( "# of objects" );*/
  
  histos1D_[ "offline_sel" ] = fileService->make< TH1D >( "offline_sel", "selected offline ", 2, 0.5, 1.5);
  histos1D_[ "offline_sel" ]->SetXTitle( " " );
  histos1D_[ "offline_sel" ]->SetYTitle( "# of objects" );
  //
  histos1D_[ "online_sel" ] = fileService->make< TH1D >( "online_sel", "selected offline ", 2, 0.5, 1.5);
  histos1D_[ "online_sel" ]->SetXTitle( " " );
  histos1D_[ "online_sel" ]->SetYTitle( "# of objects" );
  
  //
  // histos1D_[ "matchedPt6" ] = fileService->make< TH1D >( "matchedPt6", "p_{T} (GeV) of reco candidate", 150, 0., 150.);
  //histos1D_[ "matchedPt6" ]->SetXTitle( "Reco candidate p_{T} (GeV)" );
  //histos1D_[ "matchedPt6" ]->SetYTitle( "# of objects" );

}
// initialize counters for mean pt calculation

void PFjetAnalyzer::analyze( const edm::Event & iEvent, const edm::EventSetup & iSetup )
{
  

  // PAT object collection
  edm::Handle< reco::PFJetCollection > PFjets;
  iEvent.getByLabel( jetsPF_, PFjets );
  //calo jets 
  edm::Handle< reco::CaloJetCollection > Calojets;
  iEvent.getByLabel( jetsCalo_, Calojets );  

  // PAT object collection
  edm::Handle< reco::PFJetCollection > matchedPFjets;
  iEvent.getByLabel( jetsMatchedPF_, matchedPFjets );
  // PAT object collection
  edm::Handle< reco::CaloJetCollection > matchedCalojets;
  iEvent.getByLabel( jetsMatchedCalo_, matchedCalojets );
  //trigger object 
  edm::Handle<edm::TriggerResults>  hltresults;
  iEvent.getByLabel( triggerTag_, hltresults);
  
  const edm::TriggerNames TrigNames = iEvent.triggerNames(*hltresults);
  const int ntrigs = hltresults->size();
  //std::cout<<"number of triggers: "<< ntrigs <<std::endl;

  
  bool triggered  = false;
  //bool triggered_penta = false;
  //bool triggered_exa   = false;
  //bool emulate_hlt     = false;
  
    //for(std::vector<std::string>::const_iterator iname = triggerPath_.begin(); iname!=triggerPath_.end(); iname++)
  // {

   for ( int itr = 0; itr < ntrigs; ++itr ) {
    if ( hltresults->accept( itr ) && 
	 TrigNames.triggerName(itr)== triggerName_) triggered = true;
   }

/*|| 
	  (TrigNames.triggerName(itr)== "HLT_QuadJet40_v2") ||
	  (TrigNames.triggerName(itr)== "HLT_QuadJet40_v3") ||
	  (TrigNames.triggerName(itr)== "HLT_QuadJet40_v4") ||
	  (TrigNames.triggerName(itr)== "HLT_QuadJet40_v5"))
	 ){
      //std::cout<<"trigger accepted: "<< TrigNames.triggerName(itr) << std::endl;
      triggered_quad=true;
    }
    if ( hltresults->accept( itr ) && 
	 ((TrigNames.triggerName(itr)== "HLT_QuadJet50_Jet40_v1") || 
	  (TrigNames.triggerName(itr)== "HLT_QuadJet50_Jet40_v2") ||
	  (TrigNames.triggerName(itr)== "HLT_QuadJet50_Jet40_v3"))
	 ){
      //std::cout<<"trigger accepted: "<< TrigNames.triggerName(itr) << std::endl;
      if(!emulateHLT_) triggered_penta=true; 
    }
    if ( hltresults->accept( itr ) &&
         ((TrigNames.triggerName(itr)== "HLT_QuadJet50_Jet40_Jet30_v1") ||
          (TrigNames.triggerName(itr)== "HLT_QuadJet50_Jet40_Jet30_v2") ||
          (TrigNames.triggerName(itr)== "HLT_QuadJet50_Jet40_Jet30_v3"))
	 ){
      //std::cout<<"trigger accepted: "<< TrigNames.triggerName(itr) << std::endl;
      if(!emulateHLT_) triggered_exa=true;
    }

  */
 
  /*  Float_t uncer1;
  Float_t uncer2;
  Float_t uncer3;
  Float_t uncer4;
  Float_t uncer5;
  Float_t uncer6;
  if(emulateHLT_ && JESshift_ && triggered_quad) {
    string JEC_PATH("CondFormats/JetMETObjects/data/");
      edm::FileInPath fip(JEC_PATH+"Jec10V1_Uncertainty_AK5Calo.txt");
      JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(fip.fullPath());
      
      
      // The following function gives the relative uncertainty in the jet Pt.
      // i.e. ptCorSmeared = (1 +- uncer) * ptCor  
      
      
      // set clone object
      
      if( Calojets->size()>=6 &&
	  Calojets->at(0).eta()<=3.0 &&
	  Calojets->at(1).eta()<=3.0 && 
	  Calojets->at(2).eta()<=3.0 && 
	  Calojets->at(3).eta()<=3.0 &&
	  Calojets->at(4).eta()<=3.0 &&
	  Calojets->at(5).eta()<=3.0 
	  )  
	{
	  
	  jecUnc->setJetEta(Calojets->at(0).eta()); // Give rapidity of jet you want uncertainty on
	  jecUnc->setJetPt(Calojets->at(0).pt());// Also give the corrected pt of the jet you want the uncertainty on
	  uncer1 = jecUnc->getUncertainty(true); // In principle, boolean controls if uncertainty on +ve or -ve side is returned (asymmetric errors) but not yet implemented.
	  
	  jecUnc->setJetEta(Calojets->at(1).eta()); // Give rapidity of jet you want uncertainty on
	  jecUnc->setJetPt(Calojets->at(1).pt());// Also give the corrected pt of the jet you want the uncertainty on
	  uncer2 = jecUnc->getUncertainty(true); // In principle, boolean controls if uncertainty on +ve or -ve side is returned (asymmetric errors) but not yet implemented.
	
	  jecUnc->setJetEta(Calojets->at(2).eta()); // Give rapidity of jet you want uncertainty on
	  jecUnc->setJetPt(Calojets->at(2).pt());// Also give the corrected pt of the jet you want the uncertainty on
	  uncer3 = jecUnc->getUncertainty(true); // In principle, boolean controls if uncertainty on +ve or -ve side is returned (asymmetric errors) but not yet implemented.
	  
	  jecUnc->setJetEta(Calojets->at(3).eta()); // Give rapidity of jet you want uncertainty on
	  jecUnc->setJetPt(Calojets->at(3).pt());// Also give the corrected pt of the jet you want the uncertainty on
	  uncer4 = jecUnc->getUncertainty(true); // In principle, boolean controls if uncertainty on +ve or -ve side is returned (asymmetric errors) but not yet implemented.
	  
	  jecUnc->setJetEta(Calojets->at(4).eta()); // Give rapidity of jet you want uncertainty on
	  jecUnc->setJetPt(Calojets->at(4).pt());// Also give the corrected pt of the jet you want the uncertainty on
	  uncer5 = jecUnc->getUncertainty(true); // In principle, boolean controls if uncertainty on +ve or -ve side is returned (asymmetric errors) but not yet implemented.
	  
	  jecUnc->setJetEta(Calojets->at(5).eta()); // Give rapidity of jet you want uncertainty on
	  jecUnc->setJetPt(Calojets->at(5).pt());// Also give the corrected pt of the jet you want the uncertainty on
	  uncer6 = jecUnc->getUncertainty(true); // In principle, boolean controls if uncertainty on +ve or -ve side is returned (asymmetric errors) but not yet implemented.
	  
	  delete jecUnc;
	  //	jecUnc2->setJetEta(jet->eta()); // Give rapidity of jet you want uncertainty on
	  //jecUnc2->setJetPt(jet->pt());// Also give the corrected pt of the jet you want the uncertainty on
	  
	  
	  if(Calojets->at(0).eta()<=3.0 && Calojets->at(0).pt()>=50.*(1+uncer1) &&
	     Calojets->at(1).eta()<=3.0 && Calojets->at(1).pt()>=50.*(1+uncer2) &&
	     Calojets->at(2).eta()<=3.0 && Calojets->at(2).pt()>=50.*(1+uncer3) &&
	     Calojets->at(3).eta()<=3.0 && Calojets->at(3).pt()>=50.*(1+uncer4) &&
	     Calojets->at(4).eta()<=3.0 && Calojets->at(4).pt()>=40.*(1+uncer5) 
	     )
	    {    
	      triggered_penta=true;
	      //cout<<"emulation ok"<<endl;
	    }
	  if(Calojets->at(0).eta()<=3.0 && Calojets->at(0).pt()>=50.*(1+uncer1) &&
	     Calojets->at(1).eta()<=3.0 && Calojets->at(1).pt()>=50.*(1+uncer2) &&
	     Calojets->at(2).eta()<=3.0 && Calojets->at(2).pt()>=50.*(1+uncer3) &&
	     Calojets->at(3).eta()<=3.0 && Calojets->at(3).pt()>=50.*(1+uncer4) &&
	     Calojets->at(4).eta()<=3.0 && Calojets->at(4).pt()>=40.*(1+uncer5) && 
	     Calojets->at(5).eta()<=3.0 && Calojets->at(5).pt()>=40.*(1+uncer6)
	     )
	    {    
	      triggered_exa=true;
	      //cout<<"emulation ok"<<endl;
	    }
	  

	}
    }

    else if(emulateHLT_ && !JESshift_ && triggered_quad)
      {
	if( Calojets->size()>=6 &&
	    Calojets->at(0).eta()<=3.0 &&
	    Calojets->at(1).eta()<=3.0 &&
	    Calojets->at(2).eta()<=3.0 &&
	    Calojets->at(3).eta()<=3.0 &&
	    Calojets->at(4).eta()<=3.0 &&
	    Calojets->at(5).eta()<=3.0
	    )
	  {
	    if(Calojets->at(0).eta()<=3.0 && Calojets->at(0).pt()>=50.*(1+uncer1) &&
	       Calojets->at(1).eta()<=3.0 && Calojets->at(1).pt()>=50.*(1+uncer2) &&
	       Calojets->at(2).eta()<=3.0 && Calojets->at(2).pt()>=50.*(1+uncer3) &&
	       Calojets->at(3).eta()<=3.0 && Calojets->at(3).pt()>=50.*(1+uncer4) &&
	       Calojets->at(4).eta()<=3.0 && Calojets->at(4).pt()>=40.*(1+uncer5) &&
	       Calojets->at(5).eta()<=3.0 && Calojets->at(5).pt()>=40.*(1+uncer6)
	       )
	      {
		triggered_exa=true;
		//cout<<"emulation ok"<<endl;                                                                                                                  
	      }
	  }
      }
  }
  */
  
  if(triggered)
    {
      // loop over jet references (PAT jets have been used in the matcher in task 3)
       double offline_sel=0.;
       //double online_sel=0.;
       
       //if(PFjets->size()>=6){
       for(unsigned int i=0; i<PFjets->size(); i++){
	 //if(fabs(PFjets->at(0).eta())<=2.4){
	 histos1D_[ "PFjetPt" ]->Fill( PFjets->at(i).pt() );
	 if(triggered) histos1D_[ "matchedPt" ]->Fill( PFjets->at(i).pt() );
	}
    
       
       /*if(fabs(PFjets->at(1).eta())<=2.4){
	 histos1D_[ "PFjetPt2" ]->Fill( PFjets->at(1).pt() );
	 if(triggered_exa) histos1D_[ "matchedPt2" ]->Fill( PFjets->at(1).pt() );
	 }
	 if(fabs(PFjets->at(2).eta())<=2.4){
	 histos1D_[ "PFjetPt3" ]->Fill( PFjets->at(2).pt() );
	 if(triggered_exa) histos1D_[ "matchedPt3" ]->Fill( PFjets->at(2).pt() );
	 }
	 if(fabs(PFjets->at(3).eta())<=2.4){
	 histos1D_[ "PFjetPt4" ]->Fill( PFjets->at(3).pt() );
	 if(triggered_exa) histos1D_[ "matchedPt4" ]->Fill( PFjets->at(3).pt() );
	 }
	 if(fabs(PFjets->at(4).eta())<=2.4){
	 histos1D_[ "PFjetPt5" ]->Fill( PFjets->at(4).pt() );
	 if(triggered_exa) histos1D_[ "matchedPt5" ]->Fill( PFjets->at(4).pt() );
	 }
	 if(fabs(PFjets->at(5).eta())<=2.4){
	 histos1D_[ "PFjetPt6" ]->Fill( PFjets->at(5).pt() );
	 if(triggered_exa) histos1D_[ "matchedPt6" ]->Fill( PFjets->at(5).pt() );
	 }*/ 
    }
         
  /*if(PFjets->size()>=6 &&
	 PFjets->at(0).eta()<=2.4 && PFjets->at(0).pt()>=60. &&
	 PFjets->at(1).eta()<=2.4 && PFjets->at(1).pt()>=60. &&
	 PFjets->at(2).eta()<=2.4 && PFjets->at(2).pt()>=60. &&
	 PFjets->at(3).eta()<=2.4 && PFjets->at(3).pt()>=60. &&
	 PFjets->at(4).eta()<=2.4 && PFjets->at(4).pt()>=50. &&
	 PFjets->at(5).eta()<=2.4 && PFjets->at(5).pt()>=40.)
	{
	  offline_sel=1.;
	  histos1D_[ "offline_sel"]->Fill( 1. );
	  }*/
      
  //if(triggered_exa && offline_sel) histos1D_[ "online_sel"]->Fill( 1. );
      //if(triggered_penta && offline_sel) histos1D_[ "online_sel"]->Fill( 1. ); 
  //}
   }

void PFjetAnalyzer::endJob()
{
  
  //histos1D_[ "turnOn" ]->Sumw2();
  //histos1D_[ "turnOn" ]->Divide( histos1D_[ "hltPt" ] , histos1D_[ "recoPt" ], 1., 1., "B");
  //histos1D_[ "turnOn" ]->Divide( histos1D_[ "hltDiscr" ] , histos1D_[ "recoDiscr" ], 1., 1., "B");
  
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE( PFjetAnalyzer );
