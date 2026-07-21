#include "metaeorite/maxwell_to_geometry/Registration.hpp"

#include <memory>

#include "metaeorite/core/Registry.hpp"
#include "metaeorite/maxwell_to_geometry/IMaxwellToGeometryEngine.hpp"
#include "metaeorite/maxwell_to_geometry/PlaceholderEngine.hpp"

namespace metaeorite::maxwell_to_geometry {

void registerEngines() {
    core::EngineRegistry<IMaxwellToGeometryEngine>::instance().registerEngine(
        "vacuum-baseline", [] { return std::make_unique<PlaceholderEngine>(); });
}

} // namespace metaeorite::maxwell_to_geometry
