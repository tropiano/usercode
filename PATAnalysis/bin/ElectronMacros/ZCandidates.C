#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <cstdlib>

#include "TFile.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"
#include "TTreePlayer.h"

using namespace std;

void Tokenize(const string& str,
                      vector<string>* tokens,
                      const string& delimiters = " ")
{
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        tokens->push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void ZCandidates(){

        TFile *data = TFile::Open("Data_17Jul.root");
        string output = "ZCandReport.txt";
        
        ofstream outlist;
        outlist.open(output.c_str());
        outlist<<endl;
        
        TTree *tree = (TTree*) data->Get("RecoElectronNtuple/ZeeTree");
        
        string t="tmp.txt";
       
        tree->SetScanField(0);    
        ((TTreePlayer*)(tree->GetPlayer()))->SetScanRedirect(true);
        ((TTreePlayer*)(tree->GetPlayer()))->SetScanFileName(t.c_str());
        
        tree->Scan("run:lumi:event:zmass:ncalojetsele:calojetet1:calojetet2:calojetet3:npfjetsele:pfjetet1:pfjetet2:pfjetet3","zmass_AllCuts>0");
     
        ifstream out1(t.c_str());       
        string s1;       
        map<int,string> ordout1;
        int linecounter1 = 0;
        while(getline(out1,s1)){
        vector<string> *tokens = new vector<string>();
        if(linecounter1==0){
        ordout1[1]=s1;
        }else{
        Tokenize(s1, tokens);
        vector<string>::iterator iter;
        int colcounter1 = 0;
        for(iter = tokens->begin(); iter!=tokens->end(); iter++){
        if(colcounter1==3)ordout1[atoi(iter->c_str())]=s1;
        colcounter1++;}
        }
        linecounter1++;
        delete tokens;
        }
  
        map<int,string>::iterator mapiter1;
        for(mapiter1 = ordout1.begin(); mapiter1 != ordout1.end(); mapiter1++){
        outlist<<mapiter1->second<<endl;}
        outlist<<endl<<endl;
        
        remove(t.c_str());
   
        tree->Scan("run:lumi:event:zmass:elept1:eleeta1:elephi1:elefbrem1:elept2:eleeta2:elephi2:elefbrem2","zmass_AllCuts>0");
        
        ifstream out2(t.c_str());       
        string s2;  
        map<int,string> ordout2;
        int linecounter2 = 0;
        while(getline(out2,s2)){
        vector<string> *tokens = new vector<string>();
        if(linecounter2==0){
        ordout2[1]=s2;
        }else{
        Tokenize(s2, tokens);
        vector<string>::iterator iter;
        int colcounter2 = 0;
        for(iter = tokens->begin(); iter!=tokens->end(); iter++){
        if(colcounter2==3)ordout2[atoi(iter->c_str())]=s2;
        colcounter2++;}
        }
        linecounter2++;
        delete tokens;
        }
  
        map<int,string>::iterator mapiter2;
        for(mapiter2 = ordout2.begin(); mapiter2 != ordout2.end(); mapiter2++){
        outlist<<mapiter2->second<<endl;}
        outlist<<endl<<endl;
       
        remove(t.c_str());
     
        outlist.close();
}


