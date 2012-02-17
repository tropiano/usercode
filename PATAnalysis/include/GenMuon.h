#ifndef GenMuon_h
#define GenMuon_h

#include "TFile.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TH1D.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"

#include <string>

class GenMuon : public MyPatAnalysis {
public :
   GenMuon();

   virtual ~GenMuon();

   static MyPatAnalysis* create() {return new GenMuon(); }

   virtual void begin(TFile* out, const edm::ParameterSet&);

   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

   //Histograms
   TH1D *genPtZ, *genEtaZ, *genMassZ, *genPtZ_Acc, *genEtaZ_Acc, *genMassZ_Acc;
   
   TH1D *GenIsoJetPt_Acc, *GenJetPt_Acc, *genLeadIsoJetPt_Acc, *genLeadIsoJetEta_Acc;
   
   TH1D *GenJetCounter, *GenJetCounter_Acc, *GenIsoJetCounter,  *GenIsoJetCounter_Acc;
   
   TH1D *genLeadMuPt, *genSecMuPt, *genLeadMuEta, *genSecMuEta, *genLeadMuPt_Acc, *genSecMuPt_Acc, *genLeadMuEta_Acc, *genSecMuEta_Acc;
   
   TH1D *JetMinDeltaRZDau_GEN;
  

   std::vector<TH1D*> genJetPtVsInclMulti;
   std::vector<TH1D*> genJetEtaVsInclMulti;

   std::vector<TH1D*> genZPtVsInclMulti;
   std::vector<TH1D*> genZEtaVsInclMulti;

   std::vector<TH1D*> genMu1PtVsInclMulti;
   std::vector<TH1D*> genMu1EtaVsInclMulti;

   std::vector<TH1D*> genMu2PtVsInclMulti;
   std::vector<TH1D*> genMu2EtaVsInclMulti;

   std::vector<TH1D*> genZPtVsExclMulti;
   std::vector<TH1D*> genZEtaVsExclMulti;

   std::vector<TH1D*> genMu1PtVsExclMulti;
   std::vector<TH1D*> genMu1EtaVsExclMulti;

   std::vector<TH1D*> genMu2PtVsExclMulti;
   std::vector<TH1D*> genMu2EtaVsExclMulti;
   
private:
  double _ptjetmin;

  double _etajetmax;
  
  double _targetLumi;
  
  double _xsec;
  
  double _norm;
  
  bool _Norm;

  int _entries, _EventsPerFile, _EventNumber, _ProcEvents;

  TFile* _file;  

  TDirectory *_dir, *_Zdir, *_Mudir, *_Jetdir;
  
  std::string _selections;

  std::vector<TH1D*> _histovector;

};
#endif

