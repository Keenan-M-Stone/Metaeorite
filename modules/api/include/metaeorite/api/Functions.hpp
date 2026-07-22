#pragma once

#include <memory>
#include <string>
#include <vector>

#include "metaeorite/core/ConstitutiveRelations.hpp"
#include "metaeorite/core/Geometry.hpp"
#include "metaeorite/core/MaterialProperties.hpp"
#include "metaeorite/core/Metric.hpp"
#include "metaeorite/core/ReconstructionResult.hpp"

/// Thin, function-based facade over the four transformation engines
/// (metaeorite::metric_to_maxwell, maxwell_to_metric, maxwell_to_geometry,
/// geometry_to_maxwell) for callers who just want to call a function rather
/// than construct a Pipeline or talk to core::EngineRegistry directly - the
/// intended entry point for external C++ projects, the Python bindings
/// (bindings/python), and ad-hoc testing/notebook demos.
///
/// Every function resolves its engine by string id from the same
/// core::EngineRegistry the rest of the project uses, defaulting to
/// "vacuum-baseline" (the trivial flat-spacetime/vacuum case implemented by
/// every module's PlaceholderEngine - see docs/THEORY.ipynb Part 7 for what
/// remains to be implemented). Engines are registered lazily on first call,
/// so no separate setup step is required before using this API.
namespace metaeorite::api {

/// The only engine id guaranteed to be registered for every transformation
/// today (see PlaceholderEngine in each module).
inline constexpr const char* kDefaultEngineId = "vacuum-baseline";

/// Converts a spacetime metric into the generalized constitutive relations
/// of an equivalent electromagnetic medium, via the Plebanski formulation
/// of transformation optics (docs/THEORY.ipynb Part 3).
///
/// Throws std::out_of_range if `engineId` is not registered, or whatever
/// the underlying engine throws for unsupported input (the baseline engine
/// throws std::logic_error for any non-Minkowski metric).
[[nodiscard]] core::ConstitutiveRelations metric_to_maxwell(
    const core::Metric& metric, const std::string& engineId = kDefaultEngineId);

/// Attempts the inverse of metric_to_maxwell: given constitutive relations,
/// reconstruct one or more compatible spacetime metrics. This inverse
/// problem is generally non-unique (docs/THEORY.ipynb Part 4), so the
/// result is a ranked list of candidates rather than a single metric.
[[nodiscard]] core::ReconstructionResult<core::Metric> maxwell_to_metric(
    const core::ConstitutiveRelations& constitutive, const std::string& engineId = kDefaultEngineId);

/// Synthesizes candidate metamaterial unit-cell geometries realizing the
/// given constitutive relations (also generally non-unique).
[[nodiscard]] core::ReconstructionResult<std::shared_ptr<core::IGeometry>> maxwell_to_geometry(
    const core::ConstitutiveRelations& constitutive, const std::string& engineId = kDefaultEngineId);

/// Homogenizes an existing metamaterial geometry into effective
/// constitutive relations (docs/THEORY.ipynb Part 5).
[[nodiscard]] core::ConstitutiveRelations geometry_to_maxwell(
    const core::IGeometry& geometry,
    const core::MaterialProperties& materials = core::MaterialProperties::vacuum(),
    const std::string& engineId = kDefaultEngineId);

/// Engine ids currently registered for each of the four transformation
/// categories - useful for discovering what is available beyond the
/// baseline as real engines are added.
[[nodiscard]] std::vector<std::string> availableMetricToMaxwellEngines();
[[nodiscard]] std::vector<std::string> availableMaxwellToMetricEngines();
[[nodiscard]] std::vector<std::string> availableMaxwellToGeometryEngines();
[[nodiscard]] std::vector<std::string> availableGeometryToMaxwellEngines();

} // namespace metaeorite::api
