// dllmain.cpp : Defines the entry point for the DLL application.

#pragma region Defines_and_Includes

#include "core/platform.h"

#define MIN_EDITOR_VERSION 1
#define MIN_ENGINE_VERSION 3

#if AGS_PLATFORM_OS_WINDOWS

struct IUnknown; // Workaround for "combaseapi.h(229): error C2187: syntax error: 'identifier' was unexpected here" when using /permissive-s
#include <d3d9.h>
#include <d3dx9.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if !defined(BUILTIN_PLUGINS)
#define THIS_IS_THE_PLUGIN
#endif

#include "ac/keycode.h"
#include "plugin/agsplugin.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "imgui/imgui.h" 
#include "imgui/misc/softraster/softraster.h"
#include "imgui/examples/imgui_impl_softraster.h"
#include "imgui/examples/imgui_impl_dx9.h"

#include "imgui/misc/cpp/imgui_stdlib.h"

#include "agsimgui.h"
#include "Screen.h"
#include "libs/clip/clip.h"

#include "AgsImVec2.h"
#include "AgsImVec4.h"
#include "AgsImGuiStyle.h"

#include <cstring>

#if defined(BUILTIN_PLUGINS)
namespace agsimgui {
#endif

	typedef unsigned char uint8;

#define DEFAULT_RGB_R_SHIFT_32  16
#define DEFAULT_RGB_G_SHIFT_32  8
#define DEFAULT_RGB_B_SHIFT_32  0
#define DEFAULT_RGB_A_SHIFT_32  24

#if !AGS_PLATFORM_OS_WINDOWS
#define min(x,y) (((x) < (y)) ? (x) : (y))
#define max(x,y) (((x) > (y)) ? (x) : (y))
#endif

#define abs(a)             ((a)<0 ? -(a) : (a))

#pragma endregion //Defines_and_Includes

#if AGS_PLATFORM_OS_WINDOWS
	// The standard Windows DLL entry point

	BOOL APIENTRY DllMain(HANDLE hModule,
		DWORD ul_reason_for_call,
		LPVOID lpReserved) {

		switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		}
		return TRUE;
	}
#endif

	//define engine
	IAGSEngine *engine;

#pragma region Color_Functions

	int getr32(int c)
	{
		return ((c >> DEFAULT_RGB_R_SHIFT_32) & 0xFF);
	}


	int getg32(int c)
	{
		return ((c >> DEFAULT_RGB_G_SHIFT_32) & 0xFF);
	}


	int getb32(int c)
	{
		return ((c >> DEFAULT_RGB_B_SHIFT_32) & 0xFF);
	}


	int geta32(int c)
	{
		return ((c >> DEFAULT_RGB_A_SHIFT_32) & 0xFF);
	}


	int makeacol32(int r, int g, int b, int a)
	{
		return ((r << DEFAULT_RGB_R_SHIFT_32) |
			(g << DEFAULT_RGB_G_SHIFT_32) |
			(b << DEFAULT_RGB_B_SHIFT_32) |
			(a << DEFAULT_RGB_A_SHIFT_32));
	}

#pragma endregion



	/// <summary>
	/// Gets the alpha value at coords x,y
	/// </summary>
	int GetAlpha(int sprite, int x, int y) {

		BITMAP *engineSprite = engine->GetSpriteGraphic(sprite);

		unsigned char **charbuffer = engine->GetRawBitmapSurface(engineSprite);
		unsigned int **longbuffer = (unsigned int**)charbuffer;

		int alpha = geta32(longbuffer[y][x]);

		engine->ReleaseBitmapSurface(engineSprite);

		return alpha;
	}

	/// <summary>
	/// Sets the alpha value at coords x,y
	/// </summary>
	int PutAlpha(int sprite, int x, int y, int alpha) {

		BITMAP *engineSprite = engine->GetSpriteGraphic(sprite);

		unsigned char **charbuffer = engine->GetRawBitmapSurface(engineSprite);
		unsigned int **longbuffer = (unsigned int**)charbuffer;


		int r = getr32(longbuffer[y][x]);
		int g = getg32(longbuffer[y][x]);
		int b = getb32(longbuffer[y][x]);
		longbuffer[y][x] = makeacol32(r, g, b, alpha);

		engine->ReleaseBitmapSurface(engineSprite);

		return alpha;
	}


	/// <summary>
	///  Translates index from a 2D array to a 1D array
	/// </summary>
	int xytolocale(int x, int y, int width) {
		return (y * width + x);
	}

	int Clamp(int val, int min, int max) {
		if (val < min) return min;
		else if (val > max) return max;
		else return val;
	}


#if AGS_PLATFORM_OS_WINDOWS

	//==============================================================================

	// ***** Design time *****

	IAGSEditor *editor; // Editor interface

	const char *ourScriptHeader =
