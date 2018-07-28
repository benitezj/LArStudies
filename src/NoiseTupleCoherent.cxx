#include "LArStudies/NoiseTupleCoherent.h"

#include <iostream>
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include "TFile.h"
#include "TH1F.h"

using namespace std;
using namespace LArSamples;

#define MAXDIFF 50

NoiseTupleCoherent::NoiseTupleCoherent(TString input) :
  m_debugLevel(0),
  m_input(input),
  m_maxCells(0)
{ 
}

NoiseTupleCoherent::~NoiseTupleCoherent(){
}

void NoiseTupleCoherent::setBranches(TTree* T){
  

  T->SetBranchAddress("slot",&slot_);
  T->SetBranchAddress("channel",&channel_);
  //T->SetBranchAddress("layer",&layer_);
  T->SetBranchAddress("run",&run_);
  T->SetBranchAddress("event",&event_);
  T->SetBranchAddress("lb",&lb_);
  T->SetBranchAddress("sample0",&sample0_);
  T->SetBranchAddress("pedestal",&pedestal_);

}



void NoiseTupleCoherent::run(TString outpath){

  //Load trees
  for(int d=0;d<NDET;d++)
    for(int s=0;s<NSIDE;s++)
      for(int F=0;F<NFT;F++){
	InputTree[d][s][F]=new TChain(TString("tuple_")+(long)d+"_"+(long)s+"_"+(long)F);
	InputTree[d][s][F]->Add(m_input.Data());//input is like "../*.root"
	setBranches(InputTree[d][s][F]);
      }


  ///Open the output file
  TFile OUTFILE(outpath+"/tuple.root","recreate");
  if(OUTFILE.IsZombie()){cout<<"Could not open output file. "<<endl; return;}
  OUTFILE.cd();


  //Histograms for the coherent rms
  TH1F * HistoCoh128[NDET][NSIDE][NFT][NFEB];
  for(int d=0;d<NDET;d++)
    for(int s=0;s<NSIDE;s++)
      for(int F=0;F<NFT;F++)
	for(int FEB=0;FEB<NFEB;FEB++){
	  HistoCoh128[d][s][F][FEB]=new TH1F(TString("HistoCoh128")+"_Det"+(long)d+"_Side"+(long)s+"_FT"+(long)F+"_Slot"+(long)FEB+"_0","",100,-50,50);
	}


  ///histograms tracking the RMS sums for each channel
  TH1F * HistoRMSChannel[NDET][NSIDE][NFT][NFEB];
  TH1F * HistoRMSCoh[NDET][NSIDE][NFT][NFEB];
  for(int d=0;d<NDET;d++)
    for(int s=0;s<NSIDE;s++)
      for(int F=0;F<NFT;F++)
	for(int FEB=0;FEB<NFEB;FEB++){
	  HistoRMSChannel[d][s][F][FEB]=new TH1F(TString("HistoRMSChannel")+"_Det"+(long)d+"_Side"+(long)s+"_FT"+(long)F+"_Slot"+(long)FEB,"",NCHAN,0,NCHAN);
	  HistoRMSCoh[d][s][F][FEB]=new TH1F(TString("HistoRMSCoh")+"_Det"+(long)d+"_Side"+(long)s+"_FT"+(long)F+"_Slot"+(long)FEB,"",1+1+1+4,0,1);
	}
 
  for(int d=0;d<NDET;d++)
    for(int s=0;s<NSIDE;s++)
      for(int F=0;F<NFT;F++){
	//if(!(d==0&&s==0&&F==1))continue;

	//Initialize the counters:
	float sample064[NFEB][2];
	int nSample064[NFEB][2];
	float sample0128[NFEB];
	int nSample0128[NFEB];
	for(int FEB=0;FEB<NFEB;FEB++){
	  for(int i=0;i<2;i++){
	    sample064[FEB][i]=0.;
	    nSample064[FEB][i]=0;
	  }
	  sample0128[FEB]=0.;
	  nSample0128[FEB]=0;
	}
	
	//Get the first cell entry
	InputTree[d][s][F]->GetEntry(0);
	int eventTracker=event_;
	if(channel_ < 0 || channel_ >= NCHAN || slot_ <=0 || slot_ > NFEB ){
	  cout<<"Channel="<<channel_<<" or slot="<<slot_<<" are out of range"<<endl;
	  return;
	}
	
	//start adding in the histograms  and sums
	if(fabs(sample0_-pedestal_) < MAXDIFF){//remove outliers
	  sample064[slot_-1][channel_/64] += (sample0_-pedestal_);
	  nSample064[slot_-1][channel_/64]++;
	  sample0128[slot_-1] += (sample0_-pedestal_);
	  nSample0128[slot_-1]++;

	  HistoRMSChannel[d][s][F][slot_-1]->AddBinContent(channel_+1,(sample0_-pedestal_)*(sample0_-pedestal_));
	}
	
	///Loop over the cells
	for(unsigned int i = 1; (i < InputTree[d][s][F]->GetEntries()) && (i < m_maxCells ||  m_maxCells==0); i++ ){ 
	  if( i%10000 == 0 ) cout << "Processing "<<d<<" "<<s<<" "<<F<<" cell " << i <<" / "<<InputTree[d][s][F]->GetEntries()<<endl;

	  InputTree[d][s][F]->GetEntry(i);
	  if(channel_ < 0 || channel_ >= NCHAN || slot_ <=0 || slot_ > NFEB ){
	    cout<<"Channel="<<channel_<<" or slot="<<slot_<<" are out of range"<<endl;
	    return;
	  }
	  
	  if( m_debugLevel > 1) cout<<" Run="<<run_
				    <<" lb="<<lb_
				    <<" Evt="<<event_
				    <<" Slot="<<slot_
				    <<" Chan= "<<channel_
				    <<" sample0="<<sample0_
				    <<" pedestal="<<pedestal_
				    <<endl;

	  /////////sum the channels
	  if(fabs(sample0_-pedestal_) < MAXDIFF){//remove outliers
	    sample064[slot_-1][channel_/64] += (sample0_-pedestal_);
	    nSample064[slot_-1][channel_/64]++;
	    sample0128[slot_-1] += (sample0_-pedestal_);
	    nSample0128[slot_-1]++;
	    
	    HistoRMSChannel[d][s][F][slot_-1]->AddBinContent(channel_+1,(sample0_-pedestal_)*(sample0_-pedestal_));
	  }

	  int tmpevent=event_;
	  int tmplb=lb_;
	  InputTree[d][s][F]->GetEntry(i+1);//check if the next cell has switched event
	  if( event_ != eventTracker ){
	    eventTracker=event_;//update the event tracker

	    //Here set the variables which you actually want to save in the output because they were overwritten in GetEntry(i+1) above
	    event_= tmpevent;
	    lb_= tmplb;
	    pedestal_=0.;

	    for(int slot_=1;slot_<=NFEB;slot_++){

	      if(nSample0128[slot_-1]>0){
		//to check the RMS_sum distribution
		HistoCoh128[d][s][F][slot_-1]->Fill(sample0128[slot_-1] / sqrt(nSample0128[slot_-1]));

		//each FEB counts number of events 
		HistoRMSCoh[d][s][F][slot_-1]->AddBinContent(1,1);

		//coherence in full FEB
		HistoRMSCoh[d][s][F][slot_-1]->AddBinContent(2,nSample0128[slot_-1]);//Number of channels used in the sum
		HistoRMSCoh[d][s][F][slot_-1]->AddBinContent(3,sample0128[slot_-1]*sample0128[slot_-1]);//Needed for RMS_sum (coherent rms)

		//coherence in FEB halfs
		HistoRMSCoh[d][s][F][slot_-1]->AddBinContent(4,nSample064[slot_-1][0]);
		HistoRMSCoh[d][s][F][slot_-1]->AddBinContent(5,sample064[slot_-1][0]*sample064[slot_-1][0]);
		HistoRMSCoh[d][s][F][slot_-1]->AddBinContent(6,nSample064[slot_-1][1]);
		HistoRMSCoh[d][s][F][slot_-1]->AddBinContent(7,sample064[slot_-1][1]*sample064[slot_-1][1]);

	      }
	      sample0128[slot_-1]=0.;
	      nSample0128[slot_-1]=0;

	    }

	  }

	}
	cout<< "Done processing tree: "<<d<<" "<<s<<" "<<F<<" : "<<InputTree[d][s][F]->GetEntries()<<endl;

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
	

  if( m_debugLevel > 2 ) OUTFILE.ls();
  OUTFILE.Close();
  cout<<"Closed output file."<<endl;

}

