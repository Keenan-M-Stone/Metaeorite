#include "metaeorite/core/ConstitutiveRelations.hpp"

namespace metaeorite::core {

ConstitutiveRelations ConstitutiveRelations::vacuum() {
    return ConstitutiveRelations{};
}

bool ConstitutiveRelations::isVacuumLike(Real tolerance) const {
    return epsilon.isApprox(Mat3c::Identity(), tolerance) &&
           mu.isApprox(Mat3c::Identity(), tolerance) &&
           xi.isZero(tolerance) &&
           zeta.isZero(tolerance);
}

} // namespace metaeorite::core