" // ags imgui module header \r\n"
"  \r\n"
" enum ImGuiCol_ \r\n"
" { \r\n"
"     eImGuiCol_Text, \r\n"
"     eImGuiCol_TextDisabled, \r\n"
"     eImGuiCol_WindowBg,              // Background of normal windows \r\n"
"     eImGuiCol_ChildBg,               // Background of child windows \r\n"
"     eImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows \r\n"
"     eImGuiCol_Border, \r\n"
"     eImGuiCol_BorderShadow, \r\n"
"     eImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input \r\n"
"     eImGuiCol_FrameBgHovered, \r\n"
"     eImGuiCol_FrameBgActive, \r\n"
"     eImGuiCol_TitleBg, \r\n"
"     eImGuiCol_TitleBgActive, \r\n"
"     eImGuiCol_TitleBgCollapsed, \r\n"
"     eImGuiCol_MenuBarBg, \r\n"
"     eImGuiCol_ScrollbarBg, \r\n"
"     eImGuiCol_ScrollbarGrab, \r\n"
"     eImGuiCol_ScrollbarGrabHovered, \r\n"
"     eImGuiCol_ScrollbarGrabActive, \r\n"
"     eImGuiCol_CheckMark, \r\n"
"     eImGuiCol_SliderGrab, \r\n"
"     eImGuiCol_SliderGrabActive, \r\n"
"     eImGuiCol_Button, \r\n"
"     eImGuiCol_ButtonHovered, \r\n"
"     eImGuiCol_ButtonActive, \r\n"
"     eImGuiCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem \r\n"
"     eImGuiCol_HeaderHovered, \r\n"
"     eImGuiCol_HeaderActive, \r\n"
"     eImGuiCol_Separator, \r\n"
"     eImGuiCol_SeparatorHovered, \r\n"
"     eImGuiCol_SeparatorActive, \r\n"
"     eImGuiCol_ResizeGrip, \r\n"
"     eImGuiCol_ResizeGripHovered, \r\n"
"     eImGuiCol_ResizeGripActive, \r\n"
"     eImGuiCol_Tab, \r\n"
"     eImGuiCol_TabHovered, \r\n"
"     eImGuiCol_TabActive, \r\n"
"     eImGuiCol_TabUnfocused, \r\n"
"     eImGuiCol_TabUnfocusedActive, \r\n"
"     eImGuiCol_PlotLines, \r\n"
"     eImGuiCol_PlotLinesHovered, \r\n"
"     eImGuiCol_PlotHistogram, \r\n"
"     eImGuiCol_PlotHistogramHovered, \r\n"
"     eImGuiCol_TextSelectedBg, \r\n"
"     eImGuiCol_DragDropTarget, \r\n"
"     eImGuiCol_NavHighlight,          // Gamepad/keyboard: current highlighted item \r\n"
"     eImGuiCol_NavWindowingHighlight, // Highlight window when using CTRL+TAB \r\n"
"     eImGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active \r\n"
"     eImGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active \r\n"
"     eImGuiCol_COUNT \r\n"
" }; \r\n"
"  \r\n"
" enum ImGuiFocusedFlags \r\n"
" { \r\n"
"   eImGuiFocusedFlags_None = 0, \r\n"
"   eImGuiFocusedFlags_ChildWindows = 1,   // IsWindowFocused(): Return true if any children of the window is focused \r\n"
"   eImGuiFocusedFlags_RootWindow = 2,   // IsWindowFocused(): Test from root window (top most parent of the current hierarchy) \r\n"
"   eImGuiFocusedFlags_AnyWindow = 4,   // IsWindowFocused(): Return true if any window is focused.  \r\n"
"   eImGuiFocusedFlags_RootAndChildWindows  = 3 \r\n"
" }; \r\n"
"  \r\n"
" enum ImGuiHoveredFlags \r\n"
" { \r\n"
"   eImGuiHoveredFlags_None                          = 0,  // Return true if directly over the item/window, not obstructed by another window, not obstructed by an active popup or modal blocking inputs under them. \r\n"
"   eImGuiHoveredFlags_ChildWindows                  = 1,  // IsWindowHovered() only: Return true if any children of the window is hovered \r\n"
"   eImGuiHoveredFlags_RootWindow                    = 2,  // IsWindowHovered() only: Test from root window (top most parent of the current hierarchy) \r\n"
"   eImGuiHoveredFlags_AnyWindow                     = 4,  // IsWindowHovered() only: Return true if any window is hovered \r\n"
"   eImGuiHoveredFlags_AllowWhenBlockedByPopup       = 8,  // Return true even if a popup window is normally blocking access to this item/window \r\n"
"   eImGuiHoveredFlags_AllowWhenBlockedByActiveItem  = 32, // Return true even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns. \r\n"
"   eImGuiHoveredFlags_AllowWhenOverlapped           = 64, // Return true even if the position is obstructed or overlapped by another window \r\n"
"   eImGuiHoveredFlags_AllowWhenDisabled             = 128, // Return true even if the item is disabled \r\n"
"   eImGuiHoveredFlags_RectOnly                      = 104, \r\n"
"   eImGuiHoveredFlags_RootAndChildWindows           = 3 \r\n"
" }; \r\n"
"  \r\n"
" enum ImGuiTabBarFlags \r\n"
" { \r\n"
"   eImGuiTabBarFlags_None                           = 0, \r\n"
"   eImGuiTabBarFlags_Reorderable                    = 1,   // Allow manually dragging tabs to re-order them + New tabs are appended at the end of list \r\n"
"   eImGuiTabBarFlags_AutoSelectNewTabs              = 2,   // Automatically select new tabs when they appear \r\n"
"   eImGuiTabBarFlags_TabListPopupButton             = 4,   // Disable buttons to open the tab list popup \r\n"
"   eImGuiTabBarFlags_NoCloseWithMiddleMouseButton   = 8,   // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button.  \r\n"
"   eImGuiTabBarFlags_NoTabListScrollingButtons      = 16,   // Disable scrolling buttons (apply when fitting policy is ImGuiTabBarFlags_FittingPolicyScroll) \r\n"
"   eImGuiTabBarFlags_NoTooltip                      = 32,   // Disable tooltips when hovering a tab \r\n"
"   eImGuiTabBarFlags_FittingPolicyResizeDown        = 64,   // Resize tabs when they don't fit \r\n"
"   eImGuiTabBarFlags_FittingPolicyScroll            = 128,   // Add scroll buttons when tabs don't fit \r\n"
"   eImGuiTabBarFlags_FittingPolicyMask_             = 192, \r\n"
"   eImGuiTabBarFlags_FittingPolicyDefault_          = 64 \r\n"
" }; \r\n"
"  \r\n"
" enum ImGuiTabItemFlags \r\n"
" { \r\n"
"   ImGuiTabItemFlags_None = 0, \r\n"
"   ImGuiTabItemFlags_UnsavedDocument = 1,   // Append '*' to title without affecting the ID. \r\n"
"   ImGuiTabItemFlags_SetSelected = 2,   // Trigger flag to programmatically make the tab selected when calling BeginTabItem() \r\n"
"   ImGuiTabItemFlags_NoCloseWithMiddleMouseButton = 4,   // Disable behavior of closing tabs  \r\n"
"   ImGuiTabItemFlags_NoPushId = 8    // Don't call PushID(tab->ID)/PopID() on BeginTabItem()/EndTabItem() \r\n"
" }; \r\n"
"  \r\n"
" enum ImGuiBeginWindow \r\n"
" { \r\n"
"    eImGuiBeginWindow_Fail = 0, \r\n"
"    eImGuiBeginWindow_OK = 1, \r\n"
"    eImGuiBeginWindow_Collapsed = 2, \r\n"
"    eImGuiBeginWindow_OK_Closed = 3, \r\n"
"    eImGuiBeginWindow_Collapsed_Closed = 4, \r\n"
" }; \r\n"
"  \r\n"
" enum ImGuiSelectableFlags \r\n"
" { \r\n"
"    eImGuiSelectableFlags_None               = 0, \r\n"
"    eImGuiSelectableFlags_DontClosePopups    = 1,  // Clicking this don't close parent popup window \r\n"
"    eImGuiSelectableFlags_SpanAllColumns     = 2,  // Selectable frame can span all columns (text will still fit in current column) \r\n"
"    eImGuiSelectableFlags_AllowDoubleClick   = 4,  // Generate press events on double clicks too \r\n"
"    eImGuiSelectableFlags_Disabled           = 8,  // Cannot be selected, display grayed out text \r\n"
"    eImGuiSelectableFlags_AllowItemOverlap   = 16  // (WIP) Hit testing to allow subsequent widgets to overlap this one \r\n"
" }; \r\n"
"  \r\n"
" enum ImGuiComboFlags \r\n"
" { \r\n"
"   eImGuiComboFlags_None = 0, \r\n"
"   eImGuiComboFlags_PopupAlignLeft = 1,  // Align the popup toward the left by default \r\n"
"   eImGuiComboFlags_HeightSmall    = 2,  // Max ~4 items visible.  \r\n"
"   eImGuiComboFlags_HeightRegular  = 4,  // Max ~8 items visible (default) \r\n"
"   eImGuiComboFlags_HeightLarge    = 8,  // Max ~20 items visible \r\n"
"   eImGuiComboFlags_HeightLargest  = 16, // As many fitting items as possible \r\n"
"   eImGuiComboFlags_NoArrowButton  = 32, // Display on the preview box without the square arrow button \r\n"
"   eImGuiComboFlags_NoPreview      = 64, // Display only a square arrow button \r\n"
"   eImGuiComboFlags_HeightMask_    = 30, \r\n"
" }; \r\n"
" enum ImGuiDir \r\n"
" { \r\n"
" eImGuiDir_None = -1, \r\n"
" eImGuiDir_Left = 0, \r\n"
" eImGuiDir_Right = 1, \r\n"
" eImGuiDir_Up = 2, \r\n"
" eImGuiDir_Down = 3, \r\n"
" eImGuiDir_COUNT \r\n"
" }; \r\n"
"  \r\n"
" enum ImGuiWindowFlags \r\n"
" { \r\n"
" eImGuiWindowFlags_None                   = 0, \r\n"
" eImGuiWindowFlags_NoTitleBar             = 1,   // Disable title-bar \r\n"
" eImGuiWindowFlags_NoResize               = 2,   // Disable user resizing with the lower-right grip \r\n"
" eImGuiWindowFlags_NoMove                 = 4,   // Disable user moving the window \r\n"
" eImGuiWindowFlags_NoScrollbar            = 8,   // Disable scrollbars (window can still scroll with mouse or programmatically) \r\n"
" eImGuiWindowFlags_NoScrollWithMouse      = 16,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set. \r\n"
" eImGuiWindowFlags_NoCollapse             = 32,   // Disable user collapsing window by double-clicking on it \r\n"
" eImGuiWindowFlags_AlwaysAutoResize       = 64,   // Resize every window to its content every frame \r\n"
" eImGuiWindowFlags_NoBackground           = 128,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f). \r\n"
" eImGuiWindowFlags_NoSavedSettings        = 256,   // Never load/save settings in .ini file \r\n"
" eImGuiWindowFlags_NoMouseInputs          = 512,   // Disable catching mouse, hovering test with pass through. \r\n"
" eImGuiWindowFlags_MenuBar                = 1024,  // Has a menu-bar \r\n"
" eImGuiWindowFlags_HorizontalScrollbar    = 2048,  // Allow horizontal scrollbar to appear (off by default). \r\n"
" eImGuiWindowFlags_NoFocusOnAppearing     = 4096,  // Disable taking focus when transitioning from hidden to visible state \r\n"
" eImGuiWindowFlags_NoBringToFrontOnFocus  = 8192,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus) \r\n"
" eImGuiWindowFlags_AlwaysVerticalScrollbar= 16384,  // Always show vertical scrollbar (even if ContentSize.y < Size.y) \r\n"
" eImGuiWindowFlags_AlwaysHorizontalScrollbar= 32768,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x) \r\n"
" eImGuiWindowFlags_AlwaysUseWindowPadding = 65536,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient) \r\n"
" eImGuiWindowFlags_NoNavInputs            = 262144,  // No gamepad/keyboard navigation within the window \r\n"
" eImGuiWindowFlags_NoNavFocus             = 524288,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB) \r\n"
" eImGuiWindowFlags_UnsavedDocument        = 1048576,  // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. \r\n"
" eImGuiWindowFlags_NoNav                  = 786432, \r\n"
" eImGuiWindowFlags_NoDecoration           = 43, \r\n"
" eImGuiWindowFlags_NoInputs               = 786944, \r\n"
" }; \r\n"
"  \r\n"
" enum ImGuiInputTextFlags \r\n"
" { \r\n"
" eImGuiInputTextFlags_None                = 0, \r\n"
" eImGuiInputTextFlags_CharsDecimal        = 1,   // Allow 0123456789.+-*/ \r\n"
" eImGuiInputTextFlags_CharsHexadecimal    = 2,   // Allow 0123456789ABCDEFabcdef \r\n"
" eImGuiInputTextFlags_CharsUppercase      = 4,   // Turn a..z into A..Z \r\n"
" eImGuiInputTextFlags_CharsNoBlank        = 8,   // Filter out spaces, tabs \r\n"
" eImGuiInputTextFlags_AutoSelectAll       = 16,   // Select entire text when first taking mouse focus \r\n"
" eImGuiInputTextFlags_EnterReturnsTrue    = 32,   // Return 'true' when Enter is pressed (as opposed to every time the value was modified). \r\n"
" eImGuiInputTextFlags_CallbackCompletion  = 64,   // Callback on pressing TAB (for completion handling) \r\n"
" eImGuiInputTextFlags_CallbackHistory     = 128,   // Callback on pressing Up/Down arrows (for history handling) \r\n"
" eImGuiInputTextFlags_CallbackAlways      = 256,   // Callback on each iteration. User code may query cursor position, modify text buffer. \r\n"
" eImGuiInputTextFlags_CallbackCharFilter  = 512,   // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard. \r\n"
" eImGuiInputTextFlags_AllowTabInput       = 1024,  // Pressing TAB input a 't' character into the text field \r\n"
" eImGuiInputTextFlags_CtrlEnterForNewLine = 2048,  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter). \r\n"
" eImGuiInputTextFlags_NoHorizontalScroll  = 4096,  // Disable following the cursor horizontally \r\n"
" eImGuiInputTextFlags_AlwaysInsertMode    = 8192,  // Insert mode \r\n"
" eImGuiInputTextFlags_ReadOnly            = 16384,  // Read-only mode \r\n"
" eImGuiInputTextFlags_Password            = 32768,  // Password mode, display all characters as '*' \r\n"
" eImGuiInputTextFlags_NoUndoRedo          = 65536,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID(). \r\n"
" eImGuiInputTextFlags_CharsScientific     = 131072,  // Allow 0123456789.+-*/eE (Scientific notation input) \r\n"
" eImGuiInputTextFlags_CallbackResize      = 262144,  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. \r\n"
" }; \r\n"
"  \r\n"
" enum ImGuiCond \r\n"
" { \r\n"
"   ImGuiCond_Always       = 1, // Set the variable \r\n"
"   ImGuiCond_Once         = 2, // Set the variable once per runtime session (only the first call with succeed) \r\n"
"   ImGuiCond_FirstUseEver = 4, // Set the variable if the object/window has no persistently saved data (no entry in .ini file) \r\n"
"   ImGuiCond_Appearing    = 8, // Set the variable if the object/window is appearing after being hidden/inactive (or the first time) \r\n"
" }; \r\n"
"  \r\n"
" enum ImGuiTreeNodeFlags \r\n"
" { \r\n"
"   ImGuiTreeNodeFlags_None               = 0, \r\n"
"   ImGuiTreeNodeFlags_Selected           = 1,   // Draw as selected \r\n"
"   ImGuiTreeNodeFlags_Framed             = 2,   // Full colored frame (e.g. for CollapsingHeader) \r\n"
"   ImGuiTreeNodeFlags_AllowItemOverlap   = 4,   // Hit testing to allow subsequent widgets to overlap this one \r\n"
"   ImGuiTreeNodeFlags_NoTreePushOnOpen   = 8,   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack \r\n"
"   ImGuiTreeNodeFlags_NoAutoOpenOnLog    = 16,   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes) \r\n"
"   ImGuiTreeNodeFlags_DefaultOpen        = 32,   // Default node to be open \r\n"
"   ImGuiTreeNodeFlags_OpenOnDoubleClick  = 64,   // Need double-click to open node \r\n"
"   ImGuiTreeNodeFlags_OpenOnArrow        = 128,   // Only open when clicking on the arrow part. If ImGuiTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open. \r\n"
"   ImGuiTreeNodeFlags_Leaf               = 256,   // No collapsing, no arrow (use as a convenience for leaf nodes). \r\n"
"   ImGuiTreeNodeFlags_Bullet             = 512,   // Display a bullet instead of arrow \r\n"
"   ImGuiTreeNodeFlags_FramePadding       = 1024,  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height.  \r\n"
"   ImGuiTreeNodeFlags_SpanAvailWidth     = 2048,  // Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line.  \r\n"
"   ImGuiTreeNodeFlags_SpanFullWidth      = 4096,  // Extend hit box to the left-most and right-most edges (bypass the indented area). \r\n"
"   ImGuiTreeNodeFlags_NavLeftJumpsBackHere = 8192,  // (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop) \r\n"
"   ImGuiTreeNodeFlags_CollapsingHeader   = 26, \r\n"
" }; \r\n"
" \r\n"
"builtin managed struct ImVec4 { \r\n"
"  \r\n"
"  /// Creates a ImVec4 with float X and Y coordinates. \r\n"
"  import static ImVec4* Create(float x=0, float y=0, float z=0, float w=0); // $AUTOCOMPLETESTATICONLY$ \r\n"
"  \r\n"
"  /// Float X coordinate of the ImVec4. \r\n"
"  import attribute float X; \r\n"
"  \r\n"
"  /// Float Y coordinate of the ImVec4. \r\n"
"  import attribute float Y; \r\n"
"  \r\n"
"  /// Float X coordinate of the ImVec4. \r\n"
"  import attribute float Z; \r\n"
"  \r\n"
"  /// Float Y coordinate of the ImVec4. \r\n"
"  import attribute float W; \r\n"
"  \r\n"
"  /// Multiplies x and y coordinates by a scalar and returns a new ImVec4 with the result. \r\n"
"  import ImVec4* Scale(float scale); \r\n"
"  \r\n"
"  /// Returns length from ImVec4 (distance from 0,0 origin). \r\n"
"  import float Length(); \r\n"
"  \r\n"
"  ///  Returns squared length from ImVec4 (distance from 0,0 origin). Faster than length. \r\n"
"  import float SquaredLength(); \r\n"
"  \r\n"
"  /// Returns a new ImVec4 with the sum of this with imVec4. \r\n"
"  import ImVec4* Add(ImVec4* imVec4); \r\n"
"  \r\n"
"  /// Returns a new ImVec4 with the subtraction of imVec4 from this. \r\n"
"  import ImVec4* Sub(ImVec4* imVec4); \r\n"
"  \r\n"
"}; \r\n"
"  \r\n"
"builtin managed struct ImVec2 { \r\n"
"  \r\n"
"  /// Creates a ImVec2 with float X and Y coordinates. \r\n"
"  import static ImVec2* Create(float x=0, float y=0); // $AUTOCOMPLETESTATICONLY$ \r\n"
"  \r\n"
"  /// Float X coordinate of the ImVec2. \r\n"
"  import attribute float X; \r\n"
"  \r\n"
"  /// Float Y coordinate of the ImVec2. \r\n"
"  import attribute float Y; \r\n"
"  \r\n"
"  /// Multiplies x and y coordinates by a scalar and returns a new ImVec2 with the result. \r\n"
"  import ImVec2* Scale(float scale); \r\n"
"  \r\n"
"  /// Returns length from ImVec2 (distance from 0,0 origin). \r\n"
"  import float Length(); \r\n"
"  \r\n"
"  ///  Returns squared length from ImVec2 (distance from 0,0 origin). Faster than length. \r\n"
"  import float SquaredLength(); \r\n"
"  \r\n"
"  /// Returns a new ImVec2 with the sum of this with imVec2. \r\n"
"  import ImVec2* Add(ImVec2* imVec2); \r\n"
"  \r\n"
"  /// Returns a new ImVec2 with the subtraction of imVec2 from this. \r\n"
"  import ImVec2* Sub(ImVec2* imVec2); \r\n"
"  \r\n"
"}; \r\n"
"  \r\n"
"builtin managed struct ImGuiStyle \r\n"
" { \r\n"
"    /// Creates an empty ImGuiStyle. \r\n"
"    import static ImGuiStyle* Create(); // $AUTOCOMPLETESTATICONLY$ \r\n"
"     \r\n"
"    /// Global alpha applies to everything in Dear ImGui. \r\n"
"    import attribute float Alpha; \r\n"
"     \r\n"
"    /// Padding within a window. \r\n"
"    import attribute ImVec2* WindowPadding; \r\n"
"     \r\n"
"    /// Radius of window corners rounding. Set to 0.0f to have rectangular windows. \r\n"
"    import attribute float WindowRounding; \r\n"
"     \r\n"
"    /// Thickness of border around windows. Generally set to 0.0f or 1.0f. \r\n"
"    import attribute float WindowBorderSize; \r\n"
"     \r\n"
"    /// Minimum window size. This is a global setting. For individual windows, use SetNextWindowSizeConstraints(). \r\n"
"    import attribute ImVec2* WindowMinSize; \r\n"
"     \r\n"
"    /// Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered. \r\n"
"    import attribute ImVec2* WindowTitleAlign; \r\n"
"     \r\n"
"    /// Side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to ImGuiDir_Left. \r\n"
"    import attribute ImGuiDir WindowMenuButtonPosition; \r\n"
"     \r\n"
"    /// Radius of child window corners rounding. Set to 0.0f to have rectangular windows. \r\n"
"    import attribute float ChildRounding; \r\n"
"     \r\n"
"    /// Thickness of border around child windows. Generally set to 0.0f or 1.0f. \r\n"
"    import attribute float ChildBorderSize; \r\n"
"     \r\n"
"    /// Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding) \r\n"
"    import attribute float PopupRounding; \r\n"
"     \r\n"
"    /// Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. \r\n"
"    import attribute float PopupBorderSize; \r\n"
"     \r\n"
"    /// Padding within a framed rectangle (used by most widgets). \r\n"
"    import attribute ImVec2* FramePadding; \r\n"
"     \r\n"
"    /// Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets). \r\n"
"    import attribute float FrameRounding; \r\n"
"     \r\n"
"    /// Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly). \r\n"
"    import attribute float FrameBorderSize; \r\n"
"     \r\n"
"    /// Horizontal and vertical spacing between widgets/lines. \r\n"
"    import attribute ImVec2* ItemSpacing; \r\n"
"     \r\n"
"    /// Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label). \r\n"
"    import attribute ImVec2* ItemInnerSpacing; \r\n"
"     \r\n"
"    /// Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. Don't grow this too much! \r\n"
"    import attribute ImVec2* TouchExtraPadding; \r\n"
"     \r\n"
"    /// Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2). \r\n"
"    import attribute float IndentSpacing; \r\n"
"     \r\n"
"    /// Minimum horizontal spacing between two columns. Preferably > (FramePadding.x + 1). \r\n"
"    import attribute float ColumnsMinSpacing; \r\n"
"     \r\n"
"    /// Width of the vertical scrollbar, Height of the horizontal scrollbar. \r\n"
"    import attribute float ScrollbarSize; \r\n"
"     \r\n"
"    /// Radius of grab corners for scrollbar. \r\n"
"    import attribute float ScrollbarRounding; \r\n"
"     \r\n"
"    /// Minimum width/height of a grab box for slider/scrollbar. \r\n"
"    import attribute float GrabMinSize; \r\n"
"     \r\n"
"    /// Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs. \r\n"
"    import attribute float GrabRounding; \r\n"
"     \r\n"
"    /// Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs. \r\n"
"    import attribute float TabRounding; \r\n"
"     \r\n"
"    /// Thickness of border around tabs. \r\n"
"    import attribute float TabBorderSize; \r\n"
"     \r\n"
"    /// Minimum width for close button to appears on an unselected tab when hovered. Set to 0.0f to always show when hovering, set to FLT_MAX to never show close button unless selected. \r\n"
"    import attribute float TabMinWidthForUnselectedCloseButton; \r\n"
"     \r\n"
"    /// Side of the color button in the ColorEdit4 widget (left/right). Defaults to ImGuiDir_Right. \r\n"
"    import attribute ImGuiDir ColorButtonPosition; \r\n"
"     \r\n"
"    /// Alignment of button text when button is larger than text. Defaults to (0.5f, 0.5f) (centered). \r\n"
"    import attribute ImVec2* ButtonTextAlign; \r\n"
"     \r\n"
"    /// Alignment of selectable text. Defaults to (0.0f, 0.0f) (top-left aligned). It's generally important to keep this left-aligned if you want to lay multiple items on a same line. \r\n"
"    import attribute ImVec2* SelectableTextAlign; \r\n"
"     \r\n"
"    /// Window position are clamped to be visible within the display area by at least this amount. Only applies to regular windows. \r\n"
"    import attribute ImVec2* DisplayWindowPadding; \r\n"
"     \r\n"
"    /// If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly! \r\n"
"    import attribute ImVec2* DisplaySafeAreaPadding; \r\n"
"     \r\n"
"    /// Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). May be removed later. \r\n"
"    import attribute float MouseCursorScale; \r\n"
"     \r\n"
"    /// Enable anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU. \r\n"
"    import attribute bool AntiAliasedLines; \r\n"
"     \r\n"
"    /// Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.) \r\n"
"    import attribute bool AntiAliasedFill; \r\n"
"     \r\n"
"    /// Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality. \r\n"
"    import attribute float CurveTessellationTol; \r\n"
"     \r\n"
"    /// Maximum error (in pixels) allowed when using AddCircle()/AddCircleFilled() or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry. \r\n"
"    import attribute float CircleSegmentMaxError; \r\n"
"     \r\n"
"    /// Colors \r\n"
"    import attribute ImVec4* Colors[]; \r\n"
" }; \r\n"
"  \r\n"
"builtin managed struct AgsImGui{ \r\n"
"  \r\n"
" /// Gets or Sets the Style customization use in AgsImGui \r\n"
" import static attribute ImGuiStyle* Style; \r\n"
"  \r\n"
" // Main \r\n"
"  \r\n"
" /// start a new Dear ImGui frame, you can submit any command from this point until Render()/EndFrame(). \r\n"
" import static void NewFrame(); \r\n"
"  \r\n"
" /// ends the Dear ImGui frame. automatically called by Render(), you likely don't need to call that yourself directly. \r\n"
" import static void EndFrame(); \r\n"
"  \r\n"
" /// ends the Dear ImGui frame, finalize the draw data. You can get call GetDrawData() to obtain it and run your rendering function. \r\n"
" import static void Render(); \r\n"
"  \r\n"
" // valid after Render() and until the next call to NewFrame(). this is what you have to render. \r\n"
" //import static int GetDrawData(); \r\n"
"  \r\n"
"  \r\n"
" // Demo, Debug, Information \r\n"
"  \r\n"
" /// create Demo window (previously called ShowTestWindow). demonstrate most ImGui features. call this to learn about the library! \r\n"
" import static void ShowDemoWindow(); \r\n"
"  \r\n"
" /// create About window. display Dear ImGui version, credits and build/system information. \r\n"
" import static void ShowAboutWindow(); \r\n"
"  \r\n"
" /// create Metrics/Debug window. display Dear ImGui internals: draw commands (with individual draw calls and vertices), window list, basic internal state, etc. \r\n"
" import static void ShowMetricsWindow(); \r\n"
"  \r\n"
" /// add basic help/info block (not a window): how to manipulate ImGui as a end-user (mouse/keyboard controls). \r\n"
" import static void ShowUserGuide(); \r\n"
"  \r\n"
" /// get the compiled version string e.g. \"1.23\" (essentially the compiled value for IMGUI_VERSION) \r\n"
" import static String GetVersion(); \r\n"
"  \r\n"
" import static void StyleColorsDark(); \r\n"
"  \r\n"
" import static void StyleColorsClassic(); \r\n"
"  \r\n"
" import static void StyleColorsLight(); \r\n"
"  \r\n"
" // Windows \r\n"
"  \r\n"
" /// Push window to the stack. Always call a matching End() for each Begin() call. Return value indicates if it's collapsed or if clicked to close. \r\n"
" import static ImGuiBeginWindow BeginWindow(String name, bool has_close_button = 0, ImGuiWindowFlags flags = 0); \r\n"
"  \r\n"
" /// pop window from the stack. \r\n"
" import static void EndWindow(); \r\n"
"  \r\n"
" // Child Windows \r\n"
"  \r\n"
" /// Child Windows. Always call a matching EndChild() for each BeginChild() call, regardless of its return value. Child windows can embed their own child. \r\n"
" import static bool BeginChild(String str_id, int width = 0, int height = 0, bool border = false, ImGuiWindowFlags flags = 0); \r\n"
"  \r\n"
" /// pop child window from the stack. \r\n"
" import static void EndChild(); \r\n"
"  \r\n"
" /// set next window position. call before Begin(). use pivot=(0.5,0.5) to center on given point, etc. \r\n"
" import static void SetNextWindowPos(int position_x, int position_y, ImGuiCond cond = 0, float pivot_x = 0, float pivot_y = 0); \r\n"
"  \r\n"
" /// set next window size. set axis to 0 to force an auto-fit on this axis. call before Begin() \r\n"
" import static void SetNextWindowSize(int width = 0, int height = 0, ImGuiCond cond = 0); \r\n"
"  \r\n"
" /// set next window size limits. use -1,-1 on either X/Y axis to preserve the current size. Sizes will be rounded down. \r\n"
" import static void SetNextWindowSizeConstraints(int min_width, int min_height, int max_width, int max_height); \r\n"
"  \r\n"
" /// set next window content size (~ scrollable client area, which enforce the range of scrollbars). Not including window decorations nor WindowPadding. set an axis to 0 to leave it automatic. call before Begin() \r\n"
" import static void SetNextWindowContentSize(int width = 0, int height = 0); \r\n"
"  \r\n"
" /// set next window collapsed state. call before Begin() \r\n"
" import static void SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0); \r\n"
"  \r\n"
" /// set next window to be focused / top-most. call before Begin() \r\n"
" import static void SetNextWindowFocus(); \r\n"
"  \r\n"
" /// set next window background color alpha. helper to easily modify ImGuiCol_WindowBg/ChildBg/PopupBg. \r\n"
" import static void SetNextWindowBgAlpha(float alpha); \r\n"
"  \r\n"
" // Item/Widgets Utilities \r\n"
" // - Most of the functions are referring to the last/previous item we submitted. \r\n"
"  \r\n"
" /// is the last item hovered? (and usable, aka not blocked by a popup, etc.). See ImGuiHoveredFlags for more options. \r\n"
" import static bool IsItemHovered(ImGuiHoveredFlags flags = 0);  \r\n"
"  \r\n"
" /// is the last item active? (e.g. button being held, text field being edited. This will continuously return true while holding mouse button on an item.) \r\n"
" import static bool IsItemActive();  \r\n"
"  \r\n"
" /// is the last item focused for keyboard/gamepad navigation? \r\n"
" import static bool IsItemFocused(); \r\n"
"  \r\n"
" /// is the last item visible? (items may be out of sight because of clipping/scrolling) \r\n"
" import static bool IsItemVisible(); \r\n"
"  \r\n"
" /// did the last item modify its underlying value this frame? or was pressed? This is generally the same as the bool return value of many widgets. \r\n"
" import static bool IsItemEdited(); \r\n"
"  \r\n"
" /// was the last item just made active (item was previously inactive). \r\n"
" import static bool IsItemActivated(); \r\n"
"  \r\n"
" /// was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that requires continuous editing. \r\n"
" import static bool IsItemDeactivated(); \r\n"
"  \r\n"
" /// was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that requires continuous editing. \r\n"
" import static bool IsItemDeactivatedAfterEdit(); \r\n"
"  \r\n"
" /// was the last item open state toggled? set by TreeNode(). \r\n"
" import static bool IsItemToggledOpen(); \r\n"
"  \r\n"
" /// is any item hovered? \r\n"
" import static bool IsAnyItemHovered();  \r\n"
"  \r\n"
" /// is any item active? \r\n"
" import static bool IsAnyItemActive(); \r\n"
"  \r\n"
" /// is any item focused? \r\n"
" import static bool IsAnyItemFocused();  \r\n"
"  \r\n"
" // Windows Utilities \r\n"
" // - 'current window' = the window we are appending into while inside a Begin()/End() block.  \r\n"
" import static bool IsWindowAppearing(); \r\n"
"  \r\n"
" /// return true when window is collapsed. Use this between Begin and End of a window. \r\n"
" import static bool IsWindowCollapsed(); \r\n"
"  \r\n"
" /// is current window focused? or its root/child, depending on flags. see flags for options. Use this between Begin and End of a window. \r\n"
" import static bool IsWindowFocused(ImGuiFocusedFlags flags=0); \r\n"
"  \r\n"
" /// is current window hovered (and typically: not blocked by a popup/modal)? see flags for options. Use this between Begin and End of a window.\r\n"
" import static bool IsWindowHovered(ImGuiHoveredFlags flags=0); \r\n"
"  \r\n"
" // Layout \r\n"
" // - By cursor we mean the current output position. \r\n"
" // - The typical widget behavior is to output themselves at the current cursor position, then move the cursor one line down. \r\n"
" // - You can call SameLine() between widgets to undo the last carriage return and output at the right of the preceeding widget. \r\n"
"  \r\n"
" /// separator, generally horizontal. inside a menu bar or in horizontal layout mode, this becomes a vertical separator. \r\n"
" import static void Separator(); \r\n"
"  \r\n"
" /// call between widgets or groups to layout them horizontally. X position given in window coordinates. \r\n"
" import static void SameLine(float offset_from_start_x = 0, float spacing=0); \r\n"
"  \r\n"
" /// undo a SameLine() or force a new line when in an horizontal-layout context. \r\n"
" import static void NewLine(); \r\n"
"  \r\n"
" /// add vertical spacing. \r\n"
" import static void Spacing(); \r\n"
"  \r\n"
" /// add a dummy item of given size. unlike InvisibleButton(), Dummy() won't take the mouse click or be navigable into. \r\n"
" import static void Dummy(float width, float height); \r\n"
"  \r\n"
" /// move content position toward the right, by style.IndentSpacing or indent_w if != 0 \r\n"
" import static void Indent(float indent_w = 0); \r\n"
"  \r\n"
" /// move content position back to the left, by style.IndentSpacing or indent_w if != 0 \r\n"
" import static void Unindent(float indent_w = 0); \r\n"
"  \r\n"
" // ID stack/scopes \r\n"
" // - Read the FAQ for more details about how ID are handled in dear imgui. If you are creating widgets in a loop you most \r\n"
" //   likely want to push a unique identifier (e.g. object pointer, loop index) to uniquely differentiate them. \r\n"
" // - The resulting ID are hashes of the entire stack. \r\n"
" // - You can also use the `Label##foobar` syntax within widget label to distinguish them from each others. \r\n"
" // - In this header file we use the `label`/`name` terminology to denote a string that will be displayed and used as an ID, \r\n"
" //   whereas `str_id` denote a string that is only used as an ID and not normally displayed. \r\n"
"  \r\n"
" /// push string into the ID stack (will hash string). \r\n"
" import static void PushID(String str_id); \r\n"
"  \r\n"
" /// pop from the ID stack. \r\n"
" import static void PopID(); \r\n"
"  \r\n"
" // Widgets: Text \r\n"
"  \r\n"
" /// draws text \r\n"
" import static void Text(String text); \r\n"
"  \r\n"
" /// shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor(); \r\n"
" import static void TextColored(int color, String text); \r\n"
"  \r\n"
" /// shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor(); \r\n"
" import static void TextDisabled(String text); \r\n"
"  \r\n"
" /// shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto- \r\n"
" import static void TextWrapped(String text); \r\n"
"  \r\n"
" /// display text+label aligned the same way as value+label widgets \r\n"
" import static void LabelText(String label, String text); \r\n"
"  \r\n"
" /// shortcut for Bullet()+Text() \r\n"
" import static void BulletText(String text); \r\n"
"  \r\n"
"  \r\n"
" // Widgets: Main \r\n"
" // - Most widgets return true when the value has been changed or when pressed/selected \r\n"
" // - You may also use one of the many IsItemXXX functions (e.g. IsItemActive, IsItemHovered, etc.) to query widget state. \r\n"
"  \r\n"
" /// button \r\n"
" import static bool Button(String label, int width = 0, int height = 0); \r\n"
"  \r\n"
" /// button with FramePadding=(0,0) to easily embed within text \r\n"
" import static bool SmallButton(String label); \r\n"
"  \r\n"
" /// create an image with passed sprite ID. \r\n"
" import static void Image(int sprite_id); \r\n"
"  \r\n"
" /// create a button with an image with passed sprite ID. Returns true while clicked. \r\n"
" import static bool ImageButton(int sprite_id); \r\n"
"  \r\n"
" /// square button with an arrow shape \r\n"
" import static bool ArrowButton(String str_id, ImGuiDir dir); \r\n"
"  \r\n"
" import static bool Checkbox(String label, bool v); \r\n"
"  \r\n"
" /// use with e.g. if (RadioButton(\"one\", my_value==1)) { my_value = 1; } \r\n"
" import static bool RadioButton(String label, bool active); \r\n"
"  \r\n"
" /// draw a small circle and keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses \r\n"
" import static void Bullet(); \r\n"
"  \r\n"
" // Widgets: Selectables \r\n"
" // - A selectable highlights when hovered, and can display another color when selected. \r\n"
"  \r\n"
" /// bool selected carry the selection state (read-only). When Selectable() is clicked it returns true so you can modify your selection state. \r\n"
" import static bool Selectable(String label, bool selected = false, ImGuiSelectableFlags flags = 0, int width = 0, int height = 0); \r\n"
"  \r\n"
" // Widgets: Drag \r\n"
"  \r\n"
" /// CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped and can go off-bounds. \r\n"
" import static float DragFloat(String label, float value, float min_value = 0, float max_value = 0, float speed = 0, String format = 0); \r\n"
"  \r\n"
" /// CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped and can go off-bounds. \r\n"
" import static int DragInt(String label, int value, int min_value = 0, int max_value = 0, float speed = 0, String format = 0); \r\n"
"  \r\n"
" // Widgets: Slider \r\n"
"  \r\n"
" /// CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped and can go off-bounds. \r\n"
" import static float SliderFloat(String label, float value, float min_value = 0, float max_value = 0, String format = 0); \r\n"
"  \r\n"
" /// CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped and can go off-bounds. \r\n"
" import static int SliderInt(String label, int value, int min_value = 0, int max_value = 0, String format = 0); \r\n"
"  \r\n"
" // Widgets: Input with Keyboard \r\n"
"  \r\n"
" import static String InputText(String label, String text_buffer, int buffer_size, ImGuiInputTextFlags flags =0); \r\n"
"  \r\n"
" import static String InputTextMultiline(String label, String text_buffer, int buffer_size, int width=0, int height=0, ImGuiInputTextFlags flags = 0); \r\n"
"  \r\n"
" import static String InputTextWithHint(String label, String hint, String text_buffer, int buffer_size, ImGuiInputTextFlags flags = 0); \r\n"
"  \r\n"
" // Widgets: Combobox commands \r\n"
"  \r\n"
" /// The BeginCombo()/EndCombo() allows to manage your contents and selection state however you want it, by creating e.g. Selectable() items. \r\n"
" import static bool BeginCombo(String label, String preview_value, ImGuiComboFlags flags = 0); \r\n"
"  \r\n"
" /// Only call EndCombo() if BeginCombo() returns true! \r\n"
" import static void EndCombo(); \r\n"
"  \r\n"
" // Widgets: Trees \r\n"
" // - TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents. \r\n"
"  \r\n"
" /// show the contents and call TreePop() if this returns true. \r\n"
" import static bool TreeNode(String label); \r\n"
"  \r\n"
" /// helper variation to easily decorelate the id from the displayed string. Read the FAQ about why and how to use ID. To align arbitrary text at the same level as a TreeNode() you can use Bullet(). \r\n"
" import static bool TreeNodeWithID(String str_id, String text); \r\n"
"  \r\n"
" /// show the contents and call TreePop() if this returns true. \r\n"
" import static bool TreeNodeV(String str_id, String text); \r\n"
"  \r\n"
" /// show the contents and call TreePop() if this returns true. \r\n"
" import static bool TreeNodeEx(String label, ImGuiTreeNodeFlags flags = 0); \r\n"
"  \r\n"
" /// show the contents and call TreePop() if this returns true. \r\n"
" import static bool TreeNodeExWithID(String str_id, ImGuiTreeNodeFlags flags, String text); \r\n"
"  \r\n"
" /// show the contents and call TreePop() if this returns true. \r\n"
" import static bool TreeNodeExV(String str_id, ImGuiTreeNodeFlags flags, String text); \r\n"
"  \r\n"
" /// ~ Indent()+PushId(). Already called by TreeNode() when returning true, but you can call TreePush/TreePop yourself if desired. \r\n"
" import static void TreePush(String str_id); \r\n"
"  \r\n"
" /// ~ Unindent()+PopId() \r\n"
" import static void TreePop(); \r\n"
"  \r\n"
" /// horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode \r\n"
" import static float GetTreeNodeToLabelSpacing(); \r\n"
"  \r\n"
" /// if returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop(). \r\n"
" import static bool CollapsingHeader(String label, ImGuiTreeNodeFlags flags = 0); \r\n"
"  \r\n"
" /// set next TreeNode/CollapsingHeader open state. \r\n"
" import static void SetNextItemOpen(bool is_open, ImGuiCond cond = 0); \r\n"
"  \r\n"
" // Widgets: List Boxes \r\n"
"  \r\n"
" /// If the function return true, you can output elements then call EndListBox() afterwards. \r\n"
" import static bool BeginListBox(String label, int items_count, int height_in_items = -1); \r\n"
"  \r\n"
" /// Only call EndListBox() if BeginListBox() returns true! \r\n"
" import static void EndListBox(); \r\n"\
"  \r\n"
" // Tooltips \r\n"
" // - Tooltip are windows following the mouse which do not take focus away. \r\n"
"  \r\n"
" /// begin/append a tooltip window. to create full-featured tooltip (with any kind of items).  \r\n"
" import static void BeginTooltip(); \r\n"
"  \r\n"
" /// always call after a BeginTooltip block \r\n"
" import static void EndTooltip(); \r\n"
"  \r\n"
" /// set a text-only tooltip, typically use with AgsImGui.IsItemHovered(). override any previous call to SetTooltip(). \r\n"
" import static void SetTooltip(String text); \r\n"
"  \r\n"
"  \r\n"
" /// call to mark popup as open (don't call every frame!). popups are closed when user click outside, or if CloseCurrentPopup() is called within a BeginPopup()/EndPopup() block. Popup identifiers are relative to the current ID-stack. \r\n"
" import static void OpenPopup(String str_id); \r\n"
"  \r\n"
" /// return true if the popup is open, and you can start outputting to it. only call EndPopup() if BeginPopup() returns true! \r\n"
" import static bool BeginPopup(String str_id, ImGuiWindowFlags flags = 0);  \r\n"
"  \r\n"
" /// modal dialog (regular window with title bar, block interactions behind the modal window, can't close the modal window by clicking outside) \r\n"
" import static bool BeginPopupModal(String name, bool has_close_button = 0, ImGuiWindowFlags flags = 0); \r\n"
"  \r\n"
" /// only call EndPopup() if BeginPopupXXX() returns true! \r\n"
" import static void EndPopup(); \r\n"
"  \r\n"
" /// return true if the popup is open at the current begin-ed level of the popup stack. \r\n"
" import static bool IsPopupOpen(String str_id); \r\n"
"  \r\n"
" /// close the popup we have begin-ed into. clicking on a MenuItem or Selectable automatically close the current popup. \r\n"
" import static void CloseCurrentPopup(); \r\n"
"  \r\n"
" // Tab Bars, Tabs \r\n"
"  \r\n"
" /// create and append into a TabBar \r\n"
" import static bool BeginTabBar(String str_id, ImGuiTabBarFlags flags = 0); \r\n"
"  \r\n"
" /// only call EndTabBar() if BeginTabBar() returns true! \r\n"
" import static void EndTabBar(); \r\n"
"  \r\n"
" /// create a Tab. Returns true if the Tab is selected. \r\n"
" import static bool BeginTabItem(String label, bool has_close_button = 0, ImGuiTabItemFlags flags = 0); \r\n"
"  \r\n"
" /// only call EndTabItem() if BeginTabItem() returns true! \r\n"
" import static void EndTabItem(); \r\n"
"  \r\n"
" /// notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name. \r\n"
" import static void SetTabItemClosed(String tab_or_docked_window_label); \r\n"
"  \r\n"
" /// append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar flag set on parent window). \r\n"
" import static bool BeginMenuBar(); \r\n"
"  \r\n"
" /// Only call EndMenuBar() if BeginMenuBar() returns true! \r\n"
" import static void EndMenuBar(); \r\n"
"  \r\n"
" /// Create and append to a full screen menu-bar. \r\n"
" import static bool BeginMainMenuBar(); \r\n"
"  \r\n"
" /// Only call EndMainMenuBar() if BeginMainMenuBar() returns true! \r\n"
" import static void EndMainMenuBar(); \r\n"
"  \r\n"
" /// Create a sub-menu entry. Only call EndMenu() if this returns true! \r\n"
" import static bool BeginMenu(String label, bool enabled = true); \r\n"
"  \r\n"
" /// Only call EndMenu() if BeginMenu() returns true! \r\n"
" import static void EndMenu(); \r\n"
"  \r\n"
" /// return true when activated. shortcuts are displayed for convenience but not processed by ImGui at the moment! \r\n"
" import static bool MenuItem(String label, String shortcut, bool selected = false, bool enabled = true); \r\n"
"  \r\n"
" // Widgets: Value \r\n"
"  \r\n"
" /// Shortcut to a label followed by a boolean value. \r\n"
" import static void ValueBool(String prefix = 0, bool value); \r\n"
"  \r\n"
" /// Shortcut to a label followed by a int value. \r\n"
" import static void ValueInt(String prefix = 0, int value); \r\n"
"  \r\n"
" /// Shortcut to a label followed by a float value. \r\n"
" import static void ValueFloat(String prefix = 0, float value); \r\n"
" // io stuff \r\n"
" /// Override capture or not capture mouse by ImGui for next frame. Mouse will still be captured by AGS. \r\n"
" import static void DoCaptureMouse(bool want_capture_mouse = true); \r\n"
"  \r\n"
" /// Override capture or not capture keyboard by ImGui for next frame. Keyboard will still be captured by AGS. \r\n"
" import static void DoCaptureKeyboard(bool want_capture_keyboard = true); \r\n"
"  \r\n"
" /// Override capture or not capture keyboard by ImGui for next frame. Keyboard will still be captured by AGS. \r\n"
" import static void DoMouseWheel(ImGuiDir wheel_direction); \r\n"
"  \r\n"
" // Activation \r\n"
"  \r\n"
" ///make last item the default focused item of a window.  \r\n"
" import static void SetItemDefaultFocus();  \r\n"
"  \r\n"
" ///focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.\r\n"
" import static void SetKeyboardFocusHere(int offset = 0); \r\n"
" }; \r\n"
"  \r\n"
" struct AgsImGuiHelper { \r\n"
"  \r\n"
"  import static void SetClipboarText(String text); \r\n"
"  \r\n"
"  import static String GetClipboarText(); \r\n"
"  \r\n"
"  //import static void SetClipboarImage(int sprite_id); \r\n"
"  \r\n"
"  //import static int GetClipboarImage(); \r\n"
"  \r\n"
" }; \r\n";


	//------------------------------------------------------------------------------
	LPCSTR AGS_GetPluginName(void)
	{
		// Return the plugin description
		return "agsimgui";
	}

	//------------------------------------------------------------------------------

	int AGS_EditorStartup(IAGSEditor *lpEditor)
	{
		// User has checked the plugin to use it in their game

		// If it's an earlier version than what we need, abort.
		if (lpEditor->version < MIN_EDITOR_VERSION)
			return (-1);

		editor = lpEditor;
		editor->RegisterScriptHeader(ourScriptHeader);

		// Return 0 to indicate success
		return (0);
	}

	//------------------------------------------------------------------------------

	void AGS_EditorShutdown()
	{
		// User has un-checked the plugin from their game
		editor->UnregisterScriptHeader(ourScriptHeader);
	}

	//------------------------------------------------------------------------------

	void AGS_EditorProperties(HWND parent)                        //*** optional ***
	{
		// User has chosen to view the Properties of the plugin
		// We could load up an options dialog or something here instead
/*	MessageBox(parent,
			 L"agsimgui v0.3.0 By eri0o",
			 L"About",
		 MB_OK | MB_ICONINFORMATION);
 */
	}

	//------------------------------------------------------------------------------

	int AGS_EditorSaveGame(char *buffer, int bufsize)             //*** optional ***
	{
		// Called by the editor when the current game is saved to disk.
		// Plugin configuration can be stored in [buffer] (max [bufsize] bytes)
		// Return the amount of bytes written in the buffer
		return (0);
	}

	//------------------------------------------------------------------------------

	void AGS_EditorLoadGame(char *buffer, int bufsize)            //*** optional ***
	{
		// Called by the editor when a game is loaded from disk
		// Previous written data can be read from [buffer] (size [bufsize]).
		// Make a copy of the data, the buffer is freed after this function call.
	}

	//==============================================================================

