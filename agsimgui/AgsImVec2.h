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

class AgsImVec2
{
    float _x = 0.0;
    float _y = 0.0;
public:
    AgsImVec2(float x, float y);
    ~AgsImVec2(void);
    void SetX(float x);
    float GetX();
    void SetY(float y);
    float GetY();

    AgsImVec2* Scale(float scale);
    float Length();
    float SquaredLength();
    AgsImVec2* Add(AgsImVec2* AgsImVec2);
    AgsImVec2* Sub(AgsImVec2* AgsImVec2);
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
