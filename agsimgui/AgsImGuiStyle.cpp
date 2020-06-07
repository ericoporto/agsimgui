/*
 * Copyright (C) 2020  Érico Vieira Porto
 *
 * This program is free software. You can use and redistribute it
 *  under the terms and conditions of the MIT License (see LICENCE).
 */

#include <cmath>
#include "AgsImGuiStyle.h"

void AgsImGuiStyle::SetAgsImGuiStyle(ImGuiStyle &imGuiStyle, AgsImGuiStyle* agsImGuiStyle){
    imGuiStyle.Alpha = agsImGuiStyle->Alpha;
    imGuiStyle.WindowPadding = agsImGuiStyle->WindowPadding;
    imGuiStyle.WindowRounding = agsImGuiStyle->WindowRounding;
    imGuiStyle.WindowBorderSize = agsImGuiStyle->WindowBorderSize;
    imGuiStyle.WindowMinSize = agsImGuiStyle->WindowMinSize;
    imGuiStyle.WindowTitleAlign = agsImGuiStyle->WindowTitleAlign;
    imGuiStyle.WindowMenuButtonPosition = agsImGuiStyle->WindowMenuButtonPosition;
    imGuiStyle.ChildRounding = agsImGuiStyle->ChildRounding;
    imGuiStyle.ChildBorderSize = agsImGuiStyle->ChildBorderSize;
    imGuiStyle.PopupRounding = agsImGuiStyle->PopupRounding;
    imGuiStyle.PopupBorderSize = agsImGuiStyle->PopupBorderSize;
    imGuiStyle.FramePadding = agsImGuiStyle->FramePadding;
    imGuiStyle.FrameRounding = agsImGuiStyle->FrameRounding;
    imGuiStyle.FrameBorderSize = agsImGuiStyle->FrameBorderSize;
    imGuiStyle.ItemSpacing = agsImGuiStyle->ItemSpacing;
    imGuiStyle.ItemInnerSpacing = agsImGuiStyle->ItemInnerSpacing;
    imGuiStyle.TouchExtraPadding = agsImGuiStyle->TouchExtraPadding;
    imGuiStyle.IndentSpacing = agsImGuiStyle->IndentSpacing;
    imGuiStyle.ColumnsMinSpacing = agsImGuiStyle->ColumnsMinSpacing;
    imGuiStyle.ScrollbarSize = agsImGuiStyle->ScrollbarSize;
    imGuiStyle.ScrollbarRounding = agsImGuiStyle->ScrollbarRounding;
    imGuiStyle.GrabMinSize = agsImGuiStyle->GrabMinSize;
    imGuiStyle.GrabRounding = agsImGuiStyle->GrabRounding;
    imGuiStyle.TabRounding = agsImGuiStyle->TabRounding;
    imGuiStyle.TabBorderSize = agsImGuiStyle->TabBorderSize;
    imGuiStyle.TabMinWidthForUnselectedCloseButton = agsImGuiStyle->TabMinWidthForUnselectedCloseButton;
    imGuiStyle.ColorButtonPosition = agsImGuiStyle->ColorButtonPosition;
    imGuiStyle.ButtonTextAlign = agsImGuiStyle->ButtonTextAlign;
    imGuiStyle.SelectableTextAlign = agsImGuiStyle->SelectableTextAlign;
    imGuiStyle.DisplayWindowPadding = agsImGuiStyle->DisplayWindowPadding;
    imGuiStyle.DisplaySafeAreaPadding = agsImGuiStyle->DisplaySafeAreaPadding;
    imGuiStyle.MouseCursorScale = agsImGuiStyle->MouseCursorScale;
    imGuiStyle.AntiAliasedLines = agsImGuiStyle->AntiAliasedLines;
    imGuiStyle.AntiAliasedFill = agsImGuiStyle->AntiAliasedFill;
    imGuiStyle.CurveTessellationTol = agsImGuiStyle->CurveTessellationTol;
    imGuiStyle.CircleSegmentMaxError = agsImGuiStyle->CircleSegmentMaxError;
    for(int i=0; i<ImGuiCol_COUNT; i++){
        imGuiStyle.Colors[i] = agsImGuiStyle->Colors[i];
    }
}

//------------------------------------------------------------------------------

extern IAGSEngine* engine;

AgsImGuiStyleInterface AgsImGuiStyle_Interface;
AgsImGuiStyleReader AgsImGuiStyle_Reader;

const char* AgsImGuiStyleInterface::name = "ImGuiStyle";

//------------------------------------------------------------------------------
#include "SerialHelper.h"
using namespace SerialHelper;

int AgsImGuiStyleInterface::Dispose(const char* address, bool force)
{
    delete ((AgsImGuiStyle*)address);
    return (1);
}

//------------------------------------------------------------------------------

