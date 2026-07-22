cmake_minimum_required(VERSION 3.21)

project(Metaeorite
    VERSION 0.1.0
    DESCRIPTION "A modular C++20 toolkit for translating between spacetime metrics, generalized constitutive relations, and realizable metamaterial geometries."
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
endif()

option(METAEORITE_BUILD_TESTS "Build unit tests" ON)
option(METAEORITE_BUILD_APPS "Build example/demo applications" ON)
option(METAEORITE_BUILD_PYTHON "Build Python bindings (pybind11)" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cmake/CPM.cmake)
include(cmake/Dependencies.cmake)

# --- Transformation engine modules --------------------------------------
# Each module below implements exactly one edge of the architecture diagram
# in README.md and depends only on `core` (never on a sibling module),
# communicating solely through core::ConstitutiveRelations / core::Metric /
# core::IGeometry. This keeps every module independently buildable,
# testable, and replaceable.
add_subdirectory(modules/core)
add_subdirectory(modules/metric_to_maxwell)
add_subdirectory(modules/maxwell_to_metric)
add_subdirectory(modules/maxwell_to_geometry)
add_subdirectory(modules/geometry_to_maxwell)

# Orchestrator that wires the engines together purely via their abstract
# interfaces (dependency inversion), mirroring the architecture diagram.
add_subdirectory(modules/pipeline)

# Function-based facade over the four engines (metric_to_maxwell,
# maxwell_to_metric, maxwell_to_geometry, geometry_to_maxwell) for external
# C++ consumers and the Python bindings below.
add_subdirectory(modules/api)

if(METAEORITE_BUILD_APPS)
  add_subdirectory(apps/metaeorite_cli)
endif()

if(METAEORITE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(METAEORITE_BUILD_PYTHON)
  add_subdirectory(bindings/python)
endif()
