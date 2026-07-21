#pragma once

#include "metaeorite/core/CoordinateSystem.hpp"
#include "metaeorite/core/Tensor.hpp"

namespace metaeorite::core {

/// Represents a spacetime (or spatial) metric tensor together with the
/// coordinate chart it is expressed in. Storage uses NDTensor so the class
/// is dimension-agnostic: the current scope targets 4D spacetime -> 2D
/// metasurfaces, but the same type extends to future volumetric work
/// without an API change.
class Metric {
public:
    Metric(CoordinateSystem coordinateSystem, NDTensor components);

    [[nodiscard]] const CoordinateSystem& coordinateSystem() const noexcept;
    [[nodiscard]] const NDTensor& components() const noexcept;
    [[nodiscard]] unsigned dimension() const noexcept;

    /// Flat Minkowski metric diag(-1, 1, 1, ..., 1) with mostly-plus
    /// signature, expressed in Cartesian coordinates. Serves as the
    /// trivial baseline case exercised by the placeholder engines and unit
    /// tests until the full Plebanski transformation is implemented.
    [[nodiscard]] static Metric minkowski(unsigned spatialDimension = 3);

    /// True if `components` is diagonal with entries (-1, 1, 1, ...).
    [[nodiscard]] bool isMinkowski(Real tolerance = 1e-9) const;

private:
    CoordinateSystem coordinateSystem_;
    NDTensor components_;
};

} // namespace metaeorite::core
