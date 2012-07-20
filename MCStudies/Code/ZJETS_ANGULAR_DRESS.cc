// -*- C++ -*-
// AUTHOR:  Antonio Tropiano (antonio.tropiano@cern.ch)
#include "Rivet/Analysis.hh"
#include "Rivet/RivetAIDA.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/InvMassFinalState.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"
#include "Rivet/Projections/LeptonClusters.hh"

namespace Rivet {
  
  
  class ZJETS_ANGULAR_DRESS_NEW: public Analysis {

  public:
    
    ZJETS_ANGULAR_DRESS_NEW()
      : Analysis("ZJETS_ANGULAR_DRESS_NEW")
    {
      setBeams(PROTON, PROTON);
      setNeedsCrossSection(true);
    }
    
    
    /// Book histograms and initialise projections before the run
    void init() {
      
      //define a generic final state
      const FinalState fs(-5.0,5.0);
      addProjection(fs, "FS");
      //define dressed leptons 
      const FinalState allleptons;
      
      //allleptons.acceptIdPair(ELECTRON);
      //allleptons.acceptIdPair(MUON);
      
      //define the eta range in which one looks for leptons
      std::vector<std::pair<double, double> > etaRanges;
      etaRanges.push_back(make_pair(-2.5, 2.5));
      
      //photon are clustered in DR<0.1. Only leptons with pt>20 are kept
      LeptonClusters leptons(fs, allleptons, 0.1, true, true, 
			     etaRanges, 20.0*GeV);
      addProjection(leptons, "leptons");
      
      //cluster the jets with all the particles (cleaning is done afterwards) 
      addProjection(FastJets(fs, FastJets::ANTIKT, 0.5), "ak5Jets_NoVeto");
      
      //data points
      //histograms 
      _histMll            = bookHistogram1D("Mll",        60, 50., 130.);
      _histNjets          = bookHistogram1D("Njets",       5, -0.5, 4.5);
      _histPtll           = bookHistogram1D("Ptll", 50, 0., 1000.);
      _histMjj            = bookHistogram1D("Mjj", 70, 0., 1400.);   
      _histPtJet1         = bookHistogram1D("PtJet1", 45, 50., 500.);
      _histPtJet2         = bookHistogram1D("PtJet2", 45, 50., 500.);
      _histPtJet3         = bookHistogram1D("PtJet3", 45, 50., 500.);
      _histPtJet4         = bookHistogram1D("PtJet4", 45, 50., 500.);
      //DeltaPhi(Z,J1) for >=1 jet events
      _histDeltaPhiZJ1_1  = bookHistogram1D("DeltaPhiZJ1_1", 30, 0., 3.14160);                                                                      
      //DeltaPhi(Z,J1) for >=2 jets events                                                                                                                
      _histDeltaPhiZJ1_2  = bookHistogram1D("DeltaPhiZJ1_2", 30, 0., 3.14160);
      //DeltaPhi(Z,J1) for >=3 jets events
      _histDeltaPhiZJ1_3  = bookHistogram1D("DeltaPhiZJ1_3", 30, 0., 3.14160);
      //DeltaPhi(Z,J2) for >=3 jets events                                                                                                                  
      _histDeltaPhiZJ2_3  = bookHistogram1D("DeltaPhiZJ2_3", 30, 0., 3.14160);
      //DeltaPhi(Z,J3) for >=3 jets events
      _histDeltaPhiZJ3_3  = bookHistogram1D("DeltaPhiZJ3_3", 30, 0., 3.14160);
      //DeltaPhi(Ji,Jj) for >=3 jets events
      _histDeltaPhiJ1J2_3 = bookHistogram1D("DeltaPhiJ1J2_2", 30, 0., 3.14160);
      _histDeltaPhiJ2J3_3 = bookHistogram1D("DeltaPhiJ2J3_3", 30, 0., 3.14160);
      _histDeltaPhiJ1J3_3 = bookHistogram1D("DeltaPhiJ1J3_3", 30, 0., 3.14160);
      
      
    }
    
