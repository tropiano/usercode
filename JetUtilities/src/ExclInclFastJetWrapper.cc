#include "DataFormats/Candidate/interface/Candidate.h"
#include "RecoParticleFlow/PFRootEvent/interface/ProtoJet.h"

#include "Firenze/JetUtilities/interface/ExclInclFastJetWrapper.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
#include <string.h>

#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace edm;
using namespace std;

struct ExclInclFastJetWrapper::JetConfig{
  fastjet::JetDefinition  theJetDef;
  fastjet::ActiveAreaSpec theAreaSpec;
};

ExclInclFastJetWrapper::ExclInclFastJetWrapper(){
  theJetConfig=0;
}

ExclInclFastJetWrapper::~ExclInclFastJetWrapper(){
  delete theJetConfig;
}

ExclInclFastJetWrapper::ExclInclFastJetWrapper(const edm::ParameterSet& ps){
   //Getting information from the ParameterSet (reading config files):
  double Rparam=ps.getParameter<double>("FJ_ktRParam");
  //default ktRParam should be 1
  thePtMin=ps.getParameter<double>("PtMin");
  //default PtMin should be 10
  string JetFinder;
  JetFinder=ps.getParameter<string>("JetFinder");
  //default JetFinder should be "kt_algorithm"
  string Strategy;
  Strategy=ps.getParameter<string>("Strategy");
  //default Strategy should be "Best"
  theDcut=ps.getParameter<double>("dcut");
  //default dcut should be -1 (off)
  theNjets=ps.getParameter<int>("njets");
  //default njets should be -1 (off)
  if (ps.getParameter<string>("UE_Subtraction")=="yes") theDoSubtraction=true;
  else theDoSubtraction=false;
  //default UE_Subtraction should be false (off)
  
  theGhost_EtaMax=ps.getParameter<double>("Ghost_EtaMax");
  //default Ghost_EtaMax should be 6
  theActive_Area_Repeats=ps.getParameter<int>("Active_Area_Repeats");
  //default Active_Area_Repeats 5
  theGhostArea=ps.getParameter<double>("GhostArea");
  //default GhostArea 0.01
  theJetConfig=new JetConfig;
  theJetConfig->theAreaSpec=fastjet::ActiveAreaSpec(theGhost_EtaMax, theActive_Area_Repeats, theGhostArea);
  
  //configuring algorithm 
  
  
  fastjet::JetFinder jet_finder;
  if (JetFinder=="cambridge_algorithm") jet_finder=fastjet::cambridge_algorithm;
  else jet_finder=fastjet::kt_algorithm;

  //choosing search-strategy:

  fastjet::Strategy strategy;
  if (Strategy=="N2Plain") strategy=fastjet::N2Plain;
  // N2Plain is best for N<50
  else if (Strategy=="N2Tiled") strategy=fastjet::N2Tiled;
  // N2Tiled is best for 50<N<400
  else if (Strategy=="N2MinHeapTiled") strategy=fastjet::N2MinHeapTiled;
  // N2MinHeapTiles is best for 400<N<15000
  else if (Strategy=="NlnN") strategy=fastjet::NlnN;
  // NlnN is best for N>15000
  else if (Strategy=="NlnNCam") strategy=fastjet::NlnNCam;
  // NlnNCam is best for N>6000
  else strategy=fastjet::Best;
  // Chooses best Strategy for every event, depending on N and ktRParam

  //additional strategies are possible, but not documented in the manual as they are experimental,
  //they are also not used by the "Best" method. Note: "NlnNCam" only works with 
  //the cambridge_algorithm and does not link against CGAL, "NlnN" is only 
  //available if fastjet is linked against CGAL.
  //The above given numbers of N are for ktRaram=1.0, for other ktRParam the numbers would be 
  //different.
  
  theMode=0;
  if ((theNjets!=-1)&&(theDcut==-1)){
    theMode=3;
  }
  else if ((theNjets==-1)&&(theDcut!=-1)){
    theMode=2;
  }
  else if ((theNjets!=-1)&&(theDcut!=-1)){
    LogWarning("FastJetDefinition")<<"[FastJetWrapper] `njets` and `dcut` set!=-1! - running inclusive Mode"<<endl;
    theMode=1;
  }
  else {
    theMode=0;     
  }

  theJetConfig->theJetDef=fastjet::JetDefinition(jet_finder, Rparam, strategy);
  LogVerbatim("FastJetDefinition")<<"*******************************************"<<endl;
  LogVerbatim("FastJetDefinition")<<"* Configuration of FastJet                "<<endl;
  if (theDoSubtraction) LogVerbatim("FastJetDefinition")<<"* running with ActiveAreaSubtraction(median)"<<endl;
  switch (theMode){
  case 0:
    LogVerbatim("FastJetDefinition")<<"* Mode     : inclusive"<<endl;
    LogVerbatim("FastJetDefinition")<<"* PtMin    : "<<thePtMin<<endl;
    break;
  case 1:
    LogVerbatim("FastJetDefinition")<<"* [WARNING] Mode     : inclusive - dcut and njets set!"<<endl;
    LogVerbatim("FastJetDefinition")<<"* PtMin    : "<<thePtMin<<endl;
    break;
  case 2:    
    LogVerbatim("FastJetDefinition")<<"* Mode     : exclusive"<<endl;
    LogVerbatim("FastJetDefinition")<<"* dcut     : "<<theDcut<<endl;
    break;
  case 3:
    LogVerbatim("FastJetDefinition")<<"* Mode     : exclusive"<<endl;
    LogVerbatim("FastJetDefinition")<<"* njets    : "<<theNjets<<endl;
    break;
  }
  LogVerbatim("FastJetDefinition")<<"* Rparam   : "<<Rparam<<endl;
  LogVerbatim("FastJetDefinition")<<"* JetFinder: "<<JetFinder<<endl;
  LogVerbatim("FastJetDefinition")<<"* Strategy : "<<Strategy<<endl;
  LogVerbatim("FastJetDefinition")<<"*******************************************"<<endl;
  
}

