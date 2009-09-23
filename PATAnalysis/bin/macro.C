{
  TProof * p = TProof::Open(""); 


//p->SetParameter("PROOF_MaxSlavesPerNode", 4);
//p->SetParameter("PROOF_MinPacketTime", 100);
//p->SetParameter("PROOF_PacketAsAFraction", 1);
  //Need this to allow ROOT to be able to use a ThingsTSelector
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  //Have to load library manually since Proof does not use the 
  // mechanism used by TFile to find class dictionaries and therefore
  // the AutoLibraryLoader can not help
  gSystem->Load("libFirenzePATAnalysis");

  p->Exec( ".x remote.C" );


  //This makes sure the TSelector library and dictionary are properly
  // installed in the remote PROOF servers
  //This creates the 'data set' which defines what files we need to process
  // NOTE: the files given must be accessible by the remote systems
  TDSet c( "TTree", "Events");
  c.Add("/raid/lenzip/CMSSW/CMSSW_3_1_2/src/Firenze/Reduction/test/ZmumuPythia/res/test_pat_174.root");
 
  //This makes the actual processing happen
  //p->Process(&c,"FWLiteTSelector","",500, 2000);
  p->Process(&c,"Background","");//,500, 2000);
  p->Close();
}



