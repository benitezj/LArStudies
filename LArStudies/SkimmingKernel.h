#ifndef LArStudies_SkimmingKernel_H
#define LArStudies_SkimmingKernel_H 1

#include <string>
#include <vector>
#include <list>

#include "AthenaBaseComps/AthFilterAlgorithm.h"
#include "GaudiKernel/ToolHandle.h"
#include "GaudiKernel/ServiceHandle.h"
#include "GaudiKernel/IChronoStatSvc.h"

#include "LArStudies/FilterInterface.h"

namespace LArSamples {
    
    /////////////////////////////////////////////////////////////////////////////
    class SkimmingKernel : public AthFilterAlgorithm {
        
    public:
        SkimmingKernel (const std::string& name, ISvcLocator* pSvcLocator);
        StatusCode initialize();
        StatusCode execute();
        StatusCode finalize();
    private:
        // Tools etc
        ToolHandleArray<FilterInterface>     m_skimmingTools;
        ServiceHandle<IChronoStatSvc>      m_chronoSvc;

        // Some counters
        int m_eventCounter;
        int m_acceptCntr;        
        
    };
    
} // end of namespace
#endif 

