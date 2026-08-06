add_library(metaeorite_core
    src/Version.cpp
    src/Metric.cpp
    src/ConstitutiveRelations.cpp
    src/SymmetryClassifier.cpp
)
add_library(metaeorite::core ALIAS metaeorite_core)

target_include_directories(metaeorite_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(metaeorite_core PUBLIC Eigen xtensor)
# Spglib is an implementation detail of SymmetryClassifier.cpp (its C header
# is not included by any public metaeorite/core/*.hpp), so it is linked
# PRIVATE and does not leak into consumers of metaeorite::core.
target_link_libraries(metaeorite_core PRIVATE Spglib::symspg)
target_compile_features(metaeorite_core PUBLIC cxx_std_20)
