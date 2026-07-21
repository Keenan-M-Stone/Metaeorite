#include "metaeorite/pipeline/Pipeline.hpp"

#include <stdexcept>
#include <string>
#include <utility>

namespace metaeorite::pipeline {

namespace {

template <typename EnginePtr>
const EnginePtr& requireEngine(const EnginePtr& engine, const char* role) {
    if (!engine) {
        throw std::invalid_argument(std::string("Pipeline: missing ") + role + " engine");
    }
    return engine;
}

} // namespace

Pipeline::Pipeline(
    std::shared_ptr<metric_to_maxwell::IMetricToMaxwellEngine> metricToMaxwell,
    std::shared_ptr<maxwell_to_metric::IMaxwellToMetricEngine> maxwellToMetric,
    std::shared_ptr<maxwell_to_geometry::IMaxwellToGeometryEngine> maxwellToGeometry,
    std::shared_ptr<geometry_to_maxwell::IGeometryToMaxwellEngine> geometryToMaxwell
    ):
    metricToMaxwell_(std::move(metricToMaxwell)),
    maxwellToMetric_(std::move(maxwellToMetric)),
    maxwellToGeometry_(std::move(maxwellToGeometry)),
    geometryToMaxwell_(std::move(geometryToMaxwell))
{}

core::ConstitutiveRelations Pipeline::forward(const core::Metric& metric) const {
    return requireEngine(metricToMaxwell_, "metric-to-maxwell")->transform(metric);
}

core::ReconstructionResult<core::Metric> Pipeline::inverse(const core::ConstitutiveRelations& constitutive) const {
    return requireEngine(maxwellToMetric_, "maxwell-to-metric")->transform(constitutive);
}

core::ReconstructionResult<std::shared_ptr<core::IGeometry>> Pipeline::synthesizeGeometry(
    const core::ConstitutiveRelations& constitutive) const {
    return requireEngine(maxwellToGeometry_, "maxwell-to-geometry")->transform(constitutive);
}

core::ConstitutiveRelations Pipeline::homogenize(const core::IGeometry& geometry,
                                                  const core::MaterialProperties& materials) const {
    return requireEngine(geometryToMaxwell_, "geometry-to-maxwell")->transform(geometry, materials);
}

} // namespace metaeorite::pipeline
