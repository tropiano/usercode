#ifndef RecoElectron_h
#define RecoElectron_h

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"

class RecoElectron : public MyPatAnalysis{
public :
   RecoElectron();
   
   virtual ~RecoElectron();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new RecoElectron(); } 

   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

   //Histograms
   TH1D *recZNum,
   *recPtZ_1, *recEtaZ_1, *recMassZ_1,
   *recPtZ_12, *recEtaZ_12, *recMassZ_12,
   *recPtZ_123, *recEtaZ_123, *recMassZ_123, 
   *recPtZ_1234, *recEtaZ_1234, *recMassZ_1234,
   *recPtZ_12345, *recEtaZ_12345, *recMassZ_12345;
        
   TH1D *recLeadElPt_1, *recSecElPt_1, *recLeadElEta_1, *recSecElEta_1,
   *recLeadElPt_12, *recSecElPt_12, *recLeadElEta_12, *recSecElEta_12,
   *recLeadElPt_123, *recSecElPt_123, *recLeadElEta_123, *recSecElEta_123,
   *recLeadElPt_1234, *recSecElPt_1234, *recLeadElEta_1234, *recSecElEta_1234,
   *recLeadElPt_12345, *recSecElPt_12345, *recLeadElEta_12345, *recSecElEta_12345;
   
   TH1D *recTrackIsoLead_PreIso, *recEcalIsoLead_PreIso, *recHcalIsoLead_PreIso, *recRelIsoLead_PreIso, *recTrackIsoSec_PreIso, *recEcalIsoSec_PreIso, *recHcalIsoSec_PreIso, *recRelIsoSec_PreIso;
   
   TH1D *HoverE_PreEiD, *DeltaEtaIn_PreEiD, *DeltaPhiIn_PreEiD, *SigmaIEtaIEta_PreEiD, *HoverE_EiD, *DeltaEtaIn_EiD, *DeltaPhiIn_EiD, *SigmaIEtaIEta_EiD;
   
   TH1D *RecoJetPt_12345, *RecoIsoJetPt_12345, *recLeadIsoJetPt_12345, *recLeadIsoJetEta_12345, *JetCounter_12345, *IsoJetCounter_1, *IsoJetCounter_12, *IsoJetCounter_123, *IsoJetCounter_1234, *IsoJetCounter_12345;

   TH1D *HEnergy_IsoJet_ElType, *EMEnergy_IsoJet_ElType, *Jet_HEnergy, *Jet_EMEnergy, *MinDeltaR_ZDau, *AllJetCharge, *IsoJetCharge, *NotIsoJetCharge, *DeltaR_IsoJet, *DeltaR_NotIsoJet, *DeltaR_IsoJet_ElType;
   
   TH1D *ChargeMisID_Pt_Acc, *ChargeMisID_Eta_Acc, *ChargeMisID_Hit_Acc, *ChargeMisID_fBrem_Acc, *ChargeMisID_IP_Acc, *CorrectCharge_Pt_Acc, *CorrectCharge_Eta_Acc, *CorrectCharge_Hit_Acc, *CorrectCharge_fBrem_Acc, *CorrectCharge_IP_Acc, *AllEl_Pt_Acc, *AllEl_Eta_Acc, *AllEl_Hit_Acc, *AllEl_fBrem_Acc, *AllEl_IP_Acc;
   
   TH2D *DeltaRvsCharge_JetRec, *DeltaRvsCharge_JetRec_Iso, *DeltaRvsCharge_JetRec_NotIso; 
   
   TProfile *HitVsEta_CorrCharge, *HitVsEta_MisIDCharge, *HitVsEta_AllEl, *fBremVsEta;
        
   std::vector<TH1D*> recJetPtVsInclMulti;
   std::vector<TH1D*> recJetEtaVsInclMulti;

   std::vector<TH1D*> recZPtVsInclMulti;
   std::vector<TH1D*> recZEtaVsInclMulti;

   std::vector<TH1D*> recEl1PtVsInclMulti;
   std::vector<TH1D*> recEl1EtaVsInclMulti;

   std::vector<TH1D*> recEl2PtVsInclMulti;
   std::vector<TH1D*> recEl2EtaVsInclMulti;

   std::vector<TH1D*> recZPtVsExclMulti;
   std::vector<TH1D*> recZEtaVsExclMulti;

   std::vector<TH1D*> recEl1PtVsExclMulti;
   std::vector<TH1D*> recEl1EtaVsExclMulti;

   std::vector<TH1D*> recEl2PtVsExclMulti;
   std::vector<TH1D*> recEl2EtaVsExclMulti;
   
   std::vector<TH1D*> ElCorrChargePtExclMulti;
   std::vector<TH1D*> ElCorrChargeEtaExclMulti;
   std::vector<TH1D*> ElCorrChargeHitExclMulti;
   std::vector<TH1D*> ElCorrChargefBremExclMulti;
   
   std::vector<TH1D*> ElMisIDChargePtExclMulti;
   std::vector<TH1D*> ElMisIDChargeEtaExclMulti;
   std::vector<TH1D*> ElMisIDChargeHitExclMulti;
   std::vector<TH1D*> ElMisIDChargefBremExclMulti;

   std::vector<TH1D*> AllElPtExclMulti;
   std::vector<TH1D*> AllElEtaExclMulti;
   std::vector<TH1D*> AllElHitExclMulti;
   std::vector<TH1D*> AllElfBremExclMulti;
      
 
private:

  double _targetLumi;
  
  double _xsec;
  
  double _norm;
  
  bool _Norm;
  
  bool _Sumw2;
  
  bool _GenParticleMatch;
  
  int _entries, _EventsPerFile, _Acc, _Qual, _Imp, _Iso, _EiD;
  
  std::string _electronID;

  TFile* _file;  

  TDirectory *_dir, *_charge_dir;
  
  std::string _RecoCutFlags[6];

  std::vector<TH1D*> _histoVector;
  std::vector<TH2D*> _histoVector2D;

};
#endif

