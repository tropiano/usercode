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
   
   TH1D *AccDenom_GenExclJetNumber, *AccDenom_GenInclJetNumber, *AccDenom_RecoExclJetNumber, *AccDenom_RecoInclJetNumber, *AccDenom_GenSingleMuPt_Barrel, *AccDenom_GenSingleMuPt_Endcap;
   
   TH1D *EffDenom_GenExclJetNumber, *EffDenom_RecoExclJetNumber, *EffDenom_RecoInclJetNumber, *EffDenom_GenInclJetNumber, *EffDenom_GenSingleMuPt_Barrel, *EffDenom_GenSingleMuPt_Endcap;
   
   TH1D *RecoInclJetEffWGA_1, *RecoInclJetEffWGA_12, *RecoInclJetEffWGA_123, *RecoInclJetEffWGA_1234, *RecoInclJetEffWGA_12345, *RecoInclJetEffWGA_123456;
   TH1D *RecoExclJetEffWGA_1, *RecoExclJetEffWGA_12, *RecoExclJetEffWGA_123, *RecoExclJetEffWGA_1234, *RecoExclJetEffWGA_12345, *RecoExclJetEffWGA_123456;
   
   TH1D *RecoInclJetEff_1, *RecoInclJetEff_12, *RecoInclJetEff_123, *RecoInclJetEff_1234, *RecoInclJetEff_12345, *RecoInclJetEff_123456;
   TH1D *RecoExclJetEff_1, *RecoExclJetEff_12, *RecoExclJetEff_123, *RecoExclJetEff_1234, *RecoExclJetEff_12345, *RecoExclJetEff_123456;
   
   TH1D *RecoSingleMuPtEff_Barrel_1, *RecoSingleMuPtEff_Barrel_12, *RecoSingleMuPtEff_Barrel_123, *RecoSingleMuPtEff_Barrel_1234, *RecoSingleMuPtEff_Barrel_12345, *RecoSingleMuPtEff_Barrel_123456,
*RecoSingleMuPtEff_Endcap_1, *RecoSingleMuPtEff_Endcap_12, *RecoSingleMuPtEff_Endcap_123, *RecoSingleMuPtEff_Endcap_1234, *RecoSingleMuPtEff_Endcap_12345, *RecoSingleMuPtEff_Endcap_123456;
   

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
  
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_1;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_12;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_123;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_1234;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_12345;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_123456;
  
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_1;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_12;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_123;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_1234;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_12345;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_123456;
  
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_1;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_12;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_123;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_1234;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_12345;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_123456;
  
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
  
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Barrel_12;
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Barrel_123;
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Barrel_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Barrel_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Barrel_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Barrel_Global;
  
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Endcap_12;
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Endcap_123;
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Endcap_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Endcap_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Endcap_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon_Pt_Endcap_Global;
  
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Barrel_12;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Barrel_123;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Barrel_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Barrel_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Barrel_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Barrel_Global;
  
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Endcap_12;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Endcap_123;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Endcap_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Endcap_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Endcap_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon0_Pt_Endcap_Global;
  
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Barrel_12;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Barrel_123;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Barrel_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Barrel_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Barrel_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Barrel_Global;
  
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Endcap_12;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Endcap_123;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Endcap_1234;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Endcap_12345;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Endcap_123456;
  TagAndProbeFillerMuon* _TagProbe_Muon1_Pt_Endcap_Global;
  
  
  int _run;
  
  double _rho;
  
  bool _Norm;
  
  double _norm;
  
  int _entries, _EventsPerFile, _EventNumber, _ProcEvents;
  
  double _targetLumi;
  
  double _xsec;
  
  double _ptBinTP[11];
  double _ptBinTP0[11];
  double _ptBinTP1[11];

};
#endif

