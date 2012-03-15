
#include "Firenze/PATAnalysis/include/DeltaRAnalyzer.h"
#include "Firenze/PATAnalysis/include/MuonUtilities.h"
#include <TH2.h>
#include <TStyle.h>
#include <math.h>
//#include "Utilities.h"

#include "TList.h"
#include "TParameter.h"
#include "TFile.h"
#include "TProofOutputFile.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include "TChain.h"

#include <iostream>

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
//RooUnfold


using namespace std;
using namespace edm;


DeltaRAnalyzer::DeltaRAnalyzer(): 
/*
recPtZ(0), recEtaZ(0), recMulti(0), recMassZ(0), recTrackIsoLead(0), recEcalIsoLead(0), recHcalIsoLead(0), recRelIsoLead(0),
recTrackIsoSec(0), recEcalIsoSec(0), recHcalIsoSec(0), recRelIsoSec(0),
recLeadMuPt(0), recSecMuPt(0), recLeadMuEta(0), recSecMuEta(0),
recLeadJetPt(0), recLeadJetEta(0), 
*/
denCounterFalse(0), numCounterFalse(0), 
recoJetCounterFalse(0), 
denCounterTrue(0), numCounterTrue(0), 
recoJetCounterTrue(0),
recoJetTotalCounter(0),
//genMuPt(0), genMuPtinDeltaR(0), genMuPtinDeltaRMCMatch(0),
//genMuEta(0), genMuEtainDeltaR(0), genMuEtainDeltaRMCMatch(0),
//genJetTotalCounter(0),genJetTotalCounterinDeltaR(0), genJetTotalCounterinDeltaRMCMatch(0),
recMuSiRefPt(0), recMuSiRefEta(0),
recMuNoRefPt(0), recMuNoRefEta(0),
// _summaryPass(0),
_targetLumi(50.), _xsec(1.), _norm(1.), _Norm(false), _dir(0), _rho(0), _run(-1), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1), _fileCounter(0), _Acc(0), _Trg(0), _Qual(0), _Imp(0), _Iso(0), _MuID(0), _JECUnc(0), _selections("SYM"), _JetType("selectedJetsL1Corrected"), _sample("mc"), _file(0), _histoVector()

//_bayesUnfold("bayesUnfoldedMulti", "Bayes unfolded Jet Multiplicity")
//_efficiency(out, "EfficienciesTotalVsRecMulti", 10, -0.5, 9.5, 0.3, false),
{ }

void DeltaRAnalyzer::begin(TFile* out, const edm::ParameterSet& iConfig){
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
   _RecoCutFlags[i] = "_1";}
   
   if(_selections=="SYM"){
   _RecoCutFlags[_Acc] =  "_AccSYM";
   _RecoCutFlags[_Qual] = "_QualSYM"; 
   _RecoCutFlags[_Iso] =  "_IsoSYM";
   _RecoCutFlags[_MuID] =  "_MuIDSYM";}
   if(_selections=="ASYM"){
   _RecoCutFlags[_Acc] =  "_AccASYM";
   _RecoCutFlags[_Qual] = "_QualASYM"; 
   _RecoCutFlags[_Iso] =  "_IsoASYM";
   _RecoCutFlags[_MuID] =  "_MuIDASYM";}
     
   _RecoCutFlags[_Trg] =  "_Trg";   
   _RecoCutFlags[_Imp] =  "_Imp";

   //_file = proofFile->OpenFile("RECREATE");
   cout << "DeltaRAnalyzer file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();
