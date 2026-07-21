#include "metaeorite/metric_to_maxwell/PlaceholderEngine.hpp"

#include <stdexcept>

namespace metaeorite::metric_to_maxwell {

core::ConstitutiveRelations PlaceholderEngine::transform(const core::Metric& metric) const {
    if (!metric.isMinkowski()) {
        throw std::logic_error(
            "PlaceholderEngine: general Plebanski metric -> Maxwell transformation is not yet "
            "implemented; only the flat (Minkowski) baseline case is supported.");
    }
    return core::ConstitutiveRelations::vacuum();
}

} // namespace metaeorite::metric_to_maxwell
