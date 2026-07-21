#pragma once

#include "metaeorite/maxwell_to_geometry/IMaxwellToGeometryEngine.hpp"

namespace metaeorite::maxwell_to_geometry {

/// Baseline reference implementation covering the one exactly-known case
/// (vacuum constitutive relations require no structure, i.e. free space).
/// Any other input yields an empty result annotated with a note, since
/// geometry synthesis (effective medium theory + symmetry analysis + a
/// unit-cell library) is not yet implemented.
class PlaceholderEngine final : public IMaxwellToGeometryEngine {
public:
    [[nodiscard]] core::ReconstructionResult<std::shared_ptr<core::IGeometry>> transform(
        const core::ConstitutiveRelations& constitutive) const override;
    [[nodiscard]] std::string name() const override { return "vacuum-baseline"; }
};

} // namespace metaeorite::maxwell_to_geometry
