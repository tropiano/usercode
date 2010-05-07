#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/AnalysisLoader.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/RivetAIDA.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/InvMassFinalState.hh"
#include <algorithm>
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TH1F.h"
#include "fastjet/ClusterSequence.hh"
//#include "TMath.h"
#include <cmath>
#include <iostream>
using namespace std;

namespace Rivet {
  
  class HadronicV : public Analysis {
    
  public:
    
    /// Default constructor
    HadronicV():_jetPtCut(400.),_mass_drop_threshold(0.333),_rtycut(0.3)
    {
      //W,Z->qq (hadronic) final state
      const FinalState hfs;
      addProjection(hfs, "HFS");
      addProjection(FastJets(hfs, FastJets::CAM, 0.7, 100.0), "CAMJets");
      //addProjection(FastJets(vfs, FastJets::KT, 0.5, 30.0), "KtJets");
    }
    
    /// Factory method
    static Analysis* create() { 
      return new HadronicV(); 
    }
    
    /// Return the name of this analysis
    string name() const {
      return "HadronicV";
    }
    
    /// Get the SPIRES ID code
    string spiresId() const {
      return "NONE";
    }
    
    /// Get a description of the analysis.
    string description() const {
      return "HadronicV analysis";
    }
    
    /// Experiment which performed and published this analysis.
    string experiment() const {
      return "CMS";
    }
    
    /// When published (preprint year according to SPIRES).
    string year() const {
      return "NONE";
    }
    
    /// Summary of analysis
    string summary() const{
      return "An analysis that looks for W and Z in hadronic final states";
    }
    
    /// Beam conditions for this analysis
    string runInfo() const{
      return "Anything!";
    }
    
    string collider() const{
      return "LHC";
    }
    
    vector<string> authors() const{
      return vector<string>();
    }
    
    vector<string> references() const{
      return vector<string>();
    }

    typedef fastjet::JetDefinition::DefaultRecombiner DefRecomb;
    class FlavourRecombiner : public  DefRecomb {
    public:
      FlavourRecombiner(fastjet::RecombinationScheme recomb_scheme = fastjet::E_scheme) : 
	DefRecomb(recomb_scheme) {};
      
      virtual std::string description() const {return DefRecomb::description()
	  +" (with user index addition)";}
      
      virtual void recombine(const fastjet::PseudoJet & pa, 
			     const fastjet::PseudoJet & pb, 
			     fastjet::PseudoJet & pab) const {
	DefRecomb::recombine(pa,pb,pab);
	pab.set_user_index(pa.user_index() + pb.user_index());
      }  
    };
        
    //@name Analysis methods
    //@{
    void init() {
      //create and open the root file
      _treeFile = new TFile("hadronicV.root", "recreate");
      //create the tree
      _HadronicVTree = new TTree("HadronicVTree", "Tree for HadronicV analysis");
      //event number
      _HadronicVTree->Branch("invmass", &_invmass, "invmass/F");
      _HadronicVTree->Branch("jetmult", &_jetmult, "jetmult/F");
      _HadronicVTree->Branch("lead_jet_pt", &_lead_jet_pt, "lead_jet_pt/F");
      _HadronicVTree->Branch("lead_jet_eta", &_lead_jet_eta, "lead_jet_eta/F");
      _HadronicVTree->Branch("lead_jet_mass", &_lead_jet_mass, "lead_jet_mass/F");
      _HadronicVTree->Branch("jet_phi_diff", &_jet_phi_diff, "jet_phi_diff/F");
    }
    
