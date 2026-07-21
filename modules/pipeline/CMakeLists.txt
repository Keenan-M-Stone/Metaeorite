add_library(metaeorite_pipeline
    src/Pipeline.cpp
)
add_library(metaeorite::pipeline ALIAS metaeorite_pipeline)

target_include_directories(metaeorite_pipeline PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(metaeorite_pipeline PUBLIC
    metaeorite::core
    metaeorite::metric_to_maxwell
    metaeorite::maxwell_to_metric
    metaeorite::maxwell_to_geometry
    metaeorite::geometry_to_maxwell
)
target_compile_features(metaeorite_pipeline PUBLIC cxx_std_20)
