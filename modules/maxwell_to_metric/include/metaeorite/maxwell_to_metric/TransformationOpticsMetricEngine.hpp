#pragma once

#include "metaeorite/maxwell_to_metric/IMaxwellToMetricEngine.hpp"

namespace metaeorite::maxwell_to_metric {

/// Inverts transformationOpticsConstitutive() (see
/// metaeorite/metric_to_maxwell/TransformationOptics.hpp): given a
/// diagonal, impedance-matched (epsilon == mu) constitutive tensor
/// produced by a purely spatial, orthonormal-frame Jacobian
/// transformation, reconstructs the pulled-back spatial metric
/// g_ii = epsilon_ii / det(epsilon), embedded in a 4x4 block with
/// g_00 = -1 (no time-space mixing assumed).
///
/// This is exact for diagonal Jacobian transformations such as the
/// Pendry-Schurig-Smith cylindrical cloak, and is validated against it in
/// tests/engines/TestTransformationOptics.cpp. Non-diagonal or non
/// impedance-matched inputs yield an empty result with an explanatory
/// note; the fully general Metric reconstruction remains future work.
class TransformationOpticsMetricEngine final : public IMaxwellToMetricEngine {
public:
    [[nodiscard]] core::ReconstructionResult<core::Metric> transform(
        const core::ConstitutiveRelations& constitutive) const override;

    [[nodiscard]] std::string name() const override { return "transformation-optics"; }
};

} // namespace metaeorite::maxwell_to_metric
