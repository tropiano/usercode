
#include "Firenze/PATAnalysis/include/TagAndProbeAnalyzerMuon.h"

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
#include "RooVoigtian.h"
#include "RooBifurGauss.h"
#include "RooPolynomial.h"
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

TagAndProbeAnalyzerMuon::TagAndProbeAnalyzerMuon(TDirectory* input, TFile* output, TDirectory* sec_input, std::string dataset, bool performfits, TFile* training_signal, TFile* training_background):

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
_weight("weight", "weight", 0., 100.),
_passprobe_cat("passprobe", "passprobe"){
 
	_input->cd();

	_passprobe_cat.defineType("pass", 1);
	_passprobe_cat.defineType("fail", 0);

	TTree* tree=0; 
	TTree* tree1=0;

	tree = (TTree*) _input->Get(_dataset.c_str());
	if (!tree){
		std::cout << "Error in TagAndProbeAnalyzerMuon::TagAndProbeAnalyzerMuon : could not find tree named dataset in input file " << std::endl;
		return;
	}
	
	if(_sec_input){
		tree1 = (TTree*) _sec_input->Get(_dataset.c_str());
		if (!tree1){
			std::cout << "Error in TagAndProbeAnalyzerMuon::TagAndProbeAnalyzerMuon : could not find tree1 named dataset in input file " << std::endl;
			return;
		}
	}
	
	_initialized = true;
	
	_argset = new RooArgSet(_mass, _bin, _probe, _passprobe_cat, _weight );
 
	_output->cd();
	_rootree = new RooDataSet("roodataset", "roodataset", tree, *_argset, "", "weight");
	if(_sec_input) _rootree1 = new RooDataSet("roodataset", "roodataset", tree1, *_argset, "", "weight");
}


TagAndProbeAnalyzerMuon::~TagAndProbeAnalyzerMuon(){
	delete _argset;
}


