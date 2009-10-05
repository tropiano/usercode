#include "Firenze/PATAnalysis/include/EfficiencyMuon.h"
#include "Firenze/PATAnalysis/include/Utilities.h"

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include <iostream>

#include "TParameter.h"

using namespace std;

//ClassImp(EfficiencyMuon)

EfficiencyMuon::EfficiencyMuon(TFile* outputfile, TList* fInput) : 
_initiated(false), _outputfile(outputfile), _dirname("EfficiencyMuon"), _isocut(0.3), _ptjetmin(30.), _etajetmax(3.), _nbin(10), _xmin(-0.5), _xmax(9.5)/*, _performfits(performfits), _isocut(isocut)*/{
  TIter next(fInput);
  bool factorSet = false;
  bool requireGenSet = false; 

  while (TObject* obj = next()){
    const TParameter<double>* parDouble = dynamic_cast<const TParameter<double>* >(obj);
    const TParameter<int>* parInt = dynamic_cast<const TParameter<int>* >(obj);
    const TNamed* parNamed = dynamic_cast<const TNamed* >(obj);
    if (parDouble){
      if (!strcmp(parDouble->GetName(), "IsoCut")){
        _isocut = parDouble->GetVal();
        cout << "set isolation cut to: " << _isocut << endl;
      } else if (!strcmp(parDouble->GetName(), "MinPtJet")){
        _ptjetmin = parDouble->GetVal();
        cout << "set minimum pt for jets to: " << _ptjetmin << endl;
      } else if (!strcmp(parDouble->GetName(), "MaxEtaJet")) {
        _etajetmax = parDouble->GetVal();
        cout << "set maximim eta for jets to: " << _etajetmax << endl;
      } else if (!strcmp(parDouble->GetName(), "ScaleFactor")) {
        _norm = parDouble->GetVal();
        cout << "set scale factor to: " << _norm << endl;
        factorSet = true;
      } else if (!strcmp(parDouble->GetName(), "EfficiencyMuonXMin")) {
        _xmin = parDouble->GetVal();
        cout << "set xmin to: " << _xmin << endl;
      } else if (!strcmp(parDouble->GetName(), "EfficiencyMuonXMax")) {
        _xmax = parDouble->GetVal();
         cout << "set xmax to: " << _xmax << endl;
      } 
    } else if (parInt){
      if (!strcmp(parInt->GetName(), "EfficiencyMuonNBin")) { 
        _nbin = parInt->GetVal();
        cout << "set nbin to: " << _nbin << endl;
      }  
    } else if (parNamed) {
      if (!strcmp(parNamed->GetName(), "RequireGenInAccepance")) {
        _requireGenInAcceptance = as<bool>(string(parNamed->GetTitle()));
        cout << "set _requireGenInAcceptance to " << _requireGenInAcceptance << endl;
        requireGenSet = true;
      }
    }
  } 

  if (!factorSet){
    cout << "You must set the scale factor " << endl;
    assert (factorSet);
  }

  if (!requireGenSet) {
    cout << "You must set if you require gen in aceptance or not" << endl;
    assert (requireGenSet);
  }


  _outputfile->cd();
  TDirectory* dir = _outputfile->mkdir(_dirname.c_str(), _dirname.c_str());
  dir->cd();
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
/*
  std::vector<bool (*)(const PhysVarTreeMuon&)> tag_cuts;
  tag_cuts.push_back(singleMu_Tag);
  //tag_cuts.push_back(singleMu_isNegative); //TEST 

  std::vector<bool (*)(const PhysVarTreeMuon&)> probe_cuts;

  std::vector<bool (*)(const PhysVarTreeMuon&)> passprobe_cuts;

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
  
  probe_cuts.clear();
  passprobe_cuts.clear();
  probe_cuts.push_back(singleMu_QualityCuts);
  probe_cuts.push_back(singleMu_DXY);
  //probe_cuts.push_back(singleMu_isPositive); //TEST
  passprobe_cuts.push_back(singleMu_QualityCuts);
  passprobe_cuts.push_back(singleMu_DXY);
  passprobe_cuts.push_back(singleMu_Isolation);
  //passprobe_cuts.push_back(singleMu_isPositive);//TEST
  _tp_TM_MuT_OC_M_QC_DXY_Iso = new TagAndProbe(dir, string("TM_MuT_OC_M_QC_DXY_Iso"), nbin, xmin, xmax, tag_cuts, probe_cuts, passprobe_cuts, _performfits);
*/  
}

EfficiencyMuon::~EfficiencyMuon(){
}


