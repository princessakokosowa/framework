cmake_minimum_required(VERSION 3.26)

if (NOT WIN32)
    message(FATAL_ERROR "DirectX Graphics Infrastructure is not available outside of Windows and Windows-based platforms (e.g., Xbox).")
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
    message(FATAL_ERROR "Cannot retrieve either the directory, where Windows 10 SDK is, or what version is installed.")
    message(FATAL_ERROR "-- WIN10_SDK_PATH   : ${WIN10_SDK_PATH}")
    message(FATAL_ERROR "-- WIN10_SDK_VERSION: ${WIN10_SDK_VERSION}")
endif()

# e.g., C:\Program Files (x86)\Windows Kits\10\Lib\${WIN10_SDK_VERSION}\um\x64\dxgi.lib
set(WIN10_SDK_LIBRARY_DIR ${WIN10_SDK_PATH}/Lib/${WIN10_SDK_VERSION})

find_library(
    DXGI_LIBRARY                                 # This actually sets a variable called like shown on the
                                                 # left.
    NAMES               dxgi                     # It means that we are looking for a library called d3d12.
    PATH_SUFFIXES       um/x64                   # Uh, Microsoft's bullshit.
    PATHS               ${WIN10_SDK_LIBRARY_DIR} # This one is pretty clear I suppose.
)

# e.g., C:\Program Files (x86)\Windows Kits\10\Include\${WIN10_SDK_VERSION}\shared\dxgi1_4.h
set(WIN10_SDK_INCLUDE_DIR ${WIN10_SDK_PATH}/Include/${WIN10_SDK_VERSION})

find_path(
    DXGI_INCLUDE_DIR                                 # Same here.
    NAMES                   dxgi1_4.h                # It means that we are looking for a path, where d3d12.h is.
    PATH_SUFFIXES           shared                   # Uh, Microsoft's bullshit.
    PATHS                   ${WIN10_SDK_INCLUDE_DIR} # This one is pretty clear I suppose.
)

message(STATUS "Successfully retrieved DirectX Graphics Infrastructure library and include location!")
message(STATUS "-- DXGI_LIBRARY    : ${DXGI_LIBRARY}")
message(STATUS "-- DXGI_INCLUDE_DIR: ${DXGI_INCLUDE_DIR}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DirectXGI
	DEFAULT_MSG
	DXGI_LIBRARY
	DXGI_INCLUDE_DIR
)

if(DirectXGI_FOUND)   
    add_library(dxgi IMPORTED STATIC)
    set_target_properties(dxgi
        PROPERTIES
        IMPORTED_LOCATION ${DXGI_LIBRARY}
    )
endif()

mark_as_advanced(DXGI_LIBRARY
    DXGI_INCLUDE_DIR
)

if(DirectXGI_FOUND)
	mark_as_advanced(DirectXGI_ROOT)
endif()
