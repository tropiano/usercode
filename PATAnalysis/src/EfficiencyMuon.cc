#include "Firenze/PATAnalysis/include/EfficiencyMuon.h"
#include "Firenze/PATAnalysis/include/Utilities.h"

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "CommonTools/Utils/interface/PtComparator.h"

#include <iostream>

#include "TParameter.h"
#include "TMethodCall.h"

using namespace std;

//ClassImp(EfficiencyMuon)

EfficiencyMuon::EfficiencyMuon() : 
_initiated(false), _outputfile(0), _dirname("EfficiencyMuon"), _isocut(0.3), _ptjetmin(30.), _etajetmax(3.), _nbin(10), _xmin(-0.5), _xmax(9.5),
_jetmultiResponse(0) /*_isocut(isocut)*/{}

void EfficiencyMuon::begin(TFile* out, const edm::ParameterSet& iConfig){
  _outputfile = out;
  std::string dirname = iConfig.getParameter<std::string>("Name");
  _isocut    = iConfig.getParameter<double>("IsoCut");
  _ptjetmin  = iConfig.getParameter<double>("MinPtJet");
  _etajetmax = iConfig.getParameter<double>("MaxEtaJet");
  _norm      = iConfig.getParameter<double>("ScaleFactor");
  _nbin      = iConfig.getParameter<uint32_t>("NBin");
  _xmin      = iConfig.getParameter<double>("XMin");
  _xmax      = iConfig.getParameter<double>("XMax");
  _zcandSrc  = iConfig.getParameter<std::string>("Zsource");
  _jetSrc  = iConfig.getParameter<std::string>("JetSource");
  _oppositeCharge = iConfig.getParameter<bool>("OppositeCharge");
  _requireGenInAcceptance = iConfig.getParameter<bool>("RequireGenInAcceptance");
  _vsGenMulti = iConfig.getParameter<bool>("VsGenMulti");
  _trainUnfolding = iConfig.getParameter<bool>("TrainUnfolding");
  

  _outputfile->cd();
  TDirectory* dir = _outputfile->mkdir(dirname.c_str(), dirname.c_str());
  dir->cd();
  _acc_den                      = new TH1D("acceptance_denominator", "acceptance_denominator", _nbin, _xmin, _xmax); 
  _acc_num                      = new TH1D("acceptance_numerator", "acceptance_numerator", _nbin, _xmin, _xmax); 
  generalefficiency_numerator   = new TH1D("efficiency_numerator", "efficiency Vs #jets", _nbin, _xmin, _xmax);
  _vectorHistos.push_back(generalefficiency_numerator);
  generalefficiency_denominator = new TH1D("efficiency_denominator", "efficiency Vs #jets denominator", _nbin, _xmin, _xmax);
  _vectorHistos.push_back(generalefficiency_denominator);
  TwoMuons                      = new TH1D("TwoMuons", "TwoMuons", _nbin, _xmin, _xmax);
  _vectorHistos.push_back(TwoMuons);
  TM_MuTriggered                = new TH1D("TM_MuT", "TM_MuT", _nbin, _xmin, _xmax);
  _vectorHistos.push_back(TM_MuTriggered);
  TM_MuT_OppositeCharge         = new TH1D("TM_MuT_OppositeCharge", "TM_MuT_OppositeCharge", _nbin, _xmin, _xmax);
  _vectorHistos.push_back(TM_MuT_OppositeCharge);
  TM_MuT_OC_Mass                = new TH1D("TM_MuT_OC_Mass", "TM_MuT_OC_Mass", _nbin, _xmin, _xmax);
  _vectorHistos.push_back(TM_MuT_OC_Mass);
  TM_MuT_OC_M_QualityCuts       = new TH1D("TM_MuT_OC_M_QualityCuts", "TM_MuT_OC_M_QualityCuts", _nbin, _xmin, _xmax);
  _vectorHistos.push_back(TM_MuT_OC_M_QualityCuts);
  TM_MuT_OC_M_QC_DXY            = new TH1D("TM_MuT_OC_M_QC_DXY", "TM_MuT_OC_M_QC_DXY", _nbin, _xmin, _xmax);
  _vectorHistos.push_back(TM_MuT_OC_M_QC_DXY);
  TM_MuT_OC_M_QC_DXY_Iso        = new TH1D("TM_MuT_OC_M_QC_DXY_Iso", "TM_MuT_OC_M_QC_DXY_Iso", _nbin, _xmin, _xmax);
  _vectorHistos.push_back(TM_MuT_OC_M_QC_DXY_Iso);
  TM_MuJetTriggered             = new TH1D("TM_MuJetTriggered", "TM_MuJetTriggered", _nbin, _xmin, _xmax);
  _vectorHistos.push_back(TM_MuJetTriggered);
  TM_JetTriggered               = new TH1D("TM_JetTriggered", "TM_JetTriggered", _nbin, _xmin, _xmax);
  _vectorHistos.push_back(TM_JetTriggered);

 
  TwoMuonsVsMuPt                      = new TH1D("TwoMuonsVsMuPt", "TwoMuonsVsMuPt", 500, 0., 500.);
  _vectorHistos.push_back(TwoMuonsVsMuPt);
  TM_MuTriggeredVsMuPt                = new TH1D("TM_MuTVsMuPt", "TM_MuTVsMuPt", 500, 0., 500.);
  _vectorHistos.push_back(TM_MuTriggeredVsMuPt);
  TM_MuT_OppositeChargeVsMuPt         = new TH1D("TM_MuT_OppositeChargeVsMuPt", "TM_MuT_OppositeChargeVsMuPt", 500, 0., 500.);
  _vectorHistos.push_back(TM_MuT_OppositeChargeVsMuPt);
  TM_MuT_OC_MassVsMuPt                = new TH1D("TM_MuT_OC_MassVsMuPt", "TM_MuT_OC_MassVsMuPt", 500, 0., 500.);
  _vectorHistos.push_back(TM_MuT_OC_MassVsMuPt);
  TM_MuT_OC_M_QualityCutsVsMuPt       = new TH1D("TM_MuT_OC_M_QualityCutsVsMuPt", "TM_MuT_OC_M_QualityCutsVsMuPt", 500, 0., 500.);
  _vectorHistos.push_back(TM_MuT_OC_M_QualityCutsVsMuPt);
  TM_MuT_OC_M_QC_DXYVsMuPt            = new TH1D("TM_MuT_OC_M_QC_DXYVsMuPt", "TM_MuT_OC_M_QC_DXYVsMuPt", 500, 0., 500.);
  _vectorHistos.push_back(TM_MuT_OC_M_QC_DXYVsMuPt);
  TM_MuT_OC_M_QC_DXY_IsoVsMuPt        = new TH1D("TM_MuT_OC_M_QC_DXY_IsoVsMuPt", "TM_MuT_OC_M_QC_DXY_IsoVsMuPt", 500, 0., 500.);
  _vectorHistos.push_back(TM_MuT_OC_M_QC_DXY_IsoVsMuPt);
  TM_MuJetTriggeredVsMuPt             = new TH1D("TM_MuJetTriggeredVsMuPt", "TM_MuJetTriggeredVsMuPt", 500, 0., 500.);
  _vectorHistos.push_back(TM_MuJetTriggeredVsMuPt);
  TM_JetTriggeredVsMuPt               = new TH1D("TM_JetTriggeredVsMuPt", "TM_JetTriggeredVsMuPt", 500, 0., 500.);
  _vectorHistos.push_back(TM_JetTriggeredVsMuPt);
        
  TwoMuonsVsMuEta                      = new TH1D("TwoMuonsVsMuEta", "TwoMuonsVsMuEta", 300, -3, 3);
  _vectorHistos.push_back(TwoMuonsVsMuEta);
  TM_MuTriggeredVsMuEta                = new TH1D("TM_MuTVsMuEta", "TM_MuTVsMuEta", 300, -3, 3);
  _vectorHistos.push_back(TM_MuTriggeredVsMuEta);
  TM_MuT_OppositeChargeVsMuEta         = new TH1D("TM_MuT_OppositeChargeVsMuEta", "TM_MuT_OppositeChargeVsMuEta", 300, -3, 3);
  _vectorHistos.push_back(TM_MuT_OppositeChargeVsMuEta);
  TM_MuT_OC_MassVsMuEta                = new TH1D("TM_MuT_OC_MassVsMuEta", "TM_MuT_OC_MassVsMuEta", 300, -3, 3);
  _vectorHistos.push_back(TM_MuT_OC_MassVsMuEta);
  TM_MuT_OC_M_QualityCutsVsMuEta       = new TH1D("TM_MuT_OC_M_QualityCutsVsMuEta", "TM_MuT_OC_M_QualityCutsVsMuEta", 300, -3, 3);
  _vectorHistos.push_back(TM_MuT_OC_M_QualityCutsVsMuEta);
  TM_MuT_OC_M_QC_DXYVsMuEta            = new TH1D("TM_MuT_OC_M_QC_DXYVsMuEta", "TM_MuT_OC_M_QC_DXYVsMuEta", 300, -3, 3);
  _vectorHistos.push_back(TM_MuT_OC_M_QC_DXYVsMuEta);
  TM_MuT_OC_M_QC_DXY_IsoVsMuEta        = new TH1D("TM_MuT_OC_M_QC_DXY_IsoVsMuEta", "TM_MuT_OC_M_QC_DXY_IsoVsMuEta", 300, -3, 3);
  _vectorHistos.push_back(TM_MuT_OC_M_QC_DXY_IsoVsMuEta);
  TM_MuJetTriggeredVsMuEta             = new TH1D("TM_MuJetTriggeredVsMuEta", "TM_MuJetTriggeredVsMuEta", 300, -3, 3);
  _vectorHistos.push_back(TM_MuJetTriggeredVsMuEta);
  TM_JetTriggeredVsMuEta               = new TH1D("TM_JetTriggeredVsMuEta", "TM_JetTriggeredVsMuEta", 300, -3, 3);
  _vectorHistos.push_back(TM_JetTriggeredVsMuEta);

  TM_MuT_OC_MassVsMass                = new TH1D("TM_MuT_OC_MassVsMass", "TM_MuT_OC_MassVsMass", 100, 50., 150.);
  _vectorHistos.push_back(TM_MuT_OC_MassVsMass);
  TM_MuT_OC_M_QualityCutsVsMass       = new TH1D("TM_MuT_OC_M_QualityCutsVsMass", "TM_MuT_OC_M_QualityCutsVsMass", 100, 50., 150.);
  _vectorHistos.push_back(TM_MuT_OC_M_QualityCutsVsMass);
  TM_MuT_OC_M_QC_DXYVsMass            = new TH1D("TM_MuT_OC_M_QC_DXYVsMass", "TM_MuT_OC_M_QC_DXYVsMass", 100, 50., 150.);
  _vectorHistos.push_back(TM_MuT_OC_M_QC_DXYVsMass);
  TM_MuT_OC_M_QC_DXY_IsoVsMass        = new TH1D("TM_MuT_OC_M_QC_DXY_IsoVsMass", "TM_MuT_OC_M_QC_DXY_IsoVsMass", 100, 50., 150.);
  _vectorHistos.push_back(TM_MuT_OC_M_QC_DXY_IsoVsMass);

  std::vector<TH1*>::const_iterator ibeg = _vectorHistos.begin(); 
  std::vector<TH1*>::const_iterator iend = _vectorHistos.end(); 
  for (std::vector<TH1*>::const_iterator i = ibeg; i != iend; ++i){
    (*i)->Sumw2();
  } 

  _jetmultiResponse = new RooUnfoldResponse(10, -0.5, 9.5, "ResponseMatrix", "ResponseMatrix");

  //debug
  /*_jetmultiResponse->IsA()->Dump();
  TMethodCall callEnv;
  callEnv.InitWithPrototype(_jetmultiResponse->IsA(), "Merge", "TCollection*");
  if (callEnv.IsValid()) {
    std::cout << "callEnv IsValid()" << std::endl; 
  }*/

  dir->cd("-");

  std::vector<bool (*)(const reco::Candidate&)> tag_cuts;
  tag_cuts.push_back(singleMu_Tag);
  //tag_cuts.push_back(singleMu_isNegative); //TEST 

  std::vector<bool (*)(const reco::Candidate&)> probe_cuts;

  std::vector<bool (*)(const reco::Candidate&)> passprobe_cuts;

/*
  //probe_cuts.push_back(singleMu_isPositive);//TEST
  passprobe_cuts.push_back(singleMu_QualityCuts);
  //passprobe_cuts.push_back(singleMu_isPositive);//TEST
  _tp_TM_MuT_OC_M_QC         = new TagAndProbe(dir, string("TM_MuT_OC_M_QC"), nbin, xmin, xmax, tag_cuts, probe_cuts, passprobe_cuts, _performfits);

  probe_cuts.clear();
  passprobe_cuts.clear();
  probe_cuts.push_back(singleMu_QualityCuts);
 // probe_cuts.push_back(singleMu_isPositive);//TEST
  passprobe_cuts.push_back(singleMu_QualityCuts);
  passprobe_cuts.push_back(singleMu_DXY);
  //passprobe_cuts.push_back(singleMu_isPositive);//TEST
  _tp_TM_MuT_OC_M_QC_DXY     = new TagAndProbe(dir, string("TM_MuT_OC_M_QC_DXY"), nbin, xmin, xmax, tag_cuts, probe_cuts, passprobe_cuts, _performfits);
*/  
  probe_cuts.clear();
  passprobe_cuts.clear();
  probe_cuts.push_back(singleMu_QualityCuts);
  probe_cuts.push_back(singleMu_DXY);
  passprobe_cuts.push_back(singleMu_QualityCuts);
  passprobe_cuts.push_back(singleMu_DXY);
  passprobe_cuts.push_back(singleMu_Isolation);
  _tp_TM_MuT_OC_M_QC_DXY_Iso = new TagAndProbeFiller(dir, string("TM_MuT_OC_M_QC_DXY_Iso"), _nbin, _xmin, _xmax, tag_cuts, probe_cuts, passprobe_cuts);
  
}

