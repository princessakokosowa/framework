cmake_minimum_required (VERSION 3.26)

# I mean, I get it that there IS actually a difference between a project and a target,
# why though, why?
project(gpu C)

# Set output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/bin)
endforeach(OUTPUT_CONFIG CMAKE_CONFIGURATION_TYPES)

# Fix problems with relative paths.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add all of the source code for the exe itself.
add_subdirectory(src)
