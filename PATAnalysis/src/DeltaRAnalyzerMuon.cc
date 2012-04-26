#include "Firenze/PATAnalysis/include/DeltaRAnalyzerMuon.h"
#include "Firenze/PATAnalysis/include/MuonUtilities.h"
#include <TH2.h>
#include <TStyle.h>
#include <math.h>

#include "TList.h"
#include "TParameter.h"
#include "TFile.h"
#include "TProofOutputFile.h"
#include "TChain.h"

#include <iostream>
#include <fstream>
#include <assert.h>

//CMSSW headers
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/PATObject.h"


using namespace std;
using namespace edm;

DeltaRAnalyzerMuon::DeltaRAnalyzerMuon():

denCounterFalse(0), numCounterFalse(0), 
recoJetCounterFalse(0), 
denCounterTrue(0), numCounterTrue(0), 
recoJetCounterTrue(0),
genJetTotalCounter(0), recoJetTotalCounter(0),

numCounterFalse_iso(0), 
recoJetCounterFalse_iso(0), 
numCounterTrue_iso(0), 
recoJetCounterTrue_iso(0),
genJetTotalCounter_iso(0), recoJetTotalCounter_iso(0),

recMuSiRefPt(0), recMuSiRefEta(0),
recMuNoRefPt(0), recMuNoRefEta(0),

denCounterFalse_VertWindA(0), numCounterFalse_VertWindA(0), 
recoJetCounterFalse_VertWindA(0), 
denCounterTrue_VertWindA(0), numCounterTrue_VertWindA(0), 
recoJetCounterTrue_VertWindA(0),
genJetTotalCounter_VertWindA(0), recoJetTotalCounter_VertWindA(0),

numCounterFalse_VertWindA_iso(0), 
recoJetCounterFalse_VertWindA_iso(0), 
numCounterTrue_VertWindA_iso(0), 
recoJetCounterTrue_VertWindA_iso(0),
genJetTotalCounter_VertWindA_iso(0), recoJetTotalCounter_VertWindA_iso(0),

recMuSiRefPt_VertWindA(0), recMuSiRefEta_VertWindA(0),
recMuNoRefPt_VertWindA(0), recMuNoRefEta_VertWindA(0),

denCounterFalse_VertWindB(0), numCounterFalse_VertWindB(0), 
recoJetCounterFalse_VertWindB(0), 
denCounterTrue_VertWindB(0), numCounterTrue_VertWindB(0), 
recoJetCounterTrue_VertWindB(0),
genJetTotalCounter_VertWindB(0), recoJetTotalCounter_VertWindB(0),

numCounterFalse_VertWindB_iso(0), 
recoJetCounterFalse_VertWindB_iso(0), 
numCounterTrue_VertWindB_iso(0), 
recoJetCounterTrue_VertWindB_iso(0),
genJetTotalCounter_VertWindB_iso(0), recoJetTotalCounter_VertWindB_iso(0),

recMuSiRefPt_VertWindB(0), recMuSiRefEta_VertWindB(0),
recMuNoRefPt_VertWindB(0), recMuNoRefEta_VertWindB(0),


_targetLumi(50.), _xsec(1.), _norm(1.), _Norm(false), _rho(0), _run(-1), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1), _fileCounter(0), _Acc(0), _Trg(0), _Qual(0), _Imp(0), _Iso(0), _MuID(0), _JECUnc(0), _selections("SYM"), _JetType("selectedJetsL1Corrected"), _sample("mc"),

_file(0), _dir(0), _dir1(0), _dir1a(0), _dir1b(0), _dir1c(0), _dir2(0), _dir2a(0), _dir2b(0), _dir2c(0), _histoVector()

{ }


