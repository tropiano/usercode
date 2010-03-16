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
   *genMassZEff_12345, *genPtZEff_12345, *genEtaZEff_12345;
   
   TH1D *GenIsoJetEff_1, *GenIsoJetEff_12, *GenIsoJetEff_123, *GenIsoJetEff_1234, *GenIsoJetEff_12345;
   
   TH1D *genLeadElEtaEff_1, *genLeadElEtaEff_12, *genLeadElEtaEff_123, *genLeadElEtaEff_1234, *genLeadElEtaEff_12345, *genLeadElPtEff_1, *genLeadElPtEff_12, *genLeadElPtEff_123, *genLeadElPtEff_1234, *genLeadElPtEff_12345;
   
   TH1D *AccDenom_genPtZ, *AccDenom_genMassZ, *AccDenom_genEtaZ, *AccDenom_genLeadElEta,     *AccDenom_genLeadElPt, *AccDenom_GenIsoJetNumber, *EffDenom_genPtZ, *EffDenom_genMassZ, *EffDenom_genEtaZ, *EffDenom_genLeadElEta, *EffDenom_genLeadElPt, *EffDenom_GenIsoJetNumber;
   
private:

  TDirectory* _dir;
  
  std::string _electronID;
  
  TFile* _file;
  
  int _Acc, _Qual, _Imp, _Iso, _EiD;
  
  std::vector<TH1D*> _histoVector;
  
  std::string _RecoCutFlags[6];
  
  std::vector<bool (*)(const reco::Candidate&)> tag_cuts;
  
  TagAndProbeFiller* _TagProbe_Electron_12;
  TagAndProbeFiller* _TagProbe_Electron_123;
  TagAndProbeFiller* _TagProbe_Electron_1234;
  TagAndProbeFiller* _TagProbe_Electron_12345;
  
  int _nbin;
  
  double _xmin;

  double _xmax;
  
  bool _Norm;
  
  double _norm;
  
  int _entries, _EventsPerFile;
  
  double _targetLumi;
  
  double _xsec;

};
#endif

