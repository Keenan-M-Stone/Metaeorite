#pragma once

namespace metaeorite::core {

/// Classical (Wiener bounds) effective-medium mixing formulas for a
/// laminate/laminated stack of two isotropic dielectrics with filling
/// fraction `f` of material 1 (0 <= f <= 1). Used by both directions of
/// the Maxwell <-> Geometry correspondence for radially-layered
/// transformation-optics realizations (see e.g. Cai, Chettiar, Kildishev &
/// Shalaev, "Optical cloaking with metamaterials", Nature Photonics 1, 224
/// (2007), which realizes an anisotropic cloak this same way).
///
/// A stack of thin layers has two distinct effective permittivities:
/// - parallel to the layers (electric field tangential to the interfaces):
///   the arithmetic mean (Wiener upper bound).
/// - perpendicular to the layers (electric field normal to the
///   interfaces, displacement field continuous): the harmonic mean
///   (Wiener lower bound).
///
/// A single filling fraction cannot in general match two independently
/// prescribed target values (parallel and perpendicular) simultaneously -
/// this is a real, well-known trade-off in laminate-based transformation
/// optics realizations, not a limitation of this implementation.
struct LaminateEffectiveMedium {
    [[nodiscard]] static double parallelMix(double f, double epsilon1, double epsilon2) noexcept {
        return f * epsilon1 + (1.0 - f) * epsilon2;
    }

    [[nodiscard]] static double perpendicularMix(double f, double epsilon1, double epsilon2) noexcept {
        return 1.0 / (f / epsilon1 + (1.0 - f) / epsilon2);
    }

    /// Filling fraction `f` such that parallelMix(f, epsilon1, epsilon2) ==
    /// targetParallelEpsilon. Not clamped to [0, 1]; callers should check
    /// the result is physically realizable with the chosen material pair.
    [[nodiscard]] static double solveFillingFractionForParallel(double targetParallelEpsilon, double epsilon1,
                                                                 double epsilon2) noexcept {
        return (targetParallelEpsilon - epsilon2) / (epsilon1 - epsilon2);
    }
};

} // namespace metaeorite::core
