#include <cstdio>
#include <vector>
#include "imgui_impl_softraster.h"
#include "../misc/softraster/softraster.h"
#include "plugin/agsplugin.h"

#if !AGS_PLATFORM_OS_WINDOWS
#define min(x,y) (((x) < (y)) ? (x) : (y))
#define max(x,y) (((x) > (y)) ? (x) : (y))
#endif

#pragma region Color_Functions

#define DEFAULT_RGB_R_SHIFT_32  16
#define DEFAULT_RGB_G_SHIFT_32  8
#define DEFAULT_RGB_B_SHIFT_32  0
#define DEFAULT_RGB_A_SHIFT_32  24

uint32_t getr32(uint32_t c)
{
    return ((c >> DEFAULT_RGB_R_SHIFT_32) & 0xFF);
}


uint32_t getg32(uint32_t c)
{
    return ((c >> DEFAULT_RGB_G_SHIFT_32) & 0xFF);
}


uint32_t getb32(uint32_t c)
{
    return ((c >> DEFAULT_RGB_B_SHIFT_32) & 0xFF);
}


uint32_t geta32(uint32_t c)
{
    return ((c >> DEFAULT_RGB_A_SHIFT_32) & 0xFF);
}


uint32_t makeacol32(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    return ((r << DEFAULT_RGB_R_SHIFT_32) |
            (g << DEFAULT_RGB_G_SHIFT_32) |
            (b << DEFAULT_RGB_B_SHIFT_32) |
            (a << DEFAULT_RGB_A_SHIFT_32));
}

#pragma endregion

#pragma region Pixel32_Definition

struct Pixel32 {

public:
    Pixel32(uint32_t r = 0, uint32_t g = 0, uint32_t b = 0, uint32_t alpha = 0);
    ~Pixel32() = default;
    uint32_t GetColorAsInt();
    uint32_t Red;
    uint32_t Green;
    uint32_t Blue;
    uint32_t Alpha;

};

Pixel32::Pixel32(uint32_t r, uint32_t g, uint32_t b, uint32_t alpha) {
    Red = r;
    Blue = b;
    Green = g;
    Alpha = alpha;
}

uint32_t Pixel32::GetColorAsInt() {
    return makeacol32(Red, Green, Blue, Alpha);
}

#pragma endregion

typedef void* (*SCAPI_DYNAMICSPRITE_CREATE) (int width, int height, int alphaChannel);
typedef int (*SCAPI_DYNAMICSPRITE_GETGRAPHIC) (void*);
SCAPI_DYNAMICSPRITE_CREATE DynamicSprite_Create = NULL;
SCAPI_DYNAMICSPRITE_GETGRAPHIC DynamicSprite_GetGraphic = NULL;
std::vector<texture_color32_t* > image_texture_stack;


texture_base_t* Screen_soft = nullptr;
IAGSEngine* _Engine_soft = nullptr;
int _ScreenWidth_soft = 0;
int _ScreenHeight_soft = 0;
int _SpriteId_soft = 0;
uint32_t **_Longbuffer_soft = nullptr;

void _DrawPixel(uint32_t **longbufferBitmap, int x, int y, int agsColor, int width, int height) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        longbufferBitmap[y][x] = agsColor;
    }
}

void CopyScreenToSprite(texture_base_t* screen, uint32_t **longbufferBitmap, int width, int height){
    for(int ix=0; ix<width; ix++) {
        for (int iy = 0; iy < height; iy++) {
            longbufferBitmap[iy][ix] = (uint32_t) (reinterpret_cast<texture_color32_t*>(screen))->at(ix,iy).ARGB32();
        }
    }
}

texture_color32_t* ImGui_ImplSoftraster_SpriteIDToTexture(int sprite_id){
    texture_color32_t* image_texture = new texture_color32_t();

    BITMAP *engineSprite = _Engine_soft->GetSpriteGraphic(sprite_id);
    int sprite_width = _Engine_soft->GetSpriteWidth(sprite_id);
    int sprite_height = _Engine_soft->GetSpriteHeight(sprite_id);

    image_texture->init(sprite_width,sprite_height);

    unsigned char **charbuffer = _Engine_soft->GetRawBitmapSurface(engineSprite);
    uint32_t **longbuffer = (uint32_t**)charbuffer;

    for(int ix=0; ix<sprite_width; ix++) {
        for (int iy = 0; iy < sprite_height; iy++) {

            image_texture->at(ix,iy).r = getr32(longbuffer[iy][ix]);
            image_texture->at(ix,iy).g = getg32(longbuffer[iy][ix]);
            image_texture->at(ix,iy).b = getb32(longbuffer[iy][ix]);
            image_texture->at(ix,iy).a = geta32(longbuffer[iy][ix]);

        }
    }

    image_texture_stack.push_back(image_texture);
    _Engine_soft->ReleaseBitmapSurface(engineSprite);
    return image_texture;
}

