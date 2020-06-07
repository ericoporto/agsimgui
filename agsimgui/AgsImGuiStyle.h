/*
 * Copyright (C) 2020  Érico Vieira Porto
 *
 * This program is free software. You can use and redistribute it
 *  under the terms and conditions of the MIT License (see LICENCE).
 */

#pragma once

#ifndef _AGSIMGUISTYLE_H
#define _AGSIMGUISTYLE_H

#include "plugin/agsplugin.h"
#include "imgui/imgui.h"

struct AgsImGuiStyle: ImGuiStyle {
    public :
    int id;
    AgsImGuiStyle()
            : ImGuiStyle(){
        id = -1;
    }

    AgsImGuiStyle(int _id)
            : ImGuiStyle(){
        id = _id;
    }

    AgsImGuiStyle(ImGuiStyle imGuiStyle)
            : ImGuiStyle(){
        id = -1;
        Alpha = imGuiStyle.Alpha;
        WindowPadding = imGuiStyle.WindowPadding;
        WindowRounding = imGuiStyle.WindowRounding;
        WindowBorderSize = imGuiStyle.WindowBorderSize;
        WindowMinSize = imGuiStyle.WindowMinSize;
        WindowTitleAlign = imGuiStyle.WindowTitleAlign;
        WindowMenuButtonPosition = imGuiStyle.WindowMenuButtonPosition;
        ChildRounding = imGuiStyle.ChildRounding;
        ChildBorderSize = imGuiStyle.ChildBorderSize;
        PopupRounding = imGuiStyle.PopupRounding;
        PopupBorderSize = imGuiStyle.PopupBorderSize;
        FramePadding = imGuiStyle.FramePadding;
        FrameRounding = imGuiStyle.FrameRounding;
        FrameBorderSize = imGuiStyle.FrameBorderSize;
        ItemSpacing = imGuiStyle.ItemSpacing;
        ItemInnerSpacing = imGuiStyle.ItemInnerSpacing;
        TouchExtraPadding = imGuiStyle.TouchExtraPadding;
        IndentSpacing = imGuiStyle.IndentSpacing;
        ColumnsMinSpacing = imGuiStyle.ColumnsMinSpacing;
        ScrollbarSize = imGuiStyle.ScrollbarSize;
        ScrollbarRounding = imGuiStyle.ScrollbarRounding;
        GrabMinSize = imGuiStyle.GrabMinSize;
        GrabRounding = imGuiStyle.GrabRounding;
        TabRounding = imGuiStyle.TabRounding;
        TabBorderSize = imGuiStyle.TabBorderSize;
        TabMinWidthForUnselectedCloseButton = imGuiStyle.TabMinWidthForUnselectedCloseButton;
        ColorButtonPosition = imGuiStyle.ColorButtonPosition;
        ButtonTextAlign = imGuiStyle.ButtonTextAlign;
        SelectableTextAlign = imGuiStyle.SelectableTextAlign;
        DisplayWindowPadding = imGuiStyle.DisplayWindowPadding;
        DisplaySafeAreaPadding = imGuiStyle.DisplaySafeAreaPadding;
        MouseCursorScale = imGuiStyle.MouseCursorScale;
        AntiAliasedLines = imGuiStyle.AntiAliasedLines;
        AntiAliasedFill = imGuiStyle.AntiAliasedFill;
        CurveTessellationTol = imGuiStyle.CurveTessellationTol;
        CircleSegmentMaxError = imGuiStyle.CircleSegmentMaxError;
        for(int i=0; i<ImGuiCol_COUNT; i++){
            Colors[i] = imGuiStyle.Colors[i];
        }
    }

    static void SetAgsImGuiStyle(ImGuiStyle &imGuiStyle, AgsImGuiStyle* agsImGuiStyle);

};

//------------------------------------------------------------------------------
// AGS interface instances

class AgsImGuiStyleInterface : public IAGSScriptManagedObject
{
public:
    static const char* name;

    AgsImGuiStyleInterface() {};

    virtual int Dispose(const char* address, bool force);
    virtual const char* GetType() { return (name); }
    virtual int Serialize(const char* address, char* buffer, int bufsize);

};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

class AgsImGuiStyleReader : public IAGSManagedObjectReader
{
public:

    AgsImGuiStyleReader() {}

    virtual void Unserialize(int key, const char* serializedData, int dataSize);

};

//------------------------------------------------------------------------------

extern AgsImGuiStyleInterface AgsImGuiStyle_Interface;
extern AgsImGuiStyleReader AgsImGuiStyle_Reader;

//------------------------------------------------------------------------------

#endif /* _AGSIMGUISTYLE_H */

//............
