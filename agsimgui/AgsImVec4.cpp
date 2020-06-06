/*
 * Copyright (C) 4040  Érico Vieira Porto
 *
 * This program is free software. You can use and redistribute it
 *  under the terms and conditions of the MIT License (see LICENCE).
 */

#include <cmath>
#include "AgsImVec4.h"

float AgsImVec4::Length() {
    return sqrt(x * x + y * y + z * z + w * w);
}

AgsImVec4* AgsImVec4::Scale(float scale) {
    return new AgsImVec4(x * scale, y * scale, z*scale, w*scale);
}

float AgsImVec4::SquaredLength(){
    return (x * x + y * y + z * z + w * w);
}

AgsImVec4* AgsImVec4::Add(AgsImVec4* agsImVec4) {
    if(agsImVec4 == nullptr) return new AgsImVec4(x , y , z, w);

    return new AgsImVec4(x + agsImVec4->x, y + agsImVec4->y, z + agsImVec4->z, w + agsImVec4->w);
}

AgsImVec4* AgsImVec4::Sub(AgsImVec4* agsImVec4) {
    if(agsImVec4 == nullptr) return new AgsImVec4(x , y, z, w);

    return new AgsImVec4(x - agsImVec4->x, y - agsImVec4->y, z - agsImVec4->z, w - agsImVec4->w);
}



//------------------------------------------------------------------------------

extern IAGSEngine* engine;

AgsImVec4Interface AgsImVec4_Interface;
AgsImVec4Reader AgsImVec4_Reader;

const char* AgsImVec4Interface::name = "ImVec4";

//------------------------------------------------------------------------------
#include "SerialHelper.h"
using namespace SerialHelper;

int AgsImVec4Interface::Dispose(const char* address, bool force)
{
    delete ((AgsImVec4*)address);
    return (1);
}

//------------------------------------------------------------------------------

int AgsImVec4Interface::Serialize(const char* address, char* buffer, int bufsize)
{
    AgsImVec4* agsImVec4 = (AgsImVec4*)address;
    char* ptr = buffer;
    char* end = buffer + bufsize;

    ptr = FloatToChar(agsImVec4->x, ptr, end);
    ptr = FloatToChar(agsImVec4->y, ptr, end);
    ptr = FloatToChar(agsImVec4->z, ptr, end);
    ptr = FloatToChar(agsImVec4->w, ptr, end);

    return (ptr - buffer);
}

//------------------------------------------------------------------------------

void AgsImVec4Reader::Unserialize(int key, const char* serializedData, int dataSize)
{
    char* ptr = (char*) serializedData;

    float val_x;
    float val_y;
    float val_z;
    float val_w;

    ptr = CharToFloat( val_x, ptr);
    ptr = CharToFloat( val_y, ptr);
    ptr = CharToFloat( val_z, ptr);
    ptr = CharToFloat( val_w, ptr);

    AgsImVec4* agsImVec4 = new AgsImVec4(val_x, val_y, val_z, val_w, key);

    engine->RegisterUnserializedObject(key, agsImVec4, &AgsImVec4_Interface);
}

//..............................................................................

