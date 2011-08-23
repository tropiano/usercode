
#include "Firenze/PATAnalysis/include/TagAndProbeAnalyzerElectron.h"

#include <iostream>
#include <sstream>

#include "RooDataHist.h"
#include "RooPlot.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooMinuit.h"
#include "RooFormulaVar.h"
#include "RooCBShape.h"
#include "RooBreitWigner.h"
#include "RooSimultaneous.h"
#include "RooChi2Var.h"
#include "RooNLLVar.h"
#include "TPaveText.h"
#include "TF1.h"
#include "TTree.h"
#include "TH1.h"
#include "TVectorD.h"
#include "TMath.h"

using namespace std;
using namespace RooFit;

TagAndProbeAnalyzerElectron::TagAndProbeAnalyzerElectron(TDirectory* input, TFile* output, TDirectory* sec_input, std::string dataset, bool performfits, TFile* training_signal, TFile* training_background):
_initialized(false),
_input(input),
_sec_input(sec_input),
_output(output),
_name("Fit"),
_dataset(dataset),
_performfits(performfits),
_training_signal(training_signal),
_training_background(training_background),
_mass("mass", "mass", 60., 120.),
_bin("bin", "bin", 0, 10),
_probe("probe", "probe", -1, 2),
//_passprobe("passprobe", "passprobe", 0, 1),
_weight("weight", "weight", 0., 100.),
_passprobe_cat("passprobe", "passprobe")
{
  _input->cd();
  
  _passprobe_cat.defineType("pass",  1);
  _passprobe_cat.defineType("fail",  0);
  
  TTree* tree=0; 
  TTree* tree1=0;
  
  tree = (TTree*) _input->Get(_dataset.c_str());
  if (!tree){
    std::cout << "Error in TagAndProbeAnalyzerElectron::TagAndProbeAnalyzerElectron : could not find tree named dataset in input file " << std::endl;
    return;
  }
  
  if(_sec_input){
  tree1 = (TTree*) _sec_input->Get(_dataset.c_str());
  if (!tree1){
    std::cout << "Error in TagAndProbeAnalyzerElectron::TagAndProbeAnalyzerElectron : could not find tree1 named dataset in input file " << std::endl;
    return;
  }
  }
  
  _initialized = true;
  
  _argset = new RooArgSet(_mass, _bin, _probe, _passprobe_cat, _weight );
 
  _output->cd();
  _rootree = new RooDataSet("roodataset", "roodataset", tree, *_argset, "", "weight");
  if(_sec_input)_rootree1 = new RooDataSet("roodataset", "roodataset", tree1, *_argset, "", "weight");
  
}

TagAndProbeAnalyzerElectron::~TagAndProbeAnalyzerElectron(){
  delete _argset;
}