    void analyze(const Event& event) {
      //some flag definitions.
      bool isZ   = false;

      const double weight = event.weight();
      //create the lepton clusters.
      const LeptonClusters& leptonClusters = applyProjection<LeptonClusters>(event, "leptons");
      //extract the leptons from the clusters.   
      const ParticleVector& leptons = leptonClusters.particlesByPt(); 
      //look for at least 2 leptons
      if(!(leptons.size()>1)) vetoEvent;
      //look if the 2 most energetic leptons pass the pT, eta cuts and Mll is in the mass window (71, 111).  
      isZ = ApplyZAcceptance(leptons); 
      //initialize the pT, eta, phi of the leptons 
      double pt1=-9999.,  pt2=-9999.;
      double phi1=-9999., phi2=-9999.;
      double eta1=-9999., eta2=-9999.;
      
      //select events that pass the acceptance cuts 
      if(isZ){
        cout<<"found a Z"<<endl;
	FourMomentum lepton1=leptons[0].momentum();
	FourMomentum lepton2=leptons[1].momentum();
	pt1  = lepton1.pT();
	pt2  = lepton2.pT();
	eta1 = lepton1.eta();
	eta2 = lepton2.eta();
	phi1 = lepton1.phi();
        phi2 = lepton2.phi();

      }

      
      else vetoEvent; 
      
      //Obtain the jets.
      vector<FourMomentum> finaljet_list;
      foreach (const Jet& j, applyProjection<FastJets>(event, "ak5Jets_NoVeto").jetsByPt(30.0*GeV)) {
	const double jeta = j.momentum().eta();
	const double jphi = j.momentum().phi();
	const double jpt  = j.momentum().pT();
	//kinematic cuts on the jets 
	if (fabs(jeta) < 2.5){
	  if(jpt>50){
	    //require minimum separation between jets and leptons
	    if (deltaR(eta1, phi1, jeta, jphi)>0.4 
		&& deltaR(eta2, phi2, jeta, jphi)>0.4)
	      finaljet_list.push_back(j.momentum());
	    continue;
	  }
	}
      }
      
      double Njets = finaljet_list.size();
      
      //FourMomentum pZ = ZDecayProducts[0].momentum() + ZDecayProducts[1].momentum();
      FourMomentum pZ = leptons[0].momentum() + leptons[1].momentum();
      
      if(Njets){
	//double Njets = finaljet_list.size();
	//cout<<"No of jets: "    <<Njets<<endl;
	//cout<<"pt 1st jet: "    <<finaljet_list[0].pT()<<endl;
	//cout<<"pt 1st lepton: " <<leptons[0].momentum().pT()<<endl;
        //cout<<"pt 2nd lepton: " <<leptons[1].momentum().pT()<<endl;
        //cout<<"1st jet eta,phi: ("<<finaljet_list[0].eta()<<     ","<<finaljet_list[0].phi()     <<endl;
	//cout<<"1st lep eta,phi: ("<<leptons[0].momentum().eta()<<","<<leptons[0].momentum().phi()<<endl;
	//cout<<"2nd lep eta,phi: ("<<leptons[1].momentum().eta()<<","<<leptons[1].momentum().phi()<<endl;

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
	_histDeltaPhiZJ1_1->fill(deltaPhi(PhiJet1,PhiZ), weight);
	
	if(Njets>1){
	  FourMomentum pJ1J2 = finaljet_list[0] + finaljet_list[1];
	  double Mjj = pJ1J2.mass();
	  double PtJet2  = finaljet_list[1].pT();
	  double PhiJet2 = finaljet_list[1].phi();
	  _histMjj->fill(Mjj);
	  _histPtJet2->fill(PtJet2, weight);
	  _histDeltaPhiZJ1_2->fill(deltaPhi(PhiJet1,PhiZ), weight);
	  
	  if(Njets>2){
	    double PtJet3  = finaljet_list[2].pT();
	    double PhiJet3 = finaljet_list[2].phi();
	    //double SumDeltaPhi = deltaPhi(PhiJet1,PhiJet2) + deltaPhi(PhiJet1,PhiJet3) + deltaPhi(PhiJet2,PhiJet3);
	    _histPtJet3->fill(PtJet3, weight);
	    _histDeltaPhiZJ1_3->fill(deltaPhi(PhiJet1,PhiZ), weight);
	    _histDeltaPhiZJ2_3->fill(deltaPhi(PhiJet2,PhiZ), weight);
	    _histDeltaPhiZJ3_3->fill(deltaPhi(PhiZ,PhiJet3), weight);
	    _histDeltaPhiJ1J2_3->fill(deltaPhi(PhiJet1,PhiJet2), weight);
	    _histDeltaPhiJ1J3_3->fill(deltaPhi(PhiJet1,PhiJet3), weight);
	    _histDeltaPhiJ2J3_3->fill(deltaPhi(PhiJet2,PhiJet3), weight);
	    
	    if(Njets>3){
	      double PtJet4  = finaljet_list[2].pT();
	       _histPtJet4->fill(PtJet4, weight);
	     }
	  }
	}
      }
    }
    
    
    bool ApplyZAcceptance(const ParticleVector& leptons){
      //check if the 2 most energetic leptons are in the acceptance and have 71<Mll<111 GeV                                                                                                                                                         
      bool isAcceptance = false;
      bool isMassRange  = false;
      FourMomentum lepton1=leptons[0].momentum();
      FourMomentum lepton2=leptons[1].momentum();
      const FourMomentum pmom = leptons[0].momentum() + leptons[1].momentum();
      
      
      if (fabs(lepton1.eta()<2.4) &&
          fabs(lepton2.eta()<2.4) &&
          fabs(lepton1.pT()>20.0) &&
          fabs(lepton2.pT()>20.0)) isAcceptance = true ;
      
      double mass = sqrt(pmom.invariant());
      if (inRange(mass/GeV, 71.0, 111.0)) isMassRange = true;
      
      if(isAcceptance && isMassRange) return true;
      else return false;
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
    AIDA::IHistogram1D* _histDeltaPhiZJ1_1;    
    AIDA::IHistogram1D* _histDeltaPhiZJ1_2;    
    AIDA::IHistogram1D* _histDeltaPhiZJ1_3;    
    AIDA::IHistogram1D* _histDeltaPhiZJ2_3;  
    AIDA::IHistogram1D* _histDeltaPhiZJ3_3;  
    AIDA::IHistogram1D* _histDeltaPhiJ1J2_3; 
    AIDA::IHistogram1D* _histDeltaPhiJ1J3_3;
    AIDA::IHistogram1D* _histDeltaPhiJ2J3_3; 
    //AIDA::IHistogram1D* _histSumDeltaPhi;    
    
    };
  
    AnalysisBuilder<ZJETS_ANGULAR_DRESS_NEW> plugin_ZJETS_ANGULAR_DRESS_NEW;
    
  }

