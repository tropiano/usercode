


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ParameterSet/interface/InputTag.h"


#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "Calibration/HcalCalibAlgos/src/GammaJetAnalysis.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include <DataFormats/TrackReco/interface/TrackBase.h>
#include <TTree.h>
#include "TH1D.h"
#include <map>


#include "DataFormats/Common/interface/View.h"
#include <string>


//
// class decleration
//

class ZjetsNtupleMaker : public edm::EDAnalyzer {
public:
  explicit ZjetsNtupleMaker(const edm::ParameterSet&);
  ~ZjetsNtupleMaker();
  // void store();

private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  edm::Service<TFileService> fs;
  float PTZ,MZ,MR,Phi,pt1,pt2,MASSAZ,PTZrec,resPTrec,resPTgen,eta1,eta2,pt1gen,pt2gen,p1xgen,p1ygen,p2xgen,p2ygen,px1,py1,px2,resMZ_MZgen,DeltaR;   
  float x2,py2, PX,PY,PTZgen,phi1,phi2,Mt,pz1,pz2,PZ,EZ,MZgen,jetpt,jeteta,ETAZ,isolation3_1,isolation5_1,isolation3_2,isolation5_2,whisto,jetptGen,res_ptGenJet_ptJet,jetetaGen,jetpt_second,jeteta_second,jetptGen_second,jetetaGen_second,jetptGen_match,res_ptGenJet_ptJet_match,isoR03_1_emEt, isoR03_1_nTracks,isoR03_1_hadEt,isoR03_2_emEt, isoR03_2_nTracks,isoR03_2_hadEt,vertex_x_1,	vertex_y_1,vertex_z_1,vertex_x_2,vertex_y_2,vertex_z_2,dxy1,dxy2,dxy1_error,dxy2_error,chi2_2,chi2_1 ; 
  //seconJetPt,secondJetEta;
  size_t nphotonscounter;
  int njetscounter, njetscounterGen, n2mu, particelle_jet,nhits_1, nhits_2;
  int EventNumber,EventRun,EventSelect;
  int mamphoton,charge1,charge2,nmam;
  TTree *ZMM_PYT1;
  std::map<std::string,TH1D*> histocontainer_; // simple map to contain all histograms. Histograms are booked in the beginJob() method
 
  edm::InputTag eleLabel_;
  edm::InputTag muoLabel_;
  edm::InputTag jetLabel_;
  edm::InputTag tauLabel_;
  edm::InputTag metLabel_;
  edm::InputTag phoLabel_;
  edm::InputTag jetGenLabel_;
};



ZjetsNtupleMaker::ZjetsNtupleMaker(const edm::ParameterSet& iConfig):
  histocontainer_(),
  eleLabel_(iConfig.getUntrackedParameter<edm::InputTag>("electronTag")),
  muoLabel_(iConfig.getUntrackedParameter<edm::InputTag>("muonTag")),
  jetLabel_(iConfig.getUntrackedParameter<edm::InputTag>("jetTag")),
  tauLabel_(iConfig.getUntrackedParameter<edm::InputTag>("tauTag")),
  metLabel_(iConfig.getUntrackedParameter<edm::InputTag>("metTag")),
  phoLabel_(iConfig.getUntrackedParameter<edm::InputTag>("photonTag")),
  jetGenLabel_(iConfig.getUntrackedParameter<edm::InputTag>("jetGenTag"))
{
  //
}


ZjetsNtupleMaker::~ZjetsNtupleMaker()
{
 
 
}


