
void printNTUPevents(TString file){
  
  Interface * m_interface = Interface::open(file.Data());
  if(!m_interface){ cout<<"Interface not created"<<endl; return;}

  std::vector<std::string> triggerPaths;
  triggerPaths.push_back("L1_RD1_EMTPY");

  std::vector<int> triggerCounters;  
  for(unsigned int i = 0;i<triggerPaths.size();i++)
    triggerCounters.push_back(*(new int(0)));

  for(unsigned int ievent = 0; ievent <  m_interface->nEvents(); ievent++) {
    const LArSamples::EventData* evtData =  m_interface->eventData(ievent);
    
    cout<<" run = "<<evtData->run()<<"  event = "<<evtData->event()<<endl;

    ///This code seems not to be working. Works in a compiled code.
    for(unsigned int i = 0;i<triggerPaths.size();i++){
      if( evtData->isPassed((triggerPaths.at(i)).c_str()) ) (triggerCounters.at(i))++;
    }

  }  

  cout<<"Trigger Path counters "<<endl;
  for(unsigned int i = 0;i<triggerPaths.size();i++)
    cout<<triggerPaths.at(i)<<"  :  "<<triggerCounters.at(i)<<endl;
  

  gROOT->ProcessLine(".q");
}
