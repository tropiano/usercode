#include "Firenze/PATAnalysis/include/GenElectron.h"
#include "Firenze/PATAnalysis/include/ElectronUtilities.h"

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

GenElectron::GenElectron():  

genPtZ(0), genEtaZ(0), genMassZ(0), 
genPtZ_Acc(0), genEtaZ_Acc(0), genMassZ_Acc(0),

GenIsoJetPt_Acc(0), GenJetPt_Acc(0), genLeadIsoJetPt_Acc(0), genLeadIsoJetEta_Acc(0),

GenJetCounter(0), GenJetCounter_Acc(0), GenIsoJetCounter(0), GenIsoJetCounter_Acc(0),  

genLeadElPt(0), genSecElPt(0), genLeadElEta(0), genSecElEta(0),
genLeadElPt_Acc(0), genSecElPt_Acc(0), genLeadElEta_Acc(0), genSecElEta_Acc(0),

JetMinDeltaRZDau_GEN(0),

_ptjetmin(30.), _etajetmax(3.), _norm(1.), _Norm(false), _entries(0), _EventsPerFile(0), _EventNumber(0), _ProcEvents(-1), _file(0), _dir(0), _Zdir(0), _Eldir(0), _Jetdir(0), _selections("ASYM"), _histovector()
{   }

void GenElectron::begin(TFile* out, const edm::ParameterSet& iConfig){
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

   cout << "GenElectron file name : " << _file->GetName() << endl;
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
   
   //Z Electrons variables
   _Eldir = _dir->mkdir("genZElectrons_Plots");
   _Eldir->cd();
   
   genLeadElPt = new TH1D("genLeadElPt", "Generated Leading electron p_{T}", 200, 0, 200);
   _histovector.push_back(genLeadElPt);
   genSecElPt  = new TH1D("genSecElPt", "Generated Second electron p_{T}", 200, 0, 200);
   _histovector.push_back(genSecElPt);
   genLeadElEta = new TH1D("genLeadElEta", "Generated Leading electron #eta", 100, -2.5, 2.5);
   _histovector.push_back(genLeadElEta);
   genSecElEta  = new TH1D("genSecElEta", "Generated Second electron #eta", 100, -2.5, 2.5);
   _histovector.push_back(genSecElEta);   
   genLeadElPt_Acc = new TH1D("genLeadElPt_Acc", "Generated Leading electron p_{T}, Selection: Acc", 200, 0, 200);
   _histovector.push_back(genLeadElPt_Acc);
   genSecElPt_Acc  = new TH1D("genSecElPt_Acc", "Generated Second electron p_{T}, Selection: Acc", 200, 0, 200);
   _histovector.push_back(genSecElPt_Acc);
   genLeadElEta_Acc = new TH1D("genLeadElEta_Acc", "Generated Leading electron #eta, Selection: Acc", 100, -2.5, 2.5);
   _histovector.push_back(genLeadElEta_Acc);
   genSecElEta_Acc  = new TH1D("genSecElEta_Acc", "Generated Second electron #eta, Selection: Acc", 100, -2.5, 2.5);
   _histovector.push_back(genSecElEta_Acc);
   
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
  
  cout<<"RecoElectron analyzing nr. file = "<<fileCounter<<endl;
  cout<<"RecoElectron analyzing nr. event = "<<_entries<<endl;
  
  delete ch;

  cout << "GenElectron Worker configured." << endl;   
}

GenElectron::~GenElectron(){
  _file->ls();
}