void EfficiencyMuon::process(const fwlite::Event& iEvent){

   double w = _norm;

   fwlite::Handle<std::vector<pat::Muon> > muonHandle;
   muonHandle.getByLabel(iEvent, "selectedMuons");

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zHandle;
   zHandle.getByLabel(iEvent, "zmumurec");

   fwlite::Handle<std::vector<pat::Jet> > jetHandle;
   jetHandle.getByLabel(iEvent, "selectedJets");

   fwlite::Handle<pat::TriggerEvent> triggerHandle;
   triggerHandle.getByLabel(iEvent, "patTriggerEvent");

   fwlite::Handle<std::vector<reco::CompositeCandidate> > zGenHandle;
   if (_requireGenInAcceptance){
      zGenHandle.getByLabel(iEvent, "zmumugenfull");
   }

   std::vector<const pat::Jet*> jets = GetJets<pat::Jet>(*jetHandle, _ptjetmin, _etajetmax);  
   double size = jets.size();

   bool denominatorCondition = _requireGenInAcceptance ? GenSelectedInAcceptance(*zGenHandle) : true;

   if (denominatorCondition) generalefficiency_denominator->Fill(size, w);     

   if (!denominatorCondition) return;

   bool recselected = zHandle->size()==1 && RecSelectedWithTrigger(*zHandle, *triggerHandle, _isocut);

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

   std::vector<const pat::Muon*> muonsfromZ;
   const pat::Muon* dau0 = 0;
   const pat::Muon* dau1 = 0;
   //FEXME!!!!!!!!!!! we need to select the Z candidate
   if (zHandle->size() == 1){
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
    //assert(dau0 && dau1);
    muonsfromZ.push_back(dau0);
    muonsfromZ.push_back(dau1);
   }
   
   bool isMuTriggered  = isMuonTriggered(*triggerHandle);
   bool isJTriggered = isJetTriggered(*triggerHandle);

   bool recselectedTwoMuons = RecSelected_GlobalMuons(allmuons,2).first;
   bool recselectedTM_MuTriggered = recselectedTwoMuons && isMuTriggered;
   bool recselectedTM_MuT_OppositeCharge = RecSelected_OppositeCharge(allmuons).first && recselectedTM_MuTriggered;
   bool recselectedTM_MuT_OC_Mass = zHandle->size()==1 && RecSelected_Mass(*zHandle) && RecSelected_GlobalMuons(muonsfromZ, 2).first && isMuTriggered && RecSelected_OppositeCharge(muonsfromZ).first; 
   bool recselectedTM_MuT_OC_M_QualityCuts = RecSelected_QualityCuts(muonsfromZ, 2).first && recselectedTM_MuT_OC_Mass;
   bool recselectedTM_MuT_OC_M_QC_DXY = RecSelected_DXY(muonsfromZ, 2).first && recselectedTM_MuT_OC_M_QualityCuts;
   bool recselectedTM_MuT_OC_M_QC_DXY_Iso = RecSelected_Isolation(muonsfromZ, _isocut, 2).first && recselectedTM_MuT_OC_M_QC_DXY;

   bool recselectedTM_MuJetTriggered = recselectedTwoMuons && isMuTriggered && isJTriggered;
   bool recselectedTM_JetTriggered = recselectedTwoMuons && isJTriggered;

   if (recselectedTwoMuons) TwoMuons->Fill(size, w);
   if (recselectedTM_MuTriggered) TM_MuTriggered->Fill(size, w);
   if (recselectedTM_MuT_OppositeCharge) TM_MuT_OppositeCharge->Fill(size, w);
   if (recselectedTM_MuT_OC_Mass) TM_MuT_OC_Mass->Fill(size, w);
   if (recselectedTM_MuT_OC_M_QualityCuts) TM_MuT_OC_M_QualityCuts->Fill(size, w);
   if (recselectedTM_MuT_OC_M_QC_DXY) TM_MuT_OC_M_QC_DXY->Fill(size, w);
   if (recselectedTM_MuT_OC_M_QC_DXY_Iso) TM_MuT_OC_M_QC_DXY_Iso->Fill(size, w);
   
   if (recselectedTM_JetTriggered) TM_JetTriggered->Fill(size, w);
   if (recselectedTM_MuJetTriggered) TM_MuJetTriggered->Fill(size, w);
/*
   //tag and probe 
   if (recselectedTM_MuT_OC_Mass) {
      _tp_TM_MuT_OC_M_QC->fill(_zmuons, _zs, x, w);
      _tp_TM_MuT_OC_M_QC_DXY->fill(_zmuons, _zs, x, w);
      _tp_TM_MuT_OC_M_QC_DXY_Iso->fill(_zmuons, _zs, x, w);
   } 
*/
   //_initiated= false;

}

