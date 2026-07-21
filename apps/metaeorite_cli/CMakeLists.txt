add_executable(metaeorite_cli main.cpp)

target_link_libraries(metaeorite_cli PRIVATE
    metaeorite::core
    metaeorite::pipeline
    metaeorite::metric_to_maxwell
    metaeorite::maxwell_to_metric
    metaeorite::maxwell_to_geometry
    metaeorite::geometry_to_maxwell
)
target_compile_features(metaeorite_cli PRIVATE cxx_std_20)
