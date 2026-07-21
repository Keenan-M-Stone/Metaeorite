#include "metaeorite/maxwell_to_metric/PlaceholderEngine.hpp"

namespace metaeorite::maxwell_to_metric {

core::ReconstructionResult<core::Metric> PlaceholderEngine::transform(
    const core::ConstitutiveRelations& constitutive) const {
    core::ReconstructionResult<core::Metric> result;

    if (constitutive.isVacuumLike()) {
        result.addCandidate(
            core::Metric::minkowski(), 1.0,
            {"assumed flat spacetime; vacuum constitutive relations are consistent with, but do not "
             "uniquely imply, Minkowski spacetime"});
    } else {
        result.addNote(
            "general Maxwell -> Metric reconstruction is not yet implemented for non-vacuum "
            "constitutive relations; no candidates were produced");
    }

    return result;
}

} // namespace metaeorite::maxwell_to_metric
