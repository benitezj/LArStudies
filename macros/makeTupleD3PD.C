void makeTupleD3PD(){
  gROOT->ProcessLine(".L NoiseTupleD3PD.C++");

  TString inpath="/afs/cern.ch/user/b/benitezj/output/LAr/LArSamples/OctoberRun";  
  TString outpath="/afs/cern.ch/user/b/benitezj/output/LAr/LArSamples/OctoberRun/Tuple";


  TString file="caloD3PD_from_esd_comm.00242147.physics_MinBias.merge.RAW._lb0003._SFO-ALL.root";
  
  NoiseTupleD3PD T((inpath+"/"+file).Data());
  //T.Loop(outpath,1000,1);

  T.setCalibrationPath(inpath);
  T.Loop(outpath,1000000);
  
  gROOT->ProcessLine(".q");
}
