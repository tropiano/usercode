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
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <vector>
#include <map>
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
//https://twiki.cern.ch/twiki/bin/view/CMS/HEEPElectronID

/////// ELECTRON TRIGGER ///////

// Trigger flags
static bool elTrgMatchReq = true;
static bool TrgRange = false;

static map<std::string, std::pair<int, int> > elTrigger() {

	typedef std::pair<int, int> rrange;

	rrange rr1, rr2, rr3, rr4, rr5, rr6, rr7, rr8, rr9, rr10, rr11, rr12, rr13, rr14;
	rr1 = make_pair(136033, 139980);
	rr2 = make_pair(140058, 141882);
	rr3 = make_pair(141956, 144114);
	rr4 = make_pair(146428, 147116);
	rr5 = make_pair(147196, 148058);
	rr6 = make_pair(148819, 149064);
	rr7 = make_pair(149181, 149442);
	rr8 = make_pair(149181, 149442);
	rr9 = make_pair(149181, 149442);
	rr10 = make_pair(149181, 149442);
	rr11 = make_pair(149181, 149442);
	rr12 = make_pair(149181, 149442);
	rr13 = make_pair(149181, 149442);
	rr14 = make_pair(149181, 149442);

	if(TrgRange == false){
		rr1 = make_pair(-1, 999999999);
		rr2 = make_pair(-1, 999999999);
		rr3 = make_pair(-1, 999999999);
		rr4 = make_pair(-1, 999999999);
		rr5 = make_pair(-1, 999999999);
		rr6 = make_pair(-1, 999999999);
		rr7 = make_pair(-1, 999999999);
		rr8 = make_pair(-1, 999999999);
		rr9 = make_pair(-1, 999999999);
		rr10 = make_pair(-1, 999999999);
		rr11 = make_pair(-1, 999999999);
		rr12 = make_pair(-1, 999999999);
		rr13 = make_pair(-1, 999999999);
		rr14 = make_pair(-1, 999999999);
	}

	static map<std::string, rrange > TrgVector;

	TrgVector["HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v"] = rr1;
	TrgVector["HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v"] = rr2;
	TrgVector["HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"] = rr3;
	TrgVector["HLT_L1_SingleEG5_v"] = rr4;
	TrgVector["HLT_L1_SingleEG12_v"] = rr5;
	TrgVector["HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_v"] = rr6;
	TrgVector["HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v"] = rr7;
	//Changes for photons studies 
	TrgVector["HLT_Photon50_CaloIdVL_IsoL_v"] = rr8;
	TrgVector["HLT_Photon75_CaloIdVL_v"] = rr9;
	TrgVector["HLT_Photon90_CaloIdVL_v"] = rr10;
	TrgVector["HLT_Photon90_CaloIdVL_IsoL_v"] = rr11;
	TrgVector["HLT_Photon125_v"] = rr12;
	TrgVector["HLT_Photon135_v"] = rr13;
	TrgVector["HLT_"] = rr14; //qualunque HLT

	return TrgVector;
}

////////////////////////////////


/////// JET TRIGGER ///////

static string JetTrigger = "HLT_Jet30U";

///////////////////////////


/////// COMMON CUTS ///////

//Electrons
static double etaelcut = 2.4;
//static double eta_el_excl_up = 1.566;               //Excluded Eta region
//static double eta_el_excl_down = 1.4442;           //Excluded Eta region
static double dxycut = 0.02;     //cm

//Jets
static bool   JetIDReq = true;
static double ptjetmin = 50.;   //Gev/c
static double etajetmax = 2.5;
static double isojetcut = 0.4; //Isolation jet - Z electron

// rho correction variables
static float cAecalEB = 0.101;
static float cAecalEE = 0.046;
static float cAhcalHE = 0.021;
static float cAhcalEE = 0.040;

// rho25 correction variables
static float cTrack_A = 2.0;
static float cTrack_B = 0.001;
static float cTrack_C_EB = 0.0167;
static float cTrack_C_EE = 0.032;

static float cECAL_A = 4.2;
static float cECAL_B = 0.006;
static float cECAL_C_EB = 0.183;
static float cECAL_C_EE = 0.090;

static float cHCAL_A = 2.2;
static float cHCAL_B = 0.0025;
static float cHCAL_C_EB = 0.062;
static float cHCAL_C_EE = 0.180;


///////////////////////////


/////// SYM CUTS ///////

// SYM Kinematic cuts
static double ptelcut = 20.;    //Gev/c
static double zmassmin_sym = 71.;   //Gev/c^2
static double zmassmax_sym = 111.;  //Gev/c^2

// SYM Quality cuts
static double maxchi2_SYM = 3.;
static double minVaHit_SYM = 15.;

// SYM Electron Isolation (with rho) variables
static double electronCombinedIsoRhoCut_SYM = 0.15;
static double electronCombinedIsoCut_SYM = 0.15;


// SYM Electron Identification Based on Simple Cuts (Relative or Combined Iso)
//https://twiki.cern.ch/twiki/bin/view/CMS/VbtfEleID2011
static string ElID_End_SYM = "simpleEleId90cIso";
static string ElID_Bar_SYM = "simpleEleId95cIso";

// SYM Electron Identification Basic Cuts
static float sigmaIEtaIEta_EB_H_SYM = 0.011;
static float sigmaIEtaIEta_EB_L_SYM = 0.001;
//static float sigmaIPhiIPhi_EB_L_SYM = 0.001;
static float HoverE_EB_SYM = 0.05;

static float sigmaIEtaIEta_EE_SYM = 0.03;
static float HoverE_EE_SYM = 0.05;


//SYM Electron Trigger
//leg 8 GeV for May10-2011Av4-Aug5
static string filter = "hltEle17CaloIdIsoEle8CaloIdIsoPixelMatchDoubleFilter"; 
//leg 17 GeV for May10-2011Av4                                                                                                      
//static string filter = "hltEle17CaloIdLCaloIsoVLPixelMatchFilter";
//leg 17 GeV for Aug5
//static string filter = "hltEle17CaloIdLCaloIsoVLPixelMatchFilterDoubleEG125";
//leg 17 for 2011Av6-2011B
//static string filter = "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter";
//leg 8 GeV for 2011Av6-2011B
//static string filter = "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter";

//hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter



// SYM Tag cuts
static double SYM_TAG_ptelcut = 20.;    //Gev/c
static double SYM_TAG_etaelcut = 2.4;
//static double SYM_TAG_eta_el_excl_up = 1.566;               //Excluded Eta region
//static double SYM_TAG_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double SYM_TAG_dxycut = 0.05;     //cm
static double SYM_TAG_maxchi2 = 3.;
static double SYM_TAG_minVaHit = 15.;
static double SYM_TAG_electronCombinedIsoRhoCut = 0.2;
//static string SYM_TAG_ElID = ""; //if "" no ElID cut applied


// SYM Probe cuts
static double SYM_PRO_ptelcut = 100.;    //Gev/c
static double SYM_PRO_etaelcut = 2.4;
//static double SYM_PRO_eta_el_excl_up = 1.566;               //Excluded Eta region
//static double SYM_PRO_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double SYM_PRO_dxycut = 0.02;     //cm
static double SYM_PRO_maxchi2 = 3.;
static double SYM_PRO_minVaHit = 15.;
static double SYM_PRO_electronCombinedIsoRhoCut = 0.15;
//static string SYM_PRO_ElID = ""; //if "" no ElID cut applied
////////////////////////


/////// ASYM CUTS ///////

// ASYM Kinematic cuts
static double ptelcut0 = 20.;    //Gev/c
static double ptelcut1 = 10.;
static double zmassmin_asym = 71.;   //Gev/c^2
static double zmassmax_asym = 111.;  //Gev/c^2

// ASYM Quality cuts
static double maxchi2_ASYM0 = 3.;
static double minVaHit_ASYM0 = 15.;

static double maxchi2_ASYM1 = 3.;
static double minVaHit_ASYM1 = 15.;

// ASYM Electron Isolation (with rho) variables
static double electronCombinedIsoRhoCut_ASYM0 = 0.15;
static double electronCombinedIsoCut_ASYM0 = 0.15;

static double electronCombinedIsoRhoCut_ASYM1 = 0.15;
static double electronCombinedIsoCut_ASYM1 = 0.15;


// ASYM Electron Identification Based on Simple Cuts (Relative or Combined Iso)
static string ElID_End_ASYM0 = "simpleEleId90cIso";
static string ElID_Bar_ASYM0 = "simpleEleId95cIso";

static string ElID_End_ASYM1 = "simpleEleId90cIso";
static string ElID_Bar_ASYM1 = "simpleEleId95cIso";

// ASYM Electron Identification Basic Cuts
static float sigmaIEtaIEta_EB_H_ASYM0 = 0.011;
static float sigmaIEtaIEta_EB_L_ASYM0 = 0.001;
//static float sigmaIPhiIPhi_EB_L_ASYM0 = 0.001;
static float HoverE_EB_ASYM0 = 0.05;

static float sigmaIEtaIEta_EE_ASYM0 = 0.03;
static float HoverE_EE_ASYM0 = 0.05;

static float sigmaIEtaIEta_EB_H_ASYM1 = 0.011;
static float sigmaIEtaIEta_EB_L_ASYM1 = 0.001;
//static float sigmaIPhiIPhi_EB_L_ASYM1 = 0.001;
static float HoverE_EB_ASYM1 = 0.05;

static float sigmaIEtaIEta_EE_ASYM1 = 0.03;
static float HoverE_EE_ASYM1 = 0.05;


// ASYM0 Tag cuts (for soft electron probe)
static double ASYM0_TAG_ptelcut = 20.;    //Gev/c
static double ASYM0_TAG_etaelcut = 2.4;
//static double ASYM0_TAG_eta_el_excl_up = 1.566;               //Excluded Eta region
//static double ASYM0_TAG_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double ASYM0_TAG_minVaHit = 0.;
static double ASYM0_TAG_maxchi2 = 9999.;
static double ASYM0_TAG_dxycut = 0.05;     //cm
static double ASYM0_TAG_electronCombinedIsoRhoCut = 0.2;     //CombRelIso
//static double ASYM0_TAG_electronCombinedIsoCut = 0.2;
//static string ASYM0_TAG_ElID = ""; //if "" no ElID cut applied

// ASYM1 Tag cuts (for hard electron probe)
static double ASYM1_TAG_ptelcut = 20.;    //Gev/c
static double ASYM1_TAG_etaelcut = 2.4;
//static double ASYM1_TAG_eta_el_excl_up = 1.566;               //Excluded Eta region
//static double ASYM1_TAG_eta_el_excl_down = 1.4442;            //Excluded Eta region
static double ASYM1_TAG_minVaHit = 0.;
static double ASYM1_TAG_maxchi2 = 9999.;
static double ASYM1_TAG_dxycut = 0.05;     //cm
static double ASYM1_TAG_electronCombinedIsoRhoCut = 0.2;       //CombRelIso
//static double ASYM1_TAG_electronCombinedIsoCut = 0.2;
//static string ASYM1_TAG_ElID = ""; //if "" no ElID cut applied


// ASYM0 Probe cuts
static double ASYM0_PRO_ptelcut = 100.;    //Gev/c
static double ASYM0_PRO_etaelcut = 2.4;
//static double ASYM0_PRO_eta_el_excl_up = 1.566;               //Excluded Eta region
//static double ASYM0_PRO_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double ASYM0_PRO_dxycut = 0.02;     //cm
static double ASYM0_PRO_maxchi2 = 3.;
static double ASYM0_PRO_minVaHit = 15.;
static double ASYM0_PRO_electronCombinedIsoRhoCut = 0.15;
//static string ASYM0_PRO_ElID = ""; //if "" no ElID cut applied

// ASYM1 Probe cuts
static double ASYM1_PRO_ptelcut = 100.;    //Gev/c
static double ASYM1_PRO_etaelcut = 2.4;
//static double ASYM1_PRO_eta_el_excl_up = 1.566;               //Excluded Eta region
//static double ASYM1_PRO_eta_el_excl_down = 1.4442;           //Excluded Eta region
static double ASYM1_PRO_dxycut = 0.02;     //cm
static double ASYM1_PRO_maxchi2 = 3.;
static double ASYM1_PRO_minVaHit = 15.;
static double ASYM1_PRO_electronCombinedIsoRhoCut = 0.15;
//static string ASYM1_PRO_ElID = ""; //if "" no ElID cut applied

/////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Z Daughters Methods


// Class to sort two object by pt
template <class T> bool sortByPt(const T* j1, const T* j2){ return j1->pt() > j2->pt() ; }


// Clone Candidate: it returns the pat candidate casting a reco Candidate or its Shallow Clone
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


// it returns a vector of 2 Electrons from a GEN Z sorted by pt. It require that almost a Z exists 
inline std::vector<const pat::Electron*> ZGENDaughters(const std::vector<reco::CompositeCandidate>& ZGEN){

	std::vector<const pat::Electron*> zdaughters;

	const pat::Electron* dau0 = CloneCandidate(*(ZGEN[0].daughter(0)));
	const pat::Electron* dau1 = CloneCandidate(*(ZGEN[0].daughter(1)));

	const pat::Electron* finaldau0 = 0; 
	const pat::Electron* finaldau1 = 0; 

	if (dau0 && dau1){

	if (dau0->numberOfDaughters()){
		bool el0set = false;
		for (unsigned int i = 0; i < dau0->numberOfDaughters(); ++i ){
			if (fabs(dau0->daughter(i)->pdgId()) == 11){
				if (el0set) {
					std::cout << "something wrong in GenSelectedInAcceptance: a daughter electron was already found for dau0 " << std::endl; 
				}
				finaldau0 = CloneCandidate(*(dau0->daughter(i)));
				el0set = true;
			}
		}
	}
	else{finaldau0 = dau0;}

	if (dau1->numberOfDaughters()){
		bool el1set = false;
		for (unsigned int i = 0; i < dau1->numberOfDaughters(); ++i ){
			if (fabs(dau1->daughter(i)->pdgId()) == 11){
				if (el1set) {
					std::cout << "something wrong in GenSelectedInAcceptance: a daughter electron was already found for dau1 " << std::endl;
				}
				finaldau1 = CloneCandidate(*(dau1->daughter(i)));
				el1set = true;
			}
		}
	}
	else{finaldau1 = dau1;}
 
	const pat::Electron* leading = finaldau0->pt() > finaldau1->pt() ? finaldau0 : finaldau1;
	const pat::Electron* second  = finaldau0->pt() > finaldau1->pt() ? finaldau1 : finaldau0;
	
	zdaughters.push_back(leading);
	zdaughters.push_back(second);
	}
	if(zdaughters.size()==0 || zdaughters.size()==1 || zdaughters.size()>2){
		std::cout << "ERROR! Wrong Z daughters association. Z daughters number = "<<zdaughters.size()<<std::endl;
		std::vector<const pat::Electron*> nullzdaughters;
		return nullzdaughters;
	}
	else{
		return zdaughters;
	}
}


