
#include "Firenze/PATAnalysis/include/GenRapidityGaps.h"
#include "Firenze/PATAnalysis/include/Utilities.h"

#include <TH2.h>
#include <TStyle.h>
#include <math.h>
//#include "Utilities.h"

#include "TList.h"
#include "TParameter.h"
#include "TFile.h"
#include "TProofOutputFile.h"

#include <iostream>

//CMSSW headers
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

using namespace std;
using namespace edm; 


GenRapidityGaps::GenRapidityGaps(): 
_plots()
{ }

void GenRapidityGaps::begin(TFile* out, const edm::ParameterSet& iConfig){
  _file = out; 
  std::string dirname = iConfig.getParameter<std::string>("Name");

  cout << "GenRapidityGaps file name : " << _file->GetName() << endl;
  _file->cd();
  _dir = _file->mkdir(dirname.c_str(), dirname.c_str());
  _dir->cd();
  
  _plots.init(iConfig);

  _dir->cd("-");
   
  cout << "GenRapidityGaps Worker built." << endl;   
}

GenRapidityGaps::~GenRapidityGaps(){
  _file->ls();
}

//void  GenRapidityGaps::process(const fwlite::ChainEvent& iEvent)
void  GenRapidityGaps::process(const fwlite::Event& iEvent)
{
  _file->cd();
  _plots.cacheJets(iEvent);
  _plots.fill();
}

void GenRapidityGaps::finalize(){
  _file->Write();
}
