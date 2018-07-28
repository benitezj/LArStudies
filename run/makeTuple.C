
void makeTuple(TString infile){

  std::vector<std::string> input;
  input.push_back(infile.Data());
  NoiseTuple tuple(input);
  
  std::vector< std::string > triggerPaths;
  triggerPaths.push_back("L1_RD1_EMPTY");
  tuple.setTriggerPaths(triggerPaths);


  //tuple.setDebugLevel(2);
  //tuple.setMaxCells(2000000);
  tuple.run(".");  
  gROOT->ProcessLine(".q");
}