//void ExclInclFastJetWrapper::run(const std::vector <const reco::Candidate*>& fInput, 
//                        std::vector<ProtoJet>* fOutput){
//void ExclInclFastJetWrapper::run(const std::vector<FJCand>& fInput, 
//          std::vector<ProtoJet>* fOutput) {
void ExclInclFastJetWrapper::run(const JetReco::InputCollection& fInput, JetReco::OutputCollection* fOutput) {
  
  std::vector<fastjet::PseudoJet> input_vectors;
   int index_=0;
   for (JetReco::InputCollection::const_iterator inputCand=fInput.begin();
  inputCand!=fInput.end();inputCand++){
      
      double px=(*inputCand)->px();
      double py=(*inputCand)->py();
      double pz=(*inputCand)->pz();
      double E=(*inputCand)->energy();
      fastjet::PseudoJet PsJet(px,py,pz,E);
      PsJet.set_user_index(index_);
      input_vectors.push_back(PsJet);
      index_++;
   }
   
   // create an object that represents your choice of jet finder and 
   // the associated parameters
   // run the jet clustering with the above jet definition

   std::vector<fastjet::PseudoJet> theJets;

   if (theDoSubtraction) {
     //with subtraction
     fastjet::ClusterSequenceActiveArea clust_AAseq(input_vectors,theJetConfig->theJetDef,theJetConfig->theAreaSpec);
   
     //  LogVerbatim("FastJetDefinition") << "***************************"<<endl;
     //  LogVerbatim("FastJetDefinition") << "* Strategy adopted by FastJet for this event was "<<
     //clust_seq.strategy_string()<<endl;
     //  LogVerbatim("FastJetDefinition") << "***************************\n"<<endl;
     //get jets
   
     //select mode:

     if (theMode==0){
       theJets=clust_AAseq.inclusive_jets(thePtMin);
     }
     else if (theMode==3){
       //if the input collection has less element than requested jets do nothing.
       //An empty jet collection will appear in the event
       if (fInput.size() < theNjets) return;
       theJets=clust_AAseq.exclusive_jets(theNjets);
       /*if (theJets.size() != theNjets){
          cout << "number of jets different from expected: requested " << theNjets << " obtained " << theJets.size() << "; items in input collection " << fInput.size() << endl;
       }*/
     }
     else if (theMode==2){
       theJets=clust_AAseq.exclusive_jets(theDcut);
     }
     else if ((theNjets!=-1)&&(theDcut!=-1)){
       LogWarning("FastJetDefinition")<<"[FastJetWrapper] `njets` and `dcut` set!=-1! - running inclusive Mode"<<endl;
       theJets=clust_AAseq.inclusive_jets(thePtMin);
     }
     else {
       //default mode
       theJets=clust_AAseq.inclusive_jets(thePtMin);
     }
     
     //make CMSSW-Objects:
     //std::vector<FJCand> jetConst;
     JetReco::InputCollection jetConst;
     theMedian_Pt_Per_Area=clust_AAseq.pt_per_unit_area();
     for (std::vector<fastjet::PseudoJet>::const_iterator itJet=theJets.begin(); itJet!=theJets.end();itJet++){
       std::vector<fastjet::PseudoJet> jet_constituents = clust_AAseq.constituents(*itJet);
       for (std::vector<fastjet::PseudoJet>::const_iterator itConst=jet_constituents.begin(); itConst!=jet_constituents.end();itConst++){
          jetConst.push_back(fInput[(*itConst).user_index()]);
       }
       fastjet::PseudoJet corrected_jet;
       fastjet::PseudoJet area_4vect = theMedian_Pt_Per_Area * clust_AAseq.area_4vector(*itJet);
       if (area_4vect.perp2() >= (*itJet).perp2() || 
     area_4vect.E() >= (*itJet).E()) { 
   // if the correction is too large, set the jet to zero
   corrected_jet = 0.0 * (*itJet);
       } 
       else {   // otherwise do an E-scheme subtraction
        corrected_jet = (*itJet) - area_4vect;
       }
       
       double px=corrected_jet.px();
       double py=corrected_jet.py();
       double pz=corrected_jet.pz();
       double E=corrected_jet.E();
       math::XYZTLorentzVector p4(px,py,pz,E);
       fOutput->push_back(ProtoJet(p4,jetConst));
       jetConst.clear();
     }
   }


   // or run without subtraction:
   else {
     fastjet::ClusterSequence clust_seq(input_vectors, theJetConfig->theJetDef);
     
     //select mode:
     if ((theNjets==-1)&&(theDcut==-1)){
       theJets=clust_seq.inclusive_jets(thePtMin);
     }
     else if ((theNjets!=-1)&&(theDcut==-1)){
       //cout << "Running..." << endl;
       //if the input collection has less element than requested jets do nothing.
       //An empty jet collection will appear in the event 
       if (fInput.size() < theNjets) return;
       theJets=clust_seq.exclusive_jets(theNjets);
       /*if (theJets.size() != theNjets){
          cout << "number of jets different from expected: requested " << theNjets << " obtained " << theJets.size() << "; items in input collection " << fInput.size() << endl;
       } */  
     }
     else if ((theNjets==-1)&&(theDcut!=-1)){
       theJets=clust_seq.exclusive_jets(theDcut);
     }
     else if ((theNjets!=-1)&&(theDcut!=-1)){
       LogWarning("FastJetDefinition")<<"[FastJetWrapper] `njets` and `dcut` set!=-1! - running inclusive Mode"<<endl;
       theJets=clust_seq.inclusive_jets(thePtMin);
     }
     else {
       //default mode
       theJets=clust_seq.inclusive_jets(thePtMin);
     }
     
     //make CMSSW-Objects:
     //std::vector<FJCand> jetConst;
     JetReco::InputCollection jetConst;
     for (std::vector<fastjet::PseudoJet>::const_iterator itJet=theJets.begin(); itJet!=theJets.end();itJet++){
       std::vector<fastjet::PseudoJet> jet_constituents = clust_seq.constituents(*itJet);
       for (std::vector<fastjet::PseudoJet>::const_iterator itConst=jet_constituents.begin();
          itConst!=jet_constituents.end();itConst++){
         jetConst.push_back(fInput[(*itConst).user_index()]);
       }
       double px=(*itJet).px();
       double py=(*itJet).py();
       double pz=(*itJet).pz();
       double E=(*itJet).E();
       math::XYZTLorentzVector p4(px,py,pz,E);
       fOutput->push_back(ProtoJet(p4,jetConst));
       jetConst.clear();
     }
     /*if (fOutput->size() != theNjets){
          cout << "number of FINAL jets different from expected: requested " << theNjets << " obtained " << fOutput->size() << "; items in input collection " << fInput.size() << endl;
     } */
   }
}



