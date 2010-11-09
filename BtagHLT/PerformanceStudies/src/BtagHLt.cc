// -*- C++ -*-
//
// Package:    BtagHLt
// Class:      BtagHLt
// 
/**\class BtagHLt BtagHLt.cc test/BtagHLt/src/BtagHLt.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Antonio Tropiano,40 3-B32,+41227671551,
//         Created:  Tue Sep  7 12:17:49 CEST 2010
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include <vector>
#include <string>
#include <algorithm>
#include <TTree.h>
#include "TFile.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Math/interface/deltaR.h"
// class declaration
//

class BtagHLt : public edm::EDAnalyzer {
public:
  explicit BtagHLt(const edm::ParameterSet&);
  ~BtagHLt();
  

private:
  virtual void beginJob() ;
  virtual void beginRun(const edm::Run & run, const edm::EventSetup & setup);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  std::vector<std::pair<reco::TrackIPTagInfo,reco::TrackIPTagInfo> > associateJets(std::vector<reco::TrackIPTagInfo> reco, std::vector<reco::TrackIPTagInfo> hlt, double deltaRcut);
  

      // ----------member data ---------------------------
  
  std::string m_outputFile;
  edm::InputTag m_L1Filter;
  edm::InputTag m_L2Filter;
  edm::InputTag m_L25Filter;
  edm::InputTag m_L3Filter;
  edm::InputTag m_L2Jets;
  edm::InputTag m_L25TagInfo;
  edm::InputTag m_L25JetTags;
  edm::InputTag m_L3TagInfo;
  edm::InputTag m_L3JetTags;
  edm::InputTag m_recoTagInfo;
  edm::InputTag m_recoJetTags;
  edm::InputTag m_triggerResults;
  std::string m_processName;
  std::string m_pathName;
  std::string m_monitorName;
  bool m_storeROOT;
  unsigned int m_size;
  //edm::Service<DQMStore> m_dbe;
  bool m_init;
  unsigned int m_pathIndex;
  unsigned int m_L1FilterIndex;
  unsigned int m_L2FilterIndex;
  unsigned int m_L25FilterIndex;
  unsigned int m_L3FilterIndex;
  //tree variables
  float L25Reco_reco_discriminator;
  float L25Reco_l25_discriminator;
  float L25Reco_reco_trackmult;
  float L25Reco_l25_trackmult;
  float L25Reco_reco_ip2ndtrack;
  float L25Reco_l25_ip2ndtrack;
  float L25Reco_reco_ip3rdtrack;
  float L25Reco_l25_ip3rdtrack;
  float L25Reco_reco_sigmaip2ndtrack;
  float L25Reco_l25_sigmaip2ndtrack;
  float L25Reco_reco_sigmaip3rdtrack;
  float L25Reco_l25_sigmaip3rdtrack;
  float L25Reco_reco_ip2ndtrack3D;
  float L25Reco_l25_ip2ndtrack3D;
  float L25Reco_reco_ip3rdtrack3D;
  float L25Reco_l25_ip3rdtrack3D;
  float L25Reco_reco_sigmaip2ndtrack3D;
  float L25Reco_l25_sigmaip2ndtrack3D;
  float L25Reco_reco_sigmaip3rdtrack3D;
  float L25Reco_l25_sigmaip3rdtrack3D;
  float L25Reco_reco_deltar;
  float L25Reco_l25_deltar;
  float L25Reco_reco_etajet;
  float L25Reco_l25_etajet;
  float L25Reco_reco_Etjet;
  float L25Reco_l25_Etjet;
  float L25Reco_reco_phijet;
  float L25Reco_l25_phijet;
  float L3Reco_reco_discriminator;
  float L3Reco_l3_discriminator;
  float L3Reco_reco_trackmult;
  float L3Reco_l3_trackmult;
  float L3Reco_reco_ip2ndtrack;
  float L3Reco_l3_ip2ndtrack;
  float L3Reco_reco_ip3rdtrack;
  float L3Reco_l3_ip3rdtrack;
  float L3Reco_reco_sigmaip2ndtrack;
  float L3Reco_l3_sigmaip2ndtrack;
  float L3Reco_reco_sigmaip3rdtrack;
  float L3Reco_l3_sigmaip3rdtrack;
  float L3Reco_reco_ip2ndtrack3D;
  float L3Reco_l3_ip2ndtrack3D;
  float L3Reco_reco_ip3rdtrack3D;
  float L3Reco_l3_ip3rdtrack3D;
  float L3Reco_reco_sigmaip2ndtrack3D;
  float L3Reco_l3_sigmaip2ndtrack3D;
  float L3Reco_reco_sigmaip3rdtrack3D;
  float L3Reco_l3_sigmaip3rdtrack3D;
  float L3Reco_reco_deltar;
  float L3Reco_l3_deltar;
  float L3Reco_reco_etajet;
  float L3Reco_l3_etajet;
  float L3Reco_reco_Etjet;
  float L3Reco_l3_Etjet;
  float L3Reco_reco_phijet;
  float L3Reco_l3_phijet;
  
  float L3Reco_l3_dxy;
  float L3Reco_l3_sigmadxy;
  float L3Reco_l3_dz;
  float L3Reco_l3_sigmadz;
  float L3Reco_l3_eta;
  float L3Reco_l3_phi;
  
  float L3Reco_reco_dxy;
  float L3Reco_reco_sigmadxy;
  float L3Reco_reco_dz;
  float L3Reco_reco_sigmadz;
  float L3Reco_reco_eta;
  float L3Reco_reco_phi;
  
  float L25Reco_l25_dxy;
  float L25Reco_l25_sigmadxy;
  float L25Reco_l25_dz;
  float L25Reco_l25_sigmadz;
  float L25Reco_l25_eta;
  float L25Reco_l25_phi;
  
  float L25Reco_reco_dxy;
  float L25Reco_reco_sigmadxy;
  float L25Reco_reco_dz;
  float L25Reco_reco_sigmadz;
  float L25Reco_reco_eta;
  float L25Reco_reco_phi;
  
  float l25_reco_assoc;
  float l3_reco_assoc;
  UInt_t eventNum;
  float L1pass;
  float L2pass;
  float L25pass;
  float L3pass;
  //file and tree 
  TFile* output;
  TTree* BtagTree;
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
BtagHLt::BtagHLt(const edm::ParameterSet& iConfig):
  m_outputFile(     iConfig.getUntrackedParameter<std::string>("outputFile")),
  m_L1Filter(       iConfig.getParameter<edm::InputTag>("L1Filter") ),
  m_L2Filter(       iConfig.getParameter<edm::InputTag>("L2Filter") ),
  m_L25Filter(      iConfig.getParameter<edm::InputTag>("L25Filter") ),
  m_L3Filter(       iConfig.getParameter<edm::InputTag>("L3Filter") ),
  m_L2Jets(         iConfig.getParameter<edm::InputTag>("L2Jets") ),
  m_L25TagInfo(     iConfig.getParameter<edm::InputTag>("L25TagInfo") ),
  m_L25JetTags(     iConfig.getParameter<edm::InputTag>("L25JetTags") ),
  m_L3TagInfo(      iConfig.getParameter<edm::InputTag>("L3TagInfo") ),
  m_L3JetTags(      iConfig.getParameter<edm::InputTag>("L3JetTags") ),
  m_recoTagInfo(    iConfig.getParameter<edm::InputTag>("recoTagInfo") ),
  m_recoJetTags(    iConfig.getParameter<edm::InputTag>("recoJetTags") ),
  m_triggerResults( iConfig.getParameter<edm::InputTag>("triggerResults") ),
  m_processName(    iConfig.getParameter<std::string>("processName") ),
  m_pathName(       iConfig.getParameter<std::string>("pathName") ),
  m_monitorName(    iConfig.getParameter<std::string>("monitorName" ) ),
  m_storeROOT(      iConfig.getUntrackedParameter<bool>("storeROOT", false) ),
  m_size(           iConfig.getParameter<unsigned int>("interestingJets") ),
		//m_dbe(),
  m_init(           false ),
  m_pathIndex(      (unsigned int) -1 ),
  m_L1FilterIndex(  (unsigned int) -1 ),
  m_L2FilterIndex(  (unsigned int) -1 ),
  m_L25FilterIndex( (unsigned int) -1 ),
  m_L3FilterIndex(  (unsigned int) -1 )  
{
   //now do what ever initialization is needed
  L1pass=0;
  L2pass=0;
  L25pass=0;
  L3pass=0;
}


BtagHLt::~BtagHLt()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
BtagHLt::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  
  eventNum= iEvent.id().event();
  //cout << "event number" << eventNum << endl;
  /*if (not m_dbe.isAvailable())
    return;*/
  
  if (not m_init)
    return;

  edm::Handle<edm::TriggerResults>            h_triggerResults;
  edm::Handle<edm::View<reco::Jet> >          h_L2Jets;
  edm::Handle<reco::TrackIPTagInfoCollection> h_L25TagInfo;
  edm::Handle<reco::JetTagCollection>         h_L25JetTags;
  edm::Handle<reco::TrackIPTagInfoCollection> h_L3TagInfo;
  edm::Handle<reco::JetTagCollection>         h_L3JetTags;
  edm::Handle<reco::TrackIPTagInfoCollection> h_recoTagInfo;
  edm::Handle<reco::JetTagCollection>         h_recoJetTags;
  
  iEvent.getByLabel(m_triggerResults, h_triggerResults);
  iEvent.getByLabel(m_L2Jets,         h_L2Jets);
  iEvent.getByLabel(m_L25TagInfo,     h_L25TagInfo);
  iEvent.getByLabel(m_L25JetTags,     h_L25JetTags);
  iEvent.getByLabel(m_L3TagInfo,      h_L3TagInfo);
  iEvent.getByLabel(m_L3JetTags,      h_L3JetTags);
  iEvent.getByLabel(m_recoTagInfo,    h_recoTagInfo);
  iEvent.getByLabel(m_recoJetTags,    h_recoJetTags);
  
  std::vector<reco::TrackIPTagInfo> recoTagInfo;
  //std::cout<<"recoTagInfo size: "<<h_recoTagInfo->size()<<std::endl;
  for (unsigned int i = 0; i < h_recoTagInfo->size(); ++i)
    {
      const reco::TrackIPTagInfo & reco_info   = (*h_recoTagInfo)[i];
      //const reco::Jet & reco_jet = * reco_info.jet();
      //const reco::TrackRefVector & reco_tracks = reco_info.selectedTracks();
      //const std::vector<reco::TrackIPTagInfo::TrackIPData> & reco_data = reco_info.impactParameterData();
      //const reco::JetTag & reco_tag = (*h_recoJetTags)[reco_info.jet().key()];
      recoTagInfo.push_back(reco_info);
    }
  // check if this path passed the L1, L2, L2.5 and L3 filters
  bool         wasrun = false;
  unsigned int latest = 0;
  bool         accept = false;
  if (h_triggerResults.isValid()) {
    wasrun = h_triggerResults->wasrun( m_pathIndex );
    latest = h_triggerResults->index(  m_pathIndex );
    accept = h_triggerResults->accept( m_pathIndex );
  }
  
  if (latest > m_L1FilterIndex) 
    {
      L1pass=1; 
    }
  else L1pass=0;
  cout<<"L1 = "<< L1pass << endl;
  
  if (latest > m_L2FilterIndex) 
    {
      L2pass=1; 
      //    cout <<"L2 passed"<<endl;
    }
  else L2pass=0;
  cout<<"L2 = "<< L2pass << endl;
  
  if (latest > m_L25FilterIndex) 
    {
      L25pass=1; 
      //cout <<"L25 passed "<<endl;
    }
  else L25pass=0;
  cout<<"L25 = "<< L25pass << endl;
  
  if (latest > m_L3FilterIndex) 
    {
      L3pass=1; 
      //cout <<"L3 passed"<<endl;
    }
  else L3pass=0;
  cout<<"L3 = "<< L3pass << endl;
  
  
  if (h_L25TagInfo.isValid() and h_L25JetTags.isValid() and h_L3TagInfo.isValid() and h_L3JetTags.isValid()) {
    
    std::vector<reco::TrackIPTagInfo> l25TagInfo;
    unsigned int size = std::min((unsigned int) h_L25TagInfo->size(), m_size);
    for (unsigned int i = 0; i < size; ++i) {
      const reco::TrackIPTagInfo & info   = (*h_L25TagInfo)[i];
      l25TagInfo.push_back(info);
    }
    
    using namespace std;    
    std::vector<std::pair<reco::TrackIPTagInfo, reco::TrackIPTagInfo> > recol25_association =  associateJets(recoTagInfo, l25TagInfo, 0.5);
    //cout << "Number of reco-l25 jets associated: " << recol25_association.size() << endl;
    if(recol25_association.size()) l25_reco_assoc=1.;
    else l25_reco_assoc=0.;
    if(recol25_association.size()){
    for( std::vector<std::pair<reco::TrackIPTagInfo, reco::TrackIPTagInfo> >::iterator iass = recol25_association.begin(); 
	 iass != recol25_association.end(); ++iass)
      {
	//cout << "associating objects" << endl;
	//fill tree
	const reco::TrackRefVector & reco_tracks = (iass->first).selectedTracks();
	const reco::TrackRefVector & hlt_tracks  = (iass->second).selectedTracks();
	if((iass->first).selectedTracks().size() && (iass->second).selectedTracks().size()){
	//for (unsigned int i = 0; i < reco_tracks.size(); ++i) {
	L25Reco_reco_dxy=          (iass->first).selectedTracks()[0]->dxy();
	L25Reco_reco_sigmadxy=     (iass->first).selectedTracks()[0]->dxyError();
	L25Reco_reco_dz=           (iass->first).selectedTracks()[0]->dz();
	L25Reco_reco_sigmadz=      (iass->first).selectedTracks()[0]->dzError();
	L25Reco_reco_eta=          (iass->first).selectedTracks()[0]->eta();
	L25Reco_reco_phi=          (iass->first).selectedTracks()[0]->phi();
	
	//}
	//for (unsigned int i = 0; i < hlt_tracks.size(); ++i) {
	L25Reco_l25_dxy=           (iass->second).selectedTracks()[0]->dxy();
	L25Reco_l25_sigmadxy=      (iass->second).selectedTracks()[0]->dxyError();
	L25Reco_l25_dz=            (iass->second).selectedTracks()[0]->dz();
	L25Reco_l25_sigmadz=       (iass->second).selectedTracks()[0]->dzError();
	L25Reco_l25_eta=           (iass->second).selectedTracks()[0]->eta();
	L25Reco_l25_phi=           (iass->second).selectedTracks()[0]->phi();
	}
	//cout<<"assciated tracks"<<endl;
	L25Reco_reco_discriminator               = (*h_recoJetTags)[iass->first.jet().key()].second;
	L25Reco_l25_discriminator                = (*h_L25JetTags)[iass->second.jet().key()].second;
	L25Reco_reco_trackmult                   = reco_tracks.size();
	L25Reco_l25_trackmult                    = hlt_tracks.size();  
	//keep the ip of the track 
	if((iass->first).impactParameterData().size()>1){
	  //2d IP
	  L25Reco_reco_ip2ndtrack        = (iass->first).impactParameterData()[1].ip2d.value();         
	  L25Reco_reco_ip3rdtrack        = (iass->first).impactParameterData()[2].ip2d.value();          
	  L25Reco_reco_sigmaip2ndtrack   = (iass->first).impactParameterData()[1].ip2d.significance();
	  L25Reco_reco_sigmaip3rdtrack   = (iass->first).impactParameterData()[2].ip2d.significance();
	  //3d IP
	  L25Reco_reco_ip2ndtrack3D      = (iass->first).impactParameterData()[1].ip3d.value();         
	  L25Reco_reco_ip3rdtrack3D      = (iass->first).impactParameterData()[2].ip3d.value();          
	  L25Reco_reco_sigmaip2ndtrack3D = (iass->first).impactParameterData()[1].ip3d.significance();
	  L25Reco_reco_sigmaip3rdtrack3D = (iass->first).impactParameterData()[2].ip3d.significance();
	}
	if((iass->second).impactParameterData().size()>1){
	  //2D IP
	  L25Reco_l25_ip2ndtrack         = (iass->second).impactParameterData()[1].ip2d.value();         
	  L25Reco_l25_ip3rdtrack         = (iass->second).impactParameterData()[2].ip2d.value();          
	  L25Reco_l25_sigmaip2ndtrack    = (iass->second).impactParameterData()[1].ip2d.significance();
	  L25Reco_l25_sigmaip3rdtrack    = (iass->second).impactParameterData()[2].ip2d.significance();
	  //3D IP
	  L25Reco_l25_ip2ndtrack3D       = (iass->second).impactParameterData()[1].ip3d.value();         
	  L25Reco_l25_ip3rdtrack3D       = (iass->second).impactParameterData()[2].ip3d.value();          
	  L25Reco_l25_sigmaip2ndtrack3D  = (iass->second).impactParameterData()[1].ip3d.significance();
	  L25Reco_l25_sigmaip3rdtrack3D  = (iass->second).impactParameterData()[2].ip3d.significance();
	}
	
	//distance of the first track
	if(reco_tracks.size())
	  L25Reco_reco_deltar=reco::deltaR(reco_tracks[0]->eta(),reco_tracks[0]->phi(),iass->first.jet()->eta(),iass->first.jet()->phi());
	if(hlt_tracks.size())
	  L25Reco_l25_deltar=reco::deltaR(hlt_tracks[0]->eta(),hlt_tracks[0]->phi(),iass->second.jet()->eta(),iass->second.jet()->phi());
	L25Reco_reco_etajet  =iass->first.jet()->eta();
	L25Reco_l25_etajet   =iass->second.jet()->eta();      
	L25Reco_reco_Etjet   =iass->first.jet()->et();      
	L25Reco_l25_Etjet    =iass->second.jet()->et();       
	L25Reco_reco_phijet  =iass->first.jet()->phi();
	L25Reco_l25_phijet   =iass->second.jet()->phi();      
      }
    }
   
    size = std::min((unsigned int) h_L3TagInfo->size(), m_size);
    std::vector<reco::TrackIPTagInfo> l3TagInfo;
    for (unsigned int i = 0; i < size; ++i) {
      const reco::TrackIPTagInfo & info   = (*h_L3TagInfo)[i];
      l3TagInfo.push_back(info);
      using namespace std;
      std::vector<std::pair<reco::TrackIPTagInfo, reco::TrackIPTagInfo> > recol3_association =  associateJets(recoTagInfo, l3TagInfo, 0.5);if(recol3_association.size()) l3_reco_assoc=1.;
    else l3_reco_assoc=0.;

      //cout << "Number of reco-l25 jets associated: " << recol3_association.size() << endl;

    for( std::vector<std::pair<reco::TrackIPTagInfo, reco::TrackIPTagInfo> >::iterator iiass = recol3_association.begin(); 
	 iiass != recol3_association.end(); ++iiass)
      {
	//fill tree
	const reco::TrackRefVector & reco_tracks = (iiass->first).selectedTracks();
	const reco::TrackRefVector & l3_tracks   = (iiass->second).selectedTracks();
	if((iiass->first).selectedTracks().size() && (iiass->second).selectedTracks().size()){
	//for (unsigned int i = 0; i < reco_tracks.size(); ++i) {
	L3Reco_reco_dxy=(iiass->first).selectedTracks()[0]->dxy();
	L3Reco_reco_sigmadxy=(iiass->first).selectedTracks()[0]->dxyError();
	L3Reco_reco_dz=(iiass->first).selectedTracks()[0]->dz();
	L3Reco_reco_sigmadz=(iiass->first).selectedTracks()[0]->dzError();
	L3Reco_reco_eta=(iiass->first).selectedTracks()[0]->eta();
	L3Reco_reco_phi=(iiass->first).selectedTracks()[0]->phi();
	//}
	//for (unsigned int i = 0; i < l3_tracks.size(); ++i) {
	L3Reco_l3_dxy=(iiass->second).selectedTracks()[0]->dxy();
	L3Reco_l3_sigmadxy=(iiass->second).selectedTracks()[0]->dxyError();
	L3Reco_l3_dz=(iiass->second).selectedTracks()[0]->dz();
	L3Reco_l3_sigmadz=(iiass->second).selectedTracks()[0]->dzError();
	L3Reco_l3_eta=(iiass->second).selectedTracks()[0]->eta();
	L3Reco_l3_phi=(iiass->second).selectedTracks()[0]->phi();
	}
	L3Reco_reco_discriminator                = (*h_recoJetTags)[(iiass->first).jet().key()].second;
	L3Reco_l3_discriminator                  = (*h_L3JetTags)[(iiass->second).jet().key()].second;
	L3Reco_reco_trackmult                    = reco_tracks.size();
	L3Reco_l3_trackmult                      = l3_tracks.size();  
	//keep the ip of the track
	if((iiass->first).impactParameterData().size()>2){
	  //2D
	  L3Reco_reco_ip2ndtrack        = (iiass->first).impactParameterData()[1].ip2d.value();         
	  L3Reco_reco_ip3rdtrack        = (iiass->first).impactParameterData()[2].ip2d.value();          
	  L3Reco_reco_sigmaip2ndtrack   = (iiass->first).impactParameterData()[1].ip2d.significance();
	  L3Reco_reco_sigmaip3rdtrack   = (iiass->first).impactParameterData()[2].ip2d.significance();
	  //3D
	  L3Reco_reco_ip2ndtrack3D        = (iiass->first).impactParameterData()[1].ip3d.value();         
	  L3Reco_reco_ip3rdtrack3D        = (iiass->first).impactParameterData()[2].ip3d.value();          
	  L3Reco_reco_sigmaip2ndtrack3D   = (iiass->first).impactParameterData()[1].ip3d.significance();
	  L3Reco_reco_sigmaip3rdtrack3D   = (iiass->first).impactParameterData()[2].ip3d.significance();
	  
	}
	if((iiass->second).impactParameterData().size()>2){
	  //2D
	  L3Reco_l3_ip2ndtrack       = (iiass->second).impactParameterData()[1].ip2d.value();         
	  L3Reco_l3_ip3rdtrack       = (iiass->second).impactParameterData()[2].ip2d.value();          
	  L3Reco_l3_sigmaip2ndtrack  = (iiass->second).impactParameterData()[1].ip2d.significance();
	  L3Reco_l3_sigmaip3rdtrack  = (iiass->second).impactParameterData()[2].ip2d.significance();
	  //3D
	  L3Reco_l3_ip2ndtrack3D       = (iiass->second).impactParameterData()[1].ip3d.value();         
	  L3Reco_l3_ip3rdtrack3D       = (iiass->second).impactParameterData()[2].ip3d.value();          
	  L3Reco_l3_sigmaip2ndtrack3D  = (iiass->second).impactParameterData()[1].ip3d.significance();
	  L3Reco_l3_sigmaip3rdtrack3D  = (iiass->second).impactParameterData()[2].ip3d.significance();
	  
	}
	
	//distance of the first track
	if(reco_tracks.size())
	  L3Reco_reco_deltar=reco::deltaR(reco_tracks[0]->eta(),reco_tracks[0]->phi(),iiass->first.jet()->eta(),iiass->first.jet()->phi());
	if(l3_tracks.size())
	  L3Reco_l3_deltar=reco::deltaR(l3_tracks[0]->eta(),l3_tracks[0]->phi(),iiass->second.jet()->eta(),iiass->second.jet()->phi());
	
	L3Reco_reco_etajet   =iiass->first.jet()->eta();
	L3Reco_l3_etajet     =iiass->second.jet()->eta();      
	L3Reco_reco_Etjet    =iiass->first.jet()->et();      
	L3Reco_l3_Etjet      =iiass->second.jet()->et();       
	L3Reco_reco_phijet   =iiass->first.jet()->phi();
	L3Reco_l3_phijet     =iiass->second.jet()->phi();      
      }  
    BtagTree->Fill();
    }
   }  
   
   
  
