/*
 * Copyright (C) 2020  Érico Vieira Porto
 *
 * This program is free software. You can use and redistribute it
 *  under the terms and conditions of the MIT License (see LICENCE).
 */

#include <cmath>
#include "AgsImVec2.h"

AgsImVec2::AgsImVec2(float x, float y) {
    _x = x;
    _y = y;
}

AgsImVec2::~AgsImVec2(void)
{
}

void AgsImVec2::SetX(float x){
    _x = x;
}

float AgsImVec2::GetX(){
    return _x;
}

void AgsImVec2::SetY(float y){
    _y = y;
}

float AgsImVec2::GetY(){
    return _y;
}

AgsImVec2* AgsImVec2::Scale(float scale){
    return new AgsImVec2(_x * scale, _y * scale);
}

float AgsImVec2::Length(){
    return sqrtf(_x * _x + _y * _y);
}

float AgsImVec2::SquaredLength(){
    return (_x * _x + _y * _y);
}

AgsImVec2* AgsImVec2::Add(AgsImVec2* agsImVec2) {
    if(agsImVec2 == nullptr) return new AgsImVec2(_x , _y );

    return new AgsImVec2(_x + agsImVec2->GetX(), _y + agsImVec2->GetY());
}

AgsImVec2* AgsImVec2::Sub(AgsImVec2* agsImVec2) {
    if(agsImVec2 == nullptr) return new AgsImVec2(_x , _y );

    return new AgsImVec2(_x - agsImVec2->GetX(), _y - agsImVec2->GetY());
}

//------------------------------------------------------------------------------


extern IAGSEngine* engine;

AgsImVec2Interface AgsImVec2_Interface;
AgsImVec2Reader AgsImVec2_Reader;

const char* AgsImVec2Interface::name = "AgsImVec2";

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
    AgsImVec2* agsimgui_imvec2 = (AgsImVec2*)address;
    char* ptr = buffer;
    char* end = buffer + bufsize;

    ptr = FloatToChar(agsimgui_imvec2->GetX(), ptr, end);
    ptr = FloatToChar(agsimgui_imvec2->GetY(), ptr, end);

    return (ptr - buffer);
}

//------------------------------------------------------------------------------

void AgsImVec2Reader::Unserialize(int key, const char* serializedData, int dataSize)
{
    char* ptr = (char*) serializedData;
    int agsimgui_imvec2_id = key;


    float val_x;
    float val_y;

    ptr = CharToFloat( val_x, ptr);
    ptr = CharToFloat( val_y, ptr);

    AgsImVec2* agsimgui_imvec2 = new AgsImVec2(val_x, val_y);

    engine->RegisterUnserializedObject(key, agsimgui_imvec2, &AgsImVec2_Interface);
}

//..............................................................................
