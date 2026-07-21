#include <stdexcept>

#include <catch2/catch_test_macros.hpp>
#include <xtensor/xbuilder.hpp>

#include "metaeorite/core/ConstitutiveRelations.hpp"
#include "metaeorite/core/Metric.hpp"
#include "metaeorite/core/ReconstructionResult.hpp"

using namespace metaeorite::core;

TEST_CASE("ConstitutiveRelations::vacuum is vacuum-like", "[core]") {
    const auto vacuum = ConstitutiveRelations::vacuum();
    REQUIRE(vacuum.isVacuumLike());
}

TEST_CASE("ConstitutiveRelations with non-trivial epsilon is not vacuum-like", "[core]") {
    ConstitutiveRelations exotic;
    exotic.epsilon *= 2.0;
    REQUIRE_FALSE(exotic.isVacuumLike());
}

TEST_CASE("Metric::minkowski produces the flat metric", "[core]") {
    const auto metric = Metric::minkowski();
    REQUIRE(metric.dimension() == 4);
    REQUIRE(metric.isMinkowski());
}

TEST_CASE("Metric rejects non-square components", "[core]") {
    xt::xarray<double> bad = xt::zeros<double>({3, 4});
    CoordinateSystem coords;
    REQUIRE_THROWS_AS(Metric(coords, bad), std::invalid_argument);
}

TEST_CASE("ReconstructionResult tracks candidates and notes", "[core]") {
    ReconstructionResult<int> result;
    REQUIRE(result.empty());

    result.addCandidate(1, 0.2);
    result.addCandidate(2, 0.9, {"preferred"});
    result.addNote("informational note");

    REQUIRE(result.size() == 2);
    REQUIRE(result.best().value == 2);
    REQUIRE(result.notes().size() == 1);
}

TEST_CASE("ReconstructionResult::best throws when empty", "[core]") {
    const ReconstructionResult<int> result;
    REQUIRE_THROWS_AS(result.best(), std::out_of_range);
}
