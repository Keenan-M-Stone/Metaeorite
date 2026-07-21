#pragma once

#include <string>

#include "metaeorite/core/ConstitutiveRelations.hpp"
#include "metaeorite/core/Metric.hpp"
#include "metaeorite/core/ReconstructionResult.hpp"

namespace metaeorite::maxwell_to_metric {

/// Attempts the inverse of the Metric -> Maxwell transformation: given
/// generalized constitutive relations, determine one or more compatible
/// spacetime metrics. Because this inverse problem is generally
/// non-unique, engines return a ReconstructionResult carrying every
/// candidate together with a confidence score and the assumptions made
/// while reconstructing it.
class IMaxwellToMetricEngine {
public:
    virtual ~IMaxwellToMetricEngine() = default;

    [[nodiscard]] virtual core::ReconstructionResult<core::Metric> transform(
        const core::ConstitutiveRelations& constitutive) const = 0;

    [[nodiscard]] virtual std::string name() const = 0;
};

} // namespace metaeorite::maxwell_to_metric
