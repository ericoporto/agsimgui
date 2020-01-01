#ifndef IMGUI_IMPL_SOFTRASTER_H
#define IMGUI_IMPL_SOFTRASTER_H

#include "../imgui.h"
#include "../misc/softraster/texture.h"

class IAGSEngine;

void ImGui_ImplSoftraster_InitializeEngine(IAGSEngine* engine);
void ImGui_ImplSoftraster_InitializeScreenAgs(int screenWidth, int screenHeight, int colDepth);
texture_color32_t* ImGui_ImplSoftraster_SpriteIDToTexture(int sprite_id);

int ImGui_ImplSoftraster_GetSprite();

IMGUI_IMPL_API bool     ImGui_ImplSoftraster_Init(texture_base_t* screen);
IMGUI_IMPL_API void     ImGui_ImplSoftraster_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplSoftraster_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplSoftraster_RenderDrawData(ImDrawData* draw_data);

#endif // IMGUI_IMPL_SOFTRASTER_H
