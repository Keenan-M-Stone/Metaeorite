#pragma once

#include <string>

#include "metaeorite/core/ConstitutiveRelations.hpp"
#include "metaeorite/core/Metric.hpp"

namespace metaeorite::metric_to_maxwell {

/// Converts a spacetime metric into the generalized constitutive relations
/// of an equivalent electromagnetic medium, via the Plebanski formulation
/// of transformation optics.
///
/// Every concrete implementation of this transformation - however the
/// underlying math is derived - is reachable through this single
/// polymorphic interface, so callers (the Pipeline, CLI, tests) never
/// depend on a specific engine.
class IMetricToMaxwellEngine {
public:
    virtual ~IMetricToMaxwellEngine() = default;

    [[nodiscard]] virtual core::ConstitutiveRelations transform(const core::Metric& metric) const = 0;

    /// Stable identifier used for registration/lookup and logging.
    [[nodiscard]] virtual std::string name() const = 0;
};

} // namespace metaeorite::metric_to_maxwell
