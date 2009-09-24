
#include "Firenze/PATAnalysis/include/BackgroundMuon.h"
#include <TH2.h>
#include <TStyle.h>
#include <math.h>
//#include "Utilities.h"

#include "TList.h"
#include "TParameter.h"
#include "TFile.h"

#include <iostream>

//CMSSW headers
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

using namespace std;
using namespace edm;

BackgroundMuon::BackgroundMuon() : _output(0), _norm(1.), _ptjetmin(30.), _etajetmax(3.), _isocut(0.3) {
  //_output = new TFile("ciao.root", "RECREATE");
  cout << "BackgroundMuon built" << endl;
}

BackgroundMuon::~BackgroundMuon(){

}

void BackgroundMuon::begin(TList*& list){
  TIter next(list);
  bool factorSet(false), fileSet(false);
  while (TObject* obj = next()){
    const TParameter<double>* parDouble = dynamic_cast<const TParameter<double>* >(obj);
    //TFile* parFile   = dynamic_cast<TFile*>(obj);
    TNamed* parFile   = dynamic_cast<TNamed*>(obj);
    if (parDouble){
      if (!strcmp(parDouble->GetName(), "ScaleFactor")) {
        _norm = parDouble->GetVal();
        cout << "set scale factor to: " << _norm << endl;  
        factorSet = true;
      } else if (!strcmp(parDouble->GetName(), "MinPtJet")){
        _ptjetmin = parDouble->GetVal();
        cout << "set minimum pt for jets to: " << _ptjetmin << endl;
      } else if (!strcmp(parDouble->GetName(), "IsoCut")){
        _isocut = parDouble->GetVal();
        cout << "set isolation cut to: " << _isocut << endl;
      } else if (!strcmp(parDouble->GetName(), "MaxEtaJet")) { 
        _etajetmax = parDouble->GetVal();
        cout << "set maximim eta for jets to: " << _etajetmax << endl;
      }
    } else if (parFile) {
      if (!strcmp(parFile->GetName(), "OutputFile")){
        _output = new TFile(parFile->GetTitle(), "RECREATE");
        cout << "set output file to: " << _output << endl;
        fileSet = true;
      }  
    }
  }
  //check that everything was set       
  if (!factorSet) {
    cout << "You did not set the scale factor! " << endl;
    assert(factorSet);
  }
  if (!fileSet) {
    cout << "You did not set the output file! " << endl;
    assert(fileSet);
  }
}

//norm = _targetLumi/lumi;

   //norm has been provided when filling the histograms
   //_efficiency.finalize(1.);//norm);

void BackgroundMuon::terminate(TList& out){

   cout << "list size " << out.GetSize() << endl;
   _output->cd();
   TIter next(&out);
   while (TObject* obj = next()){
      cout << "object name " << obj->GetName() << endl;
      obj->Write();
   }

   _output->Close();

}
