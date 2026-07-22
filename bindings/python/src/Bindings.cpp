// Python bindings for the Metaeorite C++ toolkit, built on top of the
// metaeorite::api function facade (modules/api) so that the same four
// transformation entry points (metric_to_maxwell, maxwell_to_metric,
// maxwell_to_geometry, geometry_to_maxwell) are available from Python with
// the same names and default-engine behavior as in C++.
//
// core::Metric wraps an xtensor NDTensor, which has no pybind11 caster
// registered here (that would require the separate xtensor-python
// project), so this file exposes Metric's components as plain nested
// Python lists instead - simple, dependency-free, and sufficient for
// building/inspecting metrics from Python/Jupyter.

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <pybind11/complex.h>
#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <xtensor/xbuilder.hpp>

#include "metaeorite/api/Functions.hpp"
#include "metaeorite/core/CoordinateSystem.hpp"
#include "metaeorite/core/Version.hpp"

namespace py = pybind11;
using namespace metaeorite;

namespace {

core::Metric makeMetric(const std::vector<std::vector<double>>& rows, core::CoordinateSystem coordinateSystem) {
    const std::size_t n = rows.size();
    core::NDTensor components = xt::zeros<core::Real>({n, n});
    for (std::size_t i = 0; i < n; ++i) {
        if (rows[i].size() != n) {
            throw std::invalid_argument("Metric: components must be a square matrix (all rows the same length)");
        }
        for (std::size_t j = 0; j < n; ++j) {
            components(i, j) = rows[i][j];
        }
    }
    return core::Metric(std::move(coordinateSystem), std::move(components));
}

std::vector<std::vector<double>> metricComponents(const core::Metric& metric) {
    const auto dim = metric.dimension();
    std::vector<std::vector<double>> rows(dim, std::vector<double>(dim));
    for (unsigned i = 0; i < dim; ++i) {
        for (unsigned j = 0; j < dim; ++j) {
            rows[i][j] = metric.components()(i, j);
        }
    }
    return rows;
}

} // namespace