#endif

// ***** Run time *****

ImVec4 FromAgsColors(int color){
    return ImVec4(
            (float)((getr32(color))/255.0),
            (float)((getg32(color))/255.0),
            (float)((getb32(color))/255.0),
            (float)((geta32(color))/255.0));
}



// Engine interface

//------------------------------------------------------------------------------

union
{
    float f;
    uint32_t ui32;
} AgsNumber;

uint32_t inline ToAgsFloat(float f) {
    AgsNumber.f = f;
    return AgsNumber.ui32;
}

float inline ToNormalFloat(uint32_t ui32) {
    AgsNumber.ui32 = ui32;
    return AgsNumber.f;
}

int inline ToAgsBool(bool b){
    return b ? 1 : 0;
}

bool inline ToNormalBool(int bi){
    return bi!=0 ? true : false;
}

std::string g_ClipboardTextData = "";

static const char* ImGui_ImplClip_GetClipboardText(void*)
{
    g_ClipboardTextData.clear();
	if (clip::has(clip::text_format())) {
		clip::get_text(g_ClipboardTextData);
	}     
    return g_ClipboardTextData.c_str();
}

static void ImGui_ImplClip_SetClipboardText(void*, const char* text)
{
    clip::set_text(text);
}

#define STRINGIFY(s) STRINGIFY_X(s)
#define STRINGIFY_X(s) #s


