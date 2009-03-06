


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
#include <vector>


#include "DataFormats/Common/interface/View.h"
#include <string>


//
// class declaration
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
  
  void resetVariables();
  
  edm::Service<TFileService> fs;
  float whisto;
  int EventNumber,EventRun,EventSelect;

// Cuts and counters
float minPtjet,    // Minimum jet Pt [GeV]
      minPtmu,    // Minimum mu Pt [GeV]
      etajetcut,  // Eta jet cut
      etamucut,  // Eta mu cut
      Mmumu1,     // Z mass cut lower
      Mmumu2;    // Z mass cut higher
int nevents, neventsmu, neventsError, nevents1,nevents2,nevents3;
int njetsall, njetsall1,njetsall2,njetsall3;

  // Ntuple variable declaration  

  // muons in a vector
  // nmuonscounter are all mu with Pt>20, nmu are also global muons etc.
  int nmuonscounter, nmu;
  // const int nmuMax=10;  non funziona
  int nhits[10],charge[10];
  float pt[10],phi[10],eta[10],px[10],py[10],pz[10];
  float isolation3[10],isolation5[10],isoR03_emEt[10],isoR03_hadEt[10],isoR03_nTracks[10],
  vertex_x[10],vertex_y[10],vertex_z[10],dxy[10],dxy_error[10],chi2[10];
  // reconstructed and generated Z
  float MZrec,PTZrec,ETAZrec;
  float MZgen,PTZgen,pt1gen,pt2gen,resMZrec,resPTZrec,ptmugen[10];
  // not used for the moment float x2, PX,PY,PTZgen,Mt,PZ,EZ,MZgen,ETAZ;
  // jets in a vector
  // const njetsMax=100;   non funziona
 float jetptGen[100], jetetaGen[100], jetphiGen[100],
        jetpt[100], jetphi[100], jeteta[100], jetptGen_match[100],
        res_ptGenJet_ptJet[100],res_ptGenJet_ptJet_match[100],
        jetDeltaR1[100],jetDeltaR2[100]; 
  size_t nphotonscounter;
  int njetscounter, njetscounterGen, particelle_jet[100];

  int mamphoton,nmam;
  TTree *ZMM;
 // histograms
 // TH1F *hEff1 = new TH1F("hEff1", "Efficiency 1 jet rec/all", 100, 0., 1.);
 // TH1F *hEff2 = new TH1F("hEff2", "Efficiency 2 jets rec/all", 100, 0., 1.);
 // TH1F *hEff3 = new TH1F("hEff3", "Efficiency 3 jets rec/all", 100, 0., 1.);

  std::map<std::string,TH1D*> histocontainer_; // simple map to contain all histograms. Histograms are booked in the beginJob() method
 
  edm::InputTag eleLabel_;
  edm::InputTag muoLabel_;
  edm::InputTag jetLabel_;
  edm::InputTag tauLabel_;
  edm::InputTag metLabel_;
  edm::InputTag phoLabel_;
  edm::InputTag jetGenLabel_;
  edm::InputTag beamspotLabel_;
  
};



