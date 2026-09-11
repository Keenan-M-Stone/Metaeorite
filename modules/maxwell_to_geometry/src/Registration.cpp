#include "metaeorite/maxwell_to_geometry/Registration.hpp"

#include <memory>

#include "metaeorite/core/Registry.hpp"
#include "metaeorite/maxwell_to_geometry/IMaxwellToGeometryEngine.hpp"
#include "metaeorite/maxwell_to_geometry/PlaceholderEngine.hpp"
#include "metaeorite/maxwell_to_geometry/PlanarLaminateEngine.hpp"
#include "metaeorite/maxwell_to_geometry/RadialLaminateEngine.hpp"

namespace metaeorite::maxwell_to_geometry {

void registerEngines() {
    core::EngineRegistry<IMaxwellToGeometryEngine>::instance().registerEngine(
        "vacuum-baseline", [] { return std::make_unique<PlaceholderEngine>(); });
    core::EngineRegistry<IMaxwellToGeometryEngine>::instance().registerEngine(
        "radial-laminate", [] { return std::make_unique<RadialLaminateEngine>(); });
    core::EngineRegistry<IMaxwellToGeometryEngine>::instance().registerEngine(
        "planar-laminate", [] { return std::make_unique<PlanarLaminateEngine>(); });
}

} // namespace metaeorite::maxwell_to_geometry
