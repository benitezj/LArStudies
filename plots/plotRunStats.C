#include "globals.h"
//#include "../LArStudies/tools.h"


void plotRunStats(){
  gROOT->ProcessLine(".x LArStudies/run/RootLogon.C");

  std::vector<string> runs=getListOfRuns();
  cout<<"# of runs : "<<runs.size()<<endl;
  
  TH1F HEvents("HEvents","",runs.size(),0.5,0.5+runs.size());
  HEvents.GetXaxis()->SetTitle("run");
  HEvents.GetYaxis()->SetTitle("number of events");
  
  for(int i=0;i<runs.size();i++){
    
    TString fileName=(const char*)(NTUPLEPATH+"/data15_comm."+TString(runs.at(i).c_str())+".physics_CosmicCalo.merge.RAW/merged.root");
    TFile INFILE(fileName,"read");
    if(INFILE.IsZombie()){cout<<"INFILE is Zombie"<<endl;continue;}
    TH1F* H=(TH1F*)INFILE.Get("HistoRMSCoh_Det0_Side0_FT0_Slot1");//this histo is filled once per event
    if(!H){cout<<"Histo not found for: "<<fileName<<endl; continue;}
    HEvents.AddBinContent(i+1,H->GetBinContent(1));    

    // TString fileName=(const char*)(NTUPLEPATH+"/data15_comm."+TString(runs.at(i).c_str())+".physics_CosmicCalo.merge.RAW");
    // std::vector<std::string> files=findSamplesInDir(fileName.Data(),".root");
    // for(int f=0;f<files.size();f++){
    //   TFile INFILE(fileName+"/"+files[f].c_str(),"read");
    //   TH1F* H=(TH1F*)INFILE.Get("HistoRMSCoh_Det0_Side0_FT0_Slot1");//this histo is filled once per event
    //   if(!H){cout<<"Histo not found for: "<<files[f]<<endl; continue;}
    //   HEvents.AddBinContent(i+1,H->GetBinContent(1));
    // }

    cout<<fileName<<" : "<<HEvents.GetBinContent(i+1)<<endl;

  }

  TCanvas C;
  HEvents.Draw("hist");

  HEvents.GetYaxis()->SetTitleSize(0.06);
  HEvents.GetYaxis()->SetTitleOffset(0.85);
  HEvents.GetXaxis()->SetNdivisions(0);
  drawXLabels(HEvents.GetXaxis(),runs);
  C.Print(PLOTSPATH+"/EventsPerRun.gif");


  gROOT->ProcessLine(".q");
}
