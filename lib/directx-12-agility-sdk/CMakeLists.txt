cmake_minimum_required(VERSION 3.26)

project(directx-12-agility-sdk)

include(FetchContent)

FetchContent_Declare(
  DIRECTX_12_AGILITY_SDK
  URL      https://www.nuget.org/api/v2/package/Microsoft.Direct3D.D3D12/1.608.2
  URL_HASH SHA256=aef1069b4a7434e07e62994b059ccbc86df08d1cd9acd63c11e079ae078fe21a
)

FetchContent_MakeAvailable(DIRECTX_12_AGILITY_SDK)

# OK, crazy things are happening here. So, at first I thought that calling
# FetchContent_GetProperties(DIRECTX_12_AGILITY_SDK) would get the properties, but how
# wrong I was, you would not believe it! You actually have to pass the names of the
# specific properties you want to retrieve to this procedure! Very intuitive.
FetchContent_GetProperties(
    DIRECTX_12_AGILITY_SDK
    SOURCE_DIR DIRECTX_12_AGILITY_SDK_SOURCE_DIR
    BINARY_DIR DIRECTX_12_AGILITY_SDK_BINARY_DIR
    POPULATED  DIRECTX_12_AGILITY_SDK_POPULATED
)

if (DIRECTX_12_AGILITY_SDK_POPULATED)
    message(STATUS "Successfully downloaded DirectX 12 Agility SDK!")
    
    # Here, we do not actually care about DIRECTX_12_AGILITY_SDK_BINARY_DIR, because we do
    # not build anything. DIRECTX_12_AGILITY_SDK_SOURCE_DIR is what we are actually looking
    # for.
    set(DIRECTX_12_AGILITY_SDK_SHARED_LIBRARY_DIR    ${DIRECTX_12_AGILITY_SDK_SOURCE_DIR}/build/native/bin/x64       CACHE STRING "The directory that contains DirectX 12 Agility SDK (DLLs).")
    set(DIRECTX_12_AGILITY_SDK_CORE_SHARED_LIBRARY   ${DIRECTX_12_AGILITY_SDK_SHARED_LIBRARY_DIR}/D3D12Core.dll      CACHE STRING "The core DLL provided by the DirectX 12 Agility SDK (should be in DIRECTX_12_AGILITY_SDK_SHARED_LIBRARY_DIR).")
    set(DIRECTX_12_AGILITY_SDK_LAYERS_SHARED_LIBRARY ${DIRECTX_12_AGILITY_SDK_SHARED_LIBRARY_DIR}/d3d12SDKLayers.dll CACHE STRING "The layers DLL provided by the DirectX 12 Agility SDK (should be in DIRECTX_12_AGILITY_SDK_SHARED_LIBRARY_DIR).")
    
    set(DIRECTX_12_AGILITY_SDK_INCLUDE_DIR ${DIRECTX_12_AGILITY_SDK_SOURCE_DIR}/build/native/include CACHE STRING "The directory that contains the include files for DirectX 12 Agility SDK.")
endif()

# message(STATUS "DIRECTX_12_AGILITY_SDK_CORE_SHARED_LIBRARY  : ${DIRECTX_12_AGILITY_SDK_CORE_SHARED_LIBRARY}")
# message(STATUS "DIRECTX_12_AGILITY_SDK_LAYERS_SHARED_LIBRARY: ${DIRECTX_12_AGILITY_SDK_LAYERS_SHARED_LIBRARY}")
# message(STATUS "DIRECTX_12_AGILITY_SDK_INCLUDE_DIR          : ${DIRECTX_12_AGILITY_SDK_INCLUDE_DIR}")

# Specify shared libraries against for target.
list(APPEND SHARED_LIBRARIES ${DIRECTX_12_AGILITY_SDK_CORE_SHARED_LIBRARY})
list(APPEND SHARED_LIBRARIES ${DIRECTX_12_AGILITY_SDK_LAYERS_SHARED_LIBRARY})

# Specify libraries against for target.
list(APPEND LIBRARIES d3d12.lib)
list(APPEND LIBRARIES dxgi.lib)

# Specify include directories for the target.
list(APPEND INCLUDES DIRECTX_12_AGILITY_SDK_INCLUDE_DIR)

# Link shared libraries against the target.
add_library(directx-12-agility-sdk INTERFACE ${SHARED_LIBRARIES})

# Link libraries against the target.
target_link_libraries(directx-12-agility-sdk INTERFACE ${LIBRARIES})

# Bind the sources and the include directories to the target, but... really? ~~There is
# DirectX-Headers project already fetched by Git there, so we pesumably do not really
# want to do that.~~ Yes, really. 
# target_include_directories(directx-12-agility-sdk INTERFACE ${INCLUDES})

# add_library(directx-12-agility-sdk SHARED IMPORTED)
# 
# set_target_properties(directx-12-agility-sdk PROPERTIES
#     IMPORTED_LOCATION ${DIRECTX_12_AGILITY_SDK_CORE_SHARED_LIBRARY}
#     IMPORTED_LOCATION ${DIRECTX_12_AGILITY_SDK_LAYERS_SHARED_LIBRARY}
# 
#     IMPORTED_IMPLIB   d3d12.lib
#     IMPORTED_IMPLIB   dxgi.lib
# )