#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif
  
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif
  
}

// ------------ method called once each job just before starting event loop  ------------
void 
BtagHLt::beginJob()
{
  output = new TFile(m_outputFile.c_str(),"RECREATE");
  
  int bufsize=64000;
  BtagTree = new TTree("btagInfo","btagInfo");
  
  BtagTree->Branch("L25Reco_reco_discriminator" , &L25Reco_reco_discriminator , "L25Reco_reco_discriminator/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_discriminator" , &L25Reco_l25_discriminator , "L25Reco_l25_discriminator/F" , bufsize);
  BtagTree->Branch("L25Reco_reco_trackmult" , &L25Reco_reco_trackmult , "L25Reco_reco_trackmult/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_trackmult" , &L25Reco_l25_trackmult , "L25Reco_l25_trackmult/F" , bufsize);  

  BtagTree->Branch("L25Reco_reco_ip2ndtrack" , &L25Reco_reco_ip2ndtrack , "L25Reco_reco_ip2ndtrack/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_ip2ndtrack" , &L25Reco_l25_ip2ndtrack , "L25Reco_l25_ip2ndtrack/F" , bufsize);
  BtagTree->Branch("L25Reco_reco_ip3rdtrack" , &L25Reco_reco_ip3rdtrack , "L25Reco_reco_ip3rdtrack/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_ip3rdtrack" , &L25Reco_l25_ip3rdtrack , "L25Reco_l25_ip3rdtrack/F" , bufsize);
  
  BtagTree->Branch("L25Reco_reco_sigmaip2ndtrack" , &L25Reco_reco_sigmaip2ndtrack , "L25Reco_reco_sigmaip2ndtrack/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_sigmaip2ndtrack" , &L25Reco_l25_sigmaip2ndtrack , "L25Reco_l25_sigmaip2ndtrack/F" , bufsize);
  BtagTree->Branch("L25Reco_reco_sigmaip3rdtrack" , &L25Reco_reco_sigmaip3rdtrack , "L25Reco_reco_sigmaip3rdtrack/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_sigmaip3rdtrack" , &L25Reco_l25_sigmaip3rdtrack , "L25Reco_l25_sigmaip3rdtrack/F" , bufsize);
  
  BtagTree->Branch("L25Reco_reco_ip2ndtrack3D" , &L25Reco_reco_ip2ndtrack3D , "L25Reco_reco_ip2ndtrack3D/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_ip2ndtrack3D" , &L25Reco_l25_ip2ndtrack3D , "L25Reco_l25_ip2ndtrack3D/F" , bufsize);
  BtagTree->Branch("L25Reco_reco_ip3rdtrack3D" , &L25Reco_reco_ip3rdtrack3D , "L25Reco_reco_ip3rdtrack3D/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_ip3rdtrack3D" , &L25Reco_l25_ip3rdtrack3D , "L25Reco_l25_ip3rdtrack3D/F" , bufsize);
  
  BtagTree->Branch("L25Reco_reco_sigmaip2ndtrack3D" , &L25Reco_reco_sigmaip2ndtrack3D , "L25Reco_reco_sigmaip2ndtrack3D/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_sigmaip2ndtrack3D" , &L25Reco_l25_sigmaip2ndtrack3D , "L25Reco_l25_sigmaip2ndtrack3D/F" , bufsize);
  BtagTree->Branch("L25Reco_reco_sigmaip3rdtrack3D" , &L25Reco_reco_sigmaip3rdtrack3D , "L25Reco_reco_sigmaip3rdtrack3D/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_sigmaip3rdtrack3D" , &L25Reco_l25_sigmaip3rdtrack3D , "L25Reco_l25_sigmaip3rdtrack3D/F" , bufsize);
  
  BtagTree->Branch("L25Reco_reco_deltar" , &L25Reco_reco_deltar , "L25Reco_reco_deltar/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_deltar" , &L25Reco_l25_deltar , "L25Reco_l25_deltar/F" , bufsize);
  BtagTree->Branch("L25Reco_reco_etajet" , &L25Reco_reco_etajet , "L25Reco_reco_etajet/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_etajet" , &L25Reco_l25_etajet , "L25Reco_l25_etajet/F" , bufsize);
  BtagTree->Branch("L25Reco_reco_Etjet" , &L25Reco_reco_Etjet , "L25Reco_reco_Etjet/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_Etjet" , &L25Reco_l25_Etjet , "L25Reco_l25_Etjet/F" , bufsize);
  BtagTree->Branch("L25Reco_reco_phijet" , &L25Reco_reco_phijet , "L25Reco_reco_phijet/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_phijet" , &L25Reco_l25_phijet , "L25Reco_l25_phijet/F" , bufsize);
  
  BtagTree->Branch("L3Reco_reco_discriminator" , &L3Reco_reco_discriminator , "L3Reco_reco_discriminator/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_discriminator" , &L3Reco_l3_discriminator , "L3Reco_l3_discriminator/F" , bufsize);
  BtagTree->Branch("L3Reco_reco_trackmult" , &L3Reco_reco_trackmult , "L3Reco_reco_trackmult/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_trackmult" , &L3Reco_l3_trackmult , "L3Reco_l3_trackmult/F" , bufsize);  
  //track parameters
  BtagTree->Branch("L3Reco_reco_dxy" , &L3Reco_reco_dxy , "L3Reco_reco_dxy/F" , bufsize);
  BtagTree->Branch("L3Reco_reco_sigmadxy" , &L3Reco_reco_sigmadxy , "L3Reco_reco_sigmadxy/F" , bufsize);
  BtagTree->Branch("L3Reco_reco_dz" ,       &L3Reco_reco_dz ,   "L3Reco_reco_dz/F" ,    bufsize);
  BtagTree->Branch("L3Reco_reco_sigmadz" ,  &L3Reco_reco_sigmadz ,   "L3Reco_reco_sigmadz/F" ,    bufsize);
  BtagTree->Branch("L3Reco_reco_eta" ,      &L3Reco_reco_eta , "L3Reco_reco_eta/F" , bufsize);
  BtagTree->Branch("L3Reco_reco_phi" ,      &L3Reco_reco_phi , "L3Reco_reco_phi/F" , bufsize);  
  BtagTree->Branch("L3Reco_l3_dxy" , &L3Reco_l3_dxy , "L3Reco_l3_dxy/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_sigmadxy" , &L3Reco_l3_sigmadxy , "L3Reco_l3_sigmadxy/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_dz" ,       &L3Reco_l3_dz ,   "L3Reco_l3_dz/F" ,    bufsize);
  BtagTree->Branch("L3Reco_l3_sigmadz" ,  &L3Reco_l3_sigmadz ,   "L3Reco_l3_sigmadz/F" ,    bufsize);
  BtagTree->Branch("L3Reco_l3_eta" ,      &L3Reco_l3_eta , "L3Reco_l3_eta/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_phi" ,      &L3Reco_l3_phi , "L3Reco_l3_phi/F" , bufsize);    
  BtagTree->Branch("L25Reco_l25_dxy" , &L25Reco_l25_dxy , "L25Reco_l25_dxy/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_sigmadxy" , &L25Reco_l25_sigmadxy , "L25Reco_l25_sigmadxy/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_dz" ,       &L25Reco_l25_dz ,   "L25Reco_l25_dz/F" ,    bufsize);
  BtagTree->Branch("L25Reco_l25_sigmadz" ,  &L25Reco_l25_sigmadz ,   "L25Reco_l25_sigmadz/F" ,    bufsize);
  BtagTree->Branch("L25Reco_l25_eta" ,      &L25Reco_l25_eta , "L25Reco_l25_eta/F" , bufsize);
  BtagTree->Branch("L25Reco_l25_phi" ,      &L25Reco_l25_phi , "L25Reco_l25_phi/F" , bufsize);  
  BtagTree->Branch("L25Reco_reco_dxy" , &L25Reco_reco_dxy , "L25Reco_reco_dxy/F" , bufsize);
  BtagTree->Branch("L25Reco_reco_sigmadxy" , &L25Reco_reco_sigmadxy , "L25Reco_reco_sigmadxy/F" , bufsize);
  BtagTree->Branch("L25Reco_reco_dz" ,       &L25Reco_reco_dz ,   "L25Reco_reco_dz/F" ,    bufsize);
  BtagTree->Branch("L25Reco_reco_sigmadz" ,  &L25Reco_reco_sigmadz ,   "L25Reco_reco_sigmadz/F" ,    bufsize);
  BtagTree->Branch("L25Reco_reco_eta" ,      &L25Reco_reco_eta , "L25Reco_reco_eta/F" , bufsize);
  BtagTree->Branch("L25Reco_reco_phi" ,      &L25Reco_reco_phi , "L25Reco_reco_phi/F" , bufsize);  
  
  BtagTree->Branch("L3Reco_reco_ip2ndtrack" , &L3Reco_reco_ip2ndtrack , "L3Reco_reco_ip2ndtrack/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_ip2ndtrack" , &L3Reco_l3_ip2ndtrack , "L3Reco_l3_ip2ndtrack/F" , bufsize);
  BtagTree->Branch("L3Reco_reco_ip3rdtrack" , &L3Reco_reco_ip3rdtrack , "L3Reco_reco_ip3rdtrack/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_ip3rdtrack" , &L3Reco_l3_ip3rdtrack , "L3Reco_l3_ip3rdtrack/F" , bufsize);
  
  BtagTree->Branch("L3Reco_reco_sigmaip2ndtrack" , &L3Reco_reco_sigmaip2ndtrack , "L3Reco_reco_sigmaip2ndtrack/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_sigmaip2ndtrack" , &L3Reco_l3_sigmaip2ndtrack , "L3Reco_l3_sigmaip2ndtrack/F" , bufsize);
  BtagTree->Branch("L3Reco_reco_sigmaip3rdtrack" , &L3Reco_reco_sigmaip3rdtrack , "L3Reco_reco_sigmaip3rdtrack/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_sigmaip3rdtrack" , &L3Reco_l3_sigmaip3rdtrack , "L3Reco_l3_sigmaip3rdtrack/F" , bufsize);

  BtagTree->Branch("L3Reco_reco_ip2ndtrack3D" , &L3Reco_reco_ip2ndtrack3D , "L3Reco_reco_ip2ndtrack3D/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_ip2ndtrack3D" , &L3Reco_l3_ip2ndtrack3D , "L3Reco_l3_ip2ndtrack3D/F" , bufsize);
  BtagTree->Branch("L3Reco_reco_ip3rdtrack3D" , &L3Reco_reco_ip3rdtrack3D , "L3Reco_reco_ip3rdtrack3D/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_ip3rdtrack3D" , &L3Reco_l3_ip3rdtrack3D , "L3Reco_l3_ip3rdtrack3D/F" , bufsize);
  
  BtagTree->Branch("L3Reco_reco_sigmaip2ndtrack3D" , &L3Reco_reco_sigmaip2ndtrack3D , "L3Reco_reco_sigmaip2ndtrack3D/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_sigmaip2ndtrack3D" , &L3Reco_l3_sigmaip2ndtrack3D , "L3Reco_l3_sigmaip2ndtrack3D/F" , bufsize);
  BtagTree->Branch("L3Reco_reco_sigmaip3rdtrack3D" , &L3Reco_reco_sigmaip3rdtrack3D , "L3Reco_reco_sigmaip3rdtrack3D/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_sigmaip3rdtrack3D" , &L3Reco_l3_sigmaip3rdtrack3D , "L3Reco_l3_sigmaip3rdtrack3D/F" , bufsize);

  BtagTree->Branch("L3Reco_reco_deltar" , &L3Reco_reco_deltar , "L3Reco_reco_deltar/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_deltar" , &L3Reco_l3_deltar , "L3Reco_l3_deltar/F" , bufsize);
  BtagTree->Branch("L3Reco_reco_etajet" , &L3Reco_reco_etajet , "L3Reco_reco_etajet/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_etajet" , &L3Reco_l3_etajet , "L3Reco_l3_etajet/F" , bufsize);
  BtagTree->Branch("L3Reco_reco_Etjet" , &L3Reco_reco_Etjet , "L3Reco_reco_Etjet/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_Etjet" , &L3Reco_l3_Etjet , "L3Reco_l3_Etjet/F" , bufsize);
  BtagTree->Branch("L3Reco_reco_phijet" , &L3Reco_reco_phijet , "L3Reco_reco_phijet/F" , bufsize);
  BtagTree->Branch("L3Reco_l3_phijet" , &L3Reco_l3_phijet , "L3Reco_l3_phijet/F" , bufsize);
  
  BtagTree->Branch("l25_reco_assoc" , &l25_reco_assoc , "l25_reco_assoc/F" , bufsize);
  BtagTree->Branch("l3_reco_assoc" , &l3_reco_assoc , "l3_reco_assoc/F" , bufsize);
  
  BtagTree->Branch("eventNum" , &eventNum , "eventNum/i" , bufsize);
  BtagTree->Branch("L1pass" ,  &L1pass ,    "L1pass/F" ,  bufsize);
  BtagTree->Branch("L2pass" ,  &L2pass ,    "L2pass/F" ,  bufsize);
  BtagTree->Branch("L25pass" , &L25pass ,   "L25pass/F" , bufsize);
  BtagTree->Branch("L3pass" ,  &L3pass ,    "L3pass/F" ,  bufsize);
}

