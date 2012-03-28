#include "Firenze/PATAnalysis/include/GenMuon.h"
#include "Firenze/PATAnalysis/include/MuonUtilities.h"

#include <TH2.h>
#include <TStyle.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include "TList.h"
#include "TParameter.h"
#include "TFile.h"
#include "TProofOutputFile.h"
#include "TChain.h"
#include "TGraphAsymmErrors.h"
#include "TDirectory.h"

//CMSSW headers
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace std;
using namespace edm;

GenMuon::GenMuon():  

genPtZ(0), genEtaZ(0), genMassZ(0), 
genPtZ_Acc(0), genEtaZ_Acc(0), genMassZ_Acc(0),

GenIsoJetPt_Acc(0), GenJetPt_Acc(0), genLeadIsoJetPt_Acc(0), genLeadIsoJetEta_Acc(0),

GenJetCounter(0), GenJetCounter_Acc(0), GenIsoJetCounter(0), GenIsoJetCounter_Acc(0),  

genLeadMuPt(0), genSecMuPt(0), genLeadMuEta(0), genSecMuEta(0),
genLeadMuPt_Acc(0), genSecMuPt_Acc(0), genLeadMuEta_Acc(0), genSecMuEta_Acc(0),

JetMinDeltaRZDau_GEN(0),

_ptjetmin(30.), _etajetmax(3.), _norm(1.), _Norm(false), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1), _file(0), _dir(0), _Zdir(0), _Mudir(0), _Jetdir(0), _selections("SYM"), _histovector()
{   }


