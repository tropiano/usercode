
#include "Firenze/PATAnalysis/include/TagAndProbeAnalyzer.h"

#include <iostream>
#include <sstream>

#include "RooDataHist.h"
#include "RooPlot.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "RooMinuit.h"
#include "RooFormulaVar.h"
#include "RooVoigtian.h"
#include "RooBifurGauss.h"
#include "RooSimultaneous.h"
#include "RooChi2Var.h"
#include "RooNLLVar.h"
#include "TTree.h"
#include "TH1.h"
#include "TVectorD.h"

using namespace std;

TagAndProbeAnalyzer::TagAndProbeAnalyzer(TDirectory* input, TFile* output, std::string name, bool performfits, TFile* training_signal, TFile* training_background):
_initialized(false),
_input(input),
_output(output),
_name(name),
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

  TTree* tree = (TTree*) _input->Get("dataset");
  if (!tree){
    std::cout << "Error in TagAndProbeAnalyzer::TagAndProbeAnalyzer : could not find tree named dataset in input file " << std::endl;
    return;
  }
  _initialized = true;
  
  _argset = new RooArgSet(_mass, _bin, _probe, _passprobe_cat, _weight );
 
  _output->cd();
  _rootree   = new RooDataSet("roodataset", "roodataset", tree, *_argset, "", "weight");
}

TagAndProbeAnalyzer::~TagAndProbeAnalyzer(){
  delete _argset;
}

void TagAndProbeAnalyzer::analyze(unsigned int nbins, std::string option )
{
   if (!_initialized) {
      std::cout << "Error in TagAndProbeAnalyzer::Analyze, unable to initialize the RooDataSet. Are you sure that the TagAndProbeFiller has been run?" << std::endl;
      return;  
   }
  
   _rootree->Write();
   
   TGraphAsymmErrors singleEfficiency;
  
   for (unsigned int i = 0; i < nbins; ++i){
      stringstream name_tp;
      name_tp << _name << "TagProbe" << i << "bin";
      stringstream name_tpp;
      name_tpp << _name << "TagPassProbe" << i << "bin";
     
      stringstream formula_tp;
      formula_tp << "bin==" << i << " && probe==1";
     
      RooAbsData* tagprobe = _rootree->reduce(formula_tp.str().c_str());
      
      //_mass.setMin(80.); //TRY smaller range
      //_mass.setMax(100.); //TRY smaller range
    
        std::pair<RooFitResult*, RooRealVar*>  tp_fit  = fit(tagprobe, name_tp.str().c_str(), option);
        
        if(tp_fit.first && tp_fit.first->status() == 0 ){
          cout << "Using backgound correctedvalues for " << _name << " in bin " << i << endl;  
          singleEfficiency.SetPoint(i, i, tp_fit.second->getVal());
          double errlow = 0.;
          double errhigh = 0.;
          if (tp_fit.second->hasAsymError()){
            errhigh = tp_fit.second->getAsymErrorHi();
            errlow  = tp_fit.second->getAsymErrorLo();
          } else {
            errlow  = tp_fit.second->getError();
            errhigh = tp_fit.second->getError();
          }
          singleEfficiency.SetPointEYhigh(i, errhigh);
          singleEfficiency.SetPointEYlow(i, errlow);
          } 
                  
          if (tp_fit.first) delete tp_fit.first;
          if (tp_fit.second) delete tp_fit.second;
   }

   //double muon efficiency
   TGraphAsymmErrors doubleEfficiency = createDoubleMuonEfficiency(singleEfficiency);
   doubleEfficiency.Write(); 
   
}


