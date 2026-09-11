#include "metaeorite/geometry_to_maxwell/PlanarLaminateEngine.hpp"

#include <stdexcept>

#include "metaeorite/core/LaminateEffectiveMedium.hpp"

namespace metaeorite::geometry_to_maxwell {

core::ConstitutiveRelations PlanarLaminateEngine::transform(const core::IGeometry& geometry,
                                                             const core::MaterialProperties& materials) const {
    if (geometry.dimension() == 0) {
        return core::ConstitutiveRelations::vacuum();
    }

    const auto* laminate = dynamic_cast<const core::PlanarLaminateGeometry*>(&geometry);
    if (laminate == nullptr) {
        throw std::invalid_argument(
            "geometry_to_maxwell::PlanarLaminateEngine: geometry '" + geometry.describe() +
            "' is not a core::PlanarLaminateGeometry; only that geometry type is supported by this engine.");
    }

    const double hostEpsilon = 1.0; // vacuum host, matching MaterialProperties::vacuum() as the trivial baseline
    const double inclusionEpsilon = materials.bulkPermittivity.real();
    const double f = laminate->fillingFraction();

    const double inPlane = core::LaminateEffectiveMedium::parallelMix(f, hostEpsilon, inclusionEpsilon);
    const double normal = core::LaminateEffectiveMedium::perpendicularMix(f, hostEpsilon, inclusionEpsilon);

    core::ConstitutiveRelations result;
    result.epsilon = core::Mat3c::Zero();
    result.epsilon(0, 0) = inPlane;
    result.epsilon(1, 1) = inPlane;
    result.epsilon(2, 2) = normal;
    result.mu = core::Mat3c::Identity(); // non-magnetic laminate (both constituents assumed mu = 1)
    result.xi = core::Mat3c::Zero();
    result.zeta = core::Mat3c::Zero();
    return result;
}

} // namespace metaeorite::geometry_to_maxwell
