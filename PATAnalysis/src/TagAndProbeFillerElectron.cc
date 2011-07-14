#define TagAndProbeFillerElectron_cxx

#include "Firenze/PATAnalysis/include/TagAndProbeFillerElectron.h"

#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;

TagAndProbeFillerElectron::TagAndProbeFillerElectron(TDirectory* output, std::string name, int nbins, double xmin, double xmax,
                         const std::vector<bool (*)(const reco::Candidate&, int run)>& tag_cuts,
                         const std::vector<bool (*)(const reco::Candidate&, int run)>& probe_cuts,
                         const std::vector<bool (*)(const reco::Candidate&, int run)>& passprobe_cuts,
                         string tpflag,
                         bool onecombinationonly): 

_output(output),
_name(name),
_mass(0.),
_bin(0),
_probe(0.),
_weight(1.),
_passprobe(0),
_tag_cuts(tag_cuts),
_probe_cuts(probe_cuts),
_passprobe_cuts(passprobe_cuts),
_tpflag(tpflag),
_onecombination(onecombinationonly)
{
  _output->cd();
  _numerator = new TH1D((name+"SingleCandTag&Probe_numerator").c_str(), (name+"SingleCandTag&Probe_numerator").c_str(), nbins, xmin, xmax);
  _denominator = new TH1D((name+"SingleCandTag&Probe_denominator").c_str(), (name+"SingleCandTag&Probe_denominator").c_str(), nbins, xmin, xmax);
  for (int i = 0; i < nbins; ++i){
    stringstream name1;
    name1 << name << "TagProbeMassBin" <<i;
    _v_mass_tagprobe.push_back(new TH1D(name1.str().c_str(), name1.str().c_str(), 100, 60., 120.));
    stringstream name2;
    name2 << name << "TagPassProbeMassBin" << i;
    _v_mass_tagpassprobe.push_back(new TH1D(name2.str().c_str(), name2.str().c_str(), 100, 60., 120.));
  }
  string dsname("dataset");
  dsname+=name;
  _rootree = new TTree(dsname.c_str(), dsname.c_str());
  _rootree->Branch("mass", &_mass, "mass/D");
  _rootree->Branch("bin",  &_bin, "bin/D");
  _rootree->Branch("probe", &_probe, "probe/D");
  _rootree->Branch("passprobe", &_passprobe, "passprobe/I");
  _rootree->Branch("weight", &_weight, "weight/D");
  _rootree->Branch("pt1", &_pt1, "pt1/D");
  _rootree->Branch("pt2", &_pt2, "pt2/D");
  _rootree->Branch("pt3", &_pt3, "pt3/D");
  srand(time(0));
}

TagAndProbeFillerElectron::~TagAndProbeFillerElectron(){
  delete _numerator;
  delete _denominator;
}

void TagAndProbeFillerElectron::fill(const reco::Candidate& Z, int run, double x, double w, double pt1, double pt2, double pt3){
  
  _mass = 0.;
  _probe = 0.;
  _passprobe = 0;
  _pt1 = pt1;
  _pt2 = pt2;
  _pt3 = pt3;
  _run = run;
  
  if((*Z.daughter(0)).pt()<(*Z.daughter(1)).pt()){
  throw cms::Exception("PATAnalysis:PtEl") << "################ ERROR! Pt0 < Pt1";
  }

  if (_onecombination){
  
    //gli elettroni sono ordinati in pt e io voglio usare il subleading come tag nel caso usi una sel asimmetrica
    //perche' e' sul leading che voglio distinguere i casi passa/non passa selezione offline
    
    if (tag(*(Z.daughter(1)), _run)) probe(*(Z.daughter(0)), Z.mass(), x, w, _run); 
  } else {
    //tag cand 1, probe cand 2
    if(_tpflag=="" || _tpflag=="soft"){
      if (tag(*(Z.daughter(0)), _run)) probe(*(Z.daughter(1)), Z.mass(), x, w, _run);
    }

    //reset
    _mass = 0.;
    _probe = 0.;
    _passprobe = 0;
 
    //tag cand 2 probe cand 1
    if(_tpflag=="" || _tpflag=="hard"){ 
      if (tag(*(Z.daughter(1)), _run)) probe(*(Z.daughter(0)), Z.mass(), x, w, _run);
    }
  }
}

bool TagAndProbeFillerElectron::tag(const reco::Candidate& cand, int run){
  return applyCuts(cand, run, _tag_cuts);
}

void TagAndProbeFillerElectron::probe(const reco::Candidate& cand, double mass, double x, double w, int run){

  if ( applyCuts(cand, run, _probe_cuts) ) { 
    _denominator->Fill(x,w);
    unsigned int ibin = _denominator->FindBin(x);
    if (ibin > 0 && ibin <= _v_mass_tagprobe.size()){
      _v_mass_tagprobe[ibin-1]->Fill(mass, w);
      _mass = mass;
      _bin = double(ibin-1);
      _probe = 1.;
    } else cout << "warning in TagAndProbeFillerElectron::probe bin index out of range: " << ibin << endl;
  }
  
  if ( applyCuts(cand, run, _passprobe_cuts) ) {
    _numerator->Fill(x,w);
    unsigned int ibin = _numerator->FindBin(x);
    if (ibin > 0 && ibin <= _v_mass_tagpassprobe.size()){
      _v_mass_tagpassprobe[ibin-1]->Fill(mass, w);
      _mass = mass;
      _bin = double(ibin-1);
      _passprobe = 1;
     
    } else cout << "warning in TagAndProbeFillerElectron::probe bin index out of range: " << ibin << endl;
  }
  
  _weight=w;
  _rootree->Fill();
  
}

bool TagAndProbeFillerElectron::applyCuts(const reco::Candidate& cand, int run, const std::vector<bool (*)(const reco::Candidate&, int run)>& cuts) {
  std::vector<bool (*)(const reco::Candidate&, int run)>::const_iterator begin = cuts.begin();
  std::vector<bool (*)(const reco::Candidate&, int run)>::const_iterator end = cuts.end();
  std::vector<bool (*)(const reco::Candidate&, int run)>::const_iterator i;
  for (i = begin; i != end; ++i){
    if ((*i)(cand, run) == false) return false;
  }
  return true;
}

void TagAndProbeFillerElectron::finalize() const {
  _output->cd();
  _rootree->Write();
}

