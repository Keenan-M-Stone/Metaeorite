#include "metaeorite/metric_to_maxwell/Registration.hpp"

#include <memory>

#include "metaeorite/core/Registry.hpp"
#include "metaeorite/metric_to_maxwell/IMetricToMaxwellEngine.hpp"
#include "metaeorite/metric_to_maxwell/PlaceholderEngine.hpp"

namespace metaeorite::metric_to_maxwell {

void registerEngines() {
    core::EngineRegistry<IMetricToMaxwellEngine>::instance().registerEngine(
        "vacuum-baseline", [] { return std::make_unique<PlaceholderEngine>(); });
}

} // namespace metaeorite::metric_to_maxwell
