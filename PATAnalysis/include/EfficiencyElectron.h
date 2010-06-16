#ifndef EfficiencyElectron_h
#define EfficiencyElectron_h

#include "TFile.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"
#include "TagAndProbeFiller.h"

class EfficiencyElectron : public MyPatAnalysis{
public :
   EfficiencyElectron();
   
   virtual ~EfficiencyElectron();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new EfficiencyElectron();} 

   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

   //Histograms
   
   TH1D *genMassZEff_1, *genPtZEff_1, *genEtaZEff_1,
   *genMassZEff_12, *genPtZEff_12, *genEtaZEff_12,
   *genMassZEff_123, *genPtZEff_123, *genEtaZEff_123,
   *genMassZEff_1234, *genPtZEff_1234, *genEtaZEff_1234,
   *genMassZEff_12345, *genPtZEff_12345, *genEtaZEff_12345,
   *genMassZEff_123456, *genPtZEff_123456, *genEtaZEff_123456;
   
   TH1D *GenIsoJetEff_1, *GenIsoJetEff_12, *GenIsoJetEff_123, *GenIsoJetEff_1234, *GenIsoJetEff_12345, *GenIsoJetEff_123456;
   
   TH1D *genLeadElEtaEff_1, *genLeadElEtaEff_12, *genLeadElEtaEff_123, *genLeadElEtaEff_1234, *genLeadElEtaEff_12345, *genLeadElEtaEff_123456, 
   *genLeadElPtEff_1, *genLeadElPtEff_12, *genLeadElPtEff_123, *genLeadElPtEff_1234, *genLeadElPtEff_12345, *genLeadElPtEff_123456;
   
   TH1D *AccDenom_genPtZ, *AccDenom_genMassZ, *AccDenom_genEtaZ, *AccDenom_genLeadElEta,     *AccDenom_genLeadElPt, *AccDenom_GenIsoJetNumber, *EffDenom_genPtZ, *EffDenom_genMassZ, *EffDenom_genEtaZ, *EffDenom_genLeadElEta, *EffDenom_genLeadElPt, *EffDenom_GenIsoJetNumber;
   
private:

  std::string _sample, _selections, _electronID, _JetType;

  TDirectory* _dir;
  
  TFile* _file;
  
  int _Acc, _Trg, _Qual, _Imp, _Iso, _EiD;
  
  std::vector<TH1D*> _histoVector;
  
  std::string _RecoCutFlags[7];
  
  std::vector<bool (*)(const reco::Candidate&)> tag_cuts;
  std::vector<bool (*)(const reco::Candidate&)> tag_cuts_0;
  std::vector<bool (*)(const reco::Candidate&)> tag_cuts_1;
  
  TagAndProbeFiller* _TagProbe_Electron_12;
  TagAndProbeFiller* _TagProbe_Electron_123;
  TagAndProbeFiller* _TagProbe_Electron_1234;
  TagAndProbeFiller* _TagProbe_Electron_12345;
  TagAndProbeFiller* _TagProbe_Electron_123456;
  
  TagAndProbeFiller* _TagProbe_Electron0_12;
  TagAndProbeFiller* _TagProbe_Electron0_123;
  TagAndProbeFiller* _TagProbe_Electron0_1234;
  TagAndProbeFiller* _TagProbe_Electron0_12345;
  TagAndProbeFiller* _TagProbe_Electron0_123456;
  
  TagAndProbeFiller* _TagProbe_Electron1_12;
  TagAndProbeFiller* _TagProbe_Electron1_123;
  TagAndProbeFiller* _TagProbe_Electron1_1234;
  TagAndProbeFiller* _TagProbe_Electron1_12345;
  TagAndProbeFiller* _TagProbe_Electron1_123456;
  
  int _nbin;
  
  double _xmin;

  double _xmax;
  
  bool _Norm;
  
  double _norm;
  
  int _entries, _EventsPerFile, _EventNumber, _ProcEvents;
  
  double _targetLumi;
  
  double _xsec;

};
#endif

