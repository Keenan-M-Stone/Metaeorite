add_library(metaeorite_api
    src/Functions.cpp
)
add_library(metaeorite::api ALIAS metaeorite_api)

target_include_directories(metaeorite_api PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(metaeorite_api PUBLIC
    metaeorite::core
    metaeorite::metric_to_maxwell
    metaeorite::maxwell_to_metric
    metaeorite::maxwell_to_geometry
    metaeorite::geometry_to_maxwell
)
target_compile_features(metaeorite_api PUBLIC cxx_std_20)
