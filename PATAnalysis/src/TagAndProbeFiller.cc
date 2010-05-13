#define TagAndProbeFiller_cxx

#include "Firenze/PATAnalysis/include/TagAndProbeFiller.h"
#include "Firenze/PATAnalysis/include/Utilities.h"

#include <iostream>
#include <sstream>

using namespace std;

TagAndProbeFiller::TagAndProbeFiller(TDirectory* output, std::string name, int nbins, double xmin, double xmax,
                         const std::vector<bool (*)(const reco::Candidate&)>& tag_cuts,
                         const std::vector<bool (*)(const reco::Candidate&)>& probe_cuts,
                         const std::vector<bool (*)(const reco::Candidate&)>& passprobe_cuts): 
_output(output),
_name(name),

_mass(0.),
_bin(0),
_probe(0.),
_weight(1.),
_passprobe(0),
_tag_cuts(tag_cuts),
_probe_cuts(probe_cuts),
_passprobe_cuts(passprobe_cuts)
{
  _output->cd();
  _numerator = new TH1D((name+"SingleCandTag&Probe_numerator").c_str(), (name+"SingleCandTag&Probe_numerator").c_str(), nbins, xmin, xmax);
  _denominator = new TH1D((name+"SingleCandTag&Probe_denominator").c_str(), (name+"SingleCandTag&Probe_denominator").c_str(), nbins, xmin, xmax);
  for (int i = 0; i < nbins; ++i){
    stringstream name1;
    name1 << name << "TagProbeMassBin" <<i;
    _v_mass_tagprobe.push_back(new TH1D(name1.str().c_str(), name1.str().c_str(), 30, 60., 120.));
    stringstream name2;
    name2 << name << "TagPassProbeMassBin" << i;
    _v_mass_tagpassprobe.push_back(new TH1D(name2.str().c_str(), name2.str().c_str(), 30, 60., 120.));
  }
  _rootree = new TTree("dataset", "dataset");
  _rootree->Branch("mass", &_mass, "mass/D");
  _rootree->Branch("bin",  &_bin, "bin/D");
  _rootree->Branch("probe", &_probe, "probe/D");
  _rootree->Branch("passprobe", &_passprobe, "passprobe/I");
  _rootree->Branch("weight", &_weight, "weight/D");
}

TagAndProbeFiller::~TagAndProbeFiller(){
  delete _numerator;
  delete _denominator;
}

void TagAndProbeFiller::fill(const reco::Candidate& Z, double x, double w){
  
  _mass = 0.;
  _probe = 0.;
  _passprobe = 0;
 
  //tag cand 1, probe cand 2 
  if (tag(*(Z.daughter(0)))) probe(*(Z.daughter(1)), Z.mass(), x, w);

  //reset
  _mass = 0.;
  _probe = 0.;
  _passprobe = 0;
 
  //tag cand 2 probe cand 1
  if (tag(*(Z.daughter(1)))) probe(*(Z.daughter(0)), Z.mass(), x, w);
  
}

bool TagAndProbeFiller::tag(const reco::Candidate& cand){
  return applyCuts(cand, _tag_cuts);
}

void TagAndProbeFiller::probe(const reco::Candidate& cand, double mass, double x, double w){
  if ( applyCuts(cand, _probe_cuts) ) { 
    _denominator->Fill(x,w);
    unsigned int ibin = _denominator->FindBin(x);
    if (ibin > 0 && ibin <= _v_mass_tagprobe.size()){
      _v_mass_tagprobe[ibin-1]->Fill(mass, w);
      _mass = mass;
      _bin = double(ibin-1);
      _probe = 1.;
    } else cout << "warning in TagAndProbeFiller::probe bin index out of range: " << ibin << endl;
  }
  
  if ( applyCuts(cand, _passprobe_cuts) ) {
    _numerator->Fill(x,w);
    unsigned int ibin = _numerator->FindBin(x);
    if (ibin > 0 && ibin <= _v_mass_tagpassprobe.size()){
      _v_mass_tagpassprobe[ibin-1]->Fill(mass, w);
      _mass = mass;
      _bin = double(ibin-1);
      _passprobe = 1;
     
    } else cout << "warning in TagAndProbeFiller::probe bin index out of range: " << ibin << endl;
  }
  
  _weight=w;
  _rootree->Fill();
  
}

bool TagAndProbeFiller::applyCuts(const reco::Candidate& cand, const std::vector<bool (*)(const reco::Candidate&)>& cuts) {
  std::vector<bool (*)(const reco::Candidate&)>::const_iterator begin = cuts.begin();
  std::vector<bool (*)(const reco::Candidate&)>::const_iterator end = cuts.end();
  std::vector<bool (*)(const reco::Candidate&)>::const_iterator i;
  for (i = begin; i != end; ++i){
    if ((*i)(cand) == false) return false;
  }
  return true;
}

void TagAndProbeFiller::finalize() const {
  _output->cd();
  _rootree->Write();
}

