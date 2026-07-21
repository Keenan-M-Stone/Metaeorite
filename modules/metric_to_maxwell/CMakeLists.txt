add_library(metaeorite_metric_to_maxwell
    src/PlaceholderEngine.cpp
    src/Registration.cpp
)
add_library(metaeorite::metric_to_maxwell ALIAS metaeorite_metric_to_maxwell)

target_include_directories(metaeorite_metric_to_maxwell PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(metaeorite_metric_to_maxwell PUBLIC metaeorite::core)
target_compile_features(metaeorite_metric_to_maxwell PUBLIC cxx_std_20)
