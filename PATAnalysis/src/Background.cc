
#include "Firenze/PATAnalysis/include/Background.h"
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

Background::Background(){
  //_output = new TFile("ciao.root", "RECREATE");
  cout << "Background built" << endl;
}

Background::~Background(){

}

void Background::begin(TList*& list){
  TIter next(list);
  while (TObject* obj = next()){
    const TParameter<double>* parDouble = dynamic_cast<const TParameter<double>* >(obj);
    //TFile* parFile   = dynamic_cast<TFile*>(obj);
    TNamed* parFile   = dynamic_cast<TNamed*>(obj);
    if (parDouble){
      if (!strcmp(parDouble->GetName(), "Luminosity")) {
        _targetLumi = parDouble->GetVal();
        cout << "set target luminosity to " << _targetLumi << endl;  
      }  
      if (!strcmp(parDouble->GetName(), "CrossSection")) {
        _xsec = parDouble->GetVal();
        cout << "set cross section to " << _xsec << endl;
      }
    } else if (parFile) {
      if (!strcmp(parFile->GetName(), "OutputFile")){
        _output = new TFile(parFile->GetTitle(), "RECREATE");
        cout << "set output file to " << _output << endl;
      }  
    }  
  }
}

//norm = _targetLumi/lumi;

   //norm has been provided when filling the histograms
   //_efficiency.finalize(1.);//norm);

void Background::terminate(TList& out){

   cout << "list size " << out.GetSize() << endl;
   _output->cd();
   TIter next(&out);
   while (TObject* obj = next()){
      cout << "object name " << obj->GetName() << endl;
      obj->Write();
   }

   _output->Close();

}
