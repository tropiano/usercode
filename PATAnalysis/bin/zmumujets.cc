#include <iostream>
//#include "Signal.h"
//#include "Firenze/PATAnalysis/include/Background.h"
#include "TFile.h"
#include "TTree.h"
//#include "RooUnfold/RooUnfoldResponse.h"
#include "TParameter.h"
#include "TFileMerger.h"
#include "TDSet.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TProof.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include <fstream>


using namespace std;


int main(){
  TProof * p = TProof::Open("");
  
  double targetLumi = 100.;
  

  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec( ".x remote.C" ); 

  TDSet train( "ciao", "Events");
  ifstream infile; 
  infile.open("files.txt");
  string filename;
  double nevents = 0;
  while(getline ( infile, filename )){
    cout << filename << endl;
    train.Add(filename.c_str());
  }  
  cout << "total entries: " << nevents << endl;
  train.Print();
  TNamed* outtrain = new TNamed("OutputFile", "train.root");
  TNamed* boolopt = new TNamed("RequireGenInAccepance", "true");
  double factor = targetLumi / 1837456 * 1200.;
  TParameter<double>* parfactor = new TParameter<double>("ScaleFactor", factor);
  p->AddInput(parfactor);
  p->AddInput(outtrain);
  p->AddInput(boolopt);

  p->Process(&train, "SignalMuon");
  p->ClearInput();


  TDSet signal( "ciao", "Events");
  ifstream infile2;
  infile2.open("files_other.txt");
  while(getline ( infile2, filename )){
    cout << filename << endl;
    signal.Add(filename.c_str());
  }
  signal.Print();
  TNamed* outsignal = new TNamed("OutputFile", "signal.root");
  p->AddInput(parfactor);
  p->AddInput(outsignal);
  p->AddInput(boolopt);

  p->Process(&signal, "SignalMuon");

  TFileMerger fm(true);
  fm.AddFile("train.root");
  fm.AddFile("signal.root");
  fm.OutputFile("total.root");
  fm.Merge();

  p->Close();

}
