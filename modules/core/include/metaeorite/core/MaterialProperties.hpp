#pragma once

#include <string>

#include "metaeorite/core/Tensor.hpp"

namespace metaeorite::core {

/// Bulk material properties supplied alongside a geometry to the
/// Geometry -> Maxwell (homogenization) engine.
struct MaterialProperties {
    std::string label{"vacuum"};
    Complex bulkPermittivity{1.0, 0.0};
    Complex bulkPermeability{1.0, 0.0};

    [[nodiscard]] static MaterialProperties vacuum() { return MaterialProperties{}; }
};

} // namespace metaeorite::core
