#include "metaeorite/core/SymmetryClassifier.hpp"

#include <cstddef>

#include <spglib.h>

namespace metaeorite::core {

SymmetryClassification classifySymmetry(const LatticeBasis& lattice, double symprec) {
    const int numAtoms = static_cast<int>(lattice.atomTypes.size());

    double latticeMatrix[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            latticeMatrix[i][j] = lattice.latticeVectors[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)];
        }
    }

    // spglib's C API requires a contiguous double[][3] buffer for positions.
    std::vector<double> flatPositions(static_cast<std::size_t>(numAtoms) * 3);
    for (int i = 0; i < numAtoms; ++i) {
        const auto& position = lattice.fractionalPositions[static_cast<std::size_t>(i)];
        flatPositions[static_cast<std::size_t>(i) * 3 + 0] = position[0];
        flatPositions[static_cast<std::size_t>(i) * 3 + 1] = position[1];
        flatPositions[static_cast<std::size_t>(i) * 3 + 2] = position[2];
    }
    auto* positions = reinterpret_cast<double(*)[3]>(flatPositions.data());

    std::vector<int> types = lattice.atomTypes;

    SymmetryClassification classification;

    SpglibDataset* dataset = spg_get_dataset(latticeMatrix, positions, types.data(), numAtoms, symprec);
    if (dataset != nullptr) {
        classification.pointGroup = dataset->pointgroup_symbol;
        spg_free_dataset(dataset);
    }

    return classification;
}

} // namespace metaeorite::core
