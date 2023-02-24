cmake_minimum_required(VERSION 3.26)

if (NOT WIN32)
    message(FATAL_ERROR "DirectX 12 is not available outside of Windows and Windows-based platforms (e.g., Xbox).")
endif()

if ("$ENV{WIN10_SDK_PATH}" STREQUAL "")
    get_filename_component(WIN10_SDK_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" ABSOLUTE CACHE)
else ()
    set(WIN10_SDK_PATH    $ENV{WIN10_SDK_PATH})
endif ()

if ("$ENV{WIN10_SDK_VERSION}" STREQUAL "")
    set(WIN10_SDK_VERSION ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
else ()
    set(WIN10_SDK_VERSION $ENV{WIN10_SDK_VERSION})
endif ()

if ("WIN10_SDK_PATH" STREQUAL "" OR "WIN10_SDK_VERSION" STREQUAL "")
    message(STATUS "Cannot retrieve either the directory, where Windows 10 SDK is, or what version is installed.")
    message(STATUS "-- WIN10_SDK_PATH   : ${WIN10_SDK_PATH}")
    message(STATUS "-- WIN10_SDK_VERSION: ${WIN10_SDK_VERSION}")
endif()

# e.g., C:\Program Files (x86)\Windows Kits\10\Lib\${WIN10_SDK_VERSION}\um\x64\d3d12.lib
set(WIN10_SDK_LIBRARY_DIR ${WIN10_SDK_PATH}/Lib/${WIN10_SDK_VERSION})

find_library(
    DIRECT3D_12_LIBRARY                          # This actually sets a variable called like shown on the
                                                 # left.
    NAMES               d3d12                    # It means that we are looking for a library called d3d12.
    PATH_SUFFIXES       um/x64                   # Uh, Microsoft's bullshit.
    PATHS               ${WIN10_SDK_LIBRARY_DIR} # This one is pretty clear I suppose.
)

# e.g., C:\Program Files (x86)\Windows Kits\10\Include\${WIN10_SDK_VERSION}\um\d3d12.h
#
# Not necessary I suppose, we are delivering these headers together with DirectX 12
# Agility SDK.
#     - princessakokosowa, 24th of February 2023
set(WIN10_SDK_INCLUDE_DIR ${WIN10_SDK_PATH}/Include/${WIN10_SDK_VERSION})

find_path(
    DIRECT3D_12_INCLUDE_DIR                          # Same here.
    NAMES                   d3d12.h                  # It means that we are looking for a path, where d3d12.h is.
    PATH_SUFFIXES           um                       # Uh, Microsoft's bullshit.
    PATHS                   ${WIN10_SDK_INCLUDE_DIR} # This one is pretty clear I suppose.
)

message(STATUS "Successfully retrieved Direct3D 12 library and include location!")
message(STATUS "-- DIRECT3D_12_LIBRARY    : ${DIRECT3D_12_LIBRARY}")
message(STATUS "-- DIRECT3D_12_INCLUDE_DIR: ${DIRECT3D_12_INCLUDE_DIR} -- unused, because of DirectX 12 Agility SDK")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Direct3D12
	DEFAULT_MSG
	DIRECT3D_12_LIBRARY
)

if(Direct3D12_FOUND)   
    add_library(d3d12 IMPORTED STATIC)
    set_target_properties(d3d12
        PROPERTIES
        IMPORTED_LOCATION ${DIRECT3D_12_LIBRARY}
    )
endif()

mark_as_advanced(DIRECT3D_12_LIBRARY)

if(Direct3D12_FOUND)
	mark_as_advanced(Direct3D12_ROOT)
endif()
