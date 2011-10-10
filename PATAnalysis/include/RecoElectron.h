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
   
   TH1D *recZnum, *recZSCnum, *BestMassIndex;
   
   TH1D *recPtZ_1, *recEtaZ_1, *recMassZ_1,
   *recPtZ_12, *recEtaZ_12, *recMassZ_12,
   *recPtZ_123, *recEtaZ_123, *recMassZ_123, 
   *recPtZ_1234, *recEtaZ_1234, *recMassZ_1234,
   *recPtZ_12345, *recEtaZ_12345, *recMassZ_12345,
   *recPtZ_123456, *recEtaZ_123456, *recMassZ_123456;
 
   TH1D *recLeadElPt_1, *recSecElPt_1, *recLeadElEta_1, *recSecElEta_1,
   *recLeadElPt_12, *recSecElPt_12, *recLeadElEta_12, *recSecElEta_12,
   *recLeadElPt_123, *recSecElPt_123, *recLeadElEta_123, *recSecElEta_123,
   *recLeadElPt_1234, *recSecElPt_1234, *recLeadElEta_1234, *recSecElEta_1234,
   *recLeadElPt_12345, *recSecElPt_12345, *recLeadElEta_12345, *recSecElEta_12345,
   *recLeadElPt_123456, *recSecElPt_123456, *recLeadElEta_123456, *recSecElEta_123456;
   
   TH1D *recElIP_1, *recLeadElfBrem_1, *recSecElfBrem_1,
   *recElIP_12, *recLeadElfBrem_12, *recSecElfBrem_12,
   *recElIP_123, *recLeadElfBrem_123, *recSecElfBrem_123,
   *recElIP_1234, *recLeadElfBrem_1234, *recSecElfBrem_1234,
   *recElIP_12345, *recLeadElfBrem_12345, *recSecElfBrem_12345,
   *recElIP_123456, *recLeadElfBrem_123456, *recSecElfBrem_123456;
   
   TH1D *recLeadElConvMissHit_1, *recLeadElConvCotDist_1,
   *recLeadElConvMissHit_12, *recLeadElConvCotDist_12, 
   *recLeadElConvMissHit_123, *recLeadElConvCotDist_123, 
   *recLeadElConvMissHit_1234, *recLeadElConvCotDist_1234, 
   *recLeadElConvMissHit_12345, *recLeadElConvCotDist_12345, 
   *recLeadElConvMissHit_123456, *recLeadElConvCotDist_123456;
   
   TH1D *recSecElConvMissHit_1, *recSecElConvCotDist_1,
   *recSecElConvMissHit_12, *recSecElConvCotDist_12, 
   *recSecElConvMissHit_123, *recSecElConvCotDist_123, 
   *recSecElConvMissHit_1234, *recSecElConvCotDist_1234, 
   *recSecElConvMissHit_12345, *recSecElConvCotDist_12345,
   *recSecElConvMissHit_123456, *recSecElConvCotDist_123456;
   
   TH1D *recZNumSC, *recZNumIndexSC,
   *recPtZSC_1, *recEtaZSC_1, *recMassZSC_1,
   *recPtZSC_12, *recEtaZSC_12, *recMassZSC_12,
   *recPtZSC_123, *recEtaZSC_123, *recMassZSC_123, 
   *recPtZSC_1234, *recEtaZSC_1234, *recMassZSC_1234,
   *recPtZSC_12345, *recEtaZSC_12345, *recMassZSC_12345,
   *recPtZSC_123456, *recEtaZSC_123456, *recMassZSC_123456;
   
   TH1D *recLeadElPtSC_1, *recSecElPtSC_1, *recLeadElEtaSC_1, *recSecElEtaSC_1,
   *recLeadElPtSC_12, *recSecElPtSC_12, *recLeadElEtaSC_12, *recSecElEtaSC_12,
   *recLeadElPtSC_123, *recSecElPtSC_123, *recLeadElEtaSC_123, *recSecElEtaSC_123,
   *recLeadElPtSC_1234, *recSecElPtSC_1234, *recLeadElEtaSC_1234, *recSecElEtaSC_1234,
   *recLeadElPtSC_12345, *recSecElPtSC_12345, *recLeadElEtaSC_12345, *recSecElEtaSC_12345,
   *recLeadElPtSC_123456, *recSecElPtSC_123456, *recLeadElEtaSC_123456, *recSecElEtaSC_123456;
   
   TH1D *recLeadElIPSC_1, *recSecElIPSC_1, *recLeadElfBremSC_1, *recSecElfBremSC_1,
   *recLeadElIPSC_12, *recSecElIPSC_12, *recLeadElfBremSC_12, *recSecElfBremSC_12,
   *recLeadElIPSC_123, *recSecElIPSC_123, *recLeadElfBremSC_123, *recSecElfBremSC_123,
   *recLeadElIPSC_1234, *recSecElIPSC_1234, *recLeadElfBremSC_1234, *recSecElfBremSC_1234,
   *recLeadElIPSC_12345, *recSecElIPSC_12345, *recLeadElfBremSC_12345, *recSecElfBremSC_12345,
   *recLeadElIPSC_123456, *recSecElIPSC_123456, *recLeadElfBremSC_123456, *recSecElfBremSC_123456;
   
   TH1D *recTrackIsoLead_PreIso, *recEcalIsoLead_PreIso, *recHcalIsoLead_PreIso, *recTrackIsoSec_PreIso, *recEcalIsoSec_PreIso, *recHcalIsoSec_PreIso;
   
   TH1D *recRelIso_Barrel_PreIso, *recRelIso_Endcap_PreIso;
   
   TH1D *HoverE_Barrel_PreEiD, *DeltaEtaIn_Barrel_PreEiD, *DeltaPhiIn_Barrel_PreEiD, *SigmaIEtaIEta_Barrel_PreEiD;
   TH1D *HoverE_Endcap_PreEiD, *DeltaEtaIn_Endcap_PreEiD, *DeltaPhiIn_Endcap_PreEiD, *SigmaIEtaIEta_Endcap_PreEiD;
   
   TH1D *RecoIsoJetPt_123456, *RecoJetPt_123456, *recLeadIsoJetPt_123456, *recLeadIsoJetEta_123456;
   
   TH1D *JetCounter_123456, *IsoJetCounter_1, *IsoJetCounter_12, *IsoJetCounter_123, *IsoJetCounter_1234, *IsoJetCounter_12345, *IsoJetCounter_123456;
   
   TH1D *MinDeltaR_ZDau, *AllJetCharge, *IsoJetCharge, *NotIsoJetCharge, *DeltaR_IsoJet, *DeltaR_NotIsoJet, *DeltaR_IsoJet_ElType;
   
   TH1D *ChargeMisID_Pt_Acc, *ChargeMisID_Eta_Acc, *ChargeMisID_Hit_Acc, *ChargeMisID_fBrem_Acc, *ChargeMisID_IP_Acc, *ChargeMisID_RecoExclJet_Acc, *ChargeMisID_RecoInclJet_Acc;
   
   TH1D *CorrectCharge_Pt_Acc, *CorrectCharge_Eta_Acc, *CorrectCharge_Hit_Acc, *CorrectCharge_fBrem_Acc, *CorrectCharge_IP_Acc, *CorrectCharge_RecoExclJet_Acc, *CorrectCharge_RecoInclJet_Acc;
   
   TH1D *AllEl_Pt_Acc, *AllEl_Eta_Acc, *AllEl_Hit_Acc, *AllEl_fBrem_Acc, *AllEl_IP_Acc, *AllEl_RecoExclJet_Acc, *AllEl_RecoInclJet_Acc;
   
   TH1D *PixelHit_OC, *PixelHit_SC, *FirstPixelBarrelHit_OC, *FirstPixelBarrelHit_SC;
   
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

  int run;

  double _targetLumi;
  
  double _xsec;
  
  double _norm;
  
  TDirectory *_dir, *_charge_dir, *_Zdir, *_Eldir, *_Jetdir;
  
  bool _Norm;
  
  bool _GenParticleMatch;
  
  int _entries, _EventsPerFile, _EventNumber, _ProcEvents, _fileCounter, _Acc, _Trg, _Conv, _Imp, _Iso, _EiD, _JECUnc;
  
  std::string _selections, _JetType, _ReportName, _sample;

  TFile* _file;
  
  std::string _sourceFileList;

  std::string _RecoCutFlags[7];

  std::vector<TH1D*> _histoVector;
  std::vector<TH2D*> _histoVector2D;
  
  int _TrgList[8];

};
#endif

