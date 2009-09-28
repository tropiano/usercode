#include <iostream>
//#include "Signal.h"
//#include "Firenze/PATAnalysis/include/Background.h"
#include "TFile.h"
#include "TTree.h"
//#include "RooUnfold/RooUnfoldResponse.h"
#include "TParameter.h"
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
  //gSystem->Load("libFWCoreTFWLiteSelector");
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec( ".x remote.C" ); 

  TDSet train( "ciao", "Events");
  ifstream infile; 
  infile.open("files.txt");
  string filename;
  double nevents = 0;
  while(getline ( infile, filename )){
    //train.Add("/raid/lenzip/CMSSW/CMSSW_3_1_2/src/Firenze/Reduction/test/ZmumuPythia/res/test_pat_174.root");
    cout << filename << endl;
    train.Add(filename.c_str());
    //nevents += train.Current()->GetEntries();
  }  
  /*TIter iter(train.GetListOfElements());
  while (TObject* obj = iter()){
    TDSetElement * ds = dynamic_cast<TDSetElement *>(obj);
    if (ds){
      nevents += ds->GetEntries(true, false);
    } else {
      cout << "not a ds" << endl;
    }
  }*/

  cout << "total entries: " << nevents << endl;
  // TFile trainFile("/raid/lenzip/CMSSW/CMSSW_3_1_2/src/Firenze/Reduction/test/ZmumuPythia/res/test_pat_174.root");
  train.Print();
  //train.GetTreeHeader(p)->Print();

  //TTree* train = (TTree*) trainFile.Get("Events");

  //TFile* outtrain = new TFile("train.root", "RECREATE");
  TNamed* outtrain = new TNamed("OutputFile", "train.root");

  //TList config;
  //double factor = targetLumi / train.GetTreeHeader(p)->GetEntries() * 1200.;
  TString none("");
  //cout << typeid(train.GetEntryList()).name() << endl;          
  double factor = targetLumi / 1837456 * 1200.;
 
  TParameter<double>* parfactor = new TParameter<double>("ScaleFactor", factor);
  //TParameter<double>* parlumi   = new TParameter<double>("Luminosity", 100);
  //config.Add(parfactor);
  //config.Add(lumi);
  //config.Add(parouttrain);

  //p->AddInput(csec);
  p->AddInput(parfactor);
  p->AddInput(outtrain);

  //processing signal
  //Background trainsignal; //517 is the target luminosity and actually the real lumy of one of the two samples
  //trainsignal.SetInputList(&config);
  //p->Process(&train, "FWLiteTSelector");//&trainsignal);
  p->Process(&train, "FWLiteTSelector<BackgroundWorkerMuon>");//"BackgroundMuon");//&trainsignal);
  //train.Process("FWLiteTSelector");
  p->ClearInput();
  p->Close();
  //train->Process(&trainsignal);

}
