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

recMuSiRefPt(0), recMuSiRefEta(0),
recMuNoRefPt(0), recMuNoRefEta(0),

_targetLumi(50.), _xsec(1.), _norm(1.), _Norm(false), _rho(0), _run(-1), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1), _fileCounter(0), _Acc(0), _Trg(0), _Qual(0), _Imp(0), _Iso(0), _MuID(0), _JECUnc(0), _selections("SYM"), _JetType("selectedJetsL1Corrected"), _sample("mc"),

_file(0), _dir(0), _dir1(0), _dir2(0), _histoVector()

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
   _dir2 = _dir->mkdir("DoubleMu_Overlap");
   _dir->cd();
   _dir2->cd();

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

   _dir2->cd("-");

   _dir1->cd();

   recMuSiRefPt = new TH1D("recMuSiRefPt", "reconstructed #mu p_{T} with gen particle reference", 200, 0, 200); 
   _histoVector.push_back(recMuSiRefPt);
   recMuSiRefEta = new TH1D("recMuSiRefEta", "reconstructed #mu #eta with gen particle reference", 100, -5., 5.); 
   _histoVector.push_back(recMuSiRefPt);
   recMuNoRefPt = new TH1D("recMuNoRefPt", "reconstructed #mu p_{T} without gen particle reference", 200, 0, 200); 
   _histoVector.push_back(recMuNoRefPt);
   recMuNoRefEta = new TH1D("recMuNoRefEta", "reconstructed #mu #eta without gen particle reference", 100, -5., 5.); 
   _histoVector.push_back(recMuNoRefPt);

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
   //seleziono un vettore di 2 muoni dalla REC Z
   std::vector<const pat::Muon*> muonsfromZ;  
   if(zHandle->size()) muonsfromZ = ZRECDaughters(*zHandle);

   //----richiamo i REC Jets----
   fwlite::Handle<std::vector<pat::Jet> > jetHandle;
   jetHandle.getByLabel(iEvent, _JetType.c_str());
   //seleziono un vettore di jets REC per Accettanza e ID
   std::vector<const pat::Jet*> jetcollID = GetJets<pat::Jet>(*jetHandle);     
   //seleziono un vettore di jets REC con Cleaning dai 2 Muoni dalla Z REC
   std::vector<const pat::Jet*> recjets = CleanJets<pat::Jet>(jetcollID, muonsfromZ);   

   //----richiamo i Trigger paths----
   fwlite::Handle<pat::TriggerEvent> triggerHandle; 
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");
 
   //----richiamo la rho----
   fwlite::Handle<double> Rho;
   Rho.getByLabel(iEvent, "kt6PFJets", "rho");
   _rho = *Rho;

   //----richiamo la GEN Z----
   fwlite::Handle<std::vector<reco::CompositeCandidate> > zHandlegen;
   zHandlegen.getByLabel(iEvent, "zmumugen");

   //----richiamo la GenParticles per il MC Matching----
   fwlite::Handle<reco::GenParticleCollection> genParticles;
   genParticles.getByLabel(iEvent, "genParticles");
   
   //----richiamo i GEN Jets----
   fwlite::Handle<std::vector<reco::GenJet> > jetHandler;
   jetHandler.getByLabel(iEvent, "selectedGenJetsOld");
   //seleziono un vettore di GEN Jets per Accettanza
   std::vector<const reco::GenJet*> genjetsr = GetJets_noJetID<reco::GenJet>(*jetHandler); 

   _dir->cd();
   _dir1->cd();

   addHistosVsMulti(genjetsr.size(), "genMuPt", " - Gen: Generated #mu p_{T}", 200, 0, 200, genMuPt);
   addHistosVsMulti(genjetsr.size(), "genMuEta", " - Gen: Generated #mu #eta", 100, -5., 5., genMuEta);
   addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaR", " - Gen: Generated #mu p_{T} (1 #mu overlap = false)", 200, 0, 200, genMuPtinDeltaR);
   addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaR", " - Gen: Generated #mu #eta (1 #mu overlap = false)", 100, -5., 5., genMuEtainDeltaR);
   addHistosVsMulti(genjetsr.size(), "genMuPtinDeltaRMCMatch", " - Gen: Generated #mu p_{T} (1 #mu overlap = false) MC Match", 200, 0, 200, genMuPtinDeltaRMCMatch);
   addHistosVsMulti(genjetsr.size(), "genMuEtainDeltaRMCMatch", " - Gen: Generated #mu #eta (1 #mu overlap = false) MC Match", 100, -5., 5., genMuEtainDeltaRMCMatch);

   _dir1->cd("-");
   _dir->cd("-");

   double DeltaR = 0.5;

	//qui apro l'if con le condizioni su Z e muoni
	//if (GenSelectedMuon(*zHandlegen) &&
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

		//histo pre-overlap
		for (unsigned int a = 0; a < 2 ; ++a) {
			genMuPt[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(a))->pt());
			genMuEta[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(a))->eta());
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

							if (singleMu_Probe_Acc_SYM((*muonHandle)[y], _run, _rho) &&
							    singleMu_Probe_Qual_SYM((*muonHandle)[y], _run, _rho) &&
							    singleMu_Probe_Imp((*muonHandle)[y], _run, _rho) &&
							    singleMu_Probe_Iso_SYM((*muonHandle)[y], _run, _rho)){

								if ((*muonHandle)[y].genParticle()->pt() == ((*zHandlegen)[0].daughter(c))->pt() && 
			     				    	    (*muonHandle)[y].genParticle()->eta() == ((*zHandlegen)[0].daughter(c))->eta()){
									cout << "MC Matching trovato: (*muonHandle)[" << y << "] matcha con ((*zHandlegen)[0].daughter(" << c << "))" << endl;

									genMuPtinDeltaRMCMatch[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->pt());
									genMuEtainDeltaRMCMatch[genjetsr.size()]-> Fill(((*zHandlegen)[0].daughter(c))->eta());
								}
								else {
									cout << "Vedo che (*muonHandle)[" << y << "] non matcha con ((*zHandlegen)[0].daughter(" << c << "))" << endl;
								}
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

			//qui apro la sezione della Z ricostruita di overlap false
			if (RecSelected(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){

				numCounterFalse->Fill(genjetsr.size());
				recoJetCounterFalse-> Fill(recjets.size());

			} // qui chiudo la sezione della Z ricostruita di overlap false
		}// qui chiudo il controllo di overlap false di 2 Mu

		//qui apro il controllo di overlap true di 2 Mu
		else {

			denCounterTrue->Fill(genjetsr.size());

			//qui apro la sezione della Z ricostruita di overlap true
			if (RecSelected(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
			    RecSelected(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){

				numCounterTrue->Fill(genjetsr.size());
				recoJetCounterTrue-> Fill(recjets.size());

			} // qui chiudo la sezione della Z ricostruita di overlap true
		}// qui chiudo il controllo di overlap true di 2 Mu

		//qui apro la sezione della Z ricostruita senza richiesta di overlap
		if (RecSelected(_RecoCutFlags[1].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected(_RecoCutFlags[2].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected(_RecoCutFlags[3].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected(_RecoCutFlags[4].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected(_RecoCutFlags[5].c_str(), *zHandle, *triggerHandle, _run, _rho) && 
		    RecSelected(_RecoCutFlags[6].c_str(), *zHandle, *triggerHandle, _run, _rho)){

			genJetTotalCounter-> Fill(genjetsr.size());
			recoJetTotalCounter-> Fill(recjets.size());

		} // qui chiudo la sezione della Z ricostruita senza richiesta di overlap
		//######### Fine Analisi di efficienza di doppio Mu #########

	} //qui chiudo l'if con le condizioni su Z e muoni

} // chiudo tutto

void DeltaRAnalyzerMuon::finalize(){
  
  _file->Write();
}


