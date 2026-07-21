#pragma once

#include <array>
#include <string>

namespace metaeorite::core {

/// Minimal description of a periodic planar unit cell. Extended by concrete
/// geometry backends (parametric primitives, CAD kernels, ...) with
/// additional fields as needed.
struct UnitCellDescriptor {
    std::array<double, 2> latticeConstants{0.0, 0.0};
    std::string symmetryGroup{"C1"};
};

/// Abstract representation of a candidate metamaterial geometry. This is an
/// interface only for now: concrete backends (parametric primitive
/// libraries, mesh exporters, CAD kernels such as OpenCASCADE, ...) are
/// intentionally left as a future extension point, so no other module needs
/// to change once one becomes available.
class IGeometry {
public:
    virtual ~IGeometry() = default;

    /// Human-readable summary, useful for logging/debugging.
    [[nodiscard]] virtual std::string describe() const = 0;

    /// Spatial dimension of the geometry (2 for the current planar scope,
    /// 0 for the free-space/"no structure" baseline).
    [[nodiscard]] virtual unsigned dimension() const = 0;

    [[nodiscard]] virtual const UnitCellDescriptor& unitCell() const = 0;
};

/// Trivial concrete geometry representing free space / "no structure".
/// Used as the baseline case paired with ConstitutiveRelations::vacuum(),
/// analogous to Metric::minkowski(), until real geometry backends exist.
class NullGeometry final : public IGeometry {
public:
    [[nodiscard]] std::string describe() const override { return "null-geometry (free space)"; }
    [[nodiscard]] unsigned dimension() const override { return 0; }
    [[nodiscard]] const UnitCellDescriptor& unitCell() const override { return unitCell_; }

private:
    UnitCellDescriptor unitCell_{};
};

} // namespace metaeorite::core
