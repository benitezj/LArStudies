
#include "LArStudies/SkimmingKernel.h"

#include <sstream>                                      // C++ utilities
#include <string>
#include <algorithm>
#include <fstream>

#include "GaudiKernel/ISvcLocator.h"
#include "DataModel/DataVector.h"
#include "DataModel/ElementLink.h"
#include "DataModel/DataVector.h"
#include "GaudiKernel/Chrono.h"

#include "StoreGate/StoreGateSvc.h"             // Storegate stuff
#include "StoreGate/DataHandle.h"
#include "AthenaKernel/DefaultKey.h"
#include "SGTools/StlVectorClids.h"

///////////////////////////////////////////////////////////////////////////////

LArSamples::SkimmingKernel::SkimmingKernel(const std::string& name, ISvcLocator* pSvcLocator) :
AthFilterAlgorithm(name, pSvcLocator),
m_chronoSvc("ChronoStatSvc", name)
{
    // ------- Python changeable properties -------
    declareProperty("SkimmingTools",         m_skimmingTools);
    //---------------------------------------------
    
    // Counters
    m_eventCounter = 0;
    m_acceptCntr = 0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

StatusCode LArSamples::SkimmingKernel::initialize() {
    
    ATH_MSG_INFO("Initializing the LArSamples::SkimmingKernel kernel " << name());
    
    // get the tool service
    IToolSvc* toolSvc;
    StatusCode sc = service("ToolSvc",toolSvc);
    if (StatusCode::SUCCESS != sc) {
        ATH_MSG_FATAL("Unable to retrieve ToolSvc");
        return StatusCode::FAILURE;
    }
    
    // get the skimming tools
    if( m_skimmingTools.retrieve().isFailure() ) {
        ATH_MSG_FATAL("Failed to retrieve skimming tools");
        return StatusCode::FAILURE;
    } else {
        ATH_MSG_INFO("The following skimming tools will be applied....");
        ATH_MSG_INFO(m_skimmingTools);
    }

 
    // get the chrono auditor
    if ( m_chronoSvc.retrieve().isFailure() ) {
	    ATH_MSG_FATAL("Failed to retrieve service " << m_chronoSvc);
	    return StatusCode::FAILURE;
    } 

    return StatusCode::SUCCESS;
    
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

StatusCode LArSamples::SkimmingKernel::execute() {
    
    // On your marks.... get set....
    Chrono chrono( &(*m_chronoSvc), name() ); 
    // GO!!!

    ATH_MSG_DEBUG(name() << " is processing next event...");
    
    // Increment the event counter
    m_eventCounter++;
  

    // Set master flag to true
    bool acceptEvent(true);

    // Loop over the filters
    ToolHandleArray<FilterInterface>::iterator skimmingTool(m_skimmingTools.begin());
    ToolHandleArray<FilterInterface>::iterator endOfSkimmingTools(m_skimmingTools.end());
    while (skimmingTool != endOfSkimmingTools) {
        if (!((**skimmingTool).eventPassesFilter())) {
            acceptEvent=false;
            ATH_MSG_DEBUG("This event failed the " << (**skimmingTool).name() << " filter. Therefore it will not be recorded.");
            break;
        }
        ++skimmingTool;
    }

    // Increment local counters if event to be accepted
    if (acceptEvent) ++m_acceptCntr;
    
    // Set the setFilterPassed flag 
    setFilterPassed(acceptEvent);

    return StatusCode::SUCCESS;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

StatusCode LArSamples::SkimmingKernel::finalize() {
    
    ATH_MSG_INFO( "============================================================================");
    ATH_MSG_INFO( "|| SUMMARY OF THE Skimming  KERNEL WITH NAME " << name() << " || ");
    ATH_MSG_INFO( "============================================================================");
    ATH_MSG_INFO( "============================================================================");
    ATH_MSG_INFO( "Events analyzed: " << m_eventCounter);
    ATH_MSG_INFO( "Events accepted: " << m_acceptCntr);
    ATH_MSG_INFO( "============================================================================");
    
    return StatusCode::SUCCESS;
    
}
