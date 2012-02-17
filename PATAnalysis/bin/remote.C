{
  //This make the TSelectors in the library available to the remote proof session
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  TString path = "/data/sfrosali/Zjets/CMSSW_4_2_5/bin/slc5_amd64_gcc434:/data/sfrosali/Zjets/CMSSW_4_2_5/external/slc5_amd64_gcc434/bin:/afs/cern.ch/cms/slc5_amd64_gcc434/cms/cmssw/CMSSW_4_2_5/bin/slc5_amd64_gcc434:/afs/cern.ch/cms/slc5_amd64_gcc434/cms/cmssw/CMSSW_4_2_5/external/slc5_amd64_gcc434/bin:/afs/cern.ch/cms/slc5_amd64_gcc434/external/gcc/4.3.4-cms/bin:/afs/cern.ch/sw/lcg/app/releases/ROOT/5.30.01/x86_64-slc5-gcc43-opt/root/bin:/afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc34-opt/bin:/afs/cern.ch/user/s/sfrosali/bin:/afs/cern.ch/user/s/sfrosali/scripts:/usr/sue/bin:/afs/cern.ch/cms/bin/amd64_linux26:/afs/cern.ch/cms/system/bin:/usr/local/bin:/usr/local/bin/X11:/usr/bin:/bin:/usr/bin/X11:/cern/pro/bin:/afs/cern.ch/cms/caf/scripts:/afs/cern.ch/cms/common:/afs/cern.ch/cms/bin:/afs/cern.ch/cms/utils:/usr/kerberos/bin:/usr/X11R6/bin"; 

  TString ldpath = "/data/sfrosali/Zjets/CMSSW_4_2_5/lib/slc5_amd64_gcc434:/data/sfrosali/Zjets/CMSSW_4_2_5/external/slc5_amd64_gcc434/lib:/afs/cern.ch/cms/slc5_amd64_gcc434/cms/cmssw/CMSSW_4_2_5/lib/slc5_amd64_gcc434:/afs/cern.ch/cms/slc5_amd64_gcc434/cms/cmssw/CMSSW_4_2_5/external/slc5_amd64_gcc434/lib:/afs/cern.ch/cms/slc5_amd64_gcc434/external/gcc/4.3.4-cms/lib64:/afs/cern.ch/cms/slc5_amd64_gcc434/external/gcc/4.3.4-cms/lib:/afs/cern.ch/sw/lcg/app/releases/ROOT/5.30.01/x86_64-slc5-gcc43-opt/root/lib:/afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc34-opt/lib64:/afs/cern.ch/sw/lcg/contrib/mpfr/2.3.1/x86_64-slc5-gcc34-opt/lib:/afs/cern.ch/sw/lcg/contrib/gmp/4.2.2/x86_64-slc5-gcc34-opt/lib"; 

  gSystem->Setenv("PATH",path);

  gSystem->Setenv("LD_LIBRARY_PATH",ldpath);

}

