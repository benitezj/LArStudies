
void makeTupleCoherent(TString inpath, TString outpath="."){
  NoiseTupleCoherent tuple(inpath+"/tuple*.root");
  
  //tuple.setDebugLevel(2);
  //tuple.setMaxCells(1000);
  tuple.run(outpath);  
  gROOT->ProcessLine(".q");
}
