/*
 * Copyright (C) 2020  Érico Vieira Porto
 *
 * This program is free software. You can use and redistribute it
 *  under the terms and conditions of the MIT License (see LICENCE).
 */

#pragma once

#ifndef AGSBOX2D_SERIALHELPER_H
#define AGSBOX2D_SERIALHELPER_H

#include "imgui/imgui.h"

namespace SerialHelper {
	
	union u_cf
	{
		float f;
		char s[sizeof(float)];
	};

	union u_ci
	{
		int i;
		char s[sizeof(int)];
	};

	char* IntToChar(int i, char * buf, char* end);
	char* FloatToChar(float f, char * buf, char* end);
	char* BoolToChar(bool b, char* buf, char* end);

    char* CharToInt(int &i, char * buf);
    char* CharToFloat(float &f, char * buf);
	char* CharToBool(bool &b, char* buf);

    char* ImVec2ToChar(ImVec2 imVec2, char * buf, char* end);
    char* ImVec4ToChar(ImVec4 imVec4, char * buf, char* end);
    char* CharToImVec2(ImVec2 &imVec2, char * buf);
    char* CharToImVec4(ImVec4 &imVec4, char * buf);
}

#endif /* AGSBOX2D_SERIALHELPER_H */