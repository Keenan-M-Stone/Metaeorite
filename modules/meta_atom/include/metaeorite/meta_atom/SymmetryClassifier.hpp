#pragma once

#include <array>
#include <vector>

#include "metaeorite/meta_atom/SymmetryClassification.hpp"

namespace metaeorite::meta_atom {

/// Periodic lattice + atomic basis, expressed in the 3-lattice-vector /
/// fractional-coordinate convention expected by spglib. Planar (2D) unit
/// cells are represented by embedding them in 3D with a third lattice
/// vector long enough to act as a vacuum buffer between periodic images
/// along that axis - the same slab-symmetry technique used by ASE/pymatgen
/// - rather than depending on spglib's experimental layer-group API.
struct LatticeBasis {
    std::array<std::array<double, 3>, 3> latticeVectors{};
    std::vector<std::array<double, 3>> fractionalPositions;
    std::vector<int> atomTypes;
};

/// Classifies the point-/space-group symmetry of a lattice + basis using
/// spglib (https://github.com/spglib/spglib, BSD-3-Clause), rather than
/// reimplementing crystallographic symmetry search from scratch. Backs the
/// symmetry/meta-atom-structure analysis step of `maxwell_to_geometry` /
/// `geometry_to_maxwell` once those engines gain real geometry backends.
///
/// `SymmetryClassification::irreducibleRepresentations` is intentionally
/// left empty: spglib reports space/point groups only, not irreducible
/// representations, so that mapping remains future representation-theory
/// work built on top of this classification.
[[nodiscard]] SymmetryClassification classifySymmetry(const LatticeBasis& lattice, double symprec = 1e-5);

} // namespace metaeorite::meta_atom
