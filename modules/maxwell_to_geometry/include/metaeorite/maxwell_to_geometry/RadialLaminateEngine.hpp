#pragma once

#include "metaeorite/maxwell_to_geometry/IMaxwellToGeometryEngine.hpp"

namespace metaeorite::maxwell_to_geometry {

/// Synthesizes a radially-layered laminate (core::RadialLaminateGeometry)
/// whose tangential (in-plane) effective permittivity - the arithmetic
/// mean of two constituent dielectrics, weighted by filling fraction f -
/// matches the constitutive tensor's tangential diagonal component
/// (assumed to be tensor index 1, i.e. epsilon_phi in a cylindrical
/// (r, phi, z) frame). See core::LaminateEffectiveMedium.
///
/// A single filling fraction cannot generally also match the radial
/// (perpendicular) component exactly; the achieved radial permittivity is
/// computed and reported via the candidate's confidence score, so callers
/// can see this realistic fabrication trade-off rather than an idealized
/// exact match (see Cai et al., "Optical cloaking with metamaterials",
/// Nature Photonics 1, 224 (2007), for the same trade-off in a fabricated
/// design).
class RadialLaminateEngine final : public IMaxwellToGeometryEngine {
public:
    RadialLaminateEngine(double hostEpsilon = 1.0, double inclusionEpsilon = 10.0, double layerPeriod = 0.05)
        : hostEpsilon_(hostEpsilon), inclusionEpsilon_(inclusionEpsilon), layerPeriod_(layerPeriod) {}

    [[nodiscard]] core::ReconstructionResult<std::shared_ptr<core::IGeometry>> transform(
        const core::ConstitutiveRelations& constitutive) const override;

    [[nodiscard]] std::string name() const override { return "radial-laminate"; }

private:
    double hostEpsilon_;
    double inclusionEpsilon_;
    double layerPeriod_;
};

} // namespace metaeorite::maxwell_to_geometry
