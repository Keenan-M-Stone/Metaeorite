#pragma once

#include "metaeorite/core/Tensor.hpp"

namespace metaeorite::core {

/// Generalized constitutive relations of Maxwell's equations, the common
/// intermediate representation shared by every transformation engine:
///
///   D = epsilon * E + xi * H
///   B = zeta * E + mu * H
///
/// All four tensors are 3x3 and complex-valued so dispersive / lossy media
/// can be represented later without an API change.
struct ConstitutiveRelations {
    Mat3c epsilon = Mat3c::Identity();
    Mat3c mu = Mat3c::Identity();
    Mat3c xi = Mat3c::Zero();
    Mat3c zeta = Mat3c::Zero();

    /// Vacuum / free-space constitutive relations: epsilon = mu = I,
    /// no magnetoelectric coupling. The trivial, exactly-known baseline
    /// case corresponding to flat (Minkowski) spacetime.
    [[nodiscard]] static ConstitutiveRelations vacuum();

    /// True if this is (approximately) the vacuum case.
    [[nodiscard]] bool isVacuumLike(Real tolerance = 1e-9) const;
};

} // namespace metaeorite::core
