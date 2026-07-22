#include <algorithm>
#include <stdexcept>

#include <catch2/catch_test_macros.hpp>

#include "metaeorite/api/Functions.hpp"
#include "metaeorite/core/Metric.hpp"

using namespace metaeorite;

TEST_CASE("api::metric_to_maxwell reproduces vacuum for Minkowski", "[api]") {
    const core::ConstitutiveRelations constitutive = api::metric_to_maxwell(core::Metric::minkowski());
    REQUIRE(constitutive.isVacuumLike());
}

TEST_CASE("api::maxwell_to_metric reconstructs Minkowski from vacuum", "[api]") {
    const auto result = api::maxwell_to_metric(core::ConstitutiveRelations::vacuum());
    REQUIRE_FALSE(result.empty());
    REQUIRE(result.best().value.isMinkowski());
}

TEST_CASE("api::maxwell_to_geometry synthesizes null geometry for vacuum", "[api]") {
    const auto result = api::maxwell_to_geometry(core::ConstitutiveRelations::vacuum());
    REQUIRE_FALSE(result.empty());
    REQUIRE(result.best().value->dimension() == 0);
}

TEST_CASE("api::geometry_to_maxwell homogenizes free space to vacuum", "[api]") {
    const core::NullGeometry freeSpace;
    const core::ConstitutiveRelations constitutive = api::geometry_to_maxwell(freeSpace);
    REQUIRE(constitutive.isVacuumLike());
}

TEST_CASE("api functions reject unknown engine ids", "[api]") {
    REQUIRE_THROWS_AS(api::metric_to_maxwell(core::Metric::minkowski(), "does-not-exist"), std::out_of_range);
}

TEST_CASE("api::availableMetricToMaxwellEngines lists the baseline engine", "[api]") {
    const auto engines = api::availableMetricToMaxwellEngines();
    REQUIRE(std::find(engines.begin(), engines.end(), "vacuum-baseline") != engines.end());
}
