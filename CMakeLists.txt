cmake_minimum_required (VERSION 3.26)

# I mean, I get it that there IS actually a difference between a project and a target,
# why though, why?
project(gpu C)

# Set output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach(OUTPUT_CONFIG CMAKE_CONFIGURATION_TYPES)

# Fix problems with relative paths.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add all of the libraries.
add_subdirectory(lib/DirectX-Headers)

# Obviously, Microsoft does not care about providing you with the information that you
# have to configure targets yourself (apparently, they also assume that you know that
# they use LLVM as a backend for their shader compiler).
#
# Here is the list of the targets that you might want to target:
# * AArch64
# * AMDGPU
# * ARM
# * AVR
# * BPF
# * Hexagon
# * Lanai
# * Mips
# * MSP430
# * NVPTX
# * PowerPC
# * RISCV
# * Sparc
# * SystemZ
# * WebAssembly
# * X86
# * XCore
#
# We will probably want to target AMDGPU, NVPTX.
#     - princessakokosowa, 23rd of February 2023 @ 13:12
#
# Actually, no, that does not seem to be a thing, because in 
# DirectXShaderCompiler\CMakeLists.txt@241 they do the following thing:
#
#     set(LLVM_ALL_TARGETS
#      AMDGPU
#      NVPTX
#     )
#
# Then in DirectXShaderCompiler\CMakeLists.txt@287:
#
#     if( LLVM_TARGETS_TO_BUILD STREQUAL "all" )
#       set( LLVM_TARGETS_TO_BUILD ${LLVM_ALL_TARGETS} )
#     endif()
#
# So what the hell is happening here actually?
#     - princessakokosowa, 23rd of February 2023 @ 13:41
#
# OK, I decoded to let it pass, there is no hope for project developed by Microsoft.
#     - princessakokosowa, 23rd of February 2023 @ 14:48
#
# add_subdirectory(lib/DirectXShaderCompiler)

# Add all of the source code for the exe itself.
add_subdirectory(src)
