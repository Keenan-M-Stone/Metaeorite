#include "metaeorite/geometry_to_maxwell/Registration.hpp"

#include <memory>

#include "metaeorite/core/Registry.hpp"
#include "metaeorite/geometry_to_maxwell/IGeometryToMaxwellEngine.hpp"
#include "metaeorite/geometry_to_maxwell/PlaceholderEngine.hpp"

namespace metaeorite::geometry_to_maxwell {

void registerEngines() {
    core::EngineRegistry<IGeometryToMaxwellEngine>::instance().registerEngine(
        "vacuum-baseline", [] { return std::make_unique<PlaceholderEngine>(); });
}

} // namespace metaeorite::geometry_to_maxwell
