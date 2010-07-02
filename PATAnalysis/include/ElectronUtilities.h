#ifndef PATAnalysisUtilities_h
#define PATAnalysisUtilities_h

#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"

#include <vector>
#include <math.h>
#include <sstream>
#include <string>
#include <assert.h>
#include <stdio.h>

#include "TMatrixD.h"
#include "TVectorD.h"
#include "TMath.h"
#include "TH1D.h"

using namespace std;
using namespace pat;
using namespace pat::helper;
using namespace TMath;

////////////////////////////////// Selection Cuts ////////////////////////////////////////////////

//Pt - VPJ
static double ptelcut = 20.;    //Gev/c

//Pt - VBTF
static double ptelcut0 = 20.;    //Gev/c
static double ptelcut1 = 10.;

//Eta
static double etaelcut = 2.4;
static double eta_el_excl_up = 1.56;               //Excluded Eta region
static double eta_el_excl_down = 1.4442;           //Excluded Eta region

//VPJ mass window
static double zmassmin_vpj = 50.;   //Gev/c^2
static double zmassmax_vpj = 130.;  //Gev/c^2

//VBTF mass window
static double zmassmin_vbtf = 60.;   //Gev/c^2
static double zmassmax_vbtf = 110.;  //Gev/c^2

static double minnhit = 11.;
static double maxchi2 = 10.;
static double dxycut = 0.02;     //cm
static double ptjetmin = 30.;   //Gev/c
static double etajetmax = 3.0;
static double isocut = 0.1;   //CombRelIso
static double isojetcut = 0.5; //Isolation jet - Z electron

//VPJ electron ID
static string eID_VPJ = "eidRobustLoose";

//Data-Spring10 triggers
static string ElectronTrigger = "HLT_Photon15_L1R";
static string JetTrigger = "HLT_Jet30U";

//Summer09 triggers
//static string ElectronTrigger = "HLT_Ele10_LW_L1R";
//static string JetTrigger = "HLT_Jet30";

//New cuts VBTF: tuned on Spring10. CombRelIso used. WP = 70/80 for dau0, 95 for dau1

/*
//// Dau0 - WP70 (70%)// EB
static double Dau0_CombIso_EB = 0.04;static double Dau0_sihih_EB = 0.01;static double Dau0_Dphi_vtx_EB = 0.03;static double Dau0_Deta_vtx_EB = 0.004;static double Dau0_HovE_EB = 0.025;// EE
static double Dau0_CombIso_EE = 0.03;static double Dau0_sihih_EE = 0.03;static double Dau0_Dphi_vtx_EE = 0.02;static double Dau0_Deta_vtx_EE = 0.005;static double Dau0_HovE_EE = 0.025;*/
//// Dau0 - WP80 (80%)// EB
static double Dau0_CombIso_EB = 0.07;static double Dau0_sihih_EB = 0.01;static double Dau0_Dphi_vtx_EB = 0.06;static double Dau0_Deta_vtx_EB = 0.004;static double Dau0_HovE_EB = 0.04;// EE
static double Dau0_CombIso_EE = 0.06;static double Dau0_sihih_EE = 0.03;static double Dau0_Dphi_vtx_EE = 0.03;static double Dau0_Deta_vtx_EE = 0.007;static double Dau0_HovE_EE = 0.025;
///// Dau1 - WP95 (95%)// EB
static double Dau1_CombIso_EB = 0.15;static double Dau1_sihih_EB = 0.01;static double Dau1_Dphi_vtx_EB = 0.8;static double Dau1_Deta_vtx_EB = 0.007;static double Dau1_HovE_EB = 0.5;
//EEstatic double Dau1_CombIso_EE = 0.1;static double Dau1_sihih_EE = 0.03;static double Dau1_Dphi_vtx_EE = 0.7;static double Dau1_Deta_vtx_EE = 0.01;static double Dau1_HovE_EE = 0.07;


