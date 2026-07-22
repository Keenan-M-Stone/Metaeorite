add_executable(metaeorite_tests
    core/TestCoreTypes.cpp
    core/TestRegistry.cpp
    engines/TestPlaceholderEngines.cpp
    pipeline/TestPipeline.cpp
    api/TestApiFunctions.cpp
)

target_link_libraries(metaeorite_tests PRIVATE
    Catch2::Catch2WithMain
    metaeorite::core
    metaeorite::pipeline
    metaeorite::api
    metaeorite::metric_to_maxwell
    metaeorite::maxwell_to_metric
    metaeorite::maxwell_to_geometry
    metaeorite::geometry_to_maxwell
)
target_compile_features(metaeorite_tests PRIVATE cxx_std_20)

list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")
include(CTest)
include(Catch)
catch_discover_tests(metaeorite_tests)