void TagAndProbeAnalyzerMuon::analyze(unsigned int nbins, std::string option ){

	if (!_initialized) {
		std::cout << "Error in TagAndProbeAnalyzerMuon::Analyze, unable to initialize the RooDataSet. Are you sure that the TagAndProbeFiller has been run?" << std::endl;
		return;
	}
 
	_rootree->Write();
	
	if(_sec_input) _rootree1->Write();
	
	TGraphAsymmErrors singleEfficiency, singleEfficiency1;
	
	_YieldPlots = _output->mkdir("YieldPlots", "YieldPlots");
	
	_SignalYield_0 = new TGraphAsymmErrors();
	_SignalYield_0->SetNameTitle("SignalYield_0", "SignalYield_0");
	_SignalYield_1 = new TGraphAsymmErrors();
	_SignalYield_1->SetNameTitle("SignalYield_1", "SignalYield_1");
	
	_BackgroundYield_0 = new TGraphAsymmErrors();
	_BackgroundYield_0->SetNameTitle("BackgroundYield_0", "BackgroundYield_0");
	_BackgroundYield_1 = new TGraphAsymmErrors();
	_BackgroundYield_1->SetNameTitle("BackgroundYield_1", "BackgroundYield_1");
 
	for (unsigned int bin = 0; bin < nbins; ++bin){
	
		stringstream name_tp;
		name_tp << _name << "TagProbe" << bin << "bin_0";
		stringstream name_tp1;
		name_tp1 << _name << "TagProbe" << bin << "bin_1";
			
		stringstream name_tpp;
		name_tpp << _name << "TagPassProbe" << bin << "bin";
		
		stringstream formula_tp;
		formula_tp << "bin==" << bin << " && probe==1";
		
		RooAbsData* tagprobe=0; 
		RooAbsData* tagprobe1=0;
			
		tagprobe = _rootree->reduce(formula_tp.str().c_str());
		if(_sec_input) tagprobe1 = _rootree1->reduce(formula_tp.str().c_str());
		
		std::pair<RooFitResult*, RooRealVar*> tp_fit, tp_fit1;
			
		cout<<endl<<"### Fitting bin n. "<<bin<<endl<<endl;
			
		tp_fit = fit(tagprobe, name_tp.str().c_str(), option, bin);
		if(_sec_input) tp_fit1 = fit(tagprobe1, name_tp1.str().c_str(), option, bin);
			
		if(tp_fit.first && tp_fit.first->status() == 0 ){
			cout << "Using backgound correctedvalues for " << _name << " in bin " << bin << endl;
			singleEfficiency.SetPoint(bin, bin, tp_fit.second->getVal());
			double errlow = 0.;
			double errhigh = 0.;
			if (tp_fit.second->hasAsymError()){
				errhigh = TMath::Abs(tp_fit.second->getAsymErrorHi());
				errlow = TMath::Abs(tp_fit.second->getAsymErrorLo());
			}
			else {
				errlow = TMath::Abs(tp_fit.second->getError());
				errhigh = TMath::Abs(tp_fit.second->getError());
			}
			singleEfficiency.SetPointEYhigh(bin, errhigh);
			singleEfficiency.SetPointEYlow(bin, errlow);
		}
			
		if(_sec_input){ 
			if(tp_fit1.first && tp_fit1.first->status() == 0 ){
				cout << "Using backgound correctedvalues for " << _name << " in bin " << bin << endl;
				singleEfficiency1.SetPoint(bin, bin, tp_fit1.second->getVal());
				double errlow1 = 0.;
				double errhigh1 = 0.;
				if (tp_fit1.second->hasAsymError()){
					errhigh1 = TMath::Abs(tp_fit1.second->getAsymErrorHi());
					errlow1 = TMath::Abs(tp_fit1.second->getAsymErrorLo());
				}
				else{
					errlow1 = TMath::Abs(tp_fit1.second->getError());
					errhigh1 = TMath::Abs(tp_fit1.second->getError());
				}
				singleEfficiency1.SetPointEYhigh(bin, errhigh1);
				singleEfficiency1.SetPointEYlow(bin, errlow1);
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

	_YieldPlots->cd();
	
	_SignalYield_0->Write();
	_SignalYield_1->Write();
	_BackgroundYield_0->Write();
	_BackgroundYield_1->Write();
}


std::pair<RooFitResult*, RooRealVar*> TagAndProbeAnalyzerMuon::fit(RooAbsData* data, const char* name, std::string option, unsigned int bin) {

	string dirname(_input->GetName());

	cout << "bin mass" << _mass.getBins() << endl; 

	stringstream nllname;
	nllname << "nll_" << name;
	
	RooFitResult* training_results_signal = (_training_signal) ? (RooFitResult*)_training_signal->Get(nllname.str().c_str()) : 0;
	RooFitResult* training_results_background = (_training_background) ? (RooFitResult*)_training_background->Get(nllname.str().c_str()) : 0;

	TH1* hmass = data->createHistogram("mass", 100);
	
	RooRealVar mu_pass("mu_pass", "average_pass", 91., 80., 100.);
	RooRealVar width_pass("width_pass", "width_pass", 2, 0, 10);
	RooRealVar sigma_pass("sigma_pass", "sigma_pass", 1, 0, 20);
	RooRealVar d_pass("d_pass","d_pass", 10., -10., 20.);
	RooFormulaVar mu_pass2("mu_pass2","mu_pass+d_pass", RooArgList(mu_pass,d_pass));
	RooRealVar sigma_pass2("sigma_pass2", "sigma_pass2", hmass->GetRMS(), 2, 100);
	RooRealVar a_pass("a_pass", "a_pass", 10, 0, 100);
	RooRealVar n_pass("n_pass", "n_pass", 5, 0, 100);
	RooRealVar fraction_pass("fraction_pass", "fraction_pass", 0.9, 0., 1.);

	RooRealVar mu_fail("mu_fail", "average_fail", 91., 80., 100.);
	RooRealVar width_fail("width_fail", "width_fail", 2, 0, 10);
	RooRealVar sigma_fail("sigma_fail", "sigma_fail", 1, 0, 20);
	RooRealVar d_fail("d_fail","d_fail", 10., -10., 20.);
	RooFormulaVar mu_fail2("mu_fail2","mu_fail+d_fail", RooArgList(mu_fail,d_fail));
	RooRealVar sigma_fail2("sigma_fail2", "sigma_fail2", hmass->GetRMS(), 2, 100);
	RooRealVar a_fail("a_fail", "a_fail", 10, 0, 100);
	RooRealVar n_fail("n_fail", "n_fail", 5, 0, 100);
	RooRealVar fraction_fail("fraction_fail", "fraction_fail", 0.9, 0., 1.);
	
	RooVoigtian voigtian_pass("voigtian_pass", "voigtian_pass", _mass, mu_pass, width_pass, sigma_pass);
	RooCBShape crystal_pass("crystalball_pass", "crystalball_pass", _mass, mu_pass2, sigma_pass2, a_pass, n_pass);
	RooAddPdf signal_pass("signal_pass", "signal_pass", voigtian_pass, crystal_pass, fraction_pass);

	RooVoigtian voigtian_fail("voigtian_fail", "voigtian_fail", _mass, mu_fail, width_fail, sigma_fail);
	RooCBShape crystal_fail("crystalball_fail", "crystalball_fail", _mass, mu_fail2, sigma_fail2, a_fail, n_fail);
	RooAddPdf signal_fail("signal_fail", "signal_fail", voigtian_fail, crystal_fail, fraction_fail);
	
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
					//mu_pass.setConstant();
				}
				else if (parname == "sigma_pass"){
					sigma_pass.setVal(par->getVal());
					sigma_pass.setConstant();
				}
				else if (parname == "width_pass"){
					width_pass.setVal(par->getVal());
					width_pass.setConstant();
				}
				else if (parname == "d_pass"){
					d_pass.setVal(par->getVal());
					d_pass.setConstant();
				}
				else if (parname == "sigma_pass2"){
					sigma_pass2.setVal(par->getVal());
					sigma_pass2.setConstant();
				}
				else if (parname == "a_pass"){
					a_pass.setVal(par->getVal());
					a_pass.setConstant();
				}
				else if (parname == "n_pass"){
					n_pass.setVal(par->getVal());
					n_pass.setConstant();
				}
				else if (parname == "fraction_pass"){
					fraction_pass.setVal(par->getVal());
					fraction_pass.setConstant();
				}
				else if (parname == "mu_fail"){
					mu_fail.setVal(par->getVal());
					//mu_fail.setConstant();
				}
				else if (parname == "sigma_fail"){
					sigma_fail.setVal(par->getVal());
					sigma_fail.setConstant();
				}
				else if (parname == "width_fail"){
					width_fail.setVal(par->getVal());
					width_fail.setConstant();
				}
				else if (parname == "d_fail"){
					d_fail.setVal(par->getVal());
					d_fail.setConstant();
				}
				else if (parname == "sigma_fail2"){
					sigma_fail2.setVal(par->getVal());
					sigma_fail2.setConstant();
				}
				else if (parname == "a_fail"){
					a_fail.setVal(par->getVal());
					a_fail.setConstant();
				}
				else if (parname == "n_fail"){
					n_fail.setVal(par->getVal());
					n_fail.setConstant();
				}
				else if (parname == "fraction_fail"){
					fraction_fail.setVal(par->getVal());
					fraction_fail.setConstant();
				}
			}
		}
		else {//either the fit did not converge or was not retrieved 
			cout << "Error retrieving fit info from training signal! " << endl;
			return make_pair((RooFitResult*) 0, (RooRealVar*) 0);
		}
	} 
	
