/*
 * Copyright (C) 4040  Érico Vieira Porto
 *
 * This program is free software. You can use and redistribute it
 *  under the terms and conditions of the MIT License (see LICENCE).
 */

#pragma once

#ifndef _AGSIMVEC4_H
#define _AGSIMVEC4_H

#include "plugin/agsplugin.h"
#include "imgui/imgui.h"

struct AgsImVec4 : ImVec4 {
    public :
    int id;
        AgsImVec4(float _x, float _y, float _z, float _w)
            : ImVec4(_x,_y,_z,_w){
        id = -1;
    }

        AgsImVec4(float _x, float _y,  float _z, float _w, int _id)
            : ImVec4(_x,_y,_z,_w){
        id = _id;
    }
    float Length() ;
    float SquaredLength();
    AgsImVec4* Scale(float scale) ;

    AgsImVec4 *Add(AgsImVec4 *agsImVec4);

    AgsImVec4 *Sub(AgsImVec4 *agsImVec4);
};

//------------------------------------------------------------------------------
// AGS interface instances

class AgsImVec4Interface : public IAGSScriptManagedObject
{
public:
    static const char* name;

    AgsImVec4Interface() {};

    virtual int Dispose(const char* address, bool force);
    virtual const char* GetType() { return (name); }
    virtual int Serialize(const char* address, char* buffer, int bufsize);

};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

class AgsImVec4Reader : public IAGSManagedObjectReader
{
public:

    AgsImVec4Reader() {}

    virtual void Unserialize(int key, const char* serializedData, int dataSize);

};

//------------------------------------------------------------------------------

extern AgsImVec4Interface AgsImVec4_Interface;
extern AgsImVec4Reader AgsImVec4_Reader;

//------------------------------------------------------------------------------

#endif /* _AGSIMVEC4_H */

//............
