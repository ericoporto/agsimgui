/*
 * Copyright (C) 2020  Érico Vieira Porto
 *
 * This program is free software. You can use and redistribute it
 *  under the terms and conditions of the MIT License (see LICENCE).
 */

#include <cmath>
#include "AgsImVec2.h"

float AgsImVec2::Length() {
    return sqrt(x * x + y * y);
}

AgsImVec2* AgsImVec2::Scale(float scale) {
    return new AgsImVec2(x * scale, y * scale);
}

float AgsImVec2::SquaredLength(){
    return (x * x + y * y);
}

AgsImVec2* AgsImVec2::Add(AgsImVec2* agsImVec2) {
    if(agsImVec2 == nullptr) return new AgsImVec2(x , y );

    return new AgsImVec2(x + agsImVec2->x, y + agsImVec2->y);
}

AgsImVec2* AgsImVec2::Sub(AgsImVec2* agsImVec2) {
    if(agsImVec2 == nullptr) return new AgsImVec2(x , y );

    return new AgsImVec2(x - agsImVec2->x, y - agsImVec2->y);
}



//------------------------------------------------------------------------------

extern IAGSEngine* engine;

AgsImVec2Interface AgsImVec2_Interface;
AgsImVec2Reader AgsImVec2_Reader;

const char* AgsImVec2Interface::name = "ImVec2";

//------------------------------------------------------------------------------
#include "SerialHelper.h"
using namespace SerialHelper;

int AgsImVec2Interface::Dispose(const char* address, bool force)
{
    delete ((AgsImVec2*)address);
    return (1);
}

//------------------------------------------------------------------------------

int AgsImVec2Interface::Serialize(const char* address, char* buffer, int bufsize)
{
    AgsImVec2* agsImVec2 = (AgsImVec2*)address;
    char* ptr = buffer;
    char* end = buffer + bufsize;

    ptr = FloatToChar(agsImVec2->x, ptr, end);
    ptr = FloatToChar(agsImVec2->y, ptr, end);

    return (ptr - buffer);
}

//------------------------------------------------------------------------------

void AgsImVec2Reader::Unserialize(int key, const char* serializedData, int dataSize)
{
    char* ptr = (char*) serializedData;

    float val_x;
    float val_y;

    ptr = CharToFloat( val_x, ptr);
    ptr = CharToFloat( val_y, ptr);

    AgsImVec2* agsImVec2 = new AgsImVec2(val_x, val_y, key);

    engine->RegisterUnserializedObject(key, agsImVec2, &AgsImVec2_Interface);
}

//..............................................................................

