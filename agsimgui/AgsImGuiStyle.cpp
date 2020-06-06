/*
 * Copyright (C) 2020  Érico Vieira Porto
 *
 * This program is free software. You can use and redistribute it
 *  under the terms and conditions of the MIT License (see LICENCE).
 */

#include <cmath>
#include "AgsImGuiStyle.h"


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

