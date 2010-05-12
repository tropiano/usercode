
#include "Firenze/PATAnalysis/include/TestAnalysis.h"


//CMSSW headers
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

using namespace std;
using namespace edm; 


TestAnalysis::TestAnalysis(): 
_plot(0)
{ }

void TestAnalysis::begin(TFile* out, const edm::ParameterSet& iConfig){
  _file = out; 
  std::string dirname = iConfig.getParameter<std::string>("Name");

  cout << "TestAnalysis file name : " << _file->GetName() << endl;
  _file->cd();
  _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
  _dir->cd();
  
  _plot = new TH1F("jetpt", "jetpt", 100, 0., 100.);

  _dir->cd("-");
   
  cout << "TestAnalysis Worker built." << endl;   
}

TestAnalysis::~TestAnalysis(){
  _file->ls();
}

//void  TestAnalysis::process(const fwlite::ChainEvent& iEvent)
void  TestAnalysis::process(const fwlite::Event& iEvent)
{
  _file->cd();
  //_plots.cacheJets(iEvent);
  fwlite::Handle<std::vector<pat::Jet> > jetHandle;
  jetHandle.getByLabel(iEvent, "selectedPFJets");

  std::vector<pat::Jet>::const_iterator ijet;
  std::vector<pat::Jet>::const_iterator beg = jetHandle->begin();
  std::vector<pat::Jet>::const_iterator end = jetHandle->end();


  for (ijet = beg; ijet != end; ++ijet){
    _plot->Fill(ijet->pt());
  }
}

void TestAnalysis::finalize(){
  _file->Write();
}
