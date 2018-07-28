
void printNTUPcells(TString file){
  
  Interface * m_interface = Interface::open(file.Data());
  if(!m_interface){ cout<<"Interface not created"<<endl; return;}

  for( int i = 0; i < m_interface->nChannels(); i++ ){ //loop over the cells
    
    if( i%10000 == 0 ) cout << "Processing cell" << i << endl;
    if( m_interface->historySize(i) == 0 ) continue;

    const History* history = m_interface->cellHistory(i);
    if( !history )continue;
    
    int calo = abs(history->cellInfo()->calo());

    int caloPart = history->cellInfo()->calo(); 
    int det = 9;
    if( caloPart > 0 ) det = 1; //sideA
    if( caloPart < 0 ) det = 0; //sideC
    
    int layer = history->cellInfo()->layer();
    int ft = history->cellInfo()->feedThrough() ; 
    int slot = history->cellInfo()->slot() ; 
    int channel = history->cellInfo()->channel() ; 
    
    for( int j = 0; j < history->nData(); j++ ){
        int event = history->data(j)->event();
        double e = history->data(j)->energy(); //energy         
	int Nsamples = history->data(j)->nPoints(); //number of samples        
	double sample0 = history->data(j)->value(0); //ADC value for first sample

	
	cout<<" Det="<<calo
	    <<" Side="<<det
	    <<" Layer="<<layer
	    <<" FT="<<ft
	    <<" Slot="<<slot
	    <<" Channel="<<channel
	    <<" Event="<<event
	    <<" Energy="<<e
	    <<" Nsamples="<<Nsamples
	    <<" sample0="<<sample0
	    <<endl;
    }

  }

  gROOT->ProcessLine(".q");
}