void TagAndProbeAnalyzerElectron::analyze(unsigned int nbins, std::string option )
{
   if (!_initialized) {
      std::cout << "Error in TagAndProbeAnalyzerElectron::Analyze, unable to initialize the RooDataSet. Are you sure that the TagAndProbeFiller has been run?" << std::endl;
      return;  
   }
 
   _rootree->Write();
   if(_sec_input)_rootree1->Write();
   
   TGraphAsymmErrors singleEfficiency, singleEfficiency1;
 
   for (unsigned int i = 0; i < nbins; ++i){
   
      stringstream name_tp;
      name_tp << _name << "TagProbe" << i << "bin_0";
      stringstream name_tp1;
      name_tp1 << _name << "TagProbe" << i << "bin_1";
      
      stringstream name_tpp;
      name_tpp << _name << "TagPassProbe" << i << "bin";
     
      stringstream formula_tp;
      formula_tp << "bin==" << i << " && probe==1";
     
      RooAbsData* tagprobe=0; 
      RooAbsData* tagprobe1=0;
      
      tagprobe = _rootree->reduce(formula_tp.str().c_str());
      if(_sec_input)tagprobe1 = _rootree1->reduce(formula_tp.str().c_str());
    
      std::pair<RooFitResult*, RooRealVar*>  tp_fit, tp_fit1;
      
      cout<<endl<<"### Fitting bin n. "<<i<<endl<<endl;
      
      tp_fit  = fit(tagprobe, name_tp.str().c_str(), option);
      if(_sec_input)tp_fit1  = fit(tagprobe1, name_tp1.str().c_str(), option);
      
      if(tp_fit.first && tp_fit.first->status() == 0 ){
        cout << "Using backgound correctedvalues for " << _name << " in bin " << i << endl;  
        singleEfficiency.SetPoint(i, i, tp_fit.second->getVal());
        double errlow = 0.;
        double errhigh = 0.;
        if (tp_fit.second->hasAsymError()){
          errhigh = TMath::Abs(tp_fit.second->getAsymErrorHi());
          errlow  = TMath::Abs(tp_fit.second->getAsymErrorLo());
        } else {
          errlow  = TMath::Abs(tp_fit.second->getError());
          errhigh = TMath::Abs(tp_fit.second->getError());
        }
        singleEfficiency.SetPointEYhigh(i, errhigh);
        singleEfficiency.SetPointEYlow(i, errlow);
       }
       
       if(_sec_input){ 
        if(tp_fit1.first && tp_fit1.first->status() == 0 ){
          cout << "Using backgound correctedvalues for " << _name << " in bin " << i << endl;  
          singleEfficiency1.SetPoint(i, i, tp_fit1.second->getVal());
          double errlow1 = 0.;
          double errhigh1 = 0.;
          if (tp_fit1.second->hasAsymError()){
            errhigh1 = TMath::Abs(tp_fit1.second->getAsymErrorHi());
            errlow1  = TMath::Abs(tp_fit1.second->getAsymErrorLo());
          } else {
            errlow1  = TMath::Abs(tp_fit1.second->getError());
            errhigh1 = TMath::Abs(tp_fit1.second->getError());
          }
          singleEfficiency1.SetPointEYhigh(i, errhigh1);
          singleEfficiency1.SetPointEYlow(i, errlow1);
        }
                  
        if (tp_fit.first) delete tp_fit.first;
          if (tp_fit.second) delete tp_fit.second;
          if(_sec_input){
            if (tp_fit1.first) delete tp_fit1.first;
            if (tp_fit1.second) delete tp_fit1.second;
          }
       }

   //double efficiency
       if(!_sec_input){
         TGraphAsymmErrors doubleEfficiency = createDoubleEfficiency(singleEfficiency);
         singleEfficiency.SetNameTitle("SingleTag&ProbeEff","SingleTag&ProbeEff");
         singleEfficiency.Write();
         doubleEfficiency.Write();
       }
       if(_sec_input){
         TGraphAsymmErrors doubleEfficiency = createAsymmCutEfficiency(singleEfficiency, singleEfficiency1);
         singleEfficiency.SetNameTitle("SingleTag&ProbeEff_0","SingleTag&ProbeEff_0");
         singleEfficiency1.SetNameTitle("SingleTag&ProbeEff_1","SingleTag&ProbeEff_1");
         singleEfficiency.Write();
         singleEfficiency1.Write();
         doubleEfficiency.Write();
       }
  
  }
}

