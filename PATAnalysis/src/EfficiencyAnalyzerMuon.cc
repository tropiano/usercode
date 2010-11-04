#include "Firenze/PATAnalysis/include/EfficiencyAnalyzerMuon.h"
#include "Firenze/PATAnalysis/include/TagAndProbeAnalyzer.h"
#include <iostream>
#include <fstream>
#include <iomanip>

#include "TH1D.h"
#include "TGraphAsymmErrors.h"

EfficiencyAnalyzerMuon::EfficiencyAnalyzerMuon(TFile* input, TFile* output, TFile* sec_input, std::string dirname): _dir(0), _dir1(0), _sec_input(sec_input), _output(output) {
  //file->Open("UPDATE");
  _dir = (TDirectory*) input->Get(dirname.c_str());
  if(_sec_input)_dir1 = (TDirectory*) _sec_input->Get(dirname.c_str());
  
}


void EfficiencyAnalyzerMuon::analyze(std::string summaryfile, double scale){
  if (!_dir) {
    std::cout << "Error in EfficiencyAnalyzerMuon, dir not found in file" << std::endl;
    return;
  }

  TH1D *acc_num = (TH1D *) _dir->Get("acceptance_numerator");
  acc_num->Scale(scale);
  TH1D *acc_den = (TH1D *) _dir->Get("acceptance_denominator");
  acc_den->Scale(scale);

  TH1D *TwoMuons = (TH1D *) _dir->Get("TwoMuons");
  TwoMuons->Scale(scale);
  TH1D *TM_MuTriggered = (TH1D *) _dir->Get("TM_MuT");
  TM_MuTriggered->Scale(scale);
  TH1D *TM_MuT_OppositeCharge = (TH1D *) _dir->Get("TM_MuT_OppositeCharge");
  TM_MuT_OppositeCharge->Scale(scale);
  TH1D *TM_MuT_OC_Mass  = (TH1D *) _dir->Get("TM_MuT_OC_Mass");
  TM_MuT_OC_Mass->Scale(scale);
  TH1D *TM_MuT_OC_M_QualityCuts = (TH1D *) _dir->Get("TM_MuT_OC_M_QualityCuts");
  TM_MuT_OC_M_QualityCuts->Scale(scale);
  TH1D *TM_MuT_OC_M_QC_DXY  = (TH1D *) _dir->Get("TM_MuT_OC_M_QC_DXY");
  TM_MuT_OC_M_QC_DXY->Scale(scale);
  TH1D *TM_MuT_OC_M_QC_DXY_Iso = (TH1D *) _dir->Get("TM_MuT_OC_M_QC_DXY_Iso"); 
  TM_MuT_OC_M_QC_DXY_Iso->Scale(scale);


 
  TH1D *generalefficiency_numerator = (TH1D *) _dir->Get("efficiency_numerator"); 
  generalefficiency_numerator->Scale(scale);
  TH1D *generalefficiency_denominator = (TH1D *) _dir->Get("efficiency_denominator");
  generalefficiency_denominator->Scale(scale);

  if (_output) //return;
    _output->cd();
  TGraphAsymmErrors acceptance(acc_num, acc_den);
  acceptance.SetNameTitle("acceptance", "acceptance");
  TGraphAsymmErrors inclAcceptance = inclusiveEfficiency(acc_num, acc_den, "inclusive acceptance", "inclusive acceptance");
  
  TGraphAsymmErrors efficiency(generalefficiency_numerator, generalefficiency_denominator);
  efficiency.SetNameTitle("efficiency", "Reconstruction efficiency");
  TGraphAsymmErrors inclEfficiency = inclusiveEfficiency(generalefficiency_numerator, generalefficiency_denominator, "inclusive efficiency", "inclusive Reconstruction efficiency");

  TGraphAsymmErrors TwoMuonsEff(TwoMuons, generalefficiency_denominator);
  TwoMuonsEff.SetNameTitle("TwoMuonsEff", "2 muons eff");
  TGraphAsymmErrors inclTwoMuonsEff = inclusiveEfficiency(TwoMuons, generalefficiency_denominator, "inclusive TwoMuonsEff", "inclusive 2 muons eff");

  TGraphAsymmErrors TM_MuTriggeredEff(TM_MuTriggered, TwoMuons);
  TM_MuTriggeredEff.SetNameTitle("TM_MuTriggeredEff", "2 muons + trigger eff");
  TGraphAsymmErrors inclTM_MuTriggeredEff = inclusiveEfficiency(TM_MuTriggered, TwoMuons, "inclusive TM_MuTriggeredEff", "inclusive 2 muons + trigger eff");

  TGraphAsymmErrors TM_MuT_OppositeCharge_Eff(TM_MuT_OppositeCharge, TM_MuTriggered);
  TM_MuT_OppositeCharge_Eff.SetNameTitle("TM_MuT_OppositeCharge_Eff", "2 triggered muons Opposite Charge eff");
  TGraphAsymmErrors inclTM_MuT_OppositeCharge_Eff = inclusiveEfficiency(TM_MuT_OppositeCharge, TM_MuTriggered, "incluive TM_MuT_OppositeCharge_Eff", "inclusive 2 triggered muons Opposite Charge eff");

  TGraphAsymmErrors TM_MuT_OC_Mass_Eff(TM_MuT_OC_Mass, TM_MuT_OppositeCharge);
  TM_MuT_OC_Mass_Eff.SetNameTitle("TM_MuT_OC_Mass_Eff", "2 triggered muons Opposite Charge with Mass constraint");
  TGraphAsymmErrors inclTM_MuT_OC_Mass_Eff = inclusiveEfficiency(TM_MuT_OC_Mass, TM_MuT_OppositeCharge, "inclusive TM_MuT_OC_Mass_Eff", "inclusive 2 triggered muons Opposite Charge with Mass constraint");

  TGraphAsymmErrors TM_MuT_OC_M_QualityCuts_Eff(TM_MuT_OC_M_QualityCuts, TM_MuT_OC_Mass);
  TM_MuT_OC_M_QualityCuts_Eff.SetNameTitle("TM_MuT_OC_M_QualityCuts_Eff", "2 triggered muons Opposite Charge with Mass constraint Quality Cuts eff");
  TGraphAsymmErrors inclTM_MuT_OC_M_QualityCuts_Eff = inclusiveEfficiency(TM_MuT_OC_M_QualityCuts, TM_MuT_OC_Mass, "inclusive TM_MuT_OC_M_QualityCuts_Eff", "inclusive 2 triggered muons Opposite Charge with Mass constraint Quality Cuts eff");

  TGraphAsymmErrors TM_MuT_OC_M_QC_DXY_Eff(TM_MuT_OC_M_QC_DXY, TM_MuT_OC_M_QualityCuts);
  TM_MuT_OC_M_QC_DXY_Eff.SetNameTitle("TM_MuT_OC_M_QC_DXY_Eff", "2 triggered muons Opposite Charge with Mass constraint with Quality Cuts and DXY cut eff");
  TGraphAsymmErrors inclTM_MuT_OC_M_QC_DXY_Eff = inclusiveEfficiency(TM_MuT_OC_M_QC_DXY, TM_MuT_OC_M_QualityCuts, "inclusive TM_MuT_OC_M_QC_DXY_Eff", "inclusive 2 triggered muons Opposite Charge with Mass constraint with Quality Cuts and DXY cut eff");

  TGraphAsymmErrors TM_MuT_OC_M_QC_DXY_Iso_Eff(TM_MuT_OC_M_QC_DXY_Iso, TM_MuT_OC_M_QC_DXY);
  TM_MuT_OC_M_QC_DXY_Iso_Eff.SetNameTitle("TM_MuT_OC_M_QC_DXY_Iso_Eff", "2 triggered muons Opposite Charge Quality Cuts and DXY cut, mass constraint with isolation eff");
  TGraphAsymmErrors inclTM_MuT_OC_M_QC_DXY_Iso_Eff = inclusiveEfficiency(TM_MuT_OC_M_QC_DXY_Iso, TM_MuT_OC_M_QC_DXY, "inclusive TM_MuT_OC_M_QC_DXY_Iso_Eff", "inclusive 2 triggered muons Opposite Charge Quality Cuts and DXY cut, mass constraint with isolation eff");


  if (_output){
    acceptance.Write();
    inclAcceptance.Write();

    efficiency.Write();
    inclEfficiency.Write();

    TwoMuonsEff.Write();
    inclTwoMuonsEff.Write();
    TM_MuTriggeredEff.Write();
    inclTM_MuTriggeredEff.Write();
    TM_MuT_OppositeCharge_Eff.Write();
    inclTM_MuT_OppositeCharge_Eff.Write();
    TM_MuT_OC_Mass_Eff.Write();
    inclTM_MuT_OC_Mass_Eff.Write();
    TM_MuT_OC_M_QualityCuts_Eff.Write();
    inclTM_MuT_OC_M_QualityCuts_Eff.Write();
    TM_MuT_OC_M_QC_DXY_Eff.Write();
    inclTM_MuT_OC_M_QC_DXY_Eff.Write();
    TM_MuT_OC_M_QC_DXY_Iso_Eff.Write();
    inclTM_MuT_OC_M_QC_DXY_Iso_Eff.Write();
  } 

  //print summary info
  if (!summaryfile.empty()){
    std::fstream stream(summaryfile.c_str(), fstream::out);
    stream << "\\documentclass[12pt]{article}\n\\begin{document}\n\\begin{tabular}{c|c|c|c|c|}" << endl;
    stream << "& 0 jets & 1 jets & 2 jets & 3 jets \\\\ \n \\hline" << endl;
    stream << " Two Muons & " << TwoMuons->Integral() << " & " << TwoMuons->Integral(2, 10) << " & " << TwoMuons->Integral(3, 10) << " & " << TwoMuons->Integral(4, 10) << "\\\\" << endl;
    stream << " Triggered & " << TM_MuTriggered->Integral() << " & " << TM_MuTriggered->Integral(2, 10) << " & " << TM_MuTriggered->Integral(3, 10) << " & " << TM_MuTriggered->Integral(4, 10) << "\\\\" << endl;
    stream << " Opposite Charge & " << TM_MuT_OppositeCharge->Integral() << " & " << TM_MuT_OppositeCharge->Integral(2, 10) << " & " << TM_MuT_OppositeCharge->Integral(3, 10) << " & " << TM_MuT_OppositeCharge->Integral(4, 10) << "\\\\" << endl;
    stream << " Mass  & " << TM_MuT_OC_Mass->Integral() << " & " << TM_MuT_OC_Mass->Integral(2, 10) << " & " << TM_MuT_OC_Mass->Integral(3, 10) << " & " << TM_MuT_OC_Mass->Integral(4, 10) << "\\\\" << endl;
    stream << " Quality cuts & " << TM_MuT_OC_M_QualityCuts->Integral() << " & " << TM_MuT_OC_M_QualityCuts->Integral(2, 10) << " & " << TM_MuT_OC_M_QualityCuts->Integral(3, 10) << " & " << TM_MuT_OC_M_QualityCuts->Integral(4, 10) << "\\\\" << endl;
    stream << " Impact Parameter & " << TM_MuT_OC_M_QC_DXY->Integral() << " & " << TM_MuT_OC_M_QC_DXY->Integral(2, 10) << " & " << TM_MuT_OC_M_QC_DXY->Integral(3, 10) << " & " << TM_MuT_OC_M_QC_DXY->Integral(4, 10) << "\\\\" << endl;
    stream << " Isolation & " << TM_MuT_OC_M_QC_DXY_Iso->Integral() << " & " << TM_MuT_OC_M_QC_DXY_Iso->Integral(2, 10) << " & " << TM_MuT_OC_M_QC_DXY_Iso->Integral(3, 10) << " & " << TM_MuT_OC_M_QC_DXY_Iso->Integral(4, 10) << "\\\\" << endl;
    stream << "\\end{tabular}\n" << endl;
    stream << "\\hspace{2cm}" << endl;
    stream << "\\begin{tabular}{c|c|c|c|c|}" << endl;
    stream << "& 0 jets & 1 jets & 2 jets & 3 jets \\\\ \n \\hline" << endl;
    stream << "acceptance &";
    print(acceptance, stream);
    stream << "efficiency &" ;
    print(efficiency, stream);
    stream <<"\\hline" << endl;
    stream << " Two Muons & ";
    print(TwoMuonsEff, stream);
    stream << " Triggered & ";
    print(TM_MuTriggeredEff, stream);
    stream << " Opposite Charge & ";
    print(TM_MuT_OppositeCharge_Eff, stream);
    stream << " Mass  & ";
    print(TM_MuT_OC_Mass_Eff, stream);
    stream << " Quality cuts & ";
    print(TM_MuT_OC_M_QualityCuts_Eff, stream);
    stream << " Impact Parameter & ";
    print(TM_MuT_OC_M_QC_DXY_Eff, stream);
    stream << " Isolation & ";
    print(TM_MuT_OC_M_QC_DXY_Iso_Eff, stream);       
    stream << "\\end{tabular}" << endl;

    stream << "\\hspace{2cm}" << endl;
    stream << "\\begin{tabular}{c|c|c|c|c|}" << endl;
    stream << "& $\\geq$0 jets & $\\geq$1 jets & $\\geq$2 jets & $\\geq$3 jets \\\\ \n \\hline" << endl;
    stream << "acceptance &";
    print(inclAcceptance, stream);
    stream << "efficiency &" ;
    print(inclEfficiency, stream);
    stream <<"\\hline" << endl;
    stream << " Two Muons & ";
    print(inclTwoMuonsEff, stream);
    stream << " Triggered & ";
    print(inclTM_MuTriggeredEff, stream);
    stream << " Opposite Charge & ";
    print(inclTM_MuT_OppositeCharge_Eff, stream);
    stream << " Mass  & ";
    print(inclTM_MuT_OC_Mass_Eff, stream);
    stream << " Quality cuts & ";
    print(inclTM_MuT_OC_M_QualityCuts_Eff, stream);
    stream << " Impact Parameter & ";
    print(inclTM_MuT_OC_M_QC_DXY_Eff, stream);
    stream << " Isolation & ";
    print(inclTM_MuT_OC_M_QC_DXY_Iso_Eff, stream);
    stream << "\\end{tabular}\n\\end{document}" << endl;

    stream.close();
  }
  TagAndProbeAnalyzer tpana(_dir, _output, 0, "Offline");
  tpana.analyze(3);
 
}

