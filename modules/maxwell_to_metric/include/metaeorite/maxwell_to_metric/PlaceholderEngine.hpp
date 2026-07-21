#pragma once

#include "metaeorite/maxwell_to_metric/IMaxwellToMetricEngine.hpp"

namespace metaeorite::maxwell_to_metric {

/// Baseline reference implementation covering the one exactly-known case
/// (vacuum constitutive relations <-> flat/Minkowski spacetime). Any other
/// input yields an empty result annotated with a note explaining that the
/// general (non-unique) reconstruction is not yet implemented, rather than
/// guessing a candidate.
class PlaceholderEngine final : public IMaxwellToMetricEngine {
public:
    [[nodiscard]] core::ReconstructionResult<core::Metric> transform(
        const core::ConstitutiveRelations& constitutive) const override;
    [[nodiscard]] std::string name() const override { return "vacuum-baseline"; }
};

} // namespace metaeorite::maxwell_to_metric
