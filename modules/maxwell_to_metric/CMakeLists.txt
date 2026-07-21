add_library(metaeorite_maxwell_to_metric
    src/PlaceholderEngine.cpp
    src/Registration.cpp
)
add_library(metaeorite::maxwell_to_metric ALIAS metaeorite_maxwell_to_metric)

target_include_directories(metaeorite_maxwell_to_metric PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(metaeorite_maxwell_to_metric PUBLIC metaeorite::core)
target_compile_features(metaeorite_maxwell_to_metric PUBLIC cxx_std_20)
