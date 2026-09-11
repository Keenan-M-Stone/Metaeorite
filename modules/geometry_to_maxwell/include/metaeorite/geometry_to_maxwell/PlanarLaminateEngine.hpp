#pragma once

#include "metaeorite/geometry_to_maxwell/IGeometryToMaxwellEngine.hpp"

namespace metaeorite::geometry_to_maxwell {

/// Homogenizes a core::PlanarLaminateGeometry into effective constitutive
/// relations using the Wiener-bounds laminate mixing formulas (see
/// core::LaminateEffectiveMedium): the normal (perpendicular-to-layers,
/// index 2) component uses the harmonic mean, the in-plane
/// (parallel-to-layers, indices 0 and 1) component uses the arithmetic
/// mean. The host material is assumed to be vacuum (epsilon = 1); the
/// inclusion material's permittivity is taken from
/// `materials.bulkPermittivity`.
///
/// This is the inverse of maxwell_to_geometry::PlanarLaminateEngine, and
/// the sibling of geometry_to_maxwell::RadialLaminateEngine for
/// macroscopic configurations without cylindrical symmetry.
class PlanarLaminateEngine final : public IGeometryToMaxwellEngine {
public:
    [[nodiscard]] core::ConstitutiveRelations transform(const core::IGeometry& geometry,
                                                         const core::MaterialProperties& materials) const override;

    [[nodiscard]] std::string name() const override { return "planar-laminate"; }
};

} // namespace metaeorite::geometry_to_maxwell
