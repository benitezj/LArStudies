#ifndef LArStudies_TriggerFilter_H
#define LArStudies_TriggerFilter_H 1

#include <vector>
#include <string>

#include "LArStudies/FilterInterface.h"
#include "AthenaBaseComps/AthAlgTool.h"

#include "TrigDecisionTool/TrigDecisionTool.h"

namespace LArSamples {

  class TriggerFilter : public AthAlgTool, public FilterInterface {
  public:
    
    TriggerFilter(const std::string& t, const std::string& n, const IInterface* p);
    
    ~TriggerFilter();
    
    StatusCode  initialize();
    StatusCode  finalize();
    virtual bool eventPassesFilter() const;

  private:
    bool m_enabled;

    ToolHandle<Trig::TrigDecisionTool> m_trigDecisionTool;
    std::vector<std::string> m_triggers;
    std::vector<int*> m_counters;
    
  };
  
} // end of namespace

#endif 
