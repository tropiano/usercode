
#include "Firenze/PATAnalysis/include/RecoRapidityGaps.h"
#include "Firenze/PATAnalysis/include/JetUtilities.h"

#include <TH2.h>
#include <TStyle.h>
#include <math.h>

#include "TList.h"
#include "TParameter.h"
#include "TFile.h"
#include "TProofOutputFile.h"

#include <iostream>

//CMSSW headers
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

using namespace std;
using namespace edm; 


RecoRapidityGaps::RecoRapidityGaps(): 
_plots(), _tpfiller2jet(0), _tpfiller3jet(0), _2jnum(0), _2jden(0), _3jnum(0), _3jden(0)
{}

void RecoRapidityGaps::begin(TFile* out, const edm::ParameterSet& iConfig){
  _file = out; 
  std::string dirname = iConfig.getParameter<std::string>("Name");
  _isMC = iConfig.getParameter<bool>("isMC");

  cout << "RecoRapidityGaps file name : " << _file->GetName() << endl;
  _file->cd();
  _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
  _dir->cd();
  
  _plots.init(iConfig);
  
  if (_isMC){
    _2jnum = new TH1F("2jMCnum", "2jMCnum", 10, 0., 5.);
    _2jden = new TH1F("2jMCden", "2jMCden", 10, 0., 5.);
    _3jnum = new TH1F("3jMCnum", "3jMCnum", 10, 0., 5.);
    _3jden = new TH1F("3jMCden", "3jMCden", 10, 0., 5.);
  }

  _dir->cd("-");

  std::vector<bool (*)(const reco::Candidate&)> tag_cuts;
  //tag_cuts.push_back(isJetMatched);
  //tag_cuts.push_back(tightJetPt);
  std::vector<bool (*)(const reco::Candidate&)> probe_cuts;
  std::vector<bool (*)(const reco::Candidate&)> passprobe_cuts;
  passprobe_cuts.push_back(isJetMatched);

  _tpfiller2jet = new TagAndProbeFiller(_dir, string("JetTriggerTP_2jets"), 10, 0., 5., tag_cuts, probe_cuts, passprobe_cuts);
  _tpfiller3jet = new TagAndProbeFiller(_dir, string("JetTriggerTP_3jets"), 10, 0., 5., tag_cuts, probe_cuts, passprobe_cuts);

  cout << "RecoRapidityGaps Worker built." << endl;  

  _activity = new TH1F("activity", "activity", 10, 0., 5.);
  _jet15andactivity = new TH1F("jet15andactivity", "jet15andactivity", 10, 0., 5.);
}

RecoRapidityGaps::~RecoRapidityGaps(){
  _file->ls();
}

//void  RecoRapidityGaps::process(const fwlite::ChainEvent& iEvent)
void  RecoRapidityGaps::process(const fwlite::Event& iEvent)
{
  _file->cd();

  //std::cout << "Inizio " << std::endl;       
  bool isaccept15 = accept(iEvent, "HLT_Jet15U");
  bool isaccept6 = accept(iEvent, "HLT_Activity_PixelClusters");//"HLT_L1Jet10U");

  _plots.cacheJets(iEvent);


  bool isMatched = false;  
  const std::vector<pat::Jet>& jets = _plots.getJets();
  std::vector<pat::Jet>::const_iterator ijet;
  std::vector<pat::Jet>::const_iterator beg = jets.begin();
  std::vector<pat::Jet>::const_iterator end = jets.end();
  for (ijet = beg ; ijet != end; ++ijet){
    if (isJetMatched(*ijet)){
      isMatched = true;
      break;
    }
  }  
  //if (!_isMC) 
  //  if (!isaccept15 || !isMatched) return;

  if (isaccept15 && isMatched) _plots.fill();


  //to use our t&p code make a composite candicate out of the two jets
  if (jets.size()<2) return;
  double deltay = fabs(jets.front().rapidity()-jets.back().rapidity());
  if (isaccept6) _activity->Fill(deltay);

  if (jets.size() == 2){
    reco::CompositeCandidate cc;
    cc.addDaughter(jets.front(), "FWD_jet");
    cc.addDaughter(jets.back(), "BWD_jet");
    if (isaccept6) {
      if (isaccept15 && isMatched) _jet15andactivity->Fill(deltay); 
      _tpfiller2jet->fill(cc, deltay);
    }  
    if (_isMC){
      _2jden->Fill(deltay);
      if (isaccept15 && isMatched) _2jnum->Fill(deltay);
    }
  } else {
    for (int ii = 0; ii < jets.size()-1; ++ii){
      for (int jj = ii+1; jj < jets.size(); ++jj){
        reco::CompositeCandidate cc;
        cc.addDaughter(jets[ii], "FWD_jet");
        cc.addDaughter(jets[jj], "BWD_jet");
        if (isaccept6) _tpfiller3jet->fill(cc, deltay);
      }
    }
    if (_isMC){
      _3jden->Fill(deltay);
      if (isaccept15 && isMatched) _3jnum->Fill(deltay);
    }
  }
  //std::cout << "Fine " << std::endl;      
}

void RecoRapidityGaps::finalize(){
  _tpfiller2jet->finalize();
  _tpfiller3jet->finalize();
  _file->Write();
}

bool RecoRapidityGaps::accept(const fwlite::Event& iEvent, std::string name) const {
  try {
    fwlite::Handle<pat::TriggerEvent> triggerHandle;
    triggerHandle.getByLabel(iEvent, "patTriggerEvent");


    //mimic requirement on trigger bits
    const pat::TriggerPath* path1 = triggerHandle->path(name);
    if (!path1) {
      std::cout << "ERROR! trigger path " << name << " not found " << std::endl;
      throw cms::Exception("PATAnalysis:isTriggered") << "ERROR! trigger path  not found ";
      return false;
    }
    if (! path1->wasAccept() ) return false;
  } catch (cms::Exception e) {
    std::cout << e.what() << std::endl;
  }

  return true;

}