ZjetsNtupleMaker::ZjetsNtupleMaker(const edm::ParameterSet& iConfig):
  histocontainer_(),
  eleLabel_(iConfig.getParameter<edm::InputTag>("electronTag")),
  muoLabel_(iConfig.getParameter<edm::InputTag>("muonTag")),
  jetLabel_(iConfig.getParameter<edm::InputTag>("jetTag")),
  tauLabel_(iConfig.getParameter<edm::InputTag>("tauTag")),
  metLabel_(iConfig.getParameter<edm::InputTag>("metTag")),
  phoLabel_(iConfig.getParameter<edm::InputTag>("photonTag")),
  jetGenLabel_(iConfig.getParameter<edm::InputTag>("jetGenTag")),
  beamspotLabel_(iConfig.getParameter<edm::InputTag>("beamSpotTag"))
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

  resetVariables();
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

  //edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
  //iEvent.getByLabel(beamspotLabel_, recoBeamSpotHandle);
  //iEvent.getByType(recoBeamSpotHandle);
  //  cout << "beamSpot position" << recoBeamSpotHandle->position() << endl;
  reco::TrackBase::Point beamSpot(0,0,0);


  // double p1=0;     // all this needed?
  // double p2=0;
  // double absP=-1000;
   
  reco::Particle::LorentzVector Pmu[10], Pmugen[10], P, P1, P2, P1gen,P2gen,PG;

  EventNumber = iEvent.id().event();
  EventRun=iEvent.id().run();
  nevents++;
  if( (nevents%5000)==0) cout<<"Event being analyzed = "<<nevents<<endl;
  

  //count jets here, without muon selection
  njetsall=0;
      for(edm::View<pat::Jet>::const_iterator jet_iter = jets.begin(); jet_iter!=jets.end(); ++jet_iter){
	if(jet_iter->pt()>minPtjet && abs(jet_iter->eta())<etajetcut){
	  njetsall++;
	}}

          if(njetsall==1){
            njetsall1++;
          }
          if(njetsall==2){
            njetsall2++;
          }
          if(njetsall==3){
            njetsall3++;
          }


  //std::map<int,pat::Muon> MuonMap;
  std::vector<pat::Muon> MuonMap;
  for(edm::View<pat::Muon>::const_iterator muon_iter = muons.begin(); muon_iter!=muons.end(); ++muon_iter){
    if(muon_iter->pt()>20){
      //MuonMap[nmuonscounter]=*muon_iter;
      MuonMap.push_back(*muon_iter);
    }
      }
  nmuonscounter=MuonMap.size();
  //cout<<"il numero di muoni è: "<<nmuonscounter<<endl;
 
 
 
  // do stuff only if at least 2 muons are present and cut on 10 muons for safety
  // if (nmuonscounter < 2 || nmuonscounter>10) return;
  if(nmuonscounter>10) return; 
  neventsmu++;
  nmu=0;
  for(int i=0; i<nmuonscounter;i++){
    if((MuonMap[i].isGlobalMuon())) {     // fill for good muons
  
  // fill info about the muons
      // note: need to take global normalized chi2 and nvalidhits
    chi2[nmu]=MuonMap[i].globalTrack()->normalizedChi2();
    nhits[nmu]=MuonMap[i].innerTrack()->numberOfValidHits();
    charge[nmu]=MuonMap[i].charge();
    pt[nmu]=MuonMap[i].pt();
    phi[nmu]=MuonMap[i].phi();
    eta[nmu]=MuonMap[i].eta();
    px[nmu]=MuonMap[i].px();
    py[nmu]=MuonMap[i].py();
    pz[nmu]=MuonMap[i].pz();
    isolation3[nmu]=MuonMap[i].isolationR03().sumPt;
    isolation5[nmu]=MuonMap[i].isolationR05().sumPt;
    isoR03_emEt[nmu]=MuonMap[i].isolationR03().emEt;
    isoR03_hadEt[nmu]=MuonMap[i].isolationR03().hadEt;
    isoR03_nTracks[nmu]=MuonMap[i].isolationR03().nTracks;
    vertex_x[nmu]=MuonMap[i].vertex().X();
    vertex_y[nmu]=MuonMap[i].vertex().Y();
    vertex_z[nmu]=MuonMap[i].vertex().Z();
    dxy[nmu]=MuonMap[i].innerTrack()->dxy(beamSpot);
    dxy_error[nmu]=MuonMap[i].innerTrack()->dxyError();
    Pmu[nmu]=MuonMap[i].p4();        // for reconstructed Z

    if(MuonMap[i].genLepton()){     // for generated Z
      Pmugen[nmu]=MuonMap[i].genLepton()->p4();
      ptmugen[nmu]=MuonMap[i].genLepton()->pt();
      }
    nmu++;
    } // endif for requirements on muons
  } // end loop on muons
      

