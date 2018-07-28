#include "LArStudies/NoiseTuple.h"

#include "LArCafJobs/HistoryContainer.h"
#include "LArCafJobs/CellInfo.h"
#include "LArCafJobs/DataContainer.h"
#include "LArCafJobs/EventData.h"
#include "LArCafJobs/RunData.h"

#include <iostream>
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include "TFile.h"

using namespace std;
using namespace LArSamples;

#define NDET 5
#define NSIDE 2
#define NFT 32

NoiseTuple::NoiseTuple(std::vector<std::string> input) :
  m_debugLevel(0),
  m_input(input),
  m_maxCells(0)
{ 
}

NoiseTuple::~NoiseTuple(){
}


TTree * NoiseTuple::makeTree(TString name){

  TTree* tree =new TTree(name.Data(),name.Data());
  
  //tree->Branch("det",&det_,"det/I");
  //tree->Branch("side",&side_,"side/I");
  //tree->Branch("FT",&FT_,"FT/I");
  tree->Branch("slot",&slot_,"slot/I");
  tree->Branch("channel",&channel_,"channel/I");
  tree->Branch("layer",&layer_,"layer/I");

  tree->Branch("run",&run_,"run/I");
  tree->Branch("event",&event_,"event/I");
  tree->Branch("lb",&lb_,"lb/I");
  //tree->Branch("nSamples",&nSamples_,"nSamples/I");
  tree->Branch("sample0",&sample0_,"sample0/I");
  tree->Branch("pedestal",&pedestal_,"pedestal/F");
  tree->Branch("pedestalRMS",&pedestalRMS_,"pedestalRMS/F");
  tree->Branch("energy",&energy_,"energy/F");
  tree->Branch("time",&time_,"time/F");
  tree->Branch("eta",&eta_,"eta/F");
  tree->Branch("phi",&phi_,"phi/F");

  return tree;
}


bool NoiseTuple::fillTrigger(){
  if(m_triggerPaths.size()==0) return 1;
  
  cout<<"using the following triggers:"<<endl;
  for(unsigned int p=0; p< m_triggerPaths.size(); p++)
    cout<<m_triggerPaths.at(p)<<endl;

  for(unsigned int ievent = 0; ievent <  EVENTTREE->GetEntries(); ievent++) {
    EVENTTREE->GetEntry(ievent);
    RUNTREE->GetEntry(EVENT->runIndex());
    
    for(unsigned int p=0; p< m_triggerPaths.size(); p++){

      unsigned int wpos=RUN->triggerBitPosition(m_triggerPaths.at(p).c_str())/32;
      unsigned int bpos=RUN->triggerBitPosition(m_triggerPaths.at(p).c_str())%32;
      int word=EVENT->triggerData().at(wpos);
      
      if( m_debugLevel > 1){
	cout<<" TrigPath="<<m_triggerPaths.at(p)
	    <<" run="<<RUN->run()
	    <<" event="<<EVENT->event()
	    <<" trigWords="<<EVENT->triggerData().size()
	    <<" wPos="<<wpos
	    <<" bPos="<<bpos
	    <<" word="<<word
	    <<" pass="<< (word >> bpos & 0x1) 
	    <<endl;
      }
      

      if(word >> bpos & 0x1){
	m_triggerEvents.push_back(std::pair<int,int>(RUN->run(),EVENT->event()));
	break;//dont add same event twice
      }
    }
  }  
  
  return 1;
}


bool NoiseTuple::checkTrigger(int run, int event){
  
  if(m_triggerPaths.size()==0) return 1;

  if( m_debugLevel > 1) cout<<"checkTrigger for :"<<run<<" "<<event<<endl;

  for(unsigned int ievent = 0; ievent <  m_triggerEvents.size(); ievent++) {
    if((m_triggerEvents.at(ievent)).first == run 
       && (m_triggerEvents.at(ievent)).second == event){
      return 1;
    }
  }
  
  return 0;
}