int AgsImGuiStyleInterface::Serialize(const char* address, char* buffer, int bufsize)
{
    AgsImGuiStyle* agsImGuiStyle = (AgsImGuiStyle*)address;
    char* ptr = buffer;
    char* end = buffer + bufsize;

    ptr = FloatToChar(agsImGuiStyle->Alpha, ptr, end);
    ptr = ImVec2ToChar(agsImGuiStyle->WindowPadding, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->WindowRounding, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->WindowBorderSize, ptr, end);
    ptr = ImVec2ToChar(agsImGuiStyle->WindowMinSize, ptr, end);
    ptr = ImVec2ToChar(agsImGuiStyle->WindowTitleAlign, ptr, end);
    ptr = IntToChar(agsImGuiStyle->WindowMenuButtonPosition, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->ChildRounding, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->ChildBorderSize, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->PopupRounding, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->PopupBorderSize, ptr, end);
    ptr = ImVec2ToChar(agsImGuiStyle->FramePadding, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->FrameRounding, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->FrameBorderSize, ptr, end);
    ptr = ImVec2ToChar(agsImGuiStyle->ItemSpacing, ptr, end);
    ptr = ImVec2ToChar(agsImGuiStyle->ItemInnerSpacing, ptr, end);
    ptr = ImVec2ToChar(agsImGuiStyle->TouchExtraPadding, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->IndentSpacing, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->ColumnsMinSpacing, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->ScrollbarSize, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->ScrollbarRounding, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->GrabMinSize, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->GrabRounding, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->TabRounding, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->TabBorderSize, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->TabMinWidthForUnselectedCloseButton, ptr, end);
    ptr = IntToChar(agsImGuiStyle->ColorButtonPosition, ptr, end);
    ptr = ImVec2ToChar(agsImGuiStyle->ButtonTextAlign, ptr, end);
    ptr = ImVec2ToChar(agsImGuiStyle->SelectableTextAlign, ptr, end);
    ptr = ImVec2ToChar(agsImGuiStyle->DisplayWindowPadding, ptr, end);
    ptr = ImVec2ToChar(agsImGuiStyle->DisplaySafeAreaPadding, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->MouseCursorScale, ptr, end);
    ptr = BoolToChar(agsImGuiStyle->AntiAliasedLines, ptr, end);
    ptr = BoolToChar(agsImGuiStyle->AntiAliasedFill, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->CurveTessellationTol, ptr, end);
    ptr = FloatToChar(agsImGuiStyle->CircleSegmentMaxError, ptr, end);

    for(int i=0; i<ImGuiCol_COUNT; i++){
        ptr = ImVec4ToChar(agsImGuiStyle->Colors[i], ptr, end);
    }

    return (ptr - buffer);
}

//------------------------------------------------------------------------------

