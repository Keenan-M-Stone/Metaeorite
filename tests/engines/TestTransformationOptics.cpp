#include <cmath>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "metaeorite/metric_to_maxwell/TransformationOptics.hpp"

using namespace metaeorite::metric_to_maxwell;
using Catch::Matchers::WithinAbs;

namespace {

// Closed-form cylindrical cloak permittivity/permeability, transcribed
// directly from Pendry, Schurig & Smith, "Controlling Electromagnetic
// Fields", Science 312, 1780 (2006), eq. (5)-(7); computed independently
// of transformationOpticsConstitutive() so the test is a genuine check
// against the published result, not a restatement of the implementation.
struct ClosedFormCloak {
    double epsilonR;
    double epsilonPhi;
    double epsilonZ;
};

ClosedFormCloak closedFormCloak(double r, double a, double b) {
    const double scale = b / (b - a);
    return ClosedFormCloak{
        .epsilonR = (r - a) / r,
        .epsilonPhi = r / (r - a),
        .epsilonZ = scale * scale * (r - a) / r,
    };
}

} // namespace

TEST_CASE("transformationOpticsConstitutive reproduces the published cylindrical cloak", "[metric_to_maxwell]") {
    constexpr double innerRadius = 1.0;
    constexpr double outerRadius = 2.0;

    for (const double r : {1.2, 1.5, 1.8}) {
        const auto jacobian = cylindricalCloakJacobian(r, innerRadius, outerRadius);
        const auto constitutive = transformationOpticsConstitutive(jacobian);
        const auto expected = closedFormCloak(r, innerRadius, outerRadius);

        CAPTURE(r);
        REQUIRE_THAT(constitutive.epsilon(0, 0).real(), WithinAbs(expected.epsilonR, 1e-9));
        REQUIRE_THAT(constitutive.epsilon(1, 1).real(), WithinAbs(expected.epsilonPhi, 1e-9));
        REQUIRE_THAT(constitutive.epsilon(2, 2).real(), WithinAbs(expected.epsilonZ, 1e-9));

        // mu equals epsilon for this (impedance-matched, backgroundMu=1) case.
        REQUIRE_THAT(constitutive.mu(0, 0).real(), WithinAbs(expected.epsilonR, 1e-9));
        REQUIRE_THAT(constitutive.mu(1, 1).real(), WithinAbs(expected.epsilonPhi, 1e-9));
        REQUIRE_THAT(constitutive.mu(2, 2).real(), WithinAbs(expected.epsilonZ, 1e-9));

        // No magnetoelectric coupling and no off-diagonal terms for this
        // purely radial, orthogonal coordinate transformation.
        REQUIRE_THAT(constitutive.epsilon(0, 1).real(), WithinAbs(0.0, 1e-9));
        REQUIRE(constitutive.xi.isZero());
        REQUIRE(constitutive.zeta.isZero());
    }
}

TEST_CASE("transformationOpticsConstitutive is vacuum for the identity (untransformed) Jacobian",
          "[metric_to_maxwell]") {
    const auto constitutive = transformationOpticsConstitutive(Eigen::Matrix3d::Identity());
    REQUIRE(constitutive.isVacuumLike(1e-9));
}

TEST_CASE("transformationOpticsConstitutive is impedance-matched (mu == epsilon) throughout the cloak",
          "[metric_to_maxwell]") {
    // The cylindrical cloak's defining property: wave impedance equals
    // free space everywhere in the shell (mu == epsilon at every radius),
    // even though epsilon != 1 - this is what avoids reflections, not a
    // literal match to vacuum's identity tensor.
    for (const double r : {1.01, 1.4, 1.99}) {
        const auto jacobian = cylindricalCloakJacobian(r, 1.0, 2.0);
        const auto constitutive = transformationOpticsConstitutive(jacobian);
        REQUIRE(constitutive.epsilon.isApprox(constitutive.mu, 1e-9));
    }
}
