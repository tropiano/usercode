#ifndef RecoElectron_h
#define RecoElectron_h

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"

class RecoElectron : public MyPatAnalysis{
public :
   RecoElectron();
   
   virtual ~RecoElectron();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new RecoElectron(); } 

   //void  process(const fwlite::ChainEvent& iEvent);
   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

   //Histograms
   TH1D *recPtZ_Acc, *recEtaZ_Acc, *recMassZ_Acc,
   *recPtZ_Acc_Qual, *recEtaZ_Acc_Qual, *recMassZ_Acc_Qual,
   *recPtZ_Acc_Qual_Imp, *recEtaZ_Acc_Qual_Imp, *recMassZ_Acc_Qual_Imp, 
   *recPtZ_Acc_Qual_Imp_Iso, *recEtaZ_Acc_Qual_Imp_Iso, *recMassZ_Acc_Qual_Imp_Iso,
   *recPtZ_Acc_Qual_Imp_Iso_EiD, *recEtaZ_Acc_Qual_Imp_Iso_EiD, *recMassZ_Acc_Qual_Imp_Iso_EiD;
   
   TH1D *recTrackIsoLead_Acc_Qual_Imp, *recEcalIsoLead_Acc_Qual_Imp, *recHcalIsoLead_Acc_Qual_Imp, *recRelIsoLead_Acc_Qual_Imp, *recTrackIsoSec_Acc_Qual_Imp, *recEcalIsoSec_Acc_Qual_Imp, *recHcalIsoSec_Acc_Qual_Imp, *recRelIsoSec_Acc_Qual_Imp;
    
   TH1D *RecoJetPt_Acc_Qual_Imp_Iso_EiD, *RecoIsoJetPt_Acc_Qual_Imp_Iso_EiD, *recLeadIsoJetPt_Acc_Qual_Imp_Iso_EiD, *recLeadIsoJetEta_Acc_Qual_Imp_Iso_EiD, *JetCounter_Acc_Qual_Imp_Iso_EiD, *IsoJetCounter_Acc, *IsoJetCounter_Acc_Qual, *IsoJetCounter_Acc_Qual_Imp, *IsoJetCounter_Acc_Qual_Imp_Iso, *IsoJetCounter_Acc_Qual_Imp_Iso_EiD, 
*HoverE_Acc_Qual_Imp_Iso, *DeltaEtaIn_Acc_Qual_Imp_Iso, *DeltaPhiIn_Acc_Qual_Imp_Iso, *SigmaIEtaIEta_Acc_Qual_Imp_Iso, *HoverE_Acc_Qual_Imp_Iso_EiD, *DeltaEtaIn_Acc_Qual_Imp_Iso_EiD, *DeltaPhiIn_Acc_Qual_Imp_Iso_EiD, *SigmaIEtaIEta_Acc_Qual_Imp_Iso_EiD;
        
   TH1D *recLeadElPt_Acc, *recSecElPt_Acc, *recLeadElEta_Acc, *recSecElEta_Acc,
   *recLeadElPt_Acc_Qual, *recSecElPt_Acc_Qual, *recLeadElEta_Acc_Qual, *recSecElEta_Acc_Qual,
   *recLeadElPt_Acc_Qual_Imp, *recSecElPt_Acc_Qual_Imp, *recLeadElEta_Acc_Qual_Imp, *recSecElEta_Acc_Qual_Imp,
   *recLeadElPt_Acc_Qual_Imp_Iso, *recSecElPt_Acc_Qual_Imp_Iso, *recLeadElEta_Acc_Qual_Imp_Iso, *recSecElEta_Acc_Qual_Imp_Iso,
   *recLeadElPt_Acc_Qual_Imp_Iso_EiD, *recSecElPt_Acc_Qual_Imp_Iso_EiD, *recLeadElEta_Acc_Qual_Imp_Iso_EiD, *recSecElEta_Acc_Qual_Imp_Iso_EiD;
   
   TH2D *DeltaRvsCharge_JetRec, *DeltaRvsCharge_JetRec_Iso, *DeltaRvsCharge_JetRec_NotIso;  
   TH1D *HEnergy_IsoJet_ElType, *EMEnergy_IsoJet_ElType, *Jet_HEnergy, *Jet_EMEnergy, *MinDeltaR_ZDau, *AllJetCharge, *IsoJetCharge, *NotIsoJetCharge, *DeltaR_IsoJet, *DeltaR_NotIsoJet, *DeltaR_IsoJet_ElType;
        

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
 
private:

  double _targetLumi;
  
  double _xsec;
  
  double _norm;
  
  bool _Norm;
  
  bool _Sumw2;
  
  int _entries;
  
  std::string _electronID;

  TFile* _file;  

  TDirectory* _dir;

  std::vector<TH1D*> _histoVector;
  std::vector<TH2D*> _histoVector2D;

};
#endif