// reconstruct the Z from two highest pt selected muons

  P1=Pmu[0];
  P2=Pmu[1];
  P=P1+P2;
  MZrec=P.mass();
  //PTZrec=sqrt(pow(px1+px2,2)+pow(py1+py2,2));
  PTZrec = P.pt();

  // now the generated Z
    P1gen=Pmugen[0];
    P2gen=Pmugen[1];
    pt1gen=ptmugen[0];
    pt2gen=ptmugen[1];
    PG=P1gen+P2gen;
    MZgen=PG.mass();
    PTZgen=PG.pt();
    resMZrec=(MZrec-MZgen)/MZgen;
    resPTZrec=(PTZrec-PTZgen)/PTZgen;  



    /*   all this below commented out for the moment

  //now look at the generated quantities for leptons
  if(MuonMap[0].genLepton() && MuonMap[1].genLepton()){

    pt1gen=MuonMap[0].genLepton()->pt();
    pt2gen=MuonMap[1].genLepton()->pt();


    p1xgen=MuonMap[0].genLepton()->px();
    p2xgen=MuonMap[1].genLepton()->px();

    p1ygen=MuonMap[0].genLepton()->py();
    p2ygen=MuonMap[1].genLepton()->py();
    P1gen=MuonMap[0].genLepton()->p4();
    P2gen=MuonMap[1].genLepton()->p4();
    //PTZgen=sqrt(pow(p1xgen+p2xgen,2)+pow(p1ygen+p2ygen,2));


    nmam=0;
    if(!MuonMap[0].genLepton()->mother()){
      cout<<"il muone generato non non ha mamma"<<endl;
    }else{
      const reco::Candidate *mam=MuonMap[0].genLepton()->mother();
      while (abs(mam->pdgId())!=23 && mam->mother()){
        cout<<"il muone e figlio di = "<<mam->pdgId()<<endl;
        const reco::Candidate *newmam=mam->mother();
        mam=newmam;
        nmam++;
      }
      if(abs(mam->pdgId())!=23){
        cout<<"errore"<<endl;
      } else {
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
      }
    }
  }


    */




  

  // now start loop with the jets, fill them in a vector
      float newDeltaR1;
      float newDeltaR2;
      for(edm::View<pat::Jet>::const_iterator jet_iter = jets.begin(); jet_iter!=jets.end(); ++jet_iter){
	  
	  
	if(jet_iter->pt()>minPtjet && abs(jet_iter->eta())<etajetcut){
          jetDeltaR1[njetscounter]=100.;
          jetDeltaR2[njetscounter]=100.;
	  float jetPhi=jet_iter->phi();
	  float jet_eta=jet_iter->eta();
          if(nmu>=2){  
            float dphi1=phi[0]-jetPhi;                        // distance from first muon
            // if(dphi1<-1.*TMath::Pi())dphi1=dphi1+2.*TMath::Pi();
            // if(dphi1>1.*TMath::Pi()) dphi1=dphi1-2.*TMath::Pi();    
            float dphi2=phi[1]-jetPhi;                       // distance from second muon
            // if(dphi2<-1.*TMath::Pi())dphi2=dphi2+2.*TMath::Pi();
            // if(dphi2>1.*TMath::Pi()) dphi2=dphi2-2.*TMath::Pi();
	    newDeltaR1=sqrt(pow(dphi1,2)+pow(eta[0]-jet_eta,2)); // distance from first muon
	    newDeltaR2=sqrt(pow(dphi2,2)+pow(eta[1]-jet_eta,2)); // distance frm second muon
            if(newDeltaR1<jetDeltaR1[njetscounter]){
	      jetDeltaR1[njetscounter]=newDeltaR1;}
	    if(newDeltaR2<jetDeltaR2[njetscounter]){
	      jetDeltaR2[njetscounter]=newDeltaR2;}
          }

	    jetpt[njetscounter]=jet_iter->pt();
	    jeteta[njetscounter]=jet_iter->eta();
	    jetphi[njetscounter]=jet_iter->phi();
	    if(jet_iter->genJet()){
	      jetptGen_match[njetscounter] = jet_iter->genJet()->pt();
	      res_ptGenJet_ptJet_match[njetscounter]=(jetptGen_match[njetscounter]-jetpt[njetscounter])/jetptGen_match[njetscounter];
	    }  

	  njetscounter++;
	}  // endif on jet cuts
      }    // end loop on jets
    
  

// now generated jets, test for the PATtuples 


      for(reco::GenJetCollection::const_iterator jetGen_iter = jetsGen.begin(); jetGen_iter!=jetsGen.end(); ++jetGen_iter){ 
	if(jetGen_iter->pt()>minPtjet && abs(jetGen_iter->eta())<etajetcut){



	    jetptGen[njetscounterGen]=jetGen_iter->pt();
	    jetetaGen[njetscounterGen]=jetGen_iter->eta();
	    jetphiGen[njetscounterGen]=jetGen_iter->phi();

 	  bool hasMuon = false;
	  	  std::vector< const reco::GenParticle * > numeroparticelle= jetGen_iter->getGenConstituents();
                  particelle_jet[njetscounterGen]=numeroparticelle.size();
	  // these are the 2 muons from Z misidentified as jets
	 	  for(std::vector< const reco::GenParticle * >::const_iterator jet_particle=numeroparticelle.begin();jet_particle!=numeroparticelle.end();jet_particle++){
		    // cout<<"che particelle sono "<<(*jet_particle)->pdgId()<<endl;
	    if (abs((*jet_particle)->pdgId())==13 && (*jet_particle)->pt()>20){
	      hasMuon = true;
	    }
	  }
	  if (!hasMuon ||  numeroparticelle.size() > 15) njetscounterGen++; 
	    

	}   // endif on generator jets cuts
      }    // end loop on generated jets




      // Counters

          if(njetscounter>=1){
            nevents1++;
          }
          if(njetscounter>=2){
            nevents2++;
          }
          if(njetscounter>=3){
            nevents3++;
          }

                   
  ZMM->Fill(); 
              
}  
  
      
   


 

