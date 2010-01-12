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

namespace Rivet {
  
  class TTbar : public Analysis {
    
  public:
    
    /// Default constructor
    TTbar(): _electronETCut(30.0 *GeV),_electronETACut(2.5),_jetPtCut(30.0 *GeV),_muonpTCut(20.0 *GeV),_muonETACut(2.1),_jetETACut(2.4)
    {
      //W->lv (leptonic) final state
      const FinalState lfs;
      addProjection(lfs, "LFS");
      //final state without the electrons from W decay (veto) for jet clustering
      VetoedFinalState vfs(lfs);
      vfs.addVetoPairDetail(ELECTRON, 20.0*GeV, MAXDOUBLE);
      vfs.addVetoPairDetail(NU_E, 20.0*GeV, MAXDOUBLE);
      vfs.addVetoPairDetail(MUON, 20.0*GeV, MAXDOUBLE);
      vfs.addVetoPairDetail(NU_MU, 20.0*GeV, MAXDOUBLE);
      addProjection(vfs, "VFS");
      addProjection(FastJets(vfs, FastJets::SISCONE, 0.5, 30.0), "Jets");
    }
    
    /// Factory method
    static Analysis* create() { 
      return new TTbar(); 
    }
    
    /// Return the name of this analysis
    string name() const {
      return "TTbar";
    }
    
    /// Get the SPIRES ID code
    string spiresId() const {
      return "NONE";
    }
    
