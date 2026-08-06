#include "metaeorite/maxwell_to_metric/TransformationOpticsMetricEngine.hpp"

#include <xtensor/xbuilder.hpp>

namespace metaeorite::maxwell_to_metric {

core::ReconstructionResult<core::Metric> TransformationOpticsMetricEngine::transform(
    const core::ConstitutiveRelations& constitutive) const {
    core::ReconstructionResult<core::Metric> result;

    constexpr double tolerance = 1e-9;
    if (!constitutive.epsilon.isApprox(constitutive.mu, tolerance)) {
        result.addNote(
            "TransformationOpticsMetricEngine requires an impedance-matched (epsilon == mu) constitutive "
            "tensor, as produced by a purely spatial orthonormal-frame Jacobian transformation; general "
            "reconstruction of non-impedance-matched media is future work.");
        return result;
    }

    const Eigen::Matrix3d epsilonReal = constitutive.epsilon.real();
    const Eigen::Matrix3d offDiagonal = epsilonReal - epsilonReal.diagonal().asDiagonal().toDenseMatrix();
    if (offDiagonal.cwiseAbs().maxCoeff() > tolerance || constitutive.epsilon.imag().cwiseAbs().maxCoeff() > tolerance) {
        result.addNote(
            "TransformationOpticsMetricEngine requires a real, diagonal constitutive tensor (in the same "
            "orthonormal frame the metric will be expressed in); non-diagonal or lossy media are future work.");
        return result;
    }

    const double detEpsilon = epsilonReal.determinant();
    if (detEpsilon <= 0.0) {
        result.addNote("TransformationOpticsMetricEngine: det(epsilon) <= 0, no real metric reconstruction exists.");
        return result;
    }

    // g_ii = epsilon_ii / det(epsilon), the inverse of Lambda*Lambda^T/det(Lambda) at each diagonal
    // entry - exact when epsilon originated from a diagonal orthonormal-frame Jacobian (see
    // metaeorite/metric_to_maxwell/TransformationOptics.hpp).
    const Eigen::Vector3d spatialMetricDiagonal = epsilonReal.diagonal() / detEpsilon;

    core::NDTensor components = xt::zeros<core::Real>({std::size_t{4}, std::size_t{4}});
    components(0, 0) = -1.0;
    for (unsigned i = 0; i < 3; ++i) {
        components(i + 1, i + 1) = spatialMetricDiagonal(static_cast<Eigen::Index>(i));
    }

    core::CoordinateSystem coords;
    coords.chart = core::CoordinateChart::Cylindrical;
    coords.dimension = 4;
    coords.label = "cylindrical (reconstructed from transformation-optics constitutive relations)";

    result.addCandidate(
        core::Metric(std::move(coords), std::move(components)), 0.95,
        {"assumes a purely spatial coordinate transformation (no time-space mixing, g_0i = 0)",
         "assumes epsilon == mu (impedance matched)",
         "exact for diagonal orthonormal-frame Jacobian transformations (e.g. the Pendry cylindrical cloak)"});
    return result;
}

} // namespace metaeorite::maxwell_to_metric
