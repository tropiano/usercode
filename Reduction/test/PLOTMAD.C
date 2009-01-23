void PLOTMAD()
{
  gROOT->Reset();
  // gROOT->SetStyle("Plain");
  gROOT->SetStyle("BABAR");


     


 TFile* f = new TFile("histo_1M_PYTHIA_corretto.root");
 TFile* t = new TFile("histo_1M_MADGRAPH_ISO_tot.root");



//   leggo i tree memorizzati in memoria nel file     

 TTree* ZMM=(TTree*)f->Get("verySimplePATAnalysis/ZMM");
 TTree* ZMM_MAD=(TTree*)t->Get("verySimplePATAnalysis/ZMM_MAD");

 float chek=ZMM_MAD->GetEntries("njetscounter==0 && njetscounterGen==2 && MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
  cout<<"ceck"<<chek<<endl;


 float nentries1=ZMM_MAD->GetEntries("njetscounter>=1 && njetscounterGen>=1 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries2=ZMM_MAD->GetEntries("njetscounter>=2 && njetscounterGen>=2 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries3=ZMM_MAD->GetEntries("njetscounter>=3 && njetscounterGen>=3 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");

 float nentries1_prova=ZMM_MAD->GetEntries("njetscounter>=1 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries2_prova=ZMM_MAD->GetEntries("njetscounter>=2 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries3_prova=ZMM_MAD->GetEntries("njetscounter>=3 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
float nentries0_prova=ZMM_MAD->GetEntries("njetscounter>=0 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 cout<<"Prova Z>=1jet"<<(nentries1_prova)*0.37<<"-"<<nentries1_prova/nentries0_prova<<endl;
 cout<<"Prova Z>=2jet"<<(nentries2_prova)*0.37<<"-"<<nentries2_prova/nentries0_prova<<endl;
 cout<<"Prova Z>=3jet"<<(nentries3_prova)*0.37<<"-"<<nentries3_prova/nentries0_prova<<endl;
 cout<<"numero di entries nel tree"<<nentries0_prova<<endl;

 float nentries1Gen=ZMM_MAD->GetEntries("njetscounterGen>=1  && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries2Gen=ZMM_MAD->GetEntries("njetscounterGen>=2  && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries3Gen=ZMM_MAD->GetEntries("njetscounterGen>=3  && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");

 cout<<"Z+1jet"<<nentries1<<endl;
 cout<<"Z+2jet"<<nentries2<<endl;
 cout<<"Z+3jet"<<nentries3<<endl;

 cout<<"Z+1jetgen"<<nentries1Gen<<endl;
 cout<<"Z+2jetgen"<<nentries2Gen<<endl;
 cout<<"Z+3jetgen"<<nentries3Gen<<endl;

 float nentries=ZMM_MAD->GetEntries(" MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 cout<<"nentries è:"<<nentries<<endl;
 float efficienzaSelection=nentries/1000000;
 
 float errorSelection=sqrt(efficienzaSelection*(1-efficienzaSelection)/1000000);
 cout<<"l'efficienza é:"<<efficienzaSelection<<"+/- "<<errorSelection<<endl;

 float efficienza1_tot=nentries1/1000000;
 float error1_tot=sqrt(efficienza1_tot*(1-efficienza1_tot)/1000000);
 float efficienza1=nentries1/nentries;
 float error1=sqrt(efficienza1*(1-efficienza1)/1000000);
 float efficienza1Gen=nentries1/nentries1Gen;
 float error1_gen=sqrt(efficienza1Gen*(1-efficienza1Gen)/nentries1Gen);
 cout<<"Z+>=1/1000000:"<<efficienza1_tot<<"+/- "<<error1_tot<<endl;
 cout<<"(Z+>=1)rec/(Zinclusivo)rec:"<<efficienza1<<"+/-"<<error1<<endl;
 cout<<"(Z+>=1)rec/(Z+>=1)gen:"<<efficienza1Gen<<"+/-"<<error1_gen<<endl;

 float efficienza2_tot=nentries2/1000000;
 float error2_tot=sqrt(efficienza2_tot*(1-efficienza2_tot)/1000000);
 float efficienza2=nentries2/nentries;
 float error2=sqrt(efficienza2*(1-efficienza2)/1000000);
 float efficienza2Gen=nentries2/nentries2Gen;
 float error2_gen=sqrt(efficienza2Gen*(1-efficienza2Gen)/nentries2Gen);
 cout<<"Z+>=2/1000000:"<<efficienza2_tot<<"+/- "<<error2_tot<<endl;endl;
 cout<<"(Z+>=2)rec/(Zinclusivo)rec:"<<efficienza2<<"+/-"<<error2<<endl;
 cout<<"(Z+>=2)rec/(Z+>=2)gen:"<<efficienza2Gen<<"+/-"<<error2_gen<<endl;

 float efficienza3_tot=nentries3/1000000;
 float error3_tot=sqrt(efficienza3_tot*(1-efficienza3_tot)/1000000);
 float efficienza3=nentries3/nentries;
 float error3=sqrt(efficienza3*(1-efficienza3)/1000000);
 float efficienza3Gen=nentries3/nentries3Gen;
 float error3_gen=sqrt(efficienza3Gen*(1-efficienza3Gen)/nentries3Gen);
 cout<<"Z+>=3/1000000:"<<efficienza3_tot<<"+/-"<<error3_gen<<endl;
 cout<<"(Z+>=3)rec/(Zinclusivo)rec:"<<efficienza3<<"+/-"<<error3<<endl;
 cout<<"(Z+>=3)rec/(Z+>=3)gen:"<<efficienza3Gen<<"+/-"<<error3_gen<<endl;

 float nentries_MAD=ZMM_MAD->GetEntries("(MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)");
 cout<<"entries nel tree di MAD"<<nentries_MAD<<endl;

 float nentries0esc_MAD=ZMM_MAD->GetEntries("njetscounter==0 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries0esc_peso_MAD=nentries0esc_MAD*0.37;
 cout<<"numero di eventi con zero pesati per MAD= "<<nentries0esc_peso_MAD<<endl;


 float nentries1esc_MAD=ZMM_MAD->GetEntries("njetscounter==1 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries1esc_peso_MAD=nentries1esc_MAD*0.37;
 cout<<"numero di eventi con un solo jet pesati per MAD= "<<nentries1esc_peso_MAD<<endl;


 float nentries2esc_MAD=ZMM_MAD->GetEntries("njetscounter==2 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries2esc_peso_MAD=nentries2esc_MAD*0.37;
 cout<<"numero di eventi con due jet pesati per MAD= "<<nentries2esc_peso_MAD<<endl;


 float nentries3esc_MAD=ZMM_MAD->GetEntries("njetscounter==3 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries3esc_peso_MAD=nentries3esc_MAD*0.37;
 cout<<"numero di eventi con tre jet pesati per MAD= "<<nentries3esc_peso_MAD<<endl;

 //EFFICIENZE PYTHIA//
 cout<<"EFFICIENZE PYTHIA"<<endl;
 float nentries1_P=ZMM->GetEntries("njetscounter>=1 &&  MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && abs(jeteta)<3");
 float nentries2_P=ZMM->GetEntries("njetscounter>=2  && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && abs(jeteta)<3");
 float nentries3_P=ZMM->GetEntries("njetscounter>=3  && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && abs(jeteta)<3");

 

 /* float nentries1Gen_P=ZMM->GetEntries("njetscounterGen>=1  && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries2Gen_P=ZMM->GetEntries("njetscounterGen>=2  && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries3Gen_P=ZMM->GetEntries("njetscounterGen>=3  && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");*/

 cout<<"Z+1jet"<<nentries1_P<<endl;
 cout<<"Z+2jet"<<nentries2_P<<endl;
 cout<<"Z+3jet"<<nentries3_P<<endl;

 /* cout<<"Z+1jetgen"<<nentries1Gen_P<<endl;
 cout<<"Z+2jetgen"<<nentries2Gen_P<<endl;
 cout<<"Z+3jetgen"<<nentries3Gen_P<<endl;*/

 float nentries_P=ZMM->GetEntries(" MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 cout<<"nentries è:"<<nentries_P<<endl;
 float efficienzaSelection_P=nentries_P/1000000;
 
 float errorSelection_P=sqrt(efficienzaSelection_P*(1-efficienzaSelection_P)/1000000);
 cout<<"l'efficienza é:"<<efficienzaSelection_P<<"+/- "<<errorSelection_P<<endl;

 float efficienza1_tot_P=nentries1_P/1000000;
 float error1_tot_P=sqrt(efficienza1_tot_P*(1-efficienza1_tot_P)/1000000);
 float efficienza1_P=nentries1_P/nentries_P;
 float error1_P=sqrt(efficienza1_P*(1-efficienza1_P)/1000000);
 // float efficienza1Gen_P=nentries1_P/nentries1Gen_P;
 //float error1_gen_P=sqrt(efficienza1Gen_P*(1-efficienza1Gen_P)/nentries1Gen_P);
 cout<<"Z+>=1/1000000:"<<efficienza1_tot_P<<"+/- "<<error1_tot_P<<endl;
 cout<<"(Z+>=1)rec/(Zinclusivo)rec:"<<efficienza1_P<<"+/-"<<error1_P<<endl;
 // cout<<"(Z+>=1)rec/(Z+>=1gen):"<<efficienza1Gen_P<<"+/-"<<error1_gen_P<<endl;

 float efficienza2_tot_P=nentries2/100000;
 float error2_tot_P=sqrt(efficienza2_tot_P*(1-efficienza2_tot_P)/1000000);
 float efficienza2_P=nentries2_P/nentries_P;
 float error2_P=sqrt(efficienza2_P*(1-efficienza2_P)/1000000);
 // float efficienza2Gen_P=nentries2_P/nentries2Gen_P;
 // float error2_gen_P=sqrt(efficienza2Gen_P*(1-efficienza2Gen_P)/nentries2Gen_P);
 cout<<"Z+>=2/1000000:"<<efficienza2_tot_P<<"+/- "<<error2_tot_P<<endl;endl;
 cout<<"(Z+>=2)rec/(Zinclusivo)rec:"<<efficienza2_P<<"+/-"<<error2_P<<endl;
 // cout<<"(Z+>=2)rec/(Z+>=2gen):"<<efficienza2Gen_P<<"+/-"<<error2_gen_P<<endl;

 float efficienza3_tot_P=nentries3_P/1000000;
 float error3_tot_P=sqrt(efficienza3_tot_P*(1-efficienza3_tot_P)/1000000);
 float efficienza3_P=nentries3_P/nentries_P;
 float error3_P=sqrt(efficienza3_P*(1-efficienza3_P)/1000000);
 // float efficienza3Gen_P=nentries3_P/nentries3Gen_P;
 //float error3_gen_P=sqrt(efficienza3Gen_P*(1-efficienza3Gen_P)/nentries3Gen_P);
 cout<<"Z+>=3/1000000:"<<efficienza3_tot_P<<"+/-"<<error3_tot_P<<endl;
 cout<<"(Z+>=3)rec/(Zinclusivo)rec:"<<efficienza3_P<<"+/-"<<error3_P<<endl;
 // cout<<"(Z+>=3rec/(Z+>=3gen):"<<efficienza3Gen_P<<"+/-"<<error3_gen_P<<endl;

 float nentries=ZMM->GetEntries(" MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 cout<<"entries nel tree di PYTHIA"<<nentries<<endl;

 float nentries0esc_P=ZMM->GetEntries("njetscounter==0 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries0esc_peso=nentries0esc_P*0.0627597;
 cout<<"numero di eventi con zero jet pesati= "<<nentries0esc_peso<<endl;

 float nentries1esc=ZMM->GetEntries("njetscounter==1 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries1esc_peso=nentries1esc*0.0627597;
 cout<<"numero di eventi con un solo jet pesati= "<<nentries1esc_peso<<endl;


 float nentries2esc=ZMM->GetEntries("njetscounter==2 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries2esc_peso=nentries2esc*0.0627597;
 cout<<"numero di eventi con due jet pesati= "<<nentries2esc_peso<<endl;


 float nentries3esc=ZMM->GetEntries("njetscounter==3 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 float nentries3esc_peso=nentries3esc*0.0627597;
 cout<<"numero di eventi con tre jet pesati= "<<nentries3esc_peso<<endl;

 
 //metrice njet:njetgen//
 int i=0 ;
 int j=0 ;
 TMatrixD njetsMatrix(4,4);

 for(i=0;i<3;i++){
   for(j=0;j<3;j++){
     char comando[500];
     sprintf(comando,"njetscounter==%d && njetscounterGen==%d && MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3",i,j);

     cout<<"cosa stampi?"<<comando<<endl;
     njetsMatrix[i][j]=ZMM_MAD->GetEntries(comando);}}
 

 for(i=0;i<3;i++){
   
   char comando1[500];
   sprintf(comando1,"njetscounter==%d && njetscounterGen>=3 && MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3",i);
 cout<<"cosa stampi?"<<comando1<<endl;
   njetsMatrix[i][3]=ZMM_MAD->GetEntries(comando1);}
   

 for(j=0;j<3;j++){
   
   char comando2[500];
  
   sprintf(comando2,"njetscounter>=3 && njetscounterGen==%d && MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3",j);
    cout<<"cosa stampi?"<<comando2<<endl;
   njetsMatrix[3][j]=ZMM_MAD->GetEntries(comando2);}

 char comando3[500];  
   sprintf(comando3,"njetscounter>=3 && njetscounterGen>=3 && MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3",j);
 cout<<"cosa stampi?"<<comando3<<endl;
  njetsMatrix[3][3]=ZMM_MAD->GetEntries(comando3);
  njetsMatrix.Print();
  float chek=ZMM_MAD->GetEntries("njetscounter==0 && njetscounterGen>=2 && MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)");
  cout<<"ceck"<<chek<<endl;

 //matrice normalizzata//
 TMatrixD norm(4,4);
 for(i=0;i<=3;i++){
   norm[i][0]=njetsMatrix[i][0]/(njetsMatrix[0][0]+njetsMatrix[1][0]+njetsMatrix[2][0]+njetsMatrix[3][0]);	   
   norm[i][1]=njetsMatrix[i][1]/(njetsMatrix[0][1]+njetsMatrix[1][1]+njetsMatrix[2][1]+njetsMatrix[3][1]); 
   norm[i][2]=njetsMatrix[i][2]/(njetsMatrix[0][2]+njetsMatrix[1][2]+njetsMatrix[2][2]+njetsMatrix[3][2]);
   norm[i][3]=njetsMatrix[i][3]/(njetsMatrix[0][3]+njetsMatrix[1][3]+njetsMatrix[2][3]+njetsMatrix[3][3]);}
cout<<"elemento1 DELLA NORM "<<norm(0,0)<<endl;
 Double_t determinante=norm.Determinant();
 cout<<"il determinante della norm è: "<<determinante<<endl;

 norm.Print();


 //matrice inversa//
 TMatrixD inversa(4,4);
 inversa=norm.Invert();
 inversa.Print();
 cout<<"elemento 1 dell'inversa "<<inversa(0,0)<<endl;
 TVectorD hnjets(4);
hnjets[0]= ZMM_MAD->GetEntries(" njetscounterGen==0 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 
hnjets[1]= ZMM_MAD->GetEntries(" njetscounterGen==1 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
hnjets[2]= ZMM_MAD->GetEntries(" njetscounterGen==2 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 ");
hnjets[3]= ZMM_MAD->GetEntries(" njetscounterGen>=3 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");


 hnjets.Print();
		 //)={componente1,componente2,componente3,componente4};

 cout<<"stampo il vettore che deve dare la componenti njet"<<endl;
 TVectorD controllo(4);
 controllo=(norm)*hnjets;
 controllo.Print();
 double componente1_gen= ZMM_MAD->GetEntries(" njetscounter==0 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 cout<<"componente 1:  "<<componente1_gen<<endl;
double componente2_gen= ZMM_MAD->GetEntries(" njetscounter==1 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 ");
cout<<"componente 2:  "<<componente2_gen<<endl;
double componente3_gen= ZMM_MAD->GetEntries(" njetscounter==2 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 ");
cout<<"componente 3:  "<<componente3_gen<<endl;
double componente4_gen= ZMM_MAD->GetEntries(" njetscounter>=3 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 ");
cout<<"componente 4:  "<<componente4_gen<<endl;


 Canva1 = new TCanvas("Canva1","grafici di risoluzione");


 Canva1.Divide(4,3);


 Canva1 = new TCanvas("Canva1","grafici di risoluzione");
 Canva1.Divide(4,3);



 //grafici MZ-MR//

 Canva1.cd(1);
    

 ZMM_MAD->Draw("((MR-MZ)/MZ)"," MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");

     
 Canva1.cd(1).SaveAs("MZ_MR_MAD.eps");

 Canva1.cd(2);
 ZMM_MAD->Draw("(((MR-MZ)/MZ)):MZ>>h1(20,85,98,10,-0.1,0.1)"," MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
    
 TH2F *h1 = (TH2F*)gDirectory->Get("h1");
 
 h1->FitSlicesY();
 Canva1.cd(2).SaveAs("MR_MZvsMZ_MAD.eps");
    
    

 //grafici MZgen-MZ//

 Canva1.cd(3);
 ZMM_MAD->Draw("(MZgen-MZ)/MZ"," MR>60 &&MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");

     
 Canva1.cd(3).SaveAs("resMZ_MZgen_MAD.eps");

 Canva1.cd(4);
 ZMM_MAD->Draw("((MZgen-MZ)/MZ):MZ>>h2(20,88,96,10,-0.02,0.02)","  MR>60 &&MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 TH2F *h2 = (TH2F*)gDirectory->Get("h2");

 h2->FitSlicesY();
 Canva1.cd(4).SaveAs("MZgen_MZvsMZ_MAD.eps");
    

 //grafici PTZgen-PTZ//
         
   
 Canva1.cd(5);
 ZMM_MAD->Draw("(PTZ-PTZgen)/PTZ"," MR>60 &&MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
     
    
 Canva1.cd(5).SaveAs("resPTZ_PTZgen2_MAD.eps");
    
 Canva1.cd(6);

 ZMM_MAD->Draw("((PTZ-PTZgen)/PTZ):PTZ>>h3(200,0,200,20,-0.5,0.5)"," MR>60 &&MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 TH2F *h3 = (TH2F*)gDirectory->Get("h3");

 h3->FitSlicesY();
 Canva1.cd(6).SaveAs("PTZgen_PTZvsPTZ_MAD.eps");
    


 //grafici PTZrec-PTZ//

 Canva1.cd(7);
 ZMM_MAD->Draw("(PTZ-PTZrec):PTZ>>hptz"," MR>60 &&MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");

 TProfile *hptz= (TProfile*)gDirectory->Get("hptz");
 Canva1.cd(7).SaveAs ("resPTZ_PTZrec_vsPTZ_MAD.eps");

 Canva1.cd(8);
 ZMM_MAD->Draw("((PTZ-PTZrec)/PTZ):PTZ>>h4(200,1,200,20,-0.5,0.5)"," MR>60 &&MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
 TH2F *h4= (TH2F*)gDirectory->Get("h4");

 h4->FitSlicesY();
 Canva1.cd(8).SaveAs("PTZrec_PTZvsPTZ_MAD.eps");
    


 //grafici pt1-pt1gen//

 Canva1.cd(9);
 ZMM_MAD->Draw("(pt1-pt1gen)/pt1:eta1>>h5(20,-2.5,2.5,20,-0.2,0.2)","  MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
    

 TH2F *h5 = (TH2F*)gDirectory->Get("h5");

 h5->FitSlicesY();
 Canva1.cd(9).SaveAs("pt1_pt1genvspt1_MAD.eps");
    
     

 //grafici pt2-pt2gen//
 
 Canva1.cd(10);
 ZMM_MAD->Draw("(pt2-pt2gen)/pt2:eta2>>h6(20,-2.5,2.5,20,-0.2,0.2)","  MR>60 &&MR<120 &&  abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");

 TH2F *h6 = (TH2F*)gDirectory->Get("h6");

 h6->FitSlicesY();
 Canva1.cd(10).SaveAs("pt2_pt2genvspt2_MAD.eps");
    
     

 //grafico massa invariante generata//

 Canva1.cd(11);
 ZMM_MAD->Draw("MZ","( MR>60 &&MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
   
    
 Canva1.cd(11).SaveAs("MZ_MAD.eps");

 

 //grafico massa invariante ricostruita//

 Canva1.cd(12);


 ZMM_MAD->SetLineColor(kRed);
 ZMM_MAD->Draw("MR","( MR>60 &&MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");

 ZMM->SetLineColor(kBlue);
 ZMM->Draw("MR","( MR>60 &&MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.0627597","sames");
   
 Canva1.cd(12).SaveAs("MR_MAD.eps");
 Canva1.SaveAs("residui_MAD.eps");


 //grafici dei Jet leading jet//

 Canva7 = new TCanvas("Canva7","risoluzione in pt del leading jet");
      
 ZMM_MAD->Draw("res_ptGenJet_ptJet_match:jetpt>>h7(35,30,60,10,-10,1)"," MR>60 &&MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");

 TH2F *h7 = (TH2F*)gDirectory->Get("h7");

 h7->FitSlicesY();
 Canva7.SaveAs("residuiptjet_ptgenjet_MAD.eps");


 Canva8 = new TCanvas("Canva8","residui del pt del leading jet vs etajet");
      
 ZMM_MAD->Draw("res_ptGenJet_ptJet_match:jeteta>>h8(20,-3,3,10,-5,5)","jetpt>30 &&  MR>60 &&MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");  
 Canva8.SaveAs("residui_ptjet_vs_etajet_MAD.eps");

 TH2F *h8 = (TH2F*)gDirectory->Get("h8");
 h8->FitSlicesY();


 Canva9= new TCanvas("Canva9","njetscounter vs njetscounterGen");
 Canva9.Divide(2,1);
 Canva9.cd(1);
 ZMM_MAD->Draw("njetscounterGen:njetscounter>>hnj"," MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");  
 ; 

 TH2F *hnj = (TH2F*)gDirectory->Get("hnj");
 hnj->Draw("BOX");


 //metrice njet:njetgen//

 TMatrixD njetsMatrix [4][4];
 for(i=0;i<4;i++){
   for(j=0;j<4;j++){njetsMatrix[i][j]=njetsMatrix[ZMM_MAD->GetEntries("njetscounter==i"," MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)"][ZMM_MAD->GetEntries("njetscounterGen==j","MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3")];}}
 



 // for (i=0,3){njetsMatrix[i][4]=ZMM_MAD->GetEntries(njetscounter==i && njetscounterGen==4);}
 //for (j=0,3){njetsMatrix[4][j]=ZMM_MAD->GetEntries(njetscounter==4 && njetscounterGen==j);}
 // njetsMatrix[4][4]=hnj4->GetbinContent();

 njetsMatrix[0][0]=  hnj->GetBinContent(1,1);
 cout<<"contenuto 0,0: "<<hnj->GetBinContent(1,1)<<endl;
 njetsMatrix[1][0]= hnj->GetBinContent(1,2);
 cout<<"contenuto 1,0: "<<hnj->GetBinContent(1,2)<<endl;
 njetsMatrix[2][0]= hnj->GetBinContent(1,3); 
 cout<<"contenuto 2,0: "<<hnj->GetBinContent(1,3)<<endl;
 njetsMatrix[3][0]=  hnj->GetBinContent(1,4);
 cout<<"contenuto 3,0: "<<hnj->GetBinContent(1,4)<<endl; 
    


   //   cout<<"il numero di eventi con 0 jet ricostruiti è"<<sum_00<<endl;
   // double entries00= ZMM_MAD->GetEntries("njetscounter==0&& njetscounter <=4 && MR>60 &&MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3");
   //cout<<"il numero di eventi con 0 jet ricostruiti è: "<<entries00<<endl;

 njetsMatrix[0][1]= hnj->GetBinContent(2,1);
      
 njetsMatrix[1][1]=  hnj->GetBinContent(2,2);

 njetsMatrix[2][1]= hnj->GetBinContent(2,3);
  
 njetsMatrix[3][1]=  hnj->GetBinContent(2,4);
  
     

  

 njetsMatrix[0][2]=  hnj->GetBinContent(3,1);
  
 njetsMatrix[1][2]=  hnj->GetBinContent(3,2);

 njetsMatrix[2][2]= hnj->GetBinContent(3,3);

 njetsMatrix[3][2]=  hnj->GetBinContent(3,4);
  



 njetsMatrix[0][3]=  hnj->GetBinContent(4,1);
  
 njetsMatrix[1][3]=  hnj->GetBinContent(4,2);

 njetsMatrix[2][3]= hnj->GetBinContent(4,3);
  
 njetsMatrix[3][3]= hnj->GetBinContent(4,4);
  
      

      //confronto jetmultiplicity
      Canva9.cd(2);

      ZMM->SetLineColor(kBlue);
      ZMM->Draw("njetscounter","( MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.0627597");  
      gPad->SetLogy();

      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("njetscounter"," (MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames"); 
      gPad->SetLogy();

      Canva9.cd(1).SaveAs("njet_njetGen_MAD.eps");
      Canva9.cd(2).SaveAs("jetMolteplicityMAD_confronto.eps");


      //MZgen confronto
      CanvaMZgen=new TCanvas ("CanvaMZgen","MZgen");
      

      ZMM_MAD->SetLineColor(kRed); 
      ZMM_MAD->Draw("MZgen","( MR>60 &&MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)");
      ZMM->SetLineColor(kBlue); 
      ZMM->Draw("MZgen","( MR>60 &&MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)","sames");
   
      CanvaMZgen.SaveAs("MZgen_MAD.eps");

      //grafici primo muone inclusivi//

      Canva2 = new TCanvas ("Canva2","pt1 for 0,1,2,3 jets");
  
      ZMM_MAD->SetLineColor(kBlue); 
      ZMM_MAD->Draw("pt1","(njetscounter>=0 && MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      gPad->SetLogy();

      ZMM_MAD->SetLineColor(kRed); 
      ZMM_MAD->Draw("pt1","(njetscounter>=1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
      gPad->SetLogy();


      ZMM_MAD->SetLineColor(kViolet); 
      ZMM_MAD->Draw("pt1","(njetscounter>=2 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
      gPad->SetLogy();
 
      ZMM_MAD->SetLineColor(kBlack); 
      ZMM_MAD->Draw("pt1","(njetscounter>=3 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
      gPad->SetLogy();

      Canva2.SaveAs("pt1_forNjet_log_MAD_inclusivo.eps");
   
      Canva3 = new TCanvas ("Canva3","eta1 for 0,1,2,3 jets");

      ZMM_MAD->SetLineColor(kBlue); 
      ZMM_MAD->Draw("eta1","(njetscounter>=0 && MR>60 && MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      gPad->SetLogy();

 
      ZMM_MAD->SetLineColor(kRed); 
      ZMM_MAD->Draw("eta1","(njetscounter>=1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
      gPad->SetLogy();


      ZMM_MAD->SetLineColor(kBlack); 
      ZMM_MAD->Draw("eta1","(njetscounter>=3 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
      gPad->SetLogy();

      ZMM_MAD->SetLineColor(kViolet); 
      ZMM_MAD->Draw("eta1","(njetscounter>=2 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
      gPad->SetLogy();
 
      Canva3.SaveAs("eta1_forNjet_log_MAD_inclusivo.eps");
    

      //grafici della Z inclusivi//
      Canva4 = new TCanvas ("Canva4","Grafici delle var.li della Z");
      Canva4.Divide(2,2);
    
      Canva4.cd(1);

      ZMM_MAD->SetLineColor(kBlue); 
      ZMM_MAD->Draw("PTZrec","(njetscounter>=0 && MR>60 && MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      gPad->SetLogy();
      ZMM_MAD->SetLineColor(kRed); 
      ZMM_MAD->Draw("PTZrec","(njetscounter>=1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
      gPad->SetLogy(); 

      ZMM_MAD->SetLineColor(kViolet); 
      ZMM_MAD->Draw("PTZrec","(njetscounter>=2 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 )*0.37","sames");
      gPad->SetLogy();   
      ZMM_MAD->SetLineColor(kBlack); 
      ZMM_MAD->Draw("PTZrec","(njetscounter>=3 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");

      gPad->SetLogy();    
 
      Canva4.cd(2);    
      ZMM_MAD->SetLineColor(kBlue); 
      ZMM_MAD->Draw("PTZgen","(njetscounter>=0 && MR>60 && MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      gPad->SetLogy();
      ZMM_MAD->SetLineColor(kRed); 
      ZMM_MAD->Draw("PTZgen","(njetscounter>=1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5&& (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 )*0.37","sames");
 
      gPad->SetLogy();
      ZMM_MAD->SetLineColor(kViolet); 
      ZMM_MAD->Draw("PTZgen","(njetscounter>=2 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
      gPad->SetLogy();
      ZMM_MAD->SetLineColor(kBlack); 
      ZMM_MAD->Draw("PTZgen","(njetscounter>=3 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");

      gPad->SetLogy();
    
      Canva4.cd(3);    
      ZMM_MAD->SetLineColor(kBlue); 
      ZMM_MAD->Draw("ETAZ","(njetscounter>=0 && MR>60 && MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      gPad->SetLogy();
      ZMM_MAD->SetLineColor(kRed); 
      ZMM_MAD->Draw("ETAZ","(njetscounter>=1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 )*0.37","sames");
      gPad->SetLogy();

      ZMM_MAD->SetLineColor(kViolet); 
      ZMM_MAD->Draw("ETAZ","(njetscounter>=2 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
      gPad->SetLogy();
      ZMM_MAD->SetLineColor(kBlack); 
      ZMM_MAD->Draw("ETAZ","(njetscounter>=3 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5&& (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 )*0.37","sames");
      gPad->SetLogy();
      Canva4.SaveAs("ETAZ_PTZ_forNjet_MAD_inclusivo.eps");
 
      Canva5 = new TCanvas ("Canva5","ptjet (leading jet) for 0,1,2,3 jets");


      // grafici variabili del leading jet ricostruito//
      ZMM_MAD->SetLineColor(kRed); 
      ZMM_MAD->Draw("jetpt","(njetscounter>=1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      gPad->SetLogy();

      ZMM_MAD->SetLineColor(kViolet); 
      ZMM_MAD->Draw("jetpt","(njetscounter>=2 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
      gPad->SetLogy();

      ZMM_MAD->SetLineColor(kBlack); 
      ZMM_MAD->Draw("jetpt","(njetscounter>=3 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
      gPad->SetLogy();

      Canva5.SaveAs("ptjet_forNjet_MAD_inclusivo.eps");

  
      Canva6 = new TCanvas ("Canva6","etajet (leading jet) for 0,1,2,3 jets");

  
      ZMM_MAD->SetLineColor(kRed); 
      ZMM_MAD->Draw("jeteta","(njetscounter>=1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
 

      ZMM_MAD->SetLineColor(kViolet); 
      ZMM_MAD->Draw("jeteta","(njetscounter>=2 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
   
      ZMM_MAD->SetLineColor(kBlack); 
      ZMM_MAD->Draw("jeteta","(njetscounter>=3 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37","sames");
    
      Canva6.SaveAs("etajet_forNjet_MAD_inclusivo.eps");
    

      //grafici PTbalance//
      CanvaPTbalance = new TCanvas ("PTbalance","PTZ/ptjet");

      CanvaPTbalance.Divide(3,1);

      CanvaPTbalance.cd(1);
      ZMM_MAD->Draw("PTZrec/jetpt"," njetscounter==1&& njetscounterGen==1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZ>30");
 

     
      CanvaPTbalance.cd(2);
      ZMM_MAD->Draw("PTZrec/jetpt:jeteta>>hptBalance1"," njetscounter==1&& njetscounterGen==1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZ>30");
 

      TH2F *hptBalance1 = (TH2F*)gDirectory->Get("hptBalance1");

      hptBalance1->FitSlicesY();

      CanvaPTbalance.cd(3);
      ZMM_MAD->Draw("PTZrec/jetpt:jetpt>>hptBalance2(35,20,90,10,0,2)"," njetscounter==1&&njetscounterGen==1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZ>30");

      TH2F *hptBalance2 = (TH2F*)gDirectory->Get("hptBalance2");

      hptBalance2->FitSlicesY();

      CanvaPTbalance.SaveAs("PTZbalanceREC.eps");

      //confronto PTZ//
      CanvaPTZ_confronto_norm=new TCanvas("CanvaPTZ_confronto","confronto tra MADGRAPH e PYTHIA PTZ");
      CanvaPTZ_confronto_norm.Divide(2,2);

      CanvaPTZ_confronto_norm.cd(1);
    

      ZMM->SetLineColor(kBlue);
      ZMM->Draw("PTZrec>>hPTZrec0","(njetscounter>=0 && MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)");

      TH1F *hPTZrec0 = (TH1F*)gDirectory->Get("hPTZrec0");
      hPTZrec0->Scale(1/hPTZrec0->Integral());
      hPTZrec0->Draw();

      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("PTZrec>>hPTZrec0_MAD","(njetscounter>=0 && MR>60 && MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.37","sames");

      TH1F *hPTZrec0_MAD = (TH1F*)gDirectory->Get("hPTZrec0_MAD");
      hPTZrec0_MAD->Scale(1/hPTZrec0_MAD->Integral());
      hPTZrec0_MAD->Draw("sames");

      CanvaPTZ_confronto_norm.cd(2);
    
      ZMM->SetLineColor(kBlue);
      ZMM->Draw("PTZrec>>hPTZrec1","(njetscounter>=1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 &&PTZrec<200)*0.0627597");

      TH1F *hPTZrec1 = (TH1F*)gDirectory->Get("hPTZrec1");
      hPTZrec1->Scale(1/hPTZrec1->Integral());
      hPTZrec1->Draw();

      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("PTZrec>>hPTZrec1_MAD","(njetscounter>=1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.37","sames");

      TH1F *hPTZrec1_MAD = (TH1F*)gDirectory->Get("hPTZrec1_MAD");
      hPTZrec1_MAD->Scale(1/hPTZrec1_MAD->Integral());
      hPTZrec1_MAD->Draw("sames");

      CanvaPTZ_confronto_norm.cd(3);

           
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("PTZrec>>hPTZrec2_MAD","(njetscounter>=2 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.37");

      TH1F *hPTZrec2_MAD = (TH1F*)gDirectory->Get("hPTZrec2_MAD");
      hPTZrec2_MAD->Scale(1/hPTZrec2_MAD->Integral());
      hPTZrec2_MAD->Draw();

      ZMM->SetLineColor(kBlue);
      ZMM->Draw("PTZrec>>hPTZrec2","(njetscounter>=2 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3&& PTZrec<200)*0.0627597","sames");

      TH1F *hPTZrec2 = (TH1F*)gDirectory->Get("hPTZrec2");
      hPTZrec2->Scale(1/hPTZrec2->Integral());
      hPTZrec2->Draw("sames");

      CanvaPTZ_confronto_norm.cd(4);

      ZMM->SetLineColor(kBlue);
      ZMM->Draw("PTZrec>>hPTZrec3","(njetscounter>=3 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.0627597");

      TH1F *hPTZrec3 = (TH1F*)gDirectory->Get("hPTZrec3");
      hPTZrec3->Scale(1/hPTZrec3->Integral());
      hPTZrec3->Draw();
      
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("PTZrec>>hPTZrec3_MAD","(njetscounter>=3 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.37","sames");

      TH1F *hPTZrec3_MAD = (TH1F*)gDirectory->Get("hPTZrec3_MAD");
      hPTZrec3_MAD->Scale(1/hPTZrec3_MAD->Integral());
      hPTZrec3_MAD->Draw("sames");

     
      CanvaPTZ_confronto_norm.SaveAs("confrontoPTZ_norm.eps");


      CanvaPTZ_confronto=new TCanvas("CanvaPTZ_confronto","confronto tra MADGRAPH e PYTHIA PTZ");
      CanvaPTZ_confronto.Divide(2,2);

      CanvaPTZ_confronto.cd(1);

          
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("PTZrec","(njetscounter>=0 && MR>60 && MR<120  && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.37");

      ZMM->SetLineColor(kBlue);
      ZMM->Draw("PTZrec","(njetscounter>=0 && MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.0627597","sames");

      CanvaPTZ_confronto.cd(2);
      
      ZMM->SetLineColor(kBlue);
      ZMM->Draw("PTZrec","(njetscounter>=1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.0627597");

     
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("PTZrec","(njetscounter>=1 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.37","sames");


      CanvaPTZ_confronto.cd(3);
      
      
     
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("PTZrec","(njetscounter>=2 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.37");

      ZMM->SetLineColor(kBlue);
      ZMM->Draw("PTZrec","(njetscounter>=2 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.0627597","sames");


      CanvaPTZ_confronto.cd(4);
      
          
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("PTZrec","(njetscounter>=3 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.37");

      ZMM->SetLineColor(kBlue);
      ZMM->Draw("PTZrec","(njetscounter>=3 && MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 && PTZrec<200)*0.0627597","sames");

      CanvaPTZ_confronto.SaveAs("confronto_PTZrec.eps");


      //confronto ptjet//
      CanvaPTjet_confronto=new TCanvas("CanvaPTjet_confronto","confronto tra MADGRAPH e PYTHIA ptjet");

      CanvaPTjet_confronto.Divide(3,1);
      CanvaPTjet_confronto.cd(1);

      ZMM->SetLineColor(kBlue);
      ZMM->Draw("jetpt>>hjetpt1","( njetscounter>=1&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)");
      gPad->SetLogy();

      TH1F *hjetpt1 = (TH1F*)gDirectory->Get("hjetpt1");
      hjetpt1->Scale(1/hjetpt1->Integral());
      hjetpt1->Draw();

      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("jetpt>>hjetpt1_MAD","( njetscounter>=1&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)","sames");
      gPad->SetLogy();
 
      TH1F *hjetpt1_MAD = (TH1F*)gDirectory->Get("hjetpt1_MAD");
      hjetpt1_MAD->Scale(1/hjetpt1_MAD->Integral());
      hjetpt1_MAD->Draw("sames");
     

      CanvaPTjet_confronto.cd(2);

      ZMM->SetLineColor(kBlue);
      ZMM->Draw("jetpt>>hjetpt2","( njetscounter>=2&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)");
      gPad->SetLogy();

      TH1F *hjetpt2 = (TH1F*)gDirectory->Get("hjetpt2");
      hjetpt2->Scale(1/hjetpt2->Integral());
      hjetpt2->Draw();

      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("jetpt>>hjetpt2_MAD","( njetscounter>=2&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)","sames");
      gPad->SetLogy();

      TH1F *hjetpt2_MAD = (TH1F*)gDirectory->Get("hjetpt2_MAD");
      hjetpt2_MAD->Scale(1/hjetpt2_MAD->Integral());
      hjetpt2_MAD->Draw("sames");

      CanvaPTjet_confronto.cd(3);

      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("jetpt>>hjetpt3","( njetscounter>=3&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)");
      gPad->SetLogy();

      TH1F *hjetpt3 = (TH1F*)gDirectory->Get("hjetpt3");
      hjetpt3->Scale(1/hjetpt3->Integral());
      hjetpt3->Draw();

      ZMM->SetLineColor(kBlue);
      ZMM->Draw("jetpt>>hjetpt3_MAD","( njetscounter>=3&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)","sames");
      gPad->SetLogy();

      TH1F *hjetpt3_MAD = (TH1F*)gDirectory->Get("hjetpt3_MAD");
      hjetpt3_MAD->Scale(1/hjetpt3_MAD->Integral());
      hjetpt3_MAD->Draw("sames");

      CanvaPTjet_confronto.SaveAs("confronto_jetpt_norm.eps");


      //confronto ETAjet//
      CanvaETAjet_confronto= new TCanvas("CanvaETAjet_confronto","eta jet confronto");
      CanvaETAjet_confronto.Divide(3,1);
      CanvaETAjet_confronto.cd(1);

      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("jeteta","( njetscounter>=1&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      ZMM->SetLineColor(kBlue);
      ZMM->Draw("jeteta","( njetscounter>=1&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.0627597","sames");

      CanvaETAjet_confronto.cd(2);

      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("jeteta","( njetscounter>=2&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      ZMM->SetLineColor(kBlue);
      ZMM->Draw("jeteta","( njetscounter>=2&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.0627597","sames");


      CanvaETAjet_confronto.cd(3);

     
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("jeteta","( njetscounter>=3&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      ZMM->SetLineColor(kBlue);
      ZMM->Draw("jeteta","( njetscounter>=3&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.0627597","sames");

      CanvaETAjet_confronto.SaveAs("confronto_etajet.eps");


      //confronto etamu//
      Canvaeta1_confronto=new TCanvas("Canvaeta1_confronto","eta muon confronto");
      Canvaeta1_confronto.Divide(2,2);

      Canvaeta1_confronto.cd(1);

     
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("eta1>>hetamu0_MAD","( njetscounter>=0&& MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)");

      TH1F *hetamu0_MAD = (TH1F*)gDirectory->Get("hetamu0_MAD");
      hetamu0_MAD->Scale(1/hetamu0_MAD->Integral());
      hetamu0_MAD->Draw();


      ZMM->SetLineColor(kBlue);
      ZMM->Draw("eta1>>hetamu0","( njetscounter>=0&& MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)","sames");

      TH1F *hetamu0 = (TH1F*)gDirectory->Get("hetamu0");
      hetamu0->Scale(1/hetamu0->Integral());
      hetamu0->Draw("sames");

      Canvaeta1_confronto.cd(2);
    
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("eta1>>hetamu1_MAD","( njetscounter>=1&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)");

      TH1F *hetamu1_MAD = (TH1F*)gDirectory->Get("hetamu1_MAD");
      hetamu1_MAD->Scale(1/hetamu1_MAD->Integral());
      hetamu1_MAD->Draw();


      ZMM->SetLineColor(kBlue);
      ZMM->Draw("eta1>>hetamu1","( njetscounter>=1&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)","sames");

      TH1F *hetamu1 = (TH1F*)gDirectory->Get("hetamu1");
      hetamu1->Scale(1/hetamu1->Integral());
      hetamu1->Draw("sames");


      Canvaeta1_confronto.cd(3);

     
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("eta1>>hetamu2_MAD","( njetscounter>=2&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)");

      TH1F *hetamu2_MAD = (TH1F*)gDirectory->Get("hetamu2_MAD");
      hetamu2_MAD->Scale(1/hetamu2_MAD->Integral());
      hetamu2_MAD->Draw();


      ZMM->SetLineColor(kBlue);
      ZMM->Draw("eta1>>hetamu2","( njetscounter>=2&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)","sames");

      TH1F *hetamu2 = (TH1F*)gDirectory->Get("hetamu2");
      hetamu2->Scale(1/hetamu2->Integral());
      hetamu2->Draw("sames");

      Canvaeta1_confronto.cd(4);

     
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("eta1>>hetamu3_MAD","( njetscounter>=3&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)");

      TH1F *hetamu3_MAD = (TH1F*)gDirectory->Get("hetamu3_MAD");
      hetamu3_MAD->Scale(1/hetamu3_MAD->Integral());
      hetamu3_MAD->Draw();

      ZMM->SetLineColor(kBlue);
      ZMM->Draw("eta1>>hetamu3","( njetscounter>=3&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)","sames");

      TH1F *hetamu3 = (TH1F*)gDirectory->Get("hetamu3");
      hetamu3->Scale(1/hetamu3->Integral());
      hetamu3->Draw("sames");

      Canvaeta1_confronto.SaveAs("confronto_eta1_norm.eps");



      //confronto ptmu//
      Canvapt1_confronto=new TCanvas("Canvapt1_confronto","pt muon confronto");
      Canvapt1_confronto.Divide(4,1);
      Canvapt1_confronto.cd(1);

     
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("pt1","( njetscounter>=0&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      ZMM->SetLineColor(kBlue);
      ZMM->Draw("pt1","( njetscounter>=0&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.0627597","sames");

      Canvapt1_confronto.cd(2);
     
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("pt1","( njetscounter>=1&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      ZMM->SetLineColor(kBlue);
      ZMM->Draw("pt1","( njetscounter>=1&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.0627597","sames");


      Canvapt1_confronto.cd(3);

      
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("pt1","( njetscounter>=2&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37");
      ZMM->SetLineColor(kBlue);
      ZMM->Draw("pt1","( njetscounter>=2&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.0627597","sames");

      Canvapt1_confronto.cd(4);

     
      ZMM_MAD->SetLineColor(kRed);
      ZMM_MAD->Draw("pt1","( njetscounter>=3&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.37" );
      ZMM->SetLineColor(kBlue);
      ZMM->Draw("pt1","( njetscounter>=3&& MR>60 && MR<120 && abs(jeteta)<3 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)*0.0627597","sames");

      Canvapt1_confronto.SaveAs("confronto_eta1.eps");


 }
}
