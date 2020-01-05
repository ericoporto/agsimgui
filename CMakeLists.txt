cmake_minimum_required(VERSION 3.14)
project(agsimgui VERSION 0.1.0 DESCRIPTION "Dear ImGui plugin for Adventure Game Studio.")

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden -O2 -g -Wall -std=c++11")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # WIN32 is set by CMake for any Windows platform
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOS TRUE)
else()
    message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif ()

include_directories(../ags/Engine/)
include_directories(../ags/Common/)

include_directories(.)

if(NOT MACOS)
    find_package(OpenGL)
endif ()

if (WIN32)
    find_package(DirectX)
elseif (LINUX)
    find_package(X11)
endif()

add_library(agsimgui SHARED
        agsimgui/imgui/imconfig.h
        agsimgui/imgui/imgui.h
        agsimgui/imgui/imgui.cpp
        agsimgui/imgui/imgui_draw.cpp
        agsimgui/imgui/imgui_widgets.cpp
        agsimgui/imgui/misc/cpp/imgui_stdlib.cpp
        agsimgui/imgui/misc/cpp/imgui_stdlib.h
        agsimgui/imgui/misc/softraster/texture.h
        agsimgui/imgui/misc/softraster/softraster.h
        agsimgui/imgui/examples/imgui_impl_softraster.cpp
        agsimgui/imgui/examples/imgui_impl_softraster.h
        agsimgui/imgui/examples/imgui_impl_dx9.cpp
        agsimgui/imgui/examples/imgui_impl_dx9.h
        agsimgui/agsimgui.h
        agsimgui/agsimgui.cpp
        agsimgui/Screen.cpp
        agsimgui/Screen.h)

target_link_libraries(agsimgui m)
