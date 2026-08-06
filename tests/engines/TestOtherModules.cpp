#include <cmath>
#include <memory>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "metaeorite/core/Geometry.hpp"
#include "metaeorite/core/MaterialProperties.hpp"
#include "metaeorite/geometry_to_maxwell/RadialLaminateEngine.hpp"
#include "metaeorite/maxwell_to_geometry/RadialLaminateEngine.hpp"
#include "metaeorite/maxwell_to_metric/TransformationOpticsMetricEngine.hpp"
#include "metaeorite/metric_to_maxwell/TransformationOptics.hpp"

using Catch::Matchers::WithinAbs;
using namespace metaeorite;

TEST_CASE("maxwell_to_metric::TransformationOpticsMetricEngine inverts the cylindrical cloak", "[maxwell_to_metric]") {
    constexpr double innerRadius = 1.0;
    constexpr double outerRadius = 2.0;
    const maxwell_to_metric::TransformationOpticsMetricEngine engine;

    for (const double r : {1.2, 1.5, 1.8}) {
        const auto jacobian = metric_to_maxwell::cylindricalCloakJacobian(r, innerRadius, outerRadius);
        const auto constitutive = metric_to_maxwell::transformationOpticsConstitutive(jacobian);

        const auto result = engine.transform(constitutive);
        REQUIRE_FALSE(result.empty());

        const auto& metric = result.best().value;
        // Ground truth: g_ii = Lambda_ii^2 (spatial metric pulled back through the same Jacobian).
        CAPTURE(r);
        REQUIRE_THAT(metric.components()(1, 1), WithinAbs(jacobian(0, 0) * jacobian(0, 0), 1e-9));
        REQUIRE_THAT(metric.components()(2, 2), WithinAbs(jacobian(1, 1) * jacobian(1, 1), 1e-9));
        REQUIRE_THAT(metric.components()(3, 3), WithinAbs(jacobian(2, 2) * jacobian(2, 2), 1e-9));
        REQUIRE_THAT(metric.components()(0, 0), WithinAbs(-1.0, 1e-9));
    }
}

TEST_CASE("maxwell_to_metric::TransformationOpticsMetricEngine rejects non impedance-matched input",
          "[maxwell_to_metric]") {
    const maxwell_to_metric::TransformationOpticsMetricEngine engine;
    core::ConstitutiveRelations constitutive;
    constitutive.epsilon = core::Mat3c::Identity() * 2.0;
    constitutive.mu = core::Mat3c::Identity(); // epsilon != mu

    const auto result = engine.transform(constitutive);
    REQUIRE(result.empty());
    REQUIRE_FALSE(result.notes().empty());
}

TEST_CASE("maxwell_to_geometry / geometry_to_maxwell RadialLaminateEngine round-trip on the cylindrical cloak",
          "[maxwell_to_geometry][geometry_to_maxwell]") {
    constexpr double innerRadius = 1.0;
    constexpr double outerRadius = 2.0;
    constexpr double hostEpsilon = 1.0;
    constexpr double inclusionEpsilon = 10.0;

    const maxwell_to_geometry::RadialLaminateEngine synthesisEngine(hostEpsilon, inclusionEpsilon);
    const geometry_to_maxwell::RadialLaminateEngine homogenizationEngine;
    core::MaterialProperties materials;
    materials.bulkPermittivity = {inclusionEpsilon, 0.0};

    for (const double r : {1.2, 1.5, 1.8}) {
        const auto jacobian = metric_to_maxwell::cylindricalCloakJacobian(r, innerRadius, outerRadius);
        const auto target = metric_to_maxwell::transformationOpticsConstitutive(jacobian);

        const auto synthesis = synthesisEngine.transform(target);
        REQUIRE_FALSE(synthesis.empty());
        const auto& geometry = synthesis.best().value;

        const auto achieved = homogenizationEngine.transform(*geometry, materials);

        CAPTURE(r);
        // Tangential (epsilon_phi) component is solved for exactly by construction.
        REQUIRE_THAT(achieved.epsilon(1, 1).real(), WithinAbs(target.epsilon(1, 1).real(), 1e-9));
        // Radial (epsilon_r) component is only approximate - a harmonic mean of the two
        // constituent permittivities is always bounded between them, regardless of filling
        // fraction, so it generally will NOT match the ideal cloak's target value exactly.
        REQUIRE(achieved.epsilon(0, 0).real() >= hostEpsilon);
        REQUIRE(achieved.epsilon(0, 0).real() <= inclusionEpsilon);
    }
}
