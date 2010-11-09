#define BtagHLT_cxx
#include "BtagHLT.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TLegend.h>
#include <TGraphErrors.h>

void BtagHLT::Loop()
{
  //   In a ROOT session, you can do:
  //      Root > .L BtagHLT.C
  //      Root > BtagHLT t
  //      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain == 0) return;
  TFile *eff = new TFile("../skims/btagHLT.root","RECREATE");
  
  TH1F *RecoDiscr = new TH1F("RecoDiscr","Reco Discr", 100, -10,10);
  TH1F *L3Discr = new TH1F("L3Discr","Reco Discr", 100, -10,10);
  TH1F *RecoIP = new TH1F("RecoIP","Reco IP", 100, -10,10);
  TH1F *track_dz_reco = new TH1F("track_dz_reco","Reco dz",100,-20,20);
  TH1F *track_dz_l3 = new TH1F("track_dz_l3","L3 dz",100,-20,20);
  TH1F *track_sigmadz_reco = new TH1F("track_sigmadz_reco","Reco #sigma dz",100,0,0.1);
  TH1F *track_sigmadz_l3 = new TH1F("track_sigmadz_l3","L3 #sigma dz",100,0,0.1);
  TH1F *track_dxy_reco = new TH1F("track_dxy_reco","Reco dxy",100,-0.3,0.3);
  TH1F *track_dxy_l3 = new TH1F("track_dxy_l3","L3 dxy",100,-0.3,0.3);
  TH1F *track_sigmadxy_reco = new TH1F("track_sigmadxy_reco","Reco #sigma dxy",100,0,0.1);
  TH1F *track_sigmadxy_l3 = new TH1F("track_sigmadxy_l3","L3 #sigma dxy",100,0,0.1);
  //discriminators
  TH2F *RecoDiscr_vs_eta = new TH2F("RecoDiscr_vs_eta","Reco Discr vs eta", 100,-2.5,2.5, 100,-10,10);
  TH2F *RecoDiscr_vs_phi = new TH2F("RecoDiscr_vs_phi","Reco Discr vs phi",  100,-3.2,3.2,100,-10,10);
  TH2F *RecoDiscr_vs_et = new TH2F("RecoDiscr_vs_et","Reco Discr vs et", 100,0,200, 100,-10,10);
  TH2F *L3Discr_vs_eta = new TH2F("L3Discr_vs_eta","L3 Discr vs eta",  100,-2.5,2.5,100,-10,10);
  TH2F *L3Discr_vs_phi = new TH2F("L3Discr_vs_phi","L3 Discr vs phi", 100,-3.2,3.2,100,-10,10);
  TH2F *L3Discr_vs_et = new TH2F("L3Discr_vs_et","L3 Discr vs et",  100,0,200,100,-10,10);
  TH2F *L25Discr_vs_eta = new TH2F("L25Discr_vs_eta","L25 Discr vs eta",  100,-2.5,2.5,100,-10,10);
  TH2F *L25Discr_vs_phi = new TH2F("L25Discr_vs_phi","L25 Discr vs phi", 100,-3.2,3.2, 100,-10,10);
  TH2F *L25Discr_vs_et = new TH2F("L25Discr_vs_et","L25 Discr vs et",  100,0,200,100,-10,10);
  //discriminator comparisons
  TH2F *RecovsL3Discr = new TH2F("RecovsL3Discr","Reco vs L3 Discr",     400,-10,30,400,-10,30);
  TH2F *RecovsL25Discr = new TH2F("RecovsL25Discr","Reco vs L25 Discr",  400,-10,30,400,-10,30);
  TH2F *RecovsL3ip = new TH2F("RecovsL3ip","Reco vs L3 IP",     100,-.1,.1,100,-.1,.1);
  TH2F *RecovsL25ip = new TH2F("RecovsL25ip","Reco vs L25 IP",  100,-.1,.1,100,-.1,.1);
  TH2F *RecovsL3ipHP = new TH2F("RecovsL3ipHP","Reco vs L3 IP HP",     100,-.1,.1,100,-.1,.1);
  TH2F *RecovsL25ipHP = new TH2F("RecovsL25ipHP","Reco vs L25 IP HP",  100,-.1,.1,100,-.1,.1);
  TH2F *RecovsL3sigmaip = new TH2F("RecovsL3sigmaip","Reco vs L3 sigma IP", 200,0,10,200,0,10);
  //IP 2nd track
  TH2F *RecoIP2ndtrack_vs_eta = new TH2F("RecoIP2ndtrack_vs_eta","Reco IP2ndtrack vs eta",  100,-2.5,2.5,100,-0.1,0.1);
  TH2F *RecoIP2ndtrack_vs_phi = new TH2F("RecoIP2ndtrack_vs_phi","Reco IP2ndtrack vs phi",  100,-3.2,3.2,100,0.,0.1);
  TH2F *RecoIP2ndtrack_vs_et = new TH2F("RecoIP2ndtrack_vs_et","Reco IP2ndtrack vs et",  100,0,200,100,-0.1,0.1);
  TH2F *L3IP2ndtrack_vs_eta = new TH2F("L3IP2ndtrack_vs_eta","L3 IP2ndtrack vs eta",  100,-2.5,2.5,100,-0.1,0.1);
  TH2F *L3IP2ndtrack_vs_phi = new TH2F("L3IP2ndtrack_vs_phi","L3 IP2ndtrack vs phi",  100,-3.2,3.2,100,0,0.1);
  TH2F *L3IP2ndtrack_vs_et = new TH2F("L3IP2ndtrack_vs_et","L3 IP2ndtrack vs et",  100,0,200,100,-0.1,0.1);
  TH2F *L25IP2ndtrack_vs_eta = new TH2F("L25IP2ndtrack_vs_eta","L25 IP2ndtrack vs eta",  100,-2.5,2.5,100,-0.1,0.1);
  TH2F *L25IP2ndtrack_vs_phi = new TH2F("L25IP2ndtrack_vs_phi","L25 IP2ndtrack vs phi", 100,-3.2,3.2, 100,-0.1,0.1);
  TH2F *L25IP2ndtrack_vs_et = new TH2F("L25IP2ndtrack_vs_et","L25 IP2ndtrack vs et",  100,0,200,100,-0.1,0.1);
  TH2F *RecoSigmaIP2ndtrack_vs_eta = new TH2F("RecoSigmaIP2ndtrack_vs_eta","Reco SigmaIP2ndtrack vs eta",  100,-2.5,2.5,100,-10,10);
  TH2F *RecoSigmaIP2ndtrack_vs_phi = new TH2F("RecoSigmaIP2ndtrack_vs_phi","Reco SigmaIP2ndtrack vs phi",  100,-3.2,3.2,100,-10,10);
  TH2F *RecoSigmaIP2ndtrack_vs_et = new TH2F("RecoSigmaIP2ndtrack_vs_et","Reco SigmaIP2ndtrack vs et",  100,0,200,100,-10,10);
  TH2F *L3SigmaIP2ndtrack_vs_eta = new TH2F("L3SigmaIP2ndtrack_vs_eta","L3 SigmaIP2ndtrack vs eta",  100,-2.5,2.5,100,-10,10);
  TH2F *L3SigmaIP2ndtrack_vs_phi = new TH2F("L3SigmaIP2ndtrack_vs_phi","L3 SigmaIP2ndtrack vs phi", 100,-3.2,3.2,100,-10,10);
  TH2F *L3SigmaIP2ndtrack_vs_et = new TH2F("L3SigmaIP2ndtrack_vs_et","L3 SigmaIP2ndtrack vs et",  100,0,200,100,-10,10);
  TH2F *L25SigmaIP2ndtrack_vs_eta = new TH2F("L25SigmaIP2ndtrack_vs_eta","L25 SigmaIP2ndtrack vs eta",  100,-2.5,2.5,100,-10,10);
  TH2F *L25SigmaIP2ndtrack_vs_phi = new TH2F("L25SigmaIP2ndtrack_vs_phi","L25 SigmaIP2ndtrack vs phi", 100,-3.2,3.2,100,-10,10 );
  TH2F *L25SigmaIP2ndtrack_vs_et = new TH2F("L25SigmaIP2ndtrack_vs_et","L25 SigmaIP2ndtrack vs et",  100,0,200,100,-10,10);
  //IP 3rd track
  TH2F *RecoIP3rdtrack_vs_eta = new TH2F("RecoIP3rdtrack_vs_eta","Reco IP3rdtrack vs eta",  100,-2.5,2.5,100,-0.1,0.1);
  TH2F *RecoIP3rdtrack_vs_phi = new TH2F("RecoIP3rdtrack_vs_phi","Reco IP3rdtrack vs phi",  100,-3.2,3.2,100,0.,0.1);
  TH2F *RecoIP3rdtrack_vs_et = new TH2F("RecoIP3rdtrack_vs_et","Reco IP3rdtrack vs et",  100,0,200,100,-0.1,0.1);
  TH2F *L3IP3rdtrack_vs_eta = new TH2F("L3IP3rdtrack_vs_eta","L3 IP3rdtrack vs eta",  100,-2.5,2.5,100,-0.1,0.1);
  TH2F *L3IP3rdtrack_vs_phi = new TH2F("L3IP3rdtrack_vs_phi","L3 IP3rdtrack vs phi",  100,-3.2,3.2,100,0,0.1);
  TH2F *L3IP3rdtrack_vs_et = new TH2F("L3IP3rdtrack_vs_et","L3 IP3rdtrack vs et",  100,0,200,100,-0.1,0.1);
  TH2F *L25IP3rdtrack_vs_eta = new TH2F("L25IP3rdtrack_vs_eta","L25 IP3rdtrack vs eta",  100,-2.5,2.5,100,-0.1,0.1);
  TH2F *L25IP3rdtrack_vs_phi = new TH2F("L25IP3rdtrack_vs_phi","L25 IP3rdtrack vs phi", 100,-3.2,3.2, 100,-0.1,0.1);
  TH2F *L25IP3rdtrack_vs_et = new TH2F("L25IP3rdtrack_vs_et","L25 IP3rdtrack vs et",  100,0,200,100,-0.1,0.1);
  TH2F *RecoSigmaIP3rdtrack_vs_eta = new TH2F("RecoSigmaIP3rdtrack_vs_eta","Reco SigmaIP3rdtrack vs eta",  100,-2.5,2.5,100,-10,10);
  TH2F *RecoSigmaIP3rdtrack_vs_phi = new TH2F("RecoSigmaIP3rdtrack_vs_phi","Reco SigmaIP3rdtrack vs phi",  100,-3.2,3.2,100,-10,10);
  TH2F *RecoSigmaIP3rdtrack_vs_et = new TH2F("RecoSigmaIP3rdtrack_vs_et","Reco SigmaIP3rdtrack vs et",  100,0,200,100,-10,10);
  TH2F *L3SigmaIP3rdtrack_vs_eta = new TH2F("L3SigmaIP3rdtrack_vs_eta","L3 SigmaIP3rdtrack vs eta",  100,-2.5,2.5,100,-10,10);
  TH2F *L3SigmaIP3rdtrack_vs_phi = new TH2F("L3SigmaIP3rdtrack_vs_phi","L3 SigmaIP3rdtrack vs phi", 100,-3.2,3.2,100,-10,10);
  TH2F *L3SigmaIP3rdtrack_vs_et = new TH2F("L3SigmaIP3rdtrack_vs_et","L3 SigmaIP3rdtrack vs et",  100,0,200,100,-10,10);
  TH2F *L25SigmaIP3rdtrack_vs_eta = new TH2F("L25SigmaIP3rdtrack_vs_eta","L25 SigmaIP3rdtrack vs eta",  100,-2.5,2.5,100,-10,10);
  TH2F *L25SigmaIP3rdtrack_vs_phi = new TH2F("L25SigmaIP3rdtrack_vs_phi","L25 SigmaIP3rdtrack vs phi", 100,-3.2,3.2,100,-10,10 );
  TH2F *L25SigmaIP3rdtrack_vs_et = new TH2F("L25SigmaIP3rdtrack_vs_et","L25 SigmaIP3rdtrack vs et",  100,0,200,100,-10,10);
  //control histo
  TH2F *L3vsReco_et = new TH2F("L3vsReco_et","L3 vs Reco Et", 200,0,200,200,0,200);  
  TH1F *Reco_et = new TH1F("Reco_et","Reco Et", 200,0,200);
  TH1F *L3_et = new TH1F("L3_et","l3 Et", 200,0,200);
  TH1F *Reco_phi = new TH1F("Reco_phi","Reco  #phi", 200,-3.2,3.2);
  TH1F *L3_phi = new TH1F("L3_phi","l3 #phi", 200,-3.2,3.2);
  TH1F *Reco_eta = new TH1F("Reco_eta","Reco Eta", 200,-2.5,2.5);
  TH1F *L3_eta = new TH1F("L3_eta","l3 Eta", 200,-2.5,2.5);
  TH1F *DeltaR = new TH1F("DeltaR","DeltaR", 200,0,0.2);
  TH2F *l3Reco_dz = new TH2F("l3reco_dz","L3-reco dz",  200,-20,20,200,-20,20);
  TH2F *l3Reco_dxy = new TH2F("l3reco_dxy","L3-reco dxy",  300,-.3,.3,300,-.3,.3);
  TH2F *l3Reco_etajet = new TH2F("l3reco_etajet","L3-reco #eta jet",  100,-2.5,2.5,100,-2.5,2.5);
  TH2F *l3Reco_phijet = new TH2F("l3reco_phijet","L3-reco #phi jet",  100,-3.2,3.2,100,-3.2,3.2);
 
  TH2F *rate_recol3discr = new TH2F("rate_recol3discr","Rate reco vs L3 Discr",     400,-10,30,400,-10,30);
  TH2F *rate_recol25discr = new TH2F("rate_recol25discr","Rate reco vs L25 Discr",  400,-10,30,400,-10,30);
  
 
  Long64_t nentries = fChain->GetEntriesFast();
  
  Long64_t nbytes = 0, nb = 0;
  Float_t reco_discr = 0;
  Double_t deltaR=0;
  UInt_t prev_evtnum=0;
  
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    //get discriminator, IP and sigmaIP
    b_L3Reco_reco_discriminator->GetEntry(ientry);
    b_L3Reco_reco_ip2ndtrack->GetEntry(ientry);
    b_L3Reco_reco_sigmaip2ndtrack->GetEntry(ientry);
    b_L3Reco_reco_ip3rdtrack->GetEntry(ientry);
    b_L3Reco_reco_sigmaip3rdtrack->GetEntry(ientry);
    b_L3Reco_l3_discriminator->GetEntry(ientry);
    b_L3Reco_l3_ip2ndtrack->GetEntry(ientry);
    b_L3Reco_l3_sigmaip2ndtrack->GetEntry(ientry);
    b_L3Reco_l3_ip3rdtrack->GetEntry(ientry);
    b_L3Reco_l3_sigmaip3rdtrack->GetEntry(ientry);
    b_L25Reco_reco_discriminator->GetEntry(ientry);
    b_L25Reco_l25_discriminator->GetEntry(ientry);
    b_L25Reco_l25_ip2ndtrack->GetEntry(ientry);
    b_L25Reco_l25_sigmaip2ndtrack->GetEntry(ientry);
    b_L25Reco_l25_ip3rdtrack->GetEntry(ientry);
    b_L25Reco_l25_sigmaip3rdtrack->GetEntry(ientry);
    //get jet Et, eta and phi
    b_L3Reco_reco_Etjet->GetEntry(ientry);
    b_L3Reco_reco_etajet->GetEntry(ientry);
    b_L3Reco_reco_phijet->GetEntry(ientry);
    b_L3Reco_l3_Etjet->GetEntry(ientry);
    b_L3Reco_l3_etajet->GetEntry(ientry);
    b_L3Reco_l3_phijet->GetEntry(ientry);
    b_L25Reco_l25_Etjet->GetEntry(ientry);
    b_L25Reco_l25_etajet->GetEntry(ientry);
    b_L25Reco_l25_phijet->GetEntry(ientry);
    b_L3Reco_reco_dz->GetEntry(ientry);
    b_L3Reco_l3_dz->GetEntry(ientry);
    b_L3Reco_reco_sigmadz->GetEntry(ientry);
    b_L3Reco_l3_sigmadz->GetEntry(ientry);
    b_L3Reco_reco_dxy->GetEntry(ientry);
    b_L3Reco_l3_dxy->GetEntry(ientry);
    b_L3Reco_reco_sigmadxy->GetEntry(ientry);
    b_L3Reco_l3_sigmadxy->GetEntry(ientry);
    b_eventNum->GetEntry(ientry);
    
    //cout << "previous evt number" << prev_evtnum<<endl;
    //cout << "current evt number" << eventNum<<endl;
    //if(prev_evtnum==eventNum) continue;
    //prev_evtnum=eventNum;
 
    //deltaR=deltaR(L3Reco_l3_etajet,L3Reco_l3_phijet,L3Reco_reco_etajet,L3Reco_reco_phijet);
    deltaR= sqrt(pow(L3Reco_l3_etajet-L3Reco_reco_etajet,2)+pow(L3Reco_l3_phijet-L3Reco_reco_phijet,2));
    
    //cout <<"delta R: " << deltaR << endl;
    //fill histograms 
    RecoDiscr->Fill(L3Reco_reco_discriminator);
    L3Discr->Fill(L3Reco_l3_discriminator);
    RecoIP->Fill(L3Reco_reco_ip2ndtrack);
    //discriminator vs all
    //reco
    RecoDiscr_vs_eta->Fill(L3Reco_reco_etajet,L3Reco_reco_discriminator);
    RecoDiscr_vs_et->Fill(L3Reco_reco_Etjet,L3Reco_reco_discriminator);
    RecoDiscr_vs_phi->Fill(L3Reco_reco_phijet,L3Reco_reco_discriminator);
    //L3
    L3Discr_vs_eta->Fill(L3Reco_l3_etajet,L3Reco_l3_discriminator);
    L3Discr_vs_et->Fill(L3Reco_l3_Etjet,L3Reco_l3_discriminator);
    L3Discr_vs_phi->Fill(L3Reco_l3_phijet,L3Reco_l3_discriminator);
    //L25
    L25Discr_vs_eta->Fill(L25Reco_l25_etajet,L25Reco_l25_discriminator);
    L25Discr_vs_et->Fill(L25Reco_l25_Etjet,L25Reco_l25_discriminator);
    L25Discr_vs_phi->Fill(L25Reco_l25_phijet,L25Reco_l25_discriminator);
    
    RecovsL3Discr->Fill(L3Reco_reco_discriminator,  L3Reco_l3_discriminator);
    RecovsL25Discr->Fill(L25Reco_reco_discriminator,L25Reco_l25_discriminator);
    
    //impact parameter vs all 2nd track 
    //reco
    RecoIP2ndtrack_vs_eta->Fill(L3Reco_reco_etajet,L3Reco_reco_ip2ndtrack);
    RecoIP2ndtrack_vs_et->Fill(L3Reco_reco_Etjet,L3Reco_reco_ip2ndtrack);
    RecoIP2ndtrack_vs_phi->Fill(L3Reco_reco_phijet,fabs(L3Reco_reco_ip2ndtrack));
    //L3
    L3IP2ndtrack_vs_eta->Fill(L3Reco_l3_etajet,L3Reco_l3_ip2ndtrack);
    L3IP2ndtrack_vs_et->Fill(L3Reco_l3_Etjet,L3Reco_l3_ip2ndtrack);
    L3IP2ndtrack_vs_phi->Fill(L3Reco_l3_phijet,fabs(L3Reco_l3_ip2ndtrack));
    //L25
    L25IP2ndtrack_vs_eta->Fill(L25Reco_l25_etajet,L25Reco_l25_ip2ndtrack);
    L25IP2ndtrack_vs_et->Fill(L25Reco_l25_Etjet,L25Reco_l25_ip2ndtrack);
    L25IP2ndtrack_vs_phi->Fill(L25Reco_l25_phijet,L25Reco_l25_ip2ndtrack);
    //sigma impact parameter vs all 
    //reco
    RecoSigmaIP2ndtrack_vs_eta->Fill(L3Reco_reco_etajet,L3Reco_reco_sigmaip2ndtrack);
    RecoSigmaIP2ndtrack_vs_et->Fill(L3Reco_reco_Etjet,L3Reco_reco_sigmaip2ndtrack);
    RecoSigmaIP2ndtrack_vs_phi->Fill(L3Reco_reco_phijet,L3Reco_reco_sigmaip2ndtrack);
    //L3
    L3SigmaIP2ndtrack_vs_eta->Fill(L3Reco_l3_etajet,L3Reco_l3_sigmaip2ndtrack);
    L3SigmaIP2ndtrack_vs_et->Fill(L3Reco_l3_Etjet,L3Reco_l3_sigmaip2ndtrack);
    L3SigmaIP2ndtrack_vs_phi->Fill(L3Reco_l3_phijet,L3Reco_l3_sigmaip2ndtrack);
    //L25
    L25SigmaIP2ndtrack_vs_eta->Fill(L25Reco_l25_etajet,L25Reco_l25_sigmaip2ndtrack);
    L25SigmaIP2ndtrack_vs_et->Fill(L25Reco_l25_Etjet,L25Reco_l25_sigmaip2ndtrack);
    L25SigmaIP2ndtrack_vs_phi->Fill(L25Reco_l25_phijet,L25Reco_l25_sigmaip2ndtrack);
    //IP vs all 3rd track
    //reco
    RecoIP3rdtrack_vs_eta->Fill(L3Reco_reco_etajet,L3Reco_reco_ip3rdtrack);
    RecoIP3rdtrack_vs_et->Fill(L3Reco_reco_Etjet,L3Reco_reco_ip3rdtrack);
    RecoIP3rdtrack_vs_phi->Fill(L3Reco_reco_phijet,L3Reco_reco_ip3rdtrack);
    //L3
    L3IP3rdtrack_vs_eta->Fill(L3Reco_l3_etajet,L3Reco_l3_ip3rdtrack);
    L3IP3rdtrack_vs_et->Fill(L3Reco_l3_Etjet,L3Reco_l3_ip3rdtrack);
    L3IP3rdtrack_vs_phi->Fill(L3Reco_l3_phijet,L3Reco_l3_ip3rdtrack);
    //L25
    L25IP3rdtrack_vs_eta->Fill(L25Reco_l25_etajet,L25Reco_l25_ip3rdtrack);
    L25IP3rdtrack_vs_et->Fill(L25Reco_l25_Etjet,L25Reco_l25_ip3rdtrack);
    L25IP3rdtrack_vs_phi->Fill(L25Reco_l25_phijet,L25Reco_l25_ip3rdtrack);
    //sigma impact parameter vs all 
    //reco
    RecoSigmaIP3rdtrack_vs_eta->Fill(L3Reco_reco_etajet,L3Reco_reco_sigmaip3rdtrack);
    RecoSigmaIP3rdtrack_vs_et->Fill(L3Reco_reco_Etjet,L3Reco_reco_sigmaip3rdtrack);
    RecoSigmaIP3rdtrack_vs_phi->Fill(L3Reco_reco_phijet,L3Reco_reco_sigmaip3rdtrack);
    //L3
    L3SigmaIP3rdtrack_vs_eta->Fill(L3Reco_l3_etajet,L3Reco_l3_sigmaip3rdtrack);
    L3SigmaIP3rdtrack_vs_et->Fill(L3Reco_l3_Etjet,L3Reco_l3_sigmaip3rdtrack);
    L3SigmaIP3rdtrack_vs_phi->Fill(L3Reco_l3_phijet,L3Reco_l3_sigmaip3rdtrack);
    //L25
    L25SigmaIP3rdtrack_vs_eta->Fill(L25Reco_l25_etajet,L25Reco_l25_sigmaip3rdtrack);
    L25SigmaIP3rdtrack_vs_et->Fill(L25Reco_l25_Etjet,L25Reco_l25_sigmaip3rdtrack);
    L25SigmaIP3rdtrack_vs_phi->Fill(L25Reco_l25_phijet,L25Reco_l25_sigmaip3rdtrack);
    //IP hlt vs reco
    RecovsL3ip->Fill(L3Reco_l3_ip2ndtrack,  L3Reco_reco_ip2ndtrack);
    RecovsL25ip->Fill(L25Reco_l25_ip2ndtrack,L25Reco_reco_ip2ndtrack);
    RecovsL3sigmaip->Fill(L3Reco_l3_sigmaip2ndtrack, L3Reco_reco_sigmaip2ndtrack);
    RecovsL25ipHP->Fill(L25Reco_l25_ip3rdtrack,L25Reco_reco_ip3rdtrack);
    //control histo
    L3vsReco_et->Fill(L3Reco_reco_Etjet,L3Reco_l3_Etjet);
    Reco_et->Fill(L3Reco_reco_Etjet);
    L3_et->Fill(L3Reco_l3_Etjet);
    Reco_phi->Fill(L3Reco_reco_phijet);
    L3_phi->Fill(L3Reco_l3_phijet);
    Reco_eta->Fill(L3Reco_reco_etajet);
    L3_eta->Fill(L3Reco_l3_etajet);
    DeltaR->Fill(deltaR);
    track_dz_reco->Fill(L3Reco_reco_dz);
    track_dz_l3->Fill(L3Reco_l3_dz);
    track_sigmadz_reco->Fill(L3Reco_reco_sigmadz);
    track_sigmadz_l3->Fill(L3Reco_l3_sigmadz);
    track_dxy_reco->Fill(fabs(L3Reco_reco_dxy));
    track_dxy_l3->Fill(fabs(L3Reco_l3_dxy));
    track_sigmadxy_reco->Fill(L3Reco_reco_sigmadxy);
    track_sigmadxy_l3->Fill(L3Reco_l3_sigmadxy);
    l3Reco_dz->Fill(L3Reco_reco_dz,L3Reco_l3_dz);    
    l3Reco_dxy->Fill(L3Reco_reco_dxy,L3Reco_l3_dxy);     
    l3Reco_etajet->Fill(L3Reco_reco_etajet,L3Reco_l3_etajet);
    l3Reco_phijet->Fill(L3Reco_reco_phijet,L3Reco_l3_phijet);
    //cout <<"event number:          "<<eventNum<<endl;
    //cout <<"previous event number: "<<prev_evtnum<<endl;
    
    if(prev_evtnum!=eventNum){
      rate_recol3discr->Fill(L3Reco_reco_discriminator,  L3Reco_l3_discriminator);
      rate_recol25discr->Fill(L25Reco_reco_discriminator,  L25Reco_l25_discriminator);
      prev_evtnum=eventNum;
      //cout <<"*** we use this event *** "<<eventNum<<endl;
    }
  }
  
  float lumi;
  float prescale;
  prescale=1; 
  //bool test=true;
  //if(test) 
  lumi=188./prescale;
  //else lumi=2904;
  
  cout<<"for cicle"<<endl;
  
  Double_t ind,den;
  Double_t effl25[70];
  Double_t effl3[30];
  Double_t discr[70];
  Double_t err_effl25[70];
  Double_t err_effl3[30];
  Double_t err_discr[70];
  Double_t err_evts[30];
  Double_t evts[30];
  Double_t evtsl25[30];
  Double_t err_evtsl25[30];
  Double_t purl25[30];
  Double_t err_purl25[30];
  Double_t discrl25[30];
  Double_t err_discrl25[30];
  Double_t discrl3[30];
  Double_t err_discrl3[30];
  Double_t redul25[30];
  Double_t err_redul25[30];
  Double_t redul3[30];
  Double_t err_redul3[30];
 

  Double_t purity[30];
  Double_t efficiency[30];
  Double_t err_purity[30];
  Double_t err_efficiency[30];
  Double_t efficiencyl25[30];
  Double_t err_efficiencyl25[30];
 
  /*for (int d=0; d<70; d++)
    {
      cout<< "d= "<< d <<endl; 
      if(rate_recol3discr->Integral(d*5, 400, 0, 400) && rate_recol25discr->Integral(d*5, 400, 0, 400))
	{      
	  discr[d]=-10+0.5*d;
	  err_discr[d]=0;
	  //if(rate_recol3discr->Integral(d*5, 400, 0, 400))
	  den=rate_recol3discr->Integral(d*5, 400, 135, 400)/rate_recol3discr->Integral(d*5, 400, 0, 400);
	  //cout<< "den"
	  effl3[d]=den;
	  //if(rate_recol3discr->Integral(d*5, 400, 0, 400))
	  err_effl3[d]=sqrt(den*(1-den)/rate_recol3discr->Integral(d*5, 400, 0, 400));
	  
	  //if(rate_recol25discr->Integral(d*5, 400, 0, 400))
	  den=rate_recol25discr->Integral(d*5, 400, 100, 400)/rate_recol25discr->Integral(d*5, 400, 0, 400);
	  effl25[d]=den;
	  
	  //if(rate_recol25discr->Integral(d*5, 400, 0, 400))
	  err_effl25[d]=sqrt(den*(1-den)/rate_recol25discr->Integral(d*5, 400, 0, 400));
	  //eff_l25->Fill(-10+d*10,(rate_recol25discr->Integral(d*10, 400, 125, 400)/rate_recol25discr->Integral(0, 400,125, 400)));
	}
	}*/
  //  cout<<"eff L3 plateau: "<<rate_recol3discr->Integral(250, 400, 135, 400)/rate_recol3discr->Integral(250, 400, 0, 400)<<endl;
  //cout<<"L3 purity plateau: " << rate_recol3discr->Integral(250, 400, 135, 400)/rate_recol3discr->Integral(0, 400, 135, 400) <<endl;
  
  for (int c=5; c<35; c++)
    {
      if(rate_recol3discr->Integral(140, 400, 0, 400) && rate_recol25discr->Integral(140, 400, 0, 400) && 
	 rate_recol3discr->Integral(0, 400, c*10, 400) && rate_recol25discr->Integral(0, 400, c*10, 400))
	{
	  //cout<<"c= "<<c<<endl;
	  //efficiency l3
	  den=rate_recol3discr->Integral(140, 400, c*10, 400)/rate_recol3discr->Integral(140, 400, 0, 400);
	  efficiency[c-5]=den;
	  err_efficiency[c-5]=sqrt(den*(1-den)/rate_recol3discr->Integral(140, 400, 0, 400));
	  //redu l3
	  den=rate_recol3discr->Integral(0, 400, c*10, 400)/rate_recol3discr->Integral(0, 400, 0, 400);
	  redul3[c-5]=den;
	  err_redul3[c-5]=sqrt(den*(1-den)/rate_recol3discr->Integral(0, 400, 0, 400));
	  discrl3[c-5]=c-10;
	  err_discrl3[c-5]=0;
	  
	  //efficiency l25
	  den=rate_recol25discr->Integral(140, 400, c*10, 400)/rate_recol25discr->Integral(140, 400, 0, 400);
	  efficiencyl25[c-5]=den;
	  err_efficiencyl25[c-5]=sqrt(den*(1-den)/rate_recol25discr->Integral(140, 400, 0, 400));
	  //redu l25
	  den=rate_recol25discr->Integral(0, 400, c*10, 400)/rate_recol25discr->Integral(0, 400, 0, 400);
	  redul25[c-5]=den;
	  err_redul25[c-5]=sqrt(den*(1-den)/rate_recol25discr->Integral(0, 400, 0, 400));
	  //purity l3
	  ind=rate_recol3discr->Integral(140, 400, c*10, 400)/rate_recol3discr->Integral(0, 400, c*10, 400);
	  purity[c-5]=ind;
	  err_purity[c-5]=0;
	  //purity l25
	  ind=rate_recol25discr->Integral(140, 400, c*10, 400)/rate_recol25discr->Integral(0, 400, c*10, 400);
	  purl25[c-5]=ind;
	  err_purl25[c-5]=sqrt(ind*(1-ind)/rate_recol25discr->Integral(0, 400, c*10, 400));
	  discrl25[c-5]=c-10;
	  err_discrl25[c-5]=0;
	  //rate l3
	  ind=rate_recol3discr->Integral(0, 400, c*10, 400)/(lumi*23);
	  evts[c-5]=ind;
	  err_evts[c-5]=sqrt(ind)/(lumi*23);
	  //rate l25
	  ind=rate_recol25discr->Integral(0, 400, c*10, 400)/(lumi*23);
	  evtsl25[c-5]=ind;
	  err_evtsl25[c-5]=sqrt(ind)/(lumi*23);
	  cout<<"discriminant value: "<<discrl3[c-5]<<endl;
	  cout<<"efficiency l3: "<<efficiency[c-5]<<endl;
	  cout<<"reduction l3: "<<redul3[c-5]<<endl;
	  cout<<"efficiency l25: "<<efficiencyl25[c-5]<<endl;
	  cout<<"reduction l25: "<<redul25[c-5]<<endl;
	  cout<<"rate l3: "<<evts[c-5]<<endl;
	  cout<<"rate l25: "<<evtsl25[c-5]<<endl;
	  cout<<"rate L2: "<<rate_recol25discr->Integral(0, 400, 0, 400)/(lumi*23)<<endl;
	  
	}
    }

  
  deltaR= sqrt(pow(L3Reco_l3_etajet-L3Reco_reco_etajet,2)+pow(L3Reco_l3_phijet-L3Reco_reco_phijet,2));
  
  //fill histograms 
  
  RecoDiscr->Write();
  L3Discr->Write();
  RecoIP->Write();
  //discriminator vs all
     //reco
  RecoDiscr_vs_eta->Write();
  RecoDiscr_vs_et->Write();
  RecoDiscr_vs_phi->Write();
  //L3
  L3Discr_vs_eta->Write();
  L3Discr_vs_et->Write();
  L3Discr_vs_phi->Write();
  //L25
  L25Discr_vs_eta->Write();
  L25Discr_vs_et->Write();
  L25Discr_vs_phi->Write();
  //if(L3Reco_reco_Etjet>50){
  RecovsL3Discr->Write();
  RecovsL25Discr->Write();
  //impact parameter vs all 2nd t
  //reco
  RecoIP2ndtrack_vs_eta->Write();
  RecoIP2ndtrack_vs_et->Write();
  RecoIP2ndtrack_vs_phi->Write();
  //L3
  L3IP2ndtrack_vs_eta->Write();
  L3IP2ndtrack_vs_et->Write();
  L3IP2ndtrack_vs_phi->Write();
  //L25
  L25IP2ndtrack_vs_eta->Write();
  L25IP2ndtrack_vs_et->Write();
  L25IP2ndtrack_vs_phi->Write();
  //sigma impact parameter vs all
  //reco
  RecoSigmaIP2ndtrack_vs_eta->Write();
  RecoSigmaIP2ndtrack_vs_et->Write();
  RecoSigmaIP2ndtrack_vs_phi->Write();
  //L3
  L3SigmaIP2ndtrack_vs_eta->Write();
  L3SigmaIP2ndtrack_vs_et->Write();
  L3SigmaIP2ndtrack_vs_phi->Write();
  //L25
  L25SigmaIP2ndtrack_vs_eta->Write();
  L25SigmaIP2ndtrack_vs_et->Write();
  L25SigmaIP2ndtrack_vs_phi->Write();
  //IP vs all 3rd track
  //reco
  RecoIP3rdtrack_vs_eta->Write();
  RecoIP3rdtrack_vs_et->Write();
  RecoIP3rdtrack_vs_phi->Write();
  //L3
  L3IP3rdtrack_vs_eta->Write();
  L3IP3rdtrack_vs_et->Write();
  L3IP3rdtrack_vs_phi->Write();
  //L25
  L25IP3rdtrack_vs_eta->Write();
  L25IP3rdtrack_vs_et->Write();
  L25IP3rdtrack_vs_phi->Write();
  //sigma impact parameter vs all
  //reco
  RecoSigmaIP3rdtrack_vs_eta->Write();
  RecoSigmaIP3rdtrack_vs_et->Write();
  RecoSigmaIP3rdtrack_vs_phi->Write();
  //L3
  L3SigmaIP3rdtrack_vs_eta->Write();
  L3SigmaIP3rdtrack_vs_et->Write();
  L3SigmaIP3rdtrack_vs_phi->Write();
  //L25
  L25SigmaIP3rdtrack_vs_eta->Write();
  L25SigmaIP3rdtrack_vs_et->Write();
  L25SigmaIP3rdtrack_vs_phi->Write();
  //IP hlt vs reco
  RecovsL3ip->Write();
  RecovsL25ip->Write();
  RecovsL3sigmaip->Write();
  RecovsL25ipHP->Write();
  //control histo
  L3vsReco_et->Write();
  Reco_et->Write();
  L3_et->Write();
  Reco_phi->Write();
  L3_phi->Write();
  Reco_eta->Write();
  L3_eta->Write();
  DeltaR->Write();
  track_dz_reco->Write();
  track_dz_l3->Write();
  track_sigmadz_reco->Write();
  track_sigmadz_l3->Write();
  track_dxy_reco->Write();
  track_dxy_l3->Write();
  track_sigmadxy_reco->Write();
  track_sigmadxy_l3->Write();
  l3Reco_dz->Write();
  l3Reco_dxy->Write();
  l3Reco_etajet->Write();
  l3Reco_phijet->Write();
  rate_recol3discr->Write();
  rate_recol25discr->Write();
  
  
  TGraphErrors *pur_l25_evts = new TGraphErrors(30,evtsl25,purl25,err_evtsl25,err_purl25);
  TGraphErrors *eff_l3 = new TGraphErrors(30,discrl3,efficiency,err_discrl3,err_efficiency);
  TGraphErrors *pur_l25 = new TGraphErrors(30,discrl25,purl25,err_discrl25,err_purl25);
  TGraphErrors *eff_pur = new TGraphErrors(30,efficiency,purity,err_efficiency,err_purity);
  TGraphErrors *effl3_evts = new TGraphErrors(30,efficiency,evts,err_efficiency,err_evts);
  TGraphErrors *effl25_evts = new TGraphErrors(30,efficiencyl25,evtsl25,err_efficiencyl25,err_evtsl25);
  TGraphErrors *redu_l25 = new TGraphErrors(30,discrl25, redul25, err_discrl25, err_redul25);
  TGraphErrors *redu_l3 = new TGraphErrors(30,discrl3, redul3, err_discrl3, err_redul3);
  TGraphErrors *eff_l25 = new TGraphErrors(30,discrl25,efficiencyl25,err_discrl25,err_efficiencyl25);
 
  eff_l25->SetTitle("Efficiency_vs_discriminator_L25");
  eff_l3->SetTitle("Efficiency_vs_discriminator_L3");
  eff_pur->SetTitle("Efficiency_vs_Purity");
  effl3_evts->SetTitle("Efficiency_vs_Rate_L3");
  effl25_evts->SetTitle("Efficiency_vs_Rate_L25");
  pur_l25_evts->SetTitle("Purity_vs_Rate_L25");
  pur_l25->SetTitle("Purity_vs_discriminator_L25");
  redu_l25->SetTitle("Reduction_vs_discrL25");
  redu_l3->SetTitle("Reduction_vs_discrL3");
  
  
  //eff_l25->Write("eff_l25");
  cout<<"about to write"<<endl;
  eff_l3->Write("eff_l3");
  eff_l25->Write("eff_l25");
  eff_pur->Write("eff_pur");
  effl3_evts->Write("effl3_evts");
  effl25_evts->Write("effl25_evts");
  pur_l25->Write("purl25"); 
  pur_l25_evts->Write("purl25_evts");
  redu_l25->Write("redu_l25");
  redu_l3->Write("redu_l3");
  eff->Close();

}


void changeStyle(TProfile *profile, const char* labelX, const char* labelY, int color, int style, float min, float max)
{
  profile->GetYaxis()->SetTitle(labelY);
  profile->GetYaxis()->SetLabelSize(0.025);
  profile->GetYaxis()->SetTitleOffset(1.7);
  profile->SetMarkerColor(color);
  profile->SetFillColor(color);
  profile->SetMarkerStyle(style);
  profile->SetMinimum(min);
  profile->SetMaximum(max);
  profile->GetYaxis()->SetTitleSize(0.03);
  profile->GetXaxis()->SetLabelSize(0.03);
  profile->GetXaxis()->SetTitleSize(0.03);
  profile->GetXaxis()->SetTitleOffset(1.1); 
}

