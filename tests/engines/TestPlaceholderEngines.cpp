#include <stdexcept>

#include <catch2/catch_test_macros.hpp>
#include <xtensor/xbuilder.hpp>

#include "metaeorite/geometry_to_maxwell/PlaceholderEngine.hpp"
#include "metaeorite/maxwell_to_geometry/PlaceholderEngine.hpp"
#include "metaeorite/maxwell_to_metric/PlaceholderEngine.hpp"
#include "metaeorite/metric_to_maxwell/PlaceholderEngine.hpp"

using namespace metaeorite;

TEST_CASE("metric_to_maxwell::PlaceholderEngine maps flat spacetime to vacuum", "[engines]") {
    const metric_to_maxwell::PlaceholderEngine engine;
    const auto constitutive = engine.transform(core::Metric::minkowski());
    REQUIRE(constitutive.isVacuumLike());
}

TEST_CASE("metric_to_maxwell::PlaceholderEngine rejects curved metrics", "[engines]") {
    const metric_to_maxwell::PlaceholderEngine engine;

    core::CoordinateSystem coords;
    coords.dimension = 4;
    xt::xarray<double> curved = xt::eye<double>(4);
    curved(1, 1) = 2.0; // not Minkowski

    REQUIRE_THROWS_AS(engine.transform(core::Metric(coords, curved)), std::logic_error);
}

TEST_CASE("maxwell_to_metric::PlaceholderEngine reconstructs flat spacetime from vacuum", "[engines]") {
    const maxwell_to_metric::PlaceholderEngine engine;
    const auto result = engine.transform(core::ConstitutiveRelations::vacuum());

    REQUIRE_FALSE(result.empty());
    REQUIRE(result.best().value.isMinkowski());
}

TEST_CASE("maxwell_to_metric::PlaceholderEngine yields no candidates for unknown media", "[engines]") {
    const maxwell_to_metric::PlaceholderEngine engine;

    core::ConstitutiveRelations exotic;
    exotic.epsilon *= 2.0;
    const auto result = engine.transform(exotic);

    REQUIRE(result.empty());
    REQUIRE_FALSE(result.notes().empty());
}

TEST_CASE("maxwell_to_geometry::PlaceholderEngine yields free space for vacuum", "[engines]") {
    const maxwell_to_geometry::PlaceholderEngine engine;
    const auto result = engine.transform(core::ConstitutiveRelations::vacuum());

    REQUIRE_FALSE(result.empty());
    REQUIRE(result.best().value->dimension() == 0);
}

TEST_CASE("geometry_to_maxwell::PlaceholderEngine homogenizes free space to vacuum", "[engines]") {
    const geometry_to_maxwell::PlaceholderEngine engine;
    const core::NullGeometry geometry;

    const auto constitutive = engine.transform(geometry, core::MaterialProperties::vacuum());
    REQUIRE(constitutive.isVacuumLike());
}