void  GenElectron::process(const fwlite::Event& iEvent)
{

   _file->cd();

   double weight = 1.;

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zgenHandle;
   zgenHandle.getByLabel(iEvent, "zeegenfull");

   fwlite::Handle<std::vector<reco::GenJet> > jetgenHandle;
   jetgenHandle.getByLabel(iEvent, "selectedGenJets");

   fwlite::Handle<std::vector<pat::Electron> > electronHandle;
   electronHandle.getByLabel(iEvent, "selectedElectrons");
   
   std::vector<const pat::Electron*> zdaughters;
   if(zgenHandle->size())zdaughters = ZGENDaughters((*zgenHandle));
   
   std::vector<const reco::GenJet*> genjets = GetJets_noJetID<reco::GenJet>(*jetgenHandle);   
   std::vector<const reco::GenJet*> isogenjets;
 
   if(zdaughters.size()){
   for(unsigned int i = 0; i < genjets.size(); i++){
   if(IsoJet<pat::Electron>(zdaughters,*genjets[i]))isogenjets.push_back(genjets[i]);}
   }else if(!zdaughters.size()){
   for(unsigned int i = 0; i < genjets.size(); i++)isogenjets.push_back(genjets[i]);}
   
   if(zgenHandle->size()){
   
   const reco::Candidate *dau0 = 0;
   const reco::Candidate *dau1 = 0;
   
     if(zdaughters.size() != 0){        
     dau0 = zdaughters[0];
     dau1 = zdaughters[1];
     
     
     }
     
   //Events with a selected GEN Zee - NO Acceptance cuts applied
   if(GenSelected((*zgenHandle), _selections)&&zdaughters.size()!=0){
   
      genPtZ->Fill((*zgenHandle)[0].pt(), weight);
      genEtaZ->Fill((*zgenHandle)[0].eta(), weight);
      genMassZ->Fill((*zgenHandle)[0].mass(), weight);
   
      double leadelpt, leadeleta, secondelpt, secondeleta;
      leadelpt  = dau0->pt(); 
      secondelpt  = dau1->pt();
      leadeleta = dau0->eta();
      secondeleta = dau1->eta();
      
      if(secondelpt>leadelpt)throw cms::Exception("PATAnalysis:GenElectron_WrongElectronOrder") << "ERROR! Z electrons are in wrong order!";
      
      genLeadElPt->Fill(leadelpt, weight);
      genLeadElEta->Fill(leadeleta, weight);
      genSecElPt->Fill(secondelpt, weight);
      genSecElEta->Fill(secondeleta, weight); 
      
      GenJetCounter->Fill(genjets.size());
      GenIsoJetCounter->Fill(isogenjets.size()); 
   
   }
   

   //Events with a selected GEN Zee in Acceptance
   if (GenSelectedInAcceptance((*zgenHandle), _selections)&&zdaughters.size()!=0){
   
      genPtZ_Acc->Fill((*zgenHandle)[0].pt(), weight);
      genEtaZ_Acc->Fill((*zgenHandle)[0].eta(), weight);
      genMassZ_Acc->Fill((*zgenHandle)[0].mass(), weight);
      
      double leadelpt, leadeleta, secondelpt, secondeleta;
      leadelpt  = dau0->pt(); 
      secondelpt  = dau1->pt();
      leadeleta = dau0->eta();
      secondeleta = dau1->eta();
      
      if(secondelpt>leadelpt)throw cms::Exception("PATAnalysis:GenElectron_WrongElectronOrder") << "ERROR! Z electrons are in wrong order!";
      
      genLeadElPt_Acc->Fill(leadelpt, weight);
      genLeadElEta_Acc->Fill(leadeleta, weight);
      genSecElPt_Acc->Fill(secondelpt, weight);
      genSecElEta_Acc->Fill(secondeleta, weight);  
      
      GenJetCounter_Acc->Fill(genjets.size());
      
      GenIsoJetCounter_Acc->Fill(isogenjets.size());
      
      for(unsigned int i = 0; i < genjets.size(); i++){
      GenJetPt_Acc->Fill(genjets[i]->pt());
      JetMinDeltaRZDau_GEN->Fill(MinDeltaRZDau<pat::Electron>(zdaughters,*genjets[i]));
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
      _Eldir->cd();
      addHistosVsMulti(isogenjets.size(), "genEl1PtExcl", " generated lead electron p_{T} spectrum", 200, 0., 200., genEl1PtVsExclMulti);
      addHistosVsMulti(isogenjets.size(), "genEl1EtaExcl", " generated lead electron #eta spectrum", 100, -5., 5., genEl1EtaVsExclMulti);
      addHistosVsMulti(isogenjets.size(), "genEl2PtExcl", " generated sec electron p_{T} spectrum", 200, 0., 200., genEl2PtVsExclMulti);
      addHistosVsMulti(isogenjets.size(), "genEl2EtaExcl", " generated sec electron #eta spectrum", 100, -5., 5., genEl2EtaVsExclMulti);    
      addHistosVsMulti(isogenjets.size(), "genEl1PtIncl", " generated lead electron p_{T} spectrum", 200, 0., 200., genEl1PtVsInclMulti);
      addHistosVsMulti(isogenjets.size(), "genEl1EtaIncl", " generated lead electron #eta spectrum", 100, -5., 5., genEl1EtaVsInclMulti);
      addHistosVsMulti(isogenjets.size(), "genEl2PtIncl", " generated sec electron p_{T} spectrum", 200, 0., 200., genEl2PtVsInclMulti);
      addHistosVsMulti(isogenjets.size(), "genEl2EtaIncl", " generated sec electron #eta spectrum", 100, -5., 5., genEl2EtaVsInclMulti);

        for (unsigned int i = 0; i < isogenjets.size(); ++i){
          genJetPtVsInclMulti[i+1]->Fill(isogenjets[i]->pt(), weight);
          genJetEtaVsInclMulti[i+1]->Fill(isogenjets[i]->eta(), weight);
        }
      

      //fill inclusive quantities
      for (unsigned int i = 0; i < isogenjets.size()+1; ++i){
        genZPtVsInclMulti[i]->Fill((*zgenHandle)[0].pt(), weight);
        genZEtaVsInclMulti[i]->Fill((*zgenHandle)[0].eta(), weight);
        genEl1PtVsInclMulti[i]->Fill(leadelpt, weight);
        genEl2PtVsInclMulti[i]->Fill(secondelpt, weight);
        genEl1EtaVsInclMulti[i]->Fill(leadeleta, weight);
        genEl2EtaVsInclMulti[i]->Fill(secondeleta, weight);
      }

      //fill exclusive quantities
      genZPtVsExclMulti[isogenjets.size()]->Fill((*zgenHandle)[0].pt(), weight);
      genZEtaVsExclMulti[isogenjets.size()]->Fill((*zgenHandle)[0].eta(), weight);
      genEl1PtVsExclMulti[isogenjets.size()]->Fill(leadelpt, weight);
      genEl1EtaVsExclMulti[isogenjets.size()]->Fill(leadeleta, weight);
      genEl2PtVsExclMulti[isogenjets.size()]->Fill(secondelpt, weight);
      genEl2EtaVsExclMulti[isogenjets.size()]->Fill(secondeleta, weight);

   }
   
   }
   
}

void GenElectron::finalize(){
    
   _histovector.insert(_histovector.end(), genJetPtVsInclMulti.begin(), genJetPtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genJetEtaVsInclMulti.begin(), genJetEtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genZPtVsInclMulti.begin(), genZPtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genZEtaVsInclMulti.begin(), genZEtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genEl1PtVsInclMulti.begin(), genEl1PtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genEl1EtaVsInclMulti.begin(), genEl1EtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genEl2PtVsInclMulti.begin(), genEl2PtVsInclMulti.end());
   _histovector.insert(_histovector.end(), genEl2EtaVsInclMulti.begin(), genEl2EtaVsInclMulti.end());

   _histovector.insert(_histovector.end(), genZPtVsExclMulti.begin(), genZPtVsExclMulti.end());
   _histovector.insert(_histovector.end(), genZEtaVsExclMulti.begin(), genZEtaVsExclMulti.end());

   _histovector.insert(_histovector.end(), genEl1PtVsExclMulti.begin(), genEl1PtVsExclMulti.end());
   _histovector.insert(_histovector.end(), genEl1EtaVsExclMulti.begin(), genEl1EtaVsExclMulti.end());

   _histovector.insert(_histovector.end(), genEl2PtVsExclMulti.begin(), genEl2PtVsExclMulti.end());
   _histovector.insert(_histovector.end(), genEl2EtaVsExclMulti.begin(), genEl2EtaVsExclMulti.end());
   
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
