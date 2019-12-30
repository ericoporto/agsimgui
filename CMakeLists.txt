cmake_minimum_required(VERSION 3.14)
project(agsimgui VERSION 0.1.0 DESCRIPTION "Dear ImGui plugin for Adventure Game Studio.")

set(CMAKE_CXX_STANDARD 14)

include_directories(../ags/Engine/)
include_directories(../ags/Common/)

include_directories(.)

add_library(agsimgui SHARED
        agsimgui/imgui/imconfig.h
        agsimgui/imgui/imgui.h
        agsimgui/imgui/imgui.cpp
        agsimgui/imgui/imgui_draw.cpp
        agsimgui/imgui/imgui_widgets.cpp
        agsimgui/imgui/misc/softraster/texture.h
        agsimgui/imgui/misc/softraster/softraster.h
        agsimgui/imgui/examples/imgui_impl_softraster.cpp
        agsimgui/imgui/examples/imgui_impl_softraster.h
        agsimgui/agsimgui.h
        agsimgui/agsimgui.cpp)