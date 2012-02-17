#ifndef EfficiencyMuon_h
#define EfficiencyMuon_h

#include "TFile.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"
#include "TagAndProbeFillerMuon.h"

class EfficiencyMuon : public MyPatAnalysis{
public :
   EfficiencyMuon();
   
   virtual ~EfficiencyMuon();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new EfficiencyMuon();} 

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
   
   TH1D *genLeadMuEtaEff_1, *genLeadMuEtaEff_12, *genLeadMuEtaEff_123, *genLeadMuEtaEff_1234, *genLeadMuEtaEff_12345, *genLeadMuEtaEff_123456, 
   *genLeadMuPtEff_1, *genLeadMuPtEff_12, *genLeadMuPtEff_123, *genLeadMuPtEff_1234, *genLeadMuPtEff_12345, *genLeadMuPtEff_123456;
   
   TH1D *AccDenom_genPtZ, *AccDenom_genMassZ, *AccDenom_genEtaZ, *AccDenom_genLeadMuEta,     *AccDenom_genLeadMuPt, *AccDenom_GenExclJetNumber, *AccDenom_RecoExclJetNumber, *AccDenom_RecoInclJetNumber, *AccDenom_GenInclJetNumber, *EffDenom_genPtZ, *EffDenom_genMassZ, *EffDenom_genEtaZ, *EffDenom_genLeadMuEta, *EffDenom_genLeadMuPt, *EffDenom_GenExclJetNumber, *EffDenom_RecoExclJetNumber, *EffDenom_RecoInclJetNumber, *EffDenom_GenInclJetNumber;
      
private:

  std::string _sample, _selections, _JetType;

  TDirectory* _dir;
  
  TFile* _file;
  
  int _Acc, _Trg, _Qual, _Imp, _Iso, _MuID;
  
  std::vector<TH1D*> _histoVector;
  
  std::string _RecoCutFlags[7];
  
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts_0;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts_1;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts_AllSel;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts_0_AllSel;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts_1_AllSel;
  
  TagAndProbeFillerMuon* _TagProbe_Muon_Excl_12;
  TagAndProbeFillerMuon* _TagProbe_Muon_Excl_123;
  TagAndProbeFillerMuon* _TagProbe_Muon_Excl_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon_Excl_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon_Excl_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon_Excl_Global;
  TagAndProbeFillerMuon* _TagProbe_Muon_Excl_Double;
  
  TagAndProbeFillerMuon* _TagProbe_Muon0_Excl_12;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Excl_123;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Excl_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Excl_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Excl_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Excl_Global;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Excl_Double;
  
  TagAndProbeFillerMuon* _TagProbe_Muon1_Excl_12;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Excl_123;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Excl_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Excl_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Excl_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Excl_Global;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Excl_Double;
  
  TagAndProbeFillerMuon* _TagProbe_Muon_Incl_12;
  TagAndProbeFillerMuon* _TagProbe_Muon_Incl_123;
  TagAndProbeFillerMuon* _TagProbe_Muon_Incl_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon_Incl_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon_Incl_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon_Incl_Global;
  TagAndProbeFillerMuon* _TagProbe_Muon_Incl_Double;
  
  TagAndProbeFillerMuon* _TagProbe_Muon0_Incl_12;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Incl_123;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Incl_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Incl_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Incl_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Incl_Global;  
  TagAndProbeFillerMuon* _TagProbe_Muon0_Incl_Double;
  
  TagAndProbeFillerMuon* _TagProbe_Muon1_Incl_12;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Incl_123;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Incl_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Incl_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Incl_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Incl_Global;  
  TagAndProbeFillerMuon* _TagProbe_Muon1_Incl_Double;
  
  int _run, _nbin;
  
  double _rho;
  
  double _xmin;

  double _xmax;
  
  bool _Norm;
  
  double _norm;
  
  int _entries, _EventsPerFile, _EventNumber, _ProcEvents;
  
  double _targetLumi;
  
  double _xsec;

};
#endif

