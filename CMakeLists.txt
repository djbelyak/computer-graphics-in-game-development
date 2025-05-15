cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("Computer graphics in Game development")

set(INCLUDE
        libs/stb
        libs/tinyobjloader
        libs/linalg
        libs/cxxopts/include
        libs/D3DX12
        src)

set(SOURCE
        src/settings.cpp
        src/renderer/renderer.cpp
        src/world/camera.cpp
        src/world/model.cpp
        src/utils/resource_utils.cpp)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(Rasterization src/main.cpp src/renderer/rasterizer/rasterizer_renderer.cpp ${SOURCE})
target_compile_definitions(Rasterization PUBLIC RASTERIZATION)
target_include_directories(Rasterization PRIVATE ${INCLUDE})
set_property(TARGET Rasterization PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

find_package(OpenMP REQUIRED)
add_executable(Raytracing src/main.cpp src/renderer/raytracer/raytracer_renderer.cpp ${SOURCE})
target_compile_definitions(Raytracing PUBLIC RAYTRACING)
target_include_directories(Raytracing PRIVATE ${INCLUDE})
target_link_libraries(Raytracing PRIVATE OpenMP::OpenMP_CXX)
set_property(TARGET Raytracing PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_executable(DirectX12 WIN32 src/win_main.cpp src/renderer/dx12/dx12_renderer.cpp src/utils/window.cpp ${SOURCE})
target_compile_definitions(DirectX12 PUBLIC DX12 WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS _UNICODE UNICODE)
target_include_directories(DirectX12 PRIVATE ${INCLUDE})
target_link_libraries(DirectX12 d3d12.lib dxgi.lib d3dcompiler.lib dxguid.lib)
set_property(TARGET DirectX12 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
