#ifndef BackgroundMuon_h
#define BackgroundMuon_h

//#include "FWCore/TFWLiteSelector/interface/TFWLiteSelector.h"
#include "Firenze/PATAnalysis/include/FWLiteTSelector.h"
#include "TH1D.h"

#include "Firenze/PATAnalysis/include/BackgroundWorkerMuon.h"

class BackgroundMuon : public FWLiteTSelector<BackgroundWorkerMuon> {

private:
   //Efficiency histogram
   //Efficiency _efficiency;

   // output file
   //TFile* _output;

   //normalization
   double _norm;
/*
   //jet definition
   double _ptjetmin, _etajetmax;

   //isolation
   double _isocut;
*/
   void destroyHistosVector(std::vector<TH1D*>& t){
     std::vector<TH1D*>::iterator i;
     for (i = t.begin(); i != t.end(); ++i) delete *i;
   }

    

public:
  BackgroundMuon();
  virtual ~BackgroundMuon();

  void Begin(TTree * iTree);

  //void terminate(TList&);
  void Terminate();

private:
  BackgroundMuon(BackgroundMuon const&);
  BackgroundMuon operator=(BackgroundMuon const&);  

};
#endif

