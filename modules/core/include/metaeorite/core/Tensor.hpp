#pragma once

#include <complex>

#include <Eigen/Dense>
#include <xtensor/xarray.hpp>

namespace metaeorite::core {

/// Common scalar types used across the toolkit. Constitutive tensors are
/// complex-valued so lossy / dispersive media can be represented later
/// without changing the public API.
using Real = double;
using Complex = std::complex<Real>;

/// Rank-2 (3x3) complex tensor, used for spatial constitutive tensors
/// (permittivity, permeability, magnetoelectric coupling).
using Mat3c = Eigen::Matrix<Complex, 3, 3>;

/// Rank-2 (4x4) real tensor, provided for callers that want a fixed-size
/// spacetime metric representation.
using Mat4r = Eigen::Matrix<Real, 4, 4>;

/// General N-dimensional tensor storage. Used where the rank/dimension is
/// not fixed at compile time (e.g. metrics, once volumetric metamaterials
/// and higher-dimensional charts are supported).
using NDTensor = xt::xarray<Real>;

} // namespace metaeorite::core
