#include "metaeorite/maxwell_to_geometry/PlaceholderEngine.hpp"

#include <memory>

namespace metaeorite::maxwell_to_geometry {

core::ReconstructionResult<std::shared_ptr<core::IGeometry>> PlaceholderEngine::transform(
    const core::ConstitutiveRelations& constitutive) const {
    core::ReconstructionResult<std::shared_ptr<core::IGeometry>> result;

    if (constitutive.isVacuumLike()) {
        result.addCandidate(std::make_shared<core::NullGeometry>(), 1.0,
                             {"vacuum constitutive relations require no structure (free space)"});
    } else {
        result.addNote(
            "geometry synthesis from non-trivial constitutive relations is not yet implemented "
            "(requires effective medium theory, symmetry analysis, and a unit-cell library)");
    }

    return result;
}

} // namespace metaeorite::maxwell_to_geometry
