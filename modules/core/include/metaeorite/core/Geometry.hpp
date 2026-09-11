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

/// A radially-layered laminate: alternating thin shells of two isotropic
/// dielectrics, with `fillingFraction` the fraction occupied by the first
/// material. Structural-only (the two materials' permittivities are
/// supplied separately as MaterialProperties/engine parameters at
/// homogenization/synthesis time, not stored here), so the same geometry
/// can be re-homogenized against different material choices. See
/// core::LaminateEffectiveMedium for the mixing formulas used to realize
/// and homogenize this geometry against a target anisotropic medium (e.g.
/// the cylindrical invisibility cloak's radial/tangential permittivities).
class RadialLaminateGeometry final : public IGeometry {
public:
    RadialLaminateGeometry(double fillingFraction, double layerPeriod) : fillingFraction_(fillingFraction) {
        unitCell_.latticeConstants = {layerPeriod, layerPeriod};
        unitCell_.symmetryGroup = "radial-laminate";
    }

    [[nodiscard]] std::string describe() const override {
        return "radial-laminate-geometry (filling fraction = " + std::to_string(fillingFraction_) + ")";
    }
    [[nodiscard]] unsigned dimension() const override { return 2; }
    [[nodiscard]] const UnitCellDescriptor& unitCell() const override { return unitCell_; }
    [[nodiscard]] double fillingFraction() const noexcept { return fillingFraction_; }

private:
    double fillingFraction_;
    UnitCellDescriptor unitCell_{};
};

/// A planar (Cartesian) laminate: alternating thin slabs of two isotropic
/// dielectrics stacked along one Cartesian axis, with `fillingFraction`
/// the fraction occupied by the first material. Sibling of
/// RadialLaminateGeometry for macroscopic configurations without
/// cylindrical symmetry (e.g. flat metasurfaces, planar GRIN devices);
/// see core::LaminateEffectiveMedium for the shared mixing formulas.
class PlanarLaminateGeometry final : public IGeometry {
public:
    PlanarLaminateGeometry(double fillingFraction, double layerPeriod) : fillingFraction_(fillingFraction) {
        unitCell_.latticeConstants = {layerPeriod, layerPeriod};
        unitCell_.symmetryGroup = "planar-laminate";
    }

    [[nodiscard]] std::string describe() const override {
        return "planar-laminate-geometry (filling fraction = " + std::to_string(fillingFraction_) + ")";
    }
    [[nodiscard]] unsigned dimension() const override { return 2; }
    [[nodiscard]] const UnitCellDescriptor& unitCell() const override { return unitCell_; }
    [[nodiscard]] double fillingFraction() const noexcept { return fillingFraction_; }

private:
    double fillingFraction_;
    UnitCellDescriptor unitCell_{};
};

} // namespace metaeorite::core
