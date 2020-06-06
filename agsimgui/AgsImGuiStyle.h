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