    /// Get a description of the analysis.
    string description() const {
      return "TTbar analysis";
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
      return "An analysis that reproduces default cuts of top leptonic analyses. Jet multiplicity and kinematical variables.";
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
    
    //@name Analysis methods
    //@{
    void init() {
      //create and open the root file
      _treeFile = new TFile("top.root", "recreate");
      //create the tree
      _TTbarTree = new TTree("TTbar Tree", "Tree for TTbar analysis");
      //event number
      _TTbarTree->Branch("nevt", &_nevt, "nevt/I");
      _TTbarTree->Branch("jetmult", &_jetmult, "jetmult/I");
      _TTbarTree->Branch("jetpt", &_jetpt, "jetpt/F");
      _TTbarTree->Branch("electronet", &_electronet, "electronet/F");
      _TTbarTree->Branch("electroneta", &_electroneta, "electroneta/F");
      _TTbarTree->Branch("jetetael", &_jetetael, "jetetael/F");
      _TTbarTree->Branch("jetmultmu", &_jetmultmu, "jetmultmu/I");
      _TTbarTree->Branch("jetptmu", &_jetptmu, "jetptmu/F");
      _TTbarTree->Branch("muonet", &_muonet, "muonet/F");
      _TTbarTree->Branch("muoneta", &_muoneta, "muoneta/F");
      _TTbarTree->Branch("jetetamu", &_jetetamu, "jetetamu/F");
      //electron histograms 
      //_h_one = new TH1F("JetMult", "Jet multiplicity", 
      //		10, 0, 10);
      //_h_two = new TH1F("JetPt", "Highest jet Pt", 
      //		100, 0, 300);
      //_h_three = new TH1F("ElectronEt", "Electron Et", 
      //		  60 , 0, 300);
      //_h_four = new TH1F("ElectronEta", "Electron Eta", 
      //		 60, -3, 3);
      //_h_five =new TH1F("JetEtaEl", "Jet Eta", 
      //		60, -3, 3);
      //muon histograms
      //_h_one_mu =new TH1F("JetMultMu", "Jet multiplicity (muon decay)", 
      //		  10, 0, 10);
      //_h_two_mu = new TH1F("JetPtMu", "Highest jet Pt (muon decay)", 
      //		   100, 0, 300);
      //_h_three_mu =new TH1F("MuonEt", "Muon pT", 
      //	    60, 0, 300); 
      //_h_four_mu =new TH1F("MuonEta", "Muon Eta", 
      //		   60, -3, 3);
      //_h_five_mu =new TH1F("JetEtaMu", "Jet Eta", 
      //		   60, -3, 3);
    }
    
    void analyze(const Event& event) {
      const FinalState& lfs = applyProjection<FinalState>(event, "LFS");
      const FinalState& vfs = applyProjection<FinalState>(event, "VFS");
      const ParticleVector&  WDecayProducts =  lfs.particles();  
      double leptonP=-100;
      double leptonEta=-100;
      bool gotLepton = false;
      FourMomentum neutrinoP;
      bool gotNeutrino = false;
      double HighJetPt=-100;
      double HighJetEta=-100;
      ParticleVector leptons;
      
      for(ParticleVector::const_iterator p = WDecayProducts.begin();
	  p !=  WDecayProducts.end(); ++p) {
	if( (p->momentum().Et() > _electronETCut &&                         //apply electron cuts
	     fabs(p->momentum().pseudorapidity()) < _electronETACut && 
	     abs(p->pdgId()) == ELECTRON) || 
	    (p->momentum().pT() > _muonpTCut &&                               //apply muon cuts
	     fabs(p->momentum().pseudorapidity()) < _muonETACut && 
	     abs(p->pdgId()) == MUON) )  {
	  leptonP = p->momentum().Et();
	  //_electronet=p->momentum().Et();
	  leptonEta = p->momentum().pseudorapidity();
	  gotLepton = true;
	  leptons.push_back(*p);
	}
      }
      if (!gotLepton || leptons.size()!=1) {
	//cout<<"JETS"<<std::endl;
	vetoEvent;
      }
      //electron decay channel 
      if(abs(leptons.front().pdgId())==ELECTRON)
	{
	  //cout<<"ELECTRON"<<std::endl;
	  _electronet=leptonP;
	  _electroneta=leptonEta;
	  //_h_three->Fill(leptonP);
	  //_h_four->Fill(leptonEta);
	  const FastJets &jetProj = applyProjection<FastJets>(event, "Jets");
	  Jets theJets = jetProj.jetsByPt(30.);
	  size_t njets = 0;
	  //cout<<"number of jets in the event: "<<theJets.size()<<endl;
	  if(theJets.size()) 
	    {
	      //sort(theJets.begin(), theJets.end(), Particle::byETDescending());
	      //cout<<"number of jets in the event: "<<theJets.size()<<endl
	      //  <<"highest pT: "<<theJets.begin()->momentum().pT()<<endl
	      //  <<"lowest pT: "<<theJets.back().momentum().pT()<<endl;
	      //HighJetPt=theJets.begin()->momentum().pT();
	      //HighJetEta=theJets.begin()->momentum().rapidity();
	      //Jets foundJets;
	      for (Jets::const_iterator jIt =theJets.begin(); 
		   jIt != theJets.end(); ++jIt){
		if(jIt->momentum().Et() > _jetPtCut &&
		   fabs(jIt->momentum().rapidity()) < _jetETACut)
		  {
		    HighJetPt=jIt->momentum().pT();
		    HighJetEta=jIt->momentum().rapidity();
		    njets++; 
		  }
	      }
	    }
	  _jetpt=HighJetPt;
	  _jetmult=njets;
	  _jetetael=HighJetEta;
	  //_h_two->Fill(HighJetPt);
	  //_h_one->Fill(njets);
	  //_h_five->Fill(HighJetEta);
	}
      //muon decay channel
      else 
	{
	  //cout<<"MUON"<<std::endl;
	  _muonet=leptonP;
	  _muoneta=leptonEta;
	  //_h_three_mu->Fill(leptonP);
	  //_h_four_mu->Fill(leptonEta);
	  //cout<<"filled lepton histos"<<endl;
	  const FastJets &jetProj = applyProjection<FastJets>(event, "Jets");
	  //cout<<"applied jet projection"<<endl;
	  Jets theJets = jetProj.jetsByPt(30.);
	  //cout<<"sorted jets by pt with cut on pt"<<endl;
	  size_t njets = 0;
	  //cout<<"number of jets in the event: "<<theJets.size()<<endl;
	  if(theJets.size())
	    {
	      //sort(theJets.begin(), theJets.end(), Particle::byETDescending());
	      //  cout<<"number of jets in the event: "<<theJets.size()<<endl
	      //  <<"highest pT: "<<theJets.begin()->momentum().pT()<<endl
	      //  <<"lowest pT: "<<theJets.back().momentum().pT()<<endl;
	      //HighJetPt=theJets.begin()->momentum().pT();
	      //HighJetEta=theJets.begin()->momentum().rapidity(); 
	      for (Jets::const_iterator jIt =theJets.begin(); 
		   jIt != theJets.end(); ++jIt){
		//if (fabs(jIt->momentum().rapidity()) < _jetETA) {
		// Store all jets with ET > 20. for differential histograms 
		//if (jIt->momentum().Et() > _jetEtCutA) {
		//foundJets.push_back(*jIt);
		//}
		// Count number of jets with ET > 25. for multiplicity histograms
		//if (jIt->momentum().Et() > _jetEtCutB) {
		if(jIt->momentum().pT() > _jetPtCut && 
		   fabs(jIt->momentum().rapidity()) < _jetETACut)
		  {
		    HighJetPt=jIt->momentum().pT();
		    HighJetEta=jIt->momentum().rapidity(); 
		    njets++; 
		  }
	      }
	      
	    }
	  _jetptmu=HighJetPt;
	  _jetmultmu=njets;
	  _jetetamu=HighJetEta;

	  //_h_two_mu->Fill(HighJetPt);
	  //_h_one_mu->Fill(njets);
	  //_h_five_mu->Fill(HighJetEta);
	}
      //fill the tree
      _TTbarTree->Fill(); 
    }
    
    void finalize() {
      
      _TTbarTree->Write(); 
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
      //_treeFile->close();
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
    TFile* _treeFile;
    TTree* _TTbarTree;
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
  };
  
  
  extern "C" {
    AnalysisBuilders getAnalysisBuilders() {
      AnalysisBuilders fns;
      fns["TTBAR"] = Rivet::TTbar::create;
      return fns;
    }
  }
  
  
}
