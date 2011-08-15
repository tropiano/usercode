#ifndef EfficiencyElectron_h
#define EfficiencyElectron_h

#include "TFile.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"
#include "TagAndProbeFillerElectron.h"

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
   
   TH1D *RecoInclJetEffWGA_1, *RecoInclJetEffWGA_12, *RecoInclJetEffWGA_123, *RecoInclJetEffWGA_1234, *RecoInclJetEffWGA_12345, *RecoInclJetEffWGA_123456;
   TH1D *RecoExclJetEffWGA_1, *RecoExclJetEffWGA_12, *RecoExclJetEffWGA_123, *RecoExclJetEffWGA_1234, *RecoExclJetEffWGA_12345, *RecoExclJetEffWGA_123456;
   
   TH1D *RecoInclJetEff_1, *RecoInclJetEff_12, *RecoInclJetEff_123, *RecoInclJetEff_1234, *RecoInclJetEff_12345, *RecoInclJetEff_123456;
   TH1D *RecoExclJetEff_1, *RecoExclJetEff_12, *RecoExclJetEff_123, *RecoExclJetEff_1234, *RecoExclJetEff_12345, *RecoExclJetEff_123456;
   
   TH1D *genLeadElEtaEff_1, *genLeadElEtaEff_12, *genLeadElEtaEff_123, *genLeadElEtaEff_1234, *genLeadElEtaEff_12345, *genLeadElEtaEff_123456, 
   *genLeadElPtEff_1, *genLeadElPtEff_12, *genLeadElPtEff_123, *genLeadElPtEff_1234, *genLeadElPtEff_12345, *genLeadElPtEff_123456;
   
   TH1D *AccDenom_genPtZ, *AccDenom_genMassZ, *AccDenom_genEtaZ, *AccDenom_genLeadElEta,     *AccDenom_genLeadElPt, *AccDenom_GenExclJetNumber, *AccDenom_RecoExclJetNumber, *AccDenom_RecoInclJetNumber, *AccDenom_GenInclJetNumber, *EffDenom_genPtZ, *EffDenom_genMassZ, *EffDenom_genEtaZ, *EffDenom_genLeadElEta, *EffDenom_genLeadElPt, *EffDenom_GenExclJetNumber, *EffDenom_RecoExclJetNumber, *EffDenom_RecoInclJetNumber, *EffDenom_GenInclJetNumber;
      
private:

  std::string _sample, _selections, _JetType;

  TDirectory* _dir;
  
  TFile* _file;
  
  int _Acc, _Trg, _Conv, _Imp, _Iso, _EiD;
  
  std::vector<TH1D*> _histoVector;
  
  std::string _RecoCutFlags[7];
  
  std::vector<bool (*)(const reco::Candidate&, int run)> tag_cuts;
  std::vector<bool (*)(const reco::Candidate&, int run)> tag_cuts_0;
  std::vector<bool (*)(const reco::Candidate&, int run)> tag_cuts_1;
  std::vector<bool (*)(const reco::Candidate&, int run)> tag_cuts_0_AllSel;
  std::vector<bool (*)(const reco::Candidate&, int run)> tag_cuts_1_AllSel;
  
  TagAndProbeFillerElectron* _TagProbe_Electron_12;
  TagAndProbeFillerElectron* _TagProbe_Electron_123;
  TagAndProbeFillerElectron* _TagProbe_Electron_1234;
  TagAndProbeFillerElectron* _TagProbe_Electron_12345;
  TagAndProbeFillerElectron* _TagProbe_Electron_123456;
  TagAndProbeFillerElectron* _TagProbe_Electron_Global;
  
  TagAndProbeFillerElectron* _TagProbe_Electron0_12;
  TagAndProbeFillerElectron* _TagProbe_Electron0_123;
  TagAndProbeFillerElectron* _TagProbe_Electron0_1234;
  TagAndProbeFillerElectron* _TagProbe_Electron0_12345;
  TagAndProbeFillerElectron* _TagProbe_Electron0_123456;
  TagAndProbeFillerElectron* _TagProbe_Electron0_Global;
  
  TagAndProbeFillerElectron* _TagProbe_Electron1_12;
  TagAndProbeFillerElectron* _TagProbe_Electron1_123;
  TagAndProbeFillerElectron* _TagProbe_Electron1_1234;
  TagAndProbeFillerElectron* _TagProbe_Electron1_12345;
  TagAndProbeFillerElectron* _TagProbe_Electron1_123456;
  TagAndProbeFillerElectron* _TagProbe_Electron1_Global;
  
  TagAndProbeFillerElectron* _TagProbe_Electron0_Incl_Double;
  TagAndProbeFillerElectron* _TagProbe_Electron1_Incl_Double;
  
  int _run, _nbin;
  
  double _xmin;

  double _xmax;
  
  bool _Norm;
  
  double _norm;
  
  int _entries, _EventsPerFile, _EventNumber, _ProcEvents;
  
  double _targetLumi;
  
  double _xsec;
  
  std::string _tp_mult;

};
#endif

