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

	*numCounterFalse_iso,
	*recoJetCounterFalse_iso,
        *numCounterTrue_iso,
	*recoJetCounterTrue_iso,
	*genJetTotalCounter_iso, *recoJetTotalCounter_iso,

	*recMuSiRefPt, *recMuSiRefEta,
	*recMuNoRefPt, *recMuNoRefEta,

	*denCounterFalse_VertWindA, *numCounterFalse_VertWindA,
	*recoJetCounterFalse_VertWindA,
        *denCounterTrue_VertWindA, *numCounterTrue_VertWindA,
	*recoJetCounterTrue_VertWindA,
	*genJetTotalCounter_VertWindA, *recoJetTotalCounter_VertWindA,

	*numCounterFalse_VertWindA_iso,
	*recoJetCounterFalse_VertWindA_iso,
        *numCounterTrue_VertWindA_iso,
	*recoJetCounterTrue_VertWindA_iso,
	*genJetTotalCounter_VertWindA_iso, *recoJetTotalCounter_VertWindA_iso,

	*recMuSiRefPt_VertWindA, *recMuSiRefEta_VertWindA,
	*recMuNoRefPt_VertWindA, *recMuNoRefEta_VertWindA,

	*denCounterFalse_VertWindB, *numCounterFalse_VertWindB,
	*recoJetCounterFalse_VertWindB,
        *denCounterTrue_VertWindB, *numCounterTrue_VertWindB,
	*recoJetCounterTrue_VertWindB,
	*genJetTotalCounter_VertWindB, *recoJetTotalCounter_VertWindB,

	*numCounterFalse_VertWindB_iso,
	*recoJetCounterFalse_VertWindB_iso,
        *numCounterTrue_VertWindB_iso,
	*recoJetCounterTrue_VertWindB_iso,
	*genJetTotalCounter_VertWindB_iso, *recoJetTotalCounter_VertWindB_iso,

	*recMuSiRefPt_VertWindB, *recMuSiRefEta_VertWindB,
	*recMuNoRefPt_VertWindB, *recMuNoRefEta_VertWindB;

   std::vector<TH1D*> genMuPt;
   std::vector<TH1D*> genMuPtinDeltaR;
   std::vector<TH1D*> genMuPtinDeltaRMCMatch;
   std::vector<TH1D*> genMuPtinDeltaRMCMatch_iso;

   std::vector<TH1D*> genMuEta;
   std::vector<TH1D*> genMuEtainDeltaR;
   std::vector<TH1D*> genMuEtainDeltaRMCMatch;
   std::vector<TH1D*> genMuEtainDeltaRMCMatch_iso;

   std::vector<TH1D*> genMuPt_VertWindA;
   std::vector<TH1D*> genMuPtinDeltaR_VertWindA;
   std::vector<TH1D*> genMuPtinDeltaRMCMatch_VertWindA;
   std::vector<TH1D*> genMuPtinDeltaRMCMatch_VertWindA_iso;

   std::vector<TH1D*> genMuEta_VertWindA;
   std::vector<TH1D*> genMuEtainDeltaR_VertWindA;
   std::vector<TH1D*> genMuEtainDeltaRMCMatch_VertWindA;
   std::vector<TH1D*> genMuEtainDeltaRMCMatch_VertWindA_iso;

   std::vector<TH1D*> genMuPt_VertWindB;
   std::vector<TH1D*> genMuPtinDeltaR_VertWindB;
   std::vector<TH1D*> genMuPtinDeltaRMCMatch_VertWindB;
   std::vector<TH1D*> genMuPtinDeltaRMCMatch_VertWindB_iso;

   std::vector<TH1D*> genMuEta_VertWindB;
   std::vector<TH1D*> genMuEtainDeltaR_VertWindB;
   std::vector<TH1D*> genMuEtainDeltaRMCMatch_VertWindB;
   std::vector<TH1D*> genMuEtainDeltaRMCMatch_VertWindB_iso;

