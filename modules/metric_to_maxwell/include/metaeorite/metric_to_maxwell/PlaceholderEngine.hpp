#pragma once

#include "metaeorite/metric_to_maxwell/IMetricToMaxwellEngine.hpp"

namespace metaeorite::metric_to_maxwell {

/// Baseline reference implementation covering the one exactly-known case
/// (flat/Minkowski spacetime <-> vacuum): any other metric currently
/// throws, clearly marking the general Plebanski transformation as not yet
/// implemented rather than silently returning an incorrect result. Replace
/// or extend this class - without touching any caller - once the full
/// transformation is implemented.
class PlaceholderEngine final : public IMetricToMaxwellEngine {
public:
    [[nodiscard]] core::ConstitutiveRelations transform(const core::Metric& metric) const override;
    [[nodiscard]] std::string name() const override { return "vacuum-baseline"; }
};

} // namespace metaeorite::metric_to_maxwell
