add_library(metaeorite_core
    src/Version.cpp
    src/Metric.cpp
    src/ConstitutiveRelations.cpp
)
add_library(metaeorite::core ALIAS metaeorite_core)

target_include_directories(metaeorite_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(metaeorite_core PUBLIC Eigen xtensor)
target_compile_features(metaeorite_core PUBLIC cxx_std_20)