texture_color32_t software_renderer_screen;
texture_alpha8_t fontAtlas;
ImGuiContext *context = nullptr;

typedef int (*SCAPI_MOUSE_ISBUTTONDOWN) (int button);
SCAPI_MOUSE_ISBUTTONDOWN Mouse_IsButtonDown = NULL;
bool has_new_render = false;
bool has_new_frame = false;


// -- begin AgsImVec2

AgsImVec2* AgsImVec2_Create(uint32_t x, uint32_t y){
    float fx = ToNormalFloat(x);
    float fy = ToNormalFloat(y);
    AgsImVec2* agsImVec2 = new AgsImVec2(fx, fy);
    agsImVec2->id = engine->RegisterManagedObject(agsImVec2, &AgsImVec2_Interface);
    return  agsImVec2;
}

int32 AgsImVec2_GetY(AgsImVec2* self) {
    return ToAgsFloat(self->y);
}

void AgsImVec2_SetX(AgsImVec2* self, uint32_t x) {
    float fx = ToNormalFloat(x);

    self->x = fx;
}

uint32_t AgsImVec2_GetX(AgsImVec2* self) {
    return ToAgsFloat(self->x);
}

void AgsImVec2_SetY(AgsImVec2* self, uint32_t y) {
    float fy = ToNormalFloat(y);

    self->y = fy;
}

uint32_t AgsImVec2_Length(AgsImVec2* self) {
    return ToAgsFloat(self->Length());
}

uint32_t AgsImVec2_SquaredLength(AgsImVec2* self) {
    return ToAgsFloat(self->SquaredLength());
}

AgsImVec2* AgsImVec2_Add(AgsImVec2* self, AgsImVec2* other){
    AgsImVec2* agsImVec2 = self->Add(other);
    agsImVec2->id = engine->RegisterManagedObject(agsImVec2, &AgsImVec2_Interface);
    return  agsImVec2;
}

AgsImVec2* AgsImVec2_Sub(AgsImVec2* self, AgsImVec2* other){
    AgsImVec2* agsImVec2 = self->Sub(other);
    agsImVec2->id = engine->RegisterManagedObject(agsImVec2, &AgsImVec2_Interface);
    return  agsImVec2;
}


AgsImVec2* AgsImVec2_Scale(AgsImVec2* self, uint32_t scale){
    float f_scale = ToNormalFloat(scale);
    AgsImVec2* agsImVec2 = self->Scale(f_scale);
    agsImVec2->id = engine->RegisterManagedObject(agsImVec2, &AgsImVec2_Interface);
    return  agsImVec2;
}

// -- end AgsImVec2


// -- begin AgsImVec4

AgsImVec4* AgsImVec4_Create(uint32_t x, uint32_t y, uint32_t z, uint32_t w){
    float fx = ToNormalFloat(x);
    float fy = ToNormalFloat(y);
    float fz = ToNormalFloat(z);
    float fw = ToNormalFloat(w);
    AgsImVec4* agsImVec4 = new AgsImVec4(fx, fy, fz, fw);
    agsImVec4->id = engine->RegisterManagedObject(agsImVec4, &AgsImVec4_Interface);
    return  agsImVec4;
}

int32 AgsImVec4_GetY(AgsImVec4* self) {
    return ToAgsFloat(self->y);
}

void AgsImVec4_SetX(AgsImVec4* self, uint32_t x) {
    float fx = ToNormalFloat(x);

    self->x = fx;
}

uint32_t AgsImVec4_GetX(AgsImVec4* self) {
    return ToAgsFloat(self->x);
}

void AgsImVec4_SetY(AgsImVec4* self, uint32_t y) {
    float fy = ToNormalFloat(y);

    self->y = fy;
}

int32 AgsImVec4_GetZ(AgsImVec4* self) {
    return ToAgsFloat(self->z);
}

void AgsImVec4_SetZ(AgsImVec4* self, uint32_t z) {
    float fz = ToNormalFloat(z);

    self->z = fz;
}

uint32_t AgsImVec4_GetW(AgsImVec4* self) {
    return ToAgsFloat(self->w);
}

void AgsImVec4_SetW(AgsImVec4* self, uint32_t w) {
    float fw = ToNormalFloat(w);

    self->w = fw;
}

uint32_t AgsImVec4_Length(AgsImVec4* self) {
    return ToAgsFloat(self->Length());
}

uint32_t AgsImVec4_SquaredLength(AgsImVec4* self) {
    return ToAgsFloat(self->SquaredLength());
}

AgsImVec4* AgsImVec4_Add(AgsImVec4* self, AgsImVec4* other){
    AgsImVec4* agsImVec4 = self->Add(other);
    agsImVec4->id = engine->RegisterManagedObject(agsImVec4, &AgsImVec4_Interface);
    return  agsImVec4;
}

AgsImVec4* AgsImVec4_Sub(AgsImVec4* self, AgsImVec4* other){
    AgsImVec4* agsImVec4 = self->Sub(other);
    agsImVec4->id = engine->RegisterManagedObject(agsImVec4, &AgsImVec4_Interface);
    return  agsImVec4;
}


AgsImVec4* AgsImVec4_Scale(AgsImVec4* self, uint32_t scale){
    float f_scale = ToNormalFloat(scale);
    AgsImVec4* agsImVec4 = self->Scale(f_scale);
    agsImVec4->id = engine->RegisterManagedObject(agsImVec4, &AgsImVec4_Interface);
    return  agsImVec4;
}

// -- end AgsImVec4

// -- being AgsImGuiStyle

AgsImVec2* NewAgsImVec2(ImVec2 &imVec2) {
    AgsImVec2 *agsImVec2 = new AgsImVec2(imVec2.x,imVec2.y);
    agsImVec2->id = engine->RegisterManagedObject(agsImVec2, &AgsImVec2_Interface);
    return agsImVec2;
}

AgsImVec4* NewAgsImVec4(ImVec4 &imVec4) {
    AgsImVec4 *agsImVec4 = new AgsImVec4(imVec4.x,imVec4.y, imVec4.z, imVec4.w);
    agsImVec4->id = engine->RegisterManagedObject(agsImVec4, &AgsImVec4_Interface);
    return agsImVec4;
}

void SetAgsImVec2(ImVec2 &imVec2, AgsImVec2* agsImVec2){
    imVec2.x = agsImVec2->x;
    imVec2.y = agsImVec2->y;
}

void SetAgsImVec4(ImVec4 &imVec4, AgsImVec4* agsImVec4){
    imVec4.x = agsImVec4->x;
    imVec4.y = agsImVec4->y;
    imVec4.z = agsImVec4->z;
    imVec4.w = agsImVec4->w;
}

AgsImGuiStyle* NewAgsImGuiStyle(ImGuiStyle &imGuiStyle) {
    AgsImGuiStyle *agsImGuiStyle = new AgsImGuiStyle(imGuiStyle);
    agsImGuiStyle->id = engine->RegisterManagedObject(agsImGuiStyle, &AgsImGuiStyle_Interface);
    return agsImGuiStyle;
}

void SetAgsImGuiStyle(ImGuiStyle &imGuiStyle, AgsImGuiStyle* agsImGuiStyle){
    AgsImGuiStyle::SetAgsImGuiStyle(imGuiStyle, agsImGuiStyle);
}

void AgsImGuiStyle_SetAlpha(AgsImGuiStyle* self, uint32_t alpha){
    float f_alpha = ToNormalFloat(alpha);
    self->Alpha = f_alpha;
}

uint32_t AgsImGuiStyle_GetAlpha(AgsImGuiStyle* self){
    return ToAgsFloat(self->Alpha);
}


void AgsImGuiStyle_SetWindowPadding(AgsImGuiStyle* self, AgsImVec2* windowPadding){
    SetAgsImVec2(self->WindowPadding, windowPadding);
}

AgsImVec2* AgsImGuiStyle_GetWindowPadding(AgsImGuiStyle* self){
    return NewAgsImVec2(self->WindowPadding);
}


void AgsImGuiStyle_SetWindowRounding(AgsImGuiStyle* self, uint32_t windowRounding){
    float f_windowRounding = ToNormalFloat(windowRounding);
    self->WindowRounding = f_windowRounding;
}

uint32_t AgsImGuiStyle_GetWindowRounding(AgsImGuiStyle* self){
    return ToAgsFloat(self->WindowRounding);
}


void AgsImGuiStyle_SetWindowBorderSize(AgsImGuiStyle* self, uint32_t windowBorderSize){
    float f_windowBorderSize = ToNormalFloat(windowBorderSize);
    self->WindowBorderSize = f_windowBorderSize;
}

uint32_t AgsImGuiStyle_GetWindowBorderSize(AgsImGuiStyle* self){
    return ToAgsFloat(self->WindowBorderSize);
}


void AgsImGuiStyle_SetWindowMinSize(AgsImGuiStyle* self, AgsImVec2* windowMinSize){
    SetAgsImVec2(self->WindowMinSize, windowMinSize);
}

AgsImVec2* AgsImGuiStyle_GetWindowMinSize(AgsImGuiStyle* self){
    return NewAgsImVec2(self->WindowMinSize);
}


void AgsImGuiStyle_SetWindowTitleAlign(AgsImGuiStyle* self, AgsImVec2* windowTitleAlign){
    SetAgsImVec2(self->WindowTitleAlign, windowTitleAlign);
}

AgsImVec2* AgsImGuiStyle_GetWindowTitleAlign(AgsImGuiStyle* self){
    return NewAgsImVec2(self->WindowTitleAlign);
}


void AgsImGuiStyle_SetWindowMenuButtonPosition(AgsImGuiStyle* self, int windowMenuButtonPosition){
    self->WindowMenuButtonPosition = windowMenuButtonPosition;
}

int AgsImGuiStyle_GetWindowMenuButtonPosition(AgsImGuiStyle* self){
    return self->WindowMenuButtonPosition;
}


void AgsImGuiStyle_SetChildRounding(AgsImGuiStyle* self, uint32_t childRounding){
    float f_childRounding = ToNormalFloat(childRounding);
    self->ChildRounding = f_childRounding;
}

uint32_t AgsImGuiStyle_GetChildRounding(AgsImGuiStyle* self){
    return ToAgsFloat(self->ChildRounding);
}


void AgsImGuiStyle_SetChildBorderSize(AgsImGuiStyle* self, uint32_t childBorderSize){
    float f_childBorderSize = ToNormalFloat(childBorderSize);
    self->ChildBorderSize = f_childBorderSize;
}

uint32_t AgsImGuiStyle_GetChildBorderSize(AgsImGuiStyle* self){
    return ToAgsFloat(self->ChildBorderSize);
}


void AgsImGuiStyle_SetPopupRounding(AgsImGuiStyle* self, uint32_t popupRounding){
    float f_popupRounding = ToNormalFloat(popupRounding);
    self->PopupRounding = f_popupRounding;
}

uint32_t AgsImGuiStyle_GetPopupRounding(AgsImGuiStyle* self){
    return ToAgsFloat(self->PopupRounding);
}


void AgsImGuiStyle_SetPopupBorderSize(AgsImGuiStyle* self, uint32_t popupBorderSize){
    float f_popupBorderSize = ToNormalFloat(popupBorderSize);
    self->PopupBorderSize = f_popupBorderSize;
}

uint32_t AgsImGuiStyle_GetPopupBorderSize(AgsImGuiStyle* self){
    return ToAgsFloat(self->PopupBorderSize);
}


void AgsImGuiStyle_SetFramePadding(AgsImGuiStyle* self, AgsImVec2* framePadding){
    SetAgsImVec2(self->FramePadding, framePadding);
}

AgsImVec2* AgsImGuiStyle_GetFramePadding(AgsImGuiStyle* self){
    return NewAgsImVec2(self->FramePadding);
}


void AgsImGuiStyle_SetFrameRounding(AgsImGuiStyle* self, uint32_t frameRounding){
    float f_frameRounding = ToNormalFloat(frameRounding);
    self->FrameRounding = f_frameRounding;
}

uint32_t AgsImGuiStyle_GetFrameRounding(AgsImGuiStyle* self){
    return ToAgsFloat(self->FrameRounding);
}


void AgsImGuiStyle_SetFrameBorderSize(AgsImGuiStyle* self, uint32_t frameBorderSize){
    float f_frameBorderSize = ToNormalFloat(frameBorderSize);
    self->FrameBorderSize = f_frameBorderSize;
}

uint32_t AgsImGuiStyle_GetFrameBorderSize(AgsImGuiStyle* self){
    return ToAgsFloat(self->FrameBorderSize);
}


void AgsImGuiStyle_SetItemSpacing(AgsImGuiStyle* self, AgsImVec2* itemSpacing){
    SetAgsImVec2(self->ItemSpacing, itemSpacing);
}

AgsImVec2* AgsImGuiStyle_GetItemSpacing(AgsImGuiStyle* self){
    return NewAgsImVec2(self->ItemSpacing);
}


void AgsImGuiStyle_SetItemInnerSpacing(AgsImGuiStyle* self, AgsImVec2* itemInnerSpacing){
    SetAgsImVec2(self->ItemInnerSpacing, itemInnerSpacing);
}

AgsImVec2* AgsImGuiStyle_GetItemInnerSpacing(AgsImGuiStyle* self){
    return NewAgsImVec2(self->ItemInnerSpacing);
}


void AgsImGuiStyle_SetTouchExtraPadding(AgsImGuiStyle* self, AgsImVec2* touchExtraPadding){
    SetAgsImVec2(self->TouchExtraPadding, touchExtraPadding);
}

AgsImVec2* AgsImGuiStyle_GetTouchExtraPadding(AgsImGuiStyle* self){
    return NewAgsImVec2(self->TouchExtraPadding);
}


void AgsImGuiStyle_SetIndentSpacing(AgsImGuiStyle* self, uint32_t indentSpacing){
    float f_indentSpacing = ToNormalFloat(indentSpacing);
    self->IndentSpacing = f_indentSpacing;
}

uint32_t AgsImGuiStyle_GetIndentSpacing(AgsImGuiStyle* self){
    return ToAgsFloat(self->IndentSpacing);
}


void AgsImGuiStyle_SetColumnsMinSpacing(AgsImGuiStyle* self, uint32_t columnsMinSpacing){
    float f_columnsMinSpacing = ToNormalFloat(columnsMinSpacing);
    self->ColumnsMinSpacing = f_columnsMinSpacing;
}

uint32_t AgsImGuiStyle_GetColumnsMinSpacing(AgsImGuiStyle* self){
    return ToAgsFloat(self->ColumnsMinSpacing);
}


void AgsImGuiStyle_SetScrollbarSize(AgsImGuiStyle* self, uint32_t scrollbarSize){
    float f_scrollbarSize = ToNormalFloat(scrollbarSize);
    self->ScrollbarSize = f_scrollbarSize;
}

uint32_t AgsImGuiStyle_GetScrollbarSize(AgsImGuiStyle* self){
    return ToAgsFloat(self->ScrollbarSize);
}


void AgsImGuiStyle_SetScrollbarRounding(AgsImGuiStyle* self, uint32_t scrollbarRounding){
    float f_scrollbarRounding = ToNormalFloat(scrollbarRounding);
    self->ScrollbarRounding = f_scrollbarRounding;
}

uint32_t AgsImGuiStyle_GetScrollbarRounding(AgsImGuiStyle* self){
    return ToAgsFloat(self->ScrollbarRounding);
}


void AgsImGuiStyle_SetGrabMinSize(AgsImGuiStyle* self, uint32_t grabMinSize){
    float f_grabMinSize = ToNormalFloat(grabMinSize);
    self->GrabMinSize = f_grabMinSize;
}

uint32_t AgsImGuiStyle_GetGrabMinSize(AgsImGuiStyle* self){
    return ToAgsFloat(self->GrabMinSize);
}


void AgsImGuiStyle_SetGrabRounding(AgsImGuiStyle* self, uint32_t grabRounding){
    float f_grabRounding = ToNormalFloat(grabRounding);
    self->GrabRounding = f_grabRounding;
}

