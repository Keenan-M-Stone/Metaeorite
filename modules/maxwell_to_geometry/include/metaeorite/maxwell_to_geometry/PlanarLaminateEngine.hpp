#pragma once

#include "metaeorite/maxwell_to_geometry/IMaxwellToGeometryEngine.hpp"

namespace metaeorite::maxwell_to_geometry {

/// Synthesizes a planar (Cartesian) laminate (core::PlanarLaminateGeometry)
/// whose in-plane effective permittivity - the arithmetic mean of two
/// constituent dielectrics, weighted by filling fraction f - matches the
/// constitutive tensor's in-plane diagonal component (assumed to be tensor
/// indices 0 and 1, with the stack normal along index 2). See
/// core::LaminateEffectiveMedium.
///
/// Sibling of RadialLaminateEngine, sharing the same effective-medium math
/// but with a Cartesian (flat-stack) rather than cylindrical-shell index
/// convention - demonstrating that other macroscopic configurations plug
/// into this module via new engines/geometries rather than by generalizing
/// RadialLaminateEngine itself.
///
/// A single filling fraction cannot generally also match the normal
/// (perpendicular) component exactly; the achieved normal permittivity is
/// computed and reported via the candidate's confidence score.
class PlanarLaminateEngine final : public IMaxwellToGeometryEngine {
public:
    PlanarLaminateEngine(double hostEpsilon = 1.0, double inclusionEpsilon = 10.0, double layerPeriod = 0.05)
        : hostEpsilon_(hostEpsilon), inclusionEpsilon_(inclusionEpsilon), layerPeriod_(layerPeriod) {}

    [[nodiscard]] core::ReconstructionResult<std::shared_ptr<core::IGeometry>> transform(
        const core::ConstitutiveRelations& constitutive) const override;

    [[nodiscard]] std::string name() const override { return "planar-laminate"; }

private:
    double hostEpsilon_;
    double inclusionEpsilon_;
    double layerPeriod_;
};

} // namespace metaeorite::maxwell_to_geometry
