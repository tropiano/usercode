#ifndef TagAndProbeFiller_h
#define TagAndProbeFiller_h

#include "TFile.h"
#include <TH1D.h>

#include <vector>
#include <string>
#include <iostream>

/*
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooCategory.h"
*/
#include "TTree.h"

#include "DataFormats/Candidate/interface/Candidate.h"


class TagAndProbeFiller {
public :

    TagAndProbeFiller(TDirectory* output, std::string name, int nbins, double xmin, double xmax,
                const std::vector<bool (*)(const reco::Candidate&)>& tag_cuts,
                const std::vector<bool (*)(const reco::Candidate&)>& probe_cuts,
                const std::vector<bool (*)(const reco::Candidate&)>& passprobe_cuts,
                std::string tpflag="");

    ~TagAndProbeFiller();

    void fill(const reco::Candidate& Z, double x, double w = 1.);

    void finalize() const;


private:  
  bool tag(const reco::Candidate& muon);
  void probe(const reco::Candidate& muon, double mass, double x, double w);
  bool applyCuts(const reco::Candidate& cand, const std::vector<bool (*)(const reco::Candidate&)>& cuts);

  TDirectory* _output;
  std::string _name;
  TH1D* _numerator;
  TH1D* _denominator;

  std::vector<TH1D*> _v_mass_tagprobe;
  std::vector<TH1D*> _v_mass_tagpassprobe;

  TTree* _rootree;
  /*
  RooDataSet* _rootree;
  //tree vars
  RooRealVar _mass;
  RooRealVar _bin;
  RooRealVar _probe, _passprobe, _weight;
  RooCategory _passprobe_cat;
  RooArgSet* _argset;
  */
  double _mass;
  double _bin;
  double _probe, /*_passprobe,*/ _weight;
  //char _passprobe[8];
  int _passprobe;
  std::vector<bool (*)(const reco::Candidate&)> _tag_cuts;
  std::vector<bool (*)(const reco::Candidate&)> _probe_cuts;
  std::vector<bool (*)(const reco::Candidate&)> _passprobe_cuts;
  
  std::string _tpflag;

};

#endif

