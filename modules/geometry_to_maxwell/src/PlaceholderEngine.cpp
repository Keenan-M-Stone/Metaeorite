#include "metaeorite/geometry_to_maxwell/PlaceholderEngine.hpp"

#include <stdexcept>

namespace metaeorite::geometry_to_maxwell {

core::ConstitutiveRelations PlaceholderEngine::transform(const core::IGeometry& geometry,
                                                          const core::MaterialProperties& /*materials*/) const {
    if (geometry.dimension() == 0) {
        return core::ConstitutiveRelations::vacuum();
    }

    throw std::logic_error(
        "PlaceholderEngine: effective medium homogenization is not yet implemented for geometry '" +
        geometry.describe() + "'; only the free-space baseline is supported.");
}

} // namespace metaeorite::geometry_to_maxwell
