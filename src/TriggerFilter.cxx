#include "LArStudies/TriggerFilter.h"

LArSamples::TriggerFilter::TriggerFilter(const std::string& t,const std::string& n,const IInterface* p):
  AthAlgTool(t, n, p),
  m_trigDecisionTool("Trig::TrigDecisionTool/TrigDecisionTool")
{
  declareInterface<LArSamples::FilterInterface>(this);

  declareProperty("enabled",  m_enabled  = true );   
  declareProperty("Triggers", m_triggers = std::vector<std::string>()); 

}


LArSamples::TriggerFilter::~TriggerFilter(){

  for (unsigned int i = 0; i < m_triggers.size(); i++)
    delete m_counters.at(i);

}

StatusCode LArSamples::TriggerFilter::initialize()
{
  ATH_MSG_VERBOSE("INITIALIZING HSG1 SELECTOR TOOL");

  ////////////////////////////
  // trigger decision tool
   if(m_trigDecisionTool.retrieve().isFailure()) {
     ATH_MSG_FATAL("Failed to retrieve tool: " << m_trigDecisionTool);
     return StatusCode::FAILURE;
   }
  ATH_MSG_INFO("Retrieved tool: " << m_trigDecisionTool);

  //Print the trigger paths
  //init the counters
  for (unsigned int i = 0; i < m_triggers.size(); i++){
    ATH_MSG_INFO("TRIGGER = " << m_triggers.at(i));
    m_counters.push_back(new int(0));
  }
  

  ATH_MSG_INFO("initialized ... ");
  return StatusCode::SUCCESS;
}

StatusCode LArSamples::TriggerFilter::finalize()
{
  ATH_MSG_VERBOSE("finalize() ... ");

  //print the counters
  for (unsigned int i = 0; i < m_triggers.size(); i++)
    ATH_MSG_INFO("TRIGGER PATH COUNTER : " << m_triggers.at(i)<< " = "<<*(m_counters.at(i)));

  return StatusCode::SUCCESS;
}


bool LArSamples::TriggerFilter::eventPassesFilter() const
{

  bool passTrigger = false;
  
  if(m_triggers.size()==0) passTrigger = true;

  for (unsigned int i = 0; i < m_triggers.size(); i++) {
    ATH_MSG_DEBUG("TRIGGER = " << m_triggers.at(i));
    if(m_trigDecisionTool->isPassed(m_triggers.at(i))){
      passTrigger = true;
      (*(m_counters.at(i)))++;
    }
  }
  
  if(m_enabled) 
    return passTrigger;
  
  return true;
}

