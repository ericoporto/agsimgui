// dear imgui: Renderer for DirectX9
// This needs to be used along with a Platform Binding (e.g. Win32)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'LPDIRECT3DTEXTURE9' as ImTextureID. Read the FAQ about ImTextureID!
//  [X] Renderer: Support for large meshes (64k+ vertices) with 16-bit indices.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#pragma once

#ifndef AGSIMGUI_RENDER_DX9_H
#define AGSIMGUI_RENDER_DX9_H

#include "core/platform.h"

struct IDirect3DDevice9;


class IAGSEngine;
struct IDirect3DTexture9;

void ImGui_ImplDX9_InitializeEngine(IAGSEngine* engine);

#if AGS_PLATFORM_OS_WINDOWS
IMGUI_IMPL_API bool     ImGui_ImplDX9_Init(IDirect3DDevice9* device);
IDirect3DTexture9* ImGui_ImplDX9_SpriteIDToTexture(int sprite_id);
#else
IMGUI_IMPL_API bool     ImGui_ImplDX9_Init(void* device);
void* ImGui_ImplDX9_SpriteIDToTexture(int sprite_id);
#endif

IMGUI_IMPL_API void     ImGui_ImplDX9_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplDX9_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplDX9_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_IMPL_API bool     ImGui_ImplDX9_CreateDeviceObjects();
IMGUI_IMPL_API void     ImGui_ImplDX9_InvalidateDeviceObjects();

#endif //AGSIMGUI_RENDER_DX9_H