void ZjetsNtupleMaker::beginJob(const edm::EventSetup&)
{ 

  nevents=0;      // number of events read
  neventsmu=0;   // number of events with two muons
  nevents1=0;     // number of events with 2mu+>=1 jet
  nevents2=0;     // number of events with 2mu+>=2 jets
  nevents3=0;     // number of events with 2mu+>=3 jets
  njetsall1=0;   // all events and exactly 1 jet
  njetsall2=0;    // all events and exactly 2 jets
  njetsall3=0;    // all events and exactly 3 jets
  minPtjet = 30.;   // Minimum jet Pt [GeV]
  minPtmu = 20.;    // Minimum mu Pt [GeV]
  etajetcut= 3.0;  // Eta jet cut
  etamucut = 2.5 ;  // Eta mu cut
  Mmumu1 = 60.;     // Z mass cut lower
  Mmumu2 = 120.;    // Z mass cut higher
  std::cout << "Minimum pt mu cut is  (in GeV) " << minPtmu <<  std::endl;
  std::cout << "Minimum pt jet cut is  (in GeV) " << minPtjet <<  std::endl;
  std::cout << "Minimum etajet cut is   " << etajetcut <<  std::endl;
 
 


 ZMM=fs->make<TTree>("ZMM","ZMM Firenze tree");
 ZMM->Branch("particelle_jet",&particelle_jet,"particelle_jet/I");
 // general quantities
 ZMM->Branch("EventNumber",&EventNumber,"EventNumber/I");
 ZMM->Branch("EventRun",&EventRun,"EventRun/I");
 ZMM->Branch("whisto",&whisto,"whisto/F");

 // muons
 ZMM->Branch("nmu",&nmu,"nmu/I");
 ZMM->Branch("chi2",&chi2,"chi2[nmu]/F");
 ZMM->Branch("nhits",&nhits,"nhits[nmu]/I");
 ZMM->Branch("charge",&charge,"charge[nmu]/I");
 ZMM->Branch("pt",&pt,"pt[nmu]/F");
 ZMM->Branch("phi",&phi,"phi[nmu]/F");
 ZMM->Branch("eta",&eta,"eta[nmu]/F");
 ZMM->Branch("px",&px,"px[nmu]/F");
 ZMM->Branch("py",&py,"py[nmu]/F");
 ZMM->Branch("pz",&pz,"pz[nmu]/F");
 ZMM->Branch("isolation3",&isolation3,"isolation3[nmu]/F");
 ZMM->Branch("isolation5",&isolation5,"isolation5[nmu]/F");
 ZMM->Branch("isoR03_emEt",&isoR03_emEt,"isoR03__emEt[nmu]/F");
 ZMM->Branch("isoR03_hadEt",&isoR03_hadEt,"isoR03_hadEt[nmu]/F");
 ZMM->Branch("isoR03_nTracks",&isoR03_nTracks,"isoR03_nTracks[nmu]/F");
 ZMM->Branch("dxy",&dxy,"dxy[nmu]/F");
 ZMM->Branch("dxy_error",&dxy_error,"dxy_error[nmu]/F");
 ZMM->Branch("vertex_x",&vertex_x,"vertex_x[nmu]/F");
 ZMM->Branch("vertex_y",&vertex_y,"vertex_y[nmu]/F");
 ZMM->Branch("vertex_z",&vertex_z,"vertex_z[nmu]/F");

 // Z reconstructed quantities
 ZMM->Branch("MZrec",&MZrec,"MZrec/F");
 ZMM->Branch("PTZrec",&PTZrec,"PTZrec/F");
 ZMM->Branch("ETAZrec",&ETAZrec,"ETAZrec/F");


 // generator quantities
 ZMM->Branch("mamphoton",&mamphoton,"mamphoton/I");
 ZMM->Branch("MZgen",&MZgen,"MZgen/F");
 ZMM->Branch("PTZgen",&PTZgen,"PTZgen/F");
 ZMM->Branch("pt1gen",&pt1gen,"pt1gen/F");
 ZMM->Branch("pt2gen",&pt2gen,"pt2gen/F");
 ZMM->Branch("resMZrec",&resMZrec,"resMZrec/F");
 ZMM->Branch("resPTZrec",&resPTZrec,"resPTZrec/F");

 //jets

 ZMM->Branch("njetscounter",&njetscounter,"njetscounter/I");
 ZMM->Branch("njetscounterGen",&njetscounterGen,"njetscounterGen/I");
 ZMM->Branch("jetpt",&jetpt,"jetpt[njetscounter]/F");
 ZMM->Branch("jeteta",&jeteta,"jeteta[njetscounter]/F");
 ZMM->Branch("jetphi",&jetphi,"jetphi[njetscounter]/F");
 ZMM->Branch("jetDeltaR1",&jetDeltaR1,"jetDeltaR1[njetscounter]/F");
 ZMM->Branch("jetDeltaR2",&jetDeltaR2,"jetDeltaR2[njetscounter]/F");
 ZMM->Branch("jetptGen",&jetptGen,"jetptGen[njetscounterGen]/F");
 ZMM->Branch("jetetaGen",&jetetaGen,"jetetaGen[njetscounterGen]/F");
 ZMM->Branch("jetphiGen",&jetphiGen,"jetphiGen[njetscounterGen]/F"); 
 ZMM->Branch("particelle_jet",&particelle_jet,"particelle_jet[njetscounterGen]/I");
 ZMM->Branch("jetptGen_match",&jetptGen_match,"jetptGen_match[njetscounter]/F");
 ZMM->Branch("res_ptGenJet_ptJet",&res_ptGenJet_ptJet,"res_ptGenJet_ptJet[njetscounter]/F");
 ZMM->Branch("res_ptGenJet_ptJet_match",&res_ptGenJet_ptJet_match,"res_ptGenJet_ptJet_match[njetscounter]/F");



}


