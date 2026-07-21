#pragma once

#include <string>

namespace metaeorite::core {

/// Identifies the coordinate chart a Metric (or geometry) is expressed in.
/// `Custom` allows user-supplied charts without needing to extend this enum.
enum class CoordinateChart {
    Cartesian,
    Cylindrical,
    Spherical,
    Custom
};

struct CoordinateSystem {
    CoordinateChart chart{CoordinateChart::Cartesian};
    unsigned dimension{4};
    std::string label{"cartesian"};

    [[nodiscard]] bool operator==(const CoordinateSystem&) const = default;
};

} // namespace metaeorite::core
