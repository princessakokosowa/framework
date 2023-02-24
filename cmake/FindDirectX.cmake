cmake_minimum_required(VERSION 3.24)

if(NOT WIN32)
    message(FATAL_ERROR "Platform not supported")
endif()

include(FindPackageHandleStandardArgs)

set(WIN10_SDK_PATH        "C:/Program Files (x86)/Windows Kits/10" CACHE STRING "Load CMake files (including those responsible for looking for D3D12, etc.).")
set(WIN10_SDK_VERSION     ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
set(WIN10_SDK_LIBRARY_DIR ${WIN10_SDK_PATH}/Lib/${WIN10_SDK_VERSION})

if(NOT IS_DIRECTORY ${WIN10_SDK_LIBRARY_DIR})
    message(FATAL_ERROR "Windows 10 SDK (version: ${WIN10_SDK_VERSION}) is not installed")
endif()

find_library(DIRECT3D_12_LIBRARY 
    NAMES         d3d12 
    PATH_SUFFIXES um/x64 
    PATHS         ${WIN10_SDK_LIBRARY_DIR})

find_library(DIRECTX_GRAPHICS_INFRASTRUCTURE_LIBRARY 
    NAMES         dxgi
    PATH_SUFFIXES um/x64 
    PATHS         ${WIN10_SDK_LIBRARY_DIR})

if(DIRECT3D_12_LIBRARY)
    set(DirectX_D3D12_FOUND ON)
endif()

if(DIRECTX_GRAPHICS_INFRASTRUCTURE_LIBRARY)
    set(DirectX_DXGI_FOUND ON)
endif()

find_package_handle_standard_args(DirectX HANDLE_COMPONENTS)

if(DirectX_D3D12_FOUND)
    mark_as_advanced(DIRECT3D_12_LIBRARY)
endif()

if(DirectX_DXGI_FOUND)
    mark_as_advanced(DIRECTX_GRAPHICS_INFRASTRUCTURE_LIBRARY)
endif()

if(DirectX_D3D12_FOUND AND NOT TARGET d3d12)
    add_library(d3d12 IMPORTED STATIC)
    set_property(TARGET d3d12 PROPERTY IMPORTED_LOCATION ${DIRECT3D_12_LIBRARY})
endif()

if(DirectX_DXGI_FOUND AND NOT TARGET dxgi)
    add_library(dxgi IMPORTED STATIC)
    set_property(TARGET dxgi PROPERTY IMPORTED_LOCATION ${DIRECTX_GRAPHICS_INFRASTRUCTURE_LIBRARY})
endif()