void AgsImGuiStyleReader::Unserialize(int key, const char* serializedData, int dataSize)
{
    char* ptr = (char*) serializedData;

    float _Alpha;
    ImVec2 _WindowPadding;
    float _WindowRounding;
    float _WindowBorderSize;
    ImVec2 _WindowMinSize;
    ImVec2 _WindowTitleAlign;
    int _WindowMenuButtonPosition;
    float _ChildRounding;
    float _ChildBorderSize;
    float _PopupRounding;
    float _PopupBorderSize;
    ImVec2 _FramePadding;
    float _FrameRounding;
    float _FrameBorderSize;
    ImVec2 _ItemSpacing;
    ImVec2 _ItemInnerSpacing;
    ImVec2 _TouchExtraPadding;
    float _IndentSpacing;
    float _ColumnsMinSpacing;
    float _ScrollbarSize;
    float _ScrollbarRounding;
    float _GrabMinSize;
    float _GrabRounding;
    float _TabRounding;
    float _TabBorderSize;
    float _TabMinWidthForUnselectedCloseButton;
    int _ColorButtonPosition;
    ImVec2 _ButtonTextAlign;
    ImVec2 _SelectableTextAlign;
    ImVec2 _DisplayWindowPadding;
    ImVec2 _DisplaySafeAreaPadding;
    float _MouseCursorScale;
    bool _AntiAliasedLines;
    bool _AntiAliasedFill;
    float _CurveTessellationTol;
    float _CircleSegmentMaxError;
    ImVec4 _Colors[ImGuiCol_COUNT];

    ptr = CharToFloat( _Alpha, ptr);
    ptr = CharToImVec2( _WindowPadding, ptr);
    ptr = CharToFloat( _WindowRounding, ptr);
    ptr = CharToFloat( _WindowBorderSize, ptr);
    ptr = CharToImVec2( _WindowMinSize, ptr);
    ptr = CharToImVec2( _WindowTitleAlign, ptr);
    ptr = CharToInt( _WindowMenuButtonPosition, ptr);
    ptr = CharToFloat( _ChildRounding, ptr);
    ptr = CharToFloat( _ChildBorderSize, ptr);
    ptr = CharToFloat( _PopupRounding, ptr);
    ptr = CharToFloat( _PopupBorderSize, ptr);
    ptr = CharToImVec2( _FramePadding, ptr);
    ptr = CharToFloat( _FrameRounding, ptr);
    ptr = CharToFloat( _FrameBorderSize, ptr);
    ptr = CharToImVec2( _ItemSpacing, ptr);
    ptr = CharToImVec2( _ItemInnerSpacing, ptr);
    ptr = CharToImVec2( _TouchExtraPadding, ptr);
    ptr = CharToFloat( _IndentSpacing, ptr);
    ptr = CharToFloat( _ColumnsMinSpacing, ptr);
    ptr = CharToFloat( _ScrollbarSize, ptr);
    ptr = CharToFloat( _ScrollbarRounding, ptr);
    ptr = CharToFloat( _GrabMinSize, ptr);
    ptr = CharToFloat( _GrabRounding, ptr);
    ptr = CharToFloat( _TabRounding, ptr);
    ptr = CharToFloat( _TabBorderSize, ptr);
    ptr = CharToFloat( _TabMinWidthForUnselectedCloseButton, ptr);
    ptr = CharToInt( _ColorButtonPosition, ptr);
    ptr = CharToImVec2( _ButtonTextAlign, ptr);
    ptr = CharToImVec2( _SelectableTextAlign, ptr);
    ptr = CharToImVec2( _DisplayWindowPadding, ptr);
    ptr = CharToImVec2( _DisplaySafeAreaPadding, ptr);
    ptr = CharToFloat( _MouseCursorScale, ptr);
    ptr = CharToBool(_AntiAliasedLines, ptr);
    ptr = CharToBool(_AntiAliasedFill, ptr);
    ptr = CharToFloat( _CurveTessellationTol, ptr);
    ptr = CharToFloat( _CircleSegmentMaxError, ptr);

    for(int i=0; i<ImGuiCol_COUNT; i++){
        ptr = CharToImVec4(  _Colors[i], ptr);
    }

    AgsImGuiStyle* agsImGuiStyle = new AgsImGuiStyle(key);
    agsImGuiStyle->Alpha = _Alpha;
    agsImGuiStyle->WindowPadding = _WindowPadding;
    agsImGuiStyle->WindowRounding = _WindowRounding;
    agsImGuiStyle->WindowBorderSize = _WindowBorderSize;
    agsImGuiStyle->WindowMinSize = _WindowMinSize;
    agsImGuiStyle->WindowTitleAlign = _WindowTitleAlign;
    agsImGuiStyle->WindowMenuButtonPosition = _WindowMenuButtonPosition;
    agsImGuiStyle->ChildRounding = _ChildRounding;
    agsImGuiStyle->ChildBorderSize = _ChildBorderSize;
    agsImGuiStyle->PopupRounding = _PopupRounding;
    agsImGuiStyle->PopupBorderSize = _PopupBorderSize;
    agsImGuiStyle->FramePadding = _FramePadding;
    agsImGuiStyle->FrameRounding = _FrameRounding;
    agsImGuiStyle->FrameBorderSize = _FrameBorderSize;
    agsImGuiStyle->ItemSpacing = _ItemSpacing;
    agsImGuiStyle->ItemInnerSpacing = _ItemInnerSpacing;
    agsImGuiStyle->TouchExtraPadding = _TouchExtraPadding;
    agsImGuiStyle->IndentSpacing = _IndentSpacing;
    agsImGuiStyle->ColumnsMinSpacing = _ColumnsMinSpacing;
    agsImGuiStyle->ScrollbarSize = _ScrollbarSize;
    agsImGuiStyle->ScrollbarRounding = _ScrollbarRounding;
    agsImGuiStyle->GrabMinSize = _GrabMinSize;
    agsImGuiStyle->GrabRounding = _GrabRounding;
    agsImGuiStyle->TabRounding = _TabRounding;
    agsImGuiStyle->TabBorderSize = _TabBorderSize;
    agsImGuiStyle->TabMinWidthForUnselectedCloseButton = _TabMinWidthForUnselectedCloseButton;
    agsImGuiStyle->ColorButtonPosition = _ColorButtonPosition;
    agsImGuiStyle->ButtonTextAlign = _ButtonTextAlign;
    agsImGuiStyle->SelectableTextAlign = _SelectableTextAlign;
    agsImGuiStyle->DisplayWindowPadding = _DisplayWindowPadding;
    agsImGuiStyle->DisplaySafeAreaPadding = _DisplaySafeAreaPadding;
    agsImGuiStyle->MouseCursorScale = _MouseCursorScale;
    agsImGuiStyle->AntiAliasedLines = _AntiAliasedLines;
    agsImGuiStyle->AntiAliasedFill = _AntiAliasedFill;
    agsImGuiStyle->CurveTessellationTol = _CurveTessellationTol;
    agsImGuiStyle->CircleSegmentMaxError = _CircleSegmentMaxError;

    for(int i=0; i<ImGuiCol_COUNT; i++){
        agsImGuiStyle->Colors[i] = _Colors[i];
    }

    engine->RegisterUnserializedObject(key, agsImGuiStyle, &AgsImGuiStyle_Interface);
}

//..............................................................................

