#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>

#include "TFile.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#include "TTree.h"
//#include "TTreeStream.h"

using namespace std;

//sample == "mc" -> MonteCarlo , sample == "data" -> Data
//Selections: "VPJ" = V+jets selections (old); "VBTF" = Vector Boson Task Force (new)

void ZCandidates(){

        TFile *data = TFile::Open("ZJets_Madgraph_Spring10_TEST.root");
        
        TTree *tree = (TTree*) data->Get("RecoElectronNtuple/ZeeTree");
       
        tree->SetScanField(0);
        
        tree->Scan("run:lumi:event:zmass:ncalojetsele:calojetet1:calojetet2:calojetet3:npfjetsele:pfjetet1:pfjetet2:pfjetet3","zmass_AllCuts>0");
     
        tree->Scan("run:lumi:event:zmass:elept1:eleeta1:elephi1:elefbrem1:elept2:eleeta2:elephi2:elefbrem2","zmass_AllCuts>0");
     
}
