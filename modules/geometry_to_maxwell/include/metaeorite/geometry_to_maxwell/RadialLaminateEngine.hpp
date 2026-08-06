#pragma once

#include "metaeorite/geometry_to_maxwell/IGeometryToMaxwellEngine.hpp"

namespace metaeorite::geometry_to_maxwell {

/// Homogenizes a core::RadialLaminateGeometry into effective constitutive
/// relations using the Wiener-bounds laminate mixing formulas (see
/// core::LaminateEffectiveMedium): the radial (perpendicular-to-layers)
/// component uses the harmonic mean, the tangential (parallel-to-layers)
/// component uses the arithmetic mean. The host material is assumed to be
/// vacuum (epsilon = 1); the inclusion material's permittivity is taken
/// from `materials.bulkPermittivity`.
///
/// This is the inverse of maxwell_to_geometry::RadialLaminateEngine, and
/// the two are validated together as a round trip against the
/// Pendry-Schurig-Smith cylindrical cloak in
/// tests/engines/TestTransformationOptics.cpp.
class RadialLaminateEngine final : public IGeometryToMaxwellEngine {
public:
    [[nodiscard]] core::ConstitutiveRelations transform(const core::IGeometry& geometry,
                                                         const core::MaterialProperties& materials) const override;

    [[nodiscard]] std::string name() const override { return "radial-laminate"; }
};

} // namespace metaeorite::geometry_to_maxwell
