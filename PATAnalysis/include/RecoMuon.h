#ifndef RecoMuon_h
#define RecoMuon_h

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"


class RecoMuon : public MyPatAnalysis{
public :
   RecoMuon();
   
   virtual ~RecoMuon();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new RecoMuon(); } 

   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

   //Histograms
   
   TH1D *recZnum, *BestMassIndex;
   
   TH1D *recPtZ_1, *recEtaZ_1, *recMassZ_1,
   *recPtZ_12, *recEtaZ_12, *recMassZ_12,
   *recPtZ_123, *recEtaZ_123, *recMassZ_123, 
   *recPtZ_1234, *recEtaZ_1234, *recMassZ_1234,
   *recPtZ_12345, *recEtaZ_12345, *recMassZ_12345,
   *recPtZ_123456, *recEtaZ_123456, *recMassZ_123456;
 
   TH1D *recLeadMuPt_1, *recSecMuPt_1, *recLeadMuEta_1, *recSecMuEta_1,
   *recLeadMuPt_12, *recSecMuPt_12, *recLeadMuEta_12, *recSecMuEta_12,
   *recLeadMuPt_123, *recSecMuPt_123, *recLeadMuEta_123, *recSecMuEta_123,
   *recLeadMuPt_1234, *recSecMuPt_1234, *recLeadMuEta_1234, *recSecMuEta_1234,
   *recLeadMuPt_12345, *recSecMuPt_12345, *recLeadMuEta_12345, *recSecMuEta_12345,
   *recLeadMuPt_123456, *recSecMuPt_123456, *recLeadMuEta_123456, *recSecMuEta_123456;
   
   TH1D *recMuIP_1, 
   *recMuIP_12, 
   *recMuIP_123, 
   *recMuIP_1234, 
   *recMuIP_12345, 
   *recMuIP_123456; 
   
   TH1D *recTrackIsoLead_PreIso, *recEcalIsoLead_PreIso, *recHcalIsoLead_PreIso, *recTrackIsoSec_PreIso, *recEcalIsoSec_PreIso, *recHcalIsoSec_PreIso;
   
   TH1D *recRelIso_Barrel_1, *recRelIso_Endcap_1,
   *recRelIso_Barrel_12, *recRelIso_Endcap_12,
   *recRelIso_Barrel_123, *recRelIso_Endcap_123,
   *recRelIso_Barrel_1234, *recRelIso_Endcap_1234,
   *recRelIso_Barrel_12345, *recRelIso_Endcap_12345,
   *recRelIso_Barrel_123456, *recRelIso_Endcap_123456;
   
   TH1D *RecoIsoJetPt_123456, *RecoJetPt_123456, *recLeadIsoJetPt_123456, *recLeadIsoJetEta_123456;
   
   TH1D *JetCounter_123456, *IsoJetCounter_1, *IsoJetCounter_12, *IsoJetCounter_123, *IsoJetCounter_1234, *IsoJetCounter_12345, *IsoJetCounter_123456;
   
   TH1D *MinDeltaR_ZDau, *AllJetCharge, *IsoJetCharge, *NotIsoJetCharge, *DeltaR_IsoJet, *DeltaR_NotIsoJet, *DeltaR_IsoJet_MuType;
        
   std::vector<TH1D*> recJetPtVsInclMulti;
   std::vector<TH1D*> recJetEtaVsInclMulti;

   std::vector<TH1D*> recZPtVsInclMulti;
   std::vector<TH1D*> recZEtaVsInclMulti;

   std::vector<TH1D*> recMu1PtVsInclMulti;
   std::vector<TH1D*> recMu1EtaVsInclMulti;

   std::vector<TH1D*> recMu2PtVsInclMulti;
   std::vector<TH1D*> recMu2EtaVsInclMulti;

   std::vector<TH1D*> recZPtVsExclMulti;
   std::vector<TH1D*> recZEtaVsExclMulti;

   std::vector<TH1D*> recMu1PtVsExclMulti;
   std::vector<TH1D*> recMu1EtaVsExclMulti;

   std::vector<TH1D*> recMu2PtVsExclMulti;
   std::vector<TH1D*> recMu2EtaVsExclMulti;
 
private:

  int _run;
  
  double _rho;

  double _targetLumi;
  
  double _xsec;
  
  double _norm;
  
  TDirectory *_dir, *_Zdir, *_Mudir, *_Jetdir;
  
  bool _Norm;
  
  bool _GenParticleMatch;
  
  int _entries, _EventsPerFile, _EventNumber, _ProcEvents, _fileCounter, _Acc, _Trg, _Qual, _Imp, _Iso, _MuID, _JECUnc;
  
  std::string _selections, _JetType, _ReportName, _sample;

  TFile* _file;
  
  std::string _sourceFileList;

  std::string _RecoCutFlags[7];

  std::vector<TH1D*> _histoVector;
  std::vector<TH2D*> _histoVector2D;
  
  int _TrgList[8];

};
#endif