/*
   recPtZ   = new TH1D("recPtZ", "Reconstructed Z p_{T}", 200, 0, 200);
   _histoVector.push_back(recPtZ);
   recEtaZ  = new TH1D("recEtaZ", "Reconstructed Z #eta", 100, -10, 10);
   _histoVector.push_back(recEtaZ);
   recMulti = new TH1D("recMulti", "Reconstructed jet multiplicity", 10, -0.5, 9.5);
   _histoVector.push_back(recMulti);
   recMassZ = new TH1D("recMassZ", "Reconstructed Z mass", 200, 50, 150);
   _histoVector.push_back(recMassZ);
   recTrackIsoLead = new TH1D("recTrackIsoLead", "Lead Track Isolation SumPt", 30, 0, 30);
   _histoVector.push_back(recTrackIsoLead);
   recEcalIsoLead  = new TH1D("recEcalIsoLead", "Lead ECAL Isolation SumPt", 30, 0, 30);
   _histoVector.push_back(recEcalIsoLead);
   recHcalIsoLead  = new TH1D("recHcalIsoLead", "Lead HCAL Isolation SumPt", 30, 0, 30);
   _histoVector.push_back(recHcalIsoLead);
   recRelIsoLead   = new TH1D("recRelIsoLead", "Lead Comb Relative Isolation", 30, 0, 3);
   _histoVector.push_back(recRelIsoLead);
   recTrackIsoSec = new TH1D("recTrackIsoSec", "Sec Track Isolation SumPt", 30, 0, 30);
   _histoVector.push_back(recTrackIsoSec);
   recEcalIsoSec  = new TH1D("recEcalIsoSec", "Sec ECAL Isolation SumPt", 30, 0, 30);
   _histoVector.push_back(recEcalIsoSec);
   recHcalIsoSec  = new TH1D("recHcalIsoSec", "Sec HCAL Isolation SumPt", 30, 0, 30); 
   _histoVector.push_back(recHcalIsoSec);
   recRelIsoSec   = new TH1D("recRelIsoSec", "Sec Comb Relative Isolation", 30, 0, 3);
   _histoVector.push_back(recRelIsoSec);
   recLeadMuPt = new TH1D("recLeadMuPt", "Reconstructed Leading #mu p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadMuPt);
   recSecMuPt  = new TH1D("recSecMuPt", "Reconstructed Second #mu p_{T}", 200, 0, 200);
   _histoVector.push_back(recSecMuPt);
   recLeadMuEta = new TH1D("recLeadMuEta", "Reconstructed Leading #mu #eta", 50, -2.5, 2.5);
   _histoVector.push_back(recLeadMuEta);
   recSecMuEta  = new TH1D("recSecMuEta", "Reconstructed Second #mu #eta", 50, 2.5, 2.5);
   _histoVector.push_back(recSecMuEta);
   recLeadJetPt = new TH1D("recLeadJetPt", "Reconstructed Leading Jet p_{T}", 200, 0, 200);
   _histoVector.push_back(recLeadJetPt);
   recLeadJetEta = new TH1D("recLeadJetEta", "Reconstructed Leading Jet #eta", 100, -5, 5); 
   _histoVector.push_back(recLeadJetEta);
*/
   denCounterFalse = new TH1D("denCounterFalse", "Generated jet multiplicity (with a gen Z)", 10, -0.5, 9.5); 
   _histoVector.push_back(denCounterFalse);
   numCounterFalse = new TH1D("numCounterFalse", "Generated jet multiplicity (with a gen Z) not overlapping muons from Z", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterFalse);
   recoJetCounterFalse = new TH1D("recoJetCounterFalse", "Reconstructed jet multiplicity (#DeltaR test)", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterFalse);
   denCounterTrue = new TH1D("denCounterTrue", "Generated jet multiplicity (with a gen Z)", 10, -0.5, 9.5); 
   _histoVector.push_back(denCounterTrue);
   numCounterTrue = new TH1D("numCounterTrue", "Generated jet multiplicity (with a gen Z) not overlapping muons from Z", 10, -0.5, 9.5); 
   _histoVector.push_back(numCounterTrue);
   recoJetCounterTrue = new TH1D("recoJetCounterTrue", "Reconstructed jet multiplicity (#DeltaR test)", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetCounterTrue);
   recoJetTotalCounter = new TH1D("recoJetTotalCounter", "Total Reconstructed jet multiplicity (#DeltaR test)", 10, -0.5, 9.5); 
   _histoVector.push_back(recoJetTotalCounter);