// ------------ method called to for each event  ------------
void
ZjetsNtupleMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  EventRun=0;

  EventNumber=0; 
  //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
  // first: get all objects from the event.
  //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}

  edm::Handle<edm::View<pat::Muon> > muonHandle;
  iEvent.getByLabel(muoLabel_,muonHandle);
  edm::View<pat::Muon> muons = *muonHandle;
   
  edm::Handle<edm::View<pat::Jet> > jetHandle;
  iEvent.getByLabel(jetLabel_,jetHandle);
  edm::View<pat::Jet> jets = *jetHandle;

  edm::Handle<reco::GenJetCollection>  jetGenHandle;
  iEvent.getByLabel(jetGenLabel_,jetGenHandle);
  reco::GenJetCollection jetsGen = *jetGenHandle;

  edm::Handle<edm::View<pat::Electron> > electronHandle;
  iEvent.getByLabel(eleLabel_,electronHandle);
  edm::View<pat::Electron> electrons = *electronHandle;

  edm::Handle<edm::View<pat::MET> > metHandle;
  iEvent.getByLabel(metLabel_,metHandle);
  edm::View<pat::MET> mets = *metHandle;

  edm::Handle<edm::View<pat::Photon> > phoHandle;
  iEvent.getByLabel(phoLabel_,phoHandle);
  edm::View<pat::Photon> photons = *phoHandle;

  edm::Handle<edm::View<pat::Tau> > tauHandle;
  iEvent.getByLabel(tauLabel_,tauHandle);
  edm::View<pat::Tau> taus = *tauHandle;

  edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
  iEvent.getByType(recoBeamSpotHandle);
  //  cout << "beamSpot position" << recoBeamSpotHandle->position() << endl;
  reco::TrackBase::Point beamSpot(recoBeamSpotHandle->position());

  EventNumber = iEvent.id().event();

  EventRun=iEvent.id().run();

 
  size_t  nmuonscounter=0;
  
  std::map<int,pat::Muon> MuonMap;
  for(edm::View<pat::Muon>::const_iterator muon_iter = muons.begin(); muon_iter!=muons.end(); ++muon_iter){
    if(muon_iter->pt()>20)
      {
	nmuonscounter++;
	
        
	MuonMap[nmuonscounter]=*muon_iter;
      }
    
  }  cout<<"il numero di muoni è: "<<nmuonscounter<<endl;
  n2mu=nmuonscounter;
  
  PTZ=-1000;
  PTZgen=-1000;
  MZ=-1000;
  MR=-1000;
  Phi=0;
  pt1=0;
  pt2=0;
  MASSAZ=-1000;
  PTZrec=800;
  resPTrec=-50;
  resPTgen=-50;
  MZgen=-1000;
  eta1=-100;
  eta2=-100;
  pt1gen=0;
  pt2gen=0; 
  p1xgen=0;
  p1ygen=0;
  p2xgen=0;
  p2ygen=0;
  px1=0;
  px2=0;
  py1=0;
  py2=0;
  PX=-1000;
  PY=-1000;
  phi1=0;
  phi2=0;  
  Mt=0;
  pz1=0; 
  pz2=0;
  PZ=-1000;
  EZ=0;
  mamphoton=-1000;
  njetscounter=0;
  njetscounterGen=0;
  ETAZ=-1000;
  isolation3_1=-1;
  isolation5_1=-1;
  isolation3_2=-1;
  isolation5_2=-1;
  jetptGen=0;
  jetptGen_match=0;
  jetetaGen=-10;
  jetptGen_second=0;
  jetetaGen_second=-10;
  jetpt=0;
  jeteta=-10;
  jetpt_second=0;
  jeteta_second=-10;
  res_ptGenJet_ptJet=-20;
  res_ptGenJet_ptJet_match=-20; 
  resMZ_MZgen=-10;
  int njetscounterGenNew=-10;
  double p1=0; 
  double p2=0;
  double absP=-1000;
  double jetPhi=-20;
  double jet_eta=-20;
  charge1=-10;
  charge2=-10;
  isoR03_1_emEt=-20;
  isoR03_1_nTracks=-10;
  isoR03_1_hadEt=-10;
 particelle_jet=-10;
  isoR03_2_emEt=-20;
  isoR03_2_nTracks=-10;
  isoR03_2_hadEt=-10;
  vertex_x_1=-10;
  vertex_y_1=-10;
  vertex_z_1=-10;

  vertex_x_2=-10;
  vertex_y_2=-10;
  vertex_z_2=-10;
  chi2_2=-10;
  chi2_1=-10;
  nhits_1=-10;
  nhits_2=-10;
  // numeroparticelle=-10;
  float newDeltaR;
  //const float NumberOfevents=50000;
  //float mclumi=NumberOfevents/1233;
  //whisto=100./mclumi;

  //  const float NumberOfevents=50000;
  //float mclumi=NumberOfevents/3700;
  //whisto=100/mclumi;
  //math::XYZTLorentzVector LorentzVector;
   
  reco::Particle::LorentzVector P1, P2, P,P1gen,P2gen,PG;
   
  cout<<"Numero di evento = "<<EventNumber<<endl;


   
  if(nmuonscounter==2){
    cout<<"sono dentro l'if"<<endl;
    
    if(MuonMap[1].isGlobalMuon() && MuonMap[2].isGlobalMuon()){ 
      
      // nhits_1=MuonMap[1].innerTrack()->nHits();      
      chi2_1=MuonMap[1].innerTrack()->chi2();
      dxy1=MuonMap[1].innerTrack()->dxy(beamSpot);
      dxy1_error=MuonMap[1].innerTrack()->dxyError();
      charge1=MuonMap[1].charge();
      phi1=MuonMap[1].phi();
      pt1=MuonMap[1].pt();
      px1=MuonMap[1].px();
      py1=MuonMap[1].py();
      pz1=MuonMap[1].pz();
      p1=MuonMap[1].p();
      P1=MuonMap[1].p4();
      eta1=MuonMap[1].eta();
      isolation3_1=MuonMap[1].isolationR03().sumPt;
      isolation5_1=MuonMap[1].isolationR05().sumPt;
      isoR03_1_emEt=MuonMap[1].isolationR03().emEt;
      isoR03_1_hadEt=MuonMap[1].isolationR03().hadEt;
      isoR03_1_nTracks=MuonMap[1].isolationR03().nTracks;


      // nhits_2=MuonMap[2].innerTrack()->nHits();	
      chi2_2=MuonMap[2].innerTrack()->chi2();
      dxy2=MuonMap[2].innerTrack()->dxy();
      dxy2_error=MuonMap[2].innerTrack()->dxyError();
      charge2=MuonMap[2].charge();
      phi2=MuonMap[2].phi();
      pt2=MuonMap[2].pt();
      px2=MuonMap[2].px();
      py2=MuonMap[2].py();
      pz2=MuonMap[2].pz();
      p2=MuonMap[2].p();
    
      P2=MuonMap[2].p4();
      eta2=MuonMap[2].eta();
      isolation3_2=MuonMap[2].isolationR03().sumPt;
      isolation5_2=MuonMap[2].isolationR05().sumPt;
      isoR03_2_emEt=MuonMap[2].isolationR03().emEt;
      isoR03_2_hadEt=MuonMap[2].isolationR03().hadEt;
      isoR03_2_nTracks=MuonMap[2].isolationR03().nTracks;

	  
      vertex_x_1=MuonMap[1].vertex().X();
      vertex_y_1=MuonMap[1].vertex().Y();
      vertex_z_1=MuonMap[1].vertex().Z();
	  
      vertex_x_2=MuonMap[2].vertex().X();
      vertex_y_2=MuonMap[2].vertex().Y();
      vertex_z_2=MuonMap[2].vertex().Z();

      P=P1+P2;
      Phi = fabs(phi1-phi2);
	  
      Mt=sqrt(2*(pt1*pt2*(1-cos(Phi))));
	  
      MR=P.mass();
      PTZrec=sqrt(pow(px1+px2,2)+pow(py1+py2,2));
  
      
	  
      DeltaR=100;
      for(edm::View<pat::Jet>::const_iterator jet_iter = jets.begin(); jet_iter!=jets.end(); ++jet_iter){
	  
	  
	if(jet_iter->pt()>30 && abs(jet_iter->eta())<3){
	  njetscounter++;
	  jetPhi=jet_iter->phi();
	  jet_eta=jet_iter->eta();
	  newDeltaR=sqrt(pow(phi1-jetPhi,2)+pow(eta1-jet_eta,2));
	    
	  if(newDeltaR<DeltaR){
	    DeltaR=newDeltaR;}

	      

	  if(njetscounter==1){
	
	    jetpt=jet_iter->pt();
	    jeteta=jet_iter->eta();
	    if(jet_iter->genJet()){
	      jetptGen_match = jet_iter->genJet()->pt();
	      res_ptGenJet_ptJet_match=(jetptGen_match-jetpt)/jetptGen_match;
	    }
	  }
		  
	    if(njetscounter==2){
	      jetpt_second=jet_iter->pt();
	      jeteta_second=jet_iter->eta();
	    }
	  
	}
      }
	
	
      
      for(reco::GenJetCollection::const_iterator jetGen_iter = jetsGen.begin(); jetGen_iter!=jetsGen.end(); ++jetGen_iter){ 
	if(jetGen_iter->pt()>30 && abs(jetGen_iter->eta())<3){

	  bool hasMuon = false;
	  
	  std::vector< const reco::GenParticle * > numeroparticelle= jetGen_iter->getGenConstituents();

	  for(std::vector< const reco::GenParticle * >::const_iterator jet_particle=numeroparticelle.begin();jet_particle!=numeroparticelle.end();jet_particle++){
	    cout<<"che particelle sono "<<(*jet_particle)->pdgId()<<endl;
	    if (abs((*jet_particle)->pdgId())==13 && (*jet_particle)->pt()>20){
	      hasMuon = true;
	    }
	  }
	  if (!hasMuon ||  numeroparticelle.size() > 15) njetscounterGen++;
	    
	  if(njetscounterGen==1){
	    jetptGen=jetGen_iter->pt();
	    jetetaGen=jetGen_iter->eta();
	  
	 
	  }
	  if(njetscounterGen==2){
	    jetptGen_second=jetGen_iter->pt();
	    jetetaGen_second=jetGen_iter->eta();

	  

	  }
	}
      }
			
      if(MuonMap[1].genLepton() && MuonMap[2].genLepton()){
      
	pt1gen=MuonMap[1].genLepton()->pt();
	pt2gen=MuonMap[2].genLepton()->pt();
       

	p1xgen=MuonMap[1].genLepton()->px();
	p2xgen=MuonMap[2].genLepton()->px();
      
	p1ygen=MuonMap[1].genLepton()->py();
	p2ygen=MuonMap[2].genLepton()->py();
	P1gen=MuonMap[1].genLepton()->p4();
	P2gen=MuonMap[2].genLepton()->p4();
	PTZgen=sqrt(pow(p1xgen+p2xgen,2)+pow(p1ygen+p2ygen,2));
	PG=P1gen+P2gen;
	MZgen=PG.mass();
	resPTrec=(PTZ-PTZrec)/PTZ;
      
	nmam=0;
	if(!MuonMap[1].genLepton()->mother()){cout<<"il muone generato non non ha mamma"<<endl;
      
	}else{
	
	  const reco::Candidate *mam=MuonMap[1].genLepton()->mother();
	  while (abs(mam->pdgId())!=23 && mam->mother()){
	    cout<<"il muone e figlio di = "<<mam->pdgId()<<endl;	  
	    const reco::Candidate *newmam=mam->mother();
	    mam=newmam;
	    nmam++;
	  }	 
	  if(abs(mam->pdgId())!=23
){cout<<"errore"<<endl;}else{
	       
	    cout<<"ho ricostruito la Z"<<endl;	
	  
	    MZ=mam->mass();
	    PX=mam->px();
	    PY=mam->py();
	    PZ=mam->pz();
	    EZ=mam->energy();
	    PTZ=mam->pt();
	    ETAZ=mam->eta();
	    cout<<"la particella e'= "<<mam->pdgId();
	    cout<<" il PTZ vale= "<<PTZ<<endl;
	    absP=pow(PX,2)+pow(PY,2)+pow(PZ,2);
	    MASSAZ=sqrt(pow(EZ,2)-absP);
	    // cout << "Genealogia " << nmam++ <<"tipo "<<mam->pdgId()<< endl;

	    resMZ_MZgen=(MZ-MZgen)/MZ;

	
	  }

	}
        
      }  

      ZMM_PYT1->Fill(); 
     
  
    }
    
  }    
	        
}  
    
      
   


 