PYBIND11_MODULE(_metaeorite_native, m) {
    m.doc() = "Native bindings for the Metaeorite transformation-optics toolkit.";
    m.attr("__version__") = std::string(core::versionString());
    m.attr("DEFAULT_ENGINE_ID") = std::string(api::kDefaultEngineId);

    py::enum_<core::CoordinateChart>(m, "CoordinateChart")
        .value("CARTESIAN", core::CoordinateChart::Cartesian)
        .value("CYLINDRICAL", core::CoordinateChart::Cylindrical)
        .value("SPHERICAL", core::CoordinateChart::Spherical)
        .value("CUSTOM", core::CoordinateChart::Custom);

    py::class_<core::CoordinateSystem>(m, "CoordinateSystem")
        .def(py::init([](core::CoordinateChart chart, unsigned dimension, std::string label) {
                 core::CoordinateSystem cs;
                 cs.chart = chart;
                 cs.dimension = dimension;
                 cs.label = std::move(label);
                 return cs;
             }),
             py::arg("chart") = core::CoordinateChart::Cartesian, py::arg("dimension") = 4,
             py::arg("label") = "cartesian")
        .def_readwrite("chart", &core::CoordinateSystem::chart)
        .def_readwrite("dimension", &core::CoordinateSystem::dimension)
        .def_readwrite("label", &core::CoordinateSystem::label)
        .def("__repr__", [](const core::CoordinateSystem& cs) {
            return "CoordinateSystem(label='" + cs.label + "', dimension=" + std::to_string(cs.dimension) + ")";
        });

    py::class_<core::Metric>(m, "Metric")
        .def(py::init(&makeMetric), py::arg("components"), py::arg("coordinate_system") = core::CoordinateSystem{},
             "Build a metric from a square nested list of components, e.g. Metric([[-1, 0], [0, 1]]).")
        .def_static("minkowski", &core::Metric::minkowski, py::arg("spatial_dimension") = 3,
                    "Flat Minkowski metric diag(-1, 1, ..., 1) with `spatial_dimension` spatial dimensions.")
        .def("is_minkowski", &core::Metric::isMinkowski, py::arg("tolerance") = 1e-9)
        .def("dimension", &core::Metric::dimension)
        .def_property_readonly("coordinate_system", &core::Metric::coordinateSystem)
        .def("components", &metricComponents, "Return the metric components as a nested list of floats.")
        .def("__repr__", [](const core::Metric& metric) {
            return "Metric(dimension=" + std::to_string(metric.dimension()) +
                   ", is_minkowski=" + (metric.isMinkowski() ? std::string("True") : std::string("False")) + ")";
        });

    py::class_<core::ConstitutiveRelations>(m, "ConstitutiveRelations")
        .def(py::init<>())
        .def_readwrite("epsilon", &core::ConstitutiveRelations::epsilon)
        .def_readwrite("mu", &core::ConstitutiveRelations::mu)
        .def_readwrite("xi", &core::ConstitutiveRelations::xi)
        .def_readwrite("zeta", &core::ConstitutiveRelations::zeta)
        .def_static("vacuum", &core::ConstitutiveRelations::vacuum)
        .def("is_vacuum_like", &core::ConstitutiveRelations::isVacuumLike, py::arg("tolerance") = 1e-9)
        .def("__repr__", [](const core::ConstitutiveRelations& c) {
            return std::string("ConstitutiveRelations(vacuum_like=") + (c.isVacuumLike() ? "True" : "False") + ")";
        });

    py::class_<core::MaterialProperties>(m, "MaterialProperties")
        .def(py::init<>())
        .def_readwrite("label", &core::MaterialProperties::label)
        .def_readwrite("bulk_permittivity", &core::MaterialProperties::bulkPermittivity)
        .def_readwrite("bulk_permeability", &core::MaterialProperties::bulkPermeability)
        .def_static("vacuum", &core::MaterialProperties::vacuum);

    py::class_<core::UnitCellDescriptor>(m, "UnitCellDescriptor")
        .def(py::init<>())
        .def_readwrite("lattice_constants", &core::UnitCellDescriptor::latticeConstants)
        .def_readwrite("symmetry_group", &core::UnitCellDescriptor::symmetryGroup);

    py::class_<core::IGeometry, std::shared_ptr<core::IGeometry>>(m, "Geometry")
        .def("describe", &core::IGeometry::describe)
        .def("dimension", &core::IGeometry::dimension)
        .def("unit_cell", &core::IGeometry::unitCell, py::return_value_policy::reference_internal)
        .def("__repr__", [](const core::IGeometry& g) { return "Geometry(" + g.describe() + ")"; });

    py::class_<core::NullGeometry, core::IGeometry, std::shared_ptr<core::NullGeometry>>(m, "NullGeometry")
        .def(py::init<>());

    using MetricResult = core::ReconstructionResult<core::Metric>;
    py::class_<MetricResult::Candidate>(m, "MetricCandidate")
        .def_readonly("value", &MetricResult::Candidate::value)
        .def_readonly("confidence", &MetricResult::Candidate::confidence)
        .def_readonly("assumptions", &MetricResult::Candidate::assumptions);
    py::class_<MetricResult>(m, "MetricReconstructionResult")
        .def("empty", &MetricResult::empty)
        .def("__len__", &MetricResult::size)
        .def("candidates", &MetricResult::candidates)
        .def("notes", &MetricResult::notes)
        .def("best", &MetricResult::best, py::return_value_policy::reference_internal,
             "The highest-confidence candidate. Raises IndexError if there are no candidates.");

    using GeometryResult = core::ReconstructionResult<std::shared_ptr<core::IGeometry>>;
    py::class_<GeometryResult::Candidate>(m, "GeometryCandidate")
        .def_readonly("value", &GeometryResult::Candidate::value)
        .def_readonly("confidence", &GeometryResult::Candidate::confidence)
        .def_readonly("assumptions", &GeometryResult::Candidate::assumptions);
    py::class_<GeometryResult>(m, "GeometryReconstructionResult")
        .def("empty", &GeometryResult::empty)
        .def("__len__", &GeometryResult::size)
        .def("candidates", &GeometryResult::candidates)
        .def("notes", &GeometryResult::notes)
        .def("best", &GeometryResult::best, py::return_value_policy::reference_internal,
             "The highest-confidence candidate. Raises IndexError if there are no candidates.");

    m.def("metric_to_maxwell", &api::metric_to_maxwell, py::arg("metric"),
          py::arg("engine_id") = api::kDefaultEngineId,
          "Metric -> Maxwell: generalized constitutive relations for the given spacetime metric.");
    m.def("maxwell_to_metric", &api::maxwell_to_metric, py::arg("constitutive"),
          py::arg("engine_id") = api::kDefaultEngineId,
          "Maxwell -> Metric: candidate spacetime metrics compatible with the given constitutive relations.");
    m.def("maxwell_to_geometry", &api::maxwell_to_geometry, py::arg("constitutive"),
          py::arg("engine_id") = api::kDefaultEngineId,
          "Maxwell -> Geometry: candidate metamaterial geometries realizing the given constitutive relations.");
    m.def("geometry_to_maxwell", &api::geometry_to_maxwell, py::arg("geometry"),
          py::arg("materials") = core::MaterialProperties::vacuum(), py::arg("engine_id") = api::kDefaultEngineId,
          "Geometry -> Maxwell: effective constitutive relations of an existing geometry (homogenization).");

    m.def(
        "available_engines",
        []() {
            py::dict result;
            result["metric_to_maxwell"] = api::availableMetricToMaxwellEngines();
            result["maxwell_to_metric"] = api::availableMaxwellToMetricEngines();
            result["maxwell_to_geometry"] = api::availableMaxwellToGeometryEngines();
            result["geometry_to_maxwell"] = api::availableGeometryToMaxwellEngines();
            return result;
        },
        "Dict mapping each transformation category to its list of registered engine ids.");
}
