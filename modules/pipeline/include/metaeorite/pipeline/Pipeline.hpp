#pragma once

#include <memory>

#include "metaeorite/core/ConstitutiveRelations.hpp"
#include "metaeorite/core/MaterialProperties.hpp"
#include "metaeorite/core/Metric.hpp"
#include "metaeorite/core/ReconstructionResult.hpp"
#include "metaeorite/geometry_to_maxwell/IGeometryToMaxwellEngine.hpp"
#include "metaeorite/maxwell_to_geometry/IMaxwellToGeometryEngine.hpp"
#include "metaeorite/maxwell_to_metric/IMaxwellToMetricEngine.hpp"
#include "metaeorite/metric_to_maxwell/IMetricToMaxwellEngine.hpp"

namespace metaeorite::pipeline {

/// Orchestrates the four transformation engines strictly through their
/// abstract interfaces, mirroring the architecture diagram in the project
/// README: every transformation communicates exclusively through
/// core::ConstitutiveRelations. The Pipeline itself never depends on any
/// concrete engine implementation - engines are injected at construction
/// time (e.g. resolved from core::EngineRegistry) - so new algorithms can
/// replace a PlaceholderEngine without changing this class or any other
/// module.
class Pipeline {
public:
    Pipeline(
        std::shared_ptr<metric_to_maxwell::IMetricToMaxwellEngine> metricToMaxwell,
        std::shared_ptr<maxwell_to_metric::IMaxwellToMetricEngine> maxwellToMetric,
        std::shared_ptr<maxwell_to_geometry::IMaxwellToGeometryEngine> maxwellToGeometry,
        std::shared_ptr<geometry_to_maxwell::IGeometryToMaxwellEngine> geometryToMaxwell
    );

    /// Metric -> Maxwell
    [[nodiscard]] core::ConstitutiveRelations forward(const core::Metric& metric) const;

    /// Maxwell -> Metric
    [[nodiscard]] core::ReconstructionResult<core::Metric> inverse(
        const core::ConstitutiveRelations& constitutive) const;

    /// Maxwell -> Geometry
    [[nodiscard]] core::ReconstructionResult<std::shared_ptr<core::IGeometry>> synthesizeGeometry(
        const core::ConstitutiveRelations& constitutive) const;

    /// Geometry -> Maxwell
    [[nodiscard]] core::ConstitutiveRelations homogenize(const core::IGeometry& geometry,
                                                          const core::MaterialProperties& materials) const;

private:
    std::shared_ptr<metric_to_maxwell::IMetricToMaxwellEngine> metricToMaxwell_;
    std::shared_ptr<maxwell_to_metric::IMaxwellToMetricEngine> maxwellToMetric_;
    std::shared_ptr<maxwell_to_geometry::IMaxwellToGeometryEngine> maxwellToGeometry_;
    std::shared_ptr<geometry_to_maxwell::IGeometryToMaxwellEngine> geometryToMaxwell_;
};

} // namespace metaeorite::pipeline
