#define TagAndProbeFillerElectron_cxx

#include "Firenze/PATAnalysis/include/TagAndProbeFillerElectron.h"

#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;

TagAndProbeFillerElectron::TagAndProbeFillerElectron(TDirectory* output, std::string name, int nbins, double xmin, double xmax,
                                             const std::vector<bool (*)(const reco::Candidate&, int run, double rho)>& tag_cuts,
                                             const std::vector<bool (*)(const reco::Candidate&, int run, double rho)>& probe_cuts,
                                             const std::vector<bool (*)(const reco::Candidate&, int run, double rho)>& passprobe_cuts,
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
_onecombination(onecombinationonly){

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
	_rootree->Branch("bin", &_bin, "bin/D");
	_rootree->Branch("probe", &_probe, "probe/D");
	_rootree->Branch("passprobe", &_passprobe, "passprobe/I");
	_rootree->Branch("weight", &_weight, "weight/D");
	srand(time(0));
}


TagAndProbeFillerElectron::~TagAndProbeFillerElectron(){
	delete _numerator;
	delete _denominator;
}


void TagAndProbeFillerElectron::fill(const reco::Candidate& Z, int run, double rho, double x, double w, double pt[], double eta){

	_mass = 0.;
	_probe = 0.;
	_passprobe = 0;
	_eta = eta;
	_run = run;
	_rho = rho;

	/*if(_eta==0 || _eta==1){
		for(int i=0; i<10; i++) cout<<"### TP = "<<pt[i]<<endl;
	}*/

	if((*Z.daughter(0)).pt()<(*Z.daughter(1)).pt()){
		throw cms::Exception("PATAnalysis:PtEl") << "################ ERROR! Pt0 < Pt1";
	}

	if (_onecombination){
	
	//i leptoni sono ordinati in pt e io voglio usare il subleading come tag nel caso usi una sel asimmetrica perche' e' sul leading che voglio distinguere i casi passa/non passa selezione offline

		if(_eta==-1 || (_eta==0 && fabs((Z.daughter(0))->eta())<1.2) || (_eta==1 && fabs((Z.daughter(0))->eta())>1.2)){

			if(_eta==0 || _eta==1){
				if(Z.daughter(0)->pt()>pt[0] && Z.daughter(0)->pt()<pt[1])  x = 0;
				if(Z.daughter(0)->pt()>pt[1] && Z.daughter(0)->pt()<pt[2])  x = 1;
				if(Z.daughter(0)->pt()>pt[2])                               x = 2;
/*
				if(Z.daughter(0)->pt()>pt[2] && Z.daughter(0)->pt()<pt[3])  x = 2;
				if(Z.daughter(0)->pt()>pt[3] && Z.daughter(0)->pt()<pt[4])  x = 3;
				if(Z.daughter(0)->pt()>pt[4] && Z.daughter(0)->pt()<pt[5])  x = 4;
				if(Z.daughter(0)->pt()>pt[5] && Z.daughter(0)->pt()<pt[6])  x = 5;
				if(Z.daughter(0)->pt()>pt[6] && Z.daughter(0)->pt()<pt[7])  x = 6;
				if(Z.daughter(0)->pt()>pt[7] && Z.daughter(0)->pt()<pt[8])  x = 7;
				if(Z.daughter(0)->pt()>pt[8] && Z.daughter(0)->pt()<pt[9])  x = 8;
				if(Z.daughter(0)->pt()>pt[9] && Z.daughter(0)->pt()<pt[10]) x = 9;
*/
			} 

			if (tag(*(Z.daughter(1)), _run, _rho)) probe(*(Z.daughter(0)), Z.mass(), x, w, _run, _rho);
		}

	}
	else {
  
		if(_eta==0 || _eta==1){
			if(Z.daughter(1)->pt()>pt[0] && Z.daughter(1)->pt()<pt[1])  x = 0;
			if(Z.daughter(1)->pt()>pt[1] && Z.daughter(1)->pt()<pt[2])  x = 1;
			if(Z.daughter(1)->pt()>pt[2])                               x = 2;
/*
			if(Z.daughter(1)->pt()>pt[2] && Z.daughter(1)->pt()<pt[3])  x = 2;
			if(Z.daughter(1)->pt()>pt[3] && Z.daughter(1)->pt()<pt[4])  x = 3;
			if(Z.daughter(1)->pt()>pt[4] && Z.daughter(1)->pt()<pt[5])  x = 4;
			if(Z.daughter(1)->pt()>pt[5] && Z.daughter(1)->pt()<pt[6])  x = 5;
			if(Z.daughter(1)->pt()>pt[6] && Z.daughter(1)->pt()<pt[7])  x = 6;
			if(Z.daughter(1)->pt()>pt[7] && Z.daughter(1)->pt()<pt[8])  x = 7;
			if(Z.daughter(1)->pt()>pt[8] && Z.daughter(1)->pt()<pt[9])  x = 8;
			if(Z.daughter(1)->pt()>pt[9] && Z.daughter(1)->pt()<pt[10]) x = 9;
*/
		} 

		//tag cand 1, probe cand 2 -> efficiency lepton 1
		if(_eta==-1 || (_eta==0 && fabs((Z.daughter(1))->eta())<1.2) || (_eta==1 && fabs((Z.daughter(1))->eta())>1.2)){
			if(_tpflag=="" || _tpflag=="soft"){
				if (tag(*(Z.daughter(0)), _run, _rho)) probe(*(Z.daughter(1)), Z.mass(), x, w, _run, _rho);
			}
		}

		//reset
		_mass = 0.;
		_probe = 0.;
		_passprobe = 0;
	
		if(_eta==0 || _eta==1){
			if(Z.daughter(0)->pt()>pt[0] && Z.daughter(0)->pt()<pt[1])  x = 0;
			if(Z.daughter(0)->pt()>pt[1] && Z.daughter(0)->pt()<pt[2])  x = 1;
			if(Z.daughter(0)->pt()>pt[2])                               x = 2;
/*
			if(Z.daughter(0)->pt()>pt[2] && Z.daughter(0)->pt()<pt[3])  x = 2;
			if(Z.daughter(0)->pt()>pt[3] && Z.daughter(0)->pt()<pt[4])  x = 3;
			if(Z.daughter(0)->pt()>pt[4] && Z.daughter(0)->pt()<pt[5])  x = 4;
			if(Z.daughter(0)->pt()>pt[5] && Z.daughter(0)->pt()<pt[6])  x = 5;
			if(Z.daughter(0)->pt()>pt[6] && Z.daughter(0)->pt()<pt[7])  x = 6;
			if(Z.daughter(0)->pt()>pt[7] && Z.daughter(0)->pt()<pt[8])  x = 7;
			if(Z.daughter(0)->pt()>pt[8] && Z.daughter(0)->pt()<pt[9])  x = 8;
			if(Z.daughter(0)->pt()>pt[9] && Z.daughter(0)->pt()<pt[10]) x = 9;
*/
		} 

		//tag cand 2, probe cand 1 -> efficiency lepton 0
		if(_eta==-1 || (_eta==0 && fabs((Z.daughter(0))->eta())<1.2) || (_eta==1 && fabs((Z.daughter(0))->eta())>1.2)){
			if(_tpflag=="" || _tpflag=="hard"){ 
				if (tag(*(Z.daughter(1)), _run, _rho)) probe(*(Z.daughter(0)), Z.mass(), x, w, _run, _rho);
			}
		}
	}
}


bool TagAndProbeFillerElectron::tag(const reco::Candidate& cand, int run, double rho){
	return applyCuts(cand, run, rho, _tag_cuts);
}


void TagAndProbeFillerElectron::probe(const reco::Candidate& cand, double mass, double x, double w, int run, double rho){

	if ( applyCuts(cand, run, rho, _probe_cuts) ){ 
		_denominator->Fill(x,w);
		unsigned int ibin = _denominator->FindBin(x);
		if (ibin > 0 && ibin <= _v_mass_tagprobe.size()){
			_v_mass_tagprobe[ibin-1]->Fill(mass, w);
			_mass = mass;
			_bin = double(ibin-1);
			_probe = 1.;
		}
		else cout << "warning in TagAndProbeFillerElectron::probe bin index out of range: " << ibin << endl;
	}
  
	if ( applyCuts(cand, run, rho, _passprobe_cuts) ){
		_numerator->Fill(x,w);
		unsigned int ibin = _numerator->FindBin(x);
		if (ibin > 0 && ibin <= _v_mass_tagpassprobe.size()){
			_v_mass_tagpassprobe[ibin-1]->Fill(mass, w);
			_mass = mass;
			_bin = double(ibin-1);
			_passprobe = 1;
		}
		else cout << "warning in TagAndProbeFillerElectron::probe bin index out of range: " << ibin << endl;
	}
  
	_weight=w;
	_rootree->Fill();
  
}


bool TagAndProbeFillerElectron::applyCuts(const reco::Candidate& cand, int run, double rho, const std::vector<bool (*)(const reco::Candidate&, int run, double rho)>& cuts) {

	std::vector<bool (*)(const reco::Candidate&, int run, double rho)>::const_iterator begin = cuts.begin();
	std::vector<bool (*)(const reco::Candidate&, int run, double rho)>::const_iterator end = cuts.end();
	std::vector<bool (*)(const reco::Candidate&, int run, double rho)>::const_iterator i;
	for (i = begin; i != end; ++i){
		if ((*i)(cand, run, rho) == false) return false;
	}
	return true;
}


void TagAndProbeFillerElectron::finalize() const {
	_output->cd();
	_rootree->Write();
}

