// -*- C++ -*-
// AUTHOR:  Antonio Tropiano (antonio.tropiano@cern.ch)

#include "Rivet/Analysis.hh"
#include "Rivet/RivetAIDA.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/InvMassFinalState.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"


namespace Rivet {
  
  
  class ZJETS_ANGULAR: public Analysis {
  public:
    
    ZJETS_ANGULAR()
      : Analysis("ZJETS_ANGULAR")
    {
      setBeams(PROTON, PROTON);
      setNeedsCrossSection(true);
    }
    
    
    /// Book histograms and initialise projections before the run
    void init() {
      
      const FinalState fs(-5.0,5.0);
      addProjection(fs, "FS");
      
      // Zee (create electron pairs to veto them afterwards)
      LeadingParticlesFinalState ZeeFS(FinalState(-2.4, 2.4, 20.)); 
      ZeeFS.addParticleIdPair(ELECTRON);
      addProjection(ZeeFS, "ZeeFS");

      // Zmm (create muon pairs to veto them afterwards)
      LeadingParticlesFinalState ZmmFS(FinalState(-2.4, 2.4, 20.)); 
      ZmmFS.addParticleIdPair(MUON);
      addProjection(ZmmFS, "ZmmFS");
      
      //create lepton pairs without pT and eta cut  
      vector<pair<PdgId,PdgId> > vidsZ;
      vidsZ.push_back(make_pair(ELECTRON, POSITRON));
      vidsZ.push_back(make_pair(MUON, ANTIMUON));
      
      //final state with leptons with inv mass around the Z peak
      FinalState fsZ(-5.0,5.0);
      InvMassFinalState invfsZ(fsZ, vidsZ, 71*GeV, 111*GeV);
      addProjection(invfsZ, "INVFSZ");

      //create a final state without electrons/muons with high pT and from a Z  
      VetoedFinalState vfs(fs);
      vfs.addVetoOnThisFinalState(invfsZ);
      vfs.addVetoOnThisFinalState(ZeeFS);
      vfs.addVetoOnThisFinalState(ZmmFS);

      //build the jets without leptons in it
      addProjection(vfs, "VFS");
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.5), "ak5Jets");
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.4), "ak4Jets");      

      //data points
      //histograms 
      _histMll            = bookHistogram1D("Mll", 60, 50., 130.);
      _histNjets          = bookHistogram1D("Njets", 5, -0.5, 4.5);
      _histPtll           = bookHistogram1D("Ptll", 50, 0., 1000.);
      _histMjj            = bookHistogram1D("Mjj", 70, 0., 1400.);   
      _histPtJet1         = bookHistogram1D("PtJet1", 45, 50., 500.);
      _histPtJet2         = bookHistogram1D("PtJet2", 45, 50., 500.);
      _histPtJet3         = bookHistogram1D("PtJet3", 45, 50., 500.);
      _histPtJet4         = bookHistogram1D("PtJet4", 45, 50., 500.);
      _histDeltaPhiZJ1    = bookHistogram1D("DeltaPhiZJ1", 32, 0., 3.15);
      _histDeltaPhiZJ2    = bookHistogram1D("DeltaPhiZJ2", 32, 0., 3.15);
      _histDeltaPhiZJ3    = bookHistogram1D("DeltaPhiZJ3", 32, 0., 3.15);
      _histDeltaPhiZJ1_2  = bookHistogram1D("DeltaPhiZJ1_2", 32, 0., 3.15);
      _histDeltaPhiZJ1_3  = bookHistogram1D("DeltaPhiZJ1_3", 32, 0., 3.15);
      _histDeltaPhiZJ2_3  = bookHistogram1D("DeltaPhiZJ2_3", 32, 0., 3.15);
      _histDeltaPhiJ1J2_2 = bookHistogram1D("DeltaPhiJ1J2_2", 32, 0., 3.15);
      _histDeltaPhiJ1J2_3 = bookHistogram1D("DeltaPhiJ1J2_3", 32, 0., 3.15);
      _histSumDeltaPhi    = bookHistogram1D("SumDeltaPhi", 32, 0, 6.30);
      
    } 
    
    void GetPtEtaPhi(Particle p1, double& pt, double& eta,double& phi){
      pt = p1.momentum().pT();
      eta = p1.momentum().eta();
      phi = p1.momentum().phi();
      return;
    }
   
    bool ApplyZAcceptance(const LeadingParticlesFinalState& zFS, std::string lepton){
      const ParticleVector& Zdaughters = zFS.particlesByPt();
      double phi1 = -9999., phi2 = -9999.;
      double pt1 = -9999., pt2 = -9999.;
      double eta1 = -9999., eta2 = -9999.;
      GetPtEtaPhi(Zdaughters[0],pt1,eta1,phi1);
      GetPtEtaPhi(Zdaughters[1],pt2,eta2,phi2);
      bool isFid1 = false;
      bool isFid2 = false;
      if(lepton=="electron"){
	isFid1 = //((fabs(eta1)<1.4442)||((fabs(eta1)>1.566)&&
	  (fabs(eta1)<2.4);
	isFid2 = //((fabs(eta2)<1.4442)||((fabs(eta2)>1.566)&&
	  (fabs(eta2)<2.4);
      }
      if(lepton=="muon"){
	isFid1 = ((fabs(eta1)<2.4));
	isFid2 = ((fabs(eta2)<2.4));
      }
      
      if( isFid1 && isFid2 && pt1>20 && pt2 >20){
	const FourMomentum pmom = Zdaughters[0].momentum() + Zdaughters[1].momentum();
	double mass = sqrt(pmom.invariant());
	if (inRange(mass/GeV, 71.0, 111.0))
	  return true;
	else return false;
      }
      else return false;
    }
    
    
    void analyze(const Event& event) {
      //some flag definitions.
      bool isZmm =false;
      bool isZee =false;
    
      const double weight = event.weight();
      
      //define the Z candidate 
      const InvMassFinalState& invMassFinalStateZ = applyProjection<InvMassFinalState>(event, "INVFSZ");
      const LeadingParticlesFinalState& ZeeFS     = applyProjection<LeadingParticlesFinalState>(event, "ZeeFS");
      const LeadingParticlesFinalState& ZmmFS     = applyProjection<LeadingParticlesFinalState>(event, "ZmmFS");
      
      bool isZ(false);
      
      isZ = ((ZeeFS.particles().size()>1) || (ZmmFS.particles().size()>1)); 
      
      const ParticleVector& ZeeDaus = ZeeFS.particlesByPt();
      const ParticleVector& ZmmDaus = ZmmFS.particlesByPt();

      double pt1=-9999.,  pt2=-9999.;
      double phi1=-9999., phi2=-9999.;
      double eta1=-9999., eta2=-9999.;
      
      if(isZ){
        cout<<"found a Z"<<endl;
	if(ZeeDaus.size()>1 && ZmmDaus.size()<2){
	  isZee = ApplyZAcceptance(ZeeFS,"electron");
	  GetPtEtaPhi(ZeeDaus[0],pt1,eta1,phi1);
	  GetPtEtaPhi(ZeeDaus[1],pt2,eta2,phi2);
	}
	if(ZmmDaus.size()>1 && ZeeDaus.size()<2){
	  isZmm = ApplyZAcceptance(ZmmFS,"muon");
	  GetPtEtaPhi(ZmmDaus[0],pt1,eta1,phi1);
          GetPtEtaPhi(ZmmDaus[1],pt2,eta2,phi2);
	}
      }

      if(!(isZmm||isZee)) vetoEvent;
      
      //Obtain the jets.
      vector<FourMomentum> finaljet_list;
      foreach (const Jet& j, applyProjection<FastJets>(event, "ak5Jets").jetsByPt(30.0*GeV)) {
	const double jeta = j.momentum().eta();
	const double jphi = j.momentum().phi();
	const double jpt  = j.momentum().pT();
	//kinematic cuts on the jets 
	if (fabs(jeta) < 2.5){
	  if(jpt>30.){
	    //require minimum separation  between jets
	    if (deltaR(pt1, phi1, jeta, jphi) > 0.4 && deltaR(pt2, phi2, jeta, jphi) > 0.4)
	      finaljet_list.push_back(j.momentum());
	    continue;
	  }
	}
      }
      
      double Njets = finaljet_list.size();
      
      //FourMomentum pZ = ZDecayProducts[0].momentum() + ZDecayProducts[1].momentum();
      FourMomentum pZ;// = 0.;
      
      if(isZee) pZ = ZeeDaus[0].momentum() + ZeeDaus[1].momentum();                                                                       
      else if (isZmm) pZ = ZmmDaus[0].momentum() + ZmmDaus[1].momentum();
      
      //double Mll  = pZ.mass();
      //_histMll->fill(Mll, weight);
      
      if(Njets){
	//double Njets = finaljet_list.size();
	cout<<"No of jets: "<<Njets<<endl;
	cout<<"pt 1st jet: "<<finaljet_list[0].pT()<<endl;
	//FourMomentum pZ = ZDecayProducts[0].momentum() + ZDecayProducts[1].momentum();
	double Mll  = pZ.mass();
	double Ptll = pZ.pT();
	double PhiZ = pZ.phi();
	double PtJet1  = finaljet_list[0].pT();
	double PhiJet1 = finaljet_list[0].phi(); 
	_histMll->fill(Mll, weight);
	_histNjets->fill(Njets, weight);
	_histPtll->fill(Ptll, weight);
	_histPtJet1->fill(PtJet1, weight);
	_histDeltaPhiZJ1->fill(deltaPhi(PhiJet1,PhiZ), weight);
	
	if(Njets>1){
	  FourMomentum pJ1J2 = finaljet_list[0] + finaljet_list[1];
	  double Mjj = pJ1J2.mass();
	  double PtJet2  = finaljet_list[1].pT();
	  double PhiJet2 = finaljet_list[1].phi();
	  _histMjj->fill(Mjj);
	  _histPtJet2->fill(PtJet2, weight);
	  _histDeltaPhiZJ2->fill(deltaPhi(PhiJet2,PhiZ), weight);
	  _histDeltaPhiZJ1_2->fill(deltaPhi(PhiJet1,PhiZ), weight);
	  _histDeltaPhiJ1J2_2->fill(deltaPhi(PhiJet1,PhiJet2), weight);
	  
	  if(Njets>2){
	    double PtJet3  = finaljet_list[2].pT();
	    double PhiJet3 = finaljet_list[2].phi();
	    double SumDeltaPhi = deltaPhi(PhiJet1,PhiJet2) + deltaPhi(PhiJet1,PhiJet3) + deltaPhi(PhiJet2,PhiJet3);
	    _histSumDeltaPhi->fill(SumDeltaPhi, weight);
	    _histPtJet3->fill(PtJet3, weight);
	    _histDeltaPhiZJ1_3->fill(deltaPhi(PhiJet1,PhiZ), weight);
	    _histDeltaPhiZJ2_3->fill(deltaPhi(PhiJet2,PhiZ), weight);
	    _histDeltaPhiJ1J2_3->fill(deltaPhi(PhiJet1,PhiJet2), weight);
	    _histDeltaPhiZJ3->fill(deltaPhi(PhiZ,PhiJet3), weight);
	    
	    if(Njets>3){
	      double PtJet4  = finaljet_list[2].pT();
	      //double SumDeltaPhi = deltaPhi(PhiJet1,PhiJet2) + deltaPhi(PhiJet1,PhiJet3) + deltaPhi(PhiJet3,PhiJet2);
	      _histPtJet4->fill(PtJet4, weight);
	      //(_histSumDeltaPhi->fill(SumDeltaPhi, weight);
	    }
	  }
	}
      }
    }
    
    /// Normalise histograms etc., after the run
    void finalize() {
    }
    
  private:
    AIDA::IHistogram1D* _histMll;            
    AIDA::IHistogram1D* _histNjets;          
    AIDA::IHistogram1D* _histPtll;           
    AIDA::IHistogram1D* _histMjj;            
    AIDA::IHistogram1D* _histPtJet1;         
    AIDA::IHistogram1D* _histPtJet2;         
    AIDA::IHistogram1D* _histPtJet3;         
    AIDA::IHistogram1D* _histPtJet4;         
    AIDA::IHistogram1D* _histDeltaPhiZJ1;    
    AIDA::IHistogram1D* _histDeltaPhiZJ2;    
    AIDA::IHistogram1D* _histDeltaPhiZJ3;    
    AIDA::IHistogram1D* _histDeltaPhiZJ1_2;  
    AIDA::IHistogram1D* _histDeltaPhiZJ1_3;  
    AIDA::IHistogram1D* _histDeltaPhiZJ2_3;  
    AIDA::IHistogram1D* _histDeltaPhiJ1J2_2; 
    AIDA::IHistogram1D* _histDeltaPhiJ1J2_3; 
    AIDA::IHistogram1D* _histSumDeltaPhi;    
    
  };
  
  AnalysisBuilder<ZJETS_ANGULAR> plugin_ZJETS_ANGULAR;
  
}

