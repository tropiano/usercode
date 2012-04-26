{
  //This make the TSelectors in the library available to the remote proof session
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");

  TString path = "/data/sandro/Analisi/rel_CMSSW_4_2_5_TeP/CMSSW_4_2_5/bin/slc5_amd64_gcc434:/data/sandro/Analisi/rel_CMSSW_4_2_5_TeP/CMSSW_4_2_5/external/slc5_amd64_gcc434/bin:/afs/cern.ch/cms/slc5_amd64_gcc434/cms/cmssw/CMSSW_4_2_5/bin/slc5_amd64_gcc434:/afs/cern.ch/cms/slc5_amd64_gcc434/cms/cmssw/CMSSW_4_2_5/external/slc5_amd64_gcc434/bin:/afs/cern.ch/cms/slc5_amd64_gcc434/external/gcc/4.3.4-cms/bin:/afs/cern.ch/user/s/sandro/bin:/afs/cern.ch/user/s/sandro/scripts:/usr/sue/bin:/afs/cern.ch/cms/bin/amd64_linux26:/afs/cern.ch/cms/system/bin:/usr/local/bin:/usr/local/bin/X11:/usr/bin:/bin:/usr/bin/X11:/cern/pro/bin:/afs/cern.ch/cms/caf/scripts:/afs/cern.ch/cms/common:/afs/cern.ch/cms/bin:/afs/cern.ch/cms/utils:/usr/kerberos/bin"; 

  TString ldpath = "/data/sandro/Analisi/rel_CMSSW_4_2_5_TeP/CMSSW_4_2_5/lib/slc5_amd64_gcc434:/data/sandro/Analisi/rel_CMSSW_4_2_5_TeP/CMSSW_4_2_5/external/slc5_amd64_gcc434/lib:/afs/cern.ch/cms/slc5_amd64_gcc434/cms/cmssw/CMSSW_4_2_5/lib/slc5_amd64_gcc434:/afs/cern.ch/cms/slc5_amd64_gcc434/cms/cmssw/CMSSW_4_2_5/external/slc5_amd64_gcc434/lib:/afs/cern.ch/cms/slc5_amd64_gcc434/external/gcc/4.3.4-cms/lib64:/afs/cern.ch/cms/slc5_amd64_gcc434/external/gcc/4.3.4-cms/lib"; 

  gSystem->Setenv("PATH",path);

  gSystem->Setenv("LD_LIBRARY_PATH",ldpath);

}

