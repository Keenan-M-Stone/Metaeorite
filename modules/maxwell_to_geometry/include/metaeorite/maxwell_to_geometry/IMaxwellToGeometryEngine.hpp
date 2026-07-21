#pragma once

#include <memory>
#include <string>

#include "metaeorite/core/ConstitutiveRelations.hpp"
#include "metaeorite/core/Geometry.hpp"
#include "metaeorite/core/ReconstructionResult.hpp"

namespace metaeorite::maxwell_to_geometry {

/// Maps generalized constitutive relations to realizable metamaterial
/// unit-cell geometries, combining effective medium theory, symmetry /
/// group-theory analysis, and parameterized unit-cell libraries. This is
/// generally non-unique, so engines return zero or more candidates via
/// ReconstructionResult.
class IMaxwellToGeometryEngine {
public:
    virtual ~IMaxwellToGeometryEngine() = default;

    [[nodiscard]] virtual core::ReconstructionResult<std::shared_ptr<core::IGeometry>> transform(
        const core::ConstitutiveRelations& constitutive) const = 0;

    [[nodiscard]] virtual std::string name() const = 0;
};

} // namespace metaeorite::maxwell_to_geometry