/*   genMuPt = new TH1D("genMuPt", "Generated Muon p_{T}", 200, 0, 200); 
   _histoVector.push_back(genMuPt);
   genMuPtinDeltaR = new TH1D("genMuPtinDeltaR", "Generated Muon p_{T} (#DeltaR test)", 200, 0, 200); 
   _histoVector.push_back(genMuPtinDeltaR);
   genMuPtinDeltaRMCMatch = new TH1D("genMuPtinDeltaRMCMatch", "Generated Muon p_{T} (#DeltaR test) MC Matching", 200, 0, 200); 
   _histoVector.push_back(genMuPtinDeltaRMCMatch);
   genMuEta = new TH1D("genMuEta", "Generated Muon #eta", 100, -10, 10); 
   _histoVector.push_back(genMuEta);
   genMuEtainDeltaR = new TH1D("genMuEtainDeltaR", "Generated Muon #eta (#DeltaR test)", 100, -10, 10); 
   _histoVector.push_back(genMuEtainDeltaR);
   genMuEtainDeltaRMCMatch = new TH1D("genMuEtainDeltaRMCMatch", "Generated Muon #eta (#DeltaR test) MC Matching", 100, -10, 10); 
   _histoVector.push_back(genMuEtainDeltaRMCMatch);
   genJetTotalCounter = new TH1D("genJetTotalCounter", "Total Generated jet multiplicity", 10, -0.5, 9.5); 
   _histoVector.push_back(genJetTotalCounter);
   genJetTotalCounterinDeltaR = new TH1D("genJetTotalCounterinDeltaR", "Total Generated jet multiplicity (#DeltaR test)", 10, -0.5, 9.5); 
   _histoVector.push_back(genJetTotalCounterinDeltaR);
   genJetTotalCounterinDeltaRMCMatch = new TH1D("genJetTotalCounterinDeltaRMCMatch", "Total Generated jet multiplicity (#DeltaR test) MC Matching", 10, -0.5, 9.5); 
   _histoVector.push_back(genJetTotalCounterinDeltaRMCMatch);
*/
   recMuSiRefPt = new TH1D("recMuSiRefPt", "reconstructed #mu p_{T} with gen particle reference", 200, 0, 200); 
   _histoVector.push_back(recMuSiRefPt);
   recMuSiRefEta = new TH1D("recMuSiRefEta", "reconstructed #mu #eta with gen particle reference", 100, -5., 5.); 
   _histoVector.push_back(recMuSiRefPt);
   recMuNoRefPt = new TH1D("recMuNoRefPt", "reconstructed #mu p_{T} without gen particle reference", 200, 0, 200); 
   _histoVector.push_back(recMuNoRefPt);
   recMuNoRefEta = new TH1D("recMuNoRefEta", "reconstructed #mu #eta without gen particle reference", 100, -5., 5.); 
   _histoVector.push_back(recMuNoRefPt);

/*
   _summaryPass = new TTree("summaryPass", "summaryPass");
   _summaryPass->Branch("_runnumber", &_runnumber, "_runnumber/I");
   _summaryPass->Branch("_lumisection", &_lumisection, "_lumisection/I");
   _summaryPass->Branch("_eventnumber", &_eventnumber, "_eventnumber/I");
   _summaryPass->Branch("_mass", &_mass, "_mass/D");
   _summaryPass->Branch("_pt", &_pt, "_pt/D");
   _summaryPass->Branch("_pt0", &_pt0, "_pt0/D");
   _summaryPass->Branch("_pt1", &_pt1, "_pt1/D");
   _summaryPass->Branch("_isorel0", &_isorel0, "_isorel0/D");
   _summaryPass->Branch("_isorel1", &_isorel1, "_isorel1/D");
   _summaryPass->Branch("_isotk0", &_isotk0, "_isotk0/D");
   _summaryPass->Branch("_isotk1", &_isotk1, "_isotk1/D");
   _summaryPass->Branch("_njets", &_njets, "_njets/I");
   _summaryPass->Branch("_ptjetsum", &_ptjetsum, "_ptjetsum/D");
   _summaryPass->Branch("_rho", &_rho, "_rho/D" );
*/

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
  
  cout << "RecoMuon analyzing nr. file = "<<_fileCounter<<endl;
  cout << "RecoMuon analyzing nr. event = "<<_entries<<endl;
  
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
   
  cout << "DeltaRAnalyzer Worker built." << endl;   
}

DeltaRAnalyzer::~DeltaRAnalyzer(){
  _file->ls();
  //_file->Write();
  //_file->Close();
}