uint32_t AgsImGuiStyle_GetGrabRounding(AgsImGuiStyle* self){
    return ToAgsFloat(self->GrabRounding);
}


void AgsImGuiStyle_SetTabRounding(AgsImGuiStyle* self, uint32_t tabRounding){
    float f_tabRounding = ToNormalFloat(tabRounding);
    self->TabRounding = f_tabRounding;
}

uint32_t AgsImGuiStyle_GetTabRounding(AgsImGuiStyle* self){
    return ToAgsFloat(self->TabRounding);
}


void AgsImGuiStyle_SetTabBorderSize(AgsImGuiStyle* self, uint32_t tabBorderSize){
    float f_tabBorderSize = ToNormalFloat(tabBorderSize);
    self->TabBorderSize = f_tabBorderSize;
}

uint32_t AgsImGuiStyle_GetTabBorderSize(AgsImGuiStyle* self){
    return ToAgsFloat(self->TabBorderSize);
}


void AgsImGuiStyle_SetTabMinWidthForUnselectedCloseButton(AgsImGuiStyle* self, uint32_t tabMinWidthForUnselectedCloseButton){
    float f_tabMinWidthForUnselectedCloseButton = ToNormalFloat(tabMinWidthForUnselectedCloseButton);
    self->TabMinWidthForUnselectedCloseButton = f_tabMinWidthForUnselectedCloseButton;
}

uint32_t AgsImGuiStyle_GetTabMinWidthForUnselectedCloseButton(AgsImGuiStyle* self){
    return ToAgsFloat(self->TabMinWidthForUnselectedCloseButton);
}


void AgsImGuiStyle_SetColorButtonPosition(AgsImGuiStyle* self, int colorButtonPosition){
    self->ColorButtonPosition = colorButtonPosition;
}

int AgsImGuiStyle_GetColorButtonPosition(AgsImGuiStyle* self){
    return  self->ColorButtonPosition;
}


void AgsImGuiStyle_SetButtonTextAlign(AgsImGuiStyle* self, AgsImVec2* buttonTextAlign){
    SetAgsImVec2(self->ButtonTextAlign, buttonTextAlign);
}

AgsImVec2* AgsImGuiStyle_GetButtonTextAlign(AgsImGuiStyle* self){
    return NewAgsImVec2(self->ButtonTextAlign);
}


void AgsImGuiStyle_SetSelectableTextAlign(AgsImGuiStyle* self, AgsImVec2* selectableTextAlign){
    SetAgsImVec2(self->SelectableTextAlign, selectableTextAlign);
}

AgsImVec2* AgsImGuiStyle_GetSelectableTextAlign(AgsImGuiStyle* self){
    return NewAgsImVec2(self->SelectableTextAlign);
}


void AgsImGuiStyle_SetDisplayWindowPadding(AgsImGuiStyle* self, AgsImVec2* displayWindowPadding){
    SetAgsImVec2(self->DisplayWindowPadding, displayWindowPadding);
}

AgsImVec2* AgsImGuiStyle_GetDisplayWindowPadding(AgsImGuiStyle* self){
    return NewAgsImVec2(self->DisplayWindowPadding);
}


void AgsImGuiStyle_SetDisplaySafeAreaPadding(AgsImGuiStyle* self, AgsImVec2* displaySafeAreaPadding){
    SetAgsImVec2(self->DisplaySafeAreaPadding, displaySafeAreaPadding);
}

AgsImVec2* AgsImGuiStyle_GetDisplaySafeAreaPadding(AgsImGuiStyle* self){
    return NewAgsImVec2(self->DisplaySafeAreaPadding);
}


void AgsImGuiStyle_SetMouseCursorScale(AgsImGuiStyle* self, uint32_t mouseCursorScale){
    float f_mouseCursorScale = ToNormalFloat(mouseCursorScale);
    self->MouseCursorScale = f_mouseCursorScale;
}

uint32_t AgsImGuiStyle_GetMouseCursorScale(AgsImGuiStyle* self){
    return ToAgsFloat(self->MouseCursorScale);
}


void AgsImGuiStyle_SetAntiAliasedLines(AgsImGuiStyle* self, int antiAliasedLines){
    self->AntiAliasedLines = ToNormalBool(antiAliasedLines);
}

int AgsImGuiStyle_GetAntiAliasedLines(AgsImGuiStyle* self){
    return ToAgsBool(self->AntiAliasedLines);
}


void AgsImGuiStyle_SetAntiAliasedFill(AgsImGuiStyle* self, int antiAliasedFill){
    self->AntiAliasedFill = ToNormalBool(antiAliasedFill);
}

int AgsImGuiStyle_GetAntiAliasedFill(AgsImGuiStyle* self){
    return ToAgsBool(self->AntiAliasedFill);
}


void AgsImGuiStyle_SetCurveTessellationTol(AgsImGuiStyle* self, uint32_t curveTessellationTol){
    float f_curveTessellationTol = ToNormalFloat(curveTessellationTol);
    self->CurveTessellationTol = f_curveTessellationTol;
}

uint32_t AgsImGuiStyle_GetCurveTessellationTol(AgsImGuiStyle* self){
    return ToAgsFloat(self->CurveTessellationTol);
}


void AgsImGuiStyle_SetCircleSegmentMaxError(AgsImGuiStyle* self, uint32_t circleSegmentMaxError){
    float f_circleSegmentMaxError= ToNormalFloat(circleSegmentMaxError);
    self->CircleSegmentMaxError = f_circleSegmentMaxError;
}

uint32_t AgsImGuiStyle_GetCircleSegmentMaxError(AgsImGuiStyle* self){
    return ToAgsFloat(self->CircleSegmentMaxError);
}


void AgsImGuiStyle_SetColors(AgsImGuiStyle* self, int i, AgsImVec4* color){
    if ((i < 0) || (i >= ImGuiCol_COUNT))
        return;

    SetAgsImVec4(self->Colors[i], color);
}

AgsImVec4* AgsImGuiStyle_GetColors(AgsImGuiStyle* self, int i){
    if ((i < 0) || (i >= ImGuiCol_COUNT))
        return nullptr;

    return NewAgsImVec4(self->Colors[i]);
}



// -- end AgsImGuiStyle

AgsImGuiStyle* AgsImGui_GetStyle(){
    ImGuiStyle &imGuiStyle = ImGui::GetStyle();
    return NewAgsImGuiStyle(imGuiStyle);
}

void AgsImGui_SetStyle(AgsImGuiStyle* agsImGuiStyle){
    ImGuiStyle &imGuiStyle = ImGui::GetStyle();
    SetAgsImGuiStyle(imGuiStyle, agsImGuiStyle);
}

void AgsImGui_NewFrame(){
	if (!screen.initialized) return;

	if (screen.driver == Screen::Driver::eOpenGL) {

	}
	if (screen.driver == Screen::Driver::eDirectx9) {
		ImGui_ImplDX9_NewFrame();
	}
	if (screen.driver == Screen::Driver::eSoftware) {
		ImGui_ImplSoftraster_NewFrame();
	}
    ImGui::NewFrame();
	has_new_frame = true;
}

void AgsImGui_EndFrame(){
    ImGui::EndFrame();
}

void AgsImGui_Render(){
	ImGui::Render();
    has_new_render = true;
}

//int AgsImGui_GetDrawData(){
//    return 0;
//    if(screen.driver == Screen::Driver::eSoftware) {
//        return ImGui_ImplSoftraster_GetSprite();
//    }
//}

const char* AgsImGui_GetVersion(){
    return engine->CreateScriptString(ImGui::GetVersion());
}

int AgsImGui_BeginWindow(const char* name, int has_close_button, int32 flags = 0){
    bool p_open = true;
    bool not_collapsed = ImGui::Begin(name, (has_close_button != 0 ? &p_open : nullptr), flags);

    if(p_open && !not_collapsed) return 4;
    else if(!p_open && not_collapsed) return 3;
    else if(!p_open && !not_collapsed) return 2;
    return 1;
}

void AgsImGui_EndWindow(){
    ImGui::End();
}

void AgsImGui_ShowDemoWindow(){
    ImGui::ShowDemoWindow();
}

void AgsImGui_ShowAboutWindow(){
    ImGui::ShowAboutWindow();
}

void AgsImGui_ShowMetricsWindow(){
    ImGui::ShowMetricsWindow();
}

void AgsImGui_ShowUserGuide(){
    ImGui::ShowUserGuide();
}

void AgsImGui_StyleColorsDark(){
    ImGui::StyleColorsDark();
}

void AgsImGui_StyleColorsClassic(){
    ImGui::StyleColorsClassic();
}

void AgsImGui_StyleColorsLight(){
    ImGui::StyleColorsLight();
}

int AgsImGui_BeginChild(const char* str_id, int width = 0, int height = 0, bool border = false, int32 flags = 0){
    return ToAgsBool(ImGui::BeginChild(str_id,ImVec2((float) width,(float) height), border, flags));
}

void AgsImGui_EndChild(){
    ImGui::EndChild();
}

void AgsImGui_SetNextWindowPos(int position_x, int position_y, int cond, uint32_t pivot_x, uint32_t pivot_y){
    float f_pivot_x = ToNormalFloat(pivot_x);
    float f_pivot_y = ToNormalFloat(pivot_y);

    ImGui::SetNextWindowPos(ImVec2((float) position_x, (float)position_y), cond, ImVec2(f_pivot_x,f_pivot_y));
}

void AgsImGui_SetNextWindowSize(int width, int height, int cond){
    ImGui::SetNextWindowPos(ImVec2((float) width, (float)height), cond);
}

void AgsImGui_SetNextWindowSizeConstraints(int min_width, int min_height, int max_width, int max_height){
    ImGui::SetNextWindowSizeConstraints(ImVec2((float) min_width, (float)min_height),ImVec2((float) max_width, (float)max_height));
}

void AgsImGui_SetNextWindowContentSize(int width, int height){
    ImGui::SetNextWindowContentSize(ImVec2((float) width, (float)height));
}

void AgsImGui_SetNextWindowCollapsed(int collapsed, int cond){
    ImGui::SetNextWindowCollapsed(collapsed != 0, cond);
}

void AgsImGui_SetNextWindowFocus(){
    ImGui::SetNextWindowFocus();
}

void AgsImGui_SetNextWindowBgAlpha(float alpha){
    ImGui::SetNextWindowBgAlpha(alpha);
}

int AgsImGui_IsWindowAppearing(){
    return ToAgsBool(ImGui::IsWindowAppearing());
}

int AgsImGui_IsWindowCollapsed(){
    return ToAgsBool(ImGui::IsWindowCollapsed());
}

int AgsImGui_IsWindowFocused(int flags){
    return ToAgsBool(ImGui::IsWindowFocused(flags));
}

int AgsImGui_IsWindowHovered(int flags){
    return ToAgsBool(ImGui::IsWindowHovered(flags));
}

int AgsImGui_IsItemHovered(int flags){
    return ToAgsBool(ImGui::IsItemHovered(flags));
}

int AgsImGui_IsItemActive(){
    return ToAgsBool(ImGui::IsItemActive());
}

int AgsImGui_IsItemFocused(){
    return ToAgsBool(ImGui::IsItemFocused());
}

int AgsImGui_IsItemVisible(){
    return ToAgsBool(ImGui::IsItemVisible());
}

int AgsImGui_IsItemEdited(){
    return ToAgsBool(ImGui::IsItemEdited());
}

int AgsImGui_IsItemActivated(){
    return ToAgsBool(ImGui::IsItemActivated());
}

int AgsImGui_IsItemDeactivated(){
    return ToAgsBool(ImGui::IsItemDeactivated());
}

int AgsImGui_IsItemDeactivatedAfterEdit(){
    return ToAgsBool(ImGui::IsItemDeactivatedAfterEdit());
}

int AgsImGui_IsItemToggledOpen(){
    return ToAgsBool(ImGui::IsItemToggledOpen());
}

int AgsImGui_IsAnyItemHovered(){
    return ToAgsBool(ImGui::IsAnyItemHovered());
}

int AgsImGui_IsAnyItemActive(){
    return ToAgsBool(ImGui::IsAnyItemActive());
}

int AgsImGui_IsAnyItemFocused(){
    return ToAgsBool(ImGui::IsAnyItemFocused());
}

void AgsImGui_SetKeyboardFocusHere(int offset){
    ImGui::SetKeyboardFocusHere(offset);
}

void AgsImGui_SetItemDefaultFocus(){
    ImGui::SetItemDefaultFocus();
}

void AgsImGui_Separator () {
    ImGui::Separator();
}

void AgsImGui_SameLine (uint32_t offset_from_start_x, uint32_t spacing) {
    float f_offset_from_start_x = ToNormalFloat(offset_from_start_x);
    float f_spacing = ToNormalFloat(spacing);
    ImGui::SameLine(f_offset_from_start_x,f_spacing);
}

void AgsImGui_NewLine () {
    ImGui::NewLine();
}

void AgsImGui_Spacing () {
    ImGui::Spacing();
}

void AgsImGui_Dummy (uint32_t width, uint32_t height) {
    float f_width = ToNormalFloat(width);
    float f_height = ToNormalFloat(height);
    ImGui::Dummy(ImVec2(f_width,f_height));
}

void AgsImGui_Indent (uint32_t indent_w) {
    float f_indent_w = ToNormalFloat(indent_w);
    ImGui::Indent(f_indent_w);
}

void AgsImGui_Unindent (uint32_t indent_w) {
    float f_indent_w = ToNormalFloat(indent_w);
    ImGui::Unindent(f_indent_w);
}

void AgsImGui_PushID(const char* str_id) {
    ImGui::PushID(str_id);
}

void AgsImGui_PopID() {
    ImGui::PopID();
}

void AgsImGui_Text(const char* text){
    ImGui::Text(text);
}

void AgsImGui_TextColored(int color, const char* text){
    ImGui::TextColored(FromAgsColors(color), text);
}

void AgsImGui_TextDisabled(const char* text){
    ImGui::TextDisabled(text);
}

void AgsImGui_TextWrapped(const char* text){
    ImGui::TextWrapped(text);
}

void AgsImGui_LabelText(const char* label, const char* text){
    ImGui::LabelText(label, text);
}

void AgsImGui_BulletText(const char* text){
    ImGui::BulletText(text);
}

int AgsImGui_Button(const char* label, int width, int height){
    return ToAgsBool(ImGui::Button(label, ImVec2((float) width, (float) height)));
}

int AgsImGui_SmallButton(const char* label){
    return ToAgsBool(ImGui::SmallButton(label));
}

void AgsImGui_Image(int sprite_id){
    int sprite_width = engine->GetSpriteWidth(sprite_id);
    int sprite_height = engine->GetSpriteHeight(sprite_id);
    if(screen.driver == Screen::Driver::eSoftware) {
        ImGui::Image(ImGui_ImplSoftraster_SpriteIDToTexture(sprite_id),
                     ImVec2((float) sprite_width, (float) sprite_height));
    }
    if(screen.driver == Screen::Driver::eDirectx9) {
        ImGui::Image(ImGui_ImplDX9_SpriteIDToTexture(sprite_id),
                     ImVec2((float) sprite_width, (float) sprite_height));
    }
}

int AgsImGui_ImageButton(int sprite_id){
    int sprite_width = engine->GetSpriteWidth(sprite_id);
    int sprite_height = engine->GetSpriteHeight(sprite_id);
    if(screen.driver == Screen::Driver::eSoftware) {
        return ToAgsBool(ImGui::ImageButton(ImGui_ImplSoftraster_SpriteIDToTexture(sprite_id),
                                  ImVec2((float) sprite_width, (float) sprite_height)));
    }
    if(screen.driver == Screen::Driver::eDirectx9) {
        return ToAgsBool(ImGui::ImageButton(ImGui_ImplDX9_SpriteIDToTexture(sprite_id),
                     ImVec2((float) sprite_width, (float) sprite_height)));
    }
    return 0;
}

int AgsImGui_ArrowButton(const char* str_id, int32 dir){
    return ToAgsBool(ImGui::ArrowButton(str_id, dir));
}

int AgsImGui_Checkbox(const char* label, int v){
    bool value = v != 0;
    return ToAgsBool(ImGui::Checkbox(label, &value));
}

int AgsImGui_RadioButton(const char* label, bool active){
    return ToAgsBool(ImGui::RadioButton(label, active));
}

void AgsImGui_Bullet(){
    ImGui::Bullet();
}

int AgsImGui_Selectable(const char* label, int selected, int flags, int width, int height){
    return ToAgsBool(ImGui::Selectable(label, selected != 0, flags, ImVec2((float) width, (float) height)));
}

int AgsImGui_BeginCombo(const char* name, const char* preview_value, int32 flags = 0){
    return ToAgsBool(ImGui::BeginCombo(name, preview_value, flags));
}

void AgsImGui_EndCombo(){
    ImGui::EndCombo();
}

uint32_t AgsImGui_DragFloat(const char* label, uint32_t value, uint32_t v_min, uint32_t v_max, uint32_t speed, const char* format){
    std::string format_string =  "%.3f";
    std::string empty_string =  "";
    float f_value = ToNormalFloat(value);
    float f_speed = ToNormalFloat(speed);
    float f_v_min = ToNormalFloat(v_min);
    float f_v_max = ToNormalFloat(v_max);
    if(f_speed == 0.0) f_speed = 1.0;
    if(format == nullptr) format = format_string.c_str();
    if(label == nullptr) label = empty_string.c_str();

    ImGui::DragFloat(label, &f_value, f_speed, f_v_min, f_v_max, format);
    return ToAgsFloat(f_value);
}

int AgsImgui_DragInt(const char* label, int value, int v_min, int v_max, uint32_t speed, const char* format){
    std::string format_string =  "%d";
    std::string empty_string =  "";
    float f_speed = ToNormalFloat(speed);
    if(f_speed == 0.0) f_speed = 1.0;
    if(format == nullptr) format = format_string.c_str();
    if(label == nullptr) label = empty_string.c_str();

    int ret_value = value;
    ImGui::DragInt(label, &ret_value, f_speed, v_min, v_max, format);
    return ret_value;
}

