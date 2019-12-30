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

texture_base_t* Screen = nullptr;
IAGSEngine* _Engine;
int _ScreenWidth;
int _ScreenHeight;
int _SpriteId;
uint32_t **_Longbuffer;

void _DrawPixel(uint32_t **longbufferBitmap, int x, int y, int agsColor, int width, int height) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        longbufferBitmap[y][x] = agsColor;
    }
}

void CopyScreenToSprite(texture_base_t* screen, uint32_t **longbufferBitmap, int width, int height){
    for(int ix=0; ix<width; ix++) {
        for (int iy = 0; iy < height; iy++) {
            _DrawPixel(longbufferBitmap, ix, iy,
                    (reinterpret_cast<texture_color32_t*>(Screen))->at(ix,iy).RGBA32(),
                    width, height);
        }
    }
}


void ImGui_ImplSoftraster_InitializeScreenAgs(IAGSEngine* engine, int screenWidth, int screenHeight, int colDepth) {
    _Engine = engine;
    _ScreenWidth = screenWidth;
    _ScreenHeight = screenHeight;
}

int ImGui_ImplSoftraster_GetSprite() {
    if(_Engine == nullptr || _SpriteId <= 0) return 0;

    _Engine->NotifySpriteUpdated(_SpriteId);
    return _SpriteId;
}

void ImGui_ImplSoftraster_GetSurface() {
    if (_Engine == nullptr) return;
    if (_SpriteId <= 0)  _SpriteId = _Engine->CreateDynamicSprite(32, _ScreenWidth, _ScreenHeight);

    BITMAP *engineSprite = _Engine->GetSpriteGraphic(_SpriteId);
    unsigned char **charbuffer = _Engine->GetRawBitmapSurface(engineSprite);
    _Longbuffer = (uint32_t**)charbuffer;
}

void ImGui_ImplSoftraster_ReleaseSurface() {
    if (_Engine == nullptr || _SpriteId <= 0) return;

    BITMAP *engineSprite = _Engine->GetSpriteGraphic(_SpriteId);
    _Engine->ReleaseBitmapSurface(engineSprite);
    _Engine->NotifySpriteUpdated(_SpriteId);
}


void ImGui_ImplSoftraster_ClearSprite() {
    if (_Engine == nullptr || _SpriteId <= 0) return;

    BITMAP *engineSprite = _Engine->GetSpriteGraphic(_SpriteId);
    unsigned char **charbuffer = _Engine->GetRawBitmapSurface(engineSprite);
    uint32_t **longbuffer = (uint32_t**)charbuffer;

    uint32_t clearColor = makeacol32(0, 0, 0, 255);

    int srcWidth, srcHeight;
    _Engine->GetBitmapDimensions(engineSprite, &srcWidth, &srcHeight, nullptr);

    for (int y = 0; y < srcHeight; y++) {
        for (int x = 0; x < srcWidth; x++) {
            _DrawPixel(longbuffer, x, y, clearColor, srcWidth, srcHeight);
        }
    }

    _Engine->ReleaseBitmapSurface(engineSprite);
}





bool ImGui_ImplSoftraster_Init(texture_base_t* screen)
{
    if (screen != nullptr)
    {
        Screen = screen;
        return true;
    }
    return false;
}

void ImGui_ImplSoftraster_Shutdown()
{
    Screen = nullptr;
}

void ImGui_ImplSoftraster_NewFrame()
{
    if (Screen == nullptr) return;

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = Screen->w;
    io.DisplaySize.y = Screen->h;
}



void ImGui_ImplSoftraster_RenderDrawData(ImDrawData* draw_data)
{
    if (Screen == nullptr) return;

    Screen->clear();

    switch (Screen->type)
    {
    case texture_type_t::ALPHA8:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_alpha8_t*>(Screen));
        break;

    case texture_type_t::VALUE8:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_value8_t*>(Screen));
        break;

    case texture_type_t::COLOR16:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_color16_t*>(Screen));
        break;

    case texture_type_t::COLOR24:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_color24_t*>(Screen));
        break;

    case texture_type_t::COLOR32:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_color32_t*>(Screen));
        break;

    default: return;
    }

    ImGui_ImplSoftraster_GetSurface();
    CopyScreenToSprite(Screen,_Longbuffer,Screen->w,Screen->h);
    ImGui_ImplSoftraster_ReleaseSurface();
}