/*	//now the background - EXPONENTIAL
	RooFormulaVar centeredmass("centeredmass", "mass - 91.", RooArgList(_mass));
	RooRealVar c_pass("c_pass", "c_pass", -2, -100., 100.);
	RooExponential background_pass("exp_pass", "exp_pass", _mass, c_pass);
	RooRealVar c_fail("c_fail", "c_fail", -2., -100., 100.);
	RooExponential background_fail("exp_fail", "exp_fail", _mass, c_fail);
*/
	
	//now the background - POLYNOMIAL
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
		}
		else {//either the fit did not converge or was not retrieved 
			cout << "Error retrieving fit info from training background! " << endl;
			return make_pair((RooFitResult*) 0, (RooRealVar*) 0);
		}
	}	

	RooRealVar s("s", "signal yield", data->sumEntries(), 1, 300000);
	RooRealVar efficiency("efficiency", "efficiency", 0.9, 0., 0.999);
	RooRealVar b_pass("b_pass", "background yield pass", 5, 0, 10000);
	RooRealVar b_fail("b_fail", "background yield fail", 5, 0, 10000);
	
	RooFormulaVar s_fail("s_fail","s*(1.0 - efficiency)", RooArgList(s, efficiency) );
	RooFormulaVar s_pass("s_pass","s*efficiency", RooArgList(s, efficiency) );
	