TGraphAsymmErrors EfficiencyAnalyzerMuon::inclusiveEfficiency(TH1* num, TH1* den, const char* name, const char* title) const{
  TH1D inclnum("inclusivenumerator", "inclusivenumerator", num->GetNbinsX(), num->GetXaxis()->GetXmin(), num->GetXaxis()->GetXmax()); 
  TH1D inclden("inclusivedenominator", "inclusivedenominator", num->GetNbinsX(), num->GetXaxis()->GetXmin(), num->GetXaxis()->GetXmax()); 

  for(int i = 1; i <= num->GetNbinsX(); ++i  ){
    inclnum.SetBinContent(i, num->Integral(i, 10));  
  }
  for(int i = 1; i <= den->GetNbinsX(); ++i  ){
    inclden.SetBinContent(i, den->Integral(i, 10));
  }

  TGraphAsymmErrors inclEff(&inclnum, &inclden);
  inclEff.SetNameTitle(name, title);
  return inclEff;

}

void EfficiencyAnalyzerMuon::print(const TGraphAsymmErrors& graph, std::ostream& os) const{
  double x, y, erryl, erryh;
  
  for (int i = 0; i < 4 ; ++i ){ 
    graph.GetPoint(i, x, y);
    erryl = graph.GetErrorYlow(i);
    erryh = graph.GetErrorYhigh(i);
    std::string end("}$ & ");
    if (i == 3) end = "}$\\\\";
    os << "$"<< setprecision(4) << fixed << y << "^{" << setprecision(4) << fixed << erryh << "}_{" << setprecision(4) << fixed << erryl << end;
  }
  os << std::endl;
}