void GenMuon::begin(TFile* out, const edm::ParameterSet& iConfig){
   _file = out;

   std::string dirname = iConfig.getParameter<std::string>("Name");
   std::string sourceFileList = iConfig.getParameter<std::string>("sourceFileList");
   _selections = iConfig.getParameter<std::string>("Selections");
   _targetLumi= iConfig.getParameter<double>("targetLumi");
   _xsec      = iConfig.getParameter<double>("CrossSection");
   _Norm      = iConfig.getParameter<bool>("Norm");
   _EventsPerFile      = iConfig.getParameter<int32_t>("EventsPerFile");
   _EventNumber    = iConfig.getParameter<int32_t>("EventNumber");
   _ProcEvents    = iConfig.getParameter<int32_t>("ProcEvents");

   cout << "GenMuon file name : " << _file->GetName() << endl;
   _file->cd();
   _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
   _dir->cd();
   
   //Z variables
   _Zdir = _dir->mkdir("genZ_Plots");
   _Zdir->cd();
   
   genPtZ   = new TH1D("genPtZ", "Generated Z p_{T}", 200, 0, 200);
   _histovector.push_back(genPtZ);
   genEtaZ  = new TH1D("genEtaZ", "Generated Z #eta", 100, -10, 10);
   _histovector.push_back(genEtaZ);
   genMassZ = new TH1D("genMassZ", "Generated Z mass", 100, 60, 120);
   _histovector.push_back(genMassZ);
   genPtZ_Acc   = new TH1D("genPtZ_Acc", "Generated Z p_{T}, Selections: Acc", 200, 0, 200);
   _histovector.push_back(genPtZ_Acc);
   genEtaZ_Acc  = new TH1D("genEtaZ_Acc", "Generated Z #eta, Selections: Acc", 100, -10, 10);
   _histovector.push_back(genEtaZ_Acc);
   genMassZ_Acc = new TH1D("genMassZ_Acc", "Generated Z mass, Selections: Acc", 200, 50, 150);
   _histovector.push_back(genMassZ_Acc);
   
   //Z Muons variables
   _Mudir = _dir->mkdir("genZMuons_Plots");
   _Mudir->cd();
   
   genLeadMuPt = new TH1D("genLeadMuPt", "Generated Leading muon p_{T}", 200, 0, 200);
   _histovector.push_back(genLeadMuPt);
   genSecMuPt  = new TH1D("genSecMuPt", "Generated Second muon p_{T}", 200, 0, 200);
   _histovector.push_back(genSecMuPt);
   genLeadMuEta = new TH1D("genLeadMuEta", "Generated Leading muon #eta", 100, -2.5, 2.5);
   _histovector.push_back(genLeadMuEta);
   genSecMuEta  = new TH1D("genSecMuEta", "Generated Second muon #eta", 100, -2.5, 2.5);
   _histovector.push_back(genSecMuEta);   
   genLeadMuPt_Acc = new TH1D("genLeadMuPt_Acc", "Generated Leading muon p_{T}, Selection: Acc", 200, 0, 200);
   _histovector.push_back(genLeadMuPt_Acc);
   genSecMuPt_Acc  = new TH1D("genSecMuPt_Acc", "Generated Second muon p_{T}, Selection: Acc", 200, 0, 200);
   _histovector.push_back(genSecMuPt_Acc);
   genLeadMuEta_Acc = new TH1D("genLeadMuEta_Acc", "Generated Leading muon #eta, Selection: Acc", 100, -2.5, 2.5);
   _histovector.push_back(genLeadMuEta_Acc);
   genSecMuEta_Acc  = new TH1D("genSecMuEta_Acc", "Generated Second muon #eta, Selection: Acc", 100, -2.5, 2.5);
   _histovector.push_back(genSecMuEta_Acc);
   
   //Jet variables
   _Jetdir = _dir->mkdir("genJet_Plots");
   _Jetdir->cd();
   
   GenIsoJetCounter = new TH1D("GenIsoJetCounter", "Number of Generated Iso Jet per event", 10, 0, 10);
   _histovector.push_back(GenIsoJetCounter);
   GenJetCounter = new TH1D("GenJetCounter", "Number of Generated Jet per event", 10, 0, 10);
   _histovector.push_back(GenJetCounter);  
   GenJetCounter_Acc = new TH1D("GenJetCounter_Acc", "Number of Generated Jet per event in Acc", 10, 0, 10);
   _histovector.push_back(GenJetCounter_Acc);
   GenIsoJetCounter_Acc = new TH1D("GenIsoJetCounter_Acc", "Number of Generated Iso Jet per event in Acc", 10, 0, 10);
   _histovector.push_back(GenIsoJetCounter_Acc);
   GenIsoJetPt_Acc = new TH1D("GenIsoJetPt_Acc", "Generated Iso Jet p_{T} in Acc", 250, 0, 250);
   _histovector.push_back(GenIsoJetPt_Acc);
   GenJetPt_Acc = new TH1D("GenJetPt_Acc", "Generated Jet p_{T} in Acc", 250, 0, 250);
   _histovector.push_back(GenJetPt_Acc);
   genLeadIsoJetPt_Acc = new TH1D("genLeadIsoJetPt_Acc", "Generated Leading Jet p_{T} in Acc", 200, 0, 200);
   _histovector.push_back(genLeadIsoJetPt_Acc);
   genLeadIsoJetEta_Acc = new TH1D("genLeadIsoJetEta_Acc", "Generated Leading Jet #eta in Acc", 100, -5, 5);
   _histovector.push_back(genLeadIsoJetEta_Acc);
   JetMinDeltaRZDau_GEN = new TH1D("JetMinDeltaRZDau_GEN", "Min Delta R Z Daughters All Gen Jets", 100, 0, 10);
   _histovector.push_back(JetMinDeltaRZDau_GEN);
   

  int fileCounter = 0;
   
  TChain *ch = new TChain("Events");
  ifstream infile;
  infile.open(sourceFileList.c_str());
  string datafile;
  while(getline (infile, datafile)){
  	ch->Add(datafile.c_str());
    	fileCounter++;
  }
   
  if(_EventNumber==0 && _EventsPerFile==0)_entries = ch->GetEntries();
  
  if(_EventNumber!=0 && _EventsPerFile==0)_entries = _EventNumber;
 
  if(_EventNumber==0 && _EventsPerFile!=0)_entries = fileCounter*_EventsPerFile;
  
  if(_ProcEvents!=-1)_entries = _ProcEvents;
  
  cout<<"RecoMuon analyzing nr. file = "<<fileCounter<<endl;
  cout<<"RecoMuon analyzing nr. event = "<<_entries<<endl;
  
  delete ch;

  cout << "GenMuon Worker configured." << endl;   
}


