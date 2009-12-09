#ifndef EfficiencyElectron_h
#define EfficiencyElectron_h

#include "TFile.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"

class EfficiencyElectron : public MyPatAnalysis{
public :
   EfficiencyElectron();
   
   virtual ~EfficiencyElectron();

   virtual void begin(TFile* out, const edm::ParameterSet&);

   static MyPatAnalysis* create() {return new EfficiencyElectron();} 

   virtual void  process(const fwlite::Event& iEvent);

   virtual void finalize(); 

   //Histograms
   
   TH1D *genLeadElEtaEff_Acc, *genLeadElEtaEff_Acc_Qual, *genLeadElEtaEff_Acc_Qual_Imp, *genLeadElEtaEff_Acc_Qual_Imp_Iso, *genLeadElEtaEff_Acc_Qual_Imp_Iso_EiD,
   *genLeadElPtEff_Acc, *genLeadElPtEff_Acc_Qual, *genLeadElPtEff_Acc_Qual_Imp, *genLeadElPtEff_Acc_Qual_Imp_Iso, *genLeadElPtEff_Acc_Qual_Imp_Iso_EiD;
   
   TH1D *genMassZEff_Acc, *genPtZEff_Acc, *genEtaZEff_Acc,
   *genMassZEff_Acc_Qual, *genPtZEff_Acc_Qual, *genEtaZEff_Acc_Qual,
   *genMassZEff_Acc_Qual_Imp, *genPtZEff_Acc_Qual_Imp, *genEtaZEff_Acc_Qual_Imp,
   *genMassZEff_Acc_Qual_Imp_Iso, *genPtZEff_Acc_Qual_Imp_Iso, *genEtaZEff_Acc_Qual_Imp_Iso,
   *genMassZEff_Acc_Qual_Imp_Iso_EiD, *genPtZEff_Acc_Qual_Imp_Iso_EiD, *genEtaZEff_Acc_Qual_Imp_Iso_EiD;
   
   TH1D *GenIsoJetEff_Acc, *GenIsoJetEff_Acc_Qual, *GenIsoJetEff_Acc_Qual_Imp, *GenIsoJetEff_Acc_Qual_Imp_Iso, *GenIsoJetEff_Acc_Qual_Imp_Iso_EiD;
   
   TH1D *AccDenom_genPtZ, *AccDenom_genMassZ, *AccDenom_genEtaZ, *AccDenom_genLeadElEta, *AccDenom_genLeadElPt, *AccDenom_GenIsoJetNumber,
   *EffDenom_genPtZ, *EffDenom_genMassZ, *EffDenom_genEtaZ, *EffDenom_genLeadElEta, *EffDenom_genLeadElPt, *EffDenom_GenIsoJetNumber;

  
private:
  
  bool _Sumw2;
  
  std::string _electronID;

  TFile* _file;  

  TDirectory* _dir;

  std::vector<TH1D*> _histoVector;
  
};
#endif

