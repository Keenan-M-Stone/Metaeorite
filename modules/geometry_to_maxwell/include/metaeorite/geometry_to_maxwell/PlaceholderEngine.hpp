#pragma once

#include "metaeorite/geometry_to_maxwell/IGeometryToMaxwellEngine.hpp"

namespace metaeorite::geometry_to_maxwell {

/// Baseline reference implementation covering the one exactly-known case
/// (free space / "no structure" homogenizes to vacuum, regardless of the
/// supplied bulk material - there is no material present). Any other
/// geometry currently throws, since effective medium homogenization is not
/// yet implemented.
class PlaceholderEngine final : public IGeometryToMaxwellEngine {
public:
    [[nodiscard]] core::ConstitutiveRelations transform(
        const core::IGeometry& geometry, const core::MaterialProperties& materials) const override;
    [[nodiscard]] std::string name() const override { return "vacuum-baseline"; }
};

} // namespace metaeorite::geometry_to_maxwell