uint32_t AgsImGui_SliderFloat(const char* label, uint32_t value, uint32_t v_min, uint32_t v_max, const char* format){
    std::string format_string =  "%.3f";
    std::string empty_string =  "";
    float f_value = ToNormalFloat(value);
    float f_v_min = ToNormalFloat(v_min);
    float f_v_max = ToNormalFloat(v_max);
    if(format == nullptr) format = format_string.c_str();
    if(label == nullptr) label = empty_string.c_str();

    ImGui::SliderFloat(label, &f_value, f_v_min, f_v_max, format);
    return ToAgsFloat(f_value);
}

int AgsImgui_SliderInt(const char* label, int value, int v_min, int v_max, const char* format){
    std::string format_string =  "%d";
    std::string empty_string =  "";
    if(format == nullptr) format = format_string.c_str();
    if(label == nullptr) label = empty_string.c_str();

    int ret_value = value;
    ImGui::SliderInt(label, &ret_value, v_min, v_max, format);
    return ret_value;
}

const char* AgsImgui_InputText(const char* label, char* buf, int buf_size, int flags) {
    if(buf_size <= 1) engine->AbortGame("Buffer size can't be smaller than 2");
    if(strlen(buf) > buf_size) engine->AbortGame("Buffer size smaller than buffer string on Input Text");

    char * resized_buffer = new char [buf_size];
    std::strcpy(resized_buffer, buf);
    bool changed =  ImGui::InputText(label,resized_buffer, buf_size,flags);
    if(changed) {
        return  engine->CreateScriptString(resized_buffer);
    }
    delete[] resized_buffer;
    return nullptr;
}

const char* AgsImgui_InputTextMultiline(const char* label, const char* buf, int buf_size, int width, int height, int flags) {
    if(buf_size <= 1) engine->AbortGame("Buffer size can't be smaller than 2");
    if(strlen(buf) > buf_size) engine->AbortGame("Buffer size smaller than buffer string on Input Text");

    char * resized_buffer = new char [buf_size];
    std::strcpy(resized_buffer, buf);
    bool changed =  ImGui::InputTextMultiline(label,resized_buffer, buf_size, ImVec2((float) width, (float) height), flags);
    if(changed) {
        return  engine->CreateScriptString(resized_buffer);
    }
    delete[] resized_buffer;
    return nullptr;
}

const char* AgsImgui_InputTextWithHint(const char* label, const char* hint, const char* buf, int buf_size, int flags) {
    if(buf_size <= 1) engine->AbortGame("Buffer size can't be smaller than 2");
    if(strlen(buf) > buf_size) engine->AbortGame("Buffer size smaller than buffer string on Input Text");

    char * resized_buffer = new char [buf_size];
    std::strcpy(resized_buffer, buf);
    bool changed =  ImGui::InputTextWithHint(label, hint, resized_buffer, buf_size,flags);
    if(changed) {
        return  engine->CreateScriptString(resized_buffer);
    }
    delete[] resized_buffer;
    return nullptr;
}

int AgsImGui_TreeNode(const char* label) {
    return ToAgsBool(ImGui::TreeNode(label));
}

int AgsImGui_TreeNodeWithID(const char* str_id, const char* text) {
    return ToAgsBool(ImGui::TreeNode(str_id,text));
}

int AgsImGui_TreeNodeV(const char* str_id, const char* text) {
    return ToAgsBool(ImGui::TreeNodeV(str_id,text,0));
}

int AgsImGui_TreeNodeEx(const char* label, int flags) {
    return ToAgsBool(ImGui::TreeNodeEx(label,flags));
}

int AgsImGui_TreeNodeExWithID(const char* str_id, int flags, const char* text) {
    return ToAgsBool(ImGui::TreeNodeEx(str_id,flags,text,0));
}

int AgsImGui_TreeNodeExV(const char* str_id, int flags, const char* text) {
    return ToAgsBool(ImGui::TreeNodeExV(str_id,flags,text,0));
}

void AgsImGui_TreePush(const char* str_id) {
    ImGui::TreePush(str_id);
}

void AgsImGui_TreePop() {
    ImGui::TreePop();
}

uint32_t AgsImGui_GetTreeNodeToLabelSpacing() {
    return ToAgsFloat(ImGui::GetTreeNodeToLabelSpacing());
}

int AgsImGui_CollapsingHeader(const char* label, int flags) {
    return ToAgsBool( ImGui::CollapsingHeader(label, flags));
}

void AgsImGui_SetNextItemOpen(int is_open, int cond) {
    ImGui::SetNextItemOpen(is_open != 0, cond);
}

int AgsImGui_BeginListBox(const char* name, int items_count, int height_in_items = -1){
    return ToAgsBool(ImGui::ListBoxHeader(name,items_count,height_in_items));
}

void AgsImGui_EndListBox(){
    ImGui::ListBoxFooter();
}

void AgsImGui_BeginTooltip(){
    ImGui::BeginTooltip();
}

void AgsImGui_EndTooltip(){
    ImGui::EndTooltip();
}

void AgsImGui_SetTooltip(const char * text){
    ImGui::SetTooltip(text);
}

void AgsImGui_OpenPopup(const char* str_id) {
    ImGui::OpenPopup(str_id);
}

int AgsImGui_BeginPopup(const char* str_id, int flags) {
    return ToAgsBool(ImGui::BeginPopup(str_id, flags));
}

int AgsImGui_BeginPopupModal(const char* name, bool has_close_button, int flags) {
    bool p_open = true;
    return ToAgsBool(ImGui::BeginPopupModal(name,(has_close_button != 0 ? &p_open : nullptr),flags));
}

void AgsImGui_EndPopup() {
    ImGui::EndPopup();
}

int AgsImGui_IsPopupOpen(const char* str_id) {
    return ToAgsBool(ImGui::IsPopupOpen(str_id));
}

void AgsImGui_CloseCurrentPopup() {
    ImGui::CloseCurrentPopup();
}

int AgsImGui_BeginTabBar(const char * str_id, int flags){
    return ToAgsBool(ImGui::BeginTabBar(str_id, flags));
}

void AgsImGui_EndTabBar(){
    ImGui::EndTabBar();
}

int AgsImGui_BeginTabItem(const char * label,  bool has_close_button, int flags){
    bool p_open = false;
    return ToAgsBool(ImGui::BeginTabItem(label, (has_close_button != 0 ? &p_open : nullptr), flags));
}

void AgsImGui_EndTabItem(){
    ImGui::EndTabItem();
}

void AgsImGui_SetTabItemClosed(const char * tab_or_docked_window_label){
    ImGui::SetTabItemClosed(tab_or_docked_window_label);
}


int AgsImGui_BeginMenuBar(){
    return ToAgsBool(ImGui::BeginMenuBar());
}

void AgsImGui_EndMenuBar(){
    ImGui::EndMenuBar();
}


int AgsImGui_BeginMainMenuBar(){
    return ToAgsBool(ImGui::BeginMainMenuBar());
}

void AgsImGui_EndMainMenuBar(){
    ImGui::EndMainMenuBar();
}

int AgsImGui_BeginMenu(const char* name, int enabled){
    return ToAgsBool(ImGui::BeginMenu(name,enabled != 0));
}

void AgsImGui_EndMenu(){
    ImGui::EndMenu();
}

int AgsImGui_MenuItem(const char* label, const char* shortcut, bool selected = false, bool enabled = true){
    return  ToAgsBool(ImGui::MenuItem(label, shortcut, &selected, enabled));
}

void AgsImGui_DoCaptureMouse(int want_capture_mouse){
    ImGui::CaptureMouseFromApp(want_capture_mouse != 0);
}

void AgsImGui_DoCaptureKeyboard(int want_capture_keyboard){
    ImGui::CaptureKeyboardFromApp(want_capture_keyboard != 0);
}


void AgsImGui_ValueBool(const char* prefix, int value){
    if(prefix == nullptr)  {
        std::string empty = "";
        prefix = empty.c_str();
    }
    ImGui::Value(prefix,value != 0);
}

void AgsImGui_ValueInt(const char* prefix, int value){
    if(prefix == nullptr)  {
        std::string empty = "";
        prefix = empty.c_str();
    }
    ImGui::Value(prefix,value);
}

void AgsImGui_ValueFloat(const char* prefix, uint32_t value){
    if(prefix == nullptr) {
        std::string empty = "";
        prefix = empty.c_str();
    }
    ImGui::Value(prefix,ToNormalFloat(value));
}

void AgsImGui_DoMouseWheel(int wheel_dir) {
	ImGuiIO& io = ImGui::GetIO();

	if (wheel_dir == 0) io.MouseWheelH -= 1;
	else if (wheel_dir == 1) io.MouseWheelH += 1;
	else if (wheel_dir == 2) io.MouseWheel -= 1;
	else if (wheel_dir == 3) io.MouseWheel += 1;
}


void AgsImGuiHelper_SetClipboarText(const char* text) {
	clip::set_text(text);
}

const char* AgsImGuiHelper_GetClipboarText() {
	if (!clip::has(clip::text_format())) {
		return engine->CreateScriptString("");
	}

	g_ClipboardTextData.clear();
	clip::get_text(g_ClipboardTextData);
	return engine->CreateScriptString(g_ClipboardTextData.c_str());
}

void AgsImGuiHelper_SetClipboarImage(int sprite_id) {
	BITMAP* engineSprite = engine->GetSpriteGraphic(sprite_id);
	int sprite_width = engine->GetSpriteWidth(sprite_id);
	int sprite_height = engine->GetSpriteHeight(sprite_id);

	unsigned char** charbuffer = engine->GetRawBitmapSurface(engineSprite);
	uint32_t** longbuffer = (uint32_t * *)charbuffer;
	char* single_char_buffer = (char*)charbuffer;
	
	clip::image_spec spec;
	spec.width = sprite_width;
	spec.height = sprite_height;
	spec.bits_per_pixel = 32;
	spec.bytes_per_row = spec.width * 4;
	spec.red_mask = 0x00ff0000;
	spec.green_mask = 0xff00;
	spec.blue_mask = 0xff;
	spec.alpha_mask = 0xff000000;
	spec.red_shift = 16;
	spec.green_shift = 8;
	spec.blue_shift = 0;
	spec.alpha_shift = 24;
	clip::image img(single_char_buffer, spec);
	clip::set_image(img);
	engine->ReleaseBitmapSurface(engineSprite);
}