std::pair<RooFitResult*, RooRealVar*> TagAndProbeAnalyzerElectron::fit(RooAbsData* data, const char* name, std::string option) {

  string dirname(_input->GetName());

  cout << "bins mass" << _mass.getBins()  << endl; 

  stringstream nllname;
  nllname << "nll_" << name;
  
  RooFitResult* training_results_signal = (_training_signal) ? (RooFitResult*)_training_signal->Get(nllname.str().c_str()) : 0;
  RooFitResult* training_results_background = (_training_background) ? (RooFitResult*)_training_background->Get(nllname.str().c_str()) : 0;

  TH1* hmass = data->createHistogram("mass", 100);
  
  RooRealVar el_pass("el_pass", "average_pass", 91.,  80., 100.); 
  RooRealVar sigma_pass("sigma_pass", "sigma_pass", hmass->GetRMS(), 2, 100);
  RooRealVar a_pass("a_pass", "a_pass", 10, 0, 100);
  RooRealVar n_pass("n_pass", "n_pass", 5, 0, 100);
  
  RooRealVar el_pass2("el_pass2", "average_pass2", 91., 80., 100.);
  RooRealVar g_pass("g_pass", "g_pass", 10, 5, 50);
  
  RooRealVar fraction_pass("fraction_pass", "fraction_pass", 0.9, 0., 1.);

  RooRealVar el_fail("el_fail", "average_fail", 91., 80., 100.); 
  RooRealVar sigma_fail("sigma_fail", "sigma_fail", hmass->GetRMS(), 2, 100);
  RooRealVar a_fail("a_fail", "a_fail", 10, 0, 100);
  RooRealVar n_fail("n_fail", "n_fail", 5, 0, 100);

  RooRealVar el_fail2("el_fail2", "average_fail2", 91., 80., 100.);
  RooRealVar g_fail("g_fail", "g_fail", 10, 5, 50);
  
  RooRealVar fraction_fail("fraction_fail", "fraction_fail", 0.9, 0., 1.);


  RooCBShape crystal_pass("crystalball_pass", "crystalball_pass", _mass, el_pass, sigma_pass, a_pass, n_pass);
  RooBreitWigner breitwig_pass("breitwig_pass", "breitwig_pass", _mass, el_pass2, g_pass); //try not to force the same el_pass
  
  RooAddPdf signal_pass("signal_pass", "signal_pass", crystal_pass, breitwig_pass, fraction_pass);

  RooCBShape crystal_fail("crystalball_fail", "crystalball_fail", _mass, el_fail, sigma_fail, a_fail, n_fail);
  RooBreitWigner breitwig_fail("breitwig_fail", "breitwig_fail", _mass, el_fail2, g_fail);
  
  RooAddPdf signal_fail("signal_fail", "signal_fail", crystal_fail, breitwig_fail, fraction_fail);
  
  // if we are using signal shape from training 
  if (_training_signal) {
  cout<<"TRAINING SIGNAL 1"<<endl;
    //if the training fit was succesfull and succesfully retrieved
    if ( training_results_signal && training_results_signal->status() == 0){ 
    cout<<"TRAINING SIGNAL 2"<<endl;
      for (int i = 0; i < training_results_signal->floatParsFinal().getSize(); ++i) { 
        RooRealVar* par = (RooRealVar*) training_results_signal->floatParsFinal().at(i); 
        string parname(par->GetName());
        if (parname == "el_pass"){
          el_pass.setVal(par->getVal());
          //el_pass.setConstant();
        } else if (parname == "el_pass2"){
          el_pass2.setVal(par->getVal());
          //el_pass2.setConstant();
        } else if (parname == "sigma_pass"){
          sigma_pass.setVal(par->getVal());
          sigma_pass.setConstant();
        } else if (parname == "a_pass"){
          a_pass.setVal(par->getVal());
          a_pass.setConstant();
        } else if (parname == "n_pass"){
          n_pass.setVal(par->getVal());
          n_pass.setConstant();
        } else if (parname == "g_pass"){
          g_pass.setVal(par->getVal());
          g_pass.setConstant();
        } else if (parname == "fraction_pass"){
          fraction_pass.setVal(par->getVal());
          fraction_pass.setConstant();
        } else if (parname == "el_fail"){
          el_fail.setVal(par->getVal());
          //el_fail.setConstant();
        } else if (parname == "el_fail2"){
          el_fail2.setVal(par->getVal());
          //el_fail2.setConstant();
        } else if (parname == "sigma_fail"){
          sigma_fail.setVal(par->getVal());
          sigma_fail.setConstant();
        } else if (parname == "a_fail"){
          a_fail.setVal(par->getVal());
          a_fail.setConstant();
        } else if (parname == "n_fail"){
          n_fail.setVal(par->getVal());
          n_fail.setConstant();
        } else if (parname == "g_fail"){
          g_fail.setVal(par->getVal());
          g_fail.setConstant();
        } else if (parname == "fraction_fail"){
          fraction_fail.setVal(par->getVal());
          fraction_fail.setConstant();
        }
      }
    } else {  //either the fit did not converge or was not retrieved 
       cout << "Error retrieving fit info from training signal! " << endl;
       return make_pair((RooFitResult*) 0, (RooRealVar*) 0);
    }
  } 
  
  //now the background - EXPONENTIAL
  RooFormulaVar centeredmass("centeredmass", "mass - 91.", RooArgList(_mass));
  
  RooRealVar c_pass("c_pass", "c_pass", -2, -100., 100.);
  RooExponential background_pass("exp_pass", "exp_pass", _mass, c_pass);

  RooRealVar c_fail("c_fail", "c_fail", -2., -100., 100.);
  RooExponential background_fail("exp_fail", "exp_fail", _mass, c_fail); 


  if (_training_background) {
  cout<<"TRAINING BACKGROUND 1"<<endl;
    //if the training fit was succesfull and succesfully retrieved
    if ( training_results_background && training_results_background->status() == 0){
    cout<<"TRAINING BACKGROUND 2"<<endl;
      for (int i = 0; i < training_results_background->floatParsFinal().getSize(); ++i) {
        RooRealVar* par = (RooRealVar*) training_results_background->floatParsFinal().at(i);
        string parname(par->GetName());
        if (parname == "c_pass"){
          c_pass.setVal(par->getVal());
          c_pass.setConstant();
        }
        else if (parname == "c_fail"){
          c_fail.setVal(par->getVal());
          c_fail.setConstant();
        }
      }
    } else {  //either the fit did not converge or was not retrieved 
       cout << "Error retrieving fit info from training background! " << endl;
       return make_pair((RooFitResult*) 0, (RooRealVar*) 0);
    }
  }   
 
  RooRealVar s("s", "signal yield", data->sumEntries(), 1, 300000);
  RooRealVar efficiency("efficiency", "efficiency", 0.9, 0., 0.999);
  RooRealVar b_pass("b_pass", "background yield pass", 50, 0, 100000);
  RooRealVar b_fail("b_fail", "background yield fail", 50, 0, 100000);

  //RooFormulaVar s_fail("s_fail","s*efficiency*(1.0 - efficiency)", RooArgList(s, efficiency) );
  //RooFormulaVar s_pass("s_pass","s*efficiency*efficiency", RooArgList(s, efficiency) );
  
  RooFormulaVar s_fail("s_fail","s*(1.0 - efficiency)", RooArgList(s, efficiency) );
  RooFormulaVar s_pass("s_pass","s*efficiency", RooArgList(s, efficiency) );

  RooArgList components_pass;//(signal_pass);
  RooArgList coefficients_pass;//(s_pass);

  RooArgList components_fail;//(signal_fail);
  RooArgList coefficients_fail;//(s_fail);

  if (option == "signal"){
    components_pass.add(signal_pass);
    coefficients_pass.add(s_pass);
    components_fail.add(signal_fail);
    coefficients_fail.add(s_fail);
  } else if (option == "background"){
    components_pass.add(background_pass);
    coefficients_pass.add(b_pass);
    components_fail.add(background_fail);
    coefficients_fail.add(b_fail);
  } else if (option == "total" ) {
    components_pass.add(signal_pass);
    coefficients_pass.add(s_pass);
    components_fail.add(signal_fail);
    coefficients_fail.add(s_fail);
    components_pass.add(background_pass);
    coefficients_pass.add(b_pass);
    components_fail.add(background_fail);
    coefficients_fail.add(b_fail);
  } else {
    std::cout << "Error in TagAndProbeAnalyzerElectron::fit : Unknown option " << option << std::endl;
    return make_pair((RooFitResult*) 0, (RooRealVar*) 0);;   
  }

  RooAddPdf sumpass("sumpass", "sumpass", components_pass, coefficients_pass);
  RooAddPdf sumfail("sumfail", "sumfail", components_fail, coefficients_fail);
  
  RooSimultaneous total_fit("total_fit","total_fit", _passprobe_cat);
  _passprobe_cat.setLabel("pass");
  total_fit.addPdf(sumpass, _passprobe_cat.getLabel());
  _passprobe_cat.setLabel("fail");
  total_fit.addPdf(sumfail, _passprobe_cat.getLabel());

 // RooFitResult* fitresult = total_fit.fitTo(*data, RooFit::Save(true), RooFit::Strategy(2), RooFit::NumCPU(8), RooFit::Extended(kTRUE), RooFit::SumW2Error(true));
  
  RooFitResult* fitresult = total_fit.fitTo(*((RooDataSet*) data), RooFit::Minos(RooArgSet(efficiency)), RooFit::Save(true),  RooFit::PrintLevel(3), RooFit::Strategy(1), RooFit::NumCPU(8), RooFit::Optimize(false), RooFit::Extended(kTRUE), RooFit::SumW2Error(true));
    
  fitresult->SetName(nllname.str().c_str());

  fitresult->Write();
  
  stringstream eff;
  eff << "Efficiency = ";
  eff << efficiency.getVal();
  eff << " +/- ";
  eff << efficiency.getPropagatedError(*fitresult);

  stringstream name_pass;
  name_pass << name << "PassProbe";
  stringstream name_fail;
  name_fail << name << "FailProbe";   
  
  RooPlot * mass_pass = _mass.frame() ;
  mass_pass->SetNameTitle(name_pass.str().c_str(), name_pass.str().c_str());
  data->plotOn(mass_pass, RooFit::Cut("passprobe==passprobe::pass"), RooFit::DataError(RooAbsData::SumW2)) ;
  total_fit.plotOn(mass_pass, RooFit::Slice(_passprobe_cat, "pass"), RooFit::ProjWData(_passprobe_cat,*data));
 
  stringstream spass;
  spass << "Signal = ";
  spass << s_pass.getVal();
  spass << " +/- ";
  spass << s_pass.getPropagatedError(*fitresult);
  
  stringstream bpass;
  bpass << "Background = ";
  bpass << b_pass.getVal();
  bpass << " +/- ";
  bpass << b_pass.getPropagatedError(*fitresult);
  
  TPaveText* tboxpass = new TPaveText(0.57, 0.70, 0.89, 0.87, "BRNDC"); 
  tboxpass->AddText(spass.str().c_str());
  tboxpass->AddText(bpass.str().c_str());
  //tboxpass->AddText(eff.str().c_str());
  mass_pass->addObject(tboxpass);
  
  data->statOn(mass_pass, RooFit::Cut("passprobe==passprobe::pass"), RooFit::DataError(RooAbsData::SumW2));
  
  total_fit.getPdf(_passprobe_cat.getLabel())->plotOn(mass_pass, Components(background_pass), LineStyle(kDashed), "F", FillStyle(1001), FillColor(kGreen));
  //mass_pass->getAttFill()->SetFillColor(kGreen);
    
  mass_pass->Write();

  RooPlot * mass_fail = _mass.frame() ;
  mass_fail->SetNameTitle(name_fail.str().c_str(), name_fail.str().c_str());
  data->plotOn(mass_fail, RooFit::Cut("passprobe==passprobe::fail"),  RooFit::DataError(RooAbsData::SumW2)) ;
  total_fit.plotOn(mass_fail, RooFit::Slice(_passprobe_cat, "fail"), RooFit::ProjWData(_passprobe_cat,*data));
  
  stringstream sfail;
  sfail << "Signal = ";
  sfail << s_fail.getVal();
  sfail << " +/- ";
  sfail << s_fail.getPropagatedError(*fitresult);
  
  stringstream bfail;
  bfail << "Background = ";
  bfail << b_fail.getVal();
  bfail << " +/- ";
  bfail << b_fail.getPropagatedError(*fitresult);
  
  TPaveText* tboxfail = new TPaveText(0.57, 0.70, 0.89, 0.87, "BRNDC"); 
  tboxfail->AddText(sfail.str().c_str());
  tboxfail->AddText(bfail.str().c_str());
  tboxfail->AddText(eff.str().c_str());
  mass_fail->addObject(tboxfail);
  
  data->statOn(mass_pass, RooFit::Cut("passprobe==passprobe::fail"));
  
  //total_fit.getPdf(_passprobe_cat.getLabel())->plotOn(mass_fail, Components(background_fail), LineStyle(kDashed));
  
  mass_fail->Write(); 
  
  delete hmass;
  delete mass_pass;
  delete mass_fail;
  return std::make_pair(fitresult, (RooRealVar*) efficiency.Clone() );
 
}