//void  DeltaRAnalyzer::process(const fwlite::ChainEvent& iEvent)
void  DeltaRAnalyzer::process(const fwlite::Event& iEvent)
{
   
   _run = iEvent.id().run();
   if(_sample=="mc")_run=-1;

   //double weight = 1.;
   _file->cd();

   fwlite::Handle<std::vector<pat::Muon> > muonHandle;
   muonHandle.getByLabel(iEvent, "selectedMuons");

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zHandle;
   //zHandle.getByLabel(iEvent, _zcandSrc.c_str());//"zmumurec");
   zHandle.getByLabel(iEvent, "zmumurec");//"zmumurec");

   fwlite::Handle<std::vector<pat::Jet> > jetHandle;
   jetHandle.getByLabel(iEvent, _JetType.c_str());

   fwlite::Handle<pat::TriggerEvent> triggerHandle; 
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");
 
   fwlite::Handle<double> Rho;
   Rho.getByLabel(iEvent, "kt6PFJets", "rho");
   
   _rho = *Rho;

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zHandlegen;
   zHandlegen.getByLabel(iEvent, "zmumugen");

   fwlite::Handle<reco::GenParticleCollection> genParticles;
   genParticles.getByLabel(iEvent, "genParticles");

   //fwlite::Handle<reco::VertexCollection> verticesHandle;
   //verticesHandle.getByLabel(iEvent, "offlinePrimaryVertices");
   
   //we need to add the piece of code that select the Z candidate in case of multiple candidates
   //if (zHandle->size() > 1) return; 
   
   //we just take the first element in the collection of Z candidates.
   //That is the candidate in which the leading muon has the highest pt
   
   std::vector<const pat::Muon*> muonsfromZ;  
   if(zHandle->size())muonsfromZ = ZRECDaughters(*zHandle);
   
      std::vector<const pat::Jet*> jetcollID = GetJets<pat::Jet>(*jetHandle);     
      std::vector<const pat::Jet*> recjets;
         
      if(muonsfromZ.size()){             
      for(unsigned int i = 0; i < jetcollID.size(); i++){         
      if(IsoJet<pat::Muon>(muonsfromZ,*jetcollID[i]))recjets.push_back(jetcollID[i]);}}

   fwlite::Handle<std::vector<reco::GenJet> > jetHandler;
   jetHandler.getByLabel(iEvent, "selectedGenJetsOld"); //jets senza Mu
   //jetHandler.getByLabel(iEvent, "selectedGenJetsNoMuMufromZ");  //jets con Mu
   std::vector<const reco::GenJet*> genjetsr = GetJets_noJetID<reco::GenJet>(*jetHandler); 

// if (muonsfromZ.size()== 2) { 

/*
	cout << "(*zHandlegen).size() = " << (*zHandlegen).size() << endl;
	cout << "(*zHandlegen)[0].daughter(0))->charge() = " << ((*zHandlegen)[0].daughter(0))->charge() <<endl;
	cout << "(*zHandlegen)[0].daughter(1))->charge() = " << ((*zHandlegen)[0].daughter(1))->charge() <<endl;
	cout << "GenSelectedInAcceptanceMuon(*zHandlegen) = " << GenSelectedInAcceptanceMuon(*zHandlegen) << endl;
*/
	//qui apro l'if con le condizioni su Z e muoni
	if (GenSelectedInAcceptanceStatus1(*zHandlegen, _selections) &&
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
                //double DeltaR = 0.4; 
		double DeltaR = isojetcut;

		//histo pre-overlap

//		genJetTotalCounter-> Fill(genjetsr.size());

		for (unsigned int a = 0; a < 2 ; ++a) {
			_dir->cd();
			addHistosVsMulti(genjetsr.size(), "genMuPt", " - Gen: Generated Muon p_{T}", 200, 0, 200, genMuPt);
			addHistosVsMulti(genjetsr.size(), "genMuEta", " - Gen: Generated Muon #eta", 100, -5., 5., genMuEta);
			_dir->cd("-");
			genMuPt[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(a))->pt());
			genMuEta[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(a))->eta());
//			genMuPt-> Fill(((*zHandlegen)[0].daughter(a))->pt());
//			genMuEta-> Fill(((*zHandlegen)[0].daughter(a))->eta());
		}

        	//Analisi di efficienza di singolo Mu
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
			// else if (genjetsr.size() == 0) overlapSingMu = true; //potrei mettere così per i Fake (ma ne faccio a meno: così ho un campione complementare a quello non fake)
		
			// qui apro il controllo di overlapSingMu
			if (!overlapSingMu){
			//if(overlapSingMu){ //per i Fake di singolo Mu
				_dir->cd();
				addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaR", " - Gen: Generated Muon p_{T} (#DeltaR test)", 200, 0, 200, genMuPtinDeltaR);
				addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaR", " - Gen: Generated Muon #eta (#DeltaR test)", 100, -5., 5., genMuEtainDeltaR);
				_dir->cd("-");
				genMuPtinDeltaR[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
				genMuEtainDeltaR[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());

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

							if ((*muonHandle)[y].genParticle()->pt() == ((*zHandlegen)[0].daughter(c))->pt() && 
			     				    (*muonHandle)[y].genParticle()->eta() == ((*zHandlegen)[0].daughter(c))->eta()){
								cout << "MC Matching trovato: (*muonHandle)[" << y << "] matcha con ((*zHandlegen)[0].daughter(" << c << "))" << endl;
								_dir->cd();
								addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaRMCMatch)", " - Gen: Generated Muon p_{T} (#DeltaR test) MC Match", 200, 0, 200, genMuPtinDeltaRMCMatch);
								addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaRMCMatch)", " - Gen: Generated Muon #eta (#DeltaR test) MC Match", 100, -5., 5., genMuEtainDeltaRMCMatch);
								_dir->cd("-");
								genMuPtinDeltaRMCMatch[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
								genMuEtainDeltaRMCMatch[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());
//								genMuPtinDeltaRMCMatch-> Fill(((*zHandlegen)[0].daughter(c))->pt());
//								genMuEtainDeltaRMCMatch-> Fill(((*zHandlegen)[0].daughter(c))->eta());
//								genJetTotalCounterinDeltaRMCMatch-> Fill(genjetsr.size());
							}
							else {
								cout << "Vedo che (*muonHandle)[" << y << "] non matcha con ((*zHandlegen)[0].daughter(" << c << "))" << endl;
							}
						}
						else {
							recMuNoRefPt->Fill((*muonHandle)[y].pt());
							recMuNoRefEta->Fill((*muonHandle)[y].eta());
						}
					}
				}
			}// qui chiudo il controllo di overlapSingMu
		}// qui chiudo il loop sui 2 Mu


		//Analisi di efficienza di doppio Mu
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
		// else if (genjetsr.size() == 0) overlap = true; //potrei mettere così per i Fake (ma ne faccio a meno: così ho un campione complementare a quello non fake)

		//qui apro il controllo di overlap false di 2 Mu
		if(!overlap){

			//histo di overlap
			denCounterFalse->Fill(genjetsr.size());

			//qui apro la sezione della Z ricostruita
					
			if (RecSelected(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){   //recjets.size()>=1 ???? /*&&

				numCounterFalse->Fill(genjetsr.size());
				recoJetCounterFalse-> Fill(recjets.size());

			} // qui chiudo la sezione della Z ricostruita
		}// qui chiudo il controllo di overlap false di 2 Mu

		//qui apro il controllo di overlap true di 2 Mu
		else {

			//histo di overlap
			denCounterTrue->Fill(genjetsr.size());

			//qui apro la sezione della Z ricostruita
			if (RecSelected(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){   //recjets.size()>=1 ???? /*&&

				numCounterTrue->Fill(genjetsr.size());
				recoJetCounterTrue-> Fill(recjets.size());

			} // qui chiudo la sezione della Z ricostruita
		}// qui chiudo il controllo di overlap true di 2 Mu

		//qui apro la sezione della Z ricostruita senza richiesta di overlap
		if (RecSelected(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && RecSelected(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){   //recjets.size()>=1 ???? /*&&

			recoJetTotalCounter-> Fill(recjets.size());

		} // qui chiudo la sezione della Z ricostruita senza richiesta di overlap


	} //qui chiudo l'if con le condizioni su Z e muoni



//	}// qui chiudo l'if su muonfromZ.size()

} // chiudo tutto vecchio stile


/*
				_runnumber  = iEvent.id().run();
				_eventnumber = iEvent.id().event();
				_lumisection = iEvent.id().luminosityBlock();
				// cout << "controllo 1"<< endl;
				_pt = (*zHandle)[0].pt();
				_mass = (*zHandle)[0].mass();
				// cout << "controllo 2"<< endl;
				const reco::MuonIsolation& iso03_0 = muonsfromZ[0]->isolationR03();
				// cout << "controllo 3"<< endl;
				const reco::MuonIsolation& iso03_1 = muonsfromZ[1]->isolationR03();
				// cout << "controllo 4"<< endl;
				_pt0 = (*zHandle)[0].daughter(0)->pt();
				_pt1 = (*zHandle)[0].daughter(1)->pt();
				_isorel0 = (iso03_0.hadEt + iso03_0.emEt + iso03_0.sumPt)/(*zHandle)[0].daughter(0)->pt();
				_isorel1 = (iso03_1.hadEt + iso03_1.emEt + iso03_1.sumPt)/(*zHandle)[0].daughter(1)->pt();
				_isotk0 = iso03_0.sumPt;  
				_isotk1 = iso03_1.sumPt;  
				_rho = *Rho;

				recPtZ->Fill((*zHandle)[0].pt(), _norm);
				recEtaZ->Fill((*zHandle)[0].eta(), _norm);
				recMassZ->Fill((*zHandle)[0].mass(), _norm);
				recLeadMuPt->Fill((*zHandle)[0].daughter(0)->pt(), _norm);
				recLeadMuEta->Fill((*zHandle)[0].daughter(0)->eta(), _norm);
				recSecMuPt->Fill((*zHandle)[0].daughter(1)->pt(), _norm);
				recSecMuEta->Fill((*zHandle)[0].daughter(1)->eta(), _norm);

  
				_dir->cd();
  
				addHistosVsMulti(recjets.size(), "recJetPtIncl", " reconstructed jet p_{T} spectrum", 200, 0, 200, recJetPtVsInclMulti);
				addHistosVsMulti(recjets.size(), "recJetEtaIncl", " reconstructed jet #eta spectrum", 100, -5., 5., recJetEtaVsInclMulti);
				addHistosVsMulti(recjets.size(), "recZPtIncl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsInclMulti);
				addHistosVsMulti(recjets.size(), "recZEtaIncl", " reconstructed Z #eta spectrum", 100, -5., 5., recZEtaVsInclMulti);
				addHistosVsMulti(recjets.size(), "recZPtExcl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsExclMulti);
				addHistosVsMulti(recjets.size(), "recZEtaExcl", " reconstructed Z #eta spectrum", 100, -5., 5., recZEtaVsExclMulti);
				addHistosVsMulti(recjets.size(), "recZPtExcl", " reconstructed Z p_{T} spectrum", 200, 0., 200., recZPtVsExclMulti);
				addHistosVsMulti(recjets.size(), "recMu1PtExcl", " reconstructed lead #mu p_{T} spectrum", 200, 0., 200., recMu1PtVsExclMulti);
				addHistosVsMulti(recjets.size(), "recMu1EtaExcl", " reconstructed lead #mu #eta spectrum", 100, -5., 5., recMu1EtaVsExclMulti);
				addHistosVsMulti(recjets.size(), "recMu2PtExcl", " reconstructed sec #mu p_{T} spectrum", 200, 0., 200., recMu2PtVsExclMulti);
				addHistosVsMulti(recjets.size(), "recMu2EtaExcl", " reconstructed sec #mu #eta spectrum", 100, -5., 5., recMu2EtaVsExclMulti);   

				_dir->cd("-");

				recMulti->Fill(recjets.size(), _norm);
 
				if (recjets.size()){
					recLeadJetPt->Fill(recjets[0]->pt(), _norm);
					recLeadJetEta->Fill(recjets[0]->eta(), _norm);
					for (unsigned int i = 0; i < recjets.size(); ++i){
					recJetPtVsInclMulti[i+1]->Fill(recjets[i]->pt(), _norm);
					recJetEtaVsInclMulti[i+1]->Fill(recjets[i]->eta(), _norm);
					}
				}
				_njets=recjets.size();
				reco::Candidate::LorentzVector jetpsum(0., 0., 0., 0.);

        
				for (unsigned int i = 0; i < recjets.size(); ++i){
				jetpsum += recjets[i]->p4();
				}  
      
				_ptjetsum = jetpsum.pt();

				//fill inclusive histograms
				for (unsigned int i = 0; i < recjets.size()+1; ++i){
				recZPtVsInclMulti[i]->Fill((*zHandle)[0].pt(), _norm);
				recZEtaVsInclMulti[i]->Fill((*zHandle)[0].eta(), _norm);
				}

				_summaryPass->Fill();

				//fill exclusive histograms
				recZPtVsExclMulti[recjets.size()]->Fill((*zHandle)[0].pt(), _norm);
				recZEtaVsExclMulti[recjets.size()]->Fill((*zHandle)[0].eta(), _norm);
				recMu1PtVsExclMulti[recjets.size()]->Fill(dau0->pt(), _norm);
				recMu1EtaVsExclMulti[recjets.size()]->Fill(dau0->eta(), _norm);
				recMu2PtVsExclMulti[recjets.size()]->Fill(dau1->pt(), _norm);
				recMu2EtaVsExclMulti[recjets.size()]->Fill(dau1->eta(), _norm);
*/
   
//			} // qui chiudo la sezione della Z ricostruita vecchio stile
//		}// qui chiudo il controllo di overlap false di 2 Mu vecchio stile

/* string IsoFlag, MuIDFlag;
   if(_selections=="SYM"){
   IsoFlag="_IsoSYM";
   MuIDFlag="_MuIDSYM";}
   if(_selections=="ASYM"){
   IsoFlag="_IsoASYM";
   MuIDFlag="_MuIDASYM";}
 
   for(int fcount = 1; fcount<7; fcount++){
   
   if(fcount != 6){
   if(_RecoCutFlags[fcount+1] == IsoFlag.c_str()){   
   bool PreIso = false;
   for(int n = 1; n < fcount+1; n++){
   if(RecSelected(_RecoCutFlags[n].c_str(), *zHandle, *triggerHandle, _run, _rho)){
   PreIso = true;
   }else{
   PreIso = false;
   n = fcount+1;
   }
   }

   if (PreIso){
      recTrackIsoLead->Fill(dau0->trackIso());
      recEcalIsoLead->Fill(dau0->ecalIso());
      recHcalIsoLead->Fill(dau0->hcalIso());
      recRelIsoLead->Fill( (dau0->trackIso() + dau0->ecalIso() + dau0->hcalIso()) / dau0->pt());
      recTrackIsoSec->Fill(dau1->trackIso());
      recEcalIsoSec->Fill(dau1->ecalIso());
      recHcalIsoSec->Fill(dau1->hcalIso());
      recRelIsoSec->Fill( (dau1->trackIso() + dau1->ecalIso() + dau1->hcalIso()) / dau1->pt());
   }
*/
//	} //qui chiudo l'if con le condizioni su Z e muoni vecchio stile
////	}// qui chiudo l'if su muonfromZ.size() vecchio stile

//} // chiudo tutto vecchio stile

void DeltaRAnalyzer::finalize(){
   /*_histoVector.insert(_histoVector.end(), recJetPtVsInclMulti.begin(), recJetPtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recJetEtaVsInclMulti.begin(), recJetEtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recZPtVsInclMulti.begin(), recZPtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recZEtaVsInclMulti.begin(), recZEtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu1PtVsInclMulti.begin(), recMu1PtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu1EtaVsInclMulti.begin(), recMu1EtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu2PtVsInclMulti.begin(), recMu2PtVsInclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu2EtaVsInclMulti.begin(), recMu2EtaVsInclMulti.end());

   _histoVector.insert(_histoVector.end(), recZPtVsExclMulti.begin(), recZPtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recZEtaVsExclMulti.begin(), recZEtaVsExclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu1PtVsExclMulti.begin(), recMu1PtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu1EtaVsExclMulti.begin(), recMu1EtaVsExclMulti.end());

   _histoVector.insert(_histoVector.end(), recMu2PtVsExclMulti.begin(), recMu2PtVsExclMulti.end());
   _histoVector.insert(_histoVector.end(), recMu2EtaVsExclMulti.begin(), recMu2EtaVsExclMulti.end()); 

   std::vector<TH1D*>::const_iterator ibeg = _histoVector.begin();
   std::vector<TH1D*>::const_iterator iend = _histoVector.end();
   //use the errors as they were from real data 
   for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
     if (*i) //(*i)->Scale(_norm);
      (*i)->Sumw2();
   }
   */
   
  _file->Write();
  //_file->Close();
}