// it returns a vector of 2 Electrons from a REC Z sorted by pt
inline std::vector<const pat::Electron*> ZRECDaughters(const std::vector<reco::CompositeCandidate>& ZREC){

	std::vector<const pat::Electron*> zdaughters;
 
	const pat::Electron* dau0 = CloneCandidate(*(ZREC[0].daughter(0)));
	const pat::Electron* dau1 = CloneCandidate(*(ZREC[0].daughter(1)));
	
	const pat::Electron* leading = 0;
	const pat::Electron* second = 0;
	
	if(dau0 && dau1){ 
		leading = dau0->pt() > dau1->pt() ? dau0 : dau1;
		second  = dau0->pt() > dau1->pt() ? dau1 : dau0;
	}
 
	zdaughters.push_back(leading);
	zdaughters.push_back(second);
	
	return zdaughters;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////


// GEN SELECTION

// GEN Z Candidate: it is true if there is 1 GEN Z with Mass cut
inline bool GenSelected(const std::vector<reco::CompositeCandidate>& ZGEN, string selections){
 
	if(selections=="SYM")
		return ZGEN[0].mass() > zmassmin_sym && 
		       ZGEN[0].mass() < zmassmax_sym;
	if(selections=="ASYM")
		return ZGEN[0].mass() > zmassmin_asym && 
		       ZGEN[0].mass() < zmassmax_asym;

	else return false;
}


// GEN Z Candidate: it is true if there is 1 GEN Z with Mass cut decaying in two GEN Electrons in the Acceptance (geometrical and kinematic) region
inline bool GenSelectedInAcceptance(const std::vector<reco::CompositeCandidate>& ZGEN, string selections){

	if (ZGEN.size() == 0) 
		return false;
	if (ZGEN.size() > 1){
		std::cout << "ERROR! Multiple Gen Z candidates found, you have to choose one before arriving here! " << std::endl;
		throw cms::Exception("PATAnalysis:RecSelectedTwoElectronsOppositeCharge_Mass") << "ERROR! Multiple Z candidates found, you have to choose one before arriving here! ";
		return false;
	}

	std::vector<const pat::Electron*> zgendaughters = ZGENDaughters(ZGEN);

	const pat::Electron *dau0 = 0;
	const pat::Electron *dau1 = 0;

	if(zgendaughters.size() != 0){
 
		dau0 = zgendaughters[0];
		dau1 = zgendaughters[1];

		if(selections=="SYM"){
			return ZGEN[0].mass() > zmassmin_sym && ZGEN[0].mass() < zmassmax_sym && 
			       dau0->pt() >= ptelcut && fabs(dau0->eta()) <= etaelcut &&
			       dau1->pt() >= ptelcut && fabs(dau1->eta()) <= etaelcut &&
			       !(dau0->isEBEEGap()) && !(dau1->isEBEEGap());
/*			       (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) && 
			       (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
*/
		}
		else if(selections=="ASYM"){
			return ZGEN[0].mass() > zmassmin_asym && ZGEN[0].mass() < zmassmax_asym &&
			       dau0->pt() >= ptelcut0 && fabs(dau0->eta()) <= etaelcut &&
			       dau1->pt() >= ptelcut1 && fabs(dau1->eta()) <= etaelcut &&
			       !(dau0->isEBEEGap()) && !(dau1->isEBEEGap());
/*			       (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) && 
			       (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
*/
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

// Single Electron Candidate: it is true if the Electron is in the Acceptance (geometrical and kinematic) region (SYM)
inline bool SingleEl_GenSelectedInAcceptance_SYM(const pat::Electron* el){
	return el->pt() > ptelcut && fabs(el->eta()) < etaelcut &&
	       !(el->isEBEEGap()) && !(el->isEBEEGap());
	       //(fabs(el->eta())<eta_el_excl_down || fabs(el->eta())>eta_el_excl_up);
}


// Single Electron Candidate: it is true if the Electron is in the Acceptance (geometrical and kinematic) region (ASYM, leg 0)
inline bool SingleEl_GenSelectedInAcceptance_ASYM0(const pat::Electron* el){
	return el->pt() > ptelcut0 && fabs(el->eta()) < etaelcut &&
	       !(el->isEBEEGap()) && !(el->isEBEEGap());
	       //(fabs(el->eta())<eta_el_excl_down || fabs(el->eta())>eta_el_excl_up);
}


// Single Electron Candidate: it is true if the Electron is in the Acceptance (geometrical and kinematic) region (ASYM, leg 1)
inline bool SingleEl_GenSelectedInAcceptance_ASYM1(const pat::Electron* el){
	return el->pt() > ptelcut1 && fabs(el->eta()) < etaelcut &&
	       !(el->isEBEEGap()) && !(el->isEBEEGap());
	       //(fabs(el->eta())<eta_el_excl_down || fabs(el->eta())>eta_el_excl_up);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Trigger

// Wild Card string Controller: it is true if the specified substring is inside the string in analysis
inline bool in_quote(std::string quote, const std::string& s){
	typedef string::const_iterator SCI;
	SCI p = search( quote.begin(), quote.end(), s.begin(), s.end() );
	return p!=quote.end();
}


// Trigger Available: it is true if the specified trigger string is found in the collection and if the trigger name that contain it was run
inline bool isTriggerAvailable(const pat::TriggerEvent& triggers, std::string triggername){

	const pat::TriggerPathCollection * paths = triggers.paths();
	bool TrgAv = false;

	if (!paths) {
		std::cout << "ERROR! trigger paths not found "<< std::endl;
		throw cms::Exception("PATAnalysis:isTriggered") << "ERROR! trigger paths not found ";
		return false;
	}
	for(pat::TriggerPathCollection::const_iterator ipath = paths->begin(); ipath != paths->end(); ++ipath){ 
		bool control = in_quote(ipath->name(), triggername);
		if(control){
			if(ipath->wasRun())TrgAv=true;
		}
	}
	return TrgAv;
}
/*old
inline bool isTriggerAvailable(const pat::TriggerEvent& triggers, std::string triggername){
	const pat::TriggerPath* path = triggers.path(triggername);
	if(!path){
		return false;
	}
	else{
		return true;
	}
}
*/


// Electron Trigger Available: it is true if the trigger (existing in a run range) was run in the specified run or in the complete run set
inline bool isElTriggerAvailable(const pat::TriggerEvent& triggers, int run){

	bool Triggered = false;
	static map<std::string, std::pair<int, int> > TrgVector = elTrigger();
	static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;

	for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
		if(run!=-1){
			if(isTriggerAvailable(triggers, TrgVectorIter->first.c_str()) && 
			  (TrgVectorIter->second.first <= run && run <= TrgVectorIter->second.second)){
				Triggered = true;
			}
		}
		else{
			if(isTriggerAvailable(triggers, TrgVectorIter->first.c_str())){
				Triggered = true;
			}
		}
	}
	return Triggered;
}
/*old
inline bool isElTriggerAvailable(const pat::TriggerEvent& triggers, int run){
	bool Triggered = false;
	static map<std::string, std::pair<int, int> > TrgVector = elTrigger();
	static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
	for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
		const pat::TriggerPath* path = triggers.path(TrgVectorIter->first.c_str());
		if(run!=-1){
			if(path && 
			   (TrgVectorIter->second.first<=run && run<=TrgVectorIter->second.second)){
				Triggered = true;
			}
		}
		else{
			if(path){
				Triggered = true;
			}
		}
	}
	return Triggered;
}
*/


// is Triggered: it is true if the specified trigger string is found in the collection and if the trigger name that contain it was Accepted
inline bool isTriggered(const pat::TriggerEvent& triggers, std::string triggername){

	const pat::TriggerPathCollection * paths = triggers.paths(); 
	bool TrgFired = false;

	if (!paths) {
		std::cout << "ERROR! trigger paths not found "<< std::endl;
		throw cms::Exception("PATAnalysis:isTriggered") << "ERROR! trigger paths not found ";
		return false;
	}
	for(pat::TriggerPathCollection::const_iterator ipath = paths->begin(); ipath != paths->end(); ++ipath){ 
		bool control = in_quote(ipath->name(), triggername);
		if(control){
			if(ipath->wasAccept()){
				TrgFired=true;
			}
		}
	}
	return TrgFired;
}
/*old
inline bool isTriggered(const pat::TriggerEvent& triggers, std::string triggername){
	const pat::TriggerPath* path = triggers.path(triggername);
	if (!path) {
		return false;
	}
	return path->wasAccept();
}
*/


// is Electron Triggered: it is true if the trigger (existing in a run range) was Accepted in the specified run or in the complete run set
inline bool isElectronTriggered(const pat::TriggerEvent& triggers, int run){

	bool Triggered = false;
	static map<std::string, std::pair<int, int> > TrgVector = elTrigger();
	static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;

	for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
		if(run!=-1){
			if(isTriggered(triggers, TrgVectorIter->first.c_str()) && 
			   (TrgVectorIter->second.first <= run && run <= TrgVectorIter->second.second)){
				Triggered = true;
			}	
		}
		else{
			if(isTriggered(triggers, TrgVectorIter->first.c_str())){
				Triggered = true;
			}
		}
	}
	return Triggered;
}
/*old
inline bool isElectronTriggered(const pat::TriggerEvent& triggers, int run){
	bool Triggered = false;
	static map<std::string, std::pair<int, int> > TrgVector = elTrigger();
	static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
	for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
		if(run!=-1){
			if(isTriggered(triggers, TrgVectorIter->first.c_str()) && 
			   (TrgVectorIter->second.first<=run && run<=TrgVectorIter->second.second)){
				Triggered = true;
			}
		}
		else{
			if(isTriggered(triggers, TrgVectorIter->first.c_str())){
				Triggered = true;
			}
		}
	}
	return Triggered;
}
*/


// Trigger Matching: it is true if the trigger path matches one of the trigger in the collection for a specified run or in the complete run set
inline bool RecSelected_TrgMatch(const pat::Electron& Electron, int run){

	bool trigmatch = false;
/*	const pat::TriggerPathCollection * paths = triggers.paths(); 
	static map<std::string, std::pair<int, int> > TrgVector = elTrigger();
	static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;

	for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
		for(pat::TriggerPathCollection::const_iterator ipath = paths->begin(); ipath != paths->end(); ++ipath){ 
			if(in_quote(ipath->name(), TrgVectorIter->first.c_str())){

				//const TriggerObjectStandAloneCollection MatchElectron = Electron.triggerObjectMatchesByPath(ipath->name().c_str(), true);
				const TriggerObjectStandAloneCollection MatchElectron = Electron.triggerObjectMatches();
*/
	const TriggerObjectStandAloneCollection MatchElectron = Electron.triggerObjectMatchesByFilter(filter.c_str());

	/*				if(TrgRange==true && run!=-1){
	  if(MatchElectron.size() && 
	  (TrgVectorIter->second.first<=run && run<=TrgVectorIter->second.second)) {
	  trigmatch = true;
	  }
	  }
	  else{
	  if(MatchElectron.size()) {
						trigmatch = true;
						}
						}
						}
						}
						}
	*/
	if(MatchElectron.size()) trigmatch = true;
	std::cout<<"matched electrons size: "<<MatchElectron.size()<<std::endl;
	
	return trigmatch;
}
/*old
inline bool RecSelected_TrgMatch(const pat::Electron& Electron, int run){
	bool trigmatch = false;
	static map<std::string, std::pair<int, int> > TrgVector = elTrigger();
	static map<std::string, std::pair<int, int> >::iterator TrgVectorIter;
	for(TrgVectorIter = TrgVector.begin(); TrgVectorIter != TrgVector.end(); TrgVectorIter++){
		const TriggerObjectStandAloneCollection MatchElectron = Electron.triggerObjectMatchesByPath(TrgVectorIter->first.c_str(), true);
		if(run!=-1){
			if(MatchElectron.size() && 
			   (TrgVectorIter->second.first <= run && run <= TrgVectorIter->second.second) ){
				trigmatch = true;
			}
		}
		else{
			if(MatchElectron.size()){ 
				trigmatch = true;
			}
		}
	}
	return trigmatch;
}
*/


// Jet Trigger: it is true if the Jet is triggered
inline bool isJetTriggered(const pat::TriggerEvent& triggers){
	return isTriggered(triggers, JetTrigger.c_str());
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Z REC SELECTION

// REC Z Candidate: it is true if there is 1 REC Z with Mass cut decaying in two REC Electrons with these cuts: Acceptance (geometrical and kinematic), Quality, Impact Parameter, Isolation with rho.
inline bool RecSelected_ele(string Flag, const std::vector<reco::CompositeCandidate>& ZREC, const pat::TriggerEvent& triggers, int run, double rho){

	std::vector<const pat::Electron*> zdaughters;

	if(ZREC.size()!=0){
		zdaughters = ZRECDaughters(ZREC);
	}
	else{
		return false;
	}

	if(!zdaughters.size()){
		return false;
	}
	else{
		const pat::Electron* dau0 = zdaughters[0];
		const pat::Electron* dau1 = zdaughters[1];
  
		bool qual0 = false;
		bool qual1 = false;

		bool iso0 = false;	
		bool iso1 = false;

//		bool isoPhoton0 = false;	
//		bool isoPhoton1 = false;
			
		bool electron_ID0 = false;
		bool electron_ID1 = false;

//		bool electron_IDPhoton0 = false;
//		bool electron_IDPhoton1 = false;

		bool conv0 = false;
		bool conv1 = false;

		if(Flag=="_AccSYM"){
			return ZREC[0].mass()>zmassmin_sym && ZREC[0].mass()<zmassmax_sym &&
			       dau0->pt() >= ptelcut && fabs(dau0->eta()) <= etaelcut &&
			       dau1->pt() >= ptelcut && fabs(dau1->eta()) <= etaelcut &&
			       !(dau0->isEBEEGap()) && !(dau1->isEBEEGap());
/*			       (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) &&
			       (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
*/
		}


		else if(Flag=="_AccASYM"){
			return ZREC[0].mass()>zmassmin_asym && ZREC[0].mass()<zmassmax_asym &&
			       dau0->pt() >= ptelcut0 && fabs(dau0->eta()) <= etaelcut &&
			       dau1->pt() >= ptelcut1 && fabs(dau1->eta()) <= etaelcut &&
			       !(dau0->isEBEEGap()) && !(dau1->isEBEEGap());
/*			       (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) &&
			       (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
*/
		}

		else if(Flag=="_Trg"){
			bool cutTrg = false;
			if(elTrgMatchReq==true){
				if(isElectronTriggered(triggers, run)&&(RecSelected_TrgMatch(*dau0, run))){
					cutTrg=true;
				}
			}
			else if(elTrgMatchReq==false){
				if(isElectronTriggered(triggers, run)){
					cutTrg=true;
				}
			}
			return cutTrg;
		}

		else if(Flag=="_QualSYM"){

			const reco::GsfTrackRef track0 = dau0->gsfTrack();
			const reco::GsfTrackRef track1 = dau1->gsfTrack();
			assert (track0.isNonnull() && track1.isNonnull());

			if(track0->numberOfValidHits() > minVaHit_SYM && 
			   track0->normalizedChi2() < maxchi2_SYM){
				qual0 = true;
			}
			if(track1->numberOfValidHits() > minVaHit_SYM && 
			   track1->normalizedChi2() < maxchi2_SYM){
				qual1 = true;
			}
			return qual0 && qual1;
		} 

		else if(Flag=="_QualASYM"){

			const reco::GsfTrackRef track0 = dau0->gsfTrack();
			const reco::GsfTrackRef track1 = dau1->gsfTrack();
			assert (track0.isNonnull() && track1.isNonnull());


			if(track0->numberOfValidHits() > minVaHit_ASYM0 && 
			   track0->normalizedChi2() < maxchi2_ASYM0){
				qual0 = true;
			}
			if(track1->numberOfValidHits() > minVaHit_ASYM1 && 
			   track1->normalizedChi2() < maxchi2_ASYM1){
				qual1 = true;
			}
			return qual0 && qual1;
		}

		else if(Flag=="_ConvSYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
			if(dau0->isEB()){
				if(dau0->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
					if(dau0->electronID(ElID_Bar_SYM.c_str())==4.0 || 
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==5.0 || 
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==6.0 ||
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==7.0 ){
						conv0 = true;
					}
				}
			}
			else if(dau0->isEE()){
				if(dau0->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
					if(dau0->electronID(ElID_End_SYM.c_str())==4.0 || 
				   	   dau0->electronID(ElID_End_SYM.c_str())==5.0 || 
				   	   dau0->electronID(ElID_End_SYM.c_str())==6.0 ||
				   	   dau0->electronID(ElID_End_SYM.c_str())==7.0 ){
						conv0 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ConvSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(dau1->isEB()){
				if(dau1->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
					if(dau1->electronID(ElID_Bar_SYM.c_str())==4.0 || 
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==5.0 || 
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==6.0 ||
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==7.0 ){
						conv1 = true;
					}
				}
			}
			else if(dau1->isEE()){
				if(dau1->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
					if(dau1->electronID(ElID_End_SYM.c_str())==4.0 || 
				   	   dau1->electronID(ElID_End_SYM.c_str())==5.0 || 
				   	   dau1->electronID(ElID_End_SYM.c_str())==6.0 ||
				   	   dau1->electronID(ElID_End_SYM.c_str())==7.0 ){
						conv1 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ConvSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return conv0 && conv1;
		}
 	
		else if(Flag=="_ConvASYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedElElID
			if(dau0->isEB()){
				if(dau0->isElectronIDAvailable(ElID_Bar_ASYM0.c_str())){ 
					if(dau0->electronID(ElID_Bar_ASYM0.c_str())==4.0 || 
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==5.0 || 
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==6.0 ||
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==7.0 ){
						conv0 = true;
					}
				}
			}
			else if(dau0->isEE()){
				if(dau0->isElectronIDAvailable(ElID_End_ASYM0.c_str())){ 
					if(dau0->electronID(ElID_End_ASYM0.c_str())==4.0 || 
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==5.0 || 
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==6.0 ||
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==7.0 ){
						conv0 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ConvASYM0") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(dau1->isEB()){
				if(dau1->isElectronIDAvailable(ElID_Bar_ASYM1.c_str())){ 
					if(dau1->electronID(ElID_Bar_ASYM1.c_str())==4.0 || 
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==5.0 || 
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==6.0 ||
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==7.0 ){
						conv1 = true;
					}
				}
			}
			else if(dau1->isEE()){
				if(dau1->isElectronIDAvailable(ElID_End_ASYM1.c_str())){ 
					if(dau1->electronID(ElID_End_ASYM1.c_str())==4.0 || 
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==5.0 || 
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==6.0 ||
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==7.0 ){
						conv1 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ConvASYM1") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return conv0 && conv1;
		}

		else if(Flag=="_Imp"){
			return dau0->dB() < dxycut && dau1->dB() < dxycut;
		}

		else if(Flag=="_IsoSYM"){

			float trackIso0 = dau0->dr03TkSumPt();
			float trackIso1 = dau1->dr03TkSumPt();

			float ecalIso0 = dau0->dr03EcalRecHitSumEt();
			float ecalIso1 = dau1->dr03EcalRecHitSumEt();

			float hcalIso0 = dau0->dr03HcalTowerSumEt();
			float hcalIso1 = dau1->dr03HcalTowerSumEt();

			float combinedIso03Rho0, combinedIso03Rho1;

			if (dau0->isEB()){
				combinedIso03Rho0 = (trackIso0 + max(0. ,ecalIso0 - cAecalEB*(rho)) + max(0.,hcalIso0 - cAhcalHE*(rho)))/dau0->pt();
			}
			else if (dau0->isEE()){
				combinedIso03Rho0 = (trackIso0 + max(0. ,ecalIso0 - cAecalEE*(rho)) + max(0.,hcalIso0 - cAhcalEE*(rho)))/dau0->pt();
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				//throw cms::Exception("PATAnalysis:RecSelected, IsoSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
						
			if (dau1->isEB()){
				combinedIso03Rho1 = (trackIso1 + max(0. ,ecalIso1 - cAecalEB*(rho)) + max(0.,hcalIso1 - cAhcalHE*(rho)))/dau1->pt();
			}
			else if (dau1->isEE()){
				combinedIso03Rho1 = (trackIso1 + max(0. ,ecalIso1 - cAecalEE*(rho)) + max(0.,hcalIso1 - cAhcalEE*(rho)))/dau1->pt();
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				//throw cms::Exception("PATAnalysis:RecSelected, IsoSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(combinedIso03Rho0 <= electronCombinedIsoRhoCut_SYM) iso0 = true;
			if(combinedIso03Rho1 <= electronCombinedIsoRhoCut_SYM) iso1 = true;
 
			return iso0 && iso1;
		}

		else if(Flag=="_IsoASYM"){
 	
			float trackIso0 = dau0->dr03TkSumPt();
			float trackIso1 = dau1->dr03TkSumPt();

			float ecalIso0 = dau0->dr03EcalRecHitSumEt();
			float ecalIso1 = dau1->dr03EcalRecHitSumEt();

			float hcalIso0 = dau0->dr03HcalTowerSumEt();
			float hcalIso1 = dau1->dr03HcalTowerSumEt();

			float combinedIso03Rho0, combinedIso03Rho1;

			if (dau0->isEB()){
				combinedIso03Rho0 = (trackIso0 + max(0. ,ecalIso0 - cAecalEB*(rho)) + max(0.,hcalIso0 - cAhcalHE*(rho)))/dau0->pt();
			}
			else if (dau0->isEE()){
				combinedIso03Rho0 = (trackIso0 + max(0. ,ecalIso0 - cAecalEE*(rho)) + max(0.,hcalIso0 - cAhcalEE*(rho)))/dau0->pt();
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, IsoASYM0") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
						
			if (dau1->isEB()){
				combinedIso03Rho1 = (trackIso1 + max(0. ,ecalIso1 - cAecalEB*(rho)) + max(0.,hcalIso1 - cAhcalHE*(rho)))/dau1->pt();
			}
			else if (dau1->isEE()){
				combinedIso03Rho1 = (trackIso1 + max(0. ,ecalIso1 - cAecalEE*(rho)) + max(0.,hcalIso1 - cAhcalEE*(rho)))/dau1->pt();
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, IsoASYM1") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(combinedIso03Rho0 <= electronCombinedIsoRhoCut_ASYM0) iso0 = true;
			if(combinedIso03Rho1 <= electronCombinedIsoRhoCut_ASYM1) iso1 = true;
 
			return iso0 && iso1;
		}

/*
		else if(Flag=="_IsoPhotonSYM"){

			double rho25 = rho; 			

			double Et0 = dau0->et();
			double Et1 = dau1->et();
	
			float HollowConeTrackIso0 = dau0->dr04TkSumPt();
			float HollowConeTrackIso1 = dau1->dr04TkSumPt();

			float JurrasicECALIso0 = dau0->dr04EcalRecHitSumEt();
			float JurrasicECALIso1 = dau1->dr04EcalRecHitSumEt();

			float TowerBasedHCALIso0 = dau0->dr04HcalTowerSumEt();
			float TowerBasedHCALIso1 = dau1->dr04HcalTowerSumEt();
			
			float AbsTrackIsoRho0, AbsTrackIsoRho1; 
			float AbsECALIsoRho0, AbsECALIsoRho1; 
			float AbsHCALIsoRho0, AbsHCALIsoRho1;

			if (dau0->isEB()){
				AbsTrackIsoRho0 = (cTrack_A + cTrack_B*(Et0) + cTrack_C_EB*(rho25));
				AbsECALIsoRho0 = (cECAL_A + cECAL_B*(Et0) + cECAL_C_EB*(rho25));
				AbsHCALIsoRho0 = (cHCAL_A + cHCAL_B*(Et0) + cHCAL_C_EB*(rho25));
			}
			else if (dau0->isEE()){
				AbsTrackIsoRho0 = (cTrack_A + cTrack_B*(Et0) + cTrack_C_EE*(rho25));
				AbsECALIsoRho0 = (cECAL_A + cECAL_B*(Et0) + cECAL_C_EE*(rho25));
				AbsHCALIsoRho0 = (cHCAL_A + cHCAL_B*(Et0) + cHCAL_C_EE*(rho25));
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, IsoPhotonSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if (dau1->isEB()){
				AbsTrackIsoRho1 = (cTrack_A + cTrack_B*(Et1) + cTrack_C_EB*(rho25));
				AbsECALIsoRho1 = (cECAL_A + cECAL_B*(Et1) + cECAL_C_EB*(rho25));
				AbsHCALIsoRho1 = (cHCAL_A + cHCAL_B*(Et1) + cHCAL_C_EB*(rho25));
			}
			else if (dau1->isEE()){
				AbsTrackIsoRho1 = (cTrack_A + cTrack_B*(Et1) + cTrack_C_EE*(rho25));
				AbsECALIsoRho1 = (cECAL_A + cECAL_B*(Et1) + cECAL_C_EE*(rho25));
				AbsHCALIsoRho1 = (cHCAL_A + cHCAL_B*(Et1) + cHCAL_C_EE*(rho25));
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, IsoPhotonSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}


			if(HollowConeTrackIso0 < AbsTrackIsoRho0 && 
			   JurrasicECALIso0 < AbsECALIsoRho0 && 
			   TowerBasedHCALIso0 < AbsHCALIsoRho0){
				isoPhoton0 = true;
			}
			if(HollowConeTrackIso1 < AbsTrackIsoRho1 && 
			   JurrasicECALIso1 < AbsECALIsoRho1 && 
			   TowerBasedHCALIso1 < AbsHCALIsoRho1){
				isoPhoton1 = true;
			}
	
				return isoPhoton0 && isoPhoton1;		
		}

		else if(Flag=="_IsoPhotonASYM"){

			double rho25 = rho; 			

			double Et0 = dau0->et();
			double Et1 = dau1->et();
	
			float HollowConeTrackIso0 = dau0->dr04TkSumPt();
			float HollowConeTrackIso1 = dau1->dr04TkSumPt();

			float JurrasicECALIso0 = dau0->dr04EcalRecHitSumEt();
			float JurrasicECALIso1 = dau1->dr04EcalRecHitSumEt();

			float TowerBasedHCALIso0 = dau0->dr04HcalTowerSumEt();
			float TowerBasedHCALIso1 = dau1->dr04HcalTowerSumEt();
			
			float AbsTrackIsoRho0, AbsTrackIsoRho1; 
			float AbsECALIsoRho0, AbsECALIsoRho1; 
			float AbsHCALIsoRho0, AbsHCALIsoRho1;

			if (dau0->isEB()){
				AbsTrackIsoRho0 = (cTrack_A + cTrack_B*(Et0) + cTrack_C_EB*(rho25));
				AbsECALIsoRho0 = (cECAL_A + cECAL_B*(Et0) + cECAL_C_EB*(rho25));
				AbsHCALIsoRho0 = (cHCAL_A + cHCAL_B*(Et0) + cHCAL_C_EB*(rho25));
			}
			else if (dau0->isEE()){
				AbsTrackIsoRho0 = (cTrack_A + cTrack_B*(Et0) + cTrack_C_EE*(rho25));
				AbsECALIsoRho0 = (cECAL_A + cECAL_B*(Et0) + cECAL_C_EE*(rho25));
				AbsHCALIsoRho0 = (cHCAL_A + cHCAL_B*(Et0) + cHCAL_C_EE*(rho25));
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, IsoPhotonASYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if (dau1->isEB()){
				AbsTrackIsoRho1 = (cTrack_A + cTrack_B*(Et1) + cTrack_C_EB*(rho25));
				AbsECALIsoRho1 = (cECAL_A + cECAL_B*(Et1) + cECAL_C_EB*(rho25));
				AbsHCALIsoRho1 = (cHCAL_A + cHCAL_B*(Et1) + cHCAL_C_EB*(rho25));
			}
			else if (dau1->isEE()){
				AbsTrackIsoRho1 = (cTrack_A + cTrack_B*(Et1) + cTrack_C_EE*(rho25));
				AbsECALIsoRho1 = (cECAL_A + cECAL_B*(Et1) + cECAL_C_EE*(rho25));
				AbsHCALIsoRho1 = (cHCAL_A + cHCAL_B*(Et1) + cHCAL_C_EE*(rho25));
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, IsoPhotonASYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}


			if(HollowConeTrackIso0 < AbsTrackIsoRho0 && 
			   JurrasicECALIso0 < AbsECALIsoRho0 && 
			   TowerBasedHCALIso0 < AbsHCALIsoRho0){
				isoPhoton0 = true;
			}
			if(HollowConeTrackIso1 < AbsTrackIsoRho1 && 
			   JurrasicECALIso1 < AbsECALIsoRho1 && 
			   TowerBasedHCALIso1 < AbsHCALIsoRho1){
				isoPhoton1 = true;
			}
	
				return isoPhoton0 && isoPhoton1;		
		}
*/
		else if(Flag=="_ElIDSYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
			if(dau0->isEB()){
				if(dau0->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
					if(dau0->electronID(ElID_Bar_SYM.c_str())==1.0 || 
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==3.0 || 
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==5.0 ||
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==7.0 ){
						electron_ID0 = true;
					}
				}
			}
			else if(dau0->isEE()){
				if(dau0->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
					if(dau0->electronID(ElID_End_SYM.c_str())==1.0 || 
				   	   dau0->electronID(ElID_End_SYM.c_str())==3.0 || 
				   	   dau0->electronID(ElID_End_SYM.c_str())==5.0 ||
				   	   dau0->electronID(ElID_End_SYM.c_str())==7.0 ){
						electron_ID0 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				//throw cms::Exception("PATAnalysis:RecSelected, ElIDSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(dau1->isEB()){
				if(dau1->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
					if(dau1->electronID(ElID_Bar_SYM.c_str())==1.0 || 
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==3.0 || 
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==5.0 ||
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==7.0 ){
						electron_ID1 = true;
					}
				}
			}
			else if(dau1->isEE()){
				if(dau1->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
					if(dau1->electronID(ElID_End_SYM.c_str())==1.0 || 
				   	   dau1->electronID(ElID_End_SYM.c_str())==3.0 || 
				   	   dau1->electronID(ElID_End_SYM.c_str())==5.0 ||
				   	   dau1->electronID(ElID_End_SYM.c_str())==7.0 ){
						electron_ID1 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				//throw cms::Exception("PATAnalysis:RecSelected, ElIDSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return electron_ID0 && electron_ID1;
		}

		else if(Flag=="_ElIDASYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
			if(dau0->isEB()){
				if(dau0->isElectronIDAvailable(ElID_Bar_ASYM0.c_str())){ 
					if(dau0->electronID(ElID_Bar_ASYM0.c_str())==1.0 || 
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==3.0 || 
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==5.0 ||
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==7.0 ){
						electron_ID0 = true;
					}
				}
			}
			else if(dau0->isEE()){
				if(dau0->isElectronIDAvailable(ElID_End_ASYM0.c_str())){ 
					if(dau0->electronID(ElID_End_ASYM0.c_str())==1.0 || 
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==3.0 || 
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==5.0 ||
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==7.0 ){
						electron_ID0 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ElIDASYM0") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(dau1->isEB()){
				if(dau1->isElectronIDAvailable(ElID_Bar_ASYM1.c_str())){ 
					if(dau1->electronID(ElID_Bar_ASYM1.c_str())==1.0 || 
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==3.0 || 
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==5.0 ||
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==7.0 ){
						electron_ID1 = true;
					}
				}
			}
			else if(dau1->isEE()){
				if(dau1->isElectronIDAvailable(ElID_End_ASYM1.c_str())){ 
					if(dau1->electronID(ElID_End_ASYM1.c_str())==1.0 || 
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==3.0 || 
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==5.0 ||
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==7.0 ){
						electron_ID1 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ElIDASYM1") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return electron_ID0 && electron_ID1;
		}

/*
		else if(Flag=="_ElIDPhotonSYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID

			float sigmaIEtaIEta0 = dau0->sigmaIetaIeta();
			//float sigmaIPhiIPhi0 = dau0->sigmaIphiIphi();
			float HoverE0 = dau0->hadronicOverEm();

			if(dau0->isEB()){
				if((sigmaIEtaIEta0 > sigmaIEtaIEta_EB_L_SYM &&
				    sigmaIEtaIEta0 < sigmaIEtaIEta_EB_H_SYM) &&
				   //sigmaIPhiIPhi0 > sigmaIPhiIPhi_EB_L_SYM &&
				   HoverE0 < HoverE_EB_SYM){
					electron_IDPhoton0 = true;
				}
			}
			else if(dau0->isEE()){
				if(sigmaIEtaIEta0 < sigmaIEtaIEta_EE_SYM &&
				   HoverE0 < HoverE_EE_SYM){
					electron_IDPhoton0 = true;
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ElIDPhotonSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			float sigmaIEtaIEta1 = dau1->sigmaIetaIeta();
			//float sigmaIPhiIPhi1 = dau1->sigmaIphiIphi();
			float HoverE1 = dau1->hadronicOverEm();

			if(dau1->isEB()){
				if((sigmaIEtaIEta1 > sigmaIEtaIEta_EB_L_SYM &&
				    sigmaIEtaIEta1 < sigmaIEtaIEta_EB_H_SYM) &&
				   //sigmaIPhiIPhi1 > sigmaIPhiIPhi_EB_L_SYM &&
				   HoverE1 < HoverE_EB_SYM){
					electron_IDPhoton1 = true;
				}
			}
			else if(dau1->isEE()){
				if(sigmaIEtaIEta1 < sigmaIEtaIEta_EE_SYM &&
				   HoverE1 < HoverE_EE_SYM){
					electron_IDPhoton1 = true;
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ElIDPhotonSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return electron_IDPhoton0 && electron_IDPhoton1;
		}

		else if(Flag=="_ElIDPhotonASYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID

			float sigmaIEtaIEta0 = dau0->sigmaIetaIeta();
			//float sigmaIPhiIPhi0 = dau0->sigmaIphiIphi();
			float HoverE0 = dau0->hadronicOverEm();

			if(dau0->isEB()){
				if((sigmaIEtaIEta0 > sigmaIEtaIEta_EB_L_ASYM0 &&
				    sigmaIEtaIEta0 < sigmaIEtaIEta_EB_H_ASYM0) &&
				   //sigmaIPhiIPhi0 > sigmaIPhiIPhi_EB_L_ASYM0 &&
				   HoverE0 < HoverE_EB_ASYM0){
					electron_IDPhoton0 = true;
				}
			}
			else if(dau0->isEE()){
				if(sigmaIEtaIEta0 < sigmaIEtaIEta_EE_ASYM0 &&
				   HoverE0 < HoverE_EE_ASYM0){
					electron_IDPhoton0 = true;
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ElIDPhotonASYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			float sigmaIEtaIEta1 = dau1->sigmaIetaIeta();
			//float sigmaIPhiIPhi1 = dau1->sigmaIphiIphi();
			float HoverE1 = dau1->hadronicOverEm();

			if(dau1->isEB()){
				if((sigmaIEtaIEta1 > sigmaIEtaIEta_EB_L_ASYM1 &&
				    sigmaIEtaIEta1 < sigmaIEtaIEta_EB_H_ASYM1) &&
				   //sigmaIPhiIPhi1 > sigmaIPhiIPhi_EB_L_ASYM1 &&
				   HoverE1 < HoverE_EB_ASYM1){
					electron_IDPhoton1 = true;
				}
			}
			else if(dau1->isEE()){
				if(sigmaIEtaIEta1 < sigmaIEtaIEta_EE_ASYM1 &&
				   HoverE1 < HoverE_EE_ASYM1){
					electron_IDPhoton1 = true;
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ElIDPhotonASYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return electron_IDPhoton0 && electron_IDPhoton1;
		}
*/
		else if(Flag=="_1"){
			return true;
		}

		else{
			return false;
		}
	}
}


// REC SELECTION

// REC Z Candidate modified: it is true if there is 1 REC Z with Mass cut decaying in two REC Electrons with these cuts: Acceptance (geometrical and kinematic), Quality, Impact Parameter, Isolation without rho.
inline bool RecSelected_mod(string Flag, const std::vector<reco::CompositeCandidate>& ZREC, const pat::TriggerEvent& triggers, int run, double rho){

	std::vector<const pat::Electron*> zdaughters;

	if(ZREC.size()!=0){
		zdaughters = ZRECDaughters(ZREC);
	}
	else{
		return false;
	}

	if(!zdaughters.size()){
		return false;
	}
	else{

		const pat::Electron* dau0 = zdaughters[0];
		const pat::Electron* dau1 = zdaughters[1];
 
		bool qual0 = false;
		bool qual1 = false;

		bool iso0 = false;	
		bool iso1 = false;
			
//		bool isoPhoton0 = false;	
//		bool isoPhoton1 = false;

		bool electron_ID0 = false;
		bool electron_ID1 = false;

//		bool electron_IDPhoton0 = false;
//		bool electron_IDPhoton1 = false;

		bool conv0 = false;
		bool conv1 = false;

		if(Flag=="_AccSYM"){
			return ZREC[0].mass()>zmassmin_sym && ZREC[0].mass()<zmassmax_sym &&
			       dau0->pt() >= ptelcut && fabs(dau0->eta()) <= etaelcut &&
			       dau1->pt() >= ptelcut && fabs(dau1->eta()) <= etaelcut &&
			       !(dau0->isEBEEGap()) && !(dau1->isEBEEGap());
/*			       (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) &&
			       (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
*/
		}


		else if(Flag=="_AccASYM"){
			return ZREC[0].mass()>zmassmin_asym && ZREC[0].mass()<zmassmax_asym &&
			       dau0->pt() >= ptelcut0 && fabs(dau0->eta()) <= etaelcut &&
			       dau1->pt() >= ptelcut1 && fabs(dau1->eta()) <= etaelcut &&
			       !(dau0->isEBEEGap()) && !(dau1->isEBEEGap());
/*			       (fabs(dau0->eta())<eta_el_excl_down || fabs(dau0->eta())>eta_el_excl_up) &&
			       (fabs(dau1->eta())<eta_el_excl_down || fabs(dau1->eta())>eta_el_excl_up);
*/
		}

		else if(Flag=="_Trg"){
			bool cutTrg = false;
			if(elTrgMatchReq==true){
				if(isElectronTriggered(triggers, run)&&(RecSelected_TrgMatch(*dau0, run))){
					cutTrg=true;
				}
			}
			else if(elTrgMatchReq==false){
				if(isElectronTriggered(triggers, run)){
					cutTrg=true;
				}
			}
			return cutTrg;
		}

		else if(Flag=="_QualSYM"){

			const reco::GsfTrackRef track0 = dau0->gsfTrack();
			const reco::GsfTrackRef track1 = dau1->gsfTrack();
			assert (track0.isNonnull() && track1.isNonnull());

			if(track0->numberOfValidHits() > minVaHit_SYM && 
			   track0->normalizedChi2() < maxchi2_SYM){
				qual0 = true;
			}
			if(track1->numberOfValidHits() > minVaHit_SYM && 
			   track1->normalizedChi2() < maxchi2_SYM){
				qual1 = true;
			}
			return qual0 && qual1;
		} 

		else if(Flag=="_QualASYM"){

			const reco::GsfTrackRef track0 = dau0->gsfTrack();
			const reco::GsfTrackRef track1 = dau1->gsfTrack();
			assert (track0.isNonnull() && track1.isNonnull());

			if(track0->numberOfValidHits() > minVaHit_ASYM0 && 
			   track0->normalizedChi2() < maxchi2_ASYM0){
				qual0 = true;
			}
			if(track1->numberOfValidHits() > minVaHit_ASYM1 && 
			   track1->normalizedChi2() < maxchi2_ASYM1){
				qual1 = true;
			}
			return qual0 && qual1;
		}

		else if(Flag=="_ConvSYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
			if(dau0->isEB()){
				if(dau0->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
					if(dau0->electronID(ElID_Bar_SYM.c_str())==4.0 || 
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==5.0 || 
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==6.0 ||
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==7.0 ){
						conv0 = true;
					}
				}
			}
			else if(dau0->isEE()){
				if(dau0->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
					if(dau0->electronID(ElID_End_SYM.c_str())==4.0 || 
				   	   dau0->electronID(ElID_End_SYM.c_str())==5.0 || 
				   	   dau0->electronID(ElID_End_SYM.c_str())==6.0 ||
				   	   dau0->electronID(ElID_End_SYM.c_str())==7.0 ){
						conv0 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, ConvSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(dau1->isEB()){
				if(dau1->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
					if(dau1->electronID(ElID_Bar_SYM.c_str())==4.0 || 
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==5.0 || 
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==6.0 ||
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==7.0 ){
						conv1 = true;
					}
				}
			}
			else if(dau1->isEE()){
				if(dau1->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
					if(dau1->electronID(ElID_End_SYM.c_str())==4.0 || 
				   	   dau1->electronID(ElID_End_SYM.c_str())==5.0 || 
				   	   dau1->electronID(ElID_End_SYM.c_str())==6.0 ||
				   	   dau1->electronID(ElID_End_SYM.c_str())==7.0 ){
						conv1 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, ConvSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return conv0 && conv1;
		}
 	
		else if(Flag=="_ConvASYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
			if(dau0->isEB()){
				if(dau0->isElectronIDAvailable(ElID_Bar_ASYM0.c_str())){ 
					if(dau0->electronID(ElID_Bar_ASYM0.c_str())==4.0 || 
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==5.0 || 
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==6.0 ||
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==7.0 ){
						conv0 = true;
					}
				}
			}
			else if(dau0->isEE()){
				if(dau0->isElectronIDAvailable(ElID_End_ASYM0.c_str())){ 
					if(dau0->electronID(ElID_End_ASYM0.c_str())==4.0 || 
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==5.0 || 
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==6.0 ||
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==7.0 ){
						conv0 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, ConvASYM0") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(dau1->isEB()){
				if(dau1->isElectronIDAvailable(ElID_Bar_ASYM1.c_str())){ 
					if(dau1->electronID(ElID_Bar_ASYM1.c_str())==4.0 || 
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==5.0 || 
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==6.0 ||
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==7.0 ){
						conv1 = true;
					}
				}
			}
			else if(dau1->isEE()){
				if(dau1->isElectronIDAvailable(ElID_End_ASYM1.c_str())){ 
					if(dau1->electronID(ElID_End_ASYM1.c_str())==4.0 || 
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==5.0 || 
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==6.0 ||
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==7.0 ){
						conv1 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, ConvASYM1") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return conv0 && conv1;
		}

		else if(Flag=="_Imp"){
			return dau0->dB() < dxycut && dau1->dB() < dxycut;
		}

		else if(Flag=="_IsoSYM"){

			float trackIso0 = dau0->dr03TkSumPt();
			float trackIso1 = dau1->dr03TkSumPt();

			float ecalIso0 = dau0->dr03EcalRecHitSumEt();
			float ecalIso1 = dau1->dr03EcalRecHitSumEt();

			float hcalIso0 = dau0->dr03HcalTowerSumEt();
			float hcalIso1 = dau1->dr03HcalTowerSumEt();

			float combinedIso03_0, combinedIso03_1;

			if (dau0->isEB()){
				combinedIso03_0 = (trackIso0 + max(0. ,ecalIso0 - 1.) + hcalIso0) / dau0->pt();
			}
			else if (dau0->isEE()){
				combinedIso03_0 = (trackIso0 + ecalIso0 + hcalIso0) / dau0->pt();
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, IsoSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
						
			if (dau1->isEB()){
				combinedIso03_1= (trackIso1 + max(0. ,ecalIso1 - 1.) + hcalIso1) / dau1->pt();
			}
			else if (dau1->isEE()){
				combinedIso03_1 = (trackIso1 + ecalIso1 + hcalIso1) / dau1->pt();
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, IsoSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(combinedIso03_0 <= electronCombinedIsoCut_SYM) iso0 = true;
			if(combinedIso03_1 <= electronCombinedIsoCut_SYM) iso1 = true;
 
			return iso0 && iso1;
		}

		else if(Flag=="_IsoASYM"){
 	
			float trackIso0 = dau0->dr03TkSumPt();
			float trackIso1 = dau1->dr03TkSumPt();

			float ecalIso0 = dau0->dr03EcalRecHitSumEt();
			float ecalIso1 = dau1->dr03EcalRecHitSumEt();

			float hcalIso0 = dau0->dr03HcalTowerSumEt();
			float hcalIso1 = dau1->dr03HcalTowerSumEt();

			float combinedIso03_0, combinedIso03_1;

			if (dau0->isEB()){
				combinedIso03_0 = (trackIso0 + max(0. ,ecalIso0 - 1.) + hcalIso0) / dau0->pt();
			}
			else if (dau0->isEE()){
				combinedIso03_0 = (trackIso0 + ecalIso0 + hcalIso0) / dau0->pt();
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, IsoSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
						
			if (dau1->isEB()){
				combinedIso03_1= (trackIso1 + max(0. ,ecalIso1 - 1.) + hcalIso1) / dau1->pt();
			}
			else if (dau1->isEE()){
				combinedIso03_1 = (trackIso1 + ecalIso1 + hcalIso1) / dau1->pt();
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, IsoSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(combinedIso03_0 <= electronCombinedIsoCut_ASYM0) iso0 = true;
			if(combinedIso03_1 <= electronCombinedIsoCut_ASYM1) iso1 = true;
 
			return iso0 && iso1;
		}

/*
		else if(Flag=="_IsoPhotonSYM"){

			double rho25 = rho; 			

			double Et0 = dau0->et();
			double Et1 = dau1->et();
	
			float HollowConeTrackIso0 = dau0->dr04TkSumPt();
			float HollowConeTrackIso1 = dau1->dr04TkSumPt();

			float JurrasicECALIso0 = dau0->dr04EcalRecHitSumEt();
			float JurrasicECALIso1 = dau1->dr04EcalRecHitSumEt();

			float TowerBasedHCALIso0 = dau0->dr04HcalTowerSumEt();
			float TowerBasedHCALIso1 = dau1->dr04HcalTowerSumEt();
			
			float AbsTrackIsoRho0, AbsTrackIsoRho1; 
			float AbsECALIsoRho0, AbsECALIsoRho1; 
			float AbsHCALIsoRho0, AbsHCALIsoRho1;

			if (dau0->isEB()){
				AbsTrackIsoRho0 = (cTrack_A + cTrack_B*(Et0) + cTrack_C_EB*(rho25));
				AbsECALIsoRho0 = (cECAL_A + cECAL_B*(Et0) + cECAL_C_EB*(rho25));
				AbsHCALIsoRho0 = (cHCAL_A + cHCAL_B*(Et0) + cHCAL_C_EB*(rho25));
			}
			else if (dau0->isEE()){
				AbsTrackIsoRho0 = (cTrack_A + cTrack_B*(Et0) + cTrack_C_EE*(rho25));
				AbsECALIsoRho0 = (cECAL_A + cECAL_B*(Et0) + cECAL_C_EE*(rho25));
				AbsHCALIsoRho0 = (cHCAL_A + cHCAL_B*(Et0) + cHCAL_C_EE*(rho25));
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, IsoPhotonSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if (dau1->isEB()){
				AbsTrackIsoRho1 = (cTrack_A + cTrack_B*(Et1) + cTrack_C_EB*(rho25));
				AbsECALIsoRho1 = (cECAL_A + cECAL_B*(Et1) + cECAL_C_EB*(rho25));
				AbsHCALIsoRho1 = (cHCAL_A + cHCAL_B*(Et1) + cHCAL_C_EB*(rho25));
			}
			else if (dau1->isEE()){
				AbsTrackIsoRho1 = (cTrack_A + cTrack_B*(Et1) + cTrack_C_EE*(rho25));
				AbsECALIsoRho1 = (cECAL_A + cECAL_B*(Et1) + cECAL_C_EE*(rho25));
				AbsHCALIsoRho1 = (cHCAL_A + cHCAL_B*(Et1) + cHCAL_C_EE*(rho25));
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, IsoPhotonSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}


			if(HollowConeTrackIso0 < AbsTrackIsoRho0 && 
			   JurrasicECALIso0 < AbsECALIsoRho0 && 
			   TowerBasedHCALIso0 < AbsHCALIsoRho0){
				isoPhoton0 = true;
			}
			if(HollowConeTrackIso1 < AbsTrackIsoRho1 && 
			   JurrasicECALIso1 < AbsECALIsoRho1 && 
			   TowerBasedHCALIso1 < AbsHCALIsoRho1){
				isoPhoton1 = true;
			}
	
				return isoPhoton0 && isoPhoton1;		
		}


		else if(Flag=="_IsoPhotonASYM"){

			double rho25 = rho; 			

			double Et0 = dau0->et();
			double Et1 = dau1->et();
	
			float HollowConeTrackIso0 = dau0->dr04TkSumPt();
			float HollowConeTrackIso1 = dau1->dr04TkSumPt();

			float JurrasicECALIso0 = dau0->dr04EcalRecHitSumEt();
			float JurrasicECALIso1 = dau1->dr04EcalRecHitSumEt();

			float TowerBasedHCALIso0 = dau0->dr04HcalTowerSumEt();
			float TowerBasedHCALIso1 = dau1->dr04HcalTowerSumEt();
			
			float AbsTrackIsoRho0, AbsTrackIsoRho1; 
			float AbsECALIsoRho0, AbsECALIsoRho1; 
			float AbsHCALIsoRho0, AbsHCALIsoRho1;

			if (dau0->isEB()){
				AbsTrackIsoRho0 = (cTrack_A + cTrack_B*(Et0) + cTrack_C_EB*(rho25));
				AbsECALIsoRho0 = (cECAL_A + cECAL_B*(Et0) + cECAL_C_EB*(rho25));
				AbsHCALIsoRho0 = (cHCAL_A + cHCAL_B*(Et0) + cHCAL_C_EB*(rho25));
			}
			else if (dau0->isEE()){
				AbsTrackIsoRho0 = (cTrack_A + cTrack_B*(Et0) + cTrack_C_EE*(rho25));
				AbsECALIsoRho0 = (cECAL_A + cECAL_B*(Et0) + cECAL_C_EE*(rho25));
				AbsHCALIsoRho0 = (cHCAL_A + cHCAL_B*(Et0) + cHCAL_C_EE*(rho25));
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, IsoPhotonASSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if (dau1->isEB()){
				AbsTrackIsoRho1 = (cTrack_A + cTrack_B*(Et1) + cTrack_C_EB*(rho25));
				AbsECALIsoRho1 = (cECAL_A + cECAL_B*(Et1) + cECAL_C_EB*(rho25));
				AbsHCALIsoRho1 = (cHCAL_A + cHCAL_B*(Et1) + cHCAL_C_EB*(rho25));
			}
			else if (dau1->isEE()){
				AbsTrackIsoRho1 = (cTrack_A + cTrack_B*(Et1) + cTrack_C_EE*(rho25));
				AbsECALIsoRho1 = (cECAL_A + cECAL_B*(Et1) + cECAL_C_EE*(rho25));
				AbsHCALIsoRho1 = (cHCAL_A + cHCAL_B*(Et1) + cHCAL_C_EE*(rho25));
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, IsoPhotonASYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}


			if(HollowConeTrackIso0 < AbsTrackIsoRho0 && 
			   JurrasicECALIso0 < AbsECALIsoRho0 && 
			   TowerBasedHCALIso0 < AbsHCALIsoRho0){
				isoPhoton0 = true;
			}
			if(HollowConeTrackIso1 < AbsTrackIsoRho1 && 
			   JurrasicECALIso1 < AbsECALIsoRho1 && 
			   TowerBasedHCALIso1 < AbsHCALIsoRho1){
				isoPhoton1 = true;
			}
	
				return isoPhoton0 && isoPhoton1;		
		}
*/
		else if(Flag=="_ElIDSYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
			if(dau0->isEB()){
				if(dau0->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
					if(dau0->electronID(ElID_Bar_SYM.c_str())==1.0 || 
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==3.0 || 
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==5.0 ||
				   	   dau0->electronID(ElID_Bar_SYM.c_str())==7.0 ){
						electron_ID0 = true;
					}
				}
			}
			else if(dau0->isEE()){
				if(dau0->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
					if(dau0->electronID(ElID_End_SYM.c_str())==1.0 || 
				   	   dau0->electronID(ElID_End_SYM.c_str())==3.0 || 
				   	   dau0->electronID(ElID_End_SYM.c_str())==5.0 ||
				   	   dau0->electronID(ElID_End_SYM.c_str())==7.0 ){
						electron_ID0 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, ElIDSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(dau1->isEB()){
				if(dau1->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
					if(dau1->electronID(ElID_Bar_SYM.c_str())==1.0 || 
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==3.0 || 
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==5.0 ||
				   	   dau1->electronID(ElID_Bar_SYM.c_str())==7.0 ){
						electron_ID1 = true;
					}
				}
			}
			else if(dau1->isEE()){
				if(dau1->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
					if(dau1->electronID(ElID_End_SYM.c_str())==1.0 || 
				   	   dau1->electronID(ElID_End_SYM.c_str())==3.0 || 
				   	   dau1->electronID(ElID_End_SYM.c_str())==5.0 ||
				   	   dau1->electronID(ElID_End_SYM.c_str())==7.0 ){
						electron_ID1 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, ElIDSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return electron_ID0 && electron_ID1;
		}

		else if(Flag=="_ElIDASYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
			if(dau0->isEB()){
				if(dau0->isElectronIDAvailable(ElID_Bar_ASYM0.c_str())){ 
					if(dau0->electronID(ElID_Bar_ASYM0.c_str())==1.0 || 
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==3.0 || 
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==5.0 ||
				   	   dau0->electronID(ElID_Bar_ASYM0.c_str())==7.0 ){
						electron_ID0 = true;
					}
				}
			}
			else if(dau0->isEE()){
				if(dau0->isElectronIDAvailable(ElID_End_ASYM0.c_str())){ 
					if(dau0->electronID(ElID_End_ASYM0.c_str())==1.0 || 
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==3.0 || 
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==5.0 ||
				   	   dau0->electronID(ElID_End_ASYM0.c_str())==7.0 ){
						electron_ID0 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, ElIDASYM0") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(dau1->isEB()){
				if(dau1->isElectronIDAvailable(ElID_Bar_ASYM1.c_str())){ 
					if(dau1->electronID(ElID_Bar_ASYM1.c_str())==1.0 || 
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==3.0 || 
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==5.0 ||
				   	   dau1->electronID(ElID_Bar_ASYM1.c_str())==7.0 ){
						electron_ID1 = true;
					}
				}
			}
			else if(dau1->isEE()){
				if(dau1->isElectronIDAvailable(ElID_End_ASYM1.c_str())){ 
					if(dau1->electronID(ElID_End_ASYM1.c_str())==1.0 || 
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==3.0 || 
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==5.0 ||
				   	   dau1->electronID(ElID_End_ASYM1.c_str())==7.0 ){
						electron_ID1 = true;
					}
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected_mod, ElIDASYM1") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return electron_ID0 && electron_ID1;
		}

/*
		else if(Flag=="_ElIDPhotonSYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID

			float sigmaIEtaIEta0 = dau0->sigmaIetaIeta();
			//float sigmaIPhiIPhi0 = dau0->sigmaIphiIphi();
			float HoverE0 = dau0->hadronicOverEm();

			if(dau0->isEB()){
				if((sigmaIEtaIEta0 > sigmaIEtaIEta_EB_L_SYM &&
				    sigmaIEtaIEta0 < sigmaIEtaIEta_EB_H_SYM) &&
				   //sigmaIPhiIPhi0 > sigmaIPhiIPhi_EB_L_SYM &&
				   HoverE0 < HoverE_EB_SYM){
					electron_IDPhoton0 = true;
				}
			}
			else if(dau0->isEE()){
				if(sigmaIEtaIEta0 < sigmaIEtaIEta_EE_SYM &&
				   HoverE0 < HoverE_EE_SYM){
					electron_IDPhoton0 = true;
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ElIDPhotonSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			float sigmaIEtaIEta1 = dau1->sigmaIetaIeta();
			//float sigmaIPhiIPhi1 = dau1->sigmaIphiIphi();
			float HoverE1 = dau1->hadronicOverEm();

			if(dau1->isEB()){
				if((sigmaIEtaIEta1 > sigmaIEtaIEta_EB_L_SYM &&
				    sigmaIEtaIEta1 < sigmaIEtaIEta_EB_H_SYM) &&
				   //sigmaIPhiIPhi1 > sigmaIPhiIPhi_EB_L_SYM &&
				   HoverE1 < HoverE_EB_SYM){
					electron_IDPhoton1 = true;
				}
			}
			else if(dau1->isEE()){
				if(sigmaIEtaIEta1 < sigmaIEtaIEta_EE_SYM &&
				   HoverE1 < HoverE_EE_SYM){
					electron_IDPhoton1 = true;
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ElIDPhotonSYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return electron_IDPhoton0 && electron_IDPhoton1;
		}

		else if(Flag=="_ElIDPhotonASYM"){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID

			float sigmaIEtaIEta0 = dau0->sigmaIetaIeta();
			//float sigmaIPhiIPhi0 = dau0->sigmaIphiIphi();
			float HoverE0 = dau0->hadronicOverEm();

			if(dau0->isEB()){
				if((sigmaIEtaIEta0 > sigmaIEtaIEta_EB_L_ASYM0 &&
				    sigmaIEtaIEta0 < sigmaIEtaIEta_EB_H_ASYM0) &&
				   //sigmaIPhiIPhi0 > sigmaIPhiIPhi_EB_L_ASYM0 &&
				   HoverE0 < HoverE_EB_ASYM0){
					electron_IDPhoton0 = true;
				}
			}
			else if(dau0->isEE()){
				if(sigmaIEtaIEta0 < sigmaIEtaIEta_EE_ASYM0 &&
				   HoverE0 < HoverE_EE_ASYM0){
					electron_IDPhoton0 = true;
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ElIDPhotonASYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			float sigmaIEtaIEta1 = dau1->sigmaIetaIeta();
			//float sigmaIPhiIPhi1 = dau1->sigmaIphiIphi();
			float HoverE1 = dau1->hadronicOverEm();

			if(dau1->isEB()){
				if((sigmaIEtaIEta1 > sigmaIEtaIEta_EB_L_ASYM1 &&
				    sigmaIEtaIEta1 < sigmaIEtaIEta_EB_H_ASYM1) &&
				   //sigmaIPhiIPhi1 > sigmaIPhiIPhi_EB_L_ASYM1 &&
				   HoverE1 < HoverE_EB_ASYM1){
					electron_IDPhoton1 = true;
				}
			}
			else if(dau1->isEE()){
				if(sigmaIEtaIEta1 < sigmaIEtaIEta_EE_ASYM1 &&
				   HoverE1 < HoverE_EE_ASYM1){
					electron_IDPhoton1 = true;
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, ElIDPhotonASYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
			return electron_IDPhoton0 && electron_IDPhoton1;
		}
*/
		else if(Flag=="_1"){
			return true;
		}

		else{
			return false;
		}
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

template<class ELECTRON> double MinDeltaRZDau(const std::vector<const ELECTRON*>& electrons, const reco::Jet& jet){

        double minDeltaRZDau = -999999;
	
	minDeltaRZDau = (Delta_R<ELECTRON>(*(electrons[0]),jet) < Delta_R<ELECTRON>(*(electrons[1]),jet)) ? Delta_R<ELECTRON>(*(electrons[0]),jet) : Delta_R<ELECTRON>(*(electrons[1]),jet);
	
	return minDeltaRZDau;
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Jet Methods

// Jets Selector Utility: it returns a vector of Jets, selected for Acceptance (geometrical and kinematic)
template<class JET> std::vector<const JET*> GetJets_noJetID(const std::vector<JET>& jets){

	std::vector<const JET*> selectedjets;
	for (unsigned int i = 0; i < jets.size(); ++i){
		if (jets[i].pt() > ptjetmin && fabs(jets[i].eta()) < etajetmax){ 
			selectedjets.push_back(&jets[i]);
		}
	}
	return selectedjets;
}


// Single Jet ID Selector Utility: it is true if the Jet pass the ID selection
template<class JET> int jetID(const JET& jet){

	 int jetID = -1;
 
	 if (jet->isPFJet()) {
 
	 	int   chm = jet->chargedHadronMultiplicity();
	 	int   npr = jet->chargedMultiplicity() + jet->neutralMultiplicity();
	 	float nhf = (jet->neutralHadronEnergy() + jet->HFHadronEnergy())/jet->energy();
	 	float phf = jet->photonEnergyFraction();
	 	float chf = jet->chargedHadronEnergyFraction();
	 	float elf = (jet->electronEnergy())/(jet->energy());

	 	if(npr>1 && 
		   phf<0.99 && 
		   nhf<0.99 && 
		   ((fabs(jet->eta())<=2.4 && nhf<0.9 && phf<0.9 && elf<0.99 && chf>0 && chm>0) || 
		    fabs(jet->eta())>2.4)){ 
	 		jetID = 1;
		}
	 }
	 return jetID;
}


// Jets Selector Utility: it returns a vector of Jets, if you want selected for ID, selected for Acceptance (geometrical and kinematic) corrected for the JEC Uncertainties
template<class JET> std::vector<const JET*> GetJets_wJECUnc(const std::vector<JET>& jets, JetCorrectionUncertainty& jecUnc, int JECUnc){

	std::vector<const JET*> selectedjets;

	bool jetIDflag = true;

	for (unsigned int i = 0; i < jets.size(); ++i){
		if(JetIDReq){
			jetIDflag=false;
			if(jetID(&jets[i])==1){
				jetIDflag=true;
			}
		}

		double unc = 0;

		if(JECUnc!=0){
			jecUnc.setJetEta(jets[i].eta());
			jecUnc.setJetPt(jets[i].pt()); 
			unc = jecUnc.getUncertainty(true)*JECUnc;
		}

		if (jets[i].pt()+(jets[i].pt()*unc) > ptjetmin && 
		    fabs(jets[i].eta()) < etajetmax && 
		    jetIDflag){ 
			selectedjets.push_back(&jets[i]);
		}
	}
	return selectedjets;
}


// Jets Selector Utility: it returns a vector of Jets, selected for Acceptance (geometrical and kinematic) and ID
template<class JET> std::vector<const JET*> GetJets(const std::vector<JET>& jets){

	std::vector<const JET*> selectedjets;

	bool jetIDflag = true;

	for (unsigned int i = 0; i < jets.size(); ++i){
		if(JetIDReq){
			jetIDflag=false;
			if(jetID(&jets[i])==1){
				jetIDflag=true;
			}
		}
		if (jets[i].pt() > ptjetmin && fabs(jets[i].eta()) < etajetmax && 
		    jetIDflag){
			selectedjets.push_back(&jets[i]);
		}
	}
	return selectedjets;
}


// Jet Selector Utility: it returns a vector of Jets, cleaned from a vector of electrons
template<class JET> std::vector<const JET*> CleanJets(std::vector<const JET*> jets, const std::vector<const pat::Electron*> electrons){
	std::vector<const JET*> cleanedjets;

	for (unsigned int i = 0; i < jets.size(); ++i){
		bool close = false; 
		for (unsigned int j = 0; j < electrons.size(); ++j){
			if (deltaR(jets[i]->p4(), electrons[j]->p4()) < isojetcut){
				close = true;
				break;
			}
		}
		if (!close){
			cleanedjets.push_back(jets[i]);
		}
	}
	return cleanedjets;
}


// Single Jet Selector Utility: it is true if the single Jet is Cleaned from a vector of Electrons (2 Electrons, because of the MinDeltaRZDau method)
template<class ELECTRON> bool IsoJet(const std::vector<const ELECTRON*>& electrons, const reco::Jet& jet){
	
	bool iso_jet = true;
	
	if(MinDeltaRZDau<ELECTRON>(electrons,jet) < isojetcut){
		iso_jet = false;
	}	
	return iso_jet;
}
/*
// Single Jet Selector Utility: it is true if the single Jet is Cleaned from a vector of Electrons (2 Electronss, because of the MinDeltaRZDau method)
template<class ELECTRON> bool IsoJet(const std::vector<const ELECTRON*>& electrons, const reco::Jet& jet){
	
	bool iso_jet = true;
	
	if(MinDeltaRZDau<ELECTRON>(electrons,jet) < isojetcut){
		iso_jet = false;
	}	
	return iso_jet;
}
*/
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


// Conditions required to fill TagAndProbe
inline bool MassWindow(const std::vector<reco::CompositeCandidate>& Z, string selections){

	if(selections=="SYM"){
		return Z[0].mass() > zmassmin_sym && Z[0].mass() < zmassmax_sym;
	}

	if(selections=="ASYM"){
	return Z[0].mass() > zmassmin_asym && Z[0].mass() < zmassmax_asym;
	}

	else{
		return false;
	}
}


// Tag cuts SYM
// Cuts applied on Tag Electron: SYM
inline bool singleEl_Tag_SYM(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	bool Acc = true;
	bool Qual = true;
	bool Iso = true;
//	bool IsoPhoton = true;
	bool Imp = true;
	bool ElID = true;
//	bool ElIDPhoton = true;
	bool Conv = true;

	bool TAG_acc = false;
	bool TAG_qual = false;
	bool TAG_iso  = false;
//	bool TAG_isoPhoton  = false;
	bool TAG_imp = false;
	bool TAG_elID = false;
//	bool TAG_elIDPhoton = false;
	bool TAG_conv = false;

	if(electron){

		//Acc
		if(Acc){
			if(electron->pt() >= SYM_TAG_ptelcut && 
			   fabs(electron->eta()) <= SYM_TAG_etaelcut &&
			   !(electron->isEBEEGap())){
			   //(fabs(electron->eta()) < SYM_TAG_eta_el_excl_down || fabs(electron->eta()) > SYM_TAG_eta_el_excl_up)){
				TAG_acc = true;
			}
		}

		//Qual
		if(Qual){
			const reco::GsfTrackRef track = electron->gsfTrack();
			assert(track.isNonnull());

			if(track->numberOfValidHits() > SYM_TAG_minVaHit && 
			   track->normalizedChi2() < SYM_TAG_maxchi2){
				TAG_qual = true;
			}
		}

		//Iso
		if(Iso){

			float trackIso = electron->dr03TkSumPt();
			float ecalIso = electron->dr03EcalRecHitSumEt();
			float hcalIso = electron->dr03HcalTowerSumEt();

			float combinedIso03Rho;

			if (electron->isEB()){
				combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEB*(rho)) + max(0.,hcalIso - cAhcalHE*(rho))) / electron->pt();
			}
			else if (electron->isEE()){
				combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEE*(rho)) + max(0.,hcalIso - cAhcalEE*(rho))) / electron->pt();
			}
			else{
				std::cout << "ERROR! The Tag electron is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis: singleEl_Tag_SYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
						
			if(combinedIso03Rho <= SYM_TAG_electronCombinedIsoRhoCut){
				TAG_iso = true;
			}
		}

/*
		//Iso Photon
		if(IsoPhoton){

			double rho25 = rho; 			

			double Et = electron->et();
	
			float HollowConeTrackIso = electron->dr04TkSumPt();

			float JurrasicECALIso = electron->dr04EcalRecHitSumEt();

			float TowerBasedHCALIso = electron->dr04HcalTowerSumEt();
			
			float AbsTrackIsoRho; 
			float AbsECALIsoRho; 
			float AbsHCALIsoRho;

			if (electron->isEB()){
				AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EB*(rho25));
				AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EB*(rho25));
				AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EB*(rho25));
			}
			else if (electron->isEE()){
				AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EE*(rho25));
				AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EE*(rho25));
				AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EE*(rho25));
			}
			else{
				std::cout << "ERROR! The electron is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, singleEl_Tag_SYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(HollowConeTrackIso < AbsTrackIsoRho && 
			   JurrasicECALIso < AbsECALIsoRho && 
			   TowerBasedHCALIso < AbsHCALIsoRho){
				TAG_isoPhoton = true;
			}
		
		}
*/

		//Imp
		if(Imp){
			if(electron->dB() < SYM_TAG_dxycut){
				TAG_imp = true;
			}
		}

		//ElID
		if(ElID){
			if(electron->isEB()){
				if(electron->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
					if(electron->electronID(ElID_Bar_SYM.c_str())==1.0 || 
				   	   electron->electronID(ElID_Bar_SYM.c_str())==3.0 || 
				   	   electron->electronID(ElID_Bar_SYM.c_str())==5.0 ||
				   	   electron->electronID(ElID_Bar_SYM.c_str())==7.0 ){
						TAG_elID = true;
					}
				}
			}
			else if(electron->isEE()){
				if(electron->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
					if(electron->electronID(ElID_End_SYM.c_str())==1.0 || 
				   	   electron->electronID(ElID_End_SYM.c_str())==3.0 || 
				   	   electron->electronID(ElID_End_SYM.c_str())==5.0 ||
				   	   electron->electronID(ElID_End_SYM.c_str())==7.0 ){
						TAG_elID = true;
					}
				}
			}
		}

/*
		//ElID Photon
		if(ElIDPhoton){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID

			float sigmaIEtaIEta = electron->sigmaIetaIeta();
			//float sigmaIPhiIPhi = electron->sigmaIphiIphi();
			float HoverE = electron->hadronicOverEm();

			if(electron->isEB()){
				if((sigmaIEtaIEta > sigmaIEtaIEta_EB_L_SYM &&
				    sigmaIEtaIEta < sigmaIEtaIEta_EB_H_SYM) &&
				   //sigmaIPhiIPhi > sigmaIPhiIPhi_EB_L_SYM &&
				   HoverE < HoverE_EB_SYM){
					TAG_elIDPhoton = true;
				}
			}
			else if(electron->isEE()){
				if(sigmaIEtaIEta < sigmaIEtaIEta_EE_SYM &&
				   HoverE < HoverE_EE_SYM){
					TAG_elIDPhoton = true;
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:singleEl_Tag_SYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
		}
*/

		//Conv
		if(Conv){
			if(electron->isEB()){
				if(electron->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
					if(electron->electronID(ElID_Bar_SYM.c_str())==4.0 || 
				   	   electron->electronID(ElID_Bar_SYM.c_str())==5.0 || 
				   	   electron->electronID(ElID_Bar_SYM.c_str())==6.0 ||
				   	   electron->electronID(ElID_Bar_SYM.c_str())==7.0 ){
						TAG_conv = true;
					}
				}
			}
			else if(electron->isEE()){
				if(electron->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
					if(electron->electronID(ElID_End_SYM.c_str())==4.0 || 
				   	   electron->electronID(ElID_End_SYM.c_str())==5.0 || 
				   	   electron->electronID(ElID_End_SYM.c_str())==6.0 ||
				   	   electron->electronID(ElID_End_SYM.c_str())==7.0 ){
						TAG_conv = true;
					}
				}
			}
		}

		return TAG_acc &&
		       TAG_imp &&
		       TAG_qual &&
		       TAG_iso &&
//		       TAG_isoPhoton &&
		       TAG_elID &&
//		       TAG_elIDPhoton &&
		       TAG_conv;
	}
	else{
		return false;
	}
}
/*old
inline bool singleEl_Tag_SYM(const reco::Candidate& cand, int run){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){
		const reco::GsfTrackRef track = electron->gsfTrack();
		assert(track.isNonnull());
		bool TAG_EiD = true;
		if(electron->isElectronIDAvailable(SYM_TagEiD.c_str())){
			TAG_EiD = false;
			if(electron->electronID(SYM_TagEiD.c_str())==1.0){
				TAG_EiD = true;
			}
		}

		return electron->pt() > SYM_TAG_ptelcut && 
		       fabs(electron->eta()) < SYM_TAG_etaelcut &&
		       (fabs(electron->eta()) < SYM_TAG_eta_el_excl_down || fabs(electron->eta()) > SYM_TAG_eta_el_excl_up) &&
		       track->numberOfValidHits() > SYM_TAG_minnhit && 
		       track->normalizedChi2() < SYM_TAG_maxchi2 &&
		       electron->dB() < SYM_TAG_dxycut &&
		       (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < SYM_TAG_isocut &&
		       TAG_EiD;
	}
	else{
		return false;
	}
}
*/


// Tag cuts ASYM
// Cuts applied on Tag Electron: ASYM, leg 0
inline bool singleEl_Tag_ASYM0(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	bool Acc = true;
	bool Qual = true;
	bool Iso = true;
//	bool IsoPhoton = true;
	bool Imp = true;
	bool ElID = true;
//	bool ElIDPhoton = true;
	bool Conv = true;

	bool TAG_acc = false;
	bool TAG_qual = false;
	bool TAG_iso  = false;
//	bool TAG_isoPhoton  = false;
	bool TAG_imp = false;
//	bool TAG_elIDPhoton = false;
	bool TAG_elID = false;

	bool TAG_conv = false;

	if(electron){

		//Acc
		if(Acc){
			if(electron->pt() >= ASYM0_TAG_ptelcut && 
			   fabs(electron->eta()) <= ASYM0_TAG_etaelcut &&
			   !(electron->isEBEEGap())){
			   //(fabs(electron->eta()) < ASYM0_TAG_eta_el_excl_down || fabs(electron->eta()) > ASYM0_TAG_eta_el_excl_up)){
				TAG_acc = true;
			}
		}

		//Qual
		if(Qual){
			const reco::GsfTrackRef track = electron->gsfTrack();
			assert(track.isNonnull());

			if(track->numberOfValidHits() > ASYM0_TAG_minVaHit && 
			   track->normalizedChi2() < ASYM0_TAG_maxchi2){
				TAG_qual = true;
			}
		}

		//Iso
		if(Iso){

			float trackIso = electron->dr03TkSumPt();
			float ecalIso = electron->dr03EcalRecHitSumEt();
			float hcalIso = electron->dr03HcalTowerSumEt();

			float combinedIso03Rho;

			if (electron->isEB()){
				combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEB*(rho)) + max(0.,hcalIso - cAhcalHE*(rho))) / electron->pt();
			}
			else if (electron->isEE()){
				combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEE*(rho)) + max(0.,hcalIso - cAhcalEE*(rho))) / electron->pt();
			}
			else{
				std::cout << "ERROR! The Tag electron is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis: singleEl_Tag_ASYM0") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
						
			if(combinedIso03Rho <= ASYM0_TAG_electronCombinedIsoRhoCut){
				TAG_iso = true;
			}
		}

/*
		//Iso Photon
		if(IsoPhoton){

			double rho25 = rho; 			

			double Et = electron->et();
	
			float HollowConeTrackIso = electron->dr04TkSumPt();

			float JurrasicECALIso = electron->dr04EcalRecHitSumEt();

			float TowerBasedHCALIso = electron->dr04HcalTowerSumEt();
			
			float AbsTrackIsoRho; 
			float AbsECALIsoRho; 
			float AbsHCALIsoRho;

			if (electron->isEB()){
				AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EB*(rho25));
				AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EB*(rho25));
				AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EB*(rho25));
			}
			else if (electron->isEE()){
				AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EE*(rho25));
				AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EE*(rho25));
				AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EE*(rho25));
			}
			else{
				std::cout << "ERROR! The electron is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, singleEl_Tag_ASYM0") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(HollowConeTrackIso < AbsTrackIsoRho && 
			   JurrasicECALIso < AbsECALIsoRho && 
			   TowerBasedHCALIso < AbsHCALIsoRho){
				TAG_isoPhoton = true;
			}
		
		}
*/

		//Imp
		if(Imp){
			if(electron->dB() < ASYM0_TAG_dxycut){
				TAG_imp = true;
			}
		}

		//ElID
		if(ElID){
			if(electron->isEB()){
				if(electron->isElectronIDAvailable(ElID_Bar_ASYM0.c_str())){ 
					if(electron->electronID(ElID_Bar_ASYM0.c_str())==1.0 || 
				   	   electron->electronID(ElID_Bar_ASYM0.c_str())==3.0 || 
				   	   electron->electronID(ElID_Bar_ASYM0.c_str())==5.0 ||
				   	   electron->electronID(ElID_Bar_ASYM0.c_str())==7.0 ){
						TAG_elID = true;
					}
				}
			}
			else if(electron->isEE()){
				if(electron->isElectronIDAvailable(ElID_End_ASYM0.c_str())){ 
					if(electron->electronID(ElID_End_ASYM0.c_str())==1.0 || 
				   	   electron->electronID(ElID_End_ASYM0.c_str())==3.0 || 
				   	   electron->electronID(ElID_End_ASYM0.c_str())==5.0 ||
				   	   electron->electronID(ElID_End_ASYM0.c_str())==7.0 ){
						TAG_elID = true;
					}
				}
			}
		}

/*
		//ElID Photon
		if(ElIDPhoton){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID

			float sigmaIEtaIEta = electron->sigmaIetaIeta();
			//float sigmaIPhiIPhi = electron->sigmaIphiIphi();
			float HoverE = electron->hadronicOverEm();

			if(electron->isEB()){
				if((sigmaIEtaIEta > sigmaIEtaIEta_EB_L_ASYM0 &&
				    sigmaIEtaIEta < sigmaIEtaIEta_EB_H_ASYM0) &&
				   //sigmaIPhiIPhi > sigmaIPhiIPhi_EB_L_ASYM0 &&
				   HoverE < HoverE_EB_ASYM0){
					TAG_elIDPhoton = true;
				}
			}
			else if(electron->isEE()){
				if(sigmaIEtaIEta < sigmaIEtaIEta_EE_ASYM0 &&
				   HoverE < HoverE_EE_ASYM0){
					TAG_elIDPhoton = true;
				}
			}
			else{
				std::cout << "ERROR! The electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:singleEl_Tag_ASYM0") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
		}
*/

		//Conv
		if(Conv){
			if(electron->isEB()){
				if(electron->isElectronIDAvailable(ElID_Bar_ASYM0.c_str())){ 
					if(electron->electronID(ElID_Bar_ASYM0.c_str())==4.0 || 
				   	   electron->electronID(ElID_Bar_ASYM0.c_str())==5.0 || 
				   	   electron->electronID(ElID_Bar_ASYM0.c_str())==6.0 ||
				   	   electron->electronID(ElID_Bar_ASYM0.c_str())==7.0 ){
						TAG_conv = true;
					}
				}
			}
			else if(electron->isEE()){
				if(electron->isElectronIDAvailable(ElID_End_ASYM1.c_str())){ 
					if(electron->electronID(ElID_End_ASYM0.c_str())==4.0 || 
				   	   electron->electronID(ElID_End_ASYM0.c_str())==5.0 || 
				   	   electron->electronID(ElID_End_ASYM0.c_str())==6.0 ||
				   	   electron->electronID(ElID_End_ASYM0.c_str())==7.0 ){
						TAG_conv = true;
					}
				}
			}
		}

		return TAG_acc &&
		       TAG_imp &&
		       TAG_qual &&
		       TAG_iso &&
//		       TAG_isoPhoton &&
		       TAG_elID &&
//		       TAG_elIDPhoton &&
		       TAG_conv;
	}
	else{
		return false;
	}
}
/*old
inline bool singleEl_Tag_ASYM0(const reco::Candidate& cand, int run){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){
		const reco::GsfTrackRef track = electron->gsfTrack();
		assert(track.isNonnull());
		bool TAG_EiD = true;
		if(electron->isElectronIDAvailable(ASYM0_TagEiD.c_str())){
			TAG_EiD = false;
			if(electron->electronID(ASYM0_TagEiD.c_str())==1.0){
				TAG_EiD = true;
			}
		}
		return electron->pt() > ASYM0_TAG_ptelcut && 
		       fabs(electron->eta()) < ASYM0_TAG_etaelcut &&
		       (fabs(electron->eta()) < ASYM0_TAG_eta_el_excl_down || fabs(electron->eta()) > ASYM0_TAG_eta_el_excl_up) &&
		       track->numberOfValidHits() > ASYM0_TAG_minnhit && 
		       track->normalizedChi2() < ASYM0_TAG_maxchi2 &&
		       electron->dB() < ASYM0_TAG_dxycut &&
		       (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < ASYM0_TAG_isocut &&
		       TAG_EiD;
	}
	else{
		return false;
	}
}
*/


// Cuts applied on Tag Electron: ASYM, leg 1
inline bool singleEl_Tag_ASYM1(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	bool Acc = true;
	bool Qual = true;
	bool Iso = true;
//	bool IsoPhoton = true;
	bool Imp = true;
	bool ElID = true;
//	bool ElIDPhoton = true;
	bool Conv = true;

	bool TAG_acc = false;
	bool TAG_qual = false;
	bool TAG_iso  = false;
//	bool TAG_isoPhoton  = false;
	bool TAG_imp = false;
	bool TAG_elID = false;
//	bool TAG_elIDPhoton = false;
	bool TAG_conv = false;

	if(electron){

		//Acc
		if(Acc){
			if(electron->pt() >= ASYM1_TAG_ptelcut && 
			   fabs(electron->eta()) <= ASYM1_TAG_etaelcut &&
			   !(electron->isEBEEGap())){
			   //(fabs(electron->eta()) < ASYM1_TAG_eta_el_excl_down || fabs(electron->eta()) > ASYM1_TAG_eta_el_excl_up)){
				TAG_acc = true;
			}
		}

		//Qual
		if(Qual){
			const reco::GsfTrackRef track = electron->gsfTrack();
			assert(track.isNonnull());

			if(track->numberOfValidHits() > ASYM1_TAG_minVaHit && 
			   track->normalizedChi2() < ASYM1_TAG_maxchi2){
				TAG_qual = true;
			}
		}

		//Iso
		if(Iso){

			float trackIso = electron->dr03TkSumPt();
			float ecalIso = electron->dr03EcalRecHitSumEt();
			float hcalIso = electron->dr03HcalTowerSumEt();

			float combinedIso03Rho;

			if (electron->isEB()){
				combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEB*(rho)) + max(0.,hcalIso - cAhcalHE*(rho))) / electron->pt();
			}
			else if (electron->isEE()){
				combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEE*(rho)) + max(0.,hcalIso - cAhcalEE*(rho))) / electron->pt();
			}
			else{
				std::cout << "ERROR! The Tag electron is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis: singleEl_Tag_ASYM1") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
						
			if(combinedIso03Rho <= ASYM1_TAG_electronCombinedIsoRhoCut){
				TAG_iso = true;
			}
		}

/*
		//Iso Photon
		if(IsoPhoton){

			double rho25 = rho; 			

			double Et = electron->et();
	
			float HollowConeTrackIso = electron->dr04TkSumPt();

			float JurrasicECALIso = electron->dr04EcalRecHitSumEt();

			float TowerBasedHCALIso = electron->dr04HcalTowerSumEt();
			
			float AbsTrackIsoRho; 
			float AbsECALIsoRho; 
			float AbsHCALIsoRho;

			if (electron->isEB()){
				AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EB*(rho25));
				AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EB*(rho25));
				AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EB*(rho25));
			}
			else if (electron->isEE()){
				AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EE*(rho25));
				AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EE*(rho25));
				AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EE*(rho25));
			}
			else{
				std::cout << "ERROR! The electron is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:RecSelected, singleEl_Tag_ASYM1") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

			if(HollowConeTrackIso < AbsTrackIsoRho && 
			   JurrasicECALIso < AbsECALIsoRho && 
			   TowerBasedHCALIso < AbsHCALIsoRho){
				TAG_isoPhoton = true;
			}
		
		}
*/

		//Imp
		if(Imp){
			if(electron->dB() < ASYM1_TAG_dxycut){
				TAG_imp = true;
			}
		}

		//ElID
		if(ElID){
			if(electron->isEB()){
				if(electron->isElectronIDAvailable(ElID_Bar_ASYM1.c_str())){ 
					if(electron->electronID(ElID_Bar_ASYM1.c_str())==1.0 || 
				   	   electron->electronID(ElID_Bar_ASYM1.c_str())==3.0 || 
				   	   electron->electronID(ElID_Bar_ASYM1.c_str())==5.0 ||
				   	   electron->electronID(ElID_Bar_ASYM1.c_str())==7.0 ){
						TAG_elID = true;
					}
				}
			}
			else if(electron->isEE()){
				if(electron->isElectronIDAvailable(ElID_End_ASYM1.c_str())){ 
					if(electron->electronID(ElID_End_ASYM1.c_str())==1.0 || 
				   	   electron->electronID(ElID_End_ASYM1.c_str())==3.0 || 
				   	   electron->electronID(ElID_End_ASYM1.c_str())==5.0 ||
				   	   electron->electronID(ElID_End_ASYM1.c_str())==7.0 ){
						TAG_elID = true;
					}
				}
			}
		}

/*
		//ElID Photon
		if(ElIDPhoton){
			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID

			float sigmaIEtaIEta = electron->sigmaIetaIeta();
			//float sigmaIPhiIPhi = electron->sigmaIphiIphi();
			float HoverE = electron->hadronicOverEm();

			if(electron->isEB()){
				if((sigmaIEtaIEta > sigmaIEtaIEta_EB_L_ASYM1 &&
				    sigmaIEtaIEta < sigmaIEtaIEta_EB_H_ASYM1) &&
				   //sigmaIPhiIPhi > sigmaIPhiIPhi_EB_L_ASYM1 &&
				   HoverE < HoverE_EB_ASYM1){
					TAG_elIDPhoton = true;
				}
			}
			else if(electron->isEE()){
				if(sigmaIEtaIEta < sigmaIEtaIEta_EE_ASYM1 &&
				   HoverE < HoverE_EE_ASYM1){
					TAG_elIDPhoton = true;
				}
			}
			else{
				std::cout << "ERROR! The electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:singleEl_Tag_ASYM1") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}
		}
*/

		//Conv
		if(Conv){
			if(electron->isEB()){
				if(electron->isElectronIDAvailable(ElID_Bar_ASYM1.c_str())){ 
					if(electron->electronID(ElID_Bar_ASYM1.c_str())==4.0 || 
				   	   electron->electronID(ElID_Bar_ASYM1.c_str())==5.0 || 
				   	   electron->electronID(ElID_Bar_ASYM1.c_str())==6.0 ||
				   	   electron->electronID(ElID_Bar_ASYM1.c_str())==7.0 ){
						TAG_conv = true;
					}
				}
			}
			else if(electron->isEE()){
				if(electron->isElectronIDAvailable(ElID_End_ASYM1.c_str())){ 
					if(electron->electronID(ElID_End_ASYM1.c_str())==4.0 || 
				   	   electron->electronID(ElID_End_ASYM1.c_str())==5.0 || 
				   	   electron->electronID(ElID_End_ASYM1.c_str())==6.0 ||
				   	   electron->electronID(ElID_End_ASYM1.c_str())==7.0 ){
						TAG_conv = true;
					}
				}
			}
		}

		return TAG_acc &&
		       TAG_imp &&
		       TAG_qual &&
		       TAG_iso &&
//		       TAG_isoPhoton &&
		       TAG_elID &&
//		       TAG_elIDPhoton &&
		       TAG_conv;
	}
	else{
		return false;
	}
}
/*old
inline bool singleEl_Tag_ASYM1(const reco::Candidate& cand, int run){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){ 
		const reco::GsfTrackRef track = electron->gsfTrack();
		assert(track.isNonnull());
		bool TAG_EiD = true;
		if(electron->isElectronIDAvailable(ASYM1_TagEiD.c_str())){
			TAG_EiD = false;
			if(electron->electronID(ASYM1_TagEiD.c_str())==1.0){
				TAG_EiD = true;
			}
		}

		return electron->pt() > ASYM1_TAG_ptelcut && 
		       fabs(electron->eta()) < ASYM1_TAG_etaelcut &&
		       (fabs(electron->eta()) < ASYM1_TAG_eta_el_excl_down || fabs(electron->eta()) > ASYM1_TAG_eta_el_excl_up) &&
		       track->numberOfValidHits() > ASYM1_TAG_minnhit && 
		       track->normalizedChi2() < ASYM1_TAG_maxchi2 &&
		       electron->dB() < ASYM1_TAG_dxycut &&
		       (electron->hcalIso() + electron->ecalIso() + electron->trackIso()) / electron->pt() < ASYM1_TAG_isocut &&
		       TAG_EiD;
	}
	else{
		return false;
	}
}
*/


// Probe cuts SYM
// Single Electron cut - Acceptance (geometrical and kinematic): SYM
inline bool singleEl_Probe_Acc_SYM(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){
		return electron->pt() >= SYM_PRO_ptelcut && 
		       fabs(electron->eta()) <= SYM_PRO_etaelcut &&
		       !(electron->isEBEEGap());
		       //(fabs(electron->eta())<eta_el_excl_down || fabs(electron->eta())>eta_el_excl_up);
	}
	else{
		return false;
	}
}


// Single Electron cut - Trigger: SYM
inline bool singleEl_Probe_Trg_SYM(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	bool TPTrgMatch=true;

	if(elTrgMatchReq){
		TPTrgMatch=false;
		if(electron){
			if(RecSelected_TrgMatch(*electron, run))
			TPTrgMatch=true;
		}
		else{
			TPTrgMatch=false;
		}
	}
 
	return TPTrgMatch; 
}
/*old
inline bool singleEl_Probe_Trg_SYM(const reco::Candidate& cand, int run){
	const pat::Electron* electron = CloneCandidate(cand);
	bool TPTrgMatch=true;
	if(elTrgMatchReq){
		TPTrgMatch=false;
		if(electron){
			if(RecSelected_TrgMatch(*electron, run)){
				TPTrgMatch=true;
			}
		}
		else{
			TPTrgMatch=false;
		}
	}
	return TPTrgMatch; 
}
*/


// Single Electron cut - Impact Parameter: SYM
inline bool singleEl_Probe_Imp_SYM(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){
		return electron->dB() < SYM_PRO_dxycut;
	}
	else{
		return false;
	}
}
/*old
inline bool singleEl_Probe_Imp(const reco::Candidate& cand, int run){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){
		return electron->dB() < dxycut;
	}
	else{
		return false;
	}
}
*/


// Single Electron cut - Quality: SYM
inline bool singleEl_Probe_Qual_SYM(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	bool qual = false;

	if(electron){

		const reco::GsfTrackRef track = electron->gsfTrack();
		assert(track.isNonnull());
		if(track->numberOfValidHits() > SYM_PRO_minVaHit && 
		   track->normalizedChi2() < SYM_PRO_maxchi2){
			qual = true;
		}

		return qual;
	}
	else{
		return false;
	}
}


/*
// Single Electron cut - Isolation with rho: SYM
inline bool singleEl_Probe_Iso_SYM(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	bool iso = false;

	if(electron){
 
		float trackIso = electron->dr03TkSumPt();
		float ecalIso = electron->dr03EcalRecHitSumEt();
		float hcalIso = electron->dr03HcalTowerSumEt();

		float combinedIso03Rho;

		if (electron->isEB()){
			combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEB*(rho)) + max(0.,hcalIso - cAhcalHE*(rho)))/electron->pt();
		}
		else if (electron->isEE()){
			combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEE*(rho)) + max(0.,hcalIso - cAhcalEE*(rho)))/electron->pt();
		}
		else{
			std::cout << "ERROR! The electron is not either in Barrel nor in Endcaps"<< std::endl;
			throw cms::Exception("PATAnalysis:singleEl_IsoRho") << "ERROR! electron Barrel or Endcaps not found ";
			return false;
		}
						
		if(combinedIso03Rho <= SYM_PRO_electronCombinedIsoRhoCut){ 
			iso = true;
		} 
		return iso;
	}
	else{
		return false;
	}

}
*/

/*
// Single Electron cut - Isolation without rho: SYM
inline bool singleEl_Probe_Iso_SYM_mod(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	bool iso = false;

	if(electron){
 
		float trackIso = electron->dr03TkSumPt();
		float ecalIso = electron->dr03EcalRecHitSumEt();
		float hcalIso = electron->dr03HcalTowerSumEt();

		float combinedIso03;

		if (electron->isEB()){
			combinedIso03 = (trackIso + max(0. ,ecalIso - 1.) + hcalIso) / electron->pt();
		}
		else if (electron->isEE()){
			combinedIso03 = (trackIso + ecalIso + hcalIso) / electron->pt();
		}
		else{
			std::cout << "ERROR! The Probe electron is not either in Barrel nor in Endcaps"<< std::endl;
			throw cms::Exception("PATAnalysis:singleEl_Probe_Iso_SYM_mod") << "ERROR! electron Barrel or Endcaps not found ";
			return false;
		}
						
		if(combinedIso03 <= SYM_PRO_electronCombinedIsoCut){ 
			iso = true;
		} 
		return iso;
	}
	else{
		return false;
	}
}
*/

// Single Electron cut - Isolation with rho Photon: SYM
inline bool singleEl_Probe_Iso_SYM(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	double rho25 = rho; 			

	bool isoPhoton = false;

	if(electron){
 
		double Et = electron->et();

		float HollowConeTrackIso = electron->dr04TkSumPt();

		float JurrasicECALIso = electron->dr04EcalRecHitSumEt();

		float TowerBasedHCALIso = electron->dr04HcalTowerSumEt();
		
		float AbsTrackIsoRho; 
		float AbsECALIsoRho; 
		float AbsHCALIsoRho;

		if (electron->isEB()){
			AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EB*(rho25));
			AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EB*(rho25));
			AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EB*(rho25));
		}
		else if (electron->isEE()){
			AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EE*(rho25));
			AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EE*(rho25));
			AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EE*(rho25));
		}
		else{
			std::cout << "ERROR! The electron is not either in Barrel nor in Endcaps"<< std::endl;
			throw cms::Exception("PATAnalysis:RecSelected, singleEl_Probe_IsoPhoton_SYM") << "ERROR! electron Barrel or Endcaps not found ";
			return false;
		}

		if(HollowConeTrackIso < AbsTrackIsoRho && 
		   JurrasicECALIso < AbsECALIsoRho && 
		   TowerBasedHCALIso < AbsHCALIsoRho){
			isoPhoton = true;
		} 
		return isoPhoton;
	}
	else{
		return false;
	}
}
/*old
inline bool singleEl_Probe_Iso_SYM(const reco::Candidate& cand, int run){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){
		return (electron->hcalIso() + electron->ecalIso() + electron->trackIso())/electron->pt() < sym_isocut;
	}
	else{
		return false;
	}
}
*/

/*
// Single Electron cut - Electron ID: SYM
inline bool singleEl_Probe_ElID_SYM(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){

		bool electron_ID = false; 
		if(electron->isEB()){
			if(electron->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
				if(electron->electronID(ElID_Bar_SYM.c_str())==1.0 || 
			   	   electron->electronID(ElID_Bar_SYM.c_str())==3.0 || 
			   	   electron->electronID(ElID_Bar_SYM.c_str())==5.0 ||
			   	   electron->electronID(ElID_Bar_SYM.c_str())==7.0 ){
					electron_ID = true;
				}
			}
		}
		else if(electron->isEE()){
			if(electron->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
				if(electron->electronID(ElID_End_SYM.c_str())==1.0 || 
			   	   electron->electronID(ElID_End_SYM.c_str())==3.0 || 
			   	   electron->electronID(ElID_End_SYM.c_str())==5.0 ||
			   	   electron->electronID(ElID_End_SYM.c_str())==7.0 ){
					electron_ID = true;
				}
			}
		}
		return electron_ID;
	}
	else{
		return false;
	}
}
*/

/*old
inline bool singleEl_Probe_EiD_SYM(const reco::Candidate& cand, int run){
	const pat::Electron* electron = CloneCandidate(cand);
	if(electron){
		bool electron_ID = false;
		if(electron->isElectronIDAvailable(SYM_TagEiD.c_str())){
			if(electron->electronID(SYM_TagEiD.c_str())==1.0){
				electron_ID = true;
			}
		}
		return electron_ID;
	}
	else{
		return false;
	}
}
*/

// Single Electron cut - Electron ID Photon: SYM
inline bool singleEl_Probe_ElID_SYM(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){

		bool electron_IDPhoton = false;

			float sigmaIEtaIEta = electron->sigmaIetaIeta();
			//float sigmaIPhiIPhi = electron->sigmaIphiIphi();
			float HoverE = electron->hadronicOverEm();

			if(electron->isEB()){
				if((sigmaIEtaIEta > sigmaIEtaIEta_EB_L_SYM &&
				    sigmaIEtaIEta < sigmaIEtaIEta_EB_H_SYM) &&
				   //sigmaIPhiIPhi > sigmaIPhiIPhi_EB_L_SYM &&
				   HoverE < HoverE_EB_SYM){
					electron_IDPhoton = true;
				}
			}
			else if(electron->isEE()){
				if(sigmaIEtaIEta < sigmaIEtaIEta_EE_SYM &&
				   HoverE < HoverE_EE_SYM){
					electron_IDPhoton = true;
				}
			}
			else{
				std::cout << "ERROR! The electron Probe is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis:singleEl_Probe_ElIDPhoton_SYM") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

		return electron_IDPhoton;
	}
	else{
		return false;
	}

}


// Single Electron cut - Electron Conv: SYM
inline bool singleEl_Probe_Conv_SYM(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){

		bool conv = false; 
		if(electron->isEB()){
			if(electron->isElectronIDAvailable(ElID_Bar_SYM.c_str())){ 
				if(electron->electronID(ElID_Bar_SYM.c_str())==4.0 || 
			   	   electron->electronID(ElID_Bar_SYM.c_str())==5.0 || 
			   	   electron->electronID(ElID_Bar_SYM.c_str())==6.0 ||
			   	   electron->electronID(ElID_Bar_SYM.c_str())==7.0 ){
					conv = true;
				}
			}
		}
		else if(electron->isEE()){
			if(electron->isElectronIDAvailable(ElID_End_SYM.c_str())){ 
				if(electron->electronID(ElID_End_SYM.c_str())==4.0 || 
			   	   electron->electronID(ElID_End_SYM.c_str())==5.0 || 
			   	   electron->electronID(ElID_End_SYM.c_str())==6.0 ||
			   	   electron->electronID(ElID_End_SYM.c_str())==7.0 ){
					conv = true;
				}
			}
		}
		return conv;
	}
	else{
		return false;
	}
}


// Probe cuts ASYM
// Single Electron cut - Acceptance (geometrical and kinematic): ASYM, leg0
inline bool singleEl_Probe_Acc_ASYM0(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el0 = CloneCandidate(cand);

	if(el0){
		return el0->pt() >= ASYM0_PRO_ptelcut && 
		       fabs(el0->eta()) <= ASYM0_PRO_etaelcut &&
		       !(el0->isEBEEGap());
		       //(fabs(el0->eta()) < eta_el_excl_down || fabs(el0->eta()) > eta_el_excl_up);
	}
	else{
		return false;
	}
}
/*old
inline bool singleEl_Probe_Acc_ASYM0(const reco::Candidate& cand, int run){
	const pat::Electron* el0 = CloneCandidate(cand);
  
	if(el0){
		return el0->pt() > ptelcut0 &&
		       fabs(el0->eta()) < etaelcut && 
		       (fabs(el0->eta())<eta_el_excl_down || fabs(el0->eta())>eta_el_excl_up));
	}
	else{
		return false;
	}
}
*/  

// Single Electron cut - Acceptance (geometrical and kinematic): ASYM, leg1
inline bool singleEl_Probe_Acc_ASYM1(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el1 = CloneCandidate(cand);

	if(el1){
		return el1->pt() >= ASYM1_PRO_ptelcut && 
		       fabs(el1->eta()) <= ASYM1_PRO_etaelcut &&
		       !(el1->isEBEEGap());
		       //(fabs(el1->eta()) < eta_el_excl_down || fabs(el1->eta()) > eta_el_excl_up);
	}
	else{
		return false;
	}
}
/*old
inline bool singleEl_Probe_Acc_ASYM1(const reco::Candidate& cand, int run){
const pat::Electron* el1 = CloneCandidate(cand);
  
	if(el1){
		return el1->pt() > ptelcut1 &&
		       fabs(el1->eta()) < etaelcut &&
		       (fabs(el1->eta())<eta_el_excl_down || fabs(el1->eta())>eta_el_excl_up);
	}
	else{
		return false;
	}
}
*/

  
// Single Electron cut - Trigger: ASYM, leg0
inline bool singleEl_Probe_Trg_ASYM0(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el0 = CloneCandidate(cand);
	bool TPTrgMatch=true;

	if(elTrgMatchReq){
		TPTrgMatch=false;
		if(el0){
			//if(RecSelected_TrgMatch(*el0, run))
			TPTrgMatch=true;
		}
		else{
			TPTrgMatch=false;
		}
	}
	return TPTrgMatch; 
}
/*old
inline bool singleEl_Probe_Trg_ASYM0(const reco::Candidate& cand, int run){

	const pat::Electron* el0 = CloneCandidate(cand);
	bool TPTrgMatch=true;
  
	if(elTrgMatchReq){
		TPTrgMatch=false;
		if(el0){
			if(RecSelected_TrgMatch(*el0, run)){
				TPTrgMatch=true;
			}
		}
		else{
			TPTrgMatch=false;
		}
	}
	return TPTrgMatch; 
}  
*/


//Per il momento non applico il probe Trg al secondo elettrone
// Single Electron cut - Trigger: ASYM, leg1
inline bool singleEl_Probe_Trg_ASYM1(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el1 = CloneCandidate(cand);
  
	if(el1){
		return true;
	}
	else{
		return false;
	}
}

// Single Electron cut - Impact Parameter: ASYM0
inline bool singleEl_Probe_Imp_ASYM0(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){
		return electron->dB() < ASYM0_PRO_dxycut;
	}
	else{
		return false;
	}
}

// Single Electron cut - Impact Parameter: ASYM1
inline bool singleEl_Probe_Imp_ASYM1(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	if(electron){
		return electron->dB() < ASYM1_PRO_dxycut;
	}
	else{
		return false;
	}
}
  
// Single Electron cut - Quality: ASYM, leg0
inline bool singleEl_Probe_Qual_ASYM0(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el0 = CloneCandidate(cand);

	bool qual0 = false;

	if(el0){

		const reco::GsfTrackRef track = el0->gsfTrack();
		assert(track.isNonnull());
		if(track->numberOfValidHits() > ASYM0_PRO_minVaHit && 
		   track->normalizedChi2() < ASYM0_PRO_maxchi2){
			qual0 = true;
		}
		return qual0;
	}
	else{
		return false;
	}
}


// Single Electron cut - Quality: ASYM, leg1
inline bool singleEl_Probe_Qual_ASYM1(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el1 = CloneCandidate(cand);

	bool qual1 = false;

	if(el1){

		const reco::GsfTrackRef track = el1->gsfTrack();
		assert(track.isNonnull());
		if(track->numberOfValidHits() > ASYM1_PRO_minVaHit && 
		   track->normalizedChi2() < ASYM1_PRO_maxchi2){
			qual1 = true;
		}
		return qual1;
	}
	else{
		return false;
	}
}

/*
// Single Electron cut - Isolation with rho: ASYM, leg0
inline bool singleEl_Probe_Iso_ASYM0(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el0 = CloneCandidate(cand);

	bool iso0 = false;

	if(el0){
 
		float trackIso = el0->dr03TkSumPt();
		float ecalIso = el0->dr03EcalRecHitSumEt();
		float hcalIso = el0->dr03HcalTowerSumEt();

		float combinedIso03Rho;

		if (el0->isEB()){
			combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEB*(rho)) + max(0.,hcalIso - cAhcalHE*(rho)))/el0->pt();
		}
		else if (el0->isEE()){
			combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEE*(rho)) + max(0.,hcalIso - cAhcalEE*(rho)))/el0->pt();
		}
		else{
			std::cout << "ERROR! The Probe electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
			throw cms::Exception("PATAnalysis:singleEl_Probe_IsoASYM0") << "ERROR! electron Barrel or Endcaps not found ";
			return false;
		}
						
		if(combinedIso03Rho <= ASYM0_PRO_electronCombinedIsoRhoCut){ 
			iso0 = true;
		} 
		return iso0;
	}
	else{
		return false;
	}
}
*/

/*
// Single Electron cut - Isolation with rho: ASYM, leg1
inline bool singleEl_Probe_Iso_ASYM1(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el1 = CloneCandidate(cand);

	bool iso1 = false;

	if(el1){
 
		float trackIso = el1->dr03TkSumPt();
		float ecalIso = el1->dr03EcalRecHitSumEt();
		float hcalIso = el1->dr03HcalTowerSumEt();

		float combinedIso03Rho;

		if (el1->isEB()){
			combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEB*(rho)) + max(0.,hcalIso - cAhcalHE*(rho)))/el1->pt();
		}
		else if (el1->isEE()){
			combinedIso03Rho = (trackIso + max(0. ,ecalIso - cAecalEE*(rho)) + max(0.,hcalIso - cAhcalEE*(rho)))/el1->pt();
		}
		else{
			std::cout << "ERROR! The Probe electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
			throw cms::Exception("PATAnalysis:singleEl_Probe_IsoASYM1") << "ERROR! electron Barrel or Endcaps not found ";
			return false;
		}
						
		if(combinedIso03Rho <= ASYM1_PRO_electronCombinedIsoRhoCut){ 
			iso1 = true;
		} 
		return iso1;
	}
	else{
		return false;
	}
}
*/

/*
// Single Electron cut - Isolation without rho: ASYM, leg0
inline bool singleEl_Probe_Iso_ASYM0_mod(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el0 = CloneCandidate(cand);

	bool iso0 = false;

	if(el0){
 
		float trackIso = el0->dr03TkSumPt();
		float ecalIso = el0->dr03EcalRecHitSumEt();
		float hcalIso = el0->dr03HcalTowerSumEt();

		float combinedIso03;

		if (el0->isEB()){
			combinedIso03 = (trackIso + max(0. ,ecalIso - 1.) + hcalIso) / el0->pt();
		}
		else if (el0->isEE()){
			combinedIso03 = (trackIso + ecalIso + hcalIso) / el0->pt();
		}
		else{
			std::cout << "ERROR! The Probe electron 0 is not either in Barrel nor in Endcaps"<< std::endl;
			throw cms::Exception("PATAnalysis:singleEl_Probe_IsoASYM0") << "ERROR! electron Barrel or Endcaps not found ";
			return false;
		}
						
		if(combinedIso03 <= ASYM0_PRO_electronCombinedIsoCut){ 
			iso0 = true;
		} 
		return iso0;
	}
	else{
		return false;
	}
}
*/
/*old
inline bool singleEl_Probe_Iso_ASYM0(const reco::Candidate& cand, int run){
	const pat::Electron* el0 = CloneCandidate(cand);
	if(el0){
		bool el0_ID = false;
		if(el0->isElectronIDAvailable(eID_ASYM0.c_str())){
			if(el0->electronID(eID_ASYM0.c_str())==7.0 || 
			   el0->electronID(eID_ASYM0.c_str())==2.0 || 
			   el0->electronID(eID_ASYM0.c_str())==3.0 || 
			   el0->electronID(eID_ASYM0.c_str())==6.0){
				el0_ID = true;
			}
		}
		return el0_ID;
	}
	else{
		return false;
	}
}
*/

/*
// Single Electron cut - Isolation without rho: ASYM, leg1
inline bool singleEl_Probe_Iso_ASYM1_mod(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el1 = CloneCandidate(cand);

	bool iso1 = false;

	if(el1){
 
		float trackIso = el1->dr03TkSumPt();
		float ecalIso = el1->dr03EcalRecHitSumEt();
		float hcalIso = el1->dr03HcalTowerSumEt();

		float combinedIso03;

		if (el1->isEB()){
			combinedIso03 = (trackIso + max(0. ,ecalIso - 1.) + hcalIso) / el1->pt();
		}
		else if (el1->isEE()){
			combinedIso03 = (trackIso + ecalIso + hcalIso) / el1->pt();
		}
		else{
			std::cout << "ERROR! The Probe electron 1 is not either in Barrel nor in Endcaps"<< std::endl;
			throw cms::Exception("PATAnalysis:singleEl_Probe_IsoASYM1") << "ERROR! electron Barrel or Endcaps not found ";
			return false;
		}
						
		if(combinedIso03 <= ASYM1_PRO_electronCombinedIsoCut){ 
			iso1 = true;
		} 
		return iso1;
	}
	else{
		return false;
	}
}
*/
/*old
inline bool singleEl_Probe_Iso_ASYM1(const reco::Candidate& cand, int run){
	const pat::Electron* el1 = CloneCandidate(cand);
	if(el1){
		bool el1_ID = false;
		if(el1->isElectronIDAvailable(eID_ASYM1.c_str())){
			if(el1->electronID(eID_ASYM1.c_str())==7.0 || 
			   el1->electronID(eID_ASYM1.c_str())==2.0 || 
			   el1->electronID(eID_ASYM1.c_str())==3.0 || 
			   el1->electronID(eID_ASYM1.c_str())==6.0){
				el1_ID = true;
			}
		}
		return el1_ID;
	}
	else{
		return false;
	}
}
*/

// Single Electron cut - Isolation with rho Photon: ASYM, leg0
inline bool singleEl_Probe_Iso_ASYM0(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	double rho25 = rho; 			

	bool isoPhoton = false;

	if(electron){
 
		double Et = electron->et();

		float HollowConeTrackIso = electron->dr04TkSumPt();

		float JurrasicECALIso = electron->dr04EcalRecHitSumEt();

		float TowerBasedHCALIso = electron->dr04HcalTowerSumEt();
		
		float AbsTrackIsoRho; 
		float AbsECALIsoRho; 
		float AbsHCALIsoRho;

		if (electron->isEB()){
			AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EB*(rho25));
			AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EB*(rho25));
			AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EB*(rho25));
		}
		else if (electron->isEE()){
			AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EE*(rho25));
			AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EE*(rho25));
			AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EE*(rho25));
		}
		else{
			std::cout << "ERROR! The electron is not either in Barrel nor in Endcaps"<< std::endl;
			throw cms::Exception("PATAnalysis:RecSelected, singleEl_Probe_IsoPhoton_ASYM0") << "ERROR! electron Barrel or Endcaps not found ";
			return false;
		}

		if(HollowConeTrackIso < AbsTrackIsoRho && 
		   JurrasicECALIso < AbsECALIsoRho && 
		   TowerBasedHCALIso < AbsHCALIsoRho){
			isoPhoton = true;
		} 
		return isoPhoton;
	}
	else{
		return false;
	}
}


// Single Electron cut - Isolation with rho Photon: ASYM, leg1
inline bool singleEl_Probe_Iso_ASYM1(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* electron = CloneCandidate(cand);

	double rho25 = rho; 			

	bool isoPhoton = false;

	if(electron){
 
		double Et = electron->et();

		float HollowConeTrackIso = electron->dr04TkSumPt();

		float JurrasicECALIso = electron->dr04EcalRecHitSumEt();

		float TowerBasedHCALIso = electron->dr04HcalTowerSumEt();
		
		float AbsTrackIsoRho; 
		float AbsECALIsoRho; 
		float AbsHCALIsoRho;

		if (electron->isEB()){
			AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EB*(rho25));
			AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EB*(rho25));
			AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EB*(rho25));
		}
		else if (electron->isEE()){
			AbsTrackIsoRho = (cTrack_A + cTrack_B*(Et) + cTrack_C_EE*(rho25));
			AbsECALIsoRho = (cECAL_A + cECAL_B*(Et) + cECAL_C_EE*(rho25));
			AbsHCALIsoRho = (cHCAL_A + cHCAL_B*(Et) + cHCAL_C_EE*(rho25));
		}
		else{
			std::cout << "ERROR! The electron is not either in Barrel nor in Endcaps"<< std::endl;
			throw cms::Exception("PATAnalysis:RecSelected, singleEl_Probe_IsoPhoton_ASYM1") << "ERROR! electron Barrel or Endcaps not found ";
			return false;
		}

		if(HollowConeTrackIso < AbsTrackIsoRho && 
		   JurrasicECALIso < AbsECALIsoRho && 
		   TowerBasedHCALIso < AbsHCALIsoRho){
			isoPhoton = true;
		} 
		return isoPhoton;
	}
	else{
		return false;
	}
}
/*old
inline bool singleEl_Probe_Iso_ASYM0(const reco::Candidate& cand, int run){
	const pat::Electron* el0 = CloneCandidate(cand);
	if(el0){
		bool el0_ID = false;
		if(el0->isElectronIDAvailable(eID_ASYM0.c_str())){
			if(el0->electronID(eID_ASYM0.c_str())==7.0 || 
			   el0->electronID(eID_ASYM0.c_str())==2.0 || 
			   el0->electronID(eID_ASYM0.c_str())==3.0 || 
			   el0->electronID(eID_ASYM0.c_str())==6.0){
				el0_ID = true;
			}
		}
		return el0_ID;
	}
	else{
		return false;
	}
}
*/

/*
// Single Electron cut - Electron ID: ASYM, leg0
inline bool singleEl_Probe_ElID_ASYM0(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el0 = CloneCandidate(cand);
  
	if(el0){
		bool el0_ID = false;

		if(el0->isEB()){
			if(el0->isElectronIDAvailable(ElID_Bar_ASYM0.c_str())){ 
				if(el0->electronID(ElID_Bar_ASYM0.c_str())==1.0 || 
			   	   el0->electronID(ElID_Bar_ASYM0.c_str())==3.0 || 
			   	   el0->electronID(ElID_Bar_ASYM0.c_str())==5.0 ||
			   	   el0->electronID(ElID_Bar_ASYM0.c_str())==7.0 ){
					el0_ID = true;
				}
			}
		}
		else if(el0->isEE()){
			if(el0->isElectronIDAvailable(ElID_End_ASYM0.c_str())){ 
				if(el0->electronID(ElID_End_ASYM0.c_str())==1.0 || 
			   	   el0->electronID(ElID_End_ASYM0.c_str())==3.0 || 
			   	   el0->electronID(ElID_End_ASYM0.c_str())==5.0 ||
			   	   el0->electronID(ElID_End_ASYM0.c_str())==7.0 ){
					el0_ID = true;
				}
			}
		}
		return el0_ID;
	}
	else{
		return false;
	}
}
*/

/*
// Single Electron cut - Electron ID: ASYM, leg1
inline bool singleEl_Probe_ElID_ASYM1(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el1 = CloneCandidate(cand);
  
	if(el1){
		bool el1_ID = false;

		if(el1->isEB()){
			if(el1->isElectronIDAvailable(ElID_Bar_ASYM1.c_str())){ 
				if(el1->electronID(ElID_Bar_ASYM1.c_str())==1.0 || 
			   	   el1->electronID(ElID_Bar_ASYM1.c_str())==3.0 || 
			   	   el1->electronID(ElID_Bar_ASYM1.c_str())==5.0 ||
			   	   el1->electronID(ElID_Bar_ASYM1.c_str())==7.0 ){
					el1_ID = true;
				}
			}
		}
		else if(el1->isEE()){
			if(el1->isElectronIDAvailable(ElID_End_ASYM1.c_str())){ 
				if(el1->electronID(ElID_End_ASYM1.c_str())==1.0 || 
			   	   el1->electronID(ElID_End_ASYM1.c_str())==3.0 || 
			   	   el1->electronID(ElID_End_ASYM1.c_str())==5.0 ||
			   	   el1->electronID(ElID_End_ASYM1.c_str())==7.0 ){
					el1_ID = true;
				}
			}
		}
		return el1_ID;
	}
	else{
		return false;
	}
}
*/

// Single Electron cut - Electron ID Photon: ASYM, leg0
inline bool singleEl_Probe_ElID_ASYM0(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el0 = CloneCandidate(cand);

	if(el0){

		bool el0_IDPhoton = false;

			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID

			float sigmaIEtaIEta = el0->sigmaIetaIeta();
			//float sigmaIPhiIPhi = el0->sigmaIphiIphi();
			float HoverE = el0->hadronicOverEm();

			if(el0->isEB()){
				if((sigmaIEtaIEta > sigmaIEtaIEta_EB_L_ASYM0 &&
				    sigmaIEtaIEta < sigmaIEtaIEta_EB_H_ASYM0) &&
				   //sigmaIPhiIPhi > sigmaIPhiIPhi_EB_L_ASYM0 &&
				   HoverE < HoverE_EB_ASYM0){
					el0_IDPhoton = true;
				}
			}
			else if(el0->isEE()){
				if(sigmaIEtaIEta < sigmaIEtaIEta_EE_ASYM0 &&
				   HoverE < HoverE_EE_ASYM0){
					el0_IDPhoton = true;
				}
			}
			else{
				std::cout << "ERROR! The electron probe is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis: singleEl_Probe_ElIDPhoton_ASYM0") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

		return el0_IDPhoton;
	}
	else{
		return false;
	}

}


// Single Electron cut - Electron ID Photon: ASYM, leg1
inline bool singleEl_Probe_ElID_ASYM1(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el1 = CloneCandidate(cand);

	if(el1){

		bool el1_IDPhoton = false;

			//https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID

			float sigmaIEtaIEta = el1->sigmaIetaIeta();
			//float sigmaIPhiIPhi = el1->sigmaIphiIphi();
			float HoverE = el1->hadronicOverEm();

			if(el1->isEB()){
				if((sigmaIEtaIEta > sigmaIEtaIEta_EB_L_ASYM1 &&
				    sigmaIEtaIEta < sigmaIEtaIEta_EB_H_ASYM1) &&
				   //sigmaIPhiIPhi > sigmaIPhiIPhi_EB_L_ASYM1 &&
				   HoverE < HoverE_EB_ASYM1){
					el1_IDPhoton = true;
				}
			}
			else if(el1->isEE()){
				if(sigmaIEtaIEta < sigmaIEtaIEta_EE_ASYM1 &&
				   HoverE < HoverE_EE_ASYM1){
					el1_IDPhoton = true;
				}
			}
			else{
				std::cout << "ERROR! The electron probe is not either in Barrel nor in Endcaps"<< std::endl;
				throw cms::Exception("PATAnalysis: singleEl_Probe_ElIDPhoton_ASYM1") << "ERROR! electron Barrel or Endcaps not found ";
				return false;
			}

		return el1_IDPhoton;
	}
	else{
		return false;
	}
}

// Single Electron cut - Electron Conv: ASYM, leg0
inline bool singleEl_Probe_Conv_ASYM0(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el0 = CloneCandidate(cand);

	if(el0){

		bool conv = false; 
		if(el0->isEB()){
			if(el0->isElectronIDAvailable(ElID_Bar_ASYM0.c_str())){ 
				if(el0->electronID(ElID_Bar_ASYM0.c_str())==4.0 || 
			   	   el0->electronID(ElID_Bar_ASYM0.c_str())==5.0 || 
			   	   el0->electronID(ElID_Bar_ASYM0.c_str())==6.0 ||
			   	   el0->electronID(ElID_Bar_ASYM0.c_str())==7.0 ){
					conv = true;
				}
			}
		}
		else if(el0->isEE()){
			if(el0->isElectronIDAvailable(ElID_End_ASYM0.c_str())){ 
				if(el0->electronID(ElID_End_ASYM0.c_str())==4.0 || 
			   	   el0->electronID(ElID_End_ASYM0.c_str())==5.0 || 
			   	   el0->electronID(ElID_End_ASYM0.c_str())==6.0 ||
			   	   el0->electronID(ElID_End_ASYM0.c_str())==7.0 ){
					conv = true;
				}
			}
		}
		return conv;
	}
	else{
		return false;
	}
}


// Single Electron cut - Electron Conv: ASYM, leg1
inline bool singleEl_Probe_Conv_ASYM1(const reco::Candidate& cand, int run, double rho){

	const pat::Electron* el1 = CloneCandidate(cand);
  
	if(el1){

		bool conv = false; 
		if(el1->isEB()){
			if(el1->isElectronIDAvailable(ElID_Bar_ASYM1.c_str())){ 
				if(el1->electronID(ElID_Bar_ASYM1.c_str())==4.0 || 
			   	   el1->electronID(ElID_Bar_ASYM1.c_str())==5.0 || 
			   	   el1->electronID(ElID_Bar_ASYM1.c_str())==6.0 ||
			   	   el1->electronID(ElID_Bar_ASYM1.c_str())==7.0 ){
					conv = true;
				}
			}
		}
		else if(el1->isEE()){
			if(el1->isElectronIDAvailable(ElID_End_ASYM1.c_str())){ 
				if(el1->electronID(ElID_End_ASYM1.c_str())==4.0 || 
			   	   el1->electronID(ElID_End_ASYM1.c_str())==5.0 || 
			   	   el1->electronID(ElID_End_ASYM1.c_str())==6.0 ||
			   	   el1->electronID(ElID_End_ASYM1.c_str())==7.0 ){
					conv = true;
				}
			}
		}
		return conv;
	}
	else{
		return false;
	}
}


// Single Electron cut - True
inline bool singleEl_Probe_True(const reco::Candidate& cand, int run, double rho){

	return true;
}


// Single Electron cut - All Selection: SYM
inline bool singleEl_Tag_AllSel_SYM(const reco::Candidate& cand, int run, double rho){
   
	return singleEl_Probe_Acc_SYM(cand, run, rho) && 
	       singleEl_Probe_Trg_SYM(cand, run, rho) && 
	       //singleEl_Probe_Imp(cand, run, rho) &&
	       //singleEl_Probe_Qual_SYM(cand, run, rho) &&
	       //singleEl_Probe_Conv_SYM(cand, run, rho) && 
	       singleEl_Probe_Iso_SYM(cand, run, rho) &&
//	       singleEl_Probe_IsoPhoton_SYM(cand, run, rho) &&
	       singleEl_Probe_ElID_SYM(cand, run, rho);
//	       singleEl_Probe_ElIDPhoton_SYM(cand, run, rho);
}


// Single Electron cut - All Selection: ASYM, leg0
inline bool singleEl_Tag_AllSel_ASYM0(const reco::Candidate& cand, int run, double rho){
   
	return singleEl_Probe_Acc_ASYM0(cand, run, rho) && 
	       singleEl_Probe_Trg_ASYM0(cand, run, rho) && 
	       //singleEl_Probe_Imp(cand, run, rho) &&
	       //singleEl_Probe_Qual_ASYM0(cand, run, rho) && 
	       //singleEl_Probe_Conv_ASYM0(cand, run, rho) && 
	       singleEl_Probe_Iso_ASYM0(cand, run, rho) && 
//	       singleEl_Probe_IsoPhoton_ASYM0(cand, run, rho) && 
	       singleEl_Probe_ElID_ASYM0(cand, run, rho);
//	       singleEl_Probe_ElIDPhoton_ASYM0(cand, run, rho);
}


// Single Electron cut - All Selection: ASYM, leg1
inline bool singleEl_Tag_AllSel_ASYM1(const reco::Candidate& cand, int run, double rho){
   
	return singleEl_Probe_Acc_ASYM1(cand, run, rho) && 
	       singleEl_Probe_Trg_ASYM1(cand, run, rho) && 
	       //singleEl_Probe_Imp(cand, run, rho) && 
	       //singleEl_Probe_Qual_ASYM1(cand, run, rho) &&
	       //singleEl_Probe_Conv_ASYM1(cand, run, rho) && 
	       singleEl_Probe_Iso_ASYM1(cand, run, rho) && 
//	       singleEl_Probe_IsoPhoton_ASYM1(cand, run, rho) && 
	       singleEl_Probe_ElID_ASYM1(cand, run, rho); 
//	       singleEl_Probe_ElIDPhoton_ASYM1(cand, run, rho);
}

    
#endif
