add_library(metaeorite_meta_atom
    src/SymmetryClassifier.cpp
)
add_library(metaeorite::meta_atom ALIAS metaeorite_meta_atom)

target_include_directories(metaeorite_meta_atom PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(metaeorite_meta_atom PUBLIC metaeorite::core)
# Spglib is an implementation detail of SymmetryClassifier.cpp (its C header
# is not included by any public metaeorite/meta_atom/*.hpp), so it is linked
# PRIVATE and does not leak into consumers of metaeorite::meta_atom.
target_link_libraries(metaeorite_meta_atom PRIVATE Spglib::symspg)
target_compile_features(metaeorite_meta_atom PUBLIC cxx_std_20)