int AgsImGuiHelper_GetClipboarImage() {
	if (!clip::has(clip::image_format())) {
		// no image on clipboard
		return 0;
	}

	clip::image img;
	if (!clip::get_image(img)) {
		// Error getting image from clipboard
		return 0;
	}

	clip::image_spec spec = img.spec();

	int sprite_width = spec.width;
	int sprite_height = spec.height;
	int color_depth = spec.bits_per_pixel;

	if (color_depth != 32) {
		return 0;
	}

	int sprite_id = engine->CreateDynamicSprite(color_depth, sprite_width, sprite_height);
	BITMAP* engineSprite = engine->GetSpriteGraphic(sprite_id);

	unsigned char** charbuffer = engine->GetRawBitmapSurface(engineSprite);
	uint32_t** longbuffer = (uint32_t * *)charbuffer;

	for (int ix = 0; ix < sprite_width; ix++) {
		for (int iy = 0; iy < sprite_height; iy++) {
			
			longbuffer[iy][ix] = ((uint32_t * *)(img.data()))[iy][ix];
		}
	}

	engine->ReleaseBitmapSurface(engineSprite);
	return sprite_id;
}


	void AGS_EngineStartup(IAGSEngine *lpEngine)
	{
		engine = lpEngine;

		// Make sure it's got the version with the features we need
		if (engine->version < MIN_ENGINE_VERSION)
			engine->AbortGame("Plugin needs engine version " STRINGIFY(MIN_ENGINE_VERSION) " or newer.");

        engine->AddManagedObjectReader(AgsImVec2Interface::name, &AgsImVec2_Reader);
        engine->AddManagedObjectReader(AgsImVec4Interface::name, &AgsImVec4_Reader);
        engine->AddManagedObjectReader(AgsImGuiStyleInterface::name, &AgsImGuiStyle_Reader);

		//register functions
        if(screen.driver == Screen::Driver::eOpenGL) {

        }
        if(screen.driver == Screen::Driver::eDirectx9) {
			
        }
        if(screen.driver == Screen::Driver::eSoftware) {
            ImGui_ImplSoftraster_InitializeEngine(engine);
        }

        context = ImGui::CreateContext();

        if(screen.driver == Screen::Driver::eOpenGL) {

        }
        if(screen.driver == Screen::Driver::eDirectx9) {

        }
        if(screen.driver == Screen::Driver::eSoftware) {
            ImGui_ImplSoftraster_Init(&software_renderer_screen);
        }

        ImGuiStyle& style = ImGui::GetStyle();
        style.AntiAliasedLines = false;
        style.AntiAliasedFill = false;
        style.WindowRounding = 0.0f;
        style.Alpha = 1.0f;

        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight | ImFontAtlasFlags_NoMouseCursors;

		if (screen.driver == Screen::Driver::eSoftware) {
			uint8_t* pixels;
			int width, height;
			io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
			fontAtlas.init(width, height, (alpha8_t*)pixels);
			io.Fonts->TexID = &fontAtlas;
		}

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        io.KeyMap[ImGuiKey_Tab] = eAGSKeyCodeTab;
        io.KeyMap[ImGuiKey_LeftArrow] = eAGSKeyCodeLeftArrow;
        io.KeyMap[ImGuiKey_RightArrow] = eAGSKeyCodeRightArrow;
        io.KeyMap[ImGuiKey_UpArrow] = eAGSKeyCodeUpArrow;
        io.KeyMap[ImGuiKey_DownArrow] = eAGSKeyCodeDownArrow;
        io.KeyMap[ImGuiKey_PageUp] = eAGSKeyCodePageUp;
        io.KeyMap[ImGuiKey_PageDown] = eAGSKeyCodePageDown;
        io.KeyMap[ImGuiKey_Home] = eAGSKeyCodeHome;
        io.KeyMap[ImGuiKey_End] = eAGSKeyCodeEnd;
        io.KeyMap[ImGuiKey_Insert] = eAGSKeyCodeInsert;
        io.KeyMap[ImGuiKey_Delete] = eAGSKeyCodeDelete;
        io.KeyMap[ImGuiKey_Backspace] = eAGSKeyCodeBackspace;
        io.KeyMap[ImGuiKey_Space] = eAGSKeyCodeSpace;
        io.KeyMap[ImGuiKey_Enter] = eAGSKeyCodeReturn;
        io.KeyMap[ImGuiKey_Escape] = eAGSKeyCodeEscape;
        io.KeyMap[ImGuiKey_KeyPadEnter] = eAGSKeyCodeReturn;
        io.KeyMap[ImGuiKey_A] = eAGSKeyCodeA;
        io.KeyMap[ImGuiKey_C] = eAGSKeyCodeC;
        io.KeyMap[ImGuiKey_V] = eAGSKeyCodeV;
        io.KeyMap[ImGuiKey_X] = eAGSKeyCodeX;
        io.KeyMap[ImGuiKey_Y] = eAGSKeyCodeY;
        io.KeyMap[ImGuiKey_Z] = eAGSKeyCodeZ;
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

        io.SetClipboardTextFn = ImGui_ImplClip_SetClipboardText;
        io.GetClipboardTextFn = ImGui_ImplClip_GetClipboardText;
        io.ClipboardUserData = NULL;

        Mouse_IsButtonDown = (SCAPI_MOUSE_ISBUTTONDOWN) engine->GetScriptFunctionAddress("Mouse::IsButtonDown^1");

        engine->RegisterScriptFunction("ImVec4::Create^4", (void*)AgsImVec4_Create);
        engine->RegisterScriptFunction("ImVec4::set_X", (void*)AgsImVec4_SetX);
        engine->RegisterScriptFunction("ImVec4::get_X", (void*)AgsImVec4_GetX);
        engine->RegisterScriptFunction("ImVec4::set_Y", (void*)AgsImVec4_SetY);
        engine->RegisterScriptFunction("ImVec4::get_Y", (void*)AgsImVec4_GetY);
        engine->RegisterScriptFunction("ImVec4::set_Z", (void*)AgsImVec4_SetZ);
        engine->RegisterScriptFunction("ImVec4::get_Z", (void*)AgsImVec4_GetZ);
        engine->RegisterScriptFunction("ImVec4::set_W", (void*)AgsImVec4_SetW);
        engine->RegisterScriptFunction("ImVec4::get_W", (void*)AgsImVec4_GetW);
        engine->RegisterScriptFunction("ImVec4::Length^0", (void*)AgsImVec4_Length);
        engine->RegisterScriptFunction("ImVec4::SquaredLength^0", (void*)AgsImVec4_SquaredLength);
        engine->RegisterScriptFunction("ImVec4::Add^1", (void*)AgsImVec4_Add);
        engine->RegisterScriptFunction("ImVec4::Sub^1", (void*)AgsImVec4_Sub);
        engine->RegisterScriptFunction("ImVec4::Scale^1", (void*)AgsImVec4_Scale);

        engine->RegisterScriptFunction("ImVec2::Create^2", (void*)AgsImVec2_Create);
        engine->RegisterScriptFunction("ImVec2::set_X", (void*)AgsImVec2_SetX);
        engine->RegisterScriptFunction("ImVec2::get_X", (void*)AgsImVec2_GetX);
        engine->RegisterScriptFunction("ImVec2::set_Y", (void*)AgsImVec2_SetY);
        engine->RegisterScriptFunction("ImVec2::get_Y", (void*)AgsImVec2_GetY);
        engine->RegisterScriptFunction("ImVec2::Length^0", (void*)AgsImVec2_Length);
        engine->RegisterScriptFunction("ImVec2::SquaredLength^0", (void*)AgsImVec2_SquaredLength);
        engine->RegisterScriptFunction("ImVec2::Add^1", (void*)AgsImVec2_Add);
        engine->RegisterScriptFunction("ImVec2::Sub^1", (void*)AgsImVec2_Sub);
        engine->RegisterScriptFunction("ImVec2::Scale^1", (void*)AgsImVec2_Scale);

        engine->RegisterScriptFunction("ImGuiStyle::set_Alpha", (void*)AgsImGuiStyle_SetAlpha);
        engine->RegisterScriptFunction("ImGuiStyle::get_Alpha", (void*)AgsImGuiStyle_GetAlpha);
        engine->RegisterScriptFunction("ImGuiStyle::set_WindowPadding", (void*)AgsImGuiStyle_SetWindowPadding);
        engine->RegisterScriptFunction("ImGuiStyle::get_WindowPadding", (void*)AgsImGuiStyle_GetWindowPadding);
        engine->RegisterScriptFunction("ImGuiStyle::set_WindowRounding", (void*)AgsImGuiStyle_SetWindowRounding);
        engine->RegisterScriptFunction("ImGuiStyle::get_WindowRounding", (void*)AgsImGuiStyle_GetWindowRounding);
        engine->RegisterScriptFunction("ImGuiStyle::set_WindowBorderSize", (void*)AgsImGuiStyle_SetWindowBorderSize);
        engine->RegisterScriptFunction("ImGuiStyle::get_WindowBorderSize", (void*)AgsImGuiStyle_GetWindowBorderSize);
        engine->RegisterScriptFunction("ImGuiStyle::set_WindowMinSize", (void*)AgsImGuiStyle_SetWindowMinSize);
        engine->RegisterScriptFunction("ImGuiStyle::get_WindowMinSize", (void*)AgsImGuiStyle_GetWindowMinSize);
        engine->RegisterScriptFunction("ImGuiStyle::set_WindowTitleAlign", (void*)AgsImGuiStyle_SetWindowTitleAlign);
        engine->RegisterScriptFunction("ImGuiStyle::get_WindowTitleAlign", (void*)AgsImGuiStyle_GetWindowTitleAlign);
        engine->RegisterScriptFunction("ImGuiStyle::set_WindowMenuButtonPosition", (void*)AgsImGuiStyle_SetWindowMenuButtonPosition);
        engine->RegisterScriptFunction("ImGuiStyle::get_WindowMenuButtonPosition", (void*)AgsImGuiStyle_GetWindowMenuButtonPosition);
        engine->RegisterScriptFunction("ImGuiStyle::set_ChildRounding", (void*)AgsImGuiStyle_SetChildRounding);
        engine->RegisterScriptFunction("ImGuiStyle::get_ChildRounding", (void*)AgsImGuiStyle_GetChildRounding);
        engine->RegisterScriptFunction("ImGuiStyle::set_ChildBorderSize", (void*)AgsImGuiStyle_SetChildBorderSize);
        engine->RegisterScriptFunction("ImGuiStyle::get_ChildBorderSize", (void*)AgsImGuiStyle_GetChildBorderSize);
        engine->RegisterScriptFunction("ImGuiStyle::set_PopupRounding", (void*)AgsImGuiStyle_SetPopupRounding);
        engine->RegisterScriptFunction("ImGuiStyle::get_PopupRounding", (void*)AgsImGuiStyle_GetPopupRounding);
        engine->RegisterScriptFunction("ImGuiStyle::set_PopupBorderSize", (void*)AgsImGuiStyle_SetPopupBorderSize);
        engine->RegisterScriptFunction("ImGuiStyle::get_PopupBorderSize", (void*)AgsImGuiStyle_GetPopupBorderSize);
        engine->RegisterScriptFunction("ImGuiStyle::set_FramePadding", (void*)AgsImGuiStyle_SetFramePadding);
        engine->RegisterScriptFunction("ImGuiStyle::get_FramePadding", (void*)AgsImGuiStyle_GetFramePadding);
        engine->RegisterScriptFunction("ImGuiStyle::set_FrameRounding", (void*)AgsImGuiStyle_SetFrameRounding);
        engine->RegisterScriptFunction("ImGuiStyle::get_FrameRounding", (void*)AgsImGuiStyle_GetFrameRounding);
        engine->RegisterScriptFunction("ImGuiStyle::set_FrameBorderSize", (void*)AgsImGuiStyle_SetFrameBorderSize);
        engine->RegisterScriptFunction("ImGuiStyle::get_FrameBorderSize", (void*)AgsImGuiStyle_GetFrameBorderSize);
        engine->RegisterScriptFunction("ImGuiStyle::set_ItemSpacing", (void*)AgsImGuiStyle_SetItemSpacing);
        engine->RegisterScriptFunction("ImGuiStyle::get_ItemSpacing", (void*)AgsImGuiStyle_GetItemSpacing);
        engine->RegisterScriptFunction("ImGuiStyle::set_ItemInnerSpacing", (void*)AgsImGuiStyle_SetItemInnerSpacing);
        engine->RegisterScriptFunction("ImGuiStyle::get_ItemInnerSpacing", (void*)AgsImGuiStyle_GetItemInnerSpacing);
        engine->RegisterScriptFunction("ImGuiStyle::set_TouchExtraPadding", (void*)AgsImGuiStyle_SetTouchExtraPadding);
        engine->RegisterScriptFunction("ImGuiStyle::get_TouchExtraPadding", (void*)AgsImGuiStyle_GetTouchExtraPadding);
        engine->RegisterScriptFunction("ImGuiStyle::set_IndentSpacing", (void*)AgsImGuiStyle_SetIndentSpacing);
        engine->RegisterScriptFunction("ImGuiStyle::get_IndentSpacing", (void*)AgsImGuiStyle_GetIndentSpacing);
        engine->RegisterScriptFunction("ImGuiStyle::set_ColumnsMinSpacing", (void*)AgsImGuiStyle_SetColumnsMinSpacing);
        engine->RegisterScriptFunction("ImGuiStyle::get_ColumnsMinSpacing", (void*)AgsImGuiStyle_GetColumnsMinSpacing);
        engine->RegisterScriptFunction("ImGuiStyle::set_ScrollbarSize", (void*)AgsImGuiStyle_SetScrollbarSize);
        engine->RegisterScriptFunction("ImGuiStyle::get_ScrollbarSize", (void*)AgsImGuiStyle_GetScrollbarSize);
        engine->RegisterScriptFunction("ImGuiStyle::set_ScrollbarRounding", (void*)AgsImGuiStyle_SetScrollbarRounding);
        engine->RegisterScriptFunction("ImGuiStyle::get_ScrollbarRounding", (void*)AgsImGuiStyle_GetScrollbarRounding);
        engine->RegisterScriptFunction("ImGuiStyle::set_GrabMinSize", (void*)AgsImGuiStyle_SetGrabMinSize);
        engine->RegisterScriptFunction("ImGuiStyle::get_GrabMinSize", (void*)AgsImGuiStyle_GetGrabMinSize);
        engine->RegisterScriptFunction("ImGuiStyle::set_GrabRounding", (void*)AgsImGuiStyle_SetGrabRounding);
        engine->RegisterScriptFunction("ImGuiStyle::get_GrabRounding", (void*)AgsImGuiStyle_GetGrabRounding);
        engine->RegisterScriptFunction("ImGuiStyle::set_TabRounding", (void*)AgsImGuiStyle_SetTabRounding);
        engine->RegisterScriptFunction("ImGuiStyle::get_TabRounding", (void*)AgsImGuiStyle_GetTabRounding);
        engine->RegisterScriptFunction("ImGuiStyle::set_TabBorderSize", (void*)AgsImGuiStyle_SetTabBorderSize);
        engine->RegisterScriptFunction("ImGuiStyle::get_TabBorderSize", (void*)AgsImGuiStyle_GetTabBorderSize);
        engine->RegisterScriptFunction("ImGuiStyle::set_TabMinWidthForUnselectedCloseButton", (void*)AgsImGuiStyle_SetTabMinWidthForUnselectedCloseButton);
        engine->RegisterScriptFunction("ImGuiStyle::get_TabMinWidthForUnselectedCloseButton", (void*)AgsImGuiStyle_GetTabMinWidthForUnselectedCloseButton);
        engine->RegisterScriptFunction("ImGuiStyle::set_ColorButtonPosition", (void*)AgsImGuiStyle_SetColorButtonPosition);
        engine->RegisterScriptFunction("ImGuiStyle::get_ColorButtonPosition", (void*)AgsImGuiStyle_GetColorButtonPosition);
        engine->RegisterScriptFunction("ImGuiStyle::set_ButtonTextAlign", (void*)AgsImGuiStyle_SetButtonTextAlign);
        engine->RegisterScriptFunction("ImGuiStyle::get_ButtonTextAlign", (void*)AgsImGuiStyle_GetButtonTextAlign);
        engine->RegisterScriptFunction("ImGuiStyle::set_SelectableTextAlign", (void*)AgsImGuiStyle_SetSelectableTextAlign);
        engine->RegisterScriptFunction("ImGuiStyle::get_SelectableTextAlign", (void*)AgsImGuiStyle_GetSelectableTextAlign);
        engine->RegisterScriptFunction("ImGuiStyle::set_DisplayWindowPadding", (void*)AgsImGuiStyle_SetDisplayWindowPadding);
        engine->RegisterScriptFunction("ImGuiStyle::get_DisplayWindowPadding", (void*)AgsImGuiStyle_GetDisplayWindowPadding);
        engine->RegisterScriptFunction("ImGuiStyle::set_DisplaySafeAreaPadding", (void*)AgsImGuiStyle_SetDisplaySafeAreaPadding);
        engine->RegisterScriptFunction("ImGuiStyle::get_DisplaySafeAreaPadding", (void*)AgsImGuiStyle_GetDisplaySafeAreaPadding);
        engine->RegisterScriptFunction("ImGuiStyle::set_MouseCursorScale", (void*)AgsImGuiStyle_SetMouseCursorScale);
        engine->RegisterScriptFunction("ImGuiStyle::get_MouseCursorScale", (void*)AgsImGuiStyle_GetMouseCursorScale);
        engine->RegisterScriptFunction("ImGuiStyle::set_AntiAliasedLines", (void*)AgsImGuiStyle_SetAntiAliasedLines);
        engine->RegisterScriptFunction("ImGuiStyle::get_AntiAliasedLines", (void*)AgsImGuiStyle_GetAntiAliasedLines);
        engine->RegisterScriptFunction("ImGuiStyle::set_AntiAliasedFill", (void*)AgsImGuiStyle_SetAntiAliasedFill);
        engine->RegisterScriptFunction("ImGuiStyle::get_AntiAliasedFill", (void*)AgsImGuiStyle_GetAntiAliasedFill);
        engine->RegisterScriptFunction("ImGuiStyle::set_CurveTessellationTol", (void*)AgsImGuiStyle_SetCurveTessellationTol);
        engine->RegisterScriptFunction("ImGuiStyle::get_CurveTessellationTol", (void*)AgsImGuiStyle_GetCurveTessellationTol);
        engine->RegisterScriptFunction("ImGuiStyle::set_CircleSegmentMaxError", (void*)AgsImGuiStyle_SetCircleSegmentMaxError);
        engine->RegisterScriptFunction("ImGuiStyle::get_CircleSegmentMaxError", (void*)AgsImGuiStyle_GetCircleSegmentMaxError);
        engine->RegisterScriptFunction("ImGuiStyle::seti_Colors", (void*)AgsImGuiStyle_SetColors);
        engine->RegisterScriptFunction("ImGuiStyle::geti_Colors", (void*)AgsImGuiStyle_GetColors);

        engine->RegisterScriptFunction("AgsImGui::get_Style", (void*)AgsImGui_GetStyle);
        engine->RegisterScriptFunction("AgsImGui::set_Style", (void*)AgsImGui_SetStyle);

        engine->RegisterScriptFunction("AgsImGui::NewFrame^0", (void*)AgsImGui_NewFrame);
        engine->RegisterScriptFunction("AgsImGui::EndFrame^0", (void*)AgsImGui_EndFrame);
        engine->RegisterScriptFunction("AgsImGui::Render^0", (void*)AgsImGui_Render);
        //engine->RegisterScriptFunction("AgsImGui::GetDrawData^0", (void*)AgsImGui_GetDrawData);
        engine->RegisterScriptFunction("AgsImGui::GetVersion^0", (void*)AgsImGui_GetVersion);
        engine->RegisterScriptFunction("AgsImGui::ShowDemoWindow^0", (void*)AgsImGui_ShowDemoWindow);
        engine->RegisterScriptFunction("AgsImGui::ShowAboutWindow^0", (void*)AgsImGui_ShowAboutWindow);
        engine->RegisterScriptFunction("AgsImGui::ShowMetricsWindow^0", (void*)AgsImGui_ShowMetricsWindow);
        engine->RegisterScriptFunction("AgsImGui::ShowUserGuide^0", (void*)AgsImGui_ShowUserGuide);
        engine->RegisterScriptFunction("AgsImGui::StyleColorsDark^0", (void*)AgsImGui_StyleColorsDark);
        engine->RegisterScriptFunction("AgsImGui::StyleColorsClassic^0", (void*)AgsImGui_StyleColorsClassic);
        engine->RegisterScriptFunction("AgsImGui::StyleColorsLight^0", (void*)AgsImGui_StyleColorsLight);
        engine->RegisterScriptFunction("AgsImGui::BeginWindow^3", (void*)AgsImGui_BeginWindow);
        engine->RegisterScriptFunction("AgsImGui::EndWindow^0", (void*)AgsImGui_EndWindow);
        engine->RegisterScriptFunction("AgsImGui::BeginChild^5", (void*)AgsImGui_BeginChild);
        engine->RegisterScriptFunction("AgsImGui::EndChild^0", (void*)AgsImGui_EndChild);
        engine->RegisterScriptFunction("AgsImGui::SetNextWindowPos^5", (void*)AgsImGui_SetNextWindowPos);
        engine->RegisterScriptFunction("AgsImGui::SetNextWindowSize^3", (void*)AgsImGui_SetNextWindowSize);
        engine->RegisterScriptFunction("AgsImGui::SetNextWindowSizeConstraints^4", (void*)AgsImGui_SetNextWindowSizeConstraints);
        engine->RegisterScriptFunction("AgsImGui::SetNextWindowContentSize^2", (void*)AgsImGui_SetNextWindowContentSize);
        engine->RegisterScriptFunction("AgsImGui::SetNextWindowCollapsed^2", (void*)AgsImGui_SetNextWindowCollapsed);
        engine->RegisterScriptFunction("AgsImGui::SetNextWindowFocus^0", (void*)AgsImGui_SetNextWindowFocus);
        engine->RegisterScriptFunction("AgsImGui::SetNextWindowBgAlpha^1", (void*)AgsImGui_SetNextWindowBgAlpha);
        engine->RegisterScriptFunction("AgsImGui::IsWindowAppearing^0", (void*)AgsImGui_IsWindowAppearing);
        engine->RegisterScriptFunction("AgsImGui::IsWindowCollapsed^0", (void*)AgsImGui_IsWindowCollapsed);
        engine->RegisterScriptFunction("AgsImGui::IsWindowFocused^1", (void*)AgsImGui_IsWindowFocused);
        engine->RegisterScriptFunction("AgsImGui::IsWindowHovered^1", (void*)AgsImGui_IsWindowHovered);
        engine->RegisterScriptFunction("AgsImGui::IsItemHovered^1", (void*)AgsImGui_IsItemHovered);
        engine->RegisterScriptFunction("AgsImGui::IsItemActive^0", (void*)AgsImGui_IsItemActive);
        engine->RegisterScriptFunction("AgsImGui::IsItemFocused^0", (void*)AgsImGui_IsItemFocused);
        engine->RegisterScriptFunction("AgsImGui::IsItemVisible^0", (void*)AgsImGui_IsItemVisible);
        engine->RegisterScriptFunction("AgsImGui::IsItemEdited^0", (void*)AgsImGui_IsItemEdited);
        engine->RegisterScriptFunction("AgsImGui::IsItemActivated^0", (void*)AgsImGui_IsItemActivated);
        engine->RegisterScriptFunction("AgsImGui::IsItemDeactivated^0", (void*)AgsImGui_IsItemDeactivated);
        engine->RegisterScriptFunction("AgsImGui::IsItemDeactivatedAfterEdit^0", (void*)AgsImGui_IsItemDeactivatedAfterEdit);
        engine->RegisterScriptFunction("AgsImGui::IsItemToggledOpen^0", (void*)AgsImGui_IsItemToggledOpen);
        engine->RegisterScriptFunction("AgsImGui::IsAnyItemHovered^0", (void*)AgsImGui_IsAnyItemHovered);
        engine->RegisterScriptFunction("AgsImGui::IsAnyItemActive^0", (void*)AgsImGui_IsAnyItemActive);
        engine->RegisterScriptFunction("AgsImGui::IsAnyItemFocused^0", (void*)AgsImGui_IsAnyItemFocused);
        engine->RegisterScriptFunction("AgsImGui::SetItemDefaultFocus^0", (void*)AgsImGui_SetItemDefaultFocus);
        engine->RegisterScriptFunction("AgsImGui::SetKeyboardFocusHere^1", (void*)AgsImGui_SetKeyboardFocusHere);
        engine->RegisterScriptFunction("AgsImGui::Separator^0", (void*)AgsImGui_Separator);
        engine->RegisterScriptFunction("AgsImGui::SameLine^2", (void*)AgsImGui_SameLine);
        engine->RegisterScriptFunction("AgsImGui::NewLine^0", (void*)AgsImGui_NewLine);
        engine->RegisterScriptFunction("AgsImGui::Spacing^0", (void*)AgsImGui_Spacing);
        engine->RegisterScriptFunction("AgsImGui::Dummy^2", (void*)AgsImGui_Dummy);
        engine->RegisterScriptFunction("AgsImGui::Indent^1", (void*)AgsImGui_Indent);
        engine->RegisterScriptFunction("AgsImGui::Unindent^1", (void*)AgsImGui_Unindent);
        engine->RegisterScriptFunction("AgsImGui::PushID^1", (void*)AgsImGui_PushID);
        engine->RegisterScriptFunction("AgsImGui::PopID^0", (void*)AgsImGui_PopID);
        engine->RegisterScriptFunction("AgsImGui::Text^1", (void*)AgsImGui_Text);
        engine->RegisterScriptFunction("AgsImGui::TextColored^2", (void*)AgsImGui_TextColored);
        engine->RegisterScriptFunction("AgsImGui::TextDisabled^1", (void*)AgsImGui_TextDisabled);
        engine->RegisterScriptFunction("AgsImGui::TextWrapped^1", (void*)AgsImGui_TextWrapped);
        engine->RegisterScriptFunction("AgsImGui::LabelText^2", (void*)AgsImGui_LabelText);
        engine->RegisterScriptFunction("AgsImGui::BulletText^1", (void*)AgsImGui_BulletText);
        engine->RegisterScriptFunction("AgsImGui::Button^3", (void*)AgsImGui_Button);
        engine->RegisterScriptFunction("AgsImGui::SmallButton^1", (void*)AgsImGui_SmallButton);
        engine->RegisterScriptFunction("AgsImGui::Image^1", (void*)AgsImGui_Image);
        engine->RegisterScriptFunction("AgsImGui::ImageButton^1", (void*)AgsImGui_ImageButton);
        engine->RegisterScriptFunction("AgsImGui::ArrowButton^2", (void*)AgsImGui_ArrowButton);
        engine->RegisterScriptFunction("AgsImGui::Checkbox^2", (void*)AgsImGui_Checkbox);
        engine->RegisterScriptFunction("AgsImGui::RadioButton^2", (void*)AgsImGui_RadioButton);
        engine->RegisterScriptFunction("AgsImGui::Bullet^0", (void*)AgsImGui_Bullet);
        engine->RegisterScriptFunction("AgsImGui::Selectable^5", (void*)AgsImGui_Selectable);
        engine->RegisterScriptFunction("AgsImGui::DragFloat^6", (void*)AgsImGui_DragFloat);
        engine->RegisterScriptFunction("AgsImGui::DragInt^6", (void*)AgsImgui_DragInt);
        engine->RegisterScriptFunction("AgsImGui::SliderFloat^5", (void*)AgsImGui_SliderFloat);
        engine->RegisterScriptFunction("AgsImGui::SliderInt^5", (void*)AgsImgui_SliderInt);
        engine->RegisterScriptFunction("AgsImGui::InputText^4", (void*)AgsImgui_InputText);
        engine->RegisterScriptFunction("AgsImGui::InputTextMultiline^6", (void*)AgsImgui_InputTextMultiline);
        engine->RegisterScriptFunction("AgsImGui::InputTextWithHint^5", (void*)AgsImgui_InputTextWithHint);
        engine->RegisterScriptFunction("AgsImGui::BeginCombo^3", (void*)AgsImGui_BeginCombo);
        engine->RegisterScriptFunction("AgsImGui::EndCombo^0", (void*)AgsImGui_EndCombo);
        engine->RegisterScriptFunction("AgsImGui::TreeNode^1", (void*)AgsImGui_TreeNode);
        engine->RegisterScriptFunction("AgsImGui::TreeNodeWithID^2", (void*)AgsImGui_TreeNodeWithID);
        engine->RegisterScriptFunction("AgsImGui::TreeNodeV^2", (void*)AgsImGui_TreeNodeV);
        engine->RegisterScriptFunction("AgsImGui::TreeNodeEx^2", (void*)AgsImGui_TreeNodeEx);
        engine->RegisterScriptFunction("AgsImGui::TreeNodeExWithID^3", (void*)AgsImGui_TreeNodeExWithID);
        engine->RegisterScriptFunction("AgsImGui::TreeNodeExV^3", (void*)AgsImGui_TreeNodeExV);
        engine->RegisterScriptFunction("AgsImGui::TreePush^1", (void*)AgsImGui_TreePush);
        engine->RegisterScriptFunction("AgsImGui::TreePop^0", (void*)AgsImGui_TreePop);
        engine->RegisterScriptFunction("AgsImGui::GetTreeNodeToLabelSpacing^0", (void*)AgsImGui_GetTreeNodeToLabelSpacing);
        engine->RegisterScriptFunction("AgsImGui::CollapsingHeader^2", (void*)AgsImGui_CollapsingHeader);
        engine->RegisterScriptFunction("AgsImGui::SetNextItemOpen^2", (void*)AgsImGui_SetNextItemOpen);
        engine->RegisterScriptFunction("AgsImGui::BeginListBox^3", (void*)AgsImGui_BeginListBox);
        engine->RegisterScriptFunction("AgsImGui::EndListBox^0", (void*)AgsImGui_EndListBox);
        engine->RegisterScriptFunction("AgsImGui::BeginTooltip^0", (void*)AgsImGui_BeginTooltip);
        engine->RegisterScriptFunction("AgsImGui::EndTooltip^0", (void*)AgsImGui_EndTooltip);
        engine->RegisterScriptFunction("AgsImGui::SetTooltip^1", (void*)AgsImGui_SetTooltip);
        engine->RegisterScriptFunction("AgsImGui::OpenPopup^1", (void*)AgsImGui_OpenPopup);
        engine->RegisterScriptFunction("AgsImGui::BeginPopup^2", (void*)AgsImGui_BeginPopup);
        engine->RegisterScriptFunction("AgsImGui::BeginPopupModal^3", (void*)AgsImGui_BeginPopupModal);
        engine->RegisterScriptFunction("AgsImGui::EndPopup^0", (void*)AgsImGui_EndPopup);
        engine->RegisterScriptFunction("AgsImGui::IsPopupOpen^1", (void*)AgsImGui_IsPopupOpen);
        engine->RegisterScriptFunction("AgsImGui::CloseCurrentPopup^0", (void*)AgsImGui_CloseCurrentPopup);
        engine->RegisterScriptFunction("AgsImGui::BeginTabBar^2", (void*)AgsImGui_BeginTabBar);
        engine->RegisterScriptFunction("AgsImGui::EndTabBar^0", (void*)AgsImGui_EndTabBar);
        engine->RegisterScriptFunction("AgsImGui::BeginTabItem^3", (void*)AgsImGui_BeginTabItem);
        engine->RegisterScriptFunction("AgsImGui::EndTabItem^0", (void*)AgsImGui_EndTabItem);
        engine->RegisterScriptFunction("AgsImGui::SetTabItemClosed^1", (void*)AgsImGui_SetTabItemClosed);
        engine->RegisterScriptFunction("AgsImGui::BeginMenuBar^0", (void*)AgsImGui_BeginMenuBar);
        engine->RegisterScriptFunction("AgsImGui::EndMenuBar^0", (void*)AgsImGui_EndMenuBar);
        engine->RegisterScriptFunction("AgsImGui::BeginMainMenuBar^0", (void*)AgsImGui_BeginMainMenuBar);
        engine->RegisterScriptFunction("AgsImGui::EndMainMenuBar^0", (void*)AgsImGui_EndMainMenuBar);
        engine->RegisterScriptFunction("AgsImGui::BeginMenu^2", (void*)AgsImGui_BeginMenu);
        engine->RegisterScriptFunction("AgsImGui::EndMenu^0", (void*)AgsImGui_EndMenu);
        engine->RegisterScriptFunction("AgsImGui::MenuItem^4", (void*)AgsImGui_MenuItem);
        engine->RegisterScriptFunction("AgsImGui::DoCaptureMouse^1", (void*)AgsImGui_DoCaptureMouse);
        engine->RegisterScriptFunction("AgsImGui::DoCaptureKeyboard^1", (void*)AgsImGui_DoCaptureKeyboard);
        engine->RegisterScriptFunction("AgsImGui::ValueBool^2", (void*)AgsImGui_ValueBool);
        engine->RegisterScriptFunction("AgsImGui::ValueInt^2", (void*)AgsImGui_ValueInt);
        engine->RegisterScriptFunction("AgsImGui::ValueFloat^2", (void*)AgsImGui_ValueFloat);
		engine->RegisterScriptFunction("AgsImGui::DoMouseWheel^1", (void*)AgsImGui_DoMouseWheel);

		engine->RegisterScriptFunction("AgsImGuiHelper::SetClipboarText^1", (void*)AgsImGuiHelper_SetClipboarText);
		engine->RegisterScriptFunction("AgsImGuiHelper::GetClipboarText^0", (void*)AgsImGuiHelper_GetClipboarText);
		//engine->RegisterScriptFunction("AgsImGuiHelper::SetClipboarImage^1", (void*)AgsImGuiHelper_SetClipboarImage);
		//engine->RegisterScriptFunction("AgsImGuiHelper::GetClipboarImage^0", (void*)AgsImGuiHelper_GetClipboarImage);

        engine->RequestEventHook(AGSE_PRESCREENDRAW);
        engine->RequestEventHook(AGSE_KEYPRESS);
		engine->RequestEventHook(AGSE_POSTSCREENDRAW);
		engine->RequestEventHook(AGSE_MOUSECLICK);
	}

	//------------------------------------------------------------------------------

	void AGS_EngineShutdown()
	{
		// Called by the game engine just before it exits.
		// This gives you a chance to free any memory and do any cleanup
		// that you need to do before the engine shuts down.
		if (screen.driver == Screen::Driver::eOpenGL) {

		}
		if (screen.driver == Screen::Driver::eDirectx9) {
			ImGui_ImplDX9_Shutdown();
		}
		if (screen.driver == Screen::Driver::eSoftware) {
			ImGui_ImplSoftraster_Shutdown();
		}
	}

	//------------------------------------------------------------------------------