GenMuon::~GenMuon(){
  _file->ls();
}


void  GenMuon::process(const fwlite::Event& iEvent){

   _file->cd();

   double weight = 1.;

   //----richiamo la GEN Z----
   fwlite::Handle<std::vector<reco::CompositeCandidate> > zgenHandle;
   //zgenHandle.getByLabel(iEvent, "zmumugen");
   zgenHandle.getByLabel(iEvent, "zmumugenfull");
   //seleziono un vettore di 2 muoni figli della GEN Z e li ordino per pt
   std::vector<const reco::Candidate*> zgendaughters;  
   const reco::Candidate *dau0 = 0;
   const reco::Candidate *dau1 = 0;
   if(zgenHandle->size()){ 
   	zgendaughters = ZGENDaughters((*zgenHandle));
   	if(zgendaughters.size() != 0){        
    		dau0 = zgendaughters[0];
     		dau1 = zgendaughters[1];
   		if(dau1->pt() > dau0->pt())throw cms::Exception("PATAnalysis:GenMuon_WrongMuonOrder") << "ERROR! Z muons are in wrong order!";
   	}
   }

   //----richiamo i GEN Jets----
   fwlite::Handle<std::vector<reco::GenJet> > jetgenHandle;
   jetgenHandle.getByLabel(iEvent, "selectedGenJetsOld"); //jets senza Mu PG
   //jetgenHandle.getByLabel(iEvent, "selectedGenJetsNoMuMufromZ");  //jets senza Mu a mano
   //seleziono un vettore di GEN Jets per Accettanza
   std::vector<const reco::GenJet*> genjets = GetJets_noJetID<reco::GenJet>(*jetgenHandle);   
   //seleziono un vettore di GEN Jets Cleaned dai 2 muoni figli della GEN Z, qualora esistano
   std::vector<const reco::GenJet*> isogenjets;
   if(zgendaughters.size()){
   	for(unsigned int i = 0; i < genjets.size(); i++){
   		if(IsoJet<reco::Candidate>(zgendaughters,*genjets[i]))
			isogenjets.push_back(genjets[i]);
   	}
   }
   else if(!zgendaughters.size()){
   	for(unsigned int i = 0; i < genjets.size(); i++)
		isogenjets.push_back(genjets[i]);
   }

  
   // inizia l'analisi 
   if(zgenHandle->size()){
        
   	//Events with a selected GEN Zmumu - NO Acceptance cuts applied
   	if(GenSelected(*zgenHandle, _selections) &&
   	   zgendaughters.size()!=0){
   
      		genPtZ->Fill((*zgenHandle)[0].pt(), weight);    
      		genEtaZ->Fill((*zgenHandle)[0].eta(), weight);
      		genMassZ->Fill((*zgenHandle)[0].mass(), weight);
   
      		double leadmupt, leadmueta, secondmupt, secondmueta;
      
      		leadmupt  = dau0->pt(); 
      		secondmupt  = dau1->pt();
      		leadmueta = dau0->eta();
      		secondmueta = dau1->eta();
      
      		if(secondmupt>leadmupt)throw cms::Exception("PATAnalysis:GenMuon_WrongMuonOrder") << "ERROR! Z muons are in wrong order!";
      
      		genLeadMuPt->Fill(leadmupt, weight);
      		genLeadMuEta->Fill(leadmueta, weight);
      		genSecMuPt->Fill(secondmupt, weight);
      		genSecMuEta->Fill(secondmueta, weight); 
      
      		GenJetCounter->Fill(genjets.size());
      		GenIsoJetCounter->Fill(isogenjets.size()); 
   
   	}

   	//Events with a selected GEN Zmumu in Acceptance
   	if (GenSelectedInAcceptance(*zgenHandle, _selections) && 
   	    zgendaughters.size()!=0){
   
      		genPtZ_Acc->Fill((*zgenHandle)[0].pt(), weight);
      		genEtaZ_Acc->Fill((*zgenHandle)[0].eta(), weight);
      		genMassZ_Acc->Fill((*zgenHandle)[0].mass(), weight);
      
      		double leadmupt, leadmueta, secondmupt, secondmueta;
      		leadmupt  = dau0->pt(); 
      		secondmupt  = dau1->pt();
      		leadmueta = dau0->eta();
      		secondmueta = dau1->eta();
      
      		if(secondmupt>leadmupt)throw cms::Exception("PATAnalysis:GenMuon_WrongMuonOrder") << "ERROR! Z muons are in wrong order!";
      
      		genLeadMuPt_Acc->Fill(leadmupt, weight);
      		genLeadMuEta_Acc->Fill(leadmueta, weight);
      		genSecMuPt_Acc->Fill(secondmupt, weight);
      		genSecMuEta_Acc->Fill(secondmueta, weight);  
      
      		GenJetCounter_Acc->Fill(genjets.size());
      
      		GenIsoJetCounter_Acc->Fill(isogenjets.size());
      
      		for(unsigned int i = 0; i < genjets.size(); i++){
      			GenJetPt_Acc->Fill(genjets[i]->pt());
      			JetMinDeltaRZDau_GEN->Fill(MinDeltaRZDau<reco::Candidate>(zgendaughters,*genjets[i]));
      		}
      
      		for(unsigned int i = 0; i < isogenjets.size(); i++){
      			GenIsoJetPt_Acc->Fill(isogenjets[i]->pt());
      		}
          
      		if(isogenjets.size()){
      			genLeadIsoJetPt_Acc->Fill(isogenjets[0]->pt(), weight);
      			genLeadIsoJetEta_Acc->Fill(isogenjets[0]->eta(), weight);   
      		}

      
      		_Jetdir->cd();
      		addHistosVsMulti(isogenjets.size(), "genJetPtIncl", " generated jet p_{T} spectrum", 200, 0, 200, genJetPtVsInclMulti);
      		addHistosVsMulti(isogenjets.size(), "genJetEtaIncl", " generated jet #eta spectrum", 100, -5., 5., genJetEtaVsInclMulti);

      		_Zdir->cd();
      		addHistosVsMulti(isogenjets.size(), "genZPtIncl", " generated Z p_{T} spectrum", 200, 0., 200., genZPtVsInclMulti);
      		addHistosVsMulti(isogenjets.size(), "genZEtaIncl", " generated Z #eta spectrum", 100, -5., 5., genZEtaVsInclMulti);
      		addHistosVsMulti(isogenjets.size(), "genZPtExcl", " generated Z p_{T} spectrum", 200, 0., 200., genZPtVsExclMulti);
      		addHistosVsMulti(isogenjets.size(), "genZEtaExcl", " generated Z #eta spectrum", 100, -5., 5., genZEtaVsExclMulti);

      		_Mudir->cd();
      		addHistosVsMulti(isogenjets.size(), "genMu1PtExcl", " generated lead muon p_{T} spectrum", 200, 0., 200., genMu1PtVsExclMulti);
      		addHistosVsMulti(isogenjets.size(), "genMu1EtaExcl", " generated lead muon #eta spectrum", 100, -5., 5., genMu1EtaVsExclMulti);
      		addHistosVsMulti(isogenjets.size(), "genMu2PtExcl", " generated sec muon p_{T} spectrum", 200, 0., 200., genMu2PtVsExclMulti);
      		addHistosVsMulti(isogenjets.size(), "genMu2EtaExcl", " generated sec muon #eta spectrum", 100, -5., 5., genMu2EtaVsExclMulti);    
     		addHistosVsMulti(isogenjets.size(), "genMu1PtIncl", " generated lead muon p_{T} spectrum", 200, 0., 200., genMu1PtVsInclMulti);
      		addHistosVsMulti(isogenjets.size(), "genMu1EtaIncl", " generated lead muon #eta spectrum", 100, -5., 5., genMu1EtaVsInclMulti);
      		addHistosVsMulti(isogenjets.size(), "genMu2PtIncl", " generated sec muon p_{T} spectrum", 200, 0., 200., genMu2PtVsInclMulti);
      		addHistosVsMulti(isogenjets.size(), "genMu2EtaIncl", " generated sec muon #eta spectrum", 100, -5., 5., genMu2EtaVsInclMulti);

        	for (unsigned int i = 0; i < isogenjets.size(); ++i){
          		genJetPtVsInclMulti[i+1]->Fill(isogenjets[i]->pt(), weight);
          		genJetEtaVsInclMulti[i+1]->Fill(isogenjets[i]->eta(), weight);
        	}
      

      		//fill inclusive quantities
      		for (unsigned int i = 0; i < isogenjets.size()+1; ++i){
        		genZPtVsInclMulti[i]->Fill((*zgenHandle)[0].pt(), weight);
        		genZEtaVsInclMulti[i]->Fill((*zgenHandle)[0].eta(), weight);
        		genMu1PtVsInclMulti[i]->Fill(leadmupt, weight);
        		genMu2PtVsInclMulti[i]->Fill(secondmupt, weight);
        		genMu1EtaVsInclMulti[i]->Fill(leadmueta, weight);
        		genMu2EtaVsInclMulti[i]->Fill(secondmueta, weight);
      		}

      		//fill exclusive quantities
      		genZPtVsExclMulti[isogenjets.size()]->Fill((*zgenHandle)[0].pt(), weight);
      		genZEtaVsExclMulti[isogenjets.size()]->Fill((*zgenHandle)[0].eta(), weight);
      		genMu1PtVsExclMulti[isogenjets.size()]->Fill(leadmupt, weight);
      		genMu1EtaVsExclMulti[isogenjets.size()]->Fill(leadmueta, weight);
      		genMu2PtVsExclMulti[isogenjets.size()]->Fill(secondmupt, weight);
      		genMu2EtaVsExclMulti[isogenjets.size()]->Fill(secondmueta, weight);

   	}
   }
}