void NoiseTuple::run(TString outpath){

  for(unsigned int i = 0 ; i< m_input.size(); i++)
    cout<<m_input[i].c_str()<<endl;


  TFile INPUTFILE(m_input[0].c_str(),"READ");

  CELLTREE = (TTree*) INPUTFILE.Get("cells");
  if(!CELLTREE){cout<<"Cells Tree not found"<<endl; return ;}
  CELLTREE->SetBranchAddress("history",&CELL);
  cout<<"Cell Tree entries "<<CELLTREE->GetEntries()<<endl;

  EVENTTREE = (TTree*) INPUTFILE.Get("events");
  if(!EVENTTREE){cout<<"events Tree not found"<<endl; return ;}
  EVENTTREE->SetBranchAddress("event",&EVENT);
  cout<<"Event Tree entries "<<EVENTTREE->GetEntries()<<endl;

  RUNTREE = (TTree*) INPUTFILE.Get("runs");
  if(!RUNTREE){cout<<"runs Tree not found"<<endl; return ;}
  RUNTREE->SetBranchAddress("run",&RUN);
  cout<<"Run Tree entries "<<RUNTREE->GetEntries()<<endl;


  fillTrigger();

  ///Open the output file
  TFile OUTFILE(outpath+"/tuple.root","recreate");
  if(OUTFILE.IsZombie()){cout<<"Could not open output file. "<<endl; return;}

  ///Create the ntuples
  TTree * Tree[NDET][NSIDE][NFT];
  for(int d=0;d<NDET;d++)
    for(int s=0;s<NSIDE;s++)
      for(int F=0;F<NFT;F++){
	Tree[d][s][F]=makeTree(TString("tuple_")+(long)d+"_"+(long)s+"_"+(long)F);
	if(!Tree[d][s][F]){cout<<"makeTree Failed"<<endl;}
	Tree[d][s][F]->SetDirectory(&OUTFILE);
      }


  for(unsigned int i = 0; (i < CELLTREE->GetEntries()) && (i < m_maxCells ||  m_maxCells==0); i++ ){ //loop over the cells
    if( i%10000 == 0 ) cout << "Processing cell  " << i <<" out of "<<CELLTREE->GetEntries()<< endl;
 
    int bytes = CELLTREE->GetEntry(i);
    if(m_debugLevel>0)cout<<"Read "<<bytes<<" bytes from cells Tree"<<endl;
    if( !CELL ){ cout<<" NO CELL read from CELLTREE "<<endl; continue;}

    const  LArSamples::CellInfo* cellInfo=CELL->cellInfo();
    if( !cellInfo ){ cout<<" NO cellInfo read from  HistoryContainer"<<endl; continue;}

    det_ = abs(cellInfo->calo());//    EMB_A = 1, EMEC_OUTER_A = 2, EMEC_INNER_A = 3, HEC_A = 4, FCAL_A = 5,
    side_ = -1;
    if( cellInfo->calo() > 0 ) side_ = 1; //sideA
    if( cellInfo->calo() < 0 ) side_ = 0; //sideC
    FT_ = cellInfo->feedThrough() ; 
    slot_ = cellInfo->slot() ; 
    channel_ = cellInfo->channel() ; 
    layer_ = cellInfo->layer() ;
    eta_ = cellInfo->eta();
    phi_ = cellInfo->phi();
    
    if( m_debugLevel > 0) cout<<" Det="<<det_
			      <<" Side="<<side_
			      <<" FT="<<FT_
			      <<" Slot="<<slot_
			      <<" Chan= "<<channel_
			      <<" nDat="<<CELL->nDataContainers()			      
			      <<endl;
    


    for(unsigned int j = 0; j < CELL->nDataContainers() ; j++ ){
      const LArSamples::DataContainer * data = CELL->dataContainer(j);
      if( !data ){cout<<"No data read from CELL"<<endl; continue;}

      int eventbytes = EVENTTREE->GetEntry(data->eventIndex());
      if(m_debugLevel>0)cout<<"         Read "<<eventbytes<<" bytes from events Tree"<<endl;
      if( !EVENT ){cout<<"No EVENT read from EVENTTREE"<<endl; continue;}
      
      int runbytes = RUNTREE->GetEntry(EVENT->runIndex());
      if(m_debugLevel>0)cout<<"         Read "<<runbytes<<" bytes from runs Tree"<<endl;
      if( !RUN ){cout<<"No RUN read from RUNTREE"<<endl;  continue;}

      if(!checkTrigger(RUN->run(),EVENT->event())) continue;
      
      run_ = RUN->run();
      event_ = EVENT->event();     
      lb_ = EVENT->lumiBlock();
      //nSamples_ = data->nSamples();// number of samples        
      sample0_ = data->sample(0);// ADC value for first sample
      pedestal_ = data->pedestal();//
      pedestalRMS_ = data->pedestalRMS();//
      energy_ = data->energy();// energy
      time_ = data->ofcTime();// time   
      
      
      if( m_debugLevel > 0) cout<<"      Run="<<run_
				<<" Evt="<<event_
				<<" lb="<<lb_
				<<" nSamples="<<nSamples_
				<<" sample0="<<sample0_
				<<" E="<<energy_
				<<" T="<<time_<<endl;

      
      Tree[det_-1][side_][FT_]->Fill();
      //delete EVENT; //appears to crash if delete  
      //EVENT=NULL; 
      //delete RUN; 
      //RUN=NULL;
    }

    //delete CELL; ///need to delete otherwise fills memory 
    //CELL=NULL; 
  }
  
  cout<<"Closing INPUTFILE"<<endl;
  INPUTFILE.Close();


  ////Write tuples 
  for(int d=0;d<NDET;d++)
    for(int s=0;s<NSIDE;s++)
      for(int F=0;F<NFT;F++){
	if( m_debugLevel > 1) cout<<Tree[d][s][F]->GetName()<<" "<<Tree[d][s][F]->GetEntries()<<endl;
	Tree[d][s][F]->Write();
      }
  if( m_debugLevel > 1 ) OUTFILE.ls();
  OUTFILE.Close();


}

