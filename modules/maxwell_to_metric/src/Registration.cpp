#include "metaeorite/maxwell_to_metric/Registration.hpp"

#include <memory>

#include "metaeorite/core/Registry.hpp"
#include "metaeorite/maxwell_to_metric/IMaxwellToMetricEngine.hpp"
#include "metaeorite/maxwell_to_metric/PlaceholderEngine.hpp"

namespace metaeorite::maxwell_to_metric {

void registerEngines() {
    core::EngineRegistry<IMaxwellToMetricEngine>::instance().registerEngine(
        "vacuum-baseline", [] { return std::make_unique<PlaceholderEngine>(); });
}

} // namespace metaeorite::maxwell_to_metric