/*	RooRealVar s1("s1", "signal1 yield", data->sumEntries(), 1, 300000);
	RooRealVar s2("s2", "signal2 yield", data->sumEntries(), 1, 300000);
	RooRealVar efficiency("efficiency", "efficiency", 0.9, 0., 0.999);
	RooRealVar b_pass("b_pass", "background yield pass", 5, 0, 10000);
	RooRealVar b_fail("b_fail", "background yield fail", 5, 0, 10000);
	
	RooFormulaVar s_fail("s_fail","s1", RooArgList(s1, efficiency) );
	RooFormulaVar s_pass("s_pass","s2", RooArgList(s2, efficiency) );
*/

	RooArgList components_pass;//(signal_pass);
	RooArgList coefficients_pass;//(s_pass);

	RooArgList components_fail;//(signal_fail);
	RooArgList coefficients_fail;//(s_fail);

	if (option == "signal"){
		components_pass.add(signal_pass);
		coefficients_pass.add(s_pass);
		components_fail.add(signal_fail);
		coefficients_fail.add(s_fail);
	}
	else if (option == "background"){
		components_pass.add(background_pass);
		coefficients_pass.add(b_pass);
		components_fail.add(background_fail);
		coefficients_fail.add(b_fail);
	}
	else if (option == "total" ) {
		components_pass.add(signal_pass);
		coefficients_pass.add(s_pass);
		components_fail.add(signal_fail);
		coefficients_fail.add(s_fail);
		components_pass.add(background_pass);
		coefficients_pass.add(b_pass);
		components_fail.add(background_fail);
		coefficients_fail.add(b_fail);
	}
	else {
		std::cout << "Error in TagAndProbeAnalyzerMuon::fit : Unknown option " << option << std::endl;
		return make_pair((RooFitResult*) 0, (RooRealVar*) 0);; 
	}

	RooAddPdf sumpass("sumpass", "sumpass", components_pass, coefficients_pass);
	RooAddPdf sumfail("sumfail", "sumfail", components_fail, coefficients_fail);
	
	RooSimultaneous total_fit("total_fit","total_fit", _passprobe_cat);
	_passprobe_cat.setLabel("pass");
	total_fit.addPdf(sumpass, _passprobe_cat.getLabel());
	_passprobe_cat.setLabel("fail");
	total_fit.addPdf(sumfail, _passprobe_cat.getLabel());
	
	RooFitResult* fitresult = total_fit.fitTo(*((RooDataSet*) data), RooFit::Minos(RooArgSet(efficiency)), RooFit::Save(true), RooFit::PrintLevel(3), RooFit::Strategy(2), RooFit::NumCPU(8), RooFit::Optimize(false), RooFit::Extended(kTRUE), RooFit::SumW2Error(true));
		
	fitresult->SetName(nllname.str().c_str());

	fitresult->Write();
	
	stringstream eff;
	eff << "Efficiency = ";
	eff << ((float)(round(efficiency.getVal()*10000)))/10000;
	eff << " +/- ";
	eff << ((float)(round(efficiency.getPropagatedError(*fitresult)*10000)))/10000;

	stringstream name_pass;
	name_pass << name << "PassProbe";
	stringstream name_fail;
	name_fail << name << "FailProbe";
	
	RooPlot * mass_pass = _mass.frame() ;
	mass_pass->SetNameTitle(name_pass.str().c_str(), name_pass.str().c_str());
	data->plotOn(mass_pass, RooFit::Cut("passprobe==passprobe::pass"), RooFit::DataError(RooAbsData::SumW2));
	mass_pass->GetXaxis()->SetLabelFont(42);
	mass_pass->GetXaxis()->SetTitleFont(42);
	mass_pass->GetXaxis()->SetTitleSize(0.06);
	mass_pass->GetYaxis()->SetLabelFont(42);
	mass_pass->GetYaxis()->SetTitleFont(42);
	mass_pass->GetYaxis()->SetTitleSize(0.06);
	
	total_fit.plotOn(mass_pass, RooFit::Slice(_passprobe_cat, "pass"), RooFit::ProjWData(_passprobe_cat,*data));
 
	stringstream spass;
	spass << "Signal = ";
	spass << ((float)(round(s_pass.getVal()*10)))/10;
	spass << " +/- ";
	spass << ((float)(round(s_pass.getPropagatedError(*fitresult)*10)))/10;
	
	string type_pass = "";
	type_pass+=name[strlen(name)-1];
	
	if(type_pass=="0"){
		_SignalYield_0->SetPoint(bin, bin, s_pass.getVal());
		_SignalYield_0->SetPointEYhigh(bin,s_pass.getPropagatedError(*fitresult));
		_SignalYield_0->SetPointEYlow(bin,s_pass.getPropagatedError(*fitresult));
	}
	if(type_pass=="1"){
		_SignalYield_1->SetPoint(bin, bin, s_pass.getVal());
		_SignalYield_1->SetPointEYhigh(bin,s_pass.getPropagatedError(*fitresult));
		_SignalYield_1->SetPointEYlow(bin,s_pass.getPropagatedError(*fitresult));
	}
	
	stringstream bpass;
	bpass << "Background = ";
	bpass << ((float)(round(b_pass.getVal()*10)))/10;
	bpass << " +/- ";
	bpass << ((float)(round(b_pass.getPropagatedError(*fitresult)*10)))/10;
	
	if(type_pass=="0"){
		_BackgroundYield_0->SetPoint(bin, bin, b_pass.getVal());
		_BackgroundYield_0->SetPointEYhigh(bin,b_pass.getPropagatedError(*fitresult));
		_BackgroundYield_0->SetPointEYlow(bin,b_pass.getPropagatedError(*fitresult));
	}
	if(type_pass=="1"){
		_BackgroundYield_1->SetPoint(bin, bin, b_pass.getVal());
		_BackgroundYield_1->SetPointEYhigh(bin,b_pass.getPropagatedError(*fitresult));
		_BackgroundYield_1->SetPointEYlow(bin,b_pass.getPropagatedError(*fitresult));
	}
	
	TPaveText* tboxpass = new TPaveText(0.647651, 0.695804, 0.949664, 0.917832, "BRNDC");
	tboxpass->SetFillColor(0);
	tboxpass->SetShadowColor(0);
	tboxpass->AddText(spass.str().c_str());
	tboxpass->AddText(bpass.str().c_str());
	tboxpass->AddText(eff.str().c_str());
	mass_pass->addObject(tboxpass);
	
	data->statOn(mass_pass, RooFit::Cut("passprobe==passprobe::pass"), RooFit::DataError(RooAbsData::SumW2));
	
	total_fit.plotOn(mass_pass, Components(background_pass), RooFit::Slice(_passprobe_cat,"pass"), RooFit::ProjWData(_passprobe_cat,*data), LineStyle(kDashed));
	total_fit.plotOn(mass_pass, Components(signal_pass), RooFit::Slice(_passprobe_cat,"pass"), RooFit::ProjWData(_passprobe_cat,*data), LineStyle(kDashed));
		
	mass_pass->Write();

	RooPlot * mass_fail = _mass.frame() ;
	mass_fail->SetNameTitle(name_fail.str().c_str(), name_fail.str().c_str());
	data->plotOn(mass_fail, RooFit::Cut("passprobe==passprobe::fail"), RooFit::DataError(RooAbsData::SumW2));
	mass_fail->GetXaxis()->SetLabelFont(42);
	mass_fail->GetXaxis()->SetTitleFont(42);
	mass_fail->GetXaxis()->SetTitleSize(0.06);
	mass_fail->GetYaxis()->SetLabelFont(42);
	mass_fail->GetYaxis()->SetTitleFont(42);
	mass_fail->GetYaxis()->SetTitleSize(0.06);
	
	total_fit.plotOn(mass_fail, RooFit::Slice(_passprobe_cat, "fail"), RooFit::ProjWData(_passprobe_cat,*data));
	
	stringstream sfail;
	sfail << "Signal = ";
	sfail << ((float)(round(s_fail.getVal()*10)))/10;
	sfail << " +/- ";
	sfail << ((float)(round(s_fail.getPropagatedError(*fitresult)*10)))/10;
	
	stringstream bfail;
	bfail << "Background = ";
	bfail << ((float)(round(b_fail.getVal()*10)))/10;
	bfail << " +/- ";
	bfail << ((float)(round(b_fail.getPropagatedError(*fitresult)*10)))/10;
	
	TPaveText* tboxfail = new TPaveText(0.647651, 0.695804, 0.949664, 0.917832, "BRNDC");
	tboxfail->SetFillColor(0);
	tboxfail->SetShadowColor(0);
	tboxfail->AddText(sfail.str().c_str());
	tboxfail->AddText(bfail.str().c_str());
	tboxfail->AddText(eff.str().c_str());
	mass_fail->addObject(tboxfail);
	
	data->statOn(mass_fail, RooFit::Cut("passprobe==passprobe::fail"), RooFit::DataError(RooAbsData::SumW2));
	
	total_fit.plotOn(mass_fail, Components(background_fail), RooFit::Slice(_passprobe_cat,"fail"), RooFit::ProjWData(_passprobe_cat,*data), LineStyle(kDashed));
	total_fit.plotOn(mass_fail, Components(signal_fail), RooFit::Slice(_passprobe_cat,"fail"), RooFit::ProjWData(_passprobe_cat,*data), LineStyle(kDashed));
	
	mass_fail->Write();
	
	delete hmass;
	delete mass_pass;
	delete mass_fail;
	return std::make_pair(fitresult, (RooRealVar*) efficiency.Clone() );
 
}


