#include "metaeorite/api/Functions.hpp"

#include <mutex>

#include "metaeorite/core/Registry.hpp"
#include "metaeorite/geometry_to_maxwell/IGeometryToMaxwellEngine.hpp"
#include "metaeorite/geometry_to_maxwell/Registration.hpp"
#include "metaeorite/maxwell_to_geometry/IMaxwellToGeometryEngine.hpp"
#include "metaeorite/maxwell_to_geometry/Registration.hpp"
#include "metaeorite/maxwell_to_metric/IMaxwellToMetricEngine.hpp"
#include "metaeorite/maxwell_to_metric/Registration.hpp"
#include "metaeorite/metric_to_maxwell/IMetricToMaxwellEngine.hpp"
#include "metaeorite/metric_to_maxwell/Registration.hpp"

// NOTE: every reference to a sibling engine-module namespace below is fully
// qualified with the leading "metaeorite::" prefix. This is required, not
// stylistic: this file lives inside `namespace metaeorite::api`, which also
// declares free functions named `metric_to_maxwell`, `maxwell_to_metric`,
// etc. (see Functions.hpp). An unqualified reference such as
// `metric_to_maxwell::IMetricToMaxwellEngine` would resolve to the sibling
// *function* of that name first and fail to compile ("not a namespace
// name"), so the engine-module namespaces must always be spelled out in
// full here.

namespace metaeorite::api {

namespace {

void ensureEnginesRegistered() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        metaeorite::metric_to_maxwell::registerEngines();
        metaeorite::maxwell_to_metric::registerEngines();
        metaeorite::maxwell_to_geometry::registerEngines();
        metaeorite::geometry_to_maxwell::registerEngines();
    });
}

} // namespace

core::ConstitutiveRelations metric_to_maxwell(const core::Metric& metric, const std::string& engineId) {
    ensureEnginesRegistered();
    const auto engine =
        core::EngineRegistry<metaeorite::metric_to_maxwell::IMetricToMaxwellEngine>::instance().create(engineId);
    return engine->transform(metric);
}

core::ReconstructionResult<core::Metric> maxwell_to_metric(
    const core::ConstitutiveRelations& constitutive, const std::string& engineId) {
    ensureEnginesRegistered();
    const auto engine =
        core::EngineRegistry<metaeorite::maxwell_to_metric::IMaxwellToMetricEngine>::instance().create(engineId);
    return engine->transform(constitutive);
}

core::ReconstructionResult<std::shared_ptr<core::IGeometry>> maxwell_to_geometry(
    const core::ConstitutiveRelations& constitutive, const std::string& engineId) {
    ensureEnginesRegistered();
    const auto engine =
        core::EngineRegistry<metaeorite::maxwell_to_geometry::IMaxwellToGeometryEngine>::instance().create(engineId);
    return engine->transform(constitutive);
}

core::ConstitutiveRelations geometry_to_maxwell(
    const core::IGeometry& geometry, const core::MaterialProperties& materials, const std::string& engineId) {
    ensureEnginesRegistered();
    const auto engine =
        core::EngineRegistry<metaeorite::geometry_to_maxwell::IGeometryToMaxwellEngine>::instance().create(engineId);
    return engine->transform(geometry, materials);
}

std::vector<std::string> availableMetricToMaxwellEngines() {
    ensureEnginesRegistered();
    return core::EngineRegistry<metaeorite::metric_to_maxwell::IMetricToMaxwellEngine>::instance().availableEngines();
}

std::vector<std::string> availableMaxwellToMetricEngines() {
    ensureEnginesRegistered();
    return core::EngineRegistry<metaeorite::maxwell_to_metric::IMaxwellToMetricEngine>::instance().availableEngines();
}

std::vector<std::string> availableMaxwellToGeometryEngines() {
    ensureEnginesRegistered();
    return core::EngineRegistry<metaeorite::maxwell_to_geometry::IMaxwellToGeometryEngine>::instance()
        .availableEngines();
}

std::vector<std::string> availableGeometryToMaxwellEngines() {
    ensureEnginesRegistered();
    return core::EngineRegistry<metaeorite::geometry_to_maxwell::IGeometryToMaxwellEngine>::instance()
        .availableEngines();
}

} // namespace metaeorite::api
