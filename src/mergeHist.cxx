#include "LArStudies/tools.h"

#define NDET 5
#define NSIDE 2
#define NFT 32
#define NFEB 15 
#define NCHAN 128

#include <TFile.h>
#include <TH1F.h>
#include <TString.h>

int main(int argc, char* argv[]){

  if(argc!=2){
    cout<<"Need to provide a input directory name"<<endl;
    return 0;
  }

  TString dir(argv[1]);
  std::vector<std::string> files=findSamplesInDir(dir.Data(),".root");
  if(files.size()==0){
    cout<<"No files found in "<<dir<<endl;
    return 0;
  }

  TFile OUTFILE(dir+"/merged.root","recreate");
  
  //Histograms for the coherent rms. Take definitions from src/NoiseTupleCoherent.cxx
  TH1F * HistoCoh128[NDET][NSIDE][NFT][NFEB];
  TH1F * HistoRMSChannel[NDET][NSIDE][NFT][NFEB];
  TH1F * HistoRMSCoh[NDET][NSIDE][NFT][NFEB];
  for(int d=0;d<NDET;d++)
    for(int s=0;s<NSIDE;s++)
      for(int F=0;F<NFT;F++)
        for(int FEB=0;FEB<NFEB;FEB++){
	  TString histID=TString("")+"_Det"+(long)d+"_Side"+(long)s+"_FT"+(long)F+"_Slot"+(long)FEB;
          HistoCoh128[d][s][F][FEB]=new TH1F(TString("HistoCoh128")+histID+"_0","",100,-50,50);
          HistoRMSChannel[d][s][F][FEB]=new TH1F(TString("HistoRMSChannel")+histID,"",NCHAN,0,NCHAN);
          HistoRMSCoh[d][s][F][FEB]=new TH1F(TString("HistoRMSCoh")+histID,"",1+1+1+4,0,1);
        }


  for(unsigned int f=0;f<files.size();f++){
    TFile INFILE(dir+"/"+files[f].c_str(),"read");
    if(INFILE.IsZombie()) continue;
    cout<<"Processing "<<dir<<" : "<<files[f]<<endl;


    for(int d=0;d<NDET;d++)
      for(int s=0;s<NSIDE;s++)
	for(int F=0;F<NFT;F++)
	  for(int FEB=0;FEB<NFEB;FEB++){
	    TString histID=TString("")+"_Det"+(long)d+"_Side"+(long)s+"_FT"+(long)F+"_Slot"+(long)FEB;
	    
	    TH1F* HCoh128=(TH1F*)INFILE.Get(TString("HistoCoh128")+histID+"_0");
	    TH1F* HRMSChannel=(TH1F*)INFILE.Get(TString("HistoRMSChannel")+histID);
	    TH1F* HRMSCoh=(TH1F*)INFILE.Get(TString("HistoRMSCoh")+histID);
	    if(!HCoh128){cout<<"HistoCoh128 not found for: "<<files[f]<<endl; continue;}
	    if(!HRMSChannel){cout<<"HistoRMSChannel not found for: "<<files[f]<<endl; continue;}
	    if(!HRMSCoh){cout<<"HistoRMSCoh not found for: "<<files[f]<<endl; continue;}

	    HistoCoh128[d][s][F][FEB]->Add(HCoh128);
	    HistoRMSChannel[d][s][F][FEB]->Add(HRMSChannel);
	    HistoRMSCoh[d][s][F][FEB]->Add(HRMSCoh);
	  }

  }

  OUTFILE.cd();
  for(int d=0;d<NDET;d++)
    for(int s=0;s<NSIDE;s++)
      for(int F=0;F<NFT;F++)
        for(int FEB=0;FEB<NFEB;FEB++){
          HistoCoh128[d][s][F][FEB]->Write();
          HistoRMSChannel[d][s][F][FEB]->Write();
          HistoRMSCoh[d][s][F][FEB]->Write();
        }
  OUTFILE.Close();

  return 1;
}