EfficiencyMuon::~EfficiencyMuon(){
}


void EfficiencyMuon::process(const fwlite::Event& iEvent){

   double w = _norm;

   fwlite::Handle<std::vector<pat::Muon> > muonHandle;
   muonHandle.getByLabel(iEvent, "selectedMuons");

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zHandle;
   zHandle.getByLabel(iEvent, _zcandSrc.c_str());

   fwlite::Handle<std::vector<pat::Jet> > jetHandle;
   jetHandle.getByLabel(iEvent, _jetSrc.c_str());

   fwlite::Handle<std::vector<reco::GenJet> > genJetHandle;
   if (_vsGenMulti){
      genJetHandle.getByLabel(iEvent, "selectedGenJets");    
   }   

   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zGenHandle;
   if (_requireGenInAcceptance || _trainUnfolding){
      zGenHandle.getByLabel(iEvent, "zmumugenfull");
   }

   //get Z candidate
   std::vector<const pat::Muon*> muonsfromZ;
   const pat::Muon* dau0 = 0;
   const pat::Muon* dau1 = 0;
   //FEXME!!!!!!!!!!! we need to select the Z candidate
   if (zHandle->size() > 0){
    //take the two muons with all the needed castings
    dau0 = dynamic_cast<const pat::Muon*>((*zHandle)[0].daughter(0));
    dau1 = dynamic_cast<const pat::Muon*>((*zHandle)[0].daughter(1));
    if (!dau0) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> ((*zHandle)[0].daughter(0));
     if (scc && scc->hasMasterClone()){
       dau0 = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
     }
    }
    if (!dau1) {
     //maybe a shallow clone
     const reco::ShallowCloneCandidate* scc = dynamic_cast<const reco::ShallowCloneCandidate*> ((*zHandle)[0].daughter(1));
     if (scc && scc->hasMasterClone()){
       dau1 = dynamic_cast<const pat::Muon*>(scc->masterClone().get());
     }
    }
    assert(dau0 && dau1);
    muonsfromZ.push_back(dau0);
    muonsfromZ.push_back(dau1);
   }

   std::sort(muonsfromZ.begin(), muonsfromZ.end(), sortByPt<pat::Muon>); 
  
   std::vector<const pat::Muon*> allHardMuons;
   for (unsigned int j = 0; j < muonHandle->size(); ++j){
     if ((*muonHandle)[j].pt()>10.) allHardMuons.push_back(&(*muonHandle)[j]);
   }

   bool recselected = RecSelectedMuonWithTrigger(*zHandle, *triggerHandle, _isocut);

   //if this is a good Z event remove Z candidates from jet collection
   std::vector<pat::Jet> cleanedJets;
   //CleanJets(*jetHandle, muonsfromZ, cleanedJets, 0.5);
   CleanJets(*jetHandle, allHardMuons, cleanedJets, 0.5);
   std::vector<const pat::Jet*> jets = GetJets<pat::Jet>(cleanedJets, _ptjetmin, _etajetmax); 

   double recosize = jets.size();
   double gensize  = 0;
   if (_vsGenMulti || _trainUnfolding){
      std::vector<const reco::GenJet*> genjets = GetJets<reco::GenJet>(*genJetHandle, _ptjetmin, _etajetmax);
      gensize = genjets.size();
   }
   //use reco size unless asked to use gen size.
   //for the tag and probe use recosize anyway
   double size = _vsGenMulti ? gensize : recosize;       

   bool denominatorCondition = _requireGenInAcceptance ? GenSelectedInAcceptanceMuon(*zGenHandle) : true;

   bool generatedCondition   = _requireGenInAcceptance ? GenSelectedMuon(*zGenHandle): true;
   if (generatedCondition) _acc_den->Fill(size, w);

   if (denominatorCondition) {
      generalefficiency_denominator->Fill(size, w);
      _acc_num->Fill(size, w); 
   }    

   if (!denominatorCondition) return;


   ///unfolding training
   if (_trainUnfolding){
      if (recselected)
        _jetmultiResponse->Fill(recosize, gensize, w);
      else _jetmultiResponse->Miss(gensize, w);  
   }

   //global efficiency 
   if (recselected) generalefficiency_numerator->Fill(size, w);

   //now the efficiencies step by step
  
   //first store vectors of pointers to the muons
   std::vector<const pat::Muon*> allmuons;
   std::vector<pat::Muon>::const_iterator imu;
   std::vector<pat::Muon>::const_iterator imubeg = muonHandle->begin(); 
   std::vector<pat::Muon>::const_iterator imuend = muonHandle->end(); 
   for (imu = imubeg; imu != imuend; ++imu){
    allmuons.push_back(&*imu);
   }

  
   bool isMuTriggered  = isMuonTriggered(*triggerHandle, allmuons);
   bool isJTriggered = isJetTriggered(*triggerHandle);

   bool recselectedTwoMuons = RecSelected_GlobalMuons(allmuons,2).first;
   bool recselectedTM_MuTriggered = recselectedTwoMuons && isMuTriggered;
   //bool recselectedTM_MuT_OppositeCharge = RecSelected_OppositeCharge(allmuons).first && recselectedTM_MuTriggered;
   bool recselectedTM_MuT_Charge = _oppositeCharge ? RecSelected_Charge(allmuons, -1).first && recselectedTM_MuTriggered : 
                                                     RecSelected_Charge(allmuons,  1).first && recselectedTM_MuTriggered;
   bool recselectedTM_MuT_OC_Mass = zHandle->size()>0 && RecSelected_MuonMass(*zHandle) && RecSelected_GlobalMuons(muonsfromZ, 2).first && isMuonTriggered(*triggerHandle, muonsfromZ); //&& RecSelected_OppositeCharge(muonsfromZ).first; these should already have the desired charge assignment
   bool recselectedTM_MuT_OC_M_QualityCuts = RecSelected_QualityCuts(muonsfromZ, 2).first && recselectedTM_MuT_OC_Mass;
   bool recselectedTM_MuT_OC_M_QC_DXY = RecSelected_DXY(muonsfromZ, 2).first && recselectedTM_MuT_OC_M_QualityCuts;
   bool recselectedTM_MuT_OC_M_QC_DXY_Iso = (muonsfromZ.size() > 0) ? singleMu_Isolation(*muonsfromZ.front()) && recselectedTM_MuT_OC_M_QC_DXY : false;
                                            //RecSelected_Isolation(muonsfromZ, _isocut, 1).first && recselectedTM_MuT_OC_M_QC_DXY;

   bool recselectedTM_MuJetTriggered = recselectedTwoMuons && isMuTriggered && isJTriggered;
   bool recselectedTM_JetTriggered = recselectedTwoMuons && isJTriggered;

   if (recselectedTwoMuons) {
      TwoMuons->Fill(size, w);
      TwoMuonsVsMuPt->Fill(allmuons.front()->pt(), w);
      TwoMuonsVsMuEta->Fill(allmuons.front()->eta(), w);
   }   
   if (recselectedTM_MuTriggered) {
      TM_MuTriggered->Fill(size, w);
      TM_MuTriggeredVsMuPt->Fill(allmuons.front()->pt(), w);
      TM_MuTriggeredVsMuEta->Fill(allmuons.front()->eta(), w);
   }   
   if (recselectedTM_MuT_Charge) {
      TM_MuT_OppositeCharge->Fill(size, w);
      TM_MuT_OppositeChargeVsMuPt->Fill(allmuons.front()->pt(), w);
      TM_MuT_OppositeChargeVsMuEta->Fill(allmuons.front()->eta(), w);
   }   
   if (recselectedTM_MuT_OC_Mass) {
      TM_MuT_OC_Mass->Fill(size, w);
      TM_MuT_OC_MassVsMuPt->Fill(muonsfromZ.front()->pt(), w);
      TM_MuT_OC_MassVsMuEta->Fill(muonsfromZ.front()->eta(), w);
      TM_MuT_OC_MassVsMass->Fill((*zHandle)[0].mass(), w);
   }   
   if (recselectedTM_MuT_OC_M_QualityCuts) {
      TM_MuT_OC_M_QualityCuts->Fill(size, w);
      TM_MuT_OC_M_QualityCutsVsMuPt->Fill(muonsfromZ.front()->pt(), w);  
      TM_MuT_OC_M_QualityCutsVsMuEta->Fill(muonsfromZ.front()->eta(), w);
      TM_MuT_OC_M_QualityCutsVsMass->Fill((*zHandle)[0].mass(), w);
   }   
   if (recselectedTM_MuT_OC_M_QC_DXY) {
      TM_MuT_OC_M_QC_DXY->Fill(size, w);
      TM_MuT_OC_M_QC_DXYVsMuPt->Fill(muonsfromZ.front()->pt(), w);
      TM_MuT_OC_M_QC_DXYVsMuEta->Fill(muonsfromZ.front()->eta(), w);
      TM_MuT_OC_M_QC_DXYVsMass->Fill((*zHandle)[0].mass(), w);

   }   
   if (recselectedTM_MuT_OC_M_QC_DXY_Iso) {
      TM_MuT_OC_M_QC_DXY_Iso->Fill(size, w);
      TM_MuT_OC_M_QC_DXY_IsoVsMuPt->Fill(muonsfromZ.front()->pt(), w);
      TM_MuT_OC_M_QC_DXY_IsoVsMuEta->Fill(muonsfromZ.front()->eta(), w);
      TM_MuT_OC_M_QC_DXY_IsoVsMass->Fill((*zHandle)[0].mass(), w);
   }   
   
   if (recselectedTM_JetTriggered) {
      TM_JetTriggered->Fill(size, w);
      TM_JetTriggeredVsMuPt->Fill(allmuons.front()->pt(), w);
      TM_JetTriggeredVsMuEta->Fill(allmuons.front()->eta(), w);
   }   
   if (recselectedTM_MuJetTriggered) {
      TM_MuJetTriggered->Fill(size, w);
      TM_MuJetTriggeredVsMuPt->Fill(allmuons.front()->pt(), w);
      TM_MuJetTriggeredVsMuEta->Fill(allmuons.front()->eta(), w);
   }   