std::pair<RooFitResult*, RooRealVar*> TagAndProbeAnalyzer::fit(RooAbsData* data, const char* name, std::string option) {

  string dirname(_input->GetName());

  cout << "bins mass" << _mass.getBins()  << endl; 

  stringstream nllname;
  nllname << "nll_" << name;

  //if we have training retrieve results
  RooFitResult* training_results_signal     = (_training_signal)     ? (RooFitResult*) _training_signal    ->Get(  (dirname + "/" + nllname.str()).c_str()  ) : 0;
  RooFitResult* training_results_background = (_training_background) ? (RooFitResult*) _training_background->Get(  (dirname + "/" + nllname.str()).c_str()  ) : 0;

  //_mass.setBins(20);
  //RooDataHist roobinned("bdata","Binned Data", RooArgSet(_mass, _passprobe_cat),*data);

  TH1* hmass = data->createHistogram("mass", 100);
/*
  RooRealVar mu_pass("mu_pass", "average_pass", hmass->GetMean(),  80, 100);
  RooRealVar mu_pass2("mu_pass2", "average_pass2", hmass->GetMean(), 80, 100);
  RooRealVar width_pass("width_pass", "width_pass", hmass->GetRMS(), 0, 10);
  RooRealVar sigma_pass("sigma_pass", "sigma_pass", hmass->GetRMS(), 0, 20);
  
  RooRealVar widthL_pass("widthL_pass", "widthL_pass", 5, 0, 30);
  RooRealVar widthR_pass("widthR_pass", "widthR_pass", 4, 0, 15);
  RooRealVar fraction_pass("fraction_pass", "fraction_pass", 0.9, 0., 1.);


  RooRealVar mu_fail("mu_fail", "average_fail", hmass->GetMean(), 80, 100);
  RooRealVar mu_fail2("mu_fail2", "average_fail2", hmass->GetMean(), 80, 100);
  RooRealVar width_fail("width_fail", "width_fail", hmass->GetRMS(), 0, 10);
  RooRealVar sigma_fail("sigma_fail", "sigma_fail", hmass->GetRMS(), 0, 20);

  RooRealVar widthL_fail("widthL_fail", "widthL_fail", 5, 0, 30);
  RooRealVar widthR_fail("widthR_fail", "widthR_fail", 4, 0, 15);
  RooRealVar fraction_fail("fraction_fail", "fraction_fail", 0.9, 0., 1.);
*/
  RooRealVar mu_pass("mu_pass", "average_pass", 90, 80, 100);
  RooRealVar mu_pass2("mu_pass2", "average_pass2", 90, 80, 100);
  RooRealVar width_pass("width_pass", "width_pass", 2, 0, 10);
  RooRealVar sigma_pass("sigma_pass", "sigma_pass", 1, 0, 20);
  
  RooRealVar widthL_pass("widthL_pass", "widthL_pass", 15, 2, 30);
  RooRealVar widthR_pass("widthR_pass", "widthR_pass", 4, 2, 15);
  RooRealVar fraction_pass("fraction_pass", "fraction_pass", 0.9, 0., 1.);

  RooRealVar mu_fail("mu_fail", "average_fail", 90, 80, 100);
  RooRealVar mu_fail2("mu_fail2", "average_fail2", 90, 80, 100);
  RooRealVar width_fail("width_fail", "width_fail", 2, 0, 10);
  RooRealVar sigma_fail("sigma_fail", "sigma_fail", 1, 0, 20);

  RooRealVar widthL_fail("widthL_fail", "widthL_fail", 15, 2, 30);
  RooRealVar widthR_fail("widthR_fail", "widthR_fail", 4, 2, 15);
  RooRealVar fraction_fail("fraction_fail", "fraction_fail", 0.9, 0., 1.); 

  RooVoigtian voigtian_pass("voigtian_pass", "voigtian_pass", _mass, mu_pass, width_pass, sigma_pass);
  RooBifurGauss bifurgauss_pass("bifurgauss_pass", "bifurgauss_pass", _mass, mu_pass2, widthL_pass, widthR_pass); //try not to force the same mu_pass
  RooAddPdf signal_pass("signal_pass", "signal_pass", voigtian_pass, bifurgauss_pass, fraction_pass);
  //RooVoigtian signal_pass("voigtian_pass", "voigtian_pass", _mass, mu_pass, width_pass, sigma_pass);

  RooVoigtian voigtian_fail("voigtian_fail", "voigtian_fail", _mass, mu_fail, width_fail, sigma_fail);
  RooBifurGauss bifurgauss_fail("bifurgauss_fail", "bifurgauss_fail", _mass, mu_fail2, widthL_fail, widthR_fail);
  RooAddPdf signal_fail("signal_fail", "signal_fail", voigtian_fail, bifurgauss_fail, fraction_fail);
  //RooVoigtian signal_fail("voigtian_fail", "voigtian_fail", _mass, mu_fail, width_fail, sigma_fail);;
   
  // if we are using signal shape from training 
  if (_training_signal) {
  cout<<"TRAINING SIGNAL 1"<<endl;
    //if the training fit was succesfull and succesfully retrieved
    if ( training_results_signal && training_results_signal->status() == 0){ 
    cout<<"TRAINING SIGNAL 2"<<endl;
      for (int i = 0; i < training_results_signal->floatParsFinal().getSize(); ++i) { 
        RooRealVar* par = (RooRealVar*) training_results_signal->floatParsFinal().at(i); 
        string parname(par->GetName());
        if (parname == "mu_pass"){
          mu_pass.setVal(par->getVal());
          mu_pass.setConstant();
        } 
          else if (parname == "mu_pass2"){
          mu_pass2.setVal(par->getVal());
          mu_pass2.setConstant();
        } else if (parname == "width_pass"){
          width_pass.setVal(par->getVal());
          width_pass.setConstant();
        } else if (parname == "sigma_pass"){
          sigma_pass.setVal(par->getVal());
          sigma_pass.setConstant();
        } else if (parname == "widthL_pass"){
          widthL_pass.setVal(par->getVal());
          widthL_pass.setConstant();
        } else if (parname == "widthR_pass"){
          widthR_pass.setVal(par->getVal());
          widthR_pass.setConstant();
        } else if (parname == "fraction_pass"){
          fraction_pass.setVal(par->getVal());
          fraction_pass.setConstant();
        } else if (parname == "mu_fail"){
          mu_fail.setVal(par->getVal());
          mu_fail.setConstant();
        }
        else if (parname == "mu_fail2"){
          mu_fail2.setVal(par->getVal());
          mu_fail2.setConstant();
        }
        else if (parname == "width_fail"){
          width_fail.setVal(par->getVal());
          width_fail.setConstant();
        }
        else if (parname == "sigma_fail"){
          sigma_fail.setVal(par->getVal());
          sigma_fail.setConstant();
        } else if (parname == "widthL_fail"){
          widthL_fail.setVal(par->getVal());
          widthL_fail.setConstant();
        } else if (parname == "widthR_fail"){
          widthR_fail.setVal(par->getVal());
          widthR_fail.setConstant();
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

  //now the background
  RooFormulaVar centeredmass("centeredmass", "mass - 91.", RooArgList(_mass));
  RooRealVar c0_pass("c0_pass", "c0_pass", 1000000.);
  RooRealVar c1_pass("c1_pass", "c1_pass", -0.1, -2., 2.);
  RooRealVar c2_pass("c2_pass", "c2_pass", -0.01, -1., 0.);
  RooPolynomial background_pass("poly_pass", "poly_pass", _mass, RooArgList(c0_pass, c1_pass, c2_pass), 0);

  RooRealVar c0_fail("c0_fail", "c0_fail", 1000000.);
  RooRealVar c1_fail("c1_fail", "c1_fail", -0.1, -2., 2.);
  RooRealVar c2_fail("c2_fail", "c2_fail", -0.01, -1., 0.);
  RooPolynomial background_fail("poly_fail", "poly_fail", _mass, RooArgList(c0_fail, c1_fail, c2_fail), 0); 


  if (_training_background) {
  cout<<"TRAINING BACKGROUND 1"<<endl;
    //if the training fit was succesfull and succesfully retrieved
    if ( training_results_background && training_results_background->status() == 0){
    cout<<"TRAINING BACKGROUND 2"<<endl;
      for (int i = 0; i < training_results_background->floatParsFinal().getSize(); ++i) {
        RooRealVar* par = (RooRealVar*) training_results_background->floatParsFinal().at(i);
        string parname(par->GetName());
        if (parname == "c0_pass"){
          c0_pass.setVal(par->getVal());
          c0_pass.setConstant();
        }
        else if (parname == "c1_pass"){
          c1_pass.setVal(par->getVal());
          c1_pass.setConstant();
        }
        else if (parname == "c2_pass"){
          c2_pass.setVal(par->getVal());
          c2_pass.setConstant();
        }
        else if (parname == "c0_fail"){
          c0_fail.setVal(par->getVal());
          c0_fail.setConstant();
        }
        else if (parname == "c1_fail"){
          c1_fail.setVal(par->getVal());
          c1_fail.setConstant();
        }
        else if (parname == "c2_fail"){
          c2_fail.setVal(par->getVal());
          c2_fail.setConstant();
        }
      }
    } else {  //either the fit did not converge or was not retrieved 
       cout << "Error retrieving fit info from training background! " << endl;
       return make_pair((RooFitResult*) 0, (RooRealVar*) 0);
    }
  }   
 
  RooRealVar s("s", "signal yield", data->sumEntries(), 1, 300000);
  RooRealVar efficiency("efficiency", "efficiency", 0.9, 0., 0.999);
  RooRealVar b_pass("b_pass", "background yield pass", 50, 0, 1000);
  RooRealVar b_fail("b_fail", "background yield fail", 50, 0, 1000);

  RooFormulaVar s_pass("s_pass","s*efficiency", RooArgList(s, efficiency) );
  RooFormulaVar s_fail("s_fail","s*(1.0 - efficiency)", RooArgList(s, efficiency) );
  //RooFormulaVar s_pass("s_pass","s*efficiency", RooArgList(efficiency) );
  //RooFormulaVar s_fail("s_fail","s*(1.0 - efficiency)", RooArgList(efficiency) ); 

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
    std::cout << "Error in TagAndProbeAnalyzer::fit : Unknown option " << option << std::endl;
    return make_pair((RooFitResult*) 0, (RooRealVar*) 0);;   
  }

  RooAddPdf sumpass("sumpass", "sumpass", components_pass, coefficients_pass);
  RooAddPdf sumfail("sumfail", "sumfail", components_fail, coefficients_fail);

  RooSimultaneous total_fit("total_fit","total_fit", _passprobe_cat);
  _passprobe_cat.setLabel("pass");
  //total_fit.addPdf(signal_pass, _passprobe_cat.getLabel());
  total_fit.addPdf(sumpass, _passprobe_cat.getLabel());
  _passprobe_cat.setLabel("fail");
  //total_fit.addPdf(signal_fail, _passprobe_cat.getLabel());
  total_fit.addPdf(sumfail, _passprobe_cat.getLabel());


  RooFitResult* fitresult = total_fit.fitTo(*data, RooFit::Save(true), RooFit::Strategy(2), RooFit::NumCPU(8), RooFit::Extended(kTRUE), RooFit::SumW2Error(true));

  fitresult->Write(); 

  stringstream name_pass;
  name_pass << name << "PassProbe";
  stringstream name_fail;
  name_fail << name << "FailProbe";   
  RooPlot * mass_pass = _mass.frame() ;
  mass_pass->SetNameTitle(name_pass.str().c_str(), name_pass.str().c_str());
  data->plotOn(mass_pass, RooFit::Cut("passprobe==passprobe::pass"), RooFit::DataError(RooAbsData::SumW2)) ;
  //roobinned.plotOn(mass_pass, RooFit::Cut("passprobe==passprobe::pass"),  RooFit::DataError(RooDataHist::SumW2)) ;
  total_fit.plotOn(mass_pass, RooFit::Slice(_passprobe_cat, "pass"), RooFit::ProjWData(_passprobe_cat,*data));
  //signal_pass.plotOn(mass_pass);
  mass_pass->Write();

  RooPlot * mass_fail = _mass.frame() ;
  mass_fail->SetNameTitle(name_fail.str().c_str(), name_fail.str().c_str());
  data->plotOn(mass_fail, RooFit::Cut("passprobe==passprobe::fail"),  RooFit::DataError(RooAbsData::SumW2)) ;
  //roobinned.plotOn(mass_fail, RooFit::Cut("passprobe==passprobe::fail"),  RooFit::DataError(RooDataHist::SumW2)) ;
  _passprobe_cat.setLabel("fail");
  total_fit.plotOn(mass_fail, RooFit::Slice(_passprobe_cat, "fail"), RooFit::ProjWData(_passprobe_cat,*data));
  mass_fail->Write(); 
    
  //roobinned.Write();
  
  delete hmass;
  delete mass_pass;
  delete mass_fail;
  return std::make_pair(fitresult, (RooRealVar*) efficiency.Clone() );
}


TGraphAsymmErrors TagAndProbeAnalyzer::createDoubleMuonEfficiency(const TGraphAsymmErrors& single) const {
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
    vexl(i) = single.GetErrorXlow(i);
    vexh(i) = single.GetErrorXhigh(i);
    vy(i)   = y * y;
    veyl(i) = 2 * y * single.GetErrorYlow(i);
    veyh(i) = 2 * y * single.GetErrorYhigh(i);
  }

  TGraphAsymmErrors doubleMuEff(vx, vy, vexl, vexh, veyl, veyh);
  doubleMuEff.SetNameTitle((_name+"DoubleMuTag&Probe").c_str(), (_name+"DoubleMuTag&Probe").c_str());
  return doubleMuEff;
} 