TGraphAsymmErrors TagAndProbeAnalyzerElectron::createDoubleEfficiency(const TGraphAsymmErrors& single) const {
  int n = single.GetN();
  TVectorD vx(n);
  TVectorD vy(n);
  TVectorD vexl(n);
  TVectorD vexh(n);
  TVectorD veyl(n);
  TVectorD veyh(n);
  for ( int i = 0; i < n; ++i ){
    double x = 0., y = 0.;
    single.GetPoint(i, x, y);
    vx(i)   = x;
    vexl(i) = TMath::Abs(single.GetErrorXlow(i));
    vexh(i) = TMath::Abs(single.GetErrorXhigh(i));
    vy(i)   = y * y;
    veyl(i) = TMath::Abs(2 * y * single.GetErrorYlow(i));
    veyh(i) = TMath::Abs(2 * y * single.GetErrorYhigh(i));
  }

  TGraphAsymmErrors doubleEff(vx, vy, vexl, vexh, veyl, veyh);
  doubleEff.SetNameTitle((_name+"DoubleTag&Probe").c_str(), (_name+"DoubleTag&Probe").c_str());
  return doubleEff;
}

TGraphAsymmErrors TagAndProbeAnalyzerElectron::createAsymmCutEfficiency(const TGraphAsymmErrors& single0, const TGraphAsymmErrors& single1) const {

  int n = single0.GetN();
  
  TVectorD vx(n);
  TVectorD vy(n);
  TVectorD vexl(n);
  TVectorD vexh(n);
  TVectorD veyl(n);
  TVectorD veyh(n);
  
  for ( int i = 0; i < n; ++i ){
  
    double x0 = 0., y0 = 0., x1 = 0., y1 = 0.;
    single0.GetPoint(i, x0, y0);
    single1.GetPoint(i, x1, y1);
    
    vx(i)   = x0;
    vexl(i) = TMath::Abs(single0.GetErrorXlow(i));
    vexh(i) = TMath::Abs(single0.GetErrorXhigh(i));
    vy(i)   = y0 * y1;
    veyl(i) = TMath::Sqrt(pow(y1 * single0.GetErrorYlow(i),2)+pow(y0 * single1.GetErrorYlow(i),2));
    veyh(i) = TMath::Sqrt(pow(y1 * single0.GetErrorYhigh(i),2)+pow(y0 * single1.GetErrorYhigh(i),2));
    
  }

  TGraphAsymmErrors doubleEff(vx, vy, vexl, vexh, veyl, veyh);
  doubleEff.SetNameTitle((_name+"DoubleTag&Probe").c_str(), (_name+"DoubleTag&Probe").c_str());
  return doubleEff;
}