void ImGui_ImplSoftraster_InitializeEngine(IAGSEngine* engine) {
    _Engine_soft = engine;
    DynamicSprite_Create = (SCAPI_DYNAMICSPRITE_CREATE) engine->GetScriptFunctionAddress("DynamicSprite::Create^3");
    DynamicSprite_GetGraphic = (SCAPI_DYNAMICSPRITE_GETGRAPHIC) engine->GetScriptFunctionAddress("DynamicSprite::get_Graphic");
}

void ImGui_ImplSoftraster_InitializeScreenAgs(int screenWidth, int screenHeight, int colDepth) {
    _ScreenWidth_soft = screenWidth;
    _ScreenHeight_soft = screenHeight;
}

int ImGui_ImplSoftraster_GetSprite() {
    if(_Engine_soft == nullptr || _SpriteId_soft <= 0) return 0;

    _Engine_soft->NotifySpriteUpdated(_SpriteId_soft);
    return _SpriteId_soft;
}

void ImGui_ImplSoftraster_GetSurface() {
    if (_Engine_soft == nullptr) return;
    if (_SpriteId_soft <= 0) {
        _SpriteId_soft = _Engine_soft->CreateDynamicSprite(32,_ScreenWidth_soft,_ScreenHeight_soft);
        _Engine_soft->SetSpriteAlphaBlended(_SpriteId_soft, 1);
        printf("\nCreateDynamicSprite\n");
    }

    BITMAP *engineSprite = _Engine_soft->GetSpriteGraphic(_SpriteId_soft);
    unsigned char **charbuffer = _Engine_soft->GetRawBitmapSurface(engineSprite);
    _Longbuffer_soft = (uint32_t**)charbuffer;
}

void ImGui_ImplSoftraster_ReleaseSurface() {
    if (_Engine_soft == nullptr || _SpriteId_soft <= 0) return;

    BITMAP *engineSprite = _Engine_soft->GetSpriteGraphic(_SpriteId_soft);
    _Engine_soft->ReleaseBitmapSurface(engineSprite);
    _Engine_soft->NotifySpriteUpdated(_SpriteId_soft);
}


void ImGui_ImplSoftraster_ClearSprite() {
    if (_Engine_soft == nullptr || _SpriteId_soft <= 0) return;

    BITMAP *engineSprite = _Engine_soft->GetSpriteGraphic(_SpriteId_soft);
    unsigned char **charbuffer = _Engine_soft->GetRawBitmapSurface(engineSprite);
    uint32_t **longbuffer = (uint32_t**)charbuffer;

    uint32_t clearColor = makeacol32(0, 0, 0, 255);

    int srcWidth, srcHeight;
    _Engine_soft->GetBitmapDimensions(engineSprite, &srcWidth, &srcHeight, nullptr);

    for (int y = 0; y < srcHeight; y++) {
        for (int x = 0; x < srcWidth; x++) {
            _DrawPixel(longbuffer, x, y, clearColor, srcWidth, srcHeight);
        }
    }

    _Engine_soft->ReleaseBitmapSurface(engineSprite);
}





bool ImGui_ImplSoftraster_Init(texture_base_t* screen)
{
    if (screen != nullptr)
    {
        Screen_soft = screen;
        return true;
    }
    return false;
}

void ImGui_ImplSoftraster_Shutdown()
{
    Screen_soft = nullptr;
}

void ImGui_ImplSoftraster_NewFrame()
{
    if (Screen_soft == nullptr) return;

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = Screen_soft->w;
    io.DisplaySize.y = Screen_soft->h;
}



void ImGui_ImplSoftraster_RenderDrawData(ImDrawData* draw_data)
{
    if (Screen_soft == nullptr) return;
    if (draw_data == nullptr) return;

    Screen_soft->clear();

    switch (Screen_soft->type)
    {
    case texture_type_t::ALPHA8:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_alpha8_t*>(Screen_soft));
        break;

    case texture_type_t::VALUE8:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_value8_t*>(Screen_soft));
        break;

    case texture_type_t::COLOR16:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_color16_t*>(Screen_soft));
        break;

    case texture_type_t::COLOR24:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_color24_t*>(Screen_soft));
        break;

    case texture_type_t::COLOR32:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_color32_t*>(Screen_soft));
        break;

    default: return;
    }

    ImGui_ImplSoftraster_GetSurface();
    CopyScreenToSprite(Screen_soft,_Longbuffer_soft,Screen_soft->w,Screen_soft->h);
    ImGui_ImplSoftraster_ReleaseSurface();

    while (!image_texture_stack.empty()) {
        texture_color32_t* image_texture = image_texture_stack.back();
        image_texture->empty();
        delete image_texture;
        image_texture_stack.pop_back();
    }
}
