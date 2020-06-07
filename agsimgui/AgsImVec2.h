/*
 * Copyright (C) 2020  Érico Vieira Porto
 *
 * This program is free software. You can use and redistribute it
 *  under the terms and conditions of the MIT License (see LICENCE).
 */

#pragma once

#ifndef _AGSIMVEC2_H
#define _AGSIMVEC2_H

#include "plugin/agsplugin.h"
#include "imgui/imgui.h"

struct AgsImVec2 : ImVec2 {
    public :
    int id;
        AgsImVec2(float _x, float _y)
            : ImVec2(_x,_y){
        id = -1;
    }

        AgsImVec2(float _x, float _y, int _id)
            : ImVec2(_x,_y){
        id = _id;
    }
    float Length() ;
    float SquaredLength();
    AgsImVec2* Scale(float scale) ;

    AgsImVec2 *Add(AgsImVec2 *agsImVec2);

    AgsImVec2 *Sub(AgsImVec2 *agsImVec2);
};

//------------------------------------------------------------------------------
// AGS interface instances

class AgsImVec2Interface : public IAGSScriptManagedObject
{
public:
    static const char* name;

    AgsImVec2Interface() {};

    virtual int Dispose(const char* address, bool force);
    virtual const char* GetType() { return (name); }
    virtual int Serialize(const char* address, char* buffer, int bufsize);

};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

class AgsImVec2Reader : public IAGSManagedObjectReader
{
public:

    AgsImVec2Reader() {}

    virtual void Unserialize(int key, const char* serializedData, int dataSize);

};

//------------------------------------------------------------------------------

extern AgsImVec2Interface AgsImVec2_Interface;
extern AgsImVec2Reader AgsImVec2_Reader;

//------------------------------------------------------------------------------

#endif /* _AGSIMVEC2_H */

//............
