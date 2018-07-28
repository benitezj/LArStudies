#include "GaudiKernel/DeclareFactoryEntries.h"
#include "LArStudies/SkimmingKernel.h"
#include "LArStudies/TriggerFilter.h"

DECLARE_NAMESPACE_ALGORITHM_FACTORY( LArSamples , SkimmingKernel)

DECLARE_TOOL_FACTORY(LArSamples::TriggerFilter)

DECLARE_FACTORY_ENTRIES(LArStudies) {
  DECLARE_NAMESPACE_ALGORITHM( LArSamples , SkimmingKernel)
  DECLARE_TOOL(LArSamples::TriggerFilter)
}
