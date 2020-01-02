cmake_minimum_required(VERSION 3.14)
project(agsimgui VERSION 0.1.0 DESCRIPTION "Dear ImGui plugin for Adventure Game Studio.")

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden -O2 -g -Wall -std=c++11")

include_directories(../ags/Engine/)
include_directories(../ags/Common/)

include_directories(.)

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
        agsimgui/agsimgui.h
        agsimgui/agsimgui.cpp)

target_link_libraries(agsimgui m)
