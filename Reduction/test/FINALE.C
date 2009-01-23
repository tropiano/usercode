void FINALE()
{
  gROOT->Reset();
  // gROOT->SetStyle("Plain");
  gROOT->SetStyle("BABAR");


     


 TFile* f = new TFile("fondoWjj.root");
 TFile* t = new TFile("histo_1M_MADGRAPH_ISO_tot.root");
 TFile* s = new TFile("fondoTT.root");

 TTree* ZMM_fondo=(TTree*)f->Get("verySimplePATAnalysis/ZMM_fondo");
 TTree* ZMM_MAD=(TTree*)t->Get("verySimplePATAnalysis/ZMM_MAD");
 TTree* ZMM_fondo_TT=(TTree*)s->Get("verySimplePATAnalysis/ZMM_fondo_TT");

 //correzione del fondo//

int nentries_MAD_4=ZMM_MAD->GetEntries("(MR>60 && MR<120&&pt1>20 && pt2>20 &&abs(eta1)<2.5 && abs(eta2)<2.5  && (charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3 && njetscounterGen==0)*0.37");
int nentries_TT_4=ZMM_fondo_TT->GetEntries("(MR>60 && MR<120 && pt1>20 && pt2>20 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3 && njetscounterGen==0)*0.0352");
int nentries_fondo_4=ZMM_fondo->GetEntries("(MR>60 && MR<120 && pt1>20 && pt2>20 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3 && njetscounterGen==0)*0.740");


 cout<<"nemero di entries nel per il segnale dopo i tagli sulla MR: "<<nentries_MAD_4<<endl;

 cout<<"nemero di entries nel per il fondoTT dopo i tagli sulla MR: "<<nentries_TT_4<<endl;

 cout<<"nemero di entries nel per il fondoWjj dopo i tagli sulla MR: "<<nentries_fondo_4<<endl;

int nentries_MAD_4_1=ZMM_MAD->GetEntries("(MR>60 && MR<120&&pt1>20 && pt2>20 &&abs(eta1)<2.5 && abs(eta2)<2.5  && (charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3&& njetscounterGen==1)*0.37");
 int nentries_TT_4_1=ZMM_fondo_TT->GetEntries("(MR>60 && MR<120 && pt1>20 && pt2>20 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3&& njetscounterGen==1)*0.0352");
 int nentries_fondo_4_1=ZMM_fondo->GetEntries("(MR>60 && MR<120 && pt1>20 && pt2>20 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3&& njetscounterGen==1)*0.740");

 cout<<"nemero di entries nel per il segnale dopo i tagli sulla MR: "<<nentries_MAD_4_1<<endl;

 cout<<"nemero di entries nel per il fondoTT dopo i tagli sulla MR: "<<nentries_TT_4_1<<endl;

 cout<<"nemero di entries nel per il fondoWjj dopo i tagli sulla MR: "<<nentries_fondo_4_1<<endl;

int nentries_MAD_4_2=ZMM_MAD->GetEntries("(MR>60 && MR<120&&pt1>20 && pt2>20 &&abs(eta1)<2.5 && abs(eta2)<2.5  && (charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3&& njetscounterGen==2)*0.37");
 int nentries_TT_4_2=ZMM_fondo_TT->GetEntries("(MR>60 && MR<120 && pt1>20 && pt2>20 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3&& njetscounterGen==2)*0.0352");
 int nentries_fondo_4_2=ZMM_fondo->GetEntries("(MR>60 && MR<120 && pt1>20 && pt2>20 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3&& njetscounterGen==2)*0.740");


 cout<<"nemero di entries nel per il segnale dopo i tagli sulla MR: "<<nentries_MAD_4_2<<endl;

 cout<<"nemero di entries nel per il fondoTT dopo i tagli sulla MR: "<<nentries_TT_4_2<<endl;

 cout<<"nemero di entries nel per il fondoWjj dopo i tagli sulla MR: "<<nentries_fondo_4_2<<endl;

int nentries_MAD_4_3=ZMM_MAD->GetEntries("(MR>60 && MR<120&&pt1>20 && pt2>20 &&abs(eta1)<2.5 && abs(eta2)<2.5  && (charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3&& njetscounterGen>=3)*0.37");
 int nentries_TT_4_3=ZMM_fondo_TT->GetEntries("(MR>60 && MR<120 && pt1>20 && pt2>20 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3&& njetscounterGen>=3)*0.0352");
 int nentries_fondo_4_3=ZMM_fondo->GetEntries("(MR>60 && MR<120 && pt1>20 && pt2>20 && abs(eta1)<2.5 && abs(eta2)<2.5 &&(charge1+charge2)==0&&   isolation3_1<3 && isolation3_2<3&& njetscounterGen>=3)*0.740");

 cout<<"nemero di entries nel per il segnale dopo i tagli sulla MR: "<<nentries_MAD_4_3<<endl;

 cout<<"nemero di entries nel per il fondoTT dopo i tagli sulla MR: "<<nentries_TT_4_3<<endl;

 cout<<"nemero di entries nel per il fondoWjj dopo i tagli sulla MR: "<<nentries_fondo_4_3<<endl;


 //matrice//


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
  float chek=ZMM_MAD->GetEntries("(njetscounter==0 && njetscounterGen>=2 && MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3)");
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
 
  //dobbiamo applicare la matrice inversa al vettore dei ricostruiti corretto per il fondo: ma, non dovrei avere un file, quindi un tree, che contiene sia segale che fondo?
 TVectorD hnjetrec(4);
  hnjetrec[0]=( ZMM_MAD->GetEntries(" njetscounter==0 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3"))-(nentries_TT_4+nentries_fondo_4);
  
  hnjetrec[1]=( ZMM_MAD->GetEntries(" njetscounter==1 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 "))-(nentries_TT_4_1+nentries_fondo_4_1);
  
  hnjetrec[2]=( ZMM_MAD->GetEntries(" njetscounter==2 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 "))-(nentries_TT_4_2+nentries_fondo_4_2);
  
  hnjetrec[3]=( ZMM_MAD->GetEntries(" njetscounter>=3 &&MR>60 && MR<120 && abs(eta1)<2.5 && abs(eta2)<2.5 && (charge1+charge2)==0 && isolation3_1<3 && isolation3_2<3 "))-(nentries_TT_4_3+nentries_fondo_4_3);
  hnjetrec.Print();
  
  
  //matrice inversa//
  
  norm.Invert();
  norm.Print();
  TVectorD orabasta(4);
  orabasta=(norm)*hnjetrec;
  //vettore dei generati//
  orabasta.Print();



  //efficienze per cui dobbiamo correggere//

  TVectorD collezioneGen(4);



 float nentries0_prova_Z_gen=ZMM_MAD->GetEntries("njetscounterGen==0 && MR>60 &&MR<120  &&(charge1+charge2)==0 ");
 
 float nentries0_prova_gen=ZMM_MAD->GetEntries("njetscounterGen==0");

 float efficienza_Z_vs_gen=nentries0_prova_Z_gen/nentries0_prova_gen;
 cout<<"efficienza ricostruiti che passano i miei tagli rispetto ai ricostruiti totali per jet==0: "<<efficienza_Z_vs_rec<<endl;
 cout<<"efficienza generati che passano i miei tagli rispetto ai generati totali per jet==0: "<<efficienza_Z_vs_gen<<endl;

 collezioneGen[0]= orabasta[0]*efficienza_Z_vs_gen;



 float nentries1_prova_Z_gen=ZMM_MAD->GetEntries("njetscounterGen==1 && MR>60 &&MR<120  &&(charge1+charge2)==0 ");
 
 float nentries1_prova_gen=ZMM_MAD->GetEntries("njetscounterGen==1");

 float efficienza_Z_vs_gen_1=nentries1_prova_Z_gen/nentries1_prova_gen;

 cout<<"efficienza dei gen rispetto ai gen totali per ==1 jet rec"<< efficienza_Z_vs_gen_1<<endl;
 collezioneGen[1]= orabasta[1]*efficienza_Z_vs_gen_1;


 

 float nentries2_prova_Z_gen=ZMM_MAD->GetEntries("njetscounterGen==2 && MR>60 &&MR<120  &&(charge1+charge2)==0 ");
 
 float nentries2_prova_gen=ZMM_MAD->GetEntries("njetscounterGen==2");

 float efficienza_Z_vs_gen_2=nentries2_prova_Z_gen/nentries2_prova_gen;

 cout<<"efficienza dei gen rispetto ai gen totali per ==2 jet rec"<< efficienza_Z_vs_gen_2<<endl;
 
 collezioneGen[2]= orabasta[2]*efficienza_Z_vs_gen_2;



 float nentries3_prova_Z_gen=ZMM_MAD->GetEntries("njetscounterGen>=3 && MR>60 &&MR<120  &&(charge1+charge2)==0 ");
 
 float nentries3_prova_gen=ZMM_MAD->GetEntries("njetscounterGen>=3");

 float efficienza_Z_vs_gen_3=nentries3_prova_Z_gen/nentries3_prova_gen;

 collezioneGen[3]= orabasta[3]*efficienza_Z_vs_gen_3;

 double Z_inclusivo_gen=collezioneGen[0]+collezioneGen[1]+collezionegen[2]+collezioneGen[3];

 //rapporti finali//

 double rapporto0= collezione[0]/(Z_inclusivo_gen)
   cout<<"rapporto tra Z+0 gen jet rispetto a Z inclusivo"<<rapporto0<<endl;

 double rapporto1= collezione[1]/(Z_inclusivo_gen)
  cout<<"rapporto tra Z+1 gen jet rispetto a Z inclusivo"<<rapporto1<<endl;


 double rapporto2= collezione[2]/(Z_inclusivo_gen)
   cout<<"rapporto tra Z+2 gen jet rispetto a Z inclusivo"<<rapporto2<<endl;


 double rapporto3= collezione[3]/(Z_inclusivo_gen)
   cout<<"rapporto tra Z+>=3 gen jet rispetto a Z inclusivo"<<rapporto3<<endl;

}
