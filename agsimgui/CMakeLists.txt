project(agsimgui CXX)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "../README.md"
    "imgui/misc/natvis/imgui.natvis"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "agsimgui.h"
    "imgui/examples/imgui_impl_softraster.h"
    "imgui/imconfig.h"
    "imgui/imgui.h"
    "imgui/misc/cpp/imgui_stdlib.h"
    "imgui/misc/softraster/softraster.h"
    "imgui/misc/softraster/texture.h"
    "Screen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Header_Files__BackendDx9
    "imgui/examples/imgui_impl_dx9.h"
)
source_group("Header Files\\BackendDx9" FILES ${Header_Files__BackendDx9})

set(Source_Files
    "agsimgui.cpp"
    "imgui/examples/imgui_impl_softraster.cpp"
    "imgui/imgui.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/misc/cpp/imgui_stdlib.cpp"
    "Screen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Source_Files__BackendDx9
    "imgui/examples/imgui_impl_dx9.cpp"
)
source_group("Source Files\\BackendDx9" FILES ${Source_Files__BackendDx9})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Header_Files__BackendDx9}
    ${Source_Files}
    ${Source_Files__BackendDx9}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE agsimgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$ENV{IncludePath};"
        "${CMAKE_CURRENT_SOURCE_DIR}/home/erico/git/ags/Common;"
        "${CMAKE_CURRENT_SOURCE_DIR}/home/erico/git/ags/Engine;"
        "$ENV{DXSDK_DIR}Include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$ENV{IncludePath};"
        "${CMAKE_CURRENT_SOURCE_DIR}/home/erico/git/ags/Common;"
        "${CMAKE_CURRENT_SOURCE_DIR}/home/erico/git/ags/Engine;"
        "$ENV{DXSDK_DIR}Include"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "AGSIMGUI_EXPORTS;"
        "_WINDOWS;"
        "_USRDLL;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "AGSIMGUI_EXPORTS;"
        "_WINDOWS;"
        "_USRDLL;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /MTd
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /MT;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /MTd
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /MT;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:WINDOWS
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:WINDOWS
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "d3d9;"
        "d3dx9"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "d3d9;"
        "d3dx9"
    )
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "$ENV{DXSDK_DIR}/Lib/x64"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "$ENV{DXSDK_DIR}/Lib/x86"
    )
endif()

