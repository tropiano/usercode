#ifndef DeltaRAnalyzer_h
#define DeltaRAnalyzer_h

#include "TFile.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TList.h"
#include "TFile.h"
#include "TTree.h"
#include "MyPatAnalysis.h"


class DeltaRAnalyzer : public MyPatAnalysis{
public :
   DeltaRAnalyzer();
   
   virtual ~DeltaRAnalyzer();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new DeltaRAnalyzer(); } 

   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

   //Histograms
   TH1D //*recPtZ, *recEtaZ, *recMulti, *recMassZ, *recTrackIsoLead, *recEcalIsoLead, *recHcalIsoLead, *recRelIsoLead,
        //*recTrackIsoSec, *recEcalIsoSec, *recHcalIsoSec, *recRelIsoSec,
        //*recLeadMuPt, *recSecMuPt, *recLeadMuEta, *recSecMuEta,
        //*recLeadJetPt, *recLeadJetEta,
        *denCounterFalse, *numCounterFalse,
	*recoJetCounterFalse,
        *denCounterTrue, *numCounterTrue,
	*recoJetCounterTrue, *recoJetTotalCounter,
//	*genMuPt, *genMuPtinDeltaR, *genMuPtinDeltaRMCMatch,
//	*genMuEta, *genMuEtainDeltaR, *genMuEtainDeltaRMCMatch,
//	*genJetTotalCounter, *genJetTotalCounterinDeltaR, *genJetTotalCounterinDeltaRMCMatch,
	*recMuSiRefPt, *recMuSiRefEta,
	*recMuNoRefPt, *recMuNoRefEta;


   std::vector<TH1D*> genMuPt;
   std::vector<TH1D*> genMuPtinDeltaR;
   std::vector<TH1D*> genMuPtinDeltaRMCMatch;

   std::vector<TH1D*> genMuEta;
   std::vector<TH1D*> genMuEtainDeltaR;
   std::vector<TH1D*> genMuEtainDeltaRMCMatch;

/*
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

   TTree* _summaryPass;
*/
 
private:
  
  double _targetLumi;
  
  double _xsec;

  double _norm;
  
  bool _Norm;
  
  TDirectory* _dir;
  
  double _rho;
  
  int _run, _entries, _EventsPerFile, _EventNumber, _ProcEvents, _fileCounter, _Acc, _Trg, _Qual, _Imp, _Iso, _MuID, _JECUnc;
  
  std::string _selections, _JetType, _ReportName, _sample;
 
  TFile* _file; 

  std::string _sourceFileList;

  std::string _RecoCutFlags[7];

  std::vector<TH1D*> _histoVector;

/*
  //tree variables
  unsigned int _runnumber;
  unsigned int _lumisection;
  unsigned int _eventnumber;
  double _mass;
  double _pt;
  double _pt0;
  double _pt1;
  double _isorel0;
  double _isorel1;
  double _isotk0;
  double _isotk1;
  unsigned int _njets;
  double _ptjetsum;
  double _rho;
*/
};
#endif