    void analyze(const Event& event) {
      const FinalState& hfs = applyProjection<FinalState>(event, "HFS");
      const FastJets &jetProj = applyProjection<FastJets>(event, "CAMJets");
      const fastjet::ClusterSequence* cs = jetProj.clusterSeq();
      Rivet::PseudoJets jets = sorted_by_pt(cs->inclusive_jets());
      _invmass=-10;
      _jet_phi_diff= fabs(jets[0].phi() - jets[1].phi());
      if (_jet_phi_diff>PI) _jet_phi_diff=2*PI-_jet_phi_diff;
      Rivet::PseudoJets jets400;
      _lead_jet_pt=jets[0].perp();
      _lead_jet_eta=jets[0].pseudorapidity();
      _lead_jet_mass=jets[0].m();
      int njets=0;
      for (PseudoJets::const_iterator jIt =jets.begin(); jIt != jets.end(); ++jIt)
	{
	  if(jIt->perp() > _jetPtCut)
	    {
	      jets400.push_back(*jIt);
	      njets++;
	    }
	}
      _jetmult=njets;
      for (PseudoJets::const_iterator jIt =jets400.begin(); jIt != jets400.end(); ++jIt)
	{
	  _lead_jet_pt=jIt->perp();
	  _lead_jet_eta=jIt->pseudorapidity();
	  _lead_jet_mass=jIt->m();
	  if(jIt->perp()>_jetPtCut)
	    {
	      //cout << "Hardest jet: " << jets[0].perp() << endl;
	      //Jets theJets = jetProj.jetsByPt(20.);
	      //cout<<"number of jets found: "<<jets.size()<<endl;
	      //bulk of the code (copied and adapted from fastjet_boosted_higgs on fastjet web page)
	      /// now do the subjet decomposition;
	      //
	      /// when unpeeling a C/A jet, often only a very soft piece may break off;
	      /// the mass_drop_threshold indicates how much "lighter" the heavier of the two
	      /// resulting pieces must be in order for us to consider that we've really
	      /// seen some form of substructure
	      //double mass_drop_threshold = 0.667; 
	      /// QCD backgrounds that give larger jet masses have a component
	      /// where a quite soft gluon is emitted; to eliminate part of this
	      /// one can place a cut on the asymmetry of the branching; 
	      ///
	      /// Here the cut is expressed in terms of y, the kt-distance scaled
	      /// to the squared jet mass; an easier way to see it is in terms of
	      /// a requirement on the momentum fraction in the splitting: z/(1-z)
	      /// and (1-z)/z > rtycut^2 [the correspondence holds only at LO]
	      //double rtycut              = 0.3;
	      fastjet::PseudoJet this_jet = *jIt, parent1, parent2;
	      bool had_parents;
	      //fastjet::ClusterSequence cs = *(jetProj.clusterSeq());
	      while ((had_parents = cs->has_parents(this_jet,parent1,parent2))) {
		// make parent1 the more massive jet
		if (parent1.m() < parent2.m()) swap(parent1,parent2);
		//
		// if we pass the conditions on the mass drop and its degree of
		// asymmetry (z/(1-z) \sim kt_dist/m^2 > rtycut), then we've found
		// something interesting, so exit the loop
		if (parent1.m() < _mass_drop_threshold * this_jet.m() &&
		    parent1.kt_distance(parent2) > pow(_rtycut,2) * this_jet.m2()) {
		  break;
		} else {
		  // otherwise try a futher decomposition on the more massive jet
		  this_jet = parent1;
		}
	      }
	      
	      // look to see what we found
	      if (had_parents) {
		/*cout << "Found suitable pair of subjets: " << endl;
		  cout << "mass 1 " << parent1.m() << endl;
		  cout << "mass 2 " << parent2.m() << endl;
		  cout << "Total = " << endl;
		  cout << " " << this_jet.m() << endl << endl;*/
		// next we "filter" it, to remove UE & pileup contamination
		//
		// [there are two ways of doing this; here we directly use the
		// exsiting cluster sequence and find the exclusive subjets of
		// this_jet (i.e. work backwards within the cs starting from
		// this_jet); alternatively one can recluster just the
		// constituents of the jet]
		//
		// first get separation between the subjets (called Rbb -- assuming it's a Higgs!)
		double   R = 0.7;
		double   Rbb = sqrt(parent1.squared_distance(parent2));
		double   Rfilt = min(Rbb/2, 0.3); // somewhat arbitrary choice
		unsigned nfilt = 3;               // number of pieces we'll take
		//cout << "Subjet separation (Rbb) = " << Rbb << ", Rfilt = " << Rfilt << endl;
		double   dcut  = pow(Rfilt/R,2);  // for C/A get a view at Rfilt by
		// using a dcut=(Rfilt/R)^2
		vector<fastjet::PseudoJet> filt_subjets = sorted_by_pt(cs->exclusive_subjets(this_jet, dcut));
		FlavourRecombiner flav_recombiner; // for tracking flavour

		// now print out the filtered jets and reconstruct total 
		// at the same time
		//cout << "Filtered pieces are " << endl;
		//cout << " " << filt_subjets[0] << endl;
		fastjet::PseudoJet filtered_total = filt_subjets[0];
		for (unsigned i = 1; i < nfilt && i < filt_subjets.size(); i++) {
		  //cout << " " << filt_subjets[i] << endl;
		  flav_recombiner.plus_equal(filtered_total, filt_subjets[i]);
		}
		//cout << "Filtered total is " << endl;
		//cout << " " << filtered_total << endl;
		//cout << "filtered total mass: "<<filtered_total.m()<<endl;
		if(filtered_total.perp()>_jetPtCut) 
		  {
		    //cout<<"pt of the filtered jet: "<<filtered_total.perp()<<endl;
		    _invmass=filtered_total.m();
		  }
	      } else {
		cout << "Did not find suitable hard substructure in this event." << endl;
	      }
	      _HadronicVTree->Fill(); 
	    } 
	}
    }
    
