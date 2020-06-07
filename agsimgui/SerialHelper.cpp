/*
 * Copyright (C) 2020  Érico Vieira Porto
 *
 * This program is free software. You can use and redistribute it
 *  under the terms and conditions of the MIT License (see LICENCE).
 */

#include "SerialHelper.h"
#include <cassert>
#include "imgui/imgui.h"

namespace SerialHelper {

	char* IntToChar(int i, char * buf, char* end) {
		assert(buf + sizeof(int) < end);

		*((int*)buf) = i;

		return buf + sizeof(int);
	}

    char* CharToInt(int &i, char * buf) {
        i = *((int*)buf);
        return buf + sizeof(int);
    }

	char* FloatToChar(float f, char * buf, char* end) {
		assert(buf + sizeof(float) < end);

		*((float*)buf) = f;

		return buf + sizeof(float);
	}

    char* ImVec2ToChar(ImVec2 imVec2, char * buf, char* end) {
        assert(buf + 2*sizeof(float) < end);

        buf = FloatToChar(imVec2.x, buf, end);
        buf = FloatToChar(imVec2.y, buf, end);

        return buf;
    }

    char* ImVec4ToChar(ImVec4 imVec4, char * buf, char* end) {
        assert(buf + 4*sizeof(float) < end);

        buf = FloatToChar(imVec4.x, buf, end);
        buf = FloatToChar(imVec4.y, buf, end);
        buf = FloatToChar(imVec4.z, buf, end);
        buf = FloatToChar(imVec4.w, buf, end);

        return buf;
    }

    char* CharToFloat(float &f, char * buf) {
        f = *((float*)buf);
        return buf + sizeof(float);
    }

    char* CharToImVec2(ImVec2 &imVec2, char * buf) {
        float _x, _y;
	    buf = CharToFloat(_x, buf);
        buf = CharToFloat(_y, buf);
        imVec2.x = _x;
        imVec2.y = _y;
        return buf;
    }

    char* CharToImVec4(ImVec4 &imVec4, char * buf) {
        float _x, _y, _z, _w;
        buf = CharToFloat(_x, buf);
        buf = CharToFloat(_y, buf);
        buf = CharToFloat(_z, buf);
        buf = CharToFloat(_w, buf);
        imVec4.x = _x;
        imVec4.y = _y;
        imVec4.z = _z;
        imVec4.w = _w;
        return buf;
    }

	char* BoolToChar(bool b, char* buf, char* end) {
		assert(buf + sizeof(char) < end);

		if (b) {
			*buf = 1;
		}
		else {
			*buf = 0;
		}
		return buf + sizeof(char);
	}

	char* CharToBool(bool &b, char* buf) {
		if (*buf == 0) {
			b = false;
		}
		else {
			b = true;
		}
		return buf + sizeof(char);
	}




}
