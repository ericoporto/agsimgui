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


    return (ptr - buffer);
}

//------------------------------------------------------------------------------

void AgsImGuiStyleReader::Unserialize(int key, const char* serializedData, int dataSize)
{
    char* ptr = (char*) serializedData;

    float val_x;
    float val_y;


    AgsImGuiStyle* agsImGuiStyle = new AgsImGuiStyle(key);

    engine->RegisterUnserializedObject(key, agsImGuiStyle, &AgsImGuiStyle_Interface);
}

//..............................................................................

