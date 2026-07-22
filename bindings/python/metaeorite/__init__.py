"""Metaeorite - Python bindings for the C++ transformation-optics toolkit.

The compiled extension `_metaeorite_native` is built by CMake (see
bindings/python/CMakeLists.txt) directly into this package directory, so
`import metaeorite` works from a source checkout once the C++ project has
been configured/built with Python bindings enabled (the default; see the
"Python bindings" section of the top-level README) - no `pip install`
step is required for local testing or Jupyter notebook demos.

Example
-------
>>> import metaeorite as mo
>>> flat = mo.Metric.minkowski()
>>> constitutive = mo.metric_to_maxwell(flat)
>>> constitutive.is_vacuum_like()
True
"""

from ._metaeorite_native import (
    ConstitutiveRelations,
    CoordinateChart,
    CoordinateSystem,
    DEFAULT_ENGINE_ID,
    Geometry,
    GeometryCandidate,
    GeometryReconstructionResult,
    MaterialProperties,
    Metric,
    MetricCandidate,
    MetricReconstructionResult,
    NullGeometry,
    UnitCellDescriptor,
    __version__,
    available_engines,
    geometry_to_maxwell,
    maxwell_to_geometry,
    maxwell_to_metric,
    metric_to_maxwell,
)

__all__ = [
    "ConstitutiveRelations",
    "CoordinateChart",
    "CoordinateSystem",
    "DEFAULT_ENGINE_ID",
    "Geometry",
    "GeometryCandidate",
    "GeometryReconstructionResult",
    "MaterialProperties",
    "Metric",
    "MetricCandidate",
    "MetricReconstructionResult",
    "NullGeometry",
    "UnitCellDescriptor",
    "__version__",
    "available_engines",
    "geometry_to_maxwell",
    "maxwell_to_geometry",
    "maxwell_to_metric",
    "metric_to_maxwell",
]