void BtagHLt::beginRun(const edm::Run & run, const edm::EventSetup & setup) {
  using namespace std;
  cout << "begin run"<<endl;
  
  HLTConfigProvider configProvider;
  
  bool changed = false;
  if (not configProvider.init(run, setup, m_processName, changed))
    {
      edm::LogWarning("ConfigurationError") << "process name \"" << m_processName << "\" is not valid.";
      m_init = false;
      return;
    }
  
  m_pathIndex = configProvider.triggerIndex( m_pathName );
  if (m_pathIndex == configProvider.size())
    {
      edm::LogWarning("ConfigurationError") << "trigger name \"" << m_pathName << "\" is not valid.";
      m_init = false;
      return;
    }
  
  m_init = true;
  
  // if their call fails, these will be set to one after the last valid module for their path
  // so they will never be "passed"
  unsigned int size = configProvider.size( m_pathIndex );
  
  m_L1FilterIndex  = configProvider.moduleIndex( m_pathIndex, m_L1Filter.encode()  );
  if (m_L1FilterIndex == size)
    edm::LogWarning("ConfigurationError") << "L1 filter \"" << m_L1Filter << "\" is not valid.";
  
  m_L2FilterIndex  = configProvider.moduleIndex( m_pathIndex, m_L2Filter.encode()  );
  if (m_L2FilterIndex == size)
    edm::LogWarning("ConfigurationError") << "L2 filter \"" << m_L2Filter << "\" is not valid.";

  m_L25FilterIndex = configProvider.moduleIndex( m_pathIndex, m_L25Filter.encode() );
  if (m_L25FilterIndex == size)
    edm::LogWarning("ConfigurationError") << "L2.5 filter \"" << m_L25Filter << "\" is not valid.";

  m_L3FilterIndex  = configProvider.moduleIndex( m_pathIndex, m_L3Filter.encode()  );
  if (m_L3FilterIndex == size)
    edm::LogWarning("ConfigurationError") << "L3 filter \"" << m_L3Filter << "\" is not valid.";
}