//
   std::vector<TH1D*> genLeadMuPtFalse;
   std::vector<TH1D*> genSecondMuPtFalse;
   std::vector<TH1D*> genLeadMuEtaFalse;
   std::vector<TH1D*> genSecondMuEtaFalse;

   std::vector<TH1D*> genLeadMuPtFalse_iso;
   std::vector<TH1D*> genSecondMuPtFalse_iso;
   std::vector<TH1D*> genLeadMuEtaFalse_iso;
   std::vector<TH1D*> genSecondMuEtaFalse_iso;

   std::vector<TH1D*> genLeadMuPtTrue;
   std::vector<TH1D*> genSecondMuPtTrue;
   std::vector<TH1D*> genLeadMuEtaTrue;
   std::vector<TH1D*> genSecondMuEtaTrue;

   std::vector<TH1D*> genLeadMuPtTrue_iso;
   std::vector<TH1D*> genSecondMuPtTrue_iso;
   std::vector<TH1D*> genLeadMuEtaTrue_iso;
   std::vector<TH1D*> genSecondMuEtaTrue_iso;

   std::vector<TH1D*> genLeadMuPtFalse_VertWindA;
   std::vector<TH1D*> genSecondMuPtFalse_VertWindA;
   std::vector<TH1D*> genLeadMuEtaFalse_VertWindA;
   std::vector<TH1D*> genSecondMuEtaFalse_VertWindA;

   std::vector<TH1D*> genLeadMuPtFalse_VertWindA_iso;
   std::vector<TH1D*> genSecondMuPtFalse_VertWindA_iso;
   std::vector<TH1D*> genLeadMuEtaFalse_VertWindA_iso;
   std::vector<TH1D*> genSecondMuEtaFalse_VertWindA_iso;

   std::vector<TH1D*> genLeadMuPtTrue_VertWindA;
   std::vector<TH1D*> genSecondMuPtTrue_VertWindA;
   std::vector<TH1D*> genLeadMuEtaTrue_VertWindA;
   std::vector<TH1D*> genSecondMuEtaTrue_VertWindA;

   std::vector<TH1D*> genLeadMuPtTrue_VertWindA_iso;
   std::vector<TH1D*> genSecondMuPtTrue_VertWindA_iso;
   std::vector<TH1D*> genLeadMuEtaTrue_VertWindA_iso;
   std::vector<TH1D*> genSecondMuEtaTrue_VertWindA_iso;

   std::vector<TH1D*> genLeadMuPtFalse_VertWindB;
   std::vector<TH1D*> genSecondMuPtFalse_VertWindB;
   std::vector<TH1D*> genLeadMuEtaFalse_VertWindB;
   std::vector<TH1D*> genSecondMuEtaFalse_VertWindB;

   std::vector<TH1D*> genLeadMuPtFalse_VertWindB_iso;
   std::vector<TH1D*> genSecondMuPtFalse_VertWindB_iso;
   std::vector<TH1D*> genLeadMuEtaFalse_VertWindB_iso;
   std::vector<TH1D*> genSecondMuEtaFalse_VertWindB_iso;

   std::vector<TH1D*> genLeadMuPtTrue_VertWindB;
   std::vector<TH1D*> genSecondMuPtTrue_VertWindB;
   std::vector<TH1D*> genLeadMuEtaTrue_VertWindB;
   std::vector<TH1D*> genSecondMuEtaTrue_VertWindB;

   std::vector<TH1D*> genLeadMuPtTrue_VertWindB_iso;
   std::vector<TH1D*> genSecondMuPtTrue_VertWindB_iso;
   std::vector<TH1D*> genLeadMuEtaTrue_VertWindB_iso;
   std::vector<TH1D*> genSecondMuEtaTrue_VertWindB_iso;


private:
  
  double _targetLumi;
  
  double _xsec;

  double _norm;
  
  bool _Norm;
    
  double _rho;
  
  int _run, _entries, _EventsPerFile, _EventNumber, _ProcEvents, _fileCounter, _Acc, _Trg, _Qual, _Imp, _Iso, _MuID, _JECUnc;
  
  std::string _selections, _JetType, _ReportName, _sample;
 
  TFile* _file; 

  TDirectory *_dir, *_dir1, *_dir1a, *_dir1b, *_dir1c, *_dir2, *_dir2a, *_dir2b, *_dir2c;

  std::string _sourceFileList;

  std::string _RecoCutFlags[7];

  std::vector<TH1D*> _histoVector;

};
#endif