TGraphAsymmErrors TagAndProbeAnalyzerMuon::createDoubleEfficiency(const TGraphAsymmErrors& single) const {
	
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
		vx(i) = x;
		vexl(i) = TMath::Abs(single.GetErrorXlow(i));
		vexh(i) = TMath::Abs(single.GetErrorXhigh(i));
		vy(i) = y * y;
		veyl(i) = TMath::Abs(2 * y * single.GetErrorYlow(i));
		veyh(i) = TMath::Abs(2 * y * single.GetErrorYhigh(i));
	}

	TGraphAsymmErrors doubleEff(vx, vy, vexl, vexh, veyl, veyh);
	doubleEff.SetNameTitle((_name+"DoubleTag&Probe").c_str(), (_name+"DoubleTag&Probe").c_str());
	return doubleEff;
}


TGraphAsymmErrors TagAndProbeAnalyzerMuon::createAsymmCutEfficiency(const TGraphAsymmErrors& single0, const TGraphAsymmErrors& single1) const {

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
		
		vx(i) = x0;
		vexl(i) = TMath::Abs(single0.GetErrorXlow(i));
		vexh(i) = TMath::Abs(single0.GetErrorXhigh(i));
		vy(i) = y0 * y1;
		veyl(i) = TMath::Sqrt(pow(y1 * single0.GetErrorYlow(i),2)+pow(y0 * single1.GetErrorYlow(i),2));
		veyh(i) = TMath::Sqrt(pow(y1 * single0.GetErrorYhigh(i),2)+pow(y0 * single1.GetErrorYhigh(i),2));
		
	}

	TGraphAsymmErrors doubleEff(vx, vy, vexl, vexh, veyl, veyh);
	doubleEff.SetNameTitle((_name+"DoubleTag&Probe").c_str(), (_name+"DoubleTag&Probe").c_str());
	return doubleEff;
}

