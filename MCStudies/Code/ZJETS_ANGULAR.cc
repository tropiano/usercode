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
      
      // Zee
      LeadingParticlesFinalState ZeeFS(FinalState(-2.5,2.5, 0.)); 
      ZeeFS.addParticleIdPair(ELECTRON);
      addProjection(ZeeFS, "ZeeFS");
      // Zmm
      LeadingParticlesFinalState ZmmFS(FinalState(-2.4,2.4, 0.)); 
      ZmmFS.addParticleIdPair(MUON);
      addProjection(ZmmFS, "ZmmFS");

      vector<pair<PdgId,PdgId> > vidsZ;
      vidsZ.push_back(make_pair(ELECTRON, POSITRON));
      vidsZ.push_back(make_pair(MUON, ANTIMUON));
      
      FinalState fsZ(-5.0,5.0);
      InvMassFinalState invfsZ(fsZ, vidsZ, 71*GeV, 111*GeV);
      addProjection(invfsZ, "INVFSZ");
  
      VetoedFinalState vfs(fs);
      vfs.addVetoOnThisFinalState(invfsZ);
      vfs.addVetoOnThisFinalState(ZeeFS);
      vfs.addVetoOnThisFinalState(ZmmFS);

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
	isFid1 = ((fabs(eta1)<1.4442)||((fabs(eta1)>1.566)&&(fabs(eta1)<2.5)));
	isFid2 = ((fabs(eta2)<1.4442)||((fabs(eta2)>1.566)&&(fabs(eta2)<2.5)));
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
    
    bool ApplyElectronCutsForZee(double pt1, double pt2, double eta1, double eta2){
      bool isFid1 = ((fabs(eta1)<1.4442)||((fabs(eta1)>1.566)&&(fabs(eta1)<2.5)));
      bool isFid2 = ((fabs(eta2)<1.4442)||((fabs(eta2)>1.566)&&(fabs(eta2)<2.5)));
      if( isFid1 && isFid2 && pt1>20 && pt2 >10) return true;
      else return false;
    }
    
    bool ApplyMuonCutsForZmm(double pt1, double pt2, double eta1, double eta2){
      if(fabs(eta1)<2.4  && fabs(eta2)<2.4 && pt1>20 && pt2>20) return true;
      else return false;
    }
    
    
    bool ApplyElectronCutsForWen(double pt1, double eta1){
      bool isFid1 = ((fabs(eta1)<1.4442)||((fabs(eta1)>1.566)&&(fabs(eta1)<2.5)));
      if( isFid1 && pt1>20 ) return true;
      return 0;
    }
 
   
    bool ApplyMuonCutsForWmn(double pt1, double eta1){
      bool isFid1 = ((fabs(eta1)<2.1));
      if( isFid1 && pt1>20) return true;
      return 0;
    }
    

    void Fill(AIDA::IHistogram1D*& _histJetMult, const double& weight, std::vector<FourMomentum>& finaljet_list){
      _histJetMult->fill(0, weight);
      for (size_t i=0 ; i<finaljet_list.size() ; ++i) {
        if (i==6) break;
        _histJetMult->fill(i+1, weight);  // inclusive
      }
    }  
    
    void FillNoverNm1(AIDA::IHistogram1D*& _histJetMult,AIDA::IDataPointSet* _histNoverNm1){
      std::vector<double> y, yerr;
      for (int i=0; i<_histJetMult->axis().bins()-1; i++) {
        double val = 0.;
        double err = 0.;
        if (!fuzzyEquals(_histJetMult->binHeight(i), 0)) {
          val = _histJetMult->binHeight(i+1) / _histJetMult->binHeight(i);
          err = val * sqrt(  pow(_histJetMult->binError(i+1)/_histJetMult->binHeight(i+1), 2)
                           + pow(_histJetMult->binError(i)  /_histJetMult->binHeight(i)  , 2) );
        }
        y.push_back(val);
        yerr.push_back(err);
      }
      _histNoverNm1->setCoordinate(1, y, yerr);
    }    
    void FillNoverN0(AIDA::IHistogram1D*& _histJetMult,AIDA::IDataPointSet* _histNoverN0){
      std::vector<double> y, yerr;
      for (int i=0; i<_histJetMult->axis().bins()-1; i++) {
        double val = 0.;
        double err = 0.;
        if (!fuzzyEquals(_histJetMult->binHeight(i), 0)) {
          val = _histJetMult->binHeight(i+1) / _histJetMult->binHeight(0);
          err = val * sqrt(  pow(_histJetMult->binError(i+1)/_histJetMult->binHeight(i+1), 2)
                           + pow(_histJetMult->binError(0)  /_histJetMult->binHeight(0)  , 2) );
        }
        y.push_back(val);
        yerr.push_back(err);
      }
      _histNoverN0->setCoordinate(1, y, yerr);
    }    

    
   void FillChargeAssymHistogramSet(  AIDA::IHistogram1D*& _histJetMult1,AIDA::IHistogram1D*& _histJetMult2, AIDA::IDataPointSet* _histJetMultRatio12 ){
      std::vector<double> yval, yerr;
      for (int i = 0; i < 4; ++i) {
        std::vector<double> xval; xval.push_back(i);
        std::vector<double> xerr; xerr.push_back(.5);
        double ratio = 0;
        double err = 0.;
        double num = _histJetMult1->binHeight(i)-_histJetMult2->binHeight(i);
	double den = _histJetMult1->binHeight(i)+_histJetMult2->binHeight(i);
	double errNum = 0;
	errNum = std::pow(_histJetMult1->binError(i),2)+std::pow(_histJetMult2->binError(i),2);
	double errDen = 0;
	errDen = std::pow(_histJetMult1->binError(i),2)+std::pow(_histJetMult2->binError(i),2); 

        if (den)ratio = num/den;

        if(num)
	  errNum = errNum/(num*num); 
        if(den) 
	  errDen = errDen/(den*den);

        err = std::sqrt(errDen+errNum);
	if(!(err==err))err=0;
        yval.push_back(ratio);
        yerr.push_back(ratio*err);
        }
        _histJetMultRatio12->setCoordinate(1,yval,yerr);
      }
    


    
    void analyze(const Event& event) {
      //some flag definitions.
      bool isZmm =false;
      bool isZee =false;
    
      const double weight = event.weight();
      
      //define the Z candidate
      const InvMassFinalState& invMassFinalStateZ = applyProjection<InvMassFinalState>(event, "INVFSZ");
      ////const InvMassFinalState& invMassFinalStateW = applyProjection<InvMassFinalState>(event, "INVFSW");
      const LeadingParticlesFinalState& ZeeFS = applyProjection<LeadingParticlesFinalState>(event, "ZeeFS");
      const LeadingParticlesFinalState& ZmmFS = applyProjection<LeadingParticlesFinalState>(event, "ZmmFS");
      
      bool isW(false); bool isZ(false);
      
      isZ= (ZeeFS.particles().size()>1 && ZmmFS.empty()) || (ZmmFS.particles().size()>1 && ZeeFS.empty()); 
      //isW=(!WminusenuFS.empty() || !WplusenuFS.empty() || !WminusmunuFS.empty() || !WplusmunuFS.empty());
      
      //isW  = (invMassFinalStateZ.empty() && !(invMassFinalStateW.empty()));
      //isZ  = !(invMassFinalStateZ.empty());
      //&& invMassFinalStateW.empty());
      
      const ParticleVector& ZeeDaus  = ZeeFS.particlesByPt();
      const ParticleVector& ZmmDaus = ZmmFS.particlesByPt();

      //const ParticleVector&  ZDecayProducts =  invMassFinalStateZ.particles();
      
      //if (ZDecayProducts.size() < 2 ) vetoEvent; 
      //&& WDecayProducts.size() <2) vetoEvent;
      
      double pt1=-9999.,  pt2=-9999.;
      double phi1=-9999., phi2=-9999.;
      double eta1=-9999., eta2=-9999.;
      
      if(isZ){
        cout<<"found a Z"<<endl;
	if(ZeeDaus.size()==2 && ZmmDaus.size()<2){
	  isZee = ApplyZAcceptance(ZeeFS,"electron");
	  GetPtEtaPhi(ZeeDaus[0],pt1,eta1,phi1);
	  GetPtEtaPhi(ZeeDaus[1],pt2,eta2,phi2);
	}
	if(ZmmDaus.size()==2 && ZeeDaus.size()<2){
	  isZmm = ApplyZAcceptance(ZmmFS,"muon");
	}
      }

      /*if(isZ){
	pt1  = ZDecayProducts[0].momentum().pT();
	pt2  = ZDecayProducts[1].momentum().pT();
	eta1 = ZDecayProducts[0].momentum().eta();
	eta2 = ZDecayProducts[1].momentum().eta();
	phi1 = ZDecayProducts[0].momentum().phi();
	phi2 = ZDecayProducts[1].momentum().phi();
      }
      
      isZmm = isZ && ((fabs(ZDecayProducts[0].pdgId()) == 13) && (fabs(ZDecayProducts[1].pdgId()) == 13));
      isZee = isZ && ((fabs(ZDecayProducts[0].pdgId()) == 11) && (fabs(ZDecayProducts[1].pdgId()) == 11));*/
      

      if(!(isZmm||isZee)) vetoEvent;
      
      /*bool passBosonConditions = false;
      //apply kinematical cuts on pt and eta
      if(isZmm)passBosonConditions = ApplyMuonCutsForZmm(pt1,pt2,eta1,eta2);
      if(isZee)passBosonConditions = ApplyElectronCutsForZee(pt1,pt2,eta1,eta2);
      
      
      if(!passBosonConditions) vetoEvent;*/
      
      //Obtain the jets.
      vector<FourMomentum> finaljet_list;
      foreach (const Jet& j, applyProjection<FastJets>(event, "ak4Jets").jetsByPt(30.0*GeV)) {
	const double jeta = j.momentum().eta();
	const double jphi = j.momentum().phi();
	const double jpt  = j.momentum().pT();
	//kinematic cuts on the jets 
	if (fabs(jeta) < 2.5){
	  if(jpt>50){
	    if(isZee){
	      if (deltaR(pt1, phi1, jeta, jphi) > 0.3 && deltaR(pt2, phi2, jeta, jphi) > 0.3)
		finaljet_list.push_back(j.momentum());
	      continue;
	    }
	    else if(isZmm){
              //require minimum separation  between jets
	      if (deltaR(pt1, phi1, jeta, jphi) > 0.4 && deltaR(pt2, phi2, jeta, jphi) > 0.4)
                finaljet_list.push_back(j.momentum());
              continue;
            }
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