void ZjetsNtupleMaker::beginJob(const edm::EventSetup&)
{ 
 n2mu=0;

 ZMM_PYT1=fs->make<TTree>("ZMM_PYT1","albero");
 ZMM_PYT1->Branch("particelle_jet",&particelle_jet,"particelle_jet/I");
 ZMM_PYT1->Branch("n2mu",&n2mu,"n2mu/I");
 ZMM_PYT1->Branch("whisto",&whisto,"whisto/F");
 ZMM_PYT1->Branch("vertex_x_1",&vertex_x_1,"vertex_x_1/F");
 ZMM_PYT1->Branch("vertex_y_1",&vertex_y_1,"vertex_y_1/F");
 ZMM_PYT1->Branch("vertex_z_1",&vertex_z_1,"vertex_z_1/F");
 ZMM_PYT1->Branch("vertex_x_2",&vertex_x_2,"vertex_x_2/F");
 ZMM_PYT1->Branch("vertex_y_2",&vertex_y_2,"vertex_y_2/F");
 ZMM_PYT1->Branch("vertex_z_2",&vertex_z_2,"vertex_z_2/F");
 // ZMM_PYT1->Branch("nhits_1",&nhits_1,"nhits_1/I");
 // ZMM_PYT1->Branch("nhits_2",&nhits_2,"nhits_2/I");

 ZMM_PYT1->Branch("isoR03_1_emEt",&isoR03_1_emEt,"isoR03_1_emEt/F");
 ZMM_PYT1->Branch("isoR03_1_hadEt",&isoR03_1_hadEt,"isoR03_1_hadEt/F");
 ZMM_PYT1->Branch("isoR03_1_nTracks",&isoR03_1_nTracks,"isoR03_1_nTracks/F");
 ZMM_PYT1->Branch("isoR03_2_emEt",&isoR03_2_emEt,"isoR03_2_emEt/F");
 ZMM_PYT1->Branch("isoR03_2_hadEt",&isoR03_2_hadEt,"isoR03_2_hadEt/F");
 ZMM_PYT1->Branch("isoR03_2_nTracks",&isoR03_2_nTracks,"isoR03_2_nTracks/F");
 ZMM_PYT1->Branch("chi2_2",&chi2_2,"chi2_2/F");
 ZMM_PYT1->Branch("chi2_1",&chi2_1,"chi2_1/F");

 ZMM_PYT1->Branch("charge1",&charge1,"charge1/I");
 ZMM_PYT1->Branch("charge2",&charge2,"charge2/I");
 ZMM_PYT1->Branch("EventNumber",&EventNumber,"EventNumber/I");
 ZMM_PYT1->Branch("EventRun",&EventRun,"EventRun/I");
 ZMM_PYT1->Branch("isolation3_1",&isolation3_1,"isolation3_1/F");
 ZMM_PYT1->Branch("isolation5_1",&isolation5_1,"isolation5_1/F");
 ZMM_PYT1->Branch("isolation3_2",&isolation3_2,"isolation3_2/F");
 ZMM_PYT1->Branch("isolation5_2",&isolation5_2,"isolation5_2/F");
 ZMM_PYT1->Branch("mamphoton",&mamphoton,"mamphoton/I");
 ZMM_PYT1->Branch("PTZ",&PTZ,"PTZ/F");
 ZMM_PYT1->Branch("ETAZ",&ETAZ,"ETAZ/F");
 ZMM_PYT1->Branch("PTZgen",&PTZgen,"PTZgen/F");
 ZMM_PYT1->Branch("MZ",&MZ,"MZ/F");
 // ZMM->Branch("MASSAZ",&MASSAZ,"MASSAZ/F");
 ZMM_PYT1->Branch("MZgen",&MZgen,"MZgen/F");
 ZMM_PYT1->Branch("MR",&MR,"MR/F");
 ZMM_PYT1->Branch("Mt",&Mt,"Mt/F");
 ZMM_PYT1->Branch("Phi",&Phi,"Phi/F");
 ZMM_PYT1->Branch("phi1",&phi1,"phi1/F");
 ZMM_PYT1->Branch("phi2",&phi2,"phi2/F");
 ZMM_PYT1->Branch("pt1",&pt1,"pt1/F");
 //ZMM->1Brah("ptinner1",&ptinner1,"ptinner1/F");
 ZMM_PYT1->Branch("pt2",&pt2,"pt2/F");
 ZMM_PYT1->Branch("pt1gen",&pt1gen,"pt1gen/F");
 ZMM_PYT1->Branch("pt2gen",&pt2gen,"pt2gen/F");
 ZMM_PYT1->Branch("p1xgen",&p1xgen,"p1xgen/F");
 ZMM_PYT1->Branch("p2xgen",&p2xgen,"p2xgen/F");
 ZMM_PYT1->Branch("p1ygen",&p1ygen,"p1ygen/F");
 ZMM_PYT1->Branch("p2ygen",&p2ygen,"p2ygen/F");
 ZMM_PYT1->Branch("px1",&px1,"px1/F");
 ZMM_PYT1->Branch("px2",&px2,"px2/F");
 ZMM_PYT1->Branch("py1",&py1,"py1/F");
 ZMM_PYT1->Branch("py2",&py2,"py2/F");
 ZMM_PYT1->Branch("pz1",&pz1,"pz1/F");
 ZMM_PYT1->Branch("pz2",&pz2,"pz2/F");
 ZMM_PYT1->Branch("PTZrec",&PTZrec,"PTZrec/F");
 ZMM_PYT1->Branch("eta1",&eta1,"eta1/F");
 ZMM_PYT1->Branch("eta2",&eta2,"eta2/F");
 ZMM_PYT1->Branch("PX",&PX,"PX/F");
 ZMM_PYT1->Branch("PY",&PY,"PY/F");
 ZMM_PYT1->Branch("PZ",&PZ,"PZ/F");
 ZMM_PYT1->Branch("EZ",&EZ,"EZ/F");
 ZMM_PYT1->Branch("jeteta",&jeteta,"jeteta/F");
 ZMM_PYT1->Branch("jetpt",&jetpt,"jetpt/F");
 ZMM_PYT1->Branch("jetptGen",&jetptGen,"jetptGen/F");
 ZMM_PYT1->Branch("jetptGen_match",&jetptGen_match,"jetptGen_match/F");
 ZMM_PYT1->Branch("jetetaGen",&jetetaGen,"jetetaGen/F");
 ZMM_PYT1->Branch("jetptGen_second",&jetptGen_second,"jetptGen_second/F");
 ZMM_PYT1->Branch("jetetaGen_second",&jetetaGen_second,"jetetaGen_second/F");
 ZMM_PYT1->Branch("res_ptGenJet_ptJet",&res_ptGenJet_ptJet,"res_ptGenJet_ptJet/F");
 ZMM_PYT1->Branch("res_ptGenJet_ptJet_match",&res_ptGenJet_ptJet_match,"res_ptGenJet_ptJet_match/F");
 ZMM_PYT1->Branch("resPTrec",&resPTrec,"resPTrec/F");
 ZMM_PYT1->Branch("resPTgen",&resPTgen,"resPTgen/F");
 ZMM_PYT1->Branch("nphotonscounter",&nphotonscounter,"nphotonscounter/I");
 ZMM_PYT1->Branch("njetscounter",&njetscounter,"njetscounter/I");
 ZMM_PYT1->Branch("njetscounterGen",&njetscounterGen,"njetscounterGen/I");
 ZMM_PYT1->Branch("jetpt_second",&jetpt_second,"jetpt_second/F");
 ZMM_PYT1->Branch("jeteta_second",&jeteta_second,"jeteta_second/F");
 ZMM_PYT1->Branch("resMZ_MZgen",&resMZ_MZgen,"resMZ_MZgen/F");
 ZMM_PYT1->Branch("DeltaR",&DeltaR,"DeltaR/F");
 ZMM_PYT1->Branch("dxy1",&dxy1,"dxy1/F");
 ZMM_PYT1->Branch("dxy2",&dxy2,"dxy2/F");
 ZMM_PYT1->Branch("dxy1_error",&dxy1_error,"dxy1_error/F");
 ZMM_PYT1->Branch("dxy2_error",&dxy2_error,"dxy2_error/F");

}


// ------------ method called once each job just after ending the event loop  ------------
void 
ZjetsNtupleMaker::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(ZjetsNtupleMaker);
