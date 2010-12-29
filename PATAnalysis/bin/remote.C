{
  //This make the TSelectors in the library available to the remote proof session
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libFirenzePATAnalysis");
  TString path = "/raid/lenzip/Zjets/CMSSW_3_8_7/bin/slc5_ia32_gcc434:/afs/cern.ch/cms/slc5_ia32_gcc434/cms/cmssw/CMSSW_3_8_7/bin/slc5_ia32_gcc434:/afs/cern.ch/cms/slc5_ia32_gcc434/cms/pool/POOL_2_9_8_pre2-cms11/slc5_ia32_gcc434/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/xdaq/VR16021-cms2/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/graphviz/2.16.1-cms/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/lcg/root/5.22.00d-cms30/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/qt/4.5.2-cms2/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/oracle/11.2.0.1.0p2/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/millepede/03.00.00-cms3/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/python/2.6.4-cms6/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/xrootd/20090727.1318-cms3/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/valgrind/3.5.0-cms/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/sqlite/3.6.10/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/gmake/3.81/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/glimpse/4.18.5/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/gdb/7.1-cms/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/expat/2.0.0-cms/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/cms/dbs-client/DBS_2_0_9_patch_4-cms2/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/db4/4.4.20/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/curl/7.15.3/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/gccxml/0.9.0_20100114-cms/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/alpgen/213-cms/bin:/afs/cern.ch/cms/slc5_ia32_gcc434/external/gcc/4.3.4/bin:/afs/cern.ch/cms/common:/afs/cern.ch/cms/slc5_ia32_gcc434/lcg/SCRAMV1/V2_2_0/bin:/afs/cern.ch/user/l/lenzip/bin:/afs/cern.ch/user/l/lenzip/scripts:/usr/sue/bin:/afs/cern.ch/cms/bin/amd64_linux26:/afs/cern.ch/cms/system/bin:/usr/local/bin:/usr/local/bin/X11:/usr/bin:/bin:/usr/bin/X11:/cern/pro/bin:/afs/cern.ch/cms/caf/scripts:/afs/cern.ch/cms/common:/afs/cern.ch/cms/bin:/afs/cern.ch/cms/utils:/usr/kerberos/bin"; 
 

  TString ldpath = "/raid/lenzip/Zjets/CMSSW_3_8_7/lib/slc5_ia32_gcc434:/raid/lenzip/Zjets/CMSSW_3_8_7/external/slc5_ia32_gcc434/lib:/afs/cern.ch/cms/slc5_ia32_gcc434/cms/cmssw/CMSSW_3_8_7/lib/slc5_ia32_gcc434:/afs/cern.ch/cms/slc5_ia32_gcc434/cms/cmssw/CMSSW_3_8_7/external/slc5_ia32_gcc434/lib:/afs/cern.ch/cms/slc5_ia32_gcc434/external/boost/1.42.0-cms/lib:/afs/cern.ch/cms/slc5_ia32_gcc434/external/xrootd/20090727.1318-cms3/lib:/afs/cern.ch/cms/slc5_ia32_gcc434/external/clhep/2.0.4.2-cms/lib:/afs/cern.ch/cms/slc5_ia32_gcc434/external/gcc/4.3.4/lib:/afs/cern.ch/cms/slc5_ia32_gcc434/lcg/SCRAMV1/V2_2_0/lib"; 
  gSystem->Setenv("PATH",path);

  gSystem->Setenv("LD_LIBRARY_PATH",ldpath);

}