void DeltaRAnalyzerMuon::begin(TFile* out, const edm::ParameterSet& iConfig){
   _file = out; 
   
   std::string dirname = iConfig.getParameter<std::string>("Name");
   _sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
   _selections = iConfig.getParameter<std::string>("Selections");
   _sample = iConfig.getParameter<std::string>("Sample");
   _targetLumi= iConfig.getParameter<double>("targetLumi");
   _xsec      = iConfig.getParameter<double>("CrossSection");
   _Norm      = iConfig.getParameter<bool>("Norm");
   _EventsPerFile    = iConfig.getParameter<int32_t>("EventsPerFile");
   _EventNumber    = iConfig.getParameter<int32_t>("EventNumber");
   _ProcEvents    = iConfig.getParameter<int32_t>("ProcEvents");
   _JetType = iConfig.getParameter<std::string>("JetType");
   _JECUnc    = iConfig.getParameter<int32_t>("JECUnc");
   _ReportName = iConfig.getParameter<std::string>("ReportName");
   
   //Selections
   _Acc = iConfig.getParameter<int32_t>("Acc");
   _Trg = iConfig.getParameter<int32_t>("Trg");
   _Qual = iConfig.getParameter<int32_t>("Qual");
   _Imp = iConfig.getParameter<int32_t>("Imp");
   _Iso = iConfig.getParameter<int32_t>("Iso");
   _MuID = iConfig.getParameter<int32_t>("MuID"); 
   
   for(int i=0; i<7; i++){
   	_RecoCutFlags[i] = "_1";
   }
   
   if(_selections=="SYM"){
   	_RecoCutFlags[_Acc] =  "_AccSYM";
   	_RecoCutFlags[_Qual] = "_QualSYM"; 
   	_RecoCutFlags[_Iso] =  "_IsoSYM";
   	_RecoCutFlags[_MuID] =  "_MuIDSYM";
   }
   if(_selections=="ASYM"){
   	_RecoCutFlags[_Acc] =  "_AccASYM";
   	_RecoCutFlags[_Qual] = "_QualASYM"; 
   	_RecoCutFlags[_Iso] =  "_IsoASYM";
   	_RecoCutFlags[_MuID] =  "_MuIDASYM";
   }
   _RecoCutFlags[_Trg] =  "_Trg";   
   _RecoCutFlags[_Imp] =  "_Imp";

   //_file = proofFile->OpenFile("RECREATE");
   cout << "DeltaRAnalyzerMuon file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());

   _dir1 = _dir->mkdir("SingleMu_Overlap"); 
   _dir1a = _dir1->mkdir("VerticesCut_No");
   _dir1b = _dir1->mkdir("VerticesCut_Less8");
   _dir1c = _dir1->mkdir("VerticesCut_Great8");

   _dir2 = _dir->mkdir("DoubleMu_Overlap");
   _dir2a = _dir2->mkdir("VerticesCut_No");
   _dir2b = _dir2->mkdir("VerticesCut_Less8");
   _dir2c = _dir2->mkdir("VerticesCut_Great8");

   _dir->cd();

   // Double Mu Overlap
   _dir2->cd();
   _dir2a->cd();

   denCounterFalse = new TH1D("denCounterFalse", "Generated jet multiplicity (2 #mu overlap = false)", 10, -0.5, 9.5); 
   _histoVector.push_back(denCounterFalse);
   numCounterFalse = new TH1D("numCounterFalse", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = false)", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterFalse);
   recoJetCounterFalse = new TH1D("recoJetCounterFalse", "Reconstructed jet multiplicity (with a rec Z) (2 #mu overlap = false)", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterFalse);

   denCounterTrue = new TH1D("denCounterTrue", "Generated jet multiplicity (2 #mu overlap = true)", 10, -0.5, 9.5); 
   _histoVector.push_back(denCounterTrue);
   numCounterTrue = new TH1D("numCounterTrue", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = true)", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterTrue);
   recoJetCounterTrue = new TH1D("recoJetCounterTrue", "Reconstructed jet multiplicity (#DeltaR test) (with a rec Z) (2 #mu overlap = true)", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterTrue);

   genJetTotalCounter = new TH1D("genJetTotalCounter", "Generated jet multiplicity (with a rec Z) (without 2 #mu overlap requirement)", 10, -0.5, 9.5); 
   _histoVector.push_back(genJetTotalCounter);
   recoJetTotalCounter = new TH1D("recoJetTotalCounter", "Reconstructed jet multiplicity (with a rec Z) (without 2 #mu overlap requirement)", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetTotalCounter);

//   denCounterFalse_iso = new TH1D("denCounterFalse_iso", "Generated jet multiplicity (2 #mu overlap = false)", 10, -0.5, 9.5); 
//   _histoVector.push_back(denCounterFalse_iso);
   numCounterFalse_iso = new TH1D("numCounterFalse_iso", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = false). Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterFalse_iso);
   recoJetCounterFalse_iso = new TH1D("recoJetCounterFalse_iso", "Reconstructed jet multiplicity (with a rec Z) (2 #mu overlap = false). Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterFalse_iso);

//   denCounterTrue_iso = new TH1D("denCounterTrue_iso", "Generated jet multiplicity (2 #mu overlap = true)", 10, -0.5, 9.5); 
//   _histoVector.push_back(denCounterTrue_iso);
   numCounterTrue_iso = new TH1D("numCounterTrue_iso", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = true). Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterTrue_iso);
   recoJetCounterTrue_iso = new TH1D("recoJetCounterTrue_iso", "Reconstructed jet multiplicity (#DeltaR test) (with a rec Z) (2 #mu overlap = true). Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterTrue_iso);

   genJetTotalCounter_iso = new TH1D("genJetTotalCounter_iso", "Generated jet multiplicity (with a rec Z) (without 2 #mu overlap requirement). Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(genJetTotalCounter_iso);
   recoJetTotalCounter_iso = new TH1D("recoJetTotalCounter_iso", "Reconstructed jet multiplicity (with a rec Z) (without 2 #mu overlap requirement). Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetTotalCounter_iso);


   //// Vertex window A: n Vertices <= 8
   _dir2a->cd("-");
   _dir2b->cd();

   denCounterFalse_VertWindA = new TH1D("denCounterFalse_VertWindA", "Generated jet multiplicity (2 #mu overlap = false): nVert <= 8", 10, -0.5, 9.5); 
   _histoVector.push_back(denCounterFalse_VertWindA);
   numCounterFalse_VertWindA = new TH1D("numCounterFalse_VertWindA", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = false): nVert <= 8", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterFalse_VertWindA);
   recoJetCounterFalse_VertWindA = new TH1D("recoJetCounterFalse_VertWindA", "Reconstructed jet multiplicity (with a rec Z) (2 #mu overlap = false): nVert <= 8", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterFalse_VertWindA);

   denCounterTrue_VertWindA = new TH1D("denCounterTrue_VertWindA", "Generated jet multiplicity (2 #mu overlap = true): nVert <= 8", 10, -0.5, 9.5); 
   _histoVector.push_back(denCounterTrue_VertWindA);
   numCounterTrue_VertWindA = new TH1D("numCounterTrue_VertWindA", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = true): nVert <= 8", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterTrue_VertWindA);
   recoJetCounterTrue_VertWindA = new TH1D("recoJetCounterTrue_VertWindA", "Reconstructed jet multiplicity (#DeltaR test) (with a rec Z) (2 #mu overlap = true): nVert <= 8", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterTrue_VertWindA);

   genJetTotalCounter_VertWindA = new TH1D("genJetTotalCounter_VertWindA", "Generated jet multiplicity (with a rec Z) (without 2 #mu overlap requirement): nVert <= 8", 10, -0.5, 9.5); 
   _histoVector.push_back(genJetTotalCounter_VertWindA);
   recoJetTotalCounter_VertWindA = new TH1D("recoJetTotalCounter_VertWindA", "Reconstructed jet multiplicity (with a rec Z) (without 2 #mu overlap requirement): nVert <= 8", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetTotalCounter_VertWindA);

   numCounterFalse_VertWindA_iso = new TH1D("numCounterFalse_VertWindA_iso", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = false): nVert <= 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterFalse_VertWindA_iso);
   recoJetCounterFalse_VertWindA_iso = new TH1D("recoJetCounterFalse_VertWindA_iso", "Reconstructed jet multiplicity (with a rec Z) (2 #mu overlap = false): nVert <= 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterFalse_VertWindA_iso);

   numCounterTrue_VertWindA_iso = new TH1D("numCounterTrue_VertWindA_iso", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = true): nVert <= 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterTrue_VertWindA_iso);
   recoJetCounterTrue_VertWindA_iso = new TH1D("recoJetCounterTrue_VertWindA_iso", "Reconstructed jet multiplicity (#DeltaR test) (with a rec Z) (2 #mu overlap = true): nVert <= 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterTrue_VertWindA_iso);

   genJetTotalCounter_VertWindA_iso = new TH1D("genJetTotalCounter_VertWindA_iso", "Generated jet multiplicity (with a rec Z) (without 2 #mu overlap requirement): nVert <= 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(genJetTotalCounter_VertWindA_iso);
   recoJetTotalCounter_VertWindA_iso = new TH1D("recoJetTotalCounter_VertWindA_iso", "Reconstructed jet multiplicity (with a rec Z) (without 2 #mu overlap requirement): nVert <= 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetTotalCounter_VertWindA_iso);


   ////// Vertex window B: n Vertices > 8
   _dir2b->cd("-");
   _dir2c->cd();

   denCounterFalse_VertWindB = new TH1D("denCounterFalse_VertWindB", "Generated jet multiplicity (2 #mu overlap = false): nVert > 8", 10, -0.5, 9.5); 
   _histoVector.push_back(denCounterFalse_VertWindB);
   numCounterFalse_VertWindB = new TH1D("numCounterFalse_VertWindB", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = false): nVert > 8", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterFalse_VertWindB);
   recoJetCounterFalse_VertWindB = new TH1D("recoJetCounterFalse_VertWindB", "Reconstructed jet multiplicity (with a rec Z) (2 #mu overlap = false): nVert > 8", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterFalse_VertWindB);

   denCounterTrue_VertWindB = new TH1D("denCounterTrue_VertWindB", "Generated jet multiplicity (2 #mu overlap = true): nVert > 8", 10, -0.5, 9.5); 
   _histoVector.push_back(denCounterTrue_VertWindB);
   numCounterTrue_VertWindB = new TH1D("numCounterTrue_VertWindB", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = true): nVert > 8", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterTrue_VertWindB);
   recoJetCounterTrue_VertWindB = new TH1D("recoJetCounterTrue_VertWindB", "Reconstructed jet multiplicity (#DeltaR test) (with a rec Z) (2 #mu overlap = true): nVert > 8", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterTrue_VertWindB);

   genJetTotalCounter_VertWindB = new TH1D("genJetTotalCounter_VertWindB", "Generated jet multiplicity (with a rec Z) (without 2 #mu overlap requirement): nVert > 8", 10, -0.5, 9.5); 
   _histoVector.push_back(genJetTotalCounter_VertWindB);
   recoJetTotalCounter_VertWindB = new TH1D("recoJetTotalCounter_VertWindB", "Reconstructed jet multiplicity (with a rec Z) (without 2 #mu overlap requirement): nVert > 8", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetTotalCounter_VertWindB);

   numCounterFalse_VertWindB_iso = new TH1D("numCounterFalse_VertWindB_iso", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = false): nVert > 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterFalse_VertWindB_iso);
   recoJetCounterFalse_VertWindB_iso = new TH1D("recoJetCounterFalse_VertWindB_iso", "Reconstructed jet multiplicity (with a rec Z) (2 #mu overlap = false): nVert > 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterFalse_VertWindB_iso);

   numCounterTrue_VertWindB_iso = new TH1D("numCounterTrue_VertWindB_iso", "Generated jet multiplicity (with a rec Z) (2 #mu overlap = true): nVert > 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterTrue_VertWindB_iso);
   recoJetCounterTrue_VertWindB_iso = new TH1D("recoJetCounterTrue_VertWindB_iso", "Reconstructed jet multiplicity (#DeltaR test) (with a rec Z) (2 #mu overlap = true): nVert > 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterTrue_VertWindB_iso);

   genJetTotalCounter_VertWindB_iso = new TH1D("genJetTotalCounter_VertWindB_iso", "Generated jet multiplicity (with a rec Z) (without 2 #mu overlap requirement): nVert > 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(genJetTotalCounter_VertWindB_iso);
   recoJetTotalCounter_VertWindB_iso = new TH1D("recoJetTotalCounter_VertWindB_iso", "Reconstructed jet multiplicity (with a rec Z) (without 2 #mu overlap requirement): nVert > 8. Iso no rho", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetTotalCounter_VertWindB_iso);


   _dir2c->cd("-");
   _dir2->cd("-");

   // Single Mu Ovelap
   _dir1->cd();
   _dir1a->cd();

   recMuSiRefPt = new TH1D("recMuSiRefPt", "reconstructed #mu p_{T} with gen particle reference", 200, 0, 200); 
   _histoVector.push_back(recMuSiRefPt);
   recMuSiRefEta = new TH1D("recMuSiRefEta", "reconstructed #mu #eta with gen particle reference", 100, -5., 5.); 
   _histoVector.push_back(recMuSiRefEta);
   recMuNoRefPt = new TH1D("recMuNoRefPt", "reconstructed #mu p_{T} without gen particle reference", 200, 0, 200); 
   _histoVector.push_back(recMuNoRefPt);
   recMuNoRefEta = new TH1D("recMuNoRefEta", "reconstructed #mu #eta without gen particle reference", 100, -5., 5.); 
   _histoVector.push_back(recMuNoRefEta);

   //// Vertex window A: n Vertices <= 8
   _dir1a->cd("-");
   _dir1b->cd();

   recMuSiRefPt_VertWindA = new TH1D("recMuSiRefPt_VertWindA", "reconstructed #mu p_{T} with gen particle reference: nVert <= 8", 200, 0, 200); 
   _histoVector.push_back(recMuSiRefPt_VertWindA);
   recMuSiRefEta_VertWindA = new TH1D("recMuSiRefEta_VertWindA", "reconstructed #mu #eta with gen particle reference: nVert <= 8", 100, -5., 5.); 
   _histoVector.push_back(recMuSiRefEta_VertWindA);
   recMuNoRefPt_VertWindA = new TH1D("recMuNoRefPt_VertWindA", "reconstructed #mu p_{T} without gen particle reference: nVert <= 8", 200, 0, 200); 
   _histoVector.push_back(recMuNoRefPt_VertWindA);
   recMuNoRefEta_VertWindA = new TH1D("recMuNoRefEta_VertWindA", "reconstructed #mu #eta without gen particle reference: nVert <= 8", 100, -5., 5.); 
   _histoVector.push_back(recMuNoRefEta_VertWindA);

   ////// Vertex window B: n Vertices > 8
   _dir1b->cd("-");
   _dir1c->cd();

   recMuSiRefPt_VertWindB = new TH1D("recMuSiRefPt_VertWindB", "reconstructed #mu p_{T} with gen particle reference: nVert > 8", 200, 0, 200); 
   _histoVector.push_back(recMuSiRefPt_VertWindB);
   recMuSiRefEta_VertWindB = new TH1D("recMuSiRefEta_VertWindB", "reconstructed #mu #eta with gen particle reference: nVert > 8", 100, -5., 5.); 
   _histoVector.push_back(recMuSiRefEta_VertWindB);
   recMuNoRefPt_VertWindB = new TH1D("recMuNoRefPt_VertWindB", "reconstructed #mu p_{T} without gen particle reference: nVert > 8", 200, 0, 200); 
   _histoVector.push_back(recMuNoRefPt_VertWindB);
   recMuNoRefEta_VertWindB = new TH1D("recMuNoRefEta_VertWindB", "reconstructed #mu #eta without gen particle reference: nVert > 8", 100, -5., 5.); 
   _histoVector.push_back(recMuNoRefEta_VertWindB);

   _dir1c->cd("-");
   _dir1->cd("-");

 ///////////////////// Normalizzazione
 
  _fileCounter = 0;
  
  TChain *ch = new TChain("Events");
  ifstream infile;
  infile.open(_sourceFileList.c_str());
  string datafile;
  while(getline (infile, datafile)){
    ch->Add(datafile.c_str());
    _fileCounter++;
  }
  
  if(_EventNumber==0 && _EventsPerFile==0)_entries = ch->GetEntries();
  
  if(_EventNumber!=0 && _EventsPerFile==0)_entries = _EventNumber;
 
  if(_EventNumber==0 && _EventsPerFile!=0)_entries = _fileCounter*_EventsPerFile;
  
  if(_ProcEvents!=-1)_entries = _ProcEvents;
  
  cout << "DeltaRAnalyzerMuon analyzing nr. file = "<<_fileCounter<<endl;
  cout << "DeltaRAnalyzerMuon analyzing nr. event = "<<_entries<<endl;
  
  delete ch;
  
   double lumi = _entries/_xsec;

   if(_Norm && lumi!=0){
   _norm = _targetLumi/lumi;
   }
   
   //////////////////////////////////////////

   std::vector<TH1D*>::const_iterator ibeg = _histoVector.begin();
   std::vector<TH1D*>::const_iterator iend = _histoVector.end();
   //use the errors as they were from real data 
   for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
     if (*i) //(*i)->Scale(_norm);
      (*i)->Sumw2();
   } 
  _dir->cd("-");
   
  cout << "DeltaRAnalyzerMuon Worker built." << endl;   
}

DeltaRAnalyzerMuon::~DeltaRAnalyzerMuon(){
  _file->ls();
  //_file->Write();
  //_file->Close();
}


//void  DeltaRAnalyzerMuon::process(const fwlite::ChainEvent& iEvent)
void  DeltaRAnalyzerMuon::process(const fwlite::Event& iEvent)
{
   
   _run = iEvent.id().run();
   if(_sample=="mc")_run=-1;

   //double weight = 1.;
   _file->cd();


   //----richiamo i REC Muons----
   fwlite::Handle<std::vector<pat::Muon> > muonHandle;
   muonHandle.getByLabel(iEvent, "selectedMuons");

   //----richiamo la REC Z----
   fwlite::Handle<std::vector<reco::CompositeCandidate> > zHandle;
   zHandle.getByLabel(iEvent, "zmumurec");
   //seleziono un vettore di 2 muoni figli della REC Z
   std::vector<const pat::Muon*> muonsfromZ;  
   if(zHandle->size()) muonsfromZ = ZRECDaughters(*zHandle);

   //----richiamo i REC Jets----
   fwlite::Handle<std::vector<pat::Jet> > jetHandle;
   jetHandle.getByLabel(iEvent, _JetType.c_str());
   //seleziono un vettore di jets REC per Accettanza e ID
   std::vector<const pat::Jet*> jetcollID = GetJets<pat::Jet>(*jetHandle);     
   //seleziono un vettore di jets REC con Cleaning dai 2 Muoni dalla Z REC
   std::vector<const pat::Jet*> recjets = CleanJets<pat::Jet>(jetcollID, muonsfromZ);   

   //----richiamo la rho----
   fwlite::Handle<double> Rho;
   Rho.getByLabel(iEvent, "kt6PFJets", "rho");
   _rho = *Rho;

   //----richiamo i Trigger paths----
   fwlite::Handle<pat::TriggerEvent> triggerHandle; 
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");
 
   //----richiamo la GEN Z----
   fwlite::Handle<std::vector<reco::CompositeCandidate> > zHandlegen;
   zHandlegen.getByLabel(iEvent, "zmumugen");

   //----richiamo la GenParticles per il MC Matching----
   fwlite::Handle<reco::GenParticleCollection> genParticles;
   genParticles.getByLabel(iEvent, "genParticles");
   
   //----richiamo i GEN Jets----
   fwlite::Handle<std::vector<reco::GenJet> > jetHandler;
   //jetHandler.getByLabel(iEvent, "selectedGenJetsOld"); //jets senza Mu PG
   jetHandler.getByLabel(iEvent, "selectedGenJetsNoMuMufromZ");  //jets senza Mu a mano
   //seleziono un vettore di GEN Jets per Accettanza
   std::vector<const reco::GenJet*> genjetsr = GetJets_noJetID<reco::GenJet>(*jetHandler); 

   //----richiamo i Vertici----
   fwlite::Handle<reco::VertexCollection> priVertHandle;
   priVertHandle.getByLabel(iEvent, "offlinePrimaryVertices");
   //creo un contatore di numero di vertici a seconda di alcuni tagli
   double nVerticesAll = priVertHandle->size();
   reco::VertexCollection vtxs = *priVertHandle;
   //   std::vector<float> vtxZ;
   //   std::vector<float> vtxNdof;
   double nVertices = 0;
   for(int iv = 0; iv < nVerticesAll; iv++){
   	if(fabs(vtxs[iv].z()) < 24 &&
   	   !vtxs[iv].isFake() &&
   	   //vtxs[iv].position().rho() <= 2 &&
   	   vtxs[iv].ndof() >= 4){
    		//vtxZ[nVertices] = vtxs[iv].z();
   		//vtxNdof[nVertices] = vtxs[iv].ndof();
    		nVertices++;
   	}
   }
//cout << "nVertices finale = " << nVertices << endl;

   _dir->cd();
   _dir1->cd();
   _dir1a->cd();

   addHistosVsMulti(genjetsr.size(), "genMuPt_", " - Gen: Generated #mu p_{T}", 200, 0, 200, genMuPt);
   addHistosVsMulti(genjetsr.size(), "genMuEta_", " - Gen: Generated #mu #eta", 100, -5., 5., genMuEta);
   addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaR_", " - Gen: Generated #mu p_{T} (1 #mu overlap = false)", 200, 0, 200, genMuPtinDeltaR);
   addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaR_", " - Gen: Generated #mu #eta (1 #mu overlap = false)", 100, -5., 5., genMuEtainDeltaR);
   addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaRMCMatch_", " - Gen: Generated #mu p_{T} (1 #mu overlap = false) MC Match", 200, 0, 200, genMuPtinDeltaRMCMatch);
   addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaRMCMatch_", " - Gen: Generated #mu #eta (1 #mu overlap = false) MC Match", 100, -5., 5., genMuEtainDeltaRMCMatch);
   addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaRMCMatch_iso_", " - Gen: Generated #mu p_{T} (1 #mu overlap = false) MC Match. Iso no rho", 200, 0, 200, genMuPtinDeltaRMCMatch_iso);
   addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaRMCMatch_iso_", " - Gen: Generated #mu #eta (1 #mu overlap = false) MC Match. Iso no rho", 100, -5., 5., genMuEtainDeltaRMCMatch_iso);

   //// Vertex window A: n Vertices <= 8
   _dir1a->cd("-");
   _dir1b->cd();

   addHistosVsMulti(genjetsr.size(), "genMuPt_VertWindA_", " - Gen: Generated #mu p_{T}. nVert <= 8", 200, 0, 200, genMuPt_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genMuEta_VertWindA_", " - Gen: Generated #mu #eta. nVert <= 8", 100, -5., 5., genMuEta_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaR_VertWindA_", " - Gen: Generated #mu p_{T} (1 #mu overlap = false). nVert <= 8", 200, 0, 200, genMuPtinDeltaR_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaR_VertWindA_", " - Gen: Generated #mu #eta (1 #mu overlap = false). nVert <= 8", 100, -5., 5., genMuEtainDeltaR_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaRMCMatch_VertWindA_", " - Gen: Generated #mu p_{T} (1 #mu overlap = false) MC Match. nVert <= 8", 200, 0, 200, genMuPtinDeltaRMCMatch_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaRMCMatch_VertWindA_", " - Gen: Generated #mu #eta (1 #mu overlap = false) MC Match. nVert <= 8", 100, -5., 5., genMuEtainDeltaRMCMatch_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaRMCMatch_VertWindA_iso_", " - Gen: Generated #mu p_{T} (1 #mu overlap = false) MC Match. nVert <= 8. Iso no rho", 200, 0, 200, genMuPtinDeltaRMCMatch_VertWindA_iso);
   addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaRMCMatch_VertWindA_iso_", " - Gen: Generated #mu #eta (1 #mu overlap = false) MC Match. nVert <= 8. Iso no rho", 100, -5., 5., genMuEtainDeltaRMCMatch_VertWindA_iso);

   ////// Vertex window B: n Vertices > 8
   _dir1b->cd("-");
   _dir1c->cd();

   addHistosVsMulti(genjetsr.size(), "genMuPt_VertWindB_", " - Gen: Generated #mu p_{T}. nVert > 8", 200, 0, 200, genMuPt_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genMuEta_VertWindB_", " - Gen: Generated #mu #eta. nVert > 8", 100, -5., 5., genMuEta_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaR_VertWindB_", " - Gen: Generated #mu p_{T} (1 #mu overlap = false). nVert > 8", 200, 0, 200, genMuPtinDeltaR_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaR_VertWindB_", " - Gen: Generated #mu #eta (1 #mu overlap = false). nVert > 8", 100, -5., 5., genMuEtainDeltaR_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaRMCMatch_VertWindB_", " - Gen: Generated #mu p_{T} (1 #mu overlap = false) MC Match. nVert > 8", 200, 0, 200, genMuPtinDeltaRMCMatch_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaRMCMatch_VertWindB_", " - Gen: Generated #mu #eta (1 #mu overlap = false) MC Match. nVert > 8", 100, -5., 5., genMuEtainDeltaRMCMatch_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaRMCMatch_VertWindB_iso_", " - Gen: Generated #mu p_{T} (1 #mu overlap = false) MC Match. nVert > 8. Iso no rho", 200, 0, 200, genMuPtinDeltaRMCMatch_VertWindB_iso);
   addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaRMCMatch_VertWindB_iso_", " - Gen: Generated #mu #eta (1 #mu overlap = false) MC Match. nVert > 8. Iso no rho", 100, -5., 5., genMuEtainDeltaRMCMatch_VertWindB_iso);

   _dir1c->cd("-");
   _dir1->cd("-");
   _dir->cd("-");

   _dir->cd();
   _dir2->cd();
   _dir2a->cd();

   addHistosVsMulti(genjetsr.size(), "genLeadMuPtFalse_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = false)", 200, 0, 200, genLeadMuPtFalse);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtFalse_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = false)", 200, 0, 200, genSecondMuPtFalse);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaFalse_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = false)", 100, -5., 5., genLeadMuEtaFalse);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaFalse_", " - Gen: Generated Second #mu #eta (2 #mu overlap = false)", 100, -5., 5., genSecondMuEtaFalse);

   addHistosVsMulti(genjetsr.size(), "genLeadMuPtFalse_iso_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = false). Iso no rho", 200, 0, 200, genLeadMuPtFalse_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtFalse_iso_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = false). Iso no rho", 200, 0, 200, genSecondMuPtFalse_iso);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaFalse_iso_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = false). Iso no rho", 100, -5., 5., genLeadMuEtaFalse_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaFalse_iso_", " - Gen: Generated Second #mu #eta (2 #mu overlap = false). Iso no rho", 100, -5., 5., genSecondMuEtaFalse_iso);

   addHistosVsMulti(genjetsr.size(), "genLeadMuPtTrue_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = true)", 200, 0, 200, genLeadMuPtTrue);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtTrue_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = true)", 200, 0, 200, genSecondMuPtTrue);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaTrue_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = true)", 100, -5., 5., genLeadMuEtaTrue);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaTrue_", " - Gen: Generated Second #mu #eta (2 #mu overlap = true)", 100, -5., 5., genSecondMuEtaTrue);

   addHistosVsMulti(genjetsr.size(), "genLeadMuPtTrue_iso_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = true). Iso no rho", 200, 0, 200, genLeadMuPtTrue_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtTrue_iso_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = true). Iso no rho", 200, 0, 200, genSecondMuPtTrue_iso);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaTrue_iso_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = true). Iso no rho", 100, -5., 5., genLeadMuEtaTrue_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaTrue_iso_", " - Gen: Generated Second #mu #eta (2 #mu overlap = true). Iso no rho", 100, -5., 5., genSecondMuEtaTrue_iso);

   _dir2a->cd("-");
   _dir2b->cd();

   ////// Vertex window A: n Vertices <= 8
   addHistosVsMulti(genjetsr.size(), "genLeadMuPtFalse_VertWindA_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = false). nVert <= 8", 200, 0, 200, genLeadMuPtFalse_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtFalse_VertWindA_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = false). nVert <= 8", 200, 0, 200, genSecondMuPtFalse_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaFalse_VertWindA_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = false). nVert <= 8", 100, -5., 5., genLeadMuEtaFalse_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaFalse_VertWindA_", " - Gen: Generated Second #mu #eta (2 #mu overlap = false). nVert <= 8", 100, -5., 5., genSecondMuEtaFalse_VertWindA);

   addHistosVsMulti(genjetsr.size(), "genLeadMuPtFalse_VertWindA_iso_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = false). nVert <= 8. Iso no rho", 200, 0, 200, genLeadMuPtFalse_VertWindA_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtFalse_VertWindA_iso_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = false). nVert <= 8. Iso no rho", 200, 0, 200, genSecondMuPtFalse_VertWindA_iso);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaFalse_VertWindA_iso_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = false). nVert <= 8. Iso no rho", 100, -5., 5., genLeadMuEtaFalse_VertWindA_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaFalse_VertWindA_iso_", " - Gen: Generated Second #mu #eta (2 #mu overlap = false). nVert <= 8. Iso no rho", 100, -5., 5., genSecondMuEtaFalse_VertWindA_iso);

   addHistosVsMulti(genjetsr.size(), "genLeadMuPtTrue_VertWindA_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = true). nVert <= 8", 200, 0, 200, genLeadMuPtTrue_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtTrue_VertWindA_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = true). nVert <= 8", 200, 0, 200, genSecondMuPtTrue_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaTrue_VertWindA_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = true). nVert <= 8", 100, -5., 5., genLeadMuEtaTrue_VertWindA);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaTrue_VertWindA_", " - Gen: Generated Second #mu #eta (2 #mu overlap = true). nVert <= 8", 100, -5., 5., genSecondMuEtaTrue_VertWindA);

   addHistosVsMulti(genjetsr.size(), "genLeadMuPtTrue_VertWindA_iso_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = true). nVert <= 8. Iso no rho", 200, 0, 200, genLeadMuPtTrue_VertWindA_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtTrue_VertWindA_iso_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = true). nVert <= 8. Iso no rho", 200, 0, 200, genSecondMuPtTrue_VertWindA_iso);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaTrue_VertWindA_iso_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = true). nVert <= 8. Iso no rho", 100, -5., 5., genLeadMuEtaTrue_VertWindA_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaTrue_VertWindA_iso_", " - Gen: Generated Second #mu #eta (2 #mu overlap = true). nVert <= 8. Iso no rho", 100, -5., 5., genSecondMuEtaTrue_VertWindA_iso);

   _dir2b->cd("-");
   _dir2c->cd();

   ////// Vertex window B: n Vertices > 8
   addHistosVsMulti(genjetsr.size(), "genLeadMuPtFalse_VertWindB_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = false). nVert > 8", 200, 0, 200, genLeadMuPtFalse_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtFalse_VertWindB_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = false). nVert > 8", 200, 0, 200, genSecondMuPtFalse_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaFalse_VertWindB_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = false). nVert > 8", 100, -5., 5., genLeadMuEtaFalse_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaFalse_VertWindB_", " - Gen: Generated Second #mu #eta (2 #mu overlap = false). nVert > 8", 100, -5., 5., genSecondMuEtaFalse_VertWindB);

   addHistosVsMulti(genjetsr.size(), "genLeadMuPtFalse_VertWindB_iso_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = false). nVert > 8. Iso no rho", 200, 0, 200, genLeadMuPtFalse_VertWindB_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtFalse_VertWindB_iso_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = false). nVert > 8. Iso no rho", 200, 0, 200, genSecondMuPtFalse_VertWindB_iso);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaFalse_VertWindB_iso_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = false). nVert > 8. Iso no rho", 100, -5., 5., genLeadMuEtaFalse_VertWindB_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaFalse_VertWindB_iso_", " - Gen: Generated Second #mu #eta (2 #mu overlap = false). nVert > 8. Iso no rho", 100, -5., 5., genSecondMuEtaFalse_VertWindB_iso);

   addHistosVsMulti(genjetsr.size(), "genLeadMuPtTrue_VertWindB_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = true). nVert > 8", 200, 0, 200, genLeadMuPtTrue_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtTrue_VertWindB_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = true). nVert > 8", 200, 0, 200, genSecondMuPtTrue_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaTrue_VertWindB_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = true). nVert > 8", 100, -5., 5., genLeadMuEtaTrue_VertWindB);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaTrue_VertWindB_", " - Gen: Generated Second #mu #eta (2 #mu overlap = true). nVert > 8", 100, -5., 5., genSecondMuEtaTrue_VertWindB);

   addHistosVsMulti(genjetsr.size(), "genLeadMuPtTrue_VertWindB_iso_", " - Gen: Generated Leading #mu p_{T} (2 #mu overlap = true). nVert > 8. Iso no rho", 200, 0, 200, genLeadMuPtTrue_VertWindB_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuPtTrue_VertWindB_iso_", " - Gen: Generated Second #mu p_{T} (2 #mu overlap = true). nVert > 8. Iso no rho", 200, 0, 200, genSecondMuPtTrue_VertWindB_iso);
   addHistosVsMulti(genjetsr.size(), "genLeadMuEtaTrue_VertWindB_iso_", " - Gen: Generated Leading #mu #eta (2 #mu overlap = true). nVert > 8. Iso no rho", 100, -5., 5., genLeadMuEtaTrue_VertWindB_iso);
   addHistosVsMulti(genjetsr.size(), "genSecondMuEtaTrue_VertWindB_iso_", " - Gen: Generated Second #mu #eta (2 #mu overlap = true). nVert > 8. Iso no rho", 100, -5., 5., genSecondMuEtaTrue_VertWindB_iso);

   _dir2c->cd("-");
   _dir2->cd("-");
   _dir->cd("-");

   double DeltaR = 0.0;

	//qui apro l'if con le condizioni su Z e muoni
	if (GenSelectedInAcceptance(*zHandlegen, _selections) &&
	    ((*zHandlegen)[0].daughter(0))->charge() * ((*zHandlegen)[0].daughter(1))->charge() == -1){ 
/*
		//Questi fanno comodo
		cout << endl << "EventNumber = " <<  iEvent.id().event() << endl;
		//cout << "(*zHandlegen)[0].daughter(0) = " << (*zHandlegen)[0].daughter(0) << endl;
		cout << "((*zHandlegen)[0].daughter(0))->pt() = " << ((*zHandlegen)[0].daughter(0))->pt() << endl;
		cout << "((*zHandlegen)[0].daughter(0))->eta() = " << ((*zHandlegen)[0].daughter(0))->eta() << endl;
		cout << "((*zHandlegen)[0].daughter(0))->status() = " << ((*zHandlegen)[0].daughter(0))->status() << endl;
		//cout << "(*zHandlegen)[0].daughter(1) = " << (*zHandlegen)[0].daughter(1) << endl;
		cout << "((*zHandlegen)[0].daughter(1))->pt() = " << ((*zHandlegen)[0].daughter(1))->pt() << endl;
		cout << "((*zHandlegen)[0].daughter(1))->eta() = " << ((*zHandlegen)[0].daughter(1))->eta() << endl;
		cout << "((*zHandlegen)[0].daughter(1))->status() = " << ((*zHandlegen)[0].daughter(1))->status() << endl;
		cout << "genjetsr.size() = " << genjetsr.size() <<endl;
*/

		//histo pre-overlap
		for (unsigned int a = 0; a < 2 ; ++a) {
			genMuPt[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(a))->pt());
			genMuEta[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(a))->eta());
			if(nVertices <=8){
				genMuPt_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(a))->pt());
				genMuEta_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(a))->eta());
			}
  			else{ 
				genMuPt_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(a))->pt());
				genMuEta_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(a))->eta());
			}

		}


		//######### Inizio Analisi di efficienza di singolo Mu #########
		//qui apro il loop sui 2 Mu

		for (unsigned int c = 0; c < 2; ++c){

			cout << "Studi su ((*zHandlegen)[0].daughter(" << c << ")):" << endl;

			//definizione di overlap di singolo Mu
			bool overlapSingMu = false;

			if (genjetsr.size() != 0) {
				int counterSingMu = 0;
				for (unsigned int k = 0; k < genjetsr.size(); ++k){
/*
					// Info su jets e DeltaR
					cout << "genjetsr[" << k <<"]->pt() = " << genjetsr[k]->pt() << endl;  
					cout << "genjetsr[" << k <<"]->eta() = " << genjetsr[k]->eta() << endl;
					cout << "deltaR(genjetsr[" << k <<"]->eta(), genjetsr[" << k << "]->phi(), ((*zHandlegen)[0].daughter(0))->eta(), ((*zHandlegen)[0].daughter(0))->phi()) =" << deltaR(genjetsr[k]->eta(), genjetsr[k]->phi(), ((*zHandlegen)[0].daughter(0))->eta(), ((*zHandlegen)[0].daughter(0))->phi()) << endl;
					cout << "deltaR(genjetsr[" << k <<"]->eta(), genjetsr[" << k << "]->phi(), ((*zHandlegen)[0].daughter(1))->eta(), ((*zHandlegen)[0].daughter(1))->phi()) =" << deltaR(genjetsr[k]->eta(), genjetsr[k]->phi(), ((*zHandlegen)[0].daughter(1))->eta(), ((*zHandlegen)[0].daughter(1))->phi()) << endl;
*/
					if(!(deltaR(genjetsr[k]->eta(), genjetsr[k]->phi(), ((*zHandlegen)[0].daughter(c))->eta(), ((*zHandlegen)[0].daughter(c))->phi()) > DeltaR)){ 
  
						counterSingMu = counterSingMu +1;
					}
		
					if (counterSingMu != 0) overlapSingMu = true;
				}
			}
		
			// qui apro il controllo di overlapSingMu
			if (!overlapSingMu){
				genMuPtinDeltaR[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
				genMuEtainDeltaR[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());

				if(nVertices <=8){
					genMuPtinDeltaR_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
					genMuEtainDeltaR_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());
				}
				else{
					genMuPtinDeltaR_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
					genMuEtainDeltaR_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());
				}					

				if ((*muonHandle).size() !=0){
        				for (unsigned int y = 0; y < (*muonHandle).size(); ++y){
	        				if((*muonHandle)[y].genParticle()!= 0){
/*
							//Info su pt, eta delle GenParticles 
	        					//cout << "(*muonHandle)[" << y << "].genParticle() = "<< (*muonHandle)[y].genParticle() << endl;
	        					cout << "(*muonHandle)[" << y << "]->pt() = "<< (*muonHandle)[y].pt() << endl;
	        					cout << "(*muonHandle)[" << y << "]->eta() = "<< (*muonHandle)[y].eta() << endl;
	        					cout << "GenParticle (*muonHandle)[" << y << "] pt() = "<< (*muonHandle)[y].genParticle()->pt() << endl;
	        					cout << "GenParticle (*muonHandle)[" << y << "] eta() = "<< (*muonHandle)[y].genParticle()->eta() << endl;
	        					cout << "GenParticle (*muonHandle)[" << y << "] status() = "<< (*muonHandle)[y].genParticle()->status() << endl;
*/
							recMuSiRefPt->Fill((*muonHandle)[y].pt());
							recMuSiRefEta->Fill((*muonHandle)[y].eta());
							if(nVertices <=8){
	 							recMuSiRefPt_VertWindA->Fill((*muonHandle)[y].pt());
								recMuSiRefEta_VertWindA->Fill((*muonHandle)[y].eta());
							}
							else{
	 							recMuSiRefPt_VertWindB->Fill((*muonHandle)[y].pt());
								recMuSiRefEta_VertWindB->Fill((*muonHandle)[y].eta());
							}
							// controllo del MC match di singolo Mu (Iso con rho)
							if (singleMu_Probe_Acc_SYM((*muonHandle)[y], _run, _rho) &&
							    singleMu_Probe_Qual_SYM((*muonHandle)[y], _run, _rho) &&
							    singleMu_Probe_Imp((*muonHandle)[y], _run, _rho) &&
							    singleMu_Probe_Iso_SYM((*muonHandle)[y], _run, _rho)){

								if ((*muonHandle)[y].genParticle()->pt() == ((*zHandlegen)[0].daughter(c))->pt() && 
			     				    	    (*muonHandle)[y].genParticle()->eta() == ((*zHandlegen)[0].daughter(c))->eta()){
									cout << "MC Matching trovato: (*muonHandle)[" << y << "] matcha con ((*zHandlegen)[0].daughter(" << c << ")(Iso con rho))" << endl;
						
									genMuPtinDeltaRMCMatch[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
									genMuEtainDeltaRMCMatch[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());
									if(nVertices <=8){
										genMuPtinDeltaRMCMatch_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
										genMuEtainDeltaRMCMatch_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());
									}
									else{
										genMuPtinDeltaRMCMatch_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
										genMuEtainDeltaRMCMatch_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());
									}
								}
								else {
									cout << "Vedo che (*muonHandle)[" << y << "] non matcha con ((*zHandlegen)[0].daughter(" << c << ")(Iso con rho))" << endl;
								}
							}// chiudo il controllo del MC match di singolo Mu (Iso con rho)
							// controllo del MC match di singolo Mu (Iso senza rho)
							if (singleMu_Probe_Acc_SYM((*muonHandle)[y], _run, _rho) &&
							    singleMu_Probe_Qual_SYM((*muonHandle)[y], _run, _rho) &&
							    singleMu_Probe_Imp((*muonHandle)[y], _run, _rho) &&
							    singleMu_Probe_Iso_SYM_mod((*muonHandle)[y], _run, _rho)){

								if ((*muonHandle)[y].genParticle()->pt() == ((*zHandlegen)[0].daughter(c))->pt() && 
			     				    	    (*muonHandle)[y].genParticle()->eta() == ((*zHandlegen)[0].daughter(c))->eta()){
									cout << "MC Matching trovato: (*muonHandle)[" << y << "] matcha con ((*zHandlegen)[0].daughter(" << c << ") (Iso senza rho))" << endl;
						
									genMuPtinDeltaRMCMatch_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
									genMuEtainDeltaRMCMatch_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());
									if(nVertices <=8){
										genMuPtinDeltaRMCMatch_VertWindA_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
										genMuEtainDeltaRMCMatch_VertWindA_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());
									}
									else{
										genMuPtinDeltaRMCMatch_VertWindB_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
										genMuEtainDeltaRMCMatch_VertWindB_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());
									}
								}
								else {
									cout << "Vedo che (*muonHandle)[" << y << "] non matcha con ((*zHandlegen)[0].daughter(" << c << ") (Iso senza rho))" << endl;
								}
							}// chiudo il controllo del MC match di singolo Mu (Iso senza rho)
						}
						else {
							recMuNoRefPt->Fill((*muonHandle)[y].pt());
							recMuNoRefEta->Fill((*muonHandle)[y].eta());
							if(nVertices <=8){
								recMuNoRefPt_VertWindA->Fill((*muonHandle)[y].pt());
								recMuNoRefEta_VertWindA->Fill((*muonHandle)[y].eta());
							}
							else{
								recMuNoRefPt_VertWindB->Fill((*muonHandle)[y].pt());
								recMuNoRefEta_VertWindB->Fill((*muonHandle)[y].eta());
							}
						}
					}
				}
			}// qui chiudo il controllo di overlapSingMu
		}// qui chiudo il loop sui 2 Mu
		//######### Fine Analisi di efficienza di singolo Mu #########


		//######### Inizio Analisi di efficienza di doppio Mu #########
		//definizione di overlap di doppio Mu
		bool overlap = false;

		if (genjetsr.size() != 0) {

			int counter = 0;
			for (unsigned int k = 0; k < genjetsr.size(); ++k){

/*
				// Info su jets e DeltaR
				cout << "genjetsr[" << k <<"]->pt() = " << genjetsr[k]->pt() << endl;  
				cout << "genjetsr[" << k <<"]->eta() = " << genjetsr[k]->eta() << endl;
				cout << "deltaR(genjetsr[" << k <<"]->eta(), genjetsr[" << k << "]->phi(), ((*zHandlegen)[0].daughter(0))->eta(), ((*zHandlegen)[0].daughter(0))->phi()) =" << deltaR(genjetsr[k]->eta(), genjetsr[k]->phi(), ((*zHandlegen)[0].daughter(0))->eta(), ((*zHandlegen)[0].daughter(0))->phi()) << endl;
				cout << "deltaR(genjetsr[" << k <<"]->eta(), genjetsr[" << k << "]->phi(), ((*zHandlegen)[0].daughter(1))->eta(), ((*zHandlegen)[0].daughter(1))->phi()) =" << deltaR(genjetsr[k]->eta(), genjetsr[k]->phi(), ((*zHandlegen)[0].daughter(1))->eta(), ((*zHandlegen)[0].daughter(1))->phi()) << endl;
*/
				if(!(deltaR(genjetsr[k]->eta(), genjetsr[k]->phi(), ((*zHandlegen)[0].daughter(0))->eta(), ((*zHandlegen)[0].daughter(0))->phi()) > DeltaR && 
		     		     deltaR(genjetsr[k]->eta(), genjetsr[k]->phi(), ((*zHandlegen)[0].daughter(1))->eta(), ((*zHandlegen)[0].daughter(1))->phi()) > DeltaR)) 
		  
					counter = counter +1;
				}

				if (counter != 0) overlap = true;
		}

		//qui apro il controllo di overlap false di 2 Mu
		if(!overlap){

			denCounterFalse->Fill(genjetsr.size());
			if(nVertices <=8){
				denCounterFalse_VertWindA->Fill(genjetsr.size());
			}
			else{
				denCounterFalse_VertWindB->Fill(genjetsr.size());
			}

			//qui apro la sezione della Z ricostruita di overlap false (Iso con rho)
			if (RecSelected(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){

				numCounterFalse->Fill(genjetsr.size());
				recoJetCounterFalse-> Fill(recjets.size());
				genLeadMuPtFalse[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
				genSecondMuPtFalse[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
				genLeadMuEtaFalse[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
				genSecondMuEtaFalse[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				if(nVertices <=8){
					numCounterFalse_VertWindA->Fill(genjetsr.size());
					recoJetCounterFalse_VertWindA-> Fill(recjets.size());
					genLeadMuPtFalse_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
					genSecondMuPtFalse_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
					genLeadMuEtaFalse_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
					genSecondMuEtaFalse_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				}
				else{
					numCounterFalse_VertWindB->Fill(genjetsr.size());
					recoJetCounterFalse_VertWindB-> Fill(recjets.size());
					genLeadMuPtFalse_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
					genSecondMuPtFalse_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
					genLeadMuEtaFalse_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
					genSecondMuEtaFalse_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				}

			} // qui chiudo la sezione della Z ricostruita di overlap false (Iso con rho)
			//qui apro la sezione della Z ricostruita di overlap false (Iso senza rho)
			if (RecSelected_mod(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_mod(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_mod(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_mod(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_mod(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_mod(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){

				numCounterFalse_iso->Fill(genjetsr.size());
				recoJetCounterFalse_iso-> Fill(recjets.size());
				genLeadMuPtFalse_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
				genSecondMuPtFalse_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
				genLeadMuEtaFalse_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
				genSecondMuEtaFalse_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				if(nVertices <=8){
					numCounterFalse_VertWindA_iso->Fill(genjetsr.size());
					recoJetCounterFalse_VertWindA_iso-> Fill(recjets.size());
					genLeadMuPtFalse_VertWindA_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
					genSecondMuPtFalse_VertWindA_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
					genLeadMuEtaFalse_VertWindA_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
					genSecondMuEtaFalse_VertWindA_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				}
				else{
					numCounterFalse_VertWindB_iso->Fill(genjetsr.size());
					recoJetCounterFalse_VertWindB_iso-> Fill(recjets.size());
					genLeadMuPtFalse_VertWindB_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
					genSecondMuPtFalse_VertWindB_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
					genLeadMuEtaFalse_VertWindB_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
					genSecondMuEtaFalse_VertWindB_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				}

			} // qui chiudo la sezione della Z ricostruita di overlap false (Iso senza rho)

		}// qui chiudo il controllo di overlap false di 2 Mu

		//qui apro il controllo di overlap true di 2 Mu
		else {
			denCounterTrue->Fill(genjetsr.size());
			if(nVertices <=8){
				denCounterTrue_VertWindA->Fill(genjetsr.size());
			}
			else{
				denCounterTrue_VertWindB->Fill(genjetsr.size());
			}

			//qui apro la sezione della Z ricostruita di overlap true (Iso con rho)
			if (RecSelected(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){

				numCounterTrue->Fill(genjetsr.size());
				recoJetCounterTrue-> Fill(recjets.size());
				genLeadMuPtTrue[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
				genSecondMuPtTrue[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
				genLeadMuEtaTrue[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
				genSecondMuEtaTrue[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				if(nVertices <=8){
					numCounterTrue_VertWindA->Fill(genjetsr.size());
					recoJetCounterTrue_VertWindA-> Fill(recjets.size());
					genLeadMuPtTrue_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
					genSecondMuPtTrue_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
					genLeadMuEtaTrue_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
					genSecondMuEtaTrue_VertWindA[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				}
				else{
					numCounterTrue_VertWindB->Fill(genjetsr.size());
					recoJetCounterTrue_VertWindB-> Fill(recjets.size());
					genLeadMuPtTrue_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
					genSecondMuPtTrue_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
					genLeadMuEtaTrue_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
					genSecondMuEtaTrue_VertWindB[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				}

			} // qui chiudo la sezione della Z ricostruita di overlap true (Iso con rho)
			//qui apro la sezione della Z ricostruita di overlap true (Iso senza rho)
			if (RecSelected_mod(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_mod(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_mod(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_mod(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_mod(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected_mod(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){

				numCounterTrue_iso->Fill(genjetsr.size());
				recoJetCounterTrue_iso-> Fill(recjets.size());
				genLeadMuPtTrue_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
				genSecondMuPtTrue_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
				genLeadMuEtaTrue_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
				genSecondMuEtaTrue_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				if(nVertices <=8){
					numCounterTrue_VertWindA_iso->Fill(genjetsr.size());
					recoJetCounterTrue_VertWindA_iso-> Fill(recjets.size());
					genLeadMuPtTrue_VertWindA_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
					genSecondMuPtTrue_VertWindA_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
					genLeadMuEtaTrue_VertWindA_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
					genSecondMuEtaTrue_VertWindA_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				}
				else{
					numCounterTrue_VertWindB_iso->Fill(genjetsr.size());
					recoJetCounterTrue_VertWindB_iso-> Fill(recjets.size());
					genLeadMuPtTrue_VertWindB_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->pt());
					genSecondMuPtTrue_VertWindB_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->pt());
					genLeadMuEtaTrue_VertWindB_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(0))->eta());
					genSecondMuEtaTrue_VertWindB_iso[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(1))->eta());
				}

			} // qui chiudo la sezione della Z ricostruita di overlap true (Iso senza rho)

		}// qui chiudo il controllo di overlap true di 2 Mu

		//qui apro la sezione della Z ricostruita senza richiesta di overlap (Iso con rho)
		if (RecSelected(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){

			genJetTotalCounter-> Fill(genjetsr.size());
			recoJetTotalCounter-> Fill(recjets.size());
			if(nVertices <=8){
				genJetTotalCounter_VertWindA-> Fill(genjetsr.size());
				recoJetTotalCounter_VertWindA-> Fill(recjets.size());
			}
			else{
				genJetTotalCounter_VertWindB-> Fill(genjetsr.size());
				recoJetTotalCounter_VertWindB-> Fill(recjets.size());
			}
		} // qui chiudo la sezione della Z ricostruita senza richiesta di overlap (Iso con rho)
		//qui apro la sezione della Z ricostruita senza richiesta di overlap (Iso senza rho)
		if (RecSelected_mod(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected_mod(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected_mod(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected_mod(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected_mod(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected_mod(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){

			genJetTotalCounter_iso-> Fill(genjetsr.size());
			recoJetTotalCounter_iso-> Fill(recjets.size());
			if(nVertices <=8){
				genJetTotalCounter_VertWindA_iso-> Fill(genjetsr.size());
				recoJetTotalCounter_VertWindA_iso-> Fill(recjets.size());
			}
			else{
				genJetTotalCounter_VertWindB_iso-> Fill(genjetsr.size());
				recoJetTotalCounter_VertWindB_iso-> Fill(recjets.size());
			}
		} // qui chiudo la sezione della Z ricostruita senza richiesta di overlap (Iso senza rho)

		//######### Fine Analisi di efficienza di doppio Mu #########

	} //qui chiudo l'if con le condizioni su Z e muoni

} // chiudo tutto

void DeltaRAnalyzerMuon::finalize(){
  
  _file->Write();
}


