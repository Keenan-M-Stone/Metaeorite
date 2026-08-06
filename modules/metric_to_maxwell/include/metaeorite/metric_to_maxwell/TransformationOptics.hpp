#pragma once

#include <Eigen/Dense>

#include "metaeorite/core/ConstitutiveRelations.hpp"

namespace metaeorite::metric_to_maxwell {

/// Generalized constitutive relations for a purely spatial coordinate
/// transformation applied to an isotropic background medium, using the
/// Jacobian-based transformation-optics formula (Pendry, Schurig & Smith,
/// "Calculation of material properties and ray tracing in transformation
/// media", Opt. Express 14, 9794 (2006)):
///
///   epsilon'^ij = mu'^ij = (Lambda * Lambda^T)^ij / det(Lambda)
///
/// where `jacobian` is Lambda^i_a = d(physical coordinate i)/d(virtual
/// coordinate a), expressed in an orthonormal frame (so the virtual,
/// undistorted background is simply the identity tensor scaled by
/// `backgroundEpsilon`/`backgroundMu`). This is the spatial-only
/// specialization of the general Plebanski correspondence - no
/// magnetoelectric coupling, since there is no time-space mixing. A fully
/// general Metric -> Maxwell engine handling arbitrary coordinate bases
/// and time-space mixing remains future work (see docs/THEORY.ipynb).
[[nodiscard]] core::ConstitutiveRelations transformationOpticsConstitutive(const Eigen::Matrix3d& jacobian,
                                                                            double backgroundEpsilon = 1.0,
                                                                            double backgroundMu = 1.0);

/// Orthonormal-frame Jacobian, at physical radius `r`, of the canonical
/// cylindrical invisibility cloak (Pendry, Schurig & Smith, "Controlling
/// Electromagnetic Fields", Science 312, 1780 (2006)): the linear radial
/// compression that maps empty virtual space 0 < r0 < outerRadius into the
/// physical shell innerRadius < r < outerRadius, hiding r < innerRadius.
///
///   r0 = (r - innerRadius) * outerRadius / (outerRadius - innerRadius)
///
/// `r` must lie in (innerRadius, outerRadius] - the inner boundary r =
/// innerRadius is the well-known coordinate singularity of the ideal
/// cloak. Feed the result directly into transformationOpticsConstitutive()
/// to reproduce the cloak's published closed-form epsilon_r, epsilon_phi,
/// epsilon_z.
[[nodiscard]] Eigen::Matrix3d cylindricalCloakJacobian(double r, double innerRadius, double outerRadius);

} // namespace metaeorite::metric_to_maxwell
