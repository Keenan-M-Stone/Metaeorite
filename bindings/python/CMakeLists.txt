find_package(Python3 COMPONENTS Interpreter Development.Module)

if(NOT Python3_FOUND)
  message(WARNING "Metaeorite: Python3 development headers not found - skipping Python bindings. "
                   "Install a Python development package (e.g. python3-dev) or configure with "
                   "-DMETAEORITE_BUILD_PYTHON=OFF to silence this warning.")
  return()
endif()

# Build the extension with the ABI tag of the selected Python3 interpreter
# (passed from the top-level configure via -DPython3_EXECUTABLE=...), so the
# generated filename is importable by that exact runtime.
Python3_add_library(_metaeorite_native MODULE WITH_SOABI src/Bindings.cpp)
target_link_libraries(_metaeorite_native PRIVATE
  Python3::Module
  pybind11::headers
  metaeorite::api
  metaeorite::core
  metaeorite::meta_atom
)
target_compile_features(_metaeorite_native PRIVATE cxx_std_20)

# Land the compiled extension right next to the pure-Python package so that
# `import metaeorite` works directly out of a build tree - no separate
# install/packaging step needed for local testing or Jupyter notebook demos
# (see examples/python). Add this directory to PYTHONPATH/sys.path to use it.
set_target_properties(_metaeorite_native PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/metaeorite"
)
