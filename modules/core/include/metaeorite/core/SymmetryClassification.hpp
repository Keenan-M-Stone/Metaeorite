#pragma once

#include <string>
#include <vector>

namespace metaeorite::core {

/// Point-group / representation-theoretic classification of a geometry's
/// symmetry, used to constrain admissible constitutive tensors. Populated
/// by future Maxwell <-> Geometry engines that implement symmetry analysis.
struct SymmetryClassification {
    std::string pointGroup{"C1"};
    std::vector<std::string> irreducibleRepresentations;
};

} // namespace metaeorite::core
