#include "metaeorite/maxwell_to_geometry/RadialLaminateEngine.hpp"

#include <algorithm>
#include <cmath>
#include <memory>

#include "metaeorite/core/LaminateEffectiveMedium.hpp"

namespace metaeorite::maxwell_to_geometry {

core::ReconstructionResult<std::shared_ptr<core::IGeometry>> RadialLaminateEngine::transform(
    const core::ConstitutiveRelations& constitutive) const {
    core::ReconstructionResult<std::shared_ptr<core::IGeometry>> result;

    if (constitutive.isVacuumLike()) {
        result.addCandidate(std::make_shared<core::NullGeometry>(), 1.0,
                             {"vacuum constitutive relations require no structure (free space)"});
        return result;
    }

    // Tangential (in-plane) component, index 1 in the (r, phi, z) convention
    // used by metric_to_maxwell::cylindricalCloakJacobian.
    const double targetTangential = constitutive.epsilon(1, 1).real();
    const double f =
        core::LaminateEffectiveMedium::solveFillingFractionForParallel(targetTangential, hostEpsilon_, inclusionEpsilon_);

    if (f < 0.0 || f > 1.0) {
        result.addNote("RadialLaminateEngine: target tangential permittivity " + std::to_string(targetTangential) +
                        " is not realizable as a mix of host (epsilon=" + std::to_string(hostEpsilon_) +
                        ") and inclusion (epsilon=" + std::to_string(inclusionEpsilon_) +
                        ") materials (solved filling fraction " + std::to_string(f) + " is outside [0, 1]).");
        return result;
    }

    const double achievedRadial = core::LaminateEffectiveMedium::perpendicularMix(f, hostEpsilon_, inclusionEpsilon_);
    const double targetRadial = constitutive.epsilon(0, 0).real();
    const double relativeError =
        (targetRadial != 0.0) ? std::abs(achievedRadial - targetRadial) / std::abs(targetRadial) : std::abs(achievedRadial);
    const double confidence = std::clamp(1.0 - relativeError, 0.0, 1.0);

    result.addCandidate(
        std::make_shared<core::RadialLaminateGeometry>(f, layerPeriod_), confidence,
        {"radial laminate of two isotropic dielectrics (host epsilon=" + std::to_string(hostEpsilon_) +
             ", inclusion epsilon=" + std::to_string(inclusionEpsilon_) + ")",
         "filling fraction solved to match the tangential (epsilon_phi) component exactly",
         "achieved radial (epsilon_r) component is " + std::to_string(achievedRadial) + " vs. target " +
             std::to_string(targetRadial) + " - a single filling fraction cannot match both exactly",
         "does not address the axial (epsilon_z) component"});
    return result;
}

} // namespace metaeorite::maxwell_to_geometry
