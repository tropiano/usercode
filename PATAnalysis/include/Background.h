//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 18 15:24:15 2009 by ROOT version 5.18/00b
// from TTree Background/tree
// found on file: ../data/ZMM_M20Pythia_full.root
//////////////////////////////////////////////////////////

#ifndef Background_h
#define Background_h

#include "FWCore/TFWLiteSelector/interface/TFWLiteSelector.h"
#include "TH1D.h"

#include "Firenze/PATAnalysis/include/BackgroundWorker.h"

class Background : public TFWLiteSelector<BackgroundWorker> {

private:
   //Efficiency histogram
   //Efficiency _efficiency;

   //xsec and lumi
   double _xsec, _targetLumi;

   //normalization
   double norm;

   // output file
   TFile* _output;

   //jet definition
   double _ptjetmin, _etajetmax;

   //isolation
   double _isocut;

   void destroyHistosVector(std::vector<TH1D*>& t){
     std::vector<TH1D*>::iterator i;
     for (i = t.begin(); i != t.end(); ++i) delete *i;
   }

    

public:
  Background();
  virtual ~Background();

  void begin(TList*&);

  void terminate(TList&);

private:
  Background(Background const&);
  Background operator=(Background const&);  

};
#endif

