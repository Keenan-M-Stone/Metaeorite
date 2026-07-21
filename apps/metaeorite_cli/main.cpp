// Demonstrates the full Metaeorite architecture wired end-to-end using only
// the trivial vacuum / flat-spacetime baseline provided by each module's
// PlaceholderEngine. This proves that the four transformation engines can
// be composed purely through abstract interfaces (see modules/*/include),
// exactly as described in the project README, before any real physics is
// implemented.

#include <iomanip>
#include <iostream>
#include <utility>

#include "metaeorite/core/Registry.hpp"
#include "metaeorite/core/Version.hpp"
#include "metaeorite/geometry_to_maxwell/IGeometryToMaxwellEngine.hpp"
#include "metaeorite/geometry_to_maxwell/Registration.hpp"
#include "metaeorite/maxwell_to_geometry/IMaxwellToGeometryEngine.hpp"
#include "metaeorite/maxwell_to_geometry/Registration.hpp"
#include "metaeorite/maxwell_to_metric/IMaxwellToMetricEngine.hpp"
#include "metaeorite/maxwell_to_metric/Registration.hpp"
#include "metaeorite/metric_to_maxwell/IMetricToMaxwellEngine.hpp"
#include "metaeorite/metric_to_maxwell/Registration.hpp"
#include "metaeorite/pipeline/Pipeline.hpp"

using namespace metaeorite;

namespace {

pipeline::Pipeline buildBaselinePipeline() {
    // Each module registers its engines explicitly (no static-init magic),
    // then the Pipeline resolves them purely by id through the registry.
    metric_to_maxwell::registerEngines();
    maxwell_to_metric::registerEngines();
    maxwell_to_geometry::registerEngines();
    geometry_to_maxwell::registerEngines();

    auto metricToMaxwell =
        core::EngineRegistry<metric_to_maxwell::IMetricToMaxwellEngine>::instance().create("vacuum-baseline");
    auto maxwellToMetric =
        core::EngineRegistry<maxwell_to_metric::IMaxwellToMetricEngine>::instance().create("vacuum-baseline");
    auto maxwellToGeometry =
        core::EngineRegistry<maxwell_to_geometry::IMaxwellToGeometryEngine>::instance().create("vacuum-baseline");
    auto geometryToMaxwell =
        core::EngineRegistry<geometry_to_maxwell::IGeometryToMaxwellEngine>::instance().create("vacuum-baseline");

    return pipeline::Pipeline(std::move(metricToMaxwell), std::move(maxwellToMetric),
                               std::move(maxwellToGeometry), std::move(geometryToMaxwell));
}

} // namespace

int main() {
    std::cout << "Metaeorite v" << core::versionString() << " -- architecture demo\n";
    std::cout << "----------------------------------------------------------------\n";

    const pipeline::Pipeline pipeline = buildBaselinePipeline();

    std::cout << std::boolalpha;

    std::cout << "1) Metric -> Maxwell (flat spacetime)\n";
    const core::Metric flat = core::Metric::minkowski();
    const core::ConstitutiveRelations constitutive = pipeline.forward(flat);
    std::cout << "   vacuum-like constitutive relations: " << constitutive.isVacuumLike() << "\n";

    std::cout << "2) Maxwell -> Metric (vacuum constitutive relations)\n";
    const auto metricCandidates = pipeline.inverse(constitutive);
    std::cout << "   candidates found: " << metricCandidates.size() << "\n";
    if (!metricCandidates.empty()) {
        const auto& best = metricCandidates.best();
        std::cout << "   best candidate confidence: " << best.confidence
                  << ", is Minkowski: " << best.value.isMinkowski() << "\n";
    }

    std::cout << "3) Maxwell -> Geometry (vacuum constitutive relations)\n";
    const auto geometryCandidates = pipeline.synthesizeGeometry(constitutive);
    std::cout << "   candidates found: " << geometryCandidates.size() << "\n";
    if (!geometryCandidates.empty()) {
        std::cout << "   best candidate: " << geometryCandidates.best().value->describe() << "\n";
    }

    std::cout << "4) Geometry -> Maxwell (free-space baseline)\n";
    const core::NullGeometry freeSpace;
    const core::ConstitutiveRelations homogenized =
        pipeline.homogenize(freeSpace, core::MaterialProperties::vacuum());
    std::cout << "   vacuum-like constitutive relations: " << homogenized.isVacuumLike() << "\n";

    std::cout << "----------------------------------------------------------------\n";
    std::cout << "All four transformation engines are wired together purely through\n"
                 "abstract interfaces (see modules/*/include). Replace any\n"
                 "PlaceholderEngine with a real implementation to extend physics\n"
                 "coverage without touching this pipeline or any other module.\n";

    return 0;
}