void GenMuon::finalize(){
    
   _histovector.insert(_histovector.end(), genJetPtVsInclMulti.begin(), genJetPtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genJetEtaVsInclMulti.begin(), genJetEtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genZPtVsInclMulti.begin(), genZPtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genZEtaVsInclMulti.begin(), genZEtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genMu1PtVsInclMulti.begin(), genMu1PtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genMu1EtaVsInclMulti.begin(), genMu1EtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genMu2PtVsInclMulti.begin(), genMu2PtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genMu2EtaVsInclMulti.begin(), genMu2EtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genZPtVsExclMulti.begin(), genZPtVsExclMulti.end());
   _histovector.insert(_histovector.end(), genZEtaVsExclMulti.begin(), genZEtaVsExclMulti.end());

   _histovector.insert(_histovector.end(), genMu1PtVsExclMulti.begin(), genMu1PtVsExclMulti.end());
   _histovector.insert(_histovector.end(), genMu1EtaVsExclMulti.begin(), genMu1EtaVsExclMulti.end());

   _histovector.insert(_histovector.end(), genMu2PtVsExclMulti.begin(), genMu2PtVsExclMulti.end());
   _histovector.insert(_histovector.end(), genMu2EtaVsExclMulti.begin(), genMu2EtaVsExclMulti.end());
   
   //Normalization
   
   std::vector<TH1D*>::const_iterator ibeg = _histovector.begin();
   std::vector<TH1D*>::const_iterator iend = _histovector.end();
   
   double lumi = _entries/_xsec;

   if(_Norm && lumi!=0){
   _norm = _targetLumi/lumi;
   }
   
   for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){
    (*i)->Sumw2();
   }
   
   for (std::vector<TH1D*>::const_iterator i = ibeg; i != iend; ++i){     
     if (*i) (*i)->Scale(_norm);
   }
   
   _file->Write();

}