std::vector<std::pair<reco::TrackIPTagInfo,reco::TrackIPTagInfo> > BtagHLt::associateJets(std::vector<reco::TrackIPTagInfo> reco, std::vector<reco::TrackIPTagInfo> hlt,double deltaRcut)   
{
  using namespace reco;
  std::vector<std::pair<TrackIPTagInfo,TrackIPTagInfo> > recohltassoc_jets;
  bool associated=false;
  for(std::vector<TrackIPTagInfo>::iterator itag= reco.begin(); itag!=reco.end(); itag++)
    {
      std::pair<TrackIPTagInfo,TrackIPTagInfo>  tagpair;
      for(std::vector<TrackIPTagInfo> ::iterator iitag= hlt.begin(); iitag!=hlt.end(); iitag++)
	{
	  //Double_t distance=100;
	  //Jet assojet;
	  //const reco::Jet & reco_jet = itag->jet();
	  //const reco::Jet & hlt_jet = iitag->jet();
	  Double_t deltaRjets=deltaR(itag->jet()->eta(),itag->jet()->phi(),iitag->jet()->eta(),iitag->jet()->phi());
	  if(deltaRjets<deltaRcut){
	    deltaRcut=deltaRjets;
	    //assojet=*iijet;
	    tagpair = std::make_pair(*itag,*iitag);
	    //jetpair.first = ijet->first;
	    //jetpair.second = iijet->first;
	    associated=true;
	  }
	  else associated=false;
	  //recohltassoc_jets.push_back(jetpair); 
	}   
      if(associated)
	recohltassoc_jets.push_back(tagpair); 
    }
  return recohltassoc_jets;
}


// ------------ method called once each job just after ending the event loop  ------------
void 
BtagHLt::endJob() {
  output->Write();
  output->Close();
}




//define this as a plug-in
DEFINE_FWK_MODULE(BtagHLt);
