#include "metaeorite/maxwell_to_geometry/PlanarLaminateEngine.hpp"

#include <algorithm>
#include <cmath>
#include <memory>

#include "metaeorite/core/LaminateEffectiveMedium.hpp"

namespace metaeorite::maxwell_to_geometry {

core::ReconstructionResult<std::shared_ptr<core::IGeometry>> PlanarLaminateEngine::transform(
    const core::ConstitutiveRelations& constitutive) const {
    core::ReconstructionResult<std::shared_ptr<core::IGeometry>> result;

    if (constitutive.isVacuumLike()) {
        result.addCandidate(std::make_shared<core::NullGeometry>(), 1.0,
                             {"vacuum constitutive relations require no structure (free space)"});
        return result;
    }

    // In-plane (parallel to layers) component, index 0, in the Cartesian
    // convention where the stack normal is index 2.
    const double targetInPlane = constitutive.epsilon(0, 0).real();
    const double f =
        core::LaminateEffectiveMedium::solveFillingFractionForParallel(targetInPlane, hostEpsilon_, inclusionEpsilon_);

    if (f < 0.0 || f > 1.0) {
        result.addNote("PlanarLaminateEngine: target in-plane permittivity " + std::to_string(targetInPlane) +
                        " is not realizable as a mix of host (epsilon=" + std::to_string(hostEpsilon_) +
                        ") and inclusion (epsilon=" + std::to_string(inclusionEpsilon_) +
                        ") materials (solved filling fraction " + std::to_string(f) + " is outside [0, 1]).");
        return result;
    }

    const double achievedNormal = core::LaminateEffectiveMedium::perpendicularMix(f, hostEpsilon_, inclusionEpsilon_);
    const double targetNormal = constitutive.epsilon(2, 2).real();
    const double relativeError =
        (targetNormal != 0.0) ? std::abs(achievedNormal - targetNormal) / std::abs(targetNormal) : std::abs(achievedNormal);
    const double confidence = std::clamp(1.0 - relativeError, 0.0, 1.0);

    result.addCandidate(
        std::make_shared<core::PlanarLaminateGeometry>(f, layerPeriod_), confidence,
        {"planar laminate of two isotropic dielectrics (host epsilon=" + std::to_string(hostEpsilon_) +
             ", inclusion epsilon=" + std::to_string(inclusionEpsilon_) + ")",
         "filling fraction solved to match the in-plane (epsilon_x = epsilon_y) component exactly",
         "achieved normal (epsilon_z) component is " + std::to_string(achievedNormal) + " vs. target " +
             std::to_string(targetNormal) + " - a single filling fraction cannot match both exactly"});
    return result;
}

} // namespace metaeorite::maxwell_to_geometry