// ------------ method called once each job just after ending the event loop  ------------
void 
ZjetsNtupleMaker::endJob() {


  std::cout << "Number of events read " << nevents << std::endl;
  std::cout << "Number of events with 1 jet " << njetsall1 << std::endl;
  std::cout << "Number of events with 2 jets " << njetsall2 << std::endl;
  std::cout << "Number of events with 3 jets " << njetsall3 << std::endl;

  std::cout << "Number of events read with >=2 muons" << neventsmu << std::endl;
  std::cout << "Number of events with 2mu+>=1 jet " << nevents1 << std::endl;
  std::cout << "Number of events with 2mu+>=2 jets " << nevents2 << std::endl;
  std::cout << "Number of events with 2mu+>=3 jets " << nevents3 << std::endl;
  
  float eff1= (float)njetsall1/(float)njetsall;
  // hEff1->Fill(eff1);
  float eff2= (float)njetsall2/(float)njetsall;
  // hEff2->Fill(eff2);
  float eff3= (float)njetsall3/(float)njetsall;
  // hEff3->Fill(eff3);

}

/// Reset all variables in the tree

void ZjetsNtupleMaker::resetVariables() {




  EventRun=0;
  EventNumber=0;
  for(int i=0; i<10; i++){
    pt[i]=0.;
    phi[i]=-100.;
    eta[i]=-100.;
    px[i]=0.;
    py[i]=0.;
    pz[i]=0.;
    isolation3[i]=-1.;
    isolation5[i]=-1.;
    isolation3[i]=-1.;
    isolation5[i]=-1.;
    isoR03_emEt[i]=-10;
    isoR03_nTracks[i]=-10;
    isoR03_hadEt[i]=-10;
    charge[i]=-10;
    chi2[i]=-10.;
    nhits[i]=-10;
    vertex_x[i]=-10;
    vertex_y[i]=-10;
    vertex_z[i]=-10;
    dxy[i]=-10.;
    dxy_error[i]=-10.;
  }

  PTZgen=-1.;
  MZgen=-1.;
  mamphoton=-1;
  MZrec=-1.;
  PTZrec=-1.;
  ETAZrec=-1.;
  pt1gen=-1.;
  pt2gen=-1.;
  resPTZrec=-50.;
  resMZrec=-50.;

  njetscounter=0;
  njetscounterGen=0;
  // for(int i=0; i++; i<maxJets){
  for(int i=0; i<100; i++){
    jetptGen[i]=0;
    jetetaGen[i]=-10.;
    jetphiGen[i]=-10.;
    jetpt[i]=0;
    jeteta[i]=-10;
    jetphi[i]=-10.;
    jetDeltaR1[i]=-10.;
    jetDeltaR2[i]=-10.;
    jetptGen_match[i]=0.; 
    res_ptGenJet_ptJet[i]=-20.;
    res_ptGenJet_ptJet_match[i]=-20.;
    particelle_jet[i]=-10; 
 }
}



//define this as a plug-in
DEFINE_FWK_MODULE(ZjetsNtupleMaker);
