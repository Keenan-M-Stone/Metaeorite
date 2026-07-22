# Fetches third-party, open-source dependencies via CPM.cmake so that no
# manual installation step is required to configure the project.
#
# Eigen and xtensor/xtl are header-only and are fetched with DOWNLOAD_ONLY so
# that we control their (trivial) CMake integration ourselves via hand-rolled
# INTERFACE targets, rather than executing their own CMakeLists.txt (which
# pull in test/doc/benchmark targets we don't need).

CPMAddPackage(
  NAME Eigen3
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 3.4.0
  GIT_SHALLOW TRUE
  DOWNLOAD_ONLY YES
)
if(Eigen3_ADDED)
  add_library(Eigen INTERFACE IMPORTED)
  target_include_directories(Eigen INTERFACE ${Eigen3_SOURCE_DIR})
endif()

CPMAddPackage(
  NAME xtl
  GITHUB_REPOSITORY xtensor-stack/xtl
  GIT_TAG 0.7.7
  GIT_SHALLOW TRUE
  DOWNLOAD_ONLY YES
)
if(xtl_ADDED)
  add_library(xtl INTERFACE IMPORTED)
  target_include_directories(xtl INTERFACE ${xtl_SOURCE_DIR}/include)
endif()

CPMAddPackage(
  NAME xtensor
  GITHUB_REPOSITORY xtensor-stack/xtensor
  GIT_TAG 0.25.0
  GIT_SHALLOW TRUE
  DOWNLOAD_ONLY YES
)
if(xtensor_ADDED)
  add_library(xtensor INTERFACE IMPORTED)
  target_include_directories(xtensor INTERFACE ${xtensor_SOURCE_DIR}/include)
  target_link_libraries(xtensor INTERFACE xtl)
endif()

if(METAEORITE_BUILD_TESTS)
  CPMAddPackage(
    NAME Catch2
    GITHUB_REPOSITORY catchorg/Catch2
    GIT_TAG v3.6.0
    GIT_SHALLOW TRUE
  )
endif()

# pybind11 is fetched with full CMake integration (not DOWNLOAD_ONLY) since
# it provides the pybind11_add_module() helper used by bindings/python.
if(METAEORITE_BUILD_PYTHON)
  CPMAddPackage(
    NAME pybind11
    GITHUB_REPOSITORY pybind/pybind11
    GIT_TAG v2.13.6
    GIT_SHALLOW TRUE
  )
endif()