enum MouseButton {
    eMouseLeft = 1,
    eMouseRight = 2,
    eMouseMiddle = 3,
    eMouseLeftInv = 5,
    eMouseRightInv = 6,
    eMouseMiddleInv = 7,
    eMouseWheelNorth = 8,
    eMouseWheelSouth = 9
};
    std::vector<int> pressed_keys;
    int32 ags_mouse_x = 0;
    int32 ags_mouse_y = 0;
    bool do_only_once = false;
    int unstuck_counter = 0;
    int AGS_EngineOnEvent(int event, int data)                    //*** optional ***
    {
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        

        if(event==AGSE_PRESCREENDRAW){
			ImGuiIO& io = ImGui::GetIO();
			//initialize debug
			if (!screen.initialized) {
				engine->GetScreenDimensions(&screen.width, &screen.height, &screen.colorDepth);
				printf("\nagsimgui 0.3.0\n");

				if (screen.driver == Screen::Driver::eOpenGL) {

					screen.initialized = true;
				}
				if (screen.driver == Screen::Driver::eDirectx9) {
					if ((IDirect3DDevice9*)data != nullptr) {
						io.DisplaySize.x = (float)screen.width;
						io.DisplaySize.y = (float)screen.height;
						ImGui_ImplDX9_Init((IDirect3DDevice9*)data);
						ImGui_ImplDX9_InvalidateDeviceObjects();
						ImGui_ImplDX9_CreateDeviceObjects();
						screen.initialized = true;
					}
				}
				if (screen.driver == Screen::Driver::eSoftware) {
					ImGui_ImplSoftraster_InitializeScreenAgs(screen.width, screen.height, screen.colorDepth);
					software_renderer_screen.init(screen.width, screen.height);
					screen.initialized = true;

				}
			}

            if(!pressed_keys.empty()) {
                unstuck_counter++;

                if(unstuck_counter>5) {
                    unstuck_counter = 0;
                    while (!pressed_keys.empty()) {
                        int key_pressed = pressed_keys.back();
                        io.KeysDown[key_pressed] = false;

                        if( key_pressed == eAGSKeyCodeCtrlC ){
                            io.KeysDown[eAGSKeyCodeC] = false;
                            io.KeyCtrl = false;
                        } else if( key_pressed == eAGSKeyCodeCtrlX ){
                            io.KeysDown[eAGSKeyCodeX] = false;
                            io.KeyCtrl = false;
                        } else if( key_pressed == eAGSKeyCodeCtrlV ){
                            io.KeysDown[eAGSKeyCodeV] = false;
                            io.KeyCtrl = false;
                        } else if( key_pressed == eAGSKeyCodeCtrlZ ){
                            io.KeysDown[eAGSKeyCodeZ] = false;
                            io.KeyCtrl = false;
                        } else if( key_pressed == eAGSKeyCodeCtrlA ){
                            io.KeysDown[eAGSKeyCodeA] = false;
                            io.KeyCtrl = false;
                        } else if( key_pressed == eAGSKeyCodeCtrlY ){
                            io.KeysDown[eAGSKeyCodeY] = false;
                            io.KeyCtrl = false;
                        }
                        pressed_keys.pop_back();

                        // probably is better to keep traversing this and remove only things that have the button lift off, but this is enough for now.
                    }
                }
            }
            else {
                engine->GetMousePosition(&ags_mouse_x, &ags_mouse_y);

                io.MousePos = ImVec2((float) ags_mouse_x, (float) ags_mouse_y);
            }

			io.MouseDown[ImGuiMouseButton_Left] = Mouse_IsButtonDown(eMouseLeft) != 0;
            io.MouseDown[ImGuiMouseButton_Right] = Mouse_IsButtonDown(eMouseRight) != 0;
            io.MouseDown[ImGuiMouseButton_Middle] = Mouse_IsButtonDown(eMouseMiddle) != 0;

        }

        if(event==AGSE_KEYPRESS){
			ImGuiIO& io = ImGui::GetIO();

			if( data == eAGSKeyCodeCtrlC ){
                io.KeysDown[eAGSKeyCodeC] = true;
                io.KeyCtrl = true;
			} else if( data == eAGSKeyCodeCtrlX ){
                io.KeysDown[eAGSKeyCodeX] = true;
                io.KeyCtrl = true;
            } else if( data == eAGSKeyCodeCtrlV ){
                io.KeysDown[eAGSKeyCodeV] = true;
                io.KeyCtrl = true;
            }  else if( data == eAGSKeyCodeCtrlZ ){
                io.KeysDown[eAGSKeyCodeZ] = true;
                io.KeyCtrl = true;
            } else if( data == eAGSKeyCodeCtrlA ){
                io.KeysDown[eAGSKeyCodeA] = true;
                io.KeyCtrl = true;
            } else if( data == eAGSKeyCodeCtrlY ){
                io.KeysDown[eAGSKeyCodeY] = true;
                io.KeyCtrl = true;
            } else {
                io.KeysDown[data] = true;
                io.KeyCtrl = false;
			}

            pressed_keys.push_back(data);

            if(data != 0 &&
               data != eAGSKeyCodeLeftArrow &&
               data != eAGSKeyCodeRightArrow &&
               data != eAGSKeyCodeUpArrow &&
               data != eAGSKeyCodeDownArrow &&
               data != eAGSKeyCodePageUp &&
               data != eAGSKeyCodePageDown && data < 177 ) {
                io.AddInputCharacter(data);
            }
        }

		if (event == AGSE_POSTSCREENDRAW) {
			if (screen.driver == Screen::Driver::eDirectx9) {
				if (has_new_frame && has_new_render) {
					ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
				}
			}
            if (screen.driver == Screen::Driver::eSoftware) {
                if (has_new_frame && has_new_render) {
                    ImGui_ImplSoftraster_RenderDrawData(ImGui::GetDrawData());
                    engine->BlitBitmap(0,0,engine->GetSpriteGraphic(ImGui_ImplSoftraster_GetSprite()),1);
                }
            }
            has_new_frame = false;
            has_new_render = false;
		}

        if(event==AGSE_MOUSECLICK){
			ImGuiIO& io = ImGui::GetIO();

			io.MouseDown[ImGuiMouseButton_Left] |= eMouseLeft == data;
			io.MouseDown[ImGuiMouseButton_Right] |= eMouseRight == data;
			io.MouseDown[ImGuiMouseButton_Middle] |= eMouseMiddle == data;
			
        }

        /*
        switch (event)
        {
                case AGSE_KEYPRESS:
                case AGSE_MOUSECLICK:
                case AGSE_POSTSCREENDRAW:
                case AGSE_PRESCREENDRAW:
                case AGSE_SAVEGAME:
                case AGSE_RESTOREGAME:
                case AGSE_PREGUIDRAW:
                case AGSE_LEAVEROOM:
                case AGSE_ENTERROOM:
                case AGSE_TRANSITIONIN:
                case AGSE_TRANSITIONOUT:
                case AGSE_FINALSCREENDRAW:
                case AGSE_TRANSLATETEXT:
                case AGSE_SCRIPTDEBUG:
                case AGSE_SPRITELOAD:
                case AGSE_PRERENDER:
                case AGSE_PRESAVEGAME:
                case AGSE_POSTRESTOREGAME:
        default:
            break;
        }
         */

		// Return 1 to stop event from processing further (when needed)
		return (0);
	}

	//------------------------------------------------------------------------------

	int AGS_EngineDebugHook(const char *scriptName,
		int lineNum, int reserved)            //*** optional ***
	{
		// Can be used to debug scripts, see documentation
		return 0;
	}

	//------------------------------------------------------------------------------

    void AGS_EngineInitGfx( char const* driverID, void* data )
    {
        // This allows you to make changes to how the graphics driver starts up.
        #if AGS_PLATFORM_OS_WINDOWS
        if ( strcmp( driverID, "D3D9" ) == 0 )
        {
            D3DPRESENT_PARAMETERS* params = (D3DPRESENT_PARAMETERS*)data;
            if (params->BackBufferFormat != D3DFMT_X8R8G8B8)
            {
                engine->AbortGame( "32bit colour mode required." );
            }
			ImGui_ImplDX9_InitializeEngine(engine);
            screen.backBufferWidth = params->BackBufferWidth;
            screen.backBufferHeight = params->BackBufferHeight;
            screen.colorDepth = 32;
            screen.driver = Screen::Driver::eDirectx9;

            return;
        }
        #endif

//        if ( strcmp( driverID, "OpenGL" ) == 0 )
//        {
//            screen.driver = Screen::Driver::eOpenGL;
//            return;
//        }


        screen.driver = Screen::Driver::eSoftware;

    }

	//..............................................................................


#if defined(BUILTIN_PLUGINS)
}
#endif
