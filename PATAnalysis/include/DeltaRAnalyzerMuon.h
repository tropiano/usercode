#ifndef DeltaRAnalyzerMuon_h
#define DeltaRAnalyzerMuon_h

#include "TFile.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TList.h"
#include "TFile.h"
#include "TTree.h"
#include "MyPatAnalysis.h"


class DeltaRAnalyzerMuon : public MyPatAnalysis{
public :
   DeltaRAnalyzerMuon();
   
   virtual ~DeltaRAnalyzerMuon();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new DeltaRAnalyzerMuon(); } 

   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

   //Histograms
   TH1D *denCounterFalse, *numCounterFalse,
	*recoJetCounterFalse,
        *denCounterTrue, *numCounterTrue,
	*recoJetCounterTrue,
	*genJetTotalCounter, *recoJetTotalCounter,
	*recMuSiRefPt, *recMuSiRefEta,
	*recMuNoRefPt, *recMuNoRefEta;

   std::vector<TH1D*> genMuPt;
   std::vector<TH1D*> genMuPtinDeltaR;
   std::vector<TH1D*> genMuPtinDeltaRMCMatch;

   std::vector<TH1D*> genMuEta;
   std::vector<TH1D*> genMuEtainDeltaR;
   std::vector<TH1D*> genMuEtainDeltaRMCMatch;

private:
  
  double _targetLumi;
  
  double _xsec;

  double _norm;
  
  bool _Norm;
    
  double _rho;
  
  int _run, _entries, _EventsPerFile, _EventNumber, _ProcEvents, _fileCounter, _Acc, _Trg, _Qual, _Imp, _Iso, _MuID, _JECUnc;
  
  std::string _selections, _JetType, _ReportName, _sample;
 
  TFile* _file; 

  TDirectory *_dir, *_dir1, *_dir2;

  std::string _sourceFileList;

  std::string _RecoCutFlags[7];

  std::vector<TH1D*> _histoVector;

};
#endif

