cmake_minimum_required(VERSION 3.26)

# Standard bullshit that a programmer has to deal with every time they set up a build
# system, why do I even have to do this?
set(TARGET gpu)
add_executable(${TARGET})

# Specify the working directory for the MSVC debugger (and probably any other debugger
# that runs on Windows, e.g., RemedyBG, which I use on a daily basis).
set_property(TARGET ${TARGET} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Specify the required C standard.
set_target_properties(${TARGET} PROPERTIES C_STANDARD                17)
set_target_properties(${TARGET} PROPERTIES CMAKE_C_STANDARD_REQUIRED True)

# Specify libraries for the target (and since we are not linking ANYTHING yet, we cannot
# do anything like what is shown below, because there is absolutely not symmetry between
# similarly looking commands in CMake). For futher reference, I did this -
# `list(APPEND LIBRARIES .)`.
list(APPEND LIBRARIES directx-12-agility-sdk)
list(APPEND LIBRARIES directx-shader-compiler)

# Specify sources and include directories for the target.
list(APPEND SOURCES   main.c)
list(APPEND INCLUDES  .)

# Link libraries against the target.
target_link_libraries(${TARGET} ${LIBRARIES})

# Bind the sources and the include directories to the target.
target_sources(${TARGET} PRIVATE ${SOURCES})
target_include_directories(${TARGET} PRIVATE ${INCLUDES})

# Find a better way to do this e.g., skip if the files are already there.
add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND if $<CONFIG:Debug> neq 0 ("${CMAKE_COMMAND}" -E copy_directory
        "${DIRECTX_12_AGILITY_SDK_SHARED_LIBRARY_DIR}"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

    COMMAND if $<CONFIG:Release> neq 0 ("${CMAKE_COMMAND}" -E copy_directory
        "${DIRECTX_12_AGILITY_SDK_SHARED_LIBRARY_DIR}"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

	COMMENT "Copying DirectX 12 Agility SDK binaries..."
)

add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND if $<CONFIG:Debug> neq 0 ("${CMAKE_COMMAND}" -E copy_directory
        "${DIRECTX_SHADER_COMPILER_SHARED_LIBRARY_DIR}"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

    COMMAND if $<CONFIG:Release> neq 0 ("${CMAKE_COMMAND}" -E copy_directory
        "${DIRECTX_SHADER_COMPILER_SHARED_LIBRARY_DIR}"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

	COMMENT "Copying DirectX Shader Compiler binaries..."
)
