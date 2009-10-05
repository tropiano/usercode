#ifndef PATAnalysis_EfficiencyMuon_h
#define PATAnalysis_EfficiencyMuon_h

#include "DataFormats/FWLite/interface/Event.h"

#include <TFile.h>
#include <TH1D.h>
#include <TList.h>
#include <string>

//#include "TagAndProbe.h"


class EfficiencyMuon {
public :

    enum cuts {TwoMuonsCut, TM_OppositeChargeCut, TM_OC_MassCut, TM_OC_M_QualityCutsCut, TM_OC_M_QC_DXYCut, TM_OC_M_QC_DXY_IsoCut};
  
    //constructor with the name of output file the extenzion to be added to the titles off all histograms
    //bins, xmin, xmax and the normalization factor.
    EfficiencyMuon(TFile* out, TList* fInput);

    ~EfficiencyMuon();

    void process(const fwlite::Event& iEvent);

    void finalize();

   ///////DEFINITIONS//////////
   /*
    selection efficiency      = [#Z rec in acceptance]/[#Z gen (mass>50)]
    acceptance                = [#Z gen in acceptance (pt>20,eta<2.4,60<m<120)]/[#Z gen (mass>50)]
    reconstruction efficiency = [#Z rec in acceptance WITH a Z gen in acceptance]/[#Z gen in acceptance (pt>20,eta<2.4,60<m<120)]
    fake rate                 = [#Z rec in acceptance WITHOUT a Z gen in acceptance]/[#Z rec in acceptance WITH a Z gen in acceptance]
   */

   //plots to estimate reconstruction efficiency step by step
   TH1D *TwoMuons, *TM_MuTriggered, *TM_MuT_OppositeCharge, *TM_MuT_OC_Mass, *TM_MuT_OC_M_QualityCuts, *TM_MuT_OC_M_QC_DXY, *TM_MuT_OC_M_QC_DXY_Iso;
   TH1D * TM_MuJetTriggered;
   TH1D * TM_JetTriggered;

   //plots to estimate fake rate (a "fake" is an event reconstructed in acceptance but generated outside acceptance)
   //the global selection efficiency (#Zrec in acceptance/#Zgen total ) is UNAFFECTED by these fake, because even if they were generated outside acceptance
   //they were generated, so they are definitely signal events
   //TH1D *fake, *fakeVsEta, *fakeVsPt, *fakeVsMass, *recMulti_fake, *recSecMuPt_fake, *recFwdMuEta_fake, *recMassZ_fake;
   
   //plots to estimate reconstruction efficiency
   TH1D *generalefficiency_numerator, *generalefficiency_denominator;


private:   
   bool _initiated;

   TFile* _outputfile;

   std::string _dirname;

   bool _performfits;

   double _isocut, _ptjetmin, _etajetmax;

   double _norm;

   int _nbin;

   double _xmin;

   double _xmax;

   bool _requireGenInAcceptance;

   std::vector<TH1*> _vectorHistos;
/*
   TagAndProbe* _tp_TM_MuT_OC_M_QC;
   TagAndProbe* _tp_TM_MuT_OC_M_QC_DXY;
   TagAndProbe* _tp_TM_MuT_OC_M_QC_DXY_Iso;
*/
   std::string getCutName(cuts i) const;

};

#endif

