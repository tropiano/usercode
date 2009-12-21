#include "Firenze/PATAnalysis/include/FWLiteTSelector.h"

// system include files
#include "TProofOutputFile.h"
#include "TFile.h"
#include "TTree.h"
#include "boost/shared_ptr.hpp"
#include "FWCore/Utilities/interface/Algorithms.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
FWLiteTSelector::
FWLiteTSelector() : 
  everythingOK_(false)  
{ std::cout << "Building a FWLiteTSelector " << std::endl;}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
FWLiteTSelector::
~FWLiteTSelector(){}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
FWLiteTSelector::
Begin(TTree * iTree) {   
  Init(iTree); 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
FWLiteTSelector::
SlaveBegin(TTree *iTree) { 

  //ifstream configfile;
  std::string configfile;

  Init(iTree);
  TIter next(fInput);
  //bool fileSet(false);
  bool configFileSet(false);
  while (TObject* obj = next()){
    TNamed* parFile   = dynamic_cast<TNamed*>(obj);
    if (parFile) {
      //if (!strcmp(parFile->GetName(), "OutputFile")){
      //  _fProofFile = new TProofOutputFile(parFile->GetTitle(),"LOCAL");
      //  std::cout << "set output file to: " << parFile->GetTitle() << std::endl;
      //  fileSet = true;
      //} else if (!strcmp(parFile->GetName(), "ConfigFile")){
      if (!strcmp(parFile->GetName(), "ConfigFile")){ 
        //configfile.open(parFile->GetTitle());
        configfile = parFile->GetTitle();
        configFileSet = true;
      }
    }
  }

  if (!configFileSet){
    std::cout << "you did not set the config file!" << std::endl;
  }
  assert(configFileSet); 

  //configfile.open("config.txt");

  //assert(configfile.is_open());
  //std::map<std::string, std::vector<std::string> > configMap;
  //handleConfigStream(configfile, configMap);
 
  //std::cout << configMap.begin()->first << " " << configMap.begin()->second.front() << std::endl;
 
  //check that everything was set       
  //if (!fileSet) {
  //  std::cout << "You did not set the output file! " << std::endl;
  //  assert(fileSet);
  //}

  PythonProcessDesc builder(configfile);
  boost::shared_ptr<edm::ParameterSet> ps = builder.processDesc()->getProcessPSet();
  std::cout << *ps << std::endl;

  edm::ParameterSet outfileps = ps->getParameter<edm::ParameterSet>("outputFile");
  std::string outfileName = outfileps.getParameter<std::string>("name");
  _fProofFile = new TProofOutputFile(outfileName.c_str(), "LOCAL");
  TFile* unique = _fProofFile->OpenFile("RECREATE");
 
  //now get the config of the modules
  std::vector<std::string> modules;
  int nmodules = ps->getParameterSetNames(modules);
  std::cout << "You are running " << nmodules-1 << " analysis modules" << std::endl;
  for (unsigned int i = 0; i < modules.size(); ++i) std::cout << modules[i] << std::endl;

  AnalysisBuilders abs = getAnalysisBuilders();
  
  //std::map<std::string, std::vector<std::string> >::const_iterator iConfig;
  //std::map<std::string, std::vector<std::string> >::const_iterator ibeg = configMap.begin();
  //std::map<std::string, std::vector<std::string> >::const_iterator iend = configMap.end();
  
  std::vector<std::string>::const_iterator iConfig;
  std::vector<std::string>::const_iterator ibeg = modules.begin();
  std::vector<std::string>::const_iterator iend = modules.end();
 
  for (iConfig = ibeg; iConfig != iend; ++iConfig ){
    edm::ParameterSet currentConfig = ps->getParameter<edm::ParameterSet>(*iConfig);
    std::cout << currentConfig << std::endl;
    std::string modname = *iConfig;
    if (!currentConfig.exists("type")) continue;
    std::string modtype = currentConfig.getParameter<std::string>("type");
    std::cout << "Configuring module " << modname << " of type " << modtype << std::endl;
    
    //AnalysisBuilders::iterator ab = abs.find(iConfig->second.front());
    AnalysisBuilders::iterator ab = abs.find(modtype);
    if (ab != abs.end()){
      //_fwLiteTreeAnalyser =  new BackgroundWorker(_fProofFile);
      _workers.push_back(boost::shared_ptr<MyPatAnalysis>(ab->second()) ) ;
      //_workers.back()->begin(unique, fInput);
      _workers.back()->begin(unique, currentConfig);
    } /*else {
      std::cout << "Cannot load symbol GenMuon" << std::endl;
      assert(0);
    }*/
  } 
  //_fwLiteTreeAnalyser->init(myAnalysers);

}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
FWLiteTSelector::
Init(TTree *iTree) { 
  std::cout<<"FWLiteTSelector::Init() "<<iTree<<std::endl;
  if(iTree==0) return;
  tree_ = iTree;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Bool_t
FWLiteTSelector::
Notify() { 

  TFile* file = tree_->GetCurrentFile();
  setupNewFile(*file);
  return everythingOK_ ? kTRUE: kFALSE; 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Bool_t
FWLiteTSelector::
Process(Long64_t iEntry) { 
/*
  for(int i=_lastEntry;i<iEntry;++i){
     ++(*event_);
     //++tmp; 
  }
*/  
  const fwlite::Event& test = event_->to(iEntry);
/*  
  //std::cout<<test.id()<<std::endl;
  //if(test.id().run()>0) _fwLiteTreeAnalyser->process(const_cast<fwlite::ChainEvent&>(test));
  _fwLiteWorker1->process(const_cast<fwlite::ChainEvent&>(*event_));
  _fwLiteWorker2->process(const_cast<fwlite::ChainEvent&>(*event_));
  _fwLiteWorker3->process(const_cast<fwlite::ChainEvent&>(*event_));
  _fwLiteWorker4->process(const_cast<fwlite::ChainEvent&>(*event_));
  _fwLiteWorker5->process(const_cast<fwlite::ChainEvent&>(*event_));
  _lastEntry = iEntry;
*/
  for (unsigned int i = 0; i < _workers.size(); ++i ){
    _workers[i]->process(const_cast<fwlite::Event&>(test)); 
  }  
  
  //_fwLiteTreeAnalyser->analyse(const_cast<fwlite::ChainEvent&>(event_->to(iEntry)));
  //_fwLiteTreeAnalyser->analyse(const_cast<fwlite::ChainEvent&>(++(*event_)));

  return kTRUE; 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
FWLiteTSelector::
SlaveTerminate() { 

  std::cout<<"ThingsTSelector::postProcessing()"<<std::endl;
  for (unsigned int i = 0; i < _workers.size(); ++i ){
    _workers[i]->finalize();
  }  
  //delete _fwLiteTreeAnalyser;
 // Write the ntuple to the file
  fOutput->Add(_fProofFile);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
FWLiteTSelector::
Terminate() {
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
FWLiteTSelector::
setupNewFile(TFile& iFile) { 

  //std::vector<std::string> fileVector;
  //fileVector.push_back(iFile.GetName());

  std::cout<<"fileName: "<<iFile.GetName()<<std::endl;
  //event_ = boost::shared_ptr<fwlite::ChainEvent>( new fwlite::ChainEvent(fileVector));
  event_ = boost::shared_ptr<fwlite::Event>( new fwlite::Event(&iFile));
  std::cout<<"size: "<<event_->size()<<std::endl;
  everythingOK_ = true;

}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

