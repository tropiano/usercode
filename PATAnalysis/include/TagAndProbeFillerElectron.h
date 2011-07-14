#ifndef TagAndProbeFillerElectron_h
#define TagAndProbeFillerElectron_h

#include "TFile.h"
#include <TH1D.h>

#include <vector>
#include <string>
#include <iostream>
#include "TTree.h"
#include "DataFormats/Candidate/interface/Candidate.h"


class TagAndProbeFillerElectron {
public :

    TagAndProbeFillerElectron(TDirectory* output, std::string name, int nbins, double xmin, double xmax,
                const std::vector<bool (*)(const reco::Candidate&, int run)>& tag_cuts,
                const std::vector<bool (*)(const reco::Candidate&, int run)>& probe_cuts,
                const std::vector<bool (*)(const reco::Candidate&, int run)>& passprobe_cuts,
                std::string tpflag="",
                bool onecombinationonly=false);

    ~TagAndProbeFillerElectron();

    void fill(const reco::Candidate& Z, int run=0, double x=0., double w = 1., double pT1=0., double pT2=0., double pT3=0.);

    void finalize() const;


private:  
  bool tag(const reco::Candidate& electron, int run);
  void probe(const reco::Candidate& electron, double mass, double x, double w, int run);
  bool applyCuts(const reco::Candidate& cand, int run, const std::vector<bool (*)(const reco::Candidate&, int run)>& cuts);

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
  std::vector<bool (*)(const reco::Candidate&, int run)> _tag_cuts;
  std::vector<bool (*)(const reco::Candidate&, int run)> _probe_cuts;
  std::vector<bool (*)(const reco::Candidate&, int run)> _passprobe_cuts;
  
  std::string _tpflag;
  bool _onecombination;
  int _run;
  
};

#endif

