#include "metaeorite/core/Metric.hpp"

#include <cmath>
#include <stdexcept>

#include <xtensor/xbuilder.hpp>

namespace metaeorite::core {

Metric::Metric(CoordinateSystem coordinateSystem, NDTensor components)
    : coordinateSystem_(std::move(coordinateSystem)), components_(std::move(components)) {
    if (components_.dimension() != 2 || components_.shape()[0] != components_.shape()[1]) {
        throw std::invalid_argument("Metric: components must be a square rank-2 tensor");
    }
}

const CoordinateSystem& Metric::coordinateSystem() const noexcept {
    return coordinateSystem_;
}

const NDTensor& Metric::components() const noexcept {
    return components_;
}

unsigned Metric::dimension() const noexcept {
    return static_cast<unsigned>(components_.shape()[0]);
}

Metric Metric::minkowski(unsigned spatialDimension) {
    const unsigned dim = spatialDimension + 1;
    NDTensor components = xt::eye<Real>(dim);
    components(0, 0) = -1.0;

    CoordinateSystem coords;
    coords.chart = CoordinateChart::Cartesian;
    coords.dimension = dim;
    coords.label = "cartesian-minkowski";

    return Metric(std::move(coords), std::move(components));
}

bool Metric::isMinkowski(Real tolerance) const {
    const auto dim = dimension();
    for (unsigned i = 0; i < dim; ++i) {
        for (unsigned j = 0; j < dim; ++j) {
            const Real expected = (i == j) ? (i == 0 ? -1.0 : 1.0) : 0.0;
            if (std::abs(components_(i, j) - expected) > tolerance) {
                return false;
            }
        }
    }
    return true;
}

} // namespace metaeorite::core