void EfficiencyMuon::finalize()
{
/*
   generalefficiency_denominator->Scale(norm_factor);
   generalefficiency_numerator->Scale(norm_factor);
   TwoMuons->Scale(norm_factor);
   TM_MuTriggered->Scale(norm_factor);
   TM_MuT_OppositeCharge->Scale(norm_factor);
   TM_MuT_OC_Mass->Scale(norm_factor);
   TM_MuT_OC_M_QualityCuts->Scale(norm_factor);
   TM_MuT_OC_M_QC_DXY->Scale(norm_factor);
   TM_MuT_OC_M_QC_DXY_Iso->Scale(norm_factor);
   TM_JetTriggered->Scale(norm_factor);
   TM_MuJetTriggered->Scale(norm_factor);
   
   //cout << "Looking for directory name " << _dirname << endl;

   for (std::vector<TFile*>::const_iterator i = plotstoadd.begin(); i != plotstoadd.end(); ++i){
      //(*i)->ls();
      (*i)->cd(_dirname.c_str());
      TH1* other_generalefficiency_denominator = (TH1*) (*i)->Get((_dirname + "/"+ string(generalefficiency_denominator->GetName())).c_str());
      TH1* other_generalefficiency_numerator = (TH1*) (*i)->Get((_dirname + "/"+ string(generalefficiency_numerator->GetName())).c_str());
      
      TH1* other_TwoMuons = (TH1*) (*i)->Get((_dirname + "/"+ string(TwoMuons->GetName())).c_str());
      TH1* other_TM_MuTriggered = (TH1*) (*i)->Get((_dirname + "/"+ string(TM_MuTriggered->GetName())).c_str());
      TH1* other_TM_MuT_OppositeCharge = (TH1*) (*i)->Get((_dirname + "/"+ string(TM_MuT_OppositeCharge->GetName())).c_str());
      TH1* other_TM_MuT_OC_Mass = (TH1*) (*i)->Get((_dirname + "/"+ string(TM_MuT_OC_Mass->GetName())).c_str());
      TH1* other_TM_MuT_OC_M_QualityCuts = (TH1*) (*i)->Get((_dirname + "/"+ string(TM_MuT_OC_M_QualityCuts->GetName())).c_str());
      TH1* other_TM_MuT_OC_M_QC_DXY = (TH1*) (*i)->Get((_dirname + "/"+ string(TM_MuT_OC_M_QC_DXY->GetName())).c_str());
      TH1* other_TM_MuT_OC_M_QC_DXY_Iso = (TH1*) (*i)->Get((_dirname + "/"+ string(TM_MuT_OC_M_QC_DXY_Iso->GetName())).c_str());
      TH1* other_TM_JetTriggered = (TH1*) (*i)->Get((_dirname + "/"+ string(TM_JetTriggered->GetName())).c_str());
      TH1* other_TM_MuJetTriggered = (TH1*) (*i)->Get((_dirname + "/"+ string(TM_MuJetTriggered->GetName())).c_str());

      generalefficiency_denominator->Add(other_generalefficiency_denominator);
      generalefficiency_numerator->Add(other_generalefficiency_numerator);
      TwoMuons->Add(other_TwoMuons);
      TM_MuTriggered->Add(other_TM_MuTriggered);
      TM_MuT_OppositeCharge->Add(other_TM_MuT_OppositeCharge);
      TM_MuT_OC_Mass->Add(other_TM_MuT_OC_Mass);
      TM_MuT_OC_M_QualityCuts->Add(other_TM_MuT_OC_M_QualityCuts);
      TM_MuT_OC_M_QC_DXY->Add(other_TM_MuT_OC_M_QC_DXY);
      TM_MuT_OC_M_QC_DXY_Iso->Add(other_TM_MuT_OC_M_QC_DXY_Iso);
      TM_MuJetTriggered->Add(other_TM_MuJetTriggered);
      TM_JetTriggered->Add(other_TM_JetTriggered);

   }

   _outputfile->cd(_dirname.c_str());     

   TGraphAsymmErrors efficiency(generalefficiency_numerator, generalefficiency_denominator);
   efficiency.SetNameTitle("efficiency", "Reconstruction efficiency");

   TGraphAsymmErrors TwoMuonsEff(TwoMuons, generalefficiency_denominator);
   TwoMuonsEff.SetNameTitle("TwoMuonsEff", "2 muons eff");

   TGraphAsymmErrors TM_MuTriggeredEff(TM_MuTriggered, TwoMuons);
   TM_MuTriggeredEff.SetNameTitle("TM_MuTriggeredEff", "2 muons + trigger eff");

   TGraphAsymmErrors TM_MuTriggeredDataDrivenEff(TM_MuJetTriggered, TM_JetTriggered);
   TM_MuTriggeredDataDrivenEff.SetNameTitle("TM_MuTriggeredDataDrivenEff", "trigger eff data driven");

   TGraphAsymmErrors TM_MuT_OppositeCharge_Eff(TM_MuT_OppositeCharge, TM_MuTriggered);
   TM_MuT_OppositeCharge_Eff.SetNameTitle("TM_MuT_OppositeCharge_Eff", "2 triggered muons Opposite Charge eff");

   TGraphAsymmErrors TM_MuT_OC_Mass_Eff(TM_MuT_OC_Mass, TM_MuT_OppositeCharge);
   TM_MuT_OC_Mass_Eff.SetNameTitle("TM_MuT_OC_Mass_Eff", "2 triggered muons Opposite Charge with Mass constraint");

   TGraphAsymmErrors TM_MuT_OC_M_QualityCuts_Eff(TM_MuT_OC_M_QualityCuts, TM_MuT_OC_Mass);
   TM_MuT_OC_M_QualityCuts_Eff.SetNameTitle("TM_MuT_OC_M_QualityCuts_Eff", "2 triggered muons Opposite Charge with Mass constraint Quality Cuts eff");

   TGraphAsymmErrors TM_MuT_OC_M_QC_DXY_Eff(TM_MuT_OC_M_QC_DXY, TM_MuT_OC_M_QualityCuts);
   TM_MuT_OC_M_QC_DXY_Eff.SetNameTitle("TM_MuT_OC_M_QC_DXY_Eff", "2 triggered muons Opposite Charge with Mass constraint with Quality Cuts and DXY cut eff");

   TGraphAsymmErrors TM_MuT_OC_M_QC_DXY_Iso_Eff(TM_MuT_OC_M_QC_DXY_Iso, TM_MuT_OC_M_QC_DXY);
   TM_MuT_OC_M_QC_DXY_Iso_Eff.SetNameTitle("TM_MuT_OC_M_QC_DXY_Iso_Eff", "2 triggered muons Opposite Charge Quality Cuts and DXY cut, mass constraint with isolation eff");

   efficiency.Write();

   TwoMuonsEff.Write();
   TM_MuTriggeredEff.Write();
   TM_MuT_OppositeCharge_Eff.Write();
   TM_MuT_OC_Mass_Eff.Write();
   TM_MuT_OC_M_QualityCuts_Eff.Write();
   TM_MuT_OC_M_QC_DXY_Eff.Write();
   TM_MuT_OC_M_QC_DXY_Iso_Eff.Write();
   TM_MuTriggeredDataDrivenEff.Write();


   //tag and probe
   _tp_TM_MuT_OC_M_QC->finalize(norm_factor, plotstoadd, trainingfile);
   _tp_TM_MuT_OC_M_QC_DXY->finalize(norm_factor, plotstoadd, trainingfile);
   _tp_TM_MuT_OC_M_QC_DXY_Iso->finalize(norm_factor, plotstoadd, trainingfile);

   if (!_performfits) return;
   //otherwise lets compute full T&P efficiency
   std::vector<TGraphAsymmErrors*> TPefficiences;
   //_outputfile->cd(_dirname.c_str());
   string name1 = _dirname + "/" + "TM_MuT_OC_M_QCDoubleMuTag&Probe";
   TPefficiences.push_back( (TGraphAsymmErrors*) _outputfile->Get( name1.c_str() ) );
   string name2 = _dirname + "/" + "TM_MuT_OC_M_QC_DXYDoubleMuTag&Probe";
   TPefficiences.push_back( (TGraphAsymmErrors*) _outputfile->Get( name2.c_str() ) );
   string name3 = _dirname + "/" + "TM_MuT_OC_M_QC_DXY_IsoDoubleMuTag&Probe";
   TPefficiences.push_back( (TGraphAsymmErrors*) _outputfile->Get( name3.c_str() ) );

   TGraphAsymmErrors totalTandP = multiplyTGraphs(TPefficiences, "totalTag&Probe");
   totalTandP.Write();

   for (int i = 0; i < TPefficiences.size(); ++i){
     if (TPefficiences[i]) delete TPefficiences[i]; 
   }
*/
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
