#include "metaeorite/metric_to_maxwell/TransformationOptics.hpp"

namespace metaeorite::metric_to_maxwell {

core::ConstitutiveRelations transformationOpticsConstitutive(const Eigen::Matrix3d& jacobian,
                                                               double backgroundEpsilon, double backgroundMu) {
    const double detJ = jacobian.determinant();
    const Eigen::Matrix3d spatial = (jacobian * jacobian.transpose()) / detJ;

    core::ConstitutiveRelations result;
    result.epsilon = (spatial * backgroundEpsilon).cast<core::Complex>();
    result.mu = (spatial * backgroundMu).cast<core::Complex>();
    result.xi = core::Mat3c::Zero();
    result.zeta = core::Mat3c::Zero();
    return result;
}

Eigen::Matrix3d cylindricalCloakJacobian(double r, double innerRadius, double outerRadius) {
    const double shellThickness = outerRadius - innerRadius;
    const double virtualRadius = (r - innerRadius) * outerRadius / shellThickness;

    Eigen::Matrix3d jacobian = Eigen::Matrix3d::Zero();
    jacobian(0, 0) = shellThickness / outerRadius; // d(r) / d(r0)
    jacobian(1, 1) = r / virtualRadius;             // d(r*dphi) / d(r0*dphi0), since phi = phi0
    jacobian(2, 2) = 1.0;                           // d(z) / d(z0)
    return jacobian;
}

} // namespace metaeorite::metric_to_maxwell