// VPJ Tag cuts

static double VPJ_TAG_ptelcut = 25.;    //Gev/c
static double VPJ_TAG_etaelcut = 2.4;
static double VPJ_TAG_eta_el_excl_up = 1.56;               //Excluded Eta region
static double VPJ_TAG_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double VPJ_TAG_minnhit = 15.;
static double VPJ_TAG_maxchi2 = 3.;
static double VPJ_TAG_dxycut = 0.02;     //cm
static double VPJ_TAG_isocut = 0.05;                        //CombRelIso
static string VPJ_TagEiD = "eidRobustLoose";

// VBTF0 Tag cuts (for soft electron probe)

static double VBTF0_TAG_ptelcut = 20.;    //Gev/c
static double VBTF0_TAG_etaelcut = 2.4;
static double VBTF0_TAG_eta_el_excl_up = 1.56;               //Excluded Eta region
static double VBTF0_TAG_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double VBTF0_TAG_minnhit = 15.;
static double VBTF0_TAG_maxchi2 = 3.;
static double VBTF0_TAG_dxycut = 0.02;     //cm
static double VBTF0_TAG_isocut = 0.05;                        //CombRelIso
static string VBTF0_TagEiD = "eidRobustLoose";

// VBTF1 Tag cuts (for hard electron probe)

static double VBTF1_TAG_ptelcut = 10.;    //Gev/c
static double VBTF1_TAG_etaelcut = 2.4;
static double VBTF1_TAG_eta_el_excl_up = 1.56;               //Excluded Eta region
static double VBTF1_TAG_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double VBTF1_TAG_minnhit = 15.;
static double VBTF1_TAG_maxchi2 = 3.;
static double VBTF1_TAG_dxycut = 0.02;     //cm
static double VBTF1_TAG_isocut = 0.05;                        //CombRelIso
static string VBTF1_TagEiD = "eidRobustLoose";


//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Trigger

inline bool isTriggered(const pat::TriggerEvent& triggers, std::string triggername){
  const pat::TriggerPath* path = triggers.path(triggername);
  if (!path) {
    std::cout << "ERROR! trigger path " << triggername << " not found " << std::endl;
    return false;
  }
  return path->wasAccept();
}

inline bool isElectronTriggered(const pat::TriggerEvent& triggers){
  return isTriggered(triggers, ElectronTrigger.c_str());
}

