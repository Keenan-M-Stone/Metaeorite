#include <memory>
#include <stdexcept>

#include <catch2/catch_test_macros.hpp>

#include "metaeorite/geometry_to_maxwell/PlaceholderEngine.hpp"
#include "metaeorite/maxwell_to_geometry/PlaceholderEngine.hpp"
#include "metaeorite/maxwell_to_metric/PlaceholderEngine.hpp"
#include "metaeorite/metric_to_maxwell/PlaceholderEngine.hpp"
#include "metaeorite/pipeline/Pipeline.hpp"

using namespace metaeorite;

namespace {

pipeline::Pipeline makeBaselinePipeline() {
    return pipeline::Pipeline(std::make_shared<metric_to_maxwell::PlaceholderEngine>(),
                               std::make_shared<maxwell_to_metric::PlaceholderEngine>(),
                               std::make_shared<maxwell_to_geometry::PlaceholderEngine>(),
                               std::make_shared<geometry_to_maxwell::PlaceholderEngine>());
}

} // namespace

TEST_CASE("Pipeline round-trips the flat-spacetime / vacuum baseline", "[pipeline]") {
    const auto pipeline = makeBaselinePipeline();

    const auto constitutive = pipeline.forward(core::Metric::minkowski());
    REQUIRE(constitutive.isVacuumLike());

    const auto metricCandidates = pipeline.inverse(constitutive);
    REQUIRE_FALSE(metricCandidates.empty());
    REQUIRE(metricCandidates.best().value.isMinkowski());

    const auto geometryCandidates = pipeline.synthesizeGeometry(constitutive);
    REQUIRE_FALSE(geometryCandidates.empty());

    const auto homogenized =
        pipeline.homogenize(*geometryCandidates.best().value, core::MaterialProperties::vacuum());
    REQUIRE(homogenized.isVacuumLike());
}

TEST_CASE("Pipeline throws when an engine dependency is missing", "[pipeline]") {
    const pipeline::Pipeline incomplete(nullptr, nullptr, nullptr, nullptr);
    REQUIRE_THROWS_AS(incomplete.forward(core::Metric::minkowski()), std::invalid_argument);
}
