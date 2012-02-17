#ifndef TagAndProbeFillerMuon_h
#define TagAndProbeFillerMuon_h

#include "TFile.h"
#include <TH1D.h>

#include <vector>
#include <string>
#include <iostream>
#include "TTree.h"
#include "DataFormats/Candidate/interface/Candidate.h"


class TagAndProbeFillerMuon {
public :

    TagAndProbeFillerMuon(TDirectory* output, std::string name, int nbins, double xmin, double xmax,
                const std::vector<bool (*)(const reco::Candidate&, int run, double rho)>& tag_cuts,
                const std::vector<bool (*)(const reco::Candidate&, int run, double rho)>& probe_cuts,
                const std::vector<bool (*)(const reco::Candidate&, int run, double rho)>& passprobe_cuts,
                std::string tpflag="",
                bool onecombinationonly=false);

    ~TagAndProbeFillerMuon();

    void fill(const reco::Candidate& Z, int run=0, double rho=0, double x=0., double w = 1., double pT1=0., double pT2=0., double pT3=0.);

    void finalize() const;


private:  
  bool tag(const reco::Candidate& muon, int run, double rho);
  void probe(const reco::Candidate& muon, double mass, double x, double w, int run, double rho);
  bool applyCuts(const reco::Candidate& cand, int run, double rho, const std::vector<bool (*)(const reco::Candidate&, int run, double rho)>& cuts);

  TDirectory* _output;
  std::string _name;
  TH1D* _numerator;
  TH1D* _denominator;

  std::vector<TH1D*> _v_mass_tagprobe;
  std::vector<TH1D*> _v_mass_tagpassprobe;

  TTree* _rootree;
  
  double _mass;
  double _bin;
  double _probe, _weight;
  double _pt1, _pt2, _pt3;
  
  int _passprobe;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> _tag_cuts;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> _probe_cuts;
  std::vector<bool (*)(const reco::Candidate&, int run, double rho)> _passprobe_cuts;
  
  std::string _tpflag;
  bool _onecombination;
  int _run;
  double _rho;
  
};

#endif

