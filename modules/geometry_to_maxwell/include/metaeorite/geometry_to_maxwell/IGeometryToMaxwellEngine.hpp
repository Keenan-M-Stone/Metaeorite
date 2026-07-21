#pragma once

#include <string>

#include "metaeorite/core/ConstitutiveRelations.hpp"
#include "metaeorite/core/Geometry.hpp"
#include "metaeorite/core/MaterialProperties.hpp"

namespace metaeorite::geometry_to_maxwell {

/// Estimates the effective constitutive relations of an existing
/// metamaterial geometry (homogenization) - the reverse of Maxwell ->
/// Geometry synthesis.
class IGeometryToMaxwellEngine {
public:
    virtual ~IGeometryToMaxwellEngine() = default;

    [[nodiscard]] virtual core::ConstitutiveRelations transform(
        const core::IGeometry& geometry, const core::MaterialProperties& materials) const = 0;

    [[nodiscard]] virtual std::string name() const = 0;
};

} // namespace metaeorite::geometry_to_maxwell
