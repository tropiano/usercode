#ifndef GenElectron_h
#define GenElectron_h

#include "TFile.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TH1D.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"

#include <string>

class GenElectron : public MyPatAnalysis {
public :
   GenElectron();

   virtual ~GenElectron();

   static MyPatAnalysis* create() {return new GenElectron(); }

   virtual void begin(TFile* out, const edm::ParameterSet&);

   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

   //Histograms
   TH1D *genPtZ, *genEtaZ, *genMassZ, *genPtZ_Acc, *genEtaZ_Acc, *genMassZ_Acc;
   
   TH1D *GenIsoJetPt_Acc, *GenJetPt_Acc, *genLeadIsoJetPt_Acc, *genLeadIsoJetEta_Acc;
   
   TH1D *GenJetCounter, *GenJetCounter_Acc, *GenIsoJetCounter,  *GenIsoJetCounter_Acc;
   
   TH1D *genLeadElPt, *genSecElPt, *genLeadElEta, *genSecElEta, *genLeadElPt_Acc, *genSecElPt_Acc, *genLeadElEta_Acc, *genSecElEta_Acc;
   
   TH1D *JetMinDeltaRZDau_GEN;
  

   std::vector<TH1D*> genJetPtVsInclMulti;
   std::vector<TH1D*> genJetEtaVsInclMulti;

   std::vector<TH1D*> genZPtVsInclMulti;
   std::vector<TH1D*> genZEtaVsInclMulti;

   std::vector<TH1D*> genEl1PtVsInclMulti;
   std::vector<TH1D*> genEl1EtaVsInclMulti;

   std::vector<TH1D*> genEl2PtVsInclMulti;
   std::vector<TH1D*> genEl2EtaVsInclMulti;

   std::vector<TH1D*> genZPtVsExclMulti;
   std::vector<TH1D*> genZEtaVsExclMulti;

   std::vector<TH1D*> genEl1PtVsExclMulti;
   std::vector<TH1D*> genEl1EtaVsExclMulti;

   std::vector<TH1D*> genEl2PtVsExclMulti;
   std::vector<TH1D*> genEl2EtaVsExclMulti;
   
private:
  double _ptjetmin;

  double _etajetmax;
  
  double _targetLumi;
  
  double _xsec;
  
  double _norm;
  
  bool _Norm;
  
  bool _Sumw2;

  int _entries, _EventsPerFile;

  TFile* _file;  

  TDirectory *_dir, *_Zdir, *_Eldir, *_Jetdir;
  
  std::string _selections;

  std::vector<TH1D*> _histovector;

};
#endif