inline bool isJetTriggered(const pat::TriggerEvent& triggers){
  return isTriggered(triggers, JetTrigger.c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Z Daughters Methods

inline const pat::Electron* CloneCandidate(const reco::Candidate& Daughter){

const pat::Electron* dau = dynamic_cast<const pat::Electron*>(&Daughter);

if(!dau){
const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> (&Daughter);
     if (scc && scc->hasMasterClone()){
       dau = dynamic_cast<const pat::Electron*>(scc->masterClone().get()); 
     }
     }

  return dau;
}

inline std::vector<const reco::Candidate*> ZGENDaughters(const reco::CompositeCandidate& ZGEN){
std::vector<const reco::Candidate*> zdaughters;

const reco::Candidate *dau = 0;
const reco::Candidate *daudau = 0;
  
  for(unsigned i = 0; i != ZGEN.numberOfDaughters(); i++){ 
  dau = ZGEN.daughter(i);
  
  if(!dau->numberOfDaughters()){
  if((dau->pdgId() == 11) || (dau->pdgId() == -11))zdaughters.push_back(dau);
  }
  
  if(dau->numberOfDaughters()){
  for(unsigned i = 0; i != dau->numberOfDaughters(); i++){
  daudau = dau->daughter(i);
  if((daudau->pdgId() == 11) || (daudau->pdgId() == -11))zdaughters.push_back(daudau);
  }
  }
  
  }
  
  if(zdaughters.size()==1 || zdaughters.size()>2){
  std::cout<<"ERROR! Wrong Z daughters association. Z daughters number = "<<zdaughters.size()<<std::endl;
  std::vector<const reco::Candidate*> nullzdaughters;
  return nullzdaughters;
  }else{
  return zdaughters;
  }
  
}

inline std::vector<const pat::Electron*> ZRECDaughters(const reco::CompositeCandidate ZREC){

 std::vector<const pat::Electron*> zdaughters;
  
  const pat::Electron* dau0 = CloneCandidate(*(ZREC.daughter(0)));
  const pat::Electron* dau1 = CloneCandidate(*(ZREC.daughter(1)));

 if(dau0 && dau1){
  zdaughters.push_back(dau0);
  zdaughters.push_back(dau1);
  }
  
    return zdaughters;
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//  GEN SELECTION

inline bool GenSelected(const reco::CompositeCandidate ZGEN, string selections){
     
  if(selections=="VPJ")return ZGEN.mass() > zmassmin_vpj && ZGEN.mass() < zmassmax_vpj;
  if(selections=="VBTF")return ZGEN.mass() > zmassmin_vbtf && ZGEN.mass() < zmassmax_vbtf;
  
  else return false;
}

inline bool GenSelectedInAcceptance(const reco::CompositeCandidate ZGEN, string selections){
  
  std::vector<const reco::Candidate*> zgendaughters = ZGENDaughters(ZGEN);
  const reco::Candidate *dau0 = 0;
  const reco::Candidate *dau1 = 0;
  
  if(zgendaughters.size() != 0){      
    
  dau0 = zgendaughters[0];
  dau1 = zgendaughters[1];
  
  if(selections=="VPJ"){
  return ZGEN.mass() > zmassmin_vpj && ZGEN.mass() < zmassmax_vpj 
         && dau0->pt() > ptelcut && fabs(dau0->eta()) < etaelcut
         && dau1->pt() > ptelcut && fabs(dau1->eta()) < etaelcut
         && (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) && 
	    (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
 }else if(selections=="VBTF"){
 return ZGEN.mass() > zmassmin_vbtf && ZGEN.mass() < zmassmax_vbtf 
         && dau0->pt() > ptelcut0 && fabs(dau0->eta()) < etaelcut
         && dau1->pt() > ptelcut1 && fabs(dau1->eta()) < etaelcut
         && (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) && 
	    (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
 }else{
 return false;
 }
 }else{
 return false;
 }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// REC SELECTION
  
inline bool RecSelected(string Flag, const reco::CompositeCandidate ZREC, const pat::TriggerEvent& triggers){
  
  std::vector<const pat::Electron*> zdaughters = ZRECDaughters(ZREC);
  
  if(!zdaughters.size()){
  return false;
  }else{
  
  const pat::Electron* dau0 = zdaughters[0];
  const pat::Electron* dau1 = zdaughters[1];
  
  const reco::GsfTrackRef track0 = dau0->gsfTrack();
  const reco::GsfTrackRef track1 = dau1->gsfTrack();
  assert (track0.isNonnull() && track1.isNonnull());		
  
  bool iso0 = false;	
  bool iso1 = false;
			
  bool electron_ID0 = false;
  bool electron_ID1 = false;

  if(Flag=="_AccVPJ"){
  return ZREC.mass()>zmassmin_vpj && ZREC.mass()<zmassmax_vpj
         && dau0->pt() > ptelcut && fabs(dau0->eta()) < etaelcut 
         && dau1->pt() > ptelcut && fabs(dau1->eta()) < etaelcut
         && (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) &&
            (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
         }
  else if(Flag=="_AccVBTF"){
  return ZREC.mass()>zmassmin_vbtf && ZREC.mass()<zmassmax_vbtf
         && dau0->pt() > ptelcut0 && fabs(dau0->eta()) < etaelcut 
         && dau1->pt() > ptelcut1 && fabs(dau1->eta()) < etaelcut
         && (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) &&
            (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
         }
  else if(Flag=="_Trg"){
  return isElectronTriggered(triggers);
         }
  else if(Flag=="_Qual"){
  return track0->numberOfValidHits() > minnhit && track1->numberOfValidHits() > minnhit &&
         track0->normalizedChi2() < maxchi2 && track1->normalizedChi2() < maxchi2;
         }  
  else if(Flag=="_Imp"){
  return dau0->dB() < dxycut && dau1->dB() < dxycut;
         }
  else if(Flag=="_IsoVPJ"){
  return (dau0->hcalIso() + dau0->ecalIso() + dau0->trackIso()) / dau0->pt() < isocut &&  
         (dau1->hcalIso() + dau1->ecalIso() + dau1->trackIso()) / dau1->pt() < isocut;
         }
  else if(Flag=="_IsoVBTF"){
	if(fabs(dau0->eta())<eta_el_excl_down){
	iso0 = ((dau0->dr03TkSumPt() + max(0., dau0->dr03EcalRecHitSumEt() - 1.) + dau0->dr03HcalTowerSumEt() ) / dau0->p4().Pt()) < Dau0_CombIso_EB;
	}else if(fabs(dau0->eta())>eta_el_excl_up){
	iso0 = ((dau0->dr03TkSumPt() + dau0->dr03EcalRecHitSumEt() + dau0->dr03HcalTowerSumEt() ) / dau0->p4().Pt()) < Dau0_CombIso_EE;
	}
	if(fabs(dau1->eta())<eta_el_excl_down){
	iso1 = ((dau1->dr03TkSumPt() + max(0., dau1->dr03EcalRecHitSumEt() - 1.) + dau1->dr03HcalTowerSumEt() ) / dau1->p4().Pt()) < Dau1_CombIso_EB;
	}else if(fabs(dau1->eta())>eta_el_excl_up){
	iso1 = ((dau1->dr03TkSumPt() + dau1->dr03EcalRecHitSumEt() + dau1->dr03HcalTowerSumEt() ) / dau1->p4().Pt()) < Dau1_CombIso_EE;
	}	
  return iso0 && iso1;         
  }
  else if(Flag=="_EiDVPJ"){
  if(dau0->isElectronIDAvailable(eID_VPJ.c_str()) && dau1->isElectronIDAvailable(eID_VPJ.c_str())){
  if(dau0->electronID(eID_VPJ.c_str())==1.0)electron_ID0 = true;
  if(dau1->electronID(eID_VPJ.c_str())==1.0)electron_ID1 = true;}
  return electron_ID0 && electron_ID1;
         }
  else if(Flag=="_EiDVBTF"){
	if(fabs(dau0->eta())<eta_el_excl_down){
	electron_ID0 = 
		 dau0->sigmaIetaIeta() < Dau0_sihih_EB
        	 && dau0->deltaPhiSuperClusterTrackAtVtx() < Dau0_Dphi_vtx_EB
        	 && dau0->deltaEtaSuperClusterTrackAtVtx() < Dau0_Deta_vtx_EB
		 && dau0->hcalOverEcal() < Dau0_HovE_EB;
	}else if(fabs(dau0->eta())>eta_el_excl_up){
	electron_ID0 = 
		 dau0->sigmaIetaIeta() < Dau0_sihih_EE
        	 && dau0->deltaPhiSuperClusterTrackAtVtx() < Dau0_Dphi_vtx_EE
        	 && dau0->deltaEtaSuperClusterTrackAtVtx() < Dau0_Deta_vtx_EE
		 && dau0->hcalOverEcal() < Dau0_HovE_EE;
	}
	if(fabs(dau1->eta())<eta_el_excl_down){
	electron_ID1 = 
		 dau1->sigmaIetaIeta() < Dau1_sihih_EB
        	 && dau1->deltaPhiSuperClusterTrackAtVtx() < Dau1_Dphi_vtx_EB
        	 && dau1->deltaEtaSuperClusterTrackAtVtx() < Dau1_Deta_vtx_EB
		 && dau1->hcalOverEcal() < Dau1_HovE_EB;
	}else if(fabs(dau1->eta())>eta_el_excl_up){	
	electron_ID1 = 
		 dau1->sigmaIetaIeta() < Dau1_sihih_EE
        	 && dau1->deltaPhiSuperClusterTrackAtVtx() < Dau1_Dphi_vtx_EE
        	 && dau1->deltaEtaSuperClusterTrackAtVtx() < Dau1_Deta_vtx_EE
		 && dau1->hcalOverEcal() < Dau1_HovE_EE;
	}
  return electron_ID0 && electron_ID1;
  }
  else if(Flag=="_1"){
  return true;
  }
  else{
  return false;
  }
  
  }
  
}

inline bool RecSelected_TrgMatch(const pat::Electron& Electron){

const TriggerObjectStandAloneCollection MatchElectron = Electron.triggerObjectMatches();
    
if(MatchElectron.size()){
return true;
}else{
return false;
}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Geometrical Methods

template<class EL> double Delta_Phi(const EL& Electron, const reco::Jet& jet){
	
	double deltaPhi = 0;

	if((TMath::Abs(Electron.phi() - jet.phi()) < TMath::Pi()) && (TMath::Abs(Electron.phi() - jet.phi()) > -(TMath::Pi()))){
	deltaPhi = Electron.phi() - jet.phi();
	}
	
	if((Electron.phi() - jet.phi()) > TMath::Pi() ){
	deltaPhi = (2*TMath::Pi()) - (Electron.phi() - jet.phi());
	}
	
        if((Electron.phi() - jet.phi()) < -(TMath::Pi())){
        deltaPhi = (2*TMath::Pi()) + (Electron.phi() - jet.phi());
	}

	return deltaPhi;
	
}

template<class EL> double Delta_Eta(const EL& Electron, const reco::Jet& jet){

	double deltaEta = Electron.eta() - jet.eta();
	
	return deltaEta;
	
}

template<class EL> double Delta_R(const EL& Electron, const reco::Jet& jet){

	double deltaR = sqrt(pow(Delta_Phi<EL>(Electron,jet),2) + pow(Delta_Eta<EL>(Electron,jet),2));
	
	return deltaR;
	
}

inline double MinDeltaRZDau(const reco::CompositeCandidate Z, const reco::Jet& jet, std::string GENRECO){

        double minDeltaRZDau = -999999;
        
        const reco::Candidate *gendau0 = 0;
        const reco::Candidate *gendau1 = 0;
        const pat::Electron *recdau0 = 0;
        const pat::Electron *recdau1 = 0;
        
        std::vector<const reco::Candidate*> zgendaughters;
        std::vector<const pat::Electron*> zrecdaughters;
        
        if(GENRECO=="GEN")zgendaughters = ZGENDaughters(Z);
        if(GENRECO=="RECO")zrecdaughters = ZRECDaughters(Z);
        
        if(zgendaughters.size()){
        gendau0 = zgendaughters[0];
        gendau1 = zgendaughters[1];

	minDeltaRZDau = (Delta_R<reco::Candidate>(*gendau0,jet) < Delta_R<reco::Candidate>(*gendau1,jet)) ? Delta_R<reco::Candidate>(*gendau0,jet) : Delta_R<reco::Candidate>(*gendau1,jet);		
	}
	
	if(zrecdaughters.size()){
        recdau0 = zrecdaughters[0];
        recdau1 = zrecdaughters[1];

	minDeltaRZDau = (Delta_R<pat::Electron>(*recdau0,jet) < Delta_R<pat::Electron>(*recdau1,jet)) ? Delta_R<pat::Electron>(*recdau0,jet) : Delta_R<pat::Electron>(*recdau1,jet);		
	}
	
	return minDeltaRZDau;
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Jet Methods

template<class JET> std::vector<const JET*> GetJets(const std::vector<JET>& jets){
  std::vector<const JET*> selectedjets;
  for (unsigned int i = 0; i < jets.size();  ++i){
    if (jets[i].pt() > ptjetmin && fabs(jets[i].eta()) < etajetmax) selectedjets.push_back(&jets[i]);
  }
  return selectedjets;
}

template<class JET> double deltaYFwdBwd(const std::vector<const JET*>& jets){
  assert(jets.size()>1);
  double maxy = -999999;
  double miny =  999999;
  for (unsigned int i = 0; i < jets.size(); ++i){
    if (jets[i]->rapidity() > maxy){
      maxy = jets[i]->rapidity();
    }
    if (jets[i]->rapidity() < miny){
      miny = jets[i]->rapidity();
    }
  }
  return maxy-miny ;   
}

inline bool IsoJet(const reco::CompositeCandidate ZGEN, const reco::Jet& jet, string GENRECO){
	
	bool iso_jet = true;
	
	if(MinDeltaRZDau(ZGEN,jet,GENRECO) < isojetcut) iso_jet = false;
	
	return iso_jet;
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Histogram Methods - book histograms according to multiplicity

inline void addHistosVsMulti(unsigned int multi, std::string name, std::string title, int nbin, double min, double max, std::vector<TH1D*>& array){
  if (array.size() < multi+1){
    for (unsigned int i = array.size(); i < multi+1; ++i){
      std::stringstream fullname;
      fullname << name << i << "jets";
      std::stringstream fulltitle;
      std::string ext;
      if (i == 1) ext = "st";
      else if (i == 2) ext = "nd";
      else if (i == 3 ) ext = "rd";
      else ext = "th";
      fulltitle << i << "^{" << ext << "} jet " << title;

      TH1D* newhisto = new TH1D(fullname.str().c_str(), fulltitle.str().c_str(), nbin, min, max);
      newhisto->Sumw2();

      array.push_back(newhisto);

    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////


// Tag & Probe

//Conditions required to fill TagAndProbe
inline bool RecSelected_TagAndProbe(const reco::CompositeCandidate ZREC, string selections){
  if(selections=="VPJ")return ZREC.mass() > zmassmin_vpj && ZREC.mass() < zmassmax_vpj;
  if(selections=="VBTF")return ZREC.mass() > zmassmin_vbtf && ZREC.mass() < zmassmax_vbtf;
  else return false;
}

//Cuts applied on Tag electron
inline bool singleEl_Tag_VPJ(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);

  if(electron){
  const reco::GsfTrackRef track = electron->gsfTrack();
  assert(track.isNonnull());
  bool TAG_EiD = false;
  if(electron->isElectronIDAvailable(VPJ_TagEiD.c_str())){
  if(electron->electronID(VPJ_TagEiD.c_str())==1.0)TAG_EiD = true;
  }
  return electron->pt() > VPJ_TAG_ptelcut && 
  	 fabs(electron->eta()) < VPJ_TAG_etaelcut &&
         (fabs(electron->eta()) < VPJ_TAG_eta_el_excl_down || fabs(electron->eta()) > VPJ_TAG_eta_el_excl_up) &&
         track->numberOfValidHits() > VPJ_TAG_minnhit && 
         track->normalizedChi2() < VPJ_TAG_maxchi2 &&
         electron->dB() < VPJ_TAG_dxycut &&
         (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < VPJ_TAG_isocut &&  
         TAG_EiD;
 }else{
 return false;}
}

inline bool singleEl_Tag_VBTF0(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  const reco::GsfTrackRef track = electron->gsfTrack();
  assert(track.isNonnull());
  bool TAG_EiD = false;
  if(electron->isElectronIDAvailable(VBTF0_TagEiD.c_str())){
  if(electron->electronID(VBTF0_TagEiD.c_str())==1.0)TAG_EiD = true;
  }
  return electron->pt() > VBTF0_TAG_ptelcut && 
  	 fabs(electron->eta()) < VBTF0_TAG_etaelcut &&
         (fabs(electron->eta()) < VBTF0_TAG_eta_el_excl_down || fabs(electron->eta()) > VBTF0_TAG_eta_el_excl_up) &&
         track->numberOfValidHits() > VBTF0_TAG_minnhit && 
         track->normalizedChi2() < VBTF0_TAG_maxchi2 &&
         electron->dB() < VBTF0_TAG_dxycut &&
         (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < VBTF0_TAG_isocut &&  
         TAG_EiD;
 }else{
 return false;}
}

inline bool singleEl_Tag_VBTF1(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){ 
  const reco::GsfTrackRef track = electron->gsfTrack();
  assert(track.isNonnull());
  
  bool TAG_EiD = false;
  if(electron->isElectronIDAvailable(VBTF1_TagEiD.c_str())){
  if(electron->electronID(VBTF1_TagEiD.c_str())==1.0)TAG_EiD = true;
  }
  
  return electron->pt() > VBTF1_TAG_ptelcut && 
  	 fabs(electron->eta()) < VBTF1_TAG_etaelcut &&
         (fabs(electron->eta()) < VBTF1_TAG_eta_el_excl_down || fabs(electron->eta()) > VBTF1_TAG_eta_el_excl_up) &&
         track->numberOfValidHits() > VBTF1_TAG_minnhit && 
         track->normalizedChi2() < VBTF1_TAG_maxchi2 &&
         electron->dB() < VBTF1_TAG_dxycut &&
         (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < VBTF1_TAG_isocut &&  
         TAG_EiD;
 }else{
 return false;}
}

//Probe cuts VPJ
inline bool singleEl_Probe_Acc_VPJ(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  return (          
          electron->pt() > ptelcut 
          && fabs(electron->eta()) < etaelcut 
          && (fabs(electron->eta())<eta_el_excl_down || fabs(electron->eta())>eta_el_excl_up));
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_Qual(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  const reco::GsfTrackRef track = electron->gsfTrack();
  assert(track.isNonnull());
  return track->numberOfValidHits() > minnhit && track->normalizedChi2() < maxchi2;
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_Imp(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  return electron->dB() < dxycut;
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_Iso_VPJ(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  return (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < isocut;
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_EiD_VPJ(const reco::Candidate& cand){

  const pat::Electron* electron = CloneCandidate(cand);
  
  if(electron){
  bool electron_ID = false;
  if(electron->isElectronIDAvailable(VPJ_TagEiD.c_str())){
  if(electron->electronID(VPJ_TagEiD.c_str())==1.0)electron_ID = true;}
  return electron_ID;
  }else{
  return false;}
}
  
inline bool singleEl_Probe_True(const reco::Candidate& cand){
return true;
}

//Probe cuts VBTF
inline bool singleEl_Probe_Acc_VBTF0(const reco::Candidate& cand){

  const pat::Electron* el0 = CloneCandidate(cand);
  
  if(el0){
  return (          
          el0->pt() > ptelcut0
          && fabs(el0->eta()) < etaelcut 
          && (fabs(el0->eta())<eta_el_excl_down || fabs(el0->eta())>eta_el_excl_up));
  }else{
  return false;}
  }
  
inline bool singleEl_Probe_Acc_VBTF1(const reco::Candidate& cand){

  const pat::Electron* el1 = CloneCandidate(cand);
  
  if(el1){
  return (          
          el1->pt() > ptelcut1
          && fabs(el1->eta()) < etaelcut 
          && (fabs(el1->eta())<eta_el_excl_down || fabs(el1->eta())>eta_el_excl_up));
  }else{
  return false;}
  }

inline bool singleEl_Probe_Iso_VBTF0(const reco::Candidate& cand){

  const pat::Electron* el0 = CloneCandidate(cand);
  
  if(el0){ 
  if(fabs(el0->eta())<eta_el_excl_down){
  return(((el0->dr03TkSumPt() + max(0., el0->dr03EcalRecHitSumEt() - 1.) + el0->dr03HcalTowerSumEt() ) / el0->p4().Pt()) < Dau0_CombIso_EB);
  }else if(fabs(el0->eta())>eta_el_excl_up){
  return(((el0->dr03TkSumPt() + el0->dr03EcalRecHitSumEt() + el0->dr03HcalTowerSumEt() ) / el0->p4().Pt()) < Dau0_CombIso_EE);
  }
  else{
  return false;} 
  }else{
  return false;}
}

inline bool singleEl_Probe_Iso_VBTF1(const reco::Candidate& cand){

  const pat::Electron* el1 = CloneCandidate(cand);
  
  if(el1){ 
  if(fabs(el1->eta())<eta_el_excl_down){
  return(((el1->dr03TkSumPt() + max(0., el1->dr03EcalRecHitSumEt() - 1.) + el1->dr03HcalTowerSumEt() ) / el1->p4().Pt()) < Dau1_CombIso_EB);
  }else if(fabs(el1->eta())>eta_el_excl_up){
  return(((el1->dr03TkSumPt() + el1->dr03EcalRecHitSumEt() + el1->dr03HcalTowerSumEt() ) / el1->p4().Pt()) < Dau1_CombIso_EE);
  }
  else{
  return false;}
  }else{
  return false;}
}

inline bool singleEl_Probe_EiD_VBTF0(const reco::Candidate& cand){

  const pat::Electron* el0 = CloneCandidate(cand);
  
  if(el0){
  bool el0_ID = false; 
  if(fabs(el0->eta())<eta_el_excl_down){
	el0_ID = 
		 el0->sigmaIetaIeta() < Dau0_sihih_EB
        	 && el0->deltaPhiSuperClusterTrackAtVtx() < Dau0_Dphi_vtx_EB
        	 && el0->deltaEtaSuperClusterTrackAtVtx() < Dau0_Deta_vtx_EB
		 && el0->hcalOverEcal() < Dau0_HovE_EB;
  }else if(fabs(el0->eta())>eta_el_excl_up){
	el0_ID = 
		 el0->sigmaIetaIeta() < Dau0_sihih_EE
        	 && el0->deltaPhiSuperClusterTrackAtVtx() < Dau0_Dphi_vtx_EE
        	 && el0->deltaEtaSuperClusterTrackAtVtx() < Dau0_Deta_vtx_EE
		 && el0->hcalOverEcal() < Dau0_HovE_EE;
  }
  return el0_ID;
  }else{
  return false;}
}

inline bool singleEl_Probe_EiD_VBTF1(const reco::Candidate& cand){

  const pat::Electron* el1 = CloneCandidate(cand);
  
  if(el1){
  bool el1_ID = false; 
  if(fabs(el1->eta())<eta_el_excl_down){
	el1_ID = 
		 el1->sigmaIetaIeta() < Dau1_sihih_EB
        	 && el1->deltaPhiSuperClusterTrackAtVtx() < Dau1_Dphi_vtx_EB
        	 && el1->deltaEtaSuperClusterTrackAtVtx() < Dau1_Deta_vtx_EB
		 && el1->hcalOverEcal() < Dau1_HovE_EB;
  }else if(fabs(el1->eta())>eta_el_excl_up){	
	el1_ID = 
		 el1->sigmaIetaIeta() < Dau1_sihih_EE
        	 && el1->deltaPhiSuperClusterTrackAtVtx() < Dau1_Dphi_vtx_EE
        	 && el1->deltaEtaSuperClusterTrackAtVtx() < Dau1_Deta_vtx_EE
		 && el1->hcalOverEcal() < Dau1_HovE_EE;
  }
  return el1_ID;
  }else{
  return false;}
}

    
#endif