    void finalize() {
      
      _HadronicVTree->Write(); 
      //_h_one->Write();
      //_h_two->Write();
      //_h_three->Write();
      //_h_four->Write();
      //_h_five->Write();
      //_h_one_mu->Write();
      //_h_two_mu->Write();
      //_h_three_mu->Write();
      //_h_four_mu->Write();
      //_h_five_mu->Write();
      _treeFile->Close();
      // No histos, so nothing to do!
    }
    //@}
    
  private:
    //TH1F* _h_one;
    //TH1F* _h_two;
    //TH1F* _h_three;
    //TH1F* _h_four; 
    //TH1F* _h_five;
    //TH1F* _h_one_mu;
    //TH1F* _h_two_mu;
    //TH1F* _h_three_mu;
    //TH1F* _h_four_mu;
    //TH1F* _h_five_mu;
    /*TFile* _treeFile;
    TTree* _HadronicVTree;
    float _electronet;
    float _electroneta;
    float _muonet;
    float _muoneta;
    int _jetmult;
    int _jetmultmu;
    float _jetetael;
    float _jetetamu;
    float _jetpt;
    float _jetptmu;
    float _nevt;
    float _electronETCut;
    float _electronETACut;
    float _jetPtCut;
    float _muonpTCut;
    float _muonETACut;
    float _jetETACut;
    float _recombinationDistance10kt;
    float _recombinationDistance21kt;
    float _recombinationDistance32kt;
    float _recombinationDistance10sis;
    float _recombinationDistance21sis;
    float _recombinationDistance32sis;
    float _ptW;
    float _Wcharge;
    */
    float _jet_phi_diff;
    float _jetPtCut;
    float _jetmult;
    float _lead_jet_pt;
    float _lead_jet_eta;
    float _lead_jet_mass;
    float _mass_drop_threshold;
    float _rtycut;
    TFile* _treeFile;
    TTree* _HadronicVTree;
    float _invmass;
  };
  
/// does the actual work for printing out a jet
/*ostream & operator<<(ostream & ostr, fastjet::PseudoJet & jet) {
  ostr << "pt, y, phi =" 
       << " " << setw(10) << jet.perp() 
       << " " << setw(6) <<  jet.rap()  
       << " " << setw(6) <<  jet.phi()  
       << ", mass = " << setw(10) << jet.m()
       << ", btag = " << jet.user_index();
  return ostr;
  }*/

  
  extern "C" {
    AnalysisBuilders getAnalysisBuilders() {
      AnalysisBuilders fns;
      fns["HADRONICV"] = Rivet::HadronicV::create;
      return fns;
    }
  }
  
  
}