/*
   //tag and probe 
   if (recselectedTM_MuT_OC_Mass) {
      //FIXME!!!!!!!!!!! we need to select the Z candidate
      if (zHandle->size() == 1){
        //_tp_TM_MuT_OC_M_QC->fill(_zmuons, _zs, x, w);
        //_tp_TM_MuT_OC_M_QC_DXY->fill(_zmuons, _zs, x, w);
        _tp_TM_MuT_OC_M_QC_DXY_Iso->fill((*zHandle)[0], recosize, w);
      }  
   } 
*/
   //_initiated= false;

}

void EfficiencyMuon::finalize()
{
  _tp_TM_MuT_OC_M_QC_DXY_Iso->finalize();
  std::cout << "response nbins measured " << _jetmultiResponse->GetNbinsMeasured() << std::endl;
  std::cout << "response nbins truth " << _jetmultiResponse->GetNbinsTruth() << std::endl;
  if (_trainUnfolding) _jetmultiResponse->Write();
  _outputfile->Write();
}

std::string EfficiencyMuon::getCutName(EfficiencyMuon::cuts i) const{
  switch (i){
    case TwoMuonsCut : return "TwoMuons";
    case TM_OppositeChargeCut: return "TM_OppositeCharge";
    case TM_OC_MassCut: return "TM_OC_Mass";
    case TM_OC_M_QualityCutsCut: return "TM_OC_M_QualityCuts";
    case TM_OC_M_QC_DXYCut: return "TM_OC_M_QC_DXY";
    case TM_OC_M_QC_DXY_IsoCut: return "TM_OC_M_QC_DXY_Iso";
    default: return "UnknownCut";
  }  
}
