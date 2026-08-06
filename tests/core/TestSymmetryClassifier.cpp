#include <catch2/catch_test_macros.hpp>

#include "metaeorite/core/SymmetryClassifier.hpp"

using namespace metaeorite::core;

TEST_CASE("classifySymmetry identifies a simple cubic lattice", "[core][symmetry]") {
    LatticeBasis cubic;
    cubic.latticeVectors = {{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}};
    cubic.fractionalPositions = {{0.0, 0.0, 0.0}};
    cubic.atomTypes = {1};

    const auto classification = classifySymmetry(cubic);
    REQUIRE(classification.pointGroup == "m-3m");
}

TEST_CASE("classifySymmetry handles a vacuum-embedded 2D unit cell", "[core][symmetry]") {
    // A planar square lattice is embedded in 3D with a large c-axis vacuum
    // buffer, per the slab-symmetry technique documented on
    // SymmetryClassifier.hpp; the single atom sits on the highest-symmetry
    // site of the resulting tetragonal lattice.
    LatticeBasis slab;
    slab.latticeVectors = {{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 10.0}}};
    slab.fractionalPositions = {{0.0, 0.0, 0.0}};
    slab.atomTypes = {1};

    const auto classification = classifySymmetry(slab);
    REQUIRE(classification.pointGroup == "4/mmm");
}

TEST_CASE("classifySymmetry reports lower symmetry for an asymmetric basis", "[core][symmetry]") {
    LatticeBasis lowSymmetry;
    lowSymmetry.latticeVectors = {{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 10.0}}};
    lowSymmetry.fractionalPositions = {{0.0, 0.0, 0.0}, {0.3, 0.1, 0.0}};
    lowSymmetry.atomTypes = {1, 2};

    const auto classification = classifySymmetry(lowSymmetry);
    REQUIRE(classification.pointGroup == "m");
}